"""Native actor controller with controlled timer, callback and deletion endpoints."""
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
regions=[(0xbcd5f8,776016),(0xea5c90,0x320),(0xeaa668,0x320),(0xd056f0,16),(0xeaac28,8),(0x5dc28c,448)]
def write(a,v,n=4):vm.mem_write(a,(v&((1<<(8*n))-1)).to_bytes(n,'little'))
def read(a,n=4):return int.from_bytes(vm.mem_read(a,n),'little')
callbacks={read(0x5dc28c+i*4) for i in range(110)}
assert callbacks=={int(x['address'],16) for x in json.loads((ROOT/'metadata/actor-callback-table-native.json').read_text())['entries']}
params=[];events=[];hits=set()
branches={0x489bbb:'countdown',0x489c05:'dead-partner',0x489c17:'bad-backlink',0x489c34:'timer-start',0x489c52:'callback',0x489c76:'delete'}
def probe(cpu,a,n,user):hits.add(branches[a])
for a in branches:vm.hook_add(UC_HOOK_CODE,probe,begin=a,end=a)
def advance(flag,divisor):
 i=read(0xd056f8)
 if params[7]&flag and i%divisor==1 and i<1999:write(0xd056f8,i+1)
def endpoint(cpu,a,n,user):
 sp=cpu.reg_read(UC_X86_REG_ESP);value=read(sp+4) if a in [0x489a50,0x4b4980] else 0;i=read(0xd056f8);player=read(0xeaac28)*0x49d8
 partner=read(0xbcd67c+i*388,2);partner=partner-65536 if partner>=32768 else partner
 events.append([a,value,i,read(0xd056f4),read(0xbcd606+i*388,1),read(0xbcd602+i*388,1),partner&0xffffffff,read(0xea5ee8+player),read(0xea5db4+player),read(0xea5f98+player)])
 if a==0x489a50:
  advance(1,17)
  if params[7]&1:write(0xbcd602+read(0xd056f8)*388,params[8]%110,1)
 elif a==0x489a80:advance(4,11)
 elif a==0x4b4980:write(0xbcd606+value*388,0,1);advance(8,7)
 else:
  advance(2,13);write(0xea5ee8+player,read(0xea5ee8+player)+params[0]+1)
  if params[7]&16:write(0xbcd606+read(0xd056f8)*388,2,1)
 cpu.reg_write(UC_X86_REG_EAX,0);cpu.reg_write(UC_X86_REG_EIP,read(sp));cpu.reg_write(UC_X86_REG_ESP,sp+4)
for a in callbacks|{0x489a50,0x489a80,0x4b4980}:vm.hook_add(UC_HOOK_CODE,endpoint,begin=a,end=a)
def execute():
 sp=STACK+0xf0000;write(sp,STOP);saved=[(UC_X86_REG_EBX,0x11111111),(UC_X86_REG_EBP,0x22222222),(UC_X86_REG_ESI,0x33333333),(UC_X86_REG_EDI,0x44444444)]
 for r,v in saved:vm.reg_write(r,v)
 vm.reg_write(UC_X86_REG_ESP,sp);vm.reg_write(UC_X86_REG_EFLAGS,2);vm.emu_start(0x489b70,STOP,count=2000000)
 assert vm.reg_read(UC_X86_REG_EIP)==STOP and vm.reg_read(UC_X86_REG_ESP)==sp+4,'Instruction budget or stack'
 assert all(vm.reg_read(r)==v for r,v in saved),'Saved registers'
def native(p):
 global params
 params=p;seed,player,count,kind,life,pairs,countdown,flags,changed,ticks=p;events.clear()
 for k,(a,n) in enumerate(regions[:5]):vm.mem_write(a,bytes([(seed+k)&255])*n)
 write(0xeaac28,player);write(0xd056f4,0x12345678);write(0xd056f8,0x23456789)
 for j in range(2):write(0xea5db4+j*0x49d8,countdown)
 states=[0,1,2,128]
 for i in range(2000):
  actor=0xbcd600+i*388;state=0 if not 0<i<=count else (1 if life==0 else states[((i+seed)&0xffffffff)%4] if life==1 else 2 if life==2 else 255)
  write(actor+2,((kind+i*7+seed)&0xffffffff)%110,1);write(actor+6,state,1);write(actor+0x7a,0,2);write(actor+0x7c,0,2)
 if pairs:
  for i in range(1,count+1):
   partner=1999 if pairs==2 else i%count+1;write(0xbcd67c+i*388,partner,2);write(0xbcd67a+partner*388,-1 if pairs==3 else i,2)
 for i in range(ticks):execute()
 return {'hashes':[sha(bytes(vm.mem_read(a,n))) for a,n in regions],'events':events.copy()}
default=[1,1,5,0,0,0,300,0,69,1];cases=[]
def case(**kw):
 p=default.copy()
 for k,v in kw.items():p[int(k[1:])]=v&0xffffffff
 cases.append(p)
for count,player,life,pairs,flags in itertools.product([0,1,5,257,1999],[0,1],[0,1,2,3],[0,1,2,3],[0,15]):
 case(p1=player,p2=count,p4=life,p5=pairs,p7=flags)
for flags,seed,countdown,ticks in itertools.product([1,2,4,8,16,31],[0,1,255,0xffffffff],[-1,0,1,256],[1,3]):
 case(p0=seed,p2=25,p6=countdown,p7=flags,p9=ticks,p5=3)
for kind in range(110):case(p2=1,p3=kind,p7=0,p0=0)
cases=list(dict.fromkeys(tuple(p) for p in cases))
exe=ROOT/'build/c-only-debug/Debug/restore_actor_update_verify.exe'
process=subprocess.Popen([str(exe),'--oracle-batch'],cwd=ROOT,stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True)
report={'format':1,'original_game_executed':False,'native_sha256':digest,'compiled_C_sha256':sha(exe.read_bytes()),'method':'Complete 00489b70 original controller versus C. Full actor/profile memory and guards, all 110 native table entries, endpoint order/arguments/intermediate state, callback mutation/reloads and stack/saved registers compared.','limitations':['Profiling/timer functions, actor callbacks and deletion endpoint are controlled. This does not prove any of those deep algorithms or imply that all 102 distinct callback bodies are implemented.','Valid players 0/1, actor IDs 0..1999 and type values 0..109. Invalid native links/table indices are excluded.'],'checked':0}
checked=0;p=None
try:
 assert json.loads(process.stdout.readline())=={'fields':10}
 for p in cases:
  process.stdin.write(' '.join(map(str,p))+'\n');process.stdin.flush();line=process.stdout.readline();assert line,'C exited: '+process.stderr.read()
  actual=json.loads(line);expected=native(p)
  assert actual==expected,repr({'regions':[hex(regions[i][0]) for i,(a,b) in enumerate(zip(actual['hashes'],expected['hashes'])) if a!=b],'events':[len(actual['events']),len(expected['events'])],'first_event_difference':[(i,a,b) for i,(a,b) in enumerate(zip(actual['events'],expected['events'])) if a!=b][:1]})
  checked+=1
  if checked%100==0:print(json.dumps({'checked':checked,'total':len(cases)}),flush=True)
 assert hits==set(branches.values()),'Missing branches '+repr(set(branches.values())-hits)
 report.update(passed=True,checked=checked)
except Exception as error:report.update(passed=False,checked=checked,case=p,error=str(error),native_eip=hex(vm.reg_read(UC_X86_REG_EIP)))
finally:process.stdin.close();process.wait(timeout=10)
sources=['include/actor_update.h','src/unit_01307.c','tests/actor_update.c','tools/verify_actor_update_emulation.py','metadata/actor-update-native.json','metadata/actor-callback-table-native.json']
report['source_hashes']={s:sha((ROOT/s).read_bytes()) for s in sources};report['observed_native_branches']=sorted(hits)
(ROOT/'metadata/actor-update-emulation.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v for k,v in report.items() if k!='source_hashes'},indent=2));sys.exit(0 if report['passed'] else 1)
