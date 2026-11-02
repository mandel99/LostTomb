"""Original immigrant/emigrant callback CFGs versus handwritten C, with typed state helpers."""
from pathlib import Path
from datetime import datetime,timezone
import argparse,hashlib,itertools,json,struct,subprocess,sys
ROOT=Path(__file__).resolve().parents[1];sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
parser=argparse.ArgumentParser();parser.add_argument('--production',action='store_true');parser.add_argument('--exe',type=Path,default=ROOT/'build/agent-actor-human/out/Debug/restore_actor_human_verify.exe');parser.add_argument('--debug-limit',type=int);parser.add_argument('--debug-start',type=int,default=0);args=parser.parse_args();exe=args.exe.resolve();phase=Path('.') if args.production else Path('artifacts/actor_callbacks_human_phase')
sha=lambda b:hashlib.sha256(b).hexdigest();data=(ROOT/'build/smoke-game/Pharaoh.exe').read_bytes();digest=sha(data);assert digest=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
u16=lambda b,p:struct.unpack_from('<H',b,p)[0];u32=lambda b,p:struct.unpack_from('<I',b,p)[0];s32=lambda x:struct.unpack('<i',struct.pack('<I',x&0xffffffff))[0];s16=lambda x:struct.unpack('<h',struct.pack('<H',x&65535))[0]
pe=u32(data,60);opt=pe+24;table=opt+u16(data,pe+20);base=u32(data,opt+28);size=u32(data,opt+56);vm=Uc(UC_ARCH_X86,UC_MODE_32);vm.mem_map(base,(size+4095)&~4095)
for i in range(u16(data,pe+6)):
 q=table+i*40;vm.mem_write(base+u32(data,q+12),data[u32(data,q+20):u32(data,q+20)+u32(data,q+16)])
STOP=0x33000000;STACK=0x34000000;vm.mem_map(STOP,4096);vm.mem_map(STACK,0x200000)
def write(a,v,n=4):vm.mem_write(a&0xffffffff,(v&((1<<(n*8))-1)).to_bytes(n,'little'))
def read(a):return int.from_bytes(vm.mem_read(a&0xffffffff,4),'little')
def word(a):return int.from_bytes(vm.mem_read(a&0xffffffff,2),'little')
def byte(a):return vm.mem_read(a&0xffffffff,1)[0]
def actor(id):return (0xbcd5f8+id*388)&0xffffffff
exports=['metadata/actor-callback-round1-native.json','metadata/actor-callback-human-queries-native.json'];dossiers=[json.loads((ROOT/p).read_text()) for p in exports];instructions={};function_addresses={};audit={};executed=set();edges=set();last=None
for d,entries in [(dossiers[0],['00489ca0','0048a260']),(dossiers[1],['004b1d50','004b3250','0047a1c0'])]:
 for entry in entries:
  a=d['functions'][entry]['assembly'];assert not a['has_more'] and len(a['items'])==a['total'];function_addresses[entry]=set()
  for ins in a['items']:
   address=int(ins['address'],16);raw=bytes.fromhex(ins['bytes']);assert bytes(vm.mem_read(address,len(raw)))==raw;instructions[address]=ins;function_addresses[entry].add(address)
  audit[entry]={'instructions':len(a['items']),'bytes':sum(len(bytes.fromhex(i['bytes'])) for i in a['items'])}
controlled={0x4b17d0:1,0x47fe60:4,0x4a5da0:2,0x4b4220:6,0x4b3ff0:2,0x46cea0:2,0x4e90f0:2}
contract_audit={}
for at in controlled:
 f=dossiers[1]['functions'][f'{at:08x}']['assembly'];assert not f['has_more'] and len(f['items'])==f['total']
 for ins in f['items']:assert bytes(vm.mem_read(int(ins['address'],16),len(bytes.fromhex(ins['bytes']))))==bytes.fromhex(ins['bytes'])
 contract_audit[hex(at)]={'instructions':len(f['items']),'stack_arguments':controlled[at],'execution':'controlled deep movement/building/population endpoint'}
regions=[(0xd056f0,20),(0xe94df8,20),(0xbcb998,20),(0xeaac20,20),(0xd3b4d8,24),(0xbca2d8,20),(0x978d68,20),(0x78d7a4,24)];tags=[0xf55d64,0xf55d5c,0xf55d04,0xf55cfc,0xf56144,0xf571c4,0xf56e14,0xf5613c,0xf571bc];p=[];home=0;events=[]
def event(address,args0):
 a=actor(read(0xd056f8));r=[address,len(args0)]+[x&0xffffffff for x in args0]+[0]*(32-2-len(args0));r[8:31]=[read(0xd056f8),byte(a+56),byte(a+17),word(a+54),byte(a+1),byte(a+14),byte(a+100),byte(a+64),read(a+4),read(a+384),read(0xe94e00),read(0xeaac28),word(home+28),word(home+30),word(home+46),byte(home+135),byte(a+101),byte(a+12),word(a+58),byte(a+114),word(0xd3b4e4),word(0xd3b4e0),read(0xbcb9a0)];events.append(r)
def mutate(address):
 if address!=p[49]:return
 flags=p[50]
 if flags&1:write(0xd056f8,p[2])
 a=actor(read(0xd056f8))
 for flag,offset,value,n in [(2,56,p[46],1),(4,54,p[47],2),(8,17,p[48],1),(64,64,p[53],1),(512,1,p[54],1),(1024,100,p[57],1)]:
  if flags&flag:write(a+offset,value,n)
 if flags&16:write(0xe94e00,p[51])
 if flags&32:write(0xeaac28,p[52])
 if flags&128:
  tile=read(actor(p[1])+32);write(0xaf598c+tile*4,read(0xaf598c+tile*4)^4)
 if flags&256:write(home+46,0,2)
 if flags&2048:write(0xbca2e0,p[51])
def code(cpu,a,n,user):
 global last
 if a==STOP:return
 if last is not None:edges.add((last,a))
 last=a;executed.add(a)
 if a in [0x4b1d50,0x4b3250,0x47a1c0]:
  sp=cpu.reg_read(UC_X86_REG_ESP);event(a,[read(sp+4),read(sp+8)] if a==0x47a1c0 else [read(sp+4)])
 elif a in [0x4b1d94,0x4b3290,0x47a1e7,0x47a1ea]:mutate(0x4b1d50 if a==0x4b1d94 else 0x4b3250 if a==0x4b3290 else 0x47a1c0)
 elif a in controlled:
  sp=cpu.reg_read(UC_X86_REG_ESP);v=[read(sp+4+i*4) for i in range(controlled[a])];event(a,v);ret=0
  if a==0x47fe60:write(0xd3b4e4,p[22],2);write(0xd3b4e0,p[23],2);ret=p[18]
  elif a==0x4a5da0:write(actor(read(0xd056f8))+17,p[20],1)
  elif a==0x4b4220:write(actor(v[0])+64,p[21],1)
  elif a==0x4b3ff0:write(actor(read(0xd056f8))+64,p[21],1);ret=p[19]
  mutate(a);cpu.reg_write(UC_X86_REG_EAX,ret&0xffffffff);cpu.reg_write(UC_X86_REG_EIP,read(sp));cpu.reg_write(UC_X86_REG_ESP,sp+4);return
 assert a in instructions,'Native escaped '+hex(a)
vm.hook_add(UC_HOOK_CODE,code)
def native(case):
 global p,home,last
 p=case;events.clear();last=None
 for a,n in regions:vm.mem_write(a,bytes([(0xa5^p[55])&255])*n)
 home=(0x7999d8+s16(p[9])*264)&0xffffffff;vm.mem_write(home-8,b'\xa5'*280)
 for off,value,n in [(0,p[10],1),(3,p[30],1),(4,p[33],1),(5,p[12],1),(6,p[28],2),(8,p[29],2),(18,p[31],2),(28,p[34],2),(30,p[61],2),(32,p[62],2),(46,p[11],2),(135,p[36],1)]:write(home+off,value,n)
 cap_address=(0x70ce14+s16(p[31])*88)&0xffffffff;vm.mem_write(cap_address-8,b'\xa5'*20);write(cap_address,p[32]);route_address=(0xd04e58+s16(p[41])*2)&0xffffffff
 if s16(p[41])>0:vm.mem_write(route_address-8,b'\xa5'*20);write(route_address,p[42],2)
 for i,id in enumerate(p[1:4]):
  a=actor(id);tile=p[13]+i*228;vm.mem_write(a-8,b'\xa5'*404)
  fields0=[(1,p[6],1),(4,0x12345678+i,4),(10,p[8]+i,1),(12,3,1),(14,p[59],1),(17,p[7],1),(20,p[26]+i,2),(22,p[27]+i,2),(32,tile,4),(36,123,2),(38,234,2),(54,p[5],2),(56,p[4],1),(57,0x55,1),(58,p[41],2),(64,p[21],1),(68,0x1234,2),(74,p[24]+i,2),(76,p[25]+i,2),(92,(p[9]+i)%4000,2),(100,p[35]+i,1),(101,0x55,1),(108,0x55,1),(109,0x55,1),(114,p[60],1),(384,p[58]+i,4)]
  for off,value,n in fields0:write(a+off,value,n)
  write(0x7090ec+((p[8]+i)&255)*56,p[17]+i*256);write(0xaf598c+tile*4,p[14]^(4 if i else 0));write(0xadc36c+tile,p[15],1)
 for i,a in enumerate(tags):write(a,p[45]+i*256)
 for i in range(8):vm.mem_write(0xea3fe4+i*0x49d8,b'\xa5'*12);write(0xea3fe8+i*0x49d8,p[39]+i,2);write(0xea3fea+i*0x49d8,p[40]+i,2)
 for a,value in [(0xd056f8,p[1]),(0xbca2e0,p[16]),(0x978d70,p[37]),(0xeaac28,p[38]),(0x78d7ac,p[43]),(0x78d7b0,p[44])]:write(a,value)
 write(0xd3b4e4,0x1234,2);write(0xd3b4e0,0x2345,2)
 sp=STACK+0x1e0000;write(sp,STOP)
 if p[0] in [2,3]:write(sp+4,p[1])
 elif p[0]==4:write(sp+4,s16(p[26]));write(sp+8,s16(p[27]))
 saved=[(UC_X86_REG_EBX,0x11223344),(UC_X86_REG_EBP,0x22334455),(UC_X86_REG_ESI,0x33445566),(UC_X86_REG_EDI,0x44556677)]
 for reg,value in saved:vm.reg_write(reg,value)
 vm.reg_write(UC_X86_REG_ESP,sp);vm.reg_write(UC_X86_REG_EFLAGS,2);vm.emu_start([0x489ca0,0x48a260,0x4b1d50,0x4b3250,0x47a1c0][p[0]],STOP,count=200000)
 assert vm.reg_read(UC_X86_REG_EIP)==STOP and vm.reg_read(UC_X86_REG_ESP)==sp+4,'Native stack/budget differs';assert all(vm.reg_read(reg)==value for reg,value in saved),'Native callee-saved registers differ'
 state=b''.join(bytes(vm.mem_read(a,n)) for a,n in regions)
 for i,id in enumerate(p[1:4]):
  a=actor(id);tile=p[13]+i*228
  for at,n in [(a-8,404),(0xaf598c+tile*4-8,20),(0xadc36c+tile-4,9),(0x7090ec+((p[8]+i)&255)*56-8,20)]:state+=bytes(vm.mem_read(at,n))
 state+=bytes(vm.mem_read(home-8,280))+bytes(vm.mem_read(cap_address-8,20))
 if s16(p[41])>0:state+=bytes(vm.mem_read(route_address-8,20))
 for i in range(8):state+=bytes(vm.mem_read(0xea3fe4+i*0x49d8,12))
 return {'result':vm.reg_read(UC_X86_REG_EAX)&255 if p[0]==4 else 0,'state':state.hex(),'events':events.copy()}
fields=['op','id','next_id','third_id','action','timer','frame','direction','kind','home','active','owner','home_kind','tile','terrain','nonwater','camera','speed','found','arrived','move_direction','route_flags','goal_x','goal_y','world_x','world_y','x','y','home_x','home_y','home_terrain_kind','capacity_index','capacity','boost','residents','people','no_population','disaster','player','exit_x','exit_y','route','route_owner','bound_width','bound_edge','tag','new_action','new_timer','new_direction','at','mutation','new_orientation','new_player','new_route_flags','new_frame','canary','spare','new_people','counter','status','shadow','free','house_counter','spare2']
default=[0]*64
for key,value in dict(id=1,next_id=256,third_id=511,action=8,timer=1,kind=1,home=1,active=1,owner=1,home_kind=1,tile=2300,speed=12,found=1,arrived=1,goal_x=10,goal_y=20,world_x=450,world_y=600,x=30,y=40,home_x=31,home_y=41,home_terrain_kind=2,capacity=20,people=10,exit_x=20,exit_y=30,route=1,route_owner=1,bound_width=512,bound_edge=10,tag=1000,new_action=10,new_timer=32,new_direction=6,new_player=7,new_route_flags=128,new_frame=255,new_people=255,counter=0x12345678,status=1,shadow=255,free=0x1234,house_counter=0x5678).items():default[fields.index(key)]=value
cases=[]
def add(**kw):
 case0=default.copy()
 for key,value in kw.items():case0[fields.index(key)]=value&0xffffffff
 if 'owner' not in kw:case0[11]=case0[1]
 if 'route_owner' not in kw:case0[42]=case0[1]
 if case0[1]==case0[2]:case0[2]=512
 if case0[1]==case0[3]:case0[3]=513
 assert len(set(case0[1:4]))==3
 cases.append(case0)
for op,action,terrain,camera in itertools.product([0,1],range(256),[0,4],[0,6]):add(op=op,action=action,terrain=terrain,camera=camera)
for op,action,terrain,camera,frame in itertools.product([0,1],[4,5,9,10],[0,4],[0,2,4,6],[0,1,10,11,12,254,255]):add(op=op,action=action,terrain=terrain,camera=camera,frame=frame)
for op,action,terrain,timer in itertools.product([0,1],[4,8],[0,4],[-32768,-1,0,1,2,4,5,31,32,33,54,55,56,127,128,32767]):add(op=op,action=action,terrain=terrain,timer=timer)
for active,owner,home_kind in itertools.product([0,1,2],[0,1,2,0xffff],[0,1,128]):add(active=active,owner=owner,home_kind=home_kind)
for op,found in itertools.product([0,1],[0,1,256,0xffffffff]):add(op=op,action=8,timer=0 if op==0 else 4,found=found)
for op,move in itertools.product([0,1],[0,1,7,8,9,10,128,255]):add(op=op,action=9 if op==0 else 10,move_direction=move)
for op,arrived,route_flags in itertools.product([0,1],[0,1,2,256,0xffffffff],[0,1,128]):add(op=op,action=10 if op==0 else 9,arrived=arrived,route_flags=route_flags)
for capacity,residents,people,boost in itertools.product([0,1,10,255,256,0x7fffffff,0x80000000,0xffffffff],[-1,0,1,127,32767],[0,1,10,255],[0,1]):add(action=10,capacity=capacity,residents=residents,people=people,boost=boost)
for no_population,disaster,residents in itertools.product([0,1,128],[0,1,256],[0,1]):add(action=10,no_population=no_population,disaster=disaster,residents=residents)
for op,id,home0,kind in itertools.product([0,1],[1,256,1999],[0,1,256,3999],[0,1,2,109]):add(op=op,id=id,home=home0,kind=kind,action=10)
for direction,camera in itertools.product([0,1,2,3,4,5,6,7,8,9,10,128,255],[0,2,4,6,256]):add(op=1,action=10,direction=direction,move_direction=direction,camera=camera)
for timer in [-32768,-1,0,1,126,127,128,32766,32767]:add(op=2,timer=timer)
for route,owner,id in itertools.product([-1,0,1,255,999],[0,1,256,0xffff],[1,256]):add(op=3,route=route,route_owner=owner,id=id)
for x,bound_width,bound_edge in itertools.product([-32768,-1,0,30,227,228,32767],[0,228,512,0x7fffffff,0x80000000],[-1,0,1,10,256]):add(op=4,x=x,bound_width=bound_width,bound_edge=bound_edge)
profiles=[(0,5,0x4b17d0),(0,4,0x4b1d50),(0,8,0x47fe60),(0,9,0x4a5da0),(0,9,0x4b3250),(0,9,0x4b4220),(0,10,0x4b3ff0),(0,10,0x46cea0),(0,10,0x4e90f0),(0,5,0x47a1c0),(1,5,0x4b17d0),(1,4,0x4b1d50),(1,8,0x47fe60),(1,10,0x4a5da0),(1,8,0x4b4220),(1,9,0x4b3ff0),(1,5,0x47a1c0)]
for op,action,at in profiles:
 for mutation in [1,3,5,9,17,33,65,129,257,513,1025,2049]:
  add(op=op,action=action,timer=0 if op==0 else 4,move_direction=9 if at==0x4b3250 else 8 if at==0x4b4220 else 0,terrain=4,at=at,mutation=mutation,new_orientation=7)
for op,action,new_orientation in itertools.product([0,1],[4,5],[-1,0,7,0x7fffffff,0x80000000]):add(op=op,action=action,terrain=4,at=0x47a1c0,mutation=21,new_orientation=new_orientation)
for op,terrain,nonwater in itertools.product([0,1],[0,4],[0,1,128]):add(op=op,action=4,terrain=terrain,nonwater=nonwater)
for value in [-32768,-1,0,1,32767]:add(action=9,move_direction=8,home_x=value,home_y=value,world_x=value,world_y=value)
for op,action,bound_width,bound_edge in itertools.product([0,1],[4,5],[0,228,512],[0,10]):add(op=op,action=action,terrain=4,bound_width=bound_width,bound_edge=bound_edge)
for op,action,mutation in itertools.product([0,1],[4,5],[1,5,17,513]):add(op=op,action=action,terrain=4,bound_width=228,at=0x47a1c0,mutation=mutation,new_orientation=7)
for index in [-1,0,1,255,256]:add(action=10,capacity_index=index)
for op,action,terrain,tag in itertools.product([0,1],[4,5],[0,4],[0,255,256,65536,0x12345678,0x7fffffff,0xffffffff]):add(op=op,action=action,terrain=terrain,tag=tag)
for op,speed in itertools.product([0,1],[0,1,256,0x12345678,0x80000000]):add(op=op,action=9 if op==0 else 10,speed=speed)
cases=list(dict.fromkeys(tuple(c) for c in cases))
if args.debug_start:cases=cases[args.debug_start:]
if args.debug_limit:cases=cases[:args.debug_limit]
smoke=subprocess.run([str(exe)],cwd=ROOT,capture_output=True,text=True,timeout=30);assert smoke.returncode==0,smoke.stdout+smoke.stderr
process=subprocess.Popen([str(exe),'--oracle-batch'],cwd=ROOT,stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE)
report={'format':1,'generated_at':datetime.now(timezone.utc).isoformat(),'passed':False,'checked':0,'stage':'production' if args.production else phase.as_posix(),'native_sha256':digest,'compiled_C_path':exe.relative_to(ROOT).as_posix(),'compiled_C_sha256':sha(exe.read_bytes()),'original_game_process_executed':False,'computer_UI_automation_used':False,'original_machine_code_fallback_used':False,'method':'Complete407+313-instruction immigrant/emigrant callbacks versus handwritten C. Actual corrected14-instruction timer and17-instruction route release plus accepted17-instruction one-input boundary predicate compose. Watched complete388-byte actor records/canaries,264-byte house records, WORD route ownership and player exit records; ordered endpoint arguments/intermediate state, consumed full EAX/exact1/AL return contracts, current-actor/player/heading reloads and original stack/saved registers compare.','limitations':['Seven movement/path/building/population algorithms are named controlled deep endpoints. Their ordered selection, arguments, state effects at the callback boundary and consumed returns are proved; their full algorithms and actual city movement/graphics are not. No RNG is called directly in these callback bodies.','Valid storage uses actor IDs1..1999, buildings0..3999, byte type indices0..111, positive route IDs1..999, players0..7, and safe mapped table addresses. Explicit signed/width/wrap fixtures additionally retain native behavior without introducing success guards for invalid native reads.','Both callbacks pass x,y to47A1C0, whose full17-instruction body reads only x at stack+4. The production adapter discards y explicitly and calls the unchanged accepted one-input C leaf. Only AL is compared/consumed; the native disabled-boundary path leaves unused high EAX scratch bits unspecified.','Boundary mutations exercise required actor/player/state/direction reloads and the cached original terrain classification. They do not assert that every current deep endpoint performs those writes. Sprite IDs and state are verified; framebuffer pixels and actual actor-registry routing remain coordinator integration work.']}
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
report['native_byte_audit']=audit;report['newly_restored_native_instructions']=751;report['unchanged_composed_native_instructions']=17;report['native_bytes_verified']=sum(v['bytes'] for v in audit.values());report['native_function_instruction_coverage']={a:{'executed':len(v&executed),'total':len(v),'unreached':[hex(i) for i in sorted(v-executed)]} for a,v in function_addresses.items()};report['controlled_deep_contracts']=contract_audit
branch_coverage={}
for a,ins in instructions.items():
 decoded=next(iter(Cs(CS_ARCH_X86,CS_MODE_32).disasm(bytes.fromhex(ins['bytes']),a)),None)
 if decoded and decoded.mnemonic.startswith('j') and decoded.mnemonic!='jmp':branch_coverage[hex(a)]={'instruction':ins['instruction'],'taken':(a,int(decoded.op_str,16)) in edges,'fallthrough':(a,a+len(decoded.bytes)) in edges}
report['native_conditional_branch_coverage']=branch_coverage;report['smoke']={'passed':True,'output':smoke.stdout.strip()}
if report.get('complete_case_set'):
 missing_instructions={a:v['unreached'] for a,v in report['native_function_instruction_coverage'].items() if v['unreached']};missing_edges=[(a,outcome) for a,v in branch_coverage.items() for outcome in ['taken','fallthrough'] if not v[outcome]];report['unqualified_missing_edges']=missing_edges
 if missing_instructions or missing_edges:report.update(passed=False,error='Incomplete native coverage '+str([missing_instructions,missing_edges]))
sources=[(phase/'include/actor_callbacks_human.h').as_posix()]+[(phase/f'runtime/{f}.c').as_posix() for f in ['actor_callbacks_human','actor_callbacks_human_boundary']]+[(phase/f'src/unit_{n:05d}.c').as_posix() for n in [1416,1427]]+['src/unit_01097.c','tests/actor_human.c','tests/actor_human_helpers.c','tools/verify_actor_human_emulation.py']
report['source_hashes']={s:sha((ROOT/s).read_bytes()) for s in sources};report['native_exports']={s:sha((ROOT/s).read_bytes()) for s in exports};name='actor-human'+('' if args.production else '-staged')+('-debug' if args.debug_limit or args.debug_start else '')+'-emulation.json';(ROOT/'metadata'/name).write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report,indent=2));sys.exit(0 if report['passed'] else 1)
