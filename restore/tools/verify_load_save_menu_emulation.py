"""Compare native load/save entry, filenames, rendering and the mode-13 input branch."""
from pathlib import Path
import hashlib,itertools,json,struct,subprocess,sys
ROOT=Path(__file__).resolve().parent.parent;sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE,UC_HOOK_BLOCK
from unicorn.x86_const import *
def sha(b):return hashlib.sha256(b).hexdigest()
data=(ROOT/'build/smoke-game/Pharaoh.exe').read_bytes();digest=sha(data)
assert digest=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
u16=lambda b,p:struct.unpack_from('<H',b,p)[0];u32=lambda b,p:struct.unpack_from('<I',b,p)[0]
pe=u32(data,0x3c);opt=pe+24;table=opt+u16(data,pe+20);base=u32(data,opt+28);size=u32(data,opt+56)
vm=Uc(UC_ARCH_X86,UC_MODE_32);vm.mem_map(base,(size+4095)&~4095)
for i in range(u16(data,pe+6)):
 p=table+i*40;vm.mem_write(base+u32(data,p+12),data[u32(data,p+20):u32(data,p+20)+u32(data,p+16)])
BASE=0x30000000;STOP=0x33000000;STACK=0x34000000
for a in [BASE,STOP,STACK]:vm.mem_map(a,0x10000)
def write(a,v,n=4):vm.mem_write(a,(v&((1<<(8*n))-1)).to_bytes(n,'little'))
def read(a):return int.from_bytes(vm.mem_read(a,4),'little')
def byte(a):return vm.mem_read(a,1)[0]
def text_at(a):
 out=bytearray()
 for i in range(1024):
  c=byte(a+i)
  if not c:return bytes(out)
  out.append(c)
 raise AssertionError('Unterminated native string '+hex(a))
def string_hash(a):
 v=2166136261
 for c in text_at(a):v=((v^c)*16777619)&0xffffffff
 return v
regions=[(0x6057c8,28),(0x6059e8,20),(0x7056b8,4),(0x7056c0,12),(0xd3b198,4),(0xd3b1b3,1),(0xd3b1bc,8),
 (0xe20998,4),(0xe38dc9,2),(0xe38dda,1),(0xe38df8,1),(0xe37100,80),(0xe37078,132),(0x700998,100),(0xe38aec,4),
 (0xe36efc,4),(0xe20764,8),(BASE+0x1000,1200),(BASE+0x4000,19200)]
counts={0x40f8f0:3,0x5169c0:0,0x516a20:0,0x4cc500:1,0x4cc3c0:0,0x4cc3e0:0,0x4cbea0:3,0x4ce3f0:1,
 0x4d5100:1,0x4d4d80:1,0x4d4e20:1,0x561a0a:2,0x561994:1,0x55f561:1,0x41b8a0:1,0x4cc110:2,0x4cc160:2,0x538cd0:3,
 0x41c740:2,0x41ca10:1,0x4cde60:2,0x4cde40:2,0x536820:4,0x4d65b0:6,0x4d6250:9,0x4d62f0:8,0x4cc770:5,
 0x425730:4,0x4cc540:0,0x537140:3,0x53e590:3,0x426450:4,0x4d6740:5,0x4261d0:4,0x425bc0:5,0x425990:4}
events=[];mask=existing=accepted=small_calls=big_calls=0
def endpoint(cpu,a,n,user):
 global small_calls,big_calls
 sp=cpu.reg_read(UC_X86_REG_ESP);args=[read(sp+4+i*4) for i in range(counts[a])];values=args.copy();cleanup=result=0;emit=True
 if a in [0x41c740,0x41ca10,0x55f561,0x41b8a0]:values.insert(0,cpu.reg_read(UC_X86_REG_ECX));cleanup=counts[a]*4
 if a==0x53e590:cleanup=12
 if a==0x41c740:values[1]=string_hash(args[0]);result=1
 elif a==0x41ca10:assert args==[0x562370]
 elif a in [0x4ce3f0,0x4d5100,0x4d4d80,0x4d4e20,0x4cc770]:values[0]=string_hash(args[0]);result=existing if a==0x4ce3f0 else 0
 elif a==0x41b8a0:values[1]=string_hash(args[0]);result=1
 elif a==0x561a0a:values=[string_hash(x) for x in args];result=37 if existing else 0
 elif a==0x538cd0:values=[string_hash(args[0]),string_hash(args[1]),args[2]];result=6 if accepted else 7
 elif a==0x5169c0:result=BASE+0x100
 elif a==0x516a20:result=BASE+0x200
 elif a==0x4cde60:emit=False;result=int(struct.unpack('<i',struct.pack('<I',args[0]))[0]*100/struct.unpack('<i',struct.pack('<I',args[1]))[0])
 elif a==0x4cde40:emit=False;result=int(struct.unpack('<i',struct.pack('<I',args[0]))[0]*struct.unpack('<i',struct.pack('<I',args[1]))[0]/100)
 elif a==0x426450:result=mask&255
 elif a==0x4d6740:result=256+args[4]
 elif a==0x4261d0:result=(mask>>(8+big_calls))&1;big_calls+=1
 elif a==0x425990:result=(mask>>(10+small_calls))&1;small_calls+=1
 elif a==0x53e590:result=1
 if emit:
  row=[a,len(values)]+values+[0]*(24-2-len(values));row[12:21]=[read(0x6059f4),read(0x6059f0),read(0xd3b198),byte(0xd3b1b3),byte(0xe38dda),byte(0xd3b1c0),byte(0xe38dca),read(0xe20998),read(0x7056c8)];events.append(row)
 if a==0x40f8f0:vm.mem_write(args[0],bytes([args[2]&255])*args[1])
 elif a==0x4cbea0:vm.mem_write(args[0],bytes(vm.mem_read(args[1],args[2])))
 elif a==0x4cc110:write(0xe20768,BASE+0x300)
 elif a==0x4cc160:write(0xe20764,BASE+0x380)
 cpu.reg_write(UC_X86_REG_EAX,result&0xffffffff);cpu.reg_write(UC_X86_REG_EIP,read(sp));cpu.reg_write(UC_X86_REG_ESP,sp+4+cleanup)
for a in counts:vm.hook_add(UC_HOOK_CODE,endpoint,begin=a,end=a)
inline_stops=[0x4dd8c3]
def guard(cpu,a,n,user):
 if a in inline_stops:cpu.emu_stop();return
 allowed=[(0x402450,0x402b85),(0x41e010,0x41e060),(0x4cdff0,0x4ce100),(0x4ce3a0,0x4ce3f0),(0x532340,0x532920),(0x4d919c,0x4d92a4)]
 if a not in counts and a!=STOP and not any(lo<=a and a+n<=hi for lo,hi in allowed):raise AssertionError('Native branch escaped '+hex(a))
vm.hook_add(UC_HOOK_BLOCK,guard)
def native(p):
 global mask,existing,accepted,small_calls,big_calls
 op,files,first,action,mode,error,ox,oy,mx,my,flags,mask,hover,existing,accepted,campaign=p
 events.clear();small_calls=big_calls=0
 for a,n in regions:vm.mem_write(a,b'\xa5'*n)
 vm.mem_write(BASE,bytes(0x1000))
 for a,s in [(BASE+0x100,b'Save/Restore'),(BASE+0x200,b'Save/Restore.dat'),(BASE+0x300,b'Overwrite?'),(BASE+0x380,b'Saved city'),(0xe370bc,b'Remembered'),(0xe37100,b'Entered'),(0xe37078,b'Map.name')]:vm.mem_write(a,s+b'\0')
 for i in range(300):write(BASE+0x1000+i*4,i*64);vm.mem_write(BASE+0x4000+i*64,('city%03u'%i+('.sav' if op<3 else '')).encode()+b'\0')
 values=[(0x6057cc,BASE+0x1000),(0x6057d8,BASE+0x4000),(0x6057d0,files),(0x6059f8,files),(0x6059f4,first),(0x6059f0,7),(0x6059ec,flags&1),(0x6059e8,0x1234),(0xd3b198,action),(0xd3b1bc,48),
  (0xe38da8,ox),(0xe38da4,oy),(0xe38e34,ox+mx),(0xe38e30,oy+my),(0xe20998,bool(flags&8)),(0x7056c8,hover),(0xf56f8c,1005),(0xf56f94,335),(0xf56f9c,871),(0xf56fb4,18765),(0xf56494,49000),(0xe20768,BASE+0x300),(0xe20764,BASE+0x380)]
 for a,v in values:write(a,v)
 for a,v in [(0xd3b1c0,mode),(0xd3b1b3,error),(0xe38e00,bool(flags&2)),(0xe38df8,bool(flags&4)),(0xe38dda,16 if flags&16 else 13),(0xe38dca,0),(0xe38dc9,0),(0x7027b4,campaign)]:write(a,v,1)
 vm.mem_write(0xe38aec,b'xyz\0')
 sp=STACK+0xe000;write(sp,STOP)
 saved=[(UC_X86_REG_EBX,0x11111111),(UC_X86_REG_EBP,0 if op==10 else 0x22222222),(UC_X86_REG_ESI,0x33333333),(UC_X86_REG_EDI,2 if op==10 else 0x44444444)]
 for r,v in saved:vm.reg_write(r,v)
 if op==10:
  for offset,value in [(0,2),(4,0x33333333),(8,0),(12,0x11111111),(140,STOP)]:write(sp+offset,value)
 vm.reg_write(UC_X86_REG_ESP,sp);vm.reg_write(UC_X86_REG_EFLAGS,2)
 entry=[0x402450,0x402540,0x402630,0x402720,0x402980,0x4029d0,0x402a80,0x532340,0x5325f0,0x532830,0x4d91a4][op]
 vm.emu_start(entry,STOP,count=400000)
 assert vm.reg_read(UC_X86_REG_EIP) in (inline_stops+[STOP] if op==10 else [STOP]),'Native budget/return differs: '+hex(vm.reg_read(UC_X86_REG_EIP))
 assert vm.reg_read(UC_X86_REG_ESP)==sp+((144 if vm.reg_read(UC_X86_REG_EIP)==STOP else 0) if op==10 else 4),'Native stack differs'
 assert all(vm.reg_read(r)==v for r,v in saved),'Native saved register differs'
 result=vm.reg_read(UC_X86_REG_EAX) if op==5 else 0
 return {'result':result,'state':b''.join(bytes(vm.mem_read(a,n)) for a,n in regions).hex(),'events':events.copy()}
default=[7,20,1,1,1,0,192,144,472,180,2,0,256,1,1,0];cases=[]
def case(**fields):
 p=default.copy()
 for k,v in fields.items():p[int(k[1:])]=v
 cases.append(p)
for op,files in itertools.product([0,1,2],[0,1,12,13,256,300]):case(p0=op,p1=files)
for mode,action,exists,accept,campaign in itertools.product([0,1,2,3,255],[0,1,2,256,0xffffffff],[0,1],[0,1],[0,1]):case(p0=3,p3=action,p4=mode,p13=exists,p14=accept,p15=campaign)
for files,first,action in itertools.product([0,1,12,13,256,300],[0,1,243,288],[0,1,2,256,0xffffffff]):case(p0=4,p1=files,p2=first,p3=action)
for files,(x,y),flags in itertools.product([0,12,13,256,300],[(463,145),(464,144),(464,145),(496,275),(497,275),(472,300),(472,301)],[0,2]):case(p0=5,p1=files,p8=x,p9=y,p10=flags)
for files,first,action,flags in itertools.product([0,1,12,256,300],[0,1,200],[0,1,11,299,300,0xffffffff],[0,16]):
 signed_action=action if action<0x80000000 else action-0x100000000
 index=(first+action)&0xffffffff
 if signed_action<files and not index<files:continue
 case(p0=6,p1=files,p2=first,p3=action,p10=flags)
for op,files,(ox,oy),mode,error,hover in itertools.product([7,8],[0,1,12,13,256,300],[(0,0),(80,60),(192,144)],[0,1,2],[0,1,255],[0,1,12,256]):
 case(p0=op,p1=files,p2=max(0,files-12),p4=mode,p5=error,p6=ox,p7=oy,p12=hover)
for files,first,flags in itertools.product([0,12,13,15,16,256,300],[0,1,13,288,300],[0,1]):case(p0=9,p1=files,p2=first,p10=flags)
for mask,flags,exists in itertools.product([0,256,1024,0x1fff],[0,2,4,8,10,12],[0,1]):case(p0=10,p10=flags,p11=mask,p13=exists)
exe=ROOT/'build/c-only-debug/Debug/restore_load_save_menu_verify.exe'
process=subprocess.Popen([str(exe),'--oracle-batch'],cwd=ROOT,stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True)
report={'format':1,'original_game_executed':False,'native_sha256':digest,'compiled_C_sha256':sha(exe.read_bytes()),
 'method':'Complete original load/save/delete entry, real file-list controller and extension transformations, local row selection, confirmation, scroll/drag, both foregrounds and scrollbar, plus the inline mode-13 dispatcher. Final draw/text, input-registry, filesystem enumeration/sort and city load/save/delete endpoints controlled. Full watched memory, string buffers and guards, ordered call arguments/global state, and native stack/register ABI compared.',
 'limitations':['Valid native selection and viewport/count ranges only; malformed row selection that dereferences NULL is excluded. C returns after clearing the field in that undefined native case. Deep city loading/saving and final pixels require separate integration.'],
 'checked':0}
checked=0;p=None
try:
 assert json.loads(process.stdout.readline())=={'fields':16}
 for p in cases:
  process.stdin.write(' '.join(map(str,p))+'\n');process.stdin.flush();line=process.stdout.readline();assert line,'C exited: '+process.stderr.read()
  actual=json.loads(line);expected=native(p)
  if actual!=expected:raise AssertionError(repr({'C_result':actual['result'],'native_result':expected['result'],'state_equal':actual['state']==expected['state'],'state_byte_differences':[(i,a,b) for i,(a,b) in enumerate(zip(bytes.fromhex(actual['state']),bytes.fromhex(expected['state']))) if a!=b][:12],'C_events':actual['events'][:3],'native_events':expected['events'][:3],
   'first_event_difference':[(i,a,b) for i,(a,b) in enumerate(zip(actual['events'],expected['events'])) if a!=b][:1],'counts':[len(actual['events']),len(expected['events'])]}))
  checked+=1
  if checked%500==0:print(json.dumps({'checked':checked,'total':len(cases)}),flush=True)
 report.update(passed=True,checked=checked)
except Exception as error:report.update(passed=False,checked=checked,case=p,error=str(error),native_eip=hex(vm.reg_read(UC_X86_REG_EIP)))
finally:process.stdin.close();process.wait(timeout=10)
sources=['include/load_save_menu.h','include/file_list.h','runtime/load_save_menu.c','tests/load_save_menu.c','tools/verify_load_save_menu_emulation.py','metadata/load-save-native.json','metadata/load-save-input-native.json']
sources += [f'src/unit_{i:05d}.c' for i in [20,21,22,23,287,1739,1745,1746,2427,2428,2429]]
report['source_hashes']={s:sha((ROOT/s).read_bytes()) for s in sources}
(ROOT/'metadata/load-save-menu-emulation.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v for k,v in report.items() if k!='source_hashes'},indent=2));sys.exit(0 if report['passed'] else 1)
