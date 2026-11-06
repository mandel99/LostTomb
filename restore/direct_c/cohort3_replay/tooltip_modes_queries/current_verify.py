"""Expanded modes/query matrix with independently derived expected outputs."""
from pathlib import Path
import argparse,datetime,json,sys,hashlib
sys.dont_write_bytecode=True
OUT=Path(__file__).resolve().parent;R=OUT.parents[2]
sys.path[:0]=[str(OUT),str(R/'direct_c/cohort3_replay'),str(R/'direct_c')]
import supplemental_registry as binding,tooltip_cases,replay,oracle_repair,snapshot_repair
from current_matrix.verify import audit
from registry import load,save

def corrected_cases(registry):
 return snapshot_repair.correct_query_snapshots(oracle_repair.correct_expected_formatted(tooltip_cases.cases(registry)))

def main():
 ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--run',action='store_true');args=ap.parse_args()
 for item in json.loads((OUT/'publication-manifest.json').read_bytes())['files']:
  assert hashlib.sha256((OUT/item['path']).read_bytes()).hexdigest()==item['sha256'],item['path']
 base=load(True);assert base['compiled_images']==json.loads((OUT/'image-profile.json').read_bytes())['compiled_images']
 from native_test_pe import PE
 native=PE(R/base['native']['path'])
 for name in ['mode2-native-evidence.json','query-native-evidence.json']:
  for member in json.loads((OUT/name).read_bytes())['members']:
   for i in member['assembly']['items']:
    at=int(i['address'],16)-native.base;raw=bytes.fromhex(i['bytes']);assert native.data[at:at+len(raw)]==raw
 with binding.temporary_registry(replay,base)as registry:
  cases=corrected_cases(registry);assert len(cases)==31 and sum(c['expected_invocations']for c in cases)==217
  if not args.run:
   print(json.dumps(dict(cases=31,invocations=217,execution=False)));return
  directory=R/'build/actor-animal-following/cohort7-tooltip-public-runs'/datetime.datetime.now(datetime.timezone.utc).strftime('%Y%m%d-%H%M%S-%f')
  directory.mkdir(parents=True,exist_ok=False);save(directory/'registry.json',registry)
  report=replay.execute(registry,cases,directory);save(directory/'authored-checks.json',audit(cases,report))
 print(json.dumps(dict(result=str(directory/'result.json'),cases=31,invocations=217,failed_cases=report['failed_cases'])),flush=True)

if __name__=='__main__':main()
