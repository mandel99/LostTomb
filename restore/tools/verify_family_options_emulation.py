"""Compare all native family-option actions and completion predicate with independent C."""
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
STOP=0x33000000;STACK=0x34000000;BASE=0x30000000
for a in [STOP,STACK]:vm.mem_map(a,0x10000)
def write(a,v,n=4):vm.mem_write(a,(v&((1<<(8*n))-1)).to_bytes(n,'little'))
def read(a):return int.from_bytes(vm.mem_read(a,4),'little')
def byte(a):return int.from_bytes(vm.mem_read(a,1),'little')
regions=[(0x605a3c,4),(0x6de515,1),(0x7027ac,8),(0x6fdc98,4),(0x704dac,8),(0xe38ebc,4),(0xe38dda,1),(0xe38dca,1),(0xd3b1bc,4),(0xe20764,8),(0xeadfb0,4),(0xeadfc8,8),(0xd3b198,4),(0xd3b1b3,1),(0xe20998,4),(0xe94dc0,4),(0xe38e74,32)]
events=[];answer=files=0
counts={0x55f561:1,0x4d5100:1,0x41f870:0,0x402450:0,0x4cc110:2,0x4cc160:2,0x538cd0:3,0x516b50:1,0x516ab0:0,0x4cc500:1,0x4cc3c0:0,0x4cc3e0:0}
def event(a,args):
 events.append([a,len(args)]+args+[0]*(5-len(args))+[read(0x605a3c),byte(0x6de515),read(0x704dac),read(0x704db0),byte(0xe38dda)])
def endpoint(cpu,a,n,user):
 sp=cpu.reg_read(UC_X86_REG_ESP);args=[read(sp+4+i*4) for i in range(counts[a])];result=0
 if a==0x55f561:args=[cpu.reg_read(UC_X86_REG_ECX)]+args;assert args==[0xe92bf8,0]
 event(a,args)
 if a==0x4d5100:assert args==[0x700998];write(0xe38dda,0x55,1);write(0xe38dca,0x77,1)
 elif a==0x41f870:write(0xe38dda,61,1);write(0xe38dca,0x77,1)
 elif a==0x402450:write(0xe38dda,10,1);write(0xd3b1bc,0x12345678)
 elif a==0x4cc110:assert args==[5,141];write(0xe20768,BASE+0x1000)
 elif a==0x4cc160:assert args==[5,142];write(0xe20764,BASE+0x2000)
 elif a==0x538cd0:assert args==[BASE+0x1000,BASE+0x2000,4];result=answer
 elif a==0x516b50:assert args==[0x5f80f0];write(0xeadfb0,files)
 elif a==0x516ab0:
  if files:vm.mem_write(0xe38e74,b'Restore\0')
  write(0xe38dca,1,1)
 cpu.reg_write(UC_X86_REG_EAX,result);cpu.reg_write(UC_X86_REG_EIP,read(sp));cpu.reg_write(UC_X86_REG_ESP,sp+4+(4 if a==0x55f561 else 0))
def guard(cpu,a,n,user):
 if a not in counts and not any(lo<=a and a+n<=hi for lo,hi in [(0x4cb800,0x4cb980),(0x41ba80,0x41baa2),(0x516bb0,0x516c52)]):raise RuntimeError('Native family option escaped at '+hex(a))
vm.hook_add(UC_HOOK_BLOCK,guard)
for a in counts:vm.hook_add(UC_HOOK_CODE,endpoint,begin=a,end=a)
def native(p):
 global answer,files
 op,flags,count,completed,answer,first,gate,seed,files,selected=p;events.clear();vm.mem_write(0x6fdd08,bytes(6404));write(0x6ff608,count)
 if completed<count and completed<100:write(0x6fdd08+completed*64+60,0x80,1)
 for a,n in regions:vm.mem_write(a,b'\xa5'*n)
 for bit,a in enumerate([0xd3b188,0xd3b189,0xd3b18a,0xd3b18b]):write(a,bool(flags&(1<<bit)),1)
 for a,v in [(0x605a3c,seed),(0x7027ac,first),(0x7027b0,gate),(0xeadfcc,selected)]:write(a,v)
 write(0xe38dda,48,1);write(0xe38dca,0x44,1);sp=STACK+0xe000;write(sp,STOP)
 saved=[(UC_X86_REG_EBX,0x11111111),(UC_X86_REG_EBP,0x22222222),(UC_X86_REG_ESI,0x33333333),(UC_X86_REG_EDI,0x44444444)]
 for r,v in saved:vm.reg_write(r,v)
 vm.reg_write(UC_X86_REG_ESP,sp);vm.reg_write(UC_X86_REG_ECX,0x6fdd08);vm.reg_write(UC_X86_REG_EFLAGS,2)
 vm.emu_start([0x4cb800,0x4cb860,0x4cb910,0x4cb940,0x4cb970,0x41ba80][op],STOP,count=100000)
 assert vm.reg_read(UC_X86_REG_EIP)==STOP and vm.reg_read(UC_X86_REG_ESP)==sp+4,'Native stack/budget differs'
 assert all(vm.reg_read(r)==v for r,v in saved),'Native saved registers differ'
 return {'result':vm.reg_read(UC_X86_REG_EAX)&255 if op==5 else 0,'state':b''.join(bytes(vm.mem_read(a,n)) for a,n in regions).hex(),'events':events.copy()}
cases=[[op,flags,count,done,answer,258,260,0x12345678,0,0] for op,flags,count,done,answer in itertools.product(range(4),range(16),[0,1,100],[0,99,0xffffffff],[0,6,7,263])]
cases.extend([4,0,0,0,0,258,260,0x12345678,files,selected] for files,selected in itertools.product([0,1,260],[0,259,0xffffffff]))
cases.extend([5,0,count,done,0,258,260,0x12345678,0,0] for count,done in itertools.product([0,1,100,0xffffffff],[0,1,99,0xffffffff]))
exe=ROOT/'build/c-only-debug/Debug/restore_family_options_verify.exe'
process=subprocess.Popen([str(exe),'--oracle-batch'],cwd=ROOT,stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True)
report={'format':1,'original_game_executed':False,'native_sha256':digest,'method':'Native five family actions, actual completion predicate and registry-return control flow; nested loaders, selection screens, confirmation, label lookup and sound endpoints controlled. Call arguments/state ordering, final bytes, AL predicate and native ABI compared.',
 'compiled_C_sha256':hashlib.sha256(exe.read_bytes()).hexdigest(),'limitations':['This verifies transitions and prerequisites; nested game loading, actual confirmation rendering and later screen controllers are separate checks.']}
checked=0;p=None
try:
 assert json.loads(process.stdout.readline())=={'fields':10}
 for p in cases:
  process.stdin.write(' '.join(map(str,p))+'\n');process.stdin.flush();line=process.stdout.readline();assert line,'C exited: '+process.stderr.read()
  actual=json.loads(line);expected=native(p);assert actual==expected,repr({'C':actual,'native':expected});checked+=1
 report.update(passed=True,checked=checked)
except Exception as error:report.update(passed=False,checked=checked,case=p,error=str(error),native_eip=hex(vm.reg_read(UC_X86_REG_EIP)))
finally:process.stdin.close();process.wait(timeout=10)
sources=['include/family_menu.h','include/campaign_index.h','runtime/family_options.c','src/unit_00257.c','src/unit_01655.c','tests/family_options.c','tools/verify_family_options_emulation.py']
report['source_hashes']={s:hashlib.sha256((ROOT/s).read_bytes()).hexdigest() for s in sources}
(ROOT/'metadata/family-options-emulation.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report,indent=2));sys.exit(0 if report['passed'] else 1)
