"""Verify native file enumeration, growth and original sorting against independent C."""
from pathlib import Path
import hashlib,itertools,json,struct,subprocess,sys
ROOT=Path(__file__).resolve().parent.parent;sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_BLOCK,UC_HOOK_CODE
from unicorn.x86_const import *
data=(ROOT/'build/smoke-game/Pharaoh.exe').read_bytes();digest=hashlib.sha256(data).hexdigest()
assert digest=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
u16=lambda b,p:struct.unpack_from('<H',b,p)[0];u32=lambda b,p:struct.unpack_from('<I',b,p)[0]
pe=u32(data,0x3c);opt=pe+24;sections=opt+u16(data,pe+20);base=u32(data,opt+28);size=u32(data,opt+56)
vm=Uc(UC_ARCH_X86,UC_MODE_32);vm.mem_map(base,(size+4095)&~4095)
for i in range(u16(data,pe+6)):
 p=sections+i*40;vm.mem_write(base+u32(data,p+12),data[u32(data,p+20):u32(data,p+20)+u32(data,p+16)])
BASE=0x30000000;STACK=0x34000000;STOP=0x33000000
vm.mem_map(BASE,0x50000);vm.mem_map(STACK,0x10000);vm.mem_map(STOP,0x1000)
def write(a,v):vm.mem_write(a,struct.pack('<I',v&0xffffffff))
def read(a):return int.from_bytes(vm.mem_read(a,4),'little')
def string(a):
 out=bytearray()
 for i in range(1024):
  c=bytes(vm.mem_read(a+i,1))
  if c==b'\0':return bytes(out)
  out+=c
 raise AssertionError('Unterminated native string')
imports={};descriptor=base+u32(data,opt+104)
while read(descriptor+12):
 names=base+(read(descriptor) or read(descriptor+16));iat=base+read(descriptor+16);i=0
 while read(names+i*4):
  rva=read(names+i*4)
  if not rva&0x80000000:imports[string(base+rva+2).decode('ascii')]=iat+i*4
  i+=1
 descriptor+=20
entries={BASE+0x3f100:('first',2,True),BASE+0x3f110:('next',2,True),BASE+0x3f120:('close',1,True),
 0x561a1d:('allocate',1,False),0x561a2b:('free',1,False),0x562370:('compare',2,False)}
for name,address in [('FindFirstFileA',BASE+0x3f100),('FindNextFileA',BASE+0x3f110),('FindClose',BASE+0x3f120)]:write(imports[name],address)
events=[];files=index=allocation=0
def name(out):
 vm.mem_write(out,b'\0'*320);vm.mem_write(out+44,('Family%03u.dat'%(files-index-1)).encode()+b'\0')
def endpoint(cpu,a,n,user):
 global index,allocation
 sp=cpu.reg_read(UC_X86_REG_ESP);kind,count,stdcall=entries[a];args=[read(sp+4+i*4) for i in range(count)];result=1
 if kind=='first':
  assert string(args[0])==b'Save\\*.dat';index=0;events.append([1,files]);result=0x1234 if files else 0xffffffff
  if files:name(args[1])
 elif kind=='next':
  assert args[0]==0x1234;index+=1;events.append([2,index]);result=int(index<files)
  if result:name(args[1])
 elif kind=='close':assert args[0]==0x1234;events.append([3,0x1234])
 elif kind=='allocate':
  events.append([4,args[0]]);result=BASE+0x40000+allocation;allocation=(allocation+args[0]+63)&~63
  assert allocation<0x10000
 elif kind=='free':events.append([5,args[0]])
 elif kind=='compare':
  left,right=string(args[0]).lower(),string(args[1]).lower();result=(left>right)-(left<right)
 cpu.reg_write(UC_X86_REG_EAX,result&0xffffffff);cpu.reg_write(UC_X86_REG_EIP,read(sp));cpu.reg_write(UC_X86_REG_ESP,sp+4+(count*4 if stdcall else 0))
def guard(cpu,a,n,user):
 if a not in entries and not any(lo<=a and a+n<=hi for lo,hi in [(0x41c740,0x41ca80),(0x562069,0x562240)]):
  raise RuntimeError('Native file-list escaped at '+hex(a))
vm.hook_add(UC_HOOK_BLOCK,guard)
for a in entries:vm.hook_add(UC_HOOK_CODE,endpoint,begin=a,end=a)
def call(entry,args):
 sp=STACK+0x8000
 for i,v in enumerate([STOP]+args):write(sp+i*4,v)
 saved=[(UC_X86_REG_EBX,0x11111111),(UC_X86_REG_EBP,0x22222222),(UC_X86_REG_ESI,0x33333333),(UC_X86_REG_EDI,0x44444444)]
 for r,v in saved:vm.reg_write(r,v)
 vm.reg_write(UC_X86_REG_ESP,sp);vm.reg_write(UC_X86_REG_ECX,BASE);vm.reg_write(UC_X86_REG_EFLAGS,2)
 vm.emu_start(entry,STOP,count=1000000)
 assert vm.reg_read(UC_X86_REG_EIP)==STOP and vm.reg_read(UC_X86_REG_ESP)==sp+4+4*len(args),'Native stack/budget differs'
 assert all(vm.reg_read(r)==v for r,v in saved)
 return vm.reg_read(UC_X86_REG_EAX)
regions=[(BASE,28),(BASE+0x1000,2048),(BASE+0x5000,16384),(BASE+0x40000,8192),(0x7027dc,4)]
def native(p):
 global files,index,allocation
 files,capacity,string_capacity,append,op,initial=p;index=allocation=0;events.clear();vm.mem_write(BASE,b'\xcc'*0x50000)
 vals=[0x56f548,BASE+0x1000 if capacity else 0,initial,capacity,BASE+0x5000 if string_capacity else 0,string_capacity,initial*6]
 vm.mem_write(BASE,struct.pack('<7I',*vals));write(0x7027dc,0);write(0x704db0,0)
 for i in range(initial):write(BASE+0x1000+i*4,i*6);vm.mem_write(BASE+0x5000+i*6,('old%02u'%i).encode()+b'\0')
 pattern=BASE+0x3f000;vm.mem_write(pattern,b'Save\\*.dat\0')
 if op==1:
  result=call(0x41c8f0,[pattern,BASE+0x3000,BASE+0x3004])&255
  events.extend([[7,read(BASE+0x3000)],[8,read(BASE+0x3004)]])
 else:
  result=call(0x41c740,[pattern,append])&255
  if op==2:call(0x41ca10,[0x562370]);result=0
 # A native reused-list growth path deletes one old offsets allocation twice.
 # The reconstruction deliberately removes only the second deletion.
 filtered=[];freed=set();removed=[]
 for event in events:
  if event[0]==5:
   if event[1] in freed:removed.append(event);continue
   freed.add(event[1])
  filtered.append(event)
 return {'result':result,'bytes':b''.join(bytes(vm.mem_read(a,n)) for a,n in regions).hex(),'events':filtered},removed
cases=[list(p)+[0] for p in itertools.product([0,1,3,8,9,18,100,260],[0,1,8,400],[0,20,300,10000],[0,1],range(3))]
for files,append,op in itertools.product([0,1,3,8,9,18,100,260],[0,1],range(3)):cases.append([files,8,300,append,op,2])
exe=ROOT/'build/c-only-debug/Debug/restore_file_list_verify.exe'
process=subprocess.Popen([str(exe),'--oracle-batch'],cwd=ROOT,stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True)
report={'format':1,'original_game_executed':False,'native_sha256':digest,'method':'Bounded original enumeration/growth/quicksort with controlled Win32 enumeration and heap; exact bytes and endpoint calls compared',
 'compiled_C_sha256':hashlib.sha256(exe.read_bytes()).hexdigest(),'return_contract':'0041c740 and 0041c8f0 return Boolean in AL; upper EAX bits are unspecified and are not compared.',
 'safety_difference':{'reason':'Remove the original duplicate deletion of an offsets buffer when growing a reused non-appending list. All final data bytes remain equal.','cases':0},
 'limitations':['Enumeration and allocation endpoints are controlled. File reads, actual OS directory ordering and campaign/save parsing are separate checks.','The legacy file-list vtable remains data; no original machine code is called.']}
checked=0;p=None
try:
 assert json.loads(process.stdout.readline())=={'fields':6}
 for p in cases:
  process.stdin.write(' '.join(map(str,p))+'\n');process.stdin.flush();line=process.stdout.readline()
  assert line,'C oracle exited: '+process.stderr.read()
  actual=json.loads(line);expected,removed=native(p)
  assert actual==expected,repr({'result':(actual['result'],expected['result']),'bytes_equal':actual['bytes']==expected['bytes'],'C_events':actual['events'],'native_events':expected['events']})
  if removed:
   assert p[3]==0 and len(removed)==1 and removed[0]==[5,BASE+0x1000]
   report['safety_difference']['cases']+=1
  checked+=1
 report.update(passed=True,checked=checked)
except Exception as error:report.update(passed=False,checked=checked,case=p,error=str(error),native_eip=hex(vm.reg_read(UC_X86_REG_EIP)))
finally:process.stdin.close();process.wait(timeout=10)
sources=['src/unit_00269.c','src/unit_00270.c','src/unit_00271.c','src/unit_00287.c','src/unit_02820.c','src/unit_02821.c','src/unit_02822.c','include/file_list.h','tests/file_list.c','tools/verify_file_list_emulation.py']
report['source_hashes']={s:hashlib.sha256((ROOT/s).read_bytes()).hexdigest() for s in sources}
(ROOT/'metadata/file-list-emulation.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report,indent=2));sys.exit(0 if report['passed'] else 1)
