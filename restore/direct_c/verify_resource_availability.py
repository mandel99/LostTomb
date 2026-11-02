"""Whole availability integer closure; actual compiled helpers, stdout only."""
from pathlib import Path
import argparse,hashlib,itertools,json,struct,sys
ROOT=Path(__file__).resolve().parents[1];sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from unicorn import *
from unicorn.x86_const import *
from capstone import Cs,CS_ARCH_X86,CS_MODE_32,CS_OP_MEM,CS_AC_WRITE
from native_test_pe import PE
ap=argparse.ArgumentParser();ap.add_argument('dll',type=Path);ap.add_argument('--quick',action='store_true');args=ap.parse_args()
pin='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d';original=ROOT/'build/smoke-game/Pharaoh.exe'
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
assert sha(original)==pin
native=PE(original);candidate=PE(args.dll);cs=Cs(CS_ARCH_X86,CS_MODE_32);cs.detail=True
files=[Path(__file__),args.dll,ROOT/'runtime/resource_availability.c',ROOT/'include/resource_availability.h',ROOT/'include/training_queries.h']+[ROOT/'src'/f'unit_{n:05d}.c' for n in [625,626,631,632,669,1603,1605]]
hashes={p:sha(p) for p in files}
targets={0x559480,0x5596d0,0x443950,0x4439c0,0x443b90,0x443d90,0x447790,0x4c5190,0x4c5240};functions={};instructions=set();callsites={};branches={}
for p in (ROOT/'metadata/native-function-audit-cache').glob('batch-*.json'):
 cache=json.loads(p.read_text());assert cache['native_sha256']==pin
 for f in cache['entries']:
  entry=int(f['requested_address'],16)
  if entry not in targets:continue
  assert f['status']=='exact_ghidra_function' and not f['assembly']['has_more'];functions[entry]=set()
  for it in f['assembly']['items']:
   a=int(it['address'],16);b=bytes.fromhex(it['bytes']);assert native.data[a-native.base:a-native.base+len(b)]==b
   functions[entry].add(a);instructions.add(a);i=next(cs.disasm(b,a))
   if entry not in [0x559480,0x5596d0]:
    assert not any(op.type==CS_OP_MEM and op.access&CS_AC_WRITE for op in i.operands),('Helper game write',hex(a))
    if i.mnemonic=='call':assert int(i.op_str,16) in targets-{0x559480,0x5596d0},('Open helper call',hex(a))
   if entry in [0x559480,0x5596d0]:
    if i.mnemonic=='call':callsites[a]=(int(i.op_str,16),a+i.size)
    if i.mnemonic.startswith('j') and i.mnemonic!='jmp':branches[a]={a+i.size,int(i.op_str,16)}
assert functions.keys()==targets and len(functions[0x559480])==182 and len(functions[0x5596d0])==106
source=(ROOT/'runtime/resource_availability.c').read_text()
for a,n in [(0x55968c,36),(0x5596b0,24)]:
 marker=f'static const uint8_t mi_rom_{a:08x}[]={{';assert source.count(marker)==1
 values=bytes(int(x,16) for x in source.split(marker)[1].split('};')[0].split(','));assert len(values)==n and values==bytes(native.data[a-native.base:a-native.base+n])
def resolved(a):
 for _ in range(8):
  i=next(cs.disasm(bytes(candidate.data[a-candidate.base:a-candidate.base+16]),a))
  if i.mnemonic!='jmp' or not i.op_str.startswith('0x'):return a
  a=int(i.op_str,16)
 raise AssertionError('Thunk cycle')
STACK=0x30000000;SP=STACK+0xc000;STOP=0x31000000
regs=[UC_X86_REG_EAX,UC_X86_REG_ECX,UC_X86_REG_EDX,UC_X86_REG_EBX,UC_X86_REG_ESP,UC_X86_REG_EBP,UC_X86_REG_ESI,UC_X86_REG_EDI]
primary=functions[0x559480]|functions[0x5596d0];hits=set();edges=set();checked=0;helper_access_totals=[0,0]
regions=[(0x77645c,4*36*4+32),(0x77ddf0,61*106+32),(0x784d22,0x24),(0x785d60,258*124+32),(0xeaac18,32),(0xf53dc8,0x150)]
for player in [0,1,9]:regions.extend([(0xea41ec+player*0x49d8,0x50),(0xea61e4+player*0x49d8,0x24)])
def machine(compiled):
 u=Uc(UC_ARCH_X86,UC_MODE_32)
 if compiled:
  u.mem_map(candidate.base,(len(candidate.data)+4095)&~4095);u.mem_write(candidate.base,bytes(candidate.data));u.mem_map(0x56f000,0x1091000);u.mem_write(0x56f000,bytes(native.data[0x56f000-native.base:]))
 else:u.mem_map(native.base,0x1200000);u.mem_write(native.base,bytes(native.data))
 u.mem_map(STACK,0x10000);u.mem_map(STOP,4096);state={}
 dispatch=resolved(candidate.entry('restore_resource_availability_dispatch')) if compiled else 0
 fatal={resolved(candidate.entry(n)) for n in ['restore_resource_availability_failure','abort']} if compiled else set()
 def word(a):return int.from_bytes(u.mem_read(a,4),'little')
 def emit(event):state['calls'].append(event);state['sequence'].append(event)
 def code(cpu,a,n,user):
  if compiled:
   if a in fatal:raise AssertionError(('Unexpected C rejection',state['case']))
   if state.get('returns') and a==state['returns'][-1][0]:
    _,frame=state['returns'].pop();emit(('RETURN',word(frame)))
   if a==dispatch:
    sp=cpu.reg_read(UC_X86_REG_ESP);site,target,frame=[word(sp+x) for x in [4,8,12]];virtual_sp=word(frame+16);argument=word(virtual_sp+4)
    emit(('CALL',site,target,argument))
    if target!=0x559480:state['returns'].append((word(sp),frame))
  else:
   assert a in instructions,hex(a);hits.add(a)
   if state.get('previous') in branches:edges.add((state['previous'],a))
   state['previous']=a;state['owned']=a in primary
   if state.get('returns') and a==state['returns'][-1][0]:state['returns'].pop();emit(('RETURN',cpu.reg_read(UC_X86_REG_EAX)))
   if a in callsites:
    target,end=callsites[a];argument=word(cpu.reg_read(UC_X86_REG_ESP));emit(('CALL',a,target,argument))
    if target!=0x559480:state['returns'].append((end,0))
 def access(cpu,kind,a,n,v,user):
  if STACK<=a and a+n<=STACK+0x10000:return
  if compiled and candidate.base<=a and a+n<=candidate.base+len(candidate.data):return
  # ROM switch bytes have an explicit PE-authenticated portable C definition.
  if 0x55968c<=a and a+n<=0x5596c8:return
  assert any(p<=a and a+n<=p+size for p,size in regions),(hex(cpu.reg_read(UC_X86_REG_EIP)),hex(a),n,state.get('case'))
  if kind!=UC_MEM_WRITE:v=int.from_bytes(cpu.mem_read(a,n),'little')
  event=(int(kind==UC_MEM_WRITE),a,n,v&((1<<(n*8))-1))
  own=not state['returns'] if compiled else state['owned']
  state['access'][0 if own else 1].append(event)
  state['sequence'].append(('OWN' if own else 'HELPER',)+event)
 # Private host/virtual stacks and DLL image are outside the game's PE range.
 # A bad read anywhere inside that original image still hits the range guard.
 u.hook_add(UC_HOOK_CODE,code);u.hook_add(UC_HOOK_MEM_READ|UC_HOOK_MEM_WRITE,access,begin=0x400000,end=0x15ffffff)
 return u,state,compiled
machines=[machine(False),machine(True)]
def run(entry=0x559480,resource=1,**c):
 global checked
 results=[]
 for u,state,compiled in machines:
  def put(a,v,n=4):u.mem_write(a,(v&((1<<(n*8))-1)).to_bytes(n,'little'))
  for a,n in regions:u.mem_write(a,b'\xa5'*n)
  u.mem_write(0x77de00,bytes(61*106));u.mem_write(0x785d74,bytes(258*124))
  for j in range(4*36):put(0x77646c+j*4,c.get('fee',40))
  for j in range(61):
   a=0x77de00+j*106;present=c.get('row',-1)==j or c.get('row',-1)==61
   put(a,int(present),1);put(a+2,c.get('type',0),1);put(a+4,c.get('category',0),1);put(a+5,c.get('enabled',1),1)
   for k in range(1,36):put(a+42+k,int(k in c.get('available',[]) or c.get('all',False)),1)
  count=c.get('count',0);assert 0<=count<=258;put(0x785d70,count)
  for j in range(count):
   a=0x785d74+j*124;put(a+2,23 if j in c.get('events',[]) else c.get('eventtype',0),1)
   for k in range(3):put(a+6+k*2,c.get('event_resource',resource) if k==c.get('field',0) else -32768,2)
  for p in [0,1,9]:
   for j,v in enumerate(c.get('metrics',[0,0,0])):put(0xea61f4+p*0x49d8+j*2,v,2)
   for j in range(0x30):put(0xea41fc+p*0x49d8+j,0,1)
   for j in c.get('flags',[]):put(0xea4205+p*0x49d8+j,c.get('flagvalue',1),1)
   for j in c.get('foodflags',[]):put(0xea41fc+p*0x49d8+j,c.get('flagvalue',1),1)
  put(0xeaac28,c.get('player',0));put(0x784d32,c.get('gate',0),2);put(0x784d34,c.get('gate',0),2)
  u.mem_write(STACK,b'\xcc'*0x10000);put(SP,STOP)
  if entry==0x559480:put(SP+4,resource)
  state.clear();state.update(case=(entry,resource,c),access=[[],[]],calls=[],sequence=[],returns=[],owned=True)
  initial=[0x87654321,0x12345678,0xabcdef01,0x22222222,SP,0x33333333,0x44444444,0x55555555]
  for r,v in zip(regs,initial):u.reg_write(r,v)
  u.reg_write(UC_X86_REG_EFLAGS,c.get('eflags',2))
  u.emu_start(candidate.entry(f'FUN_{entry:08x}') if compiled else entry,STOP,count=20000000)
  assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==SP+4
  assert bytes(u.mem_read(SP+(8 if entry==0x559480 else 4),16))==b'\xcc'*16
  for j in [3,5,6,7]:assert u.reg_read(regs[j])==initial[j]
  assert not state['returns']
  value=u.reg_read(UC_X86_REG_EAX) if entry==0x559480 else 0
  if 'expect' in c:assert value==c['expect'],(compiled,hex(value),c['expect'])
  helper_access_totals[int(compiled)]+=len(state['access'][1])
  results.append((value,tuple(bytes(u.mem_read(a,n)) for a,n in regions),state['access'][0],state['calls'],state['access'][1],state['sequence']))
 if results[0]!=results[1]:
  first=next(((i,a,b) for i,(a,b) in enumerate(itertools.zip_longest(results[0][2],results[1][2])) if a!=b),None)
  helper=next(((i,a,b) for i,(a,b) in enumerate(itertools.zip_longest(results[0][4],results[1][4])) if a!=b),None)
  sequence=next(((i,a,b) for i,(a,b) in enumerate(itertools.zip_longest(results[0][5],results[1][5])) if a!=b),None)
  raise AssertionError((hex(entry),resource,c,'EAX',results[0][0],results[1][0],'state',results[0][1]==results[1][1],'owned access',first,'helper access',helper,'interleaving',sequence,'calls',results[0][3],results[1][3]))
 checked+=1
if args.quick:run();run(resource=12);run(0x5596d0);run(count=258,events=[257],expect=0x101)
else:
 for resource in list(range(0,40))+[-2147483648,-32768,-1,0x10000,0x7fffffff]:run(resource=resource)
 for resource,count,field in itertools.product(range(1,36),[1,2,258],range(3)):run(resource=resource,count=count,events=[count-1],field=field)
 run(count=258,events=[257],expect=0x101);run(resource=-1,expect=0xffffff00)
 for event in [0,1,127,256]:run(count=258,events=[event],resource=35)
 for resource,row,type in itertools.product([1,9,10,11,12,13,15,17,19,23,28,33,34],[0,60],[0,1,3,5,2]):run(resource=resource,row=row,type=type,available=list(range(1,36)),gate=0x100)
 for resource,available in itertools.product([12,34],[[1],[9],[11],[13],[31],[1,11],[9,11],[11,31],[13,31],[1,9,11,13,31]]):
  for type in [0,1]:run(resource=resource,row=0,type=type,available=available,gate=1)
 for available,type,event in [([11],0,9),([1],0,11),([],0,11),([9],1,11),([11],1,9),([],0,13)]:
  run(resource=12,row=0,type=type,available=available,count=1,events=[0],event_resource=event)
 for event in [11,13]:run(resource=34,count=1,events=[0],event_resource=event)
 for resource,gate in itertools.product([10,28],[0,1,0x100,0xffff]):run(resource=resource,gate=gate,count=1,events=[0],event_resource=29 if resource==10 else 20)
 for metrics,fee in itertools.product([[0,0,0],[1,0,0],[0,1,0],[0,0,1],[1,1,1],[-1,0,0]],[0,1,14,15,25,40,-1]):
  for resource in [12,34]:run(resource=resource,row=60,type=1,available=list(range(1,36)),metrics=metrics,fee=fee,category=3)
 for player,scheme in itertools.product([0,1,9],range(7)):
  c={0:{},1:{'flags':[1,12,35]},2:{'foodflags':[1,8]},3:{'row':60,'type':0,'all':True},4:{'row':0,'type':1,'all':True},5:{'count':35,'events':list(range(35)),'event_resource':12},6:{'flags':list(range(1,36)),'flagvalue':0x80}}[scheme]
  run(0x5596d0,player=player,**c)
 for flags in [2,3,0xad7,0x202]:run(0x5596d0,eflags=flags,flags=[1,12,35],count=3,events=[0,2],event_resource=9)
 run(0x5596d0,count=1,events=[0],event_resource=1)
 for gate in [1,0x100]:run(0x5596d0,gate=gate,row=60,type=1,all=True)
for p,h in hashes.items():assert sha(p)==h,('Input drift',str(p))
print('PASS',checked,'whole C/native availability cases; fullEAX559480/void5596D0, ordered owned game reads/writes (including outlined mi_read/mi_alu), actual typed helper args/returns, state+ABI+guards; helper reads separately observed:',helper_access_totals)
print('Instruction coverage:',','.join(f'{a:08X}:{len(v&hits)}/{len(v)}' for a,v in functions.items()))
if not args.quick:
 # Native invariants, qualified to stable disjoint mapped records, count0..258:
 # 559480:439C0(11)!=0 implies443D90(11)!=0 (443E58..443E6B).
 # Thus reaching5595E6 with439C0(11)!=0 already tested559480(9) at5595A4.
 # That pure query either returned true at5595B0, or stays false at5595F0.
 # 5595F2/F3/F5/F6 therefore have no stable-domain execution, not test coverage.
 # 5596D0:ESI=0 at559750;559752 increments to1..8, hence55975A alwaysJG.
 # Also event EAX=count-1..0, nonzero mapped pointers, and published list items
 # are1..35 /1..8: the signed/range/null guards and final item==0 paths exclude
 # the nine conditional outcomes below without manufacturing alias mutations.
 excluded={0x5595f2,0x5595f3,0x5595f5,0x5595f6,0x55975c,0x55975d,0x559760,0x559762}
 assert primary-hits==excluded,sorted(hex(a) for a in primary-hits)
 excluded_edges={(0x5594a5,0x5594db),(0x5594a9,0x5594db),(0x5594b0,0x5594db),(0x5595f0,0x5595f2),(0x55975a,0x55975c),(0x559760,0x559762),(0x559760,0x5597b6),(0x5597cf,0x5597e1),(0x559802,0x559814)}
 assert {(a,t) for a,dest in branches.items() for t in dest if (a,t) not in edges}==excluded_edges
 print('280/288 owned instructions executed;8 stable-domain invariant exclusions,9 conditional outcome exclusions. Preserved C blocks remain unexecuted; no forced helper results or whole-helper branch-coverage claim.')
