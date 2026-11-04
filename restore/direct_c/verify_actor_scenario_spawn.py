"""Whole native/C scenario points, grouped spawning, group allocation and terrain.

Explicit child-contract lanes and actual compositions are recorded separately.
No original/restored host game, platform API or device is launched.
"""
from verify_options_startup import *
from unicorn import UC_HOOK_MEM_WRITE,UC_HOOK_MEM_READ,UC_HOOK_MEM_INVALID,UcError
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import json

catalogue=json.loads((ROOT/'metadata/actor-scenario-spawn-native.json').read_text())
evidence={e['evidence_id']:e for e in json.loads((ROOT/'metadata/actor-scenario-spawn-rea-evidence.json').read_text())['records']}
owned={};branches={};hits=set();edges=set();last=[None]
checked=invocations=actual_cases=protocol_cases=synthetic_cases=0
for f in catalogue['functions']:
 a=int(f['address'],16);raw=bytes.fromhex(f['bytes_hex']);ev=evidence[f['bytes_evidence_id']]
 assert ev['subject']['digest']['sha256']==catalogue['native_sha256'] and ev['authority']=='shipped-artifact'
 assert ev['normalized_result']['complete'] and ev['normalized_result']['bytes_hex']==f['bytes_hex']
 assert raw==bytes(images[0].data[a-images[0].base:a-images[0].base+len(raw)])
 code=list(Cs(CS_ARCH_X86,CS_MODE_32).disasm(raw,a));owned[a]={i.address for i in code};assert len(code)==f['instruction_count']
 for i in code:
  if i.mnemonic.startswith('j') and i.mnemonic!='jmp':branches[i.address]={int(i.op_str,16),i.address+i.size}
points=set.union(*owned.values());owner={pc:a for a,pcs in owned.items() for pc in pcs}
names={0x4bb3f0:'restore_actor_initialize_scenario_groups',0x4bbc10:'restore_actor_initialize_scenario_points',0x4b6ff0:'restore_actor_group_allocate'}
arity={0x4b6ff0:5,0x4b4800:5,0x4ec410:2,0x562348:0,0x4b47a0:1,0x4b5af0:1,0x4b52f0:1,0x444b30:1}
POOL,GROUP,HISTORY,RNGA,RNGB=0xbcd5f8,0xbcb9c8,0xd3b1c4,0xd3b384,0xd3b380
for c,u,state in machines:
 state.update(compiled=c,collect=False)
 def write(uc,access,at,size,value,data):
  if data['collect'] and (0x560000<=at<0x126d000 or S<=at<S+0x50000):data['writes'].append((at,size,value&((1<<(size*8))-1)))
 u.hook_add(UC_HOOK_MEM_WRITE,write,state)
 def invalid(uc,access,at,size,value,data):data['faults'].append((access,at,size));return False
 u.hook_add(UC_HOOK_MEM_INVALID,invalid,state)
 def read(uc,access,at,size,value,data):
  if not data['collect'] or at not in data['read_mutations']:return
  n=data['read_counts'].get(at,0)+1;data['read_counts'][at]=n
  for nth,changes in data['read_mutations'][at]:
   if n==nth:
    data['events'].append(('read_mutation',at,n,changes))
    for destination,replacement in changes:put(uc,destination,replacement)
 u.hook_add(UC_HOOK_MEM_READ,read,state)
 if not c:
  def visit(uc,pc,size,data):
   if not data['collect'] or owner.get(pc) in data['controlled']:return
   hits.add(pc)
   if last[0] in branches:edges.add((last[0],pc))
   last[0]=pc
  for pcs in owned.values():u.hook_add(UC_HOOK_CODE,visit,state,begin=min(pcs),end=max(pcs))
 def child(uc,pc,size,data):
  if not data['collect']:return
  token=data['entries'][pc]
  if token==data['fn']:return
  sp=uc.reg_read(UC_X86_REG_ESP);args=[get(uc,sp+4+i*4) for i in range(arity[token])]
  if token==0x4b6ff0:args=[args[0]&65535,*args[1:3],args[3]&65535,args[4]&65535]
  if token==0x4b4800:args=[args[0]&255,*args[1:4],args[4]&255]
  if token==0x4ec410:args[1]&=255
  data['counts'][token]=data['counts'].get(token,0)+1;n=data['counts'][token]
  data['events'].append((token,tuple(args)))
  for destination,replacement in data['cfg'].get('child_mutations',{}).get((token,n),[]):put(uc,destination,replacement)
  if token not in data['controlled']:return
  key={0x4b6ff0:'group_results',0x4b4800:'actor_results',0x4ec410:'terrain_results',0x562348:'random_results'}.get(token)
  values=data['cfg'].get(key,[10] if token==0x4b6ff0 else [1])
  value=values[(n-1)%len(values)]
  uc.reg_write(UC_X86_REG_EAX,value&0xffffffff);uc.reg_write(UC_X86_REG_ESP,sp+4);uc.reg_write(UC_X86_REG_EIP,get(uc,sp))
 state['entries']={}
 for token in arity:
  pc=token if not c else sym('FUN_%08x'%token)
  state['entries'][pc]=token;u.hook_add(UC_HOOK_CODE,child,state,begin=pc,end=pc)

def setup(c,u,state,fn,cfg):
 state.update(collect=False,fn=fn,cfg=cfg,writes=[],events=[],faults=[],counts={},read_counts={},read_mutations=cfg.get('read_mutations',{}),controlled=set(cfg.get('controlled',[])))
 state['hooks'].clear();state['calls'].clear();last[0]=None
 u.mem_write(0x560000,seed);u.mem_write(S,b'\xa5'*0x50000);u.mem_write(0x30000000,b'\xcc'*0x10000)
 pool=bytearray(b'\xa5'*(2000*388))
 for i in range(1,2000):pool[i*388+14]=1 if i<=cfg.get('busy_actors',0) else 0
 u.mem_write(POOL,bytes(pool));groups=bytearray(b'\xa5'*7200)
 for i in range(50):
  groups[i*144]=1 if 10<=i<10+cfg.get('busy_groups',0) else 0
  groups[i*144+105]=cfg.get('group_reserved',0)
 u.mem_write(GROUP,bytes(groups));u.mem_write(0xeaaf68,bytes(0x2bc0));u.mem_write(0x9784d8,bytes(0x898))
 u.mem_write(0x784b64,bytes(48));u.mem_write(0x784cc0,bytes(32))
 for at,value in [(0x605a3c,cfg.get('mode',0)),(0x784cb0,cfg.get('scene',0)),(HISTORY,cfg.get('history',0)),(RNGA,cfg.get('random_first',0x12345678)),(RNGB,cfg.get('random_second',0x3456789a)),(0xd3b374,0x13572468),(0x604950,cfg.get('lcg',1)),(0xbcd5f0,0),(0xbcd5f4,0),(0xd056fc,cfg.get('serial',123)),(0xd056e0,127),(0xbca2d8,1000),(0xbca2d0,228),(0xbca2d4,228),(0xbca2dc,0),(0xbca2e0,0),(0xeaac28,1),(0xf5653c,1000),(0xf55b88,0),(0x77aefc,0)]:put(u,at,value)
 word(u,0x784b94,cfg.get('variant',0))
 for base,key,values,width in [(0x784b64,'word_x',[20,0,0,0],2),(0x784b6c,'word_y',[21,0,0,0],2),(0x784b74,'point_x',[40,0,0,0,0,0,0,0],2),(0x784b84,'point_y',[41,0,0,0,0,0,0,0],2),(0x784cc0,'dword_x',[30,0,0,0],4),(0x784cd0,'dword_y',[31,0,0,0],4)]:
  for i,value in enumerate(cfg.get(key,values)):(word if width==2 else put)(u,base+i*width,value)
 scene=cfg.get('scene',0)&255;variant=cfg.get('variant',0)
 for key,at in [('count_dword',0x5e4c28+scene*48),('count_word',(0x5e4cb8+((variant&0xffff if variant>=0 else variant)+scene*2)*48)&0xffffffff),('kind_dword',0x5e4c20+scene*48),('kind_word',(0x5e4cb0+(variant+scene*2)*48)&0xffffffff)]:
  if key in cfg:put(u,at,cfg[key])
 if cfg.get('terrain_fill') is not None:u.mem_write(0xaf598c,struct.pack('<I',cfg['terrain_fill']&0xffffffff)*51984)
 for at,value in cfg.get('stores',[]):put(u,at,value)
 if fn==0x4b6ff0:return cfg.get('args',[84,9,0,20,21])
 if fn==0x4ec410:return [cfg.get('terrain',0),cfg.get('terrain_mode',0)]
 return []

def invoke(c,u,state,fn,args,route):
 for reg,value in zip(registers,[0xa5b6c7d8,0x22222222,0x33333333,saved[0],SP,*saved[1:]]):u.reg_write(reg,value)
 u.reg_write(UC_X86_REG_EFLAGS,2);put(u,SP,STOP)
 for i,value in enumerate(args):put(u,SP+4+i*4,value)
 entry=fn if not c else sym(names[fn] if route=='named' else 'FUN_%08x'%fn)
 try:u.emu_start(entry,STOP,count=50000000)
 except UcError:return ('fault',)
 assert u.reg_read(UC_X86_REG_EIP)==STOP,(hex(fn),route,hex(u.reg_read(UC_X86_REG_EIP)))
 assert u.reg_read(UC_X86_REG_ESP)==SP+4 and [u.reg_read(registers[i]) for i in [3,5,6,7]]==saved
 return u.reg_read(UC_X86_REG_EAX) if fn in [0x4b6ff0,0x4ec410] else None

def exercise(fn=0x4bb3f0,**cfg):
 global checked,invocations,actual_cases,protocol_cases,synthetic_cases
 expected=None
 if '--quick' in sys.argv:print('start',hex(fn),cfg,flush=True)
 for c,u,state in machines:
  for route in ['native'] if not c else ['named','canonical'] if fn in names else ['canonical']:
   args=setup(c,u,state,fn,cfg);state['collect']=True;invocations+=1;result=invoke(c,u,state,fn,args,route);state['collect']=False
   if cfg.get('expect_fault'):assert state['faults'] and result==('fault',),(cfg,route,state['faults'])
   else:assert not state['faults'] and result!=('fault',),(hex(fn),cfg,route,state['faults'])
   current=(result,tuple(state['writes']),tuple(state['events']),snapshot(u),bytes(u.mem_read(S,0x50000)),tuple(state['faults']))
   if expected is None:expected=current
   elif current!=expected:
    changes=[]
    for part in [1,2]:
     if current[part]!=expected[part]:changes.append((part,len(expected[part]),len(current[part]),next(((i,a,b) for i,(a,b) in enumerate(zip(expected[part],current[part])) if a!=b),None)))
    memory=[next(((hex(base+i),a,b) for i,(a,b) in enumerate(zip(x,y)) if a!=b),None) for base,x,y in [(0x560000,expected[3],current[3]),(S,expected[4],current[4])]]
    raise AssertionError((hex(fn),cfg,route,'return',expected[0],current[0],'trace',changes,'memory',memory,'faults',expected[-1],current[-1]))
 checked+=1
 if cfg.get('controlled'):protocol_cases+=1
 elif cfg.get('read_mutations') or cfg.get('child_mutations'):synthetic_cases+=1
 else:actual_cases+=1
 if checked%20==0:print('progress',checked,hex(fn),flush=True)

if __name__=='__main__':
 exercise(0x4b6ff0);exercise(0x4ec410)
 exercise(0x4bbc10);exercise()
 exercise(count_word=1,count_dword=1,controlled=[0x4b6ff0,0x4b4800,0x4ec410])
 if '--quick' in sys.argv:print('PASS quick',checked,invocations);sys.exit(0)
 for mode in [1,2,256,0xffffffff]:exercise(mode=mode);exercise(0x4bbc10,mode=mode)
 for x in [-32768,-1,0,1,32767]:exercise(0x4bbc10,point_x=[x]*8,controlled=[0x4b4800])
 for scene,variant in itertools.product([0,1,2],[0,1]):exercise(scene=scene,variant=variant)
 for first,second,history in itertools.product([0,1,0xffffffff,0x12345678],[0,1,0xffffffff],[0,98,99,100,110,111,0xffffffff]):
  exercise(0x4bbc10,random_first=first,random_second=second,history=history,controlled=[0x4b4800])
 for lane,count in itertools.product(['word','dword'],[-1,0,1,2,17]):
  exercise(**{f'count_{lane}':count,f'count_{"dword" if lane=="word" else "word"}':0},controlled=[0x4b6ff0,0x4b4800,0x4ec410])
 for results in [[0],[0,0,1],[0]*15+[1],[0x100],[0xffffffff]]:
  exercise(count_word=2,count_dword=2,terrain_results=results,controlled=[0x4b6ff0,0x4b4800,0x4ec410])
 for history in [0,98,99,100,110,111,0xffffffff,0x7fffffff,0x80000000]:
  exercise(count_word=2,count_dword=2,history=history,controlled=[0x4b6ff0,0x4b4800,0x4ec410])
 for history in [97,98,99]:exercise(count_word=0,count_dword=1,history=history,controlled=[0x4b6ff0,0x4b4800,0x4ec410])
 for results in [[0],[0xffffffff],[0x80000000],[0,10,0,11]]:
  exercise(word_x=[20,21,22,23],dword_x=[30,31,32,33],group_results=results,count_word=1,count_dword=1,controlled=[0x4b6ff0,0x4b4800,0x4ec410])
 for results in [[0],[0xffffffff],[0x80000000],[1,0,2]]:
  exercise(actor_results=results,count_word=2,count_dword=2,controlled=[0x4b6ff0,0x4b4800,0x4ec410])
  exercise(0x4bbc10,actor_results=results,controlled=[0x4b4800])
 for fn in [0x4bb3f0,0x4bbc10]:exercise(fn,actor_results=[0x1000000],count_word=1,count_dword=1,controlled=[0x4b6ff0,0x4b4800,0x4ec410],expect_fault=True)
 exercise(group_results=[0x1000000],controlled=[0x4b6ff0],expect_fault=True)
 exercise(0x4bbc10,point_x=[20]*8,point_y=[-32768,-1,0,1,32767,0x8000,0xffff,0x12345678],controlled=[0x4b4800])
 inverse=pow(97,-1,1<<30)
 for target_field,actor_field in [(0xd3b384,0xbcd658),(0xd3b374,0xbcd630),(0x784b64,0xbcd658)]:
  actor_alias=(((target_field-actor_field)&0xffffffff)//4*inverse)%(1<<30)
  exercise(actor_results=[actor_alias],count_word=1,count_dword=1,controlled=[0x4b6ff0,0x4b4800,0x4ec410])
 group_alias=(((0xd3b384-0xbcba04)&0xffffffff)//16*pow(9,-1,1<<28))%(1<<28)
 exercise(group_results=[group_alias],count_word=1,count_dword=1,controlled=[0x4b6ff0,0x4b4800,0x4ec410])
 exercise(busy_groups=40)
 exercise(busy_actors=1999)
 exercise(0x4bbc10,busy_actors=1999)
 exercise(count_word=1,count_dword=1,read_mutations={0x5e4cb8:[(1,[(0x605a3c,1)])]})
 exercise(word_x=[0,20,0,0],dword_x=[0]*4,count_word=1,stores=[(0x5e4cdc,273),(0x5e4cdc+192,546)],read_mutations={0x784b64:[(1,[(0x784cb0,2)])]})
 exercise(word_x=[20,21,0,0],dword_x=[0]*4,count_word=1,stores=[(0x5e4cdc,273),(0x5e4cdc+192,546)],child_mutations={(0x4b6ff0,1):[(0x784cb0,2)]})
 exercise(count_word=1,count_dword=1,child_mutations={(0x4b4800,1):[(0xd3b1c4,99),(RNGA,0xffffffff),(RNGB,0)]})
 exercise(0x4bbc10,point_x=[20,21,0,0,0,0,0,0],child_mutations={(0x4b4800,1):[(0x605a3c,1),(0x784b86,41)]})
 for busy,reserved,mode,orientation in itertools.product([0,1,39,40],[0,1],[0,9,10,0xffffffff],[0,1,4]):exercise(0x4b6ff0,busy_groups=busy,group_reserved=reserved,args=[0x12345678,mode,orientation,0xffff8000,0x12347fff])
 for terrain,flag,scene,variant,mode in itertools.product([0,1,4,0x40000,0x3038d022,0xffffffff],[0,1,255,256],[0,1,2],[0,1],[0,1]):exercise(0x4ec410,terrain=terrain,terrain_mode=flag,scene=scene,variant=variant,mode=mode)
 missing=points-hits;missing_edges={(a,b) for a,targets in branches.items() for b in targets}-edges
 print('Compared scenario spawning:',checked,'scenarios;',invocations,'invocations;',len(points&hits),'/',len(points),'instructions;',len(edges),'conditional outcomes;',actual_cases,'actual;',protocol_cases,'protocol;',synthetic_cases,'synthetic RAM',flush=True)
 print('Unexecuted:',[hex(a) for a in sorted(missing)],'edges:',[(hex(a),hex(b)) for a,b in sorted(missing_edges)],flush=True)
 assert not missing and not missing_edges,(missing,missing_edges)
 sources=['CMakeLists.txt','direct_c/verify_actor_scenario_spawn.py','direct_c/verify_options_startup.py','direct_c/native_test_pe.py','metadata/actor-scenario-spawn-native.json','metadata/actor-scenario-spawn-context.json','metadata/actor-scenario-spawn-rea-evidence.json']
 for directory in ['src','runtime','include','generated']:
  sources += [p.relative_to(ROOT).as_posix() for p in (ROOT/directory).rglob('*') if p.is_file() and p.suffix in ['.c','.h','.asm','.inc']]
 proof=dict(passed=True,native_sha256=catalogue['native_sha256'],compiled_C_path=str(target.resolve()),compiled_C_sha256=target_sha,checked=checked,invocations=invocations,coverage={hex(a):dict(instructions=len(p),executed=len(p&hits)) for a,p in owned.items()},conditional_edges=len(edges),unexecuted_instructions=[],unexecuted_edges=[],actual_scenarios=actual_cases,protocol_scenarios=protocol_cases,synthetic_memory_scenarios=synthetic_cases,controlled_platform_endpoints=[],source_hashes={p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest() for p in sources},limitations=['Four whole bodies; explicit child-contract cases may replace group/actor allocation or terrain returns. Actual cases execute all reached game algorithms and no platform endpoints.','Full original static/private bytes, ordered CPU stores, normalized meaningful child arguments, defined results, normal-return ABI and matching explicit memory faults are compared. Void incidental GPR/flags, fault PC/registers, caller-stack aliases and OS exception dispatch are not certified.','Read/call-mutated states are synthetic memory scenarios, not physical concurrency. Finite mapped fixtures do not prove arbitrary traversal, all scenario states or complete gameplay.'])
 suffix='diagnostic' if target.stem=='restore_link_check' else 'game';(ROOT/f'metadata/actor-scenario-spawn-{suffix}.json').write_text(json.dumps(proof,indent=2)+'\n')
