"""Compare complete ambient selection/geometry, real RNG and x87 __ftol to C."""
from pathlib import Path
import argparse,bisect,hashlib,itertools,json,random,struct,subprocess,sys,time
ROOT=Path(__file__).resolve().parents[1]
sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_BLOCK,UC_HOOK_CODE,__version__
from unicorn.x86_const import *
parser=argparse.ArgumentParser(description=__doc__)
parser.add_argument('--exe',type=Path,default=ROOT/'build/agent-ambient-sound/Debug/restore_ambient_sound_verify.exe')
args=parser.parse_args();exe=args.exe.resolve()
sha=lambda b:hashlib.sha256(b).hexdigest()
image=(ROOT/'build/smoke-game/Pharaoh.exe').read_bytes();digest=sha(image)
assert digest=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
dossier=json.loads((ROOT/'metadata/ambient-sound-native.json').read_text());assert dossier['native_sha256']==digest
functions={int(a,16):f['assembly']['items'] for a,f in dossier['functions'].items()}
for f in dossier['functions'].values():assert not f['assembly']['has_more'] and len(f['assembly']['items'])==f['assembly']['total']
rng=json.loads((ROOT/'metadata/music-tick-native.json').read_text());assert rng['native_sha256']==digest
functions[0x562348]=rng['functions']['0x00562348']['items']
u16=lambda p:struct.unpack_from('<H',image,p)[0];u32=lambda p:struct.unpack_from('<I',image,p)[0]
pe=u32(0x3c);opt=pe+24;table=opt+u16(pe+20);base=u32(opt+28);size=u32(opt+56)
vm=Uc(UC_ARCH_X86,UC_MODE_32);vm.mem_map(base,(size+4095)&~4095);vm.mem_write(base,image[:u32(opt+60)])
for i in range(u16(pe+6)):
 p=table+i*40;vm.mem_write(base+u32(p+12),image[u32(p+20):u32(p+20)+u32(p+16)])
allowed=sorted(int(i['address'],16) for f in functions.values() for i in f);allowed_set=set(allowed)
for f in functions.values():
 for i in f:
  raw=bytes.fromhex(i['bytes']);assert bytes(vm.mem_read(int(i['address'],16),len(raw)))==raw
assert bytes(vm.mem_read(0x570150,4))==struct.pack('<f',128.0)
STOP=0x33000000;STACK=0x34000000;GRID=0x70e494;GRID_COUNT=231*128
vm.mem_map(STOP,4096);vm.mem_map(STACK,0x10000)
regions=[(0x70e3f0,0xb0),(0x604940,32),(0xe92d98,40),(0xf53f50,48),(0xe38d88,0x150),(0xf55b80,32)]
read=lambda a,n=4:int.from_bytes(vm.mem_read(a,n),'little')
def write(a,v,n=4):vm.mem_write(a,(v&((1<<(8*n))-1)).to_bytes(n,'little'))
def signed(v,n=32):return (v&((1<<n)-1))-(1<<n) if v&(1<<(n-1)) else v&((1<<n)-1)
executed=set();blocks={};state={};current=None
coord_returns=[int(i['address'],16) for i in functions[0x430960] if i['instruction']=='RET']
def block(cpu,a,n,user):
 if STOP<=a<STOP+4096 or a==0x55f0e8:return
 assert a in allowed_set,'Unexpected native block '+hex(a)
 if (a,n) not in blocks:blocks[a,n]=allowed[bisect.bisect_left(allowed,a):bisect.bisect_left(allowed,a+n)]
 executed.update(blocks[a,n])
def event(tag):
 e=[tag]+[0]*9;state['events'].append(e);return e
def endpoint(cpu,a,n,user):
 sp=cpu.reg_read(UC_X86_REG_ESP)
 if a==0x562348:
  e=event(1);e[1]=read(0x604950);state['rng']=e
 elif a==0x562365:
  e=state['rng'];e[2]=cpu.reg_read(UC_X86_REG_EAX);e[3]=read(0x604950)
 elif a==0x430960:
  e=event(2);e[1:4]=[read(sp+i*4) for i in [1,2,3]];state['coord']=e
 elif a in coord_returns:state['coord'][4]=cpu.reg_read(UC_X86_REG_EAX)
 else:
  assert a==0x55f0e8
  e=event(3);e[1]=cpu.reg_read(UC_X86_REG_ECX);e[2]=read(sp+4);e[3:8]=[read(e[2]+i*4) for i in range(5)]
  if current[15]&1:
   write(0xe92da8,0x10203040);write(0xf53f60,read(0xf53f60)^0x12340000);write(0x604950,read(0x604950)^0xc001d00d)
  cpu.reg_write(UC_X86_REG_EAX,current[15]);cpu.reg_write(UC_X86_REG_ESP,sp+8);cpu.reg_write(UC_X86_REG_EIP,read(sp))
vm.hook_add(UC_HOOK_BLOCK,block)
for a in [0x562348,0x562365,0x430960,*coord_returns,0x55f0e8]:vm.hook_add(UC_HOOK_CODE,endpoint,begin=a,end=a)
vm.mem_write(STOP+0x20,b'\xdb\xe3\xd9\x2d'+struct.pack('<I',STOP+0x100)+b'\xd9\xe8')
vm.mem_write(STOP+0x40,b'\xdb\x3d'+struct.pack('<I',STOP+0x110))
def prepare(p):
 global current
 current=p;state.clear();state['events']=[]
 for a,n in regions:vm.mem_write(a,b'\xa5'*n)
 grid=struct.pack('<%dI'%GRID_COUNT,*[((i*37+p[1])%60000 if p[0]==1 and not p[15]&2 else p[18]) for i in range(GRID_COUNT)])
 vm.mem_write(GRID,grid)
 for a,v in [(0x70e490,p[19]),(0x70e48c,p[20]),(0x70e460,p[21]),(0x70e464,p[22]),(0x70e474,p[23]),(0x70e478,p[24]),(0x70e44c,p[26]),(0x70e458,p[27]),(0x70e45c,p[28]),(0x604950,p[1]),(0xf55b88,p[2]),(0xf53f78,p[4]),(0xf53f60,p[5]),(0xf53f64,p[6]),(0xe38d98,p[7]),(0xe38ed0,p[14])]:write(a,v)
 write(0xe38ec0,p[3],1)
 vm.mem_write(0xaf5988,b'\x5a'*16);vm.mem_write(0xb4e6f8,b'\x5a'*12);write(0xaf598c,p[8]);write(0xb4e6fc,p[9],2)
 terrain=0xaf598c+(p[18] if p[18]<65536 else 0)*4;building=0xb4e6fc+(p[18] if p[18]<65536 else 0)*2
 if terrain!=0xaf598c:vm.mem_write(terrain-4,b'\x5a'*24);write(terrain,p[8])
 if building!=0xb4e6fc:vm.mem_write(building-4,b'\x5a'*16);write(building,p[9],2)
 record=0x7999e8+signed(p[9],16)*264;vm.mem_write(record-8,b'\x69'*280);write(record,p[10],2);write(record+2,p[11],2)
 vm.mem_write(0xf54540,struct.pack('<%dI'%(0xec0//4),*[(p[13] if signed(p[13])<0 else (p[13]+i*17)&0xffffffff) for i in range(0xec0//4)]))
 for i in range(6):write(0xf5457c+i*48,p[12])
 if p[10]&65535!=57:write(0xf54680+signed(p[10],16)*20,p[12])
 for a in [0xf549a0,0xf549f0,0xf54a04]:write(a,p[12])
 readonly=[(GRID,grid),(0xf54540,bytes(vm.mem_read(0xf54540,0xec0))),(record-8,bytes(vm.mem_read(record-8,280))),(terrain-4,bytes(vm.mem_read(terrain-4,24))),(building-4,bytes(vm.mem_read(building-4,16)))]
 write(STOP+0x100,p[25],2);vm.emu_start(STOP+0x20,STOP+0x2a)
 return readonly
saved=[(UC_X86_REG_EBX,0x11223344),(UC_X86_REG_EBP,0x22334455),(UC_X86_REG_ESI,0x33445566),(UC_X86_REG_EDI,0x44556677)]
def native(p):
 readonly=prepare(p);sp=STACK+0x8000;vm.mem_write(sp-0x200,b'\xcc'*0x400)
 for i,v in enumerate([STOP,p[16],p[17],p[29]]):write(sp+i*4,v)
 for r,v in saved+[(UC_X86_REG_ESP,sp),(UC_X86_REG_EFLAGS,2)]:vm.reg_write(r,v)
 if p[0]==2:
  addr=0xf53f60 if p[4] else 0xf53f64;previous=read(addr);write(addr,p[17]);write(sp+4,p[16])
  vm.emu_start(0x53ae49,0x53ae71,count=10000)
  assert vm.reg_read(UC_X86_REG_EIP)==0x53ae71 and vm.reg_read(UC_X86_REG_ESP)==sp
  vm.emu_start(0x53ae7b,0x53ae7d,count=2);result=vm.reg_read(UC_X86_REG_EAX);write(addr,previous)
 else:
  entry={0:0x53acf0,1:0x430960,3:0x562348}[p[0]];vm.emu_start(entry,STOP,count=10000)
  assert vm.reg_read(UC_X86_REG_EIP)==STOP and vm.reg_read(UC_X86_REG_ESP)==sp+4
  result=vm.reg_read(UC_X86_REG_EAX) if p[0] else 0
 assert all(vm.reg_read(r)==v for r,v in saved),'Native callee-saved registers'
 assert bytes(vm.mem_read(sp+16,0x100))==b'\xcc'*0x100,'Native caller stack guard'
 assert all(bytes(vm.mem_read(a,len(b)))==b for a,b in readonly),'Native source mutation'
 cw=vm.reg_read(UC_X86_REG_FPCW);sw=vm.reg_read(UC_X86_REG_FPSW)&(0x381e if p[0]==2 and not p[17] else 0x381f)
 vm.emu_start(STOP+0x40,STOP+0x46);sentinel=bytes(vm.mem_read(STOP+0x110,10))
 assert sentinel==bytes.fromhex('0000000000000080ff3f'),'Native x87 stack imbalance'
 return struct.pack('<III',result,cw,sw)+sentinel+struct.pack('<I',len(state['events']))+b''.join(struct.pack('<10I',*e) for e in state['events'])+b''.join(bytes(vm.mem_read(a,n)) for a,n in regions)
default=[0,1234,0x10000,128,0,640,800,600,0x10000,0,0,0,4,500,0x12345678,0,45,600,500,0,0,40,14,1,1,0x37f,15,58,29,0xabcdef01]
cases=[]
def add(**kw):
 p=default.copy()
 for k,v in kw.items():p[int(k[1:])]=v&0xffffffff
 cases.append(p)
seeds=[0,1,2,1234,255,256,0x7fffffff,0x80000000,0xffffffff]
flags=[0,1,2,3,4,6,0x80004,0x800,0x10000,0x80000,0x100,0x80804,0xffffffff]
for seed,terrain,flip in itertools.product(seeds,flags,[0,0x10000]):add(p1=seed,p8=terrain,p4=flip,p5=1024,p6=800,p7=768)
for seed,building,kind,subtype in itertools.product(seeds,[1,257,999,-1],[0,39,57,128,-1],[0,11,13,0x100b]):
 if kind!=57 and subtype:continue
 add(p1=seed,p9=building,p10=kind,p11=subtype,p12=4,p4=seed&1,p5=1024,p6=800)
for enabled,byte,seed in itertools.product([0,1,0x10000],[0,1,128],[1,0xffffffff]):add(p2=enabled,p3=byte,p1=seed)
for sound,building,terrain,seed in itertools.product([-2,-1,0,1,0x7fffffff],[0,257],[1,4,0x800,0x10000],[1,1234,0xffffffff]):add(p13=sound,p9=building,p10=57,p11=13,p8=terrain,p1=seed)
for width,flip,height,origin,seed in itertools.product([640,800,1024],[0,0x10000],[480,768],[-300,0,257],[1,1234]):add(p5=width,p6=width,p4=flip,p7=height,p19=origin,p20=origin,p1=seed,p15=1)
for x,y in itertools.product(range(-1,182,7),range(-1,77,3)):add(p0=1,p16=x,p17=y,p21=5,p22=3,p23=0,p24=0)
for x,y in itertools.product([-301,-300,-299,-1,0,1,256,257,258,1023,1024,2047],[0,256,257,258,511,512,1023]):add(p0=1,p16=x,p17=y,p19=-300,p20=257,p21=3,p22=2,p26=300,p27=1022,p28=511)
for tile,x,y in itertools.product([0,500,65535,0x0fff0000,0x7fffffff,0xffffffff],[0,15,29,30,59,60,179],[0,14,15,29,59,74]):add(p0=1,p18=tile,p15=2,p16=x,p17=y,p21=5,p22=3)
cws=[pc+rc for pc in [0x7f,0x27f,0x37f] for rc in [0,0x400,0x800,0xc00]]
for cw,x,width in itertools.product(cws,[-2147483648,-1025,-800,-1,0,1,3,5,7,13,127,255,639,640,799,800,1023,1024,0x7fffffff],[0,1,-1,3,7,128,640,800,1024,32768,0x7fffffff,-2147483648]):add(p0=2,p25=cw,p16=x,p17=width,p4=(x&1)*0x10000)
r=random.Random(0x53acf0)
for _ in range(500):add(p0=2,p16=r.randrange(0x100000000),p17=r.randrange(0x100000000),p25=r.choice(cws))
for seed in seeds:add(p0=3,p1=seed)
process=subprocess.Popen([str(exe),'--oracle-batch'],cwd=ROOT,stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE)
report={'format':1,'passed':False,'checked':0,'native_sha256':digest,'compiled_C_sha256':sha(exe.read_bytes()),'original_game_executed':False,'unicorn_version':__version__,
 'method':'All original instructions of ambient controller0053acf0, geometry00430960, conversion0056133c and RNG00562348 execute in Unicorn; C composes both restored controllers, real RNG and explicit local x87 expression/conversion. Compare ordered RNG/coordinate/sound calls, request/state bytes and guards, source immutability, return values, x87 sentinel/control word/status, stack and saved registers.',
 'limitations':['Physical sound manager0055f0e8 is a controlled endpoint; exact receiver/request descriptor/order and callback mutations are compared, without audio output or codecs.','Coordinate geometry requires nonzero positive divisors and mapped231-column lookup indices. Sound-table categories/counts and selected records are bounded to mapped test storage; malformed pointers and native divide traps are excluded.','Unicorn2.1.4 does not reproduce the x87 precision exception status bit for inexact division/conversion. That status bit is excluded. For zero-width stress cases, invalid-operation status is also excluded because Unicorn omits it for FISTP infinity; numeric output, divide-by-zero status, control word and x87 stack are still compared. Nonzero widths retain the invalid-operation comparison.','Controller ambient return is void; incidental caller-saved EAX forwarding is not treated as a semantic result.','Direct conversion cases execute the original expression and ftol then the original ratio-pop instruction, without executing unrelated controller stores.']}
def exact(n):
 b=process.stdout.read(n);assert len(b)==n,'C exited: '+process.stderr.read().decode(errors='replace');return b
try:
 assert exact(4)==struct.pack('<I',0x414d4249)
 for p in cases:
  process.stdin.write((' '.join(map(str,p))+'\n').encode());process.stdin.flush()
  prefix=exact(26);count=struct.unpack_from('<I',prefix,22)[0];actual=prefix+exact(count*40+sum(n for a,n in regions));expected=native(p)
  assert actual==expected,repr({'case':p,'first_difference':next((i for i,(a,b) in enumerate(zip(actual,expected)) if a!=b),None),'C_header':actual[:26].hex(),'native_header':expected[:26].hex(),'C_events':[list(struct.unpack_from('<10I',actual,26+i*40)) for i in range(count)],'native_events':state['events']})
  report['checked']+=1
  if report['checked']%500==0:print(json.dumps({'checked':report['checked'],'total':len(cases)}),flush=True)
 report['passed']=True
except Exception as e:report.update(error=str(e),case=p,native_eip=hex(vm.reg_read(UC_X86_REG_EIP)))
finally:
 process.stdin.close();process.wait(timeout=10)
sources=['src/unit_02486.c','src/unit_00461.c','src/unit_02825.c','src/unit_02775.c','include/ambient_sound.h','include/sound_initialization.h','tests/ambient_sound.c','tests/ambient_sound_project/CMakeLists.txt','tools/verify_ambient_sound_emulation.py','metadata/ambient-sound-native.json','metadata/music-tick-native.json']
report['source_hashes']={f:sha((ROOT/f).read_bytes()) for f in sources}
report['instruction_coverage']={hex(a):{'executed':sum(int(i['address'],16) in executed for i in f),'total':len(f),'unexecuted':[i for i in f if int(i['address'],16) not in executed]} for a,f in functions.items()}
report['instructions_executed']=len(executed);report['instructions_total']=len(allowed)
(ROOT/'metadata/ambient-sound-emulation.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({k:v for k,v in report.items() if k not in ['source_hashes','instruction_coverage']},indent=2));sys.exit(0 if report['passed'] else 1)
