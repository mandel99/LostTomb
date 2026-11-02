"""Complete original 0049bd40 prey dispatcher versus handwritten C."""
from pathlib import Path
import hashlib,itertools,json,struct,subprocess,sys
ROOT=Path(__file__).resolve().parent.parent;sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
sha=lambda b:hashlib.sha256(b).hexdigest()
data=(ROOT/'build/smoke-game/Pharaoh.exe').read_bytes();digest=sha(data)
assert digest=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
u16=lambda b,p:struct.unpack_from('<H',b,p)[0];u32=lambda b,p:struct.unpack_from('<I',b,p)[0]
pe=u32(data,0x3c);opt=pe+24;table=opt+u16(data,pe+20);base=u32(data,opt+28);size=u32(data,opt+56)
vm=Uc(UC_ARCH_X86,UC_MODE_32);vm.mem_map(base,(size+4095)&~4095)
for i in range(u16(data,pe+6)):
 q=table+i*40;vm.mem_write(base+u32(data,q+12),data[u32(data,q+20):u32(data,q+20)+u32(data,q+16)])
STOP=0x33000000;STACK=0x34000000;vm.mem_map(STOP,0x1000);vm.mem_map(STACK,0x100000)
regions=[(0xbcd5f8,776016),(0x7999d8,0x101d08),(0xbcb9c8,8*144+8),(0xea5ce8,8),(0xeaa6c0,8),(0xeaac28,8),(0xd056f4,12),(0xd3b1c4,404),(0xd3b364,36),(0xbca2e0,8),(0xe94e00,12),(0xf561c0,64),(0x7090e8,110*56+8),(0x5cdb78,136)]
def write(a,v,n=4):vm.mem_write(a,(v&((1<<(8*n))-1)).to_bytes(n,'little'))
def read(a,n=4):return int.from_bytes(vm.mem_read(a,n),'little')
def signed(v,bits=32):return v-(1<<bits) if v&(1<<(bits-1)) else v
def actor(i):return 0xbcd600+i*388
params=[];events=[];other=0;hits=set()
branches={0x49be62:'death',0x49be44:'disappear',0x49beb5:'choose-idle',0x49c1f9:'walk',0x49c502:'carcass',0x49c427:'flee',0x49c445:'run',0x49c015:'idle-18',0x49c1c0:'idle-19',0x49be80:'spawn-wait',0x49c540:'default-state',0x49c146:'wander',0x49c188:'wander-goal',0x49c1b1:'wander-failed',0x49c248:'walk-reset',0x49c26b:'walk-arrived',0x49c296:'walk-random',0x49c4cc:'run-destroy',0x49c4eb:'run-stopped',0x49c5eb:'sprite-dead',0x49c697:'sprite-disappear',0x49c643:'sprite-idle',0x49c602:'sprite-carcass',0x49c681:'sprite-flee',0x49c662:'sprite-idle-19',0x49c6b6:'sprite-default'}
def probe(cpu,a,n,user):hits.add(branches[a])
for a in branches:vm.hook_add(UC_HOOK_CODE,probe,begin=a,end=a)
counts={0x4a5da0:2,0x4b17d0:1,0x4b1d50:1,0x4b3250:1,0x4b5050:1,0x4eb000:1,0x4eab10:6}
def endpoint(cpu,label,n,user):
 sp=cpu.reg_read(UC_X86_REG_ESP);args=[read(sp+4+i*4) for i in range(counts[label])];i=read(0xd056f8);a=actor(i)
 events.append([label,len(args)]+args+[0]*(6-len(args))+[i,read(a+6,1),read(a+0x30,1),read(a+9,1),signed(read(a+0x2e,2),16)&0xffffffff,read(a+0x140),read(0xd3b1c4),read(0xea5cea+read(0xeaac28)*0x49d8,2)])
 if params[15]&1:write(0xd056f8,other if i==params[2] else params[2])
 a=actor(read(0xd056f8))
 if params[15]&2:write(a+0x30,params[16],1)
 if params[15]&4:write(a+0x140,params[1]+7);write(a+0x13c,params[1]%17+1);write(a+0x2e,params[1]*129,2)
 if params[15]&16:write(0xbca2e0,params[1]^0x87654321)
 if label==0x4a5da0 and params[14]!=0xffffffff:write(a+9,params[14],1)
 if label==0x4eab10 and params[15]&8:
  f=0xbcb9c8+params[10]*144;write(f+58,read(f+58,2)+1,2);write(f+60,read(f+60,2)-2,2)
 cpu.reg_write(UC_X86_REG_EAX,params[13] if label==0x4eab10 else 0);cpu.reg_write(UC_X86_REG_EIP,read(sp));cpu.reg_write(UC_X86_REG_ESP,sp+4)
for a in counts:vm.hook_add(UC_HOOK_CODE,endpoint,begin=a,end=a)
def execute():
 sp=STACK+0xf0000;write(sp,STOP);saved=[(UC_X86_REG_EBX,0x11111111),(UC_X86_REG_EBP,0x22222222),(UC_X86_REG_ESI,0x33333333),(UC_X86_REG_EDI,0x44444444)]
 for r,v in saved:vm.reg_write(r,v)
 vm.reg_write(UC_X86_REG_ESP,sp);vm.reg_write(UC_X86_REG_EFLAGS,2);vm.emu_start(0x49bd40,STOP,count=2000000)
 assert vm.reg_read(UC_X86_REG_EIP)==STOP and vm.reg_read(UC_X86_REG_ESP)==sp+4,'Instruction budget/stack'
 assert all(vm.reg_read(r)==v for r,v in saved),'Saved registers'
def native(p):
 global params,other
 params=p;other=1 if p[2]==1999 else p[2]+1;events.clear()
 for k,(a,n) in enumerate(regions[:13]):vm.mem_write(a,bytes([(p[1]+k)&255])*n)
 for a,v in [(0xd056f8,p[2]),(0xeaac28,p[17]),(0xbca2e0,p[9]),(0xe94e00,0xeeeeeeee)]:write(a,v)
 for i,id in enumerate([p[2],other]):
  a=actor(id)
  for offset,v,n in [(2,p[19],1),(6,1,1),(0x30,p[3],1),(0x2e,p[4],2),(0x140,p[5],4),(0x13c,p[6],4),(9,p[7],1),(10,p[8],1),(0x58,p[10],2),(0x130,p[11],4),(-7,p[20],1),(-4,p[21]^a,4),(0x5a,p[1]+i,1),(12,-120 if i else 10+p[1]%200,2),(14,255 if i else 20-p[1]%200,2)]:write(a+offset,v,n)
 vm.mem_write(0x7999d8,bytes(0x101d08));b=0x7999d8+p[11]*264
 for offset,v,n in [(16,0x73 if p[12]&1 else 0,2),(0xa6,128 if p[12]&2 else 0,1),(0,1 if p[12]&4 else 3,1),(0x40,256 if p[12]&8 else 0,2)]:write(b+offset,v,n)
 for i in range(8):write(0xbcb9c8+i*144+58,p[1]+i*37,2);write(0xbcb9c8+i*144+60,0x7fff-p[1]-i*19,2)
 write(0xd3b1c4,p[18]);vm.mem_write(0xd3b1c8,b'\x5a'*400)
 for a,v in [(0xd3b380,p[1]+0x12345678),(0xd3b384,p[1]+0x76543210),(0xd3b374,p[1]&127)]:write(a,v)
 for i in range(110):write(0x7090ec+i*56,p[23]+i*257)
 for i in range(2):write(0xea5cea+i*0x49d8,p[1],2)
 for i,a in enumerate([0xf561cc,0xf561d4,0xf561dc,0xf561e4,0xf561ec]):write(a,p[21]+i*10000)
 for i in range(6 if p[0]==2 else 1):execute()
 return {'hashes':[sha(bytes(vm.mem_read(a,n))) for a,n in regions],'events':events.copy()}
default=[0,1,1,24,1,0,8,0,3,0,1,1,15,0,0xffffffff,0,14,0,99,69,255,0x12340000,0,2];cases=[]
def case(**kw):
 p=default.copy()
 for k,v in kw.items():p[int(k[1:])]=v&0xffffffff
 cases.append(p)
for state,timer,due,flags in itertools.product([0,4,5,8,9,10,12,14,15,16,17,18,19,24,25,255],[-32768,-1,0,1,7,501,32767],[0,8],[0,1,2,7]):
 case(p3=state,p4=timer,p5=due,p15=flags)
for state,direction,flags,id in itertools.product([9,10,16,17],[0,8,9,10,255],[0,1,2,3,4,7,16,31],[1,1999]):case(p3=state,p14=direction,p15=flags,p2=id,p5=7)
for result,flags in itertools.product([-1,*range(16),16],[0,1,2,8,31]):case(p3=18,p4=0,p5=8,p13=result,p15=flags)
for seed,index in itertools.product(range(100),[0,98,99]):case(p3=8,p1=seed,p18=index,p0=1)
for state,direction,rotation in itertools.product(range(26),[0,7,8,10,127,128,255],[0,7,256,0x80000000,0xffffffff]):
 case(p3=4,p15=2,p16=state,p7=direction,p9=rotation)
for flags,building,player in itertools.product(range(16),[0,1,3999],[0,1]):case(p12=flags,p11=building,p17=player)
for state,tick,interval,speed,seed in itertools.product([4,8,12,16,18,19,24],[0x7fffffff,0x80000000,0xffffffff],[0,1,12,0x7fffffff],[1,0x80000000],[0,0xffffffff]):
 case(p3=state,p5=tick,p6=interval,p23=speed,p1=seed,p4=500,p21=0xfffffff0)
for state,flags,timer in itertools.product([8,9,12,16,18,19,24],[0,1,2,4,31],[0,1,7]):case(p0=2,p3=state,p15=flags,p4=timer,p14=8,p13=16)
cases=list(dict.fromkeys(tuple(p) for p in cases))
exe=ROOT/'build/c-only-debug/Debug/restore_actor_prey_verify.exe'
process=subprocess.Popen([str(exe),'--oracle-batch'],cwd=ROOT,stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True)
report={'format':1,'original_game_executed':False,'native_sha256':digest,'compiled_C_sha256':sha(exe.read_bytes()),'method':'Complete seeded 668-instruction 0049bd40 original prey dispatcher versus handwritten C, including native inline RNG/history, state transitions, sprite/heading arithmetic and actual C callback registry for the restored entry. Full watched memory/guards, helper call order/arguments/intermediate state and stack/saved registers compared.','limitations':['All seven movement/death/route/wander/flee helper endpoints are controlled; their deep algorithms and the other 100 missing actor callback bodies are not proved. EAX is unused by the actual void actor-table caller and is not compared.','Valid actor IDs 1..1999, players 0/1, formation IDs 0..7, buildings 0..3999, type-table indices 0..109 and RNG history index 0..99. Native invalid storage is excluded.'],'checked':0}
checked=0;p=None
try:
 assert json.loads(process.stdout.readline())=={'fields':24}
 for p in cases:
  process.stdin.write(' '.join(map(str,p))+'\n');process.stdin.flush();line=process.stdout.readline();assert line,'C exited: '+process.stderr.read()
  actual=json.loads(line);expected=native(p)
  assert actual==expected,repr({'regions':[hex(regions[i][0]) for i,(a,b) in enumerate(zip(actual['hashes'],expected['hashes'])) if a!=b],'events':[len(actual['events']),len(expected['events'])],'first_event_difference':[(i,a,b) for i,(a,b) in enumerate(zip(actual['events'],expected['events'])) if a!=b][:1]})
  checked+=1
  if checked%250==0:print(json.dumps({'checked':checked,'total':len(cases)}),flush=True)
 assert hits==set(branches.values()),'Missing branches '+repr(set(branches.values())-hits)
 report.update(passed=True,checked=checked)
except Exception as error:report.update(passed=False,checked=checked,case=p,error=str(error),native_eip=hex(vm.reg_read(UC_X86_REG_EIP)))
finally:process.stdin.close();process.wait(timeout=10)
sources=['include/actor_callback_496140.h','include/actor_callbacks_chain.h','include/actor_callbacks_small.h','tests/actor_callback_unrelated.c','include/actor_callbacks_human.h','include/actor_update.h','runtime/actor_prey.c','runtime/actor_callbacks.c','tests/actor_prey.c','tools/verify_actor_prey_emulation.py','metadata/actor-prey-rea-function.json','metadata/actor-callback-table-native.json','metadata/actor-callback-seeds.tsv']
report['source_hashes']={s:sha((ROOT/s).read_bytes()) for s in sources};report['observed_native_branches']=sorted(hits)
(ROOT/'metadata/actor-prey-emulation.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v for k,v in report.items() if k!='source_hashes'},indent=2));sys.exit(0 if report['passed'] else 1)
