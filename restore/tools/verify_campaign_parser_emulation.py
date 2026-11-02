"""Offline full-parser comparison. Only CRT/file/log endpoints are controlled."""
from pathlib import Path
import hashlib,json,random,struct,subprocess,sys,time
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
OBJECT=0x6fdd08;BYTES=0x4a84;NAME=0x30000000;STOP=0x33000000;STACK=0x34000000
for a,n in [(NAME,0x1000),(STOP,0x1000),(STACK,0x10000)]:vm.mem_map(a,n)
vm.mem_write(NAME,b'fixture.txt\0')
ENDPOINTS={0x561a0a,0x561e4d,0x561994,0x561d89,0x561c96,0x562370,0x561a36,0x4ce6e0}
RANGES=[(0x41ac00,0x41b64d),(0x41b920,0x41b9cb),(0x561db1,0x561e4c)]
state={}
def read(a,n=4):return int.from_bytes(vm.mem_read(a,n),'little')
def write(a,v,n=4):vm.mem_write(a,(v&((1<<(n*8))-1)).to_bytes(n,'little'))
def cstring(a):
 result=bytearray()
 while True:
  b=read(a,1);a+=1
  if not b:return bytes(result)
  result.append(b);assert len(result)<2000
def fnv(s):
 value=2166136261
 for b in s:value=((value^b)*16777619)&0xffffffff
 return value
def guard(cpu,a,n,user):
 if a not in ENDPOINTS and not any(lo<=a and a+n-1<=hi for lo,hi in RANGES):raise RuntimeError('Native instructions escaped at '+hex(a))
def endpoint(cpu,a,n,user):
 sp=cpu.reg_read(UC_X86_REG_ESP);args=[read(sp+4+i*4) for i in range(7)];value=0
 if a==0x561a0a:
  assert cstring(args[0])==b'fixture.txt' and cstring(args[1])==b'rb';state['opens']+=1;value=0x31000000 if state['available'] else 0
 elif a==0x561e4d:
  assert args[1:3]==[200,0x31000000]
  start=state['position'];data=state['data'];end=min(start+199,len(data));newline=data.find(b'\n',start,end)
  if newline>=0:end=newline+1
  if start<len(data):vm.mem_write(args[0],data[start:end]+b'\0');state['position']=end;value=args[0]
 elif a==0x561994:assert args[0]==0x31000000;state['closes']+=1
 elif a==0x561d89:value=int(48<=(args[0]&255)<=57)
 elif a==0x561c96:
  text=cstring(args[0]);value=int(text) if text!=b'-' else 0;value&=0xffffffff
 elif a==0x562370:
  x=cstring(args[0]).lower();y=cstring(args[1]).lower();value=0 if x==y else (1 if x>y else 0xffffffff)
 elif a==0x561a36:
  fmt=cstring(args[1]);output=bytearray();index=2;i=0
  while i<len(fmt):
   if fmt[i]!=37:output.append(fmt[i]);i+=1;continue
   code=fmt[i+1];v=args[index];index+=1;i+=2
   if code==115:output.extend(cstring(v))
   elif code==100:output.extend(str(v if v<0x80000000 else v-0x100000000).encode())
   else:raise AssertionError('Unsupported format '+repr(fmt))
  vm.mem_write(args[0],bytes(output)+b'\0');value=len(output)
 elif a==0x4ce6e0:
  assert args[1:3]==[0,0];state['errors'].append(fnv(cstring(args[0])))
 cpu.reg_write(UC_X86_REG_EAX,value);cpu.reg_write(UC_X86_REG_EIP,read(sp));cpu.reg_write(UC_X86_REG_ESP,sp+4)
vm.hook_add(UC_HOOK_BLOCK,guard)
for address in ENDPOINTS:vm.hook_add(UC_HOOK_CODE,endpoint,begin=address,end=address)
def native(data,seed,available,constructor=False):
 global state
 state={'data':data,'position':0,'available':available,'opens':0,'closes':0,'errors':[]}
 vm.mem_write(OBJECT,bytes([seed&255])*BYTES);write(0x12663c8,0);write(0x7027d4,0,1)
 # Native leaves padding and unused merge bytes uninitialized on its frame.
 # Seed the frame with zeros to normalize those bytes to the C initialization.
 vm.mem_write(STACK,b'\0'*0x10000);sp=STACK+0x8000;write(sp,STOP);write(sp+4,NAME)
 saved=[(UC_X86_REG_EBX,0x11111111),(UC_X86_REG_EBP,0x22222222),(UC_X86_REG_ESI,0x33333333),(UC_X86_REG_EDI,0x44444444)]
 for reg,value in saved:vm.reg_write(reg,value)
 vm.reg_write(UC_X86_REG_ECX,OBJECT);vm.reg_write(UC_X86_REG_ESP,sp);vm.reg_write(UC_X86_REG_EFLAGS,2)
 vm.emu_start(0x41ac50 if constructor else 0x41ae40,STOP,count=3000000)
 assert vm.reg_read(UC_X86_REG_EIP)==STOP,'Instruction limit exhausted'
 assert vm.reg_read(UC_X86_REG_ESP)==sp+(4 if constructor else 8),'thiscall cleanup differs'
 assert all(vm.reg_read(r)==v for r,v in saved),'Preserved register differs'
 if constructor:return {'result':int(vm.reg_read(UC_X86_REG_EAX)==OBJECT),'object':bytes(vm.mem_read(OBJECT,BYTES)).hex()}
 return {'result':vm.reg_read(UC_X86_REG_EAX)&255,'position':state['position'],'opens':state['opens'],'closes':state['closes'],
  'object':bytes(vm.mem_read(OBJECT,BYTES)).hex(),'errors':state['errors']}
prefix=b'[MISSION_NAMES]\nCity Alpha\nCity Beta\n[PREDYNASTIC_PERIOD]\n'
cases=[(b'',0xa5,1),(prefix,0xa5,0),(b'; comments\r\n\r\n',0x5a,1)]
def add(data,seed=0xa5):cases.append((data,seed,1))
for section in [b'[PREDYNASTIC_PERIOD]',b'[ARCHAIC_PERIOD]',b'[OLD_KINGDOM]',b'[MIDDLE_KINGDOM]',b'[NEW_KINGDOM]',b'[VALLEY_OF_THE_KINGS]',b'[RAMSES_II]',b'[ANCIENT_CONQUERORS]',b'[CLEOPATRAS_ALEXANDRIA]']:
 add(b'[MISSION_NAMES]\r\nCity Alpha\r\n'+section+b'\r\nmission=0,200,0,0\r\n')
for data in [b'mission=0,200,0',b'mission=1,201,1,255',b'mission=-1,2,3',b'unknown=1',b'choicescreen=3,4',
 b'mission=0,2,3\nchoice=1,436,-2,65537',b'mission=0,2,3\nchoice=1,2,3,4\nchoice=2,3,4,5\nmission=1,2,3,1\nmission=1,2,3,2',
 b'mission=0,2,3\nchoice=1,2,3,4\nchoice=2,3,4,5',b'mission=0,2,3,0,9,3,8,4,7,5,6,6,5,7,4,8',b'm i s s i o n = 0 , 2 , 3']:
 add(prefix+data+b'\n')
for keyword,count in [(b'mission',4),(b'choice',4),(b'choicescreen',2)]:
 for field in range(count):
  for token in [b'',b'x',b'-',b'-1',b'0',b'1',b'255',b'256',b'2147483647',b'2147483648',b'4294967295',b'4294967296',b'+1']:
   values=[b'0']*count;values[field]=token;add(prefix+keyword+b'='+b','.join(values)+b'\n')
for length in [0,1,2,48,49,50,99,198,199,200,201]:add(b'[MISSION_NAMES]\n'+b'A'*length+b'\r\n')
for count in [99,100,101]:
 add(b'[MISSION_NAMES]\n'+b'City Name\n'*count)
 add(prefix+b'mission=0,200,0\n'*count)
rng=random.Random(0x41ae40)
for i in range(160):
 data=prefix
 for j in range(rng.randrange(1,8)):
  keyword=rng.choice([b'mission',b'choice',b'choicescreen']);n={b'mission':rng.randrange(2,9),b'choice':rng.randrange(2,6),b'choicescreen':rng.randrange(1,4)}[keyword]
  data+=keyword+b'='+b','.join(rng.choice([b'0',b'1',b'2',b'-1',b'255',b'256',b'x',b'65537']) for k in range(n))+b'\n'
 add(data,rng.choice([0,0x5a,0xa5]))
asset=ROOT/'build/debug-startup-game/campaign.txt'
if asset.exists():add(asset.read_bytes())
exe=ROOT/'build/c-only-debug/Debug/restore_campaign_parser_verify.exe'
process=subprocess.Popen([str(exe),'--oracle-batch'],cwd=ROOT,stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True)
report={'format':1,'unicorn_version':unicorn_version,'original_game_executed':False,'native_entry':'0x0041ae40',
 'method':'Original parser, prefix constructor, reset, append, whitespace removal, numeric readers, tokenizer and campaign lookup versus compiled C; file IO, digit/atoi, comparison, formatting and logging endpoints controlled',
 'compared':['byte return','complete 0x4a84-byte object with frame padding normalized','ordered error message hashes','file cursor/open/close lifecycle','thiscall cleanup and preserved registers'],
 'limitations':['Native frame padding/unused merge bytes are zero-seeded to match deterministic C initialization; those originally unspecified values are not equivalence claims.',
  'ASCII numeric and case-insensitive string endpoints are controlled; long error messages are safely truncated by C, and a negative append count is rejected.',
  'The enclosing save/load consumer still has separate unresolved receiver and argument contracts.'],
 'compiled_C_sha256':hashlib.sha256(exe.read_bytes()).hexdigest(),'original_sha256':hashlib.sha256(image).hexdigest(),
 'local_asset_included':asset.exists(),'local_asset_sha256':hashlib.sha256(asset.read_bytes()).hexdigest() if asset.exists() else None}
checked=0;case=None;started=time.monotonic()
try:
 assert json.loads(process.stdout.readline())=={'object':OBJECT,'bytes':BYTES}
 for case in cases:
  data,seed,available=case;process.stdin.write(f'{seed} {available} {len(data)}\n'+(data.hex()+'\n' if data else ''));process.stdin.flush()
  actual=json.loads(process.stdout.readline());expected=native(data,seed,available)
  if actual!=expected:
   report['differences']={k:{'C':actual[k] if k!='object' else '(see first bytes)','native':expected[k] if k!='object' else '(see first bytes)'} for k in actual if actual[k]!=expected[k]}
   if actual['object']!=expected['object']:report['object_first_differences']=[{'offset':i,'C':a,'native':b} for i,(a,b) in enumerate(zip(bytes.fromhex(actual['object']),bytes.fromhex(expected['object']))) if a!=b][:20]
   raise AssertionError('State, result, errors or file lifecycle differ')
  checked+=1
 constructor_process=subprocess.Popen([str(exe),'--constructor-batch'],cwd=ROOT,stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True)
 try:
  assert json.loads(constructor_process.stdout.readline())=={'object':OBJECT,'bytes':BYTES}
  for seed in [0,0x5a,0xa5]:
   constructor_process.stdin.write(str(seed)+'\n');constructor_process.stdin.flush()
   actual=json.loads(constructor_process.stdout.readline());expected=native(b'',seed,0,True)
   assert actual==expected,'Constructor object bytes or return differ'
   checked+=1
 finally:
  constructor_process.stdin.close();constructor_process.wait(timeout=10)
 report['parser_cases']=len(cases);report['constructor_cases']=3
 report.update(passed=True,checked=checked)
except Exception as error:report.update(passed=False,checked=checked,case={'data_hex':case[0].hex(),'seed':case[1],'available':case[2]} if case else None,error=str(error),native_eip=hex(vm.reg_read(UC_X86_REG_EIP)))
finally:
 process.stdin.close();process.wait(timeout=10)
report['elapsed_seconds']=round(time.monotonic()-started,3)
files=['src/unit_00241.c','src/unit_00242.c','src/unit_00243.c','src/unit_00245.c','src/unit_00246.c','src/unit_00247.c','src/unit_00248.c','src/unit_00249.c','src/unit_00253.c','src/unit_02815.c','include/campaign_parser.h','include/campaign_index.h','tests/campaign_parser.c','tools/verify_campaign_parser_emulation.py']
report['source_hashes']={f:hashlib.sha256((ROOT/f).read_bytes()).hexdigest() for f in files}
(ROOT/'metadata/campaign-parser-emulation.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({k:v for k,v in report.items() if k!='source_hashes'},indent=2));sys.exit(0 if report['passed'] else 1)
