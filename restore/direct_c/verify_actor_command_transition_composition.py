"""Additional real seeker/search compositions with exact child parameters."""
import verify_actor_command_transition as oracle
from verify_actor_command_transition import ROOT,machines,names,sym,get,put,word,byte,UC_HOOK_CODE,UC_X86_REG_ESP,hashlib,json,target,target_sha,S

# This hook only observes actual calls; it replaces no game algorithm.
for compiled,u,state in machines:
 def search_request(uc,pc,size,data):
  if not data['collect'] or data['fn']!=0x4ec990:return
  sp=uc.reg_read(UC_X86_REG_ESP)
  args=[get(uc,sp+4+i*4) for i in range(7)]
  expected=sym(names[0x4eae20]) if data['compiled'] else 0x4eae20
  assert args[6]==expected
  data['events'].append(('actual_search_request',args[0],args[1],args[4],args[5],0x4eae20))
 pc=sym(names[0x4ec0f0]) if compiled else 0x4ec0f0
 u.hook_add(UC_HOOK_CODE,search_request,state,begin=pc,end=pc)

for scene in [0,1,2,0x100,0x102,0xffffffff]:
 for variant in [-1,0,1,2,32767]:
  for radius in [0,1]:
   oracle.exercise(0x4ec990,scene=scene,variant=variant,radius=radius,
    words=[(0xb350dc+(1000+11*228+12)*2,2)],actors={2:dict(kind=83,x=0x8000,y=0xffff)})
for x,y in [(0,0),(0,227),(227,0),(227,227),(-1,0),(0,-1)]:
 oracle.exercise(0x4ec990,actor_x=x,actor_y=y,radius=1)
oracle.exercise(0x4ec990,actors={2:dict(kind=83)},words=[(0xb350dc+(1000+11*228+10)*2,2)],
 child_mutations={(0x4eada0,1):[(0xbcd60c+2*388,0x002b002a)]})

primary_path=ROOT/f"metadata/actor-command-transition-{'diagnostic' if target.stem=='restore_link_check' else 'game'}.json"
primary=json.loads(primary_path.read_text());assert primary['passed'] and primary['compiled_C_sha256']==target_sha
for p,sha in primary['source_hashes'].items():assert hashlib.sha256((ROOT/p).read_bytes()).hexdigest()==sha,p
sources={**primary['source_hashes'],str(primary_path.relative_to(ROOT)).replace('\\','/'):hashlib.sha256(primary_path.read_bytes()).hexdigest(),
 'direct_c/verify_actor_command_transition_composition.py':hashlib.sha256((ROOT/'direct_c/verify_actor_command_transition_composition.py').read_bytes()).hexdigest()}
proof=dict(passed=True,compiled_C_path=str(target.resolve()),compiled_C_sha256=target_sha,native_sha256=primary['native_sha256'],checked=oracle.checked,invocations=oracle.invocations,actual_scenarios=oracle.actual_cases,synthetic_memory_scenarios=oracle.synthetic_cases,protocol_scenarios=oracle.protocol_cases,source_hashes=sources,controlled_algorithm_endpoints=[],controlled_platform_endpoints=[],limitations=['Real EC990 -> EC0F0 -> EAE20 compositions compare full data/store traces, full results and normalized actual search parameters; scene/variant table and radius transitions are exercised.','One child-call RAM mutation is synthetic. Incidental scratch outputs, arbitrary stack aliases and complete gameplay remain outside this finite verification.'])
suffix='diagnostic' if target.stem=='restore_link_check' else 'game'
(ROOT/f'metadata/actor-command-transition-composition-{suffix}.json').write_text(json.dumps(proof,indent=2)+'\n')
print('PASS real actor target-search compositions:',oracle.checked,'scenarios;',oracle.invocations,'invocations')
