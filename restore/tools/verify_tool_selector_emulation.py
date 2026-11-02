"""Complete native tool selection/row-count/release protocol against handwritten C."""
from pathlib import Path
from datetime import datetime,timezone
import argparse,hashlib,itertools,json,struct,subprocess,sys
ROOT=Path(__file__).resolve().parents[1];sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
parser=argparse.ArgumentParser();parser.add_argument('--production',action='store_true');parser.add_argument('--exe',type=Path,default=ROOT/'build/agent-tool-selector/out/Debug/restore_tool_selector_verify.exe');parser.add_argument('--debug-limit',type=int);parser.add_argument('--debug-start',type=int,default=0);args=parser.parse_args();exe=args.exe.resolve();phase=Path('.') if args.production else Path('artifacts/tool_selector_phase')
sha=lambda b:hashlib.sha256(b).hexdigest();image=(ROOT/'build/smoke-game/Pharaoh.exe').read_bytes();digest=sha(image);assert digest=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
u16=lambda b,p:struct.unpack_from('<H',b,p)[0];u32=lambda b,p:struct.unpack_from('<I',b,p)[0];s32=lambda x:struct.unpack('<i',struct.pack('<I',x&0xffffffff))[0];s16=lambda x:struct.unpack('<h',struct.pack('<H',x&65535))[0]
pe=u32(image,60);opt=pe+24;table=opt+u16(image,pe+20);base=u32(image,opt+28);size=u32(image,opt+56);vm=Uc(UC_ARCH_X86,UC_MODE_32);vm.mem_map(base,(size+4095)&~4095)
for i in range(u16(image,pe+6)):
 q=table+i*40;vm.mem_write(base+u32(image,q+12),image[u32(image,q+20):u32(image,q+20)+u32(image,q+16)])
STOP=0x33000000;STACK=0x34000000;vm.mem_map(STOP,4096);vm.mem_map(STACK,0x200000)
def write(a,v,n=4):vm.mem_write(a&0xffffffff,(v&((1<<(n*8))-1)).to_bytes(n,'little'))
def read(a):return int.from_bytes(vm.mem_read(a&0xffffffff,4),'little')
def word(a):return int.from_bytes(vm.mem_read(a&0xffffffff,2),'little')
def byte(a):return vm.mem_read(a&0xffffffff,1)[0]
exports=['metadata/tool-selector-next-native.json','metadata/tool-selector-query-native.json'];dossier=json.loads((ROOT/exports[0]).read_text());assert dossier['native_sha256']==digest;instructions={};function_addresses={};audit={};executed=set();edges=set();last=None
for entry,f in dossier['functions'].items():
 a=f['assembly'];assert not a['has_more'] and len(a['items'])==a['total'];function_addresses[entry]=set()
 for x in a['items']:
  address=int(x['address'],16);raw=bytes.fromhex(x['bytes']);assert bytes(vm.mem_read(address,len(raw)))==raw;instructions[address]=x;function_addresses[entry].add(address)
 audit[entry]={'instructions':len(a['items']),'bytes':sum(len(bytes.fromhex(x['bytes'])) for x in a['items'])}
query_evidence={}
for entry,f in json.loads((ROOT/exports[1]).read_text())['functions'].items():
 a=f['assembly'];assert not a['has_more'] and len(a['items'])==a['total']
 for x in a['items']:assert bytes(vm.mem_read(int(x['address'],16),len(bytes.fromhex(x['bytes']))))==bytes.fromhex(x['bytes'])
 query_evidence[entry]={'instructions':len(a['items']),'execution':'controlled deep availability query; full original body authenticated for call/return contract'}
regions=[(0x6059b0,192),(0x7056dc,32),(0xbc9708,32),(0xbc97f8,16),(0xbca2bc,96),(0xd3b190,20),(0xe38dc4,32),(0x784b8c,20),(0x5e6968,20),(0x57613c,20),(0xeaac20,24),(0xe92e04,20)];p=[];events=[];loader_calls=0
def event(a,v):
 r=[a,len(v)]+[x&0xffffffff for x in v]+[0]*(20-2-len(v));r[6:18]=[read(0x7056ec),read(0x6059e8),read(0x605a48),read(0xbca310),read(0xeaac28),word(0xea5baa+read(0xeaac28)*0x49d8),read(0xbc9710),byte(0xe38dda),read(0x7056f0),read(0x6059cc),read(0x6059b4),read(0xbc9714)];events.append(r)
controlled={0x403ef0:0,0x4f5470:1,0x4f5020:0,0x4f4ff0:0,0x4c52d0:2,0x53dd90:2}
def code(cpu,a,n,user):
 global last,loader_calls
 if a==STOP:return
 if last is not None:edges.add((last,a))
 last=a;executed.add(a)
 if a in [0x404120,0x486060,0x53d850]:
  sp=cpu.reg_read(UC_X86_REG_ESP);event(a,[read(sp+4)] if a==0x486060 else [])
 elif a==0x48607d and p[26]&1:write(0xbca310,2);write(0x7056ec,p[27])
 elif a in controlled:
  sp=cpu.reg_read(UC_X86_REG_ESP);v=[read(sp+4+i*4) for i in range(controlled[a])];shown=v.copy();ret=0;cleanup=0
  if a==0x53dd90:assert cpu.reg_read(UC_X86_REG_ECX)==0xf55bb0;shown=[0xf55bb0]+v;cleanup=8
  event(a,shown)
  if a==0x403ef0:
   if p[26]&2:write(0x7056ec,p[27])
   if p[26]&4:write(0xeaac28,p[28])
   ret=p[17]|(p[16]&1)
  elif a==0x4f5470:ret=p[17]|((p[16]>>(1 if v[0]==0xb3 else 2 if v[0]==0xb2 else 3 if v[0]==0xb1 else 4))&1)
  elif a==0x4f5020:ret=p[18]
  elif a==0x4f4ff0:ret=p[19]
  elif a==0x4c52d0:
   ret=p[17]|((p[16]>>(5 if v[1]==1 else 6 if v[1]==2 else 7))&1)
   if p[26]&32 and v[1]==1:write(0xeaac28,p[28])
  elif a==0x53dd90:
   if not loader_calls:
    if p[26]&8:write(0xeaac28,p[28])
    if p[26]&16:write(0x6059e8,p[29])
   loader_calls+=1;ret=1
  cpu.reg_write(UC_X86_REG_EAX,ret&0xffffffff);cpu.reg_write(UC_X86_REG_EIP,read(sp));cpu.reg_write(UC_X86_REG_ESP,sp+4+cleanup);return
 assert a in instructions,'Native escaped '+hex(a)
vm.hook_add(UC_HOOK_CODE,code)
def fill_rows(category):
 for i in range(31):write(0x5ce3b8+(category*30+i)*4,p[30] if i and i<=p[14] else 0);write(0x70a8dc+(category*30+i)*4,p[15])
def native(case):
 global p,loader_calls,last
 p=case;events.clear();loader_calls=0;last=None
 for a,n in regions:vm.mem_write(a,bytes([(0xa5^p[31])&255])*n)
 for i in range(38):fill_rows(i)
 fill_rows(p[1]);fill_rows(p[27]);actor=(0x7999d8+p[3]*264)&0xffffffff;vm.mem_write(actor-8,b'\xa5'*280);record=(0x5d19c8+s16(p[12])*16)&0xffffffff;vm.mem_write(record-8,b'\xa5'*32)
 for off,value in [(0,p[9]),(2,p[10]),(4,p[11])]:write(record+off,value,2)
 write(0x784dde+p[2]*2,p[12],2);entry=(0x5ce3b8+(p[1]*30+p[2])*4)&0xffffffff
 if p[0]==0 and p[1]!=0x1c:write(entry,p[13])
 for i in range(31):write(0x5cf63c+i*8,p[24]+i);write(0x5cf640+i*8,p[25]+i);write(0x784de0+i*2,p[21],2)
 write(0x784dde+p[2]*2,p[12],2)
 for i in range(8):
  off=i*0x49d8
  for a,n in [(0xea5ba6,12),(0xea5ce4,12),(0xea5f98,144)]:vm.mem_write(a+off,b'\xa5'*n)
  write(0xea5baa+off,p[7] if i==p[6] else 1,2);write(0xea5ce8+off,p[8],2);write(0xea5f9c+off,p[20]+i)
  for j in range(30):write(0xea5fa0+off+j*4,p[22])
 for a,v in [(0x7056ec,p[1]),(0xd3b198,p[2]),(0xbc9800,p[3]),(0xbca310,p[4]),(0x6059e8,p[5]),(0xeaac28,p[6]),(0xe92e0c,0x12345678),(0xbca2e0,p[23])]:write(a,v)
 sp=STACK+0x1e0000;write(sp,STOP)
 if p[0]==2:write(sp+4,p[3])
 saved=[(UC_X86_REG_EBX,0x11223344),(UC_X86_REG_EBP,0x22334455),(UC_X86_REG_ESI,0x33445566),(UC_X86_REG_EDI,0x44556677)]
 for r,v in saved:vm.reg_write(r,v)
 vm.reg_write(UC_X86_REG_ESP,sp);vm.reg_write(UC_X86_REG_EFLAGS,2);vm.emu_start([0x405200,0x404120,0x486060,0x53d850][p[0]],STOP,count=200000)
 assert vm.reg_read(UC_X86_REG_EIP)==STOP and vm.reg_read(UC_X86_REG_ESP)==sp+4,'Native stack/budget differs';assert all(vm.reg_read(r)==v for r,v in saved),'Native saved registers differ'
 state=b''.join(bytes(vm.mem_read(a,n)) for a,n in regions)+bytes(vm.mem_read(actor-8,280))+bytes(vm.mem_read(record-8,32))+bytes(vm.mem_read(entry,4))+bytes(vm.mem_read((0x784dde+p[2]*2)&0xffffffff,2))
 for i in range(8):
  for a,n in [(0xea5ba6,12),(0xea5ce4,12),(0xea5f98,144)]:state+=bytes(vm.mem_read(a+i*0x49d8,n))
 return {'result':vm.reg_read(UC_X86_REG_EAX) if p[0]==1 else 0,'state':state.hex(),'events':events.copy()}
fields=['op','category','selection','actor','state','prior_mode','player','saved_word','temple_word','record_kind','subtype','record_value','record_index','table_mode','rows','allowed','queries','query_high','have','need','building','monument_word','built','rotation','width','height','mutation','new_category','new_player','new_mode','row_kind','canary']
default=[0]*32
for k,v in dict(category=1,selection=1,actor=1,prior_mode=0x12345678,saved_word=0x1234,record_kind=1,record_index=1,table_mode=1,rows=3,allowed=256,building=10,monument_word=256,width=0x12345678,height=0x87654321,row_kind=1,new_player=1,new_mode=0x6a).items():default[fields.index(k)]=v
cases=[]
def add(**kw):
 p0=default.copy()
 for k,v in kw.items():p0[fields.index(k)]=v&0xffffffff
 cases.append(p0)
for category,selection in itertools.product(range(38),range(1,31)):add(category=category,selection=selection)
for category,selection in itertools.product([0xe,0x25,0x1b,0xf,0x12,0x13,0x1a,0x11,0x10],[0,1,2,3,4,8,9,12,13,16,17,256,65536,0x7fffffff,0x80000000,0xffffffff]):
 if category in [0xe,0x25,0x1b,0xf,0x12,0x13,0x1a,0x11,0x10] and selection>65536 and category not in [0xe,0x25,0x1a,0x11]:continue
 add(category=category,selection=selection)
for mode in range(0xed):add(table_mode=mode)
for state,actor in itertools.product([0,1,2,256,257,258,0xffffffff],[0,1,255,256]):add(state=state,actor=actor,table_mode=0x6a)
for op,actor in itertools.product([2],[0,1,255,256]):add(op=op,actor=actor)
for kind,subtype,value in itertools.product([0,1,256,257],[0,1,2,3,4,5,6,7,10,11,12,13,14,256,0xffff],[-32768,-1,0,1,2,256,32767]):add(category=0x1c,record_kind=kind,subtype=subtype,record_value=value)
for index in [-1,0,1,255,256]:add(category=0x1c,record_index=index,record_kind=1,record_value=-1)
for mode in range(0xed):add(category=0x1c,record_kind=0,subtype=0,prior_mode=mode)
for temple in [0,1,256,0x8000,0xffff]:add(table_mode=0x61,temple_word=temple)
for player,saved in itertools.product([0,1,7],[0,1,256,0x1234,0x8000,0xffff]):add(category=0x1c,record_kind=0,subtype=1,player=player,saved_word=saved)
for category,rows,allowed,query,high in itertools.product([1,6,10,22,28,36,256],[0,1,2,29,30],[0,1,256],[0,1,31,255],[0,256]):add(op=1,category=category,selection=0,rows=rows,allowed=allowed,queries=query,query_high=high,row_kind=0x30 if category==6 else 4 if category==10 else 1)
for mode,query,temple in itertools.product([0x30,0x61,1],[0,1,33,65,97,255],[0,256]):add(op=1,category=6,row_kind=mode,selection=0,queries=query,temple_word=temple)
for have,need in itertools.product([-1,0,1,256,0x7fffffff,0x80000000],[-1,0,1,256,0x7fffffff,0x80000000]):add(op=1,category=6,row_kind=0x30,queries=1,have=have,need=need)
for query in [1,3,5,9,17,255]:add(op=1,category=10,row_kind=4,queries=query)
for word0,built,query in itertools.product([0,1,256,0x8000],[0,1,256],[0,1,256,257]):add(op=1,category=28,selection=0,row_kind=1,monument_word=word0,built=built,queries=query&1,query_high=query&256)
for rotation,mode in itertools.product([0,2,4,6,256,0x12345678],range(0x41,0x46)):add(table_mode=mode,rotation=rotation)
for category in [0,1,256,0xffffffff]:add(category=category,table_mode=0x6a)
for mode in [0xc6,0xdf,0x12345678,0xffffffff]:add(category=0,table_mode=mode)
for category in [1,0xe,0x1c]:add(state=1,mutation=1,new_category=category,record_kind=0,subtype=1)
for category in [6,10,22,28]:
 for new_category in [1,6,22,28]:add(op=1,category=category,selection=0,row_kind=0x30 if category==6 else 4 if category==10 else 1,queries=255,mutation=2,new_category=new_category)
for category in [6,22,28,36]:add(op=1,category=category,selection=0,row_kind=0x61 if category==6 else 1,queries=255,temple_word=256,mutation=4,new_player=7)
for mutation,new_mode in itertools.product([8,16,24],[0x6a,2,0x8c]):add(category=28,record_kind=0,subtype=1,mutation=mutation,new_player=7,new_mode=new_mode,saved_word=0x1234)
add(op=1,category=6,selection=0,row_kind=0x61,temple_word=256,queries=33,mutation=32,new_player=7)
for mode in [0x6a,0xdd,0x6b,0xd8,0xa1,0xd9,0xa2]:add(table_mode=mode,canary=0x5a)
for variant in [0,1,2]:add(op=3,saved_word=variant)
for category,allowed in itertools.product([6,10],[0,256]):add(op=1,category=category,selection=0,row_kind=1,allowed=allowed)
cases=list(dict.fromkeys(tuple(c) for c in cases))
if args.debug_start:cases=cases[args.debug_start:]
if args.debug_limit:cases=cases[:args.debug_limit]
smoke=subprocess.run([str(exe)],cwd=ROOT,capture_output=True,text=True,timeout=30);assert smoke.returncode==0,smoke.stdout+smoke.stderr
process=subprocess.Popen([str(exe),'--oracle-batch'],cwd=ROOT,stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE)
report={'format':1,'generated_at':datetime.now(timezone.utc).isoformat(),'passed':False,'checked':0,'stage':'production' if args.production else phase.as_posix(),'native_sha256':digest,'compiled_C_path':exe.relative_to(ROOT).as_posix(),'compiled_C_sha256':sha(exe.read_bytes()),'original_game_process_executed':False,'computer_UI_automation_used':False,'original_machine_code_fallback_used':False,'method':'All508 selector instructions, actual124-instruction row-count controller and7-instruction actor-release leaf, plus unchanged32-instruction asset submission controller, executed as pinned x86 and compared with handwritten C. Ordered endpoint args/state, full DWORD rule tokens/count/dimensions, signed WORD records and player save/restore, actor BYTE/264 stride, adjacent canaries, consumed AL/full-EAX returns, native stack and callee-saved registers compared.','limitations':['403EF0/4F5020/4F4FF0/4F5470/4C52D0 availability queries and53DD90 sprite-library loading remain named controlled deep endpoints. Their selection/ordered arguments and consumed return contracts are tested; their full gameplay/file-loading algorithms are not established.','The unchanged2508 asset controller is composed for native player indices0..7 and valid sphinx variants0..2. Its legacy player scalar is BYTE, which equals DWORD over this explicit player domain. No invalid native filename-pointer read is converted into success.','Record/selector/table fixtures use mapped native storage; ordinary categories/rows cover0..37 and0..30, with safe mapped full-DWORD category/wrap probes. Subtype1 forces temporary WORD variant1, reloads the player after the loader, and restores the saved WORD to that current player exactly as native.','Boundary mutation fixtures test category/player/mode reloads across callbacks. They do not assert that each current deep endpoint performs those particular writes. No actual sidebar click, placement execution or framebuffer result is claimed.']}
case=None
try:
 assert json.loads(process.stdout.readline())=={'fields':32}
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
report['native_byte_audit']=audit;report['newly_restored_native_instructions']=639;report['unchanged_composed_native_instructions']=32;report['native_bytes_verified']=sum(v['bytes'] for v in audit.values());report['native_function_instruction_coverage']={a:{'executed':len(v&executed),'total':len(v),'unreached':[hex(i) for i in sorted(v-executed)]} for a,v in function_addresses.items()};report['controlled_query_contracts']=query_evidence
branch_coverage={}
for a,x in instructions.items():
 ins=next(iter(Cs(CS_ARCH_X86,CS_MODE_32).disasm(bytes.fromhex(x['bytes']),a)),None)
 if ins and ins.mnemonic.startswith('j') and ins.mnemonic!='jmp':branch_coverage[hex(a)]={'instruction':x['instruction'],'taken':(a,int(ins.op_str,16)) in edges,'fallthrough':(a,a+len(ins.bytes)) in edges}
report['native_conditional_branch_coverage']=branch_coverage;report['smoke']={'passed':True,'output':smoke.stdout.strip()}
dispatch_coverage={}
for at,table0,n in [(0x4052db,0x405b24,4),(0x40415c,0x4042f0,6)]:
 raw=bytes(vm.mem_read(table0,n*4));destinations=set(struct.unpack('<'+'I'*n,raw));dispatch_coverage[hex(at)]={'native_table':hex(table0),'table_sha256':sha(raw),'targets':{hex(t):(at,t) in edges for t in sorted(destinations)}}
report['native_indirect_dispatch_coverage']=dispatch_coverage
if report.get('complete_case_set'):
 missing_instructions={a:v['unreached'] for a,v in report['native_function_instruction_coverage'].items() if v['unreached']};missing_branches=[(a,k) for a,v in branch_coverage.items() for k in ['taken','fallthrough'] if not v[k]];missing_dispatch=[(a,t) for a,v in dispatch_coverage.items() for t,hit in v['targets'].items() if not hit]
 report['unqualified_missing_edges']=missing_branches+missing_dispatch
 if missing_instructions or missing_branches or missing_dispatch:report.update(passed=False,error='Incomplete native coverage '+str([missing_instructions,missing_branches,missing_dispatch]))
sources=[(phase/'include/tool_selector.h').as_posix()]+[(phase/f'src/unit_{n:05d}.c').as_posix() for n in [37,42,1248]]+['src/unit_02508.c','include/sprite_libraries.h','tests/tool_selector.c','tests/tool_selector_helpers.c','tools/verify_tool_selector_emulation.py']
report['source_hashes']={s:sha((ROOT/s).read_bytes()) for s in sources};report['native_exports']={s:sha((ROOT/s).read_bytes()) for s in exports};name='tool-selector'+('' if args.production else '-staged')+('-debug' if args.debug_limit or args.debug_start else '')+'-emulation.json';(ROOT/'metadata'/name).write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report,indent=2));sys.exit(0 if report['passed'] else 1)
