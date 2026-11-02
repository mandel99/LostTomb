"""Compare native layout wrappers and coordinate-keyed records with independent C."""
from pathlib import Path
import hashlib,itertools,json,struct,subprocess,sys
ROOT=Path(__file__).resolve().parent.parent;sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_BLOCK,UC_HOOK_CODE
from unicorn.x86_const import *
data=(ROOT/'build/smoke-game/Pharaoh.exe').read_bytes()
assert hashlib.sha256(data).hexdigest()=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
u16=lambda b,p:struct.unpack_from('<H',b,p)[0];u32=lambda b,p:struct.unpack_from('<I',b,p)[0]
pe=u32(data,0x3c);opt=pe+24;table=opt+u16(data,pe+20);base=u32(data,opt+28);size=u32(data,opt+56)
vm=Uc(UC_ARCH_X86,UC_MODE_32);vm.mem_map(base,(size+4095)&~4095)
for i in range(u16(data,pe+6)):
 p=table+i*40;vm.mem_write(base+u32(data,p+12),data[u32(data,p+20):u32(data,p+20)+u32(data,p+16)])
BASE=0x30000000;STOP=0x33000000;STACK=0x34000000
for a in [BASE,STOP,STACK]:vm.mem_map(a,0x10000)
def write(a,v,n=4):vm.mem_write(a,(v&((1<<(8*n))-1)).to_bytes(n,'little'))
def read(a):return int.from_bytes(vm.mem_read(a,4),'little')
events=[]
endpoints={0x4d69b0:0,0x4cc110:2,0x4cbf30:2,0x4ccfd0:7,0x4ccb30:7,0x4ccbb0:6,0x536c30:6,0x537000:6,0x536a30:4,0x4cce00:7,0x536820:4,0x4cd9d0:5,0x4cd510:5}
def endpoint(cpu,a,n,user):
 sp=cpu.reg_read(UC_X86_REG_ESP);count=endpoints[a];args=[read(sp+4+i*4) for i in range(count)]
 events.append([a,count]+args+[0]*(9-count))
 result=0
 if a==0x4cc110:
  group,entry=args;write(0xe20768,BASE+0x1000+group*256+(entry*16 if entry<3 else 256))
 if a==0x4cbf30:result=(args[1]&0xffff)+(((args[0]-BASE)>>4)%23)
 cpu.reg_write(UC_X86_REG_EAX,result);cpu.reg_write(UC_X86_REG_EIP,read(sp));cpu.reg_write(UC_X86_REG_ESP,sp+4)
def guard(cpu,a,n,user):
 if a not in endpoints and not (0x4d5d70<=a<0x4d69b0 or 0x537140<=a<0x537260):raise RuntimeError('Escaped native layout at '+hex(a))
vm.hook_add(UC_HOOK_BLOCK,guard)
for a in endpoints:vm.hook_add(UC_HOOK_CODE,endpoint,begin=a,end=a)
def native(p):
 op,mode,x,y,ox,oy,w,h,mx,my,debug,matched,initialized,seed,sub,color=p;events.clear()
 for a,v in [(0x605a44,sub),(0xe94d98,debug),(0xe94d9c,initialized),(0xe38e34,mx),(0xe38e30,my)]:write(a,v)
 write(0xe38dda,mode,1);vm.mem_write(0xe945f8,b'\xa5'*24);index=100+sub if mode==7 else mode
 write(0xf53fe4,0xa5a5a5a5);write(0xf53fe8,0xa5a5a5a5)
 for i in range(6):write(0xe945d0+i*4,0x10000+seed+i)
 write(0xe94dac,w);write(0xe94db0,h);write(0xf56f8c,w);vm.mem_write(0x5c28ac,b'\xa5'*840)
 for i in range(3):
  vals=[x-ox+(1 if matched==2 else 0),y-oy,7+seed+i*100,9+seed+i*100,15+seed+i,17+seed+i,0]
  vm.mem_write(BASE+i*32,struct.pack('<7I',*(v&0xffffffff for v in vals)));write(0xe94618+index*16+i*4,BASE+i*32 if matched else 0)
 args=([[31,2,x,y,w,h,color,ox,oy],[31,2,x,y,w,h,color,ox,oy],[31,2,x,y,w,color,ox,oy],
  [0,x,y,w,h,color,ox,oy],[7,x,y,w,h,color,ox,oy],[x,y,w,h,ox,oy],
  [31,2,11,x,y,w,color,ox,oy],[debug,x,y,w,h,x-ox,y-oy,ox,oy],[x,y,color]][op] if op<9 else [debug] if op in [11,14] else [])
 if op==8:
  for a,v in [(0xe2078c,w),(0xe20790,ox),(0xe20794,oy),(0xe92e0c,mx),(0xf53fe4,my),(0xf53fe8,debug)]:write(a,v)
  write(0xe2079b,sub&1,1)
 sp=STACK+0x8000
 for i,v in enumerate([STOP]+args):write(sp+i*4,v)
 saved=[(UC_X86_REG_EBX,0x11111111),(UC_X86_REG_EBP,0x22222222),(UC_X86_REG_ESI,0x33333333),(UC_X86_REG_EDI,0x44444444)]
 for r,v in saved:vm.reg_write(r,v)
 vm.reg_write(UC_X86_REG_ESP,sp);vm.reg_write(UC_X86_REG_ECX,0x12345678);vm.reg_write(UC_X86_REG_EFLAGS,2)
 vm.emu_start([0x4d6160,0x4d6250,0x4d62f0,0x4d63d0,0x4d6480,0x4d65b0,0x4d6860,0x4d6090,0x537140,0x4d5dd0,0x4d5e70,0x4d5ef0,0x4d5ed0,0x4d5ee0,0x4d5f80][op],STOP,count=100000)
 assert vm.reg_read(UC_X86_REG_EIP)==STOP and vm.reg_read(UC_X86_REG_ESP)==sp+4,'Native stack/budget differs: '+hex(vm.reg_read(UC_X86_REG_ESP))
 assert all(vm.reg_read(r)==v for r,v in saved),'Native callee-saved register differs'
 return {'result':vm.reg_read(UC_X86_REG_EAX) if op in [11,12,13,14] else 0,
  'state':[read(0xe94d9c)]+[read(0xe945f8+i*4) for i in range(6)]+[read(0xf53fe4),read(0xf53fe8)]+[read(0xe945d0+i*4) for i in range(6)]+[read(0xe94dac),read(0xe94db0)]+[read(0x5c28ac+i) for i in range(0,840,4)],'events':events.copy()}
cases=[]
for op,mode,debug,matched,initialized,point in itertools.product(range(8),[0,7,54,83,255],[0,1,3,4,258],[0,1,2],[0,1],range(3)):
 x,y=800,600;ox,oy=192,144;seed=17
 mx,my=[(-100,-100),(x+3,y+3),(ox+7+seed+200+3,oy+9+seed+200+3)][point]
 cases.append([op,mode,x,y,ox,oy,335,31,mx,my,debug,matched,initialized,seed,3,0xffff])
for state,elapsed,insert,last in itertools.product([0,1,2,256],[-1,0,199,200,399,400,100000],[0,1],[0x1234fff0,0x7ffffff0,0xfffffff0]):
 cases.append([8,54,500,300,320,256,7,31,(last+elapsed)&0xffffffff,last,state,0,1,17,insert,0xffff])
for op,font,kind in itertools.product(range(9,15),[17,255,335,18765,65535],[0,1,2,3,4,7,10,28,99]):
 cases.append([op,54,800,600,192,144,font,1005,0,0,kind,1,1,17,3,0xffff])
exe=ROOT/'build/c-only-debug/Debug/restore_menu_layout_verify.exe'
process=subprocess.Popen([str(exe),'--oracle-batch'],cwd=ROOT,stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True)
report={'format':1,'original_game_executed':False,'method':'Bounded original layout initialization, extent calculation, getters, coordinate lookup/wrappers and caret; text selection, width measurement, layout-file loading and final draw endpoints controlled',
 'limitations':['Native layout-file loading and actual glyph/pixel output remain separate checks.'],
 'compiled_C_sha256':hashlib.sha256(exe.read_bytes()).hexdigest()}
checked=0;p=None
try:
 assert json.loads(process.stdout.readline())=={'fields':16}
 for p in cases:
  process.stdin.write(' '.join(map(str,p))+'\n');process.stdin.flush();line=process.stdout.readline()
  assert line,'C oracle exited: '+process.stderr.read()
  actual=json.loads(line);expected=native(p);assert actual==expected,repr({'C':actual,'native':expected});checked+=1
 report.update(passed=True,checked=checked)
except Exception as error:report.update(passed=False,checked=checked,case=p,error=str(error),native_eip=hex(vm.reg_read(UC_X86_REG_EIP)))
finally:process.stdin.close();process.wait(timeout=10)
sources=['include/menu_layout.h','src/unit_01836.c','src/unit_01837.c','src/unit_01838.c','src/unit_01839.c','src/unit_01840.c','src/unit_01841.c','src/unit_01842.c','src/unit_01844.c','src/unit_01845.c','src/unit_01846.c','src/unit_01848.c','src/unit_01849.c','src/unit_01851.c','src/unit_01852.c','src/unit_01854.c','src/unit_01855.c','src/unit_01856.c','src/unit_01859.c','src/unit_02456.c','tests/menu_layout.c','tools/verify_menu_layout_emulation.py']
report['source_hashes']={s:hashlib.sha256((ROOT/s).read_bytes()).hexdigest() for s in sources}
(ROOT/'metadata/menu-layout-emulation.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report,indent=2));sys.exit(0 if report['passed'] else 1)
