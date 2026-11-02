"""Native city trade iterator, allowance, price accumulation, count and clamp proof."""
from pathlib import Path
import hashlib,itertools,json,struct,subprocess,sys
ROOT=Path(__file__).resolve().parent.parent;sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
def sha(b):return hashlib.sha256(b).hexdigest()
data=(ROOT/'build/smoke-game/Pharaoh.exe').read_bytes();digest=sha(data)
assert digest=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
u16=lambda b,p:struct.unpack_from('<H',b,p)[0];u32=lambda b,p:struct.unpack_from('<I',b,p)[0]
pe=u32(data,0x3c);opt=pe+24;table=opt+u16(data,pe+20);base=u32(data,opt+28);size=u32(data,opt+56)
vm=Uc(UC_ARCH_X86,UC_MODE_32);vm.mem_map(base,(size+4095)&~4095)
for i in range(u16(data,pe+6)):
 q=table+i*40;vm.mem_write(base+u32(data,q+12),data[u32(data,q+20):u32(data,q+20)+u32(data,q+16)])
BASE=0x30000000;ALT=BASE+0x1000;STOP=0x33000000;STACK=0x34000000;TOWNS=0x785d70;LENGTH=18628
vm.mem_map(BASE,0x10000);vm.mem_map(STOP,0x1000);vm.mem_map(STACK,0x100000)
regions=[(TOWNS,LENGTH),(ALT,LENGTH),(0x775814,236),(0x784dec,284),(0xea1894,4),(0xea626c,4),(0xeaac28,4),(0x7999d8,0x101d00),(0x78d790,4),(BASE,4)]
def write(a,v,n=4):vm.mem_write(a,(v&((1<<(8*n))-1)).to_bytes(n,'little'))
def read(a,n=4):return int.from_bytes(vm.mem_read(a,n),'little')
events=[];params=[];hits=set()
branches={0x443384:'town-contribution',0x443402:'population-under-1000',0x443415:'population-under-2500',0x443428:'population-under-5000',0x443431:'population-5000-plus',0x443480:'fixed-base',0x443487:'complex-helper',0x443497:'building-base'}
def probe(cpu,a,n,user):hits.add(branches[a])
for a in branches:vm.hook_add(UC_HOOK_CODE,probe,begin=a,end=a)
def endpoint(cpu,a,n,user):
 sp=cpu.reg_read(UC_X86_REG_ESP);resource=read(sp+4);player=read(0xeaac28);events.append([a,resource,player,read(0xea1894+player*0x49d8)])
 cpu.reg_write(UC_X86_REG_EAX,(params[2]+resource*37)&0xffffffff);cpu.reg_write(UC_X86_REG_EIP,read(sp));cpu.reg_write(UC_X86_REG_ESP,sp+4)
vm.hook_add(UC_HOOK_CODE,endpoint,begin=0x4f57b0,end=0x4f57b0)
def execute(entry,args=(),receiver=0):
 sp=STACK+0xf0000;write(sp,STOP)
 for i,v in enumerate(args):write(sp+4+i*4,v)
 saved=[(UC_X86_REG_EBX,0x11111111),(UC_X86_REG_EBP,0x22222222),(UC_X86_REG_ESI,0x33333333),(UC_X86_REG_EDI,0x44444444)]
 for r,v in saved:vm.reg_write(r,v)
 vm.reg_write(UC_X86_REG_EAX,0xa1b2c3d4);vm.reg_write(UC_X86_REG_ECX,receiver);vm.reg_write(UC_X86_REG_ESP,sp);vm.reg_write(UC_X86_REG_EFLAGS,2)
 vm.emu_start(entry,STOP,count=30000000)
 assert vm.reg_read(UC_X86_REG_EIP)==STOP,'Native instruction budget'
 assert vm.reg_read(UC_X86_REG_ESP)==sp+4+(4 if entry==0x44baf0 else 0),'Native stack differs'
 assert all(vm.reg_read(r)==v for r,v in saved),'Native saved register differs'
 return vm.reg_read(UC_X86_REG_EAX)
def setup(address,count,seed,quantity):
 seed&=0xffffffff;b=bytearray(b'\xa5'*LENGTH);struct.pack_into('<I',b,0,count)
 for i in range(150):
  a=4+i*124;s=(i+seed)&0xffffffff;struct.pack_into('<H',b,a,i+256);b[a+2]=[1,2,23,0][s%4];b[a+0x32]=[0,1,128][s%3]
  for offset,v in [(0x30,((i//3+seed)&0xffffffff)%12-2),(4,((i*7+seed)&0xffffffff)%40-2),(12,(seed%31-15)+(i%7)+quantity)]:struct.pack_into('<H',b,a+offset,v&65535)
 vm.mem_write(address,bytes(b))
def native(p):
 global params
 params=p;op,count,seed,player,population,previous,kind,mode,quantity,buildings=p;events.clear()
 setup(TOWNS,count,seed,quantity);setup(ALT,count,seed+1,quantity);write(BASE,previous,2)
 vm.mem_write(0x775814,b'\xcc'*236);vm.mem_write(0x784dec,b'\x5a'*284)
 for i in range(4):
  for j in range(6):write(0x7758a0+i*24+j*4,1000+i*900+j*170+(seed%10)*3)
 for i in range(35):write(0x784dec+i*4,kind+i*(seed&3));write(0x784e7c+i*4,mode+i*((seed>>2)&3))
 write(0xeaac28,player);write(0xea1894,population^0x13579bdf if player else population);write(0xea626c,population if player else population^0x13579bdf)
 b=bytearray(0x101d00);types=[0x6e,0x72,0x20,0x2f,0x33,0xb1,0xcd,0x80]
 for i in range(3999):
  a=(i+1)*264;s=(i+seed)&0xffffffff;b[a]=1 if s%3==0 else 3
  for offset,v in [(16,types[s%8]),(0x40,s%2),(18,s%29-1),(0xce,((i*3+seed)&0xffffffff)%29-1)]:struct.pack_into('<H',b,a+offset,v&65535)
 vm.mem_write(0x7999d8,bytes(b));write(0x78d790,buildings);selected=TOWNS+4+(seed%150)*124
 if op==3:write(selected+2,kind,1);write(selected+0x32,quantity,1);write(selected+0x30,mode,2)
 result=0
 if op==0:execute(0x443330)
 elif op in [1,2]:result=execute(0x44baf0,(0 if previous==0xffffffff else BASE,),ALT if op==2 else TOWNS)
 elif op==3:
  result=execute(0x44d4c0,(),selected)
  if kind&255!=1 or quantity&255==0:result&=255
 elif op==4:result=execute(0x443080,(kind,))
 else:result=execute(0x559c60,(kind,quantity))
 return {'result':result,'hashes':[sha(bytes(vm.mem_read(a,n))) for a,n in regions],'events':events.copy()}
default=[0,6,0,0,1000,0xffffffff,0,0,0,12];cases=[]
def case(**kw):
 p=default.copy()
 for k,v in kw.items():p[int(k[1:])]=v&0xffffffff
 cases.append(p)
for count,seed,player,population in itertools.product([0,1,6,32,140,150],[0,1,17,0xffffffff],[0,1],[0xffffffff,0,999,1000,2499,2500,4999,5000]):
 case(p1=count,p2=seed,p3=player,p4=population,p9=[0,1,12,256,1000,3999][(count+seed+player)%6])
for high,low,quantity in itertools.product([0xffffffff,0x80000000,0x7fffffff],[0,1,0xffffffff],[0,1,0xfffffff6]):case(p1=150,p6=high,p7=low,p8=quantity,p9=256)
for op,count,seed,previous in itertools.product([1,2],[0,1,6,32,140,150],[0,1,3,255],[0xffffffff,0,5,139,149,0xffff]):case(p0=op,p1=count,p2=seed,p5=previous)
for kind,mode,flag in itertools.product([0,1,2,23,255],[0x8000,0xffff,0,1,2,8,9,256,32767],[0,1,128,255]):case(p0=3,p6=kind,p7=mode,p8=flag)
for kind,count,seed in itertools.product(list(range(41))+[0xffffffff],[0,1,12,256,3999],[0,1,255]):case(p0=4,p2=seed,p6=kind,p9=count)
for kind,value in itertools.product([0,1,9,10,12,24,25,26,28,30,35,36,0xffffffff],[0x80000000,0xffffffff,0,1,9999,10000,10001,14999,15000,15001,29999,30000,30001,0x7fffffff]):case(p0=5,p6=kind,p8=value)
exe=ROOT/'build/c-only-debug/Debug/restore_city_prices_verify.exe'
process=subprocess.Popen([str(exe),'--oracle-batch'],cwd=ROOT,stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True)
report={'format':1,'original_game_executed':False,'native_sha256':digest,'compiled_C_sha256':sha(exe.read_bytes()),
 'method':'Complete 00443330 accumulator and price preparation with actual 0044baf0 iterator, 0044d4c0 predicate, 00443080 building count and 00559c60 clamp. Global/alternate receiver, complete watched memory, helper call order/arguments, result, native stack/saved registers and selected native branches compared.',
 'limitations':['Only complex resource-demand helper 004f57b0 is controlled; this does not prove that helper or complete city/gameplay behavior.','Valid list counts 0..150, valid previous indices and building counts up to 3999. Native high EAX bits in disabled record-predicate paths are unspecified; AL is compared there and full EAX otherwise.'],
 'checked':0};checked=0;p=None
try:
 assert json.loads(process.stdout.readline())=={'fields':10}
 for p in cases:
  process.stdin.write(' '.join(map(str,p))+'\n');process.stdin.flush();line=process.stdout.readline();assert line,'C exited: '+process.stderr.read()
  actual=json.loads(line);expected=native(p)
  assert actual==expected,repr({'C_result':actual['result'],'native_result':expected['result'],'different_regions':[hex(regions[i][0]) for i,(a,b) in enumerate(zip(actual['hashes'],expected['hashes'])) if a!=b],'event_counts':[len(actual['events']),len(expected['events'])],'first_event_difference':[(i,a,b) for i,(a,b) in enumerate(zip(actual['events'],expected['events'])) if a!=b][:1]})
  checked+=1
  if checked%100==0:print(json.dumps({'checked':checked,'total':len(cases)}),flush=True)
 assert hits==set(branches.values()),'Native branches not reached: '+repr(set(branches.values())-hits)
 report.update(passed=True,checked=checked)
except Exception as error:report.update(passed=False,checked=checked,case=p,error=str(error),native_eip=hex(vm.reg_read(UC_X86_REG_EIP)))
finally:process.stdin.close();process.wait(timeout=10)
sources=['include/town_menu.h','include/town_trade.h','tests/city_prices.c','tools/verify_city_prices_emulation.py','metadata/city-price-next-native.json','metadata/city-prices-helpers-native.json','metadata/town-trade-callers-native.json']
sources += [f'src/unit_{i:05d}.c' for i in [617,618,718,741,743,2268,2373,2379,2404,2462,2632]]
report['source_hashes']={s:sha((ROOT/s).read_bytes()) for s in sources};report['observed_native_branches']=sorted(hits)
(ROOT/'metadata/city-prices-emulation.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({k:v for k,v in report.items() if k!='source_hashes'},indent=2));sys.exit(0 if report['passed'] else 1)
