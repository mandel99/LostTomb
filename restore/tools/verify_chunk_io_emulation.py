"""Compare original typed-schema loading and native record migration behavior."""
from pathlib import Path
import hashlib,itertools,json,struct,subprocess,sys
ROOT=Path(__file__).resolve().parent.parent;sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
data=(ROOT/'build/smoke-game/Pharaoh.exe').read_bytes();digest=hashlib.sha256(data).hexdigest();assert digest=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
u16=lambda b,p:struct.unpack_from('<H',b,p)[0];u32=lambda b,p:struct.unpack_from('<I',b,p)[0]
pe=u32(data,0x3c);opt=pe+24;table=opt+u16(data,pe+20);base=u32(data,opt+28);size=u32(data,opt+56)
vm=Uc(UC_ARCH_X86,UC_MODE_32);vm.mem_map(base,(size+4095)&~4095)
for i in range(u16(data,pe+6)):
 p=table+i*40;vm.mem_write(base+u32(data,p+12),data[u32(data,p+20):u32(data,p+20)+u32(data,p+16)])
BASE=0x30000000;OUTPUT=BASE+0x20000;INPUT=BASE+0x40000;HEAP=BASE+0x60000;STOP=0x33000000;STACK=0x34000000
vm.mem_map(BASE,0x100000);vm.mem_map(STOP,0x1000);vm.mem_map(STACK,0x100000);vm.mem_map(0,0x1000)
def write(a,v,n=4):vm.mem_write(a,(v&((1<<(8*n))-1)).to_bytes(n,'little'))
def read(a):return int.from_bytes(vm.mem_read(a,4),'little')
events=[];position=fail=0
counts={0x5660ed:3,0x55bcf0:3,0x561a1d:1,0x561a2b:1,0x562880:3,0x4cc110:2,0x4cc160:2,0x538cd0:3}
def endpoint(cpu,a,n,user):
 global position
 sp=cpu.reg_read(UC_X86_REG_ESP);args=[read(sp+4+i*4) for i in range(counts[a])];events.append([a]+args+[0]*(3-len(args))+[read(0xe945c0)]);result=1
 if a in [0x5660ed,0x55bcf0]:
  assert args[0]==37 and position+args[2]<=100000;vm.mem_write(args[1],bytes(vm.mem_read(INPUT+position,args[2])));position+=args[2];result=args[2] if a==0x5660ed else 1
 elif a==0x561a1d:result=0 if fail else HEAP
 elif a==0x561a2b:assert args==[HEAP]
 elif a==0x562880:vm.mem_write(args[0],bytes(vm.mem_read(args[1],args[2])));result=args[0]
 elif a==0x4cc110:write(0xe20768,BASE+0x100)
 elif a==0x4cc160:write(0xe20764,BASE+0x200)
 cpu.reg_write(UC_X86_REG_EAX,result);cpu.reg_write(UC_X86_REG_EIP,read(sp));cpu.reg_write(UC_X86_REG_ESP,sp+4)
for a in counts:vm.hook_add(UC_HOOK_CODE,endpoint,begin=a,end=a)
def normalize_uninitialized_tail(cpu,a,n,user):
 # 0044ba20 initializes metadata only through +18613. The final DWORD of
 # 004d0c60's stack object still contains _chkstk's return address, then
 # 004d0d05 copies it into reserved output padding. C defines this as zero.
 record=cpu.reg_read(UC_X86_REG_EBX)
 assert read(record+18624)==0x4d0c6a,'Unexpected native scratch-stack residue'
 # All fourteen reserved tail bytes (18614..18627) are unwritten by
 # both the constructor and migration. Nested callers can leave residue
 # before the _chkstk return address as well.
 vm.mem_write(record+18614,bytes(14))
vm.hook_add(UC_HOOK_CODE,normalize_uninitialized_tail,begin=0x4d0d07,end=0x4d0d07)
def normalize_record_padding(cpu,a,n,user):
 # The 44->48 conversion initializes only bytes 0..45 before copying
 # all 48. EDI is just past the final MOVSW, at its two padding bytes.
 write(cpu.reg_read(UC_X86_REG_EDI),0,2)
vm.hook_add(UC_HOOK_CODE,normalize_record_padding,begin=0x4d0db3,end=0x4d0db3)
def native(p):
 global position,fail
 op,index,new_size,new_rows,old_size,old_rows,codec,old_codec,cutoff,version,declared,fail,seed,conversion=p
 events.clear();position=0;descriptors=BASE+0x1000;migration=BASE+0x8000
 for a,n,value in [(descriptors,6000,0),(0xe92e50,6000,0),(OUTPUT,100000,0xa5),(HEAP,100000,0xcc),(migration,32,0),(STACK,0x100000,0)]:vm.mem_write(a,bytes([value])*n)
 write(0,0);vm.mem_write(INPUT,bytes((seed+i*29+i//251)&255 for i in range(100000)))
 for a,v in [(0xe945c0,version),(0xe945c4,declared),(0xe20764,0x11111111),(0xe20768,0x22222222)]:write(a,v)
 vm.mem_write(descriptors+index*20,struct.pack('<5I',codec,OUTPUT,new_size,new_rows,migration if cutoff else 0))
 vm.mem_write(0xe92e50+index*20,struct.pack('<5I',old_codec,0,old_size,old_rows,0))
 if cutoff:vm.mem_write(migration,struct.pack('<4I',old_size,old_rows,cutoff,0x4d0d40 if conversion==1 else 0x4d0c60))
 if op:vm.mem_write(OUTPUT,bytes(vm.mem_read(INPUT,20000)))
 sp=STACK+0xf0000;write(sp,STOP);args=[OUTPUT,fail] if op else [index,descriptors,37]
 for i,value in enumerate(args):write(sp+4+i*4,value)
 saved=[(UC_X86_REG_EBX,0x11111111),(UC_X86_REG_EBP,0x22222222),(UC_X86_REG_ESI,0x33333333),(UC_X86_REG_EDI,0x44444444)]
 for r,v in saved:vm.reg_write(r,v)
 vm.reg_write(UC_X86_REG_ESP,sp);vm.reg_write(UC_X86_REG_EFLAGS,2)
 vm.emu_start([0x4d0e10,0x4d0d40,0x4d0c60][op],STOP,count=15000000)
 assert vm.reg_read(UC_X86_REG_EIP)==STOP and vm.reg_read(UC_X86_REG_ESP)==sp+4,'Native stack/budget differs'
 assert all(vm.reg_read(r)==v for r,v in saved),'Native saved register differs'
 return {'result':0 if op else vm.reg_read(UC_X86_REG_EAX)&255,'position':position,'labels':[read(0xe20764),read(0xe20768)],'events':events.copy(),'output':bytes(vm.mem_read(OUTPUT,50000)).hex()}
cases=[]
for index,(new_size,new_rows,old_size,old_rows),codec,old_codec,version,declared,fail in itertools.product([0,14,299],[(8,3,8,3),(8,3,4,2),(4,2,8,3),(160,260,128,258),(0,0,0,0)],[0,1],[0,1],[114,115,147,160,256],[0,181,256],[0,1]):
 cases.append([0,index,new_size,new_rows,old_size,old_rows,codec,old_codec,0,version,declared,fail,7,0])
for op,seed,old in itertools.product([1,2],[0,7,128,255],[0,1]):cases.append([op,14,1592,1,1592,1,0,0,0,147,175,old,seed,0])
for rows,codec,version,fail in itertools.product([1,3,260],[0,1],[130,136,137,160],[0,1]):
 cases.append([0,77,48,rows,44 if version<=136 else 48,rows,codec,codec,136,version,181,fail,19,1])
for codec,version,fail in itertools.product([0,1],[129,130,131,160],[0,1]):cases.append([0,136,18628,1,15628 if version<=130 else 18628,1,codec,codec,130,version,181,fail,31,2])
exe=ROOT/'build/c-only-debug/Debug/restore_chunk_io_verify.exe';process=subprocess.Popen([str(exe),'--oracle-batch'],cwd=ROOT,stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True)
report={'format':1,'original_game_executed':False,'native_sha256':digest,'compiled_C_sha256':hashlib.sha256(exe.read_bytes()).hexdigest(),
 'method':'Original 004d0e10 schema loader and both actual native migration callbacks, including native constructors/stack probes. File reads, codec endpoint (separately verified), allocation, memmove and error-dialog endpoints controlled. Complete output, read/copy/memory events, versions, text pointers, returns and native ABI compared.',
 'safety_differences':['Unread native stack padding in expanded migration records is defined as zero. Native oracle stack starts zero; all fourteen town metadata tail bytes 18614..18627 are unwritten by constructor and conversion. The final DWORD is asserted to contain _chkstk return address 004d0c6a, then only those fourteen reserved bytes are normalized at 004d0d07. The 44->48 record conversion leaves offsets 46..47 uninitialized; only those two local padding bytes are zeroed at 004d0db3 before native copy-out. All initialized data and other bytes remain compared.'],
 'limitations':['This validates chunk contracts and record conversions; whole map/scenario entry controllers and their post-load initialization still need verification.','Migration test shapes match their format versions. A fabricated old record shape beyond its migration cutoff would make the native loader call a null callback; arbitrary corrupt-schema safety is separate.']}
checked=0;p=None
try:
 assert json.loads(process.stdout.readline())=={'fields':14}
 for p in cases:
  process.stdin.write(' '.join(map(str,p))+'\n');process.stdin.flush();line=process.stdout.readline();assert line,'C exited: '+process.stderr.read()
  actual=json.loads(line);expected=native(p)
  if actual!=expected:
   first=[i//2 for i in range(0,len(actual['output']),2) if actual['output'][i:i+2]!=expected['output'][i:i+2]][:8]
   raise AssertionError(repr({'C_result':actual['result'],'native_result':expected['result'],'C_position':actual['position'],'native_position':expected['position'],'C_event_count':len(actual['events']),'native_event_count':len(expected['events']),'first_different_output_bytes':first,'first_event_difference':[(i,a,b) for i,(a,b) in enumerate(zip(actual['events'],expected['events'])) if a!=b][:1]}))
  checked+=1
 report.update(passed=True,checked=checked)
except Exception as error:report.update(passed=False,checked=checked,case=p,error=str(error),native_eip=hex(vm.reg_read(UC_X86_REG_EIP)))
finally:process.stdin.close();process.wait(timeout=10)
sources=['include/chunk_io.h','runtime/chunk_migrations.c','src/unit_01793.c','tests/chunk_io.c','tools/verify_chunk_io_emulation.py','tools/inspect_migration_tables.py','metadata/native-chunk-schemas.json']
report['source_hashes']={s:hashlib.sha256((ROOT/s).read_bytes()).hexdigest() for s in sources}
(ROOT/'metadata/chunk-io-emulation.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report,indent=2));sys.exit(0 if report['passed'] else 1)
