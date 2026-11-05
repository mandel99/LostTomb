"""Curate the full finite cohort gate and same-input ABI supplements."""
from pathlib import Path
from collections import Counter
import argparse,json,sys
ROOT=Path(__file__).resolve().parents[1]
sys.path.insert(0,str(ROOT/'direct_c/cohort3_replay'))
from registry import LEDGER,digest,save

def publish(paths):
 d=json.loads(LEDGER.read_bytes());members={int(m['address'],16):m for m in d['members']}
 epochs=[];cases={};rows=[];images=None;snapshots=[]
 for index,path in enumerate(paths):
  path=Path(path).resolve();r=json.loads(path.read_bytes());folder=path.parent
  b=json.loads((folder/'binding.json').read_bytes());inputs=json.loads((folder/'case-inputs.json').read_bytes())
  assert r['failed_cases']==0 and all(c['passed']for c in r['cases_detail'])
  assert r['binding_sha256']==digest(folder/'binding.json')and r['case_inputs_sha256']==digest(folder/'case-inputs.json')
  assert images is None or images==b['registry']['compiled_images'];images=b['registry']['compiled_images']
  if index==0:assert {int(c['entry'],16)for c in r['cases_detail']}==set(members),'Primary receipt must cover every selected entry'
  for c in inputs:
   normalized={k:v for k,v in c.items()if k!='canonical_owner'}
   if index:assert c['id']in cases and normalized==cases[c['id']],'Supplement changed a scenario input'
   else:assert c['id']not in cases;cases[c['id']]=normalized
  for m in b['registry']['members']:
   current=members[int(m['address'],16)]
   assert (m['source_sha256'],m['body_sha256'])==(current['source_sha256'],current['body_sha256'])
  snapshots.append(b['source_hashes']);rows.extend(r['cases_detail'])
  epochs.append(dict(path=path.relative_to(ROOT).as_posix(),sha256=digest(path),binding_sha256=r['binding_sha256'],case_inputs_sha256=r['case_inputs_sha256'],cases=r['cases'],invocations=r['invocations'],entries_compared=r['entries_compared']))
 for name,sha in snapshots[-1].items():assert digest(ROOT/name)==sha,('changed final replay source',name)
 for snapshot in snapshots[:-1]:
  for name,sha in snapshot.items():
   if digest(ROOT/name)!=sha:assert name=='direct_c/cohort3_replay/cases.py','Only the supplementary canonical route selection may change'
 for a in images.values():assert digest(ROOT/a['path'])==a['sha256']and digest(ROOT/a['map_path'])==a['map_sha256']
 entries=[]
 for address,m in members.items():
  selected=[c for c in rows if int(c['entry'],16)==address];routes=Counter(z['image']+'/'+z['route']for c in selected for z in c['routes'])
  assert routes['native/native']and routes['retained/named']and routes['retained/canonical'],('missing required route',m['address'])
  entries.append(dict(address=m['address'],name=m['name'],source=m['source'],source_sha256=m['source_sha256'],body_sha256=m['body_sha256'],cases=len({c['id']for c in selected}),case_passes=len(selected),routes=dict(routes),authored_cases_passed=True))
 proof=ROOT/'build/actor-animal-following/cohort3-verification/retained-safe-handler.json';safe=json.loads(proof.read_bytes());assert safe['sha256']==images['retained']['sha256']and safe['map_sha256']==images['retained']['map_sha256']
 safe_path=ROOT/'metadata/cohort3-retained-safe-handlers.json';safe_path.write_bytes(proof.read_bytes())
 progress_path=ROOT/'metadata/cohort3-comparison-progress.json';prior=json.loads(progress_path.read_bytes())
 for epoch in prior['epochs']:
  p=ROOT/epoch['path'];assert digest(p)==epoch['sha256'];old=json.loads(p.read_bytes())
  for label in ('game','diagnostic'):assert old['compiled_images'][label]==images[label]
 archived=ROOT/'metadata/cohort3-pre-joint-comparison-progress.json';assert not archived.exists();archived.write_bytes(progress_path.read_bytes())
 contracts=[];limitations=['Finite authored inputs, watched write footprints and explicit external boundaries only; no all-branch/domain or complete gameplay equivalence.']
 for p in sorted((ROOT/'metadata').glob('cohort3-*-case-contracts.json')):
  q=json.loads(p.read_bytes());contracts.append(dict(path=p.relative_to(ROOT).as_posix(),sha256=digest(p)))
  limitations.extend(q.get('limitations',q.get('remaining_domains',[])))
 dest=ROOT/'metadata/cohort3-joint-comparison.json';assert not dest.exists()
 report=dict(schema='cohort3-joint-finite-comparison-v1',source_entries=len(members),entries_compared=len(entries),cases=len(cases),case_passes=len(rows),invocations=sum(e['invocations']for e in epochs),failed_cases=0,canonical_entries_compared=len(entries),entry_results=entries,compiled_images=images,native_sha256=d['native_sha256']if 'native_sha256'in d else b['registry']['native']['sha256'],receipts=epochs,case_contracts=contracts,safe_handlers=dict(path=safe_path.relative_to(ROOT).as_posix(),sha256=digest(safe_path)),publisher_sha256=digest(__file__),scope='Every selected named C definition and retained canonical entry passed authored finite native/C comparisons. ABI supplements repeat identical scenario inputs and add only canonical route ownership.',limitations=limitations,cohort_validation_complete=True,finite_case_gate_complete=True,all_branches_or_domains_validated=False,whole_game_equivalence=False,host_execution=False)
 save(dest,report)
 progress=dict(schema='cohort3-comparison-progress-v2',source_entries=len(members),entries_with_finite_comparisons=len(entries),entries_pending=0,canonical_entries_compared=len(entries),cases=len(cases),case_passes=len(rows),invocations=report['invocations'],failed_cases=0,epochs=[dict(path=dest.relative_to(ROOT).as_posix(),sha256=digest(dest))],pending_entries=[],same_compiled_images_across_epochs=True,prior_progress=dict(path=archived.relative_to(ROOT).as_posix(),sha256=digest(archived)),prior_retained_image_changed=True,prior_main_game_and_diagnostic_images_unchanged=True,cohort_validation_complete=True,finite_case_gate_complete=True,all_branches_or_domains_validated=False,whole_game_equivalence=False,source_expansion_suspended=False,host_execution=False)
 save(progress_path,progress)
 d['joint_comparison_stage']=dict(status='finite_case_gate_passed',source_expansion_suspended=False,entries_compared=len(entries),entries_pending=0,canonical_entries_compared=len(entries),path=progress_path.relative_to(ROOT).as_posix(),sha256=digest(progress_path),complete=True,validation_scope='Authored finite native/C cases; whole-game/all-domain equivalence remains false.')
 readme=ROOT/'direct_c/cohort3_replay/README.md';readme.write_bytes(readme.read_bytes()+b'\nFull finite milestone: all98 named C and retained canonical entries are covered. The retained image now forces every selected canonical symbol; main game/diagnostic images remain unchanged. Joint and same-input ABI supplement receipts retain separate exact hashes. This completes the authored finite gate, not branch/domain or gameplay equivalence.\n')
 for p in (Path(__file__),safe_path,readme):d['source_hashes'][p.relative_to(ROOT).as_posix()]=digest(p)
 save(LEDGER,d)
 doc=ROOT/'metadata/CITY_ACTIVE_BUILDINGS.md';doc.write_bytes(doc.read_bytes()+f'\nJoint finite milestone:98/98 named C definitions and98/98 retained canonical entries pass {len(cases)} unique scenarios, {len(rows)} case passes and {report["invocations"]} native/C invocations. The rebuilt offline image retains all canonical symbols; original game/diagnostic hashes and historical receipts remain unchanged. Cohort source expansion may resume. Full gameplay, branch and input-domain equivalence remain unproven. See cohort3-joint-comparison.json.\n'.encode())
 print(json.dumps({k:report[k]for k in ('entries_compared','canonical_entries_compared','cases','case_passes','invocations','failed_cases')}))

if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('receipts',nargs='+',type=Path);publish(p.parse_args().receipts)
