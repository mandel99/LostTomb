"""Exact-pixel native/C comparison for copy, mask, shadow RLE and row skip.

All ten native bodies execute in Unicorn; all ten real C bodies compose in
the isolated oracle. No drawing or row-skip algorithm is a controlled endpoint.
"""
from pathlib import Path
import argparse,bisect,hashlib,json,random,struct,subprocess,sys,time
ROOT=Path(__file__).resolve().parents[1]
sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_BLOCK,UC_HOOK_CODE,UC_PROT_READ,__version__
from unicorn.x86_const import *
parser=argparse.ArgumentParser(description=__doc__)
parser.add_argument('--exe',type=Path,default=ROOT/'build/agent-upper-raster/Debug/restore_upper_raster_verify.exe')
args=parser.parse_args();exe=args.exe.resolve()
image=(ROOT/'build/smoke-game/Pharaoh.exe').read_bytes();SHA=hashlib.sha256(image).hexdigest()
assert SHA=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
functions={}
for name in ['metadata/upper-raster-native.json','metadata/upper-raster-shadow-native.json']:
 d=json.loads((ROOT/name).read_text());assert d['native_sha256']==SHA
 for a,f in d['functions'].items():
  assert not f['assembly']['has_more'] and len(f['assembly']['items'])==f['assembly']['total']
  functions[int(a,16)]=f['assembly']['items']
u16=lambda p:struct.unpack_from('<H',image,p)[0]
u32=lambda p:struct.unpack_from('<I',image,p)[0]
pe=u32(0x3c);opt=pe+24;table=opt+u16(pe+20);base=u32(opt+28);size=u32(opt+56)
vm=Uc(UC_ARCH_X86,UC_MODE_32);vm.mem_map(base,(size+4095)&~4095);vm.mem_write(base,image[:u32(opt+60)])
for i in range(u16(pe+6)):
 p=table+i*40;vm.mem_write(base+u32(p+12),image[u32(p+20):u32(p+20)+u32(p+16)])
allowed=sorted(int(i['address'],16) for b in functions.values() for i in b);allowed_set=set(allowed)
for b in functions.values():
 for i in b:
  raw=bytes.fromhex(i['bytes']);assert bytes(vm.mem_read(int(i['address'],16),len(raw)))==raw
SRC=0x31000000;DST=0x31100000;CAPACITY=0x20000;STOP=0x33000000;STACK=0x34000000
for a,n in [(SRC,CAPACITY),(DST,CAPACITY),(STOP,0x1000),(STACK,0x10000)]:vm.mem_map(a,n)
vm.mem_protect(SRC,CAPACITY,UC_PROT_READ)
entries={0:0x410230,1:0x4105c0,2:0x410660,3:0x410700,4:0x4109d0,5:0x410a70,6:0x410f20,10:0x410b10,11:0x410de0,12:0x410e80}
read=lambda a,n=4:int.from_bytes(vm.mem_read(a,n),'little')
def write(a,v,n=4):vm.mem_write(a,(v&((1<<(8*n))-1)).to_bytes(n,'little'))
executed=set();block_cache={};state={}
def block(cpu,a,n,user):
 assert a in allowed_set,'Unexpected native block '+hex(a)
 key=(a,n)
 if key not in block_cache:block_cache[key]=allowed[bisect.bisect_left(allowed,a):bisect.bisect_left(allowed,a+n)]
 executed.update(block_cache[key])
def helper_entry(cpu,a,n,user):state['skips'].append([read(cpu.reg_read(UC_X86_REG_ESP)+4),None])
def helper_return(cpu,a,n,user):state['skips'][-1][1]=cpu.reg_read(UC_X86_REG_EAX)
vm.hook_add(UC_HOOK_BLOCK,block)
vm.hook_add(UC_HOOK_CODE,helper_entry,begin=0x410f20,end=0x410f20)
vm.hook_add(UC_HOOK_CODE,helper_return,begin=0x410f6b,end=0x410f6b)
initial_frame=bytes(((i*13+(i>>7)*7)^0xc7)&255 for i in range(CAPACITY))
def source_data(p):
 width=p[1]&65535;width=width if width<32768 else width-65536;skip=p[5] if p[5]<0x80000000 else p[5]-0x100000000
 data=bytearray(b'\xa5'*CAPACITY);cursor=0
 if width<=0:return bytes(data)
 for row in range(p[2]+max(0,skip)+1):
  col=0
  if p[10]==20:data[cursor:cursor+2]=b'\xff\0';cursor+=2
  while col<width:
   run=p[10] if 1<=p[10]<=16 else 1+(row+col+p[11])%16
   transparent=p[10]==18 or p[10] in [17,19] and (row+col+p[11])%3==0
   if p[10]==18:run=255
   run=min(run,width-col);data[cursor]=255 if transparent else run;cursor+=1
   if transparent:data[cursor]=run;cursor+=1
   else:
    for i in range(run):struct.pack_into('<H',data,cursor,((row*width+col+i)*37+p[11])&65535);cursor+=2
   col+=run
 assert cursor<CAPACITY;return bytes(data)
def invoke(mode,mask):
 sp=STACK+0x8000;vm.mem_write(sp-0x200,b'\xcc'*0x400)
 for i,v in enumerate([STOP,SRC,mask&65535]):write(sp+i*4,v)
 saved=[(UC_X86_REG_EBX,0x11223344),(UC_X86_REG_EBP,0x22334455),(UC_X86_REG_ESI,0x33445566),(UC_X86_REG_EDI,0x44556677)]
 for r,v in saved+[(UC_X86_REG_ESP,sp),(UC_X86_REG_EFLAGS,2)]:vm.reg_write(r,v)
 vm.emu_start(entries[mode],STOP,count=1000000)
 assert vm.reg_read(UC_X86_REG_EIP)==STOP and vm.reg_read(UC_X86_REG_ESP)==sp+4
 assert all(vm.reg_read(r)==v for r,v in saved)
 assert bytes(vm.mem_read(sp+12,0x100))==b'\xcc'*0x100
 return vm.reg_read(UC_X86_REG_EAX) if mode in [1,2,6,11,12] else 0
def native(p):
 source=source_data(p);vm.mem_write(SRC,source);vm.mem_write(DST,initial_frame);vm.mem_write(0xd3b520,b'\x5a'*224)
 for a,v in [(0xd3b5d4,p[2]),(0xd3b5e0,p[3]),(0xd3b5dc,p[4]),(0xd3b538,p[5]),(0xd3b540,p[6]*2),
  (0xd3b544,p[7]),(0xe92e18,DST),(0xe38d94,p[8]),(0xd3b53c,p[8]-p[1]*2)]:write(a,v)
 write(0xd3b5be,p[1],2);write(0xd3b51c,p[9],2);state.clear();state['skips']=[]
 if p[0] in [7,8,9]:assert invoke(p[0]-7,p[9])==0;invoke(p[0]-4,p[9]);result=0
 else:result=invoke(p[0],p[9])
 assert bytes(vm.mem_read(SRC,CAPACITY))==source
 return dict(result=result,state=bytes(vm.mem_read(0xd3b520,224)),frame=bytes(vm.mem_read(DST,CAPACITY)),skips=state['skips'])
cases=[];default=[2,58,2,300,8,1,29,29,2048,0xf863,17,123]
def add(**kwargs):
 p=default.copy()
 for key,v in kwargs.items():p[int(key[1:])]=v&0xffffffff
 cases.append(p)
draw_modes=[0,1,2,3,4,5,10,11,12]
for mode in draw_modes:
 for width in [1,7,31,58,358]:
  for pattern in range(1,21):add(p0=mode,p1=width,p6=width//2,p7=width//2,p10=pattern)
 for width in [58,118,178,238,298,358,640]:
  for cutoff in [0,width//2,width,width+4]:
   for skip in [0,4]:add(p0=mode,p1=width,p5=skip,p6=cutoff,p7=cutoff,p10=17)
for mode in [3,4,5,10,11,12]:
 for mask in [0,1,0x100,0xf863,0xffff]:
  for cutoff in [0,29,60]:add(p0=mode,p6=cutoff,p7=cutoff,p9=mask)
for mode in [7,8,9]:
 for width in [58,118,358]:
  for mask in [0xff00,0xf863]:
   for cutoff in [0,29,100]:add(p0=mode,p1=width,p6=cutoff,p7=cutoff,p9=mask,p2=3)
for width in [0,1,7,58,358,640,65535,32768]:
 for skip in [-1,0,1,6]:add(p0=6,p1=width,p5=skip)
for transparent in range(1,256):
 for mode in [0,6]:add(p0=mode,p1=transparent,p10=18)
rng=random.Random(0x410660)
for _ in range(120):
 width=rng.choice([1,7,31,58,118,358,640]);pitch=rng.choice([1280,1600,2048]);x=max(0,min(300,pitch//2-width-4))
 add(p0=rng.choice(draw_modes+[7,8,9]),p1=width,p2=rng.choice([1,2,7]),p3=x,p4=rng.choice([0,3,8]),p5=rng.choice([0,1,6]),
  p6=rng.randrange(width+1),p7=rng.randrange(width+1),p8=pitch,p9=rng.choice([0,0x100,0xffff,0xf863]),p10=rng.randrange(1,21),p11=rng.choice([123,54321]))
process=subprocess.Popen([str(exe),'--oracle-batch'],cwd=ROOT,stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE)
def exact(n):
 data=bytearray()
 while len(data)<n:
  part=process.stdout.read(n-len(data))
  if not part:raise RuntimeError('C oracle stopped: '+process.stderr.read().decode())
  data.extend(part)
 return bytes(data)
report=dict(format=1,passed=False,checked=0,native_functions=10,restored_native_functions=7,original_game_executed=False,
 original_sha256=SHA,compiled_C_sha256=hashlib.sha256(exe.read_bytes()).hexdigest(),unicorn_version=__version__,
 method='Compose actual native/C copy, mask and solid-shadow RLE consumers with actual row-skip helper. Compare every byte of128KiB frame and224 draw/guard bytes, unchanged read-only source, helper entry/returned pointer sequence, integer zero results, stack and saved registers. Include paired base-copy/mask passes. No controlled drawing endpoints.',
 limitations=['Raster equivalence uses positive signed-word widths, positive visible row counts and complete RLE rows. Literal controls1..16 and transparent controls0..255 are covered. C raster consumers reject nonpositive dimensions; native do/while would execute a first row.',
  'Helper-only cases include zero/negative signed-word widths and negative/zero skip counts; pointer result and preserved registers are checked.',
  'This synthetic raster proof does not establish full city rendering or validate malformed streams that escape source/frame storage.'])
start=time.monotonic();case=None
try:
 assert struct.unpack('<I',exact(4))[0]==0x55524c45
 for case in cases:
  process.stdin.write((' '.join(map(str,case))+'\n').encode());process.stdin.flush()
  result,count=struct.unpack('<II',exact(8));actual=dict(result=result,state=exact(224),frame=exact(CAPACITY),skips=[list(struct.unpack('<II',exact(8))) for _ in range(count)])
  expected=native(case)
  if actual!=expected:
   report['differences']={}
   for key in actual:
    if actual[key]==expected[key]:continue
    if isinstance(actual[key],bytes):report['differences'][key]=[dict(offset=i,C=a,native=b) for i,(a,b) in enumerate(zip(actual[key],expected[key])) if a!=b][:20]
    else:report['differences'][key]=dict(C=actual[key],native=expected[key])
   raise AssertionError('Pixel/state/return/skip-pointer difference')
  report['checked']+=1
 report['passed']=True
except Exception as error:report.update(error=str(error),case=case,native_eip=hex(vm.reg_read(UC_X86_REG_EIP)))
finally:
 process.stdin.close();process.wait(timeout=10)
report['elapsed_seconds']=round(time.monotonic()-start,3)
report['instruction_coverage']={f'{a:08x}':dict(total=len(b),executed=sum(int(i['address'],16) in executed for i in b),missing=[i for i in b if int(i['address'],16) not in executed]) for a,b in functions.items()}
report['total_instruction_coverage']=dict(total=len(allowed),executed=len(executed))
if report['passed'] and len(executed)!=len(allowed):report['passed']=False;report['error']='Target instruction coverage incomplete'
files=[f'src/unit_{n:05d}.c' for n in range(140,150)]+['runtime/upper_raster.c','runtime/sprite_pixels.c','include/upper_raster.h','include/sprite_render.h','tests/upper_raster.c','tools/verify_upper_raster_emulation.py','metadata/upper-raster-native.json','metadata/upper-raster-shadow-native.json']
report['source_hashes']={p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest() for p in files}
(ROOT/'metadata/upper-raster-emulation.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({k:v for k,v in report.items() if k not in ['source_hashes','differences','instruction_coverage']},indent=2))
print(json.dumps({a:{k:v for k,v in c.items() if k!='missing'} for a,c in report['instruction_coverage'].items()},indent=2))
sys.exit(0 if report['passed'] else 1)
