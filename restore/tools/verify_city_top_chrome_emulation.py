"""Composed native/C comparison of top chrome with twelve frozen HUD functions.

The canonical executable is data only. Its selected original instructions run
inside Unicorn. Raster, viewport, numeric/text and widget drawing primitives
are controlled endpoints with ordered argument and text-width side effects.
"""
from pathlib import Path
import argparse,hashlib,json,random,re,struct,subprocess,sys,time
ROOT=next(p for p in Path(__file__).resolve().parents if (p/'build/smoke-game/Pharaoh.exe').exists() and (p/'src').is_dir())
PHASE=ROOT/'artifacts/city_top_chrome_phase'
sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE,__version__
from unicorn.x86_const import *
parser=argparse.ArgumentParser(description=__doc__)
parser.add_argument('--exe',type=Path,default=ROOT/'build/agent-city-top-chrome/Debug/restore_city_top_chrome_verify.exe')
parser.add_argument('--production',action='store_true',help='Bind actual production sources after applying the staged pair')
args=parser.parse_args();exe=args.exe.resolve()
LAYOUT=ROOT if args.production else PHASE
NATIVE=ROOT/'metadata/city-top-chrome-native.json' if args.production else PHASE/'metadata/native.json'
REPORT=ROOT/'metadata/city-top-chrome-emulation.json' if args.production else PHASE/'metadata/emulation.json'
image=(ROOT/'build/smoke-game/Pharaoh.exe').read_bytes();SHA=hashlib.sha256(image).hexdigest()
assert SHA=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
entries=[0x536670,0x425330,0x51e930,0x51fd60,0x51e990,0x51ec90,0x51ed30,0x51edd0,0x51efb0,0x51f0a0,0x51f680,0x533e70,0x537100,0x537120]
functions={}
for filename in [ROOT/'metadata/city-ui-next-native.json',ROOT/'metadata/city-ui-children-native.json',ROOT/'metadata/city-ui-layout-native.json',NATIVE]:
 d=json.loads(filename.read_text(encoding='utf-8-sig'));assert d['native_sha256']==SHA
 for address,record in d['functions'].items():
  if int(address,16) in entries:
   assembly=record.get('assembly',record);assert not assembly['has_more'] and len(assembly['items'])==assembly['total']
   functions[int(address,16)]=assembly['items']
assert set(functions)==set(entries)
u16=lambda p:struct.unpack_from('<H',image,p)[0]
u32=lambda p:struct.unpack_from('<I',image,p)[0]
pe=u32(0x3c);opt=pe+24;table=opt+u16(pe+20);base=u32(opt+28);size=u32(opt+56)
vm=Uc(UC_ARCH_X86,UC_MODE_32);vm.mem_map(base,(size+4095)&~4095);vm.mem_write(base,image[:u32(opt+60)])
for i in range(u16(pe+6)):
 p=table+i*40;vm.mem_write(base+u32(p+12),image[u32(p+20):u32(p+20)+u32(p+16)])
allowed={int(i['address'],16) for body in functions.values() for i in body}
for body in functions.values():
 for i in body:
  raw=bytes.fromhex(i['bytes']);assert bytes(vm.mem_read(int(i['address'],16),len(raw)))==raw
regions=[(int(a,16),int(n,16)) for a,n in re.findall(r'\{0x([0-9a-f]+),0x([0-9a-f]+)\}',(LAYOUT/'tests/city_top_chrome_fixture.h').read_text())]
STOP=0x33000000;STACK=0x34000000
vm.mem_map(STOP,0x1000);vm.mem_map(STACK,0x10000);vm.mem_map(0x30000000,0x10000)
ENDPOINTS={0x4dddc0:0,0x425900:0,0x51f780:0,0x425730:4,0x43e810:4,
 0x4cd510:5,0x4cd9d0:5,0x4cda40:5,0x4cc770:5,0x4d62f0:8,0x4cce90:8,0x4d6250:9,0x4d6860:9,0x4cccb0:6,0x53e590:3,0x44bd00:0,0x4ddec0:0,0x4ddf20:0,0x4de080:0,0x4d71c0:1,0x4cc110:2,0x4cbf30:2,0x4ccbb0:6}
TEXT={0x4d62f0,0x4d6250,0x4d6860,0x4cce90,0x4cccb0,0x4cc770}
state={};executed=set();ANIMATION=bytes(vm.mem_read(0x5f8260,47*4));MENU_RECORDS={a:bytes(vm.mem_read(a,64)) for a in [0x5c91f0,0x5c9230]}
def read(a,n=4):return int.from_bytes(vm.mem_read(a,n),'little')
def write(a,v,n=4):vm.mem_write(a,(v&((1<<(n*8))-1)).to_bytes(n,'little'))
def hook(cpu,a,n,user):
 if a in allowed:executed.add(a);return
 assert a in ENDPOINTS,'Unexpected native instruction '+hex(a)
 sp=cpu.reg_read(UC_X86_REG_ESP);count=ENDPOINTS[a];p=[read(sp+4+4*i) for i in range(count)]
 if a==0x53e590:assert cpu.reg_read(UC_X86_REG_ECX)==0xf55bb0
 e=[a,count]+p+[0]*(9-count)
 if a in [0x4cc110,0x4cbf30,0x4ccbb0]:e[8:11]=[read(0xe20768),read(0xe20778),read(0xd3b510)]
 state['events'].append(e);variant=state['variant']
 if a==0x4cc110:
  write(0xe20768,0x30002000+((p[0]*37+p[1]*11)&63)*32)
  if variant==9:
   for address,value in [(0xe20778,0x12340010+(p[0]&15)),(0xf56f8c,read(0xf56f8c)^0x123400),(0xf56f84,read(0xf56f84)+0x10000),(0x705730,2)]:write(address,value)
 if a==0x4cbf30 and variant==10:write(0xf56f8c,read(0xf56f8c)^0x1000000);write(0xe20778,0x7ffffffe)
 if a==0x4ccbb0:write(0xe20778,0xfffffff8 if variant==8 else p[2]+(p[0]&15)+30+(512 if variant==2 else 0))
 if a in TEXT:write(0xe20778,(p[0]&15)+7+(512 if variant==2 else 0))
 if a==0x53e590:
  state['sprites']+=1
  if variant==1:write(0xe38db0,800);write(0xf53f78,1)
  if variant==4 and state['sprites']==1:
   for address,value in [(0xe38db0,1024),(0xf53f60,read(0xf53f60)+257),(0xf53f64,read(0xf53f64)+513),(0xf567e4,read(0xf567e4)+65536),(0xf53f78,256)]:write(address,value)
 if a==0x4dddc0 and variant==3:
  for address,value in [(0xf53f7c,45),(0xf53f78,3),(0xe38db0,800),(0xf53f60,read(0xf53f60)+37),(0xf53f64,read(0xf53f64)+113),(0xf567e4,read(0xf567e4)+256),(0xd3b558,0x9abcdef0)]:write(address,value)
 if a==0x425730 and variant==5:
  state['widgets']+=1
  if state['widgets']==1:
   for address,value in [(0xf53f64,read(0xf53f64)+256),(0xf53f60,read(0xf53f60)+1024),(0xf567e4,read(0xf567e4)+512)]:write(address,value)
 if a in [0x4ddec0,0x4ddf20] and variant==6:write(0xf53f78,0x10000);write(0xf53f7c,read(0xf53f7c)+256)
 cpu.reg_write(UC_X86_REG_EAX,state['predicate'] if a==0x44bd00 else 0x9abcdef0)
 cpu.reg_write(UC_X86_REG_ECX,0xa17e4567);cpu.reg_write(UC_X86_REG_EDX,0x5a5a5a5a)
 if a==0x4d71c0 and variant==7:cpu.reg_write(UC_X86_REG_EIP,0x51e990);return
 cpu.reg_write(UC_X86_REG_EIP,read(sp));cpu.reg_write(UC_X86_REG_ESP,sp+4+(12 if a==0x53e590 else 0))
vm.hook_add(UC_HOOK_CODE,hook)
def execute(entry,p1=0,p2=0):
 sp=STACK+0x8000;vm.mem_write(sp-0x200,b'\xcc'*0x400)
 for i,v in enumerate([STOP,p1,p2]):write(sp+4*i,v)
 saved=[(UC_X86_REG_EBX,0x11223344),(UC_X86_REG_EBP,0x22334455),(UC_X86_REG_ESI,0x33445566),(UC_X86_REG_EDI,0x44556677)]
 for r,v in saved:vm.reg_write(r,v)
 vm.reg_write(UC_X86_REG_ESP,sp);vm.reg_write(UC_X86_REG_EFLAGS,2)
 vm.emu_start(entry,STOP,count=100000)
 assert vm.reg_read(UC_X86_REG_EIP)==STOP and vm.reg_read(UC_X86_REG_ESP)==sp+4
 assert all(vm.reg_read(r)==v for r,v in saved)
 assert bytes(vm.mem_read(sp+12,0x100))==b'\xcc'*0x100

def native(case):
 entry,p1,p2,variant,predicate,buffers=case;state.clear();state.update(events=[],variant=variant,predicate=predicate,sprites=0,widgets=0)
 for (a,n),b in zip(regions,buffers):assert len(b)==n;vm.mem_write(a,bytes(b))
 if entry==0xa001:
  for child in [0x51e930,0x51e990,0x51efb0,0x51f0a0]:execute(child)
 elif entry==0xa002:
  execute(0x51e930)
  for i in range(p1):execute(0x51fd60)
 else:execute(entry,p1,p2)
 return dict(state=''.join(bytes(vm.mem_read(a,n)).hex() for a,n in regions),events=state['events'])
def put(buffers,a,v,n=4):
 for (lo,size),b in zip(regions,buffers):
  if lo<=a and a+n<=lo+size:b[a-lo:a-lo+n]=(v&((1<<(8*n))-1)).to_bytes(n,'little');return
 raise ValueError('Fixture address '+hex(a))
def fixture():
 b=[bytearray(((i*41+(i>>4)*7+idx*29)^0x5a)&255 for i in range(n)) for idx,(a,n) in enumerate(regions)]
 values={0x605a3c:0,0x705730:0,0xe20768:0x30002000,0xd3b510:0x12345678,0x6059e4:0,0x605a30:0,0x605a34:999,0x7056ec:0,0x78d7bc:300,0x78d7c0:700,
 0xbc968c:1,0xbc9694:1,0xe38db0:1024,0xe38dac:768,0xe38d98:759,0xe38da4:300,0xe38da8:400,
 0xe38dcc:0,0xe38ebc:0,0xe94e0c:0,0xe951d8:300,0xe951ec:900,0xeaac28:0,
 0xea187c:12345,0xea1894:67890,0xea6254:54321,0xea626c:98765,0xec1d64:11234,0xec1d7c:55678,
 0xf53da0:0,0xf53da4:0,0xf53da8:0,0xf53dac:0,0xf53db0:0,0xf53f60:1000,0xf53f64:864,0xf53f74:1,0xf53f78:0,0xf53f7c:0,0xf53fd0:1,0xe39104:0,0xd3b558:1024,
 0xf55c64:10000,0xf567c4:11000,0xf567e4:12000,0xf56f84:18765,0xf56f8c:18876,0xf56f9c:315,0xf56fa4:316}
 for a,v in values.items():put(b,a,v)
 for i,value in enumerate(struct.unpack('<47I',ANIMATION)):put(b,0x5f8260+i*4,value)
 for a,data in MENU_RECORDS.items():
  for i in range(0,64,2):put(b,a+i,int.from_bytes(data[i:i+2],'little'),2)
 for i in range(16):
  for offset,value in [(0,300),(2,0),(4,i*31-100),(6,3+i*17)]:put(b,0x30001000+i*16+offset,value,2)
 put(b,0xe38dda,1,1);put(b,0xe38dc9,0,1);put(b,0x784cb0,2,1);put(b,0x784b94,0,2)
 for a in range(0x784bc0,0x784c00,2):put(b,a,0xffff,2)
 for a in [0x784c60,0x784c62,0x784c64,0x784c66,0x784c68,0x784c6a,0x784c8c,0x784c8e,0x784c90,0x784c92]:put(b,a,0xffff,2)
 return b
HUD_ENTRIES=[a for a in entries if a not in [0x536670,0x425330]]
cases=[]
def add(entry=0x51e990,p1=1,p2=875,variant=0,predicate=1,changes=None):
 b=fixture()
 for item in changes or []:put(b,*item)
 cases.append((entry,p1&0xffffffff,p2&0xffffffff,variant,predicate,b))
for width,height in [(640,480),(800,600),(1024,768),(1280,960)]:
 for mode in [0,1,2,256]:
  for collapsed in [0,1,2,256]:
   for variant in [0,1,2]:add(variant=variant,changes=[(0xe38db0,width),(0xe38dac,height),(0x605a3c,mode),(0xf53f78,collapsed)])
for entry in HUD_ENTRIES:
 for force in [0,1,256]:add(entry,p1=force)
for mode in [0,1,2,256]:
 for panel in [0,1,256]:
  for dirty in [0,1,256,0xffffffff]:
   for redraw in [0,1,256]:
    for force in [0,1]:add(0x51efb0,p1=force,changes=[(0x605a3c,mode),(0xf53f78,panel),(0xf53fd0,dirty),(0xe38dcc,redraw),(0x6059e4,256)])
for period in list(range(14))+[256]:
 for panel in [0,1]:
  for force in [0,1]:add(0x51edd0,p1=force,changes=[(0x7056ec,period),(0xf53f78,panel)])
for entry in [0x51ed30,0x51f680]:
 for force in [0,1]:
  for current,cached in [(0,0),(256,0),(256,256),(0xffffffff,0),(900,900)]:
   for mode in [0,1]:
    for panel in [0,1]:add(entry,p1=force,changes=[(0xe94e0c,current),(0xf53db0,cached),(0xe951ec,current),(0xf53da0,cached),(0x605a3c,mode),(0xf53f78,panel),(0xbc968c,current),(0xbc9694,cached)])
for game in list(range(83))+[128,255]:
 for width in [640,800,1024]:add(0x51f0a0,changes=[(0xe38dda,game,1),(0xe38db0,width),(0xeaac28,1)])
for cached,force,refresh in [(False,0,0),(True,0,0),(True,0,1),(True,0,2),(True,0,3),(True,1,0),(True,0,0xffffffff)]:
 for width in [640,800,1024,1280]:
  for mode in [0,1]:
   for idx in [0,1,7]:
    a=idx*0x49d8;add(0x51f0a0,p1=force,variant=2,changes=[(0xe38db0,width),(0x605a3c,mode),(0xeaac28,idx),(0xf53da4,700 if cached else 0),(0xf53da8,{0:12345,1:54321,7:11234}[idx] if cached else 0),(0xf53dac,{0:67890,1:98765,7:55678}[idx] if cached else 0),(0xf53f74,refresh)])
for status in [0,1,2,4,5,33,30,6,11,12,26,99]:
 for count in [0,1,2,16]:
  for mask in [0,1,3,5,15]:
   changes=[(0x605a30,status),(0x784b94,status&1,2),(0x784cb0,status%4,1)]
   for i in range(count):changes.extend([(0x784bc0+i*2,123,2),(0x784be0+i*2,456,2)])
   for i,a in enumerate([0x784c8c,0x784c8e,0x784c90,0x784c92]):changes.append((a,0xffff if mask&(1<<i) else 123,2))
   for i,a in enumerate([0x784c60,0x784c62,0x784c64,0x784c66,0x784c68,0x784c6a]):changes.append((a,0xffff if mask&(1<<(i%4)) else 123,2))
   add(0x533e70,predicate=mask&1,variant=2,changes=changes)
rng=random.Random(0x51e990)
add(0x51ec90,changes=[(0xbc968c,0)])
add(0x51ec90,changes=[(0xbc9694,0)])
add(0x51f0a0,changes=[(0xea187c,0xffffffff)])
add(0x533e70,changes=[(0x784b94,0,2),(0x784cb0,1,1)])
add(0x533e70,predicate=1,changes=[(0x784c60,123,2),(0x784c62,456,2)])
add(0x533e70,predicate=1,changes=[(0x784c60,123,2),(0x784c62,0xffff,2)])
for i in range(100):
 add(rng.choice(HUD_ENTRIES),p1=rng.choice([0,1,256]),p2=rng.choice([400,875,1000]),variant=rng.randrange(3),predicate=rng.randrange(2),changes=[(0xe38db0,rng.choice([640,800,1024])),(0xf53f64,rng.choice([400,640,864])),(0xf53f78,rng.randrange(3)),(0x605a3c,rng.randrange(3))])
# New upstream cases retain the ten-controller regression matrix above.
for width in [0,128,256,640,800,1024,1280,0x10280,0x10320,0x10400,0x7fffffff,0xffffffff]:
 for compact,expanded in [(0,0),(0x12345678,0x89abcdef),(0xff,0x100),(0xffffffff,0x80000000)]:add(0x51e930,changes=[(0xe38db0,width),(0xf53f60,compact),(0xf53f64,expanded)])
for width,height in [(640,480),(800,600),(1024,768)]:
 for panel in [0,1,256]:
  for mode in [0,1]:
   for variant in [0,1,2]:add(0xa001,variant=variant,changes=[(0xe38db0,width),(0xe38dac,height),(0xe38d98,height-6),(0xf53f78,panel),(0x605a3c,mode),(0xf53f60,0x12345678),(0xf53f64,0x89abcdef)])
for width,panel,benchmark,step in __import__('itertools').product([640,800,1024],[0,1],[0,25,61,97],range(47)):
 add(0x51fd60,changes=[(0xe38db0,width),(0xf53f60,{640:598,800:718,1024:958}[width]),(0xf53f64,{640:478,800:598,1024:838}[width]),(0xf53f78,panel),(0xf53f7c,step),(0xe39104,benchmark)])
for width,panel,benchmark,step in __import__('itertools').product([640,800,1024,1280],[0,1,2,256],[-2147483648,-1,24,25,60,61,96,97,256,0x7fffffff],[0,43,46,47,256]):
 add(0x51fd60,variant=7 if step>=43 else 0,changes=[(0xe38db0,width),(0xf53f78,panel),(0xf53f7c,step),(0xe39104,benchmark)])
for variant,width,panel,step in __import__('itertools').product(range(1,8),[640,800,1024],[0,1],[0,1,20,43,46]):
 add(0x51fd60,variant=variant,changes=[(0xe38db0,width),(0xf53f78,panel),(0xf53f7c,step),(0xe39104,97 if step>=43 else 0)])
for width,panel,benchmark in __import__('itertools').product([640,800,1024],[0,1],[0,25,61,97]):
 increment=[1,2,3,4][[0,25,61,97].index(benchmark)];frames=(47+increment-1)//increment
 add(0xa002,p1=frames,variant=7,changes=[(0xe38db0,width),(0xf53f78,panel),(0xf53f7c,0),(0xe39104,benchmark)])
# Three resolution anchors, full DWORD IDs/modes, and unsupported-width exits.
for width,mode,image in __import__('itertools').product([640,800,1024],[0,1,256,0x10001],[0,255,256,14284,0x10000,0xfffffff8]):
 add(0x536670,changes=[(0xe38db0,width),(0x605a3c,mode),(0xf567e4,image)])
for width in [0,256,1280,0x10280,0x10320,0x10400,0xffffffff]:add(0x536670,changes=[(0xe38db0,width)])
for count,selected,x,variant in __import__('itertools').product([-2,0,1,2,4,16],[0,1,2,4,16,256,0x10001],[-32768,-300,0,255,256,32767],[0,2,8]):
 add(0x425330,p1=0x30001000,p2=count,variant=variant,changes=[(0x30001000,x,2),(0x705730,selected),(0xf56f84,0x12345678),(0xf56f8c,0x89abcdef),(0xd3b510,0x12345678)])
for font,selected,x,variant in __import__('itertools').product([335,0xff,0x100,0x12345678,0xffffffff],[1,2],[-32768,256,32767],[0,9,10]):
 add(0x425330,p1=0x30001000,p2=4,variant=variant,changes=[(0x30001000,x,2),(0x705730,selected),(0xf56f84,font),(0xf56f8c,font^0x10100),(0x30001004,0x8000,2),(0x30001006,0xffff,2),(0x30001014,0x7fff,2),(0x30001016,0x100,2)])
for address,count,variant in __import__('itertools').product([0xe20778,0xe20768],[0,1],[0,2,9]):add(0x425330,p1=address,p2=count,variant=variant,changes=[(0x705730,1)])
process=subprocess.Popen([str(exe),'--oracle-batch'],cwd=ROOT,stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True)
report=dict(format=1,passed=False,checked=0,native_functions=len(entries),native_sha256=SHA,compiled_C_sha256=hashlib.sha256(exe.read_bytes()).hexdigest(),original_game_executed=False,unicorn_version=__version__,production=args.production,
 method='Compose complete topbar00536670 and menu labels00425330 with twelve unchanged HUD/layout functions. Compare every fixture byte with nonuniform guards, full DWORD art/font/cursor/string-pointer contracts, signed WORD table coordinates/groups, ordered calls and state at text endpoints, both sidebar directions, three resolutions, refresh and callback mutations. Native/C stack and callee-saved registers checked.',
 limitations=['Map/viewport/camera, sprite/widget/text/primitive drawing and predicate0044bd00 are controlled endpoints. Font pixels, translation lookup and complete gameplay rendering are not proved.','Text selection004cc110 supplies a controlled full pointer; measurement004cbf30 and rendering004ccbb0 supply controlled cursor/font mutations. This proof establishes caller contracts; the endpoint algorithms are outside this suite.','004d71c0 remains controlled, with variant7 composing real0051e990 HUD refresh. Existing0051f780 remains an empty controlled helper.','Menu tables use mapped16-byte records and counts through16, plus negative/zero count and bounded alias stress. Malformed out-of-table animation steps and unbounded record extents are excluded. All14 routines have void interfaces; incidental EAX is not compared.'])
start=time.monotonic();case=None
try:
 assert json.loads(process.stdout.readline())==dict(ready=True)
 for case in cases:
  entry,p1,p2,variant,predicate,buffers=case
  process.stdin.write(' '.join(map(str,[entry,p1,p2,variant,predicate]))+' '+' '.join(bytes(b).hex() for b in buffers)+'\n');process.stdin.flush()
  line=process.stdout.readline();assert line,'C exited: '+process.stderr.read()
  actual=json.loads(line);expected=native(case)
  assert actual==expected,repr({'entry':hex(entry),'variant':variant,'args':[p1,p2], 'state_first_difference':next((i//2 for i,(a,b) in enumerate(zip(actual['state'],expected['state'])) if a!=b),None),'C_event_count':len(actual['events']),'native_event_count':len(expected['events']),'first_event_difference':[(i,a,b) for i,(a,b) in enumerate(zip(actual['events'],expected['events'])) if a!=b][:1]})
  report['checked']+=1
  if report['checked']%500==0:print(json.dumps({'checked':report['checked'],'total':len(cases)}),flush=True)
 assert executed==allowed,'Unexecuted native instructions: '+repr(sorted(allowed-executed))
 report['passed']=True
except Exception as e:
 report.update(error=str(e),case=dict(entry=hex(case[0]),args=list(case[1:5]),buffers=[bytes(b).hex() for b in case[5]]) if case else None,native_eip=hex(vm.reg_read(UC_X86_REG_EIP)))
finally:process.stdin.close();process.wait(timeout=20)
source_paths=[LAYOUT/'src/unit_00396.c',LAYOUT/'src/unit_02443.c',LAYOUT/'include/city_top_chrome.h',LAYOUT/'tests/city_top_chrome.c',LAYOUT/'tests/city_top_chrome_fixture.h',Path(__file__),NATIVE,PHASE/'CMakeLists.txt',ROOT/'include/city_ui_controllers.h',ROOT/'include/city_ui_layout.h',ROOT/'metadata/city-ui-next-native.json',ROOT/'metadata/city-ui-children-native.json',ROOT/'metadata/city-ui-layout-native.json']
source_paths += [ROOT/f'src/unit_{i:05d}.c' for i in [2343,2353,2344,2345,2346,2347,2348,2349,2350,2433,2454,2455]]
report['source_hashes']={str(p.resolve().relative_to(ROOT)).replace('\\','/'):hashlib.sha256(p.read_bytes()).hexdigest() for p in source_paths}
prior=json.loads((ROOT/'metadata/city-ui-emulation.json').read_text())
frozen={f:h for f,h in prior['source_hashes'].items() if f.startswith('src/')}
for f in json.loads((ROOT/'artifacts/city_ui_layout_phase/metadata/apply-map.json').read_text())['files']:
 if f['target'].startswith('src/'):frozen[f['target']]=f['sha256']
report['frozen_HUD_source_drift']=[f for f,h in frozen.items() if hashlib.sha256((ROOT/f).read_bytes()).hexdigest()!=h]
assert not report['frozen_HUD_source_drift'],'Frozen HUD source changed'
report['instruction_coverage']={hex(a):{'executed':sum(int(i['address'],16) in executed for i in f),'total':len(f),'unexecuted':[i['address'] for i in f if int(i['address'],16) not in executed]} for a,f in functions.items()}
report['instructions_executed']=len(executed);report['instructions_total']=len(allowed);report['elapsed_seconds']=round(time.monotonic()-start,3)
REPORT.write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({k:v for k,v in report.items() if k not in ['source_hashes','instruction_coverage','case']},indent=2));sys.exit(0 if report['passed'] else 1)
