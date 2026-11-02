"""Complete projectile and carrier callbacks with real chain and tile queries."""
from pathlib import Path
import argparse,bisect,functools,hashlib,itertools,json,random,struct,subprocess,sys,time
ROOT=next(p for p in Path(__file__).resolve().parents if (p/'build/smoke-game/Pharaoh.exe').exists() and (p/'src').is_dir());PHASE=ROOT/'artifacts/actor_callbacks_chain_phase'
sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_BLOCK,UC_HOOK_CODE,__version__
from unicorn.x86_const import *
parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--production',action='store_true');parser.add_argument('--exe',type=Path,default=ROOT/'build/agent-actor-callbacks-chain/Debug/restore_actor_callbacks_chain_verify.exe');parser.add_argument('--report',type=Path);args=parser.parse_args();exe=args.exe.resolve();OWN=ROOT if args.production else PHASE
NATIVE=ROOT/'metadata/actor-callbacks-chain-native.json' if args.production else PHASE/'metadata/native.json';REPORT=args.report.resolve() if args.report else ROOT/'metadata/actor-callbacks-chain-emulation.json' if args.production else PHASE/'metadata/emulation.json'
MASK=0xffffffff;sha=lambda b:hashlib.sha256(b).hexdigest();image=(ROOT/'build/smoke-game/Pharaoh.exe').read_bytes();SHA=sha(image);assert SHA=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
d=json.loads(NATIVE.read_text());assert d['native_sha256']==SHA;functions={int(a,16):f['assembly']['items'] for a,f in d['functions'].items()}
for f in d['functions'].values():assert not f['assembly']['has_more'] and len(f['assembly']['items'])==f['assembly']['total']
u16=lambda p:struct.unpack_from('<H',image,p)[0];u32=lambda p:struct.unpack_from('<I',image,p)[0];pe=u32(0x3c);opt=pe+24;table=opt+u16(pe+20);base=u32(opt+28);size=u32(opt+56)
vm=Uc(UC_ARCH_X86,UC_MODE_32);vm.mem_map(base,(size+4095)&~4095);vm.mem_write(base,image[:u32(opt+60)])
for k in range(u16(pe+6)):
 q=table+k*40;vm.mem_write(base+u32(q+12),image[u32(q+20):u32(q+20)+u32(q+16)])
STOP=0x33000000;STACK=0x34000000;vm.mem_map(STOP,4096);vm.mem_map(STACK,0x10000)
regions=[(0xbcd2d8,1005*388+64),(0x7998b0,302*264+64),(0xb350bc,0x40040),(0x5cc46c,0x8040),(0xf55cd0,0x100),(0xd056f0,32),(0xbca2d0,48),(0xe94df0,48),(0x784ca0,48)]
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

state={};hits=set();cache={};deep={0x4b3ff0:(2,1),0x4b22e0:(3,2),0x47d350:(1,4)};spies={0x49b3c0:1,0x4b57e0:2};returns={int(i['address'],16):a for a in spies for i in functions[a] if i['instruction'].strip()=='RET'}
def block(cpu,a,n,user):
 if a in deep:return
 assert a in allowed_set,'Unexpected native block '+hex(a)
 if (a,n) not in cache:cache[a,n]=allowed[bisect.bisect_left(allowed,a):bisect.bisect_left(allowed,a+n)]
 hits.update(cache[a,n])
def event(address,argv,result=0):
 a=current();e=[0]*24;e[0]=address;e[1]=len(argv);e[2:2+len(argv)]=argv;e[6]=result;e[7:24]=[read(0xd056f8),read(a+0x30,1),read(a-7,1),read(a+9,1),read(a+10,1),read(a+0x31,1),read(a+0x5d,1),read(a+0x6a,1),read(a+4,1),read(a+6,1),read(a-4),read(a+0x178),read(0xbca2e0),read(0xe94e00),read(a+0x52,2),read(a+0x60,2),read(a+0x56,2)];state['events'].append(e)
def mutation(mask):
 p=state['p']
 if p[25] and not p[25]&mask:return
 if p[22]&1:write(0xd056f8,p[3] if read(0xd056f8)==p[2] else p[2])
 a=current()
 if p[22]&2:write(a+0x30,p[23],1)
 if p[22]&4:write(a+9,p[24],1)
 if p[22]&8:
  for i in range(0,0x100,4):write(0xf55cd0+i,read(0xf55cd0+i)^0x120000)
 if p[22]&16:write(0xbca2e0,p[28]);write(0xe94e00,p[28]^0x12340000)
 if p[22]&32:write(0x784cb0,read(0x784cb0,1)^1,1)
 if p[22]&64:write(a+0x60,p[6],2);write(a+0x52,p[4]+1,2)
 if p[22]&128:write(actor(signed(p[5],16))+0x5d,1,1)
 if p[22]&256:
  write(actor(signed(p[5],16))+0x30,p[23],1)
  for i in range(9):write(actor(16+i)+0x30,p[23],1)
 if p[22]&512:
  for i in range(3):write(actor(100+i)+0x30,p[23],1)
def hook(cpu,a,n,user):
 sp=cpu.reg_read(UC_X86_REG_ESP);p=state['p']
 if a in spies:
  argv=[read(sp+4+i*4) for i in range(spies[a])];state['calls'][a]=argv;event(a,argv)
 elif a in returns:
  entry=returns[a];event(entry|0x80000000,state['calls'][entry],cpu.reg_read(UC_X86_REG_EAX));mutation(8 if entry==0x49b3c0 else 16)
 else:
  count,mask=deep[a];argv=[read(sp+4+i*4) for i in range(count)];event(a,argv);mutation(mask)
  cpu.reg_write(UC_X86_REG_EAX,p[21] if a==0x4b3ff0 else 0x9abcdef0);cpu.reg_write(UC_X86_REG_ECX,0xa17e4567);cpu.reg_write(UC_X86_REG_EDX,0x5a5a5a5a);cpu.reg_write(UC_X86_REG_ESP,sp+4);cpu.reg_write(UC_X86_REG_EIP,read(sp))
vm.hook_add(UC_HOOK_BLOCK,block)
for a in [*spies,*returns,*deep]:vm.hook_add(UC_HOOK_CODE,hook,begin=a,end=a)
@functools.lru_cache(maxsize=8)
def background(seed):
 b=bytes(((i*13+(i>>5)*7+seed)^0xa7)&255 for i in range(sum(n for a,n in regions)));parts=[];offset=0
 for a,n in regions:parts.append(b[offset:offset+n]);offset+=n
 return parts

def actor_fields(p,id,alternate):
 a=actor(id)
 for off,value in [(2,74 if p[0]==0 else 66),(6,0),(0x30,p[12]),(-7,p[13]+alternate*5),(9,p[14]+alternate*3),(10,p[15]+alternate*3),(0x31,p[17]),(0x72,p[18])]:write(a+off,value,1)
 for off,value in [(0x52,p[4]+alternate),(0x60,p[5]),(0x2e,p[16])]:write(a+off,value,2)
 for off,value in [(0x18,p[31]+alternate),(-4,0xc001d00d+alternate),(0x178,0xd001f00d+alternate)]:write(a+off,value)

def native(p):
 state.clear();state.update(p=p,events=[],calls={})
 for (a,n),b in zip(regions,background(p[1])):vm.mem_write(a,b)
 for i in range(0,0x100,4):write(0xf55cd0+i,0x10000+i*257+p[1])
 write(0xd056f8,p[2]);write(0xbca2e0,p[19]);write(0x784cb0,p[20],1);actor_fields(p,p[2],0);actor_fields(p,p[3],1)
 previous=signed(p[5],16)
 for i in range(10):
  id=15+i if i else previous;next=0 if i==9 else 16+i;a=actor(id);write(a+0x60,next,2);write(a+2,39 if i+1==p[7] and i else 66,1);write(a+6,1,1);write(a+0x30,p[29],1);write(a+0x5d,0,1)
 a=actor(previous)
 for off,value in [(2,p[10]),(6,p[9]),(0x30,p[8]),(0x5d,p[11])]:write(a+off,value,1)
 head=0xffff if p[26]==6 else 100 if p[26] else 0;write(0xb350dc+p[31]*2,head,2);write(0xb350dc+(p[31]+1)*2,head,2)
 for i in range(3):
  a=actor(100+i);types={1:[70,68,69],2:[68,69,70],3:[69,7,7],4:[7,70,68]}.get(p[26],[39,7,69]);write(a,101+i if i<2 else 0xffff if p[26]==7 else 0,2);write(a+2,types[i],1);write(a+0x30,p[27],1);write(a+12,300+i*13,2);write(a+14,0x100+i*19,2)
 write(0x799a2e+signed(p[4],16)*264+(p[18]&255)*2,p[6],2)
 vm.mem_write(0x5cc46c,bytes(((i*37+p[1])^0x6d)&255 for i in range(0x8040)))
 sp=STACK+0x8000;vm.mem_write(sp-0x200,b'\xcc'*0x400);write(sp,STOP);write(sp+4,p[2]);write(sp+8,p[30]);saved=[(UC_X86_REG_EBX,0x11223344),(UC_X86_REG_EBP,0x22334455),(UC_X86_REG_ESI,0x33445566),(UC_X86_REG_EDI,0x44556677)]
 for r,v in saved+[(UC_X86_REG_ESP,sp),(UC_X86_REG_EFLAGS,2)]:vm.reg_write(r,v)
 vm.emu_start([0x4a2e90,0x49b400,0x49b3c0,0x4b57e0][p[0]],STOP,count=1000000)
 assert vm.reg_read(UC_X86_REG_EIP)==STOP and vm.reg_read(UC_X86_REG_ESP)==sp+4,'Native stack/budget'
 assert all(vm.reg_read(r)==v for r,v in saved),'Native saved registers'
 assert bytes(vm.mem_read(sp+12,128))==b'\xcc'*128,'Native caller guard'
 h=hashlib.sha256()
 for a,n in regions:h.update(struct.pack('<II',a,n));h.update(vm.mem_read(a,n))
 result=vm.reg_read(UC_X86_REG_EAX) if p[0]>=2 else 0
 return struct.pack('<I',result)+h.digest()+struct.pack('<I',len(state['events']))+b''.join(struct.pack('<24I',*e) for e in state['events'])
default=[0,123,1,257,3,4,100,2,1,1,66,0,1,4,7,2,10,103,11,2,1,0,0,4,9,0,1,1,256,1,70,257];cases=[]
def add(**kw):
 p=default.copy()
 for k,v in kw.items():p[int(k[1:])]=v&MASK
 cases.append(tuple(p))
for age,frame in itertools.product([0,1,102,103,104,105,126,127,128,254,255],[0,3,4,5,254,255]):add(p17=age,p13=frame)
for direction,orientation,enabled in itertools.product([-128,-1,0,7,8,31,32,127],[0,2,6,256,0x80000000,0x7fffffff],[0,1,128]):add(p14=direction,p19=orientation,p20=enabled)
for pattern,action,arrived in itertools.product(range(8),[1,4,12,255],[0,1,256]):add(p26=pattern,p27=action,p21=arrived)
for op,id,home in itertools.product([0,1],[0,1,257,1000,0x40000001,0x80000001],[-1,0,3,257]):add(p0=op,p2=id,p4=home)
for previous,action,status,kind in itertools.product([-1,0,4],[1,4,6,7,255],[0,1,2],[39,66,70]):add(p0=1,p5=previous,p8=action,p9=status,p10=kind)
for depth,action in itertools.product(range(11),[1,4,6,7]):add(p0=1,p7=depth,p29=action)
for frame,direction,orientation in itertools.product([0,10,11,12,254,255],[-128,-1,0,7,8,127],[0,2,256,0x80000000,0x7fffffff]):add(p0=1,p13=frame,p14=direction,p19=orientation)
for resource,stock,home in itertools.product([0,11,127,255],[0,0x7fff,0xfff0,0xffff],[-1,0,3,257]):add(p0=1,p18=resource,p6=stock,p9=0,p4=home)
for timer in [0,1,7,8,31,127,128,255,256,32767,0xffff,0x8000]:add(p0=1,p12=4,p16=timer)
for op,flags,mask,action in itertools.product([0,1],[1,2,4,8,16,32,64,128,256,512,3,1|16|64,2|256|512],[1,2,4,8,16],[1,4,6,7]):add(p0=op,p22=flags,p25=mask,p23=action,p21=1,p24=9)
for op,flags in itertools.product([0,1],[1|2|4|8|16|32|64|128|256|512,1|8|16,1|64|512]):add(p0=op,p22=flags,p25=0,p21=1)
for previous,kind,depth in itertools.product([-1,0,4],[39,66,70],range(11)):add(p0=2,p5=previous,p10=kind,p7=depth)
for pattern,type in itertools.product(range(8),[0,7,39,68,69,70,256,0x146,0xffffffff]):add(p0=3,p26=pattern,p30=type)
for tile in [0,1,255,256,65535,131071]:add(p0=0,p31=tile)
cases=sorted(set(cases),key=lambda p:(p[1],p));process=subprocess.Popen([str(exe),'--oracle-batch'],cwd=ROOT,stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE)
report={'format':1,'passed':False,'checked':0,'production':args.production,'native_sha256':SHA,'compiled_C_sha256':sha(exe.read_bytes()),'original_game_executed':False,'unicorn_version':__version__,'method':'Two complete current-actor callbacks with real49b3c0 eight-hop predecessor chain and unchanged4b57e0 signed tile-list query in native and compiled C. Whole watched records/grid/tables and nonuniform guards, ordered real-query entry/exit and deep-call arguments/intermediate fields, full consumed EAX, reload/caching mutations, native caller stack canary/saved registers and C stack balance/saved registers are compared.','limitations':['4b3ff0 movement,4b22e0 following and47d350 sound are controlled native endpoints. Full algorithms/actual movement and sound are excluded. Production keeps these existing entries; the named sound adapter passes through the current no-argument C sound no-op, with the native event retained only at the adapter boundary.','Real helper return wrappers apply explicit test-only mutations after the unchanged/rewritten function returns, to expose caller caching/reloads. These are not product behaviors or production stubs.','Mapped bounded actor chains/tile lists are the input domain. Original unchecked malformed/cyclic tile lists and unmapped IDs are excluded; chain helper retains its exact8-iteration limit and signed WORD links. Signed timer boundary cases use a full mapped lookup range, without enumerating all65536 values.','Callback EAX is discarded by the native dispatcher; complete chain/query EAX and movement-arrival control are compared. No RNG is called directly in this graph.']}
def exact(n):
 b=process.stdout.read(n);assert len(b)==n,'C stopped: '+process.stderr.read().decode(errors='replace');return b
started=time.monotonic();p=None
try:
 assert exact(4)==struct.pack('<I',0x43484149)
 for p in cases:
  process.stdin.write((' '.join(map(str,p))+'\n').encode());process.stdin.flush();header=exact(40);count=struct.unpack_from('<I',header,36)[0];actual=header+exact(count*96);expected=native(p)
  assert actual==expected,repr({'case':p,'C_header':actual[:40].hex(),'native_header':expected[:40].hex(),'first_difference':next((i for i,(a,b) in enumerate(zip(actual,expected)) if a!=b),None),'C_events':[list(struct.unpack_from('<24I',actual,40+i*96)) for i in range(count)],'native_events':state['events']})
  report['checked']+=1
  if report['checked']%500==0:print(json.dumps({'checked':report['checked'],'total':len(cases)}),flush=True)
 assert hits==allowed_set,'Uncovered native instructions: '+repr([hex(a) for a in sorted(allowed_set-hits)])
 report['passed']=True
except Exception as e:report.update(error=str(e),case=p,native_eip=hex(vm.reg_read(UC_X86_REG_EIP)))
finally:process.stdin.close();process.wait(timeout=10)
sources=[OWN/'include/actor_callbacks_chain.h',OWN/'runtime/actor_callbacks_chain.c',OWN/'runtime/actor_callbacks_chain_sound.c',OWN/'src/unit_01352.c',OWN/'tests/actor_callbacks_chain.c',Path(__file__),NATIVE,PHASE/'CMakeLists.txt',ROOT/'src/unit_01462.c',ROOT/'src/unit_01130.c']
report['source_hashes']={str(p.resolve().relative_to(ROOT)).replace('\\','/'):sha(p.read_bytes()) for p in sources};report['instruction_coverage']={hex(a):{'executed':sum(int(i['address'],16) in hits for i in f),'total':len(f),'missing':[i['address'] for i in f if int(i['address'],16) not in hits]} for a,f in functions.items()};report['instructions_executed']=len(hits);report['instructions_total']=len(allowed);report['watched_bytes']=sum(n for a,n in regions);report['elapsed_seconds']=round(time.monotonic()-started,3)
REPORT.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v for k,v in report.items() if k not in ['source_hashes','instruction_coverage']},indent=2));sys.exit(0 if report['passed'] else 1)
