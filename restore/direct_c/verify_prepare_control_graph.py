"""Whole map-information parent with genuine C providers; stdout only."""
from pathlib import Path
import hashlib,itertools,struct,sys
ROOT=Path(__file__).resolve().parents[1]
sys.path[:0]=[str(ROOT/'build/offline-verification-deps'),str(ROOT/'direct_c')]
from native_test_pe import PE
from unicorn import *
from unicorn.x86_const import *
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
from capstone.x86 import X86_OP_REG,X86_OP_MEM

# Reuse byte authentication, RET contracts and the parent's CFG invariant only.
# The frozen finite-call emulator and its corpus are never executed here.
frozen=ROOT/'artifacts/menu_prepare_control_phase/test_prepare.py'
assert hashlib.sha256(frozen.read_bytes()).hexdigest()=='e19150bbccc0ddcbaf72e2d3de134abdc655160e1b4850d7b43ce22510594597'
ns={'__file__':str(frozen),'__name__':'oracle_metadata'}
text=frozen.read_text();exec(compile(text[:text.index('VS=0x30000000')],str(frozen),'exec'),ns)
native,candidate,argc,expected=ns['native'],ns['candidate'],ns['argc'],ns['expected']
guard_paths=[Path(__file__),frozen,Path(sys.argv[1]),ROOT/'direct_c/menu_004f7e50.c',ROOT/'direct_c/menu_input_boundary.c']
guard_hashes={p:hashlib.sha256(p.read_bytes()).hexdigest() for p in guard_paths}
md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True
parent={int(i['address'],16):next(md.disasm(bytes.fromhex(i['bytes']),int(i['address'],16))) for i in ns['body']}
callsites={a:int(i.op_str,16) for a,i in parent.items() if i.mnemonic=='call'}
# Pushed incoming ECX is scratch only. All normal-path calls follow its full
# overwrite; the sole earlier call is the no-argument modal reset then EAX0.
parent_graph={a:set() for a in expected}
for target,predecessors in ns['pred'].items():
 for predecessor in predecessors:parent_graph[predecessor].add(target)
pre_scratch=set();todo=[0x4f7e50]
while todo:
 a=todo.pop()
 if a==0x4f7ee7 or a in pre_scratch:continue
 pre_scratch.add(a);todo.extend(parent_graph[a]-pre_scratch)
assert pre_scratch&set(callsites)=={0x4f7e76} and callsites[0x4f7e76]==0x4e2ab0
assert ns['body'][next(i for i,x in enumerate(ns['body']) if x['address']=='004f7ee7')]['instruction']=='MOV dword ptr [ESP + 0x10],EDI'

selfcalls={0x40d940,0x417050,0x4e6710,0x545740,0x558be0,0x555e80,0x54f470}
DATA,END,VS,HS,FRAME,STOP=0x56f000,0x1600000,0x30000000,0x34000000,0x35000000,0x31000000
SP,HSP,CELL=VS+0x8000,HS+0x8000,22900
neighbor_deltas=struct.unpack_from('<9i',native.data,0x5f76c4-native.base)
assert neighbor_deltas==(0,-228,228,1,-1,-229,-227,227,229)
initial_data=bytearray(END-DATA)
initial_data[:len(native.data)-(DATA-native.base)]=native.data[DATA-native.base:]
public='--public' in sys.argv
regs=[UC_X86_REG_EAX,UC_X86_REG_ECX,UC_X86_REG_EDX,UC_X86_REG_EBX,UC_X86_REG_ESP,UC_X86_REG_EBP,UC_X86_REG_ESI,UC_X86_REG_EDI]
regids=dict(zip(['eax','ecx','edx','ebx','esp','ebp','esi','edi'],regs))
machines=[];hits=set();covered_calls=set();checked=0
def word(u,a,n=4):return int.from_bytes(u.mem_read(a&0xffffffff,n),'little')
def put(u,a,v,n=4):u.mem_write(a&0xffffffff,(v&((1<<(n*8))-1)).to_bytes(n,'little'))
for compiled in [False,True]:
 u=Uc(UC_ARCH_X86,UC_MODE_32)
 if compiled:
  u.mem_map(candidate.base,(len(candidate.data)+4095)&~4095);u.mem_write(candidate.base,bytes(candidate.data))
  u.mem_map(DATA,END-DATA)
 else:u.mem_map(native.base,END-native.base);u.mem_write(native.base,bytes(native.data))
 for a,n in [(VS,0x10000),(HS,0x10000),(FRAME,4096),(STOP,4096)]:u.mem_map(a,n)
 state={};division_cache={}
 boundary=candidate.entry('restore_menu_input_boundary_dispatch') if compiled else None
 def code(cpu,a,n,unused,compiled=compiled,state=state,boundary=boundary,division_cache=division_cache):
  state['last']=a
  if not compiled and a in expected:hits.add(a)
  if compiled and not (candidate.base<=a<candidate.base+len(candidate.data)):
   raise AssertionError(('C escaped its DLL',hex(a),state['case']))
  if (compiled and a==boundary) or (not compiled and a in callsites):
   sp=cpu.reg_read(UC_X86_REG_ESP)
   if compiled:
    site,target,frame=struct.unpack('<III',cpu.mem_read(sp+4,12));values=struct.unpack('<8I',cpu.mem_read(frame,32));at=values[4]+4;ecx=values[1]
   else:site=a;target=callsites[a];at=sp;ecx=cpu.reg_read(UC_X86_REG_ECX)
   assert target in argc,(hex(site),hex(target))
   argv=tuple(word(cpu,at+j*4) for j in range(argc[target]))
   state['calls'].append((site,target,argv,ecx if target in selfcalls else None));covered_calls.add(target)
  # Both engines stop before invalid integer division; never execute native #DE.
  if a not in division_cache:
   ins=next(md.disasm(bytes(cpu.mem_read(a,n)),a,count=1),None) if cpu.mem_read(a,1)[0] in [0xf7,0x66] else None
   division_cache[a]=ins if ins is not None and ins.mnemonic in ['idiv','div'] else None
  ins=division_cache[a]
  if ins is not None:
    op=ins.operands[0];width=op.size*8;mask=(1<<width)-1
    if op.type==X86_OP_REG:d=cpu.reg_read(regids[ins.reg_name(op.reg)])&mask
    else:
     assert op.type==X86_OP_MEM;m=op.mem;p=m.disp
     if m.base:p+=cpu.reg_read(regids[ins.reg_name(m.base)])
     if m.index:p+=cpu.reg_read(regids[ins.reg_name(m.index)])*m.scale
     d=word(cpu,p,op.size)
    dividend=((cpu.reg_read(UC_X86_REG_EDX)&mask)<<width)|(cpu.reg_read(UC_X86_REG_EAX)&mask)
    if ins.mnemonic=='idiv':
     if dividend>>(2*width-1):dividend-=1<<(2*width)
     if d>>(width-1):d-=1<<width
    q=(abs(dividend)//abs(d))*(-1 if (dividend<0)^(d<0) else 1) if d else None
    if q is None or (not -(1<<(width-1))<=q<(1<<(width-1)) if ins.mnemonic=='idiv' else not 0<=q<=mask):
     raise AssertionError(('invalid fixture division before execution',compiled,hex(a),state['case'],dividend,d))
 u.hook_add(UC_HOOK_CODE,code)
 def written(cpu,kind,a,n,v,unused,state=state):state['writes'].append((a,n,v&((1<<(8*n))-1)))
 u.hook_add(UC_HOOK_MEM_WRITE,written,begin=DATA,end=END-1)
 machines.append((u,state))

def run(**cfg):
 global checked
 results=[]
 for compiled,(u,state) in enumerate(machines):
  u.mem_write(DATA,bytes(initial_data));u.mem_write(VS,b'\xcc'*0x10000);u.mem_write(HS,b'\xa5'*0x10000)
  put(u,0x605a3c,cfg.get('editor',0));put(u,0xe950d8,cfg.get('modal',0))
  for a,v in [(0xbca2c8,100),(0xbca2cc,100),(0xbca2d0,228),(0xbca2d4,228),(0xbca2d8,0),(0xbc96c0,0xffffffff),(0xbc96c4,0xffffffff),(0xe38dac,cfg.get('height',768)),(0xe38e30,cfg.get('cursor',0)),(0xf53f78,cfg.get('sidebar',0)),(0xea4490,1)]:put(u,a,v)
  # Actual class layouts from the independently checked base/progress graphs.
  # These are input objects, not virtual-call replacements or forced results.
  for receiver,table,factor in [(0x6fd5d8,0x56f48c,2),(0x6fd368,0x56f4dc,2),(0x6de548,0x56f6e8,3),
                                (0x6f9320,0x570160,0),(0x6f5648,0x570268,0),(0x6de9e0,0x570228,0),(0x6e9188,0x5701e8,0)]:
   put(u,receiver,table)
   if factor:put(u,receiver+4,factor)
  put(u,0xaf598c+CELL*4,cfg.get('terrain',0));put(u,0xb285cc+CELL,cfg.get('access',0),1)
  put(u,0xb4e6fc+CELL*2,cfg.get('building',0),2)
  for j in range(1,10):
   record=0x7999d8+j*264
   for a,v,n in [(record,1,1),(record+3,2,1),(record+6,100,2),(record+8,100,2),(record+12,CELL,4),(record+16,cfg.get('kind',1),2),(record+56,1,2)]:put(u,a,v,n)
  put(u,0x7999dd+264,cfg.get('meta',0),1)
  for j,actor in enumerate(cfg.get('neighbors',[cfg.get('actor',0)]+[0]*8)):
   if not actor:continue
   index=j+1;record=0xbcd600+index*388
   put(u,0xb350dc+(CELL+neighbor_deltas[j])*2,index,2);put(u,record,index,2)
   put(u,record+2,actor,1);put(u,record+6,1,1);put(u,record+0x30,9,1)
   put(u,record+0x1c,100,2);put(u,record+0x1e,100,2);put(u,record+0x12a,0xffff,2)
  for a,v,n in cfg.get('set',[]):put(u,a,v,n)
  put(u,SP,STOP);put(u,SP+4,0x13579bdf)
  initial=[0x87654321,cfg.get('ecx',0x12345678),0xabcdef01,0x11111111,SP,0x22222222,0x33333333,0x44444444]
  state.clear();state.update(case=cfg,calls=[],writes=[],last=0)
  for r,v in zip(regs,initial):u.reg_write(r,v)
  cw=cfg.get('cw',0x37f);u.reg_write(UC_X86_REG_FPCW,cw);u.reg_write(UC_X86_REG_EFLAGS,2)
  if compiled:
   put(u,HSP,STOP);u.reg_write(UC_X86_REG_ESP,HSP)
   if public:start=candidate.entry('FUN_004f7e50')
   else:
    u.mem_write(FRAME,struct.pack('<10I',*initial,2,candidate.entry('restore_menu_input_boundary_dispatch')))
    put(u,HSP+4,FRAME);start=candidate.entry('restore_C_004f7e50')
  else:start=0x4f7e50
  try:u.emu_start(start,STOP,count=20000000 if compiled else 3000000)
  except Exception as e:raise AssertionError((cfg,'C' if compiled else 'native',hex(state['last']),state['calls'][-2:])) from e
  assert u.reg_read(UC_X86_REG_EIP)==STOP,('instruction budget',compiled,cfg)
  result=list(struct.unpack('<8I',u.mem_read(FRAME,32))) if compiled and not public else [u.reg_read(r) for r in regs]
  assert result[4]==(HSP+4 if compiled and public else SP+4) and all(result[i]==initial[i] for i in [3,5,6,7])
  assert u.reg_read(UC_X86_REG_FPCW)==cw and (u.reg_read(UC_X86_REG_FPSW)>>11)&7==0
  assert word(u,SP)==STOP and word(u,SP+4)==0x13579bdf
  results.append((result[0],list(state['calls']),list(state['writes']),bytes(u.mem_read(DATA,END-DATA))))
 for label,a,b in zip(['return','calls','writes','state'],results[0],results[1]):
  if a!=b:
   if label=='state':j=next(j for j,(x,y) in enumerate(zip(a,b)) if x!=y);detail=(hex(DATA+j),a[j:j+8],b[j:j+8])
   elif isinstance(a,list):j=next((j for j,(x,y) in enumerate(zip(a,b)) if x!=y),min(len(a),len(b)));detail=(j,a[j:j+2],b[j:j+2],len(a),len(b))
   else:detail=(a,b)
   raise AssertionError((cfg,label,detail))
 checked+=1
 if '--full' in sys.argv and checked%32==0:print('checked',checked,'whole-graph cases;actual providers',len(covered_calls),'/29',flush=True)

if '--abi' in sys.argv:
 run(editor=1);run(modal=1)
 for kind,ecx in itertools.product([1,225,229],[0,0x12345678,0xffffffff]):run(building=1,kind=kind,ecx=ecx)
 run(actor=7)
 for cw in [0x7f,0x27f,0x37f,0x77f,0xb7f,0xf7f]:run(building=1,kind=225,cw=cw)
 run(neighbors=[1]*9);run(neighbors=[1,4,7,15,27,38,77,78,1])
elif '--case-kind' in sys.argv:run(building=1,kind=int(sys.argv[sys.argv.index('--case-kind')+1],0))
else:
 run(editor=1);run(modal=1);run()
 for bits in [1,2,4,8,16,32,64,0x100,0x800,0x1000,0x10000,0x20000002]:run(terrain=bits)
 for kind,meta in itertools.product(range(237) if '--full' in sys.argv else [1,23,54,73,106,183,230],[0,1]):run(building=1,kind=kind,meta=meta)
 for actor in [1,4,7,15,27,38,77,78]:run(actor=actor)
 run(neighbors=[1]*9);run(neighbors=[1,4,7,15,27,38,77,78,1])
print(f'PASS {checked} whole real-graph cases;parent{len(hits)}/688 instructions;{len(covered_calls)}/29 actual providers;'
      'return/ordered calls+all game writes/full mapped state/nonvolatile+SP guards equal;no finite internal endpoint')
print('ABI mode:','public noargs C' if public else 'explicit frame',';CW/TOP preserved')
print('Original9-entry signed neighbor table preserved;all initial game data comes from pinned PE plus explicit record/constructor inputs.')
print('Unexecuted parent-call targets:',','.join(hex(a) for a in sorted(set(argc)-covered_calls)) or 'none')
for p,h in guard_hashes.items():assert hashlib.sha256(p.read_bytes()).hexdigest()==h,('Input drift',str(p))
