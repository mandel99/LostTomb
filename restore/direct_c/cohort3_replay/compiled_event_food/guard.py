"""Pure authoring guards; optional inspection of a future shared report."""
from pathlib import Path
import argparse,json,sys,struct
sys.dont_write_bytecode=True
OUT=Path(__file__).resolve().parent;ROOT=OUT.parents[2]
sys.path[:0]=[str(OUT),str(ROOT/'direct_c/cohort3_replay'),str(ROOT/'direct_c')]
import supplemental_registry as binding
import event_food_cases as recipes


def main():
 ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--registry',type=Path)
 ap.add_argument('--report',type=Path);ap.add_argument('--output',type=Path);args=ap.parse_args()
 p=binding.checked_inputs();evidence=json.loads((OUT/'native-evidence.json').read_bytes())
 from native_test_pe import PE
 native=PE(ROOT/p['native']['path'])
 for member in evidence['members']:
  assert member['status']=='exact_ghidra_function'and not member['assembly']['has_more']
  for i in member['assembly']['items']:
   at=int(i['address'],16)-native.base;raw=bytes.fromhex(i['bytes']);assert native.data[at:at+len(raw)]==raw
 for item in evidence['data_spans']:
  at=item['address']-native.base;raw=bytes.fromhex(item['hex']);assert native.data[at:at+len(raw)]==raw
 table=next(x for x in evidence['data_spans']if x['address']==0x45d994)
 assert struct.unpack('<8I',bytes.fromhex(table['hex'])[:32])==(0x45d6fd,0x45d708,0x45d70f,0x45d716,0x45d71d,0x45d701,0x45d72b,0x45d724)
 cases=recipes.cases();assert len(cases)==37 and cases==recipes.cases()
 assert len({c['id']for c in cases})==37 and recipes.cases(dict(members=[]))==[]
 counts={}
 for c in cases:
  counts[c['entry']]=counts.get(c['entry'],0)+1
  assert c['native_cleanup']==c['named_cleanup']==c['canonical_cleanup']==0
  assert c['arguments']==c['named_arguments']==c['canonical_arguments']
  assert c['result_mask']==({0x45de80:0xffffffff,0x45e150:0xffff}.get(c['entry']))
  assert len(c['authored_endpoint_order'])==len(c['authored_endpoint_observations'])==sum(c['expected_endpoint_counts'].values())
  assert c['authored_final_memory']and c['compare_x87_values']
  for ep in c['endpoints']:
   token=ep['token'];n=c['expected_endpoint_counts'][token]
   assert len(ep['responses'])==len(c['expected_endpoint_arguments'][token])==c['authored_endpoint_order'].count(token)==n
  if c['entry']==0x44ba90:assert c['receiver']==recipes.MANAGER and c['arguments']==[]and not c['endpoints']
  if c['entry']==0x45d630:
   assert c['arguments']==[recipes.PLAYER]
   oracle=c['explicit_food_oracle'];final=dict(c['authored_final_memory'])
   vector=struct.unpack('<9i',bytes.fromhex(final[recipes.STOCK]));assert list(vector)==oracle['resource_vector']
  if c['entry']==0x45e150:
   assert not c['endpoints']and c['expected_masked_result']<=0xffff
 assert counts=={0x44ba90:6,0x452160:9,0x45d630:9,0x45de80:7,0x45e150:6}
 inspected=0;planned=None
 if args.registry:
  registry=json.loads(args.registry.read_bytes());selected=recipes.cases(registry)
  assert len(selected)==37;cases=selected;planned=sum(c['expected_invocations']for c in cases)
 if args.report:
  rows={r['id']:r for r in json.loads(args.report.read_bytes())['cases_detail']}
  for c in cases:
   row=rows[c['id']];assert row['passed'],c['id']
   if 'expected_invocations'in c:assert len(row['routes'])==c['expected_invocations']
   for route in row['routes']:
    assert not route['errors']
    assert [e['token']for e in route['events']]==c['authored_endpoint_order']
    assert [e['observed']for e in route['events']]==json.loads(json.dumps(c['authored_endpoint_observations']))
    for token,expected in c['expected_endpoint_arguments'].items():
     assert [e['arguments']for e in route['events']if e['token']==token]==expected
    if c['result_mask']is not None:assert int(route['logical_result']['value'],16)&c['result_mask']==c['expected_masked_result']
    inspected+=1
  if planned is not None:assert inspected==planned
 result=dict(schema='cohort7-event-food-authoring-guard-v1',hashes_valid=True,entries=5,authored_cases=37,
  current_image_binding=False,planned_invocations=planned,inspected_report_invocations=inspected,
  expected_after_retention_invocations=223,pending_retained_symbol='FUN_00452160',
  confirmed_semantic_C_repairs=0,new_source_credit=0,new_entry_credit=0,
  replay_invoked=False,builds_run=False,host_execution=False,whole_game_equivalence=False)
 if args.output:args.output.write_bytes((json.dumps(result,indent=2)+'\n').encode())
 print(json.dumps(result))


if __name__=='__main__':main()
