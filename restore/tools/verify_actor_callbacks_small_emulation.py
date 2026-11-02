"""Four missing actor callbacks with real state, stock and classifier helpers."""
from pathlib import Path
import argparse,bisect,functools,hashlib,itertools,json,random,struct,subprocess,sys,time
ROOT=next(p for p in Path(__file__).resolve().parents if (p/'build/smoke-game/Pharaoh.exe').exists() and (p/'src').is_dir());PHASE=ROOT/'artifacts/actor_callbacks_small_phase'
sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_BLOCK,UC_HOOK_CODE,__version__
from unicorn.x86_const import *
parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--production',action='store_true');parser.add_argument('--exe',type=Path,default=ROOT/'build/agent-actor-callbacks-small/Debug/restore_actor_callbacks_small_verify.exe');parser.add_argument('--report',type=Path);args=parser.parse_args();exe=args.exe.resolve();OWN=ROOT if args.production else PHASE
NATIVE=ROOT/'metadata/actor-callbacks-small-native.json' if args.production else PHASE/'metadata/native.json';REPORT=args.report.resolve() if args.report else ROOT/'metadata/actor-callbacks-small-emulation.json' if args.production else PHASE/'metadata/emulation.json'
MASK=0xffffffff;sha=lambda b:hashlib.sha256(b).hexdigest();image=(ROOT/'build/smoke-game/Pharaoh.exe').read_bytes();SHA=sha(image);assert SHA=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
d=json.loads(NATIVE.read_text());assert d['native_sha256']==SHA;functions={int(a,16):f['assembly']['items'] for a,f in d['functions'].items()}
for f in d['functions'].values():assert not f['assembly']['has_more'] and len(f['assembly']['items'])==f['assembly']['total']
u16=lambda p:struct.unpack_from('<H',image,p)[0];u32=lambda p:struct.unpack_from('<I',image,p)[0];pe=u32(0x3c);opt=pe+24;table=opt+u16(pe+20);base=u32(opt+28);size=u32(opt+56)
vm=Uc(UC_ARCH_X86,UC_MODE_32);vm.mem_map(base,(size+4095)&~4095);vm.mem_write(base,image[:u32(opt+60)])
for k in range(u16(pe+6)):
 q=table+k*40;vm.mem_write(base+u32(q+12),image[u32(q+20):u32(q+20)+u32(q+16)])
STOP=0x33000000;STACK=0x34000000;vm.mem_map(STOP,4096);vm.mem_map(STACK,0x10000)
regions=[(0xbcd5d8,1002*388+64),(0x7998b0,302*264+64),(0x5cc46c,0x8040),(0xf53dc0,0x840),(0xf55bb0,0x900),(0x7090d0,0x3900),(0xd056f0,32),(0xbca2d0,48),(0xe94df0,48),(0xd3b4d0,48)]
allowed=sorted(int(i['address'],16) for f in functions.values() for i in f);allowed_set=set(allowed)
for f in functions.values():
 for i in f:
  b=bytes.fromhex(i['bytes']);assert bytes(vm.mem_read(int(i['address'],16),len(b)))==b
assert list(struct.unpack('<5I',vm.mem_read(0x494190,20)))==[0x49411c,0x4940f8,0x494110,0x494104,0x4940ec]
read=lambda a,n=4:int.from_bytes(vm.mem_read(a&MASK,n),'little')
def write(a,v,n=4):vm.mem_write(a&MASK,(v&((1<<(n*8))-1)).to_bytes(n,'little'))
def signed(v,n=32):return (v&((1<<n)-1))-(1<<n) if v&(1<<(n-1)) else v&((1<<n)-1)
def actor(id):return (0xbcd600+id*388)&MASK
def current():return actor(read(0xd056f8))
def building(id):return (0x7999d8+id*264)&MASK
state={};hits=set();cache={}
deep={0x47fe60:(4,1),0x4a5da0:(2,2),0x4b3250:(1,4),0x4b1ef0:(2,8),0x4b1d50:(1,16),0x4b17d0:(1,32)}
spies={0x493610,0x4578b0,0x4579d0,0x4c5120};returns={int(i['address'],16):a for a in spies for i in functions[a] if i['instruction'].strip()=='RET'}
def block(cpu,a,n,user):
 if a in deep:return
 assert a in allowed_set,'Unexpected native block '+hex(a)
 if (a,n) not in cache:cache[a,n]=allowed[bisect.bisect_left(allowed,a):bisect.bisect_left(allowed,a+n)]
 hits.update(cache[a,n])
def event(address,argv,result=0):
 a=current();e=[0]*24;e[0]=address;e[1]=len(argv);e[2:2+len(argv)]=argv;e[6]=result;e[7:22]=[read(0xd056f8),read(a+0x30,1),read(a-7,1),read(a+9,1),read(a+0x3c,2),read(a+0x3a,2),read(a+0x6a,1),read(a+4,1),read(a+6,1),read(a-4),read(0xbca2e0),read(0xe94e00),read(a+0x1c,2),read(a+0x1e,2),read(a+0x52,2)];state['events'].append(e)
def toggle():
 p=state['p'];write(0xd056f8,p[3] if read(0xd056f8)==p[2] else p[2])
def mutate_image():
 p=state['p']
 if p[22]&256:
  for i in range(0,0x900,4):write(0xf55bb0+i,read(0xf55bb0+i)^0x120000)
 if p[22]&512:write(0xbca2e0,p[28]);write(0xe94e00,p[28]^0x12340000)
def after_call(a):
 p=state['p']
 if a==0x493610 and p[22]&4:toggle()
 if a==0x4578b0 and p[22]&8:toggle()
 if a==0x4579d0 and p[22]&16:toggle()
 if a==0x4c5120:
  if p[22]&32:toggle()
  mutate_image()
 if a in [0x493610,0x4578b0,0x4579d0]:
  a=current()
  if p[22]&1024:write(a+0x30,p[23],1)
  if p[22]&2048:write(a+9,p[24],1)
def hook(cpu,a,n,user):
 sp=cpu.reg_read(UC_X86_REG_ESP);p=state['p']
 if a in spies:
  argv=[read(sp+4)];state['calls'][a]=argv;event(a,argv)
 elif a in returns:
  entry=returns[a];event(entry|0x80000000,state['calls'][entry],cpu.reg_read(UC_X86_REG_EAX) if entry!=0x493610 else 0);after_call(entry)
 else:
  count,mask=deep[a];argv=[read(sp+4+i*4) for i in range(count)];event(a,argv)
  if not p[25] or p[25]&mask:
   if p[22]&1 or a==0x4b3250 and p[22]&2:toggle()
   target=current()
   if p[22]&64:write(target+0x30,p[23],1)
   if p[22]&128:write(target+9,p[24],1)
   mutate_image()
  if a==0x47fe60:write(0xd3b4e4,p[26]);write(0xd3b4e0,p[27])
  cpu.reg_write(UC_X86_REG_EAX,p[21] if a==0x47fe60 else 0x9abcdef0);cpu.reg_write(UC_X86_REG_ECX,0xa17e4567);cpu.reg_write(UC_X86_REG_EDX,0x5a5a5a5a);cpu.reg_write(UC_X86_REG_ESP,sp+4);cpu.reg_write(UC_X86_REG_EIP,read(sp))
vm.hook_add(UC_HOOK_BLOCK,block)
for a in [*spies,*returns,*deep]:vm.hook_add(UC_HOOK_CODE,hook,begin=a,end=a)
@functools.lru_cache(maxsize=8)
def background(seed):
 b=bytes(((i*13+(i>>5)*7+seed)^0xa7)&255 for i in range(sum(n for a,n in regions)));parts=[];offset=0
 for a,n in regions:parts.append(b[offset:offset+n]);offset+=n
 return parts

def actor_fields(p,id,alt):
 a=actor(id);type=([65,40,26,27][p[0]] if p[0]<4 else p[30])+(1 if alt else 0)
 for off,value in [(2,type),(0x30,p[10]),(-7,p[11]+(5 if alt else 0)),(9,p[12]+(3 if alt else 0)),(6,0)]:write(a+off,value,1)
 for off,value in [(0x52,p[4]+(1 if alt else 0)),(0x3c,p[13]+(7 if alt else 0)),(0x2e,p[14]),(0x3a,p[15]+(11 if alt else 0))]:write(a+off,value,2)
 write(a-4,0xc001d00d+(1 if alt else 0));write(0x7090ec+(type&255)*56,p[29]^(0x12340000 if alt else 0))
def building_fields(p,home,alt):
 b=building(home);write(b,p[5],1);write(b+3,3+(home&3),1)
 for off,value in [(6,home*3+300),(8,257-home),(16,p[6]),(0x4a,p[7]),(0x2a,p[8]),(0xb4,p[9])]:write(b+off,value,2)
 for j in range(-1,12):
  if p[19]==0:value=0
  elif p[19]==1:value=signed(p[20],16)
  elif p[19]==2:value=((j+1)*17+p[1])%512-256
  elif p[19]==3:value=j*2-7
  elif p[19]==4:value=32767 if j==signed(p[31]) else -1
  else:value=signed(p[20]|0x8000,16)
  write(b+0x56+j*2,value+(1 if alt else 0),2)
def native(p):
 state.clear();state.update(p=p,events=[],calls={})
 for (a,n),b in zip(regions,background(p[1])):vm.mem_write(a,b)
 for i in range(0,0x900,4):write(0xf55bb0+i,0x10000+i*257+p[1])
 write(0xd056f8,p[2]);write(0xbca2e0,p[16]);actor_fields(p,p[2],False);actor_fields(p,p[3],True);building_fields(p,p[4],False);building_fields(p,(p[4]+1)&MASK,True)
 write(0xf53dd8,p[17]);resources=[0,1,2,3,4,5,6,7,8,13,15,17,19,9,MASK,20]
 for i in range(512):write(0xf53ddc+i*4,resources[i%16] if p[18]==0x80000000 else p[18])
 vm.mem_write(0x5cc46c,bytes(((j*37+p[1])^0x6d)&255 for j in range(0x8040)))
 entries=[0x49b390,0x496f20,0x493ec0,0x494010,0x493610,0x4578b0,0x4579d0,0x4c5120];sp=STACK+0x8000;vm.mem_write(sp-0x200,b'\xcc'*0x400);write(sp,STOP)
 if p[0]>=4:write(sp+4,1 if p[0]==4 else p[4])
 saved=[(UC_X86_REG_EBX,0x11223344),(UC_X86_REG_EBP,0x22334455),(UC_X86_REG_ESI,0x33445566),(UC_X86_REG_EDI,0x44556677)]
 for r,v in saved+[(UC_X86_REG_ESP,sp),(UC_X86_REG_EFLAGS,2)]:vm.reg_write(r,v)
 vm.emu_start(entries[p[0]],STOP,count=1000000)
 assert vm.reg_read(UC_X86_REG_EIP)==STOP and vm.reg_read(UC_X86_REG_ESP)==sp+4,'Native stack/budget'
 assert all(vm.reg_read(r)==v for r,v in saved),'Native saved registers'
 assert bytes(vm.mem_read(sp+8,128))==b'\xcc'*128,'Native caller guard'
 h=hashlib.sha256()
 for a,n in regions:h.update(struct.pack('<II',a,n));h.update(vm.mem_read(a,n))
 result=vm.reg_read(UC_X86_REG_EAX) if p[0]>=5 else 0
 return struct.pack('<I',result)+h.digest()+struct.pack('<I',len(state['events']))+b''.join(struct.pack('<24I',*e) for e in state['events'])
default=[0,123,1,257,1,1,70,0,1,1,1,10,7,95,10,99,2,16,0x80000000,0,0,1,0,2,9,0,0x1234,0xabcd,0x100,0x12345678,65,0];cases=[]
def add(**kw):
 p=default.copy()
 for k,v in kw.items():p[int(k[1:])]=v&MASK
 if actor(p[2])==actor(p[3]) and 'p3' not in kw:p[3]=1
 if 'p8' not in kw:p[8]=p[2]
 if 'p9' not in kw:p[9]=p[2]
 cases.append(tuple(p))
for op,id,home,action in itertools.product(range(4),[0,1,257,1000,0x40000001,0x80000001],[-1,0,1,257],[0,1,2,4,5,8,9,255]):add(p0=op,p2=id,p4=home,p10=action)
for op,status,slot1,slot2,disabled in itertools.product([1,2,3],[0,1,128],[0,1,-1],[0,1,-1],[0,1,256]):add(p0=op,p5=status,p8=slot1,p9=slot2,p7=disabled)
for op,frame,direction,orientation in itertools.product([1,2,3],[0,10,11,12,254,255],[-128,-1,0,7,8,127],[0,2,6,256,0x80000000,0x7fffffff]):add(p0=op,p11=frame,p12=direction,p16=orientation,p10=8)
for op,timer in itertools.product([1,2,3],[0,1,7,8,31,127,128,255,256,32767,0xffff,0x8000]):add(p0=op,p10=4,p14=timer)
for counter,limit,action,road in itertools.product([0,95,96,127,128,383,384,0x7fff,0x8000,0xffff],[0,1,128,384,0xffff],[0,1,2,4,5,8],[0,1,0x100]):add(p0=4,p13=counter,p15=limit,p10=action,p21=road)
for op,count,resource,stock,kind in itertools.product([5,6],[0,1,16,256,300,-1],[0,1,8,13,15,17,19,9,-1,256,0x80000000],[0,1,2,3,5],[70,190,0x1046,0x10be]):
 if op==6 and (count!=1 or resource!=1):continue
 if stock in [1,2,3,5] and count not in [1,16]:continue
 add(p0=op,p17=count,p18=resource,p19=stock,p20=300,p6=kind)
for index in range(-1,12):add(p0=5,p19=4,p31=index,p17=16,p18=0x80000000)
for index in range(8,12):add(p0=6,p19=4,p31=index)
for kind in list(range(59,72))+[190,0x103c,-1]:add(p0=7,p6=kind)
for kind,action in itertools.product(list(range(59,71))+[0x103c,-1],[1,4,8]):add(p0=3,p6=kind,p10=action)
for op,flags,action,direction in itertools.product([1,2,3,4],[1,2,4,8,16,32,64,128,256,512,1024,2048,1|2|4,8|16|32,32|256|512],[1,2,4,5,8],[8,9,10,11]):add(p0=op,p22=flags,p23=action,p24=direction,p13=384,p10=action,p6=60 if op==3 else 70)
for op,mask,flags in itertools.product([1,2,3,4],[1,2,4,8,16,32],[1|64|128,2|256|512]):add(p0=op,p25=mask,p22=flags,p10=1,p13=384,p6=60 if op==3 else 70)
# Use each immutable background together; every case is independently prepared.
cases=sorted(set(cases),key=lambda p:(p[1],p))
process=subprocess.Popen([str(exe),'--oracle-batch'],cwd=ROOT,stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE)
report={'format':1,'passed':False,'checked':0,'production':args.production,'native_sha256':SHA,'compiled_C_sha256':sha(exe.read_bytes()),'original_game_executed':False,'unicorn_version':__version__,
 'method':'Four complete global-current-actor callbacks plus actual493610 state helper,4578b0/4579d0 stock queries and unchanged4c5120 classifier execute in both native and C graphs. Compare full record/table/global guards bySHA256, exact ordered entry/exit/deep-call arguments and intermediate actor fields, current-actor reload mutations, full consumed query/classifier returns and native/C stack/saved registers. Original ignored stack word is observed at the named C state-adapter boundary.',
 'limitations':['Six road/path/movement/death endpoints (47fe60,4a5da0,4b3250,4b1ef0,4b1d50,4b17d0) are controlled. Their production algorithms and complete actor simulation are not proved.','State/query/classifier wrappers may apply explicit test-only mutations after real function returns, to verify caller reload/caching contracts. These wrappers are not production stubs.','Mapped actor/building records and resource lists through512 entries are the input domain; malformed unmapped IDs/list extents are excluded. Signed WORD timer boundary cases use mapped lookup guards covering the complete native index range; all65536 timer values are not enumerated.','The four callbacks and state entry are void; actor dispatcher489c52 discards callback EAX. Consumed stock/classifier EAX is compared. No function in this bounded graph directly calls RNG.']}
def exact(n):
 b=process.stdout.read(n);assert len(b)==n,'C stopped: '+process.stderr.read().decode(errors='replace');return b
started=time.monotonic();p=None
try:
 assert exact(4)==struct.pack('<I',0x41534342)
 for p in cases:
  process.stdin.write((' '.join(map(str,p))+'\n').encode());process.stdin.flush();header=exact(40);count=struct.unpack_from('<I',header,36)[0];actual=header+exact(count*96);expected=native(p)
  assert actual==expected,repr({'case':p,'C_header':actual[:40].hex(),'native_header':expected[:40].hex(),'first_difference':next((i for i,(a,b) in enumerate(zip(actual,expected)) if a!=b),None),'first_event_difference':[(i,list(struct.unpack_from('<24I',actual,40+i*96)),e) for i,e in enumerate(state['events']) if i<count and list(struct.unpack_from('<24I',actual,40+i*96))!=e][:1]})
  report['checked']+=1
  if report['checked']%500==0:print(json.dumps({'checked':report['checked'],'total':len(cases)}),flush=True)
 assert hits==allowed_set,'Uncovered native instructions: '+repr([hex(a) for a in sorted(allowed_set-hits)])
 report['passed']=True
except Exception as e:report.update(error=str(e),case=p,native_eip=hex(vm.reg_read(UC_X86_REG_EIP)))
finally:process.stdin.close();process.wait(timeout=10)
sources=[OWN/'include/actor_callbacks_small.h',OWN/'runtime/actor_callbacks_small.c',OWN/'runtime/actor_callbacks_small_state_adapter.c',OWN/'tests/actor_callbacks_small.c',OWN/'src/unit_01320.c',OWN/'src/unit_00825.c',OWN/'src/unit_00826.c',Path(__file__),NATIVE,PHASE/'CMakeLists.txt',ROOT/'src/unit_01602.c']
report['source_hashes']={str(p.resolve().relative_to(ROOT)).replace('\\','/'):sha(p.read_bytes()) for p in sources};report['instruction_coverage']={hex(a):{'executed':sum(int(i['address'],16) in hits for i in f),'total':len(f),'missing':[i['address'] for i in f if int(i['address'],16) not in hits]} for a,f in functions.items()};report['instructions_executed']=len(hits);report['instructions_total']=len(allowed);report['watched_bytes']=sum(n for a,n in regions);report['elapsed_seconds']=round(time.monotonic()-started,3)
REPORT.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v for k,v in report.items() if k not in ['source_hashes','instruction_coverage']},indent=2));sys.exit(0 if report['passed'] else 1)
