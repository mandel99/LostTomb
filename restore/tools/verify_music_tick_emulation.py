"""Native music update state machine, volume receiver and full-width RNG."""
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
BASE=0x30000000;ALT=BASE+0x1000;ALT_SOUND=BASE+0x80000;MUSIC=0xe39180;SOUND=0xe92bf8;STOP=0x33000000;STACK=0x34000000;APIS=0x35000000
vm.mem_map(BASE,0x100000);vm.mem_map(STOP,0x1000);vm.mem_map(STACK,0x100000);vm.mem_map(APIS,0x1000)
regions=[(MUSIC,0x59a74+8),(ALT,0x59a74+8),(SOUND,0x204+8),(ALT_SOUND,0x204+8),(0xe38e60,0x80),(0xf55b88,0x14),(0x604950,4),(0xeaac28,4),(0xea1894,4),(0xea626c,4)]
def write(a,v,n=4):vm.mem_write(a,(v&((1<<(8*n))-1)).to_bytes(n,'little'))
def read(a,n=4):return int.from_bytes(vm.mem_read(a,n),'little')
events=[];params=[];active=active_sound=now=0;hits=set()
branches={0x539875:'main-track',0x539946:'ambient',0x5399c1:'start-delay',0x5399da:'wait-delay',0x539a02:'playlist-wrap'}
def probe(cpu,a,n,user):hits.add(branches[a])
for a in branches:vm.hook_add(UC_HOOK_CODE,probe,begin=a,end=a)
api_map={APIS:0x56f12c,APIS+16:0x56f298,APIS+32:0x56f29c}
for a,slot in api_map.items():write(slot,a)
def endpoint(cpu,a,n,user):
 sp=cpu.reg_read(UC_X86_REG_ESP);values=[];cleanup=0;result=0;label=api_map.get(a,a)
 if a==0x55f449:values=[cpu.reg_read(UC_X86_REG_ECX)]+[read(sp+4+i*4) for i in range(3)];cleanup=12;result=1
 elif label==0x56f12c:result=now
 elif label==0x56f298:
  values=[read(sp+4)];cleanup=4
  if params[15]&512:write(active_sound+0x1d8,0x56780000)
  result=4 if params[15]&16 else 2
 elif label==0x56f29c:values=[read(sp+4),read(sp+8)];cleanup=8
 events.append([label,len(values)]+values+[0]*(4-len(values))+[read(active+0x59a64),read(active+0x59a60)])
 cpu.reg_write(UC_X86_REG_EAX,result);cpu.reg_write(UC_X86_REG_EIP,read(sp));cpu.reg_write(UC_X86_REG_ESP,sp+4+cleanup)
for a in [0x55f449,*api_map]:vm.hook_add(UC_HOOK_CODE,endpoint,begin=a,end=a)
def execute(entry,args=(),receiver=0):
 sp=STACK+0xf0000;write(sp,STOP)
 for i,v in enumerate(args):write(sp+4+i*4,v)
 saved=[(UC_X86_REG_EBX,0x11111111),(UC_X86_REG_EBP,0x22222222),(UC_X86_REG_ESI,0x33333333),(UC_X86_REG_EDI,0x44444444)]
 for r,v in saved:vm.reg_write(r,v)
 vm.reg_write(UC_X86_REG_ECX,receiver);vm.reg_write(UC_X86_REG_ESP,sp);vm.reg_write(UC_X86_REG_EFLAGS,2)
 vm.emu_start(entry,STOP,count=2000000)
 assert vm.reg_read(UC_X86_REG_EIP)==STOP,'Native instruction budget'
 assert vm.reg_read(UC_X86_REG_ESP)==sp+4+(4 if entry==0x55f38d else 0),'Native stack differs'
 assert all(vm.reg_read(r)==v for r,v in saved),'Saved register mismatch'
 return vm.reg_read(UC_X86_REG_EAX)
def native(p):
 global params,active,active_sound,now
 params=p;op,seed,category,phase,count,ambient,current,waiting,previous,first,last,tick,duration,clock,population,flags=p
 events.clear();now=clock;active=ALT if flags&64 else MUSIC;active_sound=ALT_SOUND if op==1 and flags&1024 else SOUND
 for k,(a,n) in enumerate(regions):vm.mem_write(a,bytes([(seed+k)&255])*n)
 for m in [MUSIC,ALT]:
  for i in range(53):write(m+i*0x1b10+0x1b08,0);write(m+i*0x1b10+0x1b0c,0)
  c=m+category*0x1b10;write(c+0x1b08,0 if op==2 else count);write(c+0x1b0c,ambient)
  for i in range(60):
   vm.mem_write(c+i*112,f'cat{category}-track{i}.mp3'.encode()+b'\0');write(c+i*112+100,seed+i*3);write(c+i*112+104,i*100);write(c+i*112+108,(i+2)*100-1)
  for i in range(50):write(c+0x1a40+i*4,int(bool(flags&32)))
  for offset,v in [(0x59a50,1),(0x59a64,phase),(0x59a60,current),(0x59a6c,waiting),(0x59a5c,previous),(0x59a54,first),(0x59a58,last),(0x59a68,tick)]:write(m+offset,v)
 for sound in [SOUND,ALT_SOUND]:write(sound+0x200,int(bool(flags&8)));write(sound+0x1d8,0x12340000 if flags&4 else 0);write(sound+0x14c,-999)
 for a,v,n in [(0xf55b88,0x10000 if flags&1 else 0,4),(0xe38e64,128 if flags&2 else 0,1),(0xe38ea8,category,1),(0xe38ec8,population,4),(0xf55b98,duration,4),(0x604950,seed,4),(0xeaac28,seed&1,4),(0xea1894,population^0x13579bdf if seed&1 else population,4),(0xea626c,population if seed&1 else population^0x13579bdf,4)]:write(a,v,n)
 result=0
 if op==0:execute(0x539820,(),active)
 elif op==1:execute(0x55f38d,(population,),active_sound)
 elif op==2:
  for i in range(count):result=execute(0x562348)
 else:
  for i in range(12):now=(clock+i*10000)&0xffffffff;execute(0x539820,(),active)
 return {'result':result,'hashes':[sha(bytes(vm.mem_read(a,n))) for a,n in regions],'events':events.copy()}
default=[0,1,0,0,4,1,0,1,0,0,3,0,1000,1234,150,3];cases=[]
def case(**kw):
 p=default.copy()
 for k,v in kw.items():p[int(k[1:])]=v&0xffffffff
 cases.append(p)
for phase,category,flags,current,seed in itertools.product([0,1,2,3,4,0xffffffff],[0,1,52],[0,1,2,3,7,31],[0,1,49],[0,1,0xffffffff,0x80000000]):
 case(p1=seed,p2=category,p3=phase,p4=50,p5=[0,1,10][category%3],p6=current,p15=flags,p7=seed&1)
for count,ambient,flags in itertools.product([0,1,4,50],[0,1,10],[3,35,67,99]):case(p4=count,p5=ambient,p15=flags,p7=1,p8=7,p14=5000)
for tick,duration,clock in itertools.product([0,1,10,0xffffffff,0x7fffffff],[0,1,10000,0xffffffff],[0,1,10000,0x80000000,0xffffffff]):case(p3=3,p6=20,p10=19,p9=3,p11=tick,p12=duration,p13=clock)
for flags,volume in itertools.product([0,4,8,12,20,28,20|512,20|1024,20|512|1024],[0xffffffff,0,50,100,0x80000000,0x7fffffff]):case(p0=1,p14=volume,p15=flags)
for seed,steps in itertools.product([0,1,255,256,0x7fffffff,0x80000000,0xffffffff],[0,1,2,10,100]):case(p0=2,p1=seed,p4=steps)
for category,seed,count,flags in itertools.product([0,1,52],[0,1,255],[1,4,50],[3,7,31,67,515]):case(p0=3,p1=seed,p2=category,p4=count,p5=1,p7=0,p15=flags,p13=100000)
exe=ROOT/'build/c-only-debug/Debug/restore_music_tick_verify.exe'
process=subprocess.Popen([str(exe),'--oracle-batch'],cwd=ROOT,stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True)
report={'format':1,'original_game_executed':False,'native_sha256':digest,'compiled_C_sha256':sha(exe.read_bytes()),
 'method':'Complete 00539820 music state machine with actual 00539730/00539a50 selection, 0055f38d sound-volume method and 00562348 RNG. Global and alternate objects, full watched bytes/guards, ordered audio/time calls with intermediate state, RNG returns/state and native ABI compared.',
 'limitations':['Clock, stream playback and Miles status/volume API calls are controlled; physical audio output and codecs are not exercised.','Valid categories 0..52, track counts 0..50, ambient counts 0..10 and track indices through 50; malformed native pointers/counts are excluded.'],
 'checked':0};checked=0;p=None
try:
 assert json.loads(process.stdout.readline())=={'fields':16}
 for p in cases:
  process.stdin.write(' '.join(map(str,p))+'\n');process.stdin.flush();line=process.stdout.readline();assert line,'C exited: '+process.stderr.read()
  actual=json.loads(line);expected=native(p)
  assert actual==expected,repr({'C_result':actual['result'],'native_result':expected['result'],'different_regions':[hex(regions[i][0]) for i,(a,b) in enumerate(zip(actual['hashes'],expected['hashes'])) if a!=b],'event_counts':[len(actual['events']),len(expected['events'])],'first_event_difference':[(i,a,b) for i,(a,b) in enumerate(zip(actual['events'],expected['events'])) if a!=b][:1]})
  checked+=1
  if checked%200==0:print(json.dumps({'checked':checked,'total':len(cases)}),flush=True)
 assert hits==set(branches.values()),'Native branches not reached: '+repr(set(branches.values())-hits)
 report.update(passed=True,checked=checked)
except Exception as error:report.update(passed=False,checked=checked,case=p,error=str(error),native_eip=hex(vm.reg_read(UC_X86_REG_EIP)))
finally:process.stdin.close();process.wait(timeout=10)
sources=['include/music_manager.h','include/sound_streams.h','include/sound_initialization.h','runtime/music_manager.c','tests/music_tick.c','tools/verify_music_tick_emulation.py','metadata/music-tick-native.json']
sources += [f'src/unit_{i:05d}.c' for i in [1864,2477,2478,2480,2705,2825]]
report['source_hashes']={s:sha((ROOT/s).read_bytes()) for s in sources};report['observed_native_branches']=sorted(hits)
(ROOT/'metadata/music-tick-emulation.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({k:v for k,v in report.items() if k!='source_hashes'},indent=2));sys.exit(0 if report['passed'] else 1)
