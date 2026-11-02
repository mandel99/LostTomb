"""Optional bounded native/C sound-state comparison; no audio or game process."""
from pathlib import Path
import hashlib,json,random,struct,subprocess,sys,time
ROOT=Path(__file__).resolve().parent.parent
sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_BLOCK,UC_HOOK_CODE,__version__ as unicorn_version
from unicorn.x86_const import *
image=(ROOT/'build/smoke-game/Pharaoh.exe').read_bytes()
assert hashlib.sha256(image).hexdigest()=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
u16=lambda b,p:struct.unpack_from('<H',b,p)[0]
u32=lambda b,p:struct.unpack_from('<I',b,p)[0]
pe=u32(image,0x3c);opt=pe+24;table=opt+u16(image,pe+20);base=u32(image,opt+28);size=u32(image,opt+56)
vm=Uc(UC_ARCH_X86,UC_MODE_32);vm.mem_map(base,(size+4095)&~4095);vm.mem_write(base,image[:u32(image,opt+60)])
for i in range(u16(image,pe+6)):
 p=table+i*40;vm.mem_write(base+u32(image,p+12),image[u32(image,p+20):u32(image,p+20)+u32(image,p+16)])
OBJECT=0x30000000;HEAP=0x31000000;STOP=0x33000000;STUBS=STOP+0x1000;STACK=0x34000000
for a,n in [(0,0x1000),(OBJECT,0x1000),(HEAP,0x200000),(STOP,0x10000),(STACK,0x10000)]:vm.mem_map(a,n)
def read(a,n=4):return int.from_bytes(vm.mem_read(a,n),'little')
def write(a,v,n=4):vm.mem_write(a,(v&((1<<(n*8))-1)).to_bytes(n,'little'))
def cstring(a):return bytes(vm.mem_read(a,300)).split(b'\0',1)[0]
def fnv(text):
 value=2166136261
 for b in text:value=((value^b)*16777619)&0xffffffff
 return value
imports={0x56f124:1,0x56f0f8:1,0x56f2dc:0,0x56f2e0:2,0x56f2e4:4,0x56f2f4:1,0x56f2f0:1,0x56f2cc:1,0x56f2c8:1,0x56f308:1}
endpoints={0x561a1d:0x561a1d,0x561a2b:0x561a2b,0x561bdc:0x561bdc,0x563240:0x563240,0x561441:0x561441,0x561363:0x561363}
for i,(slot,nargs) in enumerate(imports.items()):endpoints[STUBS+i*16]=slot;write(slot,STUBS+i*16)
RANGES=[(0x55dbc0,0x55dbf7),(0x55dc18,0x55dd3e),(0x55dea2,0x55e14e),(0x55e38b,0x55e589),(0x55e5d2,0x55e92a),
 (0x55eb99,0x55ec0e),(0x55ec7c,0x55ecf2),(0x560210,0x5602e2)]
state={}
def event(a,args):state['events'].append([a,len(args)]+args+[0]*(4-len(args)))
def guard(cpu,a,n,user):
 if a not in endpoints and not any(lo<=a and a+n-1<=hi for lo,hi in RANGES):raise RuntimeError('Native instruction escaped at '+hex(a))
def endpoint(cpu,a,n,user):
 sp=cpu.reg_read(UC_X86_REG_ESP);args=[read(sp+4+i*4) for i in range(5)];ident=endpoints[a];value=0;cleanup=0
 if ident in imports:
  cleanup=imports[ident]*4;args=args[:imports[ident]]
  if ident==0x56f124:
   assert read(args[0])==0x94;write(args[0]+4,state['p'][3]);write(args[0]+16,state['p'][2]);args=[];value=state['p'][4]
  elif ident==0x56f0f8:args=[fnv(cstring(args[0]))]
  elif ident==0x56f2dc:value=1
  elif ident==0x56f2e4:write(args[0],0x70123456);value=state['p'][5]
  elif ident==0x56f2f4:state['handles']+=1;value=0x70000000+state['handles']
  event(ident,args)
 elif ident==0x561a1d:
  event(ident,args[:1]);value=state['cursor'];state['cursor']+=(args[0]+15)&~15
  assert state['cursor']<HEAP+0x200000;vm.mem_write(value,b'\xcd'*args[0])
 elif ident in [0x561a2b,0x561bdc]:event(ident,args[:1])
 elif ident==0x563240:vm.mem_write(args[0],bytes([args[1]&255])*args[2]);value=args[0]
 elif ident==0x561441:
  assert args[1]==8 and args[3:5]==[0x560280,0x5602b0];vm.mem_write(args[0],b'\0'*(args[1]*args[2]));cleanup=20
 elif ident==0x561363:
  assert args[1]==8 and args[3]==0x5602b0
  for i in range(args[2]-1,-1,-1):
   address=args[0]+i*8
   if read(address+4) and read(address):event(0x561bdc,[read(address)]);write(address,0)
  cleanup=16
 cpu.reg_write(UC_X86_REG_EAX,value);cpu.reg_write(UC_X86_REG_EIP,read(sp));cpu.reg_write(UC_X86_REG_ESP,sp+4+cleanup)
vm.hook_add(UC_HOOK_BLOCK,guard)
for address in endpoints:vm.hook_add(UC_HOOK_CODE,endpoint,begin=address,end=address)
saved=[(UC_X86_REG_EBX,0x11111111),(UC_X86_REG_EBP,0x22222222),(UC_X86_REG_ESI,0x33333333),(UC_X86_REG_EDI,0x44444444)]
def call(entry,args):
 sp=STACK+0x8000
 for i,value in enumerate([STOP]+args):write(sp+i*4,value)
 for reg,value in saved:vm.reg_write(reg,value)
 vm.reg_write(UC_X86_REG_ECX,OBJECT);vm.reg_write(UC_X86_REG_ESP,sp);vm.reg_write(UC_X86_REG_EFLAGS,2)
 vm.emu_start(entry,STOP,count=2000000)
 assert vm.reg_read(UC_X86_REG_EIP)==STOP,'Instruction limit exhausted'
 assert vm.reg_read(UC_X86_REG_ESP)==sp+4+len(args)*4,'thiscall cleanup differs'
 assert all(vm.reg_read(r)==v for r,v in saved),'Saved register differs'
 return vm.reg_read(UC_X86_REG_EAX)
def native(p):
 global state
 state={'p':p,'events':[],'cursor':HEAP+0x10000,'handles':0}
 vm.mem_write(OBJECT,bytes([p[1]&255])*0x204);vm.mem_write(HEAP,b'\x5a'*0x200000);write(0x12663b0,0x10203040);write(0,0xffffffff)
 call(0x55e38b,[])
 if p[0]:
  for off,value in [(0x18c,p[8]),(0x190,p[9]),(0x194,p[10]),(0x200,p[13]),(0x178,0x70123456),(0x198,0),(0x1e4,0)]:write(OBJECT+off,value)
  if p[7] and p[0]!=3:
   for off,value in [(0x198,2),(0x19c,HEAP+0x1000),(0x1a0,HEAP+0x1100),(0x1a8,HEAP+0x1200),(0x1ac,HEAP+0x1400),(0x1e4,2),(0x1f0,HEAP+0x1700)]:write(OBJECT+off,value)
   for i in range(2):
    write(HEAP+0x1000+i*4,0x71000000+i);write(HEAP+0x1400+i*12,20+i);write(HEAP+0x1404+i*12,0x32000000+i);write(HEAP+0x1408+i*12,1)
    write(HEAP+0x1700+i*4,0x72000000+i)
  if p[7] and p[0]==3:
   for off,value in [(0,3),(4,2),(8,HEAP+0x1804)]:write(OBJECT+off,value)
   for i,value in enumerate([3,0x32000080,1,0x32000100,0,0,1]):write(HEAP+0x1800+i*4,value)
  entries={1:(0x55e6c4,[]),2:(0x55dea2,[p[6]]),3:(0x55dc63,[p[6]]),4:(0x55ec7c,[]),5:(0x55e5d2,[p[15]])}
  value=call(*entries[p[0]])
 else:value=1
 if p[0] in [4,5]:value=0
 return {'result':value,'object':bytes(vm.mem_read(OBJECT,0x204)).hex(),'counter':read(0x12663b0),'owner':read(0x12663ac),
  'heap':bytes(vm.mem_read(HEAP+0x10000,state['cursor']-HEAP-0x10000)).hex(),'old_heap':bytes(vm.mem_read(HEAP+0x1000,0x1000)).hex(),'events':state['events']}
default=[1,0,2,10,1,0,5,0,22050,16,2,0,0,0,0,0];cases=[]
def add(**kw):
 p=default.copy()
 for k,v in kw.items():p[int(k[1:])]=v&0xffffffff
 cases.append(p)
for poison in [0,0x5a,0xa5,0xff]:add(p0=0,p1=poison)
for platform in [0,1,2,3]:
 for major in [0,4,5,10]:
  for version in [0,1]:
   for wave in [0,1]:add(p2=platform,p3=major,p4=version,p5=wave)
for rate in [0,8000,22050,44100,48000,96000,0xffffffff]:
 for bits in [0,7,8,16,24,32,0xfffffff0]:
  for channels in [0,1,2,6,0xffffffff]:add(p8=rate,p9=bits,p10=channels)
for count in [-1,0,1,2,5,16]:
 for old in [0,1]:
  for disabled in [0,1]:add(p0=2,p6=count,p7=old,p13=disabled)
for count in [-1,0,1,3,17,500]:
 for old in [0,1]:add(p0=3,p6=count,p7=old)
for old in [0,1]:
 for disabled in [0,1]:add(p0=4,p7=old,p13=disabled)
for channel in [0,1]:add(p0=5,p7=1,p15=channel)
exe=ROOT/'build/c-only-debug/Debug/restore_sound_verify.exe'
process=subprocess.Popen([str(exe),'--oracle-batch'],cwd=ROOT,stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True)
report={'format':1,'unicorn_version':unicorn_version,'original_game_executed':False,'audio_output_used':False,
 'method':'Compiled C versus bounded original constructor, PCM initialization, sample-pool allocation/reset/stop and file-table allocation; media, OS, allocator and vector-lifecycle endpoints controlled',
 'compared':['complete 0x204-byte object','new and old heap bytes','global receiver and DWORD allocation counter','ordered endpoint arguments','value returns and thiscall stack/register preservation'],
 'limitations':['C failure cleanup intentionally differs from native partial-allocation and sample-handle failure behavior; these cases are verified by the source-only test, not claimed equivalent.',
  'C rejects allocation arithmetic overflow. Native 3D initialization, full shutdown and file lookup remain separate reconstruction work.',
  'Constructor/void result markers are normalized; object bytes, owner pointer and ABI are compared. No actual sound device, media library or game process is used in this comparison.'],
 'compiled_C_sha256':hashlib.sha256(exe.read_bytes()).hexdigest(),'original_sha256':hashlib.sha256(image).hexdigest()}
checked=0;case=None;started=time.monotonic()
try:
 assert json.loads(process.stdout.readline())=={'object':OBJECT,'heap':HEAP}
 for case in cases:
  process.stdin.write(' '.join(map(str,case))+'\n');process.stdin.flush();actual=json.loads(process.stdout.readline());expected=native(case)
  if actual!=expected:
   report['differences']={k:{'C':actual[k] if k not in ['object','heap','old_heap'] else '(see bytes)','native':expected[k] if k not in ['object','heap','old_heap'] else '(see bytes)'} for k in actual if actual[k]!=expected[k]}
   for k in ['object','heap','old_heap']:
    if actual[k]!=expected[k]:report[k+'_first_differences']=[{'offset':i,'C':a,'native':b} for i,(a,b) in enumerate(zip(bytes.fromhex(actual[k]),bytes.fromhex(expected[k]))) if a!=b][:16]
   raise AssertionError('State, heap, return or endpoint arguments differ')
  checked+=1
 report.update(passed=True,checked=checked)
except Exception as error:report.update(passed=False,checked=checked,case=case,error=str(error),native_eip=hex(vm.reg_read(UC_X86_REG_EIP)))
finally:
 process.stdin.close();process.wait(timeout=10)
report['elapsed_seconds']=round(time.monotonic()-started,3)
files=['src/unit_02689.c','src/unit_02691.c','src/unit_02692.c','src/unit_02687.c','src/unit_02697.c','src/unit_02685.c','include/sound_initialization.h','tests/sound_initialization.c','tools/verify_sound_initialization_emulation.py']
report['source_hashes']={f:hashlib.sha256((ROOT/f).read_bytes()).hexdigest() for f in files}
(ROOT/'metadata/sound-initialization-emulation.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({k:v for k,v in report.items() if k!='source_hashes'},indent=2));sys.exit(0 if report['passed'] else 1)
