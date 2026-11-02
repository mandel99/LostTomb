"""Compare city geometry, world/region lifetime, grid initialization and entry controllers."""
from pathlib import Path
import hashlib,itertools,json,struct,subprocess,sys
ROOT=Path(__file__).resolve().parent.parent;sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
def sha(b):return hashlib.sha256(b).hexdigest()
data=(ROOT/'build/smoke-game/Pharaoh.exe').read_bytes();digest=sha(data)
assert digest=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
u16=lambda b,p:struct.unpack_from('<H',b,p)[0];u32=lambda b,p:struct.unpack_from('<I',b,p)[0]
pe=u32(data,0x3c);opt=pe+24;table=opt+u16(data,pe+20);base=u32(data,opt+28);size=u32(data,opt+56)
vm=Uc(UC_ARCH_X86,UC_MODE_32);vm.mem_map(base,(size+4095)&~4095)
for i in range(u16(data,pe+6)):
 p=table+i*40;vm.mem_write(base+u32(data,p+12),data[u32(data,p+20):u32(data,p+20)+u32(data,p+16)])
BASE=0x30000000;STOP=0x33000000;STACK=0x34000000
vm.mem_map(BASE,0x800000);vm.mem_map(STOP,0x1000);vm.mem_map(STACK,0x100000)
regions=[(0xa43eac,0x1857e4),(0xbc9690,0xc90),(0xd05720,0x33000),(0xd3b180,0x220),(0xe38e50,0x90),
 (0xf55b90,16),(0xe945bc,8),(0xe94e00,32),(0xe95118,16),(0xea184c,0xbd00),(0x7848d8,0x800),(0x78d828,8),
 (0x5ca650,0x2040),(BASE+0x10000,0x33000),(BASE+0x50000,0x10000),(BASE+0x600,0x100),
 (0x70573c,0x390),(0x78d5f0,0x44),(0x799ae0,0x101c00)]
def write(a,v,n=4):vm.mem_write(a,(v&((1<<(8*n))-1)).to_bytes(n,'little'))
def read(a):return int.from_bytes(vm.mem_read(a,4),'little')
def byte(a):return vm.mem_read(a,1)[0]
events=[];allocations=[];bump=BASE+0x400000;params=[]
counts={0x561a1d:1,0x561a2b:1,0x40f8f0:3,0x4767d0:6,0x4ce2d0:0,0x568eab:2,0x5647eb:1,0x55f561:1,0x4d41e0:1,0x430530:2,0x540c90:0,0x540ed0:0,0x541160:0,0x46f190:0}
for a in [0x46b320,0x46f030,0x46a320,0x4f4520,0x53d970,0x4bc720,0x4771c0,0x4e7e20,0x4677c0,0x4e59a0,0x485f70,0x486e70,0x489840,0x4dea50,0x4ddf90,0x4de080,
 0x517a20,0x5178b0,0x517370,0x517660,0x5178e0,0x4b32c0,0x468860,0x457af0,0x45e040,0x404350,0x4051a0,0x4e1830,0x41d730,0x41df60,0x417e70,0x476750,0x46d840,0x4bc7a0,0x4bca90,0x442660,0x4426c0,0x4d5ad0,0x4d5bc0,0x443330,0x5596d0,0x45aa30,0x4f61e0,0x4a3560]:counts[a]=0
def terrain():
 vm.mem_write(0xaf598c,bytes(0x32c40))
 if params[10]&1:
  for y in range(params[2]):
   for x in range(params[1]):write(0xaf598c+(read(0xbca2d8)+y*228+x)*4,0x10000 if (x+y+params[3])%3 else 4)
def endpoint(cpu,a,n,user):
 global bump
 sp=cpu.reg_read(UC_X86_REG_ESP);args=[read(sp+4+i*4) for i in range(counts[a])];values=args.copy();result=cleanup=0
 if a==0x55f561:values.insert(0,cpu.reg_read(UC_X86_REG_ECX));cleanup=4
 if a==0x40f8f0 and args[1]==0xcb10 and STACK<=args[0]<STACK+0x100000:values[0]=STACK
 row=[a,len(values)]+values+[0]*(12-2-len(values));row[8:12]=[read(0xe945c0),read(0xeaac28),byte(0xd3b1b8),read(0xf55b94)];events.append(row)
 if a==0x561a1d:
  count=args[0];assert count<=28 and len(allocations)<2048;result=bump;allocations.append([bump,count,1]);vm.mem_write(bump,b'\xcc'*(count+32));bump+=(count+32+63)&~63
 elif a==0x561a2b:
  for item in allocations:
   if item[0]==args[0]:item[2]=0
 elif a==0x40f8f0:vm.mem_write(args[0],bytes([args[2]&255])*args[1])
 elif a==0x568eab:result=0xffffffff if params[10]&4 else 37
 elif a==0x4d41e0:write(0xe945c0,params[6]);terrain()
 elif a==0x46f190:result=params[3]
 cpu.reg_write(UC_X86_REG_EAX,result);cpu.reg_write(UC_X86_REG_EIP,read(sp));cpu.reg_write(UC_X86_REG_ESP,sp+4+cleanup)
for a in counts:vm.hook_add(UC_HOOK_CODE,endpoint,begin=a,end=a)
def execute(entry,args=(),receiver=0):
 sp=STACK+0xf0000;write(sp,STOP)
 for i,v in enumerate(args):write(sp+4+i*4,v)
 saved=[(UC_X86_REG_EBX,0x11111111),(UC_X86_REG_EBP,0x22222222),(UC_X86_REG_ESI,0x33333333),(UC_X86_REG_EDI,0x44444444)]
 for r,v in saved:vm.reg_write(r,v)
 vm.reg_write(UC_X86_REG_ECX,receiver);vm.reg_write(UC_X86_REG_ESP,sp);vm.reg_write(UC_X86_REG_EFLAGS,2)
 vm.reg_write(UC_X86_REG_FPCW,0x37f)
 vm.emu_start(entry,STOP,count=250000000)
 assert vm.reg_read(UC_X86_REG_EIP)==STOP,'Native instruction budget exceeded'
 cleanup=len(args)*4 if entry in [0x4be160,0x4bd740] else 0
 assert vm.reg_read(UC_X86_REG_ESP)==sp+4+cleanup,'Native stack differs'
 assert all(vm.reg_read(r)==v for r,v in saved),'Native saved registers differ'
 return vm.reg_read(UC_X86_REG_EAX)
def fixture_list(region,base,n):
 write(region,n);write(region+16,base if n else 0);write(region+20,base+(n-1)*128 if n else 0)
 for i in range(n):
  a=base+i*128;p=a+32
  for offset,v in [(0,a+128 if i+1<n else 0),(4,a-128 if i else 0),(8,10+i*30),(12,p)]:write(a+offset,v)
  write(p,region);write(p+4,a);write(p+8,100+i)
def native(p):
 global params,bump
 params=p;op,w,h,seed,x,y,version,view,player,rect,flags,nodes=p;events.clear();allocations.clear();bump=BASE+0x400000
 for i,(address,count) in enumerate(regions):vm.mem_write(address,bytes([(seed+i)&255])*count)
 origin=((228-h)//2)*228+(228-w)//2
 values=[(0xbca2d0,w),(0x784904,w),(0xbca2d4,h),(0x784908,h),(0xbca2d8,origin),(0x78490c,origin),(0x784910,seed+257),
  (0xd3b1c4,seed%100),(0xd3b374,seed&127),(0xd3b380,seed+0x12345678),(0xd3b384,seed+0x76543210),(0xf56b54,49000),
  (0xe945c0,version),(0xe94e04,view),(0xeaac28,player),(0xe38ebc,flags&8)]
 for a,v in values:write(a,v)
 mode=rect
 for i in range(5):write(0x7848e2+i*2,mode%5-1,2);mode//=5
 for i in range(2):
  city=0xea0000+i*0x49d8;write(city+0x1894,x+i*1000)
  for j in range(21):write(city+0x3e34+j*2,seed+j*129+i*17,2)
 for i in range(161):write(0x70573c+i*4,seed+i*101)
 for i in range(7):
  a=0x799afc+(3998 if i==6 else i)*264;write(a-28,2 if i%3==2 else 1,1);write(a-23,int(i%3!=1),1)
  write(a,seed+i*129,2)
  for offset,v in [(0x5a,i&1),(0x5c,i&2),(0x59,i&4),(0x52,i%3)]:write(a+offset,v,1)
 write(0x7848dc,seed,2)
 for i in range(16):write(0x784bc0+i*2,-1 if i%5==0 else x+i,2);write(0x784be0+i*2,-1 if i%4==0 else y+i,2)
 vm.mem_write(0xe38e74,b'Restore\0');vm.mem_write(BASE+0x600,b'Save/Restore/restore-native.sav\0')
 for i in range(30):
  g=0xd38384+i*28;a=BASE+0x10000+0x32c64+i*28
  for offset in [0,16,20,24]:write(g+offset,0);write(a+offset,0)
 write(BASE+0x50000,0);execute(0x46f7b0);terrain()
 world=BASE+0x10000 if flags&2 else 0xd05720
 if op==6:fixture_list(world+0x32c64,BASE+0x51000,nodes);fixture_list(world+0x32c64+29*28,BASE+0x52000,nodes)
 if op in [7,8]:fixture_list(BASE+0x50000,BASE+0x51000,nodes)
 write(0xacf85c+1000,seed,1)
 if op==5:vm.mem_write(0xacf85c,bytes((seed+i*29)&255 for i in range(51984)))
 args=();receiver=0;result=0
 entry=[0,0x46f930,0x46fa40,0x46fc30,0x46fc80,0x475b80,0x4be0f0,0x4bd740,0x4bd6a0,0x4be160,0x4d52a0,0x4d5100,0x468170,0x4cde60,0x4c06e0][op]
 if op==1:args=(x,y)
 if op==6:receiver=world
 if op==7:receiver=BASE+0x50000;args=(1000,x,y,31,flags&255)
 if op==8:receiver=BASE+0x50000
 if op==9:receiver=world;args=((w-1,h-1,0,0) if rect else (0,0,w-1,h-1))
 if op==11:args=(BASE+0x600,)
 if op in [13,14]:args=(x,y)
 if entry:returned=execute(entry,args,receiver);result=returned if op in [1,7,11,13,14] else 0
 return {'result':result,'hashes':[sha(bytes(vm.mem_read(a,n))) for a,n in regions],'events':events.copy(),
  'allocations':[[a,n,live,sha(bytes(vm.mem_read(a,n+32)))] for a,n,live in allocations]}
default=[0,12,12,7,0,0,147,6,1,0,0,3];cases=[]
def case(**fields):
 p=default.copy()
 for k,v in fields.items():p[int(k[1:])]=v
 cases.append(p)
for x,y in itertools.product([0,1,100,0xffffffff,0x7fffffff,0x80000000,21474836,21474837],[0,1,3,100,0xffffffff,0x80000000]):case(p0=13,p4=x,p5=y)
for x,y in itertools.product([0,1,4,5,9,10,0xffffffff],[0,1,2,3,0xffffffff]):case(p0=14,p4=x,p5=y)
for modes,player,seed in itertools.product(itertools.product([1,2,3],repeat=5),[0,1],[0,32768,0xffffffff]):
 mode=sum(v*5**i for i,v in enumerate(modes));case(p0=12,p3=seed,p4=123456,p8=player,p9=mode)
for mode,seed,population,player in itertools.product([0,781,3124],[1,0x80000000,0x7fffffff],[0,1,0xffffffff],[0,1]):case(p0=12,p3=seed,p4=population,p8=player,p9=mode)
for w,h in itertools.product([0,1,2,3,12,40,120,160,227,228],repeat=2):case(p1=w,p2=h)
for w,h,(x,y) in itertools.product([1,12,140,228],[1,12,140,228],[(0,0),(100,100),(113,113),(114,114),(227,227),(228,228),(0xffffffff,0xffffffff),(0x80000000,0x7fffffff),(0x7fffffff,0x7fffffff),(0x80000000,0x80000000)]):case(p0=1,p1=w,p2=h,p4=x,p5=y)
for seed in [0,1,0xffffffff]:case(p0=2,p3=seed)
for seed,w in itertools.product([0,7],[1,12,140,228]):case(p0=3,p1=w,p2=w,p3=seed)
for seed in [0,7,0xffffffff]:case(p0=4,p3=seed)
for seed,w,h in itertools.product([0,7,255],[1,12,140,228],[1,12,140,228]):case(p0=5,p1=w,p2=h,p3=seed)
for op,seed,nodes,flags in itertools.product([6,7,8],[0,10,40,70,160,255],[0,1,2,3,6],[0,2]):case(p0=op,p3=seed,p10=flags,p11=nodes)
for w,h,seed,flags,rect in itertools.product([1,3,12],[1,3,12],[0,7],[0,1,3],[0,1]):case(p0=9,p1=w,p2=h,p3=seed,p10=flags,p9=rect)
for version,view,seed,player,flags in itertools.product([140,141,142,147,160],[0,6,7,0xffffffff],[0,32768],[0,1],[0,1,8]):case(p0=10,p6=version,p7=view,p3=seed,p8=player,p10=flags)
for seed,flags in itertools.product([0,32768],[0,4]):case(p0=11,p6=147,p7=7,p3=seed,p8=1,p10=flags)
exe=ROOT/'build/c-only-debug/Debug/restore_city_prepare_verify.exe'
process=subprocess.Popen([str(exe),'--oracle-batch'],cwd=ROOT,stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True)
report={'format':1,'original_game_executed':False,'native_sha256':digest,'compiled_C_sha256':sha(exe.read_bytes()),
 'method':'Complete native city geometry/predicate, grid initialization, world reset, region lifetimes/build, city coverage 00468170 including actual category lookup, signed population bins, household coverage and percentage helper, and ordered 004d52a0/004d5100 controllers. Full memory hashes, allocation guards/lifetimes, ordered endpoint calls/intermediate state, complete predicate EAX and native stack/register ABI compared. Allocator/free, memset, final region redraw and remaining deep city/model/asset/file-controller endpoints controlled.',
 'limitations':['Valid dimensions 0..228 and valid region storage/list inputs only. Failed allocation and invalid region indexing abort safely in C.','This does not prove all deep city/model/asset loaders; existing complete SAV file-controller proof is separate.'],
 'checked':0}
checked=0;p=None
try:
 assert json.loads(process.stdout.readline())=={'fields':12}
 for p in cases:
  process.stdin.write(' '.join(map(str,p))+'\n');process.stdin.flush();line=process.stdout.readline();assert line,'C exited: '+process.stderr.read()
  actual=json.loads(line);expected=native(p)
  if actual!=expected:raise AssertionError(repr({'C_result':actual['result'],'native_result':expected['result'],'different_regions':[hex(regions[i][0]) for i,(a,b) in enumerate(zip(actual['hashes'],expected['hashes'])) if a!=b],
   'first_event_difference':[(i,a,b) for i,(a,b) in enumerate(zip(actual['events'],expected['events'])) if a!=b][:1],'event_counts':[len(actual['events']),len(expected['events'])], 'allocations_equal':actual['allocations']==expected['allocations']}))
  checked+=1
  if checked%100==0 or p[0] in [2,4]:print(json.dumps({'checked':checked,'total':len(cases),'op':p[0]}),flush=True)
 report.update(passed=True,checked=checked)
except Exception as error:report.update(passed=False,checked=checked,case=p,error=str(error),native_eip=hex(vm.reg_read(UC_X86_REG_EIP)))
finally:process.stdin.close();process.wait(timeout=10)
sources=['include/city_prepare.h','include/mission_loaders.h','tests/city_prepare.c','tools/verify_city_prepare_emulation.py','metadata/city-entry-native.json','metadata/city-prepare-native.json','metadata/city-coverage-native.json']
sources += ['runtime/city_history.c','runtime/city_history.h']
sources += [f'src/unit_{i:05d}.c' for i in [934,1009,1011,1013,1014,1015,1026,1036,1049,1050,1235,1542,1550,1551,1552,1553,1557,1558,1582,1731,1826,1828,1832,2023,2046,2047]]
report['source_hashes']={s:sha((ROOT/s).read_bytes()) for s in sources}
(ROOT/'metadata/city-prepare-emulation.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v for k,v in report.items() if k!='source_hashes'},indent=2));sys.exit(0 if report['passed'] else 1)
