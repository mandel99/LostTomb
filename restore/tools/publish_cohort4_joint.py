"""Publish a successful100-entry finite gate; keep all earlier receipts immutable."""
from pathlib import Path
from collections import Counter,defaultdict
import argparse,hashlib,json,sys

ROOT=Path(__file__).resolve().parents[1]
PIN='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()
def load(path):return json.loads(path.read_bytes())
def save(path,value):path.write_bytes((json.dumps(value,indent=2)+'\n').encode())
def binding(path):return dict(path=path.relative_to(ROOT).as_posix(),sha256=sha(path))

def main():
 parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('run_directory',type=Path);args=parser.parse_args()
 run=args.run_directory.resolve();assert run.is_relative_to((ROOT/'build').resolve())
 result=load(run/'result.json');inputs=load(run/'case-inputs.json');bound=load(run/'binding.json')
 assert result['failed_cases']==0 and result['passed_cases']==result['cases']==len(inputs)
 assert result['binding_sha256']==sha(run/'binding.json')and result['case_inputs_sha256']==sha(run/'case-inputs.json')
 assert bound['registry']['native']['sha256']==PIN and sha(ROOT/bound['registry']['native']['path'])==PIN
 for name,digest in bound['source_hashes'].items():assert sha(ROOT/name)==digest,('changed input',name)
 for item in bound['registry']['compiled_images'].values():
  assert sha(ROOT/item['path'])==item['sha256']and sha(ROOT/item['map_path'])==item['map_sha256']
 ledger=ROOT/'metadata/city-active-buildings-reconstruction.json';d=load(ledger)
 progress_path=ROOT/'metadata/cohort4-source-progress.json';progress=load(progress_path)
 wanted={int(a,16)for a in progress['new_entries']};assert len(wanted)==100 and d['selected_entries']==198
 assert sha(ledger)==bound['registry']['source_ledger_sha256'],'Registry snapshot no longer current'
 cases={c['id']:c for c in inputs};assert len(cases)==len(inputs)
 seen=defaultdict(set);per_entry=defaultdict(list);invocations=0
 for row in result['cases_detail']:
  assert row['passed']and row['id']in cases and int(row['entry'],16)==cases[row['id']]['entry']
  entry=int(row['entry'],16);assert entry in wanted
  for route in row['routes']:
   assert not route['errors'];seen[entry].add((route['image'],route['route']));invocations+=1
  assert sum(r['image']=='native'and r['route']=='native'for r in row['routes'])==1
  per_entry[entry].append(row['id'])
 assert set(seen)==wanted and invocations==result['invocations']
 for entry in wanted:assert {('native','native'),('retained','named'),('retained','canonical')}<=seen[entry]
 family_counts=Counter(c['id'].split('/')[0]for c in inputs)
 qualifications=[
  'Finite authored cases for100 new cohort4 entries; this is not complete behavior or whole-game equivalence.',
  'Every entry has original, retained named-C and retained canonical-ABI routes. Current game/diagnostic routes execute only where their providers remain linked.',
  'Controlled Win32, NLS, allocator, route-search, formatting and error callbacks compare logical arguments, actual receivers and declared output mutations; arbitrary external implementations are unproved.',
  'Live x87 values use exact raw80 fixtures and logical nonempty-register comparison. Control words are compared; only explicitly declared cases compare complete status/tag state. FPU fault resumption and arbitrary asynchronous state are unproved.',
  'Stack-probe TEST/MOV data accesses are compared exactly, including duplicate final probes. RET control reads are recorded separately and EIP/ESP restoration is checked. Guard faults and control-stack headroom remain qualified.',
  'Named optional compound output pointers use NULL; corresponding canonical register/flag reconstruction has finite cases. These optional named output writes are not claimed as separately exercised.',
  'Placement scope cleanup covers ordinary nonthrowing destruction. Exceptional unwinds, unresolved indirect consumers, discarded incidental GPR/EFLAGS and unmapped/corrupt input domains remain explicit gaps.',
  'Earlier cohort receipts retain their exact original sources and images; this report does not rebind the preceding98-entry finite gate to current artifacts.'
 ]
 report=dict(schema='cohort4-joint-native-C-comparison-v1',native_sha256=PIN,entries_compared=100,named_entries_compared=100,canonical_entries_compared=100,
  cumulative_selected_entries=198,unique_cases=len(inputs),passed_cases=len(inputs),invocations=invocations,failed_cases=0,
  family_cases=dict(family_counts),route_invocations={f'{image}/{route}':count for (image,route),count in Counter((r['image'],r['route'])for c in result['cases_detail']for r in c['routes']).items()},
  live_x87_value_cases=sum(bool(c.get('compare_x87_values'))for c in inputs),endpoint_tokens=sorted({e['token']for c in inputs for e in c.get('endpoints',[])}),
  private_result=binding(run/'result.json'),private_binding=binding(run/'binding.json'),private_cases=binding(run/'case-inputs.json'),
  source_hashes=bound['source_hashes'],compiled_images=bound['registry']['compiled_images'],unicorn_version=bound['unicorn_version'],
  entries=[dict(address=f'0x{a:08x}',case_ids=per_entry[a],routes=[list(v)for v in sorted(seen[a])])for a in sorted(wanted)],
  qualification=qualifications,finite_cohort_gate_complete=True,whole_game_equivalence=False,host_execution=False)
 initial=ROOT/'build/actor-animal-following/cohort4-joint-runs/20261105-204514-188737'
 if initial.exists():
  earlier=load(initial/'result.json');earlier_binding=load(initial/'binding.json')
  for name,digest in bound['source_hashes'].items():
   if name.startswith(('src/','runtime/','include/')):assert earlier_binding['source_hashes'][name]==digest
  report['earlier_failed_fixture_run']={**binding(initial/'result.json'),'failed_cases':earlier['failed_cases'],
   'corrections':['Separate relocated RET control reads from the TEST/MOV probe data sequence; record both and keep EIP/ESP checks.','Allow and watch the original MessageBox cache write on missing exported entry.'],
   'C_and_header_inputs_unchanged_between_runs':True}
 report_path=ROOT/'metadata/cohort4-joint-comparison.json';save(report_path,report);receipt=binding(report_path)
 plan_path=ROOT/'metadata/cohort4-joint-comparison-plan.json';plan=load(plan_path);prior_plan_sha=sha(plan_path)
 plan.update(phase='finite100_entry_joint_gate_passed',entries_compared=100,source_expansion_suspended=False,
  engine_contracts_added=plan.pop('engine_contracts_to_add'),comparison=receipt,cohort_validation_complete=True,
  initial_source_plan_sha256=prior_plan_sha)
 save(plan_path,plan)
 progress.update(source_expansion_suspended=False,source_stage='shared_strict_build_and_finite_joint_gate_passed',fresh_native_C_comparisons=len(inputs),
  cohort_validation_complete=True,joint_comparison=receipt,joint_comparison_plan=binding(plan_path),
  comparison_policy='The100-new-entry finite joint gate passed. Further connected source expansion may proceed; whole-game reconstruction and qualified domains remain incomplete.')
 save(progress_path,progress)
 d['joint_comparison_stage'].update(status='cohort4_100_entry_finite_joint_gate_passed',entries_compared=100,entries_pending=0,source_expansion_suspended=False,
  complete=True,path=receipt['path'],sha256=receipt['sha256'],progress=binding(progress_path),plan=binding(plan_path),whole_game_equivalence=False)
 d['retained_comparison_image'].update(status='current198_entry_image_cohort4_100_entry_finite_gate_passed',fresh_cohort4_entries_compared=100,previous_finite_receipts_rebound=False)
 d.update(status='cohort3_historical98_and_cohort4_current100_finite_gates_passed_full_reconstruction_incomplete',
  cohort='cohort3_and_cohort4_connected_C_entries',source_packet_contracts_are_historical=True,current_finite_comparison=receipt)
 d['limitations']=[v for v in d['limitations']if not v.startswith(('The100 new cohort4 entries have passed shared strict compile/link only.','The active source cohort4 has52 new entries'))]
 d['limitations'].append('Current cohort4 has a complete100-entry finite named/canonical comparison gate. The preceding98-entry receipts keep archived images; complete198-entry current-image or whole-game equivalence is not claimed.')
 queue_path=ROOT/'metadata/bulk-reconstruction-next-queue.json';queue=load(queue_path)
 queue.update(inventory_snapshot_source_expansion_suspended=queue['source_expansion_suspended'],source_expansion_suspended=False,
  later_cohort_gate_update=receipt,next_stage='Cohort4 finite joint gate passed; use connected review batches for the next source cohort. Driver still cannot install C or prove unknown semantics.')
 save(queue_path,queue)
 readme=ROOT/'README.md';b=readme.read_bytes();old=b'Shared strict compilation and both Release links pass. Cohort4 has reached\r\n100 new entries; joint native/C comparison must finish before further source\r\nexpansion. The preceding98-entry finite comparison retains its archived images. See\r\n'
 if old not in b:old=old.replace(b'\r\n',b'\n')
 assert b.count(old)==1
 new=b'Shared strict compilation and both Release links pass. Cohort4 passed its\r\n100-entry finite native/C gate:944 scenarios /3608 invocations, including all\r\nnamed and canonical definitions. The preceding98-entry finite gate retains\r\nits archived images. Further source reconstruction remains incomplete. See\r\n'
 if b'\r\n'not in old:new=new.replace(b'\r\n',b'\n')
 readme.write_bytes(b.replace(old,new))
 doc=ROOT/'metadata/CITY_ACTIVE_BUILDINGS.md';doc.write_bytes(doc.read_bytes()+b'\nCohort4 joint finite gate passed:100 named +100 canonical entries,944 unique scenarios /3608 invocations,0 failed cases. Live raw80 x87 inputs, partial-register results, private output aliases, callbacks and relocated stack-return context are explicit. Further source expansion may proceed. Indirect/fault/unwind and optional-output domains remain qualified in cohort4-joint-comparison.json; no whole-game equivalence or host launch.\n')
 for name in ['tools/publish_cohort4_joint.py','metadata/cohort4-joint-comparison.json',plan_path.relative_to(ROOT).as_posix()]:d['source_hashes'][name]=sha(ROOT/name)
 for name in d['source_hashes']:d['source_hashes'][name]=sha(ROOT/name)
 save(ledger,d)
 print(json.dumps(dict(entries=100,cases=len(inputs),invocations=invocations,failed=0,source_expansion_suspended=False,whole_game_equivalence=False)))

if __name__=='__main__':main()
