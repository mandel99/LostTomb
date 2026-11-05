"""Publish the closed94-entry finite gate without rebinding earlier receipts."""
from pathlib import Path
from collections import Counter,defaultdict
import argparse,hashlib,json
ROOT=Path(__file__).resolve().parents[1]
PIN='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'

def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def load(p):return json.loads(p.read_bytes())
def save(p,d):p.write_bytes((json.dumps(d,indent=2)+'\n').encode())
def binding(p):return dict(path=p.relative_to(ROOT).as_posix(),sha256=sha(p))

def main():
 parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('run_directory',type=Path);args=parser.parse_args()
 run=args.run_directory.resolve();assert run.is_relative_to((ROOT/'build').resolve())
 result=load(run/'result.json');inputs=load(run/'case-inputs.json');bound=load(run/'binding.json')
 assert result['failed_cases']==0 and result['passed_cases']==result['cases']==len(inputs)
 assert result['inputs_stable']and not result['input_integrity_errors']
 assert result['binding_sha256']==sha(run/'binding.json')and result['case_inputs_sha256']==sha(run/'case-inputs.json')
 assert bound['registry']['native']['sha256']==PIN and sha(ROOT/bound['registry']['native']['path'])==PIN
 for name,digest in bound['source_hashes'].items():assert sha(ROOT/name)==digest,('changed input',name)
 for image in bound['registry']['compiled_images'].values():
  assert sha(ROOT/image['path'])==image['sha256']and sha(ROOT/image['map_path'])==image['map_sha256']
 ledger=ROOT/'metadata/city-active-buildings-reconstruction.json';d=load(ledger)
 progress_path=ROOT/'metadata/cohort5-source-progress.json';progress=load(progress_path)
 wanted={int(a,16)for a in progress['new_entries']};assert len(wanted)==94 and d['selected_entries']==292
 assert sha(ledger)==bound['registry']['source_ledger_sha256'],'Registry snapshot no longer current'
 cases={c['id']:c for c in inputs};assert len(cases)==len(inputs)
 seen=defaultdict(set);per_entry=defaultdict(list);invocations=0;seen_cases=set()
 for row in result['cases_detail']:
  assert row['passed']and row['id']in cases and row['id']not in seen_cases
  seen_cases.add(row['id']);entry=int(row['entry'],16)
  assert entry in wanted and entry==cases[row['id']]['entry']
  for route in row['routes']:
   assert not route['errors'];seen[entry].add((route['image'],route['route']));invocations+=1
  assert sum(r['image']=='native'and r['route']=='native'for r in row['routes'])==1
  per_entry[entry].append(row['id'])
 assert seen_cases==set(cases)and set(seen)==wanted and invocations==result['invocations']
 for a in wanted:assert {('native','native'),('retained','named'),('retained','canonical')}<=seen[a]
 report=dict(schema='cohort5-finite-joint-comparison-v1',native_sha256=PIN,
  selected_source_entries=292,new_entries_compared=94,cases=len(inputs),invocations=invocations,failed_cases=0,
  family_cases=dict(Counter(c['id'].split('/')[0]for c in inputs)),
  route_invocations={f'{image}/{route}':n for (image,route),n in Counter((r['image'],r['route'])for c in result['cases_detail']for r in c['routes']).items()},
  private_result=binding(run/'result.json'),private_binding=binding(run/'binding.json'),private_cases=binding(run/'case-inputs.json'),
  source_hashes=bound['source_hashes'],compiled_images=bound['registry']['compiled_images'],unicorn_version=bound['unicorn_version'],
  entries=[dict(address=f'0x{a:08x}',case_ids=per_entry[a],routes=[list(v)for v in sorted(seen[a])])for a in sorted(wanted)],
  qualification=[
   'Finite authored cases for all94 new entries, including every retained named and canonical definition; no complete branch/domain or gameplay equivalence claim.',
   'Captured-context cases use actual original stack addresses, live incoming registers and a separate C control stack. Stack-headroom, guard-fault, asynchronous and undefined-flag domains remain qualified.',
   'Only declared typed external boundaries are controlled. Ordered arguments, nullable private packet bytes, receiver identities and declared writes are compared; other external implementations remain unproved.',
   'Destructors execute real array iteration and base cleanup, compare every vtable store and reverse element visit, and restore FS0. Throwing elements and exceptional unwinds are unvisited.',
   'NLS cases cover ANSI/wide branches, count/capacity and provider failures with actual argument slots; malformed provider data and fault resumption remain unproved.',
   'Window/tag/list cases use allocated ordinary objects and explicit logical result widths; dangling pointers, overlapping independent allocations and concurrent mutation are excluded.',
   'Detailed finite domains and controlled endpoints are retained in the hash-bound private case inputs. Earlier cohort and ABI receipts retain their original artifacts and hashes.'
  ],finite_cohort_gate_complete=True,whole_game_equivalence=False,host_execution=False)
 earlier=ROOT/'build/actor-animal-following/cohort5-joint-runs/20261105-224455-209987'
 if earlier.exists():
  old=load(earlier/'binding.json')
  changed=[name for name,digest in bound['source_hashes'].items()if name.startswith(('src/','runtime/','include/'))and old['source_hashes'].get(name)!=digest]
  assert changed==['include/native_member_bridge.h'],changed
  report['earlier_failed_run']=dict(**binding(earlier/'result.json'),failed_cases=6,changed_product_inputs=changed,
   corrections=['Capture below the native workspace to preserve untouched published packet bytes.',
    'Declare controlled typed sound backends for event effects instead of entering an uninitialized OS/audio provider.',
    'Initialize the actual reusable-record status BYTE at offset50, not offset54.'])
 report_path=ROOT/'metadata/cohort5-joint-comparison.json';save(report_path,report);receipt=binding(report_path)
 progress.update(source_expansion_suspended=False,source_stage='shared_strict_build_and_finite_joint_gate_passed',
  fresh_native_C_comparisons=len(inputs),entries_with_fresh_cases=94,entries_pending_fresh_cases=0,
  cohort_validation_complete=True,joint_comparison=receipt,
  comparison_policy='The closed94-entry finite gate passed; further connected source reconstruction may proceed. Whole-game behavior and qualified domains remain incomplete.')
 save(progress_path,progress)
 d['joint_comparison_stage'].update(status='cohort5_94_entry_finite_joint_gate_passed',entries_compared=94,entries_pending=0,
  source_expansion_suspended=False,complete=True,path=receipt['path'],sha256=receipt['sha256'],progress=binding(progress_path),whole_game_equivalence=False)
 d['retained_comparison_image'].update(status='current292_entry_image_cohort5_94_entry_finite_gate_passed',fresh_cohort5_entries_compared=94,previous_finite_receipts_rebound=False)
 d.update(status='cohort5_finite94_entry_gate_passed_full_reconstruction_incomplete',current_finite_comparison=receipt)
 d['limitations']=[v for v in d['limitations']if not v.startswith('Cohort5 has94 source entries')]
 d['limitations'].append('Cohort5 passed its94-entry finite named/canonical gate. Previous198-entry receipts keep their original archived images; all292 entries on current images and whole-game equivalence are not claimed.')
 d['native_stack_capture_fix'].update(regression='Passed44CDC0 uniform/nonuniform stack histories and the full94-entry finite gate.',finite_comparison=receipt)
 for m in d['members']:
  if int(m['address'],16)in wanted:m.update(stage='finite_cohort5_native_C_cases_passed',finite_comparison=receipt)
 names=['tools/publish_cohort5_joint.py','metadata/cohort5-joint-comparison.json']
 if progress_path.relative_to(ROOT).as_posix()in d['source_hashes']:names.append(progress_path.relative_to(ROOT).as_posix())
 for name in names:d['source_hashes'][name]=sha(ROOT/name)
 save(ledger,d)
 print(json.dumps(dict(entries=94,cases=len(inputs),invocations=invocations,failed=0,source_expansion_suspended=False,whole_game_equivalence=False)))

if __name__=='__main__':main()
