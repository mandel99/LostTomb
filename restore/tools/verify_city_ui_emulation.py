"""Composed native/C comparison of ten HUD/side-panel controller bodies.

The canonical executable is data only. Its selected original instructions run
inside Unicorn. Raster, viewport, numeric/text and widget drawing primitives
are controlled endpoints with ordered argument and text-width side effects.
"""
from pathlib import Path
import argparse,hashlib,json,random,re,struct,subprocess,sys,time
ROOT=Path(__file__).resolve().parents[1]
sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE,__version__
from unicorn.x86_const import *
parser=argparse.ArgumentParser(description=__doc__)
parser.add_argument('--exe',type=Path,default=ROOT/'build/agent-city-ui/Debug/restore_city_ui_verify.exe')
args=parser.parse_args();exe=args.exe.resolve()
image=(ROOT/'build/smoke-game/Pharaoh.exe').read_bytes();SHA=hashlib.sha256(image).hexdigest()
assert SHA=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
entries=[0x51e990,0x51ec90,0x51ed30,0x51edd0,0x51efb0,0x51f0a0,0x51f680,0x533e70,0x537100,0x537120]
functions={}
for filename in ['metadata/city-ui-next-native.json','metadata/city-ui-children-native.json']:
 d=json.loads((ROOT/filename).read_text(encoding='utf-8-sig'));assert d['native_sha256']==SHA
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
regions=[(int(a,16),int(n,16)) for a,n in re.findall(r'\{0x([0-9a-f]+),0x([0-9a-f]+)\}',(ROOT/'tests/city_ui_fixture.h').read_text())]
STOP=0x33000000;STACK=0x34000000
vm.mem_map(STOP,0x1000);vm.mem_map(STACK,0x10000)
ENDPOINTS={0x4dddc0:0,0x425900:0,0x51f780:0,0x425330:2,0x536670:2,0x425730:4,0x43e810:4,
 0x4cd510:5,0x4cd9d0:5,0x4cda40:5,0x4cc770:5,0x4d62f0:8,0x4cce90:8,0x4d6250:9,0x4d6860:9,0x4cccb0:6,0x53e590:3,0x44bd00:0}
TEXT={0x4d62f0,0x4d6250,0x4d6860,0x4cce90,0x4cccb0,0x4cc770}
state={};executed=set()
def read(a,n=4):return int.from_bytes(vm.mem_read(a,n),'little')
def write(a,v,n=4):vm.mem_write(a,(v&((1<<(n*8))-1)).to_bytes(n,'little'))
def hook(cpu,a,n,user):
 if a in allowed:executed.add(a);return
 assert a in ENDPOINTS,'Unexpected native instruction '+hex(a)
 sp=cpu.reg_read(UC_X86_REG_ESP);count=ENDPOINTS[a];p=[read(sp+4+4*i) for i in range(count)]
 if a==0x53e590:assert cpu.reg_read(UC_X86_REG_ECX)==0xf55bb0
 state['events'].append([a,count]+p+[0]*(9-count))
 if a in TEXT:write(0xe20778,(p[0]&15)+7+(512 if state['variant']==2 else 0))
 if a==0x53e590 and state['variant']==1:write(0xe38db0,800);write(0xf53f78,1)
 cpu.reg_write(UC_X86_REG_EAX,state['predicate'] if a==0x44bd00 else 0x9abcdef0)
 cpu.reg_write(UC_X86_REG_ECX,0xa17e4567);cpu.reg_write(UC_X86_REG_EDX,0x5a5a5a5a)
 cpu.reg_write(UC_X86_REG_EIP,read(sp));cpu.reg_write(UC_X86_REG_ESP,sp+4+(12 if a==0x53e590 else 0))
vm.hook_add(UC_HOOK_CODE,hook)
def native(case):
 entry,p1,p2,variant,predicate,buffers=case;state.clear();state.update(events=[],variant=variant,predicate=predicate)
 for (a,n),b in zip(regions,buffers):assert len(b)==n;vm.mem_write(a,bytes(b))
 sp=STACK+0x8000;vm.mem_write(sp-0x200,b'\xcc'*0x400)
 for i,v in enumerate([STOP,p1,p2]):write(sp+4*i,v)
 saved=[(UC_X86_REG_EBX,0x11223344),(UC_X86_REG_EBP,0x22334455),(UC_X86_REG_ESI,0x33445566),(UC_X86_REG_EDI,0x44556677)]
 for r,v in saved:vm.reg_write(r,v)
 vm.reg_write(UC_X86_REG_ESP,sp);vm.reg_write(UC_X86_REG_EFLAGS,2)
 vm.emu_start(entry,STOP,count=100000)
 assert vm.reg_read(UC_X86_REG_EIP)==STOP
 assert vm.reg_read(UC_X86_REG_ESP)==sp+4
 assert all(vm.reg_read(r)==v for r,v in saved)
 assert bytes(vm.mem_read(sp+12,0x100))==b'\xcc'*0x100
 return dict(state=''.join(bytes(vm.mem_read(a,n)).hex() for a,n in regions),events=state['events'])
def put(buffers,a,v,n=4):
 for (lo,size),b in zip(regions,buffers):
  if lo<=a and a+n<=lo+size:b[a-lo:a-lo+n]=(v&((1<<(8*n))-1)).to_bytes(n,'little');return
 raise ValueError('Fixture address '+hex(a))
def fixture():
 b=[bytearray(b'\x5a'*n) for a,n in regions]
 values={0x605a3c:0,0x6059e4:0,0x605a30:0,0x605a34:999,0x7056ec:0,0x78d7bc:300,0x78d7c0:700,
 0xbc968c:1,0xbc9694:1,0xe38db0:1024,0xe38dac:768,0xe38d98:759,0xe38da4:300,0xe38da8:400,
 0xe38dcc:0,0xe38ebc:0,0xe94e0c:0,0xe951d8:300,0xe951ec:900,0xeaac28:0,
 0xea187c:12345,0xea1894:67890,0xea6254:54321,0xea626c:98765,0xec1d64:11234,0xec1d7c:55678,
 0xf53da0:0,0xf53da4:0,0xf53da8:0,0xf53dac:0,0xf53db0:0,0xf53f60:1000,0xf53f64:864,0xf53f74:1,0xf53f78:0,0xf53fd0:1,
 0xf55c64:10000,0xf567c4:11000,0xf567e4:12000,0xf56f84:313,0xf56f8c:314,0xf56f9c:315,0xf56fa4:316}
 for a,v in values.items():put(b,a,v)
 put(b,0xe38dda,1,1);put(b,0xe38dc9,0,1);put(b,0x784cb0,2,1);put(b,0x784b94,0,2)
 for a in range(0x784bc0,0x784c00,2):put(b,a,0xffff,2)
 for a in [0x784c60,0x784c62,0x784c64,0x784c66,0x784c68,0x784c6a,0x784c8c,0x784c8e,0x784c90,0x784c92]:put(b,a,0xffff,2)
 return b
cases=[]
def add(entry=0x51e990,p1=1,p2=875,variant=0,predicate=1,changes=None):
 b=fixture()
 for item in changes or []:put(b,*item)
 cases.append((entry,p1&0xffffffff,p2&0xffffffff,variant,predicate,b))
for width,height in [(640,480),(800,600),(1024,768),(1280,960)]:
 for mode in [0,1,2,256]:
  for collapsed in [0,1,2,256]:
   for variant in [0,1,2]:add(variant=variant,changes=[(0xe38db0,width),(0xe38dac,height),(0x605a3c,mode),(0xf53f78,collapsed)])
for entry in entries:
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
 add(rng.choice(entries),p1=rng.choice([0,1,256]),p2=rng.choice([400,875,1000]),variant=rng.randrange(3),predicate=rng.randrange(2),changes=[(0xe38db0,rng.choice([640,800,1024])),(0xf53f64,rng.choice([400,640,864])),(0xf53f78,rng.randrange(3)),(0x605a3c,rng.randrange(3))])
process=subprocess.Popen([str(exe),'--oracle-batch'],cwd=ROOT,stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True)
report=dict(format=1,passed=False,checked=0,native_functions=len(entries),original_sha256=SHA,compiled_C_sha256=hashlib.sha256(exe.read_bytes()).hexdigest(),original_game_executed=False,unicorn_version=__version__,
 method='Compose all ten original HUD controllers and all ten compiled C bodies; compare every fixture-region byte and ordered external call arguments, including text-width output mutations and full image/coordinate IDs. Native and compiled C callee-saved registers and stack cleanup checked.',
 limitations=['Viewport/minimap rasterization, sprite rendering, primitive drawing, widget drawing, text/numeric rendering and predicate0044bd00 are controlled endpoints. Pixel correctness and complete city rendering are not established.',
 'Endpoint0051f780 is the existing empty helper, modeled as a controlled zero-argument endpoint. These ten source routines have void interfaces; incidental native EAX is not compared.',
 'Faction fixture indices are0,1,7; no bounds validation absent from the original is added.'])
start=time.monotonic();case=None
try:
 assert json.loads(process.stdout.readline())==dict(ready=True)
 for case in cases:
  entry,p1,p2,variant,predicate,buffers=case
  process.stdin.write(' '.join(map(str,[entry,p1,p2,variant,predicate]))+'\n'+'\n'.join(b.hex() for b in buffers)+'\n');process.stdin.flush()
  line=process.stdout.readline()
  if not line:raise RuntimeError('C oracle stopped: '+process.stderr.read())
  actual=json.loads(line);expected=native(case)
  if actual!=expected:
   report['differences']={k:dict(C=actual[k],native=expected[k]) for k in expected if actual[k]!=expected[k]}
   if actual['state']!=expected['state']:
    offset=0;diffs=[]
    for lo,n in regions:
     for i in range(n):
      a=int(actual['state'][2*(offset+i):2*(offset+i)+2],16);b=int(expected['state'][2*(offset+i):2*(offset+i)+2],16)
      if a!=b:diffs.append(dict(address=hex(lo+i),C=a,native=b))
     offset+=n
    report['state_first_differences']=diffs[:12]
   raise AssertionError('State or endpoint argument/order differs')
  report['checked']+=1
 report['passed']=True
except Exception as error:
 report['error']=str(error);report['case']=dict(entry=hex(case[0]),p1=case[1],p2=case[2],variant=case[3],predicate=case[4]) if case else None
 report['native_eip']=hex(vm.reg_read(UC_X86_REG_EIP))
finally:
 process.stdin.close();process.wait(timeout=10)
report['elapsed_seconds']=round(time.monotonic()-start,3)
report['instruction_coverage']={f'{entry:08x}':dict(total=len(body),executed=sum(int(i['address'],16) in executed for i in body),missing=[i for i in body if int(i['address'],16) not in executed]) for entry,body in functions.items()}
report['total_instruction_coverage']=dict(total=sum(len(b) for b in functions.values()),executed=len(executed))
if report['passed'] and any(c['missing'] for c in report['instruction_coverage'].values()):
 report['passed']=False;report['error']='Target native instruction coverage incomplete'
report['source_hashes']={p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest() for p in [f'src/unit_{n}.c' for n in ['02344','02345','02346','02347','02348','02349','02350','02433','02454','02455']]+['include/city_ui_controllers.h','tests/city_ui_controllers.c','tests/city_ui_fixture.h','tools/verify_city_ui_emulation.py','metadata/city-ui-next-native.json','metadata/city-ui-children-native.json']}
(ROOT/'metadata/city-ui-emulation.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({k:v for k,v in report.items() if k not in ['source_hashes','differences','instruction_coverage']},indent=2))
print(json.dumps({k:{x:y for x,y in v.items() if x!='missing'} for k,v in report['instruction_coverage'].items()},indent=2))
sys.exit(0 if report['passed'] else 1)
