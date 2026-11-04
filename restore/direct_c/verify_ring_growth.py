"""Literal gameplay growth, allocation order and padded native source reads.

The imported canonical suite executes35 additional constructor/reserve/copy
cases first. Only allocation/free are controlled with identical readable
backing on both sides. Exception unwinding and arbitrary host heap layout
are separate obligations; this proves finite normal-return algorithm behavior.
"""
from verify_dynamic_history import *
from unicorn import UC_HOOK_MEM_WRITE
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import json

catalogue=json.loads((ROOT/'metadata/ring-growth-native.json').read_text())
rea=json.loads((ROOT/'metadata/ring-growth-rea-evidence.json').read_text());evidence={r['evidence_id']:r for r in rea['records']}
f=catalogue['functions'][0];raw=bytes.fromhex(f['bytes_hex']);r=evidence[f['bytes_evidence_id']]
assert r['authority']=='shipped-artifact' and r['subject']['digest']['sha256']==catalogue['native_sha256']
assert r['normalized_result']['complete'] and r['normalized_result']['bytes_hex']==f['bytes_hex']
assert raw==bytes(images[0].data[0x4f6a50-images[0].base:0x4f6a50-images[0].base+len(raw)])
ins=list(Cs(CS_ARCH_X86,CS_MODE_32).disasm(raw,0x4f6a50));points={int(a,16) for a in f['instruction_addresses'].split()}
assert {i.address for i in ins}==points
branches={i.address:{int(i.op_str,16),i.address+i.size} for i in ins if i.mnemonic.startswith('j') and i.mnemonic!='jmp'}
hits=set();edges=set();last=[None];checked=invocations=0
for c,u,state in machines:
 state.update(collect=False,writes=[])
 def write(uc,access,at,size,value,data):
  if data['collect']:data['writes'].append((at,size,value&((1<<(size*8))-1)))
 u.hook_add(UC_HOOK_MEM_WRITE,write,state,begin=S,end=S+0x1ffff)
 u.hook_add(UC_HOOK_MEM_WRITE,write,state,begin=0x560000,end=0x126cfff)
 if not c:
  def code(uc,pc,size,data):
   if not data['collect']:return
   hits.add(pc)
   if last[0] in branches:edges.add((last[0],pc))
   last[0]=pc
  u.hook_add(UC_HOOK_CODE,code,state,begin=min(points),end=max(points))

def execute(c,u,token,args,self,route):
 member=token!=0x4a4340;argv=(self,*args) if c and member else args
 pc=token if not c else sym('restore_ring_004f6a50' if route=='compat' else 'FUN_%08x'%token)
 for r,v in zip(registers,[0x11111111,self,0x33333333,saved[0],SP,*saved[1:]]):u.reg_write(r,v)
 u.reg_write(UC_X86_REG_EFLAGS,2);put(u,SP,STOP)
 for i,v in enumerate(argv):put(u,SP+4+i*4,v)
 u.emu_start(pc,STOP,count=30000000)
 assert u.reg_read(UC_X86_REG_EIP)==STOP
 assert u.reg_read(UC_X86_REG_ESP)==SP+4+(4 if not c and member else 0)
 assert [u.reg_read(registers[i]) for i in [3,5,6,7]]==saved and get(u,TEB)==0xfeedbabe
 return u.reg_read(UC_X86_REG_EAX) if token==0x4f6a50 else None

def exercise(token=0x4f6a50,old=5,new=15,head=1,count=None):
 global checked,invocations
 count=old if count is None else count;expected=None
 for c,u,state in machines:
  routes=['native'] if not c else ['canonical',*(['compat'] if token==0x4f6a50 else [])]
  for route in routes:
   events=setup(c,u,state);self=Q if token!=0x4a4340 else 0x9783a4+28
   object(u,self,old,BUF);put(u,self+4,head);put(u,self+12,count);tail=(head+count-1)%old if old else 0;put(u,self+8,tail)
   args=[new] if token==0x4f6a50 else [0xdeadbeef] if token==0x4f68f0 else [7,2]
   if token==0x4a4340:byte(u,0x799aa3+2*264,1);byte(u,0xbcd630+7*388,0)
   state.update(collect=True,writes=[]);last[0]=None;invocations+=1
   result=execute(c,u,token,args,self,route);state['collect']=False
   grew=(new>old if token==0x4f6a50 else count==old)
   if token==0x4f6a50:
    assert result==1 and get(u,self+4)==head and get(u,self+12)==count
    if grew:
     assert get(u,self+16)==new and len(events)==4
     # These destination words come from readable backing beyond the smaller
     # temporary allocation; the previous repaired gameplay path wrote zeros.
     dest=get(u,self+20)
     assert bytes(u.mem_read(dest+old*4,(new-old)*4))==b'\xdb'*((new-old)*4)
    else:assert not events
   elif grew:
    assert get(u,self+16)==old+10 and get(u,self+12)==count+1 and len(events)==4
    out_tail=(tail+1)%(old+10);assert get(u,self+8)==out_tail
    assert get(u,get(u,self+20)+out_tail*4)==(0xdeadbeef if token==0x4f68f0 else 7)
   answer=(result,tuple(events),bytes(u.mem_read(S,0x20000)),snapshot(u),tuple(state['writes']) if token==0x4f6a50 else None)
   if expected is None:expected=answer
   elif answer!=expected:
    mem=next(((hex(S+i),a,b) for i,(a,b) in enumerate(zip(expected[2],answer[2])) if a!=b),None)
    trace=next(((i,a,b) for i,(a,b) in enumerate(zip(expected[4] or [],answer[4] or [])) if a!=b),None)
    raise AssertionError((hex(token),old,new,head,count,route,'events',(expected[1],answer[1]),'memory',mem,'trace',trace,'lengths',(len(expected[4] or []),len(answer[4] or []))))
 checked+=1

for old,new,head,count in [(5,5,1,5),(5,3,4,5),(5,-1,1,2),(0,0,1,0),(1,2,0,1),(2,12,1,2),(5,15,1,5),(5,15,4,5),(5,15,4,0),(5,15,4,12),(11,21,10,11)]:exercise(old=old,new=new,head=head,count=count)
for token in [0x4f68f0,0x4a4340]:
 for old,head,count in [(1,0,1),(5,1,5),(5,4,5),(5,4,3)]:exercise(token,old=old,head=head,count=count)
missing=points-hits;missing_edges={(a,b) for a,ends in branches.items() for b in ends if (a,b) not in edges}
assert not missing and not missing_edges,(missing,missing_edges)
print('PASS literal gameplay growth:',checked,'scenarios;',invocations,'invocations;',len(points),'native instructions; canonical baseline35 cases; actual class children; only heap allocation/free controlled',flush=True)
sources=['CMakeLists.txt','include/mission_compat.h','include/actor_teardown_model.h','runtime/dynamic_history.c','runtime/menu_drag_actor_teardown.c','runtime/actor_civic.c','direct_c/verify_ring_growth.py','direct_c/verify_dynamic_history.py','metadata/ring-growth-native.json','metadata/ring-growth-rea-evidence.json']
proof=dict(native_sha256=catalogue['native_sha256'],compiled_C_path=str(target.resolve()),compiled_C_sha256=target_sha,passed=True,checked=checked,invocations=invocations,canonical_baseline_cases=35,coverage={'0x4f6a50':dict(instructions=len(points),executed=len(points&hits))},conditional_edges=len(edges),unexecuted_instructions=[],unexecuted_edges=[],source_hashes={p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest() for p in sources},controlled_platform_endpoints=['0x561a1d','0x561a2b'],controlled_algorithm_endpoints=[],limitations=['Actual class reserve/copy/growth and gameplay enqueue/enrollment paths execute; only malloc/free are controlled with identical readable padded backing. Original destination-capacity copy reads beyond the temporary source allocation, including backing padding; no FIFO repair remains.','Ordered nonstack writes compared for direct growth; callers compare allocation events, complete static data and heap backing, return, saved ABI and restored FS.','Normal-return paths only; arbitrary host allocator adjacency, undefined source extent, allocation failure and C++ exception unwinding are not certified.'])
suffix='diagnostic' if target.stem=='restore_link_check' else 'game';(ROOT/f'metadata/ring-growth-{suffix}.json').write_text(json.dumps(proof,indent=2)+'\n')
