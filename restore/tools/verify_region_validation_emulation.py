"""Four complete terrain-region validators versus pinned original instructions."""
from pathlib import Path
import argparse,bisect,functools,hashlib,itertools,json,random,struct,subprocess,sys,time
ROOT=next(p for p in Path(__file__).resolve().parents if (p/'build/smoke-game/Pharaoh.exe').exists() and (p/'src').is_dir());PHASE=ROOT/'artifacts/region_validation_phase'
sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_BLOCK,UC_HOOK_CODE,__version__
from unicorn.x86_const import *
parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--production',action='store_true');parser.add_argument('--exe',type=Path,default=ROOT/'build/agent-region-validation/Debug/restore_region_validation_verify.exe');args=parser.parse_args();exe=args.exe.resolve();OWN=ROOT if args.production else PHASE
NATIVE=ROOT/'metadata/region-validation-native.json' if args.production else PHASE/'metadata/native.json';REPORT=ROOT/'metadata/region-validation-emulation.json' if args.production else PHASE/'metadata/emulation.json'
sha=lambda b:hashlib.sha256(b).hexdigest();MASK=0xffffffff
image=(ROOT/'build/smoke-game/Pharaoh.exe').read_bytes();SHA=sha(image);assert SHA=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
d=json.loads(NATIVE.read_text());assert d['native_sha256']==SHA
functions={int(a,16):f['assembly']['items'] for a,f in d['functions'].items()}
for f in d['functions'].values():assert not f['assembly']['has_more'] and len(f['assembly']['items'])==f['assembly']['total']
rules=json.loads((ROOT/'metadata/placement-preview-rule-predicates-native.json').read_text());assert rules['native_sha256']==SHA
for a,f in rules['functions'].items():assert f['complete_reachable_CFG'];functions[int(a,16)]=f['instructions']
u16=lambda p:struct.unpack_from('<H',image,p)[0];u32=lambda p:struct.unpack_from('<I',image,p)[0];pe=u32(0x3c);opt=pe+24;table=opt+u16(pe+20);base=u32(opt+28);size=u32(opt+56)
vm=Uc(UC_ARCH_X86,UC_MODE_32);vm.mem_map(base,(size+4095)&~4095);vm.mem_write(base,image[:u32(opt+60)])
for k in range(u16(pe+6)):
 q=table+k*40;vm.mem_write(base+u32(q+12),image[u32(q+20):u32(q+20)+u32(q+16)])
GRID=0xaf598c;CELLS=0x20000;FIRST=GRID-64;GRID_BYTES=CELLS*4+128;ROW=0xd3b4e0;COL=0xd3b4e4;STOP=0x33000000;STACK=0x34000000;CALLBACK=0x35000000
for a,n in [(STOP,4096),(STACK,0x10000),(CALLBACK,4096)]:vm.mem_map(a,n)
regions=[(FIRST,GRID_BYTES),(0xbca2c0,64),(0xd3b4d0,48)];allowed=sorted(int(i['address'],16) for f in functions.values() for i in f);allowed_set=set(allowed)
for f in functions.values():
 for i in f:
  raw=bytes.fromhex(i['bytes']);assert bytes(vm.mem_read(int(i['address'],16),len(raw)))==raw
read=lambda a:int.from_bytes(vm.mem_read(a&MASK,4),'little')
def write(a,v):vm.mem_write(a&MASK,(v&MASK).to_bytes(4,'little'))
state={};hits=set();cache={}
def block(cpu,a,n,user):
 if a==CALLBACK:return
 assert a in allowed_set,'Unexpected native block '+hex(a)
 if (a,n) not in cache:cache[a,n]=allowed[bisect.bisect_left(allowed,a):bisect.bisect_left(allowed,a+n)]
 hits.update(cache[a,n])
def callback_enter(terrain):
 p=state['p'];events=state['events'];e=[terrain,read(ROW),read(COL),read(0xbca2d0),read(0xbca2d4),read(0xbca2d8),0,len(events)+1];events.append(e)
 if len(events)==1:
  if p[16]&1:write(COL,p[17])
  if p[16]&2:write(ROW,p[18])
  if p[16]&4:write(0xbca2d0,0x12345678);write(0xbca2d4,0x87654321);write(0xbca2d8,read(0xbca2d8)^0x40010000)
  if p[16]&8:write(GRID+(p[12]%CELLS)*4,p[14]^0x10001)
 return e
rule_entries=[0x405b40,0x405b60,0x405b80];rule_returns={int(i['address'],16) for a in rule_entries for i in functions[a] if i['instruction'].strip().startswith('ret')}
def callback_hook(cpu,a,n,user):
 sp=cpu.reg_read(UC_X86_REG_ESP)
 if a in rule_entries:callback_enter(read(sp+4))
 elif a in rule_returns:state['events'][-1][6]=cpu.reg_read(UC_X86_REG_EAX)&255
 else:
  e=callback_enter(read(sp+4));p=state['p'];result=p[14] if not p[15] or len(state['events'])==p[15] else 0;e[6]=result&255
  cpu.reg_write(UC_X86_REG_EAX,result);cpu.reg_write(UC_X86_REG_ECX,0xa17e4567);cpu.reg_write(UC_X86_REG_EDX,0x5a5a5a5a);cpu.reg_write(UC_X86_REG_ESP,sp+4);cpu.reg_write(UC_X86_REG_EIP,read(sp))
vm.hook_add(UC_HOOK_BLOCK,block)
for a in [*rule_entries,*rule_returns,CALLBACK]:vm.hook_add(UC_HOOK_CODE,callback_hook,begin=a,end=a)
@functools.lru_cache(maxsize=64)
def grid(seed,pattern,mask,hit):
 b=bytearray(((i*13+(i>>4)*11+seed)^0xb7)&255 for i in range(GRID_BYTES))
 for i in range(CELLS):
  if pattern==0:v=0
  elif pattern==1:v=mask
  elif pattern==2:v=((i*0x9e3779b9+seed)&MASK)^(i>>3)
  elif pattern==3:v=mask if (i%228+i//228)&1 else 0
  elif pattern==4:v=mask if i==hit else 0
  elif pattern==5:v=mask if i<hit else 0
  else:v=seed
  struct.pack_into('<I',b,64+i*4,v)
 return bytes(b)
def native(p):
 state.clear();state.update(p=p,events=[]);vm.mem_write(FIRST,grid(p[1],p[11],p[9],p[12]));vm.mem_write(0xbca2c0,bytes(((i*37+p[1])^0x5d)&255 for i in range(64)));vm.mem_write(0xd3b4d0,bytes(((i*23+p[1])^0xc3)&255 for i in range(48)))
 for a,v in [(0xbca2d0,p[2]),(0xbca2d4,p[3]),(0xbca2d8,p[4])]:write(a,v)
 callback=rule_entries[p[13]] if p[13]<3 else CALLBACK;arg5=callback if p[0]==1 else p[9];sp=STACK+0x8000;vm.mem_write(sp-0x200,b'\xcc'*0x400)
 for i,v in enumerate([STOP,p[5],p[6],p[7],p[8],arg5,p[10]]):write(sp+i*4,v)
 saved=[(UC_X86_REG_EBX,0x11223344),(UC_X86_REG_EBP,0x22334455),(UC_X86_REG_ESI,0x33445566),(UC_X86_REG_EDI,0x44556677)]
 for r,v in saved+[(UC_X86_REG_ESP,sp),(UC_X86_REG_EFLAGS,2)]:vm.reg_write(r,v)
 vm.emu_start([0x482650,0x482720,0x4828e0,0x4829b0][p[0]],STOP,count=1000000)
 assert vm.reg_read(UC_X86_REG_EIP)==STOP and vm.reg_read(UC_X86_REG_ESP)==sp+4,'Native stack/budget'
 assert all(vm.reg_read(r)==v for r,v in saved),'Native callee saved registers'
 assert bytes(vm.mem_read(sp+28,128))==b'\xcc'*128,'Caller stack guard'
 result=vm.reg_read(UC_X86_REG_EAX);assert result in [0,1]
 h=hashlib.sha256()
 for a,n in regions:h.update(struct.pack('<II',a,n));h.update(vm.mem_read(a,n))
 return struct.pack('<I',result)+h.digest()+struct.pack('<I',len(state['events']))+b''.join(struct.pack('<8I',*e) for e in state['events'])
default=[0,123,228,228,16,3,4,3,1,0x10000,1,2,0,3,0x100,0,0,0,0,0];cases=[]
def add(**kw):
 p=default.copy()
 for k,v in kw.items():p[int(k[1:])]=v&MASK
 cases.append(tuple(p))
for op,pattern,mask,x,y in itertools.product(range(4),[0,1,2,3],[0,2,0x40,0x10000,0x20300002],[-2,0,3,226,229],[-2,0,3,226,229]):add(p0=op,p11=pattern,p9=mask,p5=x,p6=y,p13=3,p14=0x100)
for op,width,height,origin in itertools.product(range(4),[0,1,255,256,400,0x7fffffff,0xffffffff],[0,1,256,400],[0,65536,0x40000000,0xfffffff8]):add(p0=op,p2=width,p3=height,p4=origin,p5=256,p6=1,p7=2,p8=0,p11=1,p9=0x10000)
for op,size,radius in itertools.product(range(4),[-4,0,1,2,6,17],[-3,-1,0,1,3]):add(p0=op,p7=size,p8=radius,p11=3,p14=0x100)
for op,x,y in itertools.product(range(4),[0x80000000,0x7fffffff,-1,0,256],[0x80000000,0x7fffffff,-1,0,256]):add(p0=op,p5=x,p6=y,p7=2,p8=0,p11=0)
for required,pattern,mask in itertools.product([0,1,2,4,16,-1,0x80000000,0x7fffffff],[0,1,3],[0,1,0x10000]):add(p0=3,p10=required,p11=pattern,p9=mask)
for kind,pattern,mask in itertools.product(range(3),[0,1,2,3],[2,0x100002,0x20000002,0x20300002]):add(p0=1,p13=kind,p11=pattern,p9=mask)
for ret,at in itertools.product([0,1,0x100,0x101,0xffffff00,0xffffffff,0x80000000],[0,1,2,5,25]):add(p0=1,p13=3,p14=ret,p15=at,p11=2)
for kind,flags,column,row,ret in itertools.product([0,1,2,3],[1,2,4,8,3,5,10,15],[2,7],[3,8],[0x100,0x101]):add(p0=1,p13=kind,p16=flags,p17=column,p18=row,p14=ret,p15=2,p7=3,p8=0,p11=3,p12=16+4*228+4)
# Deduplicate and group by immutable grid background for repeatable fast preparation.
cases=sorted(set(cases),key=lambda p:(p[1],p[11],p[9],p[12],p))
process=subprocess.Popen([str(exe),'--oracle-batch'],cwd=ROOT,stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE)
report={'format':1,'passed':False,'checked':0,'production':args.production,'native_sha256':SHA,'compiled_C_sha256':sha(exe.read_bytes()),'original_game_executed':False,'unicorn_version':__version__,
 'method':'Four complete original region validators and three actual rule predicates versus handwritten C with the unchanged accepted resolver/rules. Full terrain grid/global bytes and guards are SHA256 compared; ordered callback terrain/row/column/dimensions/origin/AL results, live counter mutations, cached bounds, raw EAX0/1, stack and saved registers checked.',
 'limitations':['Callback mutation/return probes are controlled C/native endpoints. The three canonical rule predicates execute as real original and C instructions; no other algorithms are controlled.','Mapped, bounded scans are exercised. Native malformed dimensions/counters can wrap into unbounded traversal; such nonterminating/out-of-mapping cases are excluded without production bounds checks.','Callbacks must be real compiled C callables, such as pointers supplied by the accepted restore_placement_rule resolver. This suite does not repair other legacy token-producing callers, input, placement policy or rendering.']}
def exact(n):
 b=process.stdout.read(n);assert len(b)==n,'C stopped: '+process.stderr.read().decode(errors='replace');return b
started=time.monotonic();p=None
try:
 assert exact(4)==struct.pack('<I',0x5245474e)
 for p in cases:
  process.stdin.write((' '.join(map(str,p))+'\n').encode());process.stdin.flush();header=exact(40);count=struct.unpack_from('<I',header,36)[0];actual=header+exact(count*32);expected=native(p)
  assert actual==expected,repr({'case':p,'C_header':actual[:40].hex(),'native_header':expected[:40].hex(),'first_difference':next((i for i,(a,b) in enumerate(zip(actual,expected)) if a!=b),None),'C_events':[list(struct.unpack_from('<8I',actual,40+i*32)) for i in range(count)],'native_events':state['events']})
  report['checked']+=1
  if report['checked']%500==0:print(json.dumps({'checked':report['checked'],'total':len(cases)}),flush=True)
 assert hits==allowed_set,'Uncovered native instructions: '+repr([hex(a) for a in sorted(allowed_set-hits)])
 report['passed']=True
except Exception as e:report.update(error=str(e),case=p,native_eip=hex(vm.reg_read(UC_X86_REG_EIP)))
finally:process.stdin.close();process.wait(timeout=10)
sources=[OWN/f'src/unit_{n:05d}.c' for n in [1202,1203,1205,1206]]+[OWN/'include/region_validation.h',OWN/'tests/region_validation.c',Path(__file__),NATIVE,PHASE/'CMakeLists.txt',ROOT/'runtime/placement_preview_predicates.c',ROOT/'include/placement_preview.h',ROOT/'metadata/placement-preview-rule-predicates-native.json']
report['source_hashes']={str(p.resolve().relative_to(ROOT)).replace('\\','/'):sha(p.read_bytes()) for p in sources}
report['instruction_coverage']={hex(a):{'executed':sum(int(i['address'],16) in hits for i in f),'total':len(f),'missing':[i['address'] for i in f if int(i['address'],16) not in hits]} for a,f in functions.items()};report['instructions_executed']=len(hits);report['instructions_total']=len(allowed);report['watched_grid_cells']=CELLS;report['elapsed_seconds']=round(time.monotonic()-started,3)
REPORT.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v for k,v in report.items() if k not in ['source_hashes','instruction_coverage']},indent=2));sys.exit(0 if report['passed'] else 1)
