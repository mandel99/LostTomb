"""Optional offline x86/C comparison of sprite dispatch and clipping.

Original instructions execute only inside Unicorn. Raster, filesystem and
conversion endpoints are controlled and their ordered calls are compared.
"""
from pathlib import Path
import hashlib, json, random, struct, subprocess, sys, time
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
OBJECT=0x30000000;STOP=0x33000000;STACK=0x34000000
for address,length in [(OBJECT,0x400000),(STOP,0x1000),(STACK,0x200000)]:vm.mem_map(address,length)
one=[0x40fb60,0x40fc40,0x40fcf0,0x40fdd0,0x40fed0,0x40ff50,0x40ffd0,0x410090,0x410230,0x4105c0,0x410660,0x410b10,0x410de0,0x410e80,0x410f70,0x410fb0,0x4114a0,0x411fd0]
two=[0x4100f0,0x4101c0,0x410700,0x4109d0,0x410a70,0x427a20,0x42a160,0x42b440,0x51f780]
zero=[0x42c740,0x42c9a0,0x42cee0,0x42d810,0x42e680]
ENDPOINTS=set(one+two+zero+[0x4ce310,0x4ce480,0x53f790,0x53f7d0,0x4cdf20,0x4cdeb0])
RANGES=[(0x53e590,0x53f242),(0x4cdad0,0x4cdb79),(0x4cdb80,0x4cdc2d)]
state={}
def read(a,n=4):return int.from_bytes(vm.mem_read(a,n),'little')
def write(a,v,n=4):vm.mem_write(a,(v&((1<<(8*n))-1)).to_bytes(n,'little'))
def cstring(a):return bytes(vm.mem_read(a,260)).split(b'\0',1)[0]
def fnv(text):
 value=2166136261
 for b in text:value=((value^b)*16777619)&0xffffffff
 return value
def guard(cpu,a,n,user):
 if a not in ENDPOINTS and not any(lo<=a and a+n-1<=hi for lo,hi in RANGES):raise RuntimeError('Native instruction escaped at '+hex(a))
def endpoint(cpu,a,n,user):
 sp=cpu.reg_read(UC_X86_REG_ESP);args=[read(sp+4+i*4) for i in range(5)];value=77;cleanup=0
 if a==0x4ce310:
  text=cstring(args[0]);text=text[:text.rindex(b'.')]+b'.555';vm.mem_write(args[0],text+b'\0');args=[fnv(text)]
 elif a in [0x4ce480,0x53f7d0]:
  args=args[:4 if a==0x4ce480 else 5];args[0]=fnv(cstring(args[0]));value=state['io']
  if a==0x53f7d0:cleanup=20
 elif a==0x53f790:
  args=args[:2];args[1]&=255
  if read(args[0])!=read(0xe92e1c) or args[1]:write(args[0],0)
 elif a in [0x4cdf20,0x4cdeb0]:args=args[:2]
 elif a in one:args=args[:1]
 elif a in two:
  args=args[:2]
  if a in two[:5]:args[1]&=65535
 else:args=[]
 state['events'].append([a,len(args)]+args+[0]*(6-len(args)))
 cpu.reg_write(UC_X86_REG_EAX,value);cpu.reg_write(UC_X86_REG_EIP,read(sp));cpu.reg_write(UC_X86_REG_ESP,sp+4+cleanup)
vm.hook_add(UC_HOOK_BLOCK,guard)
for address in ENDPOINTS:vm.hook_add(UC_HOOK_CODE,endpoint,begin=address,end=address)
def native(p):
 global state
 state={'events':[],'io':p[18]};obj=bytearray(0x30f5d8);struct.pack_into('<I',obj,0x30f5cc,p[16])
 for i in range(1024):
  offset=0x180e48+i*64
  struct.pack_into('<II',obj,offset,p[17],64);struct.pack_into('<IHH',obj,offset+16,p[9],p[1]&65535,p[2]&65535)
  obj[offset+0x34]=p[5]&255;obj[offset+0x33]=p[6]&255;obj[offset+0x32]=p[7]&255;obj[offset+0x38]=i&1
 if p[0]<1024:obj[0x180e48+p[0]*64+0x3e]=p[8]&255
 for i in range(2):
  text=f'picture{i}.bmp'.encode()+b'\0';obj[0x1f40+260+i*260:0x1f40+260+i*260+len(text)]=text
 vm.mem_write(OBJECT,bytes(obj));vm.mem_write(0xd3b530,b'\x5a'*180);vm.mem_write(0xe38aa8,b'\x5a'*260)
 write(0x70e120,p[10],2)
 for address,value in [(0xe38d9c,p[11]),(0xe38d98,p[12]),(0xe38db0,p[13]),(0xe38dac,p[14]),(0xe38d94,p[11]*2),
  (0xe92e1c,0x31200000),(0xe92e18,0x31100000),(0xd3b558,p[11]),(0xd3b550,p[12]),(0xd3b55c,p[19]),(0xd3b554,p[20]),(0xf56db4,p[0])]:write(address,value)
 write(0xe38dda,p[15],1);sp=STACK+0x180000
 for i,value in enumerate([STOP,p[0],p[3],p[4]]):write(sp+i*4,value)
 saved=[(UC_X86_REG_EBX,0x11111111),(UC_X86_REG_EBP,0x22222222),(UC_X86_REG_ESI,0x33333333),(UC_X86_REG_EDI,0x44444444)]
 for reg,value in saved:vm.reg_write(reg,value)
 vm.reg_write(UC_X86_REG_ECX,OBJECT);vm.reg_write(UC_X86_REG_ESP,sp);vm.reg_write(UC_X86_REG_EFLAGS,2)
 vm.emu_start(0x53e590,STOP,count=100000)
 assert vm.reg_read(UC_X86_REG_EIP)==STOP,'Instruction limit exhausted'
 assert vm.reg_read(UC_X86_REG_ESP)==sp+16,'thiscall stack cleanup differs'
 assert all(vm.reg_read(r)==v for r,v in saved),'Saved register differs'
 value=vm.reg_read(UC_X86_REG_EAX);value=value if value<0x80000000 else value-0x100000000
 return {'result':value,'globals':bytes(vm.mem_read(0xd3b530,180)).hex(),'filename':bytes(vm.mem_read(0xe38aa8,260)).hex(),
  'scratch':read(OBJECT+0x30f5c0),'events':state['events']}
default=[5,8,7,2,3,0,0,1,0,1,0,640,480,640,480,0,16,0x31000000,1,0,0]
cases=[]
def add(**kwargs):
 p=default.copy()
 for k,v in kwargs.items():p[int(k[1:])]=v&0xffffffff
 cases.append(p)
add()
for ident in [0,1,15,16,509,0x639b,0x639c,0x7fffffff,0xffffffff]:add(p0=ident,p16=683)
for kind in [0,1,2,10,12,13,20,30]:
 for compressed in [0,1]:
  for alias in [0,1]:
   for x,y in [(2,3),(-3,3),(637,3),(640,3),(2,-3),(2,477),(2,480),(-640,-480)]:
    for tint in [0,0x1234]:add(p7=kind,p6=compressed,p8=alias,p3=x,p4=y,p10=tint)
for width in [1,10,26,58,118,178,238,298,358,641,1024]:
 for external in [0,1]:
  for kind in [0,1,30]:
   for io in [0,1]:add(p1=width,p5=external,p7=kind,p18=io)
for game in [0,1,20,21]:add(p15=game)
for width,height in [(0,7),(-1,7),(8,0),(8,-1),(32767,32767)]:add(p1=width,p2=height)
for delta in [1,10,11,0xffffffff]:add(p8=1,p9=delta,p5=1)
for count in [0,5,6,0xffffffff]:add(p16=count,p5=1)
rng=random.Random(0x53e590)
for i in range(500):
 add(p1=rng.choice([1,4,10,26,58,118,178,238,298,358,640,1024]),p2=rng.choice([1,6,14,30,480,768]),p3=rng.choice([-32,-1,0,2,630,640]),
  p4=rng.choice([-32,-1,0,3,470,480]),p5=rng.randrange(2),p6=rng.randrange(2),p7=rng.choice([0,1,2,10,12,13,20,30]),p8=rng.randrange(2),
  p10=rng.choice([0,0x1234,0xffff]),p18=rng.randrange(2),p19=rng.choice([0,4]),p20=rng.choice([0,4]))
exe=ROOT/'build/c-only-debug/Debug/restore_render_verify.exe'
process=subprocess.Popen([str(exe),'--oracle-batch'],cwd=ROOT,stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True)
report={'format':1,'unicorn_version':unicorn_version,'original_game_executed':False,'native_entry':'0x0053e590',
 'method':'Original dispatcher and both clippers versus compiled C; ordered controlled endpoint arguments and all 180 bytes of clip/draw state compared',
 'limitations':['Low-level raster, file IO and color conversion bodies are controlled endpoints here; pixel copies have a separate comparison.',
  'Only the low 16 bits of pixel modifiers are compared, matching their word-valued consumers; bounded path construction rejects oversized or invalid metadata.'],
 'compiled_C_sha256':hashlib.sha256(exe.read_bytes()).hexdigest(),'original_sha256':hashlib.sha256(image).hexdigest()}
checked=0;case=None;started=time.monotonic()
try:
 assert json.loads(process.stdout.readline())=={'base':OBJECT}
 for case in cases:
  process.stdin.write(' '.join(map(str,case))+'\n');process.stdin.flush();actual=json.loads(process.stdout.readline());expected=native(case)
  if actual!=expected:
   report['differences']={k:{'C':actual[k],'native':expected[k]} for k in actual if actual[k]!=expected[k]}
   for key in ['globals','filename']:
    if actual[key]!=expected[key]:report[key+'_first_bytes']=[{'offset':i,'C':a,'native':b} for i,(a,b) in enumerate(zip(bytes.fromhex(actual[key]),bytes.fromhex(expected[key]))) if a!=b][:12]
   raise AssertionError('State, result or endpoint arguments differ')
  checked+=1
 report.update(passed=True,checked=checked)
except Exception as error:report.update(passed=False,checked=checked,case=case,error=str(error),native_eip=hex(vm.reg_read(UC_X86_REG_EIP)))
finally:
 process.stdin.close();process.wait(timeout=10)
report['elapsed_seconds']=round(time.monotonic()-started,3)
files=['src/unit_02524.c','src/unit_01725.c','src/unit_01726.c','include/sprite_render.h','tests/sprite_render.c','tools/verify_sprite_render_emulation.py']
report['source_hashes']={f:hashlib.sha256((ROOT/f).read_bytes()).hexdigest() for f in files}
(ROOT/'metadata/sprite-render-emulation.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({k:v for k,v in report.items() if k not in ['source_hashes','differences']},indent=2));sys.exit(0 if report['passed'] else 1)
