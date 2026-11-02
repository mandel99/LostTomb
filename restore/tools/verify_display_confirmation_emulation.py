"""Timed display confirmation against pinned x86, with actual input/layout C."""
from pathlib import Path
from datetime import datetime,timezone
import argparse,hashlib,itertools,json,random,struct,subprocess,sys
ROOT=Path(__file__).resolve().parents[3] if 'display_confirmation_phase' in str(Path(__file__)) else Path(__file__).resolve().parents[1];PHASE=ROOT/'artifacts/display_confirmation_phase'
sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE,UC_HOOK_MEM_WRITE
from unicorn.x86_const import *
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
ap=argparse.ArgumentParser();ap.add_argument('--production',action='store_true');ap.add_argument('--exe',type=Path,default=ROOT/'build/agent-display-confirmation/Debug/restore_display_confirmation_verify.exe');ap.add_argument('--optimized-exe',type=Path,default=ROOT/'build/agent-display-confirmation/Release/restore_display_confirmation_verify.exe');ap.add_argument('--debug-limit',type=int);args=ap.parse_args()
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest();image=(ROOT/'build/smoke-game/Pharaoh.exe').read_bytes();digest=hashlib.sha256(image).hexdigest();assert digest=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
u16=lambda p:struct.unpack_from('<H',image,p)[0];u32=lambda p:struct.unpack_from('<I',image,p)[0];pe=u32(60);opt=pe+24;base=u32(opt+28);sec=opt+u16(pe+20);vm=Uc(UC_ARCH_X86,UC_MODE_32);vm.mem_map(base,(u32(opt+56)+4095)&~4095)
for i in range(u16(pe+6)):
 q=sec+i*40;vm.mem_write(base+u32(q+12),image[u32(q+20):u32(q+20)+u32(q+16)])
STOP=0x33000000;STACK=0x34000000;vm.mem_map(STOP,4096);vm.mem_map(STACK,0x100000)
FRAME=0x30000000;vm.mem_map(FRAME,0x200000)
def get(a,n=4):return int.from_bytes(vm.mem_read(a,n),'little')
def put(a,v,n=4):vm.mem_write(a,(v&((1<<(8*n))-1)).to_bytes(n,'little'))
def text(a):
 out=bytearray()
 for i in range(256):
  b=get(a+i,1)
  if not b:return out.decode('ascii')
  out.append(b)
 raise AssertionError('unterminated import')
def s16(v):return (v&32767)-(v&32768)
regions=[(0x6057d0,48),(0x7056a0,0x90),(0x5c6890,64),(0xe38d90,0x400),(0xe945e0,0x1100),(0xd3b180,0x280),(0x6f2000,0x100),(0xf56f70,64),(0xe92e00,64)]
native_file=ROOT/'metadata/display-confirmation-native.json' if args.production else PHASE/'metadata/native.json';d=json.loads(native_file.read_text());assert d['native_sha256']==digest
original_button=bytes(vm.mem_read(0x5c68a8,28));assert original_button.hex()==d['button']['bytes']=='a000700040019000102e400080f75100000000010000000000000000'
core={};sets={};branches={};cs=Cs(CS_ARCH_X86,CS_MODE_32)
for key,f in d['functions'].items():
 asm=f['assembly'];assert not asm['has_more'] and len(asm['items'])==asm['total'];sets[int(key,16)]=set()
 for x in asm['items']:
  a=int(x['address'],16);raw=bytes.fromhex(x['bytes']);assert bytes(vm.mem_read(a,len(raw)))==raw;core[a]=x;sets[int(key,16)].add(a);i=next(cs.disasm(raw,a,count=1))
  if i.mnemonic.startswith('j') and i.mnemonic!='jmp':branches[a]={a+i.size,int(i.op_str,16)}
assert get(0x51f780,1)==0xc3;core[0x51f780]={'bytes':'c3','instruction':'RET'}
assert bytes(vm.mem_read(0x4dd2a2,8)).hex()=='3c3e0f859b000000'
for f in d['field_meaning_facts_only'].values():
 asm=f['assembly'];assert not asm['has_more'] and len(asm['items'])==asm['total']
 for x in asm['items']:
  raw=bytes.fromhex(x['bytes']);assert bytes(vm.mem_read(int(x['address'],16),len(raw)))==raw
imports={};desc=base+u32(opt+104)
while get(desc+12):
 original=get(desc) or get(desc+16);iat=get(desc+16);index=0
 while get(base+original+index*4):
  value=get(base+original+index*4)
  if not value&0x80000000:imports[base+iat+index*4]=text(base+value+2)
  index+=1
 desc+=20
assert imports[0x56f22c]=='timeGetTime' and imports[0x56f174]=='PostMessageA'
put(0x56f22c,STOP+0x100);put(0x56f174,STOP+0x120)
endpoints={STOP+0x100:(0,0),STOP+0x120:(4,16),0x536820:(4,0),0x536c30:(6,0),0x4d6160:(9,0)}
events=[];p=[];clocks=tiles=0;executed=set();edges=set();previous=None;pixel_count=frame_count=frame_pixels=0;word_repeat_counts=set();byte_repeat_counts=set()
def event(a,argv):events.append([a,len(argv)]+[v&0xffffffff for v in argv]+[0]*(9-len(argv))+[get(0xe38dda,1),get(0xe38dca,1),get(0xe38e68),get(0xe38ee4),get(0x70570c),get(0x6057e8),get(0xe39140),get(0xd3b198),get(0x7056c8),get(0xe38e34),get(0xe38e30),get(0x5c68b9,1),get(0xf56f8c),get(frame_pixels,1) if pixel_count else 0,get(frame_pixels+pixel_count-1,1) if pixel_count else 0])
def code(cpu,a,n,user):
 global previous,clocks,tiles
 if previous in branches:edges.add((previous,a))
 previous=a
 if a==STOP:cpu.emu_stop();return
 if a not in endpoints:
  if a==0x40f90f:word_repeat_counts.add(cpu.reg_read(UC_X86_REG_ECX))
  if a==0x40f916:byte_repeat_counts.add(cpu.reg_read(UC_X86_REG_ECX))
  assert a in core,f'native escaped {a:08x}';executed.add(a);return
 count,cleanup=endpoints[a];sp=cpu.reg_read(UC_X86_REG_ESP);argv=[get(sp+4+i*4) for i in range(count)];event({STOP+0x100:0x56f22c,STOP+0x120:0x56f174}.get(a,a),argv);result=0x12345678
 if a==STOP+0x100:
  if clocks==0:
   for flag,address,value,width in [(1,0xe38dda,p[13],1),(2,0x6057e8,p[14],4),(4,0x70570c,p[15],4),(8,0xe38e68,p[16],4),(16,0xe38ee4,p[17],4)]:
    if p[12]&flag:put(address,value,width)
  else:
   if p[12]&2048:put(0x6057e8,p[28])
   if p[12]&4096:put(0xe38dda,p[29],1)
  result=p[7] if clocks else p[6];clocks+=1
 elif a==STOP+0x120:
  if p[12]&32:put(0x70570c,p[18])
  if p[12]&64:put(0xe38dda,p[19],1)
  if p[12]&128:put(0xe39140,p[20])
  result=p[11]
 elif a==0x536820:
  if tiles==0:
   if p[12]&256:put(0xe38da8,p[21]);put(0xe38da4,p[22])
   if p[12]&512:
    for offset,value in enumerate(p[23:27]):put(0x5c68a8+offset*2,value,2)
   if p[12]&1024:put(0xe38dda,p[27],1)
   if p[12]&16384:put(0xf56f8c,p[46])
  tiles+=1
 cpu.reg_write(UC_X86_REG_EAX,result);cpu.reg_write(UC_X86_REG_ECX,0xdeadc0de);cpu.reg_write(UC_X86_REG_EDX,0xfefefefe);cpu.reg_write(UC_X86_REG_ESP,sp+4+cleanup);cpu.reg_write(UC_X86_REG_EIP,get(sp))
def write(cpu,access,a,n,value,user):assert STACK<=a and a+n<=STACK+0x100000 or FRAME<=a and a+n<=FRAME+frame_count or any(lo<=a and a+n<=lo+size for lo,size in regions),f'unwatched native write {a:08x}/{n}'
vm.hook_add(UC_HOOK_CODE,code);vm.hook_add(UC_HOOK_MEM_WRITE,write)
def setup(case):
 global p,events,clocks,tiles,pixel_count,frame_count,frame_pixels
 p=case;events=[];clocks=tiles=0
 for i,(a,n) in enumerate(regions):vm.mem_write(a,bytes((p[1]+j*37+i*11)&255 for j in range(n)))
 vm.mem_write(0x5c68a8,original_button);put(0x5c68bc,p[39]);put(0x5c68c0,p[40])
 for a,v,n in [(0xe38e68,p[2],4),(0xe38ee4,p[3],4),(0xe38dda,p[4],1),(0xe38dca,p[5],1),(0x70570c,p[8],4),(0x6057e8,p[9],4),(0xe39140,p[10],4),(0xe38da8,p[30],4),(0xe38da4,p[31],4),(0xe94d98,p[37],4),(0x605a44,0,4)]:put(a,v,n)
 put(0xf56f8c,p[43])
 pixel_count=p[9] if p[0]==9 else p[49]*p[50]*2&0xffffffff;assert pixel_count<=0x100000 and p[51]<4
 frame_pixels=FRAME+16+p[51];frame_count=pixel_count+32+p[51];vm.mem_write(FRAME,bytes((p[1]+i*37+0x63)&255 for i in range(frame_count)))
 put(0xe38db0,p[49]);put(0xe38dac,p[50]);put(0xe92e18,frame_pixels if p[48] else 0)
 for i in range(256):put(0xe94624+i*16,p[36]);put(0xe9461c+i*16,0)
 for i in range(2):
  node=0x6f2080+i*32
  for off,v in [(0,1),(8,0x12348001+i),(12,0xabcd7fff+i),(16,0x1234ffff),(20,3),(24,node+32 if i==0 and p[38]==2 else 0)]:put(node+off,v)
 if p[38]:put(0xe9461c+(p[4]&255)*16,0x6f2080);put(0xe9461c+62*16,0x6f2080)
def mouse(flags,position):
 x=get(0xe38da8)+80;y=get(0xe38da4)+64;b=0x5c68a8
 for a in range(0xe38df4,0xe38e05):put(a,0,1)
 put(0xe38e34,x+s16(get(b,2))+1);put(0xe38e30,y+s16(get(b+2,2))+1)
 if position==1:put(0xe38e34,x+s16(get(b,2))-1)
 if position==2:put(0xe38e34,x+s16(get(b+4,2)))
 if position==3:put(0xe38e30,y+s16(get(b+2,2))-1)
 if position==4:put(0xe38e30,y+s16(get(b+6,2)))
 if position==5:put(0xe38e34,0x80000000);put(0xe38e30,0x80000000)
 if position==6:put(0xe38e34,p[34]);put(0xe38e30,p[35])
 if flags in [1,2,3,4]:put({1:0xe38e03,2:0xe38e02,3:0xe38df7,4:0xe38df8}[flags],1,1)
def call(entry,argv=(),local=False):
 global previous
 previous=None;sp=STACK+0x80000;saved={UC_X86_REG_EBX:p[1]^0x12345678,UC_X86_REG_ESI:p[1]^0x789abcde,UC_X86_REG_EDI:p[1]^0xabcdef12,UC_X86_REG_EBP:p[1]^0x2468ace0}
 if local:
  for i,r in enumerate([UC_X86_REG_EDI,UC_X86_REG_ESI,UC_X86_REG_EBP,UC_X86_REG_EBX]):put(sp+i*4,saved[r])
  put(sp+0x8c,STOP);expected=sp+0x90
 else:
  put(sp,STOP)
  for i,v in enumerate(argv):put(sp+4+i*4,v)
  expected=sp+4
 for r,v in saved.items():vm.reg_write(r,v)
 vm.reg_write(UC_X86_REG_EAX,p[1]);vm.reg_write(UC_X86_REG_ECX,p[1]^0x44556677);vm.reg_write(UC_X86_REG_EDX,p[1]^0x99aabbcc);vm.reg_write(UC_X86_REG_EBP,0 if local else saved[UC_X86_REG_EBP]);vm.reg_write(UC_X86_REG_EFLAGS,0x202);vm.reg_write(UC_X86_REG_ESP,sp)
 vm.emu_start(entry,STOP,count=1000000);assert vm.reg_read(UC_X86_REG_EIP)==STOP and vm.reg_read(UC_X86_REG_ESP)==expected
 for r,v in saved.items():assert vm.reg_read(r)==v
def native(case):
 setup(case);mouse(p[32],p[33]);op=p[0]
 if op in [0,1,2]:call({0:0x402da0,1:0x402de0,2:0x402e10}[op])
 elif op==3:call(0x4dd2aa,local=True)
 elif op==4:
  call(0x402da0)
  if get(0xe38dda,1)==62:call(0x4dd2aa,local=True)
 elif op==5:
  call(0x4dd2aa,local=True)
  if get(0xe38dda,1)==62:mouse(p[41],p[42]);call(0x4dd2aa,local=True)
 elif op==6:call(0x4cfd60,[p[2]])
 elif op==7:call(0x534540)
 elif op==8:call(0x4cf1c0)
 elif op==9:call(0x40f8f0,[frame_pixels,p[9],p[43]])
 else:raise AssertionError(op)
 return {'state':(b''.join(bytes(vm.mem_read(a,n)) for a,n in regions)+bytes(vm.mem_read(FRAME,frame_count))).hex(),'events':list(events)}
rng=random.Random(0x402da0);cases=[]
def add(op=0,**kw):
 p=[0]*52;p[0]=op;p[1]=rng.getrandbits(32);p[48]=1;p[49]=16;p[50]=12
 for i,v in {2:3,3:1,4:62 if op in [3,5] else 12,5:0,6:10001,7:20001,8:0xaabbcc0c,9:0,10:0x89abcdef,11:1,12:0,13:0x99,14:0xffffffff,15:0x11223344,16:256,17:0xffffffff,18:0xaabbccfe,19:0x98,20:0x76543210,21:123,22:57,23:0x8001,24:0x7fff,25:0x8000,26:0x8002,27:15,28:0xffffffff,29:0x97,30:192,31:144,32:0,33:0,34:0,35:0,36:1,37:0,38:0,39:0,40:0,41:0,42:0}.items():p[i]=v
 for k,v in kw.items():p[int(k[1:])]=v&0xffffffff
 if 'v43' not in kw:p[43]=0x12345678
 if 'v44' not in kw:p[44]=0x87654321
 if 'v45' not in kw:p[45]=0x7ffffff0
 if 'v46' not in kw:p[46]=0x89abcdef
 if 'v47' not in kw:p[47]=0xaabbccdd
 cases.append(p);return p
values=[0x80000000,0xffffffff,0,1,2,3,256,0x7fffffff]
for current,confirmed,clock,mode in itertools.product(values,values,[0,0x12345678,0xffffffff],[0,12,255]):add(v2=current,v3=confirmed,v6=clock,v4=mode)
for mask,mode in itertools.product(range(32),[1,12,62,255]):add(v12=mask,v4=mode,v6=0x80000000)
for handle,saved,result,mask in itertools.product([0,1,0x12340000,0x89abcdef,0xffffffff],[0,0x100,0xaabbccdd],[0,1,0x80000000],[0,32,64,128,224]):add(1,v10=handle,v8=saved,v11=result,v12=mask)
for current,saved,mode in itertools.product(values,[0,0x100,0xaabbccdd],[0,62,255]):add(2,v2=current,v8=saved,v4=mode)
starts=[0,1,9999,10000,0x7fffd8ef,0x7fffd8f0,0x7fffffff,0x80000000,0xffffd8ef,0xffffd8f0,0xffffffff]
for started,delta,flag,position,ready,state in itertools.product(starts,[-1,0,1],[0,1,2,3,4],[0,5],[0,1],[0,2]):add(3,v9=started,v6=(started+10000+delta)&0xffffffff,v32=flag,v33=position,v36=ready,v37=state)
for position,x,y,flag in itertools.product(range(7),[0,0x7ffffffe,0x80000000],[0,0x80000001],[0,1,3]):add(3,v33=position,v30=x,v31=y,v32=flag,v34=0x7fffffff,v35=0x80000000)
for nodes,flag,state in itertools.product([0,1,2],[0,1,3],[0,2]):add(3,v38=nodes,v36=0,v32=flag,v37=state)
for mask,flag,ready in itertools.product([2,32,64,128,224,256,512,768,1024,1280,1536,1792],[0,1,3],[0,1]):add(3,v12=mask,v32=flag,v36=ready,v37=2,v6=10001)
for mask,flag,deadline,mode in itertools.product([0,2048,4096,6144],[0,1,3],[0,10001],[1,12,255]):add(4,v12=mask,v32=flag,v6=0,v7=deadline,v4=mode)
for first,second,firstclock,secondclock,mask in itertools.product([0,3],[0,1,3],[0,10000],[10000,10001],[0,2,2048,6144]):add(5,v32=first,v41=second,v6=firstclock,v7=secondclock,v12=mask)
for primary,secondary in itertools.product([0,256,0x12345678,0xffffffff],[0,0xabcdef01]):add(3,v32=1,v39=primary,v40=secondary)
for mode in values+[1024]:add(6,v2=mode)
for x,y,font,mask in itertools.product([0,192,256,0x7fffffff,0x80000000,0xffffffff],[0,144,256,0x7fffffff,0x80000000,0xffffffff],[0,1,0x12345678,0x80000000,0xffffffff],[0,256,16384,256|16384]):add(7,v30=x,v31=y,v43=font,v12=mask)
for enabled,width,height,alignment in itertools.product([0,1],[0,1,2,3,16,256],[0,1,3,12],range(4)):add(8,v48=enabled,v49=width,v50=height,v51=alignment)
for op,width,height,alignment in itertools.product([7,8],[0x80000000,0x80000001,0xffffffff],[0,0x80000000],range(4)):add(op,v49=width,v50=height,v51=alignment)
for op,enabled,alignment in itertools.product([7,8],[0,1],[0,3]):add(op,v49=640,v50=480,v48=enabled,v51=alignment)
for count,value,alignment in itertools.product(range(18),[0,1,255,0x12345678],range(4)):add(9,v9=count,v43=value,v51=alignment)
if args.debug_limit:cases=cases[:args.debug_limit]
expected=[native(c) for c in cases];required=[0x402da0,0x402de0,0x402e10,0x4dd2aa,0x534540,0x4cf1c0];owned=set().union(*(sets[e] for e in required));owned_edges={(a,b) for a,z in branches.items() if a in owned for b in z};complete=owned<=executed and owned_edges<=edges
if not args.debug_limit:assert complete,{'instructions':[f'{a:08x}' for a in sorted(owned-executed)],'edges':[(hex(a),hex(b)) for a,b in owned_edges-edges]}
product=ROOT if args.production else PHASE
source_paths=[product/'src/unit_00025.c',product/'src/unit_00026.c',product/'src/unit_02436.c',product/'src/unit_01766.c',product/'runtime/display_confirmation.c',product/'runtime/display_confirmation_clock.c',product/'include/display_confirmation.h',product/'tests/display_confirmation.c',ROOT/'src/unit_00119.c',ROOT/'src/unit_00416.c',ROOT/'src/unit_00417.c',ROOT/'src/unit_01836.c',ROOT/'src/unit_01857.c',ROOT/'src/unit_01858.c',ROOT/'src/unit_01775.c',ROOT/'include/menu_input.h',ROOT/'include/menu_layout.h',ROOT/'include/selection_popup.h',ROOT/'include/restore_types.h',ROOT/'include/standalone_data.h',PHASE/'build/CMakeLists.txt',Path(__file__).resolve()]
report={'format':1,'generated_at':datetime.now(timezone.utc).isoformat(),'production':args.production,'passed':False,'complete_case_set':not bool(args.debug_limit),'checked':len(cases),'native_sha256':digest,'compiled_C_path':args.exe.resolve().relative_to(ROOT).as_posix(),'compiled_C_sha256':sha(args.exe.resolve()),'source_hashes':{p.relative_to(ROOT).as_posix():sha(p) for p in source_paths},'native_exports':{native_file.relative_to(ROOT).as_posix():sha(native_file)},'qualified_controller':{'complete':complete,'entries':[f'{a:08x}' for a in required],'instructions':len(owned),'executed':len(owned&executed),'conditional_edges':len(owned_edges),'edges_executed':len(owned_edges&edges),'excluded_instructions':[],'excluded_edges':[]},'instruction_coverage':{f'{a:08x}':{'total':len(z),'executed':len(z&executed),'missing':[f'{x:08x}' for x in sorted(z-executed)]} for a,z in sets.items()},'watched_memory':{'regions':regions,'bytes_per_case':sum(n for a,n in regions),'method':'Full bytes and surrounding canaries compared. Every native write is constrained to watched data or private stack.'},'imports':{'0056f22c':imports[0x56f22c],'0056f174':imports[0x56f174]},'variants':{},'original_game_process_executed':False,'original_machine_code_fallback_used':False,'limitations':['Clock and PostMessageA are explicit OS boundaries. Clock production wrapper compilesDebug/O2 but is replaced by finite controlled returns in the oracle; timeGetTime has no separate failure flag and zero is a valid tick. PostMessageA false/high-bit returns are ignored exactly as native. No real post, resize, monitor or gameprocess action is performed.','This proof composes actual416/417/1858/1836/1857 C/native bodies using the authenticated28byte5C68A8record (trigger1,left402E10,rightbareRET51F780). Only trigger1 context of the sharedbutton engine is claimed here; other sharedtriggers remain separately proved.','Deeper536820/536C30 panel/raster work is controlled with exact4/6args and mutation/reload tests. Pixels are not claimed. Linkedlayout nodes must be valid/acyclic; invalidnativepointers are not silently skipped.','The selected3Eblock begins4DD2AA and includes sharedreturnepilogue4DD8C3..D8CA; original1868prelude/selectionguard is outside this localproof, with EBP=0 and originalsavedframe prepared. Parentguard selects UI3E; allothermodes remain unchanged.','The native deadline is signed(now)>signed(uint32(start+10000)), with start reloaded afterclock. It intentionally is not replaced by an unsignedelapsed-time algorithm; boundary/wrap quirks are preserved.','ActualkeyboardEscape uses the separate414CD0window→539170/4CC110/4CC160/538CD0 route and is not closed by this localcase. InputflagE38DF7 on thisrecord routes bareRET and suppressestimeout for thatframe; it is not invented as immediate rollback.','PostingF7 requests the existingwindowmode1route. Physical4CFDC0/graphics/video transitions remain outside this callback/controller proof. E38E68 modeindex meaning is independently supported by actualunchanged4CFD60 mapper (all16instructions exercised),4CFDC0incomingDWORDstore and4147F0initialconfirmedmodecopy retained as facts-only.']}
out=ROOT/'metadata/display-confirmation-emulation.json' if args.production else PHASE/'metadata/display-confirmation-emulation.json'
report['limitations'][2]='Actual12instruction4CF1C0 clear and unchanged22instruction40F8F0 fill execute C/native code, including allisolatedRGB565framebytes andprefix/suffixguards; no clearpointer is controlled or truncated. Deeper536820/536C30 panel/raster and4D6160 text remain exact4/6/9argument endpoints with mutation/reload tests. Artwork/text pixels andphysicalsurfaces are not claimed. Whole31instruction534540 renderer executes actualC/native code using fullDWORD offset/font fields and has no countdownalgorithm. Validacycliclayoutnodes and DFclearWin32ABI are required.'
report['framebuffers']={'actual_clear_and_fill':True,'base':FRAME,'default_width':16,'default_height':12,'maximum_fixture_bytes':max((c[9] if c[0]==9 else c[49]*c[50]*2&0xffffffff) for c in cases),'real_size_fixture':'640x480x2=614400bytes; enabled/NULL andalignment0/3, throughwhole renderer anddirectclear','guards':'16+alignment prefixbytes,16suffixbytes; fullpayloadandguards compared for everycase, includingNULLskip,zero/wrappedlengths,unalignedDWORDfills and0..3byte tails.','native_size_domain':'Computedwrappedlength mustfit suppliedvalidprivatebuffer; hugeunsafeoverflowlengths are not converted to successful clear.'}
report['watched_memory']['fixed_bytes_per_case']=report['watched_memory'].pop('bytes_per_case')
report['watched_memory']['variable_frame_region']='FRAME..FRAME+wrapped_pixel_count+32+alignment; payload and prefix/suffix included in every full-byte comparison.'
report['fill_primitive']={'entry':'0040f8f0','source':'src/unit_00119.c','modified':False,'native_instructions':22,'instructions_executed':len(sets[0x40f8f0]&executed),'argument_contract':'cdecl(destinationDWORD,countDWORD,valueLowBYTE), no consumedreturn; nativeDFclear flat32bit ABI.','rep_stosd_zero_and_nonzero':0 in word_repeat_counts and any(word_repeat_counts),'rep_stosb_counts':sorted(byte_repeat_counts),'all_tails_executed':{0,1,2,3}<=byte_repeat_counts}
if not args.debug_limit:assert report['fill_primitive']['instructions_executed']==22 and report['fill_primitive']['rep_stosd_zero_and_nonzero'] and report['fill_primitive']['all_tails_executed']
try:
 for label,path in [('Debug',args.exe),('O2',args.optimized_exe)]:
  path=path.resolve();proc=subprocess.Popen([str(path),'--oracle-batch'],stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True)
  try:
   assert json.loads(proc.stdout.readline())['fields']==52
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
print(json.dumps({'passed':report['passed'],'checked':len(cases),'comparisons':report.get('comparisons'),'coverage':report['qualified_controller']}))
