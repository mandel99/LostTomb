"""Run complete original v160 MAP loading, including actual route reconstruction."""
from pathlib import Path
import hashlib,json,struct,subprocess,sys
ROOT=Path(__file__).resolve().parent.parent;sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
def sha(data):return hashlib.sha256(data).hexdigest()
u16=lambda b,p:struct.unpack_from('<H',b,p)[0];u32=lambda b,p:struct.unpack_from('<I',b,p)[0]
data=(ROOT/'build/smoke-game/Pharaoh.exe').read_bytes();assert sha(data)=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
pe=u32(data,0x3c);opt=pe+24;table=opt+u16(data,pe+20);base=u32(data,opt+28);size=u32(data,opt+56)
vm=Uc(UC_ARCH_X86,UC_MODE_32);vm.mem_map(base,(size+4095)&~4095)
for i in range(u16(data,pe+6)):
 p=table+i*40;vm.mem_write(base+u32(data,p+12),data[u32(data,p+20):u32(data,p+20)+u32(data,p+16)])
BASE=0x30000000;CODEC_HEAP=BASE+0x400000;STOP=0x33000000;STACK=0x34000000;APIS=0x35000000
vm.mem_map(BASE,0x02800000);vm.mem_map(STOP,0x1000);vm.mem_map(STACK,0x100000);vm.mem_map(APIS,0x1000)
def write(a,v):vm.mem_write(a,(v&0xffffffff).to_bytes(4,'little'))
def read(a):return int.from_bytes(vm.mem_read(a,4),'little')
def cstring(a):
 out=bytearray()
 while True:
  c=bytes(vm.mem_read(a,1));a+=1
  if c==b'\0':return out.decode('ascii')
  out+=c
api_counts={'GlobalAlloc':2,'GlobalLock':1,'GlobalUnlock':1,'GlobalFree':1};apis={}
directory=base+u32(data,opt+104)
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
initial=bytes(vm.mem_read(base,size));payload=b'';position=reads=0;bump=BASE+0x800000;allocations=[]
forbidden=[0x4d17e0,0x4d19e0,0x4d1a70,0x4d32d0,0x4d3d80,0x4d4080,0x4d40a0,0x4d40c0,0x4ce6e0,0x4cc110,0x4cc160,0x538cd0]
def endpoint(cpu,a,n,user):
 global position,reads,bump
 sp=cpu.reg_read(UC_X86_REG_ESP);result=cleanup=0
 if a in forbidden:raise AssertionError('Unexpected legacy/error branch '+hex(a))
 if a in apis:
  name=apis[a];cleanup=api_counts[name]*4
  if name=='GlobalAlloc':assert read(sp+4)==0x42 and read(sp+8)==0x8dd8;vm.mem_write(CODEC_HEAP,bytes(0x10000));result=CODEC_HEAP
  elif name=='GlobalLock':assert read(sp+4)==CODEC_HEAP;result=CODEC_HEAP
  else:assert read(sp+4)==CODEC_HEAP
 elif a==0x5660ed:
  fd,out,count=[read(sp+4+i*4) for i in range(3)];assert fd==37;reads+=1;block=payload[position:position+count]
  if block:vm.mem_write(out,block)
  position+=len(block);result=len(block)
 elif a==0x561a1d:
  count=read(sp+4);assert count<=0x100000 and count+32<0x32800000-bump
  result=bump;allocations.append({'address':bump,'size':count,'live':1});vm.mem_write(bump,b'\xcc'*(count+32));bump+=(count+32+63)&~63
 elif a==0x561a2b:
  address=read(sp+4);entry=next(x for x in allocations if x['address']==address and x['live']);entry['live']=0
 elif a==0x562880:
  out,source,count=[read(sp+4+i*4) for i in range(3)];vm.mem_write(out,bytes(vm.mem_read(source,count)));result=out
 cpu.reg_write(UC_X86_REG_EAX,result);cpu.reg_write(UC_X86_REG_EIP,read(sp));cpu.reg_write(UC_X86_REG_ESP,sp+4+cleanup)
for a in list(apis)+forbidden+[0x5660ed,0x561a1d,0x561a2b,0x562880]:vm.hook_add(UC_HOOK_CODE,endpoint,begin=a,end=a)
def native(stream):
 global payload,position,reads,bump
 payload=stream;position=reads=0;bump=BASE+0x800000;allocations.clear();vm.mem_write(base,initial);vm.mem_write(BASE,bytes(0x02800000));vm.mem_write(STACK,bytes(0x100000));write(0xe92e1c,BASE)
 sp=STACK+0xf0000;write(sp,STOP);write(sp+4,37)
 saved=[(UC_X86_REG_EBX,0x11111111),(UC_X86_REG_EBP,0x22222222),(UC_X86_REG_ESI,0x33333333),(UC_X86_REG_EDI,0x44444444)]
 for register,value in saved:vm.reg_write(register,value)
 vm.reg_write(UC_X86_REG_ESP,sp);vm.reg_write(UC_X86_REG_EFLAGS,2);vm.reg_write(UC_X86_REG_FPCW,0x37f)
 vm.emu_start(0x4d4ae0,STOP,count=160000000)
 assert vm.reg_read(UC_X86_REG_EIP)==STOP and vm.reg_read(UC_X86_REG_ESP)==sp+4,'Native stack/budget differs'
 assert all(vm.reg_read(register)==value for register,value in saved),'Native saved register differs'
 chunks=[]
 for i in range(24):
  destination,length,count=[read(0x5e17d8+i*20+offset) for offset in [4,8,12]];length*=count
  chunks.append({'index':i,'length':length,'sha256':sha(bytes(vm.mem_read(destination,length)))})
 points=[read(0x7770e4+i*324) for i in range(50)]
 return {'version':read(0xe945c0),'declared':read(0xe945c4),'position':position,'reads':reads,
  'nonempty_routes':sum(n>0 for n in points),'route_points':sum(points),'chunks':chunks,
  'allocations':[dict(x,sha256_with_guard=sha(bytes(vm.mem_read(x['address'],x['size']+32)))) for x in allocations]}

exporter=ROOT/'build/c-only-debug/Debug/restore_mission_chunks_verify.exe';exe=ROOT/'build/c-only-debug/Debug/restore_mission_map_verify.exe'
directory=ROOT/'artifacts/mission-map-fixtures';directory.mkdir(exist_ok=True)
fixtures=[]
for name in ['mission-52','owned-v160-autosave']:
 source=ROOT/f'artifacts/mission-chunks-fixtures/{name}.sav';output=directory/f'{name}.map'
 result=subprocess.run([str(exporter),'--export-map',str(source),str(output)],cwd=ROOT,capture_output=True,text=True,timeout=30)
 assert result.returncode==0,result.stderr
 fixtures.append({'path':output.relative_to(ROOT).as_posix(),'sha256':sha(output.read_bytes()),'length':output.stat().st_size,
  'derived_from':source.relative_to(ROOT).as_posix(),'source_sha256':sha(source.read_bytes())})
corpus={'provenance':'Derived modern MAP fixtures; complete verified C SAV decoding followed by native 24-entry MAP serialization with uncompressed-fallback envelopes. These are not claimed to be original authored MAP files.',
 'exporter_compiled_C_sha256':sha(exporter.read_bytes()),'fixtures':fixtures}
(ROOT/'metadata/mission-map-corpus.json').write_text(json.dumps(corpus,indent=2)+'\n')
report={'format':1,'original_game_executed':False,'native_sha256':sha(data),'compiled_C_sha256':sha(exe.read_bytes()),
 'method':'Complete original 004d4ae0 MAP controller, 004d0e10 chunk loader, 0055bcf0 envelope reader, 004d4720 route reconstruction and actual 0056133c conversions execute together. Only allocator/free/memmove, file reads and Global memory APIs controlled. Legacy migrations and error endpoints are forbidden rather than simulated. All 24 post-load chunk SHA256, full allocation hashes including guards/lifetime, read count/position, reconstructed route counts and stack/register ABI compared.',
 'limitations':['Fixtures are v160 MAPs derived from local v160 mission/save streams. Compatibility helpers for older files, wrapper environment restoration, live rendering and city preparation remain separate checks.'],
 'maps':[]}
checked=0;current=None
try:
 for entry in fixtures:
  current=entry['path'];path=ROOT/current;result=subprocess.run([str(exe),'--map',str(path)],cwd=ROOT,capture_output=True,text=True,timeout=30)
  assert result.returncode==0,{'map':current,'exit':result.returncode,'stderr':result.stderr}
  actual=json.loads(result.stdout);expected=native(path.read_bytes())
  assert actual==expected,{'map':current,'C':actual,'native':expected}
  assert actual['position']==entry['length'] and actual['version']==160 and actual['declared']==24
  report['maps'].append(dict(entry,**actual));checked+=1
  print(json.dumps({'map':current,'passed':True,'chunks':24,'nonempty_routes':actual['nonempty_routes'],'route_points':actual['route_points']}),flush=True)
 assert any(entry['nonempty_routes']>0 and entry['route_points']>0 for entry in report['maps'])
 report.update(passed=True,checked=checked,chunks_compared=checked*24)
except Exception as error:report.update(passed=False,checked=checked,map=current,error=str(error),native_eip=hex(vm.reg_read(UC_X86_REG_EIP)))
sources=['include/mission_loaders.h','include/mission_routes.h','include/chunk_io.h','include/compression.h','runtime/compression.c','runtime/chunk_migrations.c',
 'tests/mission_chunks.c','tests/mission_map.c','tools/verify_mission_map_emulation.py','metadata/mission-map-corpus.json',exporter.relative_to(ROOT).as_posix()]
sources += [entry['derived_from'] for entry in fixtures]
sources += [f'src/unit_{i:05d}.c' for i in [1542,1793,1822,1823,2640,2642,2644,2652,2653,2654,2655,2656,2657,2658,2659]]
report['source_hashes']={s:sha((ROOT/s).read_bytes()) for s in sources}
(ROOT/'metadata/mission-map-emulation.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v for k,v in report.items() if k not in ['maps','source_hashes']},indent=2));sys.exit(0 if report['passed'] else 1)
