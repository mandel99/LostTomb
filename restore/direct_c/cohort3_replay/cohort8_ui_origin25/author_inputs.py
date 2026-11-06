"""Pure cached-input extraction for the UI25 data packet; no execution."""
from pathlib import Path
import json,sys,hashlib
sys.dont_write_bytecode=True
OUT=Path(__file__).resolve().parent;ROOT=OUT.parents[2]
sys.path[:0]=[str(ROOT/'direct_c/cohort3_replay'),str(ROOT/'direct_c'),str(ROOT/'build/offline-verification-deps')]
from registry import symbol
from native_test_pe import PE
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def save(p,v):Path(p).write_bytes((json.dumps(v,indent=2)+'\n').encode())
def main():
 metadata=json.loads((ROOT/'metadata/cohort8-source100-local-value-repairs.json').read_bytes())
 members=sorted([m for m in metadata['members']if any(v['name'].lower()in('dat_00e38da4','dat_00e38da8')for v in m['local_view_changes'])and int(m['address'],16)not in(0x52da40,0x409eb0,0x44c1a0,0x4c42b0,0x4e0f90)],key=lambda m:int(m['address'],16))[:25]
 profile=json.loads((ROOT/'build/actor-animal-following/cohort8-current-images.json').read_bytes())
 maps={k:(ROOT/v['map_path']).read_text()for k,v in profile['compiled_images'].items()}
 native=PE(ROOT/'build/smoke-game/Pharaoh.exe');md=Cs(CS_ARCH_X86,CS_MODE_32)
 evidence=[];contracts=[];sources={};text=[]
 for m in members:
  a=int(m['address'],16);records=[]
  for path in m['native_cache_inputs']:
   cache=json.loads((ROOT/path).read_bytes());assert cache['native_sha256']==digest(ROOT/'build/smoke-game/Pharaoh.exe')
   records +=[r for r in cache['entries']if int(r['requested_address'],16)==a]
  assert len(records)==1;rec=records[0]
  assert not rec['assembly']['has_more']
  for i in rec['assembly']['items']:
   ia=int(i['address'],16);raw=bytes.fromhex(i['bytes']);assert native.data[ia-native.base:ia-native.base+len(raw)]==raw
  evidence.append(rec);end=int(rec['info']['body_end'],16)
  raw=native.data[a-native.base:end-native.base+1]
  text+=['\n### '+m['address']+' '+m['source']]+[i['address']+' '+i['bytes']+' '+i['instruction']for i in rec['assembly']['items']]
  provider={}
  for label,t in maps.items():
   hit=symbol(t,m['name']);assert hit and Path(m['source']).stem+'.obj'in hit['owner'];provider[label]=hit
  sources[m['source']]=digest(ROOT/m['source'])
  contracts.append(dict(address=m['address'],symbol=m['name'],source=m['source'],source_sha256=sources[m['source']],owner_object=Path(m['source']).stem+'.obj',providers=provider,native_end_inclusive=end,native_contiguous_scope_sha256=hashlib.sha256(raw).hexdigest(),logical_signature='void(void)'if a!=0x5309b0 else'void(int x,int y)',logical_output='Endpoint arguments, order, snapshots, and declared RAM writes. No top-level EAX oracle.',native_cleanup=0,canonical_only=True,native_cache_inputs=m['native_cache_inputs']))
 save(OUT/'native-evidence.json',dict(schema='ui25-cached-native-evidence-v1',members=evidence))
 (OUT/'native.asm.txt').write_text('\n'.join(text)+'\n',encoding='utf-8')
 save(OUT/'author-inputs.json',dict(members=contracts,source_guards=sources,compiled_images=profile['compiled_images']))
 # This is a disassembly read, not invocation of the restored executable.
 compiled={}
 for label,image in profile['compiled_images'].items():
  pe=PE(ROOT/image['path']);hits={n:symbol(maps[label],n)['va']for n in('FUN_00533470','FUN_004f4ff0','FUN_004cce90')}
  code=list(md.disasm(pe.data[hits['FUN_00533470']-pe.base:hits['FUN_00533470']-pe.base+0x1900],hits['FUN_00533470']))
  windows=[]
  for j,i in enumerate(code):
   if i.mnemonic=='call'and i.op_str==hex(hits['FUN_004f4ff0']):windows.append([dict(address=hex(z.address),bytes=z.bytes.hex(),instruction=z.mnemonic+' '+z.op_str)for z in code[max(0,j-7):j+10]])
  assert len(windows)==1
  assert windows[0][8]['instruction']=='push eax'and windows[0][9]['instruction']=='call '+hex(hits['FUN_004cce90'])
  compiled[label]=dict(symbols=hits,windows=windows)
 save(OUT/'compiled-533470-callsite.json',dict(images=compiled))
 print(json.dumps(dict(entries=len(contracts),native_instructions=sum(len(x['assembly']['items'])for x in evidence),compiled_533470_pending_argument_continuation=list(compiled))))
if __name__=='__main__':main()
