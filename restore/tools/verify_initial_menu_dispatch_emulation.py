"""Whole179-instruction initial menu dispatcher against compiled traced/plain C."""
from pathlib import Path
from datetime import datetime,timezone
import argparse,hashlib,itertools,json,struct,subprocess,sys,time,xml.etree.ElementTree as ET
ROOT=Path(__file__).resolve().parents[1];PHASE=ROOT/'artifacts/initial_menu_dispatch_phase'
ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--production',action='store_true');ap.add_argument('--exe',type=Path);ap.add_argument('--optimized-exe',type=Path);ap.add_argument('--limit',type=int);args=ap.parse_args()
sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE,UC_HOOK_MEM_READ,UC_HOOK_MEM_WRITE
from unicorn.x86_const import *
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest();image=(ROOT/'build/smoke-game/Pharaoh.exe').read_bytes()
assert hashlib.sha256(image).hexdigest()=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
u16=lambda p:struct.unpack_from('<H',image,p)[0];u32=lambda p:struct.unpack_from('<I',image,p)[0]
pe=u32(60);opt=pe+24;base=u32(opt+28);sec=opt+u16(pe+20);vm=Uc(UC_ARCH_X86,UC_MODE_32);vm.mem_map(base,(u32(opt+56)+4095)&~4095)
for j in range(u16(pe+6)):
 q=sec+j*40;vm.mem_write(base+u32(q+12),image[u32(q+20):u32(q+20)+u32(q+16)])
STOP=0x33000000;STACK=0x34000000;vm.mem_map(STOP,4096);vm.mem_map(STACK,0x100000)
get=lambda a,n=4:int.from_bytes(vm.mem_read(a,n),'little')
def put(a,v,n=4):vm.mem_write(a,(v&((1<<(8*n))-1)).to_bytes(n,'little'))
d=json.loads((PHASE/'native-provider.json').read_text());assert d['assembly']['total']==179 and not d['assembly']['has_more']
instructions={int(i['address'],16):i for i in d['assembly']['items']};branches={};deep=set();cs=Cs(CS_ARCH_X86,CS_MODE_32)
for a,i in instructions.items():
 raw=bytes.fromhex(i['bytes']);assert bytes(vm.mem_read(a,len(raw)))==raw;i=next(cs.disasm(raw,a,1))
 if i.mnemonic.startswith('j') and i.mnemonic!='jmp':branches[a]={a+i.size,int(i.op_str,16)}
 if i.mnemonic=='call':deep.add(int(i.op_str,16))
assert len(deep)==50;assert bytes(vm.mem_read(0x4d74b8,276))==bytes.fromhex(d['tables']['hex'])
argc={a:0 for a in deep};argc.update({0x520ab0:1,0x55ad30:1,0x530d70:3})
regions=[(0x605930,64),(0xd3a6b0,80),(0xe38dc0,48)];fields={0x605954:4,0x60594c:4,0x605950:4,0xd3a6e4:4,0xd3a6c4:4,0xe38dda:1,0xe38dca:1}
hits=set();edges=set();modes=set();switch_targets=set();reached=set();mutated=set();call_sites=set();p=None;events=[];reads=[];writes=[];previous=None
def mutate(token,ordinal,phase):
 if p[11]!=phase or (p[9] and p[9]!=token) or (p[10] and p[10]!=ordinal):return
 mutated.add((token,phase))
 for bit,a,n,slot in [(1,0xe38dca,1,13),(2,0xd3a6e4,4,14),(4,0xd3a6c4,4,15),(8,0xe38dda,1,16),(16,0x605954,4,17),(32,0x60594c,4,18),(64,0x605950,4,19)]:
  if p[12]&bit:put(a,p[slot],n)
def hook_code(_vm,a,size,_):
 global previous
 if a in deep:
  reached.add(a);sp=vm.reg_read(UC_X86_REG_ESP);ret=get(sp);vals=[get(sp+4+j*4) for j in range(argc[a])]+[0]*(3-argc[a]);mutate(a,len(events)+1,1)
  events.append([a,argc[a]]+vals+[get(0xe38dda,1),get(0xe38dca,1),get(0xd3a6e4),get(0xd3a6c4),get(0x605954),get(0x60594c),get(0x605950)])
  mutate(a,len(events),2);vm.reg_write(UC_X86_REG_EAX,p[20]);vm.reg_write(UC_X86_REG_ECX,p[20]^0x13579bdf);vm.reg_write(UC_X86_REG_EDX,p[20]^0x2468ace0);vm.reg_write(UC_X86_REG_ESP,sp+4);vm.reg_write(UC_X86_REG_EIP,ret);previous=None;return
 assert a in instructions,('unqualified native instruction',hex(a));hits.add(a)
 if previous in branches:edges.add((previous,a));assert a in branches[previous]
 if instructions[a]['instruction'].startswith('CALL '):call_sites.add(a)
 if a==0x4d7200:modes.add(vm.reg_read(UC_X86_REG_EAX))
 if previous==0x4d7206:switch_targets.add(a)
 previous=a
def hook_read(_vm,access,a,n,value,_):
 if a in fields:assert fields[a]==n,(hex(a),n);reads.append([a,n,get(a,n)])
 elif STACK<=a and a+n<=STACK+0x100000:pass
 else:assert 0x4d74b8<=a and a+n<=0x4d75cc,('unqualified data read',hex(a),n)
def hook_write(_vm,access,a,n,value,_):
 if STACK<=a and a+n<=STACK+0x100000:return
 assert a==0xe38dca and n==1,('unqualified data write',hex(a),n);writes.append([a,n,value&255])
vm.hook_add(UC_HOOK_CODE,hook_code);vm.hook_add(UC_HOOK_MEM_READ,hook_read);vm.hook_add(UC_HOOK_MEM_WRITE,hook_write)
def native(case):
 global p,events,reads,writes,previous
 p=case;events=[];reads=[];writes=[]
 for j,(a,n) in enumerate(regions):vm.mem_write(a,bytes((p[8]+j*29+k*37)&255 for k in range(n)))
 for a,n,slot in [(0xe38dca,4,1),(0xd3a6e4,4,2),(0xd3a6c4,4,3),(0xe38dda,1,4),(0x605954,4,5),(0x60594c,4,6),(0x605950,4,7)]:put(a,p[slot],n)
 for repeat in range(p[21]):
  frame=STACK+0x80000;vm.mem_write(frame-0x1000,bytes([0xa5])*0x2000);put(frame,STOP);put(frame+4,p[0]);put(frame+8,0xdec0ad01);previous=None
  for reg,value in [(UC_X86_REG_EBX,0x11111111),(UC_X86_REG_EBP,0x22222222),(UC_X86_REG_ESI,0x33333333),(UC_X86_REG_EDI,0x44444444),(UC_X86_REG_ESP,frame),(UC_X86_REG_EFLAGS,2)]:vm.reg_write(reg,value)
  vm.emu_start(0x4d71c0,STOP,count=10000)
  assert vm.reg_read(UC_X86_REG_EIP)==STOP and vm.reg_read(UC_X86_REG_ESP)==frame+4
  assert [vm.reg_read(reg) for reg in [UC_X86_REG_EBX,UC_X86_REG_EBP,UC_X86_REG_ESI,UC_X86_REG_EDI]]==[0x11111111,0x22222222,0x33333333,0x44444444]
  assert get(frame+4)==p[0] and get(frame+8)==0xdec0ad01
 return {'memory':b''.join(bytes(vm.mem_read(a,n)) for a,n in regions).hex(),'events':events[:],'reads':reads[:],'writes':writes[:]}
cases=[];seen=set()
def case(**kw):
 v=[1,1,0,0,0,70000,0x12345678,0x87654321,17,0,0,0,0,255,256,15,15,0x80000000,0xffffffff,0,0x87654321,1]
 for k,value in kw.items():v[int(k[1:])]=value&0xffffffff
 v=tuple(v)
 if v not in seen:seen.add(v);cases.append(v)
 return v
helper_mode={}
for mode in range(88):
 q=case(v4=mode)
 for event in native(q)['events']:helper_mode.setdefault(event[0],mode)
assert set(helper_mode)==deep
for mode,force,dirty in itertools.product(range(256),[0,1,0x80000000],[0,1]):case(v4=mode,v0=force,v1=dirty,v2=0,v3=0xffffffff)
for mode,flag,force,dirty in itertools.product(list(range(256))+[256,0x10f,0x10000,0x80000000,0xffffffff],[1,256,0x80000000],[0,1],[0,1]):case(v3=mode,v2=flag,v0=force,v1=dirty,v4=15)
for force,dirty,flag in itertools.product([0,1,256,0xffffffff],[0,0xabcdef00,0xabcdef01,255],[0,1,0x10000]):case(v0=force,v1=dirty,v2=flag,v3=0x100,v4=0)
for x,y,rows in itertools.product([0,1,255,256,0x7fffffff,0x80000000,0xffffffff],repeat=3):case(v4=15,v5=x,v6=y,v7=rows)
for token,mode in helper_mode.items():
 for phase,result,repeats in itertools.product([1,2],[0,0x80000000,0xffffffff],[1,3]):case(v4=mode,v9=token,v11=phase,v12=127,v20=result,v21=repeats,v14=256,v15=15)
for mode,ordinal,phase in itertools.product([9,17,40,42,44,76],range(1,5),[1,2]):case(v4=mode,v10=ordinal,v11=phase,v12=127,v21=3,v14=256,v15=15)
complete_count=len(cases);cases=cases[:args.limit] if args.limit else cases
exe=(args.exe or ROOT/'build/initial-menu-dispatch/Debug/restore_initial_menu_dispatch_verify.exe').resolve();opt_exe=(args.optimized_exe or ROOT/'build/initial-menu-dispatch/Release/restore_initial_menu_dispatch_verify.exe').resolve()
product=ROOT if args.production else PHASE
sources=[product/'src/unit_01866.c',product/'include/initial_menu_dispatch.h',product/'runtime/initial_menu_dispatch_boundary.c',ROOT/'include/restore_types.h',ROOT/'include/standalone_data.h',PHASE/'tests/initial_dispatch.c',PHASE/'tests/renderer_boundaries.inc',PHASE/'tests/actual_trace.c',PHASE/'tests/actual_plain.c',PHASE/'native-provider.json',Path(__file__).resolve()]
configuration={};ns={'m':'http://schemas.microsoft.com/developer/msbuild/2003'}
for cfg,path in [('Debug',exe),('Release',opt_exe)]:
 project=path.parent.parent/'restore_initial_menu_dispatch_verify.vcxproj';tree=ET.parse(project);groups=[g for g in tree.findall('m:ItemDefinitionGroup',ns) if "=='%s|Win32'"%cfg in g.get('Condition','')];assert len(groups)==1
 cc=groups[0].find('m:ClCompile',ns);getcc=lambda n:cc.findtext('m:'+n,default='',namespaces=ns)
 assert getcc('Optimization')==('Disabled' if cfg=='Debug' else 'MaxSpeed') and getcc('RuntimeLibrary')==('MultiThreadedDebug' if cfg=='Debug' else 'MultiThreaded')
 assert ('INITIAL_DISPATCH_STAGED' in getcc('PreprocessorDefinitions'))==(not args.production)
 actual={Path(n.get('Include')).resolve() for n in tree.findall('.//m:ClCompile[@Include]',ns)};assert actual=={PHASE/'tests'/n for n in ['initial_dispatch.c','actual_trace.c','actual_plain.c']}
 if args.production:assert path.name=='restore_initial_menu_dispatch_verify.exe' and path.parent.parent.name in ['c-only','c-only-debug']
 for source in sources+[project]:assert path.stat().st_mtime_ns>=source.stat().st_mtime_ns or source==Path(__file__).resolve(),('binary predates source',source)
 configuration[cfg]={'optimization':getcc('Optimization'),'runtime':getcc('RuntimeLibrary'),'project':project.relative_to(ROOT).as_posix()};sources.append(project)
sources=list(dict.fromkeys(sources));before={s.relative_to(ROOT).as_posix():sha(s) for s in sources};binary_before={str(path):sha(path) for path in [exe,opt_exe]}
variants=[('Debug_trace',exe,False),('Debug_plain',exe,True),('O2_trace',opt_exe,False),('O2_plain',opt_exe,True)]
processes=[subprocess.Popen([str(path),'--oracle-plain-batch' if plain else '--oracle-batch'],stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True,cwd=ROOT) for label,path,plain in variants]
report={'format':1,'production':args.production,'passed':False,'complete_case_set':not bool(args.limit),'checked':0,'complete_cases':complete_count,'native_sha256':d['native_sha256'],'compiled_C_path':exe.relative_to(ROOT).as_posix(),'compiled_C_sha256':sha(exe),'source_hashes':before,'configuration':configuration,'variants':{label:{'path':path.relative_to(ROOT).as_posix(),'sha256':sha(path)} for label,path,plain in variants},'original_game_process_executed':False,'original_machine_code_fallback_used':False,'limitations':['Whole initial dispatcher4D71C0 only. Fifty named actual screen-renderer/controller endpoints have exact argument/state/mutation contracts; their pixels and complete downstream algorithms are separate obligations. Production calls the existing C symbols; no product renderer stub is installed.','The native55AD30 context DWORD is preserved at named typed restore_initial_dispatch_training boundary. Its product adapter calls the existing void(void) C renderer and currently discards this context explicitly. Its native meaning/consumption and the deeper renderer are NOT established by this controller proof; no mismatched C declaration or native code-pointer cast is used.','Traced and plain wrappers include the same actual staged/applied body. Only trace instrumentation differs; both configurations check native/C callee-saved registers, stack and full surrounding watched bytes. Incidental void EAX is not claimed.','Original two-level switch tables and every instruction are authenticated. Native mode is full DWORD when D3A6E4 is nonzero, otherwise zero-extended UI BYTE. No signed narrowing or repaired default behavior is invented.']}
started=time.monotonic();checked=0;out=ROOT/'metadata/initial-menu-dispatch-emulation.json' if args.production else PHASE/('partial-emulation.json' if args.limit else 'private-emulation.json')
try:
 assert all(json.loads(o.stdout.readline())=={'fields':22} for o in processes)
 for q in cases:
  expected=native(q)
  for process,(label,path,plain) in zip(processes,variants):
   process.stdin.write(' '.join(map(str,q))+'\n');process.stdin.flush();line=process.stdout.readline();assert line,(label,checked,process.poll(),process.stderr.read());actual=json.loads(line);want=dict(expected,reads=[],writes=[]) if plain else expected
   if actual!=want:(PHASE/'last-difference.json').write_text(json.dumps({'case':q,'variant':label,'C':actual,'native':want},indent=2)+'\n')
   assert actual==want,(label,checked,next((k for k in want if actual[k]!=want[k]),None))
  checked+=1
  if checked%1000==0:print(json.dumps({'checked':checked,'total':len(cases),'elapsed_seconds':round(time.monotonic()-started,1)}),flush=True)
 if not args.limit:
  assert hits==set(instructions) and edges=={(a,b) for a,bs in branches.items() for b in bs}
  assert modes==set(range(88)) and len(switch_targets)==47 and len(call_sites)==58 and reached==deep and mutated=={(a,phase) for a in deep for phase in [1,2]}
 assert before=={s.relative_to(ROOT).as_posix():sha(s) for s in sources},'source drift'
 assert binary_before=={str(path):sha(path) for path in [exe,opt_exe]},'binary drift'
 report['passed']=True
finally:
 for process in processes:
  try:process.stdin.close();process.wait(timeout=5)
  except Exception:process.kill();process.wait()
 report.update(checked=checked,comparisons=checked*4,native_instruction_coverage=[len(hits),179],conditional_outcomes=[len(edges),len(branches)*2],switch_mode_coverage=[len(modes),88],switch_target_coverage=[len(switch_targets),47],native_call_site_coverage=[len(call_sites),58],deep_endpoint_coverage=[len(reached),50],mutation_phases=len(mutated),source_drift=before!={s.relative_to(ROOT).as_posix():sha(s) for s in sources},elapsed_seconds=round(time.monotonic()-started,2))
 out.write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({k:v for k,v in report.items() if k not in ['source_hashes','variants','configuration','limitations']},indent=2))
