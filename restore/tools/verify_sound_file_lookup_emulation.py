"""Sound file table native/C comparison. No files, media or game are run."""
from pathlib import Path
import hashlib,json,struct,subprocess,sys,time
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
BASE=0x30000000;HEAP=0x31000000;STOP=0x33000000;STACK=0x34000000
for a,n in [(0,0x1000),(BASE,0x1000),(HEAP,0x1000),(STOP,0x1000),(STACK,0x10000)]:vm.mem_map(a,n)
def read(a,n=4):return int.from_bytes(vm.mem_read(a,n),'little')
def write(a,v,n=4):vm.mem_write(a,(v&((1<<(n*8))-1)).to_bytes(n,'little'))
def cstring(a):return bytes(vm.mem_read(a,256)).split(b'\0',1)[0]
state={}
def guard(cpu,a,n,user):
 if a not in [0x5631b0,0x56b177] and not any(lo<=a and a+n-1<=hi for lo,hi in [(0x55dd3f,0x55dea1),(0x560280,0x5602e2)]):raise RuntimeError('Native instruction escaped at '+hex(a))
def endpoint(cpu,a,n,user):
 sp=cpu.reg_read(UC_X86_REG_ESP);x=read(sp+4);y=read(sp+8)
 if a==0x5631b0:
  x=cstring(x);y=cstring(y);value=0 if x==y else (1 if x>y else 0xffffffff)
 else:
  state['duplicates']+=1;vm.mem_write(HEAP,cstring(x)+b'\0');value=HEAP
 cpu.reg_write(UC_X86_REG_EAX,value);cpu.reg_write(UC_X86_REG_EIP,read(sp));cpu.reg_write(UC_X86_REG_ESP,sp+4)
vm.hook_add(UC_HOOK_BLOCK,guard)
for a in [0x5631b0,0x56b177]:vm.hook_add(UC_HOOK_CODE,endpoint,begin=a,end=a)
def native(p):
 global state
 state={'duplicates':0};vm.mem_write(BASE,bytes([p[6]&255])*0x1000);vm.mem_write(HEAP,b'\xcd'*0x1000)
 for i,v in enumerate([p[0],p[1],BASE+0x100 if p[2] else 0]):write(BASE+i*4,v)
 for i in range(8):write(BASE+0x100+i*4,0)
 vm.mem_write(BASE+0x400,b'cached.wav\0');vm.mem_write(BASE+0x440,b'same.wav\0')
 name=[b'new.wav',b'cached.wav',b'same.wav',b'Same.wav',b''][p[7]];vm.mem_write(BASE+0x800,name+b'\0')
 write(BASE+0x100,0 if p[5] else BASE+0x400);write(BASE+0x104,1);write(BASE+0x108,BASE+0x440)
 sp=STACK+0x8000
 for i,v in enumerate([STOP,BASE+0x800,p[3]]):write(sp+i*4,v)
 saved=[(UC_X86_REG_EBX,0x11111111),(UC_X86_REG_EBP,0x22222222),(UC_X86_REG_ESI,0x33333333),(UC_X86_REG_EDI,0x44444444)]
 for r,v in saved:vm.reg_write(r,v)
 write(0,0xffffffff);vm.reg_write(UC_X86_REG_ECX,BASE);vm.reg_write(UC_X86_REG_ESP,sp);vm.reg_write(UC_X86_REG_EFLAGS,2)
 vm.emu_start(0x55dd3f,STOP,count=100000)
 assert vm.reg_read(UC_X86_REG_EIP)==STOP,'Instruction limit exhausted'
 assert vm.reg_read(UC_X86_REG_ESP)==sp+12,'thiscall cleanup differs'
 assert all(vm.reg_read(r)==v for r,v in saved),'Preserved register differs'
 assert read(0)==0xffffffff,'Native exception chain not restored'
 v=vm.reg_read(UC_X86_REG_EAX)
 return {'result':v if v<0x80000000 else v-0x100000000,'duplicates':state['duplicates'],
  'memory':bytes(vm.mem_read(BASE,0x1000)).hex(),'heap':bytes(vm.mem_read(HEAP,0x1000)).hex()}
cases=[]
for capacity in [0,1,2,3,4]:
 for count in [0,1,2,3,4]:
  for entries in [0,1]:
   for copy in [0,1,2,0xffffffff]:
    for name in range(5):
     for null in [0,1]:cases.append([capacity,count,entries,copy,0,null,0xa5,name])
exe=ROOT/'build/c-only-debug/Debug/restore_sound_files_verify.exe'
process=subprocess.Popen([str(exe),'--oracle-batch'],cwd=ROOT,stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True)
report={'format':1,'original_game_executed':False,'unicorn_version':unicorn_version,'native_entry':'0x0055dd3f',
 'method':'Native file table lookup/append plus native temporary-string constructor/destructor versus compiled C; strcmp and strdup endpoints controlled',
 'compared':['exact table/entry/string bytes','file ID and duplicate allocation count','copy ownership flag','stack cleanup, preserved registers and restored native exception chain'],
 'limitations':['C rejects negative counts/null input and returns failure without inserting a null entry if strdup fails; these deliberate repairs are checked by source-only tests.',
  'The unused native temporary string/SEH frame is eliminated from C; arbitrary C++ exceptions are not emulated.'],
 'original_sha256':hashlib.sha256(image).hexdigest(),'compiled_C_sha256':hashlib.sha256(exe.read_bytes()).hexdigest()}
checked=0;case=None;started=time.monotonic()
try:
 assert json.loads(process.stdout.readline())=={'object':BASE}
 for case in cases:
  process.stdin.write(' '.join(map(str,case))+'\n');process.stdin.flush();actual=json.loads(process.stdout.readline());expected=native(case)
  if actual!=expected:
   report['differences']={k:{'C':actual[k] if k not in ['memory','heap'] else '(bytes differ)','native':expected[k] if k not in ['memory','heap'] else '(bytes differ)'} for k in actual if actual[k]!=expected[k]}
   raise AssertionError('Table, result or ownership differs')
  checked+=1
 report.update(passed=True,checked=checked)
except Exception as error:report.update(passed=False,checked=checked,case=case,error=str(error),native_eip=hex(vm.reg_read(UC_X86_REG_EIP)))
finally:
 process.stdin.close();process.wait(timeout=10)
report['elapsed_seconds']=round(time.monotonic()-started,3)
files=['src/unit_02686.c','include/sound_file_lookup.h','include/sound_initialization.h','tests/sound_file_lookup.c','tools/verify_sound_file_lookup_emulation.py']
report['source_hashes']={f:hashlib.sha256((ROOT/f).read_bytes()).hexdigest() for f in files}
(ROOT/'metadata/sound-file-lookup-emulation.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({k:v for k,v in report.items() if k!='source_hashes'},indent=2));sys.exit(0 if report['passed'] else 1)
