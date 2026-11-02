"""Compare whole refresh plus six actual getters against original x86 instructions."""
from pathlib import Path
from datetime import datetime,timezone
import argparse,hashlib,itertools,json,struct,subprocess,sys,time
HERE=Path(__file__).resolve().parent;ROOT=HERE.parents[1]
ap=argparse.ArgumentParser();ap.add_argument('--limit',type=int);ap.add_argument('--case',type=Path);args=ap.parse_args()
sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE,UC_HOOK_MEM_READ,UC_HOOK_MEM_WRITE
from unicorn.x86_const import *
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest();image=(ROOT/'build/smoke-game/Pharaoh.exe').read_bytes();digest=hashlib.sha256(image).hexdigest();assert digest=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
u16=lambda p:struct.unpack_from('<H',image,p)[0];u32=lambda p:struct.unpack_from('<I',image,p)[0];pe=u32(60);opt=pe+24;base=u32(opt+28);sec=opt+u16(pe+20);vm=Uc(UC_ARCH_X86,UC_MODE_32);vm.mem_map(base,(u32(opt+56)+4095)&~4095)
for j in range(u16(pe+6)):
 q=sec+j*40;vm.mem_write(base+u32(q+12),image[u32(q+20):u32(q+20)+u32(q+16)])
STOP=0x33000000;STACK=0x34000000;vm.mem_map(STOP,4096);vm.mem_map(STACK,0x100000)
vm.mem_map(0x4ac24000,0x2000)
get=lambda a,n=4:int.from_bytes(vm.mem_read(a,n),'little')
def put(a,v,n=4):vm.mem_write(a,(v&((1<<(8*n))-1)).to_bytes(n,'little'))
schema=json.loads((HERE/'fixture-schema.json').read_text());fields=schema['fields'];options=schema['options'];ret_slots={int(a,16):i for a,i in schema['returns'].items()};regions=schema['regions'];global_slots={f['address']:j for j,f in enumerate(fields)}
contract=json.loads((HERE/'fixture-contracts.json').read_text());deep={int(e['native_entry'],16):e for e in contract['controlled_endpoints']}
assert len(deep)==96
functions={0x4d75d0:json.loads((HERE/'native-provider.json').read_text())['assembly']}
for f in json.loads((HERE/'getters-native.json').read_text())['records']:functions[int(f['address'],16)]=f['assembly']
functions[0x41dfe0]=json.loads((HERE/'file-query-native.json').read_text())['assembly'];assert sum(f['total'] for f in functions.values())==959
instructions={};branches={};cs=Cs(CS_ARCH_X86,CS_MODE_32)
for fn,f in functions.items():
 assert not f['has_more'] and f['total']==len(f['items'])
 for x in f['items']:
  a=int(x['address'],16);raw=bytes.fromhex(x['bytes']);assert bytes(vm.mem_read(a,len(raw)))==raw;x=dict(x,owner=fn);instructions[a]=x;i=next(cs.disasm(raw,a,1))
  if i.mnemonic.startswith('j') and i.mnemonic!='jmp':branches[a]={a+i.size,int(i.op_str,16)}
assert get(0x51f780,1)==0xc3;instructions[0x51f780]={'instruction':'RET','bytes':'c3','owner':0x51f780}
# Authenticate the retained callee return/tail facts; no deep body is emulated.
cache={}
for file in (ROOT/'metadata/native-function-audit-cache').glob('batch-*.json'):
 for e in json.loads(file.read_text())['entries']:cache[int(e['requested_address'],16)]=e
for e in deep.values():
 assert sha(ROOT/e['native_source'])==e['native_source_sha256']
 for token in [int(e['native_entry'],16)]+[int(a,16) for a in e['tail_return_entries']]:
  f=cache[token]['assembly'];assert not f['has_more']
  for x in f['items']:
   if x['instruction'].startswith(('RET','JMP ')):
    raw=bytes.fromhex(x['bytes']);assert bytes(vm.mem_read(int(x['address'],16),len(raw)))==raw
assert (ROOT/'runtime/mission_menu.c').read_text().split('void restore_mission_menu_buttons(void)',1)[1].split('\n}',1)[0]+'\n}' in (HERE/'tests/actual_mission_buttons.c').read_text()
assert sha(ROOT/'runtime/mission_menu.c')==contract['mission_helper_extraction']['source_sha256']
hits=set();edges=set();reached=set();mutated=set();call_sites=set();native_reads=set();native_writes=set();events=[];p=None;previous=None;last_case=None
def field_value(j):f=fields[j];return get(f['address'],f['width'])
def mutation(token,ordinal,phase):
 if p[options['mutation_phase']]!=phase or (p[options['mutation_token']] and p[options['mutation_token']]!=token) or (p[options['mutation_ordinal']] and p[options['mutation_ordinal']]!=ordinal):return
 j=p[options['mutation_field']];assert 0<=j<len(fields);f=fields[j];put(f['address'],p[options['mutation_value']],f['width']);mutated.add((token,phase))
def result(token,first):
 v=p[ret_slots[token]]
 if token==0x5013a0 and not(p[options['query_mask']]&(1<<(first&31))):v=0
 return v
def hook_code(_vm,a,size,_):
 global previous
 if a in deep:
  e=deep[a];reached.add(a);sp=vm.reg_read(UC_X86_REG_ESP);ret=get(sp);vals=[get(sp+4+j*4) for j in range(e['native_stack_argument_count'])]
  if e['receiver_ECX']:vals.insert(0,vm.reg_read(UC_X86_REG_ECX))
  assert len(vals)==e['normalized_argument_count']<=6;mutation(a,len(events)+1,1)
  events.append([a,len(vals)]+vals+[0]*(6-len(vals))+[field_value(j) for j in range(len(fields))]);mutation(a,len(events),2)
  v=result(a,vals[0] if vals else 0);vm.reg_write(UC_X86_REG_EAX,v);vm.reg_write(UC_X86_REG_ECX,v^0x13579bdf);vm.reg_write(UC_X86_REG_EDX,v^0x2468ace0);vm.reg_write(UC_X86_REG_ESP,sp+4+e['callee_pops']);vm.reg_write(UC_X86_REG_EIP,ret);previous=None;return
 assert a in instructions,('unqualified native instruction',hex(a));hits.add(a)
 if previous in branches:assert a in branches[previous],(hex(previous),hex(a));edges.add((previous,a))
 if instructions[a]['instruction'].startswith('CALL '):call_sites.add(a)
 previous=a
def inside(a,n):return any(start<=a and a+n<=start+length for start,length in regions)
def hook_read(_vm,access,a,n,value,_):
 if STACK<=a and a+n<=STACK+0x100000:return
 assert inside(a,n),('unwatched data read',hex(a),n);native_reads.add((a,n))
def hook_write(_vm,access,a,n,value,_):
 if STACK<=a and a+n<=STACK+0x100000:return
 assert (a,n) in {(0xe38dc9,1),(0xe38df8,1),(0x7056c8,4)},('unexpected owned write',hex(a),n);native_writes.add((a,n))
vm.hook_add(UC_HOOK_CODE,hook_code);vm.hook_add(UC_HOOK_MEM_READ,hook_read);vm.hook_add(UC_HOOK_MEM_WRITE,hook_write)
def native(case):
 global p,events,previous
 p=case;events=[]
 for j,(a,n) in enumerate(regions):vm.mem_write(a,bytes((p[options['seed']]+j*29+k*37)&255 for k in range(n)))
 vm.mem_write(0xea4096,(p[options['training_tag']]&65535).to_bytes(2,'little')*(8*0x24ec))
 vm.mem_write(0x4ac24096,(p[options['synthetic_tag']]&65535).to_bytes(2,'little')*40)
 for j in range(257):put(0x7999e8+j*264,p[options['actor_tag']],2)
 put(0x7999e8+256*264,p[options['actor_high_tag']],2)
 for j in range(8):put(0x77f748+j*98,p[options['record_byte']],1)
 for j in range(36):put(0xf53e70+j*4,0xa0000000+j)
 for j,f in enumerate(fields):put(f['address'],p[j],f['width'])
 for repeat in range(p[options['repeats']]):
  frame=STACK+0x80000;vm.mem_write(frame-0x2000,bytes([0xa5])*0x4000);put(frame,STOP);put(frame+4,0xdec0ad01);previous=None
  for reg,value in [(UC_X86_REG_EAX,0x87654321),(UC_X86_REG_ECX,0x2468ace0),(UC_X86_REG_EDX,0x13579bdf),(UC_X86_REG_EBX,0x11111111),(UC_X86_REG_EBP,0x22222222),(UC_X86_REG_ESI,0x33333333),(UC_X86_REG_EDI,0x44444444),(UC_X86_REG_ESP,frame),(UC_X86_REG_EFLAGS,2)]:vm.reg_write(reg,value)
  vm.emu_start(0x4d75d0,STOP,count=100000)
  assert vm.reg_read(UC_X86_REG_EIP)==STOP and vm.reg_read(UC_X86_REG_ESP)==frame+4
  assert [vm.reg_read(reg) for reg in [UC_X86_REG_EBX,UC_X86_REG_EBP,UC_X86_REG_ESI,UC_X86_REG_EDI]]==[0x11111111,0x22222222,0x33333333,0x44444444]
  assert get(frame+4)==0xdec0ad01
 h=hashlib.sha256()
 for a,n in regions:h.update(struct.pack('<II',a,n));h.update(vm.mem_read(a,n))
 return {'memory':h.hexdigest(),'events':events[:]}
cases=[];seen=set()
def case(base=None,**kw):
 v=list(schema['defaults'] if base is None else base)
 for key,value in kw.items():
  slot=options[key] if key in options else global_slots[int(key[1:],16)] if key.startswith('g') else ret_slots[int(key[1:],16)]
  v[slot]=value&0xffffffff
 v=tuple(v)
 if v not in seen:seen.add(v);cases.append(v)
 return v
for mode in range(256):case(g00e38dda=mode)
for mode in [0,1,7,9,11,12,19,20,21,23,35,40,47,51,52,53,54,59,61,74,81,83]:
 for flag in [0,1,2,3,5,6,7,8,9,10,11,12,13,14,20,21,30,255,256,0x80000000,0xffffffff]:
  for address in ([0x605a3c,0x605a38] if mode==1 else [0x605a44] if mode==7 else [0xeadcc0,0xeadd50,0xf53dd8] if mode==9 else [0x775928,0x775920] if mode==21 else [0xe38e68] if mode==81 else [0xe38df8] if mode==54 else [0xe38dd9] if mode==51 else [0xf53f1c] if mode==11 else [0xf53f20] if mode==12 else [0xd3b180] if mode==19 else [0xd3a6c4] if mode==35 else [0xe3908c]):case(**{'g00e38dda':mode,'g'+f'{address:08x}':flag})
for mode,x,y in itertools.product([1,7,9,11,12,14,15,20,21,23,40,43,47,53,54,59,61,81,83],[0,256,70000,0x7fffffff,0x80000000,0xffffffff],[0,144,0x12345678,0x80000000]):case(g00e38dda=mode,g00e38da8=x,g00e38da4=y,g00f53f0c=x,g00f53f08=y,g006fdca0=x,g006fdc9c=y)
for tag,enabled,selection,mask in itertools.product([0x48,0xc1,0x46,0xbe,0,0x1234,0xffff],[0,1],[0,1,7,15],[0,1,0x55555555,0xffffffff]):case(g00e38dda=9,g00eadcc0=enabled,g00eadd6c=selection,actor_tag=tag,query_mask=mask)
for tag,city,selected in itertools.product([0,1,2,3,0x1234,0xffff],[0,1,3,7],[0,1,7,39]):case(g00e38dda=40,g00eaac28=city,g0060598c=selected,training_tag=tag)
for current,flag,returned in itertools.product([0,1,0x80000000],[0,1,255],[0,1,256,0xffffffff]):case(g00e38dda=0,g00f55b88=current,g00e38e64=flag,r0055f5a1=returned)
for kind,slot,flag,record in itertools.product([0,1,2,3,256],[0,1,7],[0,1],[0,1,255]):case(g00e38dda=21,g00775928=kind,g0077591c=slot,g00775920=flag,record_byte=record)
for city,editor,kind,index,count in itertools.product([0,1,256],[0,1,256],[0,1,2,256],[-1,0,1,7],[0,1,8]):case(g00e38dda=61,g00704db0=city,g00605a3c=editor,g00704dac=kind,g00704dbc=index,g00704c08=count)
for mode,token in [(23,0x41bb30),(53,0x41b8f0),(9,0x5027b0),(9,0x4cde80),(54,0x4261d0),(54,0x4d6740),(81,0x4261d0)]:
 for value in [0,1,2,3,255,256,0x7fffffff,0x80000000,0xffffffff]:case(**{'g00e38dda':mode,'r'+f'{token:08x}':value})
for value in [0,1,255,256,0xffffffff]:case(g00e38dda=52,g007027a4=value)
case(g00e38dda=74,g006fdc54=0x6f2000)
for value in [0,1,256,0x7fffffff,0x80000000,0xffffffff]:case(g00e38dda=9,g00eadcc0=1,actor_tag=0,g00f53dd8=value)
for tag,selected in itertools.product([0,1,2,3,0x1234,0xffff],[0,1,7,39]):case(g00e38dda=40,g00eaac28=65536,g0060598c=selected,synthetic_tag=tag)
for tag,mask in itertools.product([0x48,0xc1,0x46,0xbe,0,0xffff],[0,1,0x55555555,0xffffffff]):case(g00e38dda=9,g00eadcc0=1,g00eadd6c=256,actor_high_tag=tag,query_mask=mask)
# Select actual native routes for helper mutations. Every warmup input is also
# included in the compared corpus; no warmup-only coverage is claimed.
helper_case={}
warmups=list(cases[:256])
for kind in range(15):warmups.append(case(g00e38dda=7,g00605a44=kind))
warmups += [case(g00e38dda=1,g00605a3c=1,g00605a38=k) for k in [0,1]]
warmups += [case(g00e38dda=9,g00eadcc0=1,actor_tag=k) for k in [0x48,0x46]]
warmups += [case(g00e38dda=0,g00f55b88=1,g00e38e64=1),case(g00e38dda=54,g00e38df8=1),case(g00e38dda=74,g006fdc54=0x6f2000)]
for q in warmups:
 for e in native(q)['events']:helper_case.setdefault(e[0],q)
mutation_values={0xe38dda:40,0xe38da8:0x7fffffff,0xe38da4:0x80000000,0x605a44:2,0xeaac28:65536,0xeadd6c:256,0x77591c:7,0x60598c:39,0x6fdc54:0x6f2000}
for token,q in helper_case.items():
 for phase,j in itertools.product([1,2],range(len(fields))):
  f=fields[j];value=mutation_values.get(f['address'],255 if f['width']==1 else 0x80000000)
  case(base=q,mutation_token=token,mutation_phase=phase,mutation_field=j,mutation_value=value)
for mode in [0,1,7,9,15,23,40,54,61,81]:case(g00e38dda=mode,mutation_token=0,mutation_phase=2,mutation_field=global_slots[0xe38dda],mutation_value=15,repeats=3)
if args.case:cases=[tuple(json.loads(args.case.read_text())['case'])]
complete_count=len(cases);cases=cases[:args.limit] if args.limit else cases
executables=[ROOT/'build/refresh-menu-dispatch'/cfg/'restore_refresh_menu_dispatch_verify.exe' for cfg in ['Debug','Release']]
sources=[HERE/'src/unit_01867.c',HERE/'include/refresh_menu_dispatch.h',HERE/'runtime/refresh_menu_dispatch_boundary.c',HERE/'tests/refresh_dispatch.c',HERE/'tests/boundaries.inc',HERE/'tests/fixture_schema.h',HERE/'tests/actual_mission_buttons.c',HERE/'fixture-schema.json',HERE/'fixture-contracts.json',HERE/'native-provider.json',HERE/'getters-native.json',HERE/'file-query-native.json',Path(__file__)]+[ROOT/'src'/f'unit_{n:05d}.c' for n in [286,293,295,296,297,2276]]+[ROOT/'runtime/mission_menu.c',ROOT/'include/restore_types.h',ROOT/'include/file_list.h',ROOT/'include/standalone_data.h',HERE/'project/CMakeLists.txt']
before={p.relative_to(ROOT).as_posix():sha(p) for p in sources};binaries={p.relative_to(ROOT).as_posix():sha(p) for p in executables}
processes=[subprocess.Popen([str(path),'--oracle-batch'],stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True,cwd=ROOT) for path in executables]
report={'format':1,'production':False,'passed':False,'complete_case_set':not(args.limit or args.case),'checked':0,'complete_cases':complete_count,'native_sha256':digest,'source_hashes':before,'binary_hashes':binaries,'native_game_executed':False,'limitations':['Whole controller, six real getters and exact copied shared foreground helper. Ninety-six named complex render/audio/video/query/button endpoints are controlled; their full algorithms are not claimed.','Full watched byte hashes/guards and ordered calls/arguments/global state are compared. Ordinary C read ordering and unobservable intermediate store traces are not claimed; native data reads/writes are restricted to watched globals/records or the private stack.','Native/C saved registers and ESP are checked; void incidental EAX is not forced to match. Valid prepared city/resource/actor/record domains are required; unsafe original selectors are not silently clamped.','Unchanged295 has a single known pointer-return C4133 suppressed only in this fixture source; actual constant57CD70 native return is composed.']}
checked=0;last_case=None;start=time.monotonic();out=HERE/('partial-emulation.json' if args.limit or args.case else 'private-emulation.json')
try:
 assert all(json.loads(o.stdout.readline())=={'fields':schema['parameter_count']} for o in processes)
 for q in cases:
  last_case=q;expected=native(q)
  for cfg,process in zip(['Debug','O2'],processes):
   process.stdin.write(' '.join(map(str,q))+'\n');process.stdin.flush();line=process.stdout.readline();assert line,(cfg,checked,process.poll(),process.stderr.read());actual=json.loads(line)
   if actual!=expected:(HERE/'last-difference.json').write_text(json.dumps({'case':q,'variant':cfg,'C':actual,'native':expected},indent=2)+'\n')
   assert actual==expected,(cfg,checked,'memory' if actual['memory']!=expected['memory'] else 'events')
  checked+=1
  if checked%500==0:print(json.dumps({'checked':checked,'total':len(cases),'elapsed_seconds':round(time.monotonic()-start,1)}),flush=True)
 if not(args.limit or args.case):
  assert set(instructions)<=hits,('uncovered instructions',[hex(a) for a in sorted(set(instructions)-hits)])
  assert {(a,b) for a,bs in branches.items() for b in bs}<=edges,('uncovered conditional outcomes',[(hex(a),hex(b)) for a,bs in branches.items() for b in bs if (a,b) not in edges])
 assert before=={p.relative_to(ROOT).as_posix():sha(p) for p in sources},'source drift'
 assert binaries=={p.relative_to(ROOT).as_posix():sha(p) for p in executables},'binary drift'
 report['passed']=True
except Exception as error:report.update(error=str(error),last_case=last_case,native_eip=hex(vm.reg_read(UC_X86_REG_EIP)));raise
finally:
 for process in processes:
  try:process.stdin.close();process.wait(timeout=5)
  except Exception:process.kill();process.wait()
  if process.returncode!=0:report['passed']=False
 missing={(a,b) for a,bs in branches.items() for b in bs}-edges
 report.update(checked=checked,comparisons=checked*2,native_instruction_coverage=[len(hits),len(instructions)],controller_instruction_coverage=[len(hits&{a for a,x in instructions.items() if x['owner']==0x4d75d0}),917],conditional_outcomes=[len(edges),len(branches)*2],uncovered_instructions=[hex(a) for a in sorted(set(instructions)-hits)],missing_conditional_outcomes=[[hex(a),hex(b)] for a,b in sorted(missing)],reached_endpoints=[hex(a) for a in sorted(reached)],mutated_endpoint_phases=[[hex(a),phase] for a,phase in sorted(mutated)],native_writes=[[hex(a),n] for a,n in sorted(native_writes)],elapsed_seconds=round(time.monotonic()-start,2))
 out.write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({k:v for k,v in report.items() if k not in ['source_hashes','binary_hashes','limitations','last_case','reached_endpoints','mutated_endpoint_phases']},indent=2));sys.exit(0 if report['passed'] else 1)
