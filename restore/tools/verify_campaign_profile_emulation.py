"""Compare profile persistence with bounded original instructions and controlled I/O."""
from pathlib import Path
import hashlib,itertools,json,struct,subprocess,sys
ROOT=Path(__file__).resolve().parent.parent;sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_BLOCK,UC_HOOK_CODE
from unicorn.x86_const import *
data=(ROOT/'build/smoke-game/Pharaoh.exe').read_bytes();digest=hashlib.sha256(data).hexdigest()
assert digest=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
u16=lambda b,p:struct.unpack_from('<H',b,p)[0];u32=lambda b,p:struct.unpack_from('<I',b,p)[0]
pe=u32(data,0x3c);opt=pe+24;table=opt+u16(data,pe+20);base=u32(data,opt+28);size=u32(data,opt+56)
vm=Uc(UC_ARCH_X86,UC_MODE_32);vm.mem_map(base,(size+4095)&~4095)
for i in range(u16(data,pe+6)):
 p=table+i*40;vm.mem_write(base+u32(data,p+12),data[u32(data,p+20):u32(data,p+20)+u32(data,p+16)])
BASE=0x30000000;STREAM=0x31000000;STOP=0x33000000;STACK=0x34000000
for a in [BASE,STOP,STACK]:vm.mem_map(a,0x10000)
def write(a,v):vm.mem_write(a,struct.pack('<I',v&0xffffffff))
def read(a):return int.from_bytes(vm.mem_read(a,4),'little')
def string(a):
 out=bytearray()
 for i in range(128):
  c=bytes(vm.mem_read(a+i,1))
  if c==b'\0':return bytes(out)
  out+=c
 raise AssertionError('Unterminated endpoint string')
events=[];position=0;contents=b'';written=b'';available=write_ok=0
endpoints={0x561a0a:2,0x561ca1:4,0x561f5f:4,0x561ea4:3,0x561994:1}
def endpoint(cpu,a,n,user):
 global position,written
 sp=cpu.reg_read(UC_X86_REG_ESP);args=[read(sp+4+i*4) for i in range(endpoints[a])];result=0
 if a==0x561a0a:
  assert string(args[0])==b'fixture.dat';mode=string(args[1]);assert mode in [b'rb',b'wb']
  events.append([1,int(mode==b'wb'),available]);result=STREAM if available else 0
 elif a==0x561ca1:
  out,size,count,stream=args;assert stream==STREAM and count==1
  chunk=contents[position:position+size];vm.mem_write(out,chunk);position+=len(chunk)
  events.append([2,size,len(chunk)]);result=len(chunk)//size
 elif a==0x561f5f:
  source,size,count,stream=args;assert stream==STREAM and count==1 and size==0x4a9c
  written=bytes(vm.mem_read(source,size));events.append([3,size,write_ok]);result=write_ok
 elif a==0x561ea4:
  assert args==[STREAM,0,0];position=0;events.append([4,0,0])
 else:assert args==[STREAM];events.append([5,0,0])
 cpu.reg_write(UC_X86_REG_EAX,result);cpu.reg_write(UC_X86_REG_EIP,read(sp));cpu.reg_write(UC_X86_REG_ESP,sp+4)
def guard(cpu,a,n,user):
 if a not in endpoints and not any(lo<=a and a+n<=hi for lo,hi in [(0x41ac00,0x41ac90),(0x41b680,0x41b8f0),(0x561f30,0x561f5f)]):
  raise RuntimeError('Native profile escaped at '+hex(a))
vm.hook_add(UC_HOOK_BLOCK,guard)
for a in endpoints:vm.hook_add(UC_HOOK_CODE,endpoint,begin=a,end=a)
def native(p):
 global position,contents,written,available,write_ok
 op,available,length,count,seed,text_length,write_ok,_=p;position=0;events.clear();written=bytes(0x4a9c)
 vm.mem_write(BASE,bytes([seed])*0x4a9c);write(BASE+0x2c8c,count)
 source=bytearray((i*37+seed)&255 for i in range(0x4aa0));source[0x2c90:0x2cd1]=b'R'*65;source[0x2c90+text_length]=0
 for i in range(100):struct.pack_into('<I',source,0x2d1c+i*0x4c,i+1 if i%3 else 0)
 contents=bytes(source[:length]);vm.mem_write(BASE+0xf000,b'fixture.dat\0');vm.mem_write(STACK,bytes(0x10000))
 sp=STACK+0xe000
 for i,v in enumerate([STOP,BASE+0xf000]):write(sp+i*4,v)
 saved=[(UC_X86_REG_EBX,0x11111111),(UC_X86_REG_EBP,0x22222222),(UC_X86_REG_ESI,0x33333333),(UC_X86_REG_EDI,0x44444444)]
 for r,v in saved:vm.reg_write(r,v)
 vm.reg_write(UC_X86_REG_ESP,sp);vm.reg_write(UC_X86_REG_ECX,BASE);vm.reg_write(UC_X86_REG_EFLAGS,2)
 vm.emu_start(0x41b8a0 if op else 0x41b680,STOP,count=200000)
 assert vm.reg_read(UC_X86_REG_EIP)==STOP and vm.reg_read(UC_X86_REG_ESP)==sp+8,'Native stack/budget differs'
 assert all(vm.reg_read(r)==v for r,v in saved),'Callee-saved register differs'
 return {'result':vm.reg_read(UC_X86_REG_EAX)&255,'position':position,'object':bytes(vm.mem_read(BASE,0x4a9c)).hex(),'written':written.hex(),'events':events.copy()}
cases=[[0,a,n,c,s,t,1,0] for a,n,c,s,t in itertools.product([0,1],[0,1,0x4a6b,0x4a6c,0x4a6d,0x4a83,0x4a84,0x4a9b,0x4a9c,0x4aa0],[0,1,33,100],[0,17,255],[0,7,64])]
cases.extend([1,a,0x4a9c,100,s,7,w,0] for a,s,w in itertools.product([0,1],[0,17,255],[0,1]))
exe=ROOT/'build/c-only-debug/Debug/restore_campaign_profile_verify.exe'
process=subprocess.Popen([str(exe),'--oracle-batch'],cwd=ROOT,stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True)
report={'format':1,'original_game_executed':False,'native_sha256':digest,'method':'Bounded native constructors, stack allocation, profile reader and writer; file endpoints controlled. Full object bytes, written bytes, I/O calls, AL result, stack and saved registers compared.',
 'compiled_C_sha256':hashlib.sha256(exe.read_bytes()).hexdigest(),
 'safety_differences':['Unread stack bytes in old/partial profiles are initialized to zero in C; the comparator initializes the native stack to zero.','C rejects nonterminated active names and out-of-range destination name counts; these malformed-input checks are separate C tests.'],
 'limitations':['This checks persistence contracts, not the complete family-menu controller or operating-system file implementation.']}
checked=0;p=None
try:
 assert json.loads(process.stdout.readline())=={'fields':8}
 for p in cases:
  process.stdin.write(' '.join(map(str,p))+'\n');process.stdin.flush();line=process.stdout.readline();assert line,'C exited: '+process.stderr.read()
  actual=json.loads(line);expected=native(p)
  for key in expected:
   assert actual[key]==expected[key],f'{key} differs at '+str(next((i for i,(a,b) in enumerate(zip(actual[key],expected[key])) if a!=b),None) if isinstance(actual[key],str) else [actual[key],expected[key]])
  checked+=1
 report.update(passed=True,checked=checked)
except Exception as error:report.update(passed=False,checked=checked,case=p,error=str(error),native_eip=hex(vm.reg_read(UC_X86_REG_EIP)))
finally:process.stdin.close();process.wait(timeout=10)
sources=['include/campaign_profile.h','src/unit_00241.c','src/unit_00242.c','src/unit_00250.c','src/unit_00251.c','tests/campaign_profile.c','tools/verify_campaign_profile_emulation.py']
report['source_hashes']={s:hashlib.sha256((ROOT/s).read_bytes()).hexdigest() for s in sources}
(ROOT/'metadata/campaign-profile-emulation.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report,indent=2));sys.exit(0 if report['passed'] else 1)
