"""4099B0 plus actual filename/access/directory algorithms; controlled OS/load/UI boundaries."""
from pathlib import Path
import argparse,hashlib,itertools,json,struct,subprocess,sys,time,xml.etree.ElementTree as ET
HERE=Path(__file__).resolve().parent;ROOT=HERE.parent.parent
sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from unicorn import *
from unicorn.x86_const import *
ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--limit',type=int);ap.add_argument('--start',type=int,default=0);ap.add_argument('--production',action='store_true');ap.add_argument('--proof-sha256')
for name in ['exe','plain-exe','optimized-exe','optimized-plain-exe']:ap.add_argument('--'+name,type=Path)
args=ap.parse_args();sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest();MASK=0xffffffff
PIN='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
original=ROOT/'build/smoke-game/Pharaoh.exe';assert sha(original)==PIN;image=original.read_bytes()
u16=lambda p:struct.unpack_from('<H',image,p)[0];u32=lambda p:struct.unpack_from('<I',image,p)[0]
pe=u32(60);opt=pe+24;table=opt+u16(pe+20);base=u32(opt+28)
vm=Uc(UC_ARCH_X86,UC_MODE_32);vm.mem_map(base,(u32(opt+56)+4095)&~4095)
for i in range(u16(pe+6)):
 s=table+40*i;vm.mem_write(base+u32(s+12),image[u32(s+20):u32(s+20)+u32(s+16)])
spec={0x4099b0:(72,89,0),0x4ce100:(1740,178,1),0x4ce2d0:(1741,14,0),0x4ce3f0:(1747,34,1),0x4cbea0:(1663,14,3),0x4cbec0:(1664,15,1),0x4ce330:(1743,17,1),0x4ce3a0:(1745,10,1),0x4ce3c0:(1746,14,1)}
changed={0x4099b0,0x4ce100,0x4ce2d0,0x4ce3f0};functions={};instructions={};evidence=set()
for name in ['batch-0064.json','batch-1664.json','batch-1728.json']:
 p=ROOT/'metadata/native-function-audit-cache'/name;d=json.loads(p.read_text());assert d['native_sha256']==PIN
 for e in d['entries']:
  a=int(e['requested_address'],16)
  if a in spec:
   assert e['status']=='exact_ghidra_function' and not e['assembly']['has_more'] and len(e['assembly']['items'])==spec[a][1]
   functions[a]=e;evidence.add(p)
   for it in e['assembly']['items']:
    pc=int(it['address'],16);b=bytes.fromhex(it['bytes']);assert bytes(vm.mem_read(pc,len(b)))==b;instructions[pc]=a
assert len(functions)==9 and len(instructions)==385
regions=[(0x57625c,0x50),(0x5e06cc,0x60),(0xe36b84,0x380),(0xe37068,0x80),(0xe38adc,0x30),(0xd3b188,0x58),(0xe38dba,0xb8)]
constants={a:bytes(vm.mem_read(a,n)) for a,n in regions[:2]}
assert constants[0x57625c][16:22]==b'maps\\\0' and constants[0x57625c][24:28]==b'map\0'
STACK=0x34000000;STOP=0x33000000;vm.mem_map(STACK,0x100000);vm.mem_map(STOP,4096)
read=lambda a,n=4:int.from_bytes(vm.mem_read(a&MASK,n),'little')
def put(a,v,n=4):vm.mem_write(a&MASK,(v&((1<<(8*n))-1)).to_bytes(n,'little'))
def cstring(a):
 b=bytearray()
 while True:
  x=read(a,1);b.append(x);a+=1
  if x==0:return bytes(b)
  assert len(b)<256
def legacy(a,n=1):return any(lo<=a and a+n<=lo+size for lo,size in regions)
def token(a):return a if a==0 or legacy(a) else 0xc0000000
events=[];io=bytearray();io_count=0;opens=0;chdirs=0;current=None;running=False;hits=set();entries={a:0 for a in spec}
boundary_counts={a:0 for a in [0x568eab,0x5647eb,0x56b2dd,0x5623fc,0x4d5520,0x4de600,0x4ce6e0]}
def io_event(a,x,y,z,result,first=None,second=None):
 global io_count
 io.extend(struct.pack('<5I',a,x&MASK,y&MASK,z&MASK,result&MASK))
 for text in [first,second]:io.extend(struct.pack('<I',len(text) if text is not None else 0));io.extend(text or b'')
 io_count+=1;boundary_counts[a]+=1
def post_load():
 if current[9]&1:vm.mem_write(0xe37078,b'mutated.after.map\0')
 if current[9]&2:put(0xd3b1c0,0,1)
 if current[9]&4:put(0xe38dda,0x80,1);put(0xe38dca,0x81,1)
def boundary(cpu,a):
 global opens,chdirs
 sp=cpu.reg_read(UC_X86_REG_ESP);x=read(sp+4);y=read(sp+8);z=read(sp+12);result=0
 if a==0x568eab:
  result=current[6] if opens else current[5];opens+=1;io_event(a,token(x),y,0,result,cstring(x))
 elif a==0x5647eb:result=current[15];io_event(a,x,0,0,result)
 elif a==0x56b2dd:
  result=MASK if current[7]>>chdirs&1 else 0;chdirs+=1;io_event(a,token(x),0,0,result,cstring(x))
 elif a==0x5623fc:
  result=0 if current[8] else x
  if result:vm.mem_write(x,b'Z:\\fixture\0')
  io_event(a,x,y,0,result,None,cstring(x))
 elif a==0x4d5520:result=current[10];io_event(a,token(x),0,0,result,cstring(x));post_load()
 elif a==0x4de600:io_event(a,0,0,0,0);post_load()
 elif a==0x4ce6e0:io_event(a,x,y,z,0,cstring(x),cstring(y))
 else:raise AssertionError(hex(a))
 cpu.reg_write(UC_X86_REG_EAX,result&MASK);cpu.reg_write(UC_X86_REG_ECX,0x89abcde0);cpu.reg_write(UC_X86_REG_EDX,0x53535353)
 cpu.reg_write(UC_X86_REG_EIP,read(sp));cpu.reg_write(UC_X86_REG_ESP,sp+4)
def code(cpu,a,n,user):
 if not running or a==STOP:return
 if a in boundary_counts:boundary(cpu,a);return
 assert a in instructions,hex(a);hits.add(a)
 if a in spec:
  sp=cpu.reg_read(UC_X86_REG_ESP);argv=[read(sp+4+i*4) for i in range(spec[a][2])]+[0]*3
  if a==0x4cbea0:argv[0]=token(argv[0]);argv[1]=token(argv[1])
  elif spec[a][2]:argv[0]=token(argv[0])
  events.append((2,a,*argv[:3]));entries[a]+=1
def memory(cpu,access,a,n,value,user):
 if not running or STACK<=a<STACK+0x100000:return
 assert legacy(a,n),(hex(cpu.reg_read(UC_X86_REG_EIP)),hex(a),n)
 if instructions[cpu.reg_read(UC_X86_REG_EIP)] in changed:events.append((int(access==UC_MEM_WRITE),a,n,(value if access==UC_MEM_WRITE else read(a,n))&((1<<(n*8))-1),0))
vm.hook_add(UC_HOOK_CODE,code);vm.hook_add(UC_HOOK_MEM_READ,memory);vm.hook_add(UC_HOOK_MEM_WRITE,memory)
names=[b'',b'name',b'a.b.c',b'.hidden',b'last.part',b'a.555',b'b.sMk',b'c.wav',b'd.zzz',b'plain',b'n\x80me',b'x.a',b'no.ext.more',b'L'*90,b'abc']
extensions=[b'map',b'555',b'sMk',b'wav',b'zzz',b'a',b'',b'\x80Az']
def prepare(p):
 for i,(a,n) in enumerate(regions):vm.mem_write(a,bytes([(p[11]+37*i)&255])*n)
 for a,raw in constants.items():vm.mem_write(a,raw)
 vm.mem_write(0xe37078,names[p[3]]+b'\0');vm.mem_write(0xe38aec,b'\0'*4);vm.mem_write(0xe38aec,extensions[p[4]]+b'\0')
 vm.mem_write(0xe36b94,b'Q:\\original\0');vm.mem_write(0xe36e84,b'stale\0');put(0xe38e62,ord('R'),1)
 for a,v,n in [(0xd3b198,p[1],4),(0xd3b1c0,p[2],1),(0xd3b1b3,p[13],1),(0xe38dda,p[12],1),(0xe38dca,p[11],1)]:put(a,v,n)
 if p[0]==0 and p[1]==1:assert 5+len(names[p[3]].split(b'.')[0])+4+1<=100
mode_entries=[0x4099b0,0x4ce3f0,0x4ce100,0x4ce2d0,0x4ce3a0,0x4ce3c0,0x4ce330,0x4cbec0,0x4cbea0]
def native(p):
 global events,io,io_count,opens,chdirs,current,running
 prepare(p);sp=STACK+0xf0000;put(sp,STOP);argv=[] if p[0] in [0,3] else [0xe38aec if p[0]==7 else 0xe37078]
 if p[0]==8:argv=[0x57626c,0xe38aec,p[14]]
 for i,v in enumerate(argv):put(sp+4+i*4,v)
 canary=sp+4+4*len(argv);put(canary,0xbaadf00d)
 saved=[(UC_X86_REG_EBX,0x11111111),(UC_X86_REG_EBP,0x22222222),(UC_X86_REG_ESI,0x33333333),(UC_X86_REG_EDI,0x44444444)]
 for r,v in saved:vm.reg_write(r,v)
 for r,v in [(UC_X86_REG_EAX,0xdeadbeef),(UC_X86_REG_ECX,0xbadc0de),(UC_X86_REG_EDX,0xfefefefe),(UC_X86_REG_ESP,sp),(UC_X86_REG_EFLAGS,2)]:vm.reg_write(r,v)
 events=[];io=bytearray();io_count=opens=chdirs=0;current=p;running=True
 try:vm.emu_start(mode_entries[p[0]],STOP,count=300000)
 finally:running=False
 assert vm.reg_read(UC_X86_REG_EIP)==STOP and vm.reg_read(UC_X86_REG_ESP)==sp+4 and read(canary)==0xbaadf00d and all(vm.reg_read(r)==v for r,v in saved)
 digest=hashlib.sha256()
 for a,n in regions:digest.update(struct.pack('<II',a,n));digest.update(vm.mem_read(a,n))
 trace=hashlib.sha256()
 for e in events:trace.update(struct.pack('<5I',*e))
 return {'result':vm.reg_read(UC_X86_REG_EAX) if p[0] in [1,2] else 0,'memory':digest.hexdigest(),'trace':trace.hexdigest(),'events':len(events),'io':hashlib.sha256(io).hexdigest(),'io_count':io_count}
#entry/selection/filemode/name/ext/open1/open2/chdir-failmask/getcwd-fail/
#deep-postmask/deep-return/seed/initial-dialog/initial-error/copy-count/close-return
DEFAULT=[0,1,0,1,0,MASK,7,0,0,0,0x12345678,17,0x55,0x66,3,0];cases=[];categories=[]
def add(category,**kw):
 p=DEFAULT.copy()
 for k,v in kw.items():p[int(k[1:])]=v&MASK
 cases.append(p);categories.append(category)
for selected,mode,name in itertools.product([0,1,2,256,257,MASK],[0,1,255],[0,1,2,3,10,13,14]):add('whole_DWORD_selection_BYTE_mode_names_and_100byte_limit',p1=selected,p2=mode,p3=name)
for fd,name,post in itertools.product([0,7,0x7fffffff],[1,2,10,13],[0,7]):add('whole_successful_access_real_loader_handoff',p2=1,p3=name,p5=fd,p9=post)
for first,second,restore,cwd in itertools.product([MASK,0x80000000],[0,7,MASK,0x80000000],[0,1],[0,1]):add('whole_actual_access_retry_restore_and_fail',p2=1,p3=3,p5=first,p6=second,p7=restore,p8=cwd)
for name,ext in itertools.product([5,6,7,8,9],[0,1]):
 add('direct_access_first_open_succeeds',p0=1,p3=name,p4=ext,p5=0)
 for second,failmask in itertools.product([MASK,7],range(4)):add('direct_actual_access_directory_and_restore',p0=1,p3=name,p4=ext,p6=second,p7=failmask)
for name,ext,fail in itertools.product([5,6,7,8,9,11,0,10],[0,1,3],[0,1]):add('whole_directory_selector_original_constants',p0=2,p3=name,p4=ext,p7=fail)
for fail,cwd in [(0,0),(1,0),(1,1)]:add('whole_directory_restore_diagnostic',p0=3,p7=fail,p8=cwd)
for entry,name in itertools.product([4,6],range(len(names))):add('actual_first_dot_string_leafs',p0=entry,p3=name)
for ext in range(len(extensions)):add('actual_unsigned_ASCII_uppercase',p0=7,p4=ext)
for name,ext in itertools.product(range(len(names)),[0,5,7]):add('actual_fixed_threebyte_extension_append',p0=5,p3=name,p4=ext)
for count in [MASK,0,1,2,3,10]:add('actual_signed_count_forward_copy',p0=8,p14=count)
add('close_failure_is_ignored_after_successful_open',p0=1,p5=0,p15=MASK)
add('close_failure_is_ignored_after_retry_success',p0=1,p3=5,p6=7,p15=0x80000000)
complete=len(cases)
if args.start:assert args.limit and not args.production;cases=cases[args.start:];categories=categories[args.start:]
if args.limit:cases=cases[:args.limit];categories=categories[:args.limit]
provided=[args.exe,args.plain_exe,args.optimized_exe,args.optimized_plain_exe]
if args.production:assert all(provided) and not args.limit
exes=[(p or HERE/'build'/cfg/f'map_file_{kind}.exe').resolve() for p,(cfg,kind) in zip(provided,itertools.product(['Debug','Release'],['trace','plain']))]
source_names=[f'unit_{spec[a][0]:05d}.c' for a in changed];root_names=[f'unit_{spec[a][0]:05d}.c' for a in spec if a not in changed]
files=[Path(__file__),*sorted(evidence),HERE/'CMakeLists.txt',HERE/'record_fixture.c',HERE/'link_probe.c',HERE/'map_file_load.h',*[HERE/n for n in source_names],*[ROOT/'src'/n for n in root_names],ROOT/'include/standalone_data.h',ROOT/'include/restore_types.h',ROOT/'build/c-only-debug/Debug/restore_data.lib']
if args.production:
 assert args.proof_sha256 and sha(HERE/'file-load-proof.json')==args.proof_sha256
 baseline=json.loads((HERE/'file-load-proof.json').read_text())
 for name in ['src/'+n for n in root_names]+['include/restore_types.h','include/standalone_data.h']:assert sha(ROOT/name)==baseline['source_hashes'][name]
 for name in source_names:assert sha(HERE/name)==sha(ROOT/'src'/name)
 assert sha(HERE/'map_file_load.h')==sha(ROOT/'include/map_file_load.h');files.extend([ROOT/'src'/n for n in source_names]+[ROOT/'include/map_file_load.h'])
 expected={p.resolve() for p in [*[ROOT/'src'/n for n in source_names+root_names],HERE/'record_fixture.c']}
 for k,exe in enumerate(exes):
  target='restore_map_file_load_'+('trace' if k%2==0 else 'plain')+'_verify';assert exe.name==target+'.exe' and HERE not in exe.parents
  project=exe.parent.parent/(target+'.vcxproj');xml=ET.parse(project);ns={'m':'http://schemas.microsoft.com/developer/msbuild/2003'}
  compiles={(project.parent/e.attrib['Include'].replace('\\','/')).resolve() for e in xml.findall('.//m:ClCompile[@Include]',ns)};assert compiles==expected
  groups=[g for g in xml.findall('.//m:ItemDefinitionGroup',ns) if ('Debug' if k<2 else 'Release') in g.get('Condition','')];assert groups
  for g in groups:
   runtime=g.find('m:ClCompile/m:RuntimeLibrary',ns);assert runtime is not None and runtime.text==('MultiThreadedDebug' if k<2 else 'MultiThreaded')
   defs=g.find('m:ClCompile/m:PreprocessorDefinitions',ns);assert defs is not None and 'MAP_FILE_LOAD_PROOF' in defs.text and 'FUN_004d5520=map_file_deep_load' in defs.text and 'FUN_004de600=map_file_deep_start' in defs.text and 'FUN_004ce6e0=map_file_diagnostic' in defs.text
  data=exe.parent/'restore_data.lib';assert data.is_file();files.extend([project,data])
link_probe={}
if not args.production:
 library=ROOT/'build/c-only-debug/Debug/pharaoh_restored.lib';assert sha(library)=='56626f4563d55ae58cba2e380d8343158a76253769e23167ea6f23fe52a11254'
 files.extend([library,ROOT/'src/unit_01829.c',ROOT/'src/unit_01891.c',ROOT/'src/unit_01750.c'])
 owners={'_FUN_004d5520':'pharaoh_restored:unit_01829.obj','_FUN_004de600':'pharaoh_restored:unit_01891.obj','_FUN_004ce6e0':'pharaoh_restored:unit_01750.obj','__open':'libucrtd:open.obj','__close':'libucrtd:close.obj','__chdir':'libucrtd:chdir.obj','__getcwd':'libucrtd:getcwd.obj'}
 for cfg in ['Debug','Release']:
  exe=HERE/'build'/cfg/'map_file_link_probe.exe';mp=exe.with_suffix('.map');lines=mp.read_text().splitlines()
  for symbol,owner in owners.items():
   found=[line for line in lines if symbol in line.split()];assert len(found)==1 and found[0].split()[-1]==owner
  files.extend([exe,mp]);link_probe[cfg]={'path':exe.relative_to(ROOT).as_posix(),'sha256':sha(exe),'bindings':owners,'executed':False}
rel=lambda p:p.relative_to(ROOT).as_posix();before={rel(p):sha(p) for p in files};bins={rel(p):sha(p) for p in exes}
for exe in exes:
 lines=exe.with_suffix('.map').read_text().splitlines()
 for address,(unit,_,_) in spec.items():
  found=[x for x in lines if f'_FUN_{address:08x}' in x.split()];assert len(found)==1 and found[0].split()[-1]==f'unit_{unit:05d}.obj'
start=time.monotonic();procs=[subprocess.Popen([str(p),'--oracle-batch'],stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True) for p in exes];io_total=0
try:
 for p in procs:assert json.loads(p.stdout.readline())=={'fields':16}
 for i,p in enumerate(cases):
  expected=native(p);io_total+=expected['io_count']
  for k,proc in enumerate(procs):
   proc.stdin.write(' '.join(map(str,p))+'\n');proc.stdin.flush();line=proc.stdout.readline();assert line,'C exit:'+proc.stderr.read();actual=json.loads(line)
   want=expected if k%2==0 else dict(expected,trace=hashlib.sha256(b'').hexdigest(),events=0)
   assert actual==want,json.dumps({'case':i,'parameters':p,'variant':str(exes[k]),'actual':actual,'expected':want})
  if (i+1)%100==0:print(f'checked {i+1}/{len(cases)}',flush=True)
finally:
 for p in procs:p.stdin.close();p.wait(timeout=20);assert p.returncode==0,p.stderr.read()
coverage={f'{a:08x}':[sum(instructions[pc]==a for pc in hits),spec[a][1]] for a in spec};missing=set(instructions)-hits
dead={0x4ce287,0x4ce289}
if not args.limit:assert missing==dead,(coverage,sorted(hex(a) for a in missing))
apply={'src/'+n:{'before_sha256':sha(ROOT/'src'/n) if not args.production else None,'after_sha256':sha(HERE/n)} for n in source_names};apply['include/map_file_load.h']={'before_sha256':None,'after_sha256':sha(HERE/'map_file_load.h')}
report={'passed':True,'production':args.production,'checked':len(cases),'comparisons':len(cases)*4,'full_corpus_cases':complete,'coverage':coverage,'certified_unreachable':['004ce287','004ce289'],'unreachable_reason':'After the first three-extension validation, the repeated third comparison cannot mismatch in this stable data domain. The intervening zeroing writes only a disjoint buffer. No internal mutation/return injection.','actual_function_entries':{f'{a:08x}':n for a,n in entries.items()},'boundary_calls':{f'{a:08x}':n for a,n in boundary_counts.items()},'ordered_boundary_calls':io_total,'native_sha256':PIN,'native_exports':{rel(p):sha(p) for p in sorted(evidence)},'original_literal_sha256':{f'{a:08x}':hashlib.sha256(b).hexdigest() for a,b in constants.items()},'compiled_C_sha256':bins[rel(exes[0])],'variants':{name:{'path':rel(p),'sha256':bins[rel(p)],'checked':len(cases)} for name,p in zip(['Debug_trace','Debug_plain','O2_trace','O2_plain'],exes)},'source_hashes':before,'source_drift':[rel(p) for p in files if sha(p)!=before[rel(p)]],'binary_drift':[rel(p) for p in exes if sha(p)!=bins[rel(p)]],'corpus_sha256':hashlib.sha256(json.dumps(cases,separators=(',',':')).encode()).hexdigest(),'elapsed_seconds':round(time.monotonic()-start,2),'apply':apply,'real_C_and_CRT_link_probe':link_probe,'limits':['Nine whole native algorithms385instructions: four rewritten controllers plus five unchanged actual root string leafs. Ordered accesses traced for rewritten controllers; actual root leaf entries and final guarded bytes compared.','Open/close/chdir/getcwd are explicit controlled external CRT boundaries in tests. Production uses own standard CRT, with native binary-read flags; no legacy native-address calls. Real OS/errno/legacy CRT bookkeeping fidelity is not established by this controlled replay.','Deep4D5520/4DE600 load algorithms and4CE6E0 diagnostic UI are named boundaries: exact byte paths/arguments/return ABI and configured postmutations. Normal product calls existing real C; full loader/UI behavior remains unproved.','4099B0 uses DWORD selection, prefetched BYTE mode before final byte copy, post-filecheck mode reload, exact first-dot behavior and a100-byte local path. Only terminated paths fitting100 bytes including NUL tested; no invented truncation/clamp.','4CE3F0 and4CE100 return full EAX0/1; stack/GPR/canary and final guarded global buffers checked. Private /O2 uses Debug data/CRT. No product/frozen edits, GUI, real file I/O or original process.']}
assert not report['source_drift'] and not report['binary_drift'];output=ROOT/'metadata/map-file-load-emulation.json' if args.production else HERE/('file-load-partial.json' if args.limit else 'file-load-proof.json');output.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:report[k] for k in ['passed','checked','comparisons','coverage','ordered_boundary_calls','elapsed_seconds']}))
