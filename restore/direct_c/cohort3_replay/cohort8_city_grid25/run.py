"""Root-owned once-only shared replay driver; default is pure authoring plan."""
from pathlib import Path
import argparse,datetime,json,sys
sys.dont_write_bytecode=True
OUT=Path(__file__).resolve().parent;ROOT=OUT.parents[2]
sys.path[:0]=[str(OUT),str(ROOT/'direct_c/cohort3_replay'),str(ROOT/'direct_c')]
import guard,supplemental_registry as binding
def main():
 ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--run',action='store_true');ap.add_argument('--label',default='baseline');args=ap.parse_args()
 p,cases,rows=guard.checked_cases()
 if not args.run:print(json.dumps(dict(entries=25,cases=len(cases),planned_invocations=236,execution_performed=False)));return
 import replay
 from registry import save
 directory=OUT/'runs'/(args.label+'-'+datetime.datetime.now(datetime.timezone.utc).strftime('%Y%m%d-%H%M%S-%f'));directory.mkdir(parents=True,exist_ok=False)
 with binding.temporary_registry(replay)as registry:
  save(directory/'registry.json',registry);report=replay.execute(registry,cases,directory)
 save(directory/'native-oracle-errors.json',[dict(id=c['id'],errors=r['errors'])for c in report['cases_detail']for r in c['routes']if r['image']=='native'and r['errors']])
 if not report['failed_cases']:save(directory/'authored-checks.json',guard.audit(cases,report))
 print(json.dumps(dict(result=str(directory/'result.json'),**{k:report[k]for k in('cases','entries_compared','invocations','failed_cases','inputs_stable')})),flush=True)
if __name__=='__main__':main()
