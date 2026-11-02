"""Whole retained-x87 LOS and explicit typed math versus pinned original x86."""
from pathlib import Path
from datetime import datetime,timezone
import argparse,hashlib,itertools,json,random,re,struct,subprocess,sys
ROOT=Path(__file__).resolve().parents[3] if 'actor_order_los_phase' in str(Path(__file__)) else Path(__file__).resolve().parents[1]
PHASE=ROOT/'artifacts/actor_order_los_phase';sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_BLOCK,UC_HOOK_CODE,UC_HOOK_MEM_WRITE
from unicorn.x86_const import *
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
ap=argparse.ArgumentParser();ap.add_argument('--production',action='store_true');ap.add_argument('--exe',type=Path,default=ROOT/'build/agent-actor-order-los/Debug/restore_actor_order_los_verify.exe');ap.add_argument('--optimized-exe',type=Path,default=ROOT/'build/agent-actor-order-los/Release/restore_actor_order_los_verify.exe');ap.add_argument('--start',type=int,default=0);ap.add_argument('--limit',type=int);args=ap.parse_args()
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest();product=ROOT if args.production else PHASE
names=re.findall(r'\bL_([A-Z0-9_]+)\b',(product/'tests/actor_order_los_case.h').read_text().split('enum {',1)[1].split('};',1)[0]);names.remove('FIELDS');FIELDS=len(names);globals().update({'L_'+n:i for i,n in enumerate(names)})
peer=ROOT/'artifacts/menu_full_input_phase';flood=ROOT/'artifacts/actor_order_flood_phase'
for frozen in [peer/'numeric-freeze.json',flood/'metadata/freeze-manifest.json']:
 for path,h in json.loads(frozen.read_text())['files'].items():assert sha(ROOT/path)==h,path
numeric=ROOT if args.production else peer;flood_product=ROOT if args.production else flood
for name in ['runtime/menu_full_input_numeric.c','include/menu_full_input_numeric.h']:assert sha(numeric/name)==sha(peer/name),name
assert sha(flood_product/'src/unit_02335.c')==sha(flood/'src/unit_02335.c')
image=(ROOT/'build/smoke-game/Pharaoh.exe').read_bytes();digest=hashlib.sha256(image).hexdigest();assert digest=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
u16=lambda p:struct.unpack_from('<H',image,p)[0];u32=lambda p:struct.unpack_from('<I',image,p)[0];pe=u32(60);opt=pe+24;sec=opt+u16(pe+20);base=u32(opt+28)
vm=Uc(UC_ARCH_X86,UC_MODE_32);vm.mem_map(base,(u32(opt+56)+4095)&~4095)
for i in range(u16(pe+6)):
 q=sec+i*40;vm.mem_write(base+u32(q+12),image[u32(q+20):u32(q+20)+u32(q+16)])
BASE=0x30000000;STOP=0x32000000;STACK=0x34000000
for a,n in [(BASE,0x10000),(STOP,0x10000),(STACK,0x20000)]:vm.mem_map(a,n)
def get(a,n=4):return int.from_bytes(vm.mem_read(a,n),'little')
def put(a,v,n=4):vm.mem_write(a,(v&((1<<(n*8))-1)).to_bytes(n,'little'))
def signed(v):return (v&0x7fffffff)-(v&0x80000000)
def ptr(offset):return BASE+offset+p[L_ALIGN]
def outx():return ptr(0x260 if p[L_ALIAS]==2 else 0x250)
def outy():return ptr(0x250 if p[L_ALIAS] in [1,2] else 0x260)
flood_regions=[(0xbca2c0,48),(0xeadff0,0xcb40),(0xed4130,0xcb30),(0xeed750,0x658a0)]
def make_regions():return [(BASE+0xf0,0x200)]+([] if p[L_OP]<4 else [(0xaf597c,207968)]+([(0xbca2c0,48)] if p[L_OP]==4 else flood_regions))
native_file=ROOT/'metadata/actor-order-los-native.json' if args.production else PHASE/'metadata/native.json';d=json.loads(native_file.read_text());assert d['native_sha256']==digest
qualification_file=ROOT/'metadata/actor-order-los-qualification.json' if args.production else PHASE/'metadata/edge-qualification.json';qualification=json.loads(qualification_file.read_text());assert qualification['native_sha256']==digest and qualification['native_export_sha256']==sha(native_file);assert qualification['excluded_edges']==d['initial_infeasible_edges'] and qualification['excluded_instructions']==[]
core={};sets={};branches={};physical_owner={};cap=Cs(CS_ARCH_X86,CS_MODE_32)
for name,f in d['functions'].items():
 a=f['assembly'];assert not a['has_more'] and len(a['items'])==a['total'];sets[int(name,16)]=set()
 for x in a['items']:
  ip=int(x['address'],16);code=bytes.fromhex(x['bytes']);assert bytes(vm.mem_read(ip,len(code)))==code;core[ip]=x;sets[int(name,16)].add(ip);decoded=list(cap.disasm(code,ip));assert sum(i.size for i in decoded)==len(code)
  for i in decoded:physical_owner[i.address]=ip
  i=decoded[0]
  if i.mnemonic.startswith('j') and i.mnemonic!='jmp':branches[ip]={ip+i.size,int(i.op_str,16)}
for name,f in d['float32_constants'].items():assert bytes(vm.mem_read(int(name,16),4)).hex()==f['bytes']
guard0=(0x8fedcba987654321,0x3fff);guard1=(0xfedcba9876543210,0xc004)
p=[];regions=[];events=[];previous=None;context='primitive';whole_hits=set();whole_edges=set();primitive_hits=set();block_cache={};block_context_seen=set()
def block(cpu,address,size,user):
 global previous
 key=(address,size)
 if key not in block_cache:
  items=list(cap.disasm(bytes(cpu.mem_read(address,size)),address));assert items and sum(i.size for i in items)==size
  for i in items:assert i.address in physical_owner,f'Native escaped{ i.address:08x}'
  block_cache[key]=[physical_owner[i.address] for i in items]
 ids=block_cache[key]
 fresh=(key,context) not in block_context_seen
 if fresh:block_context_seen.add((key,context))
 if context=='whole':
  if fresh:whole_hits.update(ids)
  if previous in branches:whole_edges.add((previous,address))
 elif fresh:primitive_hits.update(ids)
 previous=ids[-1]
def fill_entry(cpu,address,size,user):
 sp=cpu.reg_read(UC_X86_REG_ESP);events.append([get(sp+4),get(sp+8),get(sp+12)&255,get(0xeae004),get(0xeae008),get(0xeae00c)]);assert len(events)<=2
def written(cpu,access,a,n,value,user):
 if STACK<=a and a+n<=STACK+0x20000:return
 assert context=='whole' and p[L_OP]>=4
 if any(v<=a and a+n<=v+4 for v in [outx(),outy()]):return
 assert p[L_OP]==5 and (0xeae004<=a and a+n<=0xebab20 or 0xeed760<=a and a+n<=0xf52fe0),f'LOS unexpected write{a:08x}/{n}'
vm.hook_add(UC_HOOK_BLOCK,block);vm.hook_add(UC_HOOK_CODE,fill_entry,begin=0x40f8f0,end=0x40f8f0);vm.hook_add(UC_HOOK_MEM_WRITE,written)
def raw_tuple(value):return struct.unpack('<QH',value)
def rawtop(offset=0):return struct.pack('<QH',*vm.reg_read(UC_X86_REG_FP0+(((vm.reg_read(UC_X86_REG_FPSW)>>11)+offset)&7)))
def setup_fp(cw,values):
 all_values=values+[guard0,guard1];vm.reg_write(UC_X86_REG_FPCW,cw);vm.reg_write(UC_X86_REG_FPSW,0);vm.reg_write(UC_X86_REG_FPTAG,(0xffff<<(2*len(all_values)))&0xffff)
 for i,value in enumerate(all_values):vm.reg_write(UC_X86_REG_FP0+i,value)
def check_guards(top,guard_index):
 assert vm.reg_read(UC_X86_REG_FPCW)==p[L_CW]
 assert (vm.reg_read(UC_X86_REG_FPSW)>>11)&7==top
 for j,g in enumerate([guard0,guard1]):assert vm.reg_read(UC_X86_REG_FP0+guard_index+j)==g,('Native FPU guard',p[L_OP],j)
def registers():
 sp=STACK+0x10000;put(sp,STOP)
 for r,v in [(UC_X86_REG_EBX,0x11111111),(UC_X86_REG_EBP,0x22222222),(UC_X86_REG_ESI,0x33333333),(UC_X86_REG_EDI,0x44444444),(UC_X86_REG_EAX,0x12345678),(UC_X86_REG_ECX,0xabcdef12),(UC_X86_REG_EDX,0x87654321)]:vm.reg_write(r,v)
 vm.reg_write(UC_X86_REG_ESP,sp);vm.reg_write(UC_X86_REG_EFLAGS,0x202);return sp
def function(entry,argv=()):
 global previous
 previous=None;sp=registers()
 for i,v in enumerate(argv):put(sp+4+i*4,v)
 vm.emu_start(entry,STOP,count=100000000)
 assert vm.reg_read(UC_X86_REG_EIP)==STOP and vm.reg_read(UC_X86_REG_ESP)==sp+4,{'entry':hex(entry),'ip':hex(vm.reg_read(UC_X86_REG_EIP))}
 assert [vm.reg_read(r) for r in [UC_X86_REG_EBX,UC_X86_REG_EBP,UC_X86_REG_ESI,UC_X86_REG_EDI]]==[0x11111111,0x22222222,0x33333333,0x44444444]
 return [vm.reg_read(UC_X86_REG_EAX),vm.reg_read(UC_X86_REG_EDX)]
def setup(case):
 global p,regions,events,previous
 p=case;regions=make_regions();events=[];previous=None
 for i,(a,n) in enumerate(regions):
  pattern=bytes((p[L_SEED]+j*37+i*11)&255 for j in range(256));vm.mem_write(a,(pattern*(n//256+1))[:n])
 vm.mem_write(ptr(0x180),bytes(v&255 for v in p[L_R0:L_R0+10]))
 if p[L_OP]>=4:
  put(0xbca2d8,p[L_BASE]);low=4 if p[L_STYLE] in [1,3,4,5] else 0
  words=[((p[L_SEED]+i*0x01020300)&0xffffff00)|low for i in range(51984)];vm.mem_write(0xaf598c,struct.pack('<51984I',*words))
  dx=signed((p[L_TX]-p[L_X])&0xffffffff);dy=signed((p[L_TY]-p[L_Y])&0xffffffff)
  probe=(p[L_BASE]+(p[L_Y]+(-1 if dy<0 else 1 if dy>0 else 0))*228+p[L_X]+(-1 if dx<0 else 1 if dx>0 else 0))&0xffffffff
  if p[L_STYLE] in [2,3]:
   cell=(probe+[0,-228,228,1,-1,-227,-229,229,227][p[L_BLOCK]%9])&0xffffffff;assert cell<51984;a=0xaf598c+cell*4;put(a,get(a)|4 if p[L_STYLE]==2 else get(a)&~4)
  if p[L_STYLE]==4:assert probe<51984;put(0xaf598c+probe*4,get(0xaf598c+probe*4)|64)
  if p[L_STYLE]==5 and p[L_BLOCK]<8:
   cell=(p[L_BASE]+p[L_Y]*228+p[L_X]+[-228,-227,228,-229,1,229,-1,227][p[L_BLOCK]])&0xffffffff;assert cell<51984;put(0xaf598c+cell*4,get(0xaf598c+cell*4)&~4)
 if p[L_OP]==5:
  terrain=bytearray([253])*51984
  for y in range(1,p[L_DIM]-1):terrain[y*228+1:y*228+p[L_DIM]-1]=bytes([p[L_CLASS]&255])*(p[L_DIM]-2)
  vm.mem_write(0xed4140,bytes(terrain))
def slopes(cw,dx,dy):
 global previous,context
 context='primitive';previous=None;registers();setup_fp(cw,[]);vm.reg_write(UC_X86_REG_ESI,dx);vm.reg_write(UC_X86_REG_EDI,dy)
 vm.emu_start(0x40c1d1,0x40c234,count=100);assert vm.reg_read(UC_X86_REG_EIP)==0x40c234
 assert vm.reg_read(UC_X86_REG_FPCW)==cw and (vm.reg_read(UC_X86_REG_FPSW)>>11)&7==6
 assert vm.reg_read(UC_X86_REG_FP0)==guard0 and vm.reg_read(UC_X86_REG_FP1)==guard1
 return rawtop(1),rawtop(0)
def native(case):
 global context,previous
 setup(case);op=p[L_OP];answer=[0,0]
 if op==0:
  a,b=slopes(p[L_CW],p[L_X],p[L_Y]);vm.mem_write(ptr(0x100),a);vm.mem_write(ptr(0x140),b)
 elif op==1:
  context='primitive';previous=None;sp=registers();setup_fp(p[L_CW],[raw_tuple(bytes(p[L_R0:L_R0+10]))]);put(sp+0x30,p[L_X]);vm.reg_write(UC_X86_REG_ESI,p[L_Y]);vm.emu_start(0x40c2b3,0x40c2c1,count=20);value=rawtop();answer=function(0x56133c);check_guards(0,1);assert rawtop()==bytes(p[L_R0:L_R0+10]);vm.mem_write(ptr(0x1c0),value)
 elif op==2:
  context='primitive';previous=None;registers();setup_fp(p[L_CW],[raw_tuple(bytes(p[L_R0:L_R0+10]))]);vm.emu_start(0x40c296,0x40c2a1,count=10);answer=[int(bool(vm.reg_read(UC_X86_REG_FPSW)&0x4000)),0];check_guards(0,1)
 elif op==3:
  context='primitive';setup_fp(p[L_CW],[raw_tuple(bytes(p[L_R0:L_R0+10]))]);answer=function(0x56133c);check_guards(1,1)
 else:
  context='whole';setup_fp(p[L_CW],[]);answer=function(0x40bda0 if op==4 else 0x40c180,[p[L_X],p[L_Y],p[L_TX],p[L_TY],outx(),outy()]);answer[1]=0;check_guards(0,0)
 return {'return':answer,'cw':p[L_CW],'guards':True,'state':b''.join(bytes(vm.mem_read(a,n)) for a,n in regions).hex(),'events':list(events)}
rng=random.Random(0x40bda0);cases=[];cws=[0x7f|(pc<<8)|(rc<<10)|(ic<<12) for pc,rc,ic in itertools.product([0,2,3],range(4),range(2))]
def add(op=4,raw=None,**kw):
 values={'OP':op,'CW':0x37f,'X':4,'Y':4,'TX':8,'TY':8,'STYLE':0,'BLOCK':0,'CLASS':0,'DIM':16,'SEED':rng.getrandbits(32),'ALIAS':0,'ALIGN':0,'BASE':0};values.update({k.upper():v for k,v in kw.items()});values.update({'R'+str(i):v for i,v in enumerate(raw or bytes(10))});cases.append([values[n]&0xffffffff for n in names])
corners=[-2147483648,-32768,-1,0,1,32767,2147483647]
for cw,x,y in itertools.product(cws,corners,corners):add(0,cw=cw,x=x,y=y)
ratio_cache={}
for cw in cws:
 for dx,dy in [(1,3),(3,1),(49,1),(1,49),(-3,7),(7,-3)]:
  pair=slopes(cw,dx&0xffffffff,dy&0xffffffff)
  for which,raw in enumerate(pair):ratio_cache[cw,dx,dy,which]=raw
for (cw,dx,dy,which),raw in ratio_cache.items():
 for origin,delta in itertools.product([-32768,0,32767],[-227,-1,0,1,227]):add(1,raw=raw,cw=cw,x=origin,y=delta)
 for origin,delta in itertools.product([-2147483648,2147483647],[-2147483648,2147483647]):add(1,raw=raw,cw=cw,x=origin,y=delta)
max80=struct.pack('<QH',0xffffff0000000000,0x407e)
compare_values=[bytes(10),struct.pack('<QH',0x8000000000000000,0x3fff),max80,struct.pack('<QH',0xfffffeffffffffff,0x407e),struct.pack('<QH',0xffffff0000000001,0x407e)]
for cw,raw in itertools.product(cws,compare_values):add(2,raw=raw,cw=cw)
for cw,raw in itertools.product(cws,compare_values+[struct.pack('<QH',0x8000000000000000,0x403e),struct.pack('<QH',0x8000000000000000,0xc03e),struct.pack('<QH',0x8000000000000000,0x7fff)]):add(3,raw=raw,cw=cw)
for op,dx,dy,alias in itertools.product([4,5],[-5,-1,0,1,5],[-5,-1,0,1,5],[0,1,2]):add(op,x=8,y=8,tx=8+dx,ty=8+dy,style=0 if op==4 else 1,alias=alias)
for block in range(9):add(4,style=2,block=block,tx=5,ty=5)
for block in range(5):add(5,style=3,block=block,tx=5,ty=5)
for alias,block,same in itertools.product([0,1,2],range(9),[0,1]):add(4,style=5,block=block,alias=alias,y=4 if same else 6,ty=8 if same else 10)
for op in [4,5]:add(op,style=4)
for classification in [0,253,254,255,252]:add(5,style=1,**{'class':classification})
for cw,op,dx,dy in itertools.product(cws,[4,5],[3,7],[-3,1,7]):add(op,cw=cw,x=8,y=8,tx=8+dx,ty=8+dy,style=0 if op==4 else 1,dim=24)
for op,alignment in itertools.product([4,5],[1,3]):add(op,style=0 if op==4 else 1,align=alignment)
for op in [4,5]:add(op,x=2,y=2,tx=50002,ty=3,style=0 if op==4 else 1,dim=228)
add(5,x=230,y=2,tx=2,ty=3,style=1,**{'class':255})
selected=cases[args.start:args.start+args.limit if args.limit else None];expected=[]
print(json.dumps({'selected_cases':len(selected),'full_case_count':len(cases),'first_whole_case':next(i for i,c in enumerate(cases) if c[L_OP]>=4)}),flush=True)
for i,c in enumerate(selected):
 if c[L_OP]>=4 and i%25==0:print(json.dumps({'native_whole_case':args.start+i,'op':c[L_OP]}),flush=True)
 answer=native(c)
 if c[L_STYLE] in [2,3]:assert bytes.fromhex(answer['state'])[0x160:0x164] != struct.pack('<I',5) or bytes.fromhex(answer['state'])[0x170:0x174] != struct.pack('<I',5),'Single-candidate rejection must change selected output.'
 if c[L_OP]==5 and c[L_X]==230:assert answer['return']==[0,0] and bytes.fromhex(answer['state'])[0x160:0x164]==struct.pack('<I',230) and bytes.fromhex(answer['state'])[0x170:0x174]==struct.pack('<I',2),'FF classification must reject a mapped positive-distance candidate.'
 expected.append(answer)
owned=sets[0x40bda0]|sets[0x40c180];excluded_edges={(int(a,16),int(b,16)) for a,b in d['initial_infeasible_edges']};all_edges={(a,b) for a,z in branches.items() if a in owned for b in z};feasible_edges=all_edges-excluded_edges;complete=owned<=whole_hits and feasible_edges<=whole_edges
source_paths=[product/x for x in ['src/unit_00086.c','src/unit_00087.c','runtime/actor_order_los_numeric.c','include/actor_order_los.h','tests/actor_order_los.c','tests/actor_order_los_actual.c','tests/actor_order_los_case.h']]+[flood_product/'src/unit_02335.c',flood_product/'include/actor_order_flood.h',numeric/'runtime/menu_full_input_numeric.c',numeric/'include/menu_full_input_numeric.h',ROOT/'src/unit_00119.c',ROOT/'include/restore_types.h',ROOT/'include/standalone_data.h',PHASE/'build/CMakeLists.txt',PHASE/'tools/extract_actor_order_los_native.py',PHASE/'tools/qualify_actor_order_los_edges.py',Path(__file__).resolve()]
report={'format':1,'generated_at':datetime.now(timezone.utc).isoformat(),'production':args.production,'passed':False,'complete_case_set':not args.limit and not args.start,'checked':len(selected),'native_sha256':digest,'compiled_C_path':args.exe.resolve().relative_to(ROOT).as_posix(),'compiled_C_sha256':sha(args.exe.resolve()),'source_hashes':{x.relative_to(ROOT).as_posix():sha(x) for x in source_paths},'native_exports':{x.relative_to(ROOT).as_posix():sha(x) for x in [native_file,qualification_file]},'control_words':cws,'watch_bytes':{'primitive':512,'clear_line':208528,'flood_line':728480},'numeric_dependency':{'freeze_path':(peer/'numeric-freeze.json').relative_to(ROOT).as_posix(),'freeze_sha256':sha(peer/'numeric-freeze.json'),'proof_sha256':sha(peer/'numeric-private-emulation.json'),'used_API':'restore_menu_input_trunc80 only'},'decoder_qualification':{'logical_00561342':'9bd97dfe decodes WAIT 00561342 plus FNSTCW 00561343; both physical instructions map to the same pinned Ghidra logical instruction.'},'qualified_controller':{'complete':complete,'native_instructions':len(owned),'executed':len(owned&whole_hits),'missing_instructions':[f'{a:08x}' for a in sorted(owned-whole_hits)],'conditional_edges':len(all_edges),'feasible_edges':len(feasible_edges),'edges_executed':len(feasible_edges&whole_edges),'missing_edges':[[f'{a:08x}',f'{b:08x}'] for a,b in sorted(feasible_edges-whole_edges)],'excluded_edges':d['initial_infeasible_edges']},'actual_dependency_coverage':{f'{a:08x}':{'executed_in_whole':len(z&whole_hits),'total':len(z),'executed_primitive_or_whole':len(z&(whole_hits|primitive_hits))} for a,z in sets.items() if a not in [0x40bda0,0x40c180]},'variants':{},'original_game_process_executed':False,'original_machine_code_fallback_used':False,'complete_actor_order_gameplay_claimed':False,'limitations':['Whole538 instruction LOS implementations, actual frozen2335/119 and all15FTOL instructions are used in native composition. Explicit retainedraw80 primitives introduce no binary32 spill. Converter is immutable independent peer numeric-only component; no wholeinput or original.text dependency.','All24definedmaskedCW are compared with hardwareCW/TOP/two raw80guards and C/native stack/callee-saved guards. FPSWexception/conditionflags beyond consumedC3 comparisons are not claimed; native integer functions return no FPstatus contract.','Primitive-site coverage is separate from whole-function coverage and cannot satisfy it. Original caller-owned ratios are checked then normalized into explicit C inputs/outputs.','Mapped valid flag/class/distance workspaces and real output objects are required. Syntheticmapped target50002 fixtures stress native32bit squared-distance overflow; x230,y2 and x2,y3 map to the same physical cell to isolate FF classification with positive distance. These are not physicalworld-validation claims.','4B6400 actor-group action/path cluster remains unclosed;1768 FALSE GetCursorPos discrepancy remains open.']}
out=ROOT/'metadata/actor-order-los-emulation.json' if args.production else PHASE/'metadata/actor-order-los-emulation.json'
try:
 if report['complete_case_set']:assert not whole_edges&excluded_edges,sorted(whole_edges&excluded_edges);assert complete,report['qualified_controller']
 for label,path in [('Debug',args.exe),('O2',args.optimized_exe)]:
  path=path.resolve();smoke=subprocess.run([str(path)],capture_output=True,text=True,timeout=10);assert smoke.returncode==0,smoke.stderr
  proc=subprocess.Popen([str(path),'--oracle-batch'],stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True)
  try:
   assert json.loads(proc.stdout.readline())=={'fields':FIELDS}
   for i,(case,want) in enumerate(zip(selected,expected)):
    proc.stdin.write(' '.join(map(str,case))+'\n');proc.stdin.flush();line=proc.stdout.readline();assert line,(label,i,proc.poll(),proc.stderr.read());actual=json.loads(line)
    if actual!=want:
     field=next(k for k in want if actual[k]!=want[k]);detail={'variant':label,'case':args.start+i,'input':dict(zip(names,case)),'field':field,'C':actual[field],'native':want[field]};(PHASE/'metadata/last-mismatch.json').write_text(json.dumps(detail,indent=2));raise AssertionError({k:v for k,v in detail.items() if k not in ['C','native']})
   proc.stdin.close();proc.wait(timeout=10);assert proc.returncode==0,proc.stderr.read()
  finally:
   if proc.poll() is None:proc.kill();proc.wait()
  report['variants'][label]={'path':path.relative_to(ROOT).as_posix(),'sha256':sha(path),'checked':len(selected),'smoke_exit':smoke.returncode}
 assert all(sha(ROOT/path)==value for path,value in report['source_hashes'].items()),'Concurrent source drift.'
 report['comparisons']=len(selected)*2;report['passed']=True
finally:out.write_text(json.dumps(report,indent=2)+'\n')
summary={k:v for k,v in report['qualified_controller'].items() if k not in ['missing_instructions','missing_edges']}
summary['missing_instruction_count']=len(report['qualified_controller']['missing_instructions']);summary['missing_edge_count']=len(report['qualified_controller']['missing_edges'])
print(json.dumps({'passed':report['passed'],'checked':len(selected),'comparisons':report.get('comparisons'),'whole':summary}))
