"""Whole map-menu direction handler and its actual coordinate-clamp leaf."""
from pathlib import Path
import argparse,hashlib,itertools,json,struct,subprocess,sys,time,xml.etree.ElementTree as ET
HERE=Path(__file__).resolve().parent;ROOT=HERE.parent.parent
sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from unicorn import *
from unicorn.x86_const import *
ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--limit',type=int);ap.add_argument('--production',action='store_true');ap.add_argument('--proof-sha256')
for name in ['exe','plain-exe','optimized-exe','optimized-plain-exe']:ap.add_argument('--'+name,type=Path)
args=ap.parse_args();sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest();MASK=0xffffffff
PIN='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
original=ROOT/'build/smoke-game/Pharaoh.exe';assert sha(original)==PIN;image=original.read_bytes()
u16=lambda p:struct.unpack_from('<H',image,p)[0];u32=lambda p:struct.unpack_from('<I',image,p)[0]
pe=u32(60);opt=pe+24;table=opt+u16(pe+20);base=u32(opt+28)
vm=Uc(UC_ARCH_X86,UC_MODE_32);vm.mem_map(base,(u32(opt+56)+4095)&~4095)
for i in range(u16(pe+6)):
 s=table+40*i;vm.mem_write(base+u32(s+12),image[u32(s+20):u32(s+20)+u32(s+16)])
cache=ROOT/'metadata/native-function-audit-cache/batch-0576.json';dossier=json.loads(cache.read_text());assert dossier['native_sha256']==PIN
functions={e['requested_address'].lower():e for e in dossier['entries'] if e['requested_address'].lower() in ['00442730','00442800']}
assert len(functions)==2;instructions={}
for name,e in functions.items():
 assert e['status']=='exact_ghidra_function' and not e['assembly']['has_more']
 for it in e['assembly']['items']:
  a=int(it['address'],16);b=bytes.fromhex(it['bytes']);assert bytes(vm.mem_read(a,len(b)))==b;instructions[a]=name
assert [len(functions[n]['assembly']['items']) for n in ['00442730','00442800']]==[54,32]
regions=[(0x7758fc,40),(0x5d2a00,48),(0xe38d88,40)]
STACK=0x34000000;STOP=0x33000000;vm.mem_map(STACK,0x100000);vm.mem_map(STOP,4096)
read=lambda a,n=4:int.from_bytes(vm.mem_read(a,n),'little')
def put(a,v,n=4):vm.mem_write(a,(v&((1<<(8*n))-1)).to_bytes(n,'little'))
events=[];hits=set();running=False;helper_calls=0
def code(cpu,a,n,user):
 global helper_calls
 if not running or a==STOP:return
 assert a in instructions,hex(a);hits.add(a)
 if a==0x442800:events.append((2,a,0,0));helper_calls+=1
def memory(cpu,access,a,n,value,user):
 if not running or STACK<=a<STACK+0x100000:return
 assert any(lo<=a and a+n<=lo+size for lo,size in regions),(hex(cpu.reg_read(UC_X86_REG_EIP)),hex(a),n)
 events.append((int(access==UC_MEM_WRITE),a,n,(value if access==UC_MEM_WRITE else read(a,n))&((1<<(n*8))-1)))
vm.hook_add(UC_HOOK_CODE,code);vm.hook_add(UC_HOOK_MEM_READ,memory);vm.hook_add(UC_HOOK_MEM_WRITE,memory)
def prepare(p):
 for i,(a,n) in enumerate(regions):vm.mem_write(a,bytes([(p[11]+37*i)&255])*n)
 put(0x77590c,p[2]);put(0x775910,p[3])
 for i in range(5):put(0x5d2a10+i*4,p[4+i])
 put(0xe38d98,p[9]);put(0xe38d9c,p[10])
def native(p):
 global events,running
 prepare(p);sp=STACK+0xf0000;put(sp,STOP);canary=sp+(8 if p[0]==0 else 4);put(canary,0xbaadf00d)
 if p[0]==0:put(sp+4,p[1])
 saved=[(UC_X86_REG_EBX,0x11111111),(UC_X86_REG_EBP,0x22222222),(UC_X86_REG_ESI,0x33333333),(UC_X86_REG_EDI,0x44444444)]
 for r,v in saved:vm.reg_write(r,v)
 for r,v in [(UC_X86_REG_EAX,0xdeadbeef),(UC_X86_REG_ECX,0xbadc0de),(UC_X86_REG_EDX,0xfefefefe),(UC_X86_REG_ESP,sp),(UC_X86_REG_EFLAGS,2)]:vm.reg_write(r,v)
 events=[];running=True
 try:vm.emu_start(0x442730 if p[0]==0 else 0x442800,STOP,count=100000)
 finally:running=False
 assert vm.reg_read(UC_X86_REG_EIP)==STOP and vm.reg_read(UC_X86_REG_ESP)==sp+4 and read(canary)==0xbaadf00d and all(vm.reg_read(r)==v for r,v in saved)
 memory=hashlib.sha256()
 for a,n in regions:memory.update(struct.pack('<II',a,n));memory.update(vm.mem_read(a,n))
 trace=hashlib.sha256()
 for e in events:trace.update(struct.pack('<4I',*e))
 return {'result':0,'memory':memory.hexdigest(),'trace':trace.hexdigest(),'events':len(events)}
#mode/direction/coordinateA/coordinateB/extentB/extentA/padB/padA1/padA2/screenA/screenB/guard-seed/reserved*4
DEFAULT=[0,0,100,100,2000,1600,10,20,30,800,1024,17,0,0,0,0];cases=[];categories=[]
def add(category,**kw):
 p=DEFAULT.copy()
 for k,v in kw.items():p[int(k[1:])]=v&MASK
 cases.append(p);categories.append(category)
positions=[(-1,-1),(0,0),(19,19),(20,20),(100,100),(849,995),(850,996),(0x7fffffff,0x80000000),(0x80000000,0x7fffffff),(0xfffffff0,0xfffffffe)]
for direction,(a,b) in itertools.product([0,1,2,3,4,5,6,7,8,9,0xffffffff,0x80000000],positions):add('actual_handler_and_clamp',p1=direction,p2=a,p3=b)
layouts=[(2000,1600,10,20,30,800,1024),(0,0,0,0,0,0,0),(1,1,0,0,0,100,100),(0xffffffff,0xffffffff,1,1,1,1,1),(0x7fffffff,0x7fffffff,0x7fffffff,0x80000000,0x80000000,0xffffffff,0xffffffff),(0x80000000,0x80000000,0,0,0,0,0)]
for layout,(a,b) in itertools.product(layouts,positions):
 kw={f'p{4+i}':value for i,value in enumerate(layout)};add('direct_clamp_signed_and_modular_bounds',p0=1,p2=a,p3=b,**kw)
for direction in range(10):add('zero_extents_preserve_negative_upper_result',p1=direction,p2=-10,p3=-20,p4=0,p5=0,p6=0,p7=0,p8=0,p9=0,p10=0,p11=255)
complete=len(cases)
if args.limit:cases=cases[:args.limit];categories=categories[:args.limit]
provided=[args.exe,args.plain_exe,args.optimized_exe,args.optimized_plain_exe]
if args.production:assert all(provided) and not args.limit
exes=[(p or HERE/'build'/config/f'map_scroll_{kind}.exe').resolve() for p,(config,kind) in zip(provided,itertools.product(['Debug','Release'],['trace','plain']))]
files=[Path(__file__),cache,HERE/'CMakeLists.txt',HERE/'record_fixture.c',HERE/'map_menu_scroll.h',HERE/'unit_00609.c',HERE/'unit_00610.c',ROOT/'include/standalone_data.h',ROOT/'build/c-only-debug/Debug/restore_data.lib']
if args.production:
 assert args.proof_sha256 and sha(HERE/'scroll-proof.json')==args.proof_sha256,'exact immutable private proof SHA required'
 for name in ['unit_00609.c','unit_00610.c']:assert sha(HERE/name)==sha(ROOT/'src'/name)
 assert sha(HERE/'map_menu_scroll.h')==sha(ROOT/'include/map_menu_scroll.h')
 files.extend([ROOT/'src/unit_00609.c',ROOT/'src/unit_00610.c',ROOT/'include/map_menu_scroll.h'])
 expected={p.resolve() for p in [ROOT/'src/unit_00609.c',ROOT/'src/unit_00610.c',HERE/'record_fixture.c']}
 for k,exe in enumerate(exes):
  target='restore_map_menu_scroll_'+('trace' if k%2==0 else 'plain')+'_verify'
  assert exe.name==target+'.exe' and HERE not in exe.parents
  project=exe.parent.parent/(target+'.vcxproj');xml=ET.parse(project);ns={'m':'http://schemas.microsoft.com/developer/msbuild/2003'}
  compiles={(project.parent/e.attrib['Include'].replace('\\','/')).resolve() for e in xml.findall('.//m:ClCompile[@Include]',ns)}
  assert compiles==expected,(project,compiles^expected)
  selected=[g for g in xml.findall('.//m:ItemDefinitionGroup',ns) if ('Debug' if k<2 else 'Release') in g.get('Condition','')];assert selected
  for group in selected:
   runtime=group.find('m:ClCompile/m:RuntimeLibrary',ns);assert runtime is not None and runtime.text==('MultiThreadedDebug' if k<2 else 'MultiThreaded')
  data=exe.parent/'restore_data.lib';assert data.is_file();files.extend([project,data])
rel=lambda p:p.relative_to(ROOT).as_posix();before={rel(p):sha(p) for p in files};bins={rel(p):sha(p) for p in exes}
for exe in exes:
 lines=exe.with_suffix('.map').read_text().splitlines()
 for symbol,owner in [('_FUN_00442730','unit_00609.obj'),('_FUN_00442800','unit_00610.obj')]:
  found=[x for x in lines if symbol in x.split()];assert len(found)==1 and found[0].split()[-1]==owner,(exe,symbol,found)
start=time.monotonic();processes=[subprocess.Popen([str(p),'--oracle-batch'],stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True) for p in exes]
try:
 for p in processes:assert json.loads(p.stdout.readline())=={'fields':16}
 for i,p in enumerate(cases):
  expected=native(p)
  for k,proc in enumerate(processes):
   proc.stdin.write(' '.join(map(str,p))+'\n');proc.stdin.flush();line=proc.stdout.readline();assert line,'C exit:'+proc.stderr.read();actual=json.loads(line)
   want=expected if k%2==0 else dict(expected,trace=hashlib.sha256(b'').hexdigest(),events=0)
   assert actual==want,json.dumps({'case':i,'parameters':p,'variant':str(exes[k]),'actual':actual,'expected':want})
finally:
 for p in processes:p.stdin.close();p.wait(timeout=20);assert p.returncode==0,p.stderr.read()
coverage={name:[sum(instructions[a]==name for a in hits),len(f['assembly']['items'])] for name,f in functions.items()}
if not args.limit:assert all(a==b for a,b in coverage.values()),coverage
report={'passed':True,'production':args.production,'checked':len(cases),'comparisons':len(cases)*4,'full_corpus_cases':complete,'coverage':coverage,'helper_calls':helper_calls,'native_sha256':PIN,'native_exports':{rel(cache):sha(cache)},'compiled_C_sha256':bins[rel(exes[0])],'variants':{name:{'path':rel(p),'sha256':bins[rel(p)],'checked':len(cases)} for name,p in zip(['Debug_trace','Debug_plain','O2_trace','O2_plain'],exes)},'source_hashes':before,'source_drift':[rel(p) for p in files if sha(p)!=before[rel(p)]],'binary_drift':[rel(p) for p in exes if sha(p)!=bins[rel(p)]],'corpus_sha256':hashlib.sha256(json.dumps(cases,separators=(',',':')).encode()).hexdigest(),'elapsed_seconds':round(time.monotonic()-start,2),'apply':{'src/unit_00609.c':{'before_sha256':'d6ee8f8dc6062bf1fe275915b9cbab32dd3469add21f045f9cf43f2afc59167d','after_sha256':sha(HERE/'unit_00609.c')},'src/unit_00610.c':{'before_sha256':'418f09f0ec20ebe9e837764e528eb0ea90954229532a9cda08a88693c1c13c88','after_sha256':sha(HERE/'unit_00610.c')},'include/map_menu_scroll.h':{'before_sha256':None,'after_sha256':sha(HERE/'map_menu_scroll.h')}},'limits':['Two complete C functions, actual handler-to-clamp dependency; no additional callees/imports or fake boundary. Original54+32 instructions authenticated against pinned PE.','Handler one cdecl DWORD direction, helper zero arguments, both void; native tail JMP normalized to actual C call. Callee-saved GPRs, caller SP/canary and guarded memory checked. Scratch registers/EFLAGS are not an ABI result.','All ordered DWORD reads/writes and helper entries compared, including dual-coordinate prefetch order. Direction8 performs no access/call; invalid directions still clamp. Lower-then-upper clamp retains negative/zero-bound results and modulo32 arithmetic.','Finite representative positive-layout and synthetic raw DWORD overflow/layout cases; no exhaustive all-layout claim. Private /O2 uses Debug data/CRT; actual product/replay remains separate. No GUI/original process.']}
assert not report['source_drift'] and not report['binary_drift'];output=ROOT/'metadata/map-menu-scroll-emulation.json' if args.production else HERE/('scroll-partial.json' if args.limit else 'scroll-proof.json');output.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:report[k] for k in ['passed','checked','comparisons','coverage','helper_calls','elapsed_seconds']}))
