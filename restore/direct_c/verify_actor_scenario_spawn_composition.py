"""Actual adjacent point/group initializers and full-width cached table values."""
import verify_actor_scenario_spawn as oracle
from verify_actor_scenario_spawn import *

compositions=0;root_invocations=0
for scene,variant,history in itertools.product([0,1,2],[0,1],[0,99]):
 expected=None
 for c,u,state in machines:
  for route in ['native'] if not c else ['named','canonical']:
   setup(c,u,state,0x4bbc10,dict(scene=scene,variant=variant,history=history,serial=0xffffffff))
   state['collect']=True
   for fn in [0x4bbc10,0x4bb3f0]:
    state['fn']=fn;result=invoke(c,u,state,fn,[],route);root_invocations+=1
    assert result is None and not state['faults'],(scene,variant,history,route,state['faults'])
   state['collect']=False
   current=(tuple(state['writes']),tuple(state['events']),snapshot(u),bytes(u.mem_read(S,0x50000)))
   if expected is None:expected=current
   else:assert current==expected,('adjacent initializers',scene,variant,history,route)
 compositions+=1

for kind in [0x10013,0xffff0014,0x80000000]:
 exercise(kind_word=kind,kind_dword=kind,count_word=1,count_dword=1,controlled=[0x4b6ff0,0x4b4800,0x4ec410])
exercise(count_word=1,count_dword=1,child_mutations={(0x4b6ff0,1):[(0x5e4cb8,0x7fffffff),(0x5e4c28,0)]})
assert oracle.checked==4 and oracle.invocations==12 and compositions==12 and root_invocations==72
primary=json.loads((ROOT/f'metadata/actor-scenario-spawn-{"diagnostic" if target.stem=="restore_link_check" else "game"}.json').read_text())
assert primary['passed'] and primary['compiled_C_sha256']==target_sha
paths=[*primary['source_hashes'],'direct_c/verify_actor_scenario_spawn_composition.py']
proof=dict(passed=True,native_sha256=catalogue['native_sha256'],compiled_C_path=str(target.resolve()),compiled_C_sha256=target_sha,actual_compositions=compositions,actual_parent_invocations=root_invocations,width_cache_scenarios=oracle.checked,width_cache_invocations=oracle.invocations,controlled_endpoints_in_compositions=[],source_hashes={p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest() for p in paths},limitations=['Twelve actual adjacent4BBC10 then4BB3F0 sequences compare complete data, ordered stores and meaningful child arguments through original/named/canonical C. No game algorithm is replaced in these compositions.','Three explicit child-contract cases verify full DWORD kinds at actor creation versus WORD kinds at group allocation. One actual-child mutation case verifies that both member counts remain cached. These extra finite cases do not expand the primary instruction ownership or prove all mission-loading behavior.'])
suffix='diagnostic' if target.stem=='restore_link_check' else 'game';(ROOT/f'metadata/actor-scenario-spawn-composition-{suffix}.json').write_text(json.dumps(proof,indent=2)+'\n')
print('PASS12 actual adjacent initializer compositions /72 parent invocations, plus four width/cache scenarios /12 invocations; no platform endpoints')
