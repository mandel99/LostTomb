"""Whole nested Escape modal versus pinned x86, with actual text/mouse/layout/input."""
from pathlib import Path
from datetime import datetime,timezone
import argparse,hashlib,itertools,json,re,struct,subprocess,sys,random
ROOT=Path(__file__).resolve().parents[3] if 'menu_keyboard_modal_phase' in str(Path(__file__)) else Path(__file__).resolve().parents[1];PHASE=ROOT/'artifacts/menu_keyboard_modal_phase'
sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE,UC_HOOK_MEM_WRITE,UC_HOOK_MEM_INVALID
from unicorn.x86_const import *
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
ap=argparse.ArgumentParser();ap.add_argument('--production',action='store_true');ap.add_argument('--exe',type=Path,default=ROOT/'build/agent-menu-keyboard-modal/Debug/restore_menu_keyboard_modal_verify.exe');ap.add_argument('--optimized-exe',type=Path,default=ROOT/'build/agent-menu-keyboard-modal/Release/restore_menu_keyboard_modal_verify.exe');ap.add_argument('--limit',type=int);ap.add_argument('--start',type=int,default=0);args=ap.parse_args()
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest();product=ROOT if args.production else PHASE;schema=(product/'tests/menu_keyboard_modal_case.h').read_text();names=re.findall(r'\bP_([A-Z0-9_]+)\b',schema.split('enum {',1)[1].split('};',1)[0]);names.remove('FIELDS');FIELDS=len(names);globals().update({'P_'+n:i for i,n in enumerate(names)})
image=(ROOT/'build/smoke-game/Pharaoh.exe').read_bytes();digest=hashlib.sha256(image).hexdigest();assert digest=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
u16=lambda p:struct.unpack_from('<H',image,p)[0];u32=lambda p:struct.unpack_from('<I',image,p)[0];pe=u32(60);opt=pe+24;sec=opt+u16(pe+20);base=u32(opt+28);vm=Uc(UC_ARCH_X86,UC_MODE_32);vm.mem_map(base,(u32(opt+56)+4095)&~4095)
for i in range(u16(pe+6)):
 q=sec+i*40;vm.mem_write(base+u32(q+12),image[u32(q+20):u32(q+20)+u32(q+16)])
BASE=0x30000000;STOP=0x33000000;STACK=0x34000000;BODY=BASE+0x102;TITLE=BASE+0x115
for a,n in [(BASE,0x10000),(STOP,0x10000),(STACK,0x200000)]:vm.mem_map(a,n)
def get(a,n=4):return int.from_bytes(vm.mem_read(a,n),'little')
def put(a,v,n=4):vm.mem_write(a,(v&((1<<(8*n))-1)).to_bytes(n,'little'))
def signed(v):return (v&0x7fffffff)-(v&0x80000000)
def short(v):return (v&32767)-(v&32768)
def div(a,b):return (abs(a)//abs(b))*(-1 if (a<0) != (b<0) else 1)
def string(a):
 out=bytearray()
 for _ in range(512):
  b=get(a,1);a+=1
  if not b:return out.decode('ascii')
  out.append(b)
 raise AssertionError('unterminated string')
regions=[(0xf535a0,64),(0xf53fe0,0x160),(0xe38d80,0x440),(0xd3b180,0x2d0),(0x7056a0,0x90),(0x5f8900,0x70),(0xe945e0,0x1100),(0xe34c10,0x80),(0xe36c60,64),(0xe20750,48),(0xe92df0,64),(0xf56f70,0x60),(0x6057d0,48),(0x6fdc30,48),(BASE+0xf0,0x200),(BASE+0x800,0x200),(0x5c6890,64)]
native_file=ROOT/'metadata/menu-keyboard-modal-native.json' if args.production else PHASE/'metadata/native.json';d=json.loads(native_file.read_text());assert d['native_sha256']==digest
qualification_file=ROOT/'metadata/menu-keyboard-modal-qualification.json' if args.production else PHASE/'metadata/native-qualification.json';qualification=json.loads(qualification_file.read_text());assert qualification['native_sha256']==digest and qualification['native_export_sha256']==sha(native_file)
core={};sets={};branches={};cap=Cs(CS_ARCH_X86,CS_MODE_32)
def attach(entry,items):
 sets[entry]=set()
 for x in items:
  a=int(x['address'],16);b=bytes.fromhex(x['bytes']);assert bytes(vm.mem_read(a,len(b)))==b;core[a]=x;sets[entry].add(a);i=next(cap.disasm(b,a,count=1))
  if i.mnemonic.startswith('j') and i.mnemonic!='jmp':branches[a]={a+i.size,int(i.op_str,16)}
for k,f in d['functions'].items():
 a=f['assembly'];assert not a['has_more'] and len(a['items'])==a['total'];attach(int(k,16),a['items'])
controller=json.loads((ROOT/'metadata/high-scores-input-native.json').read_text());attach(0x4dd7b8,[x for x in controller['items'] if 0x4dd7b8<=int(x['address'],16)<=0x4dd810])
display_file=ROOT/'metadata/display-confirmation-native.json';dd=json.loads(display_file.read_text())
for k in ['00402da0','00402de0','00402e10','004dd2aa']:attach(int(k,16),dd['functions'][k]['assembly']['items'])
assert get(0x51f780,1)==0xc3;core[0x51f780]={'bytes':'c3','instruction':'RET'}
original_buttons=bytes(vm.mem_read(0x5f8910,84));original_display_button=bytes(vm.mem_read(0x5c68a8,28))
assert original_buttons==b''.join(bytes.fromhex(x['bytes']) for x in d['modal_buttons'])
assert list(struct.unpack('<5I',bytes(vm.mem_read(0x538c68,20))))==[int(x,16) for x in d['renderer_dispatch_table']['targets']]
imports={};desc=base+u32(opt+104)
while get(desc+12):
 original=get(desc) or get(desc+16);iat=get(desc+16);index=0
 while get(base+original+index*4):
  v=get(base+original+index*4)
  if not v&0x80000000:imports[base+iat+index*4]=string(base+v+2)
  index+=1
 desc+=20
api_counts={0x56f184:(5,20),0x56f190:(4,16),0x56f18c:(1,4),0x56f188:(1,4),0x56f198:(0,0),0x56f174:(4,16),0x56f1cc:(1,4),0x56f32c:(1,4),0x56f22c:(0,0),0x56f1e4:(1,4),0x56f1f8:(4,16)}
expected_names={0x56f184:'PeekMessageA',0x56f190:'GetMessageA',0x56f18c:'TranslateMessage',0x56f188:'DispatchMessageA',0x56f198:'WaitMessage',0x56f174:'PostMessageA',0x56f1cc:'GetCursorPos',0x56f22c:'timeGetTime'}
expected_names.update({0x56f1e4:'GetAsyncKeyState',0x56f1f8:'DefWindowProcA'})
for a,n in expected_names.items():assert imports[a]==n
assert 'SmackClose' in imports[0x56f32c]
traps={};endpoints={}
for i,(iat,contract) in enumerate(api_counts.items()):a=STOP+0x100+i*16;put(iat,a);traps[a]=iat;endpoints[a]=contract
game_counts={0x4d71c0:1,0x4d0280:1,0x4cf460:1,0x4ce100:1,0x4ce2d0:0,0x4ce6e0:3,0x4cff20:0,0x4cbf30:2,0x4d5d90:0,0x536820:4,0x536a30:4,0x536c30:6,0x4cc720:6,0x4d6250:9,0x4d63d0:8,0x53b090:1}
endpoints.update({a:(n,0) for a,n in game_counts.items()})
events=[];p=[];frames=peeks=gets=waits=metrics=labels=borders=texts=panels=last_modal=0;previous=None;executed=set();edges=set();jump_indices=set()
def event(a,argv):events.append([a,len(argv)]+[v&0xffffffff for v in argv]+[0]*(9-len(argv))+[get(0xe38dda,1),get(0xe38dca,1),get(0xf535c0,1),get(0xf535c4),get(0xf535c8),get(0xd3b43d,1),get(0xe39140),get(0xe39128),get(0xe3912c),get(0x7056c8),get(0xd3b198),get(0xf54054),get(0xf5404c),get(0xf54114),get(0xf54118),get(0xe38e34),get(0xe38e30),get(0xe38e07,1),get(0xe38da8),get(0xe38da4),get(0x6057e8)])
def msg_write(a):
 for i,v in enumerate([p[P_HWND],0x400+gets,0x12340000+gets,0x87654321^gets,0x11223344+gets,7+gets,11+gets]):put(a+i*4,v)
def msg_event(a,ptr):event(a,[0xabc00001]+[get(ptr+i*4) for i in range(7)])
def prepare_local(parent_sp,entry):
 sp=parent_sp-0x8c
 for i,r in enumerate([UC_X86_REG_EDI,UC_X86_REG_ESI,UC_X86_REG_EBP,UC_X86_REG_EBX]):put(sp+i*4,vm.reg_read(r))
 vm.reg_write(UC_X86_REG_EBP,0);vm.reg_write(UC_X86_REG_ESP,sp);vm.reg_write(UC_X86_REG_EIP,entry)
def hook(cpu,a,n,user):
 global previous,frames,peeks,gets,waits,metrics,labels,borders,texts,panels,last_modal
 if previous in branches:edges.add((previous,a))
 previous=a
 if a==STOP:cpu.emu_stop();return
 if a==0x4d8310:
  if get(0xe38dda,1)==0x54:prepare_local(cpu.reg_read(UC_X86_REG_ESP),0x4dd7b8);return
  event(a,[]);sp=cpu.reg_read(UC_X86_REG_ESP);cpu.reg_write(UC_X86_REG_ESP,sp+4);cpu.reg_write(UC_X86_REG_EIP,get(sp));return
 if a==0x4162a2:last_modal=cpu.reg_read(UC_X86_REG_EAX)
 if a==0x538a12:jump_indices.add(cpu.reg_read(UC_X86_REG_EAX))
 if a not in endpoints:
  assert a in core,f'nativeescaped{a:08x}';executed.add(a);return
 count,cleanup=endpoints[a];sp=cpu.reg_read(UC_X86_REG_ESP);argv=[get(sp+4+i*4) for i in range(count)];address=traps.get(a,a);answer=0x12345678;mask=p[P_MUTATIONS]
 if address==0x56f184:
  peeks+=1;assert peeks<=32;event(address,[0xabc00001]+argv[1:]);ready=(p[P_SCRIPT] in [1,2,5] and peeks==1) or (p[P_SCRIPT]==8 and gets<2) or (p[P_SCRIPT] in [4,6] and frames>=(1 if p[P_SCRIPT]==4 else 2))
  if ready:msg_write(argv[0])
  answer=p[P_PEEK_RESULT] if ready else 0
 elif address==0x56f190:
  event(address,[0xabc00001]+argv[1:]);gets+=1;msg_write(argv[0]);answer=0 if p[P_SCRIPT] in [1,4,6] else p[P_GET_RESULT]
 elif address in [0x56f18c,0x56f188]:
  msg_event(address,argv[0]);answer=0 if address==0x56f18c else 0x89abcdef
  if address==0x56f188:
   if p[P_SCRIPT]==5:put(0xf535c0,1,1);put(0xf535c4,p[P_TARGET])
   if p[P_SCRIPT]==8:put(0xd3b43d,1,1)
   if mask&1024:put(0xe38dda,p[P_MUT_MODE],1);put(0xe39140,p[P_EXTRA])
 elif address==0x56f198:event(address,[]);waits+=1;put(0xe39128,0);put(0xe3912c,1);answer=p[P_WAIT_RESULT]
 elif address==0x56f174:
  event(address,argv)
  if mask&2048:put(0xd3b43d,2,1);put(0xe39140,p[P_EXTRA])
  answer=p[P_POST_RESULT]
 elif address==0x56f1cc:
  frames+=1;event(address,[0xabc00002]);b=0x5f8910+p[P_TARGET]*28;put(argv[0],div(short(get(b,2))+short(get(b+4,2)),2));put(argv[0]+4,div(short(get(b+2,2))+short(get(b+6,2)),2));put(0xe38e07,1 if frames>=p[P_CLICK_AT] else 0,1);put(0xe92e0c,p[P_NOW]+frames*17);answer=0 if p[P_CURSOR_FAIL]==0xffffffff or p[P_CURSOR_FAIL]==frames else 1
 elif address==0x56f32c:
  event(address,argv)
  if mask&2:put(0xf54114,p[P_SMACK_NEW_HANDLE])
  answer=p[P_SMACK_RESULT]
 elif address==0x56f22c:event(address,[]);answer=p[P_NOW]
 elif address==0x56f1e4:event(address,argv);answer=p[P_TRACE]
 elif address==0x56f1f8:event(address,argv);answer=p[P_EXTRA]
 else:
  event(address,argv)
  if address==0x4d0280:
   if mask&256:put(0xf535c4,p[P_MUT_INDEX]);put(0xf535c0,0,1)
   if mask&512:put(0xd3b43d,1,1)
  elif address==0x4ce100:
   if mask&1:put(0xf54114,p[P_SMACK_NEW_HANDLE]);put(0xf5404c,1)
   answer=p[P_CONTEXT_RESULT]
  elif address==0x4ce6e0:
   if argv[0]==0x5f8fe4 and mask&4:put(0xf54118,p[P_SMACK_NEW_KIND])
  elif address==0x4cff20:answer=p[P_MEMORY_RESULT]
  elif address==0x4cbf30:
   which=metrics%3;metrics+=1
   if which==0 and mask&8:put(0xf56fb4,p[P_FONT_ALT]);put(0xf56f8c,p[P_FONT_BODY]^0x100)
   if which==1 and mask&16:put(0xf56fb4,p[P_FONT_ALT]^0x200);put(0xf56f8c,p[P_FONT_ALT]^0x400)
   if which==0 and mask&8192:put(0xe38db0,p[P_WIDTH_ALT]);put(0xe38dac,p[P_HEIGHT_ALT])
   answer=p[[P_BODY_WIDTH,P_TITLE_WIDTH,P_FINAL_WIDTH][which]]
  elif address==0x4d5d90:put(0xe94d9c,1)
  elif address==0x536820:
   if panels==0 and mask&32:put(0xe38da8,p[P_OX_ALT]);put(0xe38da4,p[P_OY_ALT]);put(0xf56f8c,p[P_FONT_ALT])
   panels+=1
  elif address==0x4cc720:
   if texts==1 and mask&16384:put(0xf535c0,p[P_LATCH],1);put(0xf535c4,p[P_MUT_INDEX])
   texts+=1
  elif address==0x4d6250:
   if labels==0 and mask&64:put(0xf535c8,p[P_MUT_COUNT])
   labels+=1
  elif address==0x4d63d0:
   borders+=1
   if mask&128:put(0xf535c0,1,1);put(0xf535c4,p[P_MUT_INDEX])
 cpu.reg_write(UC_X86_REG_EAX,answer);cpu.reg_write(UC_X86_REG_ECX,0xdeadc0de);cpu.reg_write(UC_X86_REG_EDX,0xfefefefe);cpu.reg_write(UC_X86_REG_ESP,sp+4+cleanup);cpu.reg_write(UC_X86_REG_EIP,get(sp))
def written(cpu,access,a,n,value,user):assert STACK<=a and a+n<=STACK+0x200000 or any(lo<=a and a+n<=lo+size for lo,size in regions),f'unwatchedwrite{a:08x}/{n}'
vm.hook_add(UC_HOOK_CODE,hook);vm.hook_add(UC_HOOK_MEM_WRITE,written)
def invalid(cpu,access,a,n,value,user):raise AssertionError({'invalid_access':access,'address':f'{a:08x}','size':n,'eip':f'{cpu.reg_read(UC_X86_REG_EIP):08x}','eax':f'{cpu.reg_read(UC_X86_REG_EAX):08x}','ecx':f'{cpu.reg_read(UC_X86_REG_ECX):08x}','edx':f'{cpu.reg_read(UC_X86_REG_EDX):08x}','esi':f'{cpu.reg_read(UC_X86_REG_ESI):08x}','case':dict(zip(names,p))})
vm.hook_add(UC_HOOK_MEM_INVALID,invalid)
def setup(case):
 global p,events,frames,peeks,gets,waits,metrics,labels,borders,texts,panels,last_modal
 p=case;events=[];frames=peeks=gets=waits=metrics=labels=borders=texts=panels=last_modal=0
 for i,(a,n) in enumerate(regions):vm.mem_write(a,bytes((p[P_SEED]+j*37+i*11)&255 for j in range(n)))
 vm.mem_write(0x5f8910,original_buttons);vm.mem_write(0x5c68a8,original_display_button);vm.mem_write(BASE+0x100,b'\1\3Exit the city?\0\1\0\3\0Confirm exit\0\0')
 vals={0xe36c70:BASE+0x100,0xe34c38+5*8:0,0xe20768:BODY,0xe20764:TITLE,0xf54054:p[P_ACTIVE],0xf5404c:p[P_READY],0xf54114:p[P_HANDLE],0xf54118:p[P_KIND],0xe39128:p[P_ICONIC],0xe3912c:p[P_FOREGROUND],0xe39140:p[P_HWND],0x6fdc40:0,0x6fdc54:BASE+0x900 if p[P_TEXT] else 0,0xf56f8c:p[P_FONT_BODY],0xf56fb4:p[P_FONT_TITLE],0xf56fbc:p[P_FONT_DRAW],0xe38db0:p[P_WIDTH],0xe38dac:p[P_HEIGHT],0xe38da8:p[P_OX],0xe38da4:p[P_OY],0xe38d9c:p[P_WIDTH],0xe38d98:p[P_HEIGHT],0xe39130:0,0xe39134:0,0xe39138:p[P_WIDTH],0xe3913c:p[P_HEIGHT],0xe38dcc:1,0xe94d9c:p[P_LAYOUT_READY],0xe94d98:p[P_HOVER],0x605a44:0,0xf535c4:p[P_INDEX],0xf535c8:p[P_INPUT_COUNT],0xd3b198:p[P_PRIMARY],0xd3b194:p[P_SECONDARY],0x7056c8:0,0x6057e8:p[P_STARTED],0x70570c:p[P_API_SAVED]}
 for a,v in vals.items():put(a,v)
 put(BASE+0x904,1 if p[P_TEXT]==2 else 0,1)
 for a,v in [(0xe38dda,p[P_UI]),(0xe38dca,0),(0xf53ffc,p[P_MOVIE_UI]),(0xd3b43d,p[P_CLOSE]),(0xf535c0,p[P_LATCH])]:put(a,v,1)
 for a in range(0xe38dec,0xe38e15):put(a,0,1)
 for a,v in [(0xe38e34,0xfffffc18),(0xe38e30,0xfffffc18),(0xe92e0c,0),(0xe36c8c,0)]:put(a,v)
 for i in range(256):
  for offset,value in [(0xe94618,0),(0xe9461c,0),(0xe94620,0),(0xe94624,p[P_BUTTON_READY])]:put(offset+i*16,value)
 if p[P_NODES]&1:
  node=BASE+0x800;span=signed(p[P_FINAL_WIDTH]+20&0xffffffff);x=signed(div(signed(p[P_WIDTH]),2)-div(span,2)+8-signed(p[P_OX]));y=signed(div(signed(p[P_HEIGHT]),2)-45-signed(p[P_OY]));values=[x,y,p[P_NODE_X],p[P_NODE_Y],7,2,0]
  for i,v in enumerate(values):put(node+i*4,v)
  put(0xe94620+84*16,node)
 if p[P_NODES]&2:
  for i in range(3):
   node=BASE+0x840+i*32
   for offset,value in [(0,i+1),(8,100+i*140),(12,100),(16,120),(20,24),(24,node+32 if i<2 else 0)]:put(node+offset,value)
  put(0xe9461c+84*16,BASE+0x840)
def call(entry,argv=(),local=False,window=False):
 global previous
 previous=None;sp=STACK+0x100000;saved={UC_X86_REG_EBX:p[P_SEED]^0x12345678,UC_X86_REG_ESI:p[P_SEED]^0x789abcde,UC_X86_REG_EDI:p[P_SEED]^0xabcdef12,UC_X86_REG_EBP:p[P_SEED]^0x2468ace0}
 put(sp,STOP)
 for i,v in enumerate(argv):put(sp+4+i*4,v)
 for r,v in saved.items():vm.reg_write(r,v)
 vm.reg_write(UC_X86_REG_EAX,p[P_SEED]);vm.reg_write(UC_X86_REG_ECX,p[P_SEED]^0x44556677);vm.reg_write(UC_X86_REG_EDX,p[P_SEED]^0x99aabbcc);vm.reg_write(UC_X86_REG_EFLAGS,0x202);vm.reg_write(UC_X86_REG_ESP,sp)
 if local:prepare_local(sp,entry);entry=vm.reg_read(UC_X86_REG_EIP)
 try:vm.emu_start(entry,STOP,count=300000)
 except Exception as error:
  ip=vm.reg_read(UC_X86_REG_EIP);raise AssertionError({'entry':hex(entry),'EIP':hex(ip),'ESP':hex(vm.reg_read(UC_X86_REG_ESP)),'EAX':hex(vm.reg_read(UC_X86_REG_EAX)),'ECX':hex(vm.reg_read(UC_X86_REG_ECX)),'EDX':hex(vm.reg_read(UC_X86_REG_EDX)),'ESI':hex(vm.reg_read(UC_X86_REG_ESI)),'last_events':events[-2:],'instruction':core.get(ip),'frames':frames,'peeks':peeks,'gets':gets}) from error
 assert vm.reg_read(UC_X86_REG_EIP)==STOP and vm.reg_read(UC_X86_REG_ESP)==sp+4+(16 if window else 0),hex(entry)
 for r,v in saved.items():assert vm.reg_read(r)==v
 return vm.reg_read(UC_X86_REG_EAX)
def native(case):
 setup(case);op=p[P_OP];result=0
 if op in [0,1,2]:result=call({0:0x538940,1:0x538c80,2:0x538cd0}[op],[TITLE,BODY,p[P_FLAGS]])
 elif op==3:call(0x539170)
 elif op==4:call(0x539220,[p[P_IGNORED]])
 elif op==5:result=call(0x414cd0,[p[P_HWND],0x100,0x1b,0],window=True)
 elif op==6:call(0x538920)
 elif op==7:call(0x4dd7b8,local=True)
 else:raise AssertionError(op)
 if p[P_AFTER_DISPLAY] and get(0xe38dda,1)==0x3e and not get(0xd3b43d,1):call(0x4cf210);call(0x4dd2aa,local=True)
 return {'result':result,'modal':last_modal,'state':b''.join(bytes(vm.mem_read(a,n)) for a,n in regions).hex(),'events':list(events)}
rng=random.Random(0x538cd0);cases=[]
def add(op=0,**kw):
 p=[0]*FIELDS
 defaults={'OP':op,'SEED':rng.getrandbits(32),'FLAGS':4,'UI':84 if op in [0,1,7] else 62,'ACTIVE':0,'READY':1,'HANDLE':0x12345678,'KIND':2,'MOVIE_UI':12,'CLOSE':0,'ICONIC':0,'FOREGROUND':1,'HWND':0x89abcdef,'BODY_WIDTH':200,'TITLE_WIDTH':240,'FINAL_WIDTH':240,'FONT_BODY':0x11223344,'FONT_TITLE':0x55667788,'FONT_DRAW':0x99aabbcc,'WIDTH':640,'HEIGHT':480,'OX':0,'OY':0,'LAYOUT_READY':1,'HOVER':0,'BUTTON_READY':1,'LATCH':0,'INDEX':0,'TARGET':1,'CURSOR_FAIL':0,'CLICK_AT':2,'SCRIPT':0,'PEEK_RESULT':1,'GET_RESULT':1,'POST_RESULT':1,'WAIT_RESULT':1,'MUTATIONS':0,'MUT_INDEX':0,'MUT_MODE':84,'MUT_COUNT':2,'MEMORY_RESULT':0xffffffff,'SMACK_RESULT':1,'CONTEXT_RESULT':1,'IGNORED':1,'NODES':0,'NODE_X':20,'NODE_Y':30,'API_SAVED':0xaabbcc0c,'SMACK_NEW_HANDLE':0x87654321,'SMACK_NEW_KIND':2,'INPUT_COUNT':2,'PRIMARY':0x12345678,'SECONDARY':0xabcdef01,'STARTED':0,'NOW':10001,'AFTER_DISPLAY':0,'TRACE':0,'TEXT':0,'WIDTH_ALT':800,'HEIGHT_ALT':600,'FONT_ALT':0xabcdef12,'OX_ALT':31,'OY_ALT':17,'EXTRA':0x76543210}
 defaults.update({k.upper():v for k,v in kw.items()})
 for k,v in defaults.items():p[globals()['P_'+k]]=v&0xffffffff
 cases.append(p);return p
def count(flags):return {1:2,2:3,3:3,4:2,5:2}.get(flags&15,1)
for flags in range(16):
 for index,latch,relation,nodes in itertools.product(range(count(flags)),[0,1,255],[0,1,2],[0,1,2,3]):add(flags=flags,index=index,latch=latch,body_width=[240,200,300][relation],title_width=240,nodes=nodes)
for width,x,y,flags in itertools.product([-40,-21,-20,-19,-1,0,1,15,16,239,240,241,8192],[0,192],[0,144],[0,2,4]):add(final_width=width,ox=x,oy=y,flags=flags,latch=0)
for mask,flags in itertools.product([8,16,24,32,64,96,128,8192,16384,16408],[0,2,4]):add(flags=flags,mutations=mask,latch=1,index=0,mut_index=0)
for active,ready,handle,kind in itertools.product([0,1,256,0xffffffff],[0,1,256],[0,0x12345678,0xffffffff],[0,2,258]):add(3,active=active,ready=ready,handle=handle,kind=kind)
for mask,available in itertools.product(range(8),[0,0xffffffff,0x7fffffff]):add(3,active=1,ready=0,mutations=mask,memory_result=available)
for kind,mode,arg in itertools.product([0,1,2,258,0xffffffff],[0,12,255],[0,1,0xffffffff]):add(4,kind=kind,movie_ui=mode,ignored=arg)
for script,flags,close,target in itertools.product(range(9),[0,2,4],[0,1,2],range(3)):
 if target<count(flags):add(2,script=script,flags=flags,close=close,target=target,iconic=256 if script==3 else 0,foreground=0 if script==7 else 1,get_result=0xffffffff if script==2 else 1)
for flags in [0,1,2,3,4,5,0x10000004]:
 for index in range(count(flags)):add(1,flags=flags,latch=1,index=index,target=0,mutations=256,mut_index=(index+1)%count(flags))
for ui,target,script,nodes,active in itertools.product([1,12,62,255],[0,1],[0,1,2,3,4,5,7,8],[0,3],[0,256]):add(5,ui=ui,target=target,script=script,nodes=nodes,active=active,get_result=0xffffffff if script==2 else 1,iconic=256 if script==3 else 0,foreground=0 if script==7 else 1)
for mask,script,post in itertools.product([512,1024,1536,2048,2560],[1,2,5,8],[0,1]):add(2,mutations=mask,script=script,post_result=post,get_result=0xffffffff if script==2 else 1)
for target,now,started in itertools.product([0,1],[10000,10001,0x80000000],[0,0x7fffd8ef]):add(5,target=target,ui=62,after_display=1,now=now,started=started)
for primary in [0,1,2,256,0x12345678,0xffffffff]:add(6,primary=primary)
for n in [0,1,2,3,0x80000000,0xffffffff]:add(7,input_count=n,ui=84)
for script,ret,peek,post in itertools.product([2,5,8],[1,256,0x80000000,0xffffffff],[1,256,0x80000000],[0,0x80000000]):add(2,script=script,get_result=ret,peek_result=peek,post_result=post,close=1)
for script,wait,icon in itertools.product([3,7],[0,1,0x80000000],[0,256]):add(2,script=script,wait_result=wait,iconic=icon,foreground=0 if script==7 else 1)
for result,ctx in itertools.product([0,0x80000000],[0,0x80000000]):add(3,active=1,ready=0,smack_result=result,context_result=ctx)
for ui,video,trace in itertools.product([1,62,84],[0,1,2],[0,0xffff]):add(5,ui=ui,text=video,trace=trace)
for mode in [84,170]:add(2,script=5,mutations=1024,mut_mode=mode,target=1)
for nodes in [0,1,2,3]:add(1,nodes=nodes,button_ready=0,latch=0,target=0)
for hover in [0,2,4]:add(2,button_ready=0,hover=hover,nodes=0,target=1)
for flags in [0,2,4]:add(1,flags=flags,latch=1,index=0,mutations=64,mut_count=0xffffffff)
selected=cases[args.start:args.start+args.limit if args.limit else None];expected=[native(c) for c in selected]
assert all(c[P_CURSOR_FAIL]==0 for c in selected),'GetCursorPos FALSE is outside the unchanged native poll contract.'
required=[0x538940,0x538c80,0x538cd0,0x539170,0x539220,0x538920,0x4dd7b8];owned=set().union(*(sets[e] for e in required));owned_edges={(a,b) for a,z in branches.items() if a in owned for b in z};excluded={(0x538b8d,0x538c46)};feasible=owned_edges-excluded;complete=owned<=executed and feasible<=edges and jump_indices==set(range(5))
product=ROOT if args.production else PHASE
source_paths=[product/p for p in ['src/unit_02464.c','src/unit_02465.c','src/unit_02466.c','src/unit_02470.c','src/unit_02472.c','runtime/menu_keyboard_modal.c','runtime/menu_keyboard_modal_platform.c','include/menu_keyboard_modal.h','tests/menu_keyboard_modal.c','tests/menu_keyboard_modal_case.h']]+[ROOT/p for p in ['runtime/window_messages.c','runtime/text_selection.c','src/unit_01672.c','src/unit_01673.c','src/unit_00416.c','src/unit_00417.c','src/unit_01836.c','src/unit_01845.c','src/unit_01846.c','src/unit_01856.c','src/unit_01857.c','src/unit_01858.c','src/unit_01730.c','src/unit_01731.c','src/unit_01767.c','src/unit_01768.c','runtime/display_confirmation.c','src/unit_00025.c','src/unit_00026.c','include/display_confirmation.h','include/selection_popup.h','include/menu_input.h','include/menu_layout.h','include/window_messages.h','include/text_selection.h','include/media_bindings.h','include/restore_types.h','include/standalone_data.h']]+[PHASE/'build/CMakeLists.txt',Path(__file__).resolve()]
source_paths += [product/'tests/menu_keyboard_modal_keyboard_context.c',ROOT/'src/unit_00274.c',ROOT/'include/campaign_profile.h',ROOT/'include/campaign_parser.h',ROOT/'include/video_player.h']
source_paths += [PHASE/'tools/extract_menu_keyboard_modal_native.py',PHASE/'tools/qualify_menu_keyboard_modal_native.py']
report={'format':1,'generated_at':datetime.now(timezone.utc).isoformat(),'production':args.production,'passed':False,'complete_case_set':not args.limit and not args.start,'checked':len(selected),'native_sha256':digest,'compiled_C_path':args.exe.resolve().relative_to(ROOT).as_posix(),'compiled_C_sha256':sha(args.exe.resolve()),'source_hashes':{x.relative_to(ROOT).as_posix():sha(x) for x in source_paths},'native_exports':{native_file.relative_to(ROOT).as_posix():sha(native_file),display_file.relative_to(ROOT).as_posix():sha(display_file),'metadata/high-scores-input-native.json':sha(ROOT/'metadata/high-scores-input-native.json')},'qualified_controller':{'complete':complete,'entries':[f'{a:08x}' for a in required],'instructions':len(owned),'executed':len(owned&executed),'missing_instructions':[f'{a:08x}' for a in sorted(owned-executed)],'conditional_edges':len(owned_edges),'feasible_edges':len(feasible),'feasible_executed':len(feasible&edges),'missing_edges':[[f'{a:08x}',f'{b:08x}'] for a,b in sorted(feasible-edges)],'dispatch_indices':sorted(jump_indices),'excluded_edges':[['00538b8d','00538c46']]},'context_coverage':{f'{a:08x}':{'executed':len(z&executed),'total':len(z)} for a,z in sets.items() if a not in required},'watched_memory':{'regions':regions,'bytes_per_case':sum(n for a,n in regions),'all_native_writes_guarded':True},'variants':{},'original_game_process_executed':False,'original_machine_code_fallback_used':False,'limitations':['FiniteWin32queue andsuccessfulGetCursorPosPOINT are exactAPI boundaries; no liveGUI/messagepost/cursor/mediaDDraw action occurs. Currentfrozen1768Cguardreturns onFALSE whereasnative151ignoresBOOL andconsumesundefinedPOINT; thatfailure-domain difference is disclosed, not silently claimed equivalent.','Actualtext_selection,mousepoll/math/reset,layoutlookup/hover andbutton416/417 algorithms execute. Original4D8310sharedprelude is qualified; extractedselected54case executes with preparednativeframe/EBP=0. Othermodes remainexplicitdeeperdispatcher boundaries.','Initialrender4D71C0(0),present4D0280(0),cursor4CF460(0),context/file/log/memorystatus/SmackClose andfont/pixelcalls are exacttypeddeeperboundaries. Physicalgraphics/cursor/media correctness is not claimed. Existing1769BYTEcursor-handle gap remainsoutsideownedscope.','Rendererlocalcount is1/2/3 for everylowflag/default. ItsJLEzero-loopedge538B8D→538C46 is excluded onlyunderprivatecaller-stack/noalias ABI: intervening4CC720 gets globaltitle/body pointers andnumericargs, no pointertocountslot. Globalcount/selection/font/offset mutations are exercised independently.','Nativeinvalidselectedindex reads uninitialized/out-of-rangeprivate resultstack. NewC diagnosesinvalidindex andaborts outsidevalidnativeindex0..count-1; no defaultsuccess/FFchoice is invented. Nativepending-1 remains onlywhenBYTElatchiszero.','Fonts/text/panelartwork remainqualified, withfullDWORDarguments/WORDrecord writes/consumedmetricreturns compared. Validstrings andacycliclayoutnodes are required. Currenthelpergeometrycomposition uses boundeddisplay/font dimensions.']}
out=ROOT/'metadata/menu-keyboard-modal-emulation.json' if args.production else PHASE/'metadata/menu-keyboard-modal-emulation.json'
report['native_exports'][qualification_file.relative_to(ROOT).as_posix()]=sha(qualification_file)
report['outcomes']={str(op):{'cases':sum(c[P_OP]==op for c in selected),'returns':sorted({r['result'] for c,r in zip(selected,expected) if c[P_OP]==op}),'modal_returns':sorted({r['modal'] for c,r in zip(selected,expected) if c[P_OP]==op})} for op in range(8)}
report['native_ABI_guards']={'all_top_level_native_calls_check_stack_return':True,'native_callee_saved_registers':['EBX','ESI','EDI','EBP'],'stdcall_window_cleanup':16,'deep_endpoints_clobber':['EAX return','ECX','EDX'],'message_bytes':28,'ordered_endpoint_arguments_and_snapshot_DWORDs':32,'full_DWORD_callback_identity':True,'own_stackless_functions_void_returns':'Native volatile EAX values are unconsumed by all qualified callers; only consumed returns are compared.'}
report['limitations'][4]='Invalid selected indices read uninitialized or out-of-range private native result-stack storage. The candidate diagnoses invalid full DWORD indices and calls ExitProcess(3), outside valid native index 0..count-1. Native pending -1 remains only when the BYTE latch is zero.'
try:
 if report['complete_case_set']:assert complete,report['qualified_controller']
 for label,path in [('Debug',args.exe),('O2',args.optimized_exe)]:
  path=path.resolve();reject=subprocess.run([str(path),'--invalid-index'],capture_output=True,text=True,timeout=10);assert reject.returncode==3 and '268435457 of 2' in reject.stderr,(label,reject.returncode,reject.stderr)
  proc=subprocess.Popen([str(path),'--oracle-batch'],stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True)
  try:
   hello=json.loads(proc.stdout.readline());assert hello=={'fields':FIELDS,'message_size':28},hello
   for i,(case,want) in enumerate(zip(selected,expected)):
    proc.stdin.write(' '.join(map(str,case))+'\n');proc.stdin.flush();line=proc.stdout.readline();assert line,(label,i,proc.poll(),proc.stderr.read());actual=json.loads(line)
    if actual!=want:
     field=next(k for k in want if actual[k]!=want[k]);detail={'variant':label,'case':args.start+i,'input':dict(zip(names,case)),'field':field,'C':actual[field],'native':want[field]};(PHASE/'metadata/last-mismatch.json').write_text(json.dumps(detail,indent=2));raise AssertionError({k:v for k,v in detail.items() if k not in ['C','native']})
   proc.stdin.close();proc.wait(timeout=30);assert proc.returncode==0,proc.stderr.read()
  finally:
   if proc.poll() is None:proc.kill();proc.wait()
  report['variants'][label]={'path':path.relative_to(ROOT).as_posix(),'sha256':sha(path),'checked':len(selected),'invalid_full_DWORD_index_rejected_exit':reject.returncode}
 assert all(sha(ROOT/path)==value for path,value in report['source_hashes'].items()),'Sources changed during the comparison.'
 assert all(sha(ROOT/path)==value for path,value in report['native_exports'].items()),'Native evidence changed during the comparison.'
 report['comparisons']=len(selected)*2;report['passed']=True
finally:out.write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({'passed':report['passed'],'checked':len(selected),'comparisons':report.get('comparisons'),'qualified_controller':report['qualified_controller']}))
