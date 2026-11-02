"""Compare five sound stream methods against bounded native x86; Miles is controlled."""
from pathlib import Path
import hashlib,json,struct,subprocess,sys,time
ROOT=Path(__file__).resolve().parent.parent
sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_BLOCK,UC_HOOK_CODE,__version__ as unicorn_version
from unicorn.x86_const import *
image=(ROOT/'build/smoke-game/Pharaoh.exe').read_bytes()
assert hashlib.sha256(image).hexdigest()=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
u16=lambda b,p:struct.unpack_from('<H',b,p)[0]
u32=lambda b,p:struct.unpack_from('<I',b,p)[0]
pe=u32(image,0x3c);opt=pe+24;table=opt+u16(image,pe+20);base=u32(image,opt+28);size=u32(image,opt+56)
vm=Uc(UC_ARCH_X86,UC_MODE_32);vm.mem_map(base,(size+4095)&~4095);vm.mem_write(base,image[:u32(image,opt+60)])
for i in range(u16(image,pe+6)):
 p=table+i*40;vm.mem_write(base+u32(image,p+12),image[u32(image,p+20):u32(image,p+20)+u32(image,p+16)])
BASE=0x30000000;HOOKS=0x32000000;STOP=0x33000000;STACK=0x34000000;BYTES=532
for a,n in [(BASE,0x2000),(HOOKS,0x1000),(STOP,0x1000),(STACK,0x10000)]:vm.mem_map(a,n)
def read(a):return u32(vm.mem_read(a,4),0)
def write(a,v):vm.mem_write(a,struct.pack('<I',v&0xffffffff))
slots=[(0x56f2d0,1),(0x56f290,3),(0x56f300,2),(0x56f29c,2),(0x56f304,1),(0x56f298,1),(0x56f28c,1)]
endpoints={}
for i,(slot,count) in enumerate(slots):
 address=HOOKS+i*16;write(slot,address);vm.mem_write(address,b'\xc3');endpoints[address]=(i+1,count)
state={}
def endpoint(cpu,a,n,user):
 op,count=endpoints[a];sp=cpu.reg_read(UC_X86_REG_ESP);args=[read(sp+4+i*4) for i in range(count)]
 state['events'].append([op]+args+[0]*(3-count))
 value=state['p'][5] if op==2 else state['p'][4] if op==6 else 0xf1234567 if op==7 else 0
 cpu.reg_write(UC_X86_REG_EAX,value);cpu.reg_write(UC_X86_REG_EIP,read(sp));cpu.reg_write(UC_X86_REG_ESP,sp+4+count*4)
def guard(cpu,a,n,user):
 if a not in endpoints and not 0x55f449<=a<=0x55f67b:raise RuntimeError('Native instruction escaped at '+hex(a))
vm.hook_add(UC_HOOK_BLOCK,guard)
for a in endpoints:vm.hook_add(UC_HOOK_CODE,endpoint,begin=a,end=a)
def native(p):
 global state
 state={'p':p,'events':[]};vm.mem_write(BASE,b'\xa5'*BYTES)
 for off,value in [(0x178,0xfedcba98),(0x200,p[2]),(0x14c,0x123),(0x154,0x234),(0x158,0x345)]+[(0x1d8+i*4,0xabc00000+i) for i in range(3)]:write(BASE+off,value)
 write(BASE+0x1d8+p[1]*4,p[3])
 entry=[0x55f449,0x55f561,0x55f5a1,0x55f5f4,0x55f639][p[0]]
 args=[BASE+0x1000,p[1],0xffffffff] if p[0]==0 else [p[1],0xfffffffb] if p[0]==4 else [p[1]]
 sp=STACK+0x8000
 for i,v in enumerate([STOP]+args):write(sp+i*4,v)
 saved=[(UC_X86_REG_EBX,0x11111111),(UC_X86_REG_EBP,0x22222222),(UC_X86_REG_ESI,0x33333333),(UC_X86_REG_EDI,0x44444444)]
 for reg,v in saved:vm.reg_write(reg,v)
 vm.reg_write(UC_X86_REG_ECX,BASE);vm.reg_write(UC_X86_REG_ESP,sp);vm.reg_write(UC_X86_REG_EFLAGS,2)
 vm.emu_start(entry,STOP,count=10000)
 assert vm.reg_read(UC_X86_REG_EIP)==STOP,'Instruction budget exhausted'
 assert vm.reg_read(UC_X86_REG_ESP)==sp+4+4*len(args),'Native stack cleanup differs'
 assert all(vm.reg_read(reg)==v for reg,v in saved),'Callee-saved register differs'
 return {'result':vm.reg_read(UC_X86_REG_EAX) if p[0] in [0,2,3] else 0,'events':state['events'],'object':bytes(vm.mem_read(BASE,BYTES))}
cases=[]
for op in range(5):
 for channel in range(3):
  for disabled in [0,1,0x100]:
   for old in [0,0x87654321]:
    for status in [0,1,4,8,0xffffffff]:
     for opened in [0,0xf7654321]:cases.append([op,channel,disabled,old,status,opened])
exe=ROOT/'build/c-only-debug/Debug/restore_sound_streams_verify.exe'
process=subprocess.Popen([str(exe),'--oracle-batch'],cwd=ROOT,stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE)
checked=0;current=None;started=time.monotonic()
report={'format':1,'unicorn_version':unicorn_version,'original_game_executed':False,'media_playback_used':False,
 'original_sha256':hashlib.sha256(image).hexdigest(),'compiled_C_sha256':hashlib.sha256(exe.read_bytes()).hexdigest(),
 'method':'Five native thiscall stream bodies against explicit-receiver C with controlled Miles endpoints',
 'compared':['All 516 object bytes and 16 guard bytes','Ordered media calls, all arguments and value returns','Native stack cleanup and preserved registers'],
 'limitations':['Native negative or out-of-range channels are unsafe and excluded. C rejects those separately. Actual audio device and codecs are covered only by separate startup runs.']}
try:
 assert json.loads(process.stdout.readline())=={'bytes':BYTES}
 for current in cases:
  process.stdin.write((' '.join(map(str,current))+'\n').encode());process.stdin.flush()
  actual=json.loads(process.stdout.readline());actual['object']=process.stdout.read(BYTES);expected=native(current)
  if actual!=expected:
   report['differences']={k:{'C':actual[k],'native':expected[k]} for k in actual if k!='object' and actual[k]!=expected[k]}
   if actual['object']!=expected['object']:report['object_differences']=[{'offset':i,'C':a,'native':b} for i,(a,b) in enumerate(zip(actual['object'],expected['object'])) if a!=b][:16]
   raise AssertionError('Stream results differ')
  checked+=1
 report.update(passed=True,checked=checked)
except Exception as error:report.update(passed=False,checked=checked,case=current,error=str(error),native_eip=hex(vm.reg_read(UC_X86_REG_EIP)))
finally:
 process.stdin.close()
 try:process.wait(timeout=10)
 except subprocess.TimeoutExpired:process.kill();process.wait()
sources=['src/unit_%05d.c'%i for i in range(2707,2712)]+['runtime/sound_streams.c','include/sound_streams.h','include/sound_initialization.h','tests/sound_streams.c','tools/verify_sound_streams_emulation.py']
report['source_hashes']={s:hashlib.sha256((ROOT/s).read_bytes()).hexdigest() for s in sources}
report['elapsed_seconds']=round(time.monotonic()-started,3)
(ROOT/'metadata/sound-streams-emulation.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({k:v for k,v in report.items() if k!='source_hashes'},indent=2));sys.exit(0 if report['passed'] else 1)
