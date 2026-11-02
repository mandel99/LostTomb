"""Staging-only typed candidate pilot. Never writes production C or analysis databases."""
import argparse,hashlib,json,re,struct,sys
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2];PRIVATE=ROOT/'build/bulk-type-pilot';PIN='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
sha=lambda p:hashlib.sha256(Path(p).read_bytes()).hexdigest()
def confined(p):
 p=p.resolve()
 if not p.is_relative_to(PRIVATE.resolve()) or p==PRIVATE.resolve():raise ValueError('Output must be strictly within build/bulk-type-pilot')
 return p
def protection():
 files=[ROOT/'units.json',ROOT/'CMakeLists.txt',ROOT/'generation.json',ROOT/'src/units.cmake']
 for q,pat in [('src','*.c'),('runtime','*.c'),('include','*.h'),('tools','*')]:files.extend(p for p in (ROOT/q).glob(pat) if p.is_file())
 return {p.relative_to(ROOT).as_posix():sha(p) for p in sorted(set(files))}
def emit(facts_path,output):
 output=confined(output)
 if output.exists() and any(output.iterdir()):raise ValueError('Candidate output must be fresh; refusing overwrite')
 baseline=protection();facts=json.loads(facts_path.read_text());assert facts['native_sha256']==PIN and facts['private_noanalysis_import']
 image=(ROOT/'build/smoke-game/Pharaoh.exe').read_bytes();assert hashlib.sha256(image).hexdigest()==PIN;pe=struct.unpack_from('<I',image,60)[0];sec=pe+24+struct.unpack_from('<H',image,pe+20)[0];ss=[struct.unpack_from('<IIII',image,sec+i*40+8) for i in range(struct.unpack_from('<H',image,pe+6)[0])]
 def raw(a,n):
  a-=0x400000
  for vs,rv,rs,ro in ss:
   if rv<=a<rv+max(vs,rs):return image[ro+a-rv:ro+a-rv+n]
  raise ValueError(hex(a))
 mapping={int(q['provenance']['address'],16):q['file'] for q in json.loads((ROOT/'units.json').read_text()) if q.get('provenance')};report={'native_sha256':PIN,'facts_path':facts_path.relative_to(ROOT).as_posix(),'facts_sha256':sha(facts_path),'candidate_status':'Staged experimental export; not applied, not automatically accepted','staged':[],'quarantined':[],'native_instructions_authenticated':0}
 for entry,f in facts['functions'].items():
  for q in f['native_instructions']:
   b=bytes.fromhex(q['bytes']);assert raw(int(q['address'].split(':')[1],16),len(b))==b;report['native_instructions_authenticated']+=1
  if entry not in ['0040fa40','0040fa70','0040fae0']:
   reason='Pointer projections require all pointee/access/stride and callback contracts; facts retained without C emission' if entry=='00425f20' else 'Receiver/register parameter and nested member calls require explicit ABI helper; facts retained without C emission'
   report['quarantined'].append({'entry':entry,'reason':reason,'high_prototype':f['high_prototype'],'global_pointer_tokens':[t for t in f['clang_tokens'] if t['text'].lstrip('_')=='DAT_00705724']});continue
  rows={};bindings=[]
  for t in f['clang_tokens']:
   if not re.fullmatch(r'_?DAT_[0-9a-f]{8}',t['text']):continue
   h=t['high_variable'];v=t['varnode'];assert h and h['class']=='HighGlobal' and h['size_bytes']==4 and h['datatype']['size_bytes']==4 and v and v['size_bytes']==4
   addr=h['representative'];assert addr in ['ram:e92e18','ram:e38d94'];assert h['datatype']['display']=='int';name=t['text'];rows[name]=(int(addr.split(':')[1],16),h,t)
  assert {a for a,h,t in rows.values()}=={0xe92e18,0xe38d94}
  for address,h,t in rows.values():
   native_sizes={v['size_bytes'] for ins in f['native_instructions'] for op in ins['raw_pcode'] for v in op['inputs'] if v and v['memory'] and v['address']=='ram:'+format(address,'x')}
   assert native_sizes=={4},'Global read size conflict or missing native evidence'
  assert all(q['instruction']=='RET' for q in f['native_instructions'] if q['instruction'].startswith('RET'))
  code=f['raw_pseudocode'].replace('\r\r\n','\n').replace('\r\n','\n');prefix=['#include "restore_types.h"']
  for name,(addr,h,t) in sorted(rows.items()):
   prefix.append(f'#define {name} (*(int32_t *)0x{addr:08x}u)');bindings.append({'name':name,'address':f'{addr:08x}','C_type':'int32_t','width':4,'HighGlobal_type':h['datatype'],'varnode':t['varnode'],'listing_type_advisory':[g['listing_datatype_advisory'] for g in f['global_symbols'] if g['name'].lstrip('_')==name.lstrip('_')]})
  # Raw body/aliases remain byte-identical after newline normalization; no expression/index/signature rewrite.
  dest=output/(entry+'.c');output.mkdir(parents=True,exist_ok=True);dest.write_text('\n'.join(prefix)+'\n'+code,encoding='utf-8');report['staged'].append({'entry':entry,'candidate':dest.relative_to(ROOT).as_posix(),'sha256':sha(dest),'proposed_product_path':mapping[int(entry,16)],'raw_body_sha256':hashlib.sha256(code.encode()).hexdigest(),'bindings':bindings,'high_prototype':f['high_prototype'],'native_stack_contract':'RET without immediate; native reads3argument slots and lowWORDthird value. GeneratedC retains raw default-C signature, not inferred __stdcall label. Caller/ABI facts are not silently erased.','arithmetic_domain':'Raw signed C arithmetic is retained. This candidate is only verified for bounded positive framebuffer coordinates/pitch where signed addition/multiplication cannot overflow; arbitrary32bit wrap is not yet accepted.'})
 final=protection();drift=[p for p,h in baseline.items() if final.get(p)!=h];assert not drift,'Protected production changed while emitting: '+str(drift)
 report['protected_source_hashes_before']=baseline;report['protected_source_hashes_after']=final;report['protected_source_drift']=drift;report['units_mapping_sha256']=sha(ROOT/'units.json');report['product_files_written']=[];report['no_byte_fallback']=True;report['no_index_or_receiver_rewrite']=True;(output/'manifest.json').write_text(json.dumps(report,indent=2)+'\n');return report
if __name__=='__main__':
 ap=argparse.ArgumentParser();ap.add_argument('--facts',type=Path,default=PRIVATE/'typed-facts.json');ap.add_argument('--output-root',type=Path,required=True);ap.add_argument('--guard-test',action='store_true');a=ap.parse_args()
 if a.guard_test:
  rejected=[]
  for p in [ROOT,ROOT/'src',ROOT/'build/other',PRIVATE/'../outside']:
   try:confined(p)
   except ValueError:rejected.append(str(p));continue
   raise AssertionError('Unsafe output accepted')
  PRIVATE.mkdir(exist_ok=True);(PRIVATE/'cli-guard-evidence.json').write_text(json.dumps({'rejected':rejected,'no_writes_to_rejected_paths':True},indent=2)+'\n');print('CLI guard rejected4unsafe outputs');sys.exit(0)
 r=emit(a.facts.resolve(),a.output_root);print(json.dumps({'staged':len(r['staged']),'quarantined':len(r['quarantined']),'native_instructions_authenticated':r['native_instructions_authenticated'],'protected_files':len(r['protected_source_hashes_before']),'drift':r['protected_source_drift']}))
