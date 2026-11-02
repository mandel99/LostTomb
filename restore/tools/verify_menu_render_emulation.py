"""Compare menu layout, segment selection, measurement and glyph placement with native x86."""
from pathlib import Path
import hashlib,json,struct,subprocess,sys
ROOT=Path(__file__).resolve().parent.parent;sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_BLOCK,UC_HOOK_CODE
from unicorn.x86_const import *
image=(ROOT/'build/smoke-game/Pharaoh.exe').read_bytes()
assert hashlib.sha256(image).hexdigest()=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
u16=lambda b,p:struct.unpack_from('<H',b,p)[0];u32=lambda b,p:struct.unpack_from('<I',b,p)[0]
pe=u32(image,0x3c);opt=pe+24;table=opt+u16(image,pe+20);base=u32(image,opt+28);size=u32(image,opt+56)
vm=Uc(UC_ARCH_X86,UC_MODE_32);vm.mem_map(base,(size+4095)&~4095)
for i in range(u16(image,pe+6)):
 p=table+i*40;vm.mem_write(base+u32(image,p+12),image[u32(image,p+20):u32(image,p+20)+u32(image,p+16)])
NAME=0x30000000;STOP=0x33000000;STACK=0x34000000
for a in [NAME,STOP,STACK]:vm.mem_map(a,0x10000)
def write(a,v,n=4):vm.mem_write(a,(v&((1<<(n*8))-1)).to_bytes(n,'little'))
def read(a):return int.from_bytes(vm.mem_read(a,4),'little')
events=[]
def endpoint(cpu,a,n,user):
 sp=cpu.reg_read(UC_X86_REG_ESP);count=3 if a==0x53e590 else 7;args=[read(sp+4+i*4) for i in range(count)]
 events.append([a,count]+args+[0]*(8-count))
 cpu.reg_write(UC_X86_REG_EAX,1);cpu.reg_write(UC_X86_REG_EIP,read(sp));cpu.reg_write(UC_X86_REG_ESP,sp+4+(12 if count==3 else 0))
ranges=[(0x4180c0,0x418138),(0x4256a0,0x42584f),(0x536710,0x53681f),(0x536820,0x536b9f),(0x4cbf30,0x4cbff2),(0x4cca60,0x4ccafd)]
def guard(cpu,a,n,user):
 if a not in [0x53e590,0x4cd240] and not any(lo<=a and a+n-1<=hi for lo,hi in ranges):raise RuntimeError('Escaped native menu at '+hex(a))
vm.hook_add(UC_HOOK_BLOCK,guard)
for a in [0x53e590,0x4cd240]:vm.hook_add(UC_HOOK_CODE,endpoint,begin=a,end=a)
def native(p):
 events.clear()
 for i in range(2048):
  write(0x10d6a0c+i*64,(i*3+p[6])%19+1,2);write(0x10d6a0e+i*64,(i*7+p[6])%27+1,2)
 vm.mem_write(0x5e047c,b'\0'*256)
 for i in range(3):write(0x5e047c+65+i,3+i,1)
 for a,v in [(0x126517c,p[7]),(0xf56fb4,871),(0xf56fbc,1005),(0xf56f84,201),(0xf56f8c,p[1]),(0xd3b500,11),(0xd3b5c0,1234),(0xd3b5e0,4567),(0xd3b5dc,8910),(0xf56494,6000),(0xe38da8,p[2]),(0xe38da4,p[3]),(0x7056c8,p[5])]:write(a,v)
 write(0xf5683c,p[6]);write(0xf56844,p[6])
 vm.mem_write(NAME+0x100,b'\xa5'*128);write(0xe38dc8,0xa5000000);write(0xe38dc9,bool(p[1]&1),1);write(0xe38dca,bool(p[1]&2),1)
 for i in range(8):write(0xf55bb4+i*8,p[6]+i*1000)
 if p[0]==6:
  for i in range(3):
   a=NAME+0x100+i*36;write(a,i*17,2);write(a+2,i*31-10,2);write(a+10,i,2);write(a+12,100+i*4)
   write(a+24,bool(p[1]&4),1);write(a+25,p[5],1);write(a+26,p[5]>>8,1);write(a+27,p[5]>>16,1)
 elif p[0]==7:
  for i in range(3):
   a=NAME+0x100+i*24;write(a,i*17,2);write(a+2,i*31-10,2);write(a+4,31234+i,2)
   write(a+12,p[5],1);write(a+13,p[5]>>8,1);write(a+15,p[1]>>3,1)
 for i in range(20):write(0xf53f10+i*4,100)
 for i,value in enumerate([4,5,14,15],1):write(0xf53f10+i*4,value)
 for i in range(9):
  for a,v in [(0x5f81e4,i),(0x5f81e8,i-4),(0x5f81ec,i-3)]:write(a+i*12,v)
 vm.mem_write(NAME,[b'',b'ABC',b'A B',b'ABC ABC'][p[2]%4]+b'\0')
 op=p[0];entry=[0x4cbf30,0x4cca60,0x536710,0x4180c0,0x536820,0x536a30,0x425730,0x4256a0][op]
 args=[NAME,p[1]] if op==0 else [p[1],p[2],p[3],p[4]] if op==1 else p[1:6] if op==2 else p[2:6] if op in [4,5] else []
 if op in [6,7]:args=[p[2],p[3],NAME+0x100,p[4]]
 sp=STACK+0x8000
 for i,v in enumerate([STOP]+args):write(sp+i*4,v)
 saved=[(UC_X86_REG_EBX,0x11111111),(UC_X86_REG_EBP,0x22222222),(UC_X86_REG_ESI,0x33333333),(UC_X86_REG_EDI,0x44444444)]
 for reg,v in saved:vm.reg_write(reg,v)
 vm.reg_write(UC_X86_REG_ESP,sp);vm.reg_write(UC_X86_REG_EFLAGS,2);vm.emu_start(entry,STOP,count=100000)
 assert vm.reg_read(UC_X86_REG_EIP)==STOP and vm.reg_read(UC_X86_REG_ESP)==sp+4,'Native stack or budget differs'
 assert all(vm.reg_read(reg)==v for reg,v in saved)
 return {'result':vm.reg_read(UC_X86_REG_EAX) if op<2 else 0,'state':[read(a) for a in [0xd3b5c0,0xd3b5e0,0xd3b5dc,0xe38dc8]]+[read(NAME+0x100+i) for i in range(0,128,4)],'events':events.copy()}
cases=[]
for font in [201,335,871,1005]:
 for s in range(4):cases.append([0,font,s,0,0,0,7,2048])
 for char in [0,33,34,35,127,199,231]:
  for limit in [100,2048]:cases.append([1,font,char,500,300,0,7,limit])
for style in [1,2,3,4,6]:
 for segments in [0,1,2,16,90]:
  for hover in [0,1]:cases.append([2,style,192,144,segments,hover,7,2048])
for x,y in [(0,0),(80,60),(192,144)]:
 for hover in range(6):cases.append([3,335,x,y,0,hover,7,2048])
for op in [4,5]:
 for width in [0,1,2,7,12,24,36]:
  for height in [0,1,2,8,14,24,36]:
   for root in [255,6000,35000]:cases.append([op,335,320,304,width,height,root,2048])
for flags in range(8):
 for pressed in [0,1,20]:
  for hover in [0,1,2]:
   for animation in [0,1,2,3,4,63,64,255]:
    for count in [0,1,3]:cases.append([6,flags,656,393,count,pressed|(hover<<8)|(animation<<16),35000,2048])
for kind in [0,1,4,5,6]:
 for pressed in [0,1,20]:
  for hover in [0,1]:cases.append([7,kind<<3,656,393,3,pressed|(hover<<8),35000,2048])
exe=ROOT/'build/c-only-debug/Debug/restore_menu_render_verify.exe'
process=subprocess.Popen([str(exe),'--oracle-batch'],cwd=ROOT,stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True)
report={'format':1,'original_game_executed':False,'real_rendering_used':False,'method':'Bounded native menu rectangles, segment IDs, text measurement and glyph positions against C; draw/label endpoints controlled','limitations':['This comparison does not prove bitmap contents, complete text styling or menu actions; live C framebuffer inspection is separate.'],'compiled_C_sha256':hashlib.sha256(exe.read_bytes()).hexdigest()}
checked=0;p=None
try:
 assert json.loads(process.stdout.readline())=={'fields':36}
 for p in cases:
  process.stdin.write(' '.join(map(str,p))+'\n');process.stdin.flush();actual=json.loads(process.stdout.readline());expected=native(p)
  assert actual==expected,repr({'C':actual,'native':expected})
  checked+=1
 report.update(passed=True,checked=checked)
except Exception as error:report.update(passed=False,checked=checked,case=p,error=str(error),native_eip=hex(vm.reg_read(UC_X86_REG_EIP)))
finally:process.stdin.close();process.wait(timeout=10)
report['source_hashes']={s:hashlib.sha256((ROOT/s).read_bytes()).hexdigest() for s in ['src/unit_00219.c','src/unit_00400.c','src/unit_00401.c','src/unit_02444.c','src/unit_02445.c','src/unit_02447.c','src/unit_01667.c','src/unit_01693.c','tests/menu_render.c','tools/verify_menu_render_emulation.py']}
(ROOT/'metadata/menu-render-emulation.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps(report,indent=2));sys.exit(0 if report['passed'] else 1)
