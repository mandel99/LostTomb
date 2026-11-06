"""Pure expanded tooltip guards; optional completed-report inspection."""
from pathlib import Path
import argparse,hashlib,json,sys
sys.dont_write_bytecode=True
OUT=Path(__file__).resolve().parent;ROOT=OUT.parents[2]
sys.path[:0]=[str(OUT),str(ROOT/'direct_c/cohort3_replay'),str(ROOT/'direct_c')]
import supplemental_registry as binding
import tooltip_cases as recipes


def oracle(case):
 keys=['authored_final_memory','authored_endpoint_order','authored_endpoint_observations','expected_endpoint_counts','expected_endpoint_arguments']
 return hashlib.sha256(json.dumps({k:case[k]for k in keys},sort_keys=True,separators=(',',':')).encode()).hexdigest()


def main():
 ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--registry',type=Path);ap.add_argument('--report',type=Path);ap.add_argument('--output',type=Path);args=ap.parse_args()
 p=binding.checked_inputs();ev=json.loads((OUT/'native-evidence.json').read_bytes())
 from native_test_pe import PE
 native=PE(ROOT/p['native']['path'])
 for m in ev['members']:
  assert m['status']=='exact_ghidra_function'and not m['assembly']['has_more']
  for i in m['assembly']['items']:
   at=int(i['address'],16)-native.base;raw=bytes.fromhex(i['bytes']);assert native.data[at:at+len(raw)]==raw
 cases=recipes.cases();assert len(cases)==31 and cases==recipes.cases()
 assert recipes.cases(dict(members=[]))==[]and len({c['id']for c in cases})==31
 group_counts={}
 for c in cases:
  assert c['entry']==0x4cabd0 and c['arguments']==c['named_arguments']==c['canonical_arguments']==[]
  assert c['result_mask']is None and c['native_cleanup']==c['named_cleanup']==c['canonical_cleanup']==0
  group=c['id'].split('/')[1].split('-')[0];group_counts[group]=group_counts.get(group,0)+1
  assert len(c['authored_endpoint_order'])==len(c['authored_endpoint_observations'])==sum(c['expected_endpoint_counts'].values())
  for ep in c['endpoints']:
   token=ep['token'];n=c['expected_endpoint_counts'][token]
   assert len(ep['responses'])==len(c['expected_endpoint_arguments'][token])==c['authored_endpoint_order'].count(token)==n
   if token=='line':assert all(ep[k]==v for k,v in recipes.MODEL_FIELDS.items())and ep['result_kind']=='void'
  if 'private_BYTE_oracle'in c:
   q=c['private_BYTE_oracle'];expected=[0x17,q['parent_message']]if q['result']else[0x44,17]
   assert c['expected_endpoint_arguments']['prepare']==[expected]
  if 'private_array_oracle'in c:
   assert c['private_array_oracle']['initialized_words']==16
   items=next(e for e in c['endpoints']if e['token']=='items')
   writes=items['responses'][0]['writes_arg_bytes']
   assert all(len(bytes.fromhex(v))==4 for _,_,v in writes)
  assert c['authored_final_memory']
 assert group_counts==dict(mode1=2,mode2=2,mode3=2,mode4=14,query20=5,query21=4,query78=2)
 assert [dict(id=c['id'],sha256=oracle(c))for c in cases]==p['authored_oracles']
 assert sum(c.get('expected_model_line_calls',0)for c in cases)*6==p['expected_live_model_visits']
 planned=None;inspected=0;live_visits=0
 if args.registry:
  cases=recipes.cases(json.loads(args.registry.read_bytes()));assert len(cases)==31
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
    frame_events=[e for e in route['physical_endpoint_arguments']if 'live_model_frame'in e]
    assert len(frame_events)==(0 if route['image']=='native'else c.get('expected_model_line_calls',0))
    for e in frame_events:
     f=e['live_model_frame']
     assert len(f['registers'])==8 and f['registers'][4]==f['stack_pointer']and f['return_site']==0x4cda31
     assert 0x2ff00000<=f['address']<0x30010000 and 0x2ff00000<=f['stack_pointer']<0x30010000
     assert f['dispatcher']!=0;live_visits+=1
     assert e['physical_C_arguments']==[f['address']]
     assert e['physical_C_stack_pointer']%4==0 and 0x2ff00000<=e['physical_C_stack_pointer']<0x30010000
    inspected+=1
  if planned is not None:assert inspected==planned
  assert live_visits==p['expected_live_model_visits']
 result=dict(schema='cohort7-tooltip-modes-query-authoring-guard-v1',hashes_valid=True,entries=1,cases=31,
  expected_invocations=217,planned_invocations=planned,inspected_report_invocations=inspected,
  expected_live_model_visits=p['expected_live_model_visits'],inspected_live_model_visits=live_visits,
  private_BYTE_paths=11,private_array_cases=2,current_image_binding=False,production_C_repairs=0,
  new_source_credit=0,new_entry_credit=0,replay_invoked=False,builds_run=False,host_execution=False,whole_game_equivalence=False)
 if args.output:args.output.write_bytes((json.dumps(result,indent=2)+'\n').encode())
 print(json.dumps(result))


if __name__=='__main__':main()
