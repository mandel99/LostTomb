"""Compare C codec/envelope with complete original DCL instructions and callbacks."""
from pathlib import Path
import hashlib,json,struct,subprocess,sys
ROOT=Path(__file__).resolve().parent.parent;sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
data=(ROOT/'build/smoke-game/Pharaoh.exe').read_bytes();digest=hashlib.sha256(data).hexdigest()
assert digest=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
u16=lambda b,p:struct.unpack_from('<H',b,p)[0];u32=lambda b,p:struct.unpack_from('<I',b,p)[0]
pe=u32(data,0x3c);opt=pe+24;table=opt+u16(data,pe+20);base=u32(data,opt+28);size=u32(data,opt+56)
vm=Uc(UC_ARCH_X86,UC_MODE_32);vm.mem_map(base,(size+4095)&~4095)
for i in range(u16(data,pe+6)):
 p=table+i*40;vm.mem_write(base+u32(data,p+12),data[u32(data,p+20):u32(data,p+20)+u32(data,p+16)])
INPUT=0x30000000;OUTPUT=0x30140000;STAGE=0x30280000;CHECKSUM=0x303ffff0;HEAP=0x30700000;STOP=0x33000000;STACK=0x34000000;APIS=0x35000000
vm.mem_map(INPUT,0x800000)
for a in [STOP,STACK,APIS]:vm.mem_map(a,0x10000)
def write(a,v,n=4):vm.mem_write(a,(v&((1<<(8*n))-1)).to_bytes(n,'little'))
def read(a):return int.from_bytes(vm.mem_read(a,4),'little')
def cstring(a):
 out=bytearray()
 while True:
  c=bytes(vm.mem_read(a,1));a+=1
  if c==b'\0':return out.decode('ascii')
  out+=c
api_counts={'GlobalAlloc':2,'GlobalLock':1,'GlobalUnlock':1,'GlobalFree':1};api_addresses={}
directory=base+u32(data,opt+104)
while read(directory+12):
 names=read(directory) or read(directory+16);iat=read(directory+16);i=0
 while read(base+names+i*4):
  name_rva=read(base+names+i*4)
  if not name_rva&0x80000000:
   name=cstring(base+name_rva+2)
   if name in api_counts:
    target=APIS+len(api_addresses)*16;api_addresses[target]=name;write(base+iat+i*4,target)
  i+=1
 directory+=20
assert set(api_addresses.values())==set(api_counts)
position=limit=0;payload=b'';errors=[];requests=[]
def endpoint(cpu,address,n,user):
 global position
 sp=cpu.reg_read(UC_X86_REG_ESP);cleanup=0;result=0
 if address in api_addresses:
  name=api_addresses[address];cleanup=api_counts[name]*4
  if name=='GlobalAlloc':vm.mem_write(HEAP,bytes(0x10000));result=HEAP
  elif name=='GlobalLock':result=HEAP
 elif address==0x4ce6e0:
  errors.append(read(sp+4));assert read(sp+8)==read(sp+12)==0
 elif address==0x5660ed:
  file,out,count=[read(sp+4+i*4) for i in range(3)];assert file==37;requests.append(count)
  block=payload[position:position+min(count,limit)]
  if block:vm.mem_write(out,block)
  position+=len(block);result=len(block)
 cpu.reg_write(UC_X86_REG_EAX,result);cpu.reg_write(UC_X86_REG_EIP,read(sp));cpu.reg_write(UC_X86_REG_ESP,sp+4+cleanup)
for a in list(api_addresses)+[0x4ce6e0,0x5660ed]:vm.hook_add(UC_HOOK_CODE,endpoint,begin=a,end=a)
def native(op,capacity,read_limit,encoded):
 global position,limit,payload
 position=0;limit=read_limit;payload=encoded;errors.clear();requests.clear();vm.mem_write(INPUT,encoded or b'\0');vm.mem_write(OUTPUT,b'\xa5'*(capacity+32));vm.mem_write(STAGE,b'\x33'*1200000)
 for a,v in [(0xe92e1c,STAGE),(0x1265188,0x12345678),(0x126518c,0x33333333),(0x1265190,0x55555555),(0x1265194,0x77777777),(CHECKSUM,0xa5a5a5a5)]:write(a,v)
 args=[37,OUTPUT,capacity] if op else [CHECKSUM,INPUT,len(encoded),OUTPUT,capacity]
 sp=STACK+0xe000;write(sp,STOP)
 for i,value in enumerate(args):write(sp+4+i*4,value)
 saved=[(UC_X86_REG_EBX,0x11111111),(UC_X86_REG_EBP,0x22222222),(UC_X86_REG_ESI,0x33333333),(UC_X86_REG_EDI,0x44444444)]
 for r,v in saved:vm.reg_write(r,v)
 vm.reg_write(UC_X86_REG_ESP,sp);vm.reg_write(UC_X86_REG_EFLAGS,2)
 vm.emu_start(0x55bcf0 if op else 0x55c000,STOP,count=80000000)
 assert vm.reg_read(UC_X86_REG_EIP)==STOP and vm.reg_read(UC_X86_REG_ESP)==sp+4,'Native stack/budget differs'
 assert all(vm.reg_read(r)==v for r,v in saved),'Native saved registers differ'
 return {'result':vm.reg_read(UC_X86_REG_EAX),'crc':read(CHECKSUM),'position':position,'errors':errors.copy(),'requests':requests.copy(),'globals':[read(a) for a in [0x1265188,0x126518c,0x1265190,0x1265194]],'output':bytes(vm.mem_read(OUTPUT,capacity+32)).hex()}
sample=bytes.fromhex('00048224258f807f');cases=[]
for cap in [0,1,12,13,14,300,4096]:
 for stream in [b'',b'\0\4',sample,sample[:-1],bytes([2])+sample[1:],bytes([1])+sample[1:],sample[:1]+bytes([3])+sample[2:],sample+b'trailing']:
  cases.append((0,cap,0xffffffff,stream))
for cap in [0,1,13,300,4096]:
 for stream in [struct.pack('<I',0),struct.pack('<I',0x80000000)+bytes(range(256))*16,struct.pack('<I',len(sample))+sample]:
  for maximum in [0,1,4,0xffffffff]:cases.append((1,cap,maximum,stream))
corpus=json.loads((ROOT/'metadata/compression-corpus.json').read_text())
for entry in corpus['entries']:
 encoded=(ROOT/entry['path']).read_bytes();assert hashlib.sha256(encoded).hexdigest()==entry['sha256']
 cases.append((0,entry['capacity'],0xffffffff,encoded));cases.append((1,entry['capacity'],0xffffffff,struct.pack('<I',len(encoded))+encoded))
exe=ROOT/'build/c-only-debug/Debug/restore_compression_verify.exe';process=subprocess.Popen([str(exe),'--oracle-batch'],cwd=ROOT,stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True)
report={'format':1,'original_game_executed':False,'native_sha256':digest,'compiled_C_sha256':hashlib.sha256(exe.read_bytes()).hexdigest(),
 'method':'Bounded original 0055c000/0055bcf0, DCL decoder, original input/output callbacks and CRC instructions. Only GlobalAlloc/Lock/Unlock/Free, legacy file reads and error logging are controlled. Full output plus guard bytes, CRC, read order/count, global metadata and native ABI compared.',
 'limitations':['Corpus covers licensed v147/v160 blocks and bounded synthetic failures; arbitrary malformed expansion and compression/writing are separate checks.']}
checked=0;case=None
try:
 assert json.loads(process.stdout.readline())=={'fields':4}
 for case in cases:
  op,cap,maxread,encoded=case;process.stdin.write(f'{op} {cap} {maxread} '+(encoded.hex() or '-')+'\n');process.stdin.flush();line=process.stdout.readline();assert line,'C exited: '+process.stderr.read()
  actual=json.loads(line);expected=native(*case)
  if actual!=expected:
   differences={key:{'C':actual[key] if key!='output' else actual[key][:96],'native':expected[key] if key!='output' else expected[key][:96]} for key in actual if actual[key]!=expected[key]}
   raise AssertionError(repr(differences))
  checked+=1
 report.update(passed=True,checked=checked,corpus_blocks=len(corpus['entries']))
except Exception as error:report.update(passed=False,checked=checked,case=None if case is None else {'op':case[0],'capacity':case[1],'read_limit':case[2],'input_length':len(case[3]),'input_prefix':case[3][:32].hex()},error=str(error),native_eip=hex(vm.reg_read(UC_X86_REG_EIP)))
finally:process.stdin.close();process.wait(timeout=10)
sources=['include/compression.h','runtime/compression.c','tests/compression.c','tools/prepare_compression_corpus.py','tools/verify_compression_emulation.py','metadata/compression-corpus.json']+[f'src/unit_{i:05d}.c' for i in [2640,2642,2644,2652,2653,2654,2655,2656,2657,2658,2659]]
report['source_hashes']={s:hashlib.sha256((ROOT/s).read_bytes()).hexdigest() for s in sources}
(ROOT/'metadata/compression-emulation.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report,indent=2));sys.exit(0 if report['passed'] else 1)
