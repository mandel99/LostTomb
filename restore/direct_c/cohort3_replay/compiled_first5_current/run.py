"""Offline comparison only; --run is required to instantiate Unicorn."""
import argparse,datetime,json,sys
from pathlib import Path
OUT=Path(__file__).resolve().parent
ROOT=OUT.parents[2]
sys.path[:0]=[str(OUT),str(ROOT/'direct_c/cohort3_replay')]
import replay
from supplemental_registry import temporary_registry
from cohort7_compiled_first5_cases import definitions
p=argparse.ArgumentParser(description=__doc__);p.add_argument('--run',action='store_true');a=p.parse_args()
with temporary_registry(replay) as registry:
 cases=definitions(registry)
 assert len(cases)==24 and sum(c['expected_invocations']for c in cases)==150
 if not a.run:
  print(json.dumps(dict(cases=24,planned_invocations=150,source_entries=registry['source_entries'],execution_performed=False)))
 else:
  directory=ROOT/'build/actor-animal-following/cohort7-first5-refreshed-runs'/datetime.datetime.now(datetime.timezone.utc).strftime('%Y%m%d-%H%M%S-%f')
  directory.mkdir(parents=True,exist_ok=False)
  r=replay.execute(registry,cases,directory)
  print(json.dumps(dict(cases=r['cases'],entries=r['entries_compared'],invocations=r['invocations'],failed_cases=r['failed_cases'],inputs_stable=r['inputs_stable'],result=str(directory/'result.json'),joint_gate_complete=False)))
  sys.exit(bool(r['failed_cases']or not r['inputs_stable']))
