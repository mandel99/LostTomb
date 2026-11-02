"""Compare route interpolation and city endpoints with original x87 instructions."""
from pathlib import Path
import hashlib,itertools,json,struct,subprocess,sys
ROOT=Path(__file__).resolve().parent.parent;sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
original=(ROOT/'build/smoke-game/Pharaoh.exe').read_bytes();digest=hashlib.sha256(original).hexdigest()
assert digest=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
u16=lambda b,p:struct.unpack_from('<H',b,p)[0];u32=lambda b,p:struct.unpack_from('<I',b,p)[0]
pe=u32(original,0x3c);opt=pe+24;table=opt+u16(original,pe+20);base=u32(original,opt+28);size=u32(original,opt+56)
vm=Uc(UC_ARCH_X86,UC_MODE_32);vm.mem_map(base,(size+4095)&~4095)
for i in range(u16(original,pe+6)):
 p=table+i*40;vm.mem_write(base+u32(original,p+12),original[u32(original,p+20):u32(original,p+20)+u32(original,p+16)])
BASE=0x30000000;ROUTE=BASE+0x1000;OLD=BASE+0x10000;HEAP=BASE+0x100000;STOP=0x33000000;STACK=0x34000000
vm.mem_map(BASE,0x300000);vm.mem_map(STOP,0x1000);vm.mem_map(STACK,0x100000)
def write(a,v,n=4):vm.mem_write(a,(v&((1<<(n*8))-1)).to_bytes(n,'little'))
def read(a):return int.from_bytes(vm.mem_read(a,4),'little')
def signed(v):return (v+32768)%65536-32768
events=[];requested=fail=0
def endpoint(cpu,address,n,user):
 global requested
 sp=cpu.reg_read(UC_X86_REG_ESP);argument=read(sp+4);events.append([address,argument])
 if address==0x561a1d:
  assert argument<=0x100000;requested=argument;result=0 if fail else HEAP
 else:assert argument==OLD;result=0
 cpu.reg_write(UC_X86_REG_EAX,result);cpu.reg_write(UC_X86_REG_EIP,read(sp));cpu.reg_write(UC_X86_REG_ESP,sp+4)
for a in [0x561a1d,0x561a2b]:vm.hook_add(UC_HOOK_CODE,endpoint,begin=a,end=a)
def point(p,i):
 count,step,shape,seed,capacity,hasbuffer,fail,graphics,sprite,match,xoff,yoff=p
 a,b=[(i*7+seed%5,seed%11),(seed%7,i*9),(i*3,i*4),(i*3,7 if i&1 else 0),(3,-7),
  (32767 if i else -32768,1 if i else -1),(-170+(seed*17+i*19)%340,-170+(seed*7+i*31)%340),(i*500,i*500)][shape]
 return signed(a+signed(xoff)),signed(b+signed(yoff))
def native(p):
 global requested,fail
 count,step,shape,seed,capacity,hasbuffer,fail,graphics,sprite,match,xoff,yoff=p
 events.clear();requested=0
 for a,n,v in [(ROUTE,356,0xa5),(OLD,0xf0000,0xa7),(HEAP,0x100000,0xcc),(0x77f748,19600,0),(0x10d6a00,258*64,0),(STACK,0x100000,0)]:vm.mem_write(a,bytes([v])*n)
 for offset,value,n in [(0,step,1),(321,count,1),(4,OLD if hasbuffer else 0,4),(308,-123,4),(312,capacity,4),(316,0x2222,2),(318,0x3333,2)]:write(ROUTE+offset,value,n)
 for i in range(count):
  x,y=point(p,i);write(ROUTE+8+i*6,x,2);write(ROUTE+10+i*6,y,2)
 write(0x126517c,graphics)
 for i in range(258):
  width,height={0:(7,8),1:(300,300),256:(4,5)}.get(i,(999,999));write(0x10d6a0c+i*64,width,2);write(0x10d6a0e+i*64,height,2)
 if match:
  first_x,first_y=point(p,0);last_x,last_y=point(p,count-1 if count else 0)
  for i,index in enumerate([1,7,199]):
   city=0x77f748+index*98;write(city,2 if match==3 and i==0 else 1,1);write(city+1,0 if match==3 and i==2 else 1,1);write(city+12,sprite,2)
   x,y=(first_x-2,first_y-2) if match==2 else ((last_x-1,last_y-1) if i==2 else (first_x-1,first_y-1))
   write(city+4,x,2);write(city+6,y,2)
 sp=STACK+0xf0000;write(sp,STOP)
 saved=[(UC_X86_REG_EBX,0x11111111),(UC_X86_REG_EBP,0x22222222),(UC_X86_REG_ESI,0x33333333),(UC_X86_REG_EDI,0x44444444)]
 for register,value in saved:vm.reg_write(register,value)
 vm.reg_write(UC_X86_REG_ECX,ROUTE);vm.reg_write(UC_X86_REG_ESP,sp);vm.reg_write(UC_X86_REG_EFLAGS,2);vm.reg_write(UC_X86_REG_FPCW,0x37f)
 vm.emu_start(0x4d4720,STOP,count=10000000)
 assert vm.reg_read(UC_X86_REG_EIP)==STOP and vm.reg_read(UC_X86_REG_ESP)==sp+4,'Native stack/budget differs'
 assert all(vm.reg_read(register)==value for register,value in saved),'Native saved register differs'
 state=bytes(vm.mem_read(ROUTE,356))
 if hasbuffer:state+=bytes(vm.mem_read(OLD,capacity*6+32))
 if requested:state+=bytes(vm.mem_read(HEAP,requested+32))
 return {'result':vm.reg_read(UC_X86_REG_EAX)&255,'events':events.copy(),'state':state.hex()}

cases=[]
for count,step,shape,(capacity,hasbuffer) in itertools.product([0,1,2,6,50],[0,1,2,5,255],range(5),[(0,0),(0,1),(8,1),(1000,1)]):
 cases.append([count,step,shape,7,capacity,hasbuffer,0,258,257,1,(-23)&0xffffffff,9])
for count,graphics,sprite,match in itertools.product([0,1,2,6],[0,1,2,257,258],[0,1,256,257,65535],range(4)):
 cases.append([count,5,6,2,0,0,0,graphics,sprite,match,0,0])
for count,seed in itertools.product([2,6,50],range(12)):
 cases.append([count,3,6,seed,0,0,0,258,256,seed%4,0,0])
for capacity,hasbuffer in [(0,0),(100000,1)]:cases.append([2,255,5,0,capacity,hasbuffer,0,258,257,1,0,0])
exe=ROOT/'build/c-only-debug/Debug/restore_mission_routes_verify.exe'
selftest=subprocess.run([str(exe)],cwd=ROOT,capture_output=True,text=True,timeout=10);assert selftest.returncode==0,selftest.stderr
process=subprocess.Popen([str(exe),'--oracle-batch'],cwd=ROOT,stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True)
report={'format':1,'original_game_executed':False,'native_sha256':digest,'compiled_C_sha256':hashlib.sha256(exe.read_bytes()).hexdigest(),
 'method':'Actual original 004d4720 interpolation, FSQRT and 0056133c float-to-integer conversion instructions. Only malloc/free controlled. Full route plus guards, complete old/new buffers plus guards, allocator events, AL result and saved-register/stack ABI compared.',
 'safety_differences':['C rejects more than 50 route control points, missing sample buffers and failed allocations. It checks each sample against capacity; the native estimate can underallocate long diagonals. Native comparisons use geometries within allocated storage; C-only selftests verify those failure paths without native overflow.'],
 'C_safety_selftest':{'passed':True,'stdout':selftest.stdout.strip()},
 'limitations':['Bounded route/city fixtures establish this function contract. Actual top-level map/save loading and post-load initialization are separate checks.']}
checked=0;p=None
try:
 assert json.loads(process.stdout.readline())=={'fields':12}
 for p in cases:
  process.stdin.write(' '.join(map(str,p))+'\n');process.stdin.flush();line=process.stdout.readline();assert line,'C exited: '+process.stderr.read()
  actual=json.loads(line);expected=native(p)
  if actual!=expected:
   first=[i//2 for i in range(0,min(len(actual['state']),len(expected['state'])),2) if actual['state'][i:i+2]!=expected['state'][i:i+2]][:8]
   raise AssertionError(repr({'C_result':actual['result'],'native_result':expected['result'],'C_events':actual['events'],'native_events':expected['events'],'first_state_differences':first,'C_state_length':len(actual['state'])//2,'native_state_length':len(expected['state'])//2}))
  checked+=1
 report.update(passed=True,checked=checked)
except Exception as error:report.update(passed=False,checked=checked,case=p,error=str(error),native_eip=hex(vm.reg_read(UC_X86_REG_EIP)))
finally:process.stdin.close();process.wait(timeout=10)
sources=['include/mission_routes.h','src/unit_01822.c','src/unit_01821.c','src/unit_01823.c','tests/mission_routes.c','tools/verify_mission_routes_emulation.py','metadata/mission-route-builder-native.json']
report['source_hashes']={s:hashlib.sha256((ROOT/s).read_bytes()).hexdigest() for s in sources}
(ROOT/'metadata/mission-routes-emulation.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report,indent=2));sys.exit(0 if report['passed'] else 1)
