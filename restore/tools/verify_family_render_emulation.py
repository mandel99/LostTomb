"""Compare family/options layout and exact draw/audio calls with native instructions."""
from pathlib import Path
import hashlib,itertools,json,struct,subprocess,sys
ROOT=Path(__file__).resolve().parent.parent;sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_BLOCK,UC_HOOK_CODE
from unicorn.x86_const import *
data=(ROOT/'build/smoke-game/Pharaoh.exe').read_bytes();digest=hashlib.sha256(data).hexdigest()
assert digest=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
u16=lambda b,p:struct.unpack_from('<H',b,p)[0];u32=lambda b,p:struct.unpack_from('<I',b,p)[0]
pe=u32(data,0x3c);opt=pe+24;table=opt+u16(data,pe+20);base=u32(data,opt+28);size=u32(data,opt+56)
vm=Uc(UC_ARCH_X86,UC_MODE_32);vm.mem_map(base,(size+4095)&~4095)
for i in range(u16(data,pe+6)):
 p=table+i*40;vm.mem_write(base+u32(data,p+12),data[u32(data,p+20):u32(data,p+20)+u32(data,p+16)])
BASE=0x30000000;STOP=0x33000000;STACK=0x34000000
for a in [BASE,STOP,STACK]:vm.mem_map(a,0x10000)
def write(a,v,n=4):vm.mem_write(a,(v&((1<<(8*n))-1)).to_bytes(n,'little'))
def read(a):return int.from_bytes(vm.mem_read(a,4),'little')
def string(a):
 out=bytearray()
 for i in range(1024):
  c=bytes(vm.mem_read(a+i,1))
  if c==b'\0':return bytes(out)
  out+=c
 raise AssertionError('Unterminated draw text')
def hash_string(a):
 v=2166136261
 for c in string(a):v=((v^c)*16777619)&0xffffffff
 return v
events=[];playing=0
counts={0x4cf1c0:0,0x53e590:3,0x536820:4,0x536a30:4,0x536c30:6,0x425730:4,0x4ccb30:7,0x4cc770:5,0x4cc720:6,0x4cc110:2,0x448370:3,0x4cde60:2,0x4cde40:2,0x55f561:1,0x55f5a1:1,0x55f449:3}
def endpoint(cpu,a,n,user):
 sp=cpu.reg_read(UC_X86_REG_ESP);args=[read(sp+4+i*4) for i in range(counts[a])];record=args.copy();result=1;cleanup=0
 if a in [0x4cde60,0x4cde40]:
  x,y=[struct.unpack('<i',struct.pack('<I',v))[0] for v in args];result=int(x*100/y) if a==0x4cde60 else int(x*y/100);record=None
 elif a in [0x4cc770,0x4cc720]:record[0]=hash_string(args[0])
 elif a==0x4cc110:write(0xe20768,BASE+0x4000)
 elif a==0x448370:
  record=[hash_string(args[0]),args[2],read(0x784f0c)];assert args[2]>=15;vm.mem_write(args[1],b'Restore family\0')
 elif a in [0x55f561,0x55f5a1,0x55f449]:
  record=[cpu.reg_read(UC_X86_REG_ECX)]+args;cleanup=counts[a]*4
  if a==0x55f449:record[1]=hash_string(args[0])
  if a==0x55f5a1:result=playing
 if a==0x53e590:cleanup=12
 if record is not None:events.append([a,len(record)]+record+[0]*(8-len(record)))
 cpu.reg_write(UC_X86_REG_EAX,result&0xffffffff);cpu.reg_write(UC_X86_REG_EIP,read(sp));cpu.reg_write(UC_X86_REG_ESP,sp+4+cleanup)
def guard(cpu,a,n,user):
 if a not in counts and not any(lo<=a and a+n<=hi for lo,hi in [(0x5165b0,0x516890),(0x4cb4d0,0x4cb6a0),(0x4cd1a0,0x4cd1f0),(0x4cd240,0x4cd290),(0x4cd390,0x4cd3d1)]):raise RuntimeError('Native family drawing escaped at '+hex(a))
vm.hook_add(UC_HOOK_BLOCK,guard)
for a in counts:vm.hook_add(UC_HOOK_CODE,endpoint,begin=a,end=a)
def native(p):
 global playing
 op,files,scroll,selected,hover,redraw,ox,oy,flags,playing,music,gender=p;events.clear();vm.mem_write(BASE,bytes(0x10000))
 vals=[(0xeadfac,BASE+0x1000),(0xeadfb8,BASE+0x2000),(0xeadfb0,files),(0xeadfc8,scroll),(0xeadfcc,selected),(0x7056c8,hover),(0xe38da8,ox),(0xe38da4,oy),
  (0xf55c9c,35000),(0xf56494,46000),(0xf56f8c,1005),(0xf56f94,335),(0xf56f9c,871),(0xf56fb4,18765),(0xf55cac,37000),(0xf55b88,music),(0x784f0c,gender),(0xe20768,0x12345678)]
 for a,v in vals:write(a,v)
 for a,v in [(0xe38dca,redraw),(0xe38dc9,0),(0xd3b188,bool(flags&1)),(0xd3b18a,bool(flags&2)),(0xd3b18b,bool(flags&4)),(0xe38e64,1)]:write(a,v,1)
 vm.mem_write(BASE+0x4000,b'[player_name] family\0')
 for i in range(files):write(BASE+0x1000+i*4,i*32);vm.mem_write(BASE+0x2000+i*32,('Family%03u'%i).encode()+b'\0')
 sp=STACK+0xe000;write(sp,STOP)
 saved=[(UC_X86_REG_EBX,0x11111111),(UC_X86_REG_EBP,0x22222222),(UC_X86_REG_ESI,0x33333333),(UC_X86_REG_EDI,0x44444444)]
 for r,v in saved:vm.reg_write(r,v)
 vm.reg_write(UC_X86_REG_ESP,sp);vm.reg_write(UC_X86_REG_EFLAGS,2)
 vm.emu_start([0x516630,0x516720,0x4cb4d0,0x4cb5a0][op],STOP,count=100000)
 assert vm.reg_read(UC_X86_REG_EIP)==STOP and vm.reg_read(UC_X86_REG_ESP)==sp+4,'Native stack/budget differs'
 assert all(vm.reg_read(r)==v for r,v in saved),'Native saved registers differ'
 return {'state':[int.from_bytes(vm.mem_read(0xe38dc9,1),'little'),read(0x784f0c),read(0xe20768)],'events':events.copy()}
cases=[]
for op,count,scroll,selected,hover,redraw,viewport in itertools.product([0,1],[0,1,12,13,260],[0,1,248],[0,11,259],[0,1,12,13,15,16,256],[0,1,2],[(0,0),(80,60),(192,144)]):
 cases.append([op,count,scroll,selected,hover,redraw,*viewport,7,0,256,258])
for op,flags,hover,redraw,playing,music,viewport in itertools.product([2,3],range(8),[0,1,2,3,4,5,256],[0,1],[0,1],[0,1,256],[(0,0),(80,60),(192,144)]):
 cases.append([op,0,0,0,hover,redraw,*viewport,flags,playing,music,258])
exe=ROOT/'build/c-only-debug/Debug/restore_family_render_verify.exe'
process=subprocess.Popen([str(exe),'--oracle-batch'],cwd=ROOT,stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True)
report={'format':1,'original_game_executed':False,'native_sha256':digest,'method':'Bounded original family/options background and foreground, row text and border/label coordinate wrappers; final sprite, text, sound and percentage endpoints controlled.',
 'compiled_C_sha256':hashlib.sha256(exe.read_bytes()).hexdigest(),'limitations':['Exact draw/label/audio arguments and state are compared; final pixels, actual template expansion and audio playback have separate verification.']}
checked=0;p=None
try:
 assert json.loads(process.stdout.readline())=={'fields':12}
 for p in cases:
  process.stdin.write(' '.join(map(str,p))+'\n');process.stdin.flush();line=process.stdout.readline();assert line,'C exited: '+process.stderr.read()
  actual=json.loads(line);expected=native(p);assert actual==expected,repr({'C':actual,'native':expected});checked+=1
 report.update(passed=True,checked=checked)
except Exception as error:report.update(passed=False,checked=checked,case=p,error=str(error),native_eip=hex(vm.reg_read(UC_X86_REG_EIP)))
finally:process.stdin.close();process.wait(timeout=10)
sources=['src/unit_01652.c','src/unit_01653.c','src/unit_01706.c','src/unit_01708.c','src/unit_01712.c','src/unit_02276.c','src/unit_02277.c','src/unit_02278.c','tests/family_render.c','tools/verify_family_render_emulation.py']
report['source_hashes']={s:hashlib.sha256((ROOT/s).read_bytes()).hexdigest() for s in sources}
(ROOT/'metadata/family-render-emulation.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report,indent=2));sys.exit(0 if report['passed'] else 1)
