"""Closed real training-query cluster versus pinned x86; no controlled algorithm endpoints."""
from pathlib import Path
import argparse,hashlib,itertools,json,re,struct,subprocess,sys,time
ROOT=Path(__file__).resolve().parent.parent;PHASE=ROOT/'artifacts/training_queries_phase'
parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--exe',type=Path);parser.add_argument('--optimized-exe',type=Path);parser.add_argument('--production',action='store_true');parser.add_argument('--limit',type=int);parser.add_argument('--start',type=int,default=0);parser.add_argument('--case',type=Path);args=parser.parse_args()
sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE,UC_HOOK_BLOCK,UC_HOOK_MEM_READ,UC_HOOK_MEM_WRITE
from unicorn.x86_const import *
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
sha=lambda b:hashlib.sha256(b).hexdigest();MASK=0xffffffff;AUX=0x4ac20000;STOP=0x33000000;STACK=0x34000000
schema=(PHASE/'tests/query_case.h').read_text();names=[x.lower() for x in re.findall(r'\bP_([A-Z0-9_]+)\b',schema.split('enum{',1)[1].split('};',1)[0]) if x!='FIELDS'];FIELDS=len(names);globals().update({'P_'+name.upper():j for j,name in enumerate(names)})
image=(ROOT/'build/smoke-game/Pharaoh.exe').read_bytes();digest=sha(image);assert digest=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
u16=lambda p:struct.unpack_from('<H',image,p)[0];u32=lambda p:struct.unpack_from('<I',image,p)[0];pe=u32(60);opt=pe+24;sec=opt+u16(pe+20);base=u32(opt+28);size=u32(opt+56)
vm=Uc(UC_ARCH_X86,UC_MODE_32);vm.mem_map(base,(size+4095)&~4095)
for j in range(u16(pe+6)):
 q=sec+j*40;vm.mem_write(base+u32(q+12),image[u32(q+20):u32(q+20)+u32(q+16)])
for a,n in [(AUX,0x10000),(STOP,4096),(STACK,0x100000)]:vm.mem_map(a,n)
def read(a,n=4):return int.from_bytes(vm.mem_read(a&MASK,n),'little')
def write(a,v,n=4):vm.mem_write(a&MASK,(v&((1<<(n*8))-1)).to_bytes(n,'little'))
def signed(v):return (v&0x7fffffff)-(v&0x80000000)
def valid_city(city):return city<4 or city==65536
def fee_address(category,resource):return(0x77646c+(category*36+resource)*4)&MASK
def timers(city,t0,t1,t2):
 assert valid_city(city);offset=city*0x49d8
 for a,v in [(0xea61f4+offset,t0),(0xea61f6+offset,t1),(0xea61f8+offset,t2)]:write(a,v,2)
def flags(low,high):return low|(high<<32)
def record(row,enabled,typ,category,present,supply,demand):
 assert row<61;a=0x77de00+row*106;vm.mem_write(a,bytes(106))
 for off,v in [(0,enabled),(2,typ),(4,category),(5,present)]:write(a+off,v,1)
 for j in range(36):write(a+0x2a+j,(supply>>j)&1,1);write(a+6+j,(demand>>j)&1,1)
packet=json.loads((PHASE/'retained-native.json').read_text());assert packet['native_sha256']==digest
functions={int(a,16):f['assembly']['items'] for a,f in packet['functions'].items()}
instructions={int(i['address'],16):i for body in functions.values() for i in body};decoded={};branches={};cap=Cs(CS_ARCH_X86,CS_MODE_32)
for a,i in instructions.items():
 encoded=bytes.fromhex(i['bytes']);assert bytes(vm.mem_read(a,len(encoded)))==encoded;d=next(cap.disasm(encoded,a));decoded[a]=d
 if d.mnemonic.startswith('j') and d.mnemonic!='jmp':branches[a]=(a+d.size,int(d.op_str,16))
for f in packet['functions'].values():assert not f['assembly']['has_more']
assert len(instructions)==517
caller_evidence=json.loads((PHASE/'447790-callers.json').read_text());assert caller_evidence['native_sha256']==digest and len(caller_evidence['sites'])==21
for site in caller_evidence['sites']:
 a=int(site['call_address'],16);assert bytes(vm.mem_read(a,5))==bytes.fromhex(site['bytes']) and bytes(vm.mem_read(a+5,5))==bytes.fromhex('83c40884c0');assert site['cached_owners'] and all(not owner['has_more'] for owner in site['cached_owners'])
entries=[0x443950,0x4439c0,0x443a20,0x443b20,0x443b90,0x443d90,0x447790,0x4c5240,0x4c5190];saved_regs=[UC_X86_REG_EBX,UC_X86_REG_EBP,UC_X86_REG_ESI,UC_X86_REG_EDI]
owned_functions=set(entries)-{0x4c5240};owned={int(i['address'],16) for fn in owned_functions for i in functions[fn]};regions=[(0x77644c,0x9350),(0x784d22,0x24),(0xea61e4,0x12790),(0x4ac261e4,0x30)]
events=[];frames=[];hits=set();edges=set();ordinary_edges=set();case_edges=set();mutated=set();raw_eligibility_returns=set();params=None;seen=0;case_mutation_applied=False;last_block=None;block_cache={};fill_cache={};fee_cache={}
def observe(fn,phase,values,result=0):
 global seen,case_mutation_applied
 p=params;city=read(0xeaac28);assert valid_city(city);offset=city*0x49d8;first=0x77de00+p[P_ROW]*106;zero=0x77de00+p[P_ROW2]*106;changed=0x77de00+p[P_MUTROW]*106
 e=[fn,phase,len(values)]+list(values)+[0]*(2-len(values))+[result,city,read(0xea61f4+offset,2),read(0xea61f6+offset,2),read(0xea61f8+offset,2),read(0x784d32,2),read(0x784d34,2)]
 e +=[read(first+off,1) for off in [0,2,4,5]]+[read(first+0x2a+p[P_RESOURCE],1),read(first+6+p[P_RESOURCE],1)]
 e +=[read(zero+off,1) for off in [0,2,4,5]]+[read(zero+0x2a+p[P_RESOURCE],1),read(zero+6+p[P_RESOURCE],1)]
 e +=[read(fee_address(read(first+4,1),p[P_RESOURCE])),read(fee_address(read(zero+4,1),p[P_RESOURCE])),p[P_ROW],p[P_ROW2],read(fee_address(p[P_CATEGORY]&255,p[P_RESOURCE]))]
 offset=p[P_CITY]*0x49d8;e +=[read(0xea61f4+offset,2),read(0xea61f6+offset,2),read(0xea61f8+offset,2)]
 e +=[read(changed+off,1) for off in [0,2,4,5]]+[read(changed+0x2a+p[P_RESOURCE],1),read(changed+6+p[P_RESOURCE],1),read(fee_address(p[P_NEWCATEGORY]&255,p[P_COSTRESOURCE])),p[P_RESOURCE]]
 assert len(e)==40;events.append(e)
 if fn==p[P_MFN] and phase==p[P_MPHASE]:
  seen+=1
  if seen==p[P_MOCC]:
   bits=p[P_MBITS];mutated.add((fn,phase));case_mutation_applied=True
   if bits&1:assert valid_city(p[P_NEWCITY]);write(0xeaac28,p[P_NEWCITY]);timers(p[P_NEWCITY],p[P_NEWT0],p[P_NEWT1],p[P_NEWT2])
   if bits&2:record(p[P_MUTROW],p[P_NEWENABLED],p[P_NEWTYPE],p[P_NEWCATEGORY],p[P_NEWPRESENT],flags(p[P_NEWSUP0],p[P_NEWSUP1]),flags(p[P_NEWDEM0],p[P_NEWDEM1]))
   if bits&4:assert p[P_COSTRESOURCE]<40;write(fee_address(p[P_NEWCATEGORY]&255,p[P_COSTRESOURCE]),p[P_NEWFEE])
   if bits&8:write(0x784d32,p[P_NEWGATE10],2);write(0x784d34,p[P_NEWGATE28],2)
def semantic(fn,value):
 if fn==0x447790:raw_eligibility_returns.add(value);value&=255;assert value in [0,1]
 elif fn==0x4c5190:assert -3<=signed(value)<=1
 elif fn!=0x4c5240:assert value in [0,1]
 return value
def enter(cpu,a,n,user):
 sp=cpu.reg_read(UC_X86_REG_ESP);count=0 if a==0x4c5190 else 2 if a==0x447790 else 1;values=[read(sp+4+j*4) for j in range(count)];saved=[cpu.reg_read(reg) for reg in saved_regs];frames.append((a,sp,values,saved));observe(a,0,values)
def leave(cpu,a,n,user):
 sp=cpu.reg_read(UC_X86_REG_ESP);assert frames and frames[-1][1]==sp,'Native frame cleanup';fn,old,values,saved=frames.pop();assert [cpu.reg_read(reg) for reg in saved_regs]==saved,'Native nested callee-saved register';observe(fn,1,values,semantic(fn,cpu.reg_read(UC_X86_REG_EAX)))
def block(cpu,a,n,user):
 global last_block
 assert a in instructions,'Native escaped closed graph '+hex(a)
 if last_block is not None:edges.add((last_block,a));case_edges.add((last_block,a))
 if(a,n) not in block_cache:block_cache[a,n]=sorted(x for x in instructions if a<=x<a+n)
 values=block_cache[a,n];hits.update(values);last_block=values[-1]
def memory_write(cpu,access,a,n,value,user):
 if STACK<=a and a+n<=STACK+0x100000:return
 raise AssertionError('Native pure graph wrote global '+hex(a))
def memory_read(cpu,access,a,n,value,user):
 if STACK<=a and a+n<=STACK+0x100000:return
 assert any(s<=a and a+n<=s+z for s,z in regions),'Unwatched native read '+hex(a)
vm.hook_add(UC_HOOK_BLOCK,block);vm.hook_add(UC_HOOK_MEM_WRITE,memory_write);vm.hook_add(UC_HOOK_MEM_READ,memory_read)
for fn,body in functions.items():
 vm.hook_add(UC_HOOK_CODE,enter,begin=fn,end=fn)
 for i in body:
  a=int(i['address'],16)
  if decoded[a].mnemonic.startswith('ret'):vm.hook_add(UC_HOOK_CODE,leave,begin=a,end=a)
def native(p):
 global params,seen,last_block,case_mutation_applied
 params=p;seen=0;case_mutation_applied=False;events.clear();frames.clear();case_edges.clear();r=p[P_RESOURCE];assert r<40 and p[P_ROW]<61 and p[P_ROW2]<61 and p[P_ROW]!=p[P_ROW2] and valid_city(p[P_CITY]) and p[P_MUTROW]<61
 for j,(a,n) in enumerate(regions):
  value=(p[P_SEED]+j*29)&255
  if(n,value) not in fill_cache:fill_cache[n,value]=bytes([value])*n
  vm.mem_write(a,fill_cache[n,value])
 if p[P_FEE] not in fee_cache:fee_cache[p[P_FEE]]=struct.pack('<I',p[P_FEE])*256*36
 vm.mem_write(0x77646c,fee_cache[p[P_FEE]]);vm.mem_write(0x77de00,bytes(61*106))
 supply=flags(p[P_SUP0],p[P_SUP1]);demand=flags(p[P_DEM0],p[P_DEM1]);zmask=flags(p[P_ZERO0],p[P_ZERO1])
 if p[P_SCENARIO]!=MASK:
  s=p[P_SCENARIO];assert s<512;supply=zmask=0
  for bit,resource in [(0,1),(1,11),(4,9),(5,12),(7,34)]:
   if s&(1<<bit):zmask|=1<<resource
  for bit,resource in [(2,9),(3,11),(6,31),(8,13)]:
   if s&(1<<bit):supply|=1<<resource
  demand=supply
 record(p[P_ROW],p[P_ENABLED],p[P_TYPE],p[P_CATEGORY],p[P_PRESENT],supply,demand);record(p[P_ROW2],p[P_ZERO_ENABLED],0,0,0,zmask,0)
 first=0x77de00+p[P_ROW]*106;zero=0x77de00+p[P_ROW2]*106
 if p[P_SCENARIO]==MASK:write(first+0x2a+r,p[P_SUPPLY_VALUE],1);write(first+6+r,p[P_DEMAND_VALUE],1);write(zero+0x2a+r,p[P_ZERO_VALUE],1)
 category=read(first+4,1);vm.mem_write(fee_address(category,0),struct.pack('<I',p[P_FEE])*36);write(fee_address(category,r),p[P_FEE])
 for city in [0,1,2,3,65536]:timers(city,p[P_T0],p[P_T1],p[P_T2])
 write(0xeaac28,p[P_CITY]);write(0x784d32,p[P_GATE10],2);write(0x784d34,p[P_GATE28],2)
 fn=entries[p[P_OP]];top=[p[P_ROW],r] if fn==0x447790 else[p[P_ARGUMENT]] if fn==0x4c5240 else[] if fn==0x4c5190 else[r]
 saved_values=[0x11111111,0x22222222,0x33333333,0x44444444]
 for repeat in range(p[P_REPEATS]):
  sp=STACK+0xf0000;vm.mem_write(sp-0x1000,b'\xcc'*0x1200);write(sp,STOP)
  for j,value in enumerate(top):write(sp+4+j*4,value)
  for reg,value in zip(saved_regs,saved_values):vm.reg_write(reg,value)
  for reg,value in [(UC_X86_REG_EAX,0xa5a55a5a),(UC_X86_REG_ECX,0x12345678),(UC_X86_REG_EDX,0x87654321),(UC_X86_REG_ESP,sp),(UC_X86_REG_EFLAGS,2)]:vm.reg_write(reg,value)
  last_block=None;vm.emu_start(fn,STOP,count=300000)
  assert vm.reg_read(UC_X86_REG_EIP)==STOP and not frames;assert vm.reg_read(UC_X86_REG_ESP)==sp+4;assert [vm.reg_read(reg) for reg in saved_regs]==saved_values
  assert [read(sp+4+j*4) for j in range(len(top))]==top and bytes(vm.mem_read(sp+4+len(top)*4,128))==b'\xcc'*128,'Native argument guard';result=semantic(fn,vm.reg_read(UC_X86_REG_EAX))
 h=hashlib.sha256()
 for a,n in regions:h.update(struct.pack('<II',a,n));h.update(vm.mem_read(a,n))
 if not case_mutation_applied:ordinary_edges.update(case_edges)
 return {'return':result,'memory':h.hexdigest(),'events':events.copy()}

default=[0]*FIELDS;defaults={'seed':1,'resource':1,'row2':60,'enabled':1,'type':1,'present':1,'sup0':MASK,'sup1':15,'dem0':MASK,'dem1':15,'supply_value':1,'demand_value':1,'zero_enabled':1,'zero0':MASK,'zero1':15,'zero_value':1,'scenario':MASK,'fee':15,'gate10':1,'gate28':1,'argument':15,'repeats':1,'mocc':1,'newcity':65536,'newt0':1,'mutrow':1,'newenabled':1,'newtype':1,'newpresent':1,'newsup0':MASK,'newsup1':15,'newdem0':MASK,'newdem1':15,'costresource':1,'newfee':25,'newgate10':256,'newgate28':256}
for key,value in defaults.items():default[names.index(key)]=value&MASK
cases=[]
def case(**kw):
 p=default.copy()
 for key,value in kw.items():p[names.index(key)]=value&MASK
 cases.append(tuple(p))
for op in range(9):case(op=op)
for op,row,typ in itertools.product(range(4),range(61),[0,1,2,3,4,5,6,255]):case(op=op,row=row,row2=60 if row!=60 else 0,type=typ,zero_enabled=0)
for op,resource,value in itertools.product(range(6),range(40),[0,1,2,255,256,257]):case(op=op,resource=resource,supply_value=value,demand_value=value,zero_value=value,gate10=value,gate28=value)
for op,scenario,resource in itertools.product([4,5],range(512),[12,34]):case(op=op,scenario=scenario,resource=resource)
for category,resource,fee in itertools.product(range(256),[0,1,35],[0,15,-1]):case(op=6,category=category,resource=resource,fee=fee,zero_enabled=0)
fee_values=[0,1,14,15,16,24,25,26,39,40,41,127,128,255,256,257,65535,0x7fffffff,0x80000000,0xffffff00,MASK]
for argument,bits,city in itertools.product(fee_values,range(8),[0,1,65536]):case(op=7,argument=argument,city=city,t0=1 if bits&1 else 0,t1=1 if bits&2 else 0,t2=1 if bits&4 else 0)
for t0,t1,t2,city in itertools.product([0,1,256,32767,65535,32768],[0,1,256,32767,65535,32768],[0,1,256,32767,65535,32768],[0,1,65536]):case(op=8,city=city,t0=t0,t1=t1,t2=t2)
for fee,bits in itertools.product(fee_values,range(8)):case(op=6,fee=fee,t0=bool(bits&1),t1=bool(bits&2),t2=bool(bits&4))
for op,enabled,present in itertools.product(range(4),[0,1,2,255,256,257],[0,1,255,256,257]):case(op=op,enabled=enabled,present=present,zero_enabled=0)
def mutate(fn,phase,bits=1,**kw):case(mfn=fn,mphase=phase,mbits=bits,**kw)
for phase in [0,1]:
 for op,fn in enumerate(entries):mutate(fn,phase,1|2|4|8,op=op)
 for fn in [0x447790,0x4c5240,0x4c5190]:
  for op in range(4):mutate(fn,phase,1|2|4,op=op,fee=0,zero_enabled=0,newfee=15,mutrow=1)
 for op in [4,5]:
  for fn in [0x443b90,0x443d90,0x4439c0,0x443950]:
   for occurrence in [1,2,3]:mutate(fn,phase,2|4,op=op,resource=34,scenario=0,mutrow=0,newtype=0,newsup0=MASK,newsup1=15,mocc=occurrence)
 for op in range(4):mutate(0x4c5190,phase,4,op=op,fee=0,newfee=15,zero_enabled=0)
for op in range(9):case(op=op,repeats=3)
# Redundant third availability alternative becomes live only after an actual
# nested B90(1) returns: enable a real type1 record before the real C0(11).
# No return value is overridden; this is an explicitly qualified reload case.
mutate(0x443b90,1,bits=2,op=4,resource=12,scenario=1,mocc=3,mutrow=1)
# Natural closed-graph false eligibility: also exercise the scanners after
# ALfalse/full-EAX-nonzero, without any helper mutation or result control.
for op,fee in itertools.product(range(4),[0,0xffffff00]):case(op=op,fee=fee,zero_enabled=0)
if args.case:cases=[tuple(json.loads(args.case.read_text())['case'])]
elif args.start:cases=cases[args.start:]
if args.limit:cases=cases[:args.limit]
exe=args.exe or ROOT/'build/agent-training-queries/Debug/training_queries_verify.exe';optimized=args.optimized_exe or(None if args.exe else ROOT/'build/agent-training-queries/Release/training_queries_verify.exe')
variants=[exe]+([optimized] if optimized else[]);oracles=[subprocess.Popen([str(path),'--oracle-batch'],cwd=ROOT,stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True) for path in variants]
dead_instructions={0x4c5274,0x4c5277,0x4c5279,0x4c527e,0x4c527f,0x4c5282,0x4c5284,0x4c5289,0x4c528a,0x4c528c,0x4c5291}
dead_edges={(a,t) for a,targets in branches.items() for t in targets if a in dead_instructions}|{(0x4c5272,0x4c5274)}
report={'format':1,'native_sha256':digest,'original_game_executed':False,'passed':False,'checked':0,'compiled_C_sha256':sha(exe.read_bytes()),'method':'Closed9-function training-query graph, real record traversal/recursive queries/fee policy/selected-city modifier against compiled actual plain C. Zero controlled algorithm endpoints. Full guarded memory, native data-read/no-global-write guards, ordered real function arguments/semantic returns/state, declared helper-exit mutations and native/C stack/saved registers compared.','limitations':['Six query functions preserve full EAX0/1.447790 is a byte Boolean role: all21 authenticated direct native callers consume TEST AL; upper EAX carries incidental fee bits and is recorded only as native evidence, never forced to a C Boolean whole-EAX return.','Actual selected-city modifier4C5190 has closed domain{-3,-2,-1,0,1}. Eleven instructions and seven conditional outcomes in unchanged4C5240 greater-than1 modifier arm are unreachable in this closed graph. Full generic C algorithm remains unchanged; no fake modifier result forces them.','Record helpers use exact flat native memory addresses:61records of106bytes, BYTE fields/columns, BYTE category and DWORD fee matrix. High-category data may overlap neighboring original records; fixture construction retains those physical aliases.','Selected-city fixtures0..3 plus guarded synthetic65536 prove DWORD addressing; only0/1 are normal serialized city records. No C# city clamp or production bounds/fallback is introduced.','Declared real-helper exit state mutations establish reload/caching robustness; they do not establish ordinary closed-graph reachability for paths requiring a pure helper to change state. Actual function results are never replaced or controlled.']}
started=time.monotonic();checked=0;p=None
try:
 assert all(json.loads(o.stdout.readline())=={'fields':FIELDS} for o in oracles)
 for p in cases:
  expected=native(p)
  for number,o in enumerate(oracles):
   o.stdin.write(' '.join(map(str,p))+'\n');o.stdin.flush();line=o.stdout.readline();assert line,'C exited: '+o.stderr.read();actual=json.loads(line)
   if actual!=expected:(PHASE/'last-difference.json').write_text(json.dumps({'case':p,'oracle':number,'C':actual,'native':expected},indent=2)+'\n')
   assert actual==expected,repr({'oracle':number,'return':[actual['return'],expected['return']],'memory':[actual['memory'],expected['memory']],'event_counts':[len(actual['events']),len(expected['events'])],'first_event_difference':[(j,a,b) for j,(a,b) in enumerate(zip(actual['events'],expected['events'])) if a!=b][:1]})
  checked+=1
  if checked%1000==0:print(json.dumps({'checked':checked,'total':len(cases),'elapsed_seconds':round(time.monotonic()-started,1)}),flush=True)
 if not(args.limit or args.start or args.case):
  assert set(instructions)-dead_instructions<=hits,'Uncovered instructions '+repr([hex(a) for a in sorted(set(instructions)-dead_instructions-hits)])
  missing=[(a,t) for a,ts in branches.items() for t in ts if(a,t) not in edges and(a,t) not in dead_edges];assert not missing,'Uncovered feasible branches '+repr([(hex(a),hex(t)) for a,t in missing]);assert len(mutated)==18
  mutation_only={(a,t) for a,ts in branches.items() for t in ts if(a,t) in edges and(a,t) not in ordinary_edges};assert mutation_only=={(0x443bf3,0x443bf5),(0x443d10,0x443d4e)},'Unqualified ordinary coverage gap '+repr(mutation_only)
 report.update(passed=True,checked=checked)
except Exception as error:report.update(checked=checked,case=p,error=str(error),native_eip=hex(vm.reg_read(UC_X86_REG_EIP)))
finally:
 for o in oracles:
  try:o.stdin.close();o.wait(timeout=5)
  except Exception:o.kill()
prefix='' if args.production else 'artifacts/training_queries_phase/'
sources=[prefix+f'src/unit_{n:05d}.c' for n in [625,626,627,630,631,632,669,1603]]+[prefix+'include/training_queries.h','src/unit_01605.c','include/restore_types.h','include/standalone_data.h','tools/verify_training_queries_emulation.py']+['artifacts/training_queries_phase/'+s for s in ['tests/query_case.h','tests/training_queries.c','tests/queries_actual.c','project/CMakeLists.txt','retained-native.json','447790-callers.json']]
missing_edges=[(a,t) for a,ts in branches.items() for t in ts if(a,t) not in edges]
report.update(checked=checked,comparison_checked=checked if optimized else 0,comparison_count=checked*len(variants),comparison_C_sha256=sha(optimized.read_bytes()) if optimized else None,elapsed_seconds=round(time.monotonic()-started,2),case_selection={'limit':args.limit,'start':args.start,'single_case':bool(args.case)},native_instruction_coverage=[len(hits),len(instructions)],reachable_instruction_coverage=[len(hits-dead_instructions),len(set(instructions)-dead_instructions)],owned_instruction_coverage=[len(hits&owned),len(owned)],native_conditional_outcomes=[len(branches)*2-len(missing_edges),len(branches)*2],feasible_conditional_outcomes=[len(branches)*2-len(missing_edges),len(branches)*2-len(dead_edges)],native_uncovered_instructions=[hex(a) for a in sorted(set(instructions)-hits)],missing_conditional_outcomes=[[hex(a),hex(b)] for a,b in missing_edges],declared_dead_instructions=[hex(a) for a in sorted(dead_instructions)],declared_dead_edges=[[hex(a),hex(b)] for a,b in sorted(dead_edges)],function_instruction_coverage={hex(fn):[len(hits&{int(i['address'],16) for i in body}),len(body)] for fn,body in functions.items()},native_447790_raw_return_examples=[hex(v) for v in sorted(raw_eligibility_returns)[:64]],controlled_endpoints=[],mutated_helper_phases=[[hex(a),phase] for a,phase in sorted(mutated)],source_hashes={s:sha((ROOT/s).read_bytes()) for s in sources})
conditional_edges={(a,t) for a,ts in branches.items() for t in ts}
report.update(ordinary_conditional_outcomes=[len(ordinary_edges&conditional_edges),len(conditional_edges-dead_edges)],conditional_outcomes_seen_only_with_declared_mutation=[[hex(a),hex(b)] for a,b in sorted((edges&conditional_edges)-ordinary_edges)],coverage_domain='Feasible outcome total includes explicitly declared real-helper exit state mutations; ordinary unchanged closed-graph outcomes are reported separately. No function result is controlled.')
(ROOT/'metadata/training-queries-emulation.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v for k,v in report.items() if k not in ['source_hashes','native_uncovered_instructions','missing_conditional_outcomes']},indent=2));sys.exit(0 if report['passed'] else 1)
