"""Compare old mission migrations, exact native history ABI and walker frames."""
from pathlib import Path
from functools import lru_cache
import hashlib,itertools,json,struct,subprocess,sys
ROOT=Path(__file__).resolve().parent.parent;sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
data=(ROOT/'build/smoke-game/Pharaoh.exe').read_bytes();digest=hashlib.sha256(data).hexdigest()
assert digest=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
u16=lambda b,p:struct.unpack_from('<H',b,p)[0];u32=lambda b,p:struct.unpack_from('<I',b,p)[0]
pe=u32(data,0x3c);opt=pe+24;table=opt+u16(data,pe+20);base=u32(data,opt+28);size=u32(data,opt+56)
vm=Uc(UC_ARCH_X86,UC_MODE_32);vm.mem_map(base,(size+4095)&~4095)
for i in range(u16(data,pe+6)):
 p=table+i*40;vm.mem_write(base+u32(data,p+12),data[u32(data,p+20):u32(data,p+20)+u32(data,p+16)])
BASE=0x30000000;STOP=0x33000000;STACK=0x34000000
vm.mem_map(BASE,0x200000);vm.mem_map(STOP,0x1000);vm.mem_map(STACK,0x100000)
regions=[(0x77f748,19632),(0x785d70,18660),(0xbcd5f0,776032),(0x7999e0,1056032),(0xd05640,196),
 (0x9763b8,8512),(0x5d2d40,40),(0x5d2e60,40),(0x5d9c78,256),(0xbcba32,1024),(0x6fdcbc,80),
 (0xea184c,0xbd00),(BASE+0x1000,32),(BASE+0x2000,852),(BASE+0x10000,0xa000)]
def write(a,v,n=4):vm.mem_write(a,(v&((1<<(n*8))-1)).to_bytes(n,'little'))
def read(a):return int.from_bytes(vm.mem_read(a,4),'little')
@lru_cache(maxsize=256)
def period(seed):return bytes((seed+i*29+i//251)&255 for i in range(64256))
def pattern(seed,n):p=period(seed&255);return p*(n//64256)+p[:n%64256]
events=[];allocations=[];bump=BASE+0x80000
def allocator(cpu,a,n,user):
 global bump
 sp=cpu.reg_read(UC_X86_REG_ESP);argument=read(sp+4);events.append([a,argument]);result=0
 if a==0x561a1d:
  assert argument<=4000 and len(allocations)<16;result=bump;allocations.append((bump,argument));vm.mem_write(bump,b'\xcc'*(argument+32));bump+=(argument+32+63)&~63
 cpu.reg_write(UC_X86_REG_EAX,result);cpu.reg_write(UC_X86_REG_EIP,read(sp));cpu.reg_write(UC_X86_REG_ESP,sp+4)
for a in [0x561a1d,0x561a2b]:vm.hook_add(UC_HOOK_CODE,allocator,begin=a,end=a)
def setup_history(q,s,i,p):
 for offset,v in [(0,p[1]-3),(4,p[7]),(8,p[1]-201),(12,p[1]+259)]:write(s+offset,v)
 write(s+16,p[1]+i,1)
 for offset,v in [(4,p[1]-201),(8,p[1]+259),(12,p[1]-3),(16,p[6]),(20,BASE+0x10000+i*4096 if p[9] else 0)]:write(q+offset,v)
 write(q,p[1]+i+3,1)
def native(p):
 global bump
 op,seed,count,kind,frame,building,current,stored,direction,hasbuffer=p;events.clear();allocations.clear();bump=BASE+0x80000
 for i,(address,n) in enumerate(regions):vm.mem_write(address,pattern(seed+i,n))
 write(0x785d70,count);values=[-32768,-1,0,30,31,32,32764,32767]
 for i in range(150):
  row=0x785d74+i*124;write(row+2,i+seed,1)
  for j in range(4):write(row+4+j*2,values[(i+j+seed)%8],2)
 for i in range(256):write(0x5d9c78+i,0 if i%3==0 else i|1,1)
 for i in range(4000):
  write(0x7999e8+i*264,building,2);write(0x799ae0+i*264,1,1);write(0x799af0+i*264,0x29+i%3,2);write(0x799b8a+i*264,seed+i,1)
 for i in range(1,2000):
  row=i*388;write(0xbcd602+row,kind,1);write(0xbcd652+row,1+i%3998,2);write(0xbcd658+row,i%6,2)
 for i in range(6):write(0xbcba32+i*144,(seed+i)%6,1)
 for i in range(41):write(0xd05640+i*4,frame)
 for i in range(10):setup_history(0x9783c0+i*28,0x9763b8+i*820,i,p)
 vm.mem_write(BASE+0x1000,bytes(vm.mem_read(0x9783c0,28)));vm.mem_write(BASE+0x2000,bytes(vm.mem_read(0x9763b8,820)));write(0xeaac28,count&1)
 sp=STACK+0xf0000;vm.mem_write(STACK,bytes(0x100000));write(sp,STOP)
 args=[direction] if op in [3,4] else ([257] if op==6 else ([BASE+0x2000] if op==8 else []))
 if op==7:vm.mem_write(sp+4,bytes(vm.mem_read(BASE+0x2000,820)))
 else:
  for i,value in enumerate(args):write(sp+4+i*4,value)
 saved=[(UC_X86_REG_EBX,0x11111111),(UC_X86_REG_EBP,0x22222222),(UC_X86_REG_ESI,0x33333333),(UC_X86_REG_EDI,0x44444444)]
 for register,value in saved:vm.reg_write(register,value)
 vm.reg_write(UC_X86_REG_ECX,0x785d74 if op==5 else BASE+0x1000);vm.reg_write(UC_X86_REG_ESP,sp);vm.reg_write(UC_X86_REG_EFLAGS,2)
 address=[0x4d19e0,0x4d40c0,0x4d32f0,0x4d0a40,0x4d0a40,0x4493c0,0x4c5120,0x4f67e0,0x4f6810,0x4d3c40,0x4d4040,0x4d4080,0x4d40a0,0x4d1a70,0x4d4180][op]
 vm.emu_start(address,STOP,count=20000000)
 cleanup=820 if op==7 else (4 if op==8 else 0)
 assert vm.reg_read(UC_X86_REG_EIP)==STOP and vm.reg_read(UC_X86_REG_ESP)==sp+4+cleanup,'Native stack/budget differs'
 assert all(vm.reg_read(register)==value for register,value in saved),'Native saved register differs'
 result=vm.reg_read(UC_X86_REG_EAX) if op==6 else (vm.reg_read(UC_X86_REG_EAX)&255 if op==5 else 0)
 return {'result':result,'hashes':[hashlib.sha256(bytes(vm.mem_read(a,n))).hexdigest() for a,n in regions],
  'events':events.copy(),'allocations':[[a,n,hashlib.sha256(bytes(vm.mem_read(a,n+32))).hexdigest()] for a,n in allocations]}
default=[0,7,6,3,256,60,3,7,1,1];cases=[]
for seed in [0,7,128,255]:p=default.copy();p[1]=seed;cases.append(p)
for seed,count in itertools.product([0,1,7,128,255],[0,1,6,150,0xffffffff]):
 p=default.copy();p[0]=1;p[1]=seed;p[2]=count;cases.append(p)
for kind,frame in itertools.product(list(range(110))+[128,255],[15,16,17,31,32,127,128,256,0xffffffff]):
 p=default.copy();p[0]=2;p[3]=kind;p[4]=frame;cases.append(p)
for building,seed in itertools.product([0x1e,0x1f,0x20,0x21,0x22,0x23,0x24,0x33,0x35]+list(range(0x3c,0x46))+[0x97,0x9c,0xe2,0x13c,0xffff],[0,5]):
 p=default.copy();p[0]=2;p[1]=seed;p[3]=0x5b;p[5]=building;cases.append(p)
for op,current,stored,direction,hasbuffer in itertools.product([3,4,7,8],[0,3,200,258],[0,1,7,200,258],[0,1,256,257],[0,1]):
 if op in [7,8] and direction not in [1]:continue
 p=default.copy();p[0]=op;p[6]=current;p[7]=stored;p[8]=direction;p[9]=hasbuffer;cases.append(p)
for seed in [0,1,2,127,255]:p=default.copy();p[0]=5;p[1]=seed;cases.append(p)
for building in list(range(58,72))+[0x13c,0xffff]:p=default.copy();p[0]=6;p[5]=building;cases.append(p)
for op,seed,player in itertools.product(range(9,15),[0,7,64,128,255],[0,1]):
 p=default.copy();p[0]=op;p[1]=seed;p[2]=player;cases.append(p)
exe=ROOT/'build/c-only-debug/Debug/restore_legacy_mission_verify.exe'
process=subprocess.Popen([str(exe),'--oracle-batch'],cwd=ROOT,stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True)
report={'format':1,'original_game_executed':False,'native_sha256':digest,'compiled_C_sha256':hashlib.sha256(exe.read_bytes()).hexdigest(),
 'method':'Complete original city conversion, town-link migration and actual type predicate, walker reconstruction with actual building classification, all ten history objects with native 820-byte by-value import and pointer export, native resize/free/index normalization, and six post-load defaults. Only allocator/free endpoints controlled. Full watched-region SHA256 including guards, complete allocated-buffer hashes, events, returns and native stack/register ABI compared.',
 'safety_differences':['C aborts on failed history allocation or negative capacity normalization, instead of reproducing native invalid writes/infinite loops. Tests use valid capacities and distinct snapshot/object storage.'],
 'limitations':['These are bounded helper contracts and data conversions. Complete file loading and live builtin mission preview need separate integration.']}
checked=0;p=None
try:
 assert json.loads(process.stdout.readline())=={'fields':10}
 for p in cases:
  process.stdin.write(' '.join(map(str,p))+'\n');process.stdin.flush();line=process.stdout.readline();assert line,'C exited: '+process.stderr.read()
  actual=json.loads(line);expected=native(p)
  if actual!=expected:raise AssertionError(repr({'C_result':actual['result'],'native_result':expected['result'],'different_regions':[hex(regions[i][0]) for i,(a,b) in enumerate(zip(actual['hashes'],expected['hashes'])) if a!=b],
   'C_events':actual['events'],'native_events':expected['events'],'allocations_equal':actual['allocations']==expected['allocations']}))
  checked+=1
  if checked%250==0:print(json.dumps({'checked':checked,'total':len(cases)}),flush=True)
 report.update(passed=True,checked=checked)
except Exception as error:report.update(passed=False,checked=checked,case=p,error=str(error),native_eip=hex(vm.reg_read(UC_X86_REG_EIP)))
finally:process.stdin.close();process.wait(timeout=10)
sources=['include/mission_compat.h','include/town_menu.h','tests/legacy_mission.c','tools/verify_legacy_mission_emulation.py','metadata/legacy-mission-native.json']+[f'src/unit_{i:05d}.c' for i in [695,732,1602,1792,1800,1801,1807,1809,1816,1817,1818,1819,1820,2199,2200]]
report['source_hashes']={s:hashlib.sha256((ROOT/s).read_bytes()).hexdigest() for s in sources}
(ROOT/'metadata/legacy-mission-emulation.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report,indent=2));sys.exit(0 if report['passed'] else 1)
