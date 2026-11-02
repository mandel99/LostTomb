"""Staged exact-pixel native/C proof for eleven terrain diamond procedures.

No original process runs. The complete original raster/composer bodies and
both proven clippers execute in Unicorn; the staged C composes real clippers
and real packed-pixel loops without controlled drawing endpoints.
"""
from pathlib import Path
import argparse,hashlib,json,random,struct,subprocess,sys,time
SCRIPT=Path(__file__).resolve()
if (SCRIPT.parents[1]/'metadata/terrain-diamond-native.json').exists():
 ROOT=SCRIPT.parents[1];PHASE=ROOT/'artifacts/terrain_diamond_phase'
else:
 PHASE=SCRIPT.parents[1];ROOT=PHASE.parents[1]
sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE,UC_HOOK_MEM_WRITE,UC_PROT_READ,__version__
from unicorn.x86_const import *
parser=argparse.ArgumentParser(description=__doc__)
parser.add_argument('--production',action='store_true',help='Bind actual restored src/runtime/include/test files and write the shared production report.')
parser.add_argument('--exe',type=Path)
args=parser.parse_args()
exe=(args.exe or ROOT/('build/c-only-debug/Debug/restore_terrain_diamond_verify.exe' if args.production else 'build/agent-terrain-diamond/Debug/restore_terrain_diamond_verify.exe')).resolve()
image=(ROOT/'build/smoke-game/Pharaoh.exe').read_bytes();SHA=hashlib.sha256(image).hexdigest()
assert SHA=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
evidence=json.loads((ROOT/'metadata/terrain-diamond-native.json').read_text());assert evidence['native_sha256']==SHA
functions={int(a,16):f['assembly']['items'] for a,f in evidence['functions'].items()}
for f in evidence['functions'].values():assert not f['assembly']['has_more'] and len(f['assembly']['items'])==f['assembly']['total']
LEAVES=[0x427a20,0x428c00,0x42a160,0x42aa00,0x42b440,0x42bcf0]
COMPOSERS=[0x42c740,0x42c9a0,0x42cee0,0x42d810,0x42e680]
u16=lambda p:struct.unpack_from('<H',image,p)[0]
u32=lambda p:struct.unpack_from('<I',image,p)[0]
pe=u32(0x3c);opt=pe+24;table=opt+u16(pe+20);base=u32(opt+28);size=u32(opt+56)
vm=Uc(UC_ARCH_X86,UC_MODE_32);vm.mem_map(base,(size+4095)&~4095);vm.mem_write(base,image[:u32(opt+60)])
for i in range(u16(pe+6)):
 p=table+i*40;vm.mem_write(base+u32(p+12),image[u32(p+20):u32(p+20)+u32(p+16)])
allowed={int(i['address'],16) for b in functions.values() for i in b}
for b in functions.values():
 for i in b:
  raw=bytes.fromhex(i['bytes']);assert bytes(vm.mem_read(int(i['address'],16),len(raw)))==raw
SOURCE=0x30000000;FRAME=0x31000000;SOURCE_BYTES=0x20000;FRAME_BYTES=0x100000;STOP=0x33000000;STACK=0x34000000
for a,n in [(SOURCE,SOURCE_BYTES),(FRAME,FRAME_BYTES),(STOP,0x1000),(STACK,0x10000)]:vm.mem_map(a,n)
vm.mem_protect(SOURCE,SOURCE_BYTES,UC_PROT_READ)
executed=set();state={};writes=set()
def read(a,n=4):return int.from_bytes(vm.mem_read(a,n),'little')
def write(a,v,n=4):vm.mem_write(a,(v&((1<<(8*n))-1)).to_bytes(n,'little'))
def event(a,p=0,c=0):state['trace'].append([a,p,c,read(0xd3b5e0),read(0xd3b5dc),read(0xd3b5be,2),read(0xd3b5bc,2)])
def hook(cpu,a,n,user):
 if a in functions:
  sp=cpu.reg_read(UC_X86_REG_ESP);event(a,read(sp+4) if a in LEAVES else 0,read(sp+8) if a in LEAVES else 0)
 elif a in [0x4cdad0,0x4cdb80]:event(a)
 if a in allowed:executed.add(a);return
 assert 0x4cdad0<=a and a+n-1<=0x4cdb79 or 0x4cdb80<=a and a+n-1<=0x4cdc2d,'Execution escaped at '+hex(a)
def guard(cpu,access,a,n,value,user):
 if FRAME<=a and a+n<=FRAME+FRAME_BYTES:return
 if STACK<=a and a+n<=STACK+0x10000:return
 assert 0xd3b520<=a and a+n<=0xd3b600,'Unexpected native write '+hex(a)
 writes.add((a,n))
vm.hook_add(UC_HOOK_CODE,hook);vm.hook_add(UC_HOOK_MEM_WRITE,guard)
patterns={}
def pattern(seed):
 if seed not in patterns:patterns[seed]=bytes(((i*37+(i>>8)*19+seed*11)^((i>>4)*13))&255 for i in range(SOURCE_BYTES))
 return patterns[seed]
def native(p):
 state.clear();state['trace']=[];source=pattern(p[11]);vm.mem_write(SOURCE,source);vm.mem_write(FRAME,b'\xc7'*FRAME_BYTES)
 vm.mem_write(0xd3b520,b'\x5a'*224)
 for a,v in [(0xd3b5e0,p[2]),(0xd3b5dc,p[3]),(0xe38d94,p[4]),(0xe38d9c,p[4]//2),(0xe92e18,FRAME),
  (0xd3b5b8,SOURCE+0x100),(0xe36c80,p[10]),(0xd3b55c,p[5]),(0xd3b558,p[6]),(0xd3b554,p[7]),(0xd3b550,p[8])]:write(a,v)
 write(0x70e120,p[9],2);sp=STACK+0x8000;vm.mem_write(sp-0x200,b'\xcc'*0x400)
 for i,v in enumerate([STOP,SOURCE+0x100+p[10],p[1]]):write(sp+4*i,v)
 saved=[(UC_X86_REG_EBX,0x11223344),(UC_X86_REG_EBP,0x22334455),(UC_X86_REG_ESI,0x33445566),(UC_X86_REG_EDI,0x44556677)]
 for r,v in saved+[(UC_X86_REG_EAX,0xa1b2c3d4),(UC_X86_REG_ECX,0xb2c3d4e5),(UC_X86_REG_EDX,0xc3d4e5f6),(UC_X86_REG_ESP,sp),(UC_X86_REG_EFLAGS,2)]:vm.reg_write(r,v)
 vm.emu_start(p[0],STOP,count=200000)
 assert vm.reg_read(UC_X86_REG_EIP)==STOP and vm.reg_read(UC_X86_REG_ESP)==sp+4
 assert all(vm.reg_read(r)==v for r,v in saved)
 assert bytes(vm.mem_read(sp+12,0x100))==b'\xcc'*0x100
 assert bytes(vm.mem_read(SOURCE,SOURCE_BYTES))==source
 eax=vm.reg_read(UC_X86_REG_EAX)
 if p[0] in LEAVES:
  expected=p[1] if p[0] in [0x427a20,0x42a160,0x42b440] and p[9]&65535 else 0xa1b2c3d4
  assert eax==expected,'Native register-preservation carrier changed';eax=0
 return dict(result=eax,state=bytes(vm.mem_read(0xd3b520,224)),frame=bytes(vm.mem_read(FRAME,FRAME_BYTES)),trace=state['trace'])
default=[0x427a20,0,80,40,1024,0,400,0,240,0,0,17]
cases=[]
def add(**kwargs):
 p=default.copy()
 for k,v in kwargs.items():p[int(k[1:])]=v&0xffffffff
 cases.append(p)
for entry in LEAVES:
 for clip in [0,1,2,3,4,5]:
  for tint in [0,0x100,0xff00,0xffff]:add(p0=entry,p1=clip,p9=tint)
 for pitch in [1024,1280,2048]:
  for x,y in [(0,0),(300,150),(80,40)]:add(p0=entry,p2=x,p3=y,p4=pitch,p9=0xf863,p10=4096,p11=23)
for entry in COMPOSERS:
 for x in [150,-30,300,-300,500,0,370]:
  for y in [20,-15,220,-300,300]:
   for tint in [0,0x100,0xf863]:add(p0=entry,p2=x,p3=y,p9=tint,p10=4096,p11=23)
 for pitch in [1024,1280,2048]:add(p0=entry,p2=180,p3=40,p4=pitch,p9=0xff00)
rng=random.Random(0x427a20)
for i in range(200):
 add(p0=rng.choice(COMPOSERS),p2=rng.choice([-90,-30,0,90,150,300,370]),p3=rng.choice([-30,-15,0,30,210,225]),
  p4=rng.choice([1024,1280,2048]),p5=rng.choice([0,4]),p6=rng.choice([400,500]),p7=rng.choice([0,4]),p8=rng.choice([220,240]),p9=rng.choice([0,1,0xff00,0xf863]),p10=rng.choice([0,4096]),p11=rng.choice([17,23]))
# Position each individual constituent at all horizontal/vertical clip classes.
# This exercises each unrolled native branch, not merely each footprint size.
seen={tuple(p) for p in cases}
for size,entry in enumerate(COMPOSERS,2):
 for diagonal in range(2*size-1):
  count=diagonal+1 if diagonal<size else 2*size-1-diagonal
  for column in range(count):
   dx=(2*column-count+1)*30;dy=diagonal*15
   for x in [-30,150,372]:
    for y in [-15,50,225]:
     p=default.copy();p[0]=entry;p[2]=(x-dx)&0xffffffff;p[3]=(y-dy)&0xffffffff
     if tuple(p) not in seen:cases.append(p);seen.add(tuple(p))
process=subprocess.Popen([str(exe),'--oracle-batch'],cwd=ROOT,stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE)
def exact(n):
 b=bytearray()
 while len(b)<n:
  chunk=process.stdout.read(n-len(b))
  if not chunk:raise RuntimeError('C oracle stopped: '+process.stderr.read().decode())
  b.extend(chunk)
 return bytes(b)
report=dict(format=1,passed=False,checked=0,native_functions=11,production_sources=args.production,original_game_executed=False,original_sha256=SHA,
 compiled_C_sha256=hashlib.sha256(exe.read_bytes()).hexdigest(),unicorn_version=__version__,
 method='Complete native11-function diamond module plus actual native clippers versus staged compact C module plus compiled clippers. Exact1MiB framebuffer,224 draw/guard bytes, full source preservation, entry/clip/leaf order and arguments, composer return58, stack and saved registers compared. No controlled drawing endpoints.',
 limitations=['Packed valid1800-byte diamonds and source buffers large enough for N*N blocks; native fixed half-cell edge behavior is preserved rather than generalized clipping. Inputs that would escape the owned framebuffer/source are excluded.',
  'This isolated proof establishes synthetic packed-pixel equivalence, not game art-ID production, sprite library loading or a live city frame.',
  'Six native leaves preserve EAX via PUSHAD/POPAD. All retained callers overwrite or replace that value before an explicit read; C contracts are void. Native volatile carrier values are checked separately, not exposed as invented C return values.'])
start=time.monotonic();case=None
try:
 assert struct.unpack('<I',exact(4))[0]==0x54444941
 for case in cases:
  process.stdin.write((' '.join(map(str,case))+'\n').encode());process.stdin.flush()
  result,count=struct.unpack('<II',exact(8));actual=dict(result=result,state=exact(224),frame=exact(FRAME_BYTES),trace=[list(struct.unpack('<7I',exact(28))) for _ in range(count)])
  expected=native(case)
  if actual!=expected:
   differences={}
   for key in actual:
    if actual[key]==expected[key]:continue
    if isinstance(actual[key],bytes):differences[key]=[dict(offset=i,C=a,native=b) for i,(a,b) in enumerate(zip(actual[key],expected[key])) if a!=b][:20]
    else:differences[key]=dict(C=actual[key],native=expected[key])
   report['differences']=differences;raise AssertionError('Pixel/state/trace/return mismatch')
  report['checked']+=1
 report['passed']=True
except Exception as error:report.update(error=str(error),case=case,native_eip=hex(vm.reg_read(UC_X86_REG_EIP)))
finally:
 process.stdin.close();process.wait(timeout=10)
report['elapsed_seconds']=round(time.monotonic()-start,3)
report['instruction_coverage']={f'{a:08x}':dict(total=len(b),executed=sum(int(i['address'],16) in executed for i in b),missing=[i for i in b if int(i['address'],16) not in executed]) for a,b in functions.items()}
report['total_instruction_coverage']=dict(total=len(allowed),executed=len(executed))
if report['passed'] and len(executed)!=len(allowed):report['passed']=False;report['error']='Target instruction coverage incomplete'
report['native_global_write_widths']=[dict(address=hex(a),bytes=n) for a,n in sorted(writes)]
if args.production:
 files=[ROOT/f'src/unit_{n:05d}.c' for n in range(435,446)]+[ROOT/'runtime/terrain_diamond.c',ROOT/'include/terrain_diamond.h',ROOT/'tests/terrain_diamond.c',SCRIPT]
else:
 files=sorted([p for folder in ['src','include','runtime','tests','tools'] for p in (PHASE/folder).rglob('*') if p.is_file() and '__pycache__' not in str(p)])+[PHASE/'CMakeLists.txt']
files+=[ROOT/'src/unit_01725.c',ROOT/'src/unit_01726.c',ROOT/'metadata/terrain-diamond-native.json']
report['source_hashes']={str(p.relative_to(ROOT)).replace('\\','/'):hashlib.sha256(p.read_bytes()).hexdigest() for p in files}
destination=ROOT/'metadata/terrain-diamond-emulation.json' if args.production else PHASE/'metadata/emulation.json'
destination.parent.mkdir(exist_ok=True)
destination.write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({k:v for k,v in report.items() if k not in ['source_hashes','instruction_coverage','differences']},indent=2))
print(json.dumps({a:{k:v for k,v in c.items() if k!='missing'} for a,c in report['instruction_coverage'].items()},indent=2))
sys.exit(0 if report['passed'] else 1)
