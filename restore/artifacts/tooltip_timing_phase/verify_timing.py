"""Whole tooltip timing and actual signed-DWORD record counter."""
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
cache=ROOT/'metadata/native-function-audit-cache/batch-0000.json';dossier=json.loads(cache.read_text());assert dossier['native_sha256']==PIN
other_cache=ROOT/'metadata/native-function-audit-cache/batch-0064.json';other=json.loads(other_cache.read_text());assert other['native_sha256']==PIN
functions={e['requested_address'].lower():e for e in dossier['entries']+other['entries'] if e['requested_address'].lower() in ['00405170','00409e40']}
assert len(functions)==2;instructions={}
for name,e in functions.items():
 assert e['status']=='exact_ghidra_function' and not e['assembly']['has_more']
 for it in e['assembly']['items']:
  a=int(it['address'],16);b=bytes.fromhex(it['bytes']);assert bytes(vm.mem_read(a,len(b)))==b;instructions[a]=name
assert [len(functions[n]['assembly']['items']) for n in ['00405170','00409e40']]==[14,29]
regions=[(0x576170,0x120),(0x5cf74c,16*120+32),(0x605780,32),(0x7056b4,0x38),(0xe92dfc,32)]
STACK=0x34000000;STOP=0x33000000;vm.mem_map(STACK,0x100000);vm.mem_map(STOP,4096)
read=lambda a,n=4:int.from_bytes(vm.mem_read(a,n),'little')
def put(a,v,n=4):vm.mem_write(a,(v&((1<<(8*n))-1)).to_bytes(n,'little'))
events=[];hits=set();running=False;helper_calls=0
def code(cpu,a,n,user):
 global helper_calls
 if not running or a==STOP:return
 assert a in instructions,hex(a);hits.add(a)
 if a in [0x405170,0x409e40]:events.append((2,a,read(cpu.reg_read(UC_X86_REG_ESP)+4) if a==0x405170 else 0,0));helper_calls+=int(a==0x405170)
def memory(cpu,access,a,n,value,user):
 if not running or STACK<=a<STACK+0x100000:return
 assert any(lo<=a and a+n<=lo+size for lo,size in regions),(hex(cpu.reg_read(UC_X86_REG_EIP)),hex(a),n)
 events.append((int(access==UC_MEM_WRITE),a,n,(value if access==UC_MEM_WRITE else read(a,n))&((1<<(n*8))-1)))
vm.hook_add(UC_HOOK_CODE,code);vm.hook_add(UC_HOOK_MEM_READ,memory);vm.hook_add(UC_HOOK_MEM_WRITE,memory)
def group_data(g,n,p):
 for i in range(30):put(0x5cf75c+g*120+i*4,p[8] if i<n else p[7] if i==n else p[14])
def prepare(p):
 for i,(a,n) in enumerate(regions):vm.mem_write(a,bytes([(p[9]+37*i)&255])*n)
 for i in range(64):put(0x576180+i*4,(i+3)&15)
 for g in range(16):group_data(g,(p[6]+g*7)%31,p)
 g=(p[10] if p[0] else p[5])&0x1fffffff;assert g<16;group_data(g,p[6],p)
 assert p[1]&0x3fffffff<64;put(0x576180+(p[1]&0x3fffffff)*4,p[5])
 for a,v in [(0x7056c8,p[1]),(0x7056c4,p[2]),(0xe92e0c,p[3]),(0x605790,p[4]),(0x7056dc,p[11]),(0x7056d8,p[12]),(0x7056d0,p[13])]:put(a,v)
def native(p):
 global events,running
 prepare(p);sp=STACK+0xf0000;put(sp,STOP);canary=sp+(8 if p[0]==1 else 4);put(canary,0xbaadf00d)
 if p[0]==1:put(sp+4,p[10])
 saved=[(UC_X86_REG_EBX,0x11111111),(UC_X86_REG_EBP,0x22222222),(UC_X86_REG_ESI,0x33333333),(UC_X86_REG_EDI,0x44444444)]
 for r,v in saved:vm.reg_write(r,v)
 for r,v in [(UC_X86_REG_EAX,0xdeadbeef),(UC_X86_REG_ECX,0xbadc0de),(UC_X86_REG_EDX,0xfefefefe),(UC_X86_REG_ESP,sp),(UC_X86_REG_EFLAGS,2)]:vm.reg_write(r,v)
 events=[];running=True
 try:vm.emu_start(0x409e40 if p[0]==0 else 0x405170,STOP,count=100000)
 finally:running=False
 assert vm.reg_read(UC_X86_REG_EIP)==STOP and vm.reg_read(UC_X86_REG_ESP)==sp+4 and read(canary)==0xbaadf00d and all(vm.reg_read(r)==v for r,v in saved)
 memory=hashlib.sha256()
 for a,n in regions:memory.update(struct.pack('<II',a,n));memory.update(vm.mem_read(a,n))
 trace=hashlib.sha256()
 for e in events:trace.update(struct.pack('<4I',*e))
 return {'result':vm.reg_read(UC_X86_REG_EAX),'memory':memory.hexdigest(),'trace':trace.hexdigest(),'events':len(events)}
#entry/selector/secondary/now/last/lookup-group/prefix-length/stop-value/positive-value/
#seed/direct-group/old-value/old-count/old-index/after-stop-poison/reserved
DEFAULT=[0,0,0,1000,0,3,7,0,256,17,3,0x11223344,0x55667788,0x99aabbcc,0x7fffffff,0];cases=[];categories=[]
def add(category,**kw):
 p=DEFAULT.copy()
 for k,v in kw.items():p[int(k[1:])]=v&MASK
 cases.append(p);categories.append(category)
for last,delta in itertools.product([0,0xffffff00,0x7ffffff0],[0,1,499,500,501,0x7fffffff,0x80000000,0xffffffff,0xffffff00,512]):add('signed_modulo32_timeout_threshold',p4=last,p3=(last+delta)&MASK)
for secondary,now in itertools.product([1,256,0x80000000],[0,0x12345678,0xffffffff]):add('DWORD_secondary_updates_timestamp_only',p2=secondary,p3=now)
for selector,n in itertools.product([1,2,7,15,31,63,0x40000000,0x40000001,0x80000000],[0,1,29,30]):add('DWORD_lookup_stride4_actual_group_stride120',p1=selector,p2=0xa5a50000,p5=selector&15,p6=n,p7={0:0x80000000,1:0xffffffff,29:0,30:0}[n])
for group,n in itertools.product([0x20000000,0x20000001,0x80000000],[3,30]):add('mapped_group_arithmetic_wrap',p1=2,p5=group,p6=n)
for group,n in itertools.product([0,1,7,15,0x20000000,0x20000001,0x80000000],[0,1,14,29,30]):add('whole_counter_all30_or_early_stop',p0=1,p10=group,p6=n,p7=0x80000000)
for value in [0,0xffffffff,0x80000000,0x7fffffff]:add('signed_DWORD_counter_values',p0=1,p10=2,p6=10,p8=value)
complete=len(cases)
if args.limit:cases=cases[:args.limit];categories=categories[:args.limit]
provided=[args.exe,args.plain_exe,args.optimized_exe,args.optimized_plain_exe]
if args.production:assert all(provided) and not args.limit
exes=[(p or HERE/'build'/config/f'tooltip_{kind}.exe').resolve() for p,(config,kind) in zip(provided,itertools.product(['Debug','Release'],['trace','plain']))]
files=[Path(__file__),cache,other_cache,HERE/'CMakeLists.txt',HERE/'record_fixture.c',HERE/'tooltip_timing.h',HERE/'unit_00040.c',HERE/'unit_00073.c',ROOT/'include/standalone_data.h',ROOT/'build/c-only-debug/Debug/restore_data.lib']
if args.production:
 assert args.proof_sha256 and sha(HERE/'timing-proof.json')==args.proof_sha256,'exact immutable private proof SHA required'
 for name in ['unit_00040.c','unit_00073.c']:assert sha(HERE/name)==sha(ROOT/'src'/name)
 assert sha(HERE/'tooltip_timing.h')==sha(ROOT/'include/tooltip_timing.h')
 files.extend([ROOT/'src/unit_00040.c',ROOT/'src/unit_00073.c',ROOT/'include/tooltip_timing.h'])
 expected={p.resolve() for p in [ROOT/'src/unit_00040.c',ROOT/'src/unit_00073.c',HERE/'record_fixture.c']}
 for k,exe in enumerate(exes):
  target='restore_tooltip_timing_'+('trace' if k%2==0 else 'plain')+'_verify'
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
 for symbol,owner in [('_FUN_00405170','unit_00040.obj'),('_FUN_00409e40','unit_00073.obj')]:
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
report={'passed':True,'production':args.production,'checked':len(cases),'comparisons':len(cases)*4,'full_corpus_cases':complete,'coverage':coverage,'helper_calls':helper_calls,'native_sha256':PIN,'native_exports':{rel(cache):sha(cache),rel(other_cache):sha(other_cache)},'compiled_C_sha256':bins[rel(exes[0])],'variants':{name:{'path':rel(p),'sha256':bins[rel(p)],'checked':len(cases)} for name,p in zip(['Debug_trace','Debug_plain','O2_trace','O2_plain'],exes)},'source_hashes':before,'source_drift':[rel(p) for p in files if sha(p)!=before[rel(p)]],'binary_drift':[rel(p) for p in exes if sha(p)!=bins[rel(p)]],'corpus_sha256':hashlib.sha256(json.dumps(cases,separators=(',',':')).encode()).hexdigest(),'elapsed_seconds':round(time.monotonic()-start,2),'apply':{'src/unit_00040.c':{'before_sha256':'3f0f547194939a23bcda14e8db19b3526f39ffc570fbb81311a75d6ab30d0104','after_sha256':sha(HERE/'unit_00040.c')},'src/unit_00073.c':{'before_sha256':'b02057ed70ae491fe6146b99e2d60e99ef522ce67131dbc99b6bc213275d4bd9','after_sha256':sha(HERE/'unit_00073.c')},'include/tooltip_timing.h':{'before_sha256':None,'after_sha256':sha(HERE/'tooltip_timing.h')}},'limits':['Two whole actual C functions43instructions, no imports or boundary. Native bytes authenticated against pinned PE. Both full EAX results compared; saved GPRs/caller stack/canary checked.','Timeout is signed((now-last) modulo32)>500, not unsigned age. Selector is cached once; nonzero secondary only updates timestamp. Lookup loads DWORD at576180+selector*4 before output writes.','Actual counter uses120-byte groups and stops at first signed DWORD<=0 or30 reads. Whole guarded state and all ordered DWORD accesses/helper entries compared.','Finite representative inputs; large selector/group values are explicitly synthetic mapped wrap probes, not claimed real resource IDs. Private /O2 uses Debug data/CRT. No root/frozen edits, GUI or original process.']}
assert not report['source_drift'] and not report['binary_drift'];output=ROOT/'metadata/tooltip-timing-emulation.json' if args.production else HERE/('timing-partial.json' if args.limit else 'timing-proof.json');output.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:report[k] for k in ['passed','checked','comparisons','coverage','helper_calls','elapsed_seconds']}))
