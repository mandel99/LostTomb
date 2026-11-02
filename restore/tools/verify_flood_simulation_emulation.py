"""Original complete flood controller, region traversal and flood-quality helpers versus C."""
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
BASE=0x30000000;WORLD=0xd05720;ALT=BASE+0x1000;POOL=BASE+0x40000;POOL_SIZE=30*300*64+32;STOP=0x33000000;STACK=0x34000000
vm.mem_map(BASE,0x200000);vm.mem_map(STOP,0x1000);vm.mem_map(STACK,0x100000)
regions=[(WORLD,0x33008),(ALT,0x33008),(POOL,POOL_SIZE),(0x5df75c,12),(0x7848d8,8),(0x78d7bc,24),(0xeaac28,8),(0xbc9694,8),(0xd05718,8),(0xd3b1c4,404),(0xd3b364,36),(0xbcd600,0xbd770),(0xea5ce8,8),(0xeaa6c0,8),(0xea5f9c,8),(0xeaa974,8)]
def write(a,v,n=4):vm.mem_write(a,(v&((1<<(8*n))-1)).to_bytes(n,'little'))
def read(a,n=4):return int.from_bytes(vm.mem_read(a,n),'little')
params=[];events=[];active=0;hits=set()
branches={0x4beb76:'disabled',0x4be7ba:'approach-quality',0x4be8d0:'previous-year',0x4be91f:'flooding',0x4be947:'inundated',0x4be938:'contracting',0x4be991:'imminent',0x4be9a6:'resting',0x4be9b2:'farmable',0x4bea06:'clear-growth',0x4bea1e:'warning',0x4bea2c:'prepare-season',0x4bea3a:'finish-season',0x4beada:'grow-row',0x4beaa9:'growth-progress',0x4bd8a7:'skip-list',0x4bd8d9:'partial-list',0x4bd907:'process-list'}
branches.update({0x4be5c3:'relocate-actor',0x4bd919:'update-payload',0x4bd5bc:'quality-bonus'})
def probe(cpu,a,n,user):hits.add(branches[a])
for a in branches:vm.hook_add(UC_HOOK_CODE,probe,begin=a,end=a)
def endpoint(cpu,a,n,user):
 sp=cpu.reg_read(UC_X86_REG_ESP);values=[];cleanup=0;result=0
 if a==0x4bd950:
  payload=cpu.reg_read(UC_X86_REG_ECX);values=[payload,read(sp+4)&255];cleanup=4
 elif a==0x4be4a0:values=[read(sp+4)];cleanup=4
 elif a==0x4c5120:values=[read(sp+4)];result=int(bool(params[17]&8))
 elif a==0x489510:assert cpu.reg_read(UC_X86_REG_ECX)==0xbca350,'Warning receiver'
 events.append([a,len(values)]+values+[0]*(4-len(values))+[read(active+0x32fb4),read(active+0x32fac),read(active+0x32c54),read(0x5df760)])
 if a==0x4bd950:
  write(payload+20,read(payload+20)^0x13579bdf);node=read(payload+4);following=read(node)
  if params[17]&128 and following:write(read(payload)+24,read(following+12))
 cpu.reg_write(UC_X86_REG_EAX,result);cpu.reg_write(UC_X86_REG_EIP,read(sp));cpu.reg_write(UC_X86_REG_ESP,sp+4+cleanup)
for a in [0x4bd950,0x4be4a0,0x4c5120,0x489510]:vm.hook_add(UC_HOOK_CODE,endpoint,begin=a,end=a)
def execute(entry,args=(),receiver=0):
 sp=STACK+0xf0000;write(sp,STOP)
 for i,v in enumerate(args):write(sp+4+i*4,v)
 saved=[(UC_X86_REG_EBX,0x11111111),(UC_X86_REG_EBP,0x22222222),(UC_X86_REG_ESI,0x33333333),(UC_X86_REG_EDI,0x44444444)]
 for r,v in saved:vm.reg_write(r,v)
 vm.reg_write(UC_X86_REG_ECX,receiver);vm.reg_write(UC_X86_REG_ESP,sp);vm.reg_write(UC_X86_REG_EFLAGS,2);vm.reg_write(UC_X86_REG_FPCW,0x37f)
 vm.emu_start(entry,STOP,count=2000000)
 assert vm.reg_read(UC_X86_REG_EIP)==STOP,'Instruction budget exceeded'
 assert vm.reg_read(UC_X86_REG_ESP)==sp+4+(12 if entry==0x4bd850 else 0),'Stack differs'
 assert all(vm.reg_read(r)==v for r,v in saved),'Saved registers differ'
 return vm.reg_read(UC_X86_REG_EAX)
def setup_world(world,p):
 op,seed,year,baseyear,month,day,tick,season,duration,width,quality,current,target,phase,nodes,percent,complete,flags,growth,old=p
 vm.mem_write(world,bytes([seed&255])*0x33008)
 for offset,v,n in [(0x32c4c,season,4),(0x32c50,duration,4),(0x32c54,current,4),(0x32c5c,target,4),(0x32c60,quality,4),(0x32fb8,width,4),(0x32fb4,phase,4),(0x32fac,growth,4),(0x32fbc,128 if flags&2 else 0,1),(0x32c40,season,4),(0x32c44,duration,4),(0x32c48,quality,4),(0x32c58,quality,4)]:write(world+offset,v,n)
 write(world+0x32c54,current+(0 if world==active else 17))
 for r in range(30):
  region=world+0x32c64+r*28;head=POOL+r*300*64
  for offset,v in [(0,nodes),(4,growth),(16,head if nodes and not flags&32 else 0),(24,head+32 if nodes and flags&16 else 0)]:write(region+offset,v)
def native(p):
 global active,params
 params=p;op,seed,year,baseyear,month,day,tick,season,duration,width,quality,current,target,phase,nodes,percent,complete,flags,growth,old=p;active=ALT if flags&1 else WORLD;events.clear()
 for k,(a,n) in enumerate(regions):vm.mem_write(a,bytes([(seed+k)&255])*n)
 setup_world(WORLD,p);setup_world(ALT,p);vm.mem_write(POOL,b'\xa5'*POOL_SIZE)
 for r in range(30):
  for i in range(nodes):
   a=POOL+(r*300+i)*64;region=active+0x32c64+r*28
   for offset,v in [(0,a+64 if i+1<nodes else 0),(4,a-64 if i else 0),(8,i),(12,a+32),(32,region),(36,a),(40,i+1000)]:write(a+offset,v)
   write(a+56,0 if flags&64 else (128 if ((i+seed)&0xffffffff)%3 else 0),1)
 for a,v,n in [(0x5df75c,old^0xabcdef00,4),(0x5df760,old,4),(0x5df764,0xdeadbeef,4),(0x7848d8,baseyear,2),(0x78d7bc,year,4),(0x78d7c0,month,4),(0x78d7c4,day,4),(0x78d7cc,tick,4),(0xeaac28,seed&1,4),(0xeaac2c,0xa5a5a500,4),(0xbc9694,0x87654321,4),(0xbc9698,0x12345678,4),(0xd3b1c4,seed%100,4),(0xd3b374,seed&127,4),(0xd3b380,seed+0x12345678,4),(0xd3b384,seed+0x76543210,4)]:write(a,v,n)
 vm.mem_write(0xd3b1c8,b'\x5a'*400)
 for i in range(2):write(0xea5ce8+i*0x49d8,0x100 if flags&4 else 0,2);write(0xea5f9c+i*0x49d8,seed+2048+i)
 actors=bytearray(0xbd770);types=[20,25,77,78,92,93,100,101,0,19,102,255];states=[10,12,0,128]
 for i in range(1,2000):actors[i*388+2]=types[((i+seed)&0xffffffff)%12];actors[i*388+0x30]=states[((i//3+seed)&0xffffffff)%4]
 vm.mem_write(0xbcd600,bytes(actors));execute(0x4bd310);result=0
 if op==0:execute(0x4be770,(),active)
 elif op==1:execute(0x4bd850,(phase,percent,complete),active+0x32c64)
 elif op==2:execute(0x4be0d0,(),active+0x32c64)
 elif op==3:execute(0x4be490,(),active)
 elif op==4:execute(0x4be580,(),active)
 elif op==5:result=execute(0x4bd590,(),active+0x32c40)
 elif op==6:result=execute(0x4bd590,(),WORLD+0x32c40)
 elif op==7:execute(0x4bd380,(),active+0x32c40)
 elif op==8:execute(0x4bd550,(),active+0x32c40)
 elif op==9:execute(0x4bd580,(),active+0x32c40)
 elif op==11:
  for i in range(12):write(0x78d7cc,tick+i);execute(0x4be770,(),active)
 return {'result':result,'hashes':[sha(bytes(vm.mem_read(a,n))) for a,n in regions],'events':events.copy()}
default=[0,1,0,0,0,0,1,0,60,10,50,50,80,6,0,0,0,2,0,0xffffffff];cases=[]
def case(**kw):
 p=default.copy()
 for k,v in kw.items():p[int(k[1:])]=v&0xffffffff
 cases.append(p)
# Every cycle in a complete year and adjacent years, varied span and receiver.
for season,width,quality in [(0,30,50),(100,10,100),(300,5,0),(200,0,50)]:
 for cycle in range(432):
  case(p6=cycle*25,p7=season,p9=width,p10=quality,p17=2|(cycle&1),p14=3)
for year,season,width,quality,subtick in itertools.product([-4000,-1,0,1,25,1000],[0,100,300],[1,30],[1,99],[0,1,24]):
 case(p2=year,p6=4000+subtick,p7=season,p9=width,p10=quality,p14=2,p17=3)
for phase,current,target,tick,flags in itertools.product([2,6],[0,50,100],[0,47,50,53,100],[1,257],[0,2,3]):case(p13=phase,p11=current,p12=target,p6=tick,p17=flags)
for nodes,percent,complete,flags in itertools.product([0,1,2,26,50,257],[-100,0,1,50,100,255],[0,1,256],[2,3,18,34,66,130]):
 case(p0=1,p14=nodes,p15=percent,p16=complete,p17=flags,p13=0x101)
for growth,nodes,flags in itertools.product([0,4,5,6],[0,2,50],[2,3,18,130]):case(p0=2,p18=growth,p14=nodes,p17=flags)
for op,seed,flags,current in itertools.product([3,5,6,7,8,9],[0,1,255,256,0xffffffff],[2,3,6,14],[-1,0,90,100,101]):
 case(p0=op,p1=seed,p17=flags,p11=current)
for seed in range(100):case(p0=7,p1=seed,p17=3,p10=50)
for seed,flags in itertools.product([0,1,255],[2,3]):case(p0=4,p1=seed,p17=flags)
for tick,season,width,quality,flags in itertools.product([0,24,374,1124,3374,5000],[0,100],[1,30],[0,50,100],[2,3]):
 case(p0=11,p6=tick,p7=season,p9=width,p10=quality,p17=flags,p14=3)
case(p0=10,p1=123)
# Real calendar fields and signed 16-bit scenario base years, including a
# current year above 32767 so a byte/word current-year declaration cannot pass.
for baseyear,delta,month,day,tick,flags in itertools.product([-32768,-4000,1234,32767],[-1,0,1,25],[0,5,11],[0,15],[0,1,50],[2,3]):
 case(p2=baseyear+delta,p3=baseyear,p4=month,p5=day,p6=tick,p17=flags,p14=2)
for quality,width in itertools.product(range(101),[1,10,30]):case(p6=1600,p9=width,p10=quality,p14=3)
for cycle,old in itertools.product([105,106,115,135],[0,60,100,0xffffffff]):case(p6=cycle*25,p9=30,p19=old,p14=3)
cases=list(dict.fromkeys(tuple(p) for p in cases))
exe=ROOT/'build/c-only-debug/Debug/restore_flood_simulation_verify.exe'
process=subprocess.Popen([str(exe),'--oracle-batch'],cwd=ROOT,stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True)
report={'format':1,'original_game_executed':False,'native_sha256':digest,'compiled_C_sha256':sha(exe.read_bytes()),
 'method':'Complete original 004be770 controller and actual x87 0056133c conversions, omitted 004bd310 initializer, 004bd850 list traversal, 004be0d0 growth, 004be490/004be580 season wrappers and 004bd380/004bd550/004bd580/004bd590 quality helpers versus compiled C. Full watched memory/guards, endpoint ordering/intermediate state and native stack/saved registers compared.',
 'limitations':['Warning delivery 00489510, payload mutation 004bd950, actor relocation 004be4a0 and person predicate 004c5120 are controlled. This proves caller contracts and the reviewed algorithms, not those deep endpoints or playable gameplay.','Valid calendar/index/list storage, width 0..30, quality 0..100, nodes 0..300 and history index 0..99. Native floating-point control word is 0x37f; malformed pointers and invalid native division are excluded.'],'checked':0}
checked=0;p=None
try:
 assert json.loads(process.stdout.readline())=={'fields':20}
 for p in cases:
  process.stdin.write(' '.join(map(str,p))+'\n');process.stdin.flush();line=process.stdout.readline();assert line,'C exited: '+process.stderr.read()
  actual=json.loads(line);expected=native(p)
  assert actual==expected,repr({'C_result':actual['result'],'native_result':expected['result'],'different_regions':[hex(regions[i][0]) for i,(a,b) in enumerate(zip(actual['hashes'],expected['hashes'])) if a!=b],'event_counts':[len(actual['events']),len(expected['events'])],'first_event_difference':[(i,a,b) for i,(a,b) in enumerate(zip(actual['events'],expected['events'])) if a!=b][:1]})
  checked+=1
  if checked%250==0:print(json.dumps({'checked':checked,'total':len(cases),'op':p[0]}),flush=True)
 assert hits==set(branches.values()),'Missing branches: '+repr(set(branches.values())-hits)
 report.update(passed=True,checked=checked)
except Exception as error:report.update(passed=False,checked=checked,case=p,error=str(error),native_eip=hex(vm.reg_read(UC_X86_REG_EIP)))
finally:process.stdin.close();process.wait(timeout=10)
sources=['include/flood_simulation.h','include/city_prepare.h','include/simulation_tick.h','runtime/flood_simulation.c','runtime/standalone_link_check.c','tests/flood_simulation.c','tools/verify_flood_simulation_emulation.py','metadata/city-simulation-next-native.json','metadata/flood-simulation-helpers-native.json','metadata/flood-initializer-native.json','metadata/flood-quality-callers-native.json']
sources += [f'src/unit_{i:05d}.c' for i in [1544,1545,1546,1547,1554,1555,1556,1559,1561,1563]]
report['source_hashes']={s:sha((ROOT/s).read_bytes()) for s in sources};report['observed_native_branches']=sorted(hits)
(ROOT/'metadata/flood-simulation-emulation.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({k:v for k,v in report.items() if k!='source_hashes'},indent=2));sys.exit(0 if report['passed'] else 1)
