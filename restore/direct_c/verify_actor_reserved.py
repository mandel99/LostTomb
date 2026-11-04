"""Reserved actor initialization and its real reset/allocation/fill closure.

Parent-protocol cases explicitly control game-child contracts. Genuine cases
execute every reached game child; synthetic RAM mutations are labeled separately.
No original/restored host game or platform service is launched.
"""
from verify_options_startup import *
from unicorn import UC_HOOK_MEM_WRITE,UC_HOOK_MEM_READ,UC_HOOK_MEM_INVALID,UcError
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import json
catalogue=json.loads((ROOT/'metadata/actor-reserved-native.json').read_text())
evidence={e['evidence_id']:e for e in json.loads((ROOT/'metadata/actor-reserved-rea-evidence.json').read_text())['records']}
evidence.update({e['evidence_id']:e for e in json.loads((ROOT/'metadata/actor-reserved-iterator-rea-evidence.json').read_text())['records']})
owned={};branches={};hits=set();edges=set();previous=[None]
checked=invocations=actual_cases=protocol_cases=synthetic_cases=0
for f in catalogue['functions']:
 a=int(f['address'],16);raw=bytes.fromhex(f['bytes_hex']);ev=evidence[f['bytes_evidence_id']]
 assert ev['authority']=='shipped-artifact' and ev['subject']['digest']['sha256']==catalogue['native_sha256']
 assert ev['normalized_result']['complete'] and ev['normalized_result']['bytes_hex']==f['bytes_hex']
 assert raw==bytes(images[0].data[a-images[0].base:a-images[0].base+len(raw)])
 code=list(Cs(CS_ARCH_X86,CS_MODE_32).disasm(raw,a));owned[a]={i.address for i in code};assert len(code)==f['instruction_count']
 for i in code:
  if i.mnemonic.startswith('j') and i.mnemonic!='jmp':branches[i.address]={int(i.op_str,16),i.address+i.size}
points=set.union(*owned.values());native_owner={pc:a for a,p in owned.items() for pc in p}
names={0x4bb090:'restore_actor_initialize_reserved',0x4b4760:'restore_actor_reset_pool',0x4b8c90:'restore_actor_reset_town_links',0x4b4800:'restore_actor_create',0x40f8f0:'restore_memory_fill_bytes',0x448a60:'restore_town_actor_links_00448a60',0x44bb90:'restore_town_next'}
arity={0x4bb090:0,0x4b4760:0,0x4b8c90:0,0x4b4800:5,0x40f8f0:3,0x448a60:0,0x4b47a0:1,0x4b5af0:1,0x4b52f0:1,0x444b30:1,0x562348:0,0x44bb90:2}
POOL,FLAG,STRIDE,TOWNS,PAGE=0xbcd5f8,0xbcd606,388,0x785d70,0x35000000
for c,u,state in machines:
 u.mem_map(PAGE,4096);state.update(compiled=c,collect=False)
 def write(uc,access,at,size,value,data):
  if data['collect'] and (0x560000<=at<0x126d000 or S<=at<S+0x50000 or PAGE<=at<PAGE+4096):data['writes'].append((at,size,value&((1<<(size*8))-1)))
 u.hook_add(UC_HOOK_MEM_WRITE,write,state)
 def read(uc,access,at,size,value,data):
  if not data['collect']:return
  if data['cfg'].get('block_on_read') and size==1 and FLAG+STRIDE<=at<FLAG+2000*STRIDE and (at-FLAG)%STRIDE==0 and bytes(uc.mem_read(at,1))==b'\0':
   byte(uc,at,1);data['events'].append(('read_block',at))
  if at in data['read_mutations']:
   n=data['read_counts'].get(at,0)+1;data['read_counts'][at]=n
   for nth,changes in data['read_mutations'][at]:
    if n==nth:
     data['events'].append(('read_mutation',at,n,changes))
     for destination,replacement in changes:put(uc,destination,replacement)
 u.hook_add(UC_HOOK_MEM_READ,read,state)
 def invalid(uc,access,at,size,value,data):data['faults'].append((access,at,size));return False
 u.hook_add(UC_HOOK_MEM_INVALID,invalid,state)
 if not c:
  def visit(uc,pc,size,data):
   if not data['collect'] or native_owner.get(pc) in data['controlled']:return
   hits.add(pc)
   if previous[0] in branches:edges.add((previous[0],pc))
   previous[0]=pc
  for p in owned.values():u.hook_add(UC_HOOK_CODE,visit,state,begin=min(p),end=max(p))
 def child(uc,pc,size,data):
  if not data['collect']:return
  token=data['entries'][pc]
  if token==data['fn']:return
  sp=uc.reg_read(UC_X86_REG_ESP);n=arity[token]
  args=[get(uc,sp+4+i*4) for i in range(n)]
  if token==0x448a60:args=[get(uc,sp+4) if data['compiled'] else uc.reg_read(UC_X86_REG_ECX)]
  if token==0x44bb90:
   if data['compiled']:args=[get(uc,sp+4),get(uc,sp+8),get(uc,sp+12)&255]
   else:args=[uc.reg_read(UC_X86_REG_ECX),args[0],args[1]&255]
  if token==0x4b4800:args=[args[0]&255,*args[1:4],args[4]&255]
  if token==0x40f8f0:args[2]&=255
  data['counts'][token]=data['counts'].get(token,0)+1;count=data['counts'][token]
  data['events'].append((token,tuple(args)))
  if token not in data['controlled']:return
  results=data['cfg'].get('allocation_results',[1,2,3,4,5])
  value=results[(count-1)%len(results)] if token==0x4b4800 else 0
  cleanup=8 if token==0x44bb90 and not data['compiled'] else 0
  uc.reg_write(UC_X86_REG_EAX,value&0xffffffff);uc.reg_write(UC_X86_REG_ESP,sp+4+cleanup);uc.reg_write(UC_X86_REG_EIP,get(uc,sp))
 state['entries']={}
 for token in arity:
  if c:
   name=names[token] if token==0x448a60 else 'restore_town_next' if token==0x44bb90 else 'FUN_%08x'%token
   pc=sym(name)
  else:pc=token
  state['entries'][pc]=token;u.hook_add(UC_HOOK_CODE,child,state,begin=pc,end=pc)
def setup(c,u,state,fn,cfg):
 u.mem_write(0x560000,seed);u.mem_write(S,b'\xa5'*0x50000);u.mem_write(PAGE,b'\x6b'*4096);u.mem_write(0x30000000,b'\xcc'*0x10000)
 state.update(collect=False,fn=fn,cfg=cfg,writes=[],events=[],faults=[],counts={},read_counts={},read_mutations=cfg.get('read_mutations',{}),controlled=set(cfg.get('controlled',[])))
 if cfg.get('protocol'):state['controlled'].update([0x4b4760,0x4b8c90,0x4b4800])
 state['hooks'].clear();state['calls'].clear()
 pool=bytearray([cfg.get('poison',0xa5)])*(2000*STRIDE)
 for i in range(1,2000):pool[14+i*STRIDE]=1 if i<=cfg.get('busy',0) else 0
 u.mem_write(POOL,bytes(pool));u.mem_write(TOWNS,bytes(18628));u.mem_write(0xeaaf68,bytes(0x2bc0));u.mem_write(0x9784d8,bytes(0x898))
 for at,value in [(0xbcd5f0,cfg.get('highest',0)),(0xbcd5f4,0xaabbccdd),(0xd056fc,cfg.get('serial',123)),(0xd056e0,127),(0x604950,cfg.get('seed',1)),(0xbca2d8,cfg.get('base',1000)),(0xbca2d0,228),(0xbca2d4,228),(0xbca2dc,0),(0xbca2e0,0),(0xeaac28,1),(0xf5653c,1000),(0xf55b88,0),(0x77aefc,cfg.get('history',0))]:put(u,at,value)
 count=cfg.get('towns',0);put(u,TOWNS,count)
 towns=bytearray(18600)
 for i in range(150):
  offset=i*124;towns[offset:offset+2]=struct.pack('<H',i)
  towns[offset+2]=cfg.get('town_kind',1) if i<max(0,min(count,150)) else 0
  towns[offset+0x28]=cfg.get('town_flags',1)
 towns[140*124+2]=2
 u.mem_write(TOWNS+4,bytes(towns))
 if cfg.get('formation_block'):
  byte(u,0x9784d8,1);put(u,0x9784dc,cfg['formation_block'])
 for at,value in cfg.get('initial_stores',[]):put(u,at,value)
 if fn==0x4b4800:args=[cfg.get('player',1),cfg.get('kind',36),cfg.get('x',-1),cfg.get('y',-1),cfg.get('direction',0)]
 elif fn==0x44bb90:
  prev=0 if cfg.get('previous') is None else S+0x20000
  if prev:word(u,prev,cfg['previous'])
  args=[prev,cfg.get('create',1)]
 elif fn==0x40f8f0:args=[cfg.get('destination',PAGE+128+cfg.get('alignment',0)),cfg.get('size',19),cfg.get('value',0xa5)]
 else:args=[]
 state['receiver']=cfg.get('receiver',TOWNS if fn==0x44bb90 else S+0x1000+cfg.get('alignment',0));previous[0]=None
 if fn==0x44bb90 and state['receiver']!=TOWNS:u.mem_write(state['receiver'],bytes(u.mem_read(TOWNS,18628)))
 return args
def invoke(c,u,state,fn,args,route):
 receiver=state['receiver'];argv=[receiver,*args] if c and ((fn==0x448a60 and route=='named') or (fn==0x44bb90 and route=='named')) else args
 for reg,value in zip(registers,[0xa5b6c7d8,receiver,0x33333333,saved[0],SP,*saved[1:]]):u.reg_write(reg,value)
 u.reg_write(UC_X86_REG_EFLAGS,2);put(u,SP,STOP)
 for i,value in enumerate(argv):put(u,SP+4+i*4,value)
 pc=fn if not c else sym(names[fn] if route=='named' else 'FUN_%08x'%fn)
 try:u.emu_start(pc,STOP,count=50000000)
 except UcError:return ('fault',)
 assert u.reg_read(UC_X86_REG_EIP)==STOP,(hex(fn),route,hex(u.reg_read(UC_X86_REG_EIP)))
 assert u.reg_read(UC_X86_REG_ESP)==SP+4+(8 if not c and fn==0x44bb90 else 0) and [u.reg_read(registers[i]) for i in [3,5,6,7]]==saved
 return u.reg_read(UC_X86_REG_EAX) if fn in [0x4b4800,0x44bb90] else None
def exercise(fn=0x4bb090,**cfg):
 global checked,invocations,actual_cases,protocol_cases,synthetic_cases
 expected=None
 if '--quick' in sys.argv:print('start',hex(fn),cfg,flush=True)
 for c,u,state in machines:
  for route in ['native'] if not c else ['named'] if fn==0x44bb90 and cfg.get('receiver',TOWNS)!=TOWNS else ['named','canonical']:
   args=setup(c,u,state,fn,cfg);state['collect']=True;invocations+=1;result=invoke(c,u,state,fn,args,route);state['collect']=False
   if cfg.get('expect_failure'):
    assert state['faults'] and result==('fault',),(fn,cfg,route,state['faults'])
   else:assert not state['faults'] and result!=('fault',),(hex(fn),cfg,route,state['faults'])
   if fn==0x4bb090 and not cfg.get('expect_failure'):
    assert state['counts'].get(0x4b4800)==41,(cfg,route,state['counts'])
    if not state['controlled'] and not cfg.get('block_on_read'):
     for i,tag in enumerate(catalogue['expected_tags'],1):
      expected_tag=tag or 0;assert bytes(u.mem_read(POOL+i*STRIDE+11,1))[0]==expected_tag,(cfg,route,i,tag)
   current=(result,tuple(state['writes']),tuple(state['events']),snapshot(u),bytes(u.mem_read(S,0x50000)),bytes(u.mem_read(PAGE,4096)),tuple(state['faults']))
   if expected is None:expected=current
   elif current!=expected:
    diffs=[(i,next(((n,a,b) for n,(a,b) in enumerate(zip(x,y)) if a!=b),None),len(x),len(y)) for i,(x,y) in enumerate(zip(expected[1:3],current[1:3])) if x!=y]
    memory=[next(((hex(base+i),a,b) for i,(a,b) in enumerate(zip(x,y)) if a!=b),None) for base,x,y in [(0x560000,expected[3],current[3]),(S,expected[4],current[4]),(PAGE,expected[5],current[5])]]
    raise AssertionError((hex(fn),cfg,route,'return',expected[0],current[0],'events',diffs,'memory',memory,'faults',expected[-1],current[-1]))
 checked+=1
 if cfg.get('protocol') or cfg.get('controlled'):protocol_cases+=1
 elif cfg.get('block_on_read') or cfg.get('read_mutations'):synthetic_cases+=1
 else:actual_cases+=1
 if checked%20==0:print('progress',checked,hex(fn),flush=True)
if __name__=='__main__':
 exercise(protocol=True,allocation_results=list(range(1,42)))
 exercise(0x40f8f0);exercise(0x448a60);exercise(0x4b4760);exercise(0x4b8c90,towns=3)
 exercise(0x4b4800);exercise(towns=2);exercise(0x44bb90,towns=3)
 if '--quick' in sys.argv:print('PASS quick',checked,invocations);sys.exit(0)
 for results in [[0],[1],[0xffffffff],[0x80000000],[1,0,2,0,3],[3,2,1,0],[2,2,2,2],list(range(1,42))]:exercise(protocol=True,allocation_results=results)
 for ordinal in range(41):
  values=list(range(1,42));values[ordinal]=0;exercise(protocol=True,allocation_results=values)
 exercise(protocol=True,allocation_results=[0x01000000],expect_failure=True)
 for count,kind,prev,create in itertools.product([-1,0,1,3,150],[1,2],[None,-1,0,139,140,149,32767],[0,1,255]):exercise(0x44bb90,towns=count,town_kind=kind,previous=prev,create=create)
 exercise(0x44bb90,towns=3,town_kind=1,read_mutations={TOWNS+6:[(1,[(TOWNS,0)])]})
 exercise(0x44bb90,towns=-1,previous=-32768,create=255,receiver=S+0x1000,expect_failure=True)
 exercise(0x44bb90,towns=3,town_kind=2,receiver=S+0x1000)
 for poison in [0,0x5a,0xff]:exercise(0x4b4760,poison=poison)
 for count,kind,flags in itertools.product([-1,0,1,3,150],[0,1,2,3,23],[0,1,5]):exercise(0x4b8c90,towns=count,town_kind=kind,town_flags=flags)
 # The initial null guard is exercised as an explicit child-contract lane;
 # ordinary bounded town lists always yield the reserved record on this call.
 exercise(0x4b8c90,controlled=[0x44bb90])
 for alignment in range(4):exercise(0x448a60,alignment=alignment)
 exercise(0x448a60,receiver=0,expect_failure=True)
 for value in range(256):exercise(0x40f8f0,value=value,alignment=value&3)
 for value,alignment in itertools.product([0,1,0x55,0x7f,0x80,0xa5,0xff],range(4)):exercise(0x40f8f0,value=value,alignment=alignment)
 for size,alignment in itertools.product([0,1,2,3,4,5,7,8,15,16,20,31,32,388],range(4)):exercise(0x40f8f0,size=size,alignment=alignment,value=0x1234ff80)
 exercise(0x40f8f0,destination=0,size=0);exercise(0x40f8f0,destination=0,size=1,expect_failure=True)
 exercise(0x40f8f0,destination=PAGE+4092,size=0xffffffff,expect_failure=True);exercise(0x40f8f0,destination=PAGE+4095,size=4,expect_failure=True)
 for busy,highest,kind in itertools.product([0,1,17,1998,1999],[0,1,1999,0xffffffff],[0,6,19,20,36]):exercise(0x4b4800,busy=busy,highest=highest,kind=kind,controlled=[0x4b5af0,0x4b52f0,0x444b30])
 for player,direction in itertools.product([0,1,255,0x12345678],[0,7,255,0xffffff80]):exercise(0x4b4800,player=player,direction=direction,kind=36,controlled=[0x4b5af0,0x4b52f0])
 for x,y in [(-32768,32767),(0x12345678,0x87654321),(0x7fffffff,0x80000000)]:exercise(0x4b4800,x=x,y=y,controlled=[0x4b5af0,0x4b52f0])
 exercise(0x4b4800,formation_block=1)
 for rng_seed,serial in [(0,0),(1,0xffffffff),(0xffffffff,0x1234ffff)]:exercise(seed=rng_seed,serial=serial,towns=3)
 exercise(block_on_read=True,towns=2)
 missing=points-hits;missing_edges={(a,b) for a,targets in branches.items() for b in targets} - edges
 print('Compared reserved actors:',checked,'scenarios;',invocations,'invocations;',len(points&hits),'/',len(points),'instructions;',len(edges),'conditional outcomes;',actual_cases,'actual;',protocol_cases,'protocol;',synthetic_cases,'synthetic RAM cases',flush=True)
 print('Unexecuted:',[hex(a) for a in sorted(missing)],'edges:',[(hex(a),hex(b)) for a,b in sorted(missing_edges)],flush=True)
 assert not missing and not missing_edges,(missing,missing_edges)
 sources=['CMakeLists.txt','direct_c/verify_actor_reserved.py','direct_c/verify_options_startup.py','direct_c/native_test_pe.py','direct_c/audit_actor_reserved_callers.py','metadata/actor-reserved-native.json','metadata/actor-reserved-rea-evidence.json','metadata/actor-reserved-iterator-evidence.json','metadata/actor-reserved-iterator-rea-evidence.json','metadata/actor-reserved-caller-native.json','metadata/actor-reserved-caller-audit.json','tools/actor_reserved_project/CMakeLists.txt']
 for directory in ['src','runtime','include','generated']:
  sources += [p.relative_to(ROOT).as_posix() for p in (ROOT/directory).rglob('*') if p.is_file() and p.suffix in ['.c','.h','.asm','.inc']]
 proof=dict(passed=True,native_sha256=catalogue['native_sha256'],compiled_C_path=str(target.resolve()),compiled_C_sha256=target_sha,checked=checked,invocations=invocations,coverage={hex(a):dict(instructions=len(p),executed=len(p&hits)) for a,p in owned.items()},conditional_edges=len(edges),unexecuted_instructions=[],unexecuted_edges=[],actual_scenarios=actual_cases,protocol_scenarios=protocol_cases,synthetic_memory_scenarios=synthetic_cases,controlled_algorithm_endpoints_by_lane=dict(parent_protocol=['004b4760','004b8c90','004b4800'],allocator_protocol=['004b5af0','004b52f0','00444b30'],town_reset_protocol=['0044bb90'],actual=[]),controlled_platform_endpoints=[],source_hashes={p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest() for p in sources},limitations=['Seven whole owned bodies execute. Parent protocol and allocator protocol cases explicitly control their declared game-child contracts; actual cases execute all reached game algorithms without replacement.','Full original static/private/guarded bytes, ordered CPU stores, meaningful child arguments and allocator EAX, normal-return ABI and matching explicit faults are compared. Void incidental GPRs/flags and fault PC/register state are not certified.','One read-mutated occupied-pool composition executes all actual algorithms but represents synthetic RAM changes, not proved physical concurrency. Original OS/device/heap behavior and arbitrary caller-stack corruption remain outside the fixture.'])
 suffix='diagnostic' if target.stem=='restore_link_check' else 'game';(ROOT/f'metadata/actor-reserved-{suffix}.json').write_text(json.dumps(proof,indent=2)+'\n')
