"""Original 442EF0/442F80 versus complete C, including their ordered composition."""
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
functions={e['requested_address'].lower():e for e in dossier['entries'] if e['requested_address'].lower() in ['00442ef0','00442f80']}
assert len(functions)==2;instructions={}
for name,e in functions.items():
 assert e['status']=='exact_ghidra_function' and not e['assembly']['has_more'] and len(e['assembly']['items'])==48
 for it in e['assembly']['items']:
  a=int(it['address'],16);b=bytes.fromhex(it['bytes']);assert bytes(vm.mem_read(a,len(b)))==b;instructions[a]=name
regions=[(0x77f738,0x6cf4)]
STACK=0x34000000;STOP=0x33000000;HARNESS=STOP+256;vm.mem_map(STACK,0x100000);vm.mem_map(STOP,4096)
# Test caller only: CALL actual first leaf; CALL actual second leaf; RET.
vm.mem_write(HARNESS,b'\xe8'+struct.pack('<i',0x442ef0-(HARNESS+5))+b'\xe8'+struct.pack('<i',0x442f80-(HARNESS+10))+b'\xc3')
read=lambda a,n=4:int.from_bytes(vm.mem_read(a,n),'little')
def put(a,v,n=4):vm.mem_write(a,(v&((1<<(8*n))-1)).to_bytes(n,'little'))
events=[];hits=set();running=False;max_count={'00442ef0':0,'00442f80':0}
def code(cpu,a,n,user):
 if not running or a==STOP or a in [HARNESS,HARNESS+5,HARNESS+10]:return
 assert a in instructions,hex(a);hits.add(a)
 if a in [0x442f20,0x442fba]:
  value=cpu.reg_read(UC_X86_REG_EDX);assert 0<=value<=200;owner=instructions[a];max_count[owner]=max(max_count[owner],value)
def memory(cpu,access,a,n,value,user):
 if not running or STACK<=a<STACK+0x100000:return
 assert any(lo<=a and a+n<=lo+size for lo,size in regions),(hex(cpu.reg_read(UC_X86_REG_EIP)),hex(a),n)
 events.append((int(access==UC_MEM_WRITE),a,n,(value if access==UC_MEM_WRITE else read(a,n))&((1<<(n*8))-1)))
vm.hook_add(UC_HOOK_CODE,code);vm.hook_add(UC_HOOK_MEM_READ,memory);vm.hook_add(UC_HOOK_MEM_WRITE,memory)
def entry(i,kind,active,state,timer):
 r=0x77f748+i*98
 for a,v in [(r,kind),(r+1,active),(r+24,state),(r+51,timer)]:put(a,v,1)
def prepare(p):
 vm.mem_write(regions[0][0],bytes([p[8]&255])*regions[0][1])
 for i in range(200):
  if p[2]==1:entry(i,1,1,p[6],p[7])
  elif p[2]==2:entry(i,i%4,i%3,i%7,i%5)
  else:entry(i,1,0,p[6],p[7])
 entry(p[3],p[4],p[5],p[6],p[7])
 if p[9]<200:entry(p[9],p[10],p[11],p[12],p[13])
 put(0x784f10,p[1])
def native(p):
 global events,running
 prepare(p);sp=STACK+0xf0000;put(sp,STOP);put(sp+4,0xbaadf00d)
 saved=[(UC_X86_REG_EBX,0x11111111),(UC_X86_REG_EBP,0x22222222),(UC_X86_REG_ESI,0x33333333),(UC_X86_REG_EDI,0x44444444)]
 for r,v in saved:vm.reg_write(r,v)
 for r,v in [(UC_X86_REG_EAX,0xdeadbeef),(UC_X86_REG_ECX,0xbadc0de),(UC_X86_REG_EDX,0xfefefefe),(UC_X86_REG_ESP,sp),(UC_X86_REG_EFLAGS,2)]:vm.reg_write(r,v)
 events=[];running=True
 try:vm.emu_start([0x442ef0,0x442f80,HARNESS][p[0]],STOP,count=100000)
 finally:running=False
 assert vm.reg_read(UC_X86_REG_EIP)==STOP and vm.reg_read(UC_X86_REG_ESP)==sp+4 and read(sp+4)==0xbaadf00d and all(vm.reg_read(r)==v for r,v in saved)
 memory=hashlib.sha256()
 for a,n in regions:memory.update(struct.pack('<II',a,n));memory.update(vm.mem_read(a,n))
 trace=hashlib.sha256()
 for e in events:trace.update(struct.pack('<4I',*e))
 return {'result':0,'memory':memory.hexdigest(),'trace':trace.hexdigest(),'events':len(events)}
# mode/index/pattern/primary-record/type/active/state/timer/guard-seed/
# secondary-record/type/active/state/timer/reserved/reserved
DEFAULT=[0,0,0,0,1,1,0,0,0xa5,199,1,1,0,0,0,0];cases=[];categories=[]
def add(category,**kw):
 p=DEFAULT.copy()
 for k,v in kw.items():p[int(k[1:])]=v&MASK
 cases.append(p);categories.append(category)
for mode,state,timer in itertools.product(range(3),[0,1,2,3,4,255],[0,1,2,255]):add('two_candidates_postdecrement',p0=mode,p6=state,p7=timer,p12=state,p13=timer)
for mode,state,timer in itertools.product(range(3),[0,1,2],[0,1,2]):add('unique_candidate_skips_second_pass',p0=mode,p6=state,p7=timer,p11=0)
for mode,kind,active in itertools.product(range(3),[0,1,2,255],[0,1,255]):add('BYTE_gates_mixed_records',p0=mode,p2=2,p4=kind,p5=active,p6=1,p7=128,p9=0xffffffff)
for mode,state,timer in itertools.product(range(3),[0,1,2,3],[0,1,2]):add('all200_and_second_pass',p0=mode,p2=1,p6=state,p7=timer,p9=0xffffffff)
for mode in range(3):add('zero_count_still_sets_WORD_one',p0=mode,p5=0,p11=0)
for profile in [0,1,7,127,255,256,0x80000000,0x10000000,0x10000001]:add('DWORD_index_modular_stride32',p1=profile,p6=0,p12=0)
for mode in range(3):add('last_record_exact_unique',p0=mode,p3=199,p6=1 if mode==1 else 0,p9=0xffffffff)
complete=len(cases)
if args.limit:cases=cases[:args.limit];categories=categories[:args.limit]
provided=[args.exe,args.plain_exe,args.optimized_exe,args.optimized_plain_exe]
if args.production:assert all(provided) and not args.limit
exes=[(p or HERE/'build'/config/f'map_decay_{kind}.exe').resolve() for p,(config,kind) in zip(provided,itertools.product(['Debug','Release'],['trace','plain']))]
files=[Path(__file__),cache,HERE/'CMakeLists.txt',HERE/'record_fixture.c',HERE/'map_record_decay.h',HERE/'unit_00613.c',HERE/'unit_00614.c',ROOT/'include/standalone_data.h',ROOT/'build/c-only-debug/Debug/restore_data.lib']
if args.production:
 assert args.proof_sha256 and sha(HERE/'decay-proof.json')==args.proof_sha256
 for name in ['unit_00613.c','unit_00614.c']:assert sha(HERE/name)==sha(ROOT/'src'/name)
 assert sha(HERE/'map_record_decay.h')==sha(ROOT/'include/map_record_decay.h')
 files.extend([ROOT/'src/unit_00613.c',ROOT/'src/unit_00614.c',ROOT/'include/map_record_decay.h'])
 expected={p.resolve() for p in [ROOT/'src/unit_00613.c',ROOT/'src/unit_00614.c',HERE/'record_fixture.c']}
 for k,exe in enumerate(exes):
  target='restore_map_record_decay_'+('trace' if k%2==0 else 'plain')+'_verify';assert exe.name==target+'.exe' and HERE not in exe.parents
  project=exe.parent.parent/(target+'.vcxproj');xml=ET.parse(project);ns={'m':'http://schemas.microsoft.com/developer/msbuild/2003'}
  compiles={(project.parent/e.attrib['Include'].replace('\\','/')).resolve() for e in xml.findall('.//m:ClCompile[@Include]',ns)};assert compiles==expected
  groups=[g for g in xml.findall('.//m:ItemDefinitionGroup',ns) if ('Debug' if k<2 else 'Release') in g.get('Condition','')];assert groups
  for g in groups:
   runtime=g.find('m:ClCompile/m:RuntimeLibrary',ns);assert runtime is not None and runtime.text==('MultiThreadedDebug' if k<2 else 'MultiThreaded')
  data=exe.parent/'restore_data.lib';assert data.is_file();files.extend([project,data])
rel=lambda p:p.relative_to(ROOT).as_posix();before={rel(p):sha(p) for p in files};bins={rel(p):sha(p) for p in exes}
for exe in exes:
 lines=exe.with_suffix('.map').read_text().splitlines()
 for symbol,owner in [('_FUN_00442ef0','unit_00613.obj'),('_FUN_00442f80','unit_00614.obj')]:
  found=[x for x in lines if symbol in x.split()];assert len(found)==1 and found[0].split()[-1]==owner
start=time.monotonic();procs=[subprocess.Popen([str(p),'--oracle-batch'],stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True) for p in exes]
try:
 for p in procs:assert json.loads(p.stdout.readline())=={'fields':16}
 for i,p in enumerate(cases):
  expected=native(p)
  for k,proc in enumerate(procs):
   proc.stdin.write(' '.join(map(str,p))+'\n');proc.stdin.flush();line=proc.stdout.readline();assert line,'C exit:'+proc.stderr.read();actual=json.loads(line)
   want=expected if k%2==0 else dict(expected,trace=hashlib.sha256(b'').hexdigest(),events=0)
   assert actual==want,json.dumps({'case':i,'parameters':p,'variant':str(exes[k]),'actual':actual,'expected':want})
finally:
 for p in procs:p.stdin.close();p.wait(timeout=20);assert p.returncode==0,p.stderr.read()
dead={0x442f24,0x442f29,0x442f2c,0x442f35};coverage={name:[sum(instructions[a]==name for a in hits),48] for name in functions}
if not args.limit:assert set(instructions)-hits==dead and max_count=={'00442ef0':200,'00442f80':200},(coverage,max_count)
apply={}
for name in ['unit_00613.c','unit_00614.c']:apply['src/'+name]={'before_sha256':sha(ROOT/'src'/name) if not args.production else None,'after_sha256':sha(HERE/name)}
apply['include/map_record_decay.h']={'before_sha256':None,'after_sha256':sha(HERE/'map_record_decay.h')}
report={'passed':True,'production':args.production,'checked':len(cases),'comparisons':len(cases)*4,'full_corpus_cases':complete,'coverage':coverage,'maximum_native_count':max_count,'certified_unreachable':[f'{a:08x}' for a in sorted(dead)],'unreachable_reason':'442EF0 initializes EDX=0 and increments at most once for each of exactly200 records; signed-negative branch cannot occur from its entry. C retains the original condition and both stores. No injected negative counter cases.','native_sha256':PIN,'native_exports':{rel(cache):sha(cache)},'compiled_C_sha256':bins[rel(exes[0])],'variants':{name:{'path':rel(p),'sha256':bins[rel(p)],'checked':len(cases)} for name,p in zip(['Debug_trace','Debug_plain','O2_trace','O2_plain'],exes)},'source_hashes':before,'source_drift':[rel(p) for p in files if sha(p)!=before[rel(p)]],'binary_drift':[rel(p) for p in exes if sha(p)!=bins[rel(p)]],'corpus_sha256':hashlib.sha256(json.dumps(cases,separators=(',',':')).encode()).hexdigest(),'elapsed_seconds':round(time.monotonic()-start,2),'apply':apply,'limits':['Both complete C leaf functions and their actual first-then-second composition; no internal substitute or import. Original96 instructions authenticated against pinned PE;92 reachable instructions exercised,4 certified dead.','Zero-argument void cdecl, no receiver. Whole guarded record/player window and ordered BYTE/DWORD reads, BYTE/WORD writes compared; callee-saved GPRs/stack/canary checked. Scratch EAX/ECX/EDX and EFLAGS are not promised.','Finite representative fixtures, count0/1/2/200, postdecrement timers0/1/2/128/255, state/type/active BYTE boundaries. Large DWORD indices are separately synthetic mapped wrap/width tests, not claimed real player indices.','Private /O2 uses Debug data/CRT. Product application/replay separate; old frozen phases unchanged. No GUI or original process.']}
assert not report['source_drift'] and not report['binary_drift'];output=ROOT/'metadata/map-record-decay-emulation.json' if args.production else HERE/('decay-partial.json' if args.limit else 'decay-proof.json');output.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:report[k] for k in ['passed','checked','comparisons','coverage','maximum_native_count','elapsed_seconds']}))
