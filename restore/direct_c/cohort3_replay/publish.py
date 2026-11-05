"""Publish a passing staged receipt without rebinding historical evidence."""
from pathlib import Path
from collections import Counter
import argparse,json
from registry import ROOT,LEDGER,digest,save

def publish(receipt,phase,contracts,scope):
 assert phase and all(c.isalnum()or c=='-'for c in phase)
 receipt=Path(receipt).resolve();directory=receipt.parent
 r=json.loads(receipt.read_bytes());b=json.loads((directory/'binding.json').read_bytes())
 assert r['cases']>0 and r['failed_cases']==0 and all(c['passed']for c in r['cases_detail'])
 assert r['binding_sha256']==digest(directory/'binding.json')
 assert r['case_inputs_sha256']==digest(directory/'case-inputs.json')
 assert digest(LEDGER)==b['registry']['source_ledger_sha256'],'Ledger changed since replay'
 for name,sha in b['source_hashes'].items():assert digest(ROOT/name)==sha,name
 for a in b['registry']['compiled_images'].values():
  assert digest(ROOT/a['path'])==a['sha256']and digest(ROOT/a['map_path'])==a['map_sha256']
 d=json.loads(LEDGER.read_bytes());count=d['selected_entries'];assert count==r['source_entries']
 contract_path=ROOT/contracts;contract=json.loads(contract_path.read_bytes())
 assert contract['entries']==r['entries_compared']and contract['recipes']==r['cases']
 entries=[];events=Counter()
 for addr in r['compared_entries']:
  rows=[c for c in r['cases_detail']if int(c['entry'],16)==int(addr,16)]
  m=next(m for m in b['registry']['members']if int(m['address'],16)==int(addr,16))
  for row in rows:events.update(e['token']for z in row['routes']if z['image']=='native'for e in z['events'])
  entries.append(dict(address=addr,name=m['name'],source=m['source'],source_sha256=m['source_sha256'],body_sha256=m['body_sha256'],cases=len(rows),case_ids=[c['id']for c in rows],routes=dict(Counter(z['image']+'/'+z['route']for row in rows for z in row['routes'])),authored_cases_passed=True))
 dest=ROOT/f'metadata/cohort3-{phase}-comparison.json';assert not dest.exists(),'Historical receipt already exists'
 report=dict(schema=f'cohort3-{phase}-comparison-v1',source_entries=count,entries_compared=len(entries),cases=r['cases'],invocations=r['invocations'],failed_cases=0,entry_results=entries,native_endpoint_calls=dict(events),native_sha256=b['registry']['native']['sha256'],compiled_images=b['registry']['compiled_images'],binding_sha256=r['binding_sha256'],case_inputs_sha256=r['case_inputs_sha256'],private_receipt=dict(path=receipt.relative_to(ROOT).as_posix(),sha256=digest(receipt)),source_snapshot_files=len(b['source_hashes']),source_ledger_input_sha256=b['registry']['source_ledger_sha256'],case_contracts=dict(path=contracts,sha256=digest(contract_path)),publisher_sha256=digest(__file__),scope=scope,limitations=contract.get('limitations',[]),cohort_validation_complete=False,whole_game_equivalence=False,host_execution=False)
 progress_path=ROOT/'metadata/cohort3-comparison-progress.json';previous=json.loads(progress_path.read_bytes());paths=[];reports=[]
 for epoch in previous['epochs']:
  p=ROOT/epoch['path'];assert digest(p)==epoch['sha256'];paths.append(p);reports.append(json.loads(p.read_bytes()))
 paths.append(dest);reports.append(report)
 assert all(q['compiled_images']==report['compiled_images']for q in reports),'Separate image epochs need explicit review'
 covered=set()
 for q in reports:
  for e in q['entry_results']:
   m=next(m for m in d['members']if int(m['address'],16)==int(e['address'],16))
   assert m['source_sha256']==e['source_sha256']and m['body_sha256']==e['body_sha256']
   covered.add(int(e['address'],16))
 save(dest,report)
 progress=dict(schema='cohort3-comparison-progress-v1',source_entries=count,entries_with_finite_comparisons=len(covered),entries_pending=count-len(covered),cases=sum(q['cases']for q in reports),invocations=sum(q['invocations']for q in reports),failed_cases=0,epochs=[dict(path=p.relative_to(ROOT).as_posix(),sha256=digest(p))for p in paths],pending_entries=[m['address']for m in d['members']if int(m['address'],16)not in covered],same_compiled_images_across_epochs=True,cohort_validation_complete=False,whole_game_equivalence=False,source_expansion_suspended=True,host_execution=False)
 save(progress_path,progress)
 d['joint_comparison_stage']=dict(status='in_progress',source_expansion_suspended=True,entries_compared=len(covered),entries_pending=count-len(covered),path=progress_path.relative_to(ROOT).as_posix(),sha256=digest(progress_path),complete=False)
 d['source_hashes'][Path(__file__).relative_to(ROOT).as_posix()]=digest(__file__);save(LEDGER,d)
 doc=ROOT/'metadata/CITY_ACTIVE_BUILDINGS.md'
 text=f'\n{phase} comparison:{r["cases"]} fresh cases /{r["invocations"]} invocations pass for{len(entries)} existing entries. Combined epochs:{len(covered)}/{count} entries,{progress["cases"]} cases /{progress["invocations"]} invocations;{count-len(covered)} pending. See {dest.name} and its explicit contracts/limitations. Selected C and compiled artifacts remain unchanged. Full cohort/game equivalence remains false.\n'
 doc.write_bytes(doc.read_bytes()+text.encode())
 print(json.dumps({k:progress[k]for k in ['entries_with_finite_comparisons','entries_pending','cases','invocations','failed_cases']}))

if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--receipt',type=Path,required=True);p.add_argument('--phase',required=True);p.add_argument('--contracts',required=True);p.add_argument('--scope',required=True);a=p.parse_args()
 publish(a.receipt,a.phase,a.contracts,a.scope)
