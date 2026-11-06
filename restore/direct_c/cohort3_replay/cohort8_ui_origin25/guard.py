"""Pure native-byte/source/MAP/recipe guard. Optional audit of root's report."""
from pathlib import Path
import argparse,collections,hashlib,json,sys
sys.dont_write_bytecode=True
OUT=Path(__file__).resolve().parent;ROOT=OUT.parents[2]
sys.path[:0]=[str(OUT),str(ROOT/'direct_c/cohort3_replay'),str(ROOT/'direct_c')]
import ui_cases as recipes
import supplemental_registry as binding
def packed(v):return json.dumps(v,sort_keys=True,separators=(',',':'),default=lambda b:{'bytes_hex':b.hex()}).encode()
def commitment(v):return hashlib.sha256(packed(v)).hexdigest()
def checked_cases():
 p=binding.checked_inputs();rows=binding.supplemental_members();cases=recipes.cases(dict(members=rows))
 assert len(cases)==50 and sum(c['expected_invocations']for c in cases)==200
 assert len({c['id']for c in cases})==50 and all(n==2 for n in collections.Counter(c['entry']for c in cases).values())
 assert cases==recipes.cases(dict(members=rows))and recipes.cases(dict(members=[]))==[]
 commits=json.loads((OUT/'case-commitments.json').read_bytes())['cases'];assert {c['id']:commitment(c)for c in cases}==commits
 for c in cases:
  assert c['routes']==['native','canonical']and c['required_retained_route']=='canonical'
  assert c['native_cleanup']==c['named_cleanup']==c['canonical_cleanup']==0
  assert c['canonical_registers_and_flags']is False and c['result_mask']is None
  assert c['canonical_arguments']==c['named_arguments']==c['arguments']
  assert len(c['arguments'])==(2 if c['entry']==0x5309b0 else 0)
  assert not any(k in c for k in('borrowed_native_stack','stack_prefix','route_stores','expected_eax','expected_masked_result','context_outputs'))
  assert c['watched']==c['write_ranges']==[(a,len(bytes.fromhex(b)))for a,b in c['authored_final_memory']]
  assert len(c['authored_endpoint_order'])==len(c['authored_endpoint_observations'])==sum(c['expected_endpoint_counts'].values())
  for ep in c['endpoints']:
   token=ep['token'];count=c['expected_endpoint_counts'][token]
   assert len(ep['responses'])==len(c['expected_endpoint_arguments'][token])==c['authored_endpoint_order'].count(token)==count
   assert ep['result_kind']==recipes.RESULTS.get(token,'void')
   assert ep['native_cleanup']==(12 if token=='sprite'else 0)and ep['compiled_cleanup']==0
   assert ep.get('native_receiver')==(0xf55bb0 if token=='sprite'else None)
   assert not any(k in ep for k in('compiled_frame_symbols','native_register_args','compiled_register_args','compiled_receiver_arg'))
   for arguments,response in zip(c['expected_endpoint_arguments'][token],ep['responses']):
    assert len(arguments)==ep['arity'];assert all(at==recipes.WIDTH and width==4 for at,width,value in response.get('mutations',[]))
 from registry import symbol
 maps={label:(ROOT/image['map_path']).read_text()for label,image in p['compiled_images'].items()}
 for token,by_image in p['endpoint_providers'].items():
  name=f'FUN_{recipes.HELPERS[token][0]:08x}'
  for label,text in maps.items():assert symbol(text,name)==by_image[label],(token,label)
 from native_test_pe import PE
 native=PE(ROOT/p['native']['path']);evidence=json.loads((OUT/'native-evidence.json').read_bytes())
 for m in evidence['members']:
  assert not m['assembly']['has_more']
  for i in m['assembly']['items']:
   a=int(i['address'],16)-native.base;raw=bytes.fromhex(i['bytes']);assert native.data[a:a+len(raw)]==raw
  assert {i['instruction']for i in m['assembly']['items']if i['instruction'].startswith('RET')}=={'RET'}
 for i in json.loads((OUT/'sprite-endpoint-evidence.json').read_bytes())['instructions']:
  a=int(i['address'],16)-native.base;raw=bytes.fromhex(i['bytes']);assert native.data[a:a+len(raw)]==raw
 meta=json.loads((ROOT/p['source100_metadata']).read_bytes())
 chosen=sorted([m for m in meta['members']if any(v['name'].lower()in('dat_00e38da4','dat_00e38da8')for v in m['local_view_changes'])and int(m['address'],16)not in p['excluded_entries']],key=lambda m:int(m['address'],16))[:25]
 assert [m['address']for m in chosen]==[m['address']for m in p['members']]
 assert all(m['source_sha256']==digest for m,digest in[(m,p['source_guards'][m['source']])for m in chosen])
 # A known revealing row must retain the native 124-byte stride, and both
 # modes preserve the true eight arguments after the 4F4FF0 continuation.
 for c in cases:
  if c['entry']==0x52e610:assert c['expected_endpoint_arguments']['paragraph'][0][1]in(0x10,0x11)
  if c['entry']==0x533470:
   q=c['expected_endpoint_arguments']['query_setting'][0]
   assert [17,*q]in c['expected_endpoint_arguments']['numeric_center']
 return p,cases,rows
def audit(cases,report):
 actual={r['id']:r for r in report['cases_detail']};routes=events=0
 for c in cases:
  row=actual[c['id']];assert row['passed'],c['id']
  assert {(r['image'],r['route'])for r in row['routes']}=={('native','native'),('game','canonical'),('diagnostic','canonical'),('retained','canonical')}
  assert len(row['routes'])==4
  for r in row['routes']:
   assert not r['errors'],(c['id'],r['errors'])
   assert [e['token']for e in r['events']]==c['authored_endpoint_order']
   assert [e['observed']for e in r['events']]==json.loads(json.dumps(c['authored_endpoint_observations']))
   for token,expected in c['expected_endpoint_arguments'].items():assert [e['arguments']for e in r['events']if e['token']==token]==expected
   routes+=1;events+=len(r['events'])
 assert routes==200
 return dict(routes=routes,endpoint_events=events,independent_endpoint_arguments_and_snapshots=True)
def main():
 ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--report',type=Path);ap.add_argument('--output',type=Path);args=ap.parse_args()
 p,cases,rows=checked_cases();result=dict(schema='cohort8-ui-origin-joint25-pure-guard-v1',entries=25,cases=50,planned_invocations=200,
  authored_endpoint_events=sum(len(c['authored_endpoint_order'])for c in cases),canonical_only=True,source_native_owner_recipe_guards=True,
  source_expansion=False,new_source_credit=0,new_entry_credit=0,new_algorithms=0,replay_invoked=False,build_invoked=False,host_execution=False,public_edits=False,whole_game_equivalence=False)
 if args.report:result['root_report_post_audit']=dict(path=str(args.report),sha256=binding.digest(args.report),**audit(cases,json.loads(args.report.read_bytes())))
 if args.output:args.output.write_bytes((json.dumps(result,indent=2)+'\n').encode())
 print(json.dumps(result))
if __name__=='__main__':main()
