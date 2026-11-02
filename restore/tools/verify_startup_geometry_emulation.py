"""Compare startup geometry with bounded original instruction emulation.
No game process or external runtime is executed by the native side.
"""
from pathlib import Path
import hashlib,json,struct,subprocess,sys,time
ROOT=Path(__file__).resolve().parent.parent
sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_BLOCK,__version__ as unicorn_version
from unicorn.x86_const import *
image=(ROOT/'build/smoke-game/Pharaoh.exe').read_bytes()
assert hashlib.sha256(image).hexdigest()=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
u16=lambda b,p:struct.unpack_from('<H',b,p)[0]
u32=lambda b,p:struct.unpack_from('<I',b,p)[0]
pe=u32(image,0x3c);opt=pe+24;table=opt+u16(image,pe+20);base=u32(image,opt+28);size=u32(image,opt+56)
vm=Uc(UC_ARCH_X86,UC_MODE_32);vm.mem_map(base,(size+4095)&~4095);vm.mem_write(base,image[:u32(image,opt+60)])
for i in range(u16(image,pe+6)):
 p=table+i*40;vm.mem_write(base+u32(image,p+12),image[u32(image,p+20):u32(image,p+20)+u32(image,p+16)])
BASE=0x30000000;STOP=0x33000000;STACK=0x34000000;BYTES=142304
for a,n in [(BASE,0x20000),(STOP,0x1000),(STACK,0x10000)]:vm.mem_map(a,n)
methods=[0x4013f0,0x40d7e0,0x416f60,0x4e6590,0x5456a0,0x558b40,0x555de0,0x54f3d0]
ranges=[(a,a+31) for a in methods]+[(0x4e5190,0x4e5203),(0x4e52b0,0x4e52cf),(0x541d50,0x541dd2),(0x4ded50,0x4deefa)]
def guard(cpu,a,n,user):
 if not any(lo<=a and a+n-1<=hi for lo,hi in ranges):raise RuntimeError('Native instruction escaped at '+hex(a))
vm.hook_add(UC_HOOK_BLOCK,guard)
def write(a,v):vm.mem_write(a,struct.pack('<I',v&0xffffffff))
def native(p):
 state=p[3]
 def next_value():
  nonlocal state
  state=(state*1664525+1013904223)&0xffffffff;return state
 vm.mem_write(BASE,b''.join(struct.pack('<I',next_value()) for _ in range(0x20000//4)))
 vm.mem_write(0x601dd8,b''.join(struct.pack('<I',next_value()) for _ in range(100*13)))
 for i in range(116):
  row=[next_value() for _ in range(13)];row[0]=next_value()%23;row[1]=next_value()%9;row[2]=next_value()%8
  vm.mem_write(0x5e2ad8+i*52,struct.pack('<13I',*row))
 write(0x70e44c,p[4]);write(0x70e45c,p[5])
 op=p[0];args=[BASE,p[1]] if op==1 else []
 entry={0:0x4e5190,1:0x4e52b0,2:methods[p[2]],3:0x541d50,4:0x4dee10}[op]
 sp=STACK+0x8000
 for i,v in enumerate([STOP]+args):write(sp+i*4,v)
 saved=[(UC_X86_REG_EBX,0x11111111),(UC_X86_REG_EBP,0x22222222),(UC_X86_REG_ESI,0x33333333),(UC_X86_REG_EDI,0x44444444)]
 for reg,v in saved:vm.reg_write(reg,v)
 vm.reg_write(UC_X86_REG_ECX,BASE);vm.reg_write(UC_X86_REG_ESP,sp);vm.reg_write(UC_X86_REG_EFLAGS,2)
 vm.emu_start(entry,STOP,count=2000000)
 assert vm.reg_read(UC_X86_REG_EIP)==STOP,'Instruction limit exhausted'
 assert vm.reg_read(UC_X86_REG_ESP)==sp+4+len(args)*4,'Native stack cleanup differs'
 assert all(vm.reg_read(reg)==v for reg,v in saved),'Preserved register differs'
 return bytes(vm.mem_read(BASE,0x20000))+bytes(vm.mem_read(0x601dd8,5200))+bytes(vm.mem_read(0x5e2ad8,6032))
cases=[]
for seed in [0,0x87654321,0xffffffff]:
 for a,b in [(0,0),(1,1),(256,255),(1024,511),(0xffffffff,0xffffffff)]:
  for op in [0,3,4]:cases.append([op,0,0,seed,a,b])
  for count in [0xffffffff,0,1,2,16,513]:cases.append([1,count,0,seed,a,b])
  for method in range(8):cases.append([2,0,method,seed,a,b])
exe=ROOT/'build/c-only-debug/Debug/restore_startup_geometry_verify.exe'
process=subprocess.Popen([str(exe),'--oracle-batch'],cwd=ROOT,stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE)
checked=0;current=None;started=time.monotonic()
report={'format':1,'unicorn_version':unicorn_version,'original_game_executed':False,'original_sha256':hashlib.sha256(image).hexdigest(),
 'compiled_C_sha256':hashlib.sha256(exe.read_bytes()).hexdigest(),'method':'Bounded original x86 record initializer, array loop, eight member receivers and both global coordinate tables versus compiled C',
 'compared':['All 142304 bytes of receiver, global tables and unused guard regions','Native callee-saved registers and stack balance'],
 'limitations':['No game loop, display or constructors of unrelated objects are covered. Rotation inputs use valid placement coordinates.']}
try:
 assert json.loads(process.stdout.readline())=={'bytes':BYTES}
 for current in cases:
  process.stdin.write((' '.join(map(str,current))+'\n').encode());process.stdin.flush()
  actual=process.stdout.read(BYTES)
  assert len(actual)==BYTES,'C oracle stopped: '+process.stderr.read().decode(errors='replace')
  expected=native(current)
  if actual!=expected:
   report['differences']=[{'offset':i,'C':a,'native':b} for i,(a,b) in enumerate(zip(actual,expected)) if a!=b][:24]
   raise AssertionError('Geometry buffers differ')
  checked+=1
 report.update(passed=True,checked=checked)
except Exception as error:
 report.update(passed=False,checked=checked,case=current,error=str(error),native_eip=hex(vm.reg_read(UC_X86_REG_EIP)))
finally:
 process.stdin.close()
 try:process.wait(timeout=10)
 except subprocess.TimeoutExpired:process.kill();process.wait()
sources=['src/unit_%05d.c'%i for i in [1965,1968,2551,1896,1895,2,99,195,1993,2563,2617,2604,2591]]
sources+=['include/startup_geometry.h','tests/startup_geometry.c','tools/verify_startup_geometry_emulation.py']
report['source_hashes']={s:hashlib.sha256((ROOT/s).read_bytes()).hexdigest() for s in sources}
report['elapsed_seconds']=round(time.monotonic()-started,3)
(ROOT/'metadata/startup-geometry-emulation.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({k:v for k,v in report.items() if k!='source_hashes'},indent=2));sys.exit(0 if report['passed'] else 1)
