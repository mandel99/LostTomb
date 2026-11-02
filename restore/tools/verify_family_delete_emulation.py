"""Verify deletion order, paths, object lifetime and SEH restoration against native code."""
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
for a in [0,BASE,STOP,STACK]:vm.mem_map(a,0x10000 if a else 0x1000)
def write(a,v,n=4):vm.mem_write(a,(v&((1<<(8*n))-1)).to_bytes(n,'little'))
def read(a):return int.from_bytes(vm.mem_read(a,4),'little')
def string(a):
 out=bytearray()
 for i in range(1024):
  c=bytes(vm.mem_read(a+i,1))
  if c==b'\0':return bytes(out)
  out+=c
 raise AssertionError('Unterminated deletion path')
def hashed(s):
 v=2166136261
 for c in s:v=((v^c)*16777619)&0xffffffff
 return v
events=[];files=name_length=allocation=failures=0
entries={0x562bdf:('unlink',1,False),BASE+0xf000:('rmdir',1,True),0x561a2b:('free',1,False),0x41c740:('enumerate',2,True),0x516bb0:('reload',0,False)}
write(0x56f064,BASE+0xf000)
def endpoint(cpu,a,n,user):
 sp=cpu.reg_read(UC_X86_REG_ESP);kind,count,callee_cleanup=entries[a];args=[read(sp+4+i*4) for i in range(count)];result=0
 if kind=='unlink':events.append([1,hashed(string(args[0])),0]);result=-1 if failures else 0
 elif kind=='rmdir':events.append([2,hashed(string(args[0])),0]);result=0 if failures else 1
 elif kind=='free':events.append([3,args[0],0])
 elif kind=='enumerate':
  self=cpu.reg_read(UC_X86_REG_ECX);assert args[1]==0 and [read(self+i*4) for i in range(7)]==[0x56f548,0,0,0,0,0,0]
  assert string(args[0])==string(0xeadea4)+b'\\*.sav';events.append([4,hashed(string(args[0])),files]);write(self+8,files)
  if files or allocation:write(self+4,BASE+0x1000);write(self+16,BASE+0x3000)
  for i in range(files):
   write(BASE+0x1000+i*4,i*256);name=('Save%03u'%i).encode()+b'X'*(name_length-7)+b'.sav\0';vm.mem_write(BASE+0x3000+i*256,name)
  result=1
 else:events.append([5,0,0]);write(0xe38dda,47,1)
 cpu.reg_write(UC_X86_REG_EAX,result&0xffffffff);cpu.reg_write(UC_X86_REG_EIP,read(sp));cpu.reg_write(UC_X86_REG_ESP,sp+4+(4*count if callee_cleanup else 0))
def guard(cpu,a,n,user):
 if a not in entries and not any(lo<=a and a+n<=hi for lo,hi in [(0x516dd0,0x516f90),(0x5169c0,0x516ab0),(0x41c6d0,0x41c6f0),(0x41c710,0x41c740)]):raise RuntimeError('Native family deletion escaped at '+hex(a))
vm.hook_add(UC_HOOK_BLOCK,guard)
for a in entries:vm.hook_add(UC_HOOK_CODE,endpoint,begin=a,end=a)
def native(p):
 global files,name_length,allocation,failures
 accepted,files,family_length,name_length,allocation,failures,mission_flag=p;events.clear();vm.mem_write(BASE,bytes(0x10000))
 vm.mem_write(0xe38e74,bytes(32));vm.mem_write(0xe38e74,bytes(65+i%26 for i in range(family_length)))
 vm.mem_write(0xeadda0,b'\xa5'*520);write(0xd3b439,accepted,1);write(0xe38dca,0x44,1);write(0xe38dda,63,1);write(0x704db0,mission_flag)
 vm.mem_write(STACK,bytes(0x10000));sp=STACK+0xe000;write(sp,STOP);write(0,0x12345678)
 saved=[(UC_X86_REG_EBX,0x11111111),(UC_X86_REG_EBP,0x22222222),(UC_X86_REG_ESI,0x33333333),(UC_X86_REG_EDI,0x44444444)]
 for r,v in saved:vm.reg_write(r,v)
 vm.reg_write(UC_X86_REG_ESP,sp);vm.reg_write(UC_X86_REG_EFLAGS,2)
 vm.emu_start(0x516dd0,STOP,count=500000)
 assert vm.reg_read(UC_X86_REG_EIP)==STOP and vm.reg_read(UC_X86_REG_ESP)==sp+4,'Native stack/budget differs'
 assert all(vm.reg_read(r)==v for r,v in saved) and read(0)==0x12345678,'Native saved registers/SEH differ'
 return {'mode':int.from_bytes(vm.mem_read(0xe38dda,1),'little'),'dirty':int.from_bytes(vm.mem_read(0xe38dca,1),'little'),'events':events.copy()}
cases=[list(p) for p in itertools.product([0,1,2,255],[0,1,3,100],[0,7,31],[7,100,200,220],[0,1],[0,1],[0,1]) if 5+p[2]+1+p[3]+4<260]
exe=ROOT/'build/c-only-debug/Debug/restore_family_delete_verify.exe'
process=subprocess.Popen([str(exe),'--oracle-batch'],cwd=ROOT,stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True)
report={'format':1,'original_game_executed':False,'native_sha256':digest,'method':'Native deletion callback, path builders, file-list constructor/destructor and SEH chain; enumeration, unlink, directory removal, deallocation and registry reload are controlled endpoints.',
 'compiled_C_sha256':hashlib.sha256(exe.read_bytes()).hexdigest(),'safety_difference':'C skips a file path exceeding the native 260-byte stack buffer; this malformed case is a separate C check.',
 'limitations':['Exact paths/call order/lifetime are compared. Filesystem deletion and the complete modal/registry loop are separate integration checks.']}
checked=0;p=None
try:
 assert json.loads(process.stdout.readline())=={'fields':7}
 for p in cases:
  process.stdin.write(' '.join(map(str,p))+'\n');process.stdin.flush();line=process.stdout.readline();assert line,'C exited: '+process.stderr.read()
  actual=json.loads(line);expected=native(p);assert actual==expected,repr({'C':actual,'native':expected});checked+=1
 report.update(passed=True,checked=checked)
except Exception as error:report.update(passed=False,checked=checked,case=p,error=str(error),native_eip=hex(vm.reg_read(UC_X86_REG_EIP)))
finally:process.stdin.close();process.wait(timeout=10)
sources=['include/family_menu.h','runtime/family_delete.c','include/file_list.h','src/unit_02282.c','src/unit_02283.c','tests/family_delete.c','tools/verify_family_delete_emulation.py']
report['source_hashes']={s:hashlib.sha256((ROOT/s).read_bytes()).hexdigest() for s in sources}
(ROOT/'metadata/family-delete-emulation.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report,indent=2));sys.exit(0 if report['passed'] else 1)
