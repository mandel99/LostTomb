"""Offline original-instruction/compiled-C differential for upper sprite draw.

Never launches the original executable. Both original clippers execute in
Unicorn. Raster endpoints are controlled and checked in call order; no raster
algorithm or production endpoint stub is introduced by this verification.
"""
from pathlib import Path
import argparse,hashlib,json,random,struct,subprocess,sys,time
ROOT=Path(__file__).resolve().parents[1]
sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE,UC_HOOK_MEM_WRITE,UC_PROT_READ,__version__
from unicorn.x86_const import *
image=(ROOT/'build/smoke-game/Pharaoh.exe').read_bytes()
assert hashlib.sha256(image).hexdigest()=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
u16=lambda b,p:struct.unpack_from('<H',b,p)[0]
u32=lambda b,p:struct.unpack_from('<I',b,p)[0]
pe=u32(image,0x3c);opt=pe+24;table=opt+u16(image,pe+20);base=u32(image,opt+28);size=u32(image,opt+56)
vm=Uc(UC_ARCH_X86,UC_MODE_32);vm.mem_map(base,(size+4095)&~4095);vm.mem_write(base,image[:u32(image,opt+60)])
for i in range(u16(image,pe+6)):
 p=table+i*40;vm.mem_write(base+u32(image,p+12),image[u32(image,p+20):u32(image,p+20)+u32(image,p+16)])
OBJECT=0x30000000;PIXELS=0x31000000;FRAME=0x31100000;STOP=0x33000000;STACK=0x34000000
for a,n in [(OBJECT,0x400000),(PIXELS,0x10000),(FRAME,0x10000),(STOP,0x1000),(STACK,0x100000)]:vm.mem_map(a,n)
vm.mem_write(PIXELS,b'\x6b'*0x10000);vm.mem_write(FRAME,b'\xc7'*0x10000)
vm.mem_protect(PIXELS,0x10000,UC_PROT_READ);vm.mem_protect(FRAME,0x10000,UC_PROT_READ)
RANGES=[(0x53f250,0x53f49f),(0x4cdad0,0x4cdb79),(0x4cdb80,0x4cdc2d)]
BASE_ENDPOINTS={0x410230,0x4105c0,0x410660};COLOR_ENDPOINTS={0x410700,0x4109d0,0x410a70}
state={};executed=set();global_writes=set()
def read(a,n=4):return int.from_bytes(vm.mem_read(a,n),'little')
def write(a,v,n=4):vm.mem_write(a,(v&((1<<(n*8))-1)).to_bytes(n,'little'))
def event(a,n=0,p=0,c=0):
 state['events'].append([a,n,p,c,read(0xd3b5e0),read(0xd3b5dc),read(0xd3b5bc,2),read(0xd3b5be,2)])
def hook(cpu,a,n,user):
 executed.add(a)
 if a in [0x4cdad0,0x4cdb80]:event(a)
 if any(lo<=a and a+n-1<=hi for lo,hi in RANGES):return
 if a not in BASE_ENDPOINTS|COLOR_ENDPOINTS:raise RuntimeError('Unexpected instruction '+hex(a))
 sp=cpu.reg_read(UC_X86_REG_ESP);pointer=read(sp+4)
 event(a,1 if a in BASE_ENDPOINTS else 2,pointer,read(sp+8,2) if a in COLOR_ENDPOINTS else 0)
 if a in BASE_ENDPOINTS:
  mutation=state['mutation']
  if mutation==1:write(0xd3b531,2,1);write(0x70e120,0x9080,2);write(0xd3b5b8,read(0xd3b5b8)+0x10)
  if mutation==2:write(0x70e120,0,2)
  if mutation==3:write(0xd3b531,5,1);write(0x70e120,0x80ff,2)
  if mutation==4:write(0xd3b531,1,1);write(0x70e120,0xff00,2);write(0xd3b5b8,read(0xd3b5b8)+0x20)
 cpu.reg_write(UC_X86_REG_EAX,0x9abcdef0);cpu.reg_write(UC_X86_REG_ECX,0xa17e4567);cpu.reg_write(UC_X86_REG_EDX,0x5a5a5a5a)
 cpu.reg_write(UC_X86_REG_EIP,read(sp));cpu.reg_write(UC_X86_REG_ESP,sp+4)
def guard(cpu,access,a,n,value,user):
 if STACK<=a and a+n<=STACK+0x100000:return
 if 0xd3b520<=a and a+n<=0xd3b600:
  global_writes.add((a,n));return
 raise AssertionError('Unexpected guest write '+hex(a))
vm.hook_add(UC_HOOK_CODE,hook);vm.hook_add(UC_HOOK_MEM_WRITE,guard)
def native(p):
 global state
 obj=bytearray(b'\xa5'*0x30f5d8);struct.pack_into('<I',obj,0x30f5cc,p[6]);off=0x180e48
 obj[off:off+64]=bytes(64);struct.pack_into('<I',obj,off,PIXELS+0x100);struct.pack_into('<I',obj,off+8,4)
 struct.pack_into('<HH',obj,off+20,118,70);obj[off+0x35]=p[20]&255
 if 0<p[0]<25500:
  off+=p[0]*64;obj[off:off+64]=bytes(64);struct.pack_into('<I',obj,off,p[7]);struct.pack_into('<I',obj,off+8,p[8])
  struct.pack_into('<HH',obj,off+20,p[1]&65535,p[2]&65535);obj[off+0x35]=p[5]&255
 target=0xf55bb0 if p[19] else OBJECT
 vm.mem_write(target,bytes(obj));vm.mem_write(0xd3b520,b'\x5a'*224);write(0x70e120,p[9],2)
 for a,v in [(0xd3b55c,p[10]),(0xd3b558,p[11]),(0xd3b554,p[12]),(0xd3b550,p[13]),(0xe38d9c,p[14]),
  (0xe38d94,p[14]*2),(0xe92e18,FRAME+0x100),(0xd3b5e0,p[15]),(0xd3b5dc,p[16])]:write(a,v)
 write(0xd3b5bc,p[17],2);state={'events':[],'mutation':p[18]};sp=STACK+0x80000
 vm.mem_write(sp-0x100,b'\xcc'*0x200)
 for i,v in enumerate([STOP,p[0],p[3],p[4]]):write(sp+i*4,v)
 saved=[(UC_X86_REG_EBX,0x11223344),(UC_X86_REG_EBP,0x22334455),(UC_X86_REG_ESI,0x33445566),(UC_X86_REG_EDI,0x44556677)]
 for r,v in saved:vm.reg_write(r,v)
 vm.reg_write(UC_X86_REG_ECX,target);vm.reg_write(UC_X86_REG_ESP,sp);vm.reg_write(UC_X86_REG_EFLAGS,2)
 vm.emu_start(0x53f250,STOP,count=10000)
 assert vm.reg_read(UC_X86_REG_EIP)==STOP
 assert vm.reg_read(UC_X86_REG_ESP)==sp+16
 assert all(vm.reg_read(r)==v for r,v in saved)
 assert bytes(vm.mem_read(sp+16,0xf0))==b'\xcc'*0xf0
 assert bytes(vm.mem_read(sp-0x100,0x80))==b'\xcc'*0x80
 assert bytes(vm.mem_read(target,len(obj)))==bytes(obj)
 assert bytes(vm.mem_read(PIXELS,0x10000))==b'\x6b'*0x10000
 assert bytes(vm.mem_read(FRAME,0x10000))==b'\xc7'*0x10000
 return dict(result=vm.reg_read(UC_X86_REG_EAX),globals=bytes(vm.mem_read(0xd3b520,224)).hex(),tint=read(0x70e120,2),events=state['events'])
default=[5,58,45,20,50,1,16,PIXELS+0x100,4,0x1234,0,640,0,480,640,7,8,9,0,0,1]
cases=[]
def add(**kwargs):
 p=default.copy()
 for key,v in kwargs.items():p[int(key[1:])]=v&0xffffffff
 cases.append(p)
for facade in [0,1]:
 for ident in [0,1,15,16,509,25499,25500,0x7fffffff,0x80000000,0xffffffff]:
  for count in [0,16,683,25500,0xffffffff]:
   for enabled in [0,1]:add(p0=ident,p6=count,p19=facade,p20=enabled)
for width in [58,118,178,238,298,358,0,1,57,59,32767,32768,65535]:
 for height in [0,15,16,30,45,70,180,32767,32768,65535]:
  for x,y in [(20,50),(-1,50),(630,50),(640,50),(-400,50),(20,-50),(20,470),(20,550)]:add(p1=width,p2=height,p3=x,p4=y)
for enabled in [0,1,0x80,0xff]:
 for pointer in [0,1,0x7fffffff,0x80000000,0xffffffff,PIXELS+0x100]:
  for offset in [0,4,0xffffffff,0x80000000]:add(p5=enabled,p7=pointer,p8=offset)
for mutation in range(5):
 for x in [-3,20,630]:
  for tint in [0,1,0x100,0x1234,0xffff]:add(p18=mutation,p3=x,p9=tint)
for width in [58,118,178,238,298,358]:
 size=(width+2)//60
 for height in [15*size,15*size+1,15*size+2]:add(p1=width,p2=height)
rng=random.Random(0x53f250)
for _ in range(400):
 add(p0=rng.choice([1,5,509,25499]),p1=rng.choice([58,118,178,238,298,358,65535]),p2=rng.randrange(65536),
  p3=rng.choice([-500,-1,0,10,639,640,1000,0x7fffffff,0x80000000]),p4=rng.choice([-100,0,20,479,480,0x7fffffff,0x80000000]),
  p5=rng.choice([0,1,255]),p6=rng.choice([0,16,25500]),p9=rng.randrange(65536),p10=rng.choice([0,4]),p11=rng.choice([100,640]),
  p12=rng.choice([0,4]),p13=rng.choice([100,480]),p15=rng.randrange(640),p16=rng.randrange(480),p17=rng.randrange(65536),
  p18=rng.randrange(5),p19=rng.randrange(2),p20=rng.randrange(2))
parser=argparse.ArgumentParser(description=__doc__)
parser.add_argument('--exe',type=Path,default=ROOT/'build/agent-graphics/Debug/graphics_53f250_verify.exe')
options=parser.parse_args();exe=options.exe.resolve()
process=subprocess.Popen([str(exe),'--oracle-batch'],cwd=ROOT,stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True)
report=dict(native_entry='0x0053f250',original_game_executed=False,unicorn_version=__version__,
 original_sha256=hashlib.sha256(image).hexdigest(),compiled_C_sha256=hashlib.sha256(exe.read_bytes()).hexdigest(),
 method='Full original 165-instruction function and both original clipping bodies versus compiled actual C function and clipping bodies; exact 224 global/guard bytes, 0/1 return, color word, ordered endpoint arguments and clipping-entry state. Native thiscall cleanup and compiled C caller cleanup, stack guards and saved registers checked.',
 limitations=['Raster bodies are controlled endpoints in this differential. Pixel and frame regions are read-only and verified unchanged; this does not establish raster pixel correctness.',
  'Original PUSH ECX color argument has an undefined caller-volatile upper half after the preceding raster call. Only its low 16 bits are semantic: all three color consumers read the word. No production raster stub is added.',
  'This offline comparison does not execute an original game process or validate live map rendering.'])
started=time.monotonic();checked=0;case=None
try:
 assert json.loads(process.stdout.readline())==dict(base=OBJECT)
 for case in cases:
  process.stdin.write(' '.join(map(str,case))+'\n');process.stdin.flush()
  line=process.stdout.readline()
  if not line:raise RuntimeError('C oracle stopped: '+process.stderr.read())
  actual=json.loads(line);expected=native(case)
  if actual!=expected:
   report['differences']={k:dict(C=actual[k],native=expected[k]) for k in expected if actual[k]!=expected[k]}
   raise AssertionError('State/return/endpoint difference')
  checked+=1
 report.update(passed=True,checked=checked)
except Exception as e:report.update(passed=False,checked=checked,case=case,error=str(e),native_eip=hex(vm.reg_read(UC_X86_REG_EIP)))
finally:
 process.stdin.close();process.wait(timeout=10)
report['elapsed_seconds']=round(time.monotonic()-started,3)
instructions=json.loads((ROOT/'metadata/city-draw-next-native.json').read_text())['function']['items']
report['function_instruction_coverage']=dict(total=len(instructions),executed=sum(int(i['address'],16) in executed for i in instructions),
 missing=[i for i in instructions if int(i['address'],16) not in executed])
report['native_global_write_widths']=[dict(address=f'{a:08x}',bytes=n) for a,n in sorted(global_writes)]
report['source_hashes']={p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest() for p in ['src/unit_02525.c','include/graphics_53f250.h','tests/graphics_53f250.c','tools/verify_graphics_53f250_emulation.py','src/unit_01725.c','src/unit_01726.c']}
(ROOT/'metadata/graphics-53f250-emulation.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({k:v for k,v in report.items() if k not in ['source_hashes','differences']},indent=2))
sys.exit(0 if report['passed'] else 1)
