"""Complete explicit-ID melee and real direction, exit, subpixel, route and classifier leaves."""
from pathlib import Path
import argparse,bisect,functools,hashlib,itertools,json,random,struct,subprocess,sys,time
ROOT=next(p for p in Path(__file__).resolve().parents if (p/'build/smoke-game/Pharaoh.exe').exists() and (p/'src').is_dir());PHASE=ROOT/'artifacts/actor_combat_phase'
sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_BLOCK,UC_HOOK_CODE,__version__
from unicorn.x86_const import *
parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--production',action='store_true');parser.add_argument('--exe',type=Path,default=ROOT/'build/agent-actor-combat/Debug/restore_actor_combat_verify.exe');parser.add_argument('--report',type=Path);args=parser.parse_args();exe=args.exe.resolve();OWN=ROOT if args.production else PHASE
NATIVE=ROOT/'metadata/actor-combat-native.json' if args.production else PHASE/'metadata/native.json';REPORT=args.report.resolve() if args.report else ROOT/'metadata/actor-combat-emulation.json' if args.production else PHASE/'metadata/emulation.json'
MASK=0xffffffff;sha=lambda b:hashlib.sha256(b).hexdigest();image=(ROOT/'build/smoke-game/Pharaoh.exe').read_bytes();SHA=sha(image);assert SHA=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
d=json.loads(NATIVE.read_text());assert d['native_sha256']==SHA;functions={int(a,16):f['assembly']['items'] for a,f in d['functions'].items()}
for f in d['functions'].values():assert not f['assembly']['has_more'] and len(f['assembly']['items'])==f['assembly']['total']
u16=lambda p:struct.unpack_from('<H',image,p)[0];u32=lambda p:struct.unpack_from('<I',image,p)[0];pe=u32(0x3c);opt=pe+24;table=opt+u16(pe+20);base=u32(opt+28);size=u32(opt+56)
vm=Uc(UC_ARCH_X86,UC_MODE_32);vm.mem_map(base,(size+4095)&~4095);vm.mem_write(base,image[:u32(opt+60)])
for k in range(u16(pe+6)):
 q=table+k*40;vm.mem_write(base+u32(q+12),image[u32(q+20):u32(q+20)+u32(q+16)])
STOP=0x33000000;STACK=0x34000000;vm.mem_map(STOP,4096);vm.mem_map(STACK,0x10000)
regions=[(0xbcb7c8,0xbcd600+1002*388+64-0xbcb7c8),(0x7090b0,0x3900),(0xd04e38,0x820),(0xd056f0,32),(0xeaac18,48)]
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


def formation(id):return (0xbcb9c8+id*144)&MASK
state={};hits=set();cache={};deep={0x47d350:(1,32),0x4b5050:(1,128),0x4b5210:(1,256),0x4b7ab0:(1,512)};spies={0x4b1d10:(2,1),0x4b3f00:(1,2),0x4b3250:(1,4),0x4ea9a0:(1,8),0x4ea9c0:(1,16),0x4b1da0:(1,64)};returns={int(i['address'],16):a for a in spies for i in functions[a] if i['instruction'].strip()=='RET'}
def block(cpu,a,n,user):
 if a in deep:return
 assert a in allowed_set,'Unexpected native block '+hex(a)
 if (a,n) not in cache:cache[a,n]=allowed[bisect.bisect_left(allowed,a):bisect.bisect_left(allowed,a+n)]
 hits.update(cache[a,n])
def event(address,argv,result=0):
 p=state['p'];a=actor(p[2]);t=actor(signed(p[4],16));f=formation(signed(p[6],16));e=[0]*24;e[0]=address;e[1]=len(argv);e[2:2+len(argv)]=argv;e[6]=result;e[7:24]=[read(0xd056f8),read(a+0x30,1),read(a+0x31,1),read(a+0x62,1),read(a+0x83,1),read(a+0x88,2),read(a+0x58,2),read(a+2,1),read(a+11,1),read(t+0x30,1),read(t+0x15,1),read(t+2,1),read(f+0x7a,2),read(a+0x42,2),read(a+0x44,2),read(a+0x5d,1),read(a+0x32,2)];state['events'].append(e)
def mutation(mask):
 p=state['p']
 if p[32] and not p[32]&mask:return
 a=actor(p[2]);t=actor(signed(p[4],16));f=formation(signed(p[6],16));g=formation(signed(p[7],16))
 if p[31]&1:write(0xd056f8,p[2] if read(0xd056f8)==p[3] else p[3])
 if p[31]&2:write(a+0x58,p[7],2)
 if p[31]&4:write(a+2,p[33],1);write(t+2,p[33],1)
 if p[31]&8:write(a+0x88,p[35],2)
 if p[31]&16:
  for base,kind in [(f,13),(g,12)]:write(base+4,kind,2);write(base+0x2a,6,2);write(base+0x5a,1,2)
 if p[31]&32:write(0xeaac28,p[34])
 if p[31]&64:write(a+7,p[25]^0x81,1);write(t+7,(p[25]>>8)^0x81,1)
 if p[31]&128:write(a+11,p[15]^7,1);write(t+11,(p[29]>>16)^7,1);write(f+0x6b,p[15],1);write(g+0x6b,p[15],1)
 if p[31]&256:
  for i in range(256):write(0x7090d4+i*56,p[20]^0x12340000);write(0x7090d8+i*56,p[21]^0x56780000)
 if p[31]&512:write(t+0x15,p[23]^128,1)
 if p[31]&1024:write(f+0x7a,p[30]>>16,2)
 if p[31]&2048:write(a+0x83,2,1);write(a+0x84,p[5],2);write(a+0x86,p[4],2)
def hook(cpu,a,n,user):
 sp=cpu.reg_read(UC_X86_REG_ESP)
 if a in spies:
  count=spies[a][0];argv=[read(sp+4+i*4) for i in range(count)];state['calls'][a]=argv;event(a,argv)
 elif a in returns:
  entry=returns[a];event(entry|0x80000000,state['calls'][entry],cpu.reg_read(UC_X86_REG_EAX) if entry in [0x4b1d10,0x4ea9a0,0x4ea9c0] else 0);mutation(spies[entry][1])
 else:
  count,mask=deep[a];argv=[read(sp+4+i*4) for i in range(count)];event(a,argv);mutation(mask)
  cpu.reg_write(UC_X86_REG_EAX,0x9abcdef0);cpu.reg_write(UC_X86_REG_ECX,0xa17e4567);cpu.reg_write(UC_X86_REG_EDX,0x5a5a5a5a);cpu.reg_write(UC_X86_REG_ESP,sp+4);cpu.reg_write(UC_X86_REG_EIP,read(sp))
vm.hook_add(UC_HOOK_BLOCK,block)
for a in [*spies,*returns,*deep]:vm.hook_add(UC_HOOK_CODE,hook,begin=a,end=a)
@functools.lru_cache(maxsize=8)
def background(seed):
 b=bytes(((i*13+(i>>5)*7+seed)^0xa7)&255 for i in range(sum(n for a,n in regions)));parts=[];offset=0
 for a,n in regions:parts.append(b[offset:offset+n]);offset+=n
 return parts

def target_fields(p,a,alternate):
 for off,value in [(2,p[18]^(1 if alternate else 0)),(6,p[11 if alternate else 9]),(0x30,p[12 if alternate else 10]),(7,p[25]),(11,p[29]>>16),(0x15,p[23])]:write(a+off,value,1)
 write(a+0x58,p[7]+alternate,2);write(a+0x88,p[35],2)
def formation_fields(p,id,target):
 f=formation(id);shift=16 if target else 0
 for off,value in [(4,p[26]>>shift),(0x5a,p[27]>>shift),(0x2a,p[28]>>shift),(0x7a,p[30]>>shift)]:write(f+off,value,2)
 write(f+0x6b,p[29]>>(8 if target else 0),1)
def native(p):
 state.clear();state.update(p=p,events=[],calls={})
 for (a,n),b in zip(regions,background(p[1])):vm.mem_write(a,b)
 a=actor(p[2]);write(0xd056f8,p[3]);write(0xeaac28,p[24])
 for off,value in [(2,p[17]),(6,1),(0x30,5),(8,p[39]),(9,p[16]),(11,p[15]),(7,p[25]>>8),(0x83,p[8]),(0x31,p[14]),(0x62,p[13]),(0x6f,p[38]),(0x70,p[38]>>8),(0x71,p[38]>>16),(0x5d,p[38]>>24)]:write(a+off,value,1)
 for off,value in [(0x58,p[6]),(0x88,p[4]),(0x84,p[4]),(0x86,p[5]),(0x32,p[36]),(0x42,p[20]),(0x44,p[21])]:write(a+off,value,2)
 target_fields(p,actor(signed(p[4],16)),0);target_fields(p,actor(signed(p[5],16)),1);formation_fields(p,signed(p[6],16),0);formation_fields(p,signed(p[7],16),1);formation_fields(p,signed(p[7]+1,16),1);write(0xd04e58+signed(p[36],16)*2,p[37],2)
 for i in range(256):
  for off,value in [(0,p[19]+i*256),(4,p[22]+i*3),(8,p[20]+i*257),(12,p[21]-i*13)]:write(0x7090cc+i*56+off,value)
 for type in [p[17]&255,p[18]&255,(p[18]^1)&255]:
  for off,value in [(0,p[19]),(4,p[22]),(8,p[20]),(12,p[21])]:write(0x7090cc+type*56+off,value)
 sp=STACK+0x8000;vm.mem_write(sp-0x200,b'\xcc'*0x400);write(sp,STOP)
 argv=[p[15],p[29]] if p[0]==1 else [p[18]] if p[0] in [4,5] else [p[2]]
 for i,v in enumerate(argv):write(sp+4+i*4,v)
 saved=[(UC_X86_REG_EBX,0x11223344),(UC_X86_REG_EBP,0x22334455),(UC_X86_REG_ESI,0x33445566),(UC_X86_REG_EDI,0x44556677)]
 for r,v in saved+[(UC_X86_REG_ESP,sp),(UC_X86_REG_EFLAGS,2)]:vm.reg_write(r,v)
 vm.emu_start([0x4b17d0,0x4b1d10,0x4b1da0,0x4b3f00,0x4ea9a0,0x4ea9c0,0x4b3250][p[0]],STOP,count=1000000)
 assert vm.reg_read(UC_X86_REG_EIP)==STOP and vm.reg_read(UC_X86_REG_ESP)==sp+4,'Native stack/budget'
 assert all(vm.reg_read(r)==v for r,v in saved),'Native saved registers'
 assert bytes(vm.mem_read(sp+4+len(argv)*4,128))==b'\xcc'*128,'Native caller guard'
 h=hashlib.sha256()
 for a,n in regions:h.update(struct.pack('<II',a,n));h.update(vm.mem_read(a,n))
 result=vm.reg_read(UC_X86_REG_EAX) if p[0] in [1,4,5] else 0
 return struct.pack('<I',result)+h.digest()+struct.pack('<I',len(state['events']))+b''.join(struct.pack('<24I',*e) for e in state['events'])
default=[0,123,1,257,4,5,1,2,2,1,1,1,1,23,5,0,2,11,12,0,20,0,100,0,0,0,12|(12<<16),1|(1<<16),1|(1<<16),0,0,0,0,82,256,0,1,1,0,8];cases=[]
def add(**kw):
 p=default.copy()
 for k,v in kw.items():p[int(k[1:])]=v&MASK
 cases.append(tuple(p))
for mode,status,action,status2,action2 in itertools.product([0,1,2,3,255],[0,1,2],[1,4],[0,1],[1,4]):add(p8=mode,p9=status,p10=action,p11=status2,p12=action2)
for tick,phase,kind in itertools.product([0,22,23,24,254,255],[0,5,6,127,128,255],[0,1,4,256]):add(p13=tick,p14=phase,p19=kind)
for af,tf,ac,tc,heading in itertools.product([0,11,12,13,57],[0,11,12,13,57],[-1,0,1,2,3,6],[-1,0,1,2,3,6],[0,4]):
 if af!=12 and ac not in [1,6]:continue
 if tf!=12 and tc not in [1,6]:continue
 add(p26=af|(tf<<16),p28=(ac&65535)|((tc&65535)<<16),p29=heading|(heading<<8)|(heading<<16))
for attack,defense,limit,damage in itertools.product([0,1,19,20,255,0x7fffffff,0x80000000,0xffffffff],[-1,0,19,20,21,0x7fffffff,0x80000000],[-1,0,255,256],[0,250]):add(p20=attack,p21=defense,p22=limit,p23=damage)
for type,owner,player,experience in itertools.product([11,23,68,69,70,82,83,84,102,103,104,255],[0,1],[0,1,256],[-32768,-11,-10,-1,0,9,10,99,100,32767]):
 if type not in [11,23] and experience!=99:continue
 add(p18=type,p25=owner|(owner<<8),p24=player,p30=experience&65535,p22=0,p23=255)
for op,id,target,formationid in itertools.product([0,2,3,6],[0,1,257,1000,0x40000001,0x80000001],[-1,0,4],[-1,0,1,40]):add(p0=op,p2=id,p4=target,p6=formationid,p7=formationid+1)
for x,y in itertools.product([-2147483648,-128,-1,0,1,7,8,255,256,2147483647],repeat=2):add(p0=1,p15=x,p29=y)
for direction,opacity,current,goal in itertools.product([-128,-1,0,1,2,3,4,5,6,7,8,127],[0,1,2,255],[0,1,254,255],[0,1,254,255]):add(p0=3,p16=direction,p38=opacity|(current<<8)|(goal<<16)|(1<<24),p20=65535,p21=0)
for route,owner in itertools.product([-1,0,1,999,1000],[-1,0,1,257]):add(p0=2,p36=route,p37=owner)
for op,type in itertools.product([4,5],list(range(64,112))+[-1,0,0x144,0x152,0x80000000,0x7fffffff]):add(p0=op,p18=type)
for flags,mask,kind in itertools.product([1,2,4,8,16,32,64,128,256,512,1024,2048,1|2|8,16|32|64|128],[1,2,4,8,16,32,64,128,256,512],[11,68,82]):add(p31=flags,p32=mask,p33=kind,p22=0,p23=255,p35=5,p24=0,p34=256)
for kind in [11,12,13]:add(p26=12|(kind<<16),p29=4<<8)
add(p31=8,p32=2,p35=5,p11=0)
add(p31=8,p32=2,p35=6)
cases=sorted(set(cases),key=lambda p:(p[1],p));process=subprocess.Popen([str(exe),'--oracle-batch'],cwd=ROOT,stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE)
report={'format':1,'passed':False,'checked':0,'production':args.production,'native_sha256':SHA,'compiled_C_sha256':sha(exe.read_bytes()),'original_game_executed':False,'unicorn_version':__version__,'method':'Full387-instruction explicit-ID melee plus actual23 direction,18 exit,50 subpixel and unchanged17 route/9+11 AL classifiers execute in native and compiled C. Whole guarded actor/formation/stats/route/global bytes, ordered calls and intermediate fields, full canonical direction result/AL classifier use, wrapped signed damage math and cached explicit-ID/formation/target contracts compare. Native caller stack/canary/saved registers and C stack balance/saved registers are checked.','limitations':['Physical sound47d350, death notification4b5050, hit notification4b5210 and formation notification4b7ab0 are controlled deep endpoints. Production retains these existing entries; named sound adapter explicitly retains/discards native event before current no-argument sound no-op. Their full algorithms, audio, RNG side effects and complete combat simulation are not proved.','Return wrappers may apply explicit test-only mutations after real leaves execute; they expose native cache/reload behavior without claiming these mutations are real product effects.','Mapped bounded actor,formation,stat and route records are exercised. Unmapped malformed IDs are excluded; no extra product validation is introduced. All515 original instructions are covered; the32-bit input space is not exhaustive.','This is state5 melee combat and its subpixel approach/exit helpers. General route/free-walk4A5DA0/4B3FF0 remains outside this proof. No implicit ECX receiver, x87 input or direct RNG exists in the proved graph.']}
def exact(n):
 b=process.stdout.read(n);assert len(b)==n,'C stopped: '+process.stderr.read().decode(errors='replace');return b
started=time.monotonic();p=None
try:
 assert exact(4)==struct.pack('<I',0x434f4d42)
 for p in cases:
  process.stdin.write((' '.join(map(str,p))+'\n').encode());process.stdin.flush();header=exact(40);count=struct.unpack_from('<I',header,36)[0];actual=header+exact(count*96);expected=native(p)
  assert actual==expected,repr({'case':p,'C_header':actual[:40].hex(),'native_header':expected[:40].hex(),'first_difference':next((i for i,(a,b) in enumerate(zip(actual,expected)) if a!=b),None),'first_event_difference':[(i,list(struct.unpack_from('<24I',actual,40+i*96)),e) for i,e in enumerate(state['events']) if i<count and list(struct.unpack_from('<24I',actual,40+i*96))!=e][:1]})
  report['checked']+=1
  if report['checked']%500==0:print(json.dumps({'checked':report['checked'],'total':len(cases)}),flush=True)
 assert hits==allowed_set,'Uncovered native instructions: '+repr([hex(a) for a in sorted(allowed_set-hits)])
 report['passed']=True
except Exception as e:report.update(error=str(e),case=p,native_eip=hex(vm.reg_read(UC_X86_REG_EIP)))
finally:process.stdin.close();process.wait(timeout=10)
sources=[OWN/'include/actor_combat.h',OWN/'runtime/actor_combat_boundaries.c',OWN/'tests/actor_combat.c',*[OWN/f'src/unit_{n:05d}.c' for n in [1414,1415,1417,1435]],Path(__file__),NATIVE,PHASE/'CMakeLists.txt',*[ROOT/f'src/unit_{n:05d}.c' for n in [1427,2065,2066,1130]]]
report['source_hashes']={str(p.resolve().relative_to(ROOT)).replace('\\','/'):sha(p.read_bytes()) for p in sources};report['instruction_coverage']={hex(a):{'executed':sum(int(i['address'],16) in hits for i in f),'total':len(f),'missing':[i['address'] for i in f if int(i['address'],16) not in hits]} for a,f in functions.items()};report['instructions_executed']=len(hits);report['instructions_total']=len(allowed);report['watched_bytes']=sum(n for a,n in regions);report['elapsed_seconds']=round(time.monotonic()-started,3)
REPORT.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v for k,v in report.items() if k not in ['source_hashes','instruction_coverage']},indent=2));sys.exit(0 if report['passed'] else 1)
