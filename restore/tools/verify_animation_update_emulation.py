"""Compare complete animation ticks and registries, using actual native RNG/setter."""
from pathlib import Path
import hashlib,itertools,json,struct,subprocess,sys

ROOT=Path(__file__).resolve().parent.parent
sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *

sha=lambda b:hashlib.sha256(b).hexdigest()
data=(ROOT/'build/smoke-game/Pharaoh.exe').read_bytes();digest=sha(data)
assert digest=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
u16=lambda b,p:struct.unpack_from('<H',b,p)[0]
u32=lambda b,p:struct.unpack_from('<I',b,p)[0]
pe=u32(data,0x3c);opt=pe+24;table=opt+u16(data,pe+20);base=u32(data,opt+28);size=u32(data,opt+56)
vm=Uc(UC_ARCH_X86,UC_MODE_32);vm.mem_map(base,(size+4095)&~4095)
for i in range(u16(data,pe+6)):
 q=table+i*40;vm.mem_write(base+u32(data,q+12),data[u32(data,q+20):u32(data,q+20)+u32(data,q+16)])
BASE=0x30000000;RECORDS=BASE+0x1000;DIRECT=BASE+0x8000;FRAMES=BASE+0x10000;TRANSITIONS=BASE+0x60000
STOP=0x33000000;STACK=0x34000000
vm.mem_map(BASE,0x100000);vm.mem_map(STOP,0x1000);vm.mem_map(STACK,0x100000)
regions=[(BASE,0x80000),(0x5be9b8,253*40+8),(0x586b08,168),(0x58fe48,296),(0x596088,216),(0x59edf0,440),(0x5a8148,904),(0x5ad218,440),(0x5b0060,248),(0x5be510,1208),(0x5c2750,360),(0xbcd5f8,776016),(0x7999d8,0x101d08),(0xea5fac,20),(0xeaa984,20),(0xeaac28,8),(0x604950,8)]
ranges=[(0x586b08,10,8),(0x58fe48,18,8),(0x596088,13,0),(0x59edf0,27,0),(0x5a8148,56,0),(0x5ad218,27,0),(0x5b0060,15,0),(0x5be510,75,0),(0x5c2750,22,0)]
# Merge only overlapping watched intervals, retaining the C fixture's memset order.
merged=[]
for a,n in sorted(regions):
 if merged and a<=merged[-1][1]:merged[-1][1]=max(a+n,merged[-1][1])
 else:merged.append([a,a+n])
buffers=[(a,bytearray(end-a)) for a,end in merged]
def location(a):
 for start,buffer in buffers:
  if start<=a<start+len(buffer):return buffer,a-start
 raise AssertionError(hex(a))
def put(a,v,n=4):
 b,o=location(a);struct.pack_into({1:'<B',2:'<H',4:'<I'}[n],b,o,v&((1<<(8*n))-1))
def fill(a,n,value):
 b,o=location(a);b[o:o+n]=bytes([value&255])*n
def write(a,v):vm.mem_write(a,struct.pack('<I',v&0xffffffff))
def read(a):return u32(vm.mem_read(a,4),0)
def record(p,a,kind,n):
 put(a,kind);put(a+4,p[3]);put(a+8,p[4]+n%3);put(a+16,p[5],2)
 put(a+18,p[15],2);put(a+20,p[17],1);put(a+21,p[6],1)

hits=set();random_states=[]
branches={0x41fdda:'linked-action-mismatch',0x41fe46:'reverse-step',0x41fe49:'forward-step',0x41fe62:'clamp-high',0x41fe6e:'clamp-low',0x41fe80:'transition-scan',0x41fe94:'state-setter-fallback',0x41fec0:'transition-rng',0x41fef1:'new-reverse-frame',0x41ff06:'new-forward-frame',0x41ff20:'registry-22',0x41ff40:'registry-13',0x4206d0:'registry-27-a',0x420a10:'registry-56',0x420d40:'registry-27-b',0x421040:'registry-15',0x4216d0:'registry-25-groups',0x4e5a80:'temple-registry',0x41ff60:'ordered-controller',0x4e59c0:'temple-selector',0x41fca0:'real-state-setter',0x562348:'real-rng'}
def probe(cpu,a,n,user):
 hits.add(branches[a])
 if a==0x562348:random_states.append(read(0x604950))
for a in branches:vm.hook_add(UC_HOOK_CODE,probe,begin=a,end=a)

entries=[0x41fdb0,0x41fca0,0x41ff20,0x41ff40,0x4206d0,0x420a10,0x420d40,0x421040,0x4216d0,0x4e5a80,0x41ff60,0x4e59c0,0x41fdb0]
def execute(op,p):
 sp=STACK+0xf0000;write(sp,STOP)
 if op==1:write(sp+4,p[5]&65535)
 saved=[(UC_X86_REG_EBX,0x11111111),(UC_X86_REG_EBP,0x22222222),(UC_X86_REG_ESI,0x33333333),(UC_X86_REG_EDI,0x44444444)]
 for r,v in saved:vm.reg_write(r,v)
 vm.reg_write(UC_X86_REG_ESP,sp);vm.reg_write(UC_X86_REG_ECX,DIRECT)
 vm.reg_write(UC_X86_REG_EAX,0xdeadbeef);vm.reg_write(UC_X86_REG_EFLAGS,2)
 vm.emu_start(entries[op],STOP,count=2000000)
 assert vm.reg_read(UC_X86_REG_EIP)==STOP,'Instruction budget'
 assert vm.reg_read(UC_X86_REG_ESP)==sp+4+(4 if op==1 else 0),'Stack cleanup'
 assert all(vm.reg_read(r)==v for r,v in saved),'Saved registers'
 return vm.reg_read(UC_X86_REG_EAX) if op==11 else 0

def native(p):
 random_states.clear()
 for k,(a,n) in enumerate(regions):fill(a,n,p[1]+k)
 n=0
 for a,count,field in ranges:
  for j in range(count):
   receiver=RECORDS+n*32;put(a+j*16+field,receiver);record(p,receiver,n%253,n);n+=1
 b,o=location(FRAMES);bt,ot=location(0x5be9b8)
 directions=[0,1,255]
 for k in range(253):
  f=FRAMES+k*1024;e=TRANSITIONS+k*512
  struct.pack_into('<IIII',bt,ot+k*40+16,f,p[8],e,p[9])
  for j in range(16):struct.pack_into('<I',b,o+k*1024+j*48+12,(p[7]+(k+j)%3)&0xffffffff)
  for r in range(8):
   row=e+r*40;put(row,0x10000+r+k*10);put(row+4,directions[r%3],1)
   for c in range(4):
    index=((k+r+c+p[1])&0xffffffff)%16
    if p[13]==1:index=16
    if p[13]==2 or c>=p[12]:index=0xffffffff
    if p[13]==3 and c&1:index=16
    put(row+8+c*8,index);put(row+12+c*8,directions[c%3],1)
  if p[10]<8:put(e+p[10]*40,p[3]);put(e+p[10]*40+4,p[11],1)
  if p[14]<8:put(e+p[14]*40,0xffffffff)
 record(p,DIRECT,p[2],0);put(DIRECT+8,p[4])
 put(0xbcd630+p[15]*388,p[16],1);put(0x604950,p[1]);put(0xeaac28,p[18])
 fill(0x7999d8,0x101d08,0)
 for k,id in enumerate([1,257,3999]):
  a=0x7999d8+id*264
  put(0xea5fac+p[18]*0x49d8+k*4,id if p[22]&(1<<k) else 0)
  put(0xea5fac+(1-p[18])*0x49d8+k*4,id-1)
  put(a,1 if p[20]&(1<<k) else 3,1)
  put(a+16,p[19]^(0x1f if k==1 else 0x100 if k==2 else 0),2)
  put(a+0x55,100 if p[21]&(1<<k) else 128,1)
 for a,buffer in buffers:vm.mem_write(a,bytes(buffer))
 result=0
 for i in range(p[23] if p[0]==12 else 1):result=execute(p[0],p)
 return {'result':result,'hashes':[sha(bytes(vm.mem_read(a,n))) for a,n in regions],'random_states':random_states.copy()}

default=[0,1,0,0,0,0,0,0,16,8,2,0,4,0,7,0,0,0,1,0xcf,7,0,7,16]
cases=[]
def case(**kw):
 p=default.copy()
 for k,v in kw.items():p[int(k[1:])]=v&0xffffffff
 cases.append(tuple(p))

for kind,index,mode,direction,frame in itertools.product([-1,0,252,253],[-1,0,15,16],[0,1,2,3,256,65535],[0,1,255],[-1,0,100,0x7fffffff,0x80000000]):
 case(p2=kind,p3=index,p5=mode,p6=direction,p4=frame)
for rows,match,choices,candidate,direction in itertools.product([0,1,3,8],[0,2,7,-1],range(5),range(4),[0,1,255]):
 case(p9=rows,p10=match,p12=choices,p13=candidate,p6=direction,p11=direction,p4=100,p1=(rows+choices+candidate)*65537)
for maximum,mode,direction,frame in itertools.product([-1,0,5,0x80000000,0x7fffffff],[0,1],[0,1,255],[-1,0,0x80000000,0x7fffffff]):
 case(p7=maximum,p5=mode,p6=direction,p4=frame,p8=1,p1=0xffffffff)
for kind,mode,rows,initial,choices,candidate in itertools.product([-1,0,252,253],[0,3,256,65535],[0,1,8],[0,7,-1],[0,1,4],[0,1,2,3]):
 # Select independent crossing patterns without multiplying all invalid early returns.
 if (kind+mode+rows+initial+choices+candidate)%5:continue
 case(p0=1,p2=kind,p5=mode,p9=rows,p14=initial,p12=choices,p13=candidate,p1=0xffffffff)
for op,actor,actual,expected,mode in itertools.product([0,1,10,12],[1,257,1999],[0,255],[0,255],[0,2,3]):
 case(p0=op,p15=actor,p16=actual,p17=expected,p5=mode,p4=100)
for op,seed,frame,mode,kind in itertools.product(range(2,11),[0,1,0xffffffff],[-1,0,100],[0,1,2,3],[0xcf,0xd0,0x1cf]):
 case(p0=op,p1=seed,p4=frame,p5=mode,p19=kind,p6=seed&255,p11=seed&255)
for life,complete,membership in itertools.product(range(8),repeat=3):
 case(p0=11,p20=life,p21=complete,p22=membership,p18=(life+complete+membership)%2,p19=[0xcf,0xd0,0x1cf,0,0x1d0,0xffff][(life*3+complete+membership)%6])
for player,kind,slot in itertools.product([0,1],[0xcf,0xd0,0x1cf,0x1d0],range(3)):
 for op in [9,10,11]:case(p0=op,p18=player,p19=kind,p20=1<<slot,p22=1<<slot,p4=100)
for seed,choices,mode,direction,repeats in itertools.product([0,1,255,0xffffffff],[0,1,4],[0,1],[0,1,255],[1,16]):
 case(p0=12,p1=seed,p12=choices,p5=mode,p6=direction,p11=direction,p23=repeats,p4=0)
cases=list(dict.fromkeys(cases))
exe=ROOT/'build/c-only-debug/Debug/restore_animation_update_verify.exe'
process=subprocess.Popen([str(exe),'--oracle-batch'],cwd=ROOT,stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True)
report={'format':1,'original_game_executed':False,'native_sha256':digest,'compiled_C_sha256':sha(exe.read_bytes()),'method':'Complete 0041fdb0/0041fca0 with actual 00562348 RNG; all eight animation registries, complete ordered 0041ff60 controller and actual 004e59c0 selector. All watched memory/guards, ordered RNG input states, full selector EAX, stack and saved registers compared. No emulated algorithm endpoint is replaced.','limitations':['Valid catalog allocations, sequence counts 0..16, transition counts 0..8, actor IDs 0..1999 and player IDs 0/1. Invalid kind/sequence values and invalid transition choices are exercised; dangling pointers are excluded.','Only the four consumers of the temple selector have their return bindings corrected. Their remaining algorithms, record initialization/loading and a completed city frame are not proved by this suite.'],'checked':0}
checked=0;p=None
try:
 assert json.loads(process.stdout.readline())=={'fields':24}
 for p in cases:
  process.stdin.write(' '.join(map(str,p))+'\n');process.stdin.flush()
  line=process.stdout.readline();assert line,'C exited: '+process.stderr.read()
  actual=json.loads(line);expected=native(p)
  assert actual==expected,repr({'regions':[hex(regions[i][0]) for i,(a,b) in enumerate(zip(actual['hashes'],expected['hashes'])) if a!=b],'result':[actual['result'],expected['result']],'RNG_counts':[len(actual['random_states']),len(expected['random_states'])]})
  checked+=1
  if checked%100==0:print(json.dumps({'checked':checked,'total':len(cases)}),flush=True)
 assert hits==set(branches.values()),'Missing probes '+repr(set(branches.values())-hits)
 report.update(passed=True,checked=checked)
except Exception as error:report.update(passed=False,checked=checked,case=p,error=str(error),native_eip=hex(vm.reg_read(UC_X86_REG_EIP)))
finally:process.stdin.close();process.wait(timeout=10)
sources=['include/animation_state.h','include/audio_state.h','runtime/animation_state.c','tests/animation_update.c','tools/verify_animation_update_emulation.py','metadata/animation-update-native.json','metadata/animation-building-selector-native.json']
sources.extend('src/unit_'+n+'.c' for n in ['00316','00318','00319','00320','00321','00339','00346','00353','00360','00367','01977','01979','02825','01022','01808','01983'])
report['source_hashes']={s:sha((ROOT/s).read_bytes()) for s in sources};report['observed_native_branches']=sorted(hits)
(ROOT/'metadata/animation-update-emulation.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({k:v for k,v in report.items() if k!='source_hashes'},indent=2));sys.exit(0 if report['passed'] else 1)
