"""Optional isolated native comparison; original file is analysis input only."""
from pathlib import Path
import hashlib,json,struct,subprocess,sys,time
ROOT=Path(__file__).resolve().parent.parent
sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_BLOCK,__version__ as unicorn_version
from unicorn.x86_const import UC_X86_REG_EAX,UC_X86_REG_EBX,UC_X86_REG_EBP,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EIP,UC_X86_REG_ESP
image=(ROOT/'build/smoke-game/Pharaoh.exe').read_bytes()
assert hashlib.sha256(image).hexdigest()=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
u16=lambda b,p:struct.unpack_from('<H',b,p)[0]
u32=lambda b,p:struct.unpack_from('<I',b,p)[0]
pe=u32(image,0x3c);opt=pe+24;table=opt+u16(image,pe+20);base=u32(image,opt+28);size=u32(image,opt+56)
vm=Uc(UC_ARCH_X86,UC_MODE_32);vm.mem_map(base,(size+4095)&~4095);vm.mem_write(base,image[:u32(image,opt+60)])
for i in range(u16(image,pe+6)):
 p=table+i*40;rva=u32(image,p+12);raw=u32(image,p+20);length=u32(image,p+16);vm.mem_write(base+rva,image[raw:raw+length])
for address,size in [(0x30000000,0x10000),(0x33000000,0x1000),(0x34000000,0x10000)]:vm.mem_map(address,size)
def write(address,value):vm.mem_write(address,struct.pack('<I',value&0xffffffff))
def guard(cpu,address,size,user):
 if not 0x564a39<=address<=address+size-1<=0x564ab0:raise RuntimeError('Native execution escaped slot-selector body: '+hex(address))
vm.hook_add(UC_HOOK_BLOCK,guard)
exe=ROOT/'build/c-only-debug/Debug/restore_streams_verify.exe'
process=subprocess.Popen([str(exe),'--oracle-batch'],cwd=ROOT,stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True)
report={'format':1,'original_sha256':hashlib.sha256(image).hexdigest(),'unicorn_version':unicorn_version,'native_entry':'0x00564a39',
 'original_game_executed':False,'compiled_C_sha256':hashlib.sha256(exe.read_bytes()).hexdigest(),
 'scope':'Non-null, preallocated stream slots: selection, exact 32-byte reset, count boundaries and preserved registers; bootstrap/new allocation separately covered by real file tests'}
started=time.monotonic();checked=0;case=None
try:
 h=json.loads(process.stdout.readline());length=h['record_bytes']
 for count in [-1,0,1,20,255,256,300,512]:
  for free in [-1,0,1,count-1,count]:
   for seed in [0,37,255]:
    case=[count,free,seed];process.stdin.write(' '.join(str(v&0xffffffff) for v in case)+'\n');process.stdin.flush();c=json.loads(process.stdout.readline())
    memory=bytearray((i*13+seed)&255 for i in range(length))
    for i in range(512):
     write(h['table']+4*i,h['records']+32*i);struct.pack_into('<I',memory,32*i+12,0x83 if i&1 else 1)
    if 0<=free<512:struct.pack_into('<I',memory,32*free+12,0x410)
    vm.mem_write(h['records'],bytes(memory));write(0x1266a90,h['table']);write(0x1267aa0,count)
    sp=0x34008000;write(sp,0x33000000);vm.reg_write(UC_X86_REG_ESP,sp)
    saved=[(UC_X86_REG_EBX,0x11111111),(UC_X86_REG_EBP,0x22222222),(UC_X86_REG_ESI,0x33333333),(UC_X86_REG_EDI,0x44444444)]
    for r,v in saved:vm.reg_write(r,v)
    vm.emu_start(0x564a39,0x33000000,count=1000000)
    assert vm.reg_read(UC_X86_REG_EIP)==0x33000000 and vm.reg_read(UC_X86_REG_ESP)==sp+4
    assert all(vm.reg_read(r)==v for r,v in saved)
    expected={'selected':vm.reg_read(UC_X86_REG_EAX),'after':bytes(vm.mem_read(h['records'],length)).hex()}
    assert c==expected,'C/native slot state differs'
    checked+=1
 report.update(passed=True,checked=checked)
except Exception as error:report.update(passed=False,checked=checked,case=case,error=str(error),native_eip=hex(vm.reg_read(UC_X86_REG_EIP)))
finally:
 process.stdin.close();process.wait(timeout=10)
report['elapsed_seconds']=round(time.monotonic()-started,3)
report['source_hashes']={f:hashlib.sha256((ROOT/f).read_bytes()).hexdigest() for f in ['src/unit_02892.c','runtime/legacy_streams.c','include/legacy_streams.h','tests/legacy_streams.c','tools/verify_stream_slots_emulation.py']}
(ROOT/'metadata/stream-slots-emulation.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({k:v for k,v in report.items() if k!='source_hashes'},indent=2));sys.exit(0 if report['passed'] else 1)
