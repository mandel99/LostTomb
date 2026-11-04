"""Compare the actual actor command/search graph with pinned native bytes.

Ordinary lanes execute classifiers, tile lists, RNG and nearest-search bodies.
Explicit protocol lanes control child results. No game process is launched.
"""
from verify_options_startup import *
from unicorn import UC_HOOK_MEM_WRITE,UC_HOOK_MEM_READ,UC_HOOK_MEM_INVALID,UcError
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import json

catalogue=json.loads((ROOT/'metadata/actor-command-transition-native.json').read_text())
records=[]
for filename in ['actor-command-transition-next-rea-evidence.json','actor-command-transition-additions-rea-records.json','actor-command-transition-callers-rea-records.json']:
 records+=json.loads((ROOT/'metadata'/filename).read_text())['records']
evidence={e['evidence_id']:e for e in records}
owned={};branches={};hits=set();edges=set();last=[None]
for f in catalogue['functions']:
 a=int(f['address'],16);raw=bytes.fromhex(f['bytes_hex']);ev=evidence[f['bytes_evidence_id']]
 assert ev['authority']=='shipped-artifact' and ev['subject']['digest']['sha256']==catalogue['native_sha256']
 assert ev['normalized_result']['complete'] and ev['normalized_result']['bytes_hex']==f['bytes_hex']
 assert raw==bytes(images[0].data[a-images[0].base:a-images[0].base+len(raw)])
 code=list(Cs(CS_ARCH_X86,CS_MODE_32).disasm(raw,a));owned[a]={i.address for i in code};assert len(code)==f['instruction_count']
 for i in code:
  if i.mnemonic.startswith('j') and i.mnemonic!='jmp':branches[i.address]={int(i.op_str,16),i.address+i.size}
points=set.union(*owned.values());owner={pc:a for a,pcs in owned.items() for pc in pcs}
names={0x4ec990:'restore_actor_seek_other_kind',0x4ec7c0:'restore_actor_choose_command_state',0x4ec4a0:'restore_actor_apply_command_state',0x4ec0f0:'restore_actor_spiral_search',0x4eae20:'restore_actor_find_other_kind_on_tile'}
arity={0x4ec990:1,0x4ec7c0:1,0x4ec4a0:2,0x4ec0f0:7,0x4eae20:2,0x4ea9c0:1,0x4eada0:1,0x4eb800:9}
POOL,STRIDE,RNGA,RNGB,HISTORY,CALLBACK=0xbcd5f8,388,0xd3b384,0xd3b380,0xd3b1c4,S+0x49000
checked=invocations=actual_cases=protocol_cases=synthetic_cases=0

def restore_seed(u):
 # Exact reset, including modified code. Unchanged pages need no write/TB flush.
 current=bytes(u.mem_read(0x560000,len(seed)))
 for offset in range(0,len(seed),4096):
  page=seed[offset:offset+4096]
  if current[offset:offset+4096]!=page:u.mem_write(0x560000+offset,page)

for c,u,state in machines:
 state.update(compiled=c,collect=False)
 def write(uc,access,at,size,value,data):
  if data['collect'] and (0x560000<=at<0x126d000 or S<=at<S+0x48000):data['writes'].append((at,size,value&((1<<(size*8))-1)))
 u.hook_add(UC_HOOK_MEM_WRITE,write,state)
 def invalid(uc,access,at,size,value,data):data['faults'].append((access,at,size));return False
 u.hook_add(UC_HOOK_MEM_INVALID,invalid,state)
 def read(uc,access,at,size,value,data):
  if not data['collect'] or at not in data['read_mutations']:return
  n=data['read_counts'].get(at,0)+1;data['read_counts'][at]=n
  for nth,changes in data['read_mutations'][at]:
   if n==nth:
    data['events'].append(('read_mutation',at,n,changes))
    for dst,replacement in changes:put(uc,dst,replacement)
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
  token=data['entries'][pc];sp=uc.reg_read(UC_X86_REG_ESP)
  if token==data['fn'] and sp==SP:return
  args=[get(uc,sp+4+i*4) for i in range(2 if token==CALLBACK else arity[token])]
  data['counts'][token]=data['counts'].get(token,0)+1;n=data['counts'][token]
  if token in [0x4ea9c0,0x4eada0,CALLBACK]:data['events'].append((token,tuple(args)))
  if token==0x4eb800:data['events'].append((token,tuple(args[:5]),get(uc,args[5]),get(uc,args[6]),tuple(args[7:])))
  for dst,replacement in data['cfg'].get('child_mutations',{}).get((token,n),[]):put(uc,dst,replacement)
  if token!=CALLBACK and token not in data['controlled']:return
  if token==CALLBACK:
   values=data['cfg'].get('callback_results',[0]);result=values[min(n-1,len(values)-1)]
   for dst,replacement in data['cfg'].get('callback_mutations',{}).get(n,[]):put(uc,dst,replacement)
  else:
   values=data['cfg'].get('results',{}).get(token,[0]);result=values[min(n-1,len(values)-1)]
  if token==0x4eb800:
   for i,key in [(5,'nearest_x'),(6,'nearest_y')]:
    if key in data['cfg']:put(uc,args[i],data['cfg'][key])
  uc.reg_write(UC_X86_REG_EAX,result&0xffffffff);uc.reg_write(UC_X86_REG_ESP,sp+4);uc.reg_write(UC_X86_REG_EIP,get(uc,sp))
 state['entries']={}
 for token in [*arity,CALLBACK]:
  pc=token if not c or token==CALLBACK else sym(names[token] if token in names else 'restore_building_nearest_search' if token==0x4eb800 else 'FUN_%08x'%token)
  state['entries'][pc]=token;u.hook_add(UC_HOOK_CODE,child,state,begin=pc,end=pc)

def setup(c,u,state,fn,cfg):
 state.update(collect=False,fn=fn,cfg=cfg,writes=[],events=[],faults=[],counts={},controlled=set(cfg.get('controlled',[])),read_mutations=cfg.get('read_mutations',{}),read_counts={})
 state['hooks'].clear();state['calls'].clear();last[0]=None
 restore_seed(u);u.mem_write(S,b'\xa5'*0x50000);u.mem_write(0x30000000,b'\xcc'*0x10000)
 u.mem_write(POOL,bytes(2000*STRIDE));u.mem_write(0xb350dc,bytes(51984*2));u.mem_write(0xaf598c,struct.pack('<I',cfg.get('terrain',0))*51984)
 for at,value in [(0xbca2d0,cfg.get('width',228)),(0xbca2d4,cfg.get('height',228)),(0xbca2d8,cfg.get('base',1000)),(0xbc9768,0x12345678),(RNGA,cfg.get('first',0x12345678)),(RNGB,cfg.get('second',0x3456789a)),(HISTORY,cfg.get('history',0)),(0xd3b374,0x13572468),(0x784cb0,cfg.get('scene',0))]:put(u,at,value)
 word(u,0x784b94,cfg.get('variant',0));put(u,0x5e4cd4+(cfg.get('variant',0)+(cfg.get('scene',0)&255)*2)*48,cfg.get('radius',0))
 actor=cfg.get('actor',1);offset=(actor*STRIDE)&0xffffffff
 for at,value in [(0xbcd602,cfg.get('kind',82)),(0xbcd606,cfg.get('status',1)),(0xbcd630,cfg.get('state',8))]:byte(u,(at+offset)&0xffffffff,value)
 for at,value in [(0xbcd60c,cfg.get('actor_x',10)),(0xbcd60e,cfg.get('actor_y',11)),(0xbcd62e,1234)]:word(u,(at+offset)&0xffffffff,value)
 tile=(cfg.get('base',1000)+cfg.get('actor_y',11)*228+cfg.get('actor_x',10))&0xffffffff;put(u,(0xbcd618+offset)&0xffffffff,tile)
 for i,spec in cfg.get('actors',{}).items():
  off=i*STRIDE
  for at,key,default in [(0xbcd602,'kind',83),(0xbcd606,'status',1),(0xbcd630,'state',8)]:byte(u,at+off,spec.get(key,default))
  for at,key,default in [(0xbcd600,'next',0),(0xbcd60c,'x',20),(0xbcd60e,'y',21)]:word(u,at+off,spec.get(key,default))
  put(u,0x7090cc+spec.get('kind',83)*56,spec.get('enabled',1))
 for at,value in cfg.get('words',[]):word(u,at,value)
 for at,value in cfg.get('stores',[]):put(u,at,value)
 if fn==0x4eae20:return [cfg.get('tile',tile),actor]
 if fn==0x4ec4a0:return [actor,cfg.get('new_state',19)]
 if fn==0x4ec0f0:return [cfg.get('x',10),cfg.get('y',11),cfg.get('out_x',S+128),cfg.get('out_y',S+132),cfg.get('rings',0),actor,(sym(names[0x4eae20]) if c else 0x4eae20) if cfg.get('actual_callback') else CALLBACK]
 return [actor]

def invoke(c,u,state,fn,args,route):
 for reg,value in zip(registers,[0xa5b6c7d8,0x22222222,0x33333333,saved[0],SP,*saved[1:]]):u.reg_write(reg,value)
 u.reg_write(UC_X86_REG_EFLAGS,2);put(u,SP,STOP)
 for i,value in enumerate(args):put(u,SP+4+i*4,value)
 pc=fn if not c else sym(names[fn] if route=='named' else 'FUN_%08x'%fn)
 try:u.emu_start(pc,STOP,count=50000000)
 except UcError:return ('fault',)
 assert u.reg_read(UC_X86_REG_EIP)==STOP,(hex(fn),route,hex(u.reg_read(UC_X86_REG_EIP)))
 assert u.reg_read(UC_X86_REG_ESP)==SP+4 and [u.reg_read(registers[i]) for i in [3,5,6,7]]==saved
 return u.reg_read(UC_X86_REG_EAX)

def exercise(fn=0x4ec4a0,**cfg):
 global checked,invocations,actual_cases,protocol_cases,synthetic_cases
 expected=None
 if '--quick' in sys.argv:print('start',hex(fn),cfg,flush=True)
 for c,u,state in machines:
  for route in ['native'] if not c else ['named'] if fn==0x4eae20 else ['named','canonical']:
   args=setup(c,u,state,fn,cfg);state['collect']=True;invocations+=1;result=invoke(c,u,state,fn,args,route);state['collect']=False
   if cfg.get('expect_fault'):assert state['faults'] and result==('fault',),(fn,cfg,route,state['faults'])
   else:assert not state['faults'] and result!=('fault',),(hex(fn),cfg,route,state['faults'])
   current=(result,tuple(state['writes']),tuple(state['events']),snapshot(u),bytes(u.mem_read(S,0x48000)),tuple(state['faults']))
   if expected is None:expected=current
   elif expected!=current:
    changes=[]
    for part in [1,2]:
     if current[part]!=expected[part]:changes.append((part,len(expected[part]),len(current[part]),next(((i,a,b) for i,(a,b) in enumerate(zip(expected[part],current[part])) if a!=b),None)))
    memory=[next(((hex(base+i),a,b) for i,(a,b) in enumerate(zip(x,y)) if a!=b),None) for base,x,y in [(0x560000,expected[3],current[3]),(S,expected[4],current[4])]]
    raise AssertionError((hex(fn),cfg,route,'return',expected[0],current[0],'trace',changes,'memory',memory,'faults',expected[-1],current[-1]))
 checked+=1
 if cfg.get('controlled') or (fn==0x4ec0f0 and not cfg.get('actual_callback')):protocol_cases+=1
 elif cfg.get('read_mutations') or cfg.get('child_mutations'):synthetic_cases+=1
 else:actual_cases+=1
 if checked%20==0:print('progress',checked,hex(fn),flush=True)

if __name__=='__main__':
 exercise(0x4eae20);exercise(0x4ec0f0,callback_results=[0,0,0,7]);exercise(new_state=18);exercise(new_state=26);exercise()
 exercise(0x4ec7c0);exercise(0x4ec990,kind=1);exercise(0x4ec990)
 if '--quick' in sys.argv:print('PASS quick',checked,invocations);sys.exit(0)
 for state in [0,4,8,12,18,19,25,26,255,0x10012,0x10013,0x1001a,0xffffffff]:exercise(new_state=state)
 for kind,status,state in itertools.product([0,25,77,81,82,83,84,85,92,100,101,102,103,104,105,255],[0,1,255],[4,8,12]):exercise(0x4ec990,kind=kind,status=status,state=state)
 for first,second,history in itertools.product([0,1,0xffffffff,0x12345678],[0,1,0xffffffff],[0,98,99,100,110,111,0xffffffff]):
  exercise(new_state=18,first=first,second=second,history=history)
  exercise(new_state=19,first=first,second=second,history=history,terrain=4)
 for state,first in itertools.product([0,8,19,25,26,255],range(128)):exercise(0x4ec7c0,state=state,first=first,terrain=4)
 for history in [99,100,110,111,0xffffffff,0x7fffffff,0x80000000]:exercise(0x4ec7c0,history=history,terrain=4)
 exercise(new_state=19,terrain=0x80004)
 for value in [0,1,0xffffffff,0x80000000,0x12345678]:
  exercise(new_state=19,controlled=[0x4eb800],results={0x4eb800:[value]})
  exercise(new_state=19,controlled=[0x4eb800],results={0x4eb800:[value]},nearest_x=0xffff8000,nearest_y=0xffff8123)
 for value in [0,1,0x100,0x12345601,0x80000001,0xffffffff]:
  for status,state in [(0,8),(1,4),(1,12)]:exercise(0x4ec990,controlled=[0x4ea9c0],results={0x4ea9c0:[value]},status=status,state=state)
 for value in [2,0x40000002,0x80000002,0xc0000002]:exercise(0x4ec990,controlled=[0x4ec0f0],results={0x4ec0f0:[value]},actors={2:dict(x=0x8000,y=0xffff)})
 for first in range(8):
  for position in range(8):
   delta=struct.unpack_from('<i',seed,0x5e4e0c-0x560000+position*4)[0]
   exercise(new_state=19,first=first,stores=[(0xaf598c+(1000+11*228+10+delta)*4,4)])
 for x,y,rings in itertools.product([-1,0,10,228],[0,11,228],[0,1,2]):exercise(0x4ec0f0,x=x,y=y,rings=rings)
 for n in range(1,17):exercise(0x4ec0f0,rings=2,callback_results=[0]*(n-1)+[0x12340001])
 for ox,oy in [(S+128,S+128),(0xbc9768,S+132),(S+128,0xbc9768),(0xbca2d0,0xbca2d4),(0xbca2d8,0xbca2d8)]:exercise(0x4ec0f0,out_x=ox,out_y=oy,rings=1)
 exercise(0x4ec0f0,x=0,y=0,width=0,height=0,rings=-1)
 exercise(0x4ec0f0,x=10,y=11,terrain=0x80000,rings=1)
 exercise(0x4ec0f0,rings=1,callback_results=[0,0,7],callback_mutations={1:[(S+128,12),(S+132,13),(0xbc9768,2000)]})
 exercise(0x4ec0f0,rings=1,callback_results=[0,0,7],callback_mutations={1:[(0xbca2d0,0),(0xbca2d4,0)]})
 exercise(0x4ec0f0,callback_mutations={2:[(S+132,0xffffffff)]})
 for kind,status,state,enabled in itertools.product([25,77,78,82,83,92,93,100,101,255],[0,1],[4,8],[0,1]):
  exercise(0x4eae20,words=[(0xb350dc+(1000+11*228+10)*2,2)],actors={2:dict(kind=kind,status=status,state=state,enabled=enabled)})
 exercise(0x4eae20,words=[(0xb350dc+(1000+11*228+10)*2,1)],actors={1:dict(next=2,kind=82),2:dict(next=3,kind=82),3:dict(kind=83)})
 exercise(0x4eae20,words=[(0xb350dc+(1000+11*228+10)*2,0xffff)],actors={-1:dict(kind=83)})
 for fn in [0x4ec990,0x4ec0f0]:exercise(fn,actual_callback=True,words=[(0xb350dc+(1000+11*228+10)*2,2)],actors={2:dict(kind=83)})
 # Synthetic reads test that native cached values are not silently reloaded.
 exercise(new_state=19,terrain=4,read_mutations={0xbcd60c+388:[(1,[(0xbcd60c+388,0x002b002a)])]})
 exercise(0x4ec990,actors={2:dict(kind=83)},words=[(0xb350dc+(1000+11*228+10)*2,2)],read_mutations={0xbcd60c+2*388:[(1,[(0xbcd60c+2*388,0x001f001e)])]})
 missing=points-hits;missing_edges={(a,b) for a,targets in branches.items() for b in targets}-edges
 print('Compared command transitions:',checked,'scenarios;',invocations,'invocations;',len(points&hits),'/',len(points),'instructions;',len(edges),'conditional outcomes;',actual_cases,'actual;',protocol_cases,'protocol;',synthetic_cases,'synthetic RAM',flush=True)
 print('Unexecuted:',[hex(a) for a in sorted(missing)],'edges:',[(hex(a),hex(b)) for a,b in sorted(missing_edges)],flush=True)
 audit=json.loads((ROOT/'metadata/actor-command-transition-audit.json').read_text())
 assert audit['passed'] and audit['native_sha256']==catalogue['native_sha256']
 for p,sha in audit['source_hashes'].items():assert hashlib.sha256((ROOT/p).read_bytes()).hexdigest()==sha,('stale audit',p)
 dead={int(a,16) for a in audit['analytically_unreachable_instructions']}
 dead_edges={(int(a,16),int(b,16)) for a,b in audit['analytically_unreachable_edges']}
 assert missing==dead and missing_edges==dead_edges,(missing-dead,missing_edges-dead_edges,dead-missing,dead_edges-missing_edges)
 sources=['CMakeLists.txt','direct_c/verify_actor_command_transition.py','direct_c/audit_actor_command_transition.py','direct_c/verify_options_startup.py','direct_c/native_test_pe.py','metadata/actor-command-transition-native.json','metadata/actor-command-transition-audit.json','metadata/actor-command-transition-next-rea-evidence.json','metadata/actor-command-transition-additions-rea-records.json','metadata/actor-command-transition-callers-rea-records.json']
 for directory in ['src','runtime','include','generated']:
  sources += [p.relative_to(ROOT).as_posix() for p in (ROOT/directory).rglob('*') if p.is_file() and p.suffix in ['.c','.h','.asm','.inc']]
 proof=dict(passed=True,native_sha256=catalogue['native_sha256'],compiled_C_path=str(target.resolve()),compiled_C_sha256=target_sha,compiled_C_map_sha256=hashlib.sha256(target.with_suffix('.map').read_bytes()).hexdigest(),checked=checked,invocations=invocations,coverage={hex(a):dict(instructions=len(p),executed=len(p&hits),analytically_unreachable=len(p&dead)) for a,p in owned.items()},conditional_edges=len(edges),unexecuted_instructions=[],unexecuted_edges=[],analytically_unreachable_instructions=sorted(hex(a) for a in dead),analytically_unreachable_edges=[[hex(a),hex(b)] for a,b in sorted(dead_edges)],actual_scenarios=actual_cases,protocol_scenarios=protocol_cases,synthetic_memory_scenarios=synthetic_cases,controlled_algorithm_endpoints_by_lane=dict(actual=[],protocol=['explicit child result overrides and/or synthetic tile callback']),controlled_platform_endpoints=[],source_hashes={p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest() for p in sources},limitations=[
  'Five whole bodies. Actual cases execute all reached game algorithms; explicit child result/callback lanes are protocol tests. The original and restored host games are never launched.',
  'Full original data/private bytes, ordered CPU stores, normalized meaningful child arguments, full EAX results and normal-return nonvolatile ABI are compared. Three instructions and six outcomes are separately proved unreachable, not executed.',
  'Read/call-mutated RAM is synthetic; no physical concurrency is claimed. Incidental volatile registers/flags, arbitrary caller-stack aliases, self-modifying code, private-stack corruption, fault CPU state, OS exceptions and complete gameplay are outside this finite verification.'
 ])
 suffix='diagnostic' if target.stem=='restore_link_check' else 'game';(ROOT/f'metadata/actor-command-transition-{suffix}.json').write_text(json.dumps(proof,indent=2)+'\n')
 print('PASS whole command graph, exact memory/store/call traces and EAX; qualified dead branches remain separate',flush=True)
