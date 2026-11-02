"""Compare original modal input, callbacks, layout and byte text measurement with C."""
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
def byte(a):return int.from_bytes(vm.mem_read(a,1),'little')
def string(a):
 out=bytearray()
 for i in range(1024):
  c=bytes(vm.mem_read(a+i,1))
  if c==b'\0':return bytes(out)
  out+=c
 raise AssertionError('Unterminated text')
events=[];measured=mutate=return_mode=0
counts={BASE+0xf000:0,0x536820:4,0x4ccb30:7,0x4ccfd0:7,0x425730:4,0x47d350:1,0x4cbf30:2,0x51f780:0}
def endpoint(cpu,a,n,user):
 sp=cpu.reg_read(UC_X86_REG_ESP);args=[read(sp+4+i*4) for i in range(counts[a])];result=0
 if a==BASE+0xf000:
  args=[byte(0xd3b439),read(0x70570c),byte(0xe38dda)]
  if mutate:write(0x70570c,return_mode);write(0xe38dda,0x55,1);write(0xe38dca,0x7a,1)
 elif a==0x4cbf30:args.append(len(string(args[0])));result=measured
 if a!=0x51f780:events.append([a,len(args)]+args+[0]*(8-len(args)))
 cpu.reg_write(UC_X86_REG_EAX,result);cpu.reg_write(UC_X86_REG_EIP,read(sp));cpu.reg_write(UC_X86_REG_ESP,sp+4)
def guard(cpu,a,n,user):
 if a not in counts and not any(lo<=a and a+n<=hi for lo,hi in [(0x425570,0x4256a0),(0x425990,0x425bc0),(0x4264d0,0x4265d0),(0x4cc6c0,0x4cc720)]):raise RuntimeError('Native modal escaped at '+hex(a))
vm.hook_add(UC_HOOK_BLOCK,guard)
for a in counts:vm.hook_add(UC_HOOK_CODE,endpoint,begin=a,end=a)
regions=[(0x705704,24),(0xe38dc9,2),(0xe38dda,1),(0xd3b439,1),(0xd3b194,8),(0x5c8168+24,4),(0x5c818c+24,4),(0xe20768,4)]
def native(p):
 global measured,mutate,return_mode
 op,buttons,previous,message,x,y,mx,my,flags,return_mode,mutate,measured,font,entry=p;events.clear();vm.mem_write(BASE,bytes(0x10000));vm.mem_write(BASE+0x1000,b'Alpha\0Beta\0Gamma\0Tail\0')
 for a,v in [(0xe36c70,BASE),(0xe34c38+5*8,0x1000),(0xe34c3c+5*8,0xeeeeeeee),(0xe20768,0x12345678),
  (0x705704,BASE+0xf000),(0x705708,buttons),(0x70570c,previous),(0x705710,message),(0x705718,x),(0x705714,y),(0xe38da8,x),(0xe38da4,y),(0xe38e34,mx),(0xe38e30,my),(0xe38e2c,0),(0xe38e28,0),(0xf56f8c,font),(0xf56fb4,18765),(0xd3b194,0x11111111),(0xd3b198,0x22222222)]:write(a,v)
 for i,a in enumerate([0xe38e00,0xe38e03,0xe38e04,0xe38e28,0xe38df8,0xe38df4,0xe38df7]):write(a,bool(flags&(1<<i)),1)
 for a,v in [(0xe38dda,63),(0xe38dca,0x31),(0xe38dc9,0x77),(0xd3b439,0x44)]:write(a,v,1)
 for i in range(2):write(0x5c8168+i*36+24,1)
 args=[]
 if op==0:write(0xe38dda,previous,1);args=[message,BASE+0xf000,buttons]
 if op==4:write(0x705710,0)
 if op==6:args=[5,entry,font]
 sp=STACK+0xe000
 for i,v in enumerate([STOP]+args):write(sp+i*4,v)
 saved=[(UC_X86_REG_EBX,0x11111111),(UC_X86_REG_EBP,0x22222222),(UC_X86_REG_ESI,0x33333333),(UC_X86_REG_EDI,0x44444444)]
 for r,v in saved:vm.reg_write(r,v)
 vm.reg_write(UC_X86_REG_ESP,sp);vm.reg_write(UC_X86_REG_EFLAGS,2)
 vm.emu_start([0x4264d0,0x426530,0x426590,0x4265b0,0x425570,0x425640,0x4cc6c0][op],STOP,count=100000)
 assert vm.reg_read(UC_X86_REG_EIP)==STOP and vm.reg_read(UC_X86_REG_ESP)==sp+4,'Native stack/budget differs'
 assert all(vm.reg_read(r)==v for r,v in saved),'Native saved registers differ'
 return {'result':vm.reg_read(UC_X86_REG_EAX) if op==6 else 0,'state':b''.join(bytes(vm.mem_read(a,n)) for a,n in regions).hex(),'events':events.copy()}
cases=[]
for buttons,flags,mouse,mut in itertools.product([0,1,2,0xffffffff],range(128),[(-100,-100),(260,110),(200,110),(289,133),(226,133)],[0,1]):
 cases.append([1,buttons,47,90,272,224,272+mouse[0],224+mouse[1],flags,258,mut,419,1005,1])
for op,buttons,mode,origin,mut in itertools.product([0,2,3,4,5],[0,1,2],[0,47,128,255],[(0,0),(80,60),(192,144)],[0,1]):
 cases.append([op,buttons,mode,90,*origin,0,0,0,258,mut,419,18765,1])
for width,origin,font in itertools.product([0,419,420,1005],[(0,0),(80,60),(192,144)],[335,18765]):cases.append([4,2,47,90,*origin,0,0,0,258,0,width,font,1])
for entry,width,font in itertools.product([0,1,2,0xffffffff],[0,419,420],[0,335,18765,65535]):cases.append([6,2,47,90,0,0,0,0,0,258,0,width,font,entry])
exe=ROOT/'build/c-only-debug/Debug/restore_menu_dialog_verify.exe'
process=subprocess.Popen([str(exe),'--oracle-batch'],cwd=ROOT,stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True)
report={'format':1,'original_game_executed':False,'native_sha256':digest,'method':'Native modal setup, real small-button dispatch, yes/no callbacks, escape return, background/foreground and text byte selector; nested callback, draw, sound and width endpoints controlled.',
 'compiled_C_sha256':hashlib.sha256(exe.read_bytes()).hexdigest(),'limitations':['Response callback behavior, final pixels and sound playback have separate checks.']}
checked=0;p=None
try:
 assert json.loads(process.stdout.readline())=={'fields':14}
 for p in cases:
  process.stdin.write(' '.join(map(str,p))+'\n');process.stdin.flush();line=process.stdout.readline();assert line,'C exited: '+process.stderr.read()
  actual=json.loads(line);expected=native(p);assert actual==expected,repr({'C':actual,'native':expected});checked+=1
 report.update(passed=True,checked=checked)
except Exception as error:report.update(passed=False,checked=checked,case=p,error=str(error),native_eip=hex(vm.reg_read(UC_X86_REG_EIP)))
finally:process.stdin.close();process.wait(timeout=10)
sources=['include/menu_dialog.h','runtime/menu_dialog.c','runtime/text_selection.c','src/unit_00398.c','src/unit_00399.c','src/unit_00404.c','src/unit_00405.c','src/unit_00406.c','src/unit_00418.c','src/unit_00419.c','src/unit_01690.c','tests/menu_dialog.c','tools/verify_menu_dialog_emulation.py']
report['source_hashes']={s:hashlib.sha256((ROOT/s).read_bytes()).hexdigest() for s in sources}
(ROOT/'metadata/menu-dialog-emulation.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report,indent=2));sys.exit(0 if report['passed'] else 1)
