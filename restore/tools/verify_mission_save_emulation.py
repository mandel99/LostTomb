"""Compare complete original v147/v160 SAV loading against reconstructed C."""
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
initial=bytes(vm.mem_read(base,size));payload=b'';position=reads=0;bump=BASE+0x800000;allocations=[];random_state=1;random_calls=0
extra_regions=[(0x580000,0x68000),(0x6fdcbc,80),(0x9763b8,8512),(0xbcba32,7240),(0xd05640,196),(0xea184c,0xbd00),(0xeaaf5c,4)]
forbidden=[0x4d17e0,0x4d1840,0x4d1860,0x4d1880,0x4d18b0,0x4d18f0,0x4d1ad0,0x4d2fa0,0x4d3200,0x4bd5d0,0x4d32d0,0x4d32a0,0x4d3c20,0x4d3cc0,0x4de140,0x4d3d80,0x4d3da0,0x4d3f40,0x4ce6e0,0x4cc110,0x4cc160,0x538cd0]
def endpoint(cpu,a,n,user):
 global position,reads,bump,random_state,random_calls
 sp=cpu.reg_read(UC_X86_REG_ESP);result=cleanup=0
 if a in forbidden:raise AssertionError('Unexpected legacy/error branch '+hex(a))
 if a in apis:
  name=apis[a];cleanup=api_counts[name]*4
  if name=='GlobalAlloc':assert read(sp+4)==0x42 and read(sp+8)==0x8dd8;vm.mem_write(CODEC_HEAP,bytes(0x10000));result=CODEC_HEAP
  elif name=='GlobalLock':assert read(sp+4)==CODEC_HEAP;result=CODEC_HEAP
  else:assert read(sp+4)==CODEC_HEAP
 elif a==0x562348:
  random_calls+=1;random_state=(random_state*214013+2531011)&0xffffffff;result=(random_state>>16)&32767
 elif a==0x5660ed:
  fd,out,count=[read(sp+4+i*4) for i in range(3)];assert fd==37;reads+=1;block=payload[position:position+count]
  if block:vm.mem_write(out,block)
  position+=len(block);result=len(block)
 elif a==0x561a1d:
  count=read(sp+4);assert count<=0x200000 and count+32<0x32800000-bump
  result=bump;allocations.append({'address':bump,'size':count,'live':1});vm.mem_write(bump,b'\xcc'*(count+32));bump+=(count+32+63)&~63
 elif a==0x561a2b:
  address=read(sp+4);entry=next(x for x in allocations if x['address']==address and x['live']);entry['live']=0
 elif a==0x562880:
  out,source,count=[read(sp+4+i*4) for i in range(3)];vm.mem_write(out,bytes(vm.mem_read(source,count)));result=out
 cpu.reg_write(UC_X86_REG_EAX,result);cpu.reg_write(UC_X86_REG_EIP,read(sp));cpu.reg_write(UC_X86_REG_ESP,sp+4+cleanup)
for a in list(apis)+forbidden+[0x5660ed,0x561a1d,0x561a2b,0x562880,0x562348]:vm.hook_add(UC_HOOK_CODE,endpoint,begin=a,end=a)
def native(stream):
 global payload,position,reads,bump,random_state,random_calls
 payload=stream;position=reads=random_calls=0;random_state=1;bump=BASE+0x800000;allocations.clear();vm.mem_write(base,initial);vm.mem_write(BASE,bytes(0x02800000));vm.mem_write(STACK,bytes(0x100000));write(0xe92e1c,BASE)
 sp=STACK+0xf0000;write(sp,STOP);write(sp+4,37)
 saved=[(UC_X86_REG_EBX,0x11111111),(UC_X86_REG_EBP,0x22222222),(UC_X86_REG_ESI,0x33333333),(UC_X86_REG_EDI,0x44444444)]
 for register,value in saved:vm.reg_write(register,value)
 vm.reg_write(UC_X86_REG_ESP,sp);vm.reg_write(UC_X86_REG_EFLAGS,2);vm.reg_write(UC_X86_REG_FPCW,0x37f)
 vm.emu_start(0x4d41e0,STOP,count=300000000)
 assert vm.reg_read(UC_X86_REG_EIP)==STOP and vm.reg_read(UC_X86_REG_ESP)==sp+4,'Native stack/budget differs'
 assert all(vm.reg_read(register)==value for register,value in saved),'Native saved register differs'
 chunks=[]
 for i in range(181):
  destination,length,count=[read(0x5e09a0+i*20+offset) for offset in [4,8,12]];length*=count
  chunks.append({'index':i,'length':length,'sha256':sha(bytes(vm.mem_read(destination,length)))})
 points=[read(0x7770e4+i*324) for i in range(50)]
 return {'version':read(0xe945c0),'declared':read(0xe945c4),'position':position,'reads':reads,
  'nonempty_routes':sum(n>0 for n in points),'route_points':sum(points),'chunks':chunks,
  'allocations':[dict(x,sha256_with_guard=sha(bytes(vm.mem_read(x['address'],x['size']+32)))) for x in allocations],
  'random_calls':random_calls,'extra_hashes':[sha(bytes(vm.mem_read(a,n))) for a,n in extra_regions]}

exe=ROOT/'build/c-only-debug/Debug/restore_mission_save_verify.exe'
fixtures=[]
for name in ['mission-00','mission-01','mission-52','owned-v160-autosave']:
 path=ROOT/f'artifacts/mission-chunks-fixtures/{name}.sav'
 fixtures.append({'path':path.relative_to(ROOT).as_posix(),'sha256':sha(path.read_bytes()),'length':path.stat().st_size})
report={'format':1,'original_game_executed':False,'native_sha256':sha(data),'compiled_C_sha256':sha(exe.read_bytes()),
 'method':'Complete original 004d41e0 SAV controller, actual codec/chunk migrations, route reconstruction, all reached v147/v160 compatibility helpers, all ten dynamic history imports, walker rebuilding and five state reset groups execute together. Only allocator/free/memmove, file reads, Global memory APIs and deterministic CRT random endpoint controlled. All 181 post-load chunk hashes, seven additional regions including state tables/player/history state, complete allocations with guards and lifetime, reads, random call count and native stack/register ABI compared.',
 'limitations':['Four local v147/v160 streams only. Pre-v147 compatibility branches fail explicitly in this harness. State selection uses deterministic substituted CRT randomness. Live builtin preview and subsequent city preparation remain separate integration.'],
 'saves':[]}
checked=0;current=None
def native_state(p):
 global random_state,random_calls
 seed,kind,rows,negative,choices,index,count,flag,state=p
 object,entries,samples=BASE+0x1000,BASE+0x2000,BASE+0x3000
 table=0x5be9b8+(kind*40 if kind<253 else 0)
 for address,length in [(object,64),(table,40),(entries,160),(samples,192)]:vm.mem_write(address,bytes((seed+i*29)&255 for i in range(length)))
 write(object,kind);write(table+16,samples);write(table+20,count);write(table+24,entries);write(table+28,rows)
 for i in range(4):
  write(entries+i*40,0xffffffff if i==negative else i)
  for j in range(4):
   write(entries+i*40+8+j*8,(index+j)&0xffffffff if j<choices else 0xffffffff)
   vm.mem_write(entries+i*40+12+j*8,bytes([(flag+j)&255]))
 sp=STACK+0xf0000;write(sp,STOP);write(sp+4,state);random_state=seed;random_calls=0
 saved=[(UC_X86_REG_EBX,0x11111111),(UC_X86_REG_EBP,0x22222222),(UC_X86_REG_ESI,0x33333333),(UC_X86_REG_EDI,0x44444444)]
 for register,value in saved:vm.reg_write(register,value)
 vm.reg_write(UC_X86_REG_ECX,object);vm.reg_write(UC_X86_REG_ESP,sp);vm.reg_write(UC_X86_REG_EFLAGS,2)
 vm.emu_start(0x41fca0,STOP,count=100000)
 assert vm.reg_read(UC_X86_REG_EIP)==STOP and vm.reg_read(UC_X86_REG_ESP)==sp+8
 assert all(vm.reg_read(register)==value for register,value in saved)
 return {'random_calls':random_calls,'hashes':[sha(bytes(vm.mem_read(a,n))) for a,n in [(object,64),(table,40),(entries,160),(samples,192)]]}
try:
 for entry in fixtures:
  current=entry['path'];path=ROOT/current;result=subprocess.run([str(exe),'--save',str(path)],cwd=ROOT,capture_output=True,text=True,timeout=30)
  assert result.returncode==0,{'save':current,'exit':result.returncode,'stderr':result.stderr}
  actual=json.loads(result.stdout);expected=native(path.read_bytes())
  assert actual==expected,{'save':current,'C':actual,'native':expected}
  assert actual['position']==entry['length'] and actual['version'] in [147,160] and actual['declared'] in [175,181]
  report['saves'].append(dict(entry,**actual));checked+=1
  print(json.dumps({'save':current,'passed':True,'chunks':181,'nonempty_routes':actual['nonempty_routes'],'route_points':actual['route_points']}),flush=True)
 assert any(entry['nonempty_routes']>0 and entry['route_points']>0 for entry in report['saves'])
 save_count=checked;state_count=0
 process=subprocess.Popen([str(exe),'--state-batch'],cwd=ROOT,stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True)
 try:
  assert json.loads(process.stdout.readline())=={'fields':9}
  cases=[];default=[7,1,3,1,4,0,4,1,3]
  for field,values in [(0,[0,1,7,255,65535]),(1,[0xffffffff,0,1,252,253,0x7fffffff]),(2,[0,1,2,3]),(3,[0xffffffff,0,1,2,3]),(4,[0,1,2,3,4]),(5,[0xffffffff,0,1,3,4,0x7fffffff]),(6,[0,1,2,3,4]),(7,[0,1,127,255]),(8,[0,1,3,65535,65536,0xffffffff])]:
   for value in values:
    p=default.copy();p[field]=value;cases.append(p)
  for kind in [0,1,252]:
   for choices in range(5):
    for seed in [0,1,7,255]:p=default.copy();p[0]=seed;p[1]=kind;p[4]=choices;cases.append(p)
  for p in cases:
   current={'state':p};process.stdin.write(' '.join(map(str,p))+'\n');process.stdin.flush()
   line=process.stdout.readline();assert line,'C exited: '+process.stderr.read()
   actual=json.loads(line);expected=native_state(p);assert actual==expected,{'case':p,'C':actual,'native':expected}
   state_count+=1;checked+=1
 finally:process.stdin.close();process.wait(timeout=10)
 report.update(passed=True,checked=checked,saves_compared=save_count,state_cases=state_count,chunks_compared=save_count*181)
except Exception as error:report.update(passed=False,checked=checked,save=current,error=str(error),native_eip=hex(vm.reg_read(UC_X86_REG_EIP)))
sources=['include/mission_loaders.h','include/mission_routes.h','include/mission_compat.h','include/audio_state.h','include/town_menu.h','include/chunk_io.h','include/compression.h','runtime/compression.c','runtime/chunk_migrations.c',
 'tests/mission_save.c','tools/verify_mission_save_emulation.py','metadata/mission-save-native.json']
sources += [entry['path'] for entry in fixtures]
sources += [f'src/unit_{i:05d}.c' for i in [316,338,345,352,359,366,695,1542,1602,1792,1793,1800,1801,1807,1809,1811,1815,1816,1817,1818,1819,1820,1821,1822,2199,2200,2640,2642,2644,2652,2653,2654,2655,2656,2657,2658,2659]]
report['source_hashes']={s:sha((ROOT/s).read_bytes()) for s in sources}
(ROOT/'metadata/mission-save-emulation.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v for k,v in report.items() if k not in ['saves','source_hashes']},indent=2));sys.exit(0 if report['passed'] else 1)
