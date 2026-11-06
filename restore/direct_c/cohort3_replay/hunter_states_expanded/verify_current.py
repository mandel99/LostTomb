"""Expanded hunter states with exact current PE/MAP/source binding."""
from pathlib import Path
import argparse,datetime,json,sys,hashlib
sys.dont_write_bytecode=True
OUT=Path(__file__).resolve().parent;R=OUT.parents[2]
sys.path[:0]=[str(OUT),str(R/'direct_c/cohort3_replay'),str(R/'direct_c')]
from compiled_final6 import supplemental_registry as binding
from current_matrix.verify import audit
import hunter_cases,replay,guard
from registry import load,save

def main():
 ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--run',action='store_true');args=ap.parse_args()
 for item in json.loads((OUT/'publication-manifest.json').read_bytes())['files']:
  assert hashlib.sha256((OUT/item['path']).read_bytes()).hexdigest()==item['sha256'],item['path']
 guard.checked_cases();base=load(True)
 assert base['compiled_images']==json.loads((OUT/'image-profile.json').read_bytes())['compiled_images']
 with binding.temporary_registry(replay,base)as registry:
  cases=hunter_cases.cases(registry);assert len(cases)==30 and sum(c['expected_invocations']for c in cases)==210
  if not args.run:
   print(json.dumps(dict(cases=30,invocations=210,execution=False)));return
  directory=R/'build/actor-animal-following/cohort7-hunter-public-runs'/datetime.datetime.now(datetime.timezone.utc).strftime('%Y%m%d-%H%M%S-%f')
  directory.mkdir(parents=True,exist_ok=False);save(directory/'registry.json',registry)
  report=replay.execute(registry,cases,directory);save(directory/'authored-checks.json',audit(cases,report))
  save(directory/'detailed-checks.json',guard.audit(cases,report))
 print(json.dumps(dict(result=str(directory/'result.json'),cases=30,invocations=210,failed_cases=report['failed_cases'])),flush=True)

if __name__=='__main__':main()
