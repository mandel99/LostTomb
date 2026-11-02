"""Whole actual430790 pixel-to-tile geometry, prerequisite of405F80."""
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
cache=ROOT/'metadata/native-function-audit-cache/batch-0448.json';dossier=json.loads(cache.read_text());assert dossier['native_sha256']==PIN
functions={e['requested_address'].lower():e for e in dossier['entries'] if e['requested_address'].lower() in ['00430790']}
assert len(functions)==1;instructions={}
for name,e in functions.items():
 assert e['status']=='exact_ghidra_function' and not e['assembly']['has_more']
 for it in e['assembly']['items']:
  a=int(it['address'],16);b=bytes.fromhex(it['bytes']);assert bytes(vm.mem_read(a,len(b)))==b;instructions[a]=name
assert [len(functions[n]['assembly']['items']) for n in ['00430790']]==[174]
regions=[(0x70e3e8,0xac+24*231*4+16),(0xe38e20,40)]
STACK=0x34000000;STOP=0x33000000;vm.mem_map(STACK,0x100000);vm.mem_map(STOP,4096)
read=lambda a,n=4:int.from_bytes(vm.mem_read(a,n),'little')
def put(a,v,n=4):vm.mem_write(a,(v&((1<<(8*n))-1)).to_bytes(n,'little'))
events=[];hits=set();running=False;helper_calls=0;lookup_cache={}
def code(cpu,a,n,user):
 global helper_calls
 if not running or a==STOP:return
 assert a in instructions,hex(a);hits.add(a)
 if a==0x430790:events.append((2,a,read(cpu.reg_read(UC_X86_REG_ESP)+4),0));helper_calls+=1
def memory(cpu,access,a,n,value,user):
 if not running or STACK<=a<STACK+0x100000:return
 assert any(lo<=a and a+n<=lo+size for lo,size in regions),(hex(cpu.reg_read(UC_X86_REG_EIP)),hex(a),n)
 events.append((int(access==UC_MEM_WRITE),a,n,(value if access==UC_MEM_WRITE else read(a,n))&((1<<(n*8))-1)))
vm.hook_add(UC_HOOK_CODE,code);vm.hook_add(UC_HOOK_MEM_READ,memory);vm.hook_add(UC_HOOK_MEM_WRITE,memory)
def prepare(p):
 for i,(a,n) in enumerate(regions):vm.mem_write(a,bytes([(p[13]+37*i)&255])*n)
 for a,v in [(0xe38e34,p[0]),(0xe38e30,p[1]),(0x70e490,p[2]),(0x70e48c,p[3]),(0x70e458,p[4]),(0x70e45c,p[5]),(0x70e44c,p[6]),(0x70e460,p[7]),(0x70e464,p[8]),(0x70e474,p[9]),(0x70e478,p[10])]:put(a,v)
 key=(p[11],p[12])
 if key not in lookup_cache:
  values=[]
  for i in range(24):
   for j in range(231):values.append([p[12]+i*228+j%228,0,0x0fff0000,0x0ffeffff,0x80000000,0xffffffff][p[11]])
  lookup_cache[key]=struct.pack('<'+str(len(values))+'I',*values)
 vm.mem_write(0x70e494,lookup_cache[key])
def native(p):
 global events,running
 prepare(p);sp=STACK+0xf0000;put(sp,STOP);canary=sp+8;put(canary,0xbaadf00d)
 put(sp+4,p[14])
 saved=[(UC_X86_REG_EBX,0x11111111),(UC_X86_REG_EBP,0x22222222),(UC_X86_REG_ESI,0x33333333),(UC_X86_REG_EDI,0x44444444)]
 for r,v in saved:vm.reg_write(r,v)
 for r,v in [(UC_X86_REG_EAX,0xdeadbeef),(UC_X86_REG_ECX,0xbadc0de),(UC_X86_REG_EDX,0xfefefefe),(UC_X86_REG_ESP,sp),(UC_X86_REG_EFLAGS,2)]:vm.reg_write(r,v)
 events=[];running=True
 try:vm.emu_start(0x430790,STOP,count=100000)
 finally:running=False
 assert vm.reg_read(UC_X86_REG_EIP)==STOP and vm.reg_read(UC_X86_REG_ESP)==sp+4 and read(canary)==0xbaadf00d and all(vm.reg_read(r)==v for r,v in saved)
 memory=hashlib.sha256()
 for a,n in regions:memory.update(struct.pack('<II',a,n));memory.update(vm.mem_read(a,n))
 trace=hashlib.sha256()
 for e in events:trace.update(struct.pack('<4I',*e))
 return {'result':vm.reg_read(UC_X86_REG_EAX),'memory':memory.hexdigest(),'trace':trace.hexdigest(),'events':len(events)}
#pixels0/1,origins0/1,extent0,stepminus1,span1,rows,columns,rowoffset,coloffset,payloadmode,payloadbase,seed,ignoredarg,reserved
DEFAULT=[0,0,0,0,4,1,2,2,2,2,2,0,1000,17,0,0];cases=[];categories=[]
def add(category,**kw):
 p=DEFAULT.copy()
 for k,v in kw.items():p[int(k[1:])]=v&MASK
 cases.append(p);categories.append(category)
for x,y in itertools.product(range(12),range(4)):add('all_pixels_small_actual_geometry',p0=x,p1=y)
for x,y in itertools.product([0,1,2,3,4,5,6,7,15,23],range(8)):add('half_remainders_parities_and_cached_adjustments',p0=x,p1=y,p5=3,p6=4,p8=4)
for step,span,width,height in [(1,2,12,4),(3,4,24,8)]:
 for x,y in [(-1,0),(width,0),(0,-1),(0,height)]:add('four_rectangle_rejections',p0=x,p1=y,p5=step,p6=span,p8=2 if step==1 else 4)
for mode,(x,y) in itertools.product(range(6),[(0,0),(3,1),(11,3)]):add('raw_lookup_zero_invalid_and_signed_payloads',p0=x,p1=y,p11=mode)
for (ox,oy),(x,y) in itertools.product([(0,0),(-40,-20),(1024,768)],[(0,0),(5,2),(11,3)]):add('signed_origins',p0=ox+x,p1=oy+y,p2=ox,p3=oy)
add('empty_rows',p7=0);add('empty_columns',p8=0);add('modulo_extent_zero',p4=0xfffffffe)
add('wrapped_first_extent_rejects',p0=0x7ffffffb,p2=0x7ffffffa)
add('wrapped_second_extent_rejects',p0=1,p1=0x7fffffff,p3=0x7ffffffe)
add('wrapped_row_product_rejects',p0=1,p7=0x7fffffff)
for ignored in [0,1,0xdeadbeef,0xffffffff]:add('observed_caller_DWORD_is_unused',p0=5,p1=2,p14=ignored)
complete=len(cases)
if args.limit:cases=cases[:args.limit];categories=categories[:args.limit]
provided=[args.exe,args.plain_exe,args.optimized_exe,args.optimized_plain_exe]
if args.production:assert all(provided) and not args.limit
exes=[(p or HERE/'build'/config/f'map_hover_{kind}.exe').resolve() for p,(config,kind) in zip(provided,itertools.product(['Debug','Release'],['trace','plain']))]
files=[Path(__file__),cache,HERE/'CMakeLists.txt',HERE/'record_fixture.c',HERE/'map_hover_geometry.h',HERE/'unit_00460.c',ROOT/'include/standalone_data.h',ROOT/'build/c-only-debug/Debug/restore_data.lib']
if args.production:
 assert args.proof_sha256 and sha(HERE/'geometry-proof.json')==args.proof_sha256,'exact immutable private proof SHA required'
 for name in ['unit_00460.c']:assert sha(HERE/name)==sha(ROOT/'src'/name)
 assert sha(HERE/'map_hover_geometry.h')==sha(ROOT/'include/map_hover_geometry.h')
 files.extend([ROOT/'src/unit_00460.c',ROOT/'include/map_hover_geometry.h'])
 expected={p.resolve() for p in [ROOT/'src/unit_00460.c',HERE/'record_fixture.c']}
 for k,exe in enumerate(exes):
  target='restore_map_hover_geometry_'+('trace' if k%2==0 else 'plain')+'_verify'
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
 for symbol,owner in [('_FUN_00430790','unit_00460.obj')]:
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
report={'passed':True,'production':args.production,'checked':len(cases),'comparisons':len(cases)*4,'full_corpus_cases':complete,'coverage':coverage,'helper_calls':helper_calls,'native_sha256':PIN,'native_exports':{rel(cache):sha(cache)},'compiled_C_sha256':bins[rel(exes[0])],'variants':{name:{'path':rel(p),'sha256':bins[rel(p)],'checked':len(cases)} for name,p in zip(['Debug_trace','Debug_plain','O2_trace','O2_plain'],exes)},'source_hashes':before,'source_drift':[rel(p) for p in files if sha(p)!=before[rel(p)]],'binary_drift':[rel(p) for p in exes if sha(p)!=bins[rel(p)]],'corpus_sha256':hashlib.sha256(json.dumps(cases,separators=(',',':')).encode()).hexdigest(),'elapsed_seconds':round(time.monotonic()-start,2),'apply':{'src/unit_00460.c':{'before_sha256':'dddb73fcedf189ab48add08b556cf3d4da0fa799233b40c61dfa869ae56b22e7','after_sha256':sha(HERE/'unit_00460.c')},'include/map_hover_geometry.h':{'before_sha256':None,'after_sha256':sha(HERE/'map_hover_geometry.h')}},'limits':['Whole174instruction actual C algorithm, no callees/imports/FPU or artificial boundary. Ordered DWORD reads/writes, full EAX, saved GPRs/caller stack/canary and guarded cache/table state compared.','C contract retains the unused DWORD passed by all three observed cached callers; native body has no incoming argument/receiver data dependency. Several argument bitpatterns checked.','Positive signed divisors70E44C and70E45C+1, mapped231-stride lookup fixtures and finite viewport layouts. Out-of-contract division traps are not tested or replaced with clamps. Opaque valid linear IDs and separately synthetic raw signed/sentinel payloads tested; viewport-table producer is not proved here.','430790 is a prerequisite only. 405F80 and its editor callback/range/mutation graph remain unclosed and unpromoted. Invalid lookup still publishes raw cache but preserves old edge flags. Private /O2 uses Debug data/CRT; no root/frozen edits, GUI or original process.']}
assert not report['source_drift'] and not report['binary_drift'];output=ROOT/'metadata/map-hover-geometry-emulation.json' if args.production else HERE/('geometry-partial.json' if args.limit else 'geometry-proof.json');output.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:report[k] for k in ['passed','checked','comparisons','coverage','helper_calls','elapsed_seconds']}))
