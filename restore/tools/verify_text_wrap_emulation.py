"""Verify original glyph advances, word scans and wrapped lines against compiled C."""
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
 for i in range(4096):
  c=bytes(vm.mem_read(a+i,1))
  if c==b'\0':return bytes(out)
  out+=c
 raise AssertionError('Unterminated line')
events=[];height=0
def endpoint(cpu,a,n,user):
 sp=cpu.reg_read(UC_X86_REG_ESP)
 if a==0x4cc110:assert [read(sp+4),read(sp+8)]==[5,1];write(0xe20768,BASE)
 else:
  text,x,y,font,color=[read(sp+4+i*4) for i in range(5)];value=string(text);assert len(value)<201
  events.append([[x,y,font,color,read(0xe20780)],value.hex()]);write(0xd3b500,height);write(0xe20778,len(value)*7)
 cpu.reg_write(UC_X86_REG_EAX,0);cpu.reg_write(UC_X86_REG_EIP,read(sp));cpu.reg_write(UC_X86_REG_ESP,sp+4)
def guard(cpu,a,n,user):
 if a not in [0x4cc110,0x4cc770] and not any(lo<=a and a+n<=hi for lo,hi in [(0x4cc020,0x4cc06e),(0x4ccfd0,0x4cd1a0)]):raise RuntimeError('Native wrapping escaped at '+hex(a))
vm.hook_add(UC_HOOK_BLOCK,guard)
for a in [0x4cc110,0x4cc770]:vm.hook_add(UC_HOOK_CODE,endpoint,begin=a,end=a)
texts=[b'',b'ABC',b'ABC DEF GHI JKL',b' ABC  DEF',b'AB$CD EF',b'one two three four five six seven eight nine ten',b'A_#B C',b'ABCDEFGHIJKLMNOPQRSTUVWXYZ',b'This deletes the selected dynasty and all of its saved games. Do you really want to do that?',b'A'*300]
fields=[0xd3b5c0,0xd3b500,0xe20760,0xe2076c,0xe20774,0xe20778,0xe20780,0xe20768]
for i in range(256):write(0x5e047c+i,i%19+1 if i%7 else 0,1)
for i in range(2048):write(0x10d6a0c+i*64,(i*3+17)%19+1,2)
def native(p):
 global height
 op,text,char,font,limit,width,initial,height=p;events.clear();vm.mem_write(BASE,bytes(0x10000))
 for a,v in [(0x126517c,limit),(0xd3b5c0,0x12345678),(0xd3b500,11),(0xe20760,0x12345678),(0xe2076c,initial),(0xe20774,0x12345678),(0xe20778,0x76543210),(0xe20780,3),(0xe20768,0x98765432)]:write(a,v)
 write(0xe20799,0,1);vm.mem_write(0xe3439c,b'\xa5'*200)
 if op:vm.mem_write(BASE,texts[text])
 args=[[char,font],[BASE,font],[BASE,500,300,width,font,0xffff],[5,1,500,300,width,font,0xffff]][op];sp=STACK+0xe000
 for i,v in enumerate([STOP]+args):write(sp+i*4,v)
 saved=[(UC_X86_REG_EBX,0x11111111),(UC_X86_REG_EBP,0x22222222),(UC_X86_REG_ESI,0x33333333),(UC_X86_REG_EDI,0x44444444)]
 for r,v in saved:vm.reg_write(r,v)
 vm.reg_write(UC_X86_REG_ESP,sp);vm.reg_write(UC_X86_REG_EFLAGS,2)
 vm.emu_start([0x4cc020,0x4cd120,0x4cd010,0x4ccfd0][op],STOP,count=5000000)
 assert vm.reg_read(UC_X86_REG_EIP)==STOP and vm.reg_read(UC_X86_REG_ESP)==sp+4,'Native stack/budget differs'
 assert all(vm.reg_read(r)==v for r,v in saved),'Native saved registers differ'
 return {'result':vm.reg_read(UC_X86_REG_EAX) if op<2 else 0,'state':[read(a) for a in fields]+[int.from_bytes(vm.mem_read(0xe20799,1),'little')],'buffer':bytes(vm.mem_read(0xe3439c,200)).hex(),'events':events.copy()}
cases=[[0,0,c,f,l,0,0,11] for c,f,l in itertools.product(range(256),[0,17,335,18765],[0,100,2048])]
for op,text,font,width,initial,height in itertools.product([1,2,3],range(9),[17,335],[0,1,10,80,100,200,420],[0,258],[11,300]):cases.append([op,text,0,font,2048,width,initial,height])
cases.extend([op,9,0,335,2048,width,0,11] for op,width in itertools.product([1,2,3],[0,1,100]))
exe=ROOT/'build/c-only-debug/Debug/restore_text_wrap_verify.exe'
process=subprocess.Popen([str(exe),'--oracle-batch'],cwd=ROOT,stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True)
report={'format':1,'original_game_executed':False,'native_sha256':digest,'method':'Native byte character widths, 64-byte sprite metric records, word scans, wrapping and label selector facade; final line draw and label lookup controlled. Exact line bytes/positions, buffer, full state, return and ABI compared.',
 'compiled_C_sha256':hashlib.sha256(exe.read_bytes()).hexdigest(),'limitations':['Final glyph pixels and actual language assets are checked separately in the C framebuffer. Font indices are nonnegative, and tested wrapped lines fit the native 200-byte scratch buffer.']}
checked=0;p=None
try:
 assert json.loads(process.stdout.readline())=={'fields':8}
 for p in cases:
  process.stdin.write(' '.join(map(str,p))+'\n');process.stdin.flush();line=process.stdout.readline();assert line,'C exited: '+process.stderr.read()
  actual=json.loads(line);expected=native(p);assert actual==expected,repr({'C':actual,'native':expected});checked+=1
 report.update(passed=True,checked=checked)
except Exception as error:report.update(passed=False,checked=checked,case=p,error=str(error),native_eip=hex(vm.reg_read(UC_X86_REG_EIP)))
finally:process.stdin.close();process.wait(timeout=10)
sources=['src/unit_01669.c','src/unit_01703.c','src/unit_01704.c','src/unit_01705.c','tests/text_wrap.c','tools/verify_text_wrap_emulation.py']
report['source_hashes']={s:hashlib.sha256((ROOT/s).read_bytes()).hexdigest() for s in sources}
(ROOT/'metadata/text-wrap-emulation.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report,indent=2));sys.exit(0 if report['passed'] else 1)
