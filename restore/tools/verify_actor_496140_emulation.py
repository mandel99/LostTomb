"""Pinned bazaar-buyer callback, shopping dispatcher and receiver-bound queue proof."""
from pathlib import Path
from datetime import datetime,timezone
import argparse,hashlib,itertools,json,struct,subprocess,sys
ROOT=Path(__file__).resolve().parents[1];sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
parser=argparse.ArgumentParser();parser.add_argument('--production',action='store_true');parser.add_argument('--exe',type=Path,default=ROOT/'build/agent-actor-496140/out/Debug/restore_actor_496140_verify.exe');parser.add_argument('--debug-limit',type=int);parser.add_argument('--debug-start',type=int,default=0);args=parser.parse_args();exe=args.exe.resolve();phase=Path('.') if args.production else Path('artifacts/actor_callback_496140_phase');human=Path('.') if args.production else Path('artifacts/actor_callbacks_human_phase')
sha=lambda b:hashlib.sha256(b).hexdigest();data=(ROOT/'build/smoke-game/Pharaoh.exe').read_bytes();digest=sha(data);assert digest=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
u16=lambda b,p:struct.unpack_from('<H',b,p)[0];u32=lambda b,p:struct.unpack_from('<I',b,p)[0];s16=lambda x:struct.unpack('<h',struct.pack('<H',x&65535))[0]
pe=u32(data,60);opt=pe+24;table=opt+u16(data,pe+20);base=u32(data,opt+28);size=u32(data,opt+56);vm=Uc(UC_ARCH_X86,UC_MODE_32);vm.mem_map(base,(size+4095)&~4095)
for i in range(u16(data,pe+6)):
 q=table+i*40;vm.mem_write(base+u32(data,q+12),data[u32(data,q+20):u32(data,q+20)+u32(data,q+16)])
ALT=0x30010000;STOP=0x33000000;STACK=0x34000000
for at,n in [(0x30000000,0x20000),(STOP,4096),(STACK,0x200000)]:vm.mem_map(at,n)
def write(a,v,n=4):vm.mem_write(a&0xffffffff,(v&((1<<(n*8))-1)).to_bytes(n,'little'))
def read(a):return int.from_bytes(vm.mem_read(a&0xffffffff,4),'little')
def word(a):return int.from_bytes(vm.mem_read(a&0xffffffff,2),'little')
def byte(a):return vm.mem_read(a&0xffffffff,1)[0]
def actor(id):return (0xbcd5f8+id*388)&0xffffffff
exports=['metadata/actor-callback-round1-native.json','metadata/actor-callback-496140-helpers-native.json','metadata/actor-callback-496140-actions-native.json','metadata/actor-callback-human-queries-native.json'];dossiers=[json.loads((ROOT/p).read_text()) for p in exports];instructions={};function_addresses={};audit={};executed=set();edges=set();last=None
for index,entries in [(0,['00496140']),(1,['00496060','004a35e0']),(2,['004f6570']),(3,['004b1d50','004b3250'])]:
 for entry in entries:
  f=dossiers[index]['functions'][entry]['assembly'];assert not f['has_more'] and len(f['items'])==f['total'];function_addresses[entry]=set()
  for ins in f['items']:
   address=int(ins['address'],16);raw=bytes.fromhex(ins['bytes']);assert bytes(vm.mem_read(address,len(raw)))==raw;instructions[address]=ins;function_addresses[entry].add(address)
  audit[entry]={'instructions':len(f['items']),'bytes':sum(len(bytes.fromhex(i['bytes'])) for i in f['items'])}
controlled={0x4b17d0:1,0x4a5da0:2,0x495cf0:4,0x496010:4,0x495e10:3,0x495f00:3};contract_audit={}
for at in controlled:
 d=dossiers[3] if at in [0x4b17d0,0x4a5da0] else dossiers[2];f=d['functions'][f'{at:08x}']['assembly'];assert not f['has_more'] and len(f['items'])==f['total']
 for ins in f['items']:assert bytes(vm.mem_read(int(ins['address'],16),len(bytes.fromhex(ins['bytes']))))==bytes.fromhex(ins['bytes'])
 contract_audit[hex(at)]={'instructions':len(f['items']),'stack_arguments':controlled[at],'execution':'controlled deep movement or shopping action'}
regions=[(0xd056f0,20),(0xe94df8,20),(0xbca2d8,20)];p=[];home=0;events=[]
def event(address,args0):
 a=actor(read(0xd056f8));r=[address,len(args0)]+[v&0xffffffff for v in args0]+[0]*(32-2-len(args0));r[8:32]=[read(0xd056f8),byte(a+56),byte(a+17),word(a+54),byte(a+1),byte(a+14),byte(a+114),byte(a+12),read(a+4),read(a+384),read(0xe94e00),word(a+58),word(a+36),word(a+38),word(a+66),word(a+90),word(a+94),byte(a+147),read(a+156),read(a+160),read(a+172),word(home+44),word(home+182),byte(home)];events.append(r)
def mutate(at):
 if at!=p[35]:return
 flags=p[36]
 if flags&1:write(0xd056f8,p[2])
 a=actor(read(0xd056f8))
 for flag,off,val,n in [(2,56,p[37],1),(4,54,p[38],2),(8,17,p[39],1),(32,1,p[42],1),(64,94,p[43],2),(128,147,p[44],1),(512,90,p[60],2)]:
  if flags&flag:write(a+off,val,n)
 if flags&16:write(0xbca2e0,p[41])
 if flags&256:
  for i in range(4):write(a+156+i*4,p[45+i])
def code(cpu,a,n,user):
 global last
 if a==STOP:return
 if last is not None:edges.add((last,a))
 last=a;executed.add(a)
 if a in [0x4b1d50,0x4b3250,0x496060,0x4a35e0,0x4f6570]:
  sp=cpu.reg_read(UC_X86_REG_ESP);v=[read(sp+4+i*4) for i in range(3 if a==0x496060 else 1)]
  if a==0x4f6570:v=[cpu.reg_read(UC_X86_REG_ECX)]+v
  event(a,v)
 elif a in [0x4b1d94,0x4b3290,0x496117,0x496125,0x4a35fd,0x4a3606,0x4a362f,0x4f6598,0x4f65a2]:
  mutate(0x4b1d50 if a==0x4b1d94 else 0x4b3250 if a==0x4b3290 else 0x496060 if a in [0x496117,0x496125] else 0x4a35e0 if a in [0x4a35fd,0x4a3606,0x4a362f] else 0x4f6570)
 elif a in controlled:
  sp=cpu.reg_read(UC_X86_REG_ESP);v=[read(sp+4+i*4) for i in range(controlled[a])];event(a,v)
  if a==0x4a5da0:write(actor(read(0xd056f8))+17,p[18],1)
  mutate(a);cpu.reg_write(UC_X86_REG_EAX,p[34]);cpu.reg_write(UC_X86_REG_EIP,read(sp));cpu.reg_write(UC_X86_REG_ESP,sp+4);return
 assert a in instructions,'Native escaped '+hex(a)
vm.hook_add(UC_HOOK_CODE,code)
def fill_queue(q):
 for off,value in [(0,p[33]),(4,p[29]),(8,p[30]),(12,p[28])]:write(q+off,value)
 for i in range(51):write(q+20+i*4,0x12340000+i)
 if p[32]<51:write(q+20+p[32]*4,p[31])
def native(case):
 global p,home,last
 p=case;events.clear();last=None
 for at,n in regions:vm.mem_write(at,bytes([(0xa5^p[57])&255])*n)
 home=(0x7999d8+s16(p[9])*264)&0xffffffff;vm.mem_write(home-8,b'\xa5'*280);write(home,p[10],1);write(home+44,p[11],2);write(home+182,p[12],2)
 vm.mem_write(0xeaaf60,b'\xa5'*(50*224+16));vm.mem_write(0xeaaf68,bytes(50*224))
 if p[27]<50:fill_queue(0xeaaf68+p[27]*224)
 vm.mem_write(ALT-8,b'\xa5'*240);fill_queue(ALT);queue_self=ALT if p[58] else 0xeaaf68+(p[27] if p[27]<50 else 0)*224;route_address=(0xd04e58+s16(p[19])*2)&0xffffffff
 if s16(p[19])>0:vm.mem_write(route_address-8,b'\xa5'*20);write(route_address,p[20],2)
 for i,id in enumerate(p[1:4]):
  a=actor(id);vm.mem_write(a-8,b'\xa5'*404)
  for off,val,n in [(1,p[6],1),(4,p[50]+i,4),(10,p[8]+i,1),(12,p[54],1),(14,p[52],1),(17,p[7],1),(36,0x1234,2),(38,0x2345,2),(44,p[13]+i,2),(46,p[14]+i,2),(54,p[5],2),(56,p[4],1),(58,p[19],2),(66,p[55],2),(90,(p[9]+i)%4000,2),(94,p[15]+i,2),(114,p[53],1),(147,p[22],1),(384,p[51]+i,4)]:write(a+off,val,n)
  for j in range(4):write(a+156+j*4,p[23+j]);write(a+172+j*4,0x12345678+j)
  write(0x7090ec+((p[8]+i)&255)*56,p[17]+i*256)
 for at,val in [(0xd056f8,p[1]),(0xbca2e0,p[16]),(0xf55d74,p[21]),(0xf55d6c,p[21]+256)]:write(at,val)
 sp=STACK+0x1e0000;write(sp,STOP);argv=[];cleanup=0
 if p[0]==1:argv=[p[1],p[9],p[15]]
 elif p[0] in [2,4,5]:argv=[p[1]]
 elif p[0]==3:argv=[p[1]];vm.reg_write(UC_X86_REG_ECX,queue_self);cleanup=4
 for i,v in enumerate(argv):write(sp+4+i*4,v)
 saved=[(UC_X86_REG_EBX,0x11223344),(UC_X86_REG_EBP,0x22334455),(UC_X86_REG_ESI,0x33445566),(UC_X86_REG_EDI,0x44556677)]
 for reg,val in saved:vm.reg_write(reg,val)
 vm.reg_write(UC_X86_REG_ESP,sp);vm.reg_write(UC_X86_REG_EFLAGS,2);vm.emu_start([0x496140,0x496060,0x4a35e0,0x4f6570,0x4b1d50,0x4b3250][p[0]],STOP,count=200000)
 assert vm.reg_read(UC_X86_REG_EIP)==STOP and vm.reg_read(UC_X86_REG_ESP)==sp+4+cleanup,'Native stack/budget differs';assert all(vm.reg_read(reg)==val for reg,val in saved),'Native saved registers differ'
 state=b''.join(bytes(vm.mem_read(at,n)) for at,n in regions)
 for i,id in enumerate(p[1:4]):state+=bytes(vm.mem_read(actor(id)-8,404))+bytes(vm.mem_read(0x7090ec+((p[8]+i)&255)*56-8,20))
 state+=bytes(vm.mem_read(home-8,280))
 if s16(p[19])>0:state+=bytes(vm.mem_read(route_address-8,20))
 state+=bytes(vm.mem_read(0xeaaf60,50*224+16))+bytes(vm.mem_read(ALT-8,240))
 result=vm.reg_read(UC_X86_REG_EAX)&255 if p[0]==2 else vm.reg_read(UC_X86_REG_EAX) if p[0]==3 else 0
 return {'result':result,'state':state.hex(),'events':events.copy()}
fields=['op','id','next_id','third_id','action','timer','frame','direction','kind','home','active','owner1','owner2','original_x','original_y','target','camera','speed','move','route','route_owner','tag','stops','resource0','resource1','resource2','resource3','queue_index','capacity','head','tail','item','match_slot','queue_active','ignored_return','at','mutation','new_action','new_timer','new_direction','spare','new_camera','new_frame','new_target','new_stops','new_resource0','new_resource1','new_resource2','new_resource3','spare2','image','counter','status','shadow','animation','threshold','spare3','canary','alternate','spare4','new_home','spare5','spare6','spare7']
default=[0]*64
for key,value in dict(id=1,next_id=256,third_id=511,action=8,timer=100,frame=0,kind=39,home=1,active=1,owner1=1,original_x=123,original_y=234,target=345,speed=12,move=8,route=1,route_owner=1,tag=1000,resource0=0,resource1=8,resource2=12,resource3=0xffffffff,queue_index=0xffffffff,capacity=5,item=1,match_slot=0xffffffff,queue_active=256,new_action=4,new_timer=127,new_direction=6,new_camera=6,new_frame=255,new_target=256,new_stops=1,new_resource0=8,new_resource1=0,new_resource2=0xffffffff,new_resource3=11,image=0x12345678,counter=0x87654321,status=1,animation=3,threshold=0x1234,new_home=2).items():default[fields.index(key)]=value
cases=[]
def add(**kw):
 q=default.copy()
 for key,val in kw.items():q[fields.index(key)]=val&0xffffffff
 if 'owner1' not in kw:q[11]=q[1]
 if 'route_owner' not in kw:q[20]=q[1]
 if 'item' not in kw:q[31]=q[1]
 if q[1]==q[2]:q[2]=512
 if q[1]==q[3]:q[3]=513
 assert len(set(q[1:4]))==3 and q[22]<=3 and q[44]<=3
 cases.append(q)
for action,frame,camera in itertools.product(range(256),[0,11,255],[0,6]):add(action=action,frame=frame,camera=camera)
for action,direction in itertools.product([8,9],[0,1,7,8,9,10,11,127,128,129,255]):add(action=action,move=direction)
for active,owner1,owner2 in itertools.product([0,1,2],[0,1,256,0xffff],[0,1,256,0xffff]):add(active=active,owner1=owner1,owner2=owner2,action=7)
for action,timer in itertools.product([4,7],[-32768,-1,0,1,99,100,101,126,127,128,32766,32767]):add(action=action,timer=timer)
for id,home,kind in itertools.product([1,256,1999],[0,1,256,3999],[0,39,109]):add(id=id,home=home,kind=kind)
for stops,resource,ignored in itertools.product(range(4),[0,1,7,8,9,11,12,256,0xffffffff],[0,1,256,0xffffffff]):add(op=1,stops=stops,resource0=resource,resource1=resource,resource2=resource,resource3=resource,ignored_return=ignored)
for op,action in itertools.product([0,2],[0,4,5,6,7,8,9,255]):add(op=op,action=action,timer=100)
for op,queue_index,active,shape,slot in itertools.product([2,3],[0,1,49],[0,1,256],[(0,0,0),(1,0,0),(5,0,3),(5,3,1),(50,49,3),(51,50,3)],[0,1,2,3,4,48,49,50,0xffffffff]):add(op=op,action=7,queue_index=queue_index,queue_active=active,capacity=shape[0],head=shape[1],tail=shape[2],match_slot=slot)
for alt,id,item,slot in itertools.product([0,1],[1,256],[0,1,256,0x10001,0xffffffff],[0,1,2,3,4]):add(op=3,alternate=alt,id=id,item=item,queue_index=0,head=3,tail=1,match_slot=slot)
profiles=[(0,5,0x4b17d0),(0,4,0x4b1d50),(0,8,0x4a5da0),(0,8,0x496060),(0,0,0x4b3250),(0,7,0x4a35e0),(0,7,0x4f6570),(1,8,0x495cf0),(1,8,0x496010),(1,8,0x495e10),(1,8,0x495f00)]
for op,action,at in profiles:
 for mutation in [1,3,5,9,17,33,65,129,257,513]:
  add(op=op,action=action,at=at,mutation=mutation,queue_index=0,head=0,tail=2,match_slot=0,resource0=12 if at==0x495e10 else 8 if at==0x496010 else 0,resource1=12 if at==0x495e10 else 8,resource2=12,resource3=0xffffffff)
for action,camera,direction in itertools.product([4,8,9],[0,2,4,6,256],[0,2,6,8,9,10,128,255]):add(action=action,camera=camera,direction=direction,move=direction)
for tag in [0,255,256,65536,0x12345678,0x80000000,0xffffffff]:add(tag=tag,action=4);add(tag=tag,action=5)
for value in [-32768,-1,0,1,32767]:add(original_x=value,original_y=value,target=value,action=8)
for speed in [0,1,256,0x12345678,0x80000000]:add(speed=speed,action=8);add(speed=speed,action=9)
for route,owner in itertools.product([-1,0,1,255,999],[0,1,256,0xffff]):add(op=5,route=route,route_owner=owner)
for timer in [-32768,-1,0,126,127,128,32767]:add(op=4,timer=timer)
cases=list(dict.fromkeys(tuple(c) for c in cases))
if args.debug_start:cases=cases[args.debug_start:]
if args.debug_limit:cases=cases[:args.debug_limit]
smoke=subprocess.run([str(exe)],cwd=ROOT,capture_output=True,text=True,timeout=30);assert smoke.returncode==0,smoke.stdout+smoke.stderr
process=subprocess.Popen([str(exe),'--oracle-batch'],cwd=ROOT,stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE)
report={'format':1,'generated_at':datetime.now(timezone.utc).isoformat(),'passed':False,'checked':0,'stage':'production' if args.production else phase.as_posix(),'native_sha256':digest,'compiled_C_path':exe.relative_to(ROOT).as_posix(),'compiled_C_sha256':sha(exe.read_bytes()),'original_game_process_executed':False,'computer_UI_automation_used':False,'original_machine_code_fallback_used':False,'method':'Complete154-instruction496140 callback,77-instruction shopping dispatcher,34-instruction pending-queue predicate and24-instruction explicit-receiver queue scan versus handwritten C, composing accepted14+17 timer/route helpers. Full actor/house/queue/canary state, ordered deep action arguments and intermediate data, cached building/original shopping actor versus reloaded global current actor, exact BYTE/WORD/DWORD fields, consumed AL/full-EAX/ignored returns, stack and saved registers compare.','limitations':['Six deeper movement/shopping actions remain controlled endpoints; their ordered arguments/selection and callback-visible effects are checked, not their full algorithms, file interactions or actual bazaar simulation.','Valid actor/building/type/route storage follows IDs1..1999, buildings0..3999, type bytes0..111 and positive routes1..999. Shopping-stop BYTE is in0..3 before the native unchecked four-DWORD history write. No success guard is added for invalid native array indices.','Fifty224-byte queue objects use head/tail indices0..capacity-1 with capacity1..51 for nonempty rings. Empty head==tail with capacity0 is also checked. Invalid nonempty ring invariants retain native behavior and are outside equivalence; they are not converted into successful absence.','4F6570 uses an explicit owned C receiver only through rewritten1358. The unowned legacy2187 entry still has dropped ECX for other callers and remains a separate reconstruction gap.','Callbacks return void; volatile EAX/ECX/EDX are unconsumed and not compared. Only AL is consumed from4A35E0; queue membership returns full canonical EAX0/1. Both resource action return values are deliberately ignored, as native. No actual registry/UI/framebuffer integration is claimed here.']}
case=None
try:
 assert json.loads(process.stdout.readline())=={'fields':64}
 for case in cases:
  process.stdin.write((' '.join(map(str,case))+'\n').encode());process.stdin.flush();line=process.stdout.readline();assert line,'C exited '+process.stderr.read().decode(errors='replace');actual=json.loads(line);expected=native(case)
  for key in expected:
   if actual[key]!=expected[key]:
    detail=[actual[key],expected[key]]
    if key=='state':
     aa=bytes.fromhex(actual[key]);bb=bytes.fromhex(expected[key]);ix=next((i for i,(a,b) in enumerate(zip(aa,bb)) if a!=b),None);detail={'first_difference':ix,'C_bytes':len(aa),'native_bytes':len(bb),'C_slice':aa[max(0,(ix or 0)-8):(ix or 0)+16].hex(),'native_slice':bb[max(0,(ix or 0)-8):(ix or 0)+16].hex()}
    elif key=='events':
     ix=next((i for i,(a,b) in enumerate(zip(actual[key],expected[key])) if a!=b),min(len(actual[key]),len(expected[key])));detail={'first_difference':ix,'C_count':len(actual[key]),'native_count':len(expected[key]),'C_event':actual[key][ix:ix+1],'native_event':expected[key][ix:ix+1]}
    raise AssertionError(key+' differs '+str(detail))
  report['checked']+=1
  if report['checked']%500==0:print('checked',report['checked'],file=sys.stderr,flush=True)
 report.update(passed=True,complete_case_set=not bool(args.debug_limit or args.debug_start))
except Exception as error:report.update(error=str(error),case=case,native_eip=hex(vm.reg_read(UC_X86_REG_EIP)))
finally:process.stdin.close();process.wait(timeout=10)
report['native_byte_audit']=audit;report['newly_restored_native_instructions']=289;report['unchanged_composed_native_instructions']=31;report['native_bytes_verified']=sum(v['bytes'] for v in audit.values());report['native_function_instruction_coverage']={a:{'executed':len(v&executed),'total':len(v),'unreached':[hex(i) for i in sorted(v-executed)]} for a,v in function_addresses.items()};report['controlled_deep_contracts']=contract_audit;branches={}
for a,ins in instructions.items():
 decoded=next(iter(Cs(CS_ARCH_X86,CS_MODE_32).disasm(bytes.fromhex(ins['bytes']),a)),None)
 if decoded and decoded.mnemonic.startswith('j') and decoded.mnemonic!='jmp':branches[hex(a)]={'instruction':ins['instruction'],'taken':(a,int(decoded.op_str,16)) in edges,'fallthrough':(a,a+len(decoded.bytes)) in edges}
report['native_conditional_branch_coverage']=branches;dispatch={}
for at,table0,n in [(0x4961ed,0x496394,6),(0x49609f,0x496128,2)]:
 raw=bytes(vm.mem_read(table0,n*4));dispatch[hex(at)]={'table':hex(table0),'table_sha256':sha(raw),'targets':{hex(t):(at,t) in edges for t in sorted(set(struct.unpack('<'+'I'*n,raw)))}}
report['native_indirect_dispatch_coverage']=dispatch;report['smoke']={'passed':True,'output':smoke.stdout.strip()}
if report.get('complete_case_set'):
 missing_instructions={a:v['unreached'] for a,v in report['native_function_instruction_coverage'].items() if v['unreached']};missing_edges=[(a,k) for a,v in branches.items() for k in ['taken','fallthrough'] if not v[k]]+[(a,t) for a,v in dispatch.items() for t,hit in v['targets'].items() if not hit];report['unqualified_missing_edges']=missing_edges
 if missing_instructions or missing_edges:report.update(passed=False,error='Incomplete native coverage '+str([missing_instructions,missing_edges]))
sources=[(phase/'include/actor_callback_496140.h').as_posix()]+[(phase/f'runtime/{f}.c').as_posix() for f in ['actor_callback_496140','actor_callback_496140_queue']]+[(phase/f'src/unit_{n:05d}.c').as_posix() for n in [1335,1358]]+[(human/'include/actor_callbacks_human.h').as_posix()]+[(human/f'src/unit_{n:05d}.c').as_posix() for n in [1416,1427]]+['tests/actor_496140.c','tests/actor_496140_helpers.c','tools/verify_actor_496140_emulation.py']
report['source_hashes']={s:sha((ROOT/s).read_bytes()) for s in sources};report['native_exports']={s:sha((ROOT/s).read_bytes()) for s in exports};name='actor-496140'+('' if args.production else '-staged')+('-debug' if args.debug_limit or args.debug_start else '')+'-emulation.json';(ROOT/'metadata'/name).write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report,indent=2));sys.exit(0 if report['passed'] else 1)
