"""Optional independent SG3 reader comparison with bounded original x86.

Windows/filesystem/allocator endpoints are controlled; native tag-list and
pixel-conversion instructions really execute. No game process is started.
"""
from pathlib import Path
import hashlib,json,struct,subprocess,sys,time
ROOT=Path(__file__).resolve().parent.parent
sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_BLOCK,UC_HOOK_CODE,__version__ as unicorn_version
from unicorn.x86_const import UC_X86_REG_EAX,UC_X86_REG_EBX,UC_X86_REG_ECX,UC_X86_REG_EDX,UC_X86_REG_EBP,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_ESP,UC_X86_REG_EIP
image=(ROOT/'build/smoke-game/Pharaoh.exe').read_bytes()
assert hashlib.sha256(image).hexdigest()=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
u16=lambda b,p:struct.unpack_from('<H',b,p)[0]
u32=lambda b,p:struct.unpack_from('<I',b,p)[0]
pe=u32(image,0x3c);opt=pe+24;table=opt+u16(image,pe+20);base=u32(image,opt+28);size=u32(image,opt+56)
vm=Uc(UC_ARCH_X86,UC_MODE_32);vm.mem_map(base,(size+4095)&~4095);vm.mem_write(base,image[:u32(image,opt+60)])
for i in range(u16(image,pe+6)):
 p=table+i*40;rva=u32(image,p+12);raw=u32(image,p+20);length=u32(image,p+16);vm.mem_write(base+rva,image[raw:raw+length])
OBJECT=0x30000000;IMAGES=0x30010000;TAGS=0x30020000;NAME=0x30021000;HEAP=0x31000000;STOP=0x33000000;STACK=0x34000000
for a,n in [(0,0x1000),(OBJECT,0x30000),(HEAP,0x200000),(STOP,0x1000),(STACK,0x200000)]:vm.mem_map(a,n)
def read(a,n=4):return int.from_bytes(vm.mem_read(a,n),'little')
def write(a,v,n=4):vm.mem_write(a,(v&((1<<(8*n))-1)).to_bytes(n,'little'))
def cstring(a):
 data=bytes(vm.mem_read(a,260));return data[:data.index(0)].decode('ascii')
ENDPOINTS={0x561a1d,0x561a2b,0x4ceb60,0x4cea30}
RANGES=[(0x53dff0,0x53e52c),(0x53f660,0x53f78e),(0x53f790,0x53f7c6),
 (0x5404b0,0x5404df),(0x540540,0x54056f),(0x540570,0x54058f),(0x540590,0x5405cf),
 (0x561f30,0x561f5e),(0x4cdeb0,0x4cdf1f),(0x4cdf20,0x4cdf67)]
state={}
def guard(cpu,a,n,user):
 if a not in ENDPOINTS and not any(lo<=a and a+n-1<=hi for lo,hi in RANGES):raise RuntimeError('Native instructions escaped at '+hex(a))
def endpoint(cpu,a,n,user):
 sp=cpu.reg_read(UC_X86_REG_ESP);args=[read(sp+4+i*4) for i in range(3)];value=0
 if a==0x561a1d:
  value=state['heap'];state['heap']+=(args[0]+15)&~15
  assert state['heap']<HEAP+0x200000;vm.mem_write(value,b'\xcd'*args[0])
 elif a!=0x561a2b:
  path=cstring(args[0]);data=state['metadata'] if path.endswith('.sg3') else state['pixels']
  assert path in ['data\\fixture.sg3','data\\fixture.555'],path
  if a==0x4ceb60:vm.mem_write(args[1],bytes(data[args[2]:]));value=1
  else:value=len(data)-args[1]
 cpu.reg_write(UC_X86_REG_EAX,value);cpu.reg_write(UC_X86_REG_EIP,read(sp));cpu.reg_write(UC_X86_REG_ESP,sp+4)
vm.hook_add(UC_HOOK_BLOCK,guard)
for address in ENDPOINTS:vm.hook_add(UC_HOOK_CODE,endpoint,begin=address,end=address)
def native(special,previous,fmt):
 global state
 skip=200 if special else 0
 metadata=bytearray(0x9f28+(skip+4)*64);pixels=bytearray(b'\xa5'*((0x992e if special else 0)+16))
 struct.pack_into('<II',metadata,16,skip+4,2)
 metadata[0x2a8:0x2a8+11]=b'system.bmp\0';metadata[0x2a8+200:0x2a8+210]=b'other.bmp\0'
 for i,v in enumerate([1,3,2,1]):struct.pack_into('<h',metadata,0x50+i*2,skip+v)
 start=0x9f28+skip*64
 struct.pack_into('<I',metadata,start+4,4);metadata[start+0x35]=1
 struct.pack_into('<I',metadata,start+64+4,3);metadata[start+64+0x33]=1
 struct.pack_into('<II',metadata,start+128+4,5,2);metadata[start+128+0x32]=30;metadata[start+128+0x35]=1;metadata[start+192+0x34]=1
 p=0x992e if special else 0;pixels[p:p+16]=bytes.fromhex('0000000034121ff80111112222013333')
 state={'metadata':metadata,'pixels':pixels,'heap':HEAP}
 vm.mem_write(OBJECT,b'\0'*0xcc3c);write(OBJECT+0xcc2c,previous);vm.mem_write(IMAGES,b'\0'*512);vm.mem_write(TAGS,b'\0'*32);vm.mem_write(NAME,b'fixture\0')
 write(0,0xffffffff);write(0xe92dfc,fmt);write(0xe92e1c,0)
 sp=STACK+0x180000
 for i,value in enumerate([STOP,NAME,7 if special else 0,IMAGES,11,TAGS,21]):write(sp+i*4,value)
 saved=[(UC_X86_REG_EBX,0x11111111),(UC_X86_REG_EBP,0x22222222),(UC_X86_REG_ESI,0x33333333),(UC_X86_REG_EDI,0x44444444)]
 for reg,value in saved:vm.reg_write(reg,value)
 vm.reg_write(UC_X86_REG_ECX,OBJECT);vm.reg_write(UC_X86_REG_ESP,sp)
 vm.emu_start(0x53dff0,STOP,count=2000000)
 assert vm.reg_read(UC_X86_REG_EIP)==STOP,'Instruction limit exhausted'
 assert vm.reg_read(UC_X86_REG_ESP)==sp+28,'thiscall cleanup differs'
 assert all(vm.reg_read(r)==v for r,v in saved),'Saved register differs'
 record=bytearray(vm.mem_read(OBJECT,0xcc3c));pixel_base=u32(record,0xcc24);pixel_bytes=u32(record,0xcc28)
 output=bytearray(vm.mem_read(IMAGES,512));struct.pack_into('<I',record,0xcc24,1 if pixel_base else 0)
 for i in range(8):
  address=u32(output,i*64)
  if pixel_base and not output[i*64+0x34] and address>=pixel_base:struct.pack_into('<I',output,i*64,address-pixel_base)
 return {'result':vm.reg_read(UC_X86_REG_EAX)&255,'record':record.hex(),'images':output.hex(),'tags':bytes(vm.mem_read(TAGS,32)).hex(),
  'pixels':bytes(vm.mem_read(pixel_base,pixel_bytes)).hex(),'cursor':read(0xd3b5f4)-pixel_base}
exe=ROOT/'build/c-only-debug/Debug/restore_sprites_verify.exe'
process=subprocess.Popen([str(exe),'--oracle-batch'],cwd=ROOT,stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True)
report={'format':1,'unicorn_version':unicorn_version,'original_game_executed':False,'native_entry':'0x0053dff0',
 'method':'Original SG3 reader, list helpers, stack probing, pixel loader and color converters versus compiled C; allocator and filesystem endpoints controlled',
 'compiled_C_sha256':hashlib.sha256(exe.read_bytes()).hexdigest(),'original_sha256':hashlib.sha256(image).hexdigest(),
 'allocation_trace_equivalent':False,'limitation':'C replaces the native tag-node list with the same ordered pass; allocator call counts and malformed-file behavior differ intentionally'}
checked=0;case=None;started=time.monotonic()
try:
 json.loads(process.stdout.readline())
 for special in [0,1]:
  for previous in [0,1,4,8]:
   for fmt in [0,1,2,3,0x102]:
    case=[special,previous,fmt];process.stdin.write(' '.join(map(str,case))+'\n');process.stdin.flush();actual=json.loads(process.stdout.readline());expected=native(*case)
    if actual!=expected:
     report['differences']={k:{'C':actual[k][:180] if isinstance(actual[k],str) else actual[k],'native':expected[k][:180] if isinstance(expected[k],str) else expected[k]} for k in actual if actual[k]!=expected[k]}
     for key in ['record','images','tags','pixels']:
      if actual[key]!=expected[key]:report[key+'_first_bytes']=[{'offset':i,'C':a,'native':b} for i,(a,b) in enumerate(zip(bytes.fromhex(actual[key]),bytes.fromhex(expected[key]))) if a!=b][:12]
     raise AssertionError('Normalized state or exact pixel bytes differ')
    checked+=1
 report.update(passed=True,checked=checked)
except Exception as error:report.update(passed=False,checked=checked,case=case,error=str(error),native_eip=hex(vm.reg_read(UC_X86_REG_EIP)))
finally:
 process.stdin.close();process.wait(timeout=10)
report['elapsed_seconds']=round(time.monotonic()-started,3)
files=['src/unit_02517.c','src/unit_02518.c','src/unit_02519.c','src/unit_02520.c','src/unit_02522.c','src/unit_02528.c','src/unit_02529.c','src/unit_01733.c','src/unit_01734.c','include/sprite_libraries.h','runtime/sprite_libraries.c','tests/sprite_libraries.c','tools/verify_sprite_reader_emulation.py']
report['source_hashes']={f:hashlib.sha256((ROOT/f).read_bytes()).hexdigest() for f in files}
(ROOT/'metadata/sprite-reader-emulation.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({k:v for k,v in report.items() if k!='source_hashes'},indent=2));sys.exit(0 if report['passed'] else 1)
