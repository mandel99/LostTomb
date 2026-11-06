import argparse,datetime,json,sys
from pathlib import Path
O=Path(__file__).resolve().parent;R=O.parents[2]
sys.path[:0]=[str(O),str(R/'direct_c/cohort3_replay')]
import replay
from supplemental_registry import temporary_registry
from constructor_cases import definitions
p=argparse.ArgumentParser();p.add_argument('--run',action='store_true');a=p.parse_args()
with temporary_registry(replay)as registry:
 cases=definitions(registry);assert len(cases)==2
 if not a.run:print(json.dumps(dict(cases=2,planned_invocations=6,execution_performed=False)))
 else:
  directory=R/'build/actor-animal-following/cohort7-event-constructor-runs'/datetime.datetime.now(datetime.timezone.utc).strftime('%Y%m%d-%H%M%S-%f');directory.mkdir(parents=True,exist_ok=False)
  result=replay.execute(registry,cases,directory)
  print(json.dumps({k:result[k]for k in ['entries_compared','cases','invocations','failed_cases','inputs_stable']},ensure_ascii=False));print(str(directory/'result.json'))
  sys.exit(bool(result['failed_cases']or not result['inputs_stable']))
