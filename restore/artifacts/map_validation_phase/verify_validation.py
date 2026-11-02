"""Whole 442A40 and six actual algorithm helpers; named message-renderer boundary."""
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
spec={0x442a40:(612,294,1),0x442870:(611,130,1),0x40c8b0:(93,44,1),0x443000:(615,21,2),0x4437b0:(622,47,1),0x443820:(623,81,1),0x443a80:(628,31,1)}
functions={};instructions={};evidence=set();al_calls={0x442870:0,0x40c8b0:0}
index=ROOT/'metadata/native-function-audit-cache/index.json';catalog=json.loads(index.read_text());assert catalog['native_sha256']==PIN and catalog['exact_functions']==4527
for name in catalog['batches']:
 p=ROOT/name;d=json.loads(p.read_text());assert d['native_sha256']==PIN
 for e in d['entries']:
  address=int(e['requested_address'],16);items=e['assembly']['items']
  if address in spec:
   assert e['status']=='exact_ghidra_function' and not e['assembly']['has_more'] and len(items)==spec[address][1]
   functions[address]=e;evidence.add(p)
   for it in items:
    a=int(it['address'],16);b=bytes.fromhex(it['bytes']);assert bytes(vm.mem_read(a,len(b)))==b;instructions[a]=address
  for i,it in enumerate(items):
   for target in al_calls:
    if it['instruction'].lower()==f'call 0x{target:08x}':
     assert items[i+1]['instruction']=='ADD ESP,0x4' and items[i+2]['instruction']=='TEST AL,AL';al_calls[target]+=1;evidence.add(p)
assert len(functions)==7 and len(instructions)==648 and al_calls=={0x442870:27,0x40c8b0:2}
jump_bytes=bytes(vm.mem_read(0x4429f0,76));targets=struct.unpack('<11I',jump_bytes[:44]);mapper=jump_bytes[44:]
mapping={2:9,9:1,10:29,13:11,15:14,17:16,19:18,23:22,28:20,33:32}
target_values={0x442896:14,0x44289d:22,0x4428a4:16,0x4428ab:32,0x4428b2:29,0x4428b9:20,0x4428c0:18,0x4428c7:9,0x4428ce:1,0x4428d5:11}
for kind in range(2,34):
 target=targets[mapper[kind-2]];assert target_values.get(target,kind)==mapping.get(kind,kind)
regions=[(0x77f738,19632),(0x784cec,0x11a),(0x5d19ae,0x450),(0xe38dba,48)]
STACK=0x34000000;STOP=0x33000000;vm.mem_map(STACK,0x100000);vm.mem_map(STOP,4096)
read=lambda a,n=4:int.from_bytes(vm.mem_read(a&MASK,n),'little')
def put(a,v,n=4):vm.mem_write(a&MASK,(v&((1<<(8*n))-1)).to_bytes(n,'little'))
events=[];messages=[];hits=set();running=False;entries={a:0 for a in spec}
def code(cpu,a,n,user):
 if not running or a==STOP:return
 if a==0x5344c0:
  sp=cpu.reg_read(UC_X86_REG_ESP);messages.append(read(sp+4))
  # Qualified renderer boundary: observe message, preserve legacy memory.
  for r,v in [(UC_X86_REG_EAX,0xfacede10),(UC_X86_REG_ECX,0x89abcde0),(UC_X86_REG_EDX,0x53535353)]:cpu.reg_write(r,v)
  cpu.reg_write(UC_X86_REG_EIP,read(sp));cpu.reg_write(UC_X86_REG_ESP,sp+4);return
 assert a in instructions,hex(a);hits.add(a)
 if a in spec:
  sp=cpu.reg_read(UC_X86_REG_ESP);events.append((2,a,read(sp+4),read(sp+8) if spec[a][2]==2 else 0));entries[a]+=1
def memory(cpu,access,a,n,value,user):
 if not running or STACK<=a<STACK+0x100000:return
 if access==UC_MEM_READ and 0x4429f0<=a and a+n<=0x442a3c:return
 assert any(lo<=a and a+n<=lo+size for lo,size in regions),(hex(cpu.reg_read(UC_X86_REG_EIP)),hex(a),n)
 events.append((int(access==UC_MEM_WRITE),a,n,(value if access==UC_MEM_WRITE else read(a,n))&((1<<(n*8))-1)))
vm.hook_add(UC_HOOK_CODE,code);vm.hook_add(UC_HOOK_MEM_READ,memory);vm.hook_add(UC_HOOK_MEM_WRITE,memory)
patterns=[[0]*14,list(range(1,15)),list(range(14,0,-1)),[0,2,2,1,9,0,9,255,128,0,2,8,8,1],[2]*14,[0]*14,[0]*14,[1,9,8,11,13,15,17,20,23,28,29,31,33,34],[14,16,18,22,20,29,32,11,1,9,8,31,13,34],[0]*14,[255]*14,[255,1,128,2,0,200,128,1,0,2,200,255,3,3],[31,13,1,11,9,0,0,0,0,0,0,0,0,0]]
second=[[0]*8,list(range(8,0,-1)),[0,2,2,255,128,0,128,2],[0]*8,[0,255,1,128,1,0,2,2]]
flags=[0x784d0c,0x784cfc,0x784d3a,0x784d00,0x784d02,0x784d32,0x784d34,0x784d2a,0x784d2c,0x784d2e,0x784d30,0x784d28]
def inventory(row,pattern,p):
 for i in range(14):
  value=patterns[pattern][i]
  if pattern==5:value=p[2]&255
  if pattern==6:value=p[2]&255 if i%2 else 0
  if pattern==9:value=p[2]&255 if i==13 else 0
  put(0x77f766+row*98+i,value,1)
def prepare(p):
 for i,(a,n) in enumerate(regions):vm.mem_write(a,bytes([(p[13]+37*i)&255])*n)
 for i in range(200):
  r=0x77f748+i*98;put(r,1,1);put(r+1,int(p[14] in [1,2,6]),1);put(r+24,1+i%3*2 if p[14]==2 else i%7 if p[14]==6 else 0,1);inventory(i,7 if p[14]==6 else 0,p);vm.mem_write(r+52,b'\0'*8)
 r=0x77f748+p[3]*98;put(r,p[6],1);put(r+1,p[5],1);put(r+24,p[4],1);inventory(p[3],p[7],p)
 for i in range(8):put(r+52+i,p[2]&255 if p[8]==3 else second[p[8]][i],1)
 if p[14] in [3,4,5]:
  other=0 if p[3]==199 else 199;r=0x77f748+other*98;put(r+1,1,1);put(r+24,p[14]-3,1);inventory(other,8,p)
 if p[14]==2:inventory(199,9,p)
 for i,a in enumerate(flags):put(a,p[10] if p[9]>>i&1 else 0,2)
 for product in range(-1,64):
  for j in range(3):put(0x5d19ce+product*16+j*2,0,2)
 for a,v in [(0x5d19ce+16,p[15]),(0x5d19ce+34,p[15]),(0x5d19ce+52,p[15]),(0x5d19ce+64,p[15]),(0x5d19ce+66,p[2]),(0x5d19ce+33*16,65535)]:put(a,v,2)
 put(0x784de0,1 if p[11] in [1,4] else 33 if p[11]==5 else 65535 if p[11]==6 else 0,2);put(0x784de2,2 if p[11] in [2,4] else 0,2);put(0x784de4,3 if p[11] in [3,4] else 0,2)
 put(0xe38dda,p[12],1);put(0xe38dca,p[13],1)
mode_entries=[0x442a40,0x442870,0x40c8b0,0x443000,0x4437b0,0x443820,0x443a80]
def native(p):
 global events,messages,running
 prepare(p);sp=STACK+0xf0000;put(sp,STOP);put(sp+4,p[1]);argc=2 if p[0]==3 else 1
 if argc==2:put(sp+8,p[2])
 canary=sp+4+argc*4;put(canary,0xbaadf00d)
 saved=[(UC_X86_REG_EBX,0x11111111),(UC_X86_REG_EBP,0x22222222),(UC_X86_REG_ESI,0x33333333),(UC_X86_REG_EDI,0x44444444)]
 for r,v in saved:vm.reg_write(r,v)
 for r,v in [(UC_X86_REG_EAX,0xdeadbeef),(UC_X86_REG_ECX,0xbadc0de),(UC_X86_REG_EDX,0xfefefefe),(UC_X86_REG_ESP,sp),(UC_X86_REG_EFLAGS,2)]:vm.reg_write(r,v)
 events=[];messages=[];running=True
 try:vm.emu_start(mode_entries[p[0]],STOP,count=3000000)
 finally:running=False
 assert vm.reg_read(UC_X86_REG_EIP)==STOP and vm.reg_read(UC_X86_REG_ESP)==sp+4 and read(canary)==0xbaadf00d and all(vm.reg_read(r)==v for r,v in saved)
 digest=hashlib.sha256()
 for a,n in regions:digest.update(struct.pack('<II',a,n));digest.update(vm.mem_read(a,n))
 trace=hashlib.sha256()
 for e in events:trace.update(struct.pack('<4I',*e))
 result=0 if p[0]==0 else vm.reg_read(UC_X86_REG_EAX)&(255 if p[0] in [1,2] else MASK)
 return {'result':result,'memory':digest.hexdigest(),'trace':trace.hexdigest(),'events':len(events),'messages':messages.copy()}
DEFAULT=[0,0,2,0,0,1,1,0,0,0,1,0,0,17,0,9];cases=[];categories=[]
def add(category,**kw):
 p=DEFAULT.copy()
 for k,v in kw.items():p[int(k[1:])]=v&MASK
 cases.append(p);categories.append(category)
for row,pattern,other in itertools.product([0,199],range(13),[0,2,4]):add('whole_unsigned_sort_and_duplicate_removal',p1=row,p3=row,p7=pattern,p8=other)
for kind in list(range(36))+[256,65535,0xffffffff]:
 add('capability_none',p0=1,p1=kind,p5=0)
 add('capability_direct_inventory',p0=1,p1=kind,p2=kind,p7=5)
 add('capability_mapped_state_one_WORD_flags',p0=1,p1=kind,p2=mapping.get(kind,kind),p4=1,p7=5,p9=(1<<5)|(1<<6),p10=256)
 add('capability_mapped_state_zero',p0=1,p1=kind,p2=mapping.get(kind,kind),p7=5)
for kind,pattern,value in [(12,12,0),(12,5,9),(12,5,11),(34,12,0),(34,5,31),(34,5,13)]:add('recursive_capability_conjunction',p0=1,p1=kind,p7=pattern,p2=value)
for kind,other in itertools.product([15,17,33,19,13,10,28,23],[3,4]):add('persistent_scanner_mapping_actual_dependencies',p0=5,p1=kind,p14=other)
for state in [0,1,2,3,4,5,255]:add('actual_135_state_scanner',p0=4,p1=9,p2=9,p4=state,p7=5)
for kind,active in itertools.product([0,1,255],[0,255]):add('actual_all_state_scanner',p0=6,p1=33,p2=33,p6=kind,p5=active,p7=9,p3=199)
for row,kind in itertools.product([0,199],[0,1,2,255,256,0xffffffff]):add('DWORD_target_and_record_stride',p0=3,p1=row,p3=row,p2=kind,p7=9)
for product,requirement,pattern in itertools.product([0,1,2,3,4,33,0xffffffff],[0,9,12,34,65535],[0,12]):add('signed_WORD_product_and_16byte_stride',p0=2,p1=product,p15=requirement,p7=pattern)
for flag,base,provider in itertools.product(range(11),[0,1],[0,7]):add('whole_WORD_flags_and_ordered_messages',p9=(1<<flag)|(base<<11),p10=256,p7=provider,p4=1)
for flag,dependency in [(7,5),(9,6)]:add('whole_dependency_passes',p9=(1<<flag)|(1<<dependency)|(1<<11),p10=256,p4=1,p7=7)
for products,initial in itertools.product(range(1,7),[0,0x47]):add('whole_three_product_sticky_error',p11=products,p12=initial,p15=9)
for state in [0,1,3,5]:add('whole200_active_records',p4=state,p14=6,p7=3,p8=4,p9=4095,p10=256)
complete=len(cases)
if args.start:assert args.limit and not args.production;cases=cases[args.start:];categories=categories[args.start:]
if args.limit:cases=cases[:args.limit];categories=categories[:args.limit]
provided=[args.exe,args.plain_exe,args.optimized_exe,args.optimized_plain_exe]
if args.production:assert all(provided) and not args.limit
exes=[(p or HERE/'build'/cfg/f'map_validation_{kind}.exe').resolve() for p,(cfg,kind) in zip(provided,itertools.product(['Debug','Release'],['trace','plain']))]
source_names=[f'unit_{spec[a][0]:05d}.c' for a in spec]
files=[Path(__file__),index,*sorted(evidence),HERE/'CMakeLists.txt',HERE/'record_fixture.c',HERE/'link_probe.c',HERE/'map_validation.h',*[HERE/n for n in source_names],ROOT/'include/standalone_data.h',ROOT/'build/c-only-debug/Debug/restore_data.lib']
if args.production:
 assert args.proof_sha256 and sha(HERE/'validation-proof.json')==args.proof_sha256
 for name in source_names:assert sha(HERE/name)==sha(ROOT/'src'/name)
 assert sha(HERE/'map_validation.h')==sha(ROOT/'include/map_validation.h');files.extend([ROOT/'src'/n for n in source_names]+[ROOT/'include/map_validation.h'])
 expected={p.resolve() for p in [*[ROOT/'src'/n for n in source_names],HERE/'record_fixture.c']}
 for k,exe in enumerate(exes):
  target='restore_map_validation_'+('trace' if k%2==0 else 'plain')+'_verify';assert exe.name==target+'.exe' and HERE not in exe.parents
  project=exe.parent.parent/(target+'.vcxproj');xml=ET.parse(project);ns={'m':'http://schemas.microsoft.com/developer/msbuild/2003'}
  compiles={(project.parent/e.attrib['Include'].replace('\\','/')).resolve() for e in xml.findall('.//m:ClCompile[@Include]',ns)};assert compiles==expected
  groups=[g for g in xml.findall('.//m:ItemDefinitionGroup',ns) if ('Debug' if k<2 else 'Release') in g.get('Condition','')];assert groups
  for g in groups:
   runtime=g.find('m:ClCompile/m:RuntimeLibrary',ns);assert runtime is not None and runtime.text==('MultiThreadedDebug' if k<2 else 'MultiThreaded')
   defs=g.find('m:ClCompile/m:PreprocessorDefinitions',ns);assert defs is not None and 'FUN_005344c0=map_validation_message_boundary' in defs.text
  data=exe.parent/'restore_data.lib';assert data.is_file();files.extend([project,data])
link_probe={}
if not args.production:
 library=ROOT/'build/c-only-debug/Debug/pharaoh_restored.lib'
 assert sha(library)=='fed38ab7c23a488b344afff486e88d99b5b884ac48b58a212bd93f23b5ca4b83'
 files.extend([library,ROOT/'src/unit_02435.c'])
 for config in ['Debug','Release']:
  exe=HERE/'build'/config/'map_validation_link_probe.exe';mp=exe.with_suffix('.map')
  found=[line for line in mp.read_text().splitlines() if '_FUN_005344c0' in line.split()]
  assert len(found)==1 and found[0].split()[-1]=='pharaoh_restored:unit_02435.obj'
  files.extend([exe,mp]);link_probe[config]={'path':exe.relative_to(ROOT).as_posix(),'sha256':sha(exe),'renderer_owner':'pharaoh_restored:unit_02435.obj','executed':False}
rel=lambda p:p.relative_to(ROOT).as_posix();before={rel(p):sha(p) for p in files};bins={rel(p):sha(p) for p in exes}
for exe in exes:
 lines=exe.with_suffix('.map').read_text().splitlines()
 for address,(unit,_,_) in spec.items():
  found=[x for x in lines if f'_FUN_{address:08x}' in x.split()];assert len(found)==1 and found[0].split()[-1]==f'unit_{unit:05d}.obj'
start=time.monotonic();procs=[subprocess.Popen([str(p),'--oracle-batch'],stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True) for p in exes];message_total=0
try:
 for p in procs:assert json.loads(p.stdout.readline())=={'fields':16}
 for i,p in enumerate(cases):
  expected=native(p);message_total+=len(expected['messages'])
  for k,proc in enumerate(procs):
   proc.stdin.write(' '.join(map(str,p))+'\n');proc.stdin.flush();line=proc.stdout.readline();assert line,'C exit:'+proc.stderr.read();actual=json.loads(line)
   want=expected if k%2==0 else dict(expected,trace=hashlib.sha256(b'').hexdigest(),events=0)
   assert actual==want,json.dumps({'case':i,'parameters':p,'variant':str(exes[k]),'actual':actual,'expected':want})
  if (i+1)%100==0:print(f'checked {i+1}/{len(cases)}',flush=True)
finally:
 for p in procs:p.stdin.close();p.wait(timeout=20);assert p.returncode==0,p.stderr.read()
coverage={f'{a:08x}':[sum(instructions[pc]==a for pc in hits),spec[a][1]] for a in spec};missing=set(instructions)-hits
if not args.limit:assert missing=={0x442a8b},(coverage,sorted(hex(a) for a in missing))
apply={'src/'+n:{'before_sha256':sha(ROOT/'src'/n) if not args.production else None,'after_sha256':sha(HERE/n)} for n in source_names};apply['include/map_validation.h']={'before_sha256':None,'after_sha256':sha(HERE/'map_validation.h')}
report={'passed':True,'production':args.production,'checked':len(cases),'comparisons':len(cases)*4,'full_corpus_cases':complete,'coverage':coverage,'certified_unreachable':['00442a8b'],'unreachable_reason':'In the stable single-thread fixture domain, 443000(record,2) true guarantees the subsequent fourteen-byte search finds2; intervening actual capability callees are read-only. No internal return/value injection. Search-not-found JMP retained in C control flow.','AL_only_cached_consumers':{f'{a:08x}':n for a,n in al_calls.items()},'actual_function_entries':{f'{a:08x}':n for a,n in entries.items()},'ordered_render_boundary_messages':message_total,'native_sha256':PIN,'native_exports':{rel(p):sha(p) for p in sorted(evidence)},'switch_sha256':hashlib.sha256(jump_bytes).hexdigest(),'compiled_C_sha256':bins[rel(exes[0])],'variants':{name:{'path':rel(p),'sha256':bins[rel(p)],'checked':len(cases)} for name,p in zip(['Debug_trace','Debug_plain','O2_trace','O2_plain'],exes)},'source_hashes':before,'source_drift':[rel(p) for p in files if sha(p)!=before[rel(p)]],'binary_drift':[rel(p) for p in exes if sha(p)!=bins[rel(p)]],'corpus_sha256':hashlib.sha256(json.dumps(cases,separators=(',',':')).encode()).hexdigest(),'elapsed_seconds':round(time.monotonic()-start,2),'apply':apply,'real_C_renderer_link_probe':link_probe,'limits':['Seven actual complete C algorithms,648 native instructions; no internal algorithm boundary. C/native memory, ordered BYTE/WORD reads/writes and actual function entries compared.','5344C0 message rendering is explicitly controlled only in the local proof: ordered message arguments, no legacy postmutation. Normal product calls the real existing C renderer. Its graphics/text/deep drawing fidelity is unproved here.','442870 and40C8B0 return AL only; uint8_t ABI,27+2 cached callers all TEST AL. Full EAX carry is not canonicalized or promised. Other four query helpers return full EAX0/1; main is void.','200-record byte stride98, signed WORD product IDs/requirements at stride16, WORD option flags, unsigned byte sorting/dedup, sticky error propagation and missing dirty-store on messageBD preserved. Callee-saved registers/stack/canary checked.','Finite fixtures including signed-1 mapped product sentinel; no exhaustive all-state or concurrent mutation claim. Private /O2 uses Debug data/CRT. No product edits, GUI or original process.']}
assert not report['source_drift'] and not report['binary_drift'];output=ROOT/'metadata/map-validation-emulation.json' if args.production else HERE/('validation-partial.json' if args.limit else 'validation-proof.json');output.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:report[k] for k in ['passed','checked','comparisons','coverage','ordered_render_boundary_messages','elapsed_seconds']}))
