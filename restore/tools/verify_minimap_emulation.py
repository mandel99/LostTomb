"""Native minimap scan, building dispatcher, geometry and actual two-pixel writes."""
from pathlib import Path
import array,hashlib,itertools,json,re,struct,subprocess,sys
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
BASE=0x30000000;SPRITES=BASE+0x1000;FB=BASE+0x100000;ALT=BASE+0x400000;STOP=0x33000000;STACK=0x34000000
vm.mem_map(BASE,0x800000);vm.mem_map(STOP,0x1000);vm.mem_map(STACK,0x100000)
regions=[(BASE,0x40000),(FB,0x200000),(0x10d69f8,4097*64+8),(0x12650b8,72),(0x1265178,16),(ALT+0x180e48,4097*64+8),(ALT+0x30f508,72),(ALT+0x30f5c8,16),(0xd3b500,256),(0xe38d90,40),(0xe92df8,40),(0xe94dc0,80),(0x70e3c0,0x775620-0x70e3c0),(0x77567c,16),(0x5ce004,237*4+8),(0x7999d0,4000*264+16),(0xacf850,0xb74830-0xacf850),(0xbcb9f0,0xc8ad48-0xbcb9f0),(0x70e11c,12),(0x6fdc4c,12),(0x605a38,12),(0xf56880,0x2a0),(0x784cac,12)]
buffers=[(a,bytearray(n)) for a,n in regions]
def location(a):
 for start,b in buffers:
  if start<=a<start+len(b):return b,a-start
 raise AssertionError(hex(a))
def put(a,v,n=4):
 b,o=location(a);struct.pack_into({1:'<B',2:'<H',4:'<I'}[n],b,o,v&((1<<(8*n))-1))
def fill(a,n,value):
 b,o=location(a);b[o:o+n]=bytes([value&255])*n
def block(a,data):
 b,o=location(a);b[o:o+len(data)]=data
def write(a,v,n=4):vm.mem_write(a,(v&((1<<(8*n))-1)).to_bytes(n,'little'))
def read(a,n=4):return int.from_bytes(vm.mem_read(a,n),'little')
lookup=[array.array('I',(0x0fff0000 if mode==1 or (mode==2 and k%11==0) else 50000+k%1984 if mode==3 else k%51984 for k in range(231*457))).tobytes() for mode in range(4)]
metrics={int(v,16) for v in re.findall(r'METRIC\(FUN_[0-9a-f]+,0x([0-9a-f]+)\)',(ROOT/'tests/minimap.c').read_text())}|{0x433ae0,0x434820}
dossier=json.loads((ROOT/'metadata/water-ground-render-next-native.json').read_text())
calls={int(i['instruction'].split(' ')[1],16) for i in dossier['callers']['00439980']['items'] if i['instruction'].startswith('CALL ')}
assert metrics==calls-{0x43e770,0x53e590,0x53f4a0},'Every native overlay metric endpoint must be represented'
params=[];events=[];hits=set()
branches={0x53f4b7:'cell-index-fallback',0x53f4d5:'cell-coordinates',0x40fc08:'actual-pixel-copy',0x40fc36:'actual-solid-pair',0x43e6f6:'source-column-clamp',0x43e725:'source-row-low',0x43e73f:'source-row-high',0x43e74e:'source-row-inside',0x43e75f:'source-row-centered',0x43ea2f:'valid-map-cell',0x43eb04:'building-cell',0x43eb4e:'overlay-49',0x43eb8b:'terrain-extra-byte',0x43ebb5:'terrain-special-50',0x43ecfd:'actor-color-pair',0x43ed10:'black-cell',0x43ed79:'camera-outline',0x439c23:'normal-building',0x439d11:'building-two-pixels',0x439df1:'building-four-sprites',0x43ee78:'formation-sign',0x43ee9c:'actor-color-2',0x43eea3:'actor-color-3',0x43e7fa:'actor-predicate-true'}
def probe(cpu,a,n,user):hits.add(branches[a])
for a in branches:vm.hook_add(UC_HOOK_CODE,probe,begin=a,end=a)
def endpoint(cpu,a,n,user):
 sp=cpu.reg_read(UC_X86_REG_ESP);count=5 if a==0x4cd680 else 3 if a in [0x53e590,0x433ae0] else 1
 args=[read(sp+4+i*4) for i in range(count)]
 events.append([a,count]+args+[0]*(5-count)+[read(0x70e488),read(0x70e480),read(0x70e47c),read(0x70e120,2)]+[read(x) for x in [0xd3b5e0,0xd3b5dc,0xd3b55c,0xd3b554,0xd3b558,0xd3b550,0xe94e0c]])
 if a in metrics:
  if params[31]&1:write(0x70e488,2)
  if params[31]&4:write(0x7999e8+params[14]*264,31,2);write(0xaf598c+read(0x70e488)*4,read(0xaf598c+read(0x70e488)*4)|0x40)
  result=int(params[30]!=0) if a==0x434820 else params[30]
 elif a==0x4387d0:
  if params[31]&1:write(0x70e488,2)
  if params[31]&2:write(0xe94e0c,51)
  result=int(params[42]==1 or (params[42]==2 and args[0]==1000))
 else:
  result=1 if a==0x53e590 else 0
  if a==0x4cd680 and params[31]&8:write(0xd3b55c,0xdeadbeef);write(0xd3b554,0x12345678)
 cpu.reg_write(UC_X86_REG_EAX,result&0xffffffff);cpu.reg_write(UC_X86_REG_EIP,read(sp));cpu.reg_write(UC_X86_REG_ESP,sp+4+(12 if a==0x53e590 else 0))
for a in metrics|{0x4387d0,0x53e590,0x4cd680}:vm.hook_add(UC_HOOK_CODE,endpoint,begin=a,end=a)

def execute(p):
 op=p[0];entries=[0x43e810,0x439980,0x53f4a0,0x43e670,0x43ee10,0x43eeb0,0x43e790,0x43e770,0x40fbe0,0x40fc10,0x53f4a0]
 args=[p[2],p[3],p[4],p[5]] if op==0 else [p[14],p[2],p[3]] if op==1 else [p[33],p[2],p[3]] if op in [2,10] else [p[4],p[5]] if op==3 else [p[18]] if op in [4,5,6] else [p[15]] if op==7 else [SPRITES+8] if op==8 else [p[33]]
 sp=STACK+0xf0000
 for i,v in enumerate([STOP]+args):write(sp+i*4,v)
 saved=[(UC_X86_REG_EBX,0x11111111),(UC_X86_REG_EBP,0x22222222),(UC_X86_REG_ESI,0x33333333),(UC_X86_REG_EDI,0x44444444)]
 for r,v in saved:vm.reg_write(r,v)
 if op in [8,9]:write(0xd3b5e0,p[2]);write(0xd3b5dc,p[3])
 vm.reg_write(UC_X86_REG_ECX,ALT if op==10 else 0xf55bb0);vm.reg_write(UC_X86_REG_ESP,sp);vm.reg_write(UC_X86_REG_EFLAGS,2);vm.reg_write(UC_X86_REG_EAX,0xdeadbeef)
 vm.emu_start(entries[op],STOP,count=20000000)
 assert vm.reg_read(UC_X86_REG_EIP)==STOP,'Instruction budget'
 assert vm.reg_read(UC_X86_REG_ESP)==sp+4+(12 if op in [2,10] else 0),'Native stack'
 assert all(vm.reg_read(r)==v for r,v in saved),'Saved registers'
 return vm.reg_read(UC_X86_REG_EAX) if op in [1,4,5,6,7] else 0

def native(p):
 global params
 params=p;events.clear()
 for k,(a,n) in enumerate(regions):fill(a,n,p[1]+k)
 for i in list(range(4097))+[25499]:
  pointer=0 if p[35]==1 else 0xffffffff if p[35]==2 else SPRITES+i*8
  pixel=(p[1]^((i*0x1020301+0x4a5b6c7d)&0xffffffff))&0xffffffff
  put(SPRITES+i*8,pixel);put(SPRITES+i*8+4,~pixel);put(0x10d69f8+i*64,pointer);put(ALT+0x180e48+i*64,pointer)
 for a,v,n in [(0x126517c,p[34],4),(ALT+0x30f5cc,p[34],4),(0xe92e18,FB+0x10000,4),(0xe38d94,p[41],4),(0xe38db0,1024,4),(0xe38dac,768,4),(0xe92dfc,p[6],4),(0x784cb0,p[7],1)]:put(a,v,n)
 block(0x70e494,lookup[p[32]])
 for a,value,n in [(0xaf598c,p[9],4),(0xb4e6fc,p[14],2),(0xb350dc,p[18],2),(0xacf85c,p[10],1),(0xadc36c,p[11],1),(0xb67d1c,p[12],1),(0xb285cc,p[13],1)]:block(a,(value&((1<<(8*n))-1)).to_bytes(n,'little')*51984)
 put(0xacf85c+2,p[10]^7,1);put(0xadc36c+2,p[11]^1,1);put(0x70e488,1)
 for k in range(237):put(0x5ce008+k*4,0 if p[37]==1 else 0xffffffff if p[37]==2 else 0x7fffffff if p[37]==3 else k%15)
 fill(0x7999d8,4000*264,0);a=0x7999d8+p[14]*264;put(a+5,p[16],1);put(a+16,p[15],2);put(a+74,p[17],2)
 actor_buffer,actor_offset=location(0xbcd600)
 for k in range(2000):struct.pack_into('<HB',actor_buffer,actor_offset+k*388,0,0)
 ids=[p[18]]+[i for i in [1,257,1000] if i!=p[18]][:2]
 for k in range(p[21]+1):
  a=0xbcd600+ids[k]*388;put(a,ids[k+1] if k<p[21] else 0,2);put(a+2,p[22] if k==p[21] else p[19],1);put(a+48,p[20],1);put(a+88,37,2)
 put(0xbcba4a+37*144,p[23],2)
 for k,a in enumerate([0xf56884,0xf5688c,0xf56894,0xf568a4,0xf568ac,0xf568b4,0xf568bc,0xf568c4,0xf568d4,0xf568dc,0xf56ab4,0xf56b14]):put(a,1000+k*31+(p[1]&255))
 for a,v,n in [(0xe94e0c,p[8],4),(0x775680,p[29],1),(0xe94dd0,0,4),(0xe94dc8,0,4),(0xe94dcc,230,4),(0xe94dc4,457,4)]:put(a,v,n)
 if p[40]==1:put(0xe94dd0,231)
 if p[40]==2:
  for a,v in [(0xe94dd0,100),(0xe94dcc,110),(0xe94dc8,220),(0xe94dc4,224)]:put(a,v)
 for k,value in enumerate([[456,230,0,0],[60,50,10,20],[3,3,0,0],[456,230,449,224]][p[24]]):put(0x70e420+k*4,value)
 for a,v in [(0x70e478,p[25]),(0x70e474,p[26]),(0x70e464,p[27]),(0x70e460,p[28]),(0x6fdc50,p[38]),(0x605a3c,p[39])]:put(a,v)
 for a,b in buffers:vm.mem_write(a,bytes(b))
 result=0
 for i in range(p[36]):result=execute(p)
 return {'result':result,'hashes':[sha(bytes(vm.mem_read(a,n))) for a,n in regions],'events':events.copy()}

default=[0,1,320,144,3,2,2,1,0,4,3,0,64,0,257,90,0,1,0,0,0,0,0,65535,0,110,220,30,20,1,1,0,0,1,25500,0,1,0,0,0,0,4096,0,0]
cases=[]
def case(**kw):
 p=default.copy()
 for k,v in kw.items():p[int(k[1:])]=v&0xffffffff
 cases.append(tuple(p))
for op,image,count,pointer in itertools.product([2,10],[-1,0,1,255,4095,4096,25499,25500],[0,1,4096,25500,0xffffffff],range(3)):
 case(p0=op,p33=image,p34=count,p35=pointer,p1=image&0xffffffff)
for op,seed,xy,pitch in itertools.product([8,9],[0,1,0xffffffff],[(0,0),(320,144),(-2,-2)],[3840,4096,4097]):case(p0=op,p1=seed,p2=xy[0],p3=xy[1],p41=pitch,p33=0x12345678)
for wh,bounds,camera,visible in itertools.product([(-4,-8),(0,0),(3,2),(73,111),(232,458),(0x80000000,0x7fffffff)],range(4),[(-10,-10),(0,0),(110,220),(500,900)],[(0,0),(30,20),(255,511)]):case(p0=3,p4=wh[0],p5=wh[1],p24=bounds,p25=camera[0],p26=camera[1],p27=visible[0],p28=visible[1])
types=[0,11,12,13,20,22,23,24,25,41,43,57,58,76,77,78,82,83,84,92,93,99,100,101,102,103,104,106,107,109,128,255]
for op,actor,kind in itertools.product([4,5,6],[1,257,1999],types):case(p0=op,p18=actor,p22=kind)
for kind in range(110):case(p0=4,p18=257,p22=kind,p23=0)
for op,mode,links,predicate in itertools.product([4,5,6],[0,49,51],[1,2],[0,1,2]):case(p0=op,p18=1999,p19=0,p22=82 if op==4 else 93 if op==5 else 22,p8=mode,p21=links,p42=predicate,p31=3)
for mode,footprint,result in itertools.product(range(53),range(7),[-1,11]):case(p0=1,p8=mode,p9=8,p13=footprint,p30=result)
for kind in list(range(237))+[-1,237,65535]:
 case(p0=7,p15=kind);case(p0=1,p9=8,p15=kind,p13=kind%7,p16=0,p17=kind&1)
for kind,category in itertools.product([-1,0,5,236,237,65535],range(4)):case(p0=7,p15=kind,p37=category)
flags=[0,1,2,4,8,16,32,64,256,512,2048,0x4000,0x10000,0x40000,0x80000,0x2000000,0x10000000,0x40000000,0x40000044,0xc04000,0x7fffffff]
for flag,variation,extra,format in itertools.product(flags,[0,3,255],[0,1],[0,2]):case(p9=flag,p10=variation,p11=extra,p6=format,p14=0)
for mode,footprint,kind,mutations in itertools.product([0,49,51],[0,1,5,6],[5,31,90,257],[0,5]):case(p8=mode,p9=0x48,p13=footprint,p15=kind,p31=mutations)
for kind,format,pixel_mode,mode in itertools.product([0,11,43,82,20,93,22,128],[0,2],[0,1,2],[0,51]):case(p18=257,p22=kind,p6=format,p7=pixel_mode,p8=mode,p23=0xffff)
for wh,lookup_mode,bounds in itertools.product([(1,1),(3,2),(17,11),(-4,-8)],range(4),range(4)):case(p4=wh[0],p5=wh[1],p32=lookup_mode,p24=bounds,p14=0)
for visibility,show,sidebar,screen in itertools.product([0,1,2],[0,1],[0,1],[0,1]):case(p40=visibility,p29=show,p38=sidebar,p39=screen,p31=8)
for mode in [0,49]:case(p4=73,p5=111,p14=0,p8=mode,p9=0x4000,p32=2)
for repeat in [1,2]:case(p36=repeat,p18=257,p19=0,p21=2,p22=22,p31=3,p42=2)
for marker,edge,terrain,footprint in itertools.product([1,255],[0,64,255],[0,8,0x48],[0,0x15,0xf5,0xff]):
 case(p0=1,p16=marker,p12=edge,p9=terrain,p13=footprint,p15=65535,p17=65535)
for marker,edge,terrain,footprint in itertools.product([0,1,255],[0,64],[8,0x10000008],[0,0xf5]):
 case(p0=0,p16=marker,p12=edge,p9=terrain,p13=footprint,p15=257,p17=0x8000,p36=2)
cases=list(dict.fromkeys(cases))
exe=ROOT/'build/c-only-debug/Debug/restore_minimap_verify.exe'
process=subprocess.Popen([str(exe),'--oracle-batch'],cwd=ROOT,stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True)
report={'format':1,'original_game_executed':False,'native_sha256':digest,'compiled_C_sha256':sha(exe.read_bytes()),'method':'Complete 0043e810 scan, 00439980 overlay dispatch, 0043e670 geometry, three linked-actor classifiers, 0043e770 category lookup and actual 0053f4a0/0040fbe0/0040fc10 pixel writes versus C. Full watched memory/framebuffer/guards, ordered endpoint arguments/intermediate state, return EAX, stack and saved registers compared.','limitations':['The 31 overlay metric functions, 004387d0 deep actor predicate, large-sprite drawing and camera-outline drawing are controlled endpoints; their deep algorithms are not proved. 00434820/004387d0 endpoint results stay within their native Boolean domains.','Allocated tables, acyclic positive actor links, building IDs 0..3999, mapped tile IDs 0..51983/sentinel and a padded destination buffer. Loaded fixture images cover 0..4096 and 25499. Invalid initial image IDs and null/negative pixel pointers exercise native rejection.','This comparison does not establish a completed city frame, all menu branches or independent gameplay.'],'checked':0}
checked=0;p=None
try:
 assert json.loads(process.stdout.readline())=={'fields':44}
 for p in cases:
  process.stdin.write(' '.join(map(str,p))+'\n');process.stdin.flush();line=process.stdout.readline();assert line,'C exited: '+process.stderr.read()
  actual=json.loads(line);expected=native(p)
  assert actual==expected,repr({'regions':[hex(regions[i][0]) for i,(a,b) in enumerate(zip(actual['hashes'],expected['hashes'])) if a!=b],'result':[actual['result'],expected['result']],'event_counts':[len(actual['events']),len(expected['events'])],'first_event_difference':[(i,a,b) for i,(a,b) in enumerate(zip(actual['events'],expected['events'])) if a!=b][:1]})
  checked+=1
  if checked%100==0:print(json.dumps({'checked':checked,'total':len(cases)}),flush=True)
 assert hits==set(branches.values()),'Missing probes '+repr(set(branches.values())-hits)
 report.update(passed=True,checked=checked)
except Exception as error:report.update(passed=False,checked=checked,case=p,error=str(error),native_eip=hex(vm.reg_read(UC_X86_REG_EIP)))
finally:process.stdin.close();process.wait(timeout=10)
sources=['include/minimap.h','include/sprite_render.h','include/sprite_libraries.h','tests/minimap.c','tools/verify_minimap_emulation.py','metadata/minimap-helpers-native.json','metadata/water-ground-render-next-native.json']
sources.extend('src/unit_'+n+'.c' for n in ['00129','00130','00563','00576','00577','00578','00579','00580','00581','02066','02526'])
report['source_hashes']={s:sha((ROOT/s).read_bytes()) for s in sources};report['observed_native_branches']=sorted(hits)
(ROOT/'metadata/minimap-emulation.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v for k,v in report.items() if k!='source_hashes'},indent=2));sys.exit(0 if report['passed'] else 1)
