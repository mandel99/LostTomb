"""Real 41CD00 action -> command/search C compositions.

The action now has its own whole-body native/C suite. These additional
compositions do not expand that suite's instruction ownership.
"""
import verify_actor_command_transition as oracle
from verify_actor_command_transition import *

parent=json.loads((ROOT/'metadata/actor-command-nullary-next-rea-record.json').read_text())
raw=parent['normalized_result'];assert parent['authority']=='shipped-artifact' and parent['subject']['digest']['sha256']==catalogue['native_sha256']
assert raw['complete'] and raw['bytes_hex']==bytes(images[0].data[0x41cd00-images[0].base:0x41cfbd-images[0].base]).hex()
cases=invokes=0

def parent_case(**cfg):
 global cases,invokes
 expected=None
 for compiled,u,state in machines:
  oracle.setup(compiled,u,state,0x41cd00,dict(kind=0,status=0,**cfg))
  u.mem_write(0xbcb9c8,bytes(7200))
  if cfg.get('group_busy'):
   for n in range(10,50):byte(u,0xbcb9c8+n*144,1)
  if cfg.get('actor_busy'):
   for n in range(1,2000):byte(u,0xbcd606+n*388,1)
  byte(u,0xe38dda,cfg.get('enabled',1))
  for at,value in [(0xbca2c8,cfg.get('center_x',20)),(0xbca2cc,cfg.get('center_y',21)),(0xbcd5f0,0),(0xbcd5f4,0),(0xd056fc,123),(0xd056e0,127),(0x604950,1),(0xbca2dc,0),(0xbca2e0,0),(0xeaac28,1),(0xf5653c,1000),(0xf55b88,0),(0x77aefc,0)]:put(u,at,value)
  for reg,value in zip(registers,[0x11223344,0x22222222,0x33333333,saved[0],SP,*saved[1:]]):u.reg_write(reg,value)
  u.reg_write(UC_X86_REG_EFLAGS,2);put(u,SP,STOP)
  if compiled:put(u,SP+4,0x41cd00)
  state['collect']=True
  pc=sym('restore_actor_command_action') if compiled else 0x41cd00
  u.emu_start(pc,STOP,count=50000000);state['collect']=False;invokes+=1
  assert not state['faults'] and u.reg_read(UC_X86_REG_EIP)==STOP,(cfg,compiled,state['faults'])
  assert u.reg_read(UC_X86_REG_ESP)==SP+4 and [u.reg_read(registers[i]) for i in [3,5,6,7]]==saved
  if compiled:assert u.reg_read(UC_X86_REG_EAX)==1
  current=(tuple(state['writes']),tuple(state['events']),snapshot(u),bytes(u.mem_read(S,0x48000)))
  if expected is None:expected=current
  elif expected!=current:
   changes=[]
   for part in [0,1]:
    if expected[part]!=current[part]:changes.append((part,len(expected[part]),len(current[part]),next(((n,a,b) for n,(a,b) in enumerate(zip(expected[part],current[part])) if a!=b),None)))
   memory=[next(((hex(base+n),a,b) for n,(a,b) in enumerate(zip(x,y)) if a!=b),None) for base,x,y in [(0x560000,expected[2],current[2]),(S,expected[3],current[3])]]
   raise AssertionError((cfg,changes,memory))
 cases+=1

parent_case(enabled=0);parent_case()
if '--quick' in sys.argv:print('PASS quick parent composition',cases,invokes);sys.exit(0)
for enabled in [2,255]:parent_case(enabled=enabled)
for terrain in [4,0x80004]:parent_case(terrain=terrain)
for history in [99,110,111]:parent_case(history=history)
for first in [0,0xffffffff]:parent_case(first=first)
parent_case(group_busy=True);parent_case(actor_busy=True);parent_case(center_x=0,center_y=0)
assert cases==14 and invokes==28
primary_path=ROOT/f"metadata/actor-command-transition-{'diagnostic' if target.stem=='restore_link_check' else 'game'}.json"
primary=json.loads(primary_path.read_text());assert primary['passed'] and primary['compiled_C_sha256']==target_sha
for path,sha in primary['source_hashes'].items():assert hashlib.sha256((ROOT/path).read_bytes()).hexdigest()==sha,path
paths=[*primary['source_hashes'],'direct_c/verify_actor_command_parent.py','metadata/actor-command-nullary-next-rea-record.json']
proof=dict(passed=True,native_sha256=catalogue['native_sha256'],compiled_C_path=str(target.resolve()),compiled_C_sha256=target_sha,checked=cases,invocations=invokes,controlled_algorithm_endpoints=[],controlled_platform_endpoints=[],source_hashes={p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest() for p in paths},limitations=['The 195-instruction command parent is compared through its real C-child composition. Its primary suite owns the converted instructions; these extra cases add no instruction ownership.','Complete original data/private bytes, ordered stores, meaningful child arguments and normal-return nonvolatile ABI are compared. Parent incidental EAX/flags and arbitrary caller-stack aliases are not certified.','The compiled action facade must return handled=1. Native caller argument/AL contracts are qualified by the separate caller audit. No host game is launched.'])
suffix='diagnostic' if target.stem=='restore_link_check' else 'game'
(ROOT/f'metadata/actor-command-parent-{suffix}.json').write_text(json.dumps(proof,indent=2)+'\n')
print('PASS real actor command parent compositions:',cases,'scenarios;',invokes,'invocations')
