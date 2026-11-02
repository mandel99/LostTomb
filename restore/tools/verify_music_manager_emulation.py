"""Offline native/C music configuration and selection, without media playback."""
from pathlib import Path
import hashlib,json,random,re,struct,subprocess,sys,time
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
OBJECT=0xe39180;BYTES=0x59a74+8;NAME=0x30000000;STOP=0x33000000;STACK=0x34000000
for a,n in [(NAME,0x20000),(STOP,0x1000),(STACK,0x10000)]:vm.mem_map(a,n)
ENDPOINTS={0x561a0a,0x561e4d,0x561994,0x561a36,0x562dcb}
RANGES=[(0x539250,0x5394ec),(0x539550,0x539726),(0x539730,0x53981e),(0x539a40,0x539a66),(0x561db1,0x561e4c)]
state={}
def read(a,n=4):return int.from_bytes(vm.mem_read(a,n),'little')
def write(a,v,n=4):vm.mem_write(a,(v&((1<<(n*8))-1)).to_bytes(n,'little'))
def cstring(a):
 out=bytearray()
 while read(a,1):out.append(read(a,1));a+=1;assert len(out)<1000
 return bytes(out)
def fnv(s):
 n=2166136261
 for b in s:n=((n^b)*16777619)&0xffffffff
 return n
def event(a,args):state['events'].append([a,len(args)]+args+[0]*(2-len(args)))
def guard(cpu,a,n,user):
 if a not in ENDPOINTS and not any(lo<=a and a+n-1<=hi for lo,hi in RANGES):raise RuntimeError('Native instruction escaped at '+hex(a))
def endpoint(cpu,a,n,user):
 sp=cpu.reg_read(UC_X86_REG_ESP);args=[read(sp+4+i*4) for i in range(4)];value=0
 if a==0x561a0a:
  name=cstring(args[0]);mode=cstring(args[1]);event(a,[fnv(name),fnv(mode)])
  value=(0x31000000 if state['p'][15] else 0) if name==b'music.txt' else (0x31000004 if state['p'][13] else 0)
 elif a==0x561994:event(a,args[:1])
 elif a==0x561e4d:
  assert args[1:3]==[200,0x31000000];data=state['data'];start=state['position'];end=min(start+199,len(data));line=data.find(b'\n',start,end)
  if line>=0:end=line+1
  if start<len(data):vm.mem_write(args[0],data[start:end]+b'\0');state['position']=end;value=args[0]
 elif a==0x562dcb:
  assert args[2]==10;text=cstring(args[0]);match=re.match(rb'[\t\n\v\f\r ]*([+-]?\d+)',text)
  value=max(-2147483648,min(2147483647,int(match[1]))) if match else 0
  if args[1]:write(args[1],args[0]+(match.end() if match else 0))
 elif a==0x561a36:
  fmt=cstring(args[1])
  if fmt==b'audio\\music\\%s':text=b'audio\\music\\'+cstring(args[2])
  else:
   assert fmt==b'%c:\\audio\\music\\%s';text=bytes([args[2]&255])+b':\\audio\\music\\'+cstring(args[3])
  vm.mem_write(args[0],text+b'\0');value=len(text)
 cpu.reg_write(UC_X86_REG_EAX,value&0xffffffff);cpu.reg_write(UC_X86_REG_EIP,read(sp));cpu.reg_write(UC_X86_REG_ESP,sp+4)
vm.hook_add(UC_HOOK_BLOCK,guard)
for address in ENDPOINTS:vm.hook_add(UC_HOOK_CODE,endpoint,begin=address,end=address)
saved=[(UC_X86_REG_EBX,0x11111111),(UC_X86_REG_EBP,0x22222222),(UC_X86_REG_ESI,0x33333333),(UC_X86_REG_EDI,0x44444444)]
def call(entry,args):
 sp=STACK+0x8000
 for i,v in enumerate([STOP]+args):write(sp+i*4,v)
 for reg,v in saved:vm.reg_write(reg,v)
 vm.reg_write(UC_X86_REG_ECX,OBJECT);vm.reg_write(UC_X86_REG_ESP,sp);vm.reg_write(UC_X86_REG_EFLAGS,2)
 vm.emu_start(entry,STOP,count=20000000)
 assert vm.reg_read(UC_X86_REG_EIP)==STOP,'Instruction limit exhausted'
 assert vm.reg_read(UC_X86_REG_ESP)==sp+4+len(args)*4,'thiscall cleanup differs'
 assert all(vm.reg_read(r)==v for r,v in saved),'Preserved register differs'
 return vm.reg_read(UC_X86_REG_EAX)
def native(p,data):
 global state
 data=data.replace(b'\r\n',b'\n');state={'p':p,'data':data,'position':0,'events':[]}
 vm.mem_write(OBJECT,bytes([p[1]&255])*BYTES);assert call(0x539550,[])==OBJECT
 if p[0]:
  for i in range(53):write(OBJECT+i*0x1b10+0x1b0c,0)
  if p[0]!=5 and p[2]<53:
   c=OBJECT+p[2]*0x1b10;write(c+0x1b08,p[3]);write(c+0x1b0c,p[4])
   for i in range(min(p[3],50)):
    vm.mem_write(c+i*112,f'track{i}.mp3'.encode()+b'\0');write(c+i*112+100,5+i);write(c+i*112+104,i*100);write(c+i*112+108,(i+2)*100-1)
  for off,v in [(0x59a60,p[5]),(0x59a54,p[9]),(0x59a58,p[10]),(0x59a5c,p[11]),(0x59a6c,p[12]),(0x59a64,42)]:write(OBJECT+off,v)
  write(0xe38ea8,p[2],1);write(0xeaac28,p[7]);write(0xea1894+p[7]*0x49d8,p[8]);write(0xe38e62,p[14],1);write(0x12663c8,0)
  vm.mem_write(NAME,data+b'\0')
  entries={1:(0x539a40,[p[3]]),2:(0x539a50,[p[9],p[10]]),3:(0x5395e0,[NAME,p[2],p[9],p[10],p[11],p[6]]),4:(0x539730,[p[6]]),5:(0x539250,[])}
  value=call(*entries[p[0]])
  value=(value if value<0x80000000 else value-0x100000000) if p[0]==3 else 0
 else:value=1
 return {'result':value,'position':state['position'],'events':state['events'],'object':bytes(vm.mem_read(OBJECT,BYTES))}
default=[3,0,0,0,0,0,0,0,150,7,20,30,0,1,0,1];cases=[]
def add(data=b'fixture.mp3',**kw):
 p=default.copy()
 for k,v in kw.items():p[int(k[1:])]=v&0xffffffff
 cases.append((p,data))
for poison in [0,0x5a,0xa5,0xff]:add(p0=0,p1=poison)
for value in [0,1,255,0x10000,0xffffffff]:add(p0=1,p3=value);add(p0=2,p9=value,p10=0xffffffff-value)
for category in [0,1,28,52,54]:
 for count in [0,49,50]:
  for ambient in [0,1]:add(p2=category,p3=count,p6=ambient,p4=9 if ambient else 0)
for name in [b'',b'long name.mp3',b'x'*99]:add(data=name,p2=52,p9=0x80000000,p10=0x7fffffff,p11=0xffffffff)
for category in [0,1,28,52]:
 for profile in [0,1,4,7]:
  for population in [-1,0,99,100,199,200,499,100000]:
   for force in [0,1]:add(p0=4,p2=category,p7=profile,p8=population,p6=force,p3=5,p5=4,p9=0,p10=4,p11=1,p12=profile&1)
for count in [0,1,50]:add(p0=4,p3=count,p5=0,p6=1)
loader_cases=[b'',b';comment\r\n\r\n',b'a.mp3 M 0 20 0 999\nAMBIENT\nb.mp3 A 52\nc.mp3 M 0\n',b'AMBIENT\na.mp3 M 0\n',b'a.mp3 a 2 3 4 5\n',
 b'a.mp3 M 0 20 0 999\n' * 49,b'a.mp3 A 52 20 0 999\n'*10,b'a.mp3 M 0 -2147483649 2147483648 -9\n']
for data in loader_cases:
 for found,drive in [(1,0),(0,68)]:add(data=data,p0=5,p13=found,p14=drive)
add(data=b'ignored',p0=5,p15=0)
asset=ROOT/'build/debug-startup-game/music.txt'
if asset.exists():add(data=asset.read_bytes(),p0=5)
exe=ROOT/'build/c-only-debug/Debug/restore_music_verify.exe'
process=subprocess.Popen([str(exe),'--oracle-batch'],cwd=ROOT,stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE)
report={'format':1,'unicorn_version':unicorn_version,'original_game_executed':False,'media_playback_used':False,
 'method':'Native constructor, setters, track insertion, population selector, music parser and tokenizer versus compiled C; file, formatting and decimal-conversion endpoints controlled',
 'compared':['complete 0x59a74-byte object and eight adjacent guard bytes','value returns, constructor receiver and thiscall stack/register preservation','ordered file open/close arguments and read cursor'],
 'limitations':['Unsafe category 53/negative indices, bad counts, overlong paths, missing type tokens and full-table AMBIENT overflow are rejected/ignored in C and are tested separately, not claimed equivalent.',
  'With the user-requested CD bypass, a missing local file retains its relative path when the media-drive byte is zero; native missing-file comparisons use a nonzero drive.',
  'The original constructor preserves ambient counters; first startup uses zero-owned data. Other playback methods remain unreviewed.',
  'Actual music.txt is included with controlled track-file availability; no actual sound device, codec or playback is used.'],
 'compiled_C_sha256':hashlib.sha256(exe.read_bytes()).hexdigest(),'original_sha256':hashlib.sha256(image).hexdigest(),
 'local_music_sha256':hashlib.sha256(asset.read_bytes()).hexdigest() if asset.exists() else None}
checked=0;case=None;started=time.monotonic()
try:
 assert json.loads(process.stdout.readline())=={'object':OBJECT,'bytes':BYTES}
 for case in cases:
  p,data=case;process.stdin.write((' '.join(map(str,p))+f' {len(data)}\n'+(data.hex()+'\n' if data else '')).encode());process.stdin.flush()
  actual=json.loads(process.stdout.readline());actual['object']=process.stdout.read(BYTES);assert len(actual['object'])==BYTES
  expected=native(p,data)
  if actual!=expected:
   report['differences']={k:{'C':actual[k] if k!='object' else '(see bytes)','native':expected[k] if k!='object' else '(see bytes)'} for k in actual if actual[k]!=expected[k]}
   if actual['object']!=expected['object']:report['object_first_differences']=[{'offset':i,'C':a,'native':b} for i,(a,b) in enumerate(zip(actual['object'],expected['object'])) if a!=b][:20]
   raise AssertionError('Object, result or file lifecycle differs')
  checked+=1
 report.update(passed=True,checked=checked)
except Exception as error:report.update(passed=False,checked=checked,case={'parameters':case[0],'data_hex':case[1].hex()[:300]} if case else None,error=str(error),native_eip=hex(vm.reg_read(UC_X86_REG_EIP)))
finally:
 process.stdin.close();process.wait(timeout=10)
report['elapsed_seconds']=round(time.monotonic()-started,3)
files=['src/unit_02473.c','src/unit_02475.c','src/unit_02476.c','src/unit_02477.c','src/unit_02479.c','src/unit_02480.c','src/unit_02815.c','include/music_manager.h','tests/music_manager.c','tools/verify_music_manager_emulation.py']
report['source_hashes']={f:hashlib.sha256((ROOT/f).read_bytes()).hexdigest() for f in files}
(ROOT/'metadata/music-manager-emulation.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({k:v for k,v in report.items() if k!='source_hashes'},indent=2));sys.exit(0 if report['passed'] else 1)
