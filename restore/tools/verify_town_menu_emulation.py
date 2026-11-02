"""Run the original town traversal and iterator, including their actual predicates."""
from pathlib import Path
import hashlib,itertools,json,struct,subprocess,sys
ROOT=Path(__file__).resolve().parent.parent;sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_BLOCK
from unicorn.x86_const import *
data=(ROOT/'build/smoke-game/Pharaoh.exe').read_bytes();digest=hashlib.sha256(data).hexdigest()
assert digest=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
u16=lambda b,p:struct.unpack_from('<H',b,p)[0];u32=lambda b,p:struct.unpack_from('<I',b,p)[0]
pe=u32(data,0x3c);opt=pe+24;table=opt+u16(data,pe+20);base=u32(data,opt+28);size=u32(data,opt+56)
vm=Uc(UC_ARCH_X86,UC_MODE_32);vm.mem_map(base,(size+4095)&~4095)
for i in range(u16(data,pe+6)):
 p=table+i*40;vm.mem_write(base+u32(data,p+12),data[u32(data,p+20):u32(data,p+20)+u32(data,p+16)])
BASE=0x30000000;ALT=BASE+0x1000;STOP=0x33000000;STACK=0x34000000;TOWNS=0x785d70;LENGTH=18628
for a in [BASE,STOP,STACK]:vm.mem_map(a,0x10000)
def write(a,v,n=4):vm.mem_write(a,(v&((1<<(8*n))-1)).to_bytes(n,'little'))
def read(a,n=4):return int.from_bytes(vm.mem_read(a,n),'little')
def guard(cpu,a,n,user):
 ranges=[(0x41e270,0x41e468),(0x4495f0,0x4495f3),(0x449600,0x449618),(0x4496b0,0x4496c1),(0x4496d0,0x449707),(0x44bb90,0x44bc20),(0x44c0d0,0x44c0f6)]
 if not any(lo<=a and a+n<=hi for lo,hi in ranges):raise RuntimeError('Native town traversal escaped at '+hex(a))
vm.hook_add(UC_HOOK_BLOCK,guard)
def setup(address,count,seed):
 vm.mem_write(address,b'\xa5'*LENGTH);write(address,count)
 for i in range(150):
  a=address+4+i*124;write(a,i+bool(seed&64),2);write(a+2,[1,2,23,0][(i+seed)%4],1);write(a+0x28,1|(16 if count and i==seed%count else 0),1);write(a+0x60,seed%4,1)
  for offset in [0x24,0x26,0x52,0x54]:write(a+offset,0xffff,2)
  if count and i<count:write(a+[0x24,0x26,0x52,0x54][(i+seed)%4],i if seed&8 else (i+1)%count,2)
  if seed&16 and 0<count<=6 and i<count:write(a+0x24,(i+2)%count,2)
def native(p):
 op,count,seed,depth,previous,create=p;setup(TOWNS,count,seed);setup(ALT,count,seed+1);write(BASE,previous,2);write(0x5d9a40,258);write(0x78a634,0x44,1)
 sp=STACK+0xe000;write(sp,STOP)
 args=[] if op==0 else ([TOWNS+4+(seed%count)*124,depth] if op==1 else [0 if previous==0xffffffff else BASE,create])
 for i,value in enumerate(args):write(sp+4+i*4,value)
 saved=[(UC_X86_REG_EBX,0x11111111),(UC_X86_REG_EBP,0x22222222),(UC_X86_REG_ESI,0x33333333),(UC_X86_REG_EDI,0x44444444)]
 for r,v in saved:vm.reg_write(r,v)
 vm.reg_write(UC_X86_REG_ESP,sp);vm.reg_write(UC_X86_REG_ECX,ALT if op==3 else TOWNS);vm.reg_write(UC_X86_REG_EFLAGS,2)
 vm.emu_start([0x41e3b0,0x41e270,0x44bb90,0x44bb90][op],STOP,count=2000000)
 assert vm.reg_read(UC_X86_REG_EIP)==STOP and vm.reg_read(UC_X86_REG_ESP)==sp+4+(8 if op in [2,3] else 0),'Native stack/budget differs'
 assert all(vm.reg_read(r)==v for r,v in saved),'Native saved register differs'
 return {'result':0 if op==1 else vm.reg_read(UC_X86_REG_EAX),'last':read(0x5d9a40),'flag':read(0x78a634,1),'state':(bytes(vm.mem_read(TOWNS,LENGTH))+bytes(vm.mem_read(ALT,LENGTH))).hex()}
cases=[];seeds=list(range(8))+[16,24,64,72]
cases.extend([0,count,seed,1,0xffffffff,0] for count,seed in itertools.product([0,1,6,32,140,150],seeds))
cases.extend([1,count,seed,depth,0xffffffff,0] for count,seed,depth in itertools.product([1,6,32],seeds,[0,1,6,7,8]))
cases.extend([op,count,seed,1,previous,create] for op,count,seed,previous,create in itertools.product([2,3],[0,1,6,140,150],[0,1,2,3],[0xffffffff,0,5,139,140],[0,1]))
exe=ROOT/'build/c-only-debug/Debug/restore_town_menu_verify.exe'
process=subprocess.Popen([str(exe),'--oracle-batch'],cwd=ROOT,stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True)
report={'format':1,'original_game_executed':False,'native_sha256':digest,'method':'Unmodified native recursive connectivity scan, actual link/type predicates and checked lookup, global unconnected-town count, iterator, alternate object receiver and sentinel fallback. Full contents of both 150-slot lists including metadata, side-effect globals, result and native ABI compared; no endpoint replacements.',
 'compiled_C_sha256':hashlib.sha256(exe.read_bytes()).hexdigest(),'limitations':['Records are bounded valid fixtures with one/two links per node; map/scenario parser correctness remains separate.']}
checked=0;p=None
try:
 assert json.loads(process.stdout.readline())=={'fields':6}
 for p in cases:
  process.stdin.write(' '.join(map(str,p))+'\n');process.stdin.flush();line=process.stdout.readline();assert line,'C exited: '+process.stderr.read()
  actual=json.loads(line);expected=native(p)
  if actual!=expected:raise AssertionError(repr({'C_result':actual['result'],'native_result':expected['result'],'C_last':actual['last'],'native_last':expected['last'],'state_equal':actual['state']==expected['state']}))
  checked+=1
 report.update(passed=True,checked=checked)
except Exception as error:report.update(passed=False,checked=checked,case=p,error=str(error),native_eip=hex(vm.reg_read(UC_X86_REG_EIP)))
finally:process.stdin.close();process.wait(timeout=10)
sources=['include/town_menu.h','runtime/town_menu.c','src/unit_00300.c','src/unit_00301.c','src/unit_00720.c','tests/town_menu.c','tools/verify_town_menu_emulation.py','metadata/town-iterator-callers-native.json']
report['source_hashes']={s:hashlib.sha256((ROOT/s).read_bytes()).hexdigest() for s in sources}
(ROOT/'metadata/town-menu-emulation.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report,indent=2));sys.exit(0 if report['passed'] else 1)
