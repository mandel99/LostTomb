"""Compare reconstructed menu input with bounded original x86 instructions.

No game process is launched. Mouse, sound, drawing, game actions and OS calls
are controlled endpoints; their argument order and observable state are checked.
"""
from pathlib import Path
import hashlib,itertools,json,struct,subprocess,sys
ROOT=Path(__file__).resolve().parent.parent
sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_BLOCK,UC_HOOK_CODE
from unicorn.x86_const import *
image=(ROOT/'build/smoke-game/Pharaoh.exe').read_bytes()
assert hashlib.sha256(image).hexdigest()=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
u16=lambda b,p:struct.unpack_from('<H',b,p)[0]
u32=lambda b,p:struct.unpack_from('<I',b,p)[0]
pe=u32(image,0x3c);opt=pe+24;table=opt+u16(image,pe+20);base=u32(image,opt+28);size=u32(image,opt+56)
vm=Uc(UC_ARCH_X86,UC_MODE_32);vm.mem_map(base,(size+4095)&~4095)
for i in range(u16(image,pe+6)):
 p=table+i*40;vm.mem_write(base+u32(image,p+12),image[u32(image,p+20):u32(image,p+20)+u32(image,p+16)])
BASE=0x30000000;STOP=0x33000000;STACK=0x34000000
for a in [BASE,STOP,STACK]:vm.mem_map(a,0x10000)
regions=[(0xe38dc0,256),(0xd3b190,16),(0xd3b3a8,36),(0xe36c88,12),(BASE+0x100,324),
 (0xe945f8,24),(0xe94d98,16),(0x5d2a2c,4),(0x605a3c,12),(0x704dac,8),(0xf53f78,4),(0xe39140,4),
 (0xe38da4,8),(0xe20990,4),(0x7056a8,36),(0xe2079c,512),(0xe20774,40),(BASE+0x500,1280)]
observed=[0xd3b198,0xd3b194,0xd3b3c8,0xf53f78,0xe38ebc,0x605a3c,0x704dac,0x704db0,
 0xe38ea8,0x5d2a2c,0xe38dd8,0xe38e40,0xe38e38,0xe94da4]
flag_address=[0xe38e03,0xe38e02,0xe38df7,0xe38e04,0xe38df8,0xe38e00,0xe38df4,0xe38df6]
def write(a,v,n=4):vm.mem_write(a,(v&((1<<(n*8))-1)).to_bytes(n,'little'))
def read(a):return int.from_bytes(vm.mem_read(a,4),'little')
events=[];scenario=None
endpoints={0x53b090:(0x53b090,1,False),0x47d350:(0x47d350,1,False),0x4cbf30:(0x4cbf30,2,False),0x536820:(0x536820,4,False),0x536c30:(0x536c30,6,False),
 0x516bb0:(0x516bb0,0,False),0x41f870:(0x41f870,0,False),0x4c5920:(0x4c5920,0,False),
 BASE+0x3000:(0x56f1cc,1,True),BASE+0x3010:(0x56f178,2,True),
 BASE+0x3020:(0x56f16c,6,True),BASE+0x3030:(0x56f174,4,True)}
for i in range(8):
 for side in range(2):
  a=BASE+0x1000+i*0x20+side*0x10;endpoints[a]=(a,0,False)
def endpoint(cpu,a,n,user):
 sp=cpu.reg_read(UC_X86_REG_ESP);label,count,stdcall=endpoints[a]
 args=[read(sp+4+i*4) for i in range(count)]
 if label==0x56f1cc:write(args[0],scenario[3]);write(args[0]+4,scenario[4])
 else:events.append([label,count]+args+[0]*(8-count)+[read(a) for a in observed])
 value=1
 if label==0x4cbf30:
  length=0
  while vm.mem_read(args[0]+length,1)!=b'\0':
   length+=1
   assert length<64,'Unterminated input text'
  value=length*8
 cpu.reg_write(UC_X86_REG_EAX,value)
 cpu.reg_write(UC_X86_REG_EIP,read(sp));cpu.reg_write(UC_X86_REG_ESP,sp+4+(4*count if stdcall else 0))
ranges=[(0x417ff0,0x4180a9),(0x425990,0x425bc0),(0x4261d0,0x4264c4),(0x4cf210,0x4cf453),
 (0x4cde40,0x4cde82),(0x4d5d70,0x4d5d82),(0x4d66f0,0x4d673c),(0x4d6740,0x4d6860),(0x4d8310,0x4d83ce),
 (0x4cc1b0,0x4cc5b7),(0x4cbee0,0x4cbf30)]
def guard(cpu,a,n,user):
 if a not in endpoints and not any(lo<=a and a+n<=hi for lo,hi in ranges):
  raise RuntimeError('Escaped native menu input at '+hex(a))
vm.hook_add(UC_HOOK_BLOCK,guard)
for a in endpoints:vm.hook_add(UC_HOOK_CODE,endpoint,begin=a,end=a)
for iat,address in [(0x56f1cc,BASE+0x3000),(0x56f178,BASE+0x3010),(0x56f16c,BASE+0x3020),(0x56f174,BASE+0x3030)]:write(iat,address)
def native(p):
 global scenario
 scenario=p;events.clear()
 for a,n in regions:vm.mem_write(a,b'\xa5'*n)
 for i in range(8):
  left,top,right,bottom=192,100+(40*i if p[8]==1 else 0),448,125+(40*i if p[8]==1 else 0)
  if p[8]==2:left,top,right,bottom=-300,-200,-100,-80
  vm.mem_write(BASE+0x100+i*28,struct.pack('<hhhhII4Bii',left,top,right,bottom,BASE+0x1000+i*0x20,
   BASE+0x1010+i*0x20,0x36,0x5a,0x73,p[6] if i==0 else 1 if i%2 else 3,0x12345600+i,0x76543200+i))
 for i,a in enumerate(flag_address):write(a,(0x80 if p[5]&0x200 else 1) if p[5]&(1<<i) else 0,1)
 for a,v in [(0xe38e34,p[3]),(0xe38e30,p[4]),(0xd3b3ac,0x100 if p[5]&0x100 else 0),
  (0xd3b3c8,p[9]+1000),(0xd3b198,0x87654321),(0xd3b194,0xabcdef01),(0xe39140,0x12345678),(0x605a44,p[8])]:write(a,v)
 write(0xe38dda,p[6],1);ready=0xe94624+(100+p[8] if p[6]==7 else p[6])*16;write(ready,0xa5a5a5a5)
 if p[0]==2:
  for a,v in [(0xe38d9c,p[10]),(0xe38d98,p[11]),(0xe39130,11),(0xe39134,23),(0xe39138,p[12]+11),
   (0xe3913c,p[13]+23),(0xe38dcc,p[14]),(0xe38e34,p[17]),(0xe38e30,p[18]),(0xe38e14,p[20]),
   (0xe38e1c,p[21]),(0xe38e18,p[22]),(0xe92e0c,p[23]),(0xe36c8c,p[9]^0x12345678)]:write(a,v)
  write(0xe38e0a,p[15],2);write(0xe38e08,p[16],2)
  write(0xe38e00,bool(p[19]&1),1);write(0xe38df4,bool(p[19]&2),1);write(0xe38e07,p[5],1)
 elif p[0]==3:
  write(ready,p[14]);write(ready-8,BASE+0x400 if p[7] else 0);write(0xe94d98,p[5])
  for i in range(3):
   vals=[i*2+1,0,p[3]+i*111,p[4]-i*83,p[10]+i*500,p[11]+i*600,0 if i==2 else BASE+0x400+(i+1)*28]
   vm.mem_write(BASE+0x400+i*28,struct.pack('<7I',*(v&0xffffffff for v in vals)))
 elif p[0]==4:write(0xd3b198,p[1])
 elif p[0]==5:
  write(0xe38dda,0,1);write(0xe38da8,p[1]);write(0xe38da4,p[2]);write(0xe38e14,p[20])
  vm.mem_write(0x57bd88,bytes(vm.mem_read(BASE+0x100,5*28)))
  for a,v in [(0xe94624,p[14]),(0xe9461c,0),(0xe94d98,0),(0xe20990,0x12345678),
   (0x7056a8,0x23456789),(0x7056b0,0x34567890),(0x7056b8,0x45678901),(0x7056c8,0x56789012)]:write(a,v)
  ready=0xe94624
 elif p[0]==6:
  for i in range(20):
   text=BASE+0x500+i*64;vm.mem_write(text,bytes([65+i])*(p[3]%32)+b'\0')
   for a,v in [(0xe2079c,text),(0xe207ec,p[17]),(0xe208a0,p[18]),(0xe208f0,p[2]),(0xe20850,p[10]),(0xe20940,p[11])]:write(a+i*4,v)
   write(0xe2083c+i,p[16]&1,1)
  write(0xe20990,p[1]);write(0xe2079b,p[14]&1,1)
 elif p[0]==7:
  for i in range(9):
   vm.mem_write(BASE+0x100+i*36,struct.pack('<6hIII4Bii',192,100+(i*40 if p[8]==1 else 0),256,25,p[6],123,
    0x5a5a5a5a,BASE+0x1000+i*0x20,BASE+0x1010+i*0x20,int(bool(p[14]&(1<<i))),p[15],p[16],0x51,0x12345600+i,0x76543200+i))
 entry=[0x4261d0,0x426450,0x4cf210,0x4d6740,0x417ff0,0x4d8310,0,0x425990][p[0]] if p[0]!=6 else [0x4cc500,0x4cc3c0,0x4cc3e0,0x4cc370,0x4cc390,0x4cc560,0x4cc540,0x4cc400,0x4cc330,0x4cc4c0,0x4cc250,0x4cc1b0,0x4cc320][p[5]]
 args=[p[1],p[2],BASE+0x100,p[7]] if p[0]<2 else [p[1],p[2],BASE+0x100,p[7],p[17]] if p[0]==3 else [p[1]] if p[0]==6 and p[5]==0 else []
 if p[0]==6 and p[5]==5:args=[p[1],BASE+0x500+p[1]*64,p[2],p[10],p[7],p[18]]
 if p[0]==6 and p[5] in [7,10]:args=[p[12]&255 if p[5]==7 else p[12]]
 if p[0]==7:args=[p[1],p[2],BASE+0x100,p[7]]
 sp=STACK+0x8000
 for i,v in enumerate([STOP]+args):write(sp+i*4,v)
 saved=[(UC_X86_REG_EBX,0x11111111),(UC_X86_REG_EBP,0x22222222),(UC_X86_REG_ESI,0x33333333),(UC_X86_REG_EDI,0x44444444)]
 for r,v in saved:vm.reg_write(r,v)
 vm.reg_write(UC_X86_REG_ESP,sp);vm.reg_write(UC_X86_REG_EFLAGS,2);vm.emu_start(entry,STOP,count=100000)
 assert vm.reg_read(UC_X86_REG_EIP)==STOP and vm.reg_read(UC_X86_REG_ESP)==sp+4,'Native stack/budget differs'
 assert all(vm.reg_read(r)==v for r,v in saved),'Native callee-saved register differs'
 if p[0]==5:vm.mem_write(BASE+0x100,bytes(vm.mem_read(0x57bd88,5*28)))
 result=vm.reg_read(UC_X86_REG_EAX) if p[0] in [0,1,3,7] else 0
 return {'result':result,'state':[read(a+j) for a,n in regions for j in range(0,n,4)]+[read(ready)],'events':events.copy()}
def case(**kw):
 p=[0,192,144,400,250,1,1,3,1,7]+[0]*14
 for i,v in kw.items():p[int(i[1:])]=v&0xffffffff
 return p
cases=[]
for flags,trigger in itertools.product(range(512),range(6)):
 cases.append(case(p5=flags,p6=trigger,p8=0))
for flags,trigger in itertools.product(range(256),range(6)):
 cases.append(case(p5=flags|0x200,p6=trigger,p8=0))
for op,layout,offset,count in itertools.product(range(2),range(3),[(0,0),(80,60),(192,144),(-200,300)],[0,1,3,8,-1]):
 left,top,right,bottom=(-300,-200,-100,-80) if layout==2 else (192,100,448,125)
 for x,y in [(left,top),(right-1,bottom-1),(left-1,top),(right,top),(left,bottom)]:
  cases.append(case(p0=op,p1=offset[0],p2=offset[1],p3=x+offset[0],p4=y+offset[1],p7=count,p8=layout))
for mode,ready,debug,hit,linked in itertools.product([0,7,47,54,60,83,255],[0,1],[0,2,258],[0,1,3,5],[0,1]):
 cases.append(case(p0=3,p5=debug,p6=mode,p7=linked,p8=2,p14=ready,p17=hit,p10=70000,p11=90000))
for action in [0,1,2,3,4,5,255,256,0xffffffff]:cases.append(case(p0=4,p1=action))
for offset,button,ready,flags,drag in itertools.product([(0,0),(80,60),(192,144)],range(5),[0,1],[0,1,4,8],[0,2]):
 cases.append(case(p0=5,p1=offset[0],p2=offset[1],p3=320+offset[0],p4=112+40*button+offset[1],p7=5,p14=ready,p5=flags,p20=drag))
for resolution,client,direct,point,mask,previous in itertools.product([(640,480),(800,600),(1024,768)],
 [(640,480),(1024,768),(1920,1080),(0,0)],[0,1],[(900,600),(-40,-200),(32768,65535)],range(4),range(4)):
 cases.append(case(p0=2,p3=point[0],p4=point[1],p5=mask,p9=0xfffffff0,p10=resolution[0],p11=resolution[1],
  p12=client[0],p13=client[1],p14=direct,p15=500,p16=-100,p17=499,p18=-101,p19=previous,
  p20=1,p21=450,p22=-51,p23=0x12345670))
for dx,dy in itertools.product([-51,-50,-49,0,49,50,51],repeat=2):
 cases.append(case(p0=2,p3=500+dx,p4=300+dy,p10=1024,p11=768,p12=1024,p13=768,p14=1,
  p17=500+dx,p18=300+dy,p20=1,p21=500,p22=300,p23=0xffffffff,p9=0xfffffff0))
for slot,operation,length,limit,cursor_pos in itertools.product([0,1,7,19],range(5),[0,5,31],[-1,0,4,31,40],[-1,0,4,31,256]):
 cases.append(case(p0=6,p1=slot,p2=limit,p3=length,p5=operation,p17=cursor_pos,p18=5))
for slot,flag,font in itertools.product([0,1,2,4,5,6,7,19],[0,1,255],[201,335,871,1005]):
 cases.append(case(p0=6,p1=slot,p2=31,p5=5,p7=flag,p10=280,p18=font))
cases.append(case(p0=6,p1=1,p5=6))
for operation,length,limit,position,insert,char,width in itertools.product(range(7,13),[0,5,31],[4,31],[0,3,5],[0,1],[32,65,0xe1,0x7f],[32,280]):
 if position>length:continue
 cases.append(case(p0=6,p1=1,p2=limit,p3=length,p5=operation,p10=width,p11=18765,p12=char,p14=insert,p17=position,p18=length))
for char,numeric,insert in itertools.product(range(256),[0,1],[0,1]):
 cases.append(case(p0=6,p1=1,p2=31,p3=5,p5=10,p10=280,p11=18765,p12=char,p14=insert,p16=numeric,p17=2,p18=5))
for flags,kind,pressed,hover,enabled,count in itertools.product([0,1,16,32,17,33,49],[0,2,4,6],[0,1,20],[0,1,2,255],[0,1,255],[0,1,3]):
 cases.append(case(p0=7,p5=flags,p6=kind,p7=count,p8=0,p14=enabled,p15=pressed,p16=hover))
exe=ROOT/('build/c-only-debug/Debug/restore_menu_input_verify.exe' if len(sys.argv)<2 else sys.argv[1])
process=subprocess.Popen([str(exe),'--oracle-batch'],cwd=ROOT,stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True)
report={'format':1,'original_game_executed':False,'method':'Bounded x86 execution against compiled independent C; OS, cursor, text measurement, sound, draw and nested action endpoints controlled',
 'limitations':['Controlled nested actions do not establish working family, mission, rankings or settings flows.','No graphical rendering or user-input replay is proved by this comparison.'],
 'compiled_C_sha256':hashlib.sha256(exe.read_bytes()).hexdigest(),'by_operation':{}}
checked=0;p=None
try:
 assert json.loads(process.stdout.readline())=={'fields':24}
 for p in cases:
  process.stdin.write(' '.join(map(str,p))+'\n');process.stdin.flush();line=process.stdout.readline()
  assert line,'C oracle exited: '+process.stderr.read()
  actual=json.loads(line);expected=native(p)
  if actual!=expected:
   diff={'C':actual,'native':expected}
   raise AssertionError(repr(diff))
  checked+=1;op=str(p[0]);report['by_operation'][op]=report['by_operation'].get(op,0)+1
 report.update(passed=True,checked=checked)
except Exception as error:report.update(passed=False,checked=checked,case=p,error=str(error),native_eip=hex(vm.reg_read(UC_X86_REG_EIP)))
finally:process.stdin.close();process.wait(timeout=10)
report['source_hashes']={s:hashlib.sha256((ROOT/s).read_bytes()).hexdigest() for s in ['include/menu_input.h','src/unit_00404.c','src/unit_00405.c','src/unit_00406.c','src/unit_00416.c','src/unit_00417.c','src/unit_01768.c','src/unit_01836.c','src/unit_01857.c','src/unit_01858.c','src/unit_01868.c','src/unit_01665.c','src/unit_01666.c','src/unit_01674.c','src/unit_01675.c','src/unit_01676.c','src/unit_01677.c','src/unit_01678.c','src/unit_01680.c','src/unit_01681.c','src/unit_01682.c','src/unit_01683.c','src/unit_01684.c','src/unit_01685.c','src/unit_01686.c','src/unit_01687.c','src/unit_01688.c','runtime/menu_actions.c','tests/menu_input.c','tools/verify_menu_input_emulation.py']}
(ROOT/'metadata/menu-input-emulation.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps(report,indent=2));sys.exit(0 if report['passed'] else 1)
