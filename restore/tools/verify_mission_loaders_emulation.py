"""Compare MAP/SAV controllers, mission wrappers and settings-object receivers."""
from pathlib import Path
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
BASE=0x30000000;ALT=BASE+0x10000;STOP=0x33000000;STACK=0x34000000;FLOAT_RETURN=0x35000000
vm.mem_map(BASE,0x100000);vm.mem_map(STOP,0x1000);vm.mem_map(STACK,0x100000);vm.mem_map(FLOAT_RETURN,0x1000);vm.mem_map(0,0x1000)
vm.mem_write(FLOAT_RETURN,b'\xdd\x05'+struct.pack('<I',BASE+0x800)+b'\xc3');vm.mem_write(BASE+0x800,struct.pack('<d',345.75))
regions=[(0x776fb0,16232),(0x7848d8,1624),(0x78d790,40),(0xe945bc,12),(0x978d9c,207936),
 (0xd38360,896),(0xea184c,0xbd00),(0xe39080,4),(0xe20760,16),(0x7027b0,8),
 (0x5e45b0,240),(0x5e46a0,432),(0x5c1140,528),(0x5c1350,312),(ALT,64),(ALT+0x32c40,896)]
def write(a,v,n=4):vm.mem_write(a,(v&((1<<(n*8))-1)).to_bytes(n,'little'))
def read(a,n=4):return int.from_bytes(vm.mem_read(a,n),'little')
def signed(v):return (v+0x80000000)%0x100000000-0x80000000
events=[];config=[]
migrations=[0x4206a0,0x4209e0,0x420d10,0x421010,0x4212f0,0x4bd5d0,0x4d17e0,0x4d1840,0x4d1860,0x4d1880,
 0x4d18b0,0x4d18f0,0x4d19e0,0x4d1a70,0x4d1ad0,0x4d2fa0,0x4d3200,0x4d32a0,0x4d32d0,0x4d32f0,
 0x4d3c20,0x4d3c40,0x4d3cc0,0x4d3d00,0x4d3d80,0x4d3da0,0x4d3f40,0x4d3f60,0x4d4040,0x4d4080,0x4d40a0,0x4d40c0,0x4d4180,0x4de140,
 0x4bc720,0x4d52a0,0x4f4520,0x53d970]
counts={a:0 for a in migrations}
counts.update({0x4d0a40:1,0x4ce2d0:0,0x561a2b:1,0x4d0e10:3,0x4d4720:0,0x5660ed:3,0x55bcf0:3,
 0x562370:2,0x4cc110:2,0x4cc160:2,0x538cd0:3,0x568eab:2,0x5647eb:1,0x5662e3:3,0x4ce480:4,0x4f78a0:1})
def note(a,x=0,y=0,z=0):events.append([a,x&0xffffffff,y&0xffffffff,z&0xffffffff,read(0xe945c0),read(0xd38360)])
def mutate(a):
 for table in [0x5e45b0,0x5e46a0,0x5c1140,0x5c1350]:write(table,0xfeed0000+a);write(table+4,0xbeef0000+a)
def routes():
 vm.mem_write(0x776fb0,bytes(16200))
 for i in range(8):
  r=0x776fb0+i*324;write(r,5,1);write(r+321,2,1);write(r+320,1,1)
  if i==0:write(r+321,0x55,1);write(r+308,0x12345678)
  if i==2:write(r+4,BASE+0x9000)
  if i==3:write(r+312,1)
  if i==4:write(r+321,51,1)
  if i==5:write(r+320,3,1)
  if i==6:write(r,51 if config[5]&2 else 0,1)
  if i==7:write(r+321,0,1)
def endpoint(cpu,a,n,user):
 sp=cpu.reg_read(UC_X86_REG_ESP);args=[read(sp+4+i*4) for i in range(counts[a])];result=0
 if a==0x4d4720:
  route=cpu.reg_read(UC_X86_REG_ECX);note(a,route);write(route+308,333);write(route+316,1,2);write(route+318,199,2);result=1
 elif a==0x4ce480:
  path,out,size,offset=args;assert size==4;note(a,path,size,offset);write(out,config[9]);result=0 if config[8]&2 else 1
 else:
  note(a,*(args+[0]*(3-len(args))))
  if a in migrations:mutate(a)
  elif a==0x4d0a40:assert args==[1];mutate(a)
  elif a==0x4d0e10:
   index,schema,fd=args;assert fd==37 and schema in [0x5e17d8,0x5e09a0];map=schema==0x5e17d8
   if index!=config[2]:
    result=1
    if index==(1 if map else 4):write(0xe945c0,config[1])
    if index==(2 if map else 5):write(0xe945c4,24 if map else (175 if config[1]<154 else 181))
    if index==(23 if map else (174 if config[1]<154 else 180)):routes()
  elif a in [0x5660ed,0x55bcf0]:
   fd,out,size=args;assert fd==37 and size==4;vm.mem_write(out,b'MAPS' if config[3] else b'oops');result=4 if a==0x5660ed else 1
  elif a==0x562370:result=0 if bytes(vm.mem_read(args[0],4))==bytes(vm.mem_read(args[1],4)) else 1
  elif a==0x4cc110:write(0xe20768,BASE+0x600)
  elif a==0x4cc160:write(0xe20764,BASE+0x700)
  elif a==0x568eab:result=0xffffffff if config[8]&1 else 37
  elif a==0x5662e3:result=args[1]
  elif a==0x4f78a0:
   assert args==[1];cpu.reg_write(UC_X86_REG_EIP,FLOAT_RETURN);return
 cpu.reg_write(UC_X86_REG_EAX,result);cpu.reg_write(UC_X86_REG_EIP,read(sp));cpu.reg_write(UC_X86_REG_ESP,sp+4)
for a in counts:vm.hook_add(UC_HOOK_CODE,endpoint,begin=a,end=a)
def native(p):
 global config
 config=p;op=p[0];events.clear()
 for a,n in regions:vm.mem_write(a,b'\xa5'*n)
 vm.mem_write(ALT,b'\xa5'*0x33000);vm.mem_write(0x799ae0,bytes(0x89b6e8-0x799ae0));vm.mem_write(0x776fb0,bytes(16200));vm.mem_write(STACK,bytes(0x100000));write(0,0)
 if p[5]&1:
  for i in range(0,50,7):write(0x776fb4+i*324,BASE+0x8000+i*64);write(0x7770e8+i*324,0x10002)
 for i in range(4):
  table,length=regions[10+i]
  for j in range(0,length,4):write(table+j,0x12340000+j+i*0x100)
 for i,kind in enumerate([0xe3,0xe4,0xe1,0xe6,0xe5]):
  if p[6]&(1<<i):write(0x799ae0+i*264,1,1);write(0x799af0+i*264,kind,2)
 for a,v in [(0xe39080,0x12345678),(0xeaac28,p[7]),(0xe945c0,0x123400a0),(0xe945c4,0x11112222),
  (0x5e17d8,p[4]),(0x5e17e0,4),(0x5e17e4,1),(0x5e17c8,0),(0x605a3c,p[11]),(0xd38360,0x11223344)]:write(a,v)
 write(0xe38ea8,p[12],1);write(0x7848dc,-1234,2);vm.mem_write(BASE+0x500,b'restore-test.map\0')
 if op==5:write(ALT,-300);write(ALT+4,258);write(ALT+8,0x12345678)
 address=[0x4d4ae0,0x4d41e0,0x4d5820,0x4d58b0,0x4bd330,0x4bd360,0x4bd2d0,0x4bd280][op]
 args=[37] if op<2 else ([BASE+0x500,p[10]] if op==2 else ([2,p[10]] if op==3 else []))
 sp=STACK+0xf0000;write(sp,STOP)
 for i,value in enumerate(args):write(sp+4+i*4,value)
 saved=[(UC_X86_REG_EBX,0x11111111),(UC_X86_REG_EBP,0x22222222),(UC_X86_REG_ESI,0x33333333),(UC_X86_REG_EDI,0x44444444)]
 for register,value in saved:vm.reg_write(register,value)
 vm.reg_write(UC_X86_REG_ECX,ALT);vm.reg_write(UC_X86_REG_ESP,sp);vm.reg_write(UC_X86_REG_EFLAGS,2);vm.reg_write(UC_X86_REG_FPCW,0x37f)
 vm.emu_start(address,STOP,count=100000000)
 assert vm.reg_read(UC_X86_REG_EIP)==STOP and vm.reg_read(UC_X86_REG_ESP)==sp+4,'Native stack/budget differs'
 assert all(vm.reg_read(register)==value for register,value in saved),'Native saved register differs'
 result=vm.reg_read(UC_X86_REG_EAX) if op in [2,3,6] else 0
 return {'result':result,'events':events.copy(),'state':b''.join(bytes(vm.mem_read(a,n)) for a,n in regions).hex()}
default=[0,160,0xffffffff,1,0,1,31,1,0,240,1,0,2]
cases=[]
for op,version,codec in itertools.product([0,1],list(range(114,161))+[256],[0,256]):
 p=default.copy();p[0]=op;p[1]=version;p[4]=codec;cases.append(p)
for op,version,failed in itertools.product([0,1],[147,160],[0,1,4,23,174,180]):
 p=default.copy();p[0]=op;p[1]=version;p[2]=failed;cases.append(p)
for version,flags,player in itertools.product([147,160],[0,1,2,4,8,16,31],[0,1]):
 p=default.copy();p[0]=1;p[1]=version;p[6]=flags;p[7]=player;cases.append(p)
for op,version,header,editor,IO_fail in itertools.product([2,3],[147,160],[0,1],[0,1,257],[0,1,2]):
 p=default.copy();p[0]=op;p[1]=version;p[8]=IO_fail;p[10]=header;p[11]=editor;cases.append(p)
for mission,offset in itertools.product([0,126,127,128,255],[0,0xffffffff,240]):
 p=default.copy();p[0]=3;p[9]=offset;p[12]=mission;cases.append(p)
for op in [0,2]:
 p=default.copy();p[0]=op;p[3]=0;cases.append(p)
for variant in [0,2,3]:
 p=default.copy();p[5]=variant;cases.append(p)
for op in [4,5,6,7]:
 p=default.copy();p[0]=op;cases.append(p)
exe=ROOT/'build/c-only-debug/Debug/restore_mission_loaders_verify.exe'
process=subprocess.Popen([str(exe),'--oracle-batch'],cwd=ROOT,stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True)
report={'format':1,'original_game_executed':False,'native_sha256':digest,'compiled_C_sha256':hashlib.sha256(exe.read_bytes()).hexdigest(),
 'method':'Actual original 004d4ae0/004d41e0 controllers, 004d5820/004d58b0 wrappers and 004bd280/004bd2d0/004bd330/004bd360/004bd650 object initialization. Nested chunk/route/compatibility/OS endpoints controlled. Full watched state and guards, helper arguments/order/state snapshots, AL/full return contracts and saved-register/stack ABI compared. Editor return uses controlled 345.75 and actual native 0056133c conversion.',
 'limitations':['Nested chunk/route implementations have separate native verification. These controlled-controller checks do not prove actual file loading, compatibility helpers, city preparation or playable gameplay.']}
checked=0;p=None
try:
 assert json.loads(process.stdout.readline())=={'fields':13}
 for p in cases:
  process.stdin.write(' '.join(map(str,p))+'\n');process.stdin.flush();line=process.stdout.readline();assert line,'C exited: '+process.stderr.read()
  actual=json.loads(line);expected=native(p)
  if actual!=expected:
   first=[i//2 for i in range(0,min(len(actual['state']),len(expected['state'])),2) if actual['state'][i:i+2]!=expected['state'][i:i+2]][:8]
   raise AssertionError(repr({'C_result':actual['result'],'native_result':expected['result'],'first_event_difference':[(i,a,b) for i,(a,b) in enumerate(zip(actual['events'],expected['events'])) if a!=b][:1],
    'C_event_count':len(actual['events']),'native_event_count':len(expected['events']),'first_state_differences':first}))
  checked+=1
 report.update(passed=True,checked=checked)
except Exception as error:report.update(passed=False,checked=checked,case=p,error=str(error),native_eip=hex(vm.reg_read(UC_X86_REG_EIP)))
finally:process.stdin.close();process.wait(timeout=10)
sources=['include/mission_loaders.h','include/mission_routes.h','tests/mission_loaders.c','tools/verify_mission_loaders_emulation.py','metadata/mission-loaders-native.json']+[f'src/unit_{i:05d}.c' for i in [1539,1541,1542,1543,1821,1823,1826,1827,1830,1831,1832]]
report['source_hashes']={s:hashlib.sha256((ROOT/s).read_bytes()).hexdigest() for s in sources}
(ROOT/'metadata/mission-loaders-emulation.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report,indent=2));sys.exit(0 if report['passed'] else 1)
