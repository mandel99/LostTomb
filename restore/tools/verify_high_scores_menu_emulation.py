"""Compare complete rankings entry/read/render/scroll and native mode-83 input."""
from pathlib import Path
from datetime import datetime,timezone
import argparse,hashlib,itertools,json,struct,subprocess,sys
ROOT=Path(__file__).resolve().parent.parent;sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
def sha(b):return hashlib.sha256(b).hexdigest()
parser=argparse.ArgumentParser();parser.add_argument('--exe',type=Path,default=ROOT/'build/agent-menu/out/Debug/restore_high_scores_menu_verify.exe');args=parser.parse_args();exe=args.exe.resolve()
data=(ROOT/'build/smoke-game/Pharaoh.exe').read_bytes();digest=sha(data)
assert digest=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
u16=lambda b,p:struct.unpack_from('<H',b,p)[0];u32=lambda b,p:struct.unpack_from('<I',b,p)[0]
pe=u32(data,0x3c);opt=pe+24;table=opt+u16(data,pe+20);base=u32(data,opt+28);size=u32(data,opt+56)
vm=Uc(UC_ARCH_X86,UC_MODE_32);vm.mem_map(base,(size+4095)&~4095)
for i in range(u16(data,pe+6)):
 p=table+i*40;vm.mem_write(base+u32(data,p+12),data[u32(data,p+20):u32(data,p+20)+u32(data,p+16)])
BASE=0x30000000;STOP=0x33000000;STACK=0x34000000
for a in [BASE,STOP,STACK,0x14a40000]:vm.mem_map(a,0x10000)
def write(a,v,n=4):vm.mem_write(a,(v&((1<<(n*8))-1)).to_bytes(n,'little'))
def read(a):return int.from_bytes(vm.mem_read(a,4),'little')
def byte(a):return vm.mem_read(a,1)[0]
def string(a):
 out=bytearray()
 for i in range(1024):
  c=byte(a+i)
  if not c:return bytes(out)
  out.append(c)
 raise AssertionError('Unterminated native string '+hex(a))
def string_hash(a):
 v=2166136261
 for c in string(a):v=((v^c)*16777619)&0xffffffff
 return v
regions=[(0xd38700,0x1dc8),(0x6ff608,5016),(0xe20774,12),(0xe38da0,16),(0xe38dc6,16),(0xe38dd6,12),
 (0xe38df4,16),(0xe38e2c,16),(0xd3b194,12),(0xe38ea4,12),(0x5d2a28,12),(BASE+0x3ff8,0x1dc0),(0x14a439f8,92)]
counts={0x561a0a:2,0x561ca1:4,0x561994:1,0x536820:4,0x536a30:4,0x4ccb30:7,0x4ccbb0:6,0x4cc770:5,0x4cce00:7,0x53e590:3,0x425990:4}
events=[];io_open=io_bytes=io_result=advance=small_result=0;reached=set();current_op=-1;null_mission_guard_taken=False
probes={0x4c541d:'up decrement',0x4c543c:'down increment',0x4c5442:'other scroll action',0x4c5473:'thumb bottom',0x4c547a:'thumb intermediate',0x4c5466:'thumb top',
 0x4c5519:'valid score row',0x4c5513:'mission address null guard',0x4c560a:'skipped row or next row',0x4c5624:'empty score message',0x4c5620:'mission scan complete',0x4c564f:'row/count scan guard',
 0x4c57df:'drag checks',0x4c5805:'left outside',0x4c5812:'right outside',0x4c582b:'above outside',0x4c5839:'below outside',0x4c584b:'drag clamped bottom',0x4c5865:'drag accepted',
 0x4c589c:'open failure',0x4c58a3:'read call',0x4c58bc:'read return consumed',0x4c5957:'valid count',0x4c5958:'next count',0x4c5964:'entry success',0x4c5967:'entry failure',
 0x4dd76d:'back to title',0x4dd790:'small-button fallback',0x4dd8c3:'drag consumed',0x418089:'main rankings mode'}
def on_code(cpu,a,n,user):
 global null_mission_guard_taken
 if a==STOP:return
 if a in probes:reached.add(a)
 if a==0x4c5513 and cpu.reg_read(UC_X86_REG_EAX)==0:null_mission_guard_taken=True
 if a not in counts:
  assert any(lo<=a and a+n<=hi for lo,hi in [(0x4c5400,0x4c58c7),(0x4c5920,0x4c596a),(0x4cde40,0x4cde80),(0x4dd764,0x4dd7b4),(0x4dd8c3,0x4dd8cb),(0x417ff0,0x4180a9)]),'Native code escaped '+hex(a)
  return
 sp=cpu.reg_read(UC_X86_REG_ESP);argv=[read(sp+4+i*4) for i in range(counts[a])];values=argv.copy();cleanup=0;result=0x12345678
 if a==0x561a0a:
  assert string(argv[1])==b'rb';values=[string_hash(x) for x in argv];result=BASE+0xf000 if io_open else 0
 elif a==0x561ca1:
  assert argv[1:]==[0x1db0,1,BASE+0xf000];vm.mem_write(argv[0],bytes(vm.mem_read(BASE+0x1000,io_bytes)));result=io_result
 elif a==0x561994:assert argv==[BASE+0xf000]
 elif a==0x4cc770:values[0]=string_hash(argv[0])
 elif a==0x4cce00:values[2]=string_hash(argv[2])
 elif a==0x53e590:
  assert cpu.reg_read(UC_X86_REG_ECX)==0xf55bb0;values.insert(0,0xf55bb0);cleanup=12;result=1
 elif a==0x425990:result=small_result
 row=[a,len(values)]+values+[0]*(20-2-len(values));row[12:17]=[read(0xd3a4b8),read(0xd3a4bc),byte(0xe38dca),byte(0xe38dda),read(0xe20778)];events.append(row)
 if a in [0x4ccbb0,0x4cc770,0x4cce00]:write(0xe20778,read(0xe20778)+advance)
 cpu.reg_write(UC_X86_REG_EAX,result&0xffffffff);cpu.reg_write(UC_X86_REG_EIP,read(sp));cpu.reg_write(UC_X86_REG_ESP,sp+4+cleanup)
vm.hook_add(UC_HOOK_CODE,on_code)
def native(p):
 global io_open,io_bytes,io_result,advance,small_result,current_op
 op,missions,first,total,pattern,ox,oy,mx,my,flags,action,io_open,io_bytes,io_result,advance,small_result=p;current_op=op;events.clear()
 vm.mem_write(BASE,bytes(0x10000))
 for a,n in regions:vm.mem_write(a,b'\xa5'*n)
 vm.mem_write(BASE+0x1000,b'\x5a'*7600);vm.mem_write(BASE+0x100,b'fixture-scores.bin\0')
 for i in range(100):
  record=BASE+0x1000+i*76;write(record,100000+i*17);write(record+4,i);vm.mem_write(record+8,('Family%03u'%i).encode()+b'\0');write(record+64,i%5)
  valid=0 if pattern==0 else (1 if pattern==1 else ((256 if i%2 else 0) if pattern==2 else (0 if i%3 else 0x80000000)))
  write(record+72,valid);vm.mem_write(0x6ff60c+i*50,('Mission%03u'%i).encode()+b'\0')
 vm.mem_write(0xd38708,bytes(vm.mem_read(BASE+0x1000,7600)))
 if op in [0,1,8]:vm.mem_write(0xd38708,b'\x6d'*7600)
 write(0x14a43a48,1)
 for a,v in [(0x700994,missions),(0xd3a4b8,first),(0xd3a4bc,total),(0xe38da8,ox),(0xe38da4,oy),(0xe38e34,ox+mx),(0xe38e30,oy+my),
  (0xd3b198,action),(0xe20778,0x12345678),(0xf56f84,1005),(0xf56f8c,335),(0xf56f94,871),(0xf56fb4,18765),(0xf56494,49000)]:write(a,v)
 for a,v in [(0xe38e00,bool(flags&1)),(0xe38df8,bool(flags&2)),(0xe38dca,0),(0xe38dda,0x53)]:write(a,v,1)
 sp=STACK+0xe000;vm.mem_write(STACK,bytes(0x10000));write(sp,STOP)
 saved=[(UC_X86_REG_EBX,0x11111111),(UC_X86_REG_EBP,0x22222222),(UC_X86_REG_ESI,0x33333333),(UC_X86_REG_EDI,2 if op==7 else 0x44444444)]
 for r,v in saved:vm.reg_write(r,v)
 if op==1:vm.reg_write(UC_X86_REG_ECX,BASE+0x4000);write(sp+4,BASE+0x100)
 if op==7:
  for offset,value in [(0,2),(4,0x33333333),(8,0x22222222),(12,0x11111111),(140,STOP)]:write(sp+offset,value)
 vm.reg_write(UC_X86_REG_ESP,sp);vm.reg_write(UC_X86_REG_EFLAGS,2)
 vm.emu_start([0x4c5920,0x4c5880,0x4c5400,0x4c5450,0x4c54c0,0x4c5660,0x4c57d0,0x4dd764,0x417ff0][op],STOP,count=300000)
 assert vm.reg_read(UC_X86_REG_EIP)==STOP,'Native budget/return failed'
 assert vm.reg_read(UC_X86_REG_ESP)==sp+(144 if op==7 else (8 if op==1 else 4)),'Native stack differs'
 assert all(vm.reg_read(r)==v for r,v in saved),'Native saved register differs'
 result=vm.reg_read(UC_X86_REG_EAX)&(255 if op in [0,1] else 0xffffffff) if op in [0,1,6] else 0
 return {'result':result,'state':b''.join(bytes(vm.mem_read(a,n)) for a,n in regions).hex(),'events':events.copy()}
default=[0,80,0,40,1,192,144,584,200,1,0,1,7600,1,37,0];cases=[]
def add(**kw):
 p=default.copy()
 for key,value in kw.items():p[{'op':0,'missions':1,'first':2,'total':3,'pattern':4,'ox':5,'oy':6,'mx':7,'my':8,'flags':9,'action':10,'opened':11,'copied':12,'returned':13,'advance':14,'small':15}[key]]=value&0xffffffff
 cases.append(p)
for missions,pattern,opened,returned,copied in itertools.product([-256,-1,0,1,20,21,80,100],range(4),range(2),[0,1,2,0xffffffff],[123,7600]):add(missions=missions,pattern=pattern,opened=opened,returned=returned,copied=copied)
for opened,copied,returned in itertools.product(range(2),[0,1,75,76,123,7599,7600],[0,1,2,0xffffffff]):add(op=1,opened=opened,copied=copied,returned=returned)
for missions,pattern,opened,copied,returned in itertools.product([0,80],range(4),range(2),[123,7600],[0,1,2,0xffffffff]):add(op=8,action=3,missions=missions,pattern=pattern,opened=opened,copied=copied,returned=returned)
for total,first,action in itertools.product([-1,0,20,21,300],[-1,0,1,19,20,279,280,281],[0,1,2,0xffffffff]):add(op=2,total=total,first=first,action=action)
for total,first,offset in itertools.product([-1,0,20,21,300,0x80000000],[-1,0,1,279,280,281,0x7fffffff],[(0,0),(80,60),(192,144)]):add(op=3,total=total,first=first,ox=offset[0],oy=offset[1])
for op,total,first,pattern,missions,offset in itertools.product([4,5],[0,256],[0,1,20],range(4),[-256,-1,0,1,20,21,60,80],[(0,0),(80,60),(192,144)]):add(op=op,total=total,first=first,pattern=pattern,missions=missions,ox=offset[0],oy=offset[1])
for op,offset in itertools.product([4,5],[(0,0),(80,60),(192,144)]):add(op=op,total=1,first=0x1478a40a,missions=1,ox=offset[0],oy=offset[1])
for op,total,flags,mx,my in itertools.product([6,7],[21,300],range(4),[567,568,584,600,601],[127,128,256,384,385,386,401,402]):add(op=op,total=total,flags=flags,mx=mx,my=my,small=0xffffffff)
for op,total,flags,my in itertools.product([6,7],[-1,0,20],[0,1,3],[128,385]):add(op=op,total=total,flags=flags,my=my)
smoke=subprocess.run([str(exe)],cwd=ROOT,capture_output=True,text=True,timeout=30)
assert smoke.returncode==0,'Compiled C actual-file smoke failed: '+smoke.stdout+smoke.stderr
smoke_dir=exe.parent/'high-scores-fixtures'
assert not (smoke_dir/'save/highscore.jas').exists(),'Actual-file smoke left its fixture behind'
process=subprocess.Popen([str(exe),'--oracle-batch'],cwd=ROOT,stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True)
report={'format':1,'generated_at':datetime.now(timezone.utc).isoformat(),'original_game_process_executed':False,'computer_UI_automation_used':False,'original_machine_code_fallback_used':False,
 'native_sha256':digest,'compiled_C_path':str(exe.relative_to(ROOT)).replace('\\','/'),'compiled_C_sha256':sha(exe.read_bytes()),
 'actual_file_smoke':{'passed':True,'exit_code':smoke.returncode,'output':smoke.stdout.strip(),'fixture_directory':str(smoke_dir.relative_to(ROOT)).replace('\\','/'),'fixture_removed':True},
 'method':'Pinned native x86 in bounded Unicorn compared with handwritten compiled C. Complete rankings read/entry/rows/renderer/thumb/arrow/drag, root action3, and mode83 input branch. Exact file/draw endpoint arguments and ordering, all record bytes and guard neighborhoods, consumed AL/full returns, callee-saved registers and native stack cleanup are checked.',
 'limitations':['Draw/text/small-button and legacy CRT IO endpoints are controlled in differential cases; final pixels and actual small-button callback execution are integration checks. Separate C smoke performs real missing/full/partial highscore.jas reads in the private build fixture directory.',
 'Renderer mission/record scans use their native signed count and sparse-row behavior. Normal fixtures remain within the 100-record capacity; six controlled address-wrap fixtures additionally prove the native null mission-address guard against a valid alternate record.',
 '004c5880 returns a Boolean in AL; unused upper EAX bits are not a public contract. Both native callers set ECX to 00d38708; the explicit reader is additionally compared at an alternate destination.']}
checked=0;p=None
try:
 assert json.loads(process.stdout.readline())=={'fields':16}
 for p in cases:
  process.stdin.write(' '.join(map(str,p))+'\n');process.stdin.flush();line=process.stdout.readline();assert line,'C exited: '+process.stderr.read()
  actual=json.loads(line);expected=native(p)
  for key in expected:
   if actual[key]!=expected[key]:
    detail=[actual[key],expected[key]]
    if key=='state':
     aa=bytes.fromhex(actual[key]);bb=bytes.fromhex(expected[key]);detail={'first_difference':next((i for i in range(len(aa)) if aa[i]!=bb[i]),None),'C_bytes':len(aa),'native_bytes':len(bb)}
    raise AssertionError(key+' differs: '+str(detail))
  checked+=1
 assert set(probes)<=reached,'Missing probes '+str([probes[x] for x in set(probes)-reached])
 assert null_mission_guard_taken,'Null mission-address branch was not exercised'
 report.update(passed=True,checked=checked)
except Exception as error:report.update(passed=False,checked=checked,case=p,error=str(error),native_eip=hex(vm.reg_read(UC_X86_REG_EIP)))
finally:process.stdin.close();process.wait(timeout=10)
report['branch_probes']={hex(a):{'name':name,'reached':a in reached} for a,name in probes.items()}
report['null_mission_address_guard_taken']=null_mission_guard_taken
sources=['include/high_scores_menu.h','runtime/high_scores_menu.c','runtime/menu_actions.c','src/unit_01610.c','src/unit_01611.c','src/unit_01612.c','src/unit_01613.c','src/unit_01614.c','src/unit_01616.c','src/unit_01730.c','src/unit_01731.c','tests/high_scores_menu.c','tools/verify_high_scores_menu_emulation.py']
report['source_hashes']={s:sha((ROOT/s).read_bytes()) for s in sources}
report['native_exports']={s:sha((ROOT/s).read_bytes()) for s in ['metadata/high-scores-native.json','metadata/high-scores-input-native.json','metadata/high-scores-scroll-native.json']}
(ROOT/'metadata/high-scores-menu-emulation.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report,indent=2));sys.exit(0 if report['passed'] else 1)
