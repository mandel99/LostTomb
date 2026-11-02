"""Compare native template expansion and explicit C contexts without running the game."""
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
BASE=0x30000000;STOP=0x33000000;STACK=0x34000000
for a in [BASE,STOP,STACK]:vm.mem_map(a,0x10000)
def write(a,v):vm.mem_write(a,struct.pack('<I',v&0xffffffff))
def read(a):return int.from_bytes(vm.mem_read(a,4),'little')
events=[];variant=0
def callback(cpu,a,n,user):
 sp=cpu.reg_read(UC_X86_REG_ESP);events.append([a,read(0xd3b18c)])
 result=0 if variant==1 else BASE+0x5200 if variant==2 else BASE+(0x5000 if a==BASE+0xf000 else 0x5100)
 cpu.reg_write(UC_X86_REG_EAX,result);cpu.reg_write(UC_X86_REG_EIP,read(sp));cpu.reg_write(UC_X86_REG_ESP,sp+4)
def guard(cpu,a,n,user):
 if a not in [BASE+0xf000,BASE+0xf010] and not any(lo<=a and a+n<=hi for lo,hi in [(0x4cb980,0x4cba60),(0x448330,0x448390),(0x4cbdb0,0x4cbdd0),(0x56b240,0x56b307)]):raise RuntimeError('Native template escaped at '+hex(a))
vm.hook_add(UC_HOOK_BLOCK,guard)
for a in [BASE+0xf000,BASE+0xf010]:vm.hook_add(UC_HOOK_CODE,callback,begin=a,end=a)
inputs=[b'',b'abc',b'[foo]',b'x[foo]y',b'[foobar]',b'[]',b'[fo]',b'[FOO]',b'[unknown]',b'[foo][bar]',b'[foo',b'a[bar]b[foo]c',b'[bar]',b'text]end',b'[foo]]',b'[foo[bar]']
def native(p):
 global variant
 op,text,capacity,variant,prior=p;events.clear();vm.mem_write(BASE,bytes(0x10000));vm.mem_write(BASE+0x3000,b'\xa5'*128)
 for a,s in [(BASE+0x4000,b'foo'),(BASE+0x4100,b'bar'),(BASE+0x5000,b'VALUE'),(BASE+0x5100,b'Other')]:vm.mem_write(a,s+b'\0')
 vm.mem_write(BASE+0x1000,struct.pack('<9I',0,BASE+0x4000,BASE+0xf000,1,BASE+0x4100,BASE+0xf010,0xffffffff,0,0))
 write(BASE,BASE+0x1000);write(0x784f34,BASE+0x1000);write(0x784f24,BASE+0x2000);write(0x784f2c,16)
 for i,s in enumerate(inputs):write(BASE+0x2000+i*4,BASE+0x6000+i*256);vm.mem_write(BASE+0x6000+i*256,s+b'\0')
 write(0xd3b18c,prior);write(0x126648c,0);args=[text if op==2 else BASE+0x6000+text*256,BASE+0x3000,capacity];sp=STACK+0xe000
 for i,v in enumerate([STOP]+args):write(sp+i*4,v)
 saved=[(UC_X86_REG_EBX,0x11111111),(UC_X86_REG_EBP,0x22222222),(UC_X86_REG_ESI,0x33333333),(UC_X86_REG_EDI,0x44444444)]
 for r,v in saved:vm.reg_write(r,v)
 vm.reg_write(UC_X86_REG_ESP,sp);vm.reg_write(UC_X86_REG_ECX,BASE);vm.reg_write(UC_X86_REG_EFLAGS,2)
 vm.emu_start([0x4cb980,0x448370,0x448330][op],STOP,count=100000)
 assert vm.reg_read(UC_X86_REG_EIP)==STOP and vm.reg_read(UC_X86_REG_ESP)==sp+(16 if op==0 else 4),'Native stack/budget differs'
 assert all(vm.reg_read(r)==v for r,v in saved),'Native saved registers differ'
 return {'result':vm.reg_read(UC_X86_REG_EAX)&255 if op==0 else 0,'context':read(0xd3b18c),'bytes':bytes(vm.mem_read(BASE+0x3000,128)).hex(),'events':events.copy()}
cases=[list(p) for p in itertools.product(range(3),range(16),[0,1,2,3,5,6,7,10,20,127,0xffffffff],range(3),[0,0x12345678])]
cases.extend([2,index,capacity,0,0x12345678] for index,capacity in itertools.product([16,0xffffffff],[0,10,127]))
exe=ROOT/'build/c-only-debug/Debug/restore_text_templates_verify.exe'
process=subprocess.Popen([str(exe),'--oracle-batch'],cwd=ROOT,stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True)
report={'format':1,'original_game_executed':False,'native_sha256':digest,'method':'Native expansion, case-insensitive prefix comparison, message lookup and both local wrappers; substitution callbacks controlled. Full output/redzone bytes, callback context, AL return and native ABI compared.',
 'compiled_C_sha256':hashlib.sha256(exe.read_bytes()).hexdigest(),'limitations':['This verifies expansion/lookup, not every production substitution callback or message-file loading. The native locale flag is zero in these cases.']}
checked=0;p=None
try:
 assert json.loads(process.stdout.readline())=={'fields':5}
 for p in cases:
  process.stdin.write(' '.join(map(str,p))+'\n');process.stdin.flush();line=process.stdout.readline();assert line,'C exited: '+process.stderr.read()
  actual=json.loads(line);expected=native(p);assert actual==expected,repr({'C':actual,'native':expected});checked+=1
 report.update(passed=True,checked=checked)
except Exception as error:report.update(passed=False,checked=checked,case=p,error=str(error),native_eip=hex(vm.reg_read(UC_X86_REG_EIP)))
finally:process.stdin.close();process.wait(timeout=10)
sources=['include/text_templates.h','src/unit_00674.c','src/unit_00675.c','src/unit_01656.c','src/unit_01659.c','src/unit_03027.c','tests/text_templates.c','tools/verify_text_templates_emulation.py']
report['source_hashes']={s:hashlib.sha256((ROOT/s).read_bytes()).hexdigest() for s in sources}
(ROOT/'metadata/text-templates-emulation.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report,indent=2));sys.exit(0 if report['passed'] else 1)
