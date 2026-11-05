"""Current cohort byte/source/provider binding; no executable is launched."""
from pathlib import Path
import hashlib,json,re,sys
sys.dont_write_bytecode=True
ROOT=Path(__file__).resolve().parents[2]
PIN='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
LEDGER=ROOT/'metadata/city-active-buildings-reconstruction.json'

def digest(path):return hashlib.sha256(Path(path).read_bytes()).hexdigest()
def save(path,value):
 path=Path(path);path.parent.mkdir(parents=True,exist_ok=True)
 path.write_bytes((json.dumps(value,indent=2)+'\n').encode())

def symbol(text,name):
 matches=re.findall(r'^\s+\w+:\w+\s+_'+re.escape(name)+r'\s+([0-9a-fA-F]+)\s+(.+)$',text,re.M)
 if not matches:return None
 assert len({x[0]for x in matches})==1,name
 return dict(va=int(matches[0][0],16),owner=matches[0][1].strip())

def load(require_retained=True):
 sys.path[:0]=[str(ROOT/'build/offline-verification-deps'),str(ROOT/'direct_c')]
 from native_test_pe import PE
 from capstone import Cs,CS_ARCH_X86,CS_MODE_32
 d=json.loads(LEDGER.read_bytes());native_path=ROOT/'build/smoke-game/Pharaoh.exe'
 assert digest(native_path)==PIN
 native=PE(native_path);ep=ROOT/d['evidence']['path'];assert digest(ep)==d['evidence']['sha256']
 evidence={r['evidence_id']:r for r in json.loads(ep.read_bytes())['records']}
 for name,sha in d['source_hashes'].items():assert digest(ROOT/name)==sha,('source changed',name)
 images={k:dict(path=v['path'],map_path=v['map_path'])for k,v in d['strict_main_build']['compiled_artifacts'].items()}
 retained=dict(path='build/c-only/Release/restore_cohort3_retained.exe',map_path='build/c-only/Release/restore_cohort3_retained.map')
 if (ROOT/retained['path']).exists():images['retained']=retained
 elif require_retained:raise ValueError('Build restore_cohort3_retained first')
 maps={}
 for label,item in images.items():
  item.update(sha256=digest(ROOT/item['path']),map_sha256=digest(ROOT/item['map_path']))
  if label!='retained':
   assert item['sha256']==d['strict_main_build']['compiled_artifacts'][label]['sha256']
   assert item['map_sha256']==d['strict_main_build']['compiled_artifacts'][label]['map_sha256']
  maps[label]=(ROOT/item['map_path']).read_text()
 rows=[]
 for m in d['members']:
  assert digest(ROOT/m['source'])==m['source_sha256'],m['source']
  a=int(m['address'],16);ranges=[(int(r['start'],16),int(r['end'],16)+1)for r in m.get('native_ranges',[])]or[(a,a+m['native_bytes'])]
  rec=evidence[m['native_evidence_id']];base=int(rec['parameters']['address'],16);payload=bytes.fromhex(rec['normalized_result']['bytes_hex'])
  assert rec['subject']['digest']['sha256']==PIN and rec['normalized_result']['complete']
  chunks=[];decoded=[]
  for start,end in ranges:
   assert base<=start<end<=base+len(payload)
   raw=payload[start-base:end-base];assert raw==native.data[start-native.base:end-native.base]
   chunks.append(raw);ins=list(Cs(CS_ARCH_X86,CS_MODE_32).disasm(raw,start));assert sum(i.size for i in ins)==end-start;decoded+=ins
  assert sum(map(len,chunks))==m['native_bytes']and len(decoded)==m['native_instructions']
  assert hashlib.sha256(b''.join(chunks)).hexdigest()==m['native_byte_sha256']
  providers={k:dict(named=symbol(text,m['name']),canonical=symbol(text,m.get('canonical_symbol','FUN_%08x'%a)))for k,text in maps.items()}
  if require_retained:assert providers['retained']['named'],('missing selected C definition',m['name'])
  rows.append(dict(address=m['address'],name=m['name'],source=m['source'],source_definition_symbol=m.get('source_definition_symbol',m['name']),algorithm_source=m.get('algorithm_source',m['source']),source_sha256=m['source_sha256'],body_sha256=m['body_sha256'],native_ranges=[dict(start=hex(x),end=hex(y-1))for x,y in ranges],native_ret_immediates=sorted({int(i.op_str or'0',0)for i in decoded if i.mnemonic=='ret'}),logical_signature=m['logical_signature'],providers=providers,rule=m.get('rule'),case_status='requires_authored_case_and_ABI_contract'))
 return dict(schema='cohort3-replay-registry-v1',source_ledger_sha256=digest(LEDGER),source_entries=len(rows),native=dict(path=native_path.relative_to(ROOT).as_posix(),sha256=PIN),compiled_images=images,members=rows,host_execution=False,execution_performed=False,joint_native_C_comparison_complete=False,whole_game_equivalence=False)

if __name__=='__main__':
 import argparse
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,default=ROOT/'build/actor-animal-following/cohort3-verification/registry.json');p.add_argument('--allow-missing-retained',action='store_true');a=p.parse_args()
 result=load(not a.allow_missing_retained);save(a.output,result)
 print(json.dumps(dict(source_entries=result['source_entries'],retained_image='retained'in result['compiled_images'],execution_performed=False,output=str(a.output))))
