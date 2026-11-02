"""Whole actor-order commit/lookup versus pinned x86 with real small helpers."""
from pathlib import Path
from datetime import datetime,timezone
import argparse,hashlib,itertools,json,random,re,struct,subprocess,sys
ROOT=Path(__file__).resolve().parents[3] if 'menu_window_cursor_consumer_phase' in str(Path(__file__)) else Path(__file__).resolve().parents[1]
PHASE=ROOT/'artifacts/menu_window_cursor_consumer_phase';sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE,UC_HOOK_MEM_WRITE,UC_HOOK_MEM_INVALID
from unicorn.x86_const import *
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
ap=argparse.ArgumentParser();ap.add_argument('--production',action='store_true');ap.add_argument('--exe',type=Path,default=ROOT/'build/agent-menu-window-cursor-consumer/Debug/restore_menu_window_cursor_consumer_verify.exe');ap.add_argument('--optimized-exe',type=Path,default=ROOT/'build/agent-menu-window-cursor-consumer/Release/restore_menu_window_cursor_consumer_verify.exe');ap.add_argument('--limit',type=int);ap.add_argument('--start',type=int,default=0);args=ap.parse_args()
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest();product=ROOT if args.production else PHASE
names=re.findall(r'\bP_([A-Z0-9_]+)\b',(product/'tests/menu_window_cursor_consumer_case.h').read_text().split('enum {',1)[1].split('};',1)[0]);names.remove('FIELDS');FIELDS=len(names);globals().update({'P_'+n:i for i,n in enumerate(names)})
image=(ROOT/'build/smoke-game/Pharaoh.exe').read_bytes();digest=hashlib.sha256(image).hexdigest();assert digest=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
u16=lambda p:struct.unpack_from('<H',image,p)[0];u32=lambda p:struct.unpack_from('<I',image,p)[0];pe=u32(60);opt=pe+24;sec=opt+u16(pe+20);base=u32(opt+28)
vm=Uc(UC_ARCH_X86,UC_MODE_32);vm.mem_map(base,(u32(opt+56)+4095)&~4095)
for i in range(u16(pe+6)):
 q=sec+i*40;vm.mem_write(base+u32(q+12),image[u32(q+20):u32(q+20)+u32(q+16)])
STOP=0x30000000;TRAP=STOP+0x100;STACK=0x31000000;vm.mem_map(STOP,0x10000);vm.mem_map(STACK,0x20000)
def get(a,n=4):return int.from_bytes(vm.mem_read(a,n),'little')
def put(a,v,n=4):vm.mem_write(a,(v&((1<<(n*8))-1)).to_bytes(n,'little'))
def signed(v,bits=32):return (v&((1<<(bits-1))-1))-(v&(1<<(bits-1)))
def actor(i):return (0xbcd5f8+i*388)&0xffffffff
def original_id():return p[P_ID] if signed(p[P_ID])>0 else 0
def tile(x,y,b):return (b+y*228+x)&0xffffffff
def current_tile():return tile(get(0xbca2c8),get(0xbca2cc),get(0xbca2d8))
def tiles():return [tile(p[P_TARGET_X],p[P_TARGET_Y],p[P_BASE]),tile(p[P_ALT_X],p[P_ALT_Y],p[P_ALT_BASE])]
def make_regions():
 r=[(0xbca2b0,64),(0xd056e0,64),(0xd04e40,128),(0xe38dc0,192),(0xe39070,144)]
 r += [(actor(i)-32,452) for i in [original_id(),10,11,12,512]]
 for t in tiles():r += [((0xb350dc+t*2-16)&0xffffffff,34),((0xaf598c+t*4-16)&0xffffffff,36)]
 merged=[]
 for a,n in sorted(r):
  if merged and a<=merged[-1][0]+merged[-1][1]:lo,size=merged[-1];merged[-1]=(lo,max(size,a+n-lo))
  else:merged.append((a,n))
 return merged
native_file=ROOT/'metadata/menu-window-cursor-consumer-native.json' if args.production else PHASE/'metadata/native.json';d=json.loads(native_file.read_text());assert d['native_sha256']==digest
core={};sets={};branches={};cs=Cs(CS_ARCH_X86,CS_MODE_32)
for name,f in d['functions'].items():
 a=f['assembly'];assert not a['has_more'] and len(a['items'])==a['total'];entry=int(name,16);sets[entry]=set()
 for x in a['items']:
  ip=int(x['address'],16);code=bytes.fromhex(x['bytes']);assert bytes(vm.mem_read(ip,len(code)))==code;core[ip]=x;sets[entry].add(ip);i=next(cs.disasm(code,ip,count=1))
  if i.mnemonic.startswith('j') and i.mnemonic!='jmp':branches[ip]={ip+i.size,int(i.op_str,16)}
assert bytes(vm.mem_read(0x40bd64,48)).hex()==d['jump_table']['bytes']
assert get(0x4cf47f,2)==0x15ff;put(0x56f1bc,TRAP)
endpoints={0x40c180:6,0x40bda0:6,0x4b6400:4,TRAP:1};traced={0x4b3250:1,0x4b5520:1,0x51cd80:4,0x4cde60:2}
p=[];regions=[];events=[];previous=None;executed=set();edges=set();dispatch=set();maximum_watched=0;minimum_watched=0xffffffff
def event(address,argv):
 a=actor(original_id());row=[address,len(argv)]+[v&0xffffffff for v in argv]+[0]*(6-len(argv))+[0]*24
 row[8:27]=[get(0xe38dda,1),get(0xe38dca,1),get(0xd05700),get(0xd05708),get(0xbca2c8),get(0xbca2cc),get(0xbca2d8),get(a+56,1),signed(get(a+58,2),16)&0xffffffff,signed(get(a+20,2),16)&0xffffffff,signed(get(a+22,2),16)&0xffffffff,signed(get(a+36,2),16)&0xffffffff,signed(get(a+38,2),16)&0xffffffff,get(a+146,1),get(a+348),get(a+304,1),get(0xe390dc),get(0xe390c4),get(0xe390c8)]
 row[28]=get(0xe38e60,1);row[29]=get(0xbca2c4);row[31]=p[P_SEED];assert len(row)==32;events.append(row);assert len(events)<=16
def hook(cpu,address,size,user):
 global previous
 if previous in branches:edges.add((previous,address))
 previous=address
 if address==STOP:cpu.emu_stop();return
 sp=cpu.reg_read(UC_X86_REG_ESP)
 if address in traced:
  actual_args=[get(sp+4+i*4) for i in range(traced[address])]
  if address==0x51cd80:assert all(-32768<=signed(v)<=32767 for v in actual_args),'Unqualified direction coordinate domain.'
  event(address,actual_args)
 if address not in endpoints:
  assert address in core,f'Unqualified instruction {address:08x}';executed.add(address)
  if address==0x40bac7:dispatch.add(cpu.reg_read(UC_X86_REG_EAX))
  return
 argv=[get(sp+4+i*4) for i in range(endpoints[address])];a=actor(original_id());answer=0x12345678;cleanup=0;mask=p[P_MUTATIONS]
 if address in [0x40c180,0x40bda0]:
  assert argv[4]!=argv[5] and all(STACK<=v and v+4<=STACK+0x20000 for v in argv[4:])
  normalized=argv[:4]+([0xabc00001,0xabc00002] if address==0x40c180 else [0xabc00003,0xabc00004]);event(address,normalized)
  put(argv[4],p[P_OUT_X if address==0x40c180 else P_OTHER_X]);put(argv[5],p[P_OUT_Y if address==0x40c180 else P_OTHER_Y])
  if address==0x40c180:
   if mask&1:put(0xd05700,512)
   if mask&2:put(a+20,p[P_ALT_ACTOR_X],2);put(a+22,p[P_ALT_ACTOR_Y],2)
   if mask&4:put(0xbca2c8,p[P_ALT_X]);put(0xbca2cc,p[P_ALT_Y]);put(0xbca2d8,p[P_ALT_BASE])
   if mask&8:put(0xe390dc,p[P_CURSOR_VALUE]);put(0xd05708,0xabcdef00)
   answer=p[P_LINE_RETURN]
  else:
   if mask&32:put(a+56,0xed,1)
   answer=p[P_OTHER_RETURN]
 elif address==0x4b6400:
  event(address,argv)
  if mask&16:put(a+36,-1234,2);put(a+38,2345,2);put(a+146,0xbc,1)
  answer=p[P_ACTION_RETURN]
 else:event(0x56f1bc,argv);answer=p[P_CURSOR_RETURN];cleanup=4
 cpu.reg_write(UC_X86_REG_EAX,answer);cpu.reg_write(UC_X86_REG_ECX,0xdeadc0de);cpu.reg_write(UC_X86_REG_EDX,0xfefefefe);cpu.reg_write(UC_X86_REG_ESP,sp+4+cleanup);cpu.reg_write(UC_X86_REG_EIP,get(sp))
def written(cpu,access,a,n,value,user):assert STACK<=a and a+n<=STACK+0x20000 or any(lo<=a and a+n<=lo+size for lo,size in regions),f'Unwatched native write{a:08x}/{n}'
def invalid(cpu,access,a,n,value,user):raise AssertionError({'invalid':hex(a),'EIP':hex(cpu.reg_read(UC_X86_REG_EIP)),'case':dict(zip(names,p))})
vm.hook_add(UC_HOOK_CODE,hook);vm.hook_add(UC_HOOK_MEM_WRITE,written);vm.hook_add(UC_HOOK_MEM_INVALID,invalid)
def setup(case):
 global p,regions,events,maximum_watched,minimum_watched
 p=case;regions=make_regions();events=[];size=sum(n for a,n in regions);maximum_watched=max(maximum_watched,size);minimum_watched=min(minimum_watched,size)
 for i,(a,n) in enumerate(regions):vm.mem_write(a,bytes((p[P_SEED]+j*37+i*11)&255 for j in range(n)))
 for a,v in [(0xd05700,p[P_ID]),(0xd05708,p[P_ACTIVE]),(0xbca2c4,p[P_GATE]),(0xbca2c8,p[P_TARGET_X]),(0xbca2cc,p[P_TARGET_Y]),(0xbca2d8,p[P_BASE])]:put(a,v)
 put(0xe38e60,p[P_BLOCK],1);put(0xe38dda,31,1);put(0xe38dca,0,1)
 for i in range(19):put(0xe39090+i*4,0xa000005a+i*0x10000)
 put(0xe390c4,p[P_CURSOR_C4]);put(0xe390c8,p[P_CURSOR_C4] if p[P_CURSOR_MATCH]==3 else p[P_CURSOR_C8]);put(0xe390dc,get(0xe390c8) if p[P_CURSOR_MATCH]==1 else get(0xe390c4) if p[P_CURSOR_MATCH] in [2,3] else p[P_CURSOR_VALUE])
 a=actor(original_id())
 for off,v,n in [(56,p[P_STATE],1),(20,p[P_ACTOR_X],2),(22,p[P_ACTOR_Y],2),(58,p[P_ROUTE],2),(304,p[P_CAN_SPECIAL],1)]:put(a+off,v,n)
 if signed(p[P_ROUTE],16)>0:put(0xd04e58+signed(p[P_ROUTE],16)*2,p[P_ID]+(0 if p[P_OWNER_MATCH] else 1),2)
 for i in range(3):a=actor(10+i);put(a+10,p[P_KIND],1);put(a+96,p[P_TAG],2);put(a+8,0,2)
 if p[P_CHAIN]==2:put(actor(10)+10,10,1);put(actor(10)+8,11,2)
 if p[P_CHAIN]==3:put(actor(10)+10,15,1);put(actor(10)+8,11,2);put(actor(11)+10,0,1);put(actor(11)+8,12,2)
 for t in tiles():put((0xb350dc+t*2)&0xffffffff,10 if p[P_CHAIN] else 0,2);put((0xaf598c+t*4)&0xffffffff,p[P_TILE_FLAGS])
def call(entry,argv=()):
 global previous
 previous=None;sp=STACK+0x10000;put(sp,STOP)
 for i,v in enumerate(argv):put(sp+4+i*4,v)
 saved={UC_X86_REG_EBX:p[P_SEED]^0x12345678,UC_X86_REG_ESI:p[P_SEED]^0x789abcde,UC_X86_REG_EDI:p[P_SEED]^0xabcdef12,UC_X86_REG_EBP:p[P_SEED]^0x2468ace0}
 for r,v in saved.items():vm.reg_write(r,v)
 vm.reg_write(UC_X86_REG_EAX,p[P_SEED]);vm.reg_write(UC_X86_REG_ECX,p[P_SEED]^0x44556677);vm.reg_write(UC_X86_REG_EDX,p[P_SEED]^0x99aabbcc);vm.reg_write(UC_X86_REG_EFLAGS,0x202);vm.reg_write(UC_X86_REG_ESP,sp)
 vm.emu_start(entry,STOP,count=10000);assert vm.reg_read(UC_X86_REG_EIP)==STOP and vm.reg_read(UC_X86_REG_ESP)==sp+4,hex(entry)
 for r,v in saved.items():assert vm.reg_read(r)==v
 return vm.reg_read(UC_X86_REG_EAX)
def native(case):
 setup(case);op=p[P_OP];result=0
 if op==0:call(0x40ba40)
 elif op==1:result=call(0x4b5520,[tiles()[0]])
 elif op==2:result=call(0x51cd80,[p[i] for i in [P_DIR_X,P_DIR_Y,P_DIR_TO_X,P_DIR_TO_Y]])&255
 elif op==3:result=call(0x4cde60,[p[P_SCALE_VALUE],p[P_SCALE_DIVISOR]])
 elif op==4:call(0x4cf460,[p[P_CURSOR_SELECTOR]]);call(0x40ba40)
 else:raise AssertionError(op)
 return {'result':result,'state':b''.join(bytes(vm.mem_read(a,n)) for a,n in regions).hex(),'events':list(events)}
rng=random.Random(0x40ba40);cases=[]
def add(op=0,**kw):
 values={'OP':op,'SEED':rng.getrandbits(32),'ID':256,'BLOCK':0,'GATE':256,'ACTIVE':0,'STATE':9,'CURSOR_MATCH':0,'CURSOR_VALUE':0x5566005a,'CURSOR_C4':0x1122005a,'CURSOR_C8':0x3344005a,'ACTOR_X':10,'ACTOR_Y':20,'OUT_X':15,'OUT_Y':25,'OTHER_X':33,'OTHER_Y':44,'TARGET_X':5,'TARGET_Y':2,'BASE':4096,'ALT_X':7,'ALT_Y':3,'ALT_BASE':4096,'ALT_ACTOR_X':-10,'ALT_ACTOR_Y':-20,'MUTATIONS':0,'ROUTE':1,'OWNER_MATCH':1,'CHAIN':1,'KIND':11,'TAG':5,'CAN_SPECIAL':1,'TILE_FLAGS':0,'ACTION_RETURN':256,'LINE_RETURN':0xffffffff,'OTHER_RETURN':0x80000000,'CURSOR_SELECTOR':5,'CURSOR_RETURN':0x89abcdef,'SCALE_VALUE':5,'SCALE_DIVISOR':3,'DIR_X':0,'DIR_Y':0,'DIR_TO_X':4,'DIR_TO_Y':1}
 values.update({k.upper():v for k,v in kw.items()});cases.append([values[n]&0xffffffff for n in names])
for state in range(256):add(state=state)
for block,id,gate,active,state in itertools.product([0,1,128],[0,-1,1,256],[0,1,256],[0,256],[8,9]):add(block=block,id=id,gate=gate,active=active,state=state)
for state,id,active,match in itertools.product([9,10,11,12,13,20],[1,256],[0,256],range(4)):add(state=state,id=id,active=active,cursor_match=match)
for kind in range(256):add(1,kind=kind)
for chain,kind,tag in itertools.product(range(4),[0,10,11,14,15,255],[0,1,32767,32768,65535]):add(1,chain=chain,kind=kind,tag=tag)
for chain,kind,tag,answer in itertools.product(range(4),[0,10,11,14,15,255],[0,1,255,256,32767,32768,65535],[0,1,256,0x80000000]):add(state=12,chain=chain,kind=kind,tag=tag,action_return=answer)
for special,flags,mask,route,owner in itertools.product([0,1,128],[0,4,0x10000004,0xffffffff],[0,1,2,4,8,16,32,63],[0,1,17],[0,1]):add(state=13,can_special=special,tile_flags=flags,mutations=mask,route=route,owner_match=owner)
for state,mask,line,action in itertools.product([9,10,11,12,13,20],[0,1,2,3,4,8,15,16,32,63],[0,0xffffffff],[0,256]):add(state=state,mutations=mask,line_return=line,action_return=action)
for state,x,y in itertools.product([9,11,12,13,20],[0x1234abcd,0xffff8000,0x80000000],[0xfedc3456,0x7fffffff]):add(state=state,out_x=x,out_y=y,mutations=63)
for state,selector,gate,active in itertools.product([9,10,12,13],[0,4,5,0x10000004],[0,256],[0,256]):add(4,state=state,cursor_selector=selector,gate=gate,active=active)
for state,mask in itertools.product([9,10,12,13],[0,4,63]):add(state=state,target_x=0x01000005,target_y=2,base=0xff001000,alt_x=0x10000007,alt_y=3,alt_base=0xf0001000,mutations=mask)
for dx,dy,sx,sy in itertools.product([0,1,3,4,399,400,401],[0,1,3,4,100],[-1,1],[-1,1]):add(2,dir_to_x=dx*sx,dir_to_y=dy*sy)
for x,y,tx,ty in [(-32768,-32768,32767,32767),(32767,32767,-32768,-32768),(-1,1,-1,0),(1,-1,1,0)]:add(2,dir_x=x,dir_y=y,dir_to_x=tx,dir_to_y=ty)
for value,divisor in itertools.product([0,1,-1,256,0x7fffffff],[0,1,2,100,-1]):add(3,scale_value=value,scale_divisor=divisor)
selected=cases[args.start:args.start+args.limit if args.limit else None];expected=[native(c) for c in selected]
owned_entries=[0x40ba40,0x4b5520];owned=set().union(*(sets[a] for a in owned_entries));owned_edges={(a,b) for a,z in branches.items() if a in owned for b in z};complete=owned<=executed and owned_edges<=edges and dispatch==set(range(12))
source_paths=[product/x for x in ['src/unit_00085.c','src/unit_01455.c','include/menu_window_cursor_consumer.h','tests/menu_window_cursor_consumer.c','tests/menu_window_cursor_consumer_actual.c','tests/menu_window_cursor_consumer_case.h']]+[ROOT/x for x in ['src/unit_01427.c','src/unit_02334.c','src/unit_01731.c','src/unit_01769.c','include/actor_callbacks_human.h','include/menu_cursor.h','include/restore_types.h','include/standalone_data.h','src/unit_01768.c']]+[PHASE/'build/CMakeLists.txt',PHASE/'tools/extract_menu_window_cursor_consumer_native.py',Path(__file__).resolve()]
report={'format':1,'generated_at':datetime.now(timezone.utc).isoformat(),'production':args.production,'passed':False,'complete_case_set':not args.limit and not args.start,'checked':len(selected),'native_sha256':digest,'compiled_C_path':args.exe.resolve().relative_to(ROOT).as_posix(),'compiled_C_sha256':sha(args.exe.resolve()),'source_hashes':{x.relative_to(ROOT).as_posix():sha(x) for x in source_paths},'native_exports':{native_file.relative_to(ROOT).as_posix():sha(native_file)},'qualified_controller':{'complete':complete,'owned_entries':[f'{a:08x}' for a in owned_entries],'instructions':len(owned),'executed':len(owned&executed),'missing_instructions':[f'{a:08x}' for a in sorted(owned-executed)],'conditional_edges':len(owned_edges),'edges_executed':len(owned_edges&edges),'missing_edges':[[f'{a:08x}',f'{b:08x}'] for a,b in sorted(owned_edges-edges)],'dispatch_indices':sorted(dispatch),'excluded_instructions':[],'excluded_edges':[]},'actual_helper_coverage':{f'{a:08x}':{'executed':len(z&executed),'total':len(z),'missing':[f'{x:08x}' for x in sorted(z-executed)]} for a,z in sets.items() if a not in owned_entries},'watched_memory':{'minimum_bytes':minimum_watched,'maximum_bytes':maximum_watched,'actor_records':'Five selected/chain/alternate records with32-byte outer canaries; merged overlapping neighborhoods, everybyte compared.','tile_guards':'16-byte guards around each signedWORD head andDWORD tile flag; global regions include neighboring byte canaries.','all_native_writes_guarded':True},'native_ABI':{'controller_cdecl_arguments':0,'lookup_cdecl_arguments':1,'RET_cleanup':0,'stack_and_saved_registers':['ESP','EBX','ESI','EDI','EBP'],'deep_endpoint_clobbers':['EAX','ECX','EDX'],'output_locals':'Distinct fullDWORD pairs at actual stack addresses; normalized pointer identities and ordered args compared.','direction_return':'Only nativeAL consumed and compared.','action_return':'FullDWORD zero/nonzero consumed; ignored line helper EAX not promoted to success flags.'},'variants':{},'original_game_process_executed':False,'original_machine_code_fallback_used':False,'complete_actor_order_gameplay_claimed':False,'limitations':['The whole215-instruction controller and23-instruction linked-actor lookup execute real handwrittenC/native bodies. Three larger real function boundaries40C180(214),40BDA0(324),4B6400(213) use finite exact argument/output/return/mutation contracts in this oracle; production calls existingC, whose internal algorithms remain outside this proof. No internal function seam, production stub or no-op is introduced.','Actual unchanged1427 route clearing,2334 direction and1731 scaling execute. Direction inputs remain within signedWORD world coordinates, so absolute deltas andscaledratios fit32bits. Caller output truncation/fullDWORD propagation is also tested outside that coordinate subset on branches that do not call direction. Valid acyclic lookup chains and mapped actor/tile memory are required; invalid native dereferences or infinite cycles are not converted to successful fallback.','Synthetic mapped wrap fixtures prove native uint32 ID/stride/tile arithmetic and exact DWORD arguments; they do not establish that out-of-world coordinates are accepted by the larger movement algorithms.','The accepted cursor1769 selector executes unchanged in composed selection→commit cases; SetCursor is a finite WINAPI boundary, never a live GUI operation. Existing startup resources and physical cursor effects remain outside this proof.','Frozen1768 GetCursorPos FALSE difference remains open: native ignores BOOL and consumes unspecifiedPOINT, currentC returns early. No invented POINT or failure equivalence is claimed. This phase does not modify1768 or frozenmodal/cursor tests.']}
out=ROOT/'metadata/menu-window-cursor-consumer-emulation.json' if args.production else PHASE/'metadata/menu-window-cursor-consumer-emulation.json'
for name,coverage in report['actual_helper_coverage'].items():
 entry=int(name,16);helper_edges={(a,b) for a,z in branches.items() if a in sets[entry] for b in z}
 coverage['conditional_edges']=len(helper_edges);coverage['edges_executed']=len(helper_edges&edges)
 coverage['missing_edges']=[[f'{a:08x}',f'{b:08x}'] for a,b in sorted(helper_edges-edges)]
report['outcomes']={'direct_lookup_full_DWORD_returns':sorted({r['result'] for c,r in zip(selected,expected) if c[P_OP]==1}),'direct_direction_AL_returns':sorted({r['result'] for c,r in zip(selected,expected) if c[P_OP]==2}),'controller_state_inputs':sorted({c[P_STATE] for c in selected if c[P_OP]==0}),'deep_action_full_DWORD_returns':sorted({c[P_ACTION_RETURN] for c in selected}),'mutation_masks':sorted({c[P_MUTATIONS] for c in selected})}
try:
 if report['complete_case_set']:assert complete,report['qualified_controller']
 if report['complete_case_set']:assert all(report['actual_helper_coverage'][f'{a:08x}']['executed']==len(sets[a]) for a in [0x4b3250,0x51cd80,0x4cde60]),report['actual_helper_coverage']
 for label,path in [('Debug',args.exe),('O2',args.optimized_exe)]:
  path=path.resolve();smoke=subprocess.run([str(path)],capture_output=True,text=True,timeout=10);assert smoke.returncode==0,smoke.stderr
  proc=subprocess.Popen([str(path),'--oracle-batch'],stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True)
  try:
   assert json.loads(proc.stdout.readline())=={'fields':FIELDS,'actor_bytes':388}
   for i,(case,want) in enumerate(zip(selected,expected)):
    proc.stdin.write(' '.join(map(str,case))+'\n');proc.stdin.flush();line=proc.stdout.readline();assert line,(label,i,proc.poll(),proc.stderr.read());actual=json.loads(line)
    if actual!=want:
     field=next(k for k in want if actual[k]!=want[k]);detail={'variant':label,'case':args.start+i,'input':dict(zip(names,case)),'field':field,'C':actual[field],'native':want[field]};(PHASE/'metadata/last-mismatch.json').write_text(json.dumps(detail,indent=2));raise AssertionError({k:v for k,v in detail.items() if k not in ['C','native']})
   proc.stdin.close();proc.wait(timeout=10);assert proc.returncode==0,proc.stderr.read()
  finally:
   if proc.poll() is None:proc.kill();proc.wait()
  report['variants'][label]={'path':path.relative_to(ROOT).as_posix(),'sha256':sha(path),'checked':len(selected),'smoke_exit':smoke.returncode}
 assert all(sha(ROOT/path)==value for path,value in report['source_hashes'].items()),'Concurrent source drift.'
 assert all(sha(ROOT/path)==value for path,value in report['native_exports'].items()),'Concurrent native evidence drift.'
 report['comparisons']=len(selected)*2;report['passed']=True
finally:out.write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({'passed':report['passed'],'checked':len(selected),'comparisons':report.get('comparisons'),'coverage':report['qualified_controller'],'actual_helpers':report['actual_helper_coverage']}))
