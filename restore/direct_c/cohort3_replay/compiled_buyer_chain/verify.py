from pathlib import Path
import datetime,json,sys
sys.dont_write_bytecode=True
OUT=Path(__file__).resolve().parent;R=OUT.parents[2]
sys.path[:0]=[str(OUT),str(R/'direct_c/cohort3_replay'),str(R/'direct_c')]
import supplemental_registry as binding
import buyer_chain_cases,replay
from registry import load,save
base=load(True);assert base['compiled_images']==json.loads((OUT/'image-profile.json').read_bytes())
with binding.temporary_registry(replay,base)as registry:
    cases=buyer_chain_cases.cases(registry)
    assert len(cases)==22 and sum(c['expected_invocations']for c in cases)==154
    directory=R/'build/actor-animal-following/cohort7-buyer-chain-public-runs'/datetime.datetime.now(datetime.timezone.utc).strftime('%Y%m%d-%H%M%S-%f')
    directory.mkdir(parents=True,exist_ok=False);save(directory/'registry.json',registry)
    result=replay.execute(registry,cases,directory)
print(json.dumps(dict(result=str(directory/'result.json'),**{k:result[k]for k in ['cases','entries_compared','invocations','failed_cases','inputs_stable']})),flush=True)
assert not result['failed_cases']and result['inputs_stable']
