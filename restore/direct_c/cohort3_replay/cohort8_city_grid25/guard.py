"""Pure guards and optional all-route audit; no emulation or compilation."""
from pathlib import Path
import argparse,collections,hashlib,json,sys
sys.dont_write_bytecode=True
OUT=Path(__file__).resolve().parent;ROOT=OUT.parents[2]
sys.path[:0]=[str(OUT),str(ROOT/'direct_c/cohort3_replay'),str(ROOT/'direct_c')]
import city_cases as recipes
import supplemental_registry as binding
def packed(v):return json.dumps(v,sort_keys=True,separators=(',',':'),default=lambda b:{'bytes_hex':b.hex()}).encode()
def checked_cases():
 p=binding.checked_inputs();rows=binding.supplemental_members();cases=recipes.cases(dict(members=rows));assert len(cases)==62 and sum(c['expected_invocations']for c in cases)==236
 assert len({c['id']for c in cases})==62 and cases==recipes.cases(dict(members=rows))and recipes.cases(dict(members=[]))==[]
 wanted={a:2 for a in recipes.TARGETS};wanted[0x488840]=13;wanted[0x4eb800]=3;assert dict(collections.Counter(c['entry']for c in cases))==wanted
 assert {c['id']:hashlib.sha256(packed(c)).hexdigest()for c in cases}==json.loads((OUT/'case-commitments.json').read_bytes())['cases']
 from registry import symbol
 maps={k:(ROOT/v['map_path']).read_text()for k,v in p['compiled_images'].items()}
 for c in cases:
  assert c['routes']==['native','canonical']and c['required_retained_route']=='canonical'
  assert c['result_mask']==recipes.MASKS.get(c['entry'])and c['native_cleanup']==(24 if c['entry']==0x488840 else 0)
  assert c['canonical_cleanup']==c['named_cleanup']==0 and c['canonical_registers_and_flags']is False
  assert c['canonical_arguments']==c['named_arguments']==c['arguments']
  assert not any(k in c for k in('borrowed_native_stack','stack_prefix','route_stores','context_outputs'))
  assert c['watched']==c['write_ranges']==[(a,len(bytes.fromhex(data)))for a,data in c['authored_final_memory']]
  assert len(c['authored_endpoint_order'])==len(c['authored_endpoint_observations'])==sum(c['expected_endpoint_counts'].values())
  m=next(m for m in p['members']if int(m['address'],16)==c['entry'])
  assert c['expected_invocations']==1+sum(bool(v)for v in m['providers'].values())
  for e in c['endpoints']:
   token=e['token'];assert e['arity']==recipes.HELPERS[token][1]and e['result_kind']==recipes.HELPERS[token][2]
   assert e['native_cleanup']==(12 if token=='sprite'else 0)and e['compiled_cleanup']==0
   assert e.get('native_receiver')==(0xf55bb0 if token=='sprite'else None)
   assert not any(k in e for k in('compiled_frame_symbols','native_register_args','compiled_register_args'))
   n=c['expected_endpoint_counts'][token];assert n==len(e['responses'])==len(c['expected_endpoint_arguments'][token])
   for label,text in maps.items():
    if m['providers'][label]:assert symbol(text,e['compiled_symbols'][0])==p['endpoint_providers'][token][label]and p['endpoint_providers'][token][label]
   assert e['native']not in c.get('actual_children',[])
  if c['entry']==0x488840:
   assert 0x56133c in c['actual_children']and c['compare_x87_values']and c['compare_x87_control']
   assert c['initial_x87']==['00000000000000d00040','00000000000000c0ffbf']
   assert all(e['native']!=0x56133c for e in c['endpoints'])
 ratios=[c for c in cases if c['entry']==0x488840 and'real-ftol-ratio-producer'in c['id']]
 assert len(ratios)==12 and{c['initial_fpcw']for c in ratios}=={0x7f|pc|rc for pc in(0,0x200,0x300)for rc in(0,0x400,0x800,0xc00)}
 from native_test_pe import PE
 pe=PE(ROOT/p['native']['path']);ev=json.loads((OUT/'native-evidence.json').read_bytes());extra=json.loads((OUT/'additional-byte-proof.json').read_bytes())
 for row in ev['members']:
  assert not row['assembly']['has_more']
  for i in row['assembly']['items']:
   a=int(i['address'],16)-pe.base;raw=bytes.fromhex(i['bytes']);assert pe.data[a:a+len(raw)]==raw
 for row in extra['native']:
  for i in row['instructions']:
   a=int(i['address'],16)-pe.base;raw=bytes.fromhex(i['bytes']);assert pe.data[a:a+len(raw)]==raw
 for row in extra['constants']:
  a=int(row['address'],16)-pe.base;raw=bytes.fromhex(row['bytes_hex']);assert pe.data[a:a+len(raw)]==raw
 compiled=PE(ROOT/p['compiled_images']['retained']['path'])
 for i in extra['current_retained_4867e0_full_EAX_forwarding']:
  a=int(i['address'],16)-compiled.base;raw=bytes.fromhex(i['bytes']);assert compiled.data[a:a+len(raw)]==raw
 for row in recipes.INPUTS['constant_tables'].values():
  a=int(row['address'],16)-pe.base;raw=bytes.fromhex(row['bytes_hex']);assert pe.data[a:a+len(raw)]==raw
 meta=json.loads((ROOT/p['source100_metadata']).read_bytes())
 for m in p['members']:
  row=next(r for r in meta['members']if int(r['address'],16)==int(m['address'],16));assert row['source']==m['source']and row['source_sha256']==m['source_sha256']
 return p,cases,rows
def audit(cases,report):
 actual={c['id']:c for c in report['cases_detail']};routes=events=0
 for c in cases:
  row=actual[c['id']];assert row['passed'],c['id'];assert len(row['routes'])==c['expected_invocations']
  for r in row['routes']:
   assert r['route']in('native','canonical')and not r['errors'],(c['id'],r['errors'])
   assert [e['token']for e in r['events']]==c['authored_endpoint_order']
   assert [e['observed']for e in r['events']]==json.loads(json.dumps(c['authored_endpoint_observations']))
   for token,expected in c['expected_endpoint_arguments'].items():assert [e['arguments']for e in r['events']if e['token']==token]==expected
   if c['result_mask']is not None:assert int(r['logical_result']['value'],16)&c['result_mask']==c['expected_masked_result']
   routes+=1;events+=len(r['events'])
 assert routes==236
 return dict(routes=routes,endpoint_events=events,independent_endpoint_args_order_and_snapshots=True)
def main():
 ap=argparse.ArgumentParser();ap.add_argument('--report',type=Path);ap.add_argument('--output',type=Path);args=ap.parse_args();p,cases,rows=checked_cases()
 result=dict(schema='cohort8-city-grid25-pure-guard-v1',entries=25,cases=62,planned_invocations=236,authored_endpoint_events=190,source_native_provider_recipe_guards=True,
  canonical_only=True,retained_only=['004867E0','004F4320','004F4440'],real_ftol=True,ratio_PC_RC_modes=12,FPSW_traps_proved=False,
  source_expansion=False,new_source_credit=0,new_entry_credit=0,new_algorithms=0,execution_performed=False,build_invoked=False,host_execution=False,public_edits=False)
 if args.report:result['post_audit']=audit(cases,json.loads(args.report.read_bytes()))
 if args.output:args.output.write_bytes((json.dumps(result,indent=2)+'\n').encode())
 print(json.dumps(result))
if __name__=='__main__':main()
