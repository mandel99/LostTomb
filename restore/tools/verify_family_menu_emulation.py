"""Verify family-menu control flow against native instructions without game/UI execution."""
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
 raise AssertionError('Unterminated endpoint string')
def hash_string(a):
 value=2166136261
 for c in string(a):value=((value^c)*16777619)&0xffffffff
 return value
descriptor=base+u32(data,opt+104)
while read(descriptor+12):
 names=base+(read(descriptor) or read(descriptor+16));iat=base+read(descriptor+16);i=0
 while read(names+i*4):
  rva=read(names+i*4)
  if not rva&0x80000000 and string(base+rva+2)==b'CreateDirectoryA':write(iat+i*4,BASE+0xf000)
  i+=1
 descriptor+=20
events=[];files=target=available=0
def fill_list():
 write(0xeadfac,BASE+0x1000);write(0xeadfb8,BASE+0x2000);write(0xeadfb0,files)
 for i in range(files):write(BASE+0x1000+i*4,i*32);vm.mem_write(BASE+0x2000+i*32,(b'Restore' if i==target else ('Family%03u'%i).encode())+b'\0')
endpoints={0x41ae40:(1,1,1),0x41b8a0:(2,1,1),0x41b680:(3,1,1),0x561a0a:(4,2,0),0x561994:(5,1,0),BASE+0xf000:(6,2,2),
 0x516b50:(7,1,0),0x4cc500:(8,1,0),0x4cc3c0:(9,0,0),0x4cc3e0:(10,0,0),0x4cb6a0:(11,0,0),0x51f780:(12,1,0),0x4ce3a0:(13,1,0),0x4cde60:(14,2,0),0x4cde40:(15,2,0)}
def endpoint(cpu,a,n,user):
 sp=cpu.reg_read(UC_X86_REG_ESP);kind,count,cleanup=endpoints[a];args=[read(sp+4+i*4) for i in range(count)];result=1
 if kind in [1,2,3]:
  receiver=cpu.reg_read(UC_X86_REG_ECX);assert receiver==0x6fdd08;events.append([kind,receiver,hash_string(args[0]),0])
 elif kind==4:
  assert string(args[1])==b'rb';events.append([4,hash_string(args[0]),hash_string(args[1]),available]);result=0x31000000 if available else 0
 elif kind==5:assert args==[0x31000000];events.append([5,0,0,0])
 elif kind==6:assert args[1]==0;events.append([6,hash_string(args[0]),0,0])
 elif kind==7:assert string(args[0])==b'Save\\*.dat';events.append([7,files,0,0]);fill_list()
 elif kind==8:events.append([8,args[0],0,0]);write(0xe20990,args[0])
 elif kind in [9,10]:events.append([kind,0,0,0])
 elif kind==11:events.append([11,0,0,0]);write(0xe38dda,0x30,1)
 elif kind==12:assert args==[1]
 elif kind==13:
  value=string(args[0]);dot=value.rfind(b'.')
  if dot>=0:write(args[0]+dot,0,1)
 else:
  x=struct.unpack('<i',struct.pack('<I',args[0]))[0];y=struct.unpack('<i',struct.pack('<I',args[1]))[0]
  result=int(x*100/y) if kind==14 else int(x*y/100)
 cpu.reg_write(UC_X86_REG_EAX,result&0xffffffff);cpu.reg_write(UC_X86_REG_EIP,read(sp));cpu.reg_write(UC_X86_REG_ESP,sp+4+cleanup*4)
def guard(cpu,a,n,user):
 if a not in endpoints and not any(lo<=a and a+n<=hi for lo,hi in [(0x516890,0x516dd0),(0x41acd0,0x41ad20),(0x4264d0,0x426530)]):raise RuntimeError('Native family menu escaped at '+hex(a))
vm.hook_add(UC_HOOK_BLOCK,guard)
for a in endpoints:vm.hook_add(UC_HOOK_CODE,endpoint,begin=a,end=a)
regions=[(0xe38e74,32),(0xeadfcc,4),(0xeadfc8,4),(0xe38dda,1),(0xe38dca,1),(0xd3b1b3,1),(0xe20998,4),(0xe94dc0,4),(0xd3b198,4),(0xe20990,4),(0x705704,24),(0xeadda0,520)]
def native(p):
 global files,target,available
 op,files,target,selected,action,scroll,available,name_kind,mx,my=p;events.clear();vm.mem_write(BASE,bytes(0x10000));fill_list()
 vm.mem_write(0xe38e74,b'\xa5'*32);vm.mem_write(0xe38e74,[b'Restore',b'  Restore  ',b'   ',b''][name_kind]+b'\0')
 vm.mem_write(0xeadda0,b'\xa5'*520);vm.mem_write(0x705704,b'\xa5'*24)
 for a,v in [(0xeadfcc,selected),(0xd3b198,action),(0xeadfc8,scroll),(0xe20998,0x12345678),(0xe94dc0,0x12345678),(0xe20990,7),(0xe38da8,192),(0xe38da4,144),(0xe38e34,mx),(0xe38e30,my)]:write(a,v)
 for a,v in [(0xe38dda,54),(0xe38dca,0),(0xd3b1b3,0x77),(0xe38e00,1)]:write(a,v,1)
 sp=STACK+0xe000;vm.mem_write(STACK,bytes(0x10000));write(sp,STOP)
 saved=[(UC_X86_REG_EBX,0x11111111),(UC_X86_REG_EBP,0x22222222),(UC_X86_REG_ESI,0x33333333),(UC_X86_REG_EDI,0x44444444)]
 for r,v in saved:vm.reg_write(r,v)
 vm.reg_write(UC_X86_REG_ESP,sp);vm.reg_write(UC_X86_REG_EFLAGS,2)
 vm.emu_start([0x516c80,0x516bb0,0x516ab0,0x516990,0x516c10,0x516890,0x5168e0,0x516c60][op],STOP,count=100000)
 assert vm.reg_read(UC_X86_REG_EIP)==STOP and vm.reg_read(UC_X86_REG_ESP)==sp+4,'Native stack/budget differs'
 assert all(vm.reg_read(r)==v for r,v in saved),'Callee-saved register differs'
 return {'state':b''.join(bytes(vm.mem_read(a,n)) for a,n in regions).hex(),'events':events.copy()}
cases=[]
for op,count,available,name,selected in itertools.product([0,1,2,3,4,7],[0,1,3,13,260],[0,1],range(4),[0,2,259,0xffffffff]):
 cases.append([op,count,count-1 if count else 0,selected,selected,0,available,name,656,300])
for op,count,scroll,action in itertools.product([5],[0,1,12,13,20,260],[0,1,8,248,259],[0,1,2,256]):cases.append([op,count,0,0,action,scroll,0,0,656,300])
for count,mx,my in itertools.product([0,12,13,260],[655,656,671,688,689],[288,289,300,419,444,445]):cases.append([6,count,0,0,0,0,0,0,mx,my])
exe=ROOT/'build/c-only-debug/Debug/restore_family_menu_verify.exe'
process=subprocess.Popen([str(exe),'--oracle-batch'],cwd=ROOT,stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True)
report={'format':1,'original_game_executed':False,'native_sha256':digest,'method':'Bounded original family creation/selection/name entry, scroll/drag and modal setup; campaign parsing, persistence, enumeration, text caret, game-options transition and directory creation are controlled endpoints.',
 'compiled_C_sha256':hashlib.sha256(exe.read_bytes()).hexdigest(),'limitations':['Native profile/campaign/file-list implementation has separate comparators. Full renderer, delete execution and subsequent game-options controller are not proved by this suite.']}
checked=0;p=None
try:
 assert json.loads(process.stdout.readline())=={'fields':10}
 for p in cases:
  process.stdin.write(' '.join(map(str,p))+'\n');process.stdin.flush();line=process.stdout.readline();assert line,'C exited: '+process.stderr.read()
  actual=json.loads(line);expected=native(p)
  for key in expected:assert actual[key]==expected[key],f'{key} differs: '+str([actual[key],expected[key]])
  checked+=1
 report.update(passed=True,checked=checked)
except Exception as error:report.update(passed=False,checked=checked,case=p,error=str(error),native_eip=hex(vm.reg_read(UC_X86_REG_EIP)))
finally:process.stdin.close();process.wait(timeout=10)
sources=['include/family_menu.h','runtime/family_menu.c','src/unit_00244.c','src/unit_00418.c','src/unit_02279.c','src/unit_02280.c','src/unit_02281.c','src/unit_02282.c','src/unit_02283.c','src/unit_02284.c','src/unit_02286.c','src/unit_02287.c','tests/family_menu.c','tools/verify_family_menu_emulation.py']
report['source_hashes']={s:hashlib.sha256((ROOT/s).read_bytes()).hexdigest() for s in sources}
(ROOT/'metadata/family-menu-emulation.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report,indent=2));sys.exit(0 if report['passed'] else 1)
