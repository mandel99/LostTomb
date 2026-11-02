"""Compare reconstructed effects/frame control with isolated original x86.

Pixels and RNG/history are compared exactly. External city rendering and COM
presentation are controlled endpoints; no game process, window or device runs.
"""
from pathlib import Path
import hashlib,json,struct,subprocess,sys,time
ROOT=Path(__file__).resolve().parent.parent
sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE,UC_HOOK_BLOCK,__version__ as unicorn_version
from unicorn.x86_const import UC_X86_REG_EAX,UC_X86_REG_EIP,UC_X86_REG_ESP,UC_X86_REG_EBP,UC_X86_REG_ECX,UC_X86_REG_EDX,UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI
image=(ROOT/'build/smoke-game/Pharaoh.exe').read_bytes()
assert hashlib.sha256(image).hexdigest()=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
u16=lambda b,p:struct.unpack_from('<H',b,p)[0]
u32=lambda b,p:struct.unpack_from('<I',b,p)[0]
pe=u32(image,0x3c);opt=pe+24;table=opt+u16(image,pe+20);base=u32(image,opt+28);size=u32(image,opt+56)
vm=Uc(UC_ARCH_X86,UC_MODE_32);vm.mem_map(base,(size+4095)&~4095);vm.mem_write(base,image[:u32(image,opt+60)])
for i in range(u16(image,pe+6)):
 p=table+i*40;rva=u32(image,p+12);raw=u32(image,p+20);length=u32(image,p+16);vm.mem_write(base+rva,image[raw:raw+length])
OBJECTS=0x30000000;FRONT=OBJECTS+0x100;BACK=OBJECTS+0x200;LOCAL=OBJECTS+0x800;CONTEXT=0x6de518
FRONT_END=0x31041000;BACK_END=0x31141000;HOOKS=0x32000000;STOP=0x33000000;STACK=0x34000000
for address,length in [(OBJECTS,0x10000),(0x31001000,0x40000),(0x31101000,0x40000),(HOOKS,0x1000),(STOP,0x1000),(STACK,0x100000)]:vm.mem_map(address,length)
def read(a,n=4):return int.from_bytes(vm.mem_read(a,n),'little')
def write(a,v,n=4):vm.mem_write(a,(int(v)&((1<<(n*8))-1)).to_bytes(n,'little'))
def signed(v):return v-0x100000000 if v&0x80000000 else v
def hash_bytes(data):
 value=2166136261
 for byte in data:value=((value^byte)*16777619)&0xffffffff
 return value
write(FRONT,OBJECTS+0x1000);write(BACK,OBJECTS+0x1000)
write(OBJECTS+0x1014,HOOKS);write(OBJECTS+0x101c,HOOKS+16);write(0x56f22c,HOOKS+32)
RANGES=[(0x4d0040,0x4d0056),(0x4d0280,0x4d0392),(0x4d03f0,0x4d047a),
 (0x4d05f0,0x4d062a),(0x4d0630,0x4d0646),(0x4d0650,0x4d0a30)]
ENDPOINTS={HOOKS:('blt',6,24),HOOKS+16:('fast',6,24),HOOKS+32:('ticks',0,0),0x51e990:('render',0,0),0x4cbea0:('copy',3,0)}
state={}
def guard(cpu,address,size,user):
 if address in ENDPOINTS:return
 if not any(start<=address and address+size-1<=end for start,end in RANGES):raise RuntimeError('Execution escaped validated instructions at '+hex(address))
def event(id,*args):state['events'].append([id]+[v&0xffffffff for v in args]+[0]*(7-len(args)))
def endpoint(cpu,address,size,user):
 name,n,cleanup=ENDPOINTS[address];esp=cpu.reg_read(UC_X86_REG_ESP);args=[read(esp+4+4*i) for i in range(n)];result=0
 if name=='ticks':result=state['tick']
 elif name=='render':
  event(1)
  if state['count']:p=BACK_END-2;write(p,read(p,2)^0x1234,2)
 elif name=='copy':
  assert args[2]==16
  vm.mem_write(args[1],bytes(vm.mem_read(args[0],16)))
 else:
  assert args[0]==FRONT
  if name=='blt':
   assert args[2]==BACK and not args[4] and not args[5]
   rect=args[3];id=2
  else:
   assert not args[1] and not args[2] and args[3]==BACK and args[4]==0xe39130 and not args[5]
   rect=args[4];id=3
  length=(state['count']+16)*2
  front=hash_bytes(vm.mem_read(FRONT_END-length,length));back=hash_bytes(vm.mem_read(BACK_END-length,length))
  event(id,read(CONTEXT),front,back,*[read(rect+i*4) for i in range(4)])
  result=0
 cpu.reg_write(UC_X86_REG_EAX,result);cpu.reg_write(UC_X86_REG_ESP,esp+4+cleanup);cpu.reg_write(UC_X86_REG_EIP,read(esp))
vm.hook_add(UC_HOOK_BLOCK,guard)
for address in ENDPOINTS:vm.hook_add(UC_HOOK_CODE,endpoint,begin=address,end=address)
DEFAULT=[0,1,17,33,2,0,0,0,1,17,0,0x12345678,99,0,0,1,1,0,0x10,0,0xfffffff0]
OP,FORMAT,WIDTH,HEIGHT,STEPS,BRIGHTNESS,RANDOMIZE,EFFECT,BUFFER,CLIP,ALTERNATE,SEED,HISTORY,GATE,FORCE,MODE,COUNTER,ELAPSED,TICK,DIRTY,PREVIOUS_TICK=range(21)
def native(fields,before,case):
 global state
 op=case[OP];width=signed(case[WIDTH]);height=signed(case[HEIGHT]);count=width*height if width>0 and height>0 else 0
 assert count<=0x40000//2-16
 state={'events':[],'tick':case[TICK],'count':count}
 for (a,w),value in zip(fields,before):write(a,value,w)
 write(0xe38db0,case[WIDTH]);write(0xe38dac,case[HEIGHT]);write(0xe38e5c,case[ALTERNATE]);write(0xe39178,FRONT);write(0xe39174,BACK)
 for i,v in enumerate([17,19,1041,787]):write(0xe39130+i*4,v)
 for end,prefix,back in [(FRONT_END,0xa100,False),(BACK_END,0xb200,True)]:
  data=struct.pack('<16H',*[prefix+i for i in range(16)])+struct.pack('<'+'H'*count,*[((~(i+case[SEED]) if back else i+case[SEED])&65535) for i in range(count)])
  vm.mem_write(end-len(data),data)
 self=LOCAL if op>=3 else CONTEXT;sp=STACK+0x80000
 for i,r in enumerate([UC_X86_REG_EAX,UC_X86_REG_EBX,UC_X86_REG_ECX,UC_X86_REG_EDX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP]):vm.reg_write(r,0x11111111*(i+1))
 vm.reg_write(UC_X86_REG_ESP,sp);vm.reg_write(UC_X86_REG_ECX,self)
 if op==2:entry=0x4d0280;args=[case[FORCE]];cleanup=0
 elif op in [1,4]:entry=0x4d05f0;args=[case[BUFFER],case[STEPS],case[BRIGHTNESS],case[RANDOMIZE],case[EFFECT]];cleanup=20
 else:entry=0x4d0650;args=[case[BUFFER]];cleanup=4
 for i,value in enumerate([STOP]+args):write(sp+i*4,value)
 vm.emu_start(entry,STOP,count=50000000)
 assert vm.reg_read(UC_X86_REG_EIP)==STOP,'instruction budget exhausted'
 assert vm.reg_read(UC_X86_REG_ESP)==sp+4+cleanup,'native method stack cleanup differs'
 for reg,value in [(UC_X86_REG_EBX,0x22222222),(UC_X86_REG_ESI,0x55555555),(UC_X86_REG_EDI,0x66666666),(UC_X86_REG_EBP,0x77777777)]:assert vm.reg_read(reg)==value,'callee-saved register differs'
 length=(count+16)*2
 return {'after':[read(a,w) for a,w in fields],'images':[bytes(vm.mem_read(end-length,length)).hex() for end in [FRONT_END,BACK_END]],'events':state['events']}
def case(**changes):
 value=DEFAULT.copy()
 for key,v in changes.items():value[globals()[key]]=v&0xffffffff
 return value
def cases():
 for op in [0,1,3,4]:
  for fmt in [-1,0,1,2]:
   for effect in [0,1]:
    for randomize in [0,1,0x100,0x101]:
     for buffer in [0,1]:yield case(OP=op,FORMAT=fmt,EFFECT=effect,RANDOMIZE=randomize,BUFFER=buffer)
 for steps in [-2147483648,-1,0,1,15,16,2147483647]:
  for brightness in [-2147483648,-100,-1,0,1,31,2147483647]:
   for op in [0,1]:yield case(OP=op,STEPS=steps,BRIGHTNESS=brightness,RANDOMIZE=1)
 for width,height in [(0,33),(-1,33),(17,0),(17,-1),(1,29),(17,30),(17,31),(17,33),(129,34)]:
  for clip in [-1,0,1,16,129]:
   for effect in [0,1]:yield case(WIDTH=width,HEIGHT=height,CLIP=clip,EFFECT=effect,ALTERNATE=0x100,RANDOMIZE=1)
 for seed in [0,1,0xffffffff,0x87654321]:
  for history in [0,98,99]:
   for fmt in [0,1]:yield case(SEED=seed,HISTORY=history,FORMAT=fmt,EFFECT=1,RANDOMIZE=1)
 for mode in [0,1,3,5,30,31,32,73,74,255]:
  for counter in [-1,0,1,135,136,272]:
   for gate in [0,1,0x100,0x10000,0x1010000]:yield case(OP=2,MODE=mode,COUNTER=counter,GATE=gate,FORCE=1)
 for elapsed in [0,17,18,49,50,0x7fffffff,0x80000000,0xffffffff]:
  for force in [0,1,0x100]:
   for dirty in [0,1]:yield case(OP=2,ELAPSED=elapsed,FORCE=force,DIRTY=dirty)
 for fmt in [0,1]:
  for effect in [0,1]:
   for buffer in [0,1]:yield case(FORMAT=fmt,EFFECT=effect,BUFFER=buffer,WIDTH=512,HEIGHT=158,CLIP=512,SEED=0)
executable=ROOT/'build/c-only/Release/restore_effects_verify.exe'
process=subprocess.Popen([str(executable),'--oracle-batch'],cwd=ROOT,stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True)
fields=json.loads(process.stdout.readline())['fields'];started=time.monotonic();checked=0;pixels_compared=0
sources=['src/unit_01786.c','src/unit_01789.c','src/unit_01790.c','src/unit_01791.c','include/graphics_session.h','tests/surface_effects.c','tools/verify_surface_effects_emulation.py']
report={'original_sha256':hashlib.sha256(image).hexdigest(),'method':'isolated x86 instruction bodies vs compiled C; exact image bytes, context, RNG/history and ordered presentation compared',
 'unicorn_version':unicorn_version,'compiled_C_oracle_sha256':hashlib.sha256(executable.read_bytes()).hexdigest(),
 'source_hashes':{s:hashlib.sha256((ROOT/s).read_bytes()).hexdigest() for s in sources},'observed_memory_fields':len(fields),
 'native_entries':['0x004d0280','0x004d05f0','0x004d0650'],'native_helpers_executed':['0x004d0040','0x004d03f0','0x004d0630'],
 'original_game_executed':False,'window_created':False,'real_graphics_device_used':False,'media_DLLs_loaded':False}
try:
 for current in cases():
  process.stdin.write(' '.join(map(str,current))+'\n');process.stdin.flush();line=process.stdout.readline()
  if not line:raise RuntimeError('C oracle stopped: '+process.stderr.read())
  c=json.loads(line);expected=native(fields,c.pop('before'),current)
  if c!=expected:
   differences={key:{'C':c[key],'native':expected[key]} for key in ['after','events'] if c[key]!=expected[key]}
   if c['images']!=expected['images']:
    differences['pixels']=[{'buffer':i,'different_bytes':sum(a!=b for a,b in zip(bytes.fromhex(actual),bytes.fromhex(wanted))),
     'first_differences':[{'byte':j,'C':a,'native':b} for j,(a,b) in enumerate(zip(bytes.fromhex(actual),bytes.fromhex(wanted))) if a!=b][:16]} for i,(actual,wanted) in enumerate(zip(c['images'],expected['images'])) if actual!=wanted]
   report.update(passed=False,checked=checked,case=current,differences=differences)
   if c['after']!=expected['after']:report['field_differences']=[{'address':hex(a),'width':w,'C':cv,'native':nv} for (a,w),cv,nv in zip(fields,c['after'],expected['after']) if cv!=nv]
   break
  checked+=1;pixels_compared+=(len(c['images'][0])//4-16)*2
  if checked%200==0:print('Compared',checked,'effect/frame cases.',flush=True)
 else:report.update(passed=True,checked=checked)
except Exception as error:report.update(passed=False,checked=checked,case=current,error=str(error),emulator_eip=hex(vm.reg_read(UC_X86_REG_EIP)))
finally:process.stdin.close();process.wait(timeout=10)
report['pixel_values_compared']=pixels_compared;report['elapsed_seconds']=round(time.monotonic()-started,3)
(ROOT/'metadata/surface-effects-emulation.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps(report,indent=2));sys.exit(0 if report['passed'] else 1)
