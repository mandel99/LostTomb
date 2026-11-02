"""Typed sprite manager shutdown, reset and normal lifecycle versus pinned native instructions."""
from pathlib import Path
import argparse,bisect,functools,hashlib,itertools,json,random,struct,subprocess,sys,time
ROOT=next(p for p in Path(__file__).resolve().parents if (p/'build/smoke-game/Pharaoh.exe').exists() and (p/'src').is_dir());PHASE=ROOT/'artifacts/sprite_shutdown_phase'
sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_BLOCK,UC_HOOK_CODE,__version__
from unicorn.x86_const import *
parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--production',action='store_true');parser.add_argument('--exe',type=Path,default=ROOT/'build/agent-sprite-shutdown/Debug/restore_sprite_shutdown_verify.exe');parser.add_argument('--report',type=Path);args=parser.parse_args();exe=args.exe.resolve();OWN=ROOT if args.production else PHASE
NATIVE=ROOT/'metadata/sprite-shutdown-native.json' if args.production else PHASE/'metadata/native.json';REPORT=args.report.resolve() if args.report else ROOT/'metadata/sprite-shutdown-emulation.json' if args.production else PHASE/'metadata/emulation.json'
MASK=0xffffffff;sha=lambda b:hashlib.sha256(b).hexdigest();image=(ROOT/'build/smoke-game/Pharaoh.exe').read_bytes();SHA=sha(image);assert SHA=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
d=json.loads(NATIVE.read_text());assert d['native_sha256']==SHA;functions={int(a,16):f['assembly']['items'] for a,f in d['functions'].items()}
for f in d['functions'].values():assert not f['assembly']['has_more'] and len(f['assembly']['items'])==f['assembly']['total']
u16=lambda p:struct.unpack_from('<H',image,p)[0];u32=lambda p:struct.unpack_from('<I',image,p)[0];pe=u32(0x3c);opt=pe+24;table=opt+u16(pe+20);base=u32(opt+28);size=u32(opt+56)
vm=Uc(UC_ARCH_X86,UC_MODE_32);vm.mem_map(base,(size+4095)&~4095);vm.mem_write(base,image[:u32(opt+60)])
for k in range(u16(pe+6)):
 q=table+k*40;vm.mem_write(base+u32(q+12),image[u32(q+20):u32(q+20)+u32(q+16)])
STOP=0x33000000;STACK=0x34000000;vm.mem_map(STOP,4096);vm.mem_map(STACK,0x10000)
PRIVATE=0x30000040;GLOBAL=0xf55bb0;MANAGER=0x30f5d8;HEAP=0x31000000;OBJECT=0x32000000;VECTOR=0x35000000;WORKFLOW=0x35100000;VIRTUAL=0x36000000
for a,n in [(0,4096),(PRIVATE-64,(MANAGER+128+4095)&~4095),(HEAP,0x10000),(OBJECT,0x3000),(VECTOR,0x2000),(WORKFLOW,0x1000),(VIRTUAL,0x1000)]:vm.mem_map(a,n)
regions=[(PRIVATE-64,MANAGER+128),(GLOBAL-64,MANAGER+128),(0xe92dfc,64),(0xea1348,128),(HEAP,0x10000),(OBJECT,0x3000)]
allowed=sorted(int(i['address'],16) for f in functions.values() for i in f);allowed_set=set(allowed)
for f in functions.values():
 for i in f:
  b=bytes.fromhex(i['bytes']);assert bytes(vm.mem_read(int(i['address'],16),len(b)))==b
read=lambda a,n=4:int.from_bytes(vm.mem_read(a&MASK,n),'little')
def write(a,v,n=4):vm.mem_write(a&MASK,(v&((1<<(n*8))-1)).to_bytes(n,'little'))
def signed(v,n=32):return (v&((1<<n)-1))-(1<<n) if v&(1<<(n-1)) else v&((1<<n)-1)
def actor(id):return (0xbcd600+id*388)&MASK
def current():return actor(read(0xd056f8))
def building(id):return (0x7999d8+id*264)&MASK



# Authenticate the complete straight-line CFGs recovered from the two raw windows.
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
for a,count in [(0x53da80,2),(0x5404e0,8)]:
 f=d['functions'][f'{a:08x}'];raw=f['raw_window'];assert bytes(vm.mem_read(a,raw['length']))==bytes.fromhex(raw['hex']);cfg=[]
 for i in Cs(CS_ARCH_X86,CS_MODE_32).disasm(bytes.fromhex(raw['hex']),a):
  cfg.append((i.address,i.bytes.hex()))
  if i.mnemonic in ['ret','jmp']:break
 assert len(cfg)==count and cfg==[(int(i['address'],16),i['bytes']) for i in f['assembly']['items']]
state={};hits=set();cache={};spies={0x53f790:'release',0x5404e0:'construct',0x540500:'destroy'};returns={int(i['address'],16):a for a in spies for i in functions[a] if i['instruction'].strip().upper()=='RET'}
endpoints={0x561a2b,0x55d960,0x561953,0x561441,0x561363,0x36000010,0x36000020}
def active():
 p=state['p'];return GLOBAL if p[0] in [3,5,6,11,12] or p[2] else PRIVATE
def object_pointer(kind):return OBJECT+(0x200 if kind==2 else 0x100) if kind else 0
def event(tag,arg,extra=0,ordinal=0):
 p=state['p'];s=active();l=s+8000+(p[8]%30)*0xcc3c;state['events'].append([tag,arg,extra,ordinal,read(0xea136c),read(0xea1370),read(0xea1368),read(0xe92e1c),read(s+0x30f5c4),read(s+0x30f5c0),read(l+0xcc24),read(l,1),read(s+0x30f5d4),read(s+0x30f5c8),read(s+0x30f5cc),read(s+0x30f5d0)])
def free_mutation():
 p=state['p'];s=active();l=s+8000+(p[14]%30)*0xcc3c;flags=p[12]
 if p[13] and p[13]!=state['free_count']:return
 if flags&1:write(l+0xcc24,p[15])
 if flags&2:
  write(l,p[15],1)
  for off,v in [(0xcc28,p[15]),(0xcc2c,p[15]^0x12345678),(0xcc30,p[15]^0x87654321)]:write(l+off,v)
 if flags&4:write(s+0x30f5c4,p[15])
 if flags&8:write(s+0x30f5c0,p[15])
 if flags&16:write(0xe92e1c,p[16])
 if flags&32:write(0xea136c,p[15]);write(0xea1370,p[15]^0x12340000)
 if flags&64:
  for off in [0x30f5d4,0x30f5c8,0x30f5cc,0x30f5d0]:write(s+off,p[15])
 if flags&128:write(0xea1370,object_pointer(p[19]))
def virtual_mutation(o):
 p=state['p'];s=GLOBAL
 if p[18] and p[18]!=state['virtual_count']:return
 if p[17]&1:write(0xea1370,object_pointer(p[19]))
 if p[17]&2:write(0xea136c,object_pointer(2))
 if p[17]&4:write(s+8000+(p[14]%30)*0xcc3c+0xcc24,p[15])
 if p[17]&8:write(s+0x30f5c4,p[15]);write(s+0x30f5c0,p[16])
 if p[17]&16:write(0xea1368,0)
 if p[17]&32:write(o+0x10,read(o+0x10)^0x12345678)
def block(cpu,a,n,user):
 if a in endpoints or VECTOR<=a<VECTOR+0x2000 or WORKFLOW<=a<WORKFLOW+0x1000:return
 assert a in allowed_set,'Unexpected native block '+hex(a)
 if (a,n) not in cache:cache[a,n]=allowed[bisect.bisect_left(allowed,a):bisect.bisect_left(allowed,a+n)]
 hits.update(cache[a,n])
def finish(cpu,sp,ret,pop=0):
 cpu.reg_write(UC_X86_REG_EAX,ret&MASK);cpu.reg_write(UC_X86_REG_ECX,0xa17e4567);cpu.reg_write(UC_X86_REG_EDX,0x5a5a5a5a);cpu.reg_write(UC_X86_REG_ESP,sp+4+pop);cpu.reg_write(UC_X86_REG_EIP,read(sp))
def hook(cpu,a,n,user):
 sp=cpu.reg_read(UC_X86_REG_ESP);p=state['p']
 if a in spies:
  if a==0x53f790:args=[read(sp+4),read(sp+8)&255]
  else:args=[cpu.reg_read(UC_X86_REG_ECX),0]
  state['calls'][a]=args;event(a,args[0],args[1])
 elif a in returns:
  entry=returns[a];args=state['calls'][entry];event(entry|0x80000000,args[0],cpu.reg_read(UC_X86_REG_EAX) if entry==0x5404e0 else args[1] if entry==0x53f790 else 0)
 elif a==0x561a2b:
  state['free_count']+=1;event(a,read(sp+4),0,state['free_count']);free_mutation();finish(cpu,sp,0x9abcdef0)
 elif a in [0x36000010,0x36000020]:
  o=cpu.reg_read(UC_X86_REG_ECX);state['virtual_count']+=1;event(a,o,read(sp+4),state['virtual_count']);virtual_mutation(o);finish(cpu,sp,0x9abcdef0,4)
 elif a==0x55d960:
  slot=read(sp+4);assert not read(slot),'Excluded non-null auxiliary teardown';event(a,slot,0);finish(cpu,sp,0x9abcdef0)
 elif a==0x561953:
  assert read(sp+4)==0x53da80;event(a,0x53da80,p[11]);state['registered']=0x53da80 if not p[11] else 0;finish(cpu,sp,p[11])
 else:
  # Normal, nonthrowing fixed-size vector iteration is controlled scaffolding;
  # every actual library ctor/dtor instruction executes through this trampoline.
  argc=5 if a==0x561441 else 4;argv=[read(sp+4+i*4) for i in range(argc)];first,stride,count=argv[:3];assert stride==0xcc3c and count==30
  target=argv[3];assert target==(0x5404e0 if a==0x561441 else 0x540500)
  if argc==5:assert argv[4]==0x540500
  code=bytearray();order=range(count) if argc==5 else reversed(range(count))
  for i in order:
   code+=b'\xb9'+struct.pack('<I',(first+i*stride)&MASK);pc=VECTOR+len(code);code+=b'\xe8'+struct.pack('<I',(target-(pc+5))&MASK)
  code+=b'\xc2'+struct.pack('<H',argc*4);vm.mem_write(VECTOR,bytes(code));vm.ctl_remove_cache(VECTOR,VECTOR+0x2000);cpu.reg_write(UC_X86_REG_EIP,VECTOR)
vm.hook_add(UC_HOOK_BLOCK,block)
for a in [*spies,*returns,*endpoints]:vm.hook_add(UC_HOOK_CODE,hook,begin=a,end=a)
@functools.lru_cache(maxsize=8)
def background(seed):
 b=bytes(((i*13+(i>>5)*7+seed)^0xa7)&255 for i in range(sum(n for a,n in regions)));parts=[];offset=0
 for a,n in regions:parts.append(b[offset:offset+n]);offset+=n
 return parts

def pointer_kind(p,kind,heap,alternate):return 0 if kind==0 else p[6] if kind==2 else alternate if kind==3 else heap
def manager_fields(p,s):
 for i in range(30):write(s+8000+i*0xcc3c+0xcc24,(p[6] if p[7]&(1<<i) else HEAP+0x100+i*0x100) if p[3]&(1<<i) else 0)
 alt=pointer_kind(p,p[4],HEAP+0x4000,read(s+8000+(p[8]%30)*0xcc3c+0xcc24));write(s+0x30f5c4,alt);write(s+0x30f5c0,pointer_kind(p,p[5],HEAP+0x5000,alt))
def workflow(op):
 code=bytearray()
 def call(target):
  pc=WORKFLOW+len(code);code.extend(b'\xe8'+struct.pack('<I',(target-(pc+5))&MASK))
 if op==10:call(0x53db60);code+=b'\xb9'+struct.pack('<I',active());call(0x53db60)
 else:
  if op==12:call(0x53da60)
  call(0x53da70);code+=b'\x85\xc0\x75\x05';call(0x53da80)
 code+=b'\xc3';vm.mem_write(WORKFLOW,bytes(code));vm.ctl_remove_cache(WORKFLOW,WORKFLOW+0x1000);return WORKFLOW

def native(p):
 state.clear();state.update(p=p,events=[],calls={},free_count=0,virtual_count=0,registered=0)
 for (a,n),b in zip(regions,background(p[1])):vm.mem_write(a,b)
 write(0xe92e1c,p[6]);manager_fields(p,PRIVATE);manager_fields(p,GLOBAL);write(0xea136c,object_pointer(p[9]));write(0xea1370,object_pointer(p[10]));write(0xea1368,0)
 for a,v in [(OBJECT+0x100,OBJECT+0x2000),(OBJECT+0x200,OBJECT+0x2010),(OBJECT+0x2000,0x36000010),(OBJECT+0x2010,0x36000020),(0xea1390,pointer_kind(p,p[21],HEAP+0x6000,p[6]))]:write(a,v)
 sp=STACK+0x8000;vm.mem_write(sp-0x400,b'\xcc'*0x800);write(sp,STOP);vm.mem_write(0,bytes((i*17+0x5d)&255 for i in range(4096)));fs_before=bytes(vm.mem_read(0,4096))
 self=GLOBAL if p[2] else PRIVATE;library=self+8000+(p[8]%30)*0xcc3c;entries=[0x53db60,0x53da90,0x53db00,0x53da60,0x53da70,0x53da80,0x4e63b0,0x5404e0,0x540500,0x53f790];entry=entries[p[0]] if p[0]<10 else workflow(p[0]);ecx=library if p[0] in [7,8] else self
 if p[0]==9:write(sp+4,0xea1390);write(sp+8,p[20])
 saved=[(UC_X86_REG_EBX,0x11223344),(UC_X86_REG_EBP,0x22334455),(UC_X86_REG_ESI,0x33445566),(UC_X86_REG_EDI,0x44556677)]
 for r,v in saved+[(UC_X86_REG_ESP,sp),(UC_X86_REG_ECX,ecx),(UC_X86_REG_EFLAGS,2)]:vm.reg_write(r,v)
 vm.emu_start(entry,STOP,count=1000000)
 assert vm.reg_read(UC_X86_REG_EIP)==STOP and vm.reg_read(UC_X86_REG_ESP)==sp+4,'Native stack/budget'
 assert all(vm.reg_read(r)==v for r,v in saved),'Native saved registers'
 assert bytes(vm.mem_read(0,4096))==fs_before,'Native SEH chain not restored'
 assert bytes(vm.mem_read(sp+(12 if p[0]==9 else 4),128))==b'\xcc'*128,'Native caller guard'
 h=hashlib.sha256()
 for a,n in regions:h.update(struct.pack('<II',a,n));h.update(vm.mem_read(a,n))
 result=vm.reg_read(UC_X86_REG_EAX) if p[0] in [0,1,3,4,7,10] else 0
 return struct.pack('<I',result)+h.digest()+struct.pack('<I',len(state['events']))+b''.join(struct.pack('<16I',*e) for e in state['events'])
default=[0,123,0,0x3fffffff,1,1,HEAP+0x7000,0,0,0,0,0,0,0,0,HEAP+0x8000,HEAP+0x9000,0,0,0,0,1,0,0];cases=[]
def add(**kw):
 p=default.copy()
 for k,v in kw.items():p[int(k[1:])]=v&MASK
 cases.append(tuple(p))
for op,self,mask,alt,scratch in itertools.product(range(13),[0,1],[0,1,0x20000000,0x15555555,0x2aaaaaaa,0x3fffffff],[0,1,2,3],[0,1,2,3]):
 if op in [4,7,8,9,11,12] and (mask!=0x3fffffff or alt!=1 or scratch!=1):continue
 add(p0=op,p2=self,p3=mask,p4=alt,p5=scratch)
for i in range(30):
 for op in [0,2,7,8,10]:add(p0=op,p3=1<<i,p7=1<<i,p8=i)
for op,flags,at,index in itertools.product([0,2,6],[1,2,4,8,16,32,64,128,3,1|2|64],[1,2,4,31,32],[0,1,15,29]):
 if flags in [4,8,16,32,64,128] and index!=0:continue
 add(p0=op,p12=flags,p13=at,p14=index,p9=0,p10=0)
for first,second,flags,at,replacement in itertools.product([0,1],[0,2],[0,1,2,4,8,16,32,1|2|4|8|32],[0,1,2],[0,1,2]):add(p0=6,p9=first,p10=second,p17=flags,p18=at,p19=replacement)
for flag,kind,sentinel in itertools.product([0,1,2,255,256,257],[0,1,2],[0,HEAP+0x7000,0x80000100,0xfffffff0]):add(p0=9,p20=flag,p21=kind,p6=sentinel)
for status,op in itertools.product([0,1,255,0x80000000,0xffffffff],[4,11,12]):add(p0=op,p11=status)
cases=sorted(set(cases),key=lambda p:(p[1],p));process=subprocess.Popen([str(exe),'--oracle-batch'],cwd=ROOT,stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE)
report={'format':1,'passed':False,'checked':0,'production':args.production,'native_sha256':SHA,'compiled_C_sha256':sha(exe.read_bytes()),'original_game_executed':False,'unicorn_version':__version__,'method':'Full207-instruction normal lifecycle graph: reset84, shutdown19, manager ctor28/dtor23, global ctor2/register4/cleanup2, library ctor8/dtor18 and unchanged pointer-release19. Actual native leaf callbacks execute within controlled fixed30 vector iteration. Compare both complete0x30f5d8 objects and guards, globals/object/vtable/heap canaries, ordered frees/mutations/library iteration, receiver/argument contracts, native constructor pointer/reset1/registration EAX, stack/saved registers and restored native SEH chain. Compiled C uses explicit receiver APIs and compiled cleanup callback.','limitations':['Normal nonthrowing domain only. Generic CRT vector iteration61441/61363 is controlled fixed30 forward/reverse scaffolding that executes actual native library constructors/destructors; generic unwind/exception algorithms are not proved. Native manager SEH setup/restoration is executed and checked; C uses ordinary nonthrowing control flow.','Host free is a controlled typed endpoint. Fake pointer/alias/mutation fixtures prove original free ordering and field effects, not real allocator ownership validity or safe double-free behavior. Separate C smoke uses the actual host allocation/free boundary.','Optional virtual destructors are controlled oracle endpoints to prove controller receiver/flag1/order/reloads/clearing only. Production rejects unsupported non-null objects explicitly; no actual virtual destructor algorithm or original-code pointer is provided.','AuxiliaryEA1368 is null in successful scenarios. Actual unchanged2678 C executes only its null branch; a fail-fast private link boundary makes55D8B0 unreachable. Production rejects non-null before that known unreconstructed receiver path. No nested auxiliary cleanup is claimed.','Atexit is a controlled registration endpoint for status/callback scheduling comparison. Production registers a compiled C cleanup pointer through host atexit; C smoke checks actual registration. Raw callback addresses occur only in original-instruction oracle data, never product calls.']}
def exact(n):
 b=process.stdout.read(n);assert len(b)==n,'C stopped: '+process.stderr.read().decode(errors='replace');return b
started=time.monotonic();p=None
try:
 assert exact(4)==struct.pack('<I',0x53485554)
 for p in cases:
  process.stdin.write((' '.join(map(str,p))+'\n').encode());process.stdin.flush();header=exact(40);count=struct.unpack_from('<I',header,36)[0];actual=header+exact(count*64);expected=native(p)
  assert actual==expected,repr({'case':p,'C_header':actual[:40].hex(),'native_header':expected[:40].hex(),'first_difference':next((i for i,(a,b) in enumerate(zip(actual,expected)) if a!=b),None),'first_event_difference':[(i,list(struct.unpack_from('<16I',actual,40+i*64)),e) for i,e in enumerate(state['events']) if i<count and list(struct.unpack_from('<16I',actual,40+i*64))!=e][:1]})
  report['checked']+=1
  if p[0]==6 and (p[9] or p[10]):report['virtual_boundary_only_cases']=report.get('virtual_boundary_only_cases',0)+1
  if report['checked']%500==0:print(json.dumps({'checked':report['checked'],'total':len(cases)}),flush=True)
 assert hits==allowed_set,'Uncovered native instructions: '+repr([hex(a) for a in sorted(allowed_set-hits)])
 negative=[]
 for option,message in [('--unsupported-delete','Unsupported sprite-shutdown virtual destructor'),('--unsupported-slot','Unsupported sprite-shutdown auxiliary object')]:
  r=subprocess.run([str(exe),option],capture_output=True,timeout=15);assert r.returncode!=0 and message in r.stderr.decode(errors='replace'),(option,r.returncode,r.stderr)
  negative.append({'option':option,'exit_code':r.returncode,'diagnostic_matched':True})
 report['unsupported_boundaries_fail_closed']=negative;report['passed']=True
except Exception as e:report.update(error=str(e),case=p,native_eip=hex(vm.reg_read(UC_X86_REG_EIP)))
finally:process.stdin.close();process.wait(timeout=10)
sources=[OWN/'include/sprite_shutdown.h',OWN/'runtime/sprite_shutdown_lifecycle.c',OWN/'runtime/sprite_shutdown_boundaries.c',OWN/'tests/sprite_shutdown.c',*[OWN/f'src/unit_{n:05d}.c' for n in [1990,2512,2513,2514,2515,2516,2533]],Path(__file__),NATIVE,PHASE/'CMakeLists.txt',ROOT/'src/unit_02529.c',ROOT/'src/unit_02678.c',ROOT/'include/sprite_libraries.h',ROOT/'include/host_allocator.h',ROOT/'runtime/host_allocator.c']
report['source_hashes']={str(p.resolve().relative_to(ROOT)).replace('\\','/'):sha(p.read_bytes()) for p in sources};report['instruction_coverage']={hex(a):{'executed':sum(int(i['address'],16) in hits for i in f),'total':len(f),'missing':[i['address'] for i in f if int(i['address'],16) not in hits]} for a,f in functions.items()};report['instructions_executed']=len(hits);report['instructions_total']=len(allowed);report['watched_bytes']=sum(n for a,n in regions);report['elapsed_seconds']=round(time.monotonic()-started,3)
REPORT.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v for k,v in report.items() if k not in ['source_hashes','instruction_coverage']},indent=2));sys.exit(0 if report['passed'] else 1)
