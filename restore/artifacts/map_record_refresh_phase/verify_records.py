"""Two complete record-refresh functions; original instructions only in Unicorn."""
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
functions={e['requested_address'].lower():e for e in dossier['entries'] if e['requested_address'].lower() in ['004425c0','004426c0']}
assert len(functions)==2;instructions={}
for name,e in functions.items():
 assert e['status']=='exact_ghidra_function' and not e['assembly']['has_more']
 for it in e['assembly']['items']:
  a=int(it['address'],16);b=bytes.fromhex(it['bytes']);assert bytes(vm.mem_read(a,len(b)))==b;instructions[a]=name
assert [len(functions[n]['assembly']['items']) for n in ['004425c0','004426c0']]==[38,35]
jump_table=bytes(vm.mem_read(0x442644,28));assert struct.unpack('<7I',jump_table)==(0x4425e2,0x4425ec,0x44263b,0x4425f8,0x442601,0x44260b,0x442623)
regions=[(0x77f738,19632),(0x784b52,32),(0xf56434,0x540),(0x5d2a28,2080),(0x10d69fc,24000*64+32),(0x126516c,32)]
STACK=0x34000000;STOP=0x33000000;vm.mem_map(STACK,0x100000);vm.mem_map(STOP,4096)
read=lambda a,n=4:int.from_bytes(vm.mem_read(a,n),'little')
def put(a,v,n=4):vm.mem_write(a,(v&((1<<(8*n))-1)).to_bytes(n,'little'))
events=[];hits=set();running=False;helper_calls=0;lookup_cache={}
def code(cpu,a,n,user):
 global helper_calls
 if not running or a==STOP:return
 assert a in instructions,hex(a);hits.add(a)
 if a==0x4425c0:events.append((2,read(cpu.reg_read(UC_X86_REG_ESP)+4),0,0));helper_calls+=1
def memory(cpu,access,a,n,value,user):
 if not running or STACK<=a<STACK+0x100000:return
 if access==UC_MEM_READ and 0x442644<=a and a+n<=0x442660:return
 assert any(lo<=a and a+n<=lo+size for lo,size in regions),(hex(cpu.reg_read(UC_X86_REG_EIP)),hex(a),n)
 events.append((int(access==UC_MEM_WRITE),a,n,(value if access==UC_MEM_WRITE else read(a,n))&((1<<(n*8))-1)))
vm.hook_add(UC_HOOK_CODE,code);vm.hook_add(UC_HOOK_MEM_READ,memory);vm.hook_add(UC_HOOK_MEM_WRITE,memory)
def prepare(p):
 for i,(a,n) in enumerate(regions):vm.mem_write(a,bytes([(p[13]+37*i)&255])*n)
 for i in range(200):
  r=0x77f748+i*98;active=0;kind=1
  if p[12]==1:active=1
  if p[12]==2:active=i%3;kind=i%4
  if p[12]==3:active=int(i==p[1])
  if p[12]==4:active=int(i in [0,199])
  selector=(p[2]+i)%7 if p[14]==1 else (p[2]+i)&255 if p[14]==2 else p[2]
  put(r,kind,1);put(r+1,active,1);put(r+24,selector,1);put(r+25,(p[3]+i*73)&255,1)
 put(0x77f748+p[1]*98+24,p[2],1);put(0x77f748+p[1]*98+25,p[3],1)
 for a,v,n in [(0x784b62,p[4],2),(0xf56444,p[5],4),(0xf56954,p[6],4),(0xf5693c,p[7],4),(0xf56964,p[8],4),(0x126517c,p[11],4)]:put(a,v,n)
 for i in range(256):put(0x5d2a38+i*8,p[9]+i*p[10])
 if p[13] not in lookup_cache:
  raw=bytearray(bytes([(p[13]+37*4)&255])*(24000*64+32))
  for i in range(24000):struct.pack_into('<HH',raw,16+i*64,((i*313+17)^p[13])&65535,((i*701+0x8001)^p[13])&65535)
  lookup_cache[p[13]]=bytes(raw)
 vm.mem_write(0x10d69fc,lookup_cache[p[13]])
def native(p):
 global events,running
 prepare(p);sp=STACK+0xf0000;put(sp,STOP);canary=sp+(8 if p[0]==0 else 4);put(canary,0xbaadf00d)
 if p[0]==0:put(sp+4,p[1])
 saved=[(UC_X86_REG_EBX,0x11111111),(UC_X86_REG_EBP,0x22222222),(UC_X86_REG_ESI,0x33333333),(UC_X86_REG_EDI,0x44444444)]
 for r,v in saved:vm.reg_write(r,v)
 for r,v in [(UC_X86_REG_EAX,0xdeadbeef),(UC_X86_REG_ECX,0xbadc0de),(UC_X86_REG_EDX,0xfefefefe),(UC_X86_REG_ESP,sp),(UC_X86_REG_EFLAGS,2)]:vm.reg_write(r,v)
 events=[];running=True
 try:vm.emu_start(0x4425c0 if p[0]==0 else 0x4426c0,STOP,count=100000)
 finally:running=False
 assert vm.reg_read(UC_X86_REG_EIP)==STOP and vm.reg_read(UC_X86_REG_ESP)==sp+4 and read(canary)==0xbaadf00d and all(vm.reg_read(r)==v for r,v in saved)
 memory=hashlib.sha256()
 for a,n in regions:memory.update(struct.pack('<II',a,n));memory.update(vm.mem_read(a,n))
 trace=hashlib.sha256()
 for e in events:trace.update(struct.pack('<4I',*e))
 return {'result':vm.reg_read(UC_X86_REG_EAX) if p[0]==0 else 0,'memory':memory.hexdigest(),'trace':trace.hexdigest(),'events':len(events)}
DEFAULT=[0,0,0,0,10,100,200,300,400,7,3,256,1,17,0,0];cases=[];categories=[]
def add(category,**kw):
 p=DEFAULT.copy()
 for k,v in kw.items():p[int(k[1:])]=v&MASK
 cases.append(p);categories.append(category)
for selector,index,subtype in itertools.product([0,1,2,3,4,5,6,255],[0,1,127,199],[0,255]):add('leaf_switch_stride_subtype',p1=index,p2=selector,p3=subtype)
for state in [0,9,10,11,0x010a,0xa00,0xffff]:add('WORD_state',p4=state)
for selector,values in itertools.product(range(7),[(0,0,0,0),(0xffffffff,0x80000000,0x7fffffff,0xfffffffe),(65535,65536,65537,23999),(0x12345678,0x9abcdef0,0xfedcba98,0x87654321)]):add('full_EAX_modular_DWORD',p2=selector,p3=255,p5=values[0],p6=values[1],p7=values[2],p8=values[3],p9=0xfffffff0,p10=0x10001)
for pattern,policy,selector in itertools.product(range(5),range(3),[0,6,255]):add('actual_200_record_loop_gates',p0=1,p1=199,p2=selector,p3=255,p12=pattern,p14=policy)
for value,limit in itertools.product([0,1,255,256,257,23999,24000,65535,65536,0xffffffff,0x80000000],[0,1,256,24000,65536,0x80000000,0xffffffff]):
 low=value&65535
 if limit<0x80000000 and low<limit and low>=24000:continue
 add('low_AX_signed_DWORD_limit_and_stride64',p0=1,p2=1,p6=value,p11=limit)
for selector in range(7):add('whole_modular_asset_addition',p0=1,p2=selector,p3=255,p5=0xfffffffe,p8=0xfffffffc,p9=0xfffffff0,p10=0x10001,p11=256,p14=0)
complete=len(cases)
if args.limit:cases=cases[:args.limit];categories=categories[:args.limit]
provided=[args.exe,args.plain_exe,args.optimized_exe,args.optimized_plain_exe]
if args.production:assert all(provided) and not args.limit
exes=[(p or HERE/'build'/config/f'map_record_{kind}.exe').resolve() for p,(config,kind) in zip(provided,itertools.product(['Debug','Release'],['trace','plain']))]
files=[Path(__file__),cache,HERE/'CMakeLists.txt',HERE/'record_fixture.c',HERE/'map_record_refresh.h',HERE/'unit_00606.c',HERE/'unit_00608.c',ROOT/'include/standalone_data.h',ROOT/'build/c-only-debug/Debug/restore_data.lib']
if args.production:
 assert args.proof_sha256 and sha(HERE/'record-proof.json')==args.proof_sha256,'exact immutable private proof SHA required'
 for name in ['unit_00606.c','unit_00608.c']:assert sha(HERE/name)==sha(ROOT/'src'/name)
 assert sha(HERE/'map_record_refresh.h')==sha(ROOT/'include/map_record_refresh.h')
 files.extend([ROOT/'src/unit_00606.c',ROOT/'src/unit_00608.c',ROOT/'include/map_record_refresh.h'])
 expected={p.resolve() for p in [ROOT/'src/unit_00606.c',ROOT/'src/unit_00608.c',HERE/'record_fixture.c']}
 for k,exe in enumerate(exes):
  target='restore_map_record_refresh_'+('trace' if k%2==0 else 'plain')+'_verify'
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
 for symbol,owner in [('_FUN_004425c0','unit_00606.obj'),('_FUN_004426c0','unit_00608.obj')]:
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
report={'passed':True,'production':args.production,'checked':len(cases),'comparisons':len(cases)*4,'full_corpus_cases':complete,'coverage':coverage,'helper_calls':helper_calls,'native_sha256':PIN,'native_exports':{rel(cache):sha(cache),'switch_00442644_sha256':hashlib.sha256(jump_table).hexdigest()},'compiled_C_sha256':bins[rel(exes[0])],'variants':{name:{'path':rel(p),'sha256':bins[rel(p)],'checked':len(cases)} for name,p in zip(['Debug_trace','Debug_plain','O2_trace','O2_plain'],exes)},'source_hashes':before,'source_drift':[rel(p) for p in files if sha(p)!=before[rel(p)]],'binary_drift':[rel(p) for p in exes if sha(p)!=bins[rel(p)]],'corpus_sha256':hashlib.sha256(json.dumps(cases,separators=(',',':')).encode()).hexdigest(),'elapsed_seconds':round(time.monotonic()-start,2),'apply':{'src/unit_00606.c':{'before_sha256':'52d3e2bccd385d8707ad951fe5bf1cfbc814370e95be50ce846ce82e84f1758f','after_sha256':sha(HERE/'unit_00606.c')},'src/unit_00608.c':{'before_sha256':'81ffd6579cbb042a336d34be8c287c88dacbec7e4725a6dc1b8ce873a745154d','after_sha256':sha(HERE/'unit_00608.c')},'include/map_record_refresh.h':{'before_sha256':None,'after_sha256':sha(HERE/'map_record_refresh.h')}},'limits':['Complete two-function C closure, actual helper, no external or fake boundary. Native bytes and seven-entry switch table authenticated against pinned PE. No original process or GUI.','Helper one DWORD cdecl argument/full EAX; refresh zero-argument void; callee-saved GPRs, caller stack/canary, entire six regions with neighboring guards, ordered reads/writes and helper entries compared. Flags and caller-saved registers are not an ABI promise.','Representative record indices0/1/127/199; every switch arm and out-of-range BYTE default; subtypes0/255 plus generated whole-loop subtypes; WORD states and DWORD carry/wrap examples. Lookup domain uses disjoint mapped entries0..23999; unmapped accesses excluded. No exhaustive all-state claim.',('Private /O2 objects use Debug data library/CRT; normal product application and replay remain separate.' if not args.production else 'Shared verification Release objects /O2 with matching root data/CRT; normal root product /Od remains separately qualified.')+'  C# only confirms a post-load call, no semantic naming inferred. REA had no open target; existing complete Ghidra cache and pinned PE used instead of repeating timed-out import.']}
assert not report['source_drift'] and not report['binary_drift'];output=ROOT/'metadata/map-record-refresh-emulation.json' if args.production else HERE/('record-partial.json' if args.limit else 'record-proof.json');output.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:report[k] for k in ['passed','checked','comparisons','coverage','helper_calls','elapsed_seconds']}))
