"""Complete native publisher/caller/initializer, loader and existence helper versus handwritten C."""
from pathlib import Path
from datetime import datetime,timezone
import argparse,hashlib,itertools,json,random,struct,subprocess,sys
ROOT=Path(__file__).resolve().parents[1];sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
parser=argparse.ArgumentParser();parser.add_argument('--exe',type=Path,default=ROOT/'build/agent-event-publisher/out/Debug/restore_event_publisher_verify.exe');parser.add_argument('--debug-limit',type=int);args=parser.parse_args();exe=args.exe.resolve()
sha=lambda b:hashlib.sha256(b).hexdigest()
data=(ROOT/'build/smoke-game/Pharaoh.exe').read_bytes();digest=sha(data);assert digest=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
u16=lambda b,p:struct.unpack_from('<H',b,p)[0];u32=lambda b,p:struct.unpack_from('<I',b,p)[0]
pe=u32(data,60);opt=pe+24;table=opt+u16(data,pe+20);base=u32(data,opt+28);size=u32(data,opt+56)
vm=Uc(UC_ARCH_X86,UC_MODE_32);vm.mem_map(base,(size+4095)&~4095)
for i in range(u16(data,pe+6)):
 q=table+i*40;vm.mem_write(base+u32(data,q+12),data[u32(data,q+20):u32(data,q+20)+u32(data,q+16)])
BASE=0x30000000;TEXT=BASE+0x10000;MM=BASE+0x100000;ALT=BASE+0x200000;STOP=0x33000000;STACK=0x34000000
for a,n in [(BASE,0x300000),(STOP,0x10000),(STACK,0x200000)]:vm.mem_map(a,n)
def write(a,v,n=4):vm.mem_write(a,(v&((1<<(8*n))-1)).to_bytes(n,'little'))
def read(a):return int.from_bytes(vm.mem_read(a,4),'little')
def byte(a):return vm.mem_read(a,1)[0]
regions=[(0x5c7558,16),(0x7027b0,48),(0x78d7b8,16),(0xd3a6e0,0xb00),(0xe36c68,24),(0xe38dc0,0xd0),(0xe95070,0x1a0),(0xe95220,48016),(0xea0da0,16),(0xf54110,16)]
file_names=['build/debug-startup-game/Pharaoh_Text.eng','build/debug-startup-game/Pharaoh_MM.eng'];files=[b'']+[(ROOT/f).read_bytes() for f in file_names];lengths=list(map(len,files));tutorial=[read(0x5794c4+i*28) for i in range(26)]
# Instruction exports are independently authenticated against the pinned PE bytes.
exports=['metadata/event-publisher-native.json','metadata/event-publisher-support-native.json','metadata/message-panel-support-native.json','metadata/native-zero-helper-47d350.json']
byte_audit={};ranges=[]
for path in exports:
 d=json.loads((ROOT/path).read_text());functions=d.get('functions',{})
 if not functions:continue
 for entry,f in functions.items():
  if entry.lower() not in ['004e0cf0','004ce3f0','00417ae0','004cec40','004e14b0','004e1750','004c5bf0','0047d350']:continue
  ins=f['assembly']['items'];assert not f['assembly'].get('has_more',False)
  for x in ins:assert bytes(vm.mem_read(int(x['address'],16),len(bytes.fromhex(x['bytes']))))==bytes.fromhex(x['bytes']),path+':'+x['address']
  ranges.append((int(ins[0]['address'],16),int(ins[-1]['address'],16)+len(bytes.fromhex(ins[-1]['bytes']))));byte_audit[entry]={'instructions':len(ins),'bytes':sum(len(bytes.fromhex(x['bytes'])) for x in ins)}
assert bytes(vm.mem_read(0x47d350,3))==bytes.fromhex('33c0c3');ranges.append((0x47d350,0x47d353));byte_audit.setdefault('0047d350',{'instructions':2,'bytes':3})
counts={0x4e0f90:2,0x53b090:1,0x4ca980:0,0x4c8040:0,0x417a70:0,0x404350:0,0x4051a0:0,0x417aa0:0,0x4179f0:0,0x51e990:0,0x47d350:1,0x568eab:2,0x5647eb:1,0x56b1b6:1,0x561a1d:1,0x561a2b:1,0x5660ed:3,0x4ce100:1,0x4ce2d0:0}
events=[];p=[];message=open_count=alloc_count=0;positions=[0,0,0];reached=set()
probes={0x4e0d04:'forced immediate',0x4e0d12:'filter full EAX',0x4e0d34:'return from modal mode',0x4e0d47:'full event pool guard',0x4e0d60:'new message BYTE',0x4e0d7a:'WORD message',0x4e0d88:'WORD signed year',0x4e0d95:'BYTE month',0x4e0db6:'WORD sequence',0x4e0dd6:'absent template WORDs',0x4e0dfe:'seven forward MOVSD',0x4e0e00:'final MOVSW',0x4e0e17:'actual action initializer',0x4e0e43:'subtype1/7 state',0x4e0ec7:'pre-file refresh',0x4e0ed2:'reload payload pointer',0x4e0ee3:'file result full EAX',0x4e0f07:'media available DWORD',0x4e0f1b:'actual setup argument reload',0x4e0f3e:'queued signed sequence',0x4e0f3c:'queue full',0x4e0f54:'nonimmediate prior flag',0x4e1769:'1000 event slots full',0x417b24:'28byte tutorial record',0x4cec5b:'first open failure',0x4cece2:'second open failure',0x4ced25:'MM payload pointer write',0x4ced2a:'24+1000*80 header read',0x4ce441:'existence retry restoration',0x4e14cd:'action4',0x4e14d8:'action1',0x4e14e3:'action8',0x4e14ee:'action5',0x4e14f9:'action6',0x4e1504:'action0'}
def code(cpu,a,n,user):
 global open_count,alloc_count
 if a==STOP:return
 if a in probes:reached.add(a)
 if a not in counts:
  assert any(lo<=a and a+n<=hi for lo,hi in ranges),'Native escaped '+hex(a)
  return
 sp=cpu.reg_read(UC_X86_REG_ESP);argv=[read(sp+4+i*4) for i in range(counts[a])];row=[a,len(argv)]+argv+[0]*(16-2-len(argv));row[8:14]=[read(0xe950dc),byte(0xe38dda),read(0xe951ec),read(0xe951e8),read(0xe36c74),read(0xd3a6e8)];events.append(row)
 result=0
 if a==0x47d350:return # actual authenticated XOR EAX,EAX;RET
 if a==0x4e0f90:result=p[5]
 elif a==0x4ca980:
  if p[16]&1:write(0xe36c74,ALT)
 elif a==0x4c8040:vm.mem_write(0xd3a6f8,bytes(1000));write(0xd3a6ec,0);write(0xd3a6f0,0)
 elif a==0x568eab:
  open_count+=1
  if p[0] in [4,6]:
   h=1 if argv[0]==BASE+0x100 else 2
   if (p[14]==1 and h==1) or (p[14]==2 and h==2):result=0xffffffff
   else:positions[h]=0;result=h
  else:result=0xffffffff if p[14]==1 or(p[14]==2 and open_count==1) else 3
 elif a==0x56b1b6:result=lengths[argv[0]]
 elif a==0x561a1d:alloc_count+=1;result=TEXT if alloc_count==1 else MM;assert argv[0]<0xe0000
 elif a==0x5660ed:
  h,buffer,length=argv;start=positions[h];blob=files[h][start:start+length];assert len(blob)==length;vm.mem_write(buffer,blob);positions[h]+=length;result=length
 elif a==0x4ce100:
  if p[16]&2:write(0xe951dc,message+1)
  result=1
 cpu.reg_write(UC_X86_REG_EAX,result&0xffffffff);cpu.reg_write(UC_X86_REG_EIP,read(sp));cpu.reg_write(UC_X86_REG_ESP,sp+4)
vm.hook_add(UC_HOOK_CODE,code)
def invoke(entry,argv):
 sp=STACK+0x1e0000;write(sp,STOP)
 for i,v in enumerate(argv):write(sp+4+i*4,v)
 saved=[(UC_X86_REG_EBX,0x11111111),(UC_X86_REG_EBP,0x22222222),(UC_X86_REG_ESI,0x33333333),(UC_X86_REG_EDI,0x44444444)]
 for r,v in saved:vm.reg_write(r,v)
 vm.reg_write(UC_X86_REG_ESP,sp);vm.reg_write(UC_X86_REG_EFLAGS,2);vm.emu_start(entry,STOP,count=200000)
 assert vm.reg_read(UC_X86_REG_EIP)==STOP,'Native budget/return differs'
 assert vm.reg_read(UC_X86_REG_ESP)==sp+4,'Native stack differs'
 assert all(vm.reg_read(r)==v for r,v in saved),'Native saved register differs'
 return vm.reg_read(UC_X86_REG_EAX)
def native(params):
 global p,message,open_count,alloc_count
 p=params;events.clear();open_count=alloc_count=0;positions[:]=[0,0,0];op,ident,slot,mode,force,filter,prior,template,seq,total,year,month,subtype,priority,filemode,queue,mutation,arg0,arg1,pattern,oldalloc,index,_,_=p
 vm.mem_write(BASE,b'\xa5'*0x300000)
 for a,s in [(BASE+0x100,b'Pharaoh_Text.eng'),(BASE+0x200,b'Pharaoh_MM.eng'),(MM+0x400,b'fixture.smk'),(ALT+0x400,b'alternate.smk')]:vm.mem_write(a,s+b'\0')
 for a,n in regions:vm.mem_write(a,b'\xa5'*n)
 vm.mem_write(0xe34c30,b'\xa5'*8036);message=tutorial[index] if op in [1,6] else ident
 vm.mem_write(0xe20998,b'\xa5'*0x138a8)
 if message<1000:
  r=0xe209b8+message*80;write(r+2,subtype,2);write(r+56,priority);write(r+60,0x400);write(r+140,0)
 vm.mem_write(0xe95228, (b"\xa5"*10+b"\x01\x00"+b"\xa5"*36)*1000)
 if slot<1000:write(0xe95232+slot*48,0,2)
 for a,v in [(0xe950dc,0x5a010000),(0xe950e0,prior),(0xe951f0,seq),(0xe951ec,total),(0xe951e8,0x12345678),(0xe36c70,BASE+0x4000 if oldalloc else 0),(0xe36c74,(BASE+0x5000 if oldalloc else 0) if op in [4,6] else MM)]:write(a,v)
 write(0xe38dda,mode,1);write(0x78d7bc,year,2);write(0x78d7c0,month,1)
 for i in range(20):write(0xe95080+i*4,0x100+i)
 if queue<20:write(0xe95080+queue*4,0)
 param=0
 if template:
  param=BASE+0x800
  if template>=3 and slot<1000:param=0xe95228+slot*48+(12 if template==3 else 18)
  vm.mem_write(param,bytes((pattern+i*11)&255 for i in range(30)));write(param,0xffff if template==2 else 17,2)
 result=0
 if op==0:invoke(0x4e0cf0,[message,arg0,arg1,param,force])
 elif op==1:invoke(0x417ae0,[index])
 elif op==2:invoke(0x4e14b0,[ident])
 elif op==3:result=invoke(0x4e1750,[])
 elif op==4:result=invoke(0x4cec40,[BASE+0x100,BASE+0x200])
 elif op==5:result=invoke(0x4ce3f0,[MM+0x400])&255
 elif op==6:
  result=invoke(0x4cec40,[BASE+0x100,BASE+0x200])
  if result:invoke(0x417ae0,[index])
 state=b''.join(bytes(vm.mem_read(a,n)) for a,n in regions)
 if op!=2:state+=bytes(vm.mem_read(0xe20998,0x138a8))
 state+=bytes(vm.mem_read(BASE+0x7f8,48))
 if op in [4,6]:state+=bytes(vm.mem_read(0xe34c30,8036))+bytes(vm.mem_read(TEXT-8,lengths[1]-0x1f5c+16))+bytes(vm.mem_read(MM-8,lengths[2]-0x13898+16))
 return {'result':result,'state':state.hex(),'events':events.copy()}
fields=['op','ident','slot','mode','force','filter','prior','template','seq','total','year','month','subtype','priority','filemode','queue','mutation','arg0','arg1','pattern','oldalloc','index','spare0','spare1']
default=[0,249,0,1,0,1,1,0,1,0,-1500,7,0,0,0,20,0,0x12345678,0xabcdef98,17,0,15,0,0];cases=[]
def add(**kw):
 q=[v&0xffffffff for v in default]
 for k,v in kw.items():q[fields.index(k)]=v&0xffffffff
 cases.append(q)
for op in range(7):add(op=op)
for slot in [0,1,255,256,999,1000]:add(op=3,slot=slot)
for filemode,oldalloc in itertools.product([0,1,2],[0,1]):add(op=4,filemode=filemode,oldalloc=oldalloc)
for index,mode,filter in itertools.product(range(26),[1,5],[0,1,256]):add(op=1,index=index,mode=mode,filter=filter)
for index in range(26):add(op=6,index=index,oldalloc=index&1)
for ident in list(range(0x70,0x13c))+[0,1,0x7fffffff,0x80000000,0xffffffff]:add(op=2,ident=ident)
for mode,force,filter,slot in itertools.product([0,1,5,30,31,32,48,73],[0,1,128],[0,1,256],[0,256,999,1000]):add(mode=mode,force=force,filter=filter,slot=slot)
for template,seq,queue,mode in itertools.product(range(5),[0,0x7fff,0x8000,0xffff,0x10000,0xffffffff],[0,1,19,20],[1,5]):add(template=template,seq=seq,total=0xffffffff,queue=queue,mode=mode,slot=256)
for subtype,priority,filemode,mutation in itertools.product([0,1,7,256],[0,1,256],[0,1,2],[0,1,2,3]):add(subtype=subtype,priority=priority,filemode=filemode,mutation=mutation,template=1)
for prior,priority in itertools.product([0,1,256],[0,1,256]):add(filter=0,prior=prior,priority=priority)
for filemode in [0,1,2]:add(op=5,filemode=filemode)
rng=random.Random(0x4e0cf0)
for _ in range(300):add(ident=rng.randrange(1,497),slot=rng.choice([0,1,255,256,999,1000]),mode=rng.choice([1,5,30,31,32,73]),force=rng.choice([0,1,128]),filter=rng.choice([0,1,256,0xffffffff]),prior=rng.choice([0,1,256]),template=rng.randrange(5),seq=rng.getrandbits(32),total=rng.getrandbits(32),year=rng.choice([-32768,-1500,0,32767]),month=rng.randrange(256),subtype=rng.choice([0,1,7,256]),priority=rng.choice([0,1,256]),filemode=rng.randrange(3),queue=rng.randrange(21),mutation=rng.randrange(4),arg0=rng.getrandbits(32),arg1=rng.getrandbits(32),pattern=rng.randrange(256))
if args.debug_limit:cases=cases[:args.debug_limit]
smoke=subprocess.run([str(exe)],cwd=ROOT,capture_output=True,text=True,timeout=30);assert smoke.returncode==0,smoke.stdout+smoke.stderr
process=subprocess.Popen([str(exe),'--oracle-batch'],cwd=ROOT,stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE)
report={'format':1,'generated_at':datetime.now(timezone.utc).isoformat(),'native_sha256':digest,'compiled_C_path':str(exe.relative_to(ROOT)).replace('\\','/'),'compiled_C_sha256':sha(exe.read_bytes()),'original_game_process_executed':False,'computer_UI_automation_used':False,'original_machine_code_fallback_used':False,'method':'Complete native170instruction publisher,24instruction tutorial caller,19instruction action initializer,10instruction slot scan,87instruction language loader,34instruction file existence helper,47instruction panel setup and authenticated zero helper in bounded Unicorn versus compiled C; exact ordered deep endpoint arguments/state, complete event+message tables, scalar width guard bytes, actual language-file payloads, native stack and saved registers compared.','limitations':['Immediate-event filter004e0f90, refresh004ca980, tutorial preparation helpers, alert0053b090, link clear004c8040, file fallback004ce100/004ce2d0 and CRT open/read/close/allocate/free are controlled endpoints. OS device behavior, deep gameplay changes and downstream full panel rendering remain separate dependencies.','Valid publisher IDs reference allocated80byte records; the native body has no ID bounds guard. Malformed language files and failed allocation invariants are outside this equivalence domain. Both native loader open failures and replacement allocations are included.','Publisher return EAX is unused; file helper return is known0/1 and its consumed boolean is compared. Slot getter full EAX, filter full EAX, month BYTE and signed year/sequence WORD promotions are included.','No completed-city-frame or game survival claim follows from this isolated proof; root integration must replay the preserved crash.']}
checked=0;case=None
try:
 assert json.loads(process.stdout.readline())=={'fields':24}
 for case in cases:
  process.stdin.write((' '.join(map(str,case))+'\n').encode());process.stdin.flush();line=process.stdout.readline();assert line,'C exited: '+process.stderr.read().decode(errors='replace');actual=json.loads(line);expected=native(case)
  for k in expected:
   if actual[k]!=expected[k]:
    detail=[actual[k],expected[k]]
    if k=='state':
     aa=bytes.fromhex(actual[k]);bb=bytes.fromhex(expected[k]);ix=next((i for i,(a,b) in enumerate(zip(aa,bb)) if a!=b),None);detail={'first_difference':ix,'C_bytes':len(aa),'native_bytes':len(bb),'C_slice':aa[max(0,(ix or 0)-8):(ix or 0)+16].hex(),'native_slice':bb[max(0,(ix or 0)-8):(ix or 0)+16].hex()}
    raise AssertionError(k+' differs '+str(detail))
  checked+=1
  if checked%250==0:print("checked",checked,file=sys.stderr,flush=True)
 if not args.debug_limit:assert set(probes)<=reached,'Missing native probes '+str([hex(a)+':'+probes[a] for a in set(probes)-reached])
 report.update(passed=True,checked=checked,complete_case_set=not bool(args.debug_limit))
except Exception as error:report.update(passed=False,checked=checked,case=case,error=str(error),native_eip=hex(vm.reg_read(UC_X86_REG_EIP)))
finally:process.stdin.close();process.wait(timeout=10)
report['branch_probes']={hex(a):{'name':name,'reached':a in reached} for a,name in probes.items()};report['native_byte_audit']=byte_audit;report['native_instruction_count']=sum(x['instructions'] for x in byte_audit.values());report['native_bytes_verified']=sum(x['bytes'] for x in byte_audit.values());report['smoke']={'passed':True,'output':smoke.stdout.strip()}
report['source_hashes']={s:sha((ROOT/s).read_bytes()) for s in ['include/event_publisher.h','include/message_panel.h','include/sprite_libraries.h','tests/event_publisher.c','tools/verify_event_publisher_emulation.py']+[f'src/unit_{n:05d}.c' for n in [211,1911,1914,1917,1759,1747,1619,1130]]};report['native_exports']={s:sha((ROOT/s).read_bytes()) for s in exports};report['fixture_hashes']={s:sha((ROOT/s).read_bytes()) for s in file_names};report['tutorial_records']=[{'index':i,'message':v,'stride':28} for i,v in enumerate(tutorial)]
name='event-publisher-debug-emulation.json' if args.debug_limit else 'event-publisher-emulation.json';(ROOT/'metadata'/name).write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report,indent=2));sys.exit(0 if report['passed'] else 1)
