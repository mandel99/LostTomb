"""Differential campaign selector rendering and transitions against original x86."""
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
def hash_string(a):
 v=2166136261
 for i in range(1024):
  c=byte(a+i)
  if not c:return v
  v=((v^c)*16777619)&0xffffffff
 raise AssertionError('Unterminated text')
regions=[(0x6fdc98,4),(0x7027ac,9),(0x605a3c,4),(0x6de515,1),(0xe38ebc,4),(0x5d2a2c,4),(0xe38dc9,2),(0xe38dda,1),(0xe38ea8,1),(0x784f0c,4),(0x704dac,8),(0xd3b568,16),(0xe20768,4),(0xf53f28,4)]
events=[];mission_result=0
counts={0x53e590:3,0x4cc110:2,0x448370:3,0x4cc720:6,0x4ccb30:7,0x4ccfd0:7,0x4ccbb0:6,0x536710:5,0x4cd240:7,0x4cd1a0:5,0x413580:3,0x417dd0:1,0x4181e0:0,0x41f870:0,0x4cb6a0:0}
def endpoint(cpu,a,n,user):
 sp=cpu.reg_read(UC_X86_REG_ESP);args=[read(sp+4+i*4) for i in range(counts[a])];record=args.copy();cleanup=0
 if a==0x4cc720:record[0]=hash_string(args[0])
 elif a==0x448370:record=[hash_string(args[0]),args[2],read(0x784f0c)]
 elif a==0x413580:record=[cpu.reg_read(UC_X86_REG_ECX)]+args;cleanup=12
 elif a==0x417dd0:record=[struct.unpack('<b',bytes([args[0]&255]))[0]&0xffffffff]
 if a==0x53e590:cleanup=12
 events.append([a,len(record)]+record+[0]*(8-len(record))+[read(0x605a3c),byte(0x6de515),read(0xe38ebc),read(0x5d2a2c),byte(0x7027b4),byte(0xe38dca),read(0x704dac),read(0x704db0)])
 if a==0x4cc110:write(0xe20768,BASE+0x4000)
 elif a==0x448370:assert args[2]==200;vm.mem_write(args[1],b'Restore campaign\0')
 elif a==0x417dd0:write(0xe38ea8,mission_result,1)
 elif a==0x4181e0:write(0xe38dda,0x35,1)
 elif a==0x41f870:write(0xe38dda,61,1);write(0xe38dca,0x77,1)
 elif a==0x4cb6a0:write(0xe38dda,48,1);write(0xe38dca,1,1)
 cpu.reg_write(UC_X86_REG_EAX,1);cpu.reg_write(UC_X86_REG_EIP,read(sp));cpu.reg_write(UC_X86_REG_ESP,sp+4+cleanup)
def guard(cpu,a,n,user):
 ranges=[(0x41ba00,0x41ba1a),(0x41bad0,0x41bad9),(0x41bb10,0x41bb29),(0x41bb30,0x41bb68),(0x41bb70,0x41bdf2),(0x41be10,0x41c2cd),(0x41c2d0,0x41c30b),(0x41c310,0x41c34f),(0x41c350,0x41c371),(0x41f650,0x41f675)]
 if a not in counts and not any(lo<=a and a+n<=hi for lo,hi in ranges):raise RuntimeError('Native campaign selector escaped at '+hex(a))
vm.hook_add(UC_HOOK_BLOCK,guard)
for a in counts:vm.hook_add(UC_HOOK_CODE,endpoint,begin=a,end=a)
def native(p):
 global mission_result
 op,mode,selected,gate,hover,w,h,ox,oy,variant,mission_result,screen,pending,clicked=p;events.clear()
 for a,n in regions:vm.mem_write(a,b'\xa5'*n)
 vals=[(0x6fdc98,mode),(0x7027ac,selected),(0x7027b0,gate),(0x7056c8,hover),(0xe38db0,w),(0xe38dac,h),(0xe38da8,ox),(0xe38da4,oy),(0xe38edc,variant),(0x605a3c,pending),(0xd3b198,clicked),
 (0x6fdc54,BASE),(0x784f0c,258),(0xf55c94,35000),(0xf57314,46000),(0xf55cbc,37000),(0xf56f8c,1005),(0xf56f94,335),(0xf56fa4,871),(0xf56fb4,18765),(0xf56fbc,22345)]
 for a,v in vals:write(a,v)
 write(0xe38dda,screen,1);vm.mem_write(BASE+0x4000,b'[player_name] campaign\0');sp=STACK+0xe000;write(sp,STOP)
 saved=[(UC_X86_REG_EBX,0x11111111),(UC_X86_REG_EBP,0x22222222),(UC_X86_REG_ESI,0x33333333),(UC_X86_REG_EDI,0x44444444)]
 for r,v in saved:vm.reg_write(r,v)
 vm.reg_write(UC_X86_REG_ESP,sp);vm.reg_write(UC_X86_REG_EFLAGS,2)
 vm.emu_start([0x41be10,0x41c2d0,0x41bb30,0x41bb10,0x41bb70,0x41c310,0x41c350,0x41f650][op],STOP,count=100000)
 assert vm.reg_read(UC_X86_REG_EIP)==STOP and vm.reg_read(UC_X86_REG_ESP)==sp+4,'Native stack/budget differs'
 assert all(vm.reg_read(r)==v for r,v in saved),'Native saved registers differ'
 return {'result':vm.reg_read(UC_X86_REG_EAX) if op==2 else 0,'state':b''.join(bytes(vm.mem_read(a,n)) for a,n in regions).hex(),'events':events.copy()}
cases=[]
for mode,selected,gate,hover,(w,h) in itertools.product([0,1,2,256],[0,1,4,5,6,7,8,9,258,0xffffffff],[0,4,8,258],[0,1,9,10,11,256],[(640,480),(800,600),(1024,768)]):
 cases.append([0,mode,selected,gate,hover,w,h,(w-640)//2,(h-480)//2,233,0,23,0,258])
for mode,selected,gate,variant,result,(w,h) in itertools.product([0,1,256],[0,1,2,3,4,5,8,258,0xffffffff],[0,4,258],[232,233,0xffffffff],[0,127],[(640,480),(1024,768),(479,285)]):
 cases.append([4,mode,selected,gate,0,w,h,192,144,variant,result,23,0x12345678,258])
for op,mode,selected,gate,screen,pending in itertools.product([1,2,3,5,6,7],[0,1,256],[0,4,258],[0,4,258],[0,23,61],[0,256]):
 cases.append([op,mode,selected,gate,0,1024,768,192,144,233,0,screen,pending,258])
exe=ROOT/'build/c-only-debug/Debug/restore_campaign_menu_verify.exe'
process=subprocess.Popen([str(exe),'--oracle-batch'],cwd=ROOT,stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True)
report={'format':1,'original_game_executed':False,'native_sha256':digest,'method':'Bounded original selector foreground/background, actual predicates, choose/confirm/tab/back callbacks. Final drawing, text expansion, movie playback, mission loading and nested screen entry endpoints controlled; all calls, state ordering and native ABI compared.',
 'compiled_C_sha256':hashlib.sha256(exe.read_bytes()).hexdigest(),'limitations':['Checks renderer arguments and transitions, not actual movie/mission preparation or later briefing and custom-selection controllers. Final pixel output is checked separately.']}
checked=0;p=None
try:
 assert json.loads(process.stdout.readline())=={'fields':14}
 for p in cases:
  process.stdin.write(' '.join(map(str,p))+'\n');process.stdin.flush();line=process.stdout.readline();assert line,'C exited: '+process.stderr.read()
  actual=json.loads(line);expected=native(p);assert actual==expected,repr({'C':actual,'native':expected});checked+=1
 report.update(passed=True,checked=checked)
except Exception as error:report.update(passed=False,checked=checked,case=p,error=str(error),native_eip=hex(vm.reg_read(UC_X86_REG_EIP)))
finally:process.stdin.close();process.wait(timeout=10)
sources=['include/campaign_menu.h','include/campaign_index.h','include/video_player.h','runtime/campaign_menu.c','src/unit_00255.c','src/unit_00259.c','src/unit_00261.c','src/unit_00262.c','src/unit_00263.c','src/unit_00308.c','tests/campaign_menu.c','tools/verify_campaign_menu_emulation.py']
report['source_hashes']={s:hashlib.sha256((ROOT/s).read_bytes()).hexdigest() for s in sources}
(ROOT/'metadata/campaign-menu-emulation.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report,indent=2));sys.exit(0 if report['passed'] else 1)
