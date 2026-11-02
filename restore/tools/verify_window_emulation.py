"""Offline instruction/C comparison. The game EXE and its DLLs are never run.

Only 414cd0..41638c executes inside Unicorn. Every game/API call is intercepted;
unknown destinations fail. This validates the callback with controlled callees,
not the remaining game or real Windows/graphics behavior.
"""
from pathlib import Path
import hashlib, json, random, struct, subprocess, sys, time
ROOT=Path(__file__).resolve().parent.parent
sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from unicorn import Uc, UC_ARCH_X86, UC_MODE_32, UC_HOOK_CODE, __version__ as unicorn_version
from unicorn.x86_const import UC_X86_REG_EAX,UC_X86_REG_EIP,UC_X86_REG_ESP,UC_X86_REG_EBP,UC_X86_REG_ECX,UC_X86_REG_EDX,UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI
image=(ROOT/'build/smoke-game/Pharaoh.exe').read_bytes()
assert hashlib.sha256(image).hexdigest()=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
u16=lambda data,p:struct.unpack_from('<H',data,p)[0]
u32=lambda data,p:struct.unpack_from('<I',data,p)[0]
pe=u32(image,0x3c); opt=pe+24; table=opt+u16(image,pe+20)
base=u32(image,opt+28); size=u32(image,opt+56)
sections=[]
for i in range(u16(image,pe+6)):
 p=table+i*40; sections.append((u32(image,p+12),u32(image,p+20),u32(image,p+16)))
def file_offset(rva):
 for start,raw,length in sections:
  if start<=rva<start+length:return raw+rva-start
 raise ValueError(hex(rva))
def cstring(rva):
 p=file_offset(rva); return image[p:image.index(0,p)].decode('ascii')
vm=Uc(UC_ARCH_X86,UC_MODE_32); vm.mem_map(base,size)
vm.mem_write(base,image[:u32(image,opt+60)])
for rva,raw,length in sections: vm.mem_write(base+rva,image[raw:raw+length])
STACK=0x20000000; STOP=0x31000000; API=0x30000000
vm.mem_map(STACK,0x100000); vm.mem_map(STOP,0x1000); vm.mem_map(API,0x10000)
api_names={}
directory=file_offset(u32(image,opt+104)); ordinal=0
while u32(image,directory+12):
 lookup=u32(image,directory) or u32(image,directory+16); iat=u32(image,directory+16); index=0
 while (value:=u32(image,file_offset(lookup+index*4))):
  name=('ordinal_'+str(value&65535)) if value&0x80000000 else cstring(value+2)
  address=API+ordinal*16; ordinal+=1; api_names[address]=name
  vm.mem_write(base+iat+index*4,struct.pack('<I',address)); index+=1
 directory+=20
def read(address,width=4): return int.from_bytes(vm.mem_read(address,width),'little')
def write(address,value,width=4):vm.mem_write(address,(value&((1<<(width*8))-1)).to_bytes(width,'little'))
def signed(value):return value if value<0x80000000 else value-0x100000000
def trunc2(value):return int(signed(value&0xffffffff)/2)
GAME0={0x5391e0,0x4cffc0,0x4cff90,0x4cffe0,0x4d0220,0x4cc330,0x4035f0,0x4cc320,0x4cc350,0x539170,
 0x409eb0,0x40c760,0x502780,0x40c6b0,0x402980,0x516890,0x41f460,0x4cc3e0,0x4cc3c0,0x4cc370,
 0x40c630,0x4cc390,0x4cc360,0x403770,0x404350,0x4051a0,0x41bae0,0x516a20,0x4035a0,
 0x414bd0,0x4e2ab0,0x402da0,0x406fb0,0x407110,0x406b40,0x406e80,0x406c90,0x406de0,
 0x406720,0x547a60,0x416a20,0x406090}
GAME_ARGS={**{a:0 for a in GAME0},**{a:1 for a in [0x4d01e0,0x4d0280,0x55f561,0x41b8a0,0x416b10,0x416ac0,0x4cfdc0,0x4cc250,0x416ba0,0x4168e0,0x416990]},
 **{a:2 for a in [0x41d110,0x4cc110,0x4cc160,0x407c10]},**{a:3 for a in [0x4ce6e0,0x538cd0,0x4c47a0]}}
API_ARGS={'DefWindowProcA':4,'IsIconic':1,'GetForegroundWindow':0,'SetActiveWindow':1,'SetForegroundWindow':1,'ShowWindow':2,
 'GetSystemMetrics':1,'GetClientRect':2,'ClientToScreen':2,'GetWindowLongA':2,'GetMenu':1,'AdjustWindowRectEx':4,'SetWindowPos':7,
 'BeginPaint':2,'EndPaint':2,'PostQuitMessage':1,'PostMessageA':4,'GetMessageTime':0,'GetMessagePos':0,'SetRect':5}
def configure(mode):
 if mode in (1,2,3):
  w,h={1:(640,480),2:(800,600),3:(1024,768)}[mode];write(0xe38db0,w);write(0xe38dac,h)
 else:w,h=read(0xe38db0),read(0xe38dac)
 for address,value in {0xe38e68:mode,0xe38d9c:w,0xe38da8:trunc2(w-640),0xe38da4:trunc2(h-480),0xe38d94:w*2,
  0xe38d8c:w*2*h,0xe38d98:h,0xe38d90:h,0xd3b55c:0,0xd3b558:w,0xd3b554:0,0xd3b550:h}.items(): write(address,value)
state={}
def hook(cpu,address,_size,_data):
 if 0x414cd0<=address<=0x41638c:return
 esp=cpu.reg_read(UC_X86_REG_ESP); result=0; cleanup=0
 if address in GAME_ARGS:
  n=GAME_ARGS[address]; args=[read(esp+4+i*4) for i in range(n)]
  state['calls'].append([address]+args+[0]*(3-n))
  if address==0x4cff90:write(0xe39128,1)
  elif address==0x4cffc0:
   if not state['variant']&64:write(0xe39128,0)
  elif address==0x4cffe0:result=int(not state['variant']&16)
  elif address==0x4d0220:result=int(not state['variant']&32)
  elif address==0x502780:result=3
  elif address==0x538cd0:result=7 if state['variant']&128 else 6
  elif address==0x516a20:result=0xfedcba98
  elif address==0x4cfdc0:configure(args[0])
  if address==0x55f561:
   assert cpu.reg_read(UC_X86_REG_ECX)==0xe92bf8
   cleanup=4
  if address==0x41b8a0:
   assert cpu.reg_read(UC_X86_REG_ECX)==0x6fdd08
   cleanup=4
 elif address in api_names:
  name=api_names[address]
  if name not in API_ARGS:raise RuntimeError('Unexpected API '+name)
  n=API_ARGS[name]; args=[read(esp+4+i*4) for i in range(n)];cleanup=n*4
  if name=='DefWindowProcA': state['platform'][0]+=1;result=0x89abcdef
  elif name=='IsIconic':result=int(bool(state['variant']&8))
  elif name=='GetForegroundWindow':result=0 if state['variant']&64 else 0xf1234567
  elif name=='SetActiveWindow':state['platform'][5]+=1
  elif name=='SetForegroundWindow':state['platform'][6]+=1;result=1
  elif name=='ShowWindow':state['platform'][7]+=1;result=1
  elif name=='GetSystemMetrics':result=1920 if args[0]==0 else 1080
  elif name=='GetClientRect':
   for i,value in enumerate([0,0,640,480]):write(args[1]+i*4,value)
   result=1
  elif name=='SetRect':
   for i,value in enumerate(args[1:5]):write(args[0]+i*4,value)
   result=1
  elif name=='ClientToScreen':write(args[1],read(args[1])+17);write(args[1]+4,read(args[1]+4)+19);result=1
  elif name=='GetWindowLongA':result=0x81234567 if args[1]==0xfffffff0 else 0x92345678
  elif name=='GetMenu':result=0xfedcba98
  elif name=='AdjustWindowRectEx':
   for i,delta in enumerate([-4,-5,6,7]):write(args[0]+i*4,read(args[0]+i*4)+delta)
   result=1
  elif name=='SetWindowPos':state['platform'][4]+=1;state['position']=[signed(v) for v in args[2:7]];result=1
  elif name=='BeginPaint':state['platform'][2]+=1;vm.mem_write(args[1],bytes(64));result=0x87654321
  elif name=='EndPaint':state['platform'][3]+=1;result=1
  elif name=='PostQuitMessage':state['platform'][1]+=1
  elif name=='PostMessageA':state['platform'][8]+=1;result=1
  elif name=='GetMessageTime':result=0xfedcba98
  elif name=='GetMessagePos':result=0xffff8000
 else:raise RuntimeError('Execution escaped callback at '+hex(address))
 cpu.reg_write(UC_X86_REG_EAX,result&0xffffffff)
 cpu.reg_write(UC_X86_REG_ESP,esp+4+cleanup)
 cpu.reg_write(UC_X86_REG_EIP,read(esp))
vm.hook_add(UC_HOOK_CODE,hook)
def native(fields,before,case):
 global state
 for (address,width),value in zip(fields,before):write(address,value,width)
 state={'calls':[],'platform':[0]*9,'position':[0]*5,'variant':case[7]}
 sp=STACK+0x80000
 for register,value in [(UC_X86_REG_EAX,0x11111111),(UC_X86_REG_ECX,0x22222222),(UC_X86_REG_EDX,0x33333333),
  (UC_X86_REG_EBX,0x44444444),(UC_X86_REG_ESI,0x55555555),(UC_X86_REG_EDI,0x66666666),(UC_X86_REG_EBP,0x77777777)]:vm.reg_write(register,value)
 vm.reg_write(UC_X86_REG_ESP,sp)
 for i,value in enumerate([STOP,0xf1234567,case[0],case[1],case[3]]):write(sp+i*4,value)
 try:vm.emu_start(0x414cd0,STOP,count=100000)
 except Exception:
  print('Emulator context',case,hex(vm.reg_read(UC_X86_REG_EIP)),hex(vm.reg_read(UC_X86_REG_ESP)),state,flush=True)
  raise
 assert vm.reg_read(UC_X86_REG_ESP)==sp+20,'native callback stack cleanup differs'
 return {'after':[read(a,w) for a,w in fields],'return':vm.reg_read(UC_X86_REG_EAX),**{k:state[k] for k in ['calls','platform','position']}}
def cases():
 for mode in [1,7,9,13,14,16,21,30,31,32,47,61,72,73,84]:
  for message in [0x100,0x102,0x104,0x101]:
   for key in list(range(8,47))+list(range(65,124)):
    for control in [0,1]:yield (message,key,mode,0xffff8001,control,4 if mode==9 else 0,0,0)
 for mode in range(256):
  for message in [0,1,2,3,5,6,15,16,28,32,126,512,513,514,515,516,517,518]:
   yield (message,0x10001 if message==6 else 1,mode,0xffff8001,0,0,0,0)
 for mode in [1,7,9,14,30,31,32,73]:
  for variant in [1,2,4,8,16,32,64,128]:
   for message,key in [(0x102,ord('C')),(0x102,ord('W')),(0x100,0x21),(0x100,0x1b),(0x1c,1),(0x100,0x74)]:
    yield (message,key,mode,0,1,6 if mode==9 else 0,0,variant)
executable=ROOT/'build/c-only/Release/restore_window_verify.exe'
process=subprocess.Popen([str(executable),'--oracle-batch'],cwd=ROOT,stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True)
fields=json.loads(process.stdout.readline())['fields']; started=time.monotonic();checked=0
report={'original_sha256':hashlib.sha256(image).hexdigest(),'method':'isolated Unicorn x86 callback instructions vs compiled C; all external calls abstracted',
 'unicorn_version':unicorn_version,'compiled_C_oracle_sha256':hashlib.sha256(executable.read_bytes()).hexdigest(),
 'controller_source_sha256':hashlib.sha256((ROOT/'runtime/window_messages.c').read_bytes()).hexdigest(),
 'native_entry':'0x00414cd0','game_executed':False,'dlls_loaded':False,'window_or_input_used':False,'observed_memory_fields':len(fields)}
try:
 for case in cases():
  process.stdin.write(' '.join(map(str,case))+'\n');process.stdin.flush()
  line=process.stdout.readline()
  if not line:raise RuntimeError('C oracle stopped: '+process.stderr.read())
  c=json.loads(line); expected=native(fields,c.pop('before'),case)
  if c!=expected:
   differences={key:{'C':c[key],'native':expected[key]} for key in expected if c[key]!=expected[key]}
   report.update(passed=False,checked=checked,case=case,differences=differences)
   if 'after' in differences:report['field_differences']=[{'address':hex(a),'width':w,'C':cv,'native':nv} for (a,w),cv,nv in zip(fields,c['after'],expected['after']) if cv!=nv]
   break
  checked+=1
  if checked%1000==0:print(f'Compared {checked} callback cases.',flush=True)
 else: report.update(passed=True,checked=checked)
except Exception as error:
 report.update(passed=False,checked=checked,error=str(error),case=case)
finally:
 process.stdin.close();process.wait(timeout=10)
report['elapsed_seconds']=round(time.monotonic()-started,3)
(ROOT/'metadata/window-emulation.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps(report,indent=2))
sys.exit(0 if report['passed'] else 1)
