"""Complete training panel renderer versus pinned instructions and compiled traced/plain C."""
from pathlib import Path
import argparse,hashlib,itertools,json,re,struct,subprocess,sys,time
ROOT=Path(__file__).resolve().parent.parent;PHASE=ROOT/'artifacts/training_panel_phase'
parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--exe',type=Path);parser.add_argument('--optimized-exe',type=Path);parser.add_argument('--production',action='store_true');parser.add_argument('--limit',type=int);parser.add_argument('--start',type=int,default=0);parser.add_argument('--case',type=Path);args=parser.parse_args()
sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE,UC_HOOK_BLOCK,UC_HOOK_MEM_READ,UC_HOOK_MEM_WRITE
from unicorn.x86_const import *
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
sha=lambda b:hashlib.sha256(b).hexdigest();MASK=0xffffffff;AUX=0x4ac20000;STOP=0x33000000;STACK=0x34000000
schema=(PHASE/'tests/training_case.h').read_text();names=[x.lower() for x in re.findall(r'\bP_([A-Z0-9_]+)\b',schema.split('enum{',1)[1].split('};',1)[0]) if x!='FIELDS'];FIELDS=len(names);globals().update({'P_'+name.upper():j for j,name in enumerate(names)})
image=(ROOT/'build/smoke-game/Pharaoh.exe').read_bytes();digest=sha(image);assert digest=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
u16=lambda p:struct.unpack_from('<H',image,p)[0];u32=lambda p:struct.unpack_from('<I',image,p)[0];pe=u32(60);opt=pe+24;sec=opt+u16(pe+20);base=u32(opt+28);size=u32(opt+56)
vm=Uc(UC_ARCH_X86,UC_MODE_32);vm.mem_map(base,(size+4095)&~4095)
for j in range(u16(pe+6)):
 q=sec+j*40;vm.mem_write(base+u32(q+12),image[u32(q+20):u32(q+20)+u32(q+16)])
for a,n in [(AUX,0x10000),(STOP,4096),(STACK,0x100000)]:vm.mem_map(a,n)
def read(a,n=4):return int.from_bytes(vm.mem_read(a,n),'little')
def write(a,v,n=4):vm.mem_write(a,(v&((1<<(8*n))-1)).to_bytes(n,'little'))
def valid_player(player):return player<4 or player==65536
def wa(base,player,selected):assert valid_player(player) and selected<40;return(base+player*0x49d8+selected*2)&MASK
def da(base,player,selected):assert valid_player(player) and selected<40;return(base+player*0x49d8+selected*4)&MASK
def set_record(player,selected,mode,target,flag,state,value):
 for a,v,n in [(wa(0xea4096,player,selected),mode,2),(wa(0xea40de,player,selected),target,2),(wa(0xea4126,player,selected),flag,2),(da(0xea42f8,player,selected),state,4),(da(0xea6160,player,selected),value,4)]:write(a,v,n)
packet=json.loads((PHASE/'retained-native.json').read_text());abi=json.loads((PHASE/'abi-contract.json').read_text());assert packet['native_sha256']==abi['native_sha256']==digest
assert not packet['function']['assembly']['has_more'];body=packet['function']['assembly']['items'];instructions={int(i['address'],16):i for i in body};decoded={};branches={};cap=Cs(CS_ARCH_X86,CS_MODE_32)
for a,i in instructions.items():
 encoded=bytes.fromhex(i['bytes']);assert bytes(vm.mem_read(a,len(encoded)))==encoded;d=next(cap.disasm(encoded,a));decoded[a]=d
 if d.mnemonic.startswith('j') and d.mnemonic!='jmp':branches[a]=(a+d.size,int(d.op_str,16))
tables={int(a,16):f for a,f in abi['jump_tables'].items()}
for f in tables.values():
 assert bytes(vm.mem_read(f['table'],len(bytes.fromhex(f['hex']))))==bytes.fromhex(f['hex'])
 if f['compressed_mapping']:assert bytes(vm.mem_read(f['compressed_mapping'],12))==bytes.fromhex(f['compressed_hex'])
# Inner value-format switch is reached only from the outer default arm.
# Both use unchanged EAX-24 with no intervening call or EAX write. Inside its
#12-value guard, all surviving compressed indices select55B34A, never55B359.
outer=tables[0x55b2ef];inner=tables[0x55b343]
surviving=[j for j,m in enumerate(bytes.fromhex(outer['compressed_hex'])) if outer['targets'][m]==0x55b333]
assert {inner['targets'][bytes.fromhex(inner['compressed_hex'])[j]] for j in surviving}=={0x55b34a}
assert all(decoded[a].mnemonic!='call' for a in instructions if 0x55b333<=a<=0x55b343)
infeasible_computed={(0x55b343,0x55b359)}
regions=[(0xea4080,0x15000),(0x4ac24080,0x3000),(0x78d63c,0x1f0),(0x60597c,0x30),(0x7056b0,0x30),(0xf53ef8,0x30),(0xe20768,0x30),(0xe38d94,0x30),(0xf56f7c,0x60),(0xf571ec,0x30),(0xf55ba0,0x20)]
queries=[0x443950,0x4439c0,0x443a20,0x443b20,0x443b90];draw_counts={0x536820:4,0x53e590:3,0x4d62f0:8,0x4d6250:9,0x4d63d0:8,0x4d6860:9,0x4ccc30:7};deep=set(queries)|set(draw_counts);text={0x4d62f0,0x4d6250,0x4d6860,0x4ccc30}
events=[];writes=[];hits=set();edges=set();mutated=set();controlled_reached=set();params=None;seen=0;entry_sp=0;last_block=None;block_cache={};fill_cache={};immutable=[(0x55bafc,0x88)]
def original_kind():return 1 if params[P_SELECTED]==9 else params[P_SELECTED]
def observe(fn,phase,values,result=0):
 global seen
 p=params;player=read(0xeaac28);selected=read(0x60598c);kind=1 if selected==9 else selected;original=original_kind();e=[fn,phase,len(values)]+list(values)+[0]*(9-len(values))+[result]
 e +=[selected,player,read(wa(0xea4096,player,selected),2),read(wa(0xea40de,player,selected),2),read(wa(0xea4126,player,selected),2),read(da(0xea42f8,player,selected)),read(da(0xea6160,player,selected)),read(0x78d6dc+kind*4),read(0x78d64c+kind*4),kind]
 e +=[read(a) for a in [0x7056c0,0x7056c4,0x7056c8,0xf53f08,0xf53f0c,0xe38da8,0xe38da4,0xf56f8c,0xf56fb4,0xf571fc,0xe20778]]
 e +=[read(0xea60a8+player*0x49d8),read(0x78d6e0),read(0x78d700),read(0x78d650),original,read(0x78d6dc+original*4),read(0x78d64c+original*4),read(wa(0xea4096,p[P_PLAYER],p[P_SELECTED]),2),read(wa(0xea40de,p[P_PLAYER],p[P_SELECTED]),2),read(wa(0xea4126,p[P_PLAYER],p[P_SELECTED]),2),read(da(0xea42f8,p[P_PLAYER],p[P_SELECTED])),read(da(0xea6160,p[P_PLAYER],p[P_SELECTED])),read(0xea60a8+p[P_PLAYER]*0x49d8),p[P_CONTEXT]]
 assert len(e)==48;events.append(e)
 if fn==p[P_MFN] and phase==p[P_MPHASE]:
  seen+=1
  if seen==p[P_MOCC]:
   bits=p[P_MBITS];mutated.add((fn,phase))
   if bits&1:
    for a,v in [(0xf53f08,p[P_NEWY]),(0xf53f0c,p[P_NEWX]),(0xe38da8,p[P_NEWVX]),(0xe38da4,p[P_NEWVY])]:write(a,v)
   if bits&2:assert valid_player(p[P_NEWPLAYER]) and p[P_NEWSELECTED]<40;write(0xeaac28,p[P_NEWPLAYER]);write(0x60598c,p[P_NEWSELECTED])
   if bits&4:set_record(read(0xeaac28),read(0x60598c),p[P_NEWMODE],p[P_NEWTARGET],p[P_NEWFLAG],p[P_NEWSTATE],p[P_NEWVALUE])
   if bits&8:
    k=p[P_NEWKIND];assert k<40;write(0x78d6dc+k*4,p[P_NEWCOUNT]);write(0x78d64c+k*4,p[P_NEWAVAILABLE])
   if bits&16:
    for a,v in [(0x7056c0,p[P_NEWH0]),(0x7056c4,p[P_NEWH1]),(0x7056c8,p[P_NEWH2])]:write(a,v)
   if bits&32:write(0xe20778,p[P_NEWMEASURE])
   if bits&64:
    for a,v in [(0xf56f8c,p[P_NEWFONT]),(0xf56fb4,p[P_NEWTITLEFONT]),(0xf571fc,p[P_NEWICON])]:write(a,v)
def enter(cpu,a,n,user):
 global entry_sp
 entry_sp=cpu.reg_read(UC_X86_REG_ESP);observe(0x55ad30,0,[])
def leave(cpu,a,n,user):assert cpu.reg_read(UC_X86_REG_ESP)==entry_sp;observe(0x55ad30,1,[])
def endpoint(cpu,a,n,user):
 p=params;sp=cpu.reg_read(UC_X86_REG_ESP);controlled_reached.add(a)
 if a in queries:values=[read(sp+4)];observe(a,0,values);r=p[P_Q0+queries.index(a)];observe(a,1,values,r);cleanup=0
 else:
  values=[read(sp+4+j*4) for j in range(draw_counts[a])];cleanup=12 if a==0x53e590 else 0
  if a==0x53e590:assert cpu.reg_read(UC_X86_REG_ECX)==0xf55bb0;values=[0xf55bb0]+values
  if a==0x4d6860:assert values[1:3]==[64,0x57c048]
  if a==0x4ccc30:assert values[:2]==[8,56]
  observe(a,0,values)
  if a in text:write(0xe20778,read(0xe20778)+p[P_DELTA])
  r=p[P_RESULT];observe(a,1,values,r if a==0x53e590 else 0)
 cpu.reg_write(UC_X86_REG_EAX,r);cpu.reg_write(UC_X86_REG_EIP,read(sp));cpu.reg_write(UC_X86_REG_ESP,sp+4+cleanup)
def block(cpu,a,n,user):
 global last_block
 if a in deep:return
 assert a in instructions,'Native escaped '+hex(a)
 if last_block is not None:edges.add((last_block,a))
 if(a,n) not in block_cache:block_cache[a,n]=sorted(x for x in instructions if a<=x<a+n)
 values=block_cache[a,n];hits.update(values);last_block=values[-1]
def memory_write(cpu,access,a,n,value,user):
 if STACK<=a and a+n<=STACK+0x100000:return
 assert any(s<=a and a+n<=s+z for s,z in regions),'Unwatched native write '+hex(a)
 writes.append([a,n,value&((1<<(n*8))-1)])
def memory_read(cpu,access,a,n,value,user):
 if STACK<=a and a+n<=STACK+0x100000:return
 assert any(s<=a and a+n<=s+z for s,z in regions+immutable),'Unwatched native read '+hex(a)
vm.hook_add(UC_HOOK_BLOCK,block);vm.hook_add(UC_HOOK_MEM_WRITE,memory_write);vm.hook_add(UC_HOOK_MEM_READ,memory_read);vm.hook_add(UC_HOOK_CODE,enter,begin=0x55ad30,end=0x55ad30)
for a,d in decoded.items():
 if d.mnemonic.startswith('ret'):vm.hook_add(UC_HOOK_CODE,leave,begin=a,end=a)
for a in deep:vm.hook_add(UC_HOOK_CODE,endpoint,begin=a,end=a)
def native(p):
 global params,seen,last_block
 params=p;seen=0;events.clear();writes.clear()
 for j,(a,n) in enumerate(regions):
  value=(p[P_SEED]+j*29)&255
  if(n,value) not in fill_cache:fill_cache[n,value]=bytes([value])*n
  vm.mem_write(a,fill_cache[n,value])
 assert valid_player(p[P_PLAYER]) and p[P_SELECTED]<40
 for player in [0,1,2,3,65536]:
  for k in range(40):set_record(player,k,p[P_MODE],p[P_TARGET],p[P_FLAG],p[P_STATE],p[P_VALUE])
  set_record(player,p[P_SELECTED],p[P_MODE],p[P_TARGET],p[P_FLAG],p[P_STATE],p[P_VALUE])
 for j in range(36):write(0x78d6dc+j*4,p[P_COUNT]);write(0x78d64c+j*4,p[P_AVAILABLE])
 kind=original_kind();write(0x78d6dc+kind*4,p[P_COUNT]);write(0x78d64c+kind*4,p[P_AVAILABLE])
 for a,v in [(0xeaac28,p[P_PLAYER]),(0x60598c,p[P_SELECTED]),(0x7056c0,p[P_H0]),(0x7056c4,p[P_H1]),(0x7056c8,p[P_H2]),(0xf53f08,p[P_Y]),(0xf53f0c,p[P_X]),(0xe38da8,p[P_VX]),(0xe38da4,p[P_VY]),(0xf56f8c,p[P_FONT]),(0xf56fb4,p[P_TITLEFONT]),(0xf571fc,p[P_ICON]),(0xe20778,p[P_MEASURE])]:write(a,v)
 saved=[(UC_X86_REG_EBX,0x11111111),(UC_X86_REG_EBP,0x22222222),(UC_X86_REG_ESI,0x33333333),(UC_X86_REG_EDI,0x44444444)]
 for repeat in range(p[P_REPEATS]):
  sp=STACK+0xf0000;vm.mem_write(sp-0x800,b'\xcc'*0xa00);write(sp,STOP);write(sp+4,p[P_CONTEXT])
  for reg,value in saved:vm.reg_write(reg,value)
  for reg,value in [(UC_X86_REG_EAX,0xa5a55a5a),(UC_X86_REG_ECX,0x12345678),(UC_X86_REG_EDX,0x87654321),(UC_X86_REG_ESP,sp),(UC_X86_REG_EFLAGS,2)]:vm.reg_write(reg,value)
  last_block=None;vm.emu_start(0x55ad30,STOP,count=100000)
  assert vm.reg_read(UC_X86_REG_EIP)==STOP;assert vm.reg_read(UC_X86_REG_ESP)==sp+4;assert all(vm.reg_read(reg)==v for reg,v in saved);assert read(sp+4)==p[P_CONTEXT] and bytes(vm.mem_read(sp+8,128))==b'\xcc'*128,'Native context/argument guard'
 h=hashlib.sha256()
 for a,n in regions:h.update(struct.pack('<II',a,n));h.update(vm.mem_read(a,n))
 return {'memory':h.hexdigest(),'events':events.copy(),'writes':writes.copy()}

default=[0]*FIELDS;defaults={'seed':1,'context':1,'selected':9,'target':3,'value':12345,'count':2,'available':1,'q4':1,'y':144,'x':192,'vx':80,'vy':60,'font':0x123456,'titlefont':0x654321,'icon':0x10000,'measure':0x12345678,'delta':257,'result':0x89abcdef,'repeats':1,'mocc':1,'newplayer':1,'newselected':28,'newmode':2,'newtarget':-1,'newflag':1,'newstate':1,'newvalue':0x80000000,'newkind':1,'newcount':1,'newavailable':2,'newh0':1,'newh1':2,'newh2':256,'newy':0x7fffffff,'newx':0xfffffff0,'newvx':0x80000000,'newvy':0x80000100,'newfont':0xabcdef,'newtitlefont':0xfedcba,'newicon':0x10000000,'newmeasure':0xfffffff0}
for key,value in defaults.items():default[names.index(key)]=value&MASK
cases=[]
def case(**kw):
 p=default.copy()
 for key,value in kw.items():p[names.index(key)]=value&MASK
 cases.append(tuple(p))
case()
for selected,mode,qmask in itertools.product(range(40),range(5),[0,31]):case(selected=selected,mode=mode,**{'q'+str(j):1 if qmask&(1<<j) else 0 for j in range(5)})
for mode,qmask,selected in itertools.product([-32768,-1,0,1,2,3,4,256,257],range(32),[9,24]):case(mode=mode,selected=selected,**{'q'+str(j):1 if qmask&(1<<j) else 0 for j in range(5)})
for count,available,flag,selected in itertools.product([-1,0,1,2,256,0x7fffffff,0x80000000],[-1,0,1,2,256],[0,1,2,256],[1,9,24,28,36]):case(count=count,available=available,flag=flag,selected=selected)
values=[0,1,99,100,101,-1,-99,-100,0x7fffffff,0x80000000]
for selected,mode,value,target in itertools.product([0,10,24,25,26,28,30,35,36,39],[1,2],values,[-32768,-1,0,1,99,100,32767]):case(selected=selected,mode=mode,value=value,target=target,q0=1,q1=1,q2=1,q3=1)
for highlight,qmask,mode in itertools.product([0,1,2,3,255,256,257,0x1000001],range(16),range(4)):case(h1=highlight,mode=mode,**{'q'+str(j):1 if qmask&(1<<j) else 0 for j in range(4)})
for player,context,value in itertools.product([0,1,2,3,65536],[0,1,256,257,0x80000000,MASK],[0,255,256,0x7fffffff,0x80000000,0xfffffff0]):case(player=player,context=context,y=value,x=value,vx=value,vy=value,font=value,titlefont=value,icon=value,delta=value,h0=value,h2=value)
for q in range(5):
 for value in [0,1,256,0x80000000,MASK]:case(**{'q'+str(q):value})
def mutate(fn,phase,bits,**kw):case(mfn=fn,mphase=phase,mbits=bits,**kw)
for phase in [0,1]:
 mutate(0x55ad30,phase,1|2|4|8|16|32|64)
 for fn in queries:mutate(fn,phase,1|2|4|8|16|32|64)
 for fn in draw_counts:mutate(fn,phase,1|2|4|8|16|32|64,selected=24 if fn==0x4ccc30 else 9)
 for fn in [0x4d62f0,0x4d6860,0x4d6250,0x4d63d0]:
  for occurrence in range(1,5):mutate(fn,phase,2|4|8|16|32,mocc=occurrence,mode=1,q0=1,q3=1)
 for fn in [0x443950,0x4439c0,0x443a20,0x443b20]:mutate(fn,phase,2|4,mode=0,newselected=36,newplayer=65536,newmode=2)
for selected in [1,9,24,28,36]:case(selected=selected,mode=1,repeats=3,q0=1,q3=1)
if args.case:cases=[tuple(json.loads(args.case.read_text())['case'])]
elif args.start:cases=cases[args.start:]
if args.limit:cases=cases[:args.limit]
exe=args.exe or ROOT/'build/agent-training-panel/Debug/training_panel_verify.exe';optimized=args.optimized_exe or(None if args.exe else ROOT/'build/agent-training-panel/Release/training_panel_verify.exe')
variants=[(exe,False),(exe,True)]+([(optimized,False),(optimized,True)] if optimized else[]);oracles=[subprocess.Popen([str(path),'--oracle-plain-batch' if plain else '--oracle-batch'],cwd=ROOT,stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True) for path,plain in variants]
report={'format':1,'native_sha256':digest,'original_game_executed':False,'passed':False,'checked':0,'compiled_C_sha256':sha(exe.read_bytes()),'method':'Complete1001-instruction training panel algorithm against compiled traced and plain C. Full guarded watched bytes, owned DWORD store order, ordered query/render arguments/results/state, declared helper mutation/reloads, all jump tables, context nonconsumption and native/C saved registers/stack compared.','limitations':['Five availability queries443950/4439C0/443A20/443B20/443B90 are named one-DWORD/full-EAX boundaries. This proves their ordering/consumption and panel response, not their record/path algorithms.','Seven drawing/text boundaries536820/53E590/4D62F0/4D6250/4D63D0/4D6860/4CCC30 are controlled. Text providers explicitly add a declared DWORD measurement delta. No pixel fidelity or full text measurement algorithm is claimed.','Player65536 is a deliberately guarded synthetic width/wrap fixture, not a normal game player. Ordinary player fixtures0..3 and selected resources0..39 have mapped original field addresses, including actual neighboring-table overlap. No invented production bounds or fallback is added.','The complete CFG stack audit proves incoming context DWORD is ignored. Native sprite member binds ECX F55BB0 and RET12; C uses accepted global facade. All other modeled endpoints are cdecl RET0.']}
started=time.monotonic();checked=0;p=None
try:
 assert all(json.loads(o.stdout.readline())=={'fields':FIELDS} for o in oracles)
 for p in cases:
  expected=native(p)
  for number,(o,(path,plain)) in enumerate(zip(oracles,variants)):
   o.stdin.write(' '.join(map(str,p))+'\n');o.stdin.flush();line=o.stdout.readline();assert line,'C exited: '+o.stderr.read();actual=json.loads(line);want=dict(expected,writes=[]) if plain else expected
   if actual!=want:(PHASE/'last-difference.json').write_text(json.dumps({'case':p,'oracle':number,'C':actual,'native':want},indent=2)+'\n')
   assert actual==want,repr({'oracle':number,'memory':[actual['memory'],want['memory']],'event_counts':[len(actual['events']),len(want['events'])],'first_event_difference':[(j,a,b) for j,(a,b) in enumerate(zip(actual['events'],want['events'])) if a!=b][:1],'write_counts':[len(actual['writes']),len(want['writes'])],'first_write_difference':[(j,a,b) for j,(a,b) in enumerate(zip(actual['writes'],want['writes'])) if a!=b][:1]})
  checked+=1
  if checked%500==0:print(json.dumps({'checked':checked,'total':len(cases),'elapsed_seconds':round(time.monotonic()-started,1)}),flush=True)
 if not(args.limit or args.start or args.case):
  assert set(instructions)<=hits,'Uncovered instructions '+repr([hex(a) for a in sorted(set(instructions)-hits)])
  missing=[(a,t) for a,ts in branches.items() for t in ts if(a,t) not in edges];assert not missing,'Uncovered conditional outcomes '+repr([(hex(a),hex(t)) for a,t in missing]);assert deep<=controlled_reached;assert len(mutated)==26
  missing_computed={(a,t) for a,f in tables.items() for t in f['targets']} -edges-infeasible_computed;assert not missing_computed,'Uncovered feasible computed edges '+repr(missing_computed);assert not edges&infeasible_computed
 report.update(passed=True,checked=checked)
except Exception as error:report.update(checked=checked,case=p,error=str(error),native_eip=hex(vm.reg_read(UC_X86_REG_EIP)))
finally:
 for o in oracles:
  try:o.stdin.close();o.wait(timeout=5)
  except Exception:o.kill()
prefix='' if args.production else 'artifacts/training_panel_phase/'
sources=[prefix+s for s in ['src/unit_02637.c','include/training_panel.h']]+['include/restore_types.h','include/sprite_render.h','include/sprite_libraries.h','include/standalone_data.h','tools/verify_training_panel_emulation.py']+['artifacts/training_panel_phase/'+s for s in ['tests/training_case.h','tests/training_panel.c','tests/training_actual_trace.c','tests/training_actual_plain.c','project/CMakeLists.txt','retained-native.json','audit_abi.py','abi-contract.json']]
missing_edges=[(a,t) for a,ts in branches.items() for t in ts if(a,t) not in edges]
computed={(a,t) for a,f in tables.items() for t in f['targets']}
report.update(checked=checked,plain_checked=checked,comparison_checked=checked if optimized else 0,comparison_count=checked*len(variants),comparison_C_sha256=sha(optimized.read_bytes()) if optimized else None,elapsed_seconds=round(time.monotonic()-started,2),case_selection={'limit':args.limit,'start':args.start,'single_case':bool(args.case)},native_instruction_coverage=[len(hits),len(instructions)],native_conditional_outcomes=[len(branches)*2-len(missing_edges),len(branches)*2],native_uncovered_instructions=[hex(a) for a in sorted(set(instructions)-hits)],missing_conditional_outcomes=[[hex(a),hex(b)] for a,b in missing_edges],computed_jump_coverage={hex(site):{'targets':[len(set(f['targets'])&{b for a,b in edges if a==site}),len(set(f['targets']))],'uncovered':[hex(t) for t in sorted(set(f['targets'])-{b for a,b in edges if a==site})]} for site,f in tables.items()},feasible_computed_edges=[len(edges&computed),len(computed-infeasible_computed)],declared_infeasible_computed_edges={'0x55b343->0x55b359':'Outer percentage group already bypasses inner switch. Within selection24..35, every surviving outer-default compressed index selects55B34A. No intervening call/EAX change.55B359 instruction body itself is covered through direct scaled-kind checks.'},controlled_endpoints=[hex(a) for a in sorted(deep)],reached_controls=[hex(a) for a in sorted(controlled_reached)],mutated_helper_phases=[[hex(a),phase] for a,phase in sorted(mutated)],source_hashes={s:sha((ROOT/s).read_bytes()) for s in sources})
(ROOT/'metadata/training-panel-emulation.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v for k,v in report.items() if k not in ['source_hashes','native_uncovered_instructions','missing_conditional_outcomes']},indent=2));sys.exit(0 if report['passed'] else 1)
