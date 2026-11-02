"""Offline native/C timer comparison with a controlled millisecond clock."""
from pathlib import Path
import hashlib,json,random,struct,subprocess,sys
ROOT=Path(__file__).resolve().parent.parent
sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_BLOCK,UC_HOOK_CODE,__version__ as unicorn_version
from unicorn.x86_const import *
image=(ROOT/'build/smoke-game/Pharaoh.exe').read_bytes()
assert hashlib.sha256(image).hexdigest()=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
u16=lambda b,p:struct.unpack_from('<H',b,p)[0]
u32=lambda b,p:struct.unpack_from('<I',b,p)[0]
pe=u32(image,0x3c);opt=pe+24;table=opt+u16(image,pe+20);base=u32(image,opt+28);size=u32(image,opt+56)
vm=Uc(UC_ARCH_X86,UC_MODE_32);vm.mem_map(base,(size+4095)&~4095)
for i in range(u16(image,pe+6)):
 p=table+i*40;vm.mem_write(base+u32(image,p+12),image[u32(image,p+20):u32(image,p+20)+u32(image,p+16)])
HOOK=0x32000000;STOP=0x33000000;STACK=0x34000000
for a in [HOOK,STOP,STACK]:vm.mem_map(a,0x10000)
def write(a,v,n=4):vm.mem_write(a,(v&((1<<(n*8))-1)).to_bytes(n,'little'))
def read(a):return int.from_bytes(vm.mem_read(a,4),'little')
write(0x56f22c,HOOK);vm.mem_write(HOOK,b'\xc3')
now=0
def endpoint(cpu,a,n,user):
 sp=cpu.reg_read(UC_X86_REG_ESP);cpu.reg_write(UC_X86_REG_EAX,now);cpu.reg_write(UC_X86_REG_EIP,read(sp));cpu.reg_write(UC_X86_REG_ESP,sp+4)
def guard(cpu,a,n,user):
 if a!=HOOK and not 0x4ddcb0<=a<=0x4ddd53:raise RuntimeError('Escaped native timer at '+hex(a))
vm.hook_add(UC_HOOK_BLOCK,guard);vm.hook_add(UC_HOOK_CODE,endpoint,begin=HOOK,end=HOOK)
def native(p):
 global now
 now=p[0]
 for a,v,n in [(0xe94db8,p[1],4),(0xe38e6c,p[2],4),(0xe38e60,p[3],1),(0xe38dda,p[4],1),(0xd3a6c4,p[5],4),(0x6059e4,p[6],4),(0xe38dcc,p[7],4),(0x5e1c50+p[4]*4,p[8],4)]+[(0x5e1db4+i*4,p[9],4) for i in range(10)]:write(a,v,n)
 sp=STACK+0x8000;write(sp,STOP);vm.reg_write(UC_X86_REG_ESP,sp);vm.reg_write(UC_X86_REG_EFLAGS,2)
 vm.reg_write(UC_X86_REG_ESI,0x87654321);vm.reg_write(UC_X86_REG_EDI,0xfedcba98)
 vm.emu_start(0x4ddcb0,STOP,count=10000)
 assert vm.reg_read(UC_X86_REG_EIP)==STOP and vm.reg_read(UC_X86_REG_ESP)==sp+4
 assert vm.reg_read(UC_X86_REG_ESI)==0x87654321 and vm.reg_read(UC_X86_REG_EDI)==0xfedcba98
 return {'result':vm.reg_read(UC_X86_REG_EAX),'last':read(0xe94db8)}
cases=[]
for mode in [0,1,0x12,0x23,0x3c,0x40,0x41,0x44,0x52]:
 for speed in [0,1,10,90,99,100,101,255,0x100,0x80000000,0xffffffff]:
  for elapsed in [0,1,2,3,30,100,0x80000000]:
   cases.append([0xfffffff0,(0xfffffff0-elapsed)&0xffffffff,speed,0,mode,1,0,0,0,20])
rng=random.Random(0x4ddcb0)
for _ in range(1000):
 cases.append([rng.getrandbits(32),rng.getrandbits(32),rng.choice([90,100,rng.getrandbits(32)]),rng.choice([0,0,1,0x100]),
 rng.randrange(89),rng.choice([0,1,0x100]),rng.choice([0,0,0x100]),rng.choice([0,0,0x100]),rng.choice([0,0,1,0x100]),rng.choice([0,20,0x7fffffff,0xfffffffe])])
exe=ROOT/'build/c-only-debug/Debug/restore_tick_timer_verify.exe'
process=subprocess.Popen([str(exe),'--oracle-batch'],cwd=ROOT,stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True)
report={'unicorn_version':unicorn_version,'original_game_executed':False,'method':'Native timer vs C with exact DWORD state, byte flags and controlled tick callback','compiled_C_sha256':hashlib.sha256(exe.read_bytes()).hexdigest()}
checked=0;p=None
try:
 assert json.loads(process.stdout.readline())=={'fields':2}
 for p in cases:
  process.stdin.write(' '.join(map(str,p))+'\n');process.stdin.flush()
  actual=json.loads(process.stdout.readline());expected=native(p)
  assert actual==expected,repr({'C':actual,'native':expected})
  checked+=1
 report.update(passed=True,checked=checked)
except Exception as error:report.update(passed=False,checked=checked,case=p,error=str(error))
finally:process.stdin.close();process.wait(timeout=10)
report['source_hashes']={s:hashlib.sha256((ROOT/s).read_bytes()).hexdigest() for s in ['src/unit_01876.c','include/simulation_tick.h','tests/tick_timer.c','tools/verify_tick_timer_emulation.py']}
(ROOT/'metadata/tick-timer-emulation.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps(report,indent=2));sys.exit(0 if report['passed'] else 1)
