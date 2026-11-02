"""Optional offline comparison: original graphics instructions vs compiled C.

No game process, real graphics device, window or media DLL is used. Only the
listed original routines execute in Unicorn; COM/Windows/game endpoints are
controlled. Requires the caller-owned canonical EXE as data, never for building C.
"""
from pathlib import Path
import hashlib, json, re, struct, subprocess, sys, time
ROOT=Path(__file__).resolve().parent.parent
sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE,__version__ as unicorn_version
from unicorn.x86_const import UC_X86_REG_EAX,UC_X86_REG_EIP,UC_X86_REG_ESP,UC_X86_REG_EBP,UC_X86_REG_ECX,UC_X86_REG_EDX,UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI
image=(ROOT/'build/smoke-game/Pharaoh.exe').read_bytes()
assert hashlib.sha256(image).hexdigest()=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
u16=lambda b,p:struct.unpack_from('<H',b,p)[0]
u32=lambda b,p:struct.unpack_from('<I',b,p)[0]
pe=u32(image,0x3c);opt=pe+24;table=opt+u16(image,pe+20);base=u32(image,opt+28);size=u32(image,opt+56)
sections=[tuple(u32(image,table+i*40+k) for k in [12,20,16]) for i in range(u16(image,pe+6))]
def offset(rva):
 for start,raw,length in sections:
  if start<=rva<start+length:return raw+rva-start
 raise ValueError(hex(rva))
def original_string(rva):
 p=offset(rva);return image[p:image.index(0,p)].decode('ascii')
vm=Uc(UC_ARCH_X86,UC_MODE_32);vm.mem_map(base,(size+4095)&~4095)
vm.mem_write(base,image[:u32(image,opt+60)])
for start,raw,length in sections:vm.mem_write(base+start,image[raw:raw+length])
OBJECTS=0x30000000;HOOKS=0x32000000;STOP=0x33000000;STACK=0x34000000
for address,length in [(OBJECTS,0x10000),(HOOKS,0x10000),(STOP,0x1000),(STACK,0x100000)]:vm.mem_map(address,length)
DRAW=OBJECTS;FRONT=OBJECTS+0x100;BACK=OBJECTS+0x200;CLIP=OBJECTS+0x300;UNKNOWN=OBJECTS+0x400;WINDOW=0xf1234567;ERROR=0x80004005
def read(a,n=4):return int.from_bytes(vm.mem_read(a,n),'little')
def write(a,v,n=4):vm.mem_write(a,(int(v)&((1<<(n*8))-1)).to_bytes(n,'little'))
def string(a):
 data=bytearray()
 while read(a,1):data.append(read(a,1));a+=1
 return bytes(data)
def signed(v):return v-0x100000000 if v&0x80000000 else v
def text_hash(data):
 h=2166136261
 for b in data:h=((h^b)*16777619)&0xffffffff
 return h
names={};next_hook=HOOKS
def bind(name):
 global next_hook
 address=next_hook;next_hook+=16;names[address]=name;return address
# Rebind imports as inert emulated endpoints. Nothing is loaded by Windows.
import_rva=u32(image,opt+104);p=offset(import_rva)
while u32(image,p+12):
 lookup=u32(image,p) or u32(image,p+16);iat=u32(image,p+16);i=0
 while (entry:=u32(image,offset(lookup)+i*4)):
  name=('#'+str(entry&0xffff)) if entry&0x80000000 else original_string(entry+2)
  write(base+iat+i*4,bind(name));i+=1
 p+=20
methods={'draw':{8:'Release',16:'CreateClipper',24:'CreateSurface',80:'SetCooperativeLevel',84:'SetDisplayMode'},
 'surface':{8:'Release',20:'Blt',28:'BltFast',88:'GetSurfaceDesc',100:'Lock',112:'SetClipper',128:'Unlock'},
 'clipper':{8:'Release',32:'SetHWnd'},'unknown':{8:'Release'}}
for i,(kind,slots) in enumerate(methods.items()):
 vtable=OBJECTS+0x1000+i*0x100
 for slot,name in slots.items():write(vtable+slot,bind(kind+'.'+name))
 for obj in ({'draw':[DRAW],'surface':[FRONT,BACK],'clipper':[CLIP],'unknown':[UNKNOWN]}[kind]):write(obj,vtable)
RANGES=[(0x426b40,0x426edb),(0x426ee0,0x426f5a),(0x426f60,0x426fe0),(0x426ff0,0x42747d),(0x427480,0x427503),
 (0x4d0040,0x4d0056),(0x4d0060,0x4d01de),(0x4d01e0,0x4d0219),(0x4d0220,0x4d0279),(0x4d03f0,0x4d047a),(0x4d0480,0x4d05ed)]
ENTRIES=[0x426b40,0x426ff0,0x427480,0x426ee0,0x426f60,0x4d0480,0x4d03f0,0x4d0220,0x4d01e0,0x4d0040,0x4d0060]
EXTERNAL={0x4ce6e0:('log',3,0),0x4e2970:('notify',3,0),0x414bd0:('layout',0,0),0x4e2ab0:('assets',0,0),
 0x4cfdc0:('configure',1,0),0x4cfd60:('dimensions',1,0),0x427510:('error_index',1,0),0x561a36:('sprintf',2,0),0x55c0e0:('DirectDrawCreate',3,12),0x4cbea0:('copy_rect',3,0)}
ARGC={'Release':1,'CreateClipper':4,'CreateSurface':4,'SetCooperativeLevel':3,'SetDisplayMode':4,'Blt':6,'BltFast':6,'GetSurfaceDesc':2,'Lock':5,'SetClipper':2,'Unlock':2,'SetHWnd':3,
 'GetDC':1,'GetDeviceCaps':2,'ReleaseDC':2,'GetSystemMetrics':1,'GetPaletteEntries':4,'DirectDrawCreate':3,'GetWindowLongA':2,'SetWindowLongA':3,'GetMenu':1,
 'SetRect':5,'AdjustWindowRectEx':4,'SetWindowPos':7,'SystemParametersInfoA':4,'GetWindowRect':2,'UpdateWindow':1,'OutputDebugStringA':1,'timeGetTime':0}
state={}
def event(id,*args):state['events'].append([id]+[int(a)&0xffffffff for a in args]+[0]*(7-len(args)))
def describe(pointer,self):
 for delta,value in [(8,768),(12,1024),(16,2048),(36,0x31000000 if self==FRONT else 0x31010000),(88,state['red']),(92,state['green']),(96,state['blue'])]:write(pointer+delta,value)
def dimensions(mode):
 if mode in [1,2,3]:
  width,height={1:(640,480),2:(800,600),3:(1024,768)}[mode];write(0xe38db0,width);write(0xe38dac,height)
def hook(cpu,address,size,user):
 if any(start<=address<=end for start,end in RANGES):return
 esp=cpu.reg_read(UC_X86_REG_ESP);cleanup=0;result=0
 if address in EXTERNAL:name,n,cleanup=EXTERNAL[address]
 elif address in names:
  name=names[address];n=ARGC.get(name.split('.')[-1])
  if n is None:raise RuntimeError('Unexpected API '+name)
  cleanup=n*4
 else:raise RuntimeError('Execution escaped reconstructed graphics functions at '+hex(address))
 args=[read(esp+4+4*i) for i in range(n)];v=state['variant'];short=name.split('.')[-1]
 if name=='log':event(1,text_hash(string(args[0])),args[2])
 elif name=='notify':event(2,*args)
 elif name=='layout':event(3)
 elif name=='assets':event(4)
 elif name=='dimensions':dimensions(args[0])
 elif name=='copy_rect':vm.mem_write(args[1],bytes(vm.mem_read(args[0],args[2])))
 elif name=='configure':
  mode=args[0];event(5,mode);dimensions(mode);write(0xe38e68,mode);write(0xe38d9c,read(0xe38db0));write(0xe38d98,read(0xe38dac))
 elif name=='error_index':assert args[0]==ERROR;result=6
 elif name=='sprintf':
  fmt=string(args[1]).decode('ascii');types=re.findall(r'%(?:\d+)?([dxX])',fmt)
  parameters=[read(esp+12+4*i) for i in range(len(types))]
  parameters=[signed(p) if kind=='d' else p for kind,p in zip(types,parameters)]
  data=(fmt%tuple(parameters)).encode('ascii');vm.mem_write(args[0],data+b'\0');result=len(data)
 elif name=='DirectDrawCreate':event(10);result=ERROR if v&1 else 0;write(args[1],DRAW) if not result else None
 elif short=='SetCooperativeLevel':event(11,*args);result=ERROR if v&2 else 0
 elif short=='SetDisplayMode':event(12,*args);result=ERROR if state['displays']<((v>>2)&3) else 0;state['displays']+=1
 elif short=='CreateSurface':
  self,desc,out,outer=args;flags=read(desc+4);width=read(desc+12);height=read(desc+8);caps=read(desc+104);event(13,self,flags,width,height,caps);state['creates']+=1
  primary=caps==0x200;result=ERROR if (v&(16 if primary else 32)) or (primary and v&524288 and state['creates']==1) else 0
  if not result:write(out,FRONT if primary else BACK)
 elif short=='CreateClipper':event(14,args[0]);result=ERROR if v&256 else 0;write(args[2],CLIP) if not result else None
 elif short=='SetHWnd':event(15,args[0],args[2]);result=ERROR if v&512 else 0
 elif short=='SetClipper':event(16,*args);result=ERROR if v&1024 else 0
 elif short=='Lock':
  self,rect,desc,flags,handle=args;event(17,self,flags);result=ERROR if v&(64 if self==FRONT else 128) else 0
  if not result:describe(desc,self)
 elif short=='Unlock':event(18,args[0])
 elif short=='GetSurfaceDesc':event(19,args[0]);result=ERROR if v&2048 else 0;describe(args[1],args[0]) if not result else None
 elif short=='Release':event(20,args[0])
 elif short=='Blt':
  self,dest,src,source,flags,effects=args
  if flags==0x1000400:event(21,self,flags,read(effects),read(effects+80));result=ERROR if v&(4096 if self==BACK else 8192) else 0
  else:event(22,self,*[read(dest+4*i) for i in range(4)]);event(23,*[read(source+4*i) for i in range(4)])
 elif short=='BltFast':event(24,args[0],*[read(args[4]+4*i) for i in range(4)])
 elif name=='GetDC':result=0xfedcba98
 elif name=='GetDeviceCaps':result=256 if args[1]==24 else 1 if args[1]==14 else 24 if v&65536 else 16
 elif name=='ReleaseDC':result=1
 elif name=='GetSystemMetrics':result=(320 if v&32768 else 1920) if args[0]==0 else 1080
 elif name=='GetPaletteEntries':event(25);result=256
 elif name=='GetWindowLongA':result=state['style'] if args[1]==0xfffffff0 else 0x92345678
 elif name=='SetWindowLongA':event(26,*args);result=state['style'];state['style']=args[2]
 elif name=='GetMenu':result=0xfedcba98
 elif name=='SetRect':
  for i,value in enumerate(args[1:]):write(args[0]+4*i,value)
  result=1
 elif name=='AdjustWindowRectEx':
  for i,delta in enumerate([-4,-5,6,7]):write(args[0]+4*i,read(args[0]+4*i)+delta)
  result=1
 elif name=='SetWindowPos':event(27,*args[1:],0);result=1
 elif name=='SystemParametersInfoA':
  for i,value in enumerate([11,13,1900,1000]):write(args[2]+4*i,value)
  result=1
 elif name=='GetWindowRect':
  for i,value in enumerate([-10,-20,500,400]):write(args[1]+4*i,value)
  result=1
 elif name=='UpdateWindow':event(28,args[0]);result=1
 elif name=='OutputDebugStringA':event(29,text_hash(string(args[0])))
 elif name=='timeGetTime':result=0x10
 else:raise RuntimeError('Unimplemented endpoint '+name)
 cpu.reg_write(UC_X86_REG_EAX,result&0xffffffff);cpu.reg_write(UC_X86_REG_ESP,esp+4+cleanup);cpu.reg_write(UC_X86_REG_EIP,read(esp))
vm.hook_add(UC_HOOK_CODE,hook)
def native(fields,before,case):
 global state
 op,full,v,f,previous,mode=case
 state={'variant':v,'events':[],'displays':0,'creates':0,'style':0x81234567,
  'red':0x7c00 if f==1 else 0xfc00 if f==3 else 0x1234 if f==4 else 0xf800,
  'green':0x3e0 if f in [1,2,3] else 0x1234 if f==4 else 0x7e0,'blue':0x3f if f==2 else 0x1f}
 for (a,w),value in zip(fields,before):write(a,value,w)
 write(0xe39140,WINDOW)
 for i,value in enumerate([17,19,1041,787]):write(0xe39130+i*4,value)
 sp=STACK+0x80000
 registers=[UC_X86_REG_EAX,UC_X86_REG_EBX,UC_X86_REG_ECX,UC_X86_REG_EDX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP]
 for i,r in enumerate(registers):vm.reg_write(r,0x11111111*(i+1))
 vm.reg_write(UC_X86_REG_ESP,sp)
 args=[0x12345,0x23456,int(bool(full)),0] if op==2 else [int(bool(full))] if op in [3,8] else [FRONT,BACK] if op==5 else []
 if op==5:vm.reg_write(UC_X86_REG_ECX,0x6de518)
 for i,value in enumerate([STOP]+args):write(sp+4*i,value)
 vm.emu_start(ENTRIES[op],STOP,count=100000)
 assert vm.reg_read(UC_X86_REG_ESP)==sp+4+(8 if op==5 else 0),'native stack cleanup differs'
 result=0 if op in [6,8] else vm.reg_read(UC_X86_REG_EAX)
 return {'after':[read(a,w) for a,w in fields],'return':result,'events':state['events']}
def cases():
 variants=[0,1,2,4,8,12,16,32,64,128,256,512,1024,2048,4096,8192,16384,32768,65536,131072,262144,524288]
 for op in range(11):
  for full in [0,1,0x100]:
   for f in range(5):
    for v in variants:yield (op,full,v,f,0,0)
 for op in [1,5,7,10]:
  for full in [0,0xffffffff]:
   for f in range(5):
    for previous in [0,1,2,3]:
     for mode in [1,7,84]:yield (op,full,0,f,previous,mode)
executable=ROOT/'build/c-only/Release/restore_graphics_verify.exe'
process=subprocess.Popen([str(executable),'--oracle-batch'],cwd=ROOT,stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True)
fields=json.loads(process.stdout.readline())['fields'];started=time.monotonic();checked=0
report={'original_sha256':hashlib.sha256(image).hexdigest(),'method':'isolated original x86 instructions vs compiled C; all COM/Windows/game endpoints controlled',
 'unicorn_version':unicorn_version,'compiled_C_oracle_sha256':hashlib.sha256(executable.read_bytes()).hexdigest(),
 'native_entries':[hex(a) for a in ENTRIES],'observed_memory_fields':len(fields),'original_game_executed':False,'window_created':False,'real_graphics_device_used':False,'media_DLLs_loaded':False}
try:
 for case in cases():
  process.stdin.write(' '.join(map(str,case))+'\n');process.stdin.flush();line=process.stdout.readline()
  if not line:raise RuntimeError('C oracle stopped: '+process.stderr.read())
  c=json.loads(line);expected=native(fields,c.pop('before'),case)
  if c!=expected:
   report.update(passed=False,checked=checked,case=case,differences={key:{'C':c[key],'native':expected[key]} for key in expected if c[key]!=expected[key]})
   if c['after']!=expected['after']:report['field_differences']=[{'address':hex(a),'width':w,'C':cv,'native':nv} for (a,w),cv,nv in zip(fields,c['after'],expected['after']) if cv!=nv]
   break
  checked+=1
  if checked%1000==0:print('Compared',checked,'graphics cases.',flush=True)
 else:report.update(passed=True,checked=checked)
except Exception as error:report.update(passed=False,checked=checked,case=case,error=str(error),emulator_eip=hex(vm.reg_read(UC_X86_REG_EIP)))
finally:process.stdin.close();process.wait(timeout=10)
report['elapsed_seconds']=round(time.monotonic()-started,3)
(ROOT/'metadata/graphics-emulation.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps(report,indent=2));sys.exit(0 if report['passed'] else 1)
