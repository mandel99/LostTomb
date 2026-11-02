"""Full native terrain redraw and footprint comparison; no original game process."""
from pathlib import Path
from array import array
from functools import lru_cache
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
BASE=0x30000000;STOP=0x33000000;STACK=0x34000000
vm.mem_map(BASE,0x100000);vm.mem_map(STOP,0x1000);vm.mem_map(STACK,0x100000)
regions=[(0xa43eac,0x1857e4),(0xbc9690,0xc90),(0xd05720,0x33000),(0x7999d8,0x101d00),(0xf55bb4,0x1200),(BASE+0x1000,0x100)]
def write(a,v,n=4):vm.mem_write(a,(v&((1<<(n*8))-1)).to_bytes(n,'little'))
def read(a):return int.from_bytes(vm.mem_read(a,4),'little')
events=[];params=[]
counts={0x4f3d70:1,0x4f3f70:4,0x4f60d0:1,0x4f6130:0,0x51c5f0:2,0x484be0:4}
branches={0x4768cc:'building-b7',0x476a61:'building-group',0x476973:'building-e6',0x47699e:'building-e1',0x4769ee:'building-scan',
 0x476ad9:'connected-neighbor',0x476b35:'encoded-fertility',0x476b79:'ordinary-fertility',
 0x476d51:'blocked-neighbor',0x476d31:'uniform-neighborhood',0x476d11:'uneven-neighborhood',
 0x476ecd:'first-ground-pass',0x476fc6:'second-ground-pass',0x476fd2:'footprint-4',0x47700d:'footprint-3',0x477052:'footprint-2',0x477072:'footprint-1'}
hits=set()
def observe(cpu,a,n,user):hits.add(branches[a])
for a in branches:vm.hook_add(UC_HOOK_CODE,observe,begin=a,end=a)
def endpoint(cpu,a,n,user):
 sp=cpu.reg_read(UC_X86_REG_ESP);args=[read(sp+4+i*4) for i in range(counts[a])]
 row=[a,len(args)]+args+[0]*(10-len(args));row[10:12]=[read(0xbc9768),read(0xbc9748)];events.append(row);result=0
 if a==0x4f3d70:result=params[12]
 elif a==0x4f3f70:result=0xabcdef00|((params[15]>>8)&1)
 elif a==0x4f60d0:write(0x799a2c+args[0]*264,0)
 elif a==0x51c5f0:result=(args[1]+params[9])&127
 elif a==0x484be0:write(0xbc9748,params[9]&15)
 cpu.reg_write(UC_X86_REG_EAX,result);cpu.reg_write(UC_X86_REG_EIP,read(sp));cpu.reg_write(UC_X86_REG_ESP,sp+4)
for a in counts:vm.hook_add(UC_HOOK_CODE,endpoint,begin=a,end=a)
def execute(entry,args=()):
 sp=STACK+0xf0000;write(sp,STOP)
 for i,v in enumerate(args):write(sp+4+i*4,v)
 saved=[(UC_X86_REG_EBX,0x11111111),(UC_X86_REG_EBP,0x22222222),(UC_X86_REG_ESI,0x33333333),(UC_X86_REG_EDI,0x44444444)]
 for r,v in saved:vm.reg_write(r,v)
 vm.reg_write(UC_X86_REG_ESP,sp);vm.reg_write(UC_X86_REG_EFLAGS,2);vm.reg_write(UC_X86_REG_FPCW,0x37f);vm.emu_start(entry,STOP,count=10000000)
 assert vm.reg_read(UC_X86_REG_EIP)==STOP,'Native instruction budget'
 assert vm.reg_read(UC_X86_REG_ESP)==sp+4,'Native stack'
 assert all(vm.reg_read(r)==v for r,v in saved),'Saved register mismatch'
 return vm.reg_read(UC_X86_REG_EAX)
mix=[0,0x80,0x10000000,0x10000080,1,0x40,0x10000,0x10080,8,0x200,0xc0]
offsets=[-228,-227,1,229,228,227,-1,-229]
@lru_cache(maxsize=64)
def random_grid(seed,options):return bytes((0xf0|(seed&7)) if options&0x1000 else (i+seed*13)&255 for i in range(51984))
def native(p):
 global params
 params=p;op,w,h,x,y,x1,y1,preserve,exact,seed,terrain,kind,state,fertility,orientation,options=p;events.clear()
 for i,(a,n) in enumerate(regions):vm.mem_write(a,bytes([(seed+i)&255])*n)
 vm.mem_write(0xd05720,bytes(0x33000));vm.mem_write(0x7999d8,bytes(0x101d00));vm.mem_write(0xb7482c,bytes(51984*4));vm.mem_write(0xb350dc,bytes(51984*2));vm.mem_write(0xb4e6fc,bytes(51984*2))
 origin=((228-h)//2)*228+(228-w)//2;center=origin+(h//2)*228+w//2
 for a,v in [(0xbca2d0,w),(0xbca2d4,h),(0xbca2d8,origin),(0xbca2dc,228-w),(0xbca2e0,orientation),(0xbc9768,0x11223344),(0xbc9748,seed+0x12340000)]:write(a,v)
 execute(0x46f7b0)
 for i in range(0x240):write(0xf55bb4+i*8,10000+i*100)
 write(0xf56b74,17000+seed);write(0xf56b8c,18000+seed)
 ident=10+(seed&3)*500;parent=ident-(3 if options&4 else 0)
 for i in range(25):write(0x799a30+(parent+i)*264,state)
 write(0x7999e8+ident*264,kind,2);write(0x7999ea+ident*264,3 if options&4 else 0,2);write(0x799a2c+parent*264,state)
 write(BASE+0x1018,int(bool(options&2)),1)
 flags=[mix[(i+seed)%11] if options&64 else terrain for i in range(51984)]
 if options&16:flags=[0]*51984;flags[center]=terrain
 vm.mem_write(0xaf598c,array('I',flags).tobytes())
 vm.mem_write(0xa509bc,bytes([fertility&255])*51984);vm.mem_write(0xacf85c,random_grid(seed,options&0x1000))
 vm.mem_write(0xb4e6fc,array('h',[ident if f&0x10000000 else 0 for f in flags]).tobytes())
 if options&1:vm.mem_write(0xd05720,struct.pack('<I',BASE+0x1000)*51984)
 if options&8:
  previous=center-20;write(0xb4e6fc+previous*2,15,2);write(0x7999e8+15*264,0xb7,2);write(0xaf598c+previous*4,0x10000000)
 if options&32:
  for i in range(0,8,2):a=0xaf598c+(center+offsets[i])*4;write(a,read(a)|0x10000)
 if options&0x2000:a=0xaf598c+(center+229)*4;write(a,read(a)|1)
 if options&0x4000:
  for o in offsets:write(0xa509bc+center+o,88,1)
  write(0xa509bc+center-229,0,1)
 block=(options>>16)&7
 if block:write(0xb350dc+(center+block*228+block)*2,1,2)
 result=0
 if op==0:execute(0x4767d0,(x,y,x1,y1,preserve,exact))
 elif op==1:execute(0x47c140,(state,x,y,x1,y1,seed,terrain))
 elif op==2:result=execute(0x47c880,(x,y,x1,y1,kind,preserve,exact))
 elif op==3:result=execute(0x46f9a0,(x,y,x1))
 else:write(0xbc9768,center);execute(0x47db00,(terrain,));result=execute(0x484f40)
 return {'result':result,'hashes':[sha(bytes(vm.mem_read(a,n))) for a,n in regions],'events':events.copy()}
default=[0,40,40,20,20,20,20,1,1,0,0x80,0,0,88,0,16];cases=[]
def case(**kw):
 p=default.copy()
 for k,v in kw.items():p[int(k[1:])]=v&0xffffffff
 cases.append(p)
for fertility,orientation,terrain in itertools.product(range(256),[0,1,7,0xffffffff,0x80000000],[0x80,0xc0]):case(p13=fertility,p14=orientation,p10=terrain,p9=fertility)
for kind,state,options in itertools.product([0xb7,0xd5,0xe3,0xe5,0xea,0xeb,0xec,0xe4,0xe6,0xe1,0,0xb8],[0xffffffff,0,1,3,10,11],[16,20,24,272]):case(p10=0x10000080,p11=kind,p12=state,p15=options)
for fertility,options in itertools.product([88,95,96,100,101,255],[16,16|32,16|0x2000,16|0x4000]):case(p13=fertility,p15=options)
for footprint in [1,2,3,4]:case(p7=0,p9=7,p10=0,p15=0x1000|(footprint<<16))
for w,h in [(0,0),(1,1),(3,3),(12,12),(40,40),(140,228),(228,228)]:
 for preserve,exact,options in itertools.product([0,1,256,257],[0,1,256,257],[0,3,64,0x1000]):case(p1=w,p2=h,p3=0,p4=0,p5=min(w,12)-1,p6=min(h,12)-1,p7=preserve,p8=exact,p15=options,p10=0,p9=7)
for x,y,(width,height),seed in itertools.product([0,19,38,40,0xffffffff],[0,19,38,40,0xffffffff],[(0,0),(1,1),(2,3),(6,6)],[0,0xfffffffe]):case(p0=1,p3=x,p4=y,p5=width,p6=height,p9=seed,p10=0x10000008,p12=0xfffffffe)
for terrain,mask,required,allow,(width,height),options in itertools.product([0,0x80,0x10000,1],[0,0xffff,0xffffffff],[0,1],[0,1,256],[(0,0),(1,1),(4,4),(6,6)],[0,1,3]):case(p0=2,p3=20,p4=20,p5=width,p6=height,p7=required,p8=allow,p10=terrain,p11=mask,p15=options)
for w,h,(x,y),margin in itertools.product([12,140,228],[12,140,228],[(0,0),(114,114),(227,227),(228,228),(0x80000000,0x7fffffff),(0x7fffffff,0x7fffffff)],[0,3,0xffffffff,0x80000000]):case(p0=3,p1=w,p2=h,p3=x,p4=y,p5=margin)
for mask,options in itertools.product([0,1,0x10000,0x80,0xffffffff],[0,32,0x2000,64]):case(p0=4,p10=mask,p15=options)
exe=ROOT/'build/c-only-debug/Debug/restore_terrain_redraw_verify.exe'
process=subprocess.Popen([str(exe),'--oracle-batch'],cwd=ROOT,stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True)
report={'format':1,'original_game_executed':False,'native_sha256':digest,'compiled_C_sha256':sha(exe.read_bytes()),
 'method':'Complete 004767d0 three-pass terrain redraw with actual 0047c140 placement, 0047c880 footprint selection, 0046f9a0 margin predicate, 0047db00 neighbor gather and 00484f40 count. All watched bytes, endpoint order/arguments/intermediate globals, return EAX, stack and saved registers compared.',
 'limitations':['Remaining building-state callbacks, final queued-building refresh, connected-neighbor graphic selector and rotated variant callback are controlled.','Valid geometry 0..228, footprint table dimensions 0..6 and valid object/building pointers; malformed native inputs that would access invalid memory are excluded.'],
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
sources=['include/terrain_redraw.h','include/city_prepare.h','tests/terrain_redraw.c','tools/verify_terrain_redraw_emulation.py','metadata/terrain-redraw-next-native.json','metadata/terrain-redraw-helpers-native.json']
sources += [f'src/unit_{i:05d}.c' for i in [1009,1011,1012,1043,1117,1122,1137,1238]]
report['source_hashes']={s:sha((ROOT/s).read_bytes()) for s in sources};report['observed_native_branches']=sorted(hits)
(ROOT/'metadata/terrain-redraw-emulation.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({k:v for k,v in report.items() if k!='source_hashes'},indent=2));sys.exit(0 if report['passed'] else 1)
