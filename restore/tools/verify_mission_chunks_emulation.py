"""Compare the complete C chunk/codec pipeline on read-only local mission streams."""
from pathlib import Path
import hashlib,json,struct,subprocess,sys
ROOT=Path(__file__).resolve().parent.parent
sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *

def sha(data):return hashlib.sha256(data).hexdigest()
u16=lambda data,p:struct.unpack_from('<H',data,p)[0]
u32=lambda data,p:struct.unpack_from('<I',data,p)[0]
original=(ROOT/'build/smoke-game/Pharaoh.exe').read_bytes()
assert sha(original)=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
pe=u32(original,0x3c);opt=pe+24;table=opt+u16(original,pe+20)
base=u32(original,opt+28);image_size=u32(original,opt+56)
vm=Uc(UC_ARCH_X86,UC_MODE_32);vm.mem_map(base,(image_size+4095)&~4095)
for i in range(u16(original,pe+6)):
 p=table+i*40
 vm.mem_write(base+u32(original,p+12),original[u32(original,p+20):u32(original,p+20)+u32(original,p+16)])
STAGE=0x30000000;HEAP=STAGE+0x200000;CODEC_HEAP=STAGE+0x400000
STOP=0x33000000;STACK=0x34000000;APIS=0x35000000
vm.mem_map(STAGE,0x800000);vm.mem_map(STOP,0x1000);vm.mem_map(STACK,0x100000);vm.mem_map(APIS,0x1000);vm.mem_map(0,0x1000)
def read(address):return int.from_bytes(vm.mem_read(address,4),'little')
def write(address,value,n=4):vm.mem_write(address,(value&((1<<(8*n))-1)).to_bytes(n,'little'))
def cstring(address):
 text=bytearray()
 while True:
  c=bytes(vm.mem_read(address,1));address+=1
  if c==b'\0':return text.decode('ascii')
  text+=c
api_counts={'GlobalAlloc':2,'GlobalLock':1,'GlobalUnlock':1,'GlobalFree':1};apis={}
directory=base+u32(original,opt+104)
while read(directory+12):
 names=read(directory) or read(directory+16);iat=read(directory+16);i=0
 while read(base+names+i*4):
  rva=read(base+names+i*4)
  if not rva&0x80000000:
   name=cstring(base+rva+2)
   if name in api_counts:
    address=APIS+len(apis)*16;apis[address]=name;write(base+iat+i*4,address)
  i+=1
 directory+=20
assert set(apis.values())==set(api_counts)
initial=bytes(vm.mem_read(base,image_size))
position=reads=errors=0;allocated=False;payload=b''
def endpoint(cpu,address,n,user):
 global position,reads,errors,allocated
 sp=cpu.reg_read(UC_X86_REG_ESP);result=0;cleanup=0
 if address in apis:
  name=apis[address];cleanup=api_counts[name]*4
  if name=='GlobalAlloc':
   assert read(sp+4)==0x42 and read(sp+8)==0x8dd8
   vm.mem_write(CODEC_HEAP,bytes(0x10000));result=CODEC_HEAP
  elif name=='GlobalLock':assert read(sp+4)==CODEC_HEAP;result=CODEC_HEAP
  else:assert read(sp+4)==CODEC_HEAP
 elif address==0x5660ed:
  file,out,count=[read(sp+4+i*4) for i in range(3)];assert file==37;reads+=1
  block=payload[position:position+count]
  if block:vm.mem_write(out,block)
  position+=len(block);result=len(block)
 elif address==0x561a1d:
  assert not allocated and read(sp+4)<=0x200000
  allocated=True;vm.mem_write(HEAP,b'\xcc'*0x200000);result=HEAP
 elif address==0x561a2b:assert allocated and read(sp+4)==HEAP;allocated=False
 elif address==0x562880:
  out,source,count=[read(sp+4+i*4) for i in range(3)]
  vm.mem_write(out,bytes(vm.mem_read(source,count)));result=out
 elif address in [0x4ce6e0,0x4cc110,0x4cc160,0x538cd0]:errors+=1
 cpu.reg_write(UC_X86_REG_EAX,result);cpu.reg_write(UC_X86_REG_EIP,read(sp));cpu.reg_write(UC_X86_REG_ESP,sp+4+cleanup)
for address in list(apis)+[0x5660ed,0x561a1d,0x561a2b,0x562880,0x4ce6e0,0x4cc110,0x4cc160,0x538cd0]:
 vm.hook_add(UC_HOOK_CODE,endpoint,begin=address,end=address)

def native(index):
 vm.mem_write(STACK,bytes(0x100000));write(0,0)
 sp=STACK+0xf0000;write(sp,STOP)
 for i,value in enumerate([index,0x5e09a0,37]):write(sp+4+i*4,value)
 saved=[(UC_X86_REG_EBX,0x11111111),(UC_X86_REG_EBP,0x22222222),(UC_X86_REG_ESI,0x33333333),(UC_X86_REG_EDI,0x44444444)]
 for register,value in saved:vm.reg_write(register,value)
 vm.reg_write(UC_X86_REG_ESP,sp);vm.reg_write(UC_X86_REG_EFLAGS,2)
 vm.emu_start(0x4d0e10,STOP,count=160000000)
 assert vm.reg_read(UC_X86_REG_EIP)==STOP and vm.reg_read(UC_X86_REG_ESP)==sp+4,'Native stack/budget differs'
 assert all(vm.reg_read(register)==value for register,value in saved),'Native saved register differs'
 assert not allocated,'Transient native allocation remains live'
 destination,size,count=[read(0x5e09a0+index*20+offset) for offset in [4,8,12]]
 return {'index':index,'result':vm.reg_read(UC_X86_REG_EAX)&255,'position':position,
  'version':read(0xe945c0),'declared':read(0xe945c4),'length':size*count,'reads':reads,'errors':errors,
  'sha256':sha(bytes(vm.mem_read(destination,size*count)))}

# Extract only into this task's artifact directory; source files remain read-only.
fixture_dir=ROOT/'artifacts/mission-chunks-fixtures';fixture_dir.mkdir(exist_ok=True)
pak_path=ROOT/'build/debug-startup-game/mission1.pak';pak=pak_path.read_bytes()
offsets=[u32(pak,p) for p in range(0,u32(pak,0),4) if u32(pak,p)]
fixtures=[]
for index in [0,1,len(offsets)-1]:
 end=offsets[index+1] if index+1<len(offsets) else len(pak)
 stream=pak[offsets[index]:end];path=fixture_dir/f'mission-{index:02d}.sav';path.write_bytes(stream)
 fixtures.append({'path':path.relative_to(ROOT).as_posix(),'origin':f'mission1.pak entry {index}',
  'sha256':sha(stream),'length':len(stream),'version':u32(stream,4),'declared':u32(stream,8)})
save_path=ROOT/'build/smoke-game/Save/Khensthoth/autosave.sav';stream=save_path.read_bytes()
path=fixture_dir/'owned-v160-autosave.sav';path.write_bytes(stream)
fixtures.append({'path':path.relative_to(ROOT).as_posix(),'origin':'Existing isolated smoke-game autosave',
 'sha256':sha(stream),'length':len(stream),'version':u32(stream,4),'declared':u32(stream,8)})
corpus={'sources':{p.relative_to(ROOT).as_posix():sha(p.read_bytes()) for p in [pak_path,save_path]},'streams':fixtures}
(ROOT/'metadata/mission-chunks-corpus.json').write_text(json.dumps(corpus,indent=2)+'\n')
exe=ROOT/'build/c-only-debug/Debug/restore_mission_chunks_verify.exe'
report={'format':1,'original_game_executed':False,'native_sha256':sha(original),'compiled_C_sha256':sha(exe.read_bytes()),
 'method':'Complete C 004d0e10 schema loader, compressed block reader and DCL decoder run together on complete v147/v160 streams. Original loader and codec instructions run together in bounded Unicorn calls. Only file reads, allocation, memmove, Windows global-memory APIs and error endpoints controlled. Every chunk SHA256, read count, position, return byte, version/count and saved-register/stack ABI compared.',
 'limitations':['This is a headless data-pipeline check. Top-level map/save controllers, route rebuilding, legacy post-load defaults, mission preview and gameplay are separate checks.','The C executable uses only its own compiled C and non-executable data. The original executable is a static verification oracle only.'],
 'streams':[]}
checked=0;current=None
try:
 for fixture in fixtures:
  current=fixture['origin'];payload=(ROOT/fixture['path']).read_bytes();position=reads=errors=0;allocated=False
  vm.mem_write(base,initial);vm.mem_write(STAGE,bytes(0x800000))
  write(0xe92e1c,STAGE);write(0xe945c0,160);write(0xe945c4,0)
  result=subprocess.run([str(exe),'--stream',str(ROOT/fixture['path'])],cwd=ROOT,capture_output=True,text=True,timeout=90)
  assert result.returncode==0,{'stream':current,'exit':result.returncode,'stderr':result.stderr,'last_lines':result.stdout.splitlines()[-2:]}
  actual=[json.loads(line) for line in result.stdout.splitlines()]
  assert len(actual)==(175 if fixture['version']<154 else 181)
  chunk_proofs=[]
  for index,row in enumerate(actual):
   expected=native(index)
   assert row==expected,{'stream':current,'chunk':index,'C':row,'native':expected}
   assert row['result']==1 and row['errors']==0
   checked+=1;chunk_proofs.append(row)
  assert position==len(payload)
  report['streams'].append(dict(fixture,chunks_compared=len(actual),read_calls=reads,chunks=chunk_proofs))
  print(json.dumps({'stream':current,'chunks':len(actual),'bytes_consumed':position,'passed':True}),flush=True)
 report.update(passed=True,checked=checked)
except Exception as error:
 report.update(passed=False,checked=checked,stream=current,error=str(error),native_eip=hex(vm.reg_read(UC_X86_REG_EIP)))
sources=['include/chunk_io.h','include/compression.h','runtime/compression.c','runtime/chunk_migrations.c',
 'tests/mission_chunks.c','tools/verify_mission_chunks_emulation.py','metadata/mission-chunks-corpus.json','src/unit_01793.c']
sources += [f'src/unit_{i:05d}.c' for i in [2640,2642,2644,2652,2653,2654,2655,2656,2657,2658,2659]]
report['source_hashes']={s:sha((ROOT/s).read_bytes()) for s in sources}
(ROOT/'metadata/mission-chunks-emulation.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({k:v for k,v in report.items() if k not in ['streams','source_hashes']},indent=2))
sys.exit(0 if report['passed'] else 1)
