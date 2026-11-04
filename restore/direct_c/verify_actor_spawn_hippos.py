"""Whole native/C Hippo Stomp action with real and explicit child-contract lanes."""
from verify_options_startup import *
from unicorn import UC_HOOK_MEM_WRITE,UC_HOOK_MEM_READ,UC_HOOK_MEM_INVALID,UcError
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import json

catalogue=json.loads((ROOT/'metadata/actor-spawn-hippos-native.json').read_text())
record=json.loads((ROOT/'metadata/actor-command-nullary-next-rea-record.json').read_text())
f=catalogue['functions'][0];raw=bytes.fromhex(f['bytes_hex']);entry=0x41cd00
assert record['authority']=='shipped-artifact' and record['subject']['digest']['sha256']==catalogue['native_sha256']
assert record['evidence_id']==f['bytes_evidence_id'] and record['normalized_result']['complete']
assert record['normalized_result']['bytes_hex']==f['bytes_hex']
assert raw==bytes(images[0].data[entry-images[0].base:entry-images[0].base+len(raw)])
code=list(Cs(CS_ARCH_X86,CS_MODE_32).disasm(raw,entry));points={i.address for i in code};assert len(points)==195
branches={i.address:{int(i.op_str,16),i.address+i.size} for i in code if i.mnemonic.startswith('j') and i.mnemonic!='jmp'}
hits=set();edges=set();last=[None];checked=invocations=actual_cases=protocol_cases=synthetic_cases=0
names={0x4b6ff0:'FUN_004b6ff0',0x4b4800:'FUN_004b4800',0x4ec410:'FUN_004ec410',0x4ec990:'restore_actor_seek_other_kind'}
arity={0x4b6ff0:5,0x4b4800:5,0x4ec410:2,0x4ec990:1}
POOL,GROUP,HISTORY,RNGA,RNGB=0xbcd5f8,0xbcb9c8,0xd3b1c4,0xd3b384,0xd3b380

def restore_seed(u):
 current=bytes(u.mem_read(0x560000,len(seed)))
 for offset in range(0,len(seed),4096):
  page=seed[offset:offset+4096]
  if current[offset:offset+4096]!=page:u.mem_write(0x560000+offset,page)

for compiled,u,state in machines:
 state.update(compiled=compiled,collect=False)
 def write(uc,access,at,size,value,data):
  if data['collect'] and (0x560000<=at<0x126d000 or S<=at<S+0x48000):data['writes'].append((at,size,value&((1<<(size*8))-1)))
 u.hook_add(UC_HOOK_MEM_WRITE,write,state)
 def invalid(uc,access,at,size,value,data):data['faults'].append((access,at,size));return False
 u.hook_add(UC_HOOK_MEM_INVALID,invalid,state)
 def read(uc,access,at,size,value,data):
  if not data['collect'] or at not in data['read_mutations']:return
  count=data['read_counts'].get(at,0)+1;data['read_counts'][at]=count
  for nth,changes in data['read_mutations'][at]:
   if nth==count:
    data['events'].append(('read_mutation',at,count,changes))
    for dst,replacement in changes:put(uc,dst,replacement)
 u.hook_add(UC_HOOK_MEM_READ,read,state)
 if not compiled:
  def visit(uc,pc,size,data):
   if not data['collect']:return
   hits.add(pc)
   if last[0] in branches:edges.add((last[0],pc))
   last[0]=pc
  u.hook_add(UC_HOOK_CODE,visit,state,begin=entry,end=0x41cfbc)
 def child(uc,pc,size,data):
  if not data['collect']:return
  token=data['entries'][pc];sp=uc.reg_read(UC_X86_REG_ESP)
  args=[get(uc,sp+4+i*4) for i in range(arity[token])]
  if token==0x4b6ff0:args=[args[0]&65535,*args[1:3],args[3]&65535,args[4]&65535]
  if token==0x4b4800:args=[args[0]&255,*args[1:4],args[4]&255]
  if token==0x4ec410:args[1]&=255
  count=data['counts'].get(token,0)+1;data['counts'][token]=count
  data['events'].append((token,tuple(args)))
  for dst,replacement in data['cfg'].get('child_mutations',{}).get((token,count),[]):put(uc,dst,replacement)
  if token not in data['controlled']:return
  values=data['cfg'].get('results',{}).get(token,[10] if token==0x4b6ff0 else list(range(1,9)) if token==0x4b4800 else [0])
  value=values[min(count-1,len(values)-1)]
  uc.reg_write(UC_X86_REG_EAX,value&0xffffffff);uc.reg_write(UC_X86_REG_ESP,sp+4);uc.reg_write(UC_X86_REG_EIP,get(uc,sp))
 state['entries']={}
 for token in arity:
  pc=token if not compiled else sym(names[token]);state['entries'][pc]=token
  u.hook_add(UC_HOOK_CODE,child,state,begin=pc,end=pc)

def setup(u,state,cfg):
 state.update(collect=False,cfg=cfg,writes=[],events=[],faults=[],counts={},controlled=set(cfg.get('controlled',[])),read_mutations=cfg.get('read_mutations',{}),read_counts={})
 state['hooks'].clear();state['calls'].clear();last[0]=None
 restore_seed(u);u.mem_write(S,b'\xa5'*0x50000);u.mem_write(0x30000000,b'\xcc'*0x10000)
 u.mem_write(POOL,bytes(2000*388));u.mem_write(GROUP,bytes(7200));u.mem_write(0xb350dc,bytes(51984*2));u.mem_write(0xaf598c,struct.pack('<I',cfg.get('terrain',0))*51984)
 if cfg.get('group_busy'):
  for i in range(10,50):byte(u,GROUP+i*144,1)
 if cfg.get('actor_busy'):
  for i in range(1,2000):byte(u,0xbcd606+i*388,1)
 for at,value in [(0xbca2c8,cfg.get('x',20)),(0xbca2cc,cfg.get('y',21)),(0xbca2d8,cfg.get('base',1000)),(0xbca2d0,228),(0xbca2d4,228),(0xbca2dc,0),(0xbca2e0,0),(0xbcd5f0,0),(0xbcd5f4,0),(0xd056fc,123),(0xd056e0,127),(0x604950,1),(0xeaac28,1),(0xf5653c,1000),(0xf55b88,0),(0x77aefc,0),(HISTORY,cfg.get('history',0)),(RNGA,cfg.get('first',0x12345678)),(RNGB,cfg.get('second',0x3456789a)),(0xd3b374,0x13572468),(0x784cb0,cfg.get('scene',0)),(0x605a3c,cfg.get('game_mode',0)),(0x5e4cd4,0)]:put(u,at,value)
 byte(u,0xe38dda,cfg.get('enabled',1));word(u,0x784b94,cfg.get('variant',0))
 for at,value in cfg.get('stores',[]):put(u,at,value)

def invoke(compiled,u,route,cfg):
 for reg,value in zip(registers,[cfg.get('eax',0x11223344),0x22222222,0x33333333,saved[0],SP,*saved[1:]]):u.reg_write(reg,value)
 u.reg_write(UC_X86_REG_EFLAGS,2);put(u,SP,STOP)
 if route=='action':put(u,SP+4,0x41cd00)
 pc=entry if not compiled else sym({'named':'restore_actor_command_spawn_hippos','legacy':'restore_actor_command_0041cd00','action':'restore_actor_command_action'}[route])
 try:u.emu_start(pc,STOP,count=50000000)
 except UcError:return 'fault'
 assert u.reg_read(UC_X86_REG_EIP)==STOP,(route,hex(u.reg_read(UC_X86_REG_EIP)))
 assert u.reg_read(UC_X86_REG_ESP)==SP+4 and [u.reg_read(registers[i]) for i in [3,5,6,7]]==saved
 if route=='action':assert u.reg_read(UC_X86_REG_EAX)==1
 return 'return'

def exercise(**cfg):
 global checked,invocations,actual_cases,protocol_cases,synthetic_cases
 expected=None
 if '--quick' in sys.argv:print('start',cfg,flush=True)
 for compiled,u,state in machines:
  for route in ['native'] if not compiled else ['named','legacy','action']:
   setup(u,state,cfg);state['collect']=True;result=invoke(compiled,u,route,cfg);state['collect']=False;invocations+=1
   if cfg.get('expect_fault'):assert result=='fault' and state['faults'],(cfg,route,state['faults'])
   else:
    assert result=='return' and not state['faults'],(cfg,route,state['faults'])
    if cfg.get('enabled',1)&255==1 and not cfg.get('group_busy') and not cfg.get('controlled'):
     assert state['counts'].get(0x4b4800)==8 and state['counts'].get(0x4ec990)==8,(cfg,route,state['counts'])
   current=(result,tuple(state['writes']),tuple(state['events']),snapshot(u),bytes(u.mem_read(S,0x48000)),tuple(state['faults']))
   if expected is None:expected=current
   elif expected!=current:
    changes=[]
    for part in [1,2]:
     if expected[part]!=current[part]:changes.append((part,len(expected[part]),len(current[part]),next(((n,a,b) for n,(a,b) in enumerate(zip(expected[part],current[part])) if a!=b),None)))
    memory=[next(((hex(base+n),a,b) for n,(a,b) in enumerate(zip(x,y)) if a!=b),None) for base,x,y in [(0x560000,expected[3],current[3]),(S,expected[4],current[4])]]
    raise AssertionError((cfg,route,changes,memory,expected[-1],current[-1]))
 checked+=1
 if cfg.get('controlled'):protocol_cases+=1
 elif cfg.get('read_mutations') or cfg.get('child_mutations'):synthetic_cases+=1
 else:actual_cases+=1
 if checked%20==0:print('progress',checked,flush=True)

if __name__=='__main__':
 all_children=list(arity)
 exercise(enabled=0);exercise();exercise(controlled=all_children)
 if '--quick' in sys.argv:print('PASS quick Hippo Stomp',checked,invocations);sys.exit(0)
 for enabled in [2,255,256]:exercise(enabled=enabled)
 exercise(group_busy=True);exercise(actor_busy=True)
 for terrain,scene,variant in itertools.product([0,4,0x80004,0xffffffff],[0,1,2],[0,1]):exercise(terrain=terrain,scene=scene,variant=variant)
 for x,y in [(0,0),(1,1),(227,227)]:exercise(x=x,y=y)
 exercise(history=98)
 for first,second,history in itertools.product([0,1,0xffffffff,0x12345678],[0,1,0xffffffff],[0,99,100,110,111,0x7fffffff,0x80000000,0xffffffff]):exercise(controlled=all_children,first=first,second=second,history=history)
 for result in [0,0xffffffff,0x80000000,1,10,49,0x4000000a]:exercise(controlled=all_children,results={0x4b6ff0:[result]})
 for result in [0,1,0x100,0x101,0xffffffff,0x80000000]:exercise(controlled=all_children,results={0x4ec410:[result]})
 for first_zero in range(8):exercise(controlled=all_children,results={0x4ec410:[1]*first_zero+[0]})
 for results in [[0],[1],[0xffffffff],[0x80000000],[0,1,0,2,0,3,0,4],[0x40000002]]:exercise(controlled=all_children,results={0x4b4800:results})
 exercise(controlled=all_children,results={0x4b6ff0:[0x1000000]},expect_fault=True)
 exercise(controlled=all_children,results={0x4b4800:[0x1000000]},expect_fault=True)
 exercise(controlled=all_children,x=0x40000000,y=0xc0000000)
 exercise(controlled=all_children,stores=[(0x5cd978,0xffffffff),(0x5cd97c,0x80000000)])
 inverse=pow(97,-1,1<<30)
 for field,destination in [(0xbcd658,RNGA),(0xbcd630,0xd3b374),(0xbcd73c,RNGB)]:
  actor=((((destination-field)&0xffffffff)//4)*inverse)%(1<<30)
  exercise(controlled=all_children,results={0x4b4800:[actor]})
 exercise(child_mutations={(0x4ec410,1):[(0xbca2c8,22),(0xbca2cc,23)]})
 exercise(read_mutations={0x5cd984:[(1,[(0xbca2cc,99),(0xbca2c8,22)])]})
 exercise(read_mutations={0x5cd97c:[(1,[(0xbca2d8,2000)])]})
 missing=points-hits;missing_edges={(a,b) for a,targets in branches.items() for b in targets}-edges
 print('Compared Hippo Stomp:',checked,'scenarios;',invocations,'invocations;',len(points&hits),'/',len(points),'instructions;',len(edges),'conditional outcomes;',actual_cases,'actual;',protocol_cases,'protocol;',synthetic_cases,'synthetic RAM',flush=True)
 print('Unexecuted:',[hex(a) for a in sorted(missing)],'edges:',[(hex(a),hex(b)) for a,b in sorted(missing_edges)],flush=True)
 assert not missing and not missing_edges,(missing,missing_edges)
 audit=json.loads((ROOT/'metadata/actor-spawn-hippos-caller-audit.json').read_text());assert audit['passed'] and audit['void_supported_for_observed_UI_callers']
 for p,sha in audit['source_hashes'].items():assert hashlib.sha256((ROOT/p).read_bytes()).hexdigest()==sha,('stale caller audit',p)
 sources=['CMakeLists.txt','direct_c/verify_actor_spawn_hippos.py','direct_c/audit_actor_spawn_hippos.py','direct_c/verify_options_startup.py','direct_c/native_test_pe.py','metadata/actor-spawn-hippos-native.json','metadata/actor-spawn-hippos-caller-audit.json','metadata/actor-command-nullary-next-rea-record.json',*audit['source_hashes']]
 for directory in ['src','runtime','include','generated']:
  sources += [p.relative_to(ROOT).as_posix() for p in (ROOT/directory).rglob('*') if p.is_file() and p.suffix in ['.c','.h','.asm','.inc']]
 proof=dict(passed=True,native_sha256=catalogue['native_sha256'],compiled_C_path=str(target.resolve()),compiled_C_sha256=target_sha,checked=checked,invocations=invocations,coverage={hex(entry):dict(instructions=len(points),executed=len(points&hits))},conditional_edges=len(edges),unexecuted_instructions=[],unexecuted_edges=[],actual_scenarios=actual_cases,protocol_scenarios=protocol_cases,synthetic_memory_scenarios=synthetic_cases,controlled_algorithm_endpoints_by_lane=dict(actual=[],protocol=['explicit group, actor, terrain and seek results']),controlled_platform_endpoints=[],source_hashes={p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest() for p in sources},limitations=['Whole 195-instruction native action; original, named C, legacy alias and real action facade are compared. No game is launched.','Full original data/private bytes, ordered stores, meaningful child arguments, action handled result, normal-return nonvolatile ABI and matching explicit faults are compared. Native callback incidental EAX/flags and fault CPU state are not certified.','Explicit child contracts and synthetic RAM cases are separated from actual algorithms. Arbitrary stack aliases, private-stack corruption, OS exception dispatch and complete gameplay remain unknown.'])
 suffix='diagnostic' if target.stem=='restore_link_check' else 'game';(ROOT/f'metadata/actor-spawn-hippos-{suffix}.json').write_text(json.dumps(proof,indent=2)+'\n')
