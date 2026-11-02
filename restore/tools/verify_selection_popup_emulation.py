"""Whole popup builder/callback/renderer with actual input and hover bodies."""
from pathlib import Path
from datetime import datetime,timezone
import argparse,hashlib,itertools,json,random,struct,subprocess,sys
ROOT=Path(__file__).resolve().parents[3] if 'selection_popup_phase' in str(Path(__file__)) else Path(__file__).resolve().parents[1]
PHASE=ROOT/'artifacts/selection_popup_phase';sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE,UC_HOOK_MEM_WRITE
from unicorn.x86_const import *
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
ap=argparse.ArgumentParser();ap.add_argument('--production',action='store_true');ap.add_argument('--exe',type=Path,default=ROOT/'build/agent-selection-popup/Debug/restore_selection_popup_verify.exe');ap.add_argument('--optimized-exe',type=Path,default=ROOT/'build/agent-selection-popup/Release/restore_selection_popup_verify.exe');ap.add_argument('--debug-limit',type=int);args=ap.parse_args()
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
image=(ROOT/'build/smoke-game/Pharaoh.exe').read_bytes();digest=hashlib.sha256(image).hexdigest();assert digest=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
u16=lambda p:struct.unpack_from('<H',image,p)[0];u32=lambda p:struct.unpack_from('<I',image,p)[0]
pe=u32(60);opt=pe+24;base=u32(opt+28);table=opt+u16(pe+20);vm=Uc(UC_ARCH_X86,UC_MODE_32);vm.mem_map(base,(u32(opt+56)+4095)&~4095)
for i in range(u16(pe+6)):
 q=table+i*40;vm.mem_write(base+u32(q+12),image[u32(q+20):u32(q+20)+u32(q+16)])
STOP=0x33000000;STACK=0x34000000;vm.mem_map(STOP,4096);vm.mem_map(STACK,0x100000)
def put(a,v,n=4):vm.mem_write(a,(v&((1<<(n*8))-1)).to_bytes(n,'little'))
def get(a,n=4):return int.from_bytes(vm.mem_read(a,n),'little')
def s(v):return (v&0x7fffffff)-(v&0x80000000)
regions=[(0x6057d0,0x2a0),(0x704da0,0x8d0),(0x7056a0,64),(0x5c5e60,64),(0x775900,64),(0x77f740,0x180),(0x6f2000,0x100),(0x784ee0,64),(0x5d2a20,0x840),(0xe38da0,0x130),(0xd3b180,0x280),(0xe945e0,0x1100),(0xf56f70,64)]
original_button=bytes(vm.mem_read(0x5c5e78,28));assert original_button.hex()=='00000000c8001e00b092400080f75100000000030100000000000000'
native_file=(ROOT/'metadata/selection-popup-native.json') if args.production else PHASE/'metadata/native.json';d=json.loads(native_file.read_text());assert d['native_sha256']==digest
core={};sets={};branches={};cs=Cs(CS_ARCH_X86,CS_MODE_32)
for entry,f in d['functions'].items():
 a=f['assembly'];assert not a['has_more'] and len(a['items'])==a['total'];sets[int(entry,16)]=set()
 for x in a['items']:
  address=int(x['address'],16);b=bytes.fromhex(x['bytes']);assert bytes(vm.mem_read(address,len(b)))==b;core[address]=x;sets[int(entry,16)].add(address)
  ins=next(cs.disasm(b,address,count=1))
  if ins.mnemonic.startswith('j') and ins.mnemonic!='jmp':branches[address]={address+ins.size,int(ins.op_str,16)}
assert get(0x51f780,1)==0xc3;core[0x51f780]={'address':'0051f780','bytes':'c3','instruction':'RET'}
endpoints={0x443710:0,0x442870:1,0x5344c0:1,0x4de430:1,0x444b10:0,0x4bb090:0,0x536820:4,0x536c30:6,0x4d63d0:8,0x4d6250:9,0x4d6380:9,STOP+0x100:0}
executed=set();edges=set();events=[];p=[];predicates=tiles=texts=0;previous=None
def event(a,argv):
 byte_target=get(0x605930);dword_target=get(0x605938)
 events.append([a,len(argv)]+[v&0xffffffff for v in argv]+[0]*(9-len(argv))+[get(0xe38dda,1),get(0x605a58),get(0xd3b198),get(byte_target,1) if byte_target else 0,get(dword_target) if dword_target else 0,get(0x605a54),get(0x605a50),get(0x605a3c),get(0x6057f4+get(0xd3b198)*4)])
def code(cpu,a,n,user):
 global predicates,tiles,texts,previous
 if previous in branches:edges.add((previous,a))
 previous=a
 if a==STOP:cpu.emu_stop();return
 if a not in endpoints:
  assert a in core,f'native escaped {a:08x}';executed.add(a);return
 count=endpoints[a];sp=cpu.reg_read(UC_X86_REG_ESP);argv=[get(sp+4+i*4) for i in range(count)];event(a,argv);result=0x12345678
 if a==0x443710:
  if p[29]&1:put(0xd3b198,p[30])
  if p[29]&256:put(0x605930,0x6f2040)
  if p[29]&512:put(0x6057f4+get(0xd3b198)*4,p[39])
  result=p[26]
 elif a==0x442870:
  result=p[28] if predicates else p[27];predicates+=1
  if predicates==1 and p[29]&2:put(0xd3b198,p[31])
  if predicates==2 and p[29]&4:put(0xd3b198,p[32])
 elif a==0x4de430:
  if p[29]&16:put(0x605a3c,p[35])
 elif a==0x536820:
  if tiles==0:
   if p[29]&32:put(0x605948,p[36]);put(0xe38da8,p[50]);put(0xe38da4,p[51])
   if p[29]&128:
    for address,v in [(0x704dc8,-300),(0x704dca,-32768),(0x704dcc,123),(0x704dce,32767)]:put(address,v,2)
  tiles+=1
 elif a==0x4d6380:
  if texts==0 and p[29]&64:put(0x605948,p[37]);put(0x605944,p[38]);put(0xe38da8,p[50]);put(0xe38da4,p[51])
  texts+=1
 elif a==STOP+0x100:
  if p[29]&8:put(0x605a50,p[33]);put(0xe38dda,p[34],1)
 cpu.reg_write(UC_X86_REG_EAX,result);cpu.reg_write(UC_X86_REG_ECX,0xdeadc0de);cpu.reg_write(UC_X86_REG_EDX,0xfefefefe);cpu.reg_write(UC_X86_REG_ESP,sp+4);cpu.reg_write(UC_X86_REG_EIP,get(sp))
def write(cpu,access,a,n,value,user):assert STACK<=a and a+n<=STACK+0x100000 or any(lo<=a and a+n<=lo+size for lo,size in regions),f'unwatched native write {a:08x}/{n}'
vm.hook_add(UC_HOOK_CODE,code);vm.hook_add(UC_HOOK_MEM_WRITE,write)
def setup(case):
 global p,events,predicates,tiles,texts
 p=case;events=[];predicates=tiles=texts=0;b=p[6]*98
 for i,(a,n) in enumerate(regions):vm.mem_write(a,bytes((p[1]+j*37+i*11)&255 for j in range(n)))
 vm.mem_write(0x5c5e78,original_button)
 for i in range(78):
  put(0x6057f4+i*4,p[24]+i*p[25]);a=0x704dc8+i*28
  for off,v in [(0,i//26*200+5),(2,i%26*20+14),(4,i//26*200+201),(6,i%26*20+32)]:put(a+off,v,2)
  for off,v in [(8,0x403950),(12,0x51f780),(20,i),(24,0)]:put(a+off,v)
  put(a+19,3,1)
 vals={0x6057f4+p[7]*4:p[23],0x77591c:p[6],0xd3b198:p[7],0x605a58:p[4],0x605a54:p[5],0x605930:0x77f766+b+p[40] if p[40]<14 else 0x6f2010,0x605934:0x6f2020,0x605938:0x6f2030,0x60592c:p[17],0x60593c:p[16],0x605940:p[12],0x605950:p[21],0x605a50:p[56],0x605958:p[57],0x605a3c:p[42],0x605a44:p[6],0x605a18:p[43],0x605944:p[11],0x605948:p[10],0x605954:p[8],0x60594c:p[9],0x6057ec:p[59],0x7056c8:p[52],0xf56f8c:p[53],0xe38da8:p[54],0xe38da4:p[55],0xe94d98:p[47],0xe38e34:p[61],0xe38e30:p[62]}
 for a,v in vals.items():put(a,v)
 for a,v in [(0xe38dda,p[2]),(0xe38dca,p[3]),(0x6057f0,p[22]),(0x77f760+b,p[41]),(0x77f761+b,p[58])]:put(a,v,1)
 for i in range(256):put(0xe94624+i*16,p[46]);put(0xe9461c+i*16,0)
 for a,v in [(0x6f2080,1),(0x6f2088,0x12348001),(0x6f208c,0xabcd7fff),(0x6f2090,0x3434ffff),(0x6f2094,0x21210003),(0x6f2098,0x6f20a0 if p[60]==2 else 0),(0x6f20a0,2),(0x6f20a8,0x1ffff),(0x6f20ac,0x10001),(0x6f20b0,0x8001),(0x6f20b4,0xffff),(0x6f20b8,0)]:put(a,v)
 if p[60]:put(0xe9461c+(p[2]&255)*16,0x6f2080)
 for a in range(0xe38df4,0xe38e05):put(a,0,1)
def call(entry,argv=(),local=False):
 global previous
 previous=None;sp=STACK+0x80000;saved={UC_X86_REG_EBX:p[1]^0x12345678,UC_X86_REG_ESI:p[1]^0x789abcde,UC_X86_REG_EDI:p[1]^0xabcdef12,UC_X86_REG_EBP:p[1]^0x2468ace0}
 if local:
  for i,r in enumerate([UC_X86_REG_EDI,UC_X86_REG_ESI,UC_X86_REG_EBP,UC_X86_REG_EBX]):put(sp+i*4,saved[r])
  put(sp+0x8c,STOP);expected_sp=sp+0x90
 else:
  put(sp,STOP)
  for i,v in enumerate(argv):put(sp+4+i*4,v)
  expected_sp=sp+4
 for r,v in saved.items():vm.reg_write(r,v)
 vm.reg_write(UC_X86_REG_EAX,p[1]);vm.reg_write(UC_X86_REG_ECX,p[1]^0x44556677);vm.reg_write(UC_X86_REG_EDX,p[1]^0x99aabbcc);vm.reg_write(UC_X86_REG_EBP,0 if local else saved[UC_X86_REG_EBP]);vm.reg_write(UC_X86_REG_EFLAGS,0x202);vm.reg_write(UC_X86_REG_ESP,sp)
 vm.emu_start(entry,STOP,count=100000);assert vm.reg_read(UC_X86_REG_EIP)==STOP and vm.reg_read(UC_X86_REG_ESP)==expected_sp
 for r,v in saved.items():assert vm.reg_read(r)==v,(hex(entry),r)
 return vm.reg_read(UC_X86_REG_EAX)
def native(case):
 setup(case);op=p[0];result=0
 if op==0:call(0x403c00,p[8:23])
 elif op in [1,2,3,4]:call({1:0x409540,2:0x4092b0,3:0x409300,4:0x403950}[op])
 elif op in [5,8]:
  call(0x403c00,p[8:23]) if op==5 else call(0x409540);a=0x704dc8+p[45]*28
  put(0xe38e34,get(0x605954)+s16(get(a,2))+1);put(0xe38e30,get(0x60594c)+s16(get(a+2,2))+1)
  if p[44] in [1,2,3]:put({1:0xe38e04,2:0xe38df8,3:0xe38df7}[p[44]],1,1)
  if p[44]==4:put(0xe38e34,0x80000000);put(0xe38e30,0x80000000)
  if p[44]==5:put(0xe38e34,get(0x605954)+s16(get(a+4,2)))
  if p[44]==6:put(0xe38e30,get(0x60594c)+s16(get(a+6,2)))
  if p[44]==7:put(0xe38e30,get(0x60594c)+s16(get(a+2,2))-1)
  call(0x4dd826,local=True)
 elif op==6:
  if p[63]:put(0xe38e04,1,1)
  call(0x530d70,[p[8],p[9],p[48]])
 elif op==7:result=call(0x4d6740,[p[8],p[9],0x704dc8,p[10],p[45]])
 else:raise AssertionError(op)
 return {'result':result,'state':b''.join(bytes(vm.mem_read(a,n)) for a,n in regions).hex(),'events':list(events)}
def s16(v):return (v&32767)-(v&32768)
rng=random.Random(0x403c00);cases=[]
def add(op=0,**kw):
 p=[0]*64;p[0]=op;p[1]=rng.getrandbits(32)
 for i,v in {2:1,4:0,5:0,6:1,7:1,8:50,9:50,10:7,11:33,12:0,13:0x6f2010,14:0x6f2020,15:0x6f2030,16:0,17:4,18:0,19:0,20:0,21:26,22:0,23:3,24:0,25:1,26:4,27:1,28:1,30:2,31:3,32:4,33:0x12345678,34:0x99,35:1,36:5,37:6,38:23,39:2,40:3,41:0,42:0,43:2,44:0,45:1,46:1,47:0,48:26,49:0,50:7,51:9,52:2,53:0x12345678,54:0,55:0,56:0,57:1,58:3,59:1,60:0,61:0x80000000,62:0x80000000,63:0}.items():p[i]=v
 for k,v in kw.items():p[int(k[1:])]=v&0xffffffff
 cases.append(p);return p
for count,rows,preserve,mode,disable in itertools.product([-2147483648,-2,0,1,7,26,78,79,2147483647],[-1,0,1,22,26,2147483647],[0,1,256],[0,1,256],[0,1]):add(v10=count,v21=rows,v18=preserve,v20=mode,v22=disable)
for op in [1,2,3]:
 for mode in [0,1,15,72,255]:add(op,v2=mode,v57=mode^0x123400)
for kind,mode,value,disabled,index,rows in itertools.product([0,1,2,3,4,256],[15,72],[-1,0,1,2,8,9,0x12345678],[0,1],[0,1,26],[26]):add(4,v17=kind,v2=mode,v23=value,v22=disabled,v7=index,v21=rows)
for special,count,value,first,second,offset in itertools.product([0,1],[-1,3,4,5],[1,2,8],[0,0x100,1,0xabcdef01],[0,0x100,1],[0,7,10,20]):add(4,v17=1,v2=72,v41=special,v26=count,v23=value,v27=first,v28=second,v40=offset)
for mask in range(1024):
 add(4,v17=1,v2=72,v41=mask&1,v26=[3,4,5][mask%3],v23=[1,2,8][mask%3],v29=mask,v27=[0x100,1][(mask>>1)&1],v28=[0x100,1][(mask>>2)&1],v56=STOP+0x100,v39=[1,2,8][(mask>>3)%3])
for post,player,editor,mask in itertools.product([0,2,258],[-2147483648,-1,0,5,6,256],[0,1,256],[0,16,8]):add(4,v17=4,v16=post,v43=player,v42=editor,v29=mask,v56=STOP+0x100,v12=0xffffffff)
for count,rows,button,ready,hover,disable,storage in itertools.product([0,1,7,27,78],[1,26],[0,1,2,3,4],[0,1],[0,2],[0,1],[1,2,4]):
 index=min(max(count-1,0),3);add(5,v10=count,v21=rows,v44=button,v46=ready,v47=hover,v22=disable,v17=storage,v45=index,v18=1)
for ready,hit,state,node,mode,mask in itertools.product([0,1],[0,1,3],[0,2,256],[0,1],[7,15,72,255],[0,128]):add(7,v46=ready,v45=hit,v47=state,v60=node,v2=mode,v29=mask,v8=0x7fffffff,v9=0x80000000)
for count,rows,hover,disabled,group,editor in itertools.product([-1,0,1,3,7,27],[0,1,3,26],[0,1,2,3,27],[0,1],[23,33],[0,1]):
 add(6,v10=count,v48=rows,v52=hover,v22=disabled,v11=group,v42=editor,v24=19,v25=0)
for toggle,click,ready,hover in itertools.product([0,1,256],[0,1],[0,1],[0,2]):
 q=add(6,v4=1,v5=toggle,v10=7,v48=26,v46=ready,v47=hover,v63=click)
 q[61]=(q[8]+q[59]*208-218+1)&0xffffffff;q[62]=(q[9]+7*20-25+1)&0xffffffff
for mask,count,rows in itertools.product([32,64,96,128,160,192,224],[1,4,7],[1,3,26]):add(6,v10=count,v48=rows,v29=mask,v36=5,v37=6,v24=19,v25=0,v11=23,v42=1,v4=1)
for value in [-1,0,9,256]:add(4,v17=1,v2=72,v41=1,v23=value,v4=0x100)
for rows in [1,3]:add(6,v10=7,v48=rows,v29=64,v37=0)
for mode in [7,15,72,255]:add(7,v46=0,v45=1,v47=2,v60=2,v2=mode)
for row,button,editor in itertools.product(range(7),[0,1,2,3],[0,1,256]):add(8,v45=row,v44=button,v42=editor)
for button,x,y in itertools.product([5,6,7],[50,0x7ffffffe,0x80000000],[50,0x80000001]):add(5,v44=button,v8=x,v9=y)
for button,completion in itertools.product([1,2,3],[0x51f780,STOP+0x100]):add(5,v44=button,v19=completion,v29=8)
for count,rows in itertools.product([0x80000000,0x80000001,0xffffffff],[0,1,0x7fffffff]):add(6,v10=count,v48=rows,v8=0x7ffffffe,v9=0x80000001)
if args.debug_limit:cases=cases[:args.debug_limit]
expected=[native(c) for c in cases]
required=[0x403c00,0x403950,0x409540,0x4092b0,0x409300,0x530d70,0x4d6740,0x4dd826]
required_instructions=set().union(*(sets[e] for e in required))
required_edges={(a,b) for a,z in branches.items() if a in required_instructions for b in z}
complete=required_instructions<=executed and required_edges<=edges
if not args.debug_limit:
 assert complete,{'missing_instructions':[f'{a:08x}' for a in sorted(required_instructions-executed)],'missing_edges':[[f'{a:08x}',f'{b:08x}'] for a,b in sorted(required_edges-edges)]}
product=ROOT if args.production else PHASE
source_paths=[product/'src/unit_00033.c',product/'src/unit_01858.c',product/'src/unit_02421.c',product/'runtime/selection_popup.c',product/'include/selection_popup.h',ROOT/'src/unit_00416.c',ROOT/'src/unit_00417.c',ROOT/'src/unit_01836.c',ROOT/'src/unit_01857.c',ROOT/'include/menu_input.h',ROOT/'include/menu_layout.h',ROOT/'include/restore_types.h',ROOT/'include/standalone_data.h',product/'tests/selection_popup.c',PHASE/'build/CMakeLists.txt',Path(__file__).resolve()]
report={'format':1,'generated_at':datetime.now(timezone.utc).isoformat(),'passed':False,'checked':len(cases),'native_sha256':digest,'compiled_C_sha256':sha(args.exe.resolve()),'source_hashes':{x.relative_to(ROOT).as_posix():sha(x) for x in source_paths},'native_exports':{native_file.relative_to(ROOT).as_posix():sha(native_file)},'coverage':{f'{e:08x}':{'executed':len(z&executed),'total':len(z),'missing':[f'{a:08x}' for a in sorted(z-executed)]} for e,z in sets.items()},'conditional_edges':{'executed':sum((a,b) in edges for a,z in branches.items() for b in z),'total':sum(map(len,branches.values())),'missing':[[f'{a:08x}',f'{b:08x}'] for a,z in branches.items() for b in z if (a,b) not in edges]},'variants':{},'original_game_process_executed':False,'original_machine_code_fallback_used':False,'limitations':['Deeper panel/text/raster bodies536820/536C30/4D63D0/4D6250/4D6380 are explicit typed controlled endpoints; no pixels are claimed. Current accepted input416/417, layout1836/1857 and whole typed hover1858 execute actual bodies.','Gameplay validations443710/442870, message5344C0 and player/editor updates4DE430/444B10/4BB090 are controlled exact-argument endpoints. AL of442870 and signed fullEAX of443710 are consumed; mutation fixtures exercise their caller reloads. Their complete gameplay and dialogs remain separate obligations.','The local mode0F/48 input block begins4DD826 with original saved-frame layout and EBP==0; shared1868prelude is outside this proof. Native discarded extra screen-offset arguments to4argument426450 are authenticated; they have no consumed ABI semantics.','403C00 modifies its incoming count stack slot when count>78. Its callers discard all60 argument bytes; C preserves resulting global/table behavior, and stack argument storage is not exposed as an output. Return EAX from void routines is not a contract.','Mapping/destination/index and linked layout-node pointers must denote valid native data. Faulting invalid pointers, indices outside0..77, cyclic node lists and data races during endpoint-free instruction sequences are outside valid-domain equivalence. Completion test token33000100 is an explicit synthetic zeroarg endpoint, with post-call global clearing checked; production routes tokens through owned central C dispatch and diagnoses unsupported values.']}
report['qualified_controller']={'complete':complete,'entries':[f'{e:08x}' for e in required],'native_instructions':len(required_instructions),'executed':len(required_instructions&executed),'conditional_edges':len(required_edges),'edges_executed':len(required_edges&edges),'excluded_instructions':[],'excluded_edges':[]}
report['watched_memory']={'regions':regions,'total_bytes_per_case':sum(n for _,n in regions),'method':'Complete concatenated bytes, including surrounding canaries, compared for every case in both configurations. Every native write must lie in a watched region or the private stack.'}
report['composed_context']={'004261d0':'Unchanged complete C body executes actual trigger3 popup/toggle buttons. Other shared trigger1/2/4 arms remain explicitly outside this popup composition; their independent menu-input suite is not replaced.','00426450':'Unchanged whole hover engine; signedWORD hitboxes and native32bit coordinate wrap are exercised.','004d5d70':'Unchanged whole mode getter, including mode7 projection through605A44.','004d66f0':'Unchanged whole linked layout writer; empty,one-node,two-node and WORD narrowing/addition cases.','0051f780':'Authenticated native bareRET used for actual right-button/completion no-op identity.','00444b10':'Reference body retained in native dossier only; remains a controlled deeper editor endpoint, not claimed composed.'}
out=(ROOT/'metadata/selection-popup-emulation.json') if args.production else PHASE/'metadata/selection-popup-emulation.json'
try:
 for label,path in [('Debug',args.exe),('O2',args.optimized_exe)]:
  path=path.resolve();proc=subprocess.Popen([str(path),'--oracle-batch'],stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True)
  try:
   assert json.loads(proc.stdout.readline())['fields']==64
   for i,(case,want) in enumerate(zip(cases,expected)):
    proc.stdin.write(' '.join(map(str,case))+'\n');proc.stdin.flush();line=proc.stdout.readline();assert line,(label,i,proc.poll(),proc.stderr.read());actual=json.loads(line)
    if actual!=want:
     field=next(k for k in want if actual[k]!=want[k]);detail={'variant':label,'case':i,'input':case,'field':field,'C':actual[field],'native':want[field]};(PHASE/'metadata/last-mismatch.json').write_text(json.dumps(detail,indent=2));raise AssertionError({k:v for k,v in detail.items() if k not in ['C','native']})
   proc.stdin.close();proc.wait(timeout=30);assert proc.returncode==0,proc.stderr.read()
  finally:
   if proc.poll() is None:proc.kill();proc.wait()
  report['variants'][label]={'path':path.relative_to(ROOT).as_posix(),'sha256':sha(path),'checked':len(cases)}
 report['comparisons']=len(cases)*2;report['passed']=True
finally:out.write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({'passed':report['passed'],'checked':len(cases),'qualified_controller':report['qualified_controller'],'comparisons':report.get('comparisons')}))
