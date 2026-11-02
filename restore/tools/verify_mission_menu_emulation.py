"""Compare mission-list receivers, renderer and complete local controller with x86."""
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
  c=byte(a+i)
  if not c:return bytes(out)
  out.append(c)
 raise AssertionError('Unterminated text: '+hex(a))
def hash_string(a):
 value=2166136261
 for c in string(a):value=((value^c)*16777619)&0xffffffff
 return value
regions=[(0x704dac,24),(0xd3b198,4),(0xd3b1b3,1),(0xd3b1c0,1),(0xe38dc9,2),(0xe38dda,1),(0xe38dd6,1),(0xe38df8,1),(0xe38e04,1),(0xe38ea8,1),(0x7027b4,1),(0xd3b568,16),(0x784f0c,4),(0xe20764,8),(0xe20778,4),(0x7056c4,8),(0xe39080,4),(0xe37078,64),(0xea187c,4)]
counts={0x53e590:3,0x536a30:4,0x4cc110:2,0x4cc160:2,0x448370:3,0x4cc720:6,0x4cc770:5,0x4ccb30:7,0x4ccbb0:6,0x4cccb0:6,0x4ccfd0:7,0x4cce00:7,0x4ccf20:8,0x4ccd60:7,0x4cd1a0:5,0x4cd240:7,
 0x425730:4,0x425990:4,0x426450:4,0x4d6740:5,0x4261d0:4,0x51f780:0,0x4cde60:2,0x4cde40:2,0x41e3b0:0,0x55f561:1,0x41f650:0,0x4181e0:0,0x5405b0:2,
 0x540c90:0,0x540ed0:0,0x541160:0,0x4bc720:0,0x40f8f0:3,0x4ce3a0:1,0x4ce3c0:1,0x4cbea0:3,0x4d5820:2,0x4d58b0:2,0x41c740:2,0x41ca10:1}
events=[];mask=metric=small_calls=big_calls=0
def endpoint(cpu,a,n,user):
 global small_calls,big_calls
 sp=cpu.reg_read(UC_X86_REG_ESP);args=[read(sp+4+i*4) for i in range(counts[a])];record=args.copy();result=1;cleanup=0
 if a==0x51f780:record=None
 elif a==0x41e3b0:record=None;result=metric
 elif a in [0x4cde60,0x4cde40]:
  x,y=[struct.unpack('<i',struct.pack('<I',v))[0] for v in args];record=None;result=int(x*100/y) if a==0x4cde60 else int(x*y/100)
 elif a in [0x4cc720,0x4cc770,0x4ce3a0,0x4ce3c0,0x4d5820]:record[0]=hash_string(args[0])
 elif a==0x448370:record=[hash_string(args[0]),args[2],read(0x784f0c)]
 elif a==0x5405b0:record[1]&=255 # Native callee consumes a byte; upper stack bytes are not an argument.
 elif a==0x4ccf20:record[1]=hash_string(args[1]);record[2]=hash_string(args[2])
 elif a==0x4ccd60:record[0]=hash_string(args[0]);record[1]=hash_string(args[1])
 elif a in [0x55f561,0x41c740,0x41ca10]:record=[cpu.reg_read(UC_X86_REG_ECX)]+args;cleanup=counts[a]*4
 if a==0x53e590:cleanup=12
 if record is not None:events.append([a,len(record)]+record+[0]*(8-len(record))+[read(0x704db0),read(0x704dbc),read(0xd3b198),byte(0xe38dca),byte(0xe38dda),byte(0x7027b4)])
 if a==0x4cc110:write(0xe20768,BASE+0x4000)
 elif a==0x4cc160:write(0xe20764,BASE+0x4100)
 elif a==0x448370:assert args[2]==200;vm.mem_write(args[1],b'Restore missions\0')
 elif a in [0x4ccb30,0x4ccbb0,0x4cce00,0x4ccf20,0x4ccd60]:write(0xe20778,read(0xe20778)+17)
 elif a==0x425990:result=(mask>>small_calls)&1;small_calls+=1
 elif a==0x426450:result=(mask>>8)&255
 elif a==0x4d6740:result=256+args[4]
 elif a==0x4261d0:result=(mask>>(4+big_calls))&1;big_calls+=1
 elif a==0x41f650:write(0xe38dda,48,1);write(0xe38dca,1,1)
 elif a==0x4181e0:write(0xe38dda,53,1)
 elif a==0x5405b0:
  if mask&(1<<20):write(0x7027b4,0,1)
  write(0xe38dda,0x55,1);write(0xe38dca,0x77,1)
 elif a==0x40f8f0:vm.mem_write(args[0],bytes([args[2]&255])*args[1])
 elif a==0x4ce3a0:
  c=byte(args[0])
  if 97<=c<=122:write(args[0],c-32,1)
 elif a==0x4d5820:write(0xe38dca,0x77,1)
 elif a==0x4d58b0:write(0xe38ea8,0xee,1);write(0xe38dca,0x77,1)
 cpu.reg_write(UC_X86_REG_EAX,result&0xffffffff);cpu.reg_write(UC_X86_REG_EIP,read(sp));cpu.reg_write(UC_X86_REG_ESP,sp+4+cleanup)
def guard(cpu,a,n,user):
 if a in [0x4dd8c3,0x4dd162,0x4d806f]:cpu.emu_stop();return
 ranges=[(0x402e50,0x402edd),(0x41df70,0x41e26b),(0x41e470,0x41f8ae),(0x4dcfca,0x4dd162),(0x4d801c,0x4d806f)]
 if a not in counts and not any(lo<=a and a+n<=hi for lo,hi in ranges):raise RuntimeError('Native mission menu escaped at '+hex(a))
vm.hook_add(UC_HOOK_BLOCK,guard)
for a in counts:vm.hook_add(UC_HOOK_CODE,endpoint,begin=a,end=a)
def native(p):
 global mask,metric,small_calls,big_calls
 op,kind,files,scroll,selected,hover,w,h,flags,thumb,map_size,metric,year,towns,mask,clicked,difficulty,screen=p
 events.clear();small_calls=big_calls=0;vm.mem_write(BASE,bytes(0x10000));vm.mem_write(0x7848d0,bytes(0x2000))
 for a,n in regions:vm.mem_write(a,b'\xa5'*n)
 ox=(w-640)//2;oy=(h-480)//2
 vals=[(0x704db0,kind),(0x704db4,files),(0x704db8,scroll),(0x704dbc,selected),(0x7056c8,hover),(0x7056c4,hover),(0xe38db0,w),(0xe38dac,h),(0xe38da8,ox),(0xe38da4,oy),
 (0x704dac,3 if flags&2 else 2),(0x605a3c,1 if flags&1 else flags&256),(0x784904,map_size),(0x784f0c,258),(0x785d70,towns),(0x784c24,metric),(0x784c28,metric+1),(0x784c2c,metric+2),(0x784c30,metric+3),(0x784c34,metric),(0x784c38,258),
 (0x784cb0,258),(0xe39080,difficulty),(0xd3b198,clicked),(0xe38e34,ox+clicked),(0xe38e30,oy+difficulty),(0xeaac28,0),(0x7848f8,1234),(0x700994,files),
 (0xf55c2c,31000),(0xf55c94,35000),(0xf55cb4,36000),(0xf55cbc,37000),(0xf57314,46000),(0xf56494,49000),(0xf56f8c,1005),(0xf56f94,335),(0xf56f9c,871),(0xf56fa4,999),(0xf56fb4,18765)]
 for a,v in vals:write(a,v)
 for a,v in [(0xe38dca,bool(flags&4)),(0x784c3c,bool(flags&32)),(0x784c3d,bool(flags&32)),(0x784c3f,bool(flags&32)),(0xe38dda,screen),(0xe38ea8,1),(0xe38df8,bool(mask&(1<<17))),(0xe38e00,bool(mask&(1<<16)))]:write(a,v,1)
 for a,v in [(0x784b5e,thumb),(0x784b60,256 if flags&8 else 0),(0x7848d8,year),(0x784de0,258),(0x784de2,0xffff),(0x784de4,0)]:write(a,v,2)
 for i in range(5):write(0x7848e2+i*2,(flags>>(9+i*2))&3,2)
 for i in range(towns):write(0x785d74+i*124+2,1,1)
 for a in [0x784c48,0x784c50,0x784c58]:write(a,flags&16)
 for a in [0x784c4c,0x784c54,0x784c5c]:write(a,metric*100+501)
 for a,s in [(0xe37078,b'Map000.map'),(0x784914,b'Mission title'),(BASE+0x4000,b'[player_name] missions'),(BASE+0x4100,b'Secondary')]:vm.mem_write(a,s+b'\0')
 for i in range(80):
  write(BASE+0x1000+i*4,i*32);vm.mem_write(BASE+0x5000+i*32,('map%03u.map'%i).encode()+b'\0');vm.mem_write(0x6ff60c+i*50,('Mission%03u'%i).encode()+b'\0');write(0x704c1c+i*4,79-i);write(BASE+0x201c+i*4,79-i)
 for a in [BASE+0x2000,0x704c00]:write(a+8,files);write(a+4,BASE+0x1000);write(a+16,BASE+0x5000)
 for i in range(3):
  a=0x7009dc+i*76
  for offset,value in [(0,1200 if flags&64 else 0),(40,258),(44,259),(48,260),(52,261),(56,262),(60,1234),(64,2)]:write(a+offset,value)
 def execute(entry,args=(),receiver=0x704c00):
  sp=STACK+0xe000;write(sp,STOP)
  for i,value in enumerate(args):write(sp+4+i*4,value)
  saved=[(UC_X86_REG_EBX,1 if op==16 else 0x11111111),(UC_X86_REG_EBP,0x22222222),(UC_X86_REG_ESI,0x33333333),(UC_X86_REG_EDI,2 if op==15 else 0x44444444)]
  for r,v in saved:vm.reg_write(r,v)
  vm.reg_write(UC_X86_REG_ESP,sp);vm.reg_write(UC_X86_REG_ECX,receiver);vm.reg_write(UC_X86_REG_EFLAGS,2)
  vm.emu_start(entry,STOP,count=150000)
  inline=op in [15,16];assert vm.reg_read(UC_X86_REG_EIP) in ([0x4dd8c3,0x4dd162,0x4d806f] if inline else [STOP]),'Native budget/exit differs'
  assert vm.reg_read(UC_X86_REG_ESP)==sp+(0 if inline else 4+len(args)*4),'Native stack differs'
  assert all(vm.reg_read(r)==v for r,v in saved),'Native saved registers differ'
  return vm.reg_read(UC_X86_REG_EAX)
 entries=[0x41e5c0,0x41f220,0x41e1b0,0x41e170,0x41e140,0x41dfc0,0x41dfe0,0x41e010,0x41f460,0x41f4d0,0x41f870,0x41f670,0x41e0e0,0x41f5c0,0x402e50,0x4dcfca,0x4d801c,0x41e470]
 result=execute(entries[op],(selected,) if op in [5,6,7] else (),BASE+0x2000 if op in [5,6,7] else 0x704c00)
 if op==4:result=result*100+execute(0x41f210)
 if op not in [3,4,5,6,7,9,17]:result=0
 return {'result':result,'state':b''.join(bytes(vm.mem_read(a,n)) for a,n in regions).hex(),'events':events.copy()}
default=[0,1,80,1,2,256,1024,768,116,19,140,3,0xfffff448,12,0,0,2,61]
cases=[]
def case(**changes):
 p=default.copy()
 for i,v in changes.items():p[int(i[1:])]=v
 cases.append(p)
for kind,flags,thumb,(w,h),metric in itertools.product([0,1,256],[0,2,4,6,8,18,34,116,256,0x3fe74],[0,19,258,0xffff],[(640,480),(800,600),(1024,768)],[0,3,12]):case(p1=kind,p6=w,p7=h,p8=flags,p9=thumb,p11=metric)
for op,kind,files,scroll,selected,hover in itertools.product([1,2,3,4,5,6,7,8,10,11,12,13,16],[0,1,256],[0,1,15,80],[0,1,65],[0,2,79,80,0xffffffff],[0,1,15,256]):
 # Avoid the native initializer using a negative/huge selected row as a name.
 case(p0=op,p1=kind,p2=files,p3=scroll,p4=selected,p5=hover)
for op,kind,mask,(x,y) in itertools.product([9,15],[0,1,256],[0,1,2,16,32,0x1100,1<<17,1<<16],[(280,250),(300,300),(312,450),(313,451)]):case(p0=op,p1=kind,p14=mask,p15=x,p16=y)
for difficulty,clicked,screen in itertools.product([0,1,4,5,256,0xffffffff,0x7fffffff,0x80000000],[0,1,258],[53,61]):case(p0=14,p15=clicked,p16=difficulty,p17=screen)
for metric,towns,flags in itertools.product([0,1,2,3,4,5,10,11,20,40],[0,1,6,7,11,21,31,40],[0,16,512,0x3fe00]):case(p0=17,p11=metric,p13=towns,p8=flags)
exe=ROOT/'build/c-only-debug/Debug/restore_mission_menu_verify.exe'
process=subprocess.Popen([str(exe),'--oracle-batch'],cwd=ROOT,stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True)
report={'format':1,'original_game_executed':False,'native_sha256':digest,'method':'Bounded native mission foreground, rows, scroll, explicit list accessors, initialization/activation, local callbacks, difficulty score, and original inline mode-61 input/foreground branches. Actual local predicates and list selection run; final text/drawing, UI dispatch and nested mission loading endpoints are controlled.',
 'compiled_C_sha256':hashlib.sha256(exe.read_bytes()).hexdigest(),'limitations':['Town connectivity/mission data parsers, game preparation, simulation updates after difficulty changes and final pixels are separate checks. The original game process is never launched.']}
checked=0;p=None
try:
 assert json.loads(process.stdout.readline())=={'fields':18}
 for p in cases:
  process.stdin.write(' '.join(map(str,p))+'\n');process.stdin.flush();line=process.stdout.readline();assert line,'C exited: '+process.stderr.read()
  actual=json.loads(line);expected=native(p)
  if actual!=expected:
   differing=[(i,a,b) for i,(a,b) in enumerate(zip(actual['events'],expected['events'])) if a!=b]
   raise AssertionError(repr({'C_result':actual['result'],'native_result':expected['result'],'state_equal':actual['state']==expected['state'],'C_event_count':len(actual['events']),'native_event_count':len(expected['events']),'first_event_difference':differing[:1]}))
  checked+=1
 report.update(passed=True,checked=checked)
except Exception as error:report.update(passed=False,checked=checked,case=p,error=str(error),native_eip=hex(vm.reg_read(UC_X86_REG_EIP)))
finally:process.stdin.close();process.wait(timeout=10)
sources=['include/file_list.h','include/mission_menu.h','include/sound_streams.h','runtime/mission_menu.c','tests/mission_menu.c','tools/verify_mission_menu_emulation.py']+[f'src/unit_{i:05d}.c' for i in [284,285,286,287,293,294,295,296,297,298,299,302,303,304,305,306,307,309,310,311]]
report['source_hashes']={s:hashlib.sha256((ROOT/s).read_bytes()).hexdigest() for s in sources}
(ROOT/'metadata/mission-menu-emulation.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report,indent=2));sys.exit(0 if report['passed'] else 1)
