"""Compare the complete opaque16x16 pixel copy against pinned original x86."""
from pathlib import Path
import argparse,hashlib,json,random,struct,subprocess,sys,time
ROOT=Path(__file__).resolve().parent.parent
sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE,UC_HOOK_MEM_WRITE
from unicorn.x86_const import *
parser=argparse.ArgumentParser(description=__doc__)
parser.add_argument('--exe',type=Path,required=True)
parser.add_argument('--optimized-exe',type=Path)
parser.add_argument('--production',action='store_true')
args=parser.parse_args();started=time.monotonic();MASK=0xffffffff
sha=lambda b:hashlib.sha256(b).hexdigest()
image=(ROOT/'build/smoke-game/Pharaoh.exe').read_bytes()
canonical='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
assert sha(image)==canonical
u16=lambda p:struct.unpack_from('<H',image,p)[0]
u32=lambda p:struct.unpack_from('<I',image,p)[0]
pe=u32(60);opt=pe+24;table=opt+u16(pe+20);base=u32(opt+28);size=u32(opt+56)
vm=Uc(UC_ARCH_X86,UC_MODE_32);vm.mem_map(base,(size+4095)&~4095)
for i in range(u16(pe+6)):
 q=table+i*40;vm.mem_write(base+u32(q+12),image[u32(q+20):u32(q+20)+u32(q+16)])
BASE=0x30000000;CAP=65536;STACK=0x34000000;STOP=0x33000000;SP=STACK+0x8000
for a,n in [(BASE,CAP),(STACK,65536),(STOP,4096)]:vm.mem_map(a,n)
regions=[(BASE,CAP),(0xd3b5d0,32),(0xe38d90,16),(0xe92e10,16)]
export='metadata/raw-16x16-tile-native.json'
body=json.loads((ROOT/export).read_text())['functions']['00410fb0']['assembly']
assert not body['has_more'] and len(body['items'])==291
allowed={int(x['address'],16) for x in body['items']};hits=set();writes=[]
for x in body['items']:
 raw=bytes.fromhex(x['bytes']);assert bytes(vm.mem_read(int(x['address'],16),len(raw)))==raw
def code(cpu,a,n,user):
 assert a in allowed,hex(a)
 hits.add(a)
def store(cpu,access,a,n,value,user):
 if a>=STACK and a+n<=STACK+65536:return
 assert BASE<=a and a+n<=BASE+CAP,hex(a)
 writes.append((a,n,value))
vm.hook_add(UC_HOOK_CODE,code);vm.hook_add(UC_HOOK_MEM_WRITE,store)
def write(a,v):vm.mem_write(a,struct.pack('<I',v&MASK))
def native(p):
 state=p[5];memory=bytearray(CAP)
 for i in range(CAP):state=(state*1664525+1013904223)&MASK;memory[i]=state>>24
 vm.mem_write(BASE,bytes(memory))
 for a,n in regions[1:]:vm.mem_write(a,b'\x5a'*n)
 for a,v in [(0xe92e18,BASE+p[1]),(0xd3b5e0,p[3]),(0xd3b5dc,p[4]),(0xe38d94,p[2]),(SP,STOP),(SP+4,BASE+p[0])]:write(a,v)
 for reg,value in [(UC_X86_REG_EBX,0x11111111),(UC_X86_REG_ESI,0x22222222),(UC_X86_REG_EDI,0x33333333),(UC_X86_REG_EBP,0x44444444),(UC_X86_REG_ESP,SP)]:vm.reg_write(reg,value)
 writes.clear();vm.emu_start(0x410fb0,STOP,count=1000)
 assert vm.reg_read(UC_X86_REG_EIP)==STOP
 for reg,value in [(UC_X86_REG_EBX,0x11111111),(UC_X86_REG_ESI,0x22222222),(UC_X86_REG_EDI,0x33333333),(UC_X86_REG_EBP,0x44444444),(UC_X86_REG_ESP,SP+4)]:assert vm.reg_read(reg)==value
 assert len(writes)==128 and all(n==4 for a,n,v in writes)
 return {'memory':sha(b''.join(bytes(vm.mem_read(a,n)) for a,n in regions))}
cases=[]
def add(source,dest,pitch,x=0,y=0,seed=1):
 assert 0<=source<=CAP-512
 assert all(0<=dest+row*pitch and dest+row*pitch+32<=CAP for row in range(16))
 cases.append(tuple(v&MASK for v in [source,dest-x*2-y*pitch,pitch,x,y,seed]))
for pitch in [0,1,2,4,16,30,32,33,64,511,1024,2048,-1,-2,-32,-1024]:
 for source in [0,1,3,4096,65024]:
  for x,y in [(0,0),(300,8),(-300,-8),(65536,65537),(0x80000000,0x7fffffff)]:add(source,32768,pitch,x,y,source+pitch)
for pitch in [0,1,2,4,16,32,34,-1,-16,-32]:
 for overlap in [-32,-4,-1,0,1,4,8,16,32,128]:add(8192,8192+overlap,pitch,256,-257,overlap+pitch)
rng=random.Random(0x410fb0)
for i in range(500):
 pitch=rng.randrange(-1200,1201);dest=rng.randrange(20000,40001)
 if all(0<=dest+row*pitch and dest+row*pitch+32<=CAP for row in range(16)):
  add(rng.randrange(CAP-511),dest,pitch,rng.getrandbits(32),rng.getrandbits(32),rng.getrandbits(32))
cases=list(dict.fromkeys(cases));assert len(cases)==1000
variants=[args.exe.resolve()]+([args.optimized_exe.resolve()] if args.optimized_exe else [])
children=[];report={'passed':False,'checked':0,'comparison_checked':0,'stage':'production',
 'native_sha256':canonical,'compiled_C_sha256':sha(variants[0].read_bytes()),
 'complete_case_set':True,'case_count':len(cases),'controlled_algorithm_endpoints':[],
 'watched_bytes':sum(n for a,n in regions),'native_writes_per_case':128,
 'scope':'All291 native instructions, all128 ordered DWORD writes, complete64KiB pixel arena and64 global bytes; independent, overlapping, unaligned, wrapped-coordinate and negative-pitch valid mappings.',
 'limitations':['The caller must supply valid512-byte source and32-byte row destinations. Clipping, asset selection and whole-game behavior are separate contracts.']}
if len(variants)>1:report['comparison_C_sha256']=sha(variants[1].read_bytes())
try:
 for exe in variants:
  smoke=subprocess.run([str(exe)],cwd=ROOT,capture_output=True,text=True,timeout=30);assert smoke.returncode==0,smoke.stdout+smoke.stderr
  child=subprocess.Popen([str(exe),'--oracle-batch'],cwd=ROOT,stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True)
  assert json.loads(child.stdout.readline())=={'fields':6};children.append(child)
 for p in cases:
  expected=native(p)
  for i,child in enumerate(children):
   child.stdin.write(' '.join(map(str,p))+'\n');child.stdin.flush();line=child.stdout.readline();assert line,'C child ended'
   actual=json.loads(line);assert actual==expected,(p,i,actual,expected)
  report['checked']+=1
  if len(children)>1:report['comparison_checked']+=1
 assert hits==allowed
 report['passed']=True
except Exception as error:report.update(error=repr(error),case=list(p) if 'p' in globals() else None)
finally:
 for child in children:
  child.stdin.close();child.wait(timeout=10)
report['native_instruction_coverage']=[len(hits),len(allowed)]
sources=['src/unit_00151.c','include/raw_tile_raster.h','tests/raw_tile_raster.c','tools/verify_raw_tile_raster_emulation.py']
report['source_hashes']={p:sha((ROOT/p).read_bytes()) for p in sources};report['native_exports']={export:sha((ROOT/export).read_bytes())}
report['elapsed_seconds']=round(time.monotonic()-started,3)
(ROOT/'metadata/raw-tile-raster-emulation.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps(report,indent=2));sys.exit(0 if report['passed'] else 1)
