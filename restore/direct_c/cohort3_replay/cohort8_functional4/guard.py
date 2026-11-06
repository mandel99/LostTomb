"""Pure shared recipe/evidence audit; optional post-audit of root's replay report."""
from pathlib import Path
import argparse,collections,hashlib,json,sys
sys.dont_write_bytecode=True
OUT=Path(__file__).resolve().parent;ROOT=OUT.parents[2]
sys.path[:0]=[str(OUT),str(ROOT/'direct_c/cohort3_replay'),str(ROOT/'direct_c')]
import functional_cases as recipes
import supplemental_registry as binding


def packed(value):return json.dumps(value,sort_keys=True,separators=(',',':'),default=lambda b:{'bytes_hex':b.hex()}).encode()
def commitment(value):return hashlib.sha256(packed(value)).hexdigest()


def checked_cases():
 p=binding.checked_inputs();rows=binding.supplemental_members();cases=recipes.cases(dict(members=rows))
 assert len(cases)==33 and sum(c['expected_invocations']for c in cases)==177
 assert cases==recipes.cases(dict(members=rows))and recipes.cases(dict(members=[]))==[]
 assert dict(collections.Counter(c['entry']for c in cases))=={0x409eb0:8,0x44c1a0:8,0x4c42b0:6,0x4e0f90:11}
 assert len({c['id']for c in cases})==33
 commitments=json.loads((OUT/'case-commitments.json').read_bytes())['cases']
 assert {c['id']:commitment(c)for c in recipes.cases()}==commitments
 for c in cases:
  assert c['native_cleanup']==c['named_cleanup']==c['canonical_cleanup']==0
  assert c['canonical_registers_and_flags']is False
  assert c['named_arguments']==c['canonical_arguments']==c['arguments']
  if c['entry']==0x4e0f90:assert c['result_mask']==0xffffffff and c['expected_masked_result']in(0,1)
  else:assert c['result_mask']is None and'expected_masked_result'not in c
  if c['entry']in(0x44c1a0,0x4c42b0):assert c['required_retained_route']=='canonical'
  assert not any(k in c for k in['borrowed_native_stack','stack_prefix','route_stores'])
  assert c['write_ranges']==c['watched']==[(a,len(bytes.fromhex(b)))for a,b in c['authored_final_memory']]
  assert len(c['authored_endpoint_order'])==len(c['authored_endpoint_observations'])==sum(c['expected_endpoint_counts'].values())
  for ep in c['endpoints']:
   token=ep['token'];count=c['expected_endpoint_counts'][token]
   assert len(ep['responses'])==len(c['expected_endpoint_arguments'][token])==c['authored_endpoint_order'].count(token)==count
   assert ep['native_cleanup']==ep['compiled_cleanup']==0
   assert not any(k in ep for k in['compiled_frame_symbols','native_register_args','compiled_register_args'])
   assert ep['result_kind']==('full32'if token=='eligible'else'void')
 from native_test_pe import PE
 from registry import symbol
 maps={label:(ROOT/image['map_path']).read_text()for label,image in p['compiled_images'].items()}
 for c in cases:
  for ep in c['endpoints']:
   for name in ep['compiled_symbols']:
    for label,text in maps.items():assert symbol(text,name),(label,'missing declared endpoint',name)
 native=PE(ROOT/p['native']['path']);evidence=json.loads((OUT/'native-evidence.json').read_bytes())
 for m in evidence['members']:
  assert not m['assembly']['has_more']
  for i in m['assembly']['items']:
   a=int(i['address'],16)-native.base;raw=bytes.fromhex(i['bytes']);assert native.data[a:a+len(raw)]==raw
 for item in evidence['switch_data']:
  a=int(item['address'],16)-native.base;raw=bytes.fromhex(item['bytes_hex']);assert native.data[a:a+len(raw)]==raw
 proof=json.loads((OUT/'native-derivation.json').read_bytes());item=proof['inventory']['omitted_switch_code_from_pinned_PE']
 a=int(item['start'],16)-native.base;raw=bytes.fromhex(item['bytes_hex']);assert native.data[a:a+len(raw)]==raw
 metadata=json.loads((ROOT/p['source100_metadata']).read_bytes())
 for contract in p['members']:
  member=next(m for m in metadata['members']if int(m['address'],16)==int(contract['address'],16))
  assert member['source']==contract['source']and member['source_sha256']==contract['source_sha256']
 return p,cases,rows


def audit(cases,report):
 actual={r['id']:r for r in report['cases_detail']};routes=events=0
 for c in cases:
  row=actual[c['id']];assert row['passed'],c['id']
  assert len(row['routes'])==c['expected_invocations']
  wanted_routes={('native','native')}
  for label in('game','diagnostic','retained'):
   if c['entry']==0x4c42b0:
    if label=='retained':wanted_routes.add((label,'canonical'))
   elif c['entry']==0x44c1a0:wanted_routes.add((label,'canonical'))
   else:wanted_routes.update({(label,'named'),(label,'canonical')})
  assert {(r['image'],r['route'])for r in row['routes']}==wanted_routes
  for route in row['routes']:
   assert not route['errors'],(c['id'],route['errors'])
   assert [e['token']for e in route['events']]==c['authored_endpoint_order']
   assert [e['observed']for e in route['events']]==json.loads(json.dumps(c['authored_endpoint_observations']))
   for token,wanted in c['expected_endpoint_arguments'].items():assert [e['arguments']for e in route['events']if e['token']==token]==wanted
   if c['entry']==0x4e0f90:assert int(route['logical_result']['value'],16)==c['expected_masked_result']
   routes+=1;events+=len(route['events'])
 assert routes==177
 return dict(routes=routes,endpoint_events=events,independent_endpoint_arguments_and_snapshots=True)


def main():
 ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--report',type=Path);ap.add_argument('--output',type=Path);args=ap.parse_args()
 _,cases,_=checked_cases()
 result=dict(schema='cohort8-functional4-pure-authoring-guard-v1',entries=4,cases=33,planned_invocations=177,
  hashes_native_bytes_current_source_owners_and_recipes_valid=True,profile_bound=True,
  full32_result_entry='004E0F90',void_entries=['00409EB0','0044C1A0','004C42B0'],
  bridge_API='Actual canonical physical entry only; no fabricated named/raw/model frame',
  replay_required_retained_route_opt_in=True,replay_invoked=False,build_invoked=False,host_execution=False,
  public_edits=False,new_source_credit=0,new_entry_credit=0,new_algorithms=0,whole_game_equivalence=False)
 if args.report:result['root_report_post_audit']=dict(path=str(args.report),sha256=binding.digest(args.report),**audit(cases,json.loads(args.report.read_bytes())))
 if args.output:args.output.write_bytes((json.dumps(result,indent=2)+'\n').encode())
 print(json.dumps(result))


if __name__=='__main__':main()
