"""Pure final-six authoring guard; optional authored report inspection."""
from pathlib import Path
import argparse,json,sys
sys.dont_write_bytecode=True
OUT=Path(__file__).resolve().parent;ROOT=OUT.parents[2]
sys.path[:0]=[str(OUT),str(ROOT/'direct_c/cohort3_replay'),str(ROOT/'direct_c')]
import supplemental_registry as binding
import remaining_cases as recipes


def main():
 ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--registry',type=Path)
 ap.add_argument('--report',type=Path);ap.add_argument('--output',type=Path);args=ap.parse_args()
 p=binding.checked_inputs();ev=json.loads((OUT/'native-evidence.json').read_bytes())
 from native_test_pe import PE
 native=PE(ROOT/p['native']['path'])
 for m in ev['members']:
  assert m['status']=='exact_ghidra_function'and not m['assembly']['has_more']
  for i in m['assembly']['items']:
   at=int(i['address'],16)-native.base;raw=bytes.fromhex(i['bytes']);assert native.data[at:at+len(raw)]==raw
 cases=recipes.cases();assert len(cases)==36 and cases==recipes.cases()
 assert recipes.cases(dict(members=[]))==[]and len({c['id']for c in cases})==36
 counts={}
 for c in cases:
  counts[c['entry']]=counts.get(c['entry'],0)+1
  assert c['named_cleanup']==c['canonical_cleanup']==0
  assert c['native_cleanup']=={0x488400:4,0x53f7d0:20}.get(c['entry'],0)
  assert c['result_mask']==(0xffffffff if c['entry']in(0x488400,0x53f7d0)else None)
  assert len(c['authored_endpoint_order'])==len(c['authored_endpoint_observations'])==sum(c['expected_endpoint_counts'].values())
  for ep in c['endpoints']:
   token=ep['token'];n=c['expected_endpoint_counts'][token]
   assert len(ep['responses'])==len(c['expected_endpoint_arguments'][token])==c['authored_endpoint_order'].count(token)==n
  if c['entry']==0x53f7d0:
   assert c['direct_entry_proof']and len(c['arguments'])==5 and c['named_arguments']==[c['receiver'],*c['arguments']]
   assert c['arguments'][1]==recipes.BUFFER
   if c['expected_endpoint_counts']['update']:assert c['expected_endpoint_arguments']['update'][0][0]==recipes.COLOR
  elif c['entry']==0x488400:assert c['named_arguments']==c['canonical_arguments']==[c['receiver'],*c['arguments']]
  else:assert c['arguments']==c['named_arguments']==c['canonical_arguments']==[]
  assert c['authored_final_memory']
 assert counts=={0x496140:9,0x48e9e0:2,0x4a2430:2,0x488400:3,0x4cabd0:4,0x53f7d0:16}
 planned=None;inspected=0
 if args.registry:
  cases=recipes.cases(json.loads(args.registry.read_bytes()));assert len(cases)==36
  planned=sum(c['expected_invocations']for c in cases)
 if args.report:
  rows={r['id']:r for r in json.loads(args.report.read_bytes())['cases_detail']}
  for c in cases:
   row=rows[c['id']];assert row['passed'],c['id']
   if 'expected_invocations'in c:assert len(row['routes'])==c['expected_invocations']
   for route in row['routes']:
    assert not route['errors']
    assert [e['token']for e in route['events']]==c['authored_endpoint_order']
    assert [e['observed']for e in route['events']]==json.loads(json.dumps(c['authored_endpoint_observations']))
    for token,expected in c['expected_endpoint_arguments'].items():assert [e['arguments']for e in route['events']if e['token']==token]==expected
    if c['result_mask']is not None:assert int(route['logical_result']['value'],16)&c['result_mask']==c['expected_masked_result']
    inspected+=1
  if planned is not None:assert inspected==planned
 r=dict(schema='cohort7-final6-authoring-guard-v1',hashes_valid=True,entries=6,cases=36,expected_route_invocations=177,
  planned_invocations=planned,inspected_report_invocations=inspected,current_image_binding=False,
  confirmed_semantic_C_repairs=0,new_source_credit=0,new_entry_credit=0,replay_invoked=False,builds_run=False,host_execution=False,whole_game_equivalence=False)
 if args.output:args.output.write_bytes((json.dumps(r,indent=2)+'\n').encode())
 print(json.dumps(r))


if __name__=='__main__':main()
