"""Compare exact framebuffer bytes from three copy routines and two RLE decoders."""
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
SRC=0x31000000;DST=0x31100000;CAPACITY=0x100000;STOP=0x33000000;STACK=0x34000000
for a,n in [(SRC,CAPACITY),(DST,CAPACITY),(STOP,0x1000),(STACK,0x10000)]:vm.mem_map(a,n)
entries=[0x40fb60,0x40ffd0,0x410090,0x410b10,0x410230];ranges=[(0x40fb60,0x40fbd5),(0x40ffd0,0x410087),(0x410090,0x4100ed),(0x410b10,0x410dd6),(0x410f20,0x410f6b),(0x410230,0x4105b1)]
def guard(cpu,a,n,user):
 if not any(lo<=a and a+n-1<=hi for lo,hi in ranges):raise RuntimeError('Native instruction escaped at '+hex(a))
vm.hook_add(UC_HOOK_BLOCK,guard)
def write(a,v,n=4):vm.mem_write(a,(v&((1<<(8*n))-1)).to_bytes(n,'little'))
sources={}
def native(p,length):
 if p[8] not in sources:sources[p[8]]=struct.pack('<'+'H'*(CAPACITY//2),*[0xf81f if i%7==0 else (i*37+p[8])&65535 for i in range(CAPACITY//2)])
 source=sources[p[8]]
 if p[0]>=3:
  data=bytearray(b'\xa5'*CAPACITY);cursor=0
  for row in range(p[2]+p[5]):
   col=0
   while col<p[1]:
    run=min(1+(row+col)%16,p[1]-col);transparent=(row+col)%3==0
    data[cursor]=255 if transparent else run;cursor+=1
    if transparent:data[cursor]=run;cursor+=1
    else:
     for i in range(run):
      index=row*p[1]+col+i;value=0xf81f if index%7==0 else (index*37+123)&65535
      struct.pack_into('<H',data,cursor,value);cursor+=2
    col+=run
  source=bytes(data);write(0xd3b538,p[5]);write(0xd3b51c,p[8],2)
 vm.mem_write(SRC,source);vm.mem_write(DST,b'\xa5'*CAPACITY)
 for a,v in [(0xe92e18,DST),(0xe38d94,p[7]),(0xd3b5e0,p[3]),(0xd3b5dc,p[4]),(0xd3b544,p[1]),(0xd3b5d4,p[2]),(0xd3b540,p[5]),(0xd3b53c,p[6])]:write(a,v)
 write(0xd3b5be,p[1],2);sp=STACK+0x8000;write(sp,STOP);write(sp+4,SRC)
 saved=[(UC_X86_REG_EBX,0x11111111),(UC_X86_REG_EBP,0x22222222),(UC_X86_REG_ESI,0x33333333),(UC_X86_REG_EDI,0x44444444)]
 for reg,value in saved:vm.reg_write(reg,value)
 vm.reg_write(UC_X86_REG_ESP,sp);vm.reg_write(UC_X86_REG_EFLAGS,2)
 vm.emu_start(entries[p[0]],STOP,count=10000000)
 assert vm.reg_read(UC_X86_REG_EIP)==STOP,'Instruction limit exhausted'
 assert vm.reg_read(UC_X86_REG_ESP)==sp+4,'cdecl cleanup differs'
 assert all(vm.reg_read(r)==v for r,v in saved),'Saved register differs'
 assert bytes(vm.mem_read(SRC,CAPACITY))==source,'Native source modified'
 return bytes(vm.mem_read(DST,length))
exe=ROOT/'build/c-only-debug/Debug/restore_pixels_verify.exe'
process=subprocess.Popen([str(exe),'--oracle-batch'],cwd=ROOT,stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE)
cases=[]
for mode in range(5):
 for width in [1,2,3,4,7,31,640]:
  for height in [1,2,7]:
   for skip in [0,6]:cases.append([mode,width,height,2,3,skip,1280-width*2,1280,123])
 cases.append([mode,640,480,0,0,6,0,1280,0xabcd])
report={'format':1,'unicorn_version':unicorn_version,'original_game_executed':False,'native_entries':[hex(v) for v in entries],
 'method':'Exact framebuffer bytes, untouched row padding, source preservation and callee-saved registers compared against original copy instructions; no controlled drawing endpoints',
 'limitation':'Native equivalence covers positive dimensions and valid row strides. C additionally ignores nonpositive dimensions instead of executing the native do/while first iteration.',
 'compiled_C_sha256':hashlib.sha256(exe.read_bytes()).hexdigest(),'original_sha256':hashlib.sha256(image).hexdigest()}
checked=0;case=None;started=time.monotonic()
try:
 assert json.loads(process.stdout.readline())=={'source':SRC,'destination':DST}
 for case in cases:
  process.stdin.write((' '.join(map(str,case))+'\n').encode());process.stdin.flush()
  length=json.loads(process.stdout.readline())['bytes'];actual=process.stdout.read(length);assert len(actual)==length
  expected=native(case,length)
  if actual!=expected:
   report['first_differences']=[{'offset':i,'C':a,'native':b} for i,(a,b) in enumerate(zip(actual,expected)) if a!=b][:12]
   raise AssertionError('Exact pixel bytes differ')
  checked+=1
 report.update(passed=True,checked=checked)
except Exception as error:report.update(passed=False,checked=checked,case=case,error=str(error),native_eip=hex(vm.reg_read(UC_X86_REG_EIP)))
finally:
 process.stdin.close();process.wait(timeout=10)
report['elapsed_seconds']=round(time.monotonic()-started,3)
files=['src/unit_00128.c','src/unit_00136.c','src/unit_00137.c','src/unit_00140.c','src/unit_00146.c','src/unit_00149.c','runtime/sprite_pixels.c','include/sprite_render.h','tests/sprite_pixels.c','tools/verify_sprite_pixels_emulation.py']
report['source_hashes']={f:hashlib.sha256((ROOT/f).read_bytes()).hexdigest() for f in files}
(ROOT/'metadata/sprite-pixels-emulation.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({k:v for k,v in report.items() if k!='source_hashes'},indent=2));sys.exit(0 if report['passed'] else 1)
