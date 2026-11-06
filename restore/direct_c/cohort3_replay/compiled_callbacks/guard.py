"""Pure source/evidence/recipe guards; optional future report inspection."""
from pathlib import Path
import argparse,json,sys,struct
sys.dont_write_bytecode=True
OUT=Path(__file__).resolve().parent;ROOT=OUT.parents[2]
sys.path[:0]=[str(OUT),str(ROOT/'direct_c/cohort3_replay'),str(ROOT/'direct_c')]
import supplemental_registry as binding
import callback_cases as recipes


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
 table=next(x for x in evidence['data_spans']if x['address']==0x494190)
 assert struct.unpack('<5I',bytes.fromhex(table['hex']))==(0x49411c,0x4940f8,0x494110,0x494104,0x4940ec)
 cases=recipes.cases();assert len(cases)==38 and cases==recipes.cases()
 assert len({c['id']for c in cases})==38 and recipes.cases(dict(members=[]))==[]
 counts={}
 for c in cases:
  counts[c['entry']]=counts.get(c['entry'],0)+1
  assert c['native_cleanup']==c['named_cleanup']==c['canonical_cleanup']==0
  assert c['arguments']==c['named_arguments']==c['canonical_arguments']==[]
  assert c['result_mask']is None and 'expected_masked_result'not in c
  assert len(c['authored_endpoint_order'])==len(c['authored_endpoint_observations'])==sum(c['expected_endpoint_counts'].values())
  assert c['authored_final_memory']and c['compare_x87_values']
  for ep in c['endpoints']:
   token=ep['token'];n=c['expected_endpoint_counts'][token]
   assert len(ep['responses'])==len(c['expected_endpoint_arguments'][token])==c['authored_endpoint_order'].count(token)==n
   if token=='state':assert ep['native']==0x493610 and ep['arity']==0 and c['expected_endpoint_arguments'][token]==[[]]*n
  if c['entry']==0x49b390:assert not c['endpoints']
 assert counts=={0x49b390:2,0x496f20:4,0x493ec0:4,0x494010:10,0x489ca0:9,0x48a260:9}
 # This models route availability, never code PCs or a current image binding.
 fake=dict(members=[dict(address=c['address'],providers={label:dict(named=dict(present=True),canonical=dict(present=True)if c['canonical_symbol']else None)for label in('game','diagnostic','retained')})for c in p['members']])
 assert sum(c['expected_invocations']for c in recipes.cases(fake))==212
 negative=next(c for c in cases if 'negative-death-timer-and-frame-wrap'in c['id'])
 final=dict(negative['authored_final_memory']);assert final[recipes.ORIENTATION]=='85ffffff'
 inspected=0;planned=None
 if args.registry:
  registry=json.loads(args.registry.read_bytes());selected=recipes.cases(registry)
  assert len(selected)==38;cases=selected;planned=sum(c['expected_invocations']for c in cases)
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
    inspected+=1
  if planned is not None:assert inspected==planned
 result=dict(schema='cohort7-callbacks-authoring-guard-v1',hashes_valid=True,entries=6,authored_cases=38,
  current_image_binding=False,planned_invocations=planned,expected_route_invocations=212,
  inspected_report_invocations=inspected,confirmed_semantic_C_repairs=0,missing_providers=0,
  new_source_credit=0,new_entry_credit=0,replay_invoked=False,builds_run=False,host_execution=False,whole_game_equivalence=False)
 if args.output:args.output.write_bytes((json.dumps(result,indent=2)+'\n').encode())
 print(json.dumps(result))


if __name__=='__main__':main()
