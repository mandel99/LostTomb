"""Publish all49 finite cases by exact program/input identity across receipts."""
from pathlib import Path
from collections import Counter,defaultdict
import argparse,json,hashlib,sys
ROOT=Path(__file__).resolve().parents[1]
PIN='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def load(p):return json.loads(p.read_bytes())
def save(p,d):p.write_bytes((json.dumps(d,indent=2)+'\n').encode())
def binding(p):return dict(path=p.relative_to(ROOT).as_posix(),sha256=sha(p))
def product_sources(sources):return {k:v for k,v in sources.items()if k.startswith(('src/','runtime/','include/','generated/'))}
def encode(c):return json.loads(json.dumps({k:([[a,v.hex()]for a,v in value]if k=='stores'else value)for k,value in c.items()}))

def main():
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('run_directory',type=Path,nargs='+');args=p.parse_args()
 ledger=ROOT/'metadata/city-active-buildings-reconstruction.json';d=load(ledger)
 progress_path=ROOT/'metadata/cohort6-source-progress.json';progress=load(progress_path)
 wanted={int(a,16)for a in progress['new_entries']}
 assert len(wanted)==49 and d['selected_entries']==341 and progress['source_expansion_suspended']
 runs=[path.resolve()for path in args.run_directory]
 prior=ROOT/'build/actor-animal-following/cohort6-input-abi-runs/20261105-234950-820962'
 if prior not in runs:runs.append(prior)
 assert len(set(runs))==len(runs)and all(path.is_relative_to((ROOT/'build').resolve())for path in runs)
 receipts=[];epochs=[];expected_program=None;latest=None
 for path in runs:
  result=load(path/'result.json');bound=load(path/'binding.json');inputs=load(path/'case-inputs.json')
  assert result['inputs_stable']and not result['input_integrity_errors']
  assert result['binding_sha256']==sha(path/'binding.json')and result['case_inputs_sha256']==sha(path/'case-inputs.json')
  assert result['cases']==len(inputs)==len(result['cases_detail'])
  assert len({row['id']for row in result['cases_detail']})==len(inputs)
  program=dict(native=bound['registry']['native'],images=bound['registry']['compiled_images'],sources=product_sources(bound['source_hashes']))
  assert program['native']['sha256']==PIN and sha(ROOT/program['native']['path'])==PIN
  for item in program['images'].values():assert sha(ROOT/item['path'])==item['sha256']and sha(ROOT/item['map_path'])==item['map_sha256']
  for name,h in program['sources'].items():assert sha(ROOT/name)==h,name
  if expected_program is None:expected_program=program;latest=bound
  else:assert program==expected_program,'Cannot combine receipts for different program bytes'
  receipts.append((path,result,bound,{c['id']:c for c in inputs}));epochs.append(dict(result=binding(path/'result.json'),binding=binding(path/'binding.json'),inputs=binding(path/'case-inputs.json')))
 assert sha(ledger)==latest['registry']['source_ledger_sha256'],'Newest registry snapshot is not current'
 for name,h in latest['source_hashes'].items():assert sha(ROOT/name)==h,name
 sys.path.insert(0,str(ROOT/'direct_c/cohort3_replay'))
 from cases import definitions
 cases={c['id']:encode(c)for c in definitions(latest['registry'])if c['entry']in wanted}
 assert len(cases)==157
 selected={};origins={};counts=Counter();per_entry=defaultdict(list);routes=defaultdict(set)
 for epoch,(path,result,bound,inputs)in enumerate(receipts):
  for row in result['cases_detail']:
   if row['id']not in cases or inputs[row['id']]!=cases[row['id']]:continue
   assert row['passed']and all(not route['errors']for route in row['routes']),('Matching current case failed',row['id'])
   if row['id']in selected:continue
   assert int(row['entry'],16)==cases[row['id']]['entry']
   assert sum(r['image']=='native'and r['route']=='native'for r in row['routes'])==1
   selected[row['id']]=row;origins[row['id']]=epoch;counts[epoch]+=1
 assert set(selected)==set(cases),('Missing current cases',sorted(set(cases)-set(selected)))
 for identity,row in selected.items():
  entry=int(row['entry'],16);per_entry[entry].append(identity)
  routes[entry].update((r['image'],r['route'])for r in row['routes'])
 assert set(per_entry)==wanted
 for entry in wanted:assert {('native','native'),('retained','named'),('retained','canonical')}<=routes[entry]
 invocations=sum(len(row['routes'])for row in selected.values())
 for i,epoch in enumerate(epochs):epoch['selected_cases']=counts[i]
 snap=runs[0]/'publication-input-snapshots';snap.mkdir(exist_ok=True);snapshots={}
 for name,h in {**{n:h for n,h in latest['source_hashes'].items()if not n.startswith(('src/','runtime/','include/','direct_c/'))},'metadata/city-active-buildings-reconstruction.json':sha(ledger)}.items():
  payload=(ROOT/name).read_bytes();assert hashlib.sha256(payload).hexdigest()==h
  dst=snap/(h+'.bytes');assert not dst.exists()or dst.read_bytes()==payload
  if not dst.exists():dst.write_bytes(payload)
  snapshots[name]=dict(sha256=h,path=dst.relative_to(ROOT).as_posix())
 save(snap/'index.json',snapshots)
 report=dict(schema='cohort6-finite-joint-comparison-v1',native_sha256=PIN,selected_source_entries=341,new_entries_compared=49,
  cases=len(selected),invocations=invocations,failed_cases=0,comparison_epochs=epochs,
  composition_policy='Every current serialized case must match exactly; all epochs must have identical product sources, native PE, compiled PE and MAP bytes. Prior receipts are never rebound. Matching failed cases are rejected.',
  publication_input_snapshots=binding(snap/'index.json'),source_hashes=latest['source_hashes'],compiled_images=expected_program['images'],
  family_cases=dict(Counter(identity.split('/')[0]for identity in selected)),
  entries=[dict(address=hex(entry),case_ids=per_entry[entry],routes=[list(v)for v in sorted(routes[entry])])for entry in sorted(wanted)],
  case_epochs=origins,finite_cohort_gate_complete=True,whole_game_equivalence=False,host_execution=False,
  qualification=['Finite157-scenario gate for all49 source bindings; not complete branch/domain or gameplay equivalence.',
   'Actual frame/native algorithms execute with original SP aliases, dirty GPRs, inherited x87/CW and native RET0/4/8/12. Arbitrary virtual configurations, successful routing/assignment, final completion and exceptional/fault/concurrency domains remain qualified in the exact case inputs.',
   'The18 already-passed advisor/overlay cases are reused only after byte-for-byte program/artifact and serialized-input equality. The earlier17 bridge cases receive no new case credit.',
   'Resource25/pyramid-summary, full sprite decode/draw and type183 advisor refresh remain outside these new entry fixtures. Other declared finite domains are not broadened by this receipt.',
   'Prior292-entry comparison epochs retain their own source/image bindings. No complete341-entry current-image or whole-game equivalence is claimed.'])
 report_path=ROOT/'metadata/cohort6-joint-comparison.json';save(report_path,report);receipt=binding(report_path)
 progress.update(source_expansion_suspended=False,source_stage='closed49_finite_joint_gate_passed',fresh_native_C_comparisons=len(selected),
  entries_with_fresh_cases=49,entries_pending_fresh_cases=0,cohort_validation_complete=True,joint_comparison=receipt,
  comparison_policy='The closed49-entry finite joint gate passed. Continue actual unresolved behavior/native-scope work; existing C is preserved and whole-game fidelity remains incomplete.')
 save(progress_path,progress)
 d['joint_comparison_stage'].update(status='cohort6_closed49_finite_joint_gate_passed',source_expansion_suspended=False,
  entries_compared=49,entries_pending=0,complete=True,path=receipt['path'],sha256=receipt['sha256'],progress=binding(progress_path),whole_game_equivalence=False)
 d['retained_comparison_image'].update(status='current341_entry_image_C6_49_entry_finite_gate_passed',fresh_cohort6_entries_compared=49,previous_finite_receipts_rebound=False)
 d.update(status='cohort6_closed49_finite_joint_gate_passed_full_reconstruction_incomplete',current_finite_comparison=receipt)
 d['limitations']=[v for v in d['limitations']if not v.startswith('Cohort6 closes49 real source entries.')]
 d['limitations'].append('Cohort6 passed157 finite scenarios for all49 named/canonical entries. The earlier292-entry receipts remain historical; whole-game, unvisited indirect/routing/fault domains and remaining native-scope work are incomplete.')
 for m in d['members']:
  if int(m['address'],16)in wanted:m.update(stage='finite_cohort6_native_C_cases_passed',finite_comparison=receipt)
 for name in ['tools/publish_cohort6_joint.py',report_path.relative_to(ROOT).as_posix(),progress_path.relative_to(ROOT).as_posix()]:d['source_hashes'][name]=sha(ROOT/name)
 save(ledger,d)
 print(json.dumps(dict(entries=49,cases=len(selected),invocations=invocations,failed=0,source_expansion_suspended=False,whole_game_equivalence=False)))

if __name__=='__main__':main()
