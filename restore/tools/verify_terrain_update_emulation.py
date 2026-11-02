"""Native comparison of the terrain preparation hang repair. No game process."""
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
 q=table+i*40;vm.mem_write(base+u32(data,q+12),data[u32(data,q+20):u32(data,q+20)+u32(data,q+16)])
STOP=0x33000000;STACK=0x34000000;vm.mem_map(STOP,0x1000);vm.mem_map(STACK,0x100000)
regions=[(0xa43eac,0x1857e4),(0xbc9690,0xc90),(0x605a3c,4),(0x78d7ac,8)]
def write(a,v):vm.mem_write(a,(v&0xffffffff).to_bytes(4,'little'))
def read(a):return int.from_bytes(vm.mem_read(a,4),'little')
counts={0x486600:1,0x47d870:1,0x4767d0:6}
for a in [0x475dc0,0x476530,0x477210,0x47b570,0x47b1a0]:counts[a]=0
for a in [0x51f780,0x4770f0,0x4777c0,0x477500,0x4776f0,0x477890]:counts[a]=4
for a in [0x475720,0x475cc0,0x47bae0]:counts[a]=5
events=[]
def endpoint(cpu,a,n,user):
 sp=cpu.reg_read(UC_X86_REG_ESP);args=[read(sp+4+i*4) for i in range(counts[a])]
 events.append([a,len(args)]+args+[0]*(6-len(args)))
 cpu.reg_write(UC_X86_REG_EAX,0);cpu.reg_write(UC_X86_REG_EIP,read(sp));cpu.reg_write(UC_X86_REG_ESP,sp+4)
for a in counts:vm.hook_add(UC_HOOK_CODE,endpoint,begin=a,end=a)
def execute(entry,args=()):
 sp=STACK+0xf0000;write(sp,STOP)
 for i,v in enumerate(args):write(sp+4+i*4,v)
 saved=[(UC_X86_REG_EBX,0x11111111),(UC_X86_REG_EBP,0x22222222),(UC_X86_REG_ESI,0x33333333),(UC_X86_REG_EDI,0x44444444)]
 for r,v in saved:vm.reg_write(r,v)
 vm.reg_write(UC_X86_REG_ESP,sp);vm.reg_write(UC_X86_REG_EFLAGS,2);vm.emu_start(entry,STOP,count=10000000)
 assert vm.reg_read(UC_X86_REG_EIP)==STOP,'Native instruction budget'
 assert vm.reg_read(UC_X86_REG_ESP)==sp+4,'Native stack'
 assert all(vm.reg_read(r)==v for r,v in saved),'Saved register mismatch'
 return vm.reg_read(UC_X86_REG_EAX)
def native(p):
 op,w,h,seed,x,y,x1,y1,enable=p;events.clear()
 for i,(a,n) in enumerate(regions):vm.mem_write(a,bytes([(seed+i)&255])*n)
 write(0x78d7ac,seed);write(0x78d7b0,enable)
 for a,v in [(0xbca2d0,w),(0xbca2d4,h),(0xbca2d8,((228-h)//2)*228+(228-w)//2),(0xbca2dc,228-w),(0x605a3c,seed&0x100)]:write(a,v)
 execute(0x46f7b0)
 terrain=[0,1,0x201,0x401,0x601,0x10001,2,3]
 vm.mem_write(0xaf598c,b''.join(struct.pack('<I',terrain[(i+seed)&7]) for i in range(51984)))
 result=0
 if op==0:execute(0x479770,(x,y,enable))
 elif op==1:execute(0x475bf0,(x,y,x1,y1))
 elif op==2:execute(0x4dea50)
 else:
  result=execute(0x47a1c0 if op==3 else 0x47a1f0,(x,))
  if enable==0 or enable&0x80000000:result&=255
 return {'result':result,'hashes':[sha(bytes(vm.mem_read(a,n))) for a,n in regions],'events':events.copy()}
cases=[]
for w,h,seed,enable in itertools.product([0,1,2,3,12,140,228],[1,12,228],[0,7,256],[0,1,2]):
 for x,y in [(0,0),(w//2,h//2),(max(0,w-1),max(0,h-1)),(w,h),(0xffffffff,0xffffffff)]:cases.append([0,w,h,seed,x,y,0,0,enable])
for w,h,seed,(x,y,x1,y1) in itertools.product([0,1,3,12],[0,1,3,12],[0,7,256],[(0,0,11,11),(0xffffffff,0xffffffff,1,1),(3,3,1,1),(1,1,300,300),(0,0,0,0)]):cases.append([1,w,h,seed,x,y,x1,y1,0])
for w,h,seed in itertools.product([0,1,3,12],[0,1,3,12],[0,1,256,257]):cases.append([2,w,h,seed,0,0,0,0,0])
for op,span,enabled,x in itertools.product([3,4],[0,1,114,228,512,0x80000000,0x7fffffff],[0,1,3,228,512,0xffffffff,0x80000000],[0xffffffff,0,113,114,115,227,228,0x80000000,0x7fffffff]):cases.append([op,12,12,span,x,0,0,0,enabled])
exe=ROOT/'build/c-only-debug/Debug/restore_terrain_update_verify.exe'
process=subprocess.Popen([str(exe),'--oracle-batch'],cwd=ROOT,stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True)
report={'format':1,'original_game_executed':False,'native_sha256':digest,'compiled_C_sha256':sha(exe.read_bytes()),
 'method':'Complete native 00479770 tile update, actual 00475bf0 rectangle walk and ordered 004dea50 controller, actual geometry/predicate and 0047a1c0/0047a1f0 water boundaries. All map/state bytes, endpoint arguments, saved registers and stack compared; boundary full EAX compared when enabled, AL when disabled.',
 'limitations':['Tile sprite resolution 00486600 and the other terrain controller passes are controlled; this proves their caller contracts rather than those algorithms.','Valid map geometry 0..228; direct coordinate probes include edges and rejected negative/outside points.','Disabled water boundaries preserve unspecified incoming high EAX bits in native code; C defines those unused bits as zero. The Boolean AL contract is compared on that path.'],
 'checked':0}
checked=0;p=None
try:
 assert json.loads(process.stdout.readline())=={'fields':9}
 for p in cases:
  process.stdin.write(' '.join(map(str,p))+'\n');process.stdin.flush();line=process.stdout.readline();assert line,'C exited: '+process.stderr.read()
  actual=json.loads(line);expected=native(p)
  assert actual==expected,repr({'regions':[hex(regions[i][0]) for i,(a,b) in enumerate(zip(actual['hashes'],expected['hashes'])) if a!=b],'events':[len(actual['events']),len(expected['events'])],'first_event_difference':[(i,a,b) for i,(a,b) in enumerate(zip(actual['events'],expected['events'])) if a!=b][:1]})
  checked+=1
  if checked%200==0:print(json.dumps({'checked':checked,'total':len(cases)}),flush=True)
 report.update(passed=True,checked=checked)
except Exception as error:report.update(passed=False,checked=checked,case=p,error=str(error),native_eip=hex(vm.reg_read(UC_X86_REG_EIP)))
finally:process.stdin.close();process.wait(timeout=10)
sources=['tests/terrain_update.c','tools/verify_terrain_update_emulation.py','include/city_prepare.h','metadata/terrain-update-native.json']
sources += ['metadata/water-boundary-native.json']
sources += [f'src/unit_{i:05d}.c' for i in [1009,1011,1037,1091,1097,1098,1893]]
sources += [f'src/unit_{i:05d}.c' for i in [470,988,1020,1095,1099,1100,1250,1274]]
report['source_hashes']={s:sha((ROOT/s).read_bytes()) for s in sources}
(ROOT/'metadata/terrain-update-emulation.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({k:v for k,v in report.items() if k!='source_hashes'},indent=2));sys.exit(0 if report['passed'] else 1)
