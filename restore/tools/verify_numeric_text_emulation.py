"""Compare complete native numeric composition and date/plural wrappers."""
from pathlib import Path
import hashlib,itertools,json,struct,subprocess,sys
ROOT=Path(__file__).resolve().parent.parent;sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
data=(ROOT/'build/smoke-game/Pharaoh.exe').read_bytes();digest=hashlib.sha256(data).hexdigest();assert digest=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
u16=lambda b,p:struct.unpack_from('<H',b,p)[0];u32=lambda b,p:struct.unpack_from('<I',b,p)[0]
pe=u32(data,0x3c);opt=pe+24;table=opt+u16(data,pe+20);base=u32(data,opt+28);size=u32(data,opt+56)
vm=Uc(UC_ARCH_X86,UC_MODE_32);vm.mem_map(base,(size+4095)&~4095)
for i in range(u16(data,pe+6)):
 p=table+i*40;vm.mem_write(base+u32(data,p+12),data[u32(data,p+20):u32(data,p+20)+u32(data,p+16)])
BASE=0x30000000;STOP=0x33000000;STACK=0x34000000
for a in [BASE,STOP,STACK]:vm.mem_map(a,0x10000)
def write(a,v):vm.mem_write(a,(v&0xffffffff).to_bytes(4,'little'))
def read(a):return int.from_bytes(vm.mem_read(a,4),'little')
def string(a):
 out=bytearray()
 for i in range(512):
  b=bytes(vm.mem_read(a+i,1))
  if b==b'\0':return out
  out+=b
 raise AssertionError('Unterminated numeric text')
def text_hash(text):
 n=2166136261
 for b in text:n=((n^b)*16777619)&0xffffffff
 return n
events=[];counts={0x4cc770:5,0x4cc720:6,0x4ccbb0:6}
def endpoint(cpu,a,n,user):
 sp=cpu.reg_read(UC_X86_REG_ESP);args=[read(sp+4+i*4) for i in range(counts[a])]
 if a==0x4ccbb0:width=300+(args[1]&15)
 else:text=string(args[0]);args[0]=text_hash(text);width=len(text)*7
 events.append([a,len(args)]+args+[0]*(8-len(args))+[read(0xe20778)]);write(0xe20778,read(0xe20778)+width)
 cpu.reg_write(UC_X86_REG_EAX,0);cpu.reg_write(UC_X86_REG_EIP,read(sp));cpu.reg_write(UC_X86_REG_ESP,sp+4)
for a in counts:vm.hook_add(UC_HOOK_CODE,endpoint,begin=a,end=a)
def native(p):
 op,value,mode,prefix,suffix,x,y,font,color=p;events.clear();vm.mem_write(0xe34238,b'\xa5'*288);write(0xe20778,258);write(0xd3b504,0x12345678)
 for i,text in enumerate([b'',b' People',b'Prefix',b' / 1000']):vm.mem_write(BASE+i*128,text+b'\0')
 args=[[value,mode,prefix],[mode,value,x,y,font,color],[value,mode,BASE+suffix*128,x,y,font,color],
  [value,mode,BASE+suffix*128,x,y,314,font,color],[value,BASE+prefix*128,BASE+suffix*128,x,y,314,font,color],[44,128,value,x,y,font,color]][op]
 sp=STACK+0xe000;write(sp,STOP)
 for i,v in enumerate(args):write(sp+4+i*4,v)
 saved=[(UC_X86_REG_EBX,0x11111111),(UC_X86_REG_EBP,0x22222222),(UC_X86_REG_ESI,0x33333333),(UC_X86_REG_EDI,0x44444444)]
 for r,v in saved:vm.reg_write(r,v)
 vm.reg_write(UC_X86_REG_ESP,sp);vm.reg_write(UC_X86_REG_EFLAGS,2)
 vm.emu_start([0x4cc5c0,0x4cccb0,0x4cce00,0x4cce90,0x4ccf20,0x4ccc30][op],STOP,count=100000)
 assert vm.reg_read(UC_X86_REG_EIP)==STOP and vm.reg_read(UC_X86_REG_ESP)==sp+4,'Native stack/budget differs'
 assert all(vm.reg_read(r)==v for r,v in saved),'Native saved register differs'
 return {'result':vm.reg_read(UC_X86_REG_EAX) if op==0 else 0,'advance':read(0xe20778),'flag':read(0xd3b504),'events':events.copy(),'state':bytes(vm.mem_read(0xe34238,288)).hex()}
values=[v&0xffffffff for v in [-2147483648,-1000000,-1000,-35,-1,0,1,2,9,10,35,50,99,100,999,1000,999999999,1000000000,2147483647]]
cases=[]
for value,offset,sign in itertools.product(values,[0,1,7],[0,1,3]):cases.append([0,value,offset,sign,0,258,259,335,0])
for value,kind,font in itertools.product(values,[0,1,2,257],[335,871]):cases.append([1,value,kind,0,0,258,259,font,0x12345678])
for op,value,pad,suffix in itertools.product([2,3],values,[0,32,256,0xffffffff],[0,1,3]):cases.append([op,value,pad,0,suffix,258,259,335,0])
for value,prefix,suffix in itertools.product(values,[0,2],[0,1,3]):cases.append([4,value,0,prefix,suffix,258,259,335,0])
for value in values:cases.append([5,value,0,0,0,258,259,335,0])
exe=ROOT/'build/c-only-debug/Debug/restore_numeric_text_verify.exe';process=subprocess.Popen([str(exe),'--oracle-batch'],cwd=ROOT,stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True)
report={'format':1,'original_game_executed':False,'native_sha256':digest,'compiled_C_sha256':hashlib.sha256(exe.read_bytes()).hexdigest(),
 'method':'Original 004cc5c0 integer conversion, 004cccb0 year wrapper, 004ccc30 plural wrapper, and 004cce00/004cce90/004ccf20 numeric composition. Only final text/catalog drawing controlled with deterministic full-width metrics. Complete 256-byte buffer plus guards, draw text/arguments/order, metric/flag state, return and native register/stack ABI compared.',
 'limitations':['Final glyph rendering and message-catalog contents are separate checks. Native digit-count behavior at ten-digit values is preserved rather than replaced with host formatting.']}
checked=0;p=None
try:
 assert json.loads(process.stdout.readline())=={'fields':9}
 for p in cases:
  process.stdin.write(' '.join(map(str,p))+'\n');process.stdin.flush();line=process.stdout.readline();assert line,'C exited: '+process.stderr.read()
  actual=json.loads(line);expected=native(p)
  assert actual==expected,repr({'C':actual,'native':expected})
  checked+=1
 report.update(passed=True,checked=checked)
except Exception as error:report.update(passed=False,checked=checked,case=p,error=str(error),native_eip=hex(vm.reg_read(UC_X86_REG_EIP)))
finally:process.stdin.close();process.wait(timeout=10)
sources=['tests/numeric_text.c','tools/verify_numeric_text_emulation.py','metadata/numeric-text-native.json']+[f'src/unit_{i:05d}.c' for i in [1689,1697,1698,1700,1701,1702]]
report['source_hashes']={s:hashlib.sha256((ROOT/s).read_bytes()).hexdigest() for s in sources}
(ROOT/'metadata/numeric-text-emulation.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report,indent=2));sys.exit(0 if report['passed'] else 1)
