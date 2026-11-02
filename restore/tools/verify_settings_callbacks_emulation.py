"""Complete settings callbacks, speed publisher and sound-member composition versus pinned x86."""
from pathlib import Path
import argparse,hashlib,itertools,json,re,struct,subprocess,sys,time
ROOT=Path(__file__).resolve().parent.parent;PHASE=ROOT/'artifacts/settings_callbacks_phase'
parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--exe',type=Path);parser.add_argument('--optimized-exe',type=Path);parser.add_argument('--production',action='store_true');parser.add_argument('--limit',type=int);parser.add_argument('--start',type=int,default=0);parser.add_argument('--case',type=Path);args=parser.parse_args()
sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_BLOCK,UC_HOOK_CODE,UC_HOOK_MEM_WRITE
from unicorn.x86_const import *
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
sha=lambda b:hashlib.sha256(b).hexdigest();MASK=0xffffffff;SOUND=0xe92bf8;ALT=0x31000000;STOP=0x33000000;STACK=0x34000000
schema=(PHASE/'tests/settings_case.h').read_text();names=[x.lower() for x in re.findall(r'\bP_([A-Z0-9_]+)\b',schema.split('enum {',1)[1].split('};',1)[0]) if x!='FIELDS'];FIELDS=len(names);globals().update({'P_'+x.upper():j for j,x in enumerate(names)})
image=(ROOT/'build/smoke-game/Pharaoh.exe').read_bytes();digest=sha(image);assert digest=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
u16=lambda p:struct.unpack_from('<H',image,p)[0];u32=lambda p:struct.unpack_from('<I',image,p)[0];pe=u32(60);opt=pe+24;sec=opt+u16(pe+20);base=u32(opt+28);size=u32(opt+56)
vm=Uc(UC_ARCH_X86,UC_MODE_32);vm.mem_map(base,(size+4095)&~4095)
for j in range(u16(pe+6)):
 q=sec+j*40;vm.mem_write(base+u32(q+12),image[u32(q+20):u32(q+20)+u32(q+16)])
for a,n in [(ALT,4096),(STOP,4096),(STACK,0x100000)]:vm.mem_map(a,n)
def read(a,n=4):return int.from_bytes(vm.mem_read(a&MASK,n),'little')
def write(a,v,n=4):vm.mem_write(a&MASK,(v&((1<<(n*8))-1)).to_bytes(n,'little'))
functions={}
for filename in ['all15-native.json','audio-native.json']:
 p=json.loads((PHASE/filename).read_text());assert p['native_sha256']==digest
 for a,f in p['functions'].items():functions[int(a,16)]=f['instructions']
caller=json.loads((PHASE/'speed-caller-native.json').read_text());assert not caller['assembly']['has_more'];functions[0x416ba0]=caller['assembly']['items']
instructions={int(i['address'],16):i for body in functions.values() for i in body}
for a,i in instructions.items():assert bytes(vm.mem_read(a,len(bytes.fromhex(i['bytes']))))==bytes.fromhex(i['bytes'])
entries=[0x402c10,0x402c40,0x402cc0,0x402e50,0x402f70,0x403060,0x403130,0x4031a0,0x403210,0x403270,0x4032f0,0x403320,0x403390,0x4ea760,0x51f780,0x416ba0,0x55f32f,0x55f38d,0x55f3eb,0x55f561,0x55f639]
members=set(entries[16:]);owned_functions=set(entries[:14]+[0x416ba0,0x55f32f,0x55f3eb]);owned={int(i['address'],16) for f in owned_functions for i in functions[f]}
cap=Cs(CS_ARCH_X86,CS_MODE_32);branches={}
for a,i in instructions.items():
 decoded=next(cap.disasm(bytes.fromhex(i['bytes']),a))
 if decoded.mnemonic.startswith('j') and decoded.mnemonic!='jmp':branches[a]=(a+decoded.size,int(decoded.op_str,16))
assert bytes(vm.mem_read(0x402cc5,10))==bytes.fromhex('4883f80c0f8791000000')
dispatch_raw=bytes(vm.mem_read(0x402d64,13*4));dispatch_targets=set(struct.unpack('<13I',dispatch_raw));assert len(dispatch_targets)==13
regions=[(0x605790,0x2c0),(0xd3b188,0x30),(0xe38db0,0x300),(0x6de500,0x30),(0xe39130,0x30),(0xe92be8,0x224),(ALT,0x224),(0xea186c,4*0x49d8+0x30),(0xeaac18,0x30),(0x5d1950,0x40),(0x7848e8,0x30)]
deep={0x540c90:0,0x540ed0:0,0x541160:0,0x4bc720:0,0x4cde40:2};imports={0x56f174:4,0x56f298:1,0x56f29c:2,0x56f2d0:1};IAT_NAMES={0x56f174:'PostMessageA',0x56f298:'_AIL_stream_status@4',0x56f29c:'_AIL_set_stream_volume@8',0x56f2d0:'_AIL_close_stream@4'}
# Authenticate the original import names rather than assuming the IAT slots.
def cstring(a):
 result=bytearray()
 while read(a,1):result.append(read(a,1));a+=1
 return result.decode()
q=base+u32(opt+104);found={}
while any(vm.mem_read(q,20)):
 ilt,stamp,forwarder,dll,iat=struct.unpack('<IIIII',vm.mem_read(q,20));j=0
 while read(base+(ilt or iat)+j*4):
  value=read(base+(ilt or iat)+j*4);found[base+iat+j*4]=cstring(base+value+2) if not value&0x80000000 else str(value&65535);j+=1
 q+=20
assert all(found[a]==name for a,name in IAT_NAMES.items())
import_sites={}
for a,i in instructions.items():
 d=next(cap.disasm(bytes.fromhex(i['bytes']),a))
 if d.mnemonic=='call' and d.op_str.startswith('dword ptr [0x'):
  slot=int(d.op_str.split('[0x')[1].split(']')[0],16);assert slot in imports;import_sites[a]=(slot,d.size)
events=[];writes=[];frames=[];hits=set();edges=set();mutated=set();controlled_reached=set();seen=0;active_self=SOUND;params=None;last_block=None;block_cache={};fill_cache={}
def flags():return read(0xe38e63,1)|(read(0xe38e64,1)<<8)|(read(0xe38e65,1)<<16)|(read(0xe38ec0,1)<<24)
def put_flags(v):
 for a,shift in [(0xe38e63,0),(0xe38e64,8),(0xe38e65,16),(0xe38ec0,24)]:write(a,v>>shift,1)
def observe(fn,phase,values,result=0):
 global seen
 p=params;s=active_self;e=[fn,phase]+list(values)+[0]*(4-len(values))+[result]
 e +=[read(a,n) for a,n in [(0xd3b198,4),(0xe38dda,1),(0xe38dca,1),(0xe39080,4),(0xeaac28,4),(0xe38ec8,4),(0xe38ecc,4),(0xe38ec4,4),(0xe38ed0,4)]]
 e +=[flags(),read(0xe38e6c),read(0xe38e70),read(0x605a3c),read(0x6de510),read(0xe38ef0)]+[read(s+off) for off in [0x200,0x1d8,0x1dc,0x1e0,0x14c,0x154,0x158]]+[read(0x7848f8),read(0xe39140),s];assert len(e)==32;events.append(e)
 if fn==p[P_MFN] and phase==p[P_MPHASE]:
  seen+=1
  if seen==p[P_MOCC]:
   n=p[P_MBITS];mutated.add((fn,phase))
   if n&1:write(0xd3b198,p[P_NEWSELECTED])
   if n&2:write(0xe38dda,p[P_NEWMODE],1);write(0xe38dca,p[P_NEWDIRTY],1)
   if n&4:write(0xe39080,p[P_NEWDIFFICULTY])
   if n&8:write(0xeaac28,p[P_NEWPLAYER])
   if n&16:
    for a,value in [(0xe38ec8,p[P_NEWVOL0]),(0xe38ecc,p[P_NEWVOL1]),(0xe38ec4,p[P_NEWVOL2]),(0xe38ed0,p[P_NEWVOL3])]:write(a,value)
    put_flags(p[P_NEWFLAGS])
   if n&32:
    for off,value in [(0x200,p[P_NEWDISABLED]),(0x1d8,p[P_NEWSTREAM0]),(0x1dc,p[P_NEWSTREAM1]),(0x1e0,p[P_NEWSTREAM2])]:write(s+off,value)
   if n&64:
    for off,value in [(0x14c,p[P_NEWVOL0]),(0x154,p[P_NEWVOL1]),(0x158,p[P_NEWVOL3])]:write(s+off,value)
   if n&128:write(0x6de510,p[P_NEWPOPUP])
   if n&256:write(0xe38e6c,p[P_NEWSPEED]);write(0xe38e70,p[P_NEWSCROLL]);write(0x605a3c,p[P_NEWLOCK])
   if n&512:
    for a,value in [(0x6057bc,p[P_NEWVOL0]),(0x6057b4,p[P_NEWVOL1]),(0x6057ac,p[P_NEWVOL2]),(0x6057a4,p[P_NEWVOL3])]:write(a,value)
    for a,shift in [(0x6057c0,8),(0x6057b8,16),(0x6057b0,0),(0x6057a8,24)]:write(a,p[P_NEWFLAGS]>>shift,1)
   if n&1024:write(0x7848f8,p[P_NEWQUERYARG])
   if n&2048:write(0xe39140,p[P_NEWWINDOW])
def enter(cpu,a,n,user):
 global active_self
 sp=cpu.reg_read(UC_X86_REG_ESP);old=active_self
 if a in members:
  active_self=cpu.reg_read(UC_X86_REG_ECX);v=[active_self]+[read(sp+4+j*4) for j in range(2 if a==0x55f639 else 1)]
 else:v=[read(sp+4)] if a==0x416ba0 else []
 frames.append((a,sp,v,old));observe(a,0,v)
def leave(cpu,a,n,user):
 global active_self
 sp=cpu.reg_read(UC_X86_REG_ESP);assert frames and frames[-1][1]==sp,'Native RET frame'
 # The six-instruction416BA0 entry tail-jumps to403320 with the same frame.
 while frames and frames[-1][1]==sp:
  fn,saved,v,old=frames.pop();observe(fn,1,v);active_self=old
def endpoint(cpu,a,n,user):
 sp=cpu.reg_read(UC_X86_REG_ESP);v=[read(sp+4+j*4) for j in range(deep[a])];controlled_reached.add(a);observe(a,0,v);r=params[P_RESULT];observe(a,1,v,r if a==0x4cde40 else 0);cpu.reg_write(UC_X86_REG_EAX,r);cpu.reg_write(UC_X86_REG_EIP,read(sp));cpu.reg_write(UC_X86_REG_ESP,sp+4)
def imported(cpu,a,n,user):
 slot,length=import_sites[a];sp=cpu.reg_read(UC_X86_REG_ESP);v=[read(sp+j*4) for j in range(imports[slot])];controlled_reached.add(slot);observe(slot,0,v);r=params[P_STATUS] if slot==0x56f298 else params[P_RESULT];observe(slot,1,v,r);cpu.reg_write(UC_X86_REG_EAX,r);cpu.reg_write(UC_X86_REG_ESP,sp+imports[slot]*4);cpu.reg_write(UC_X86_REG_EIP,a+length)
def block(cpu,a,n,user):
 global last_block
 if a in deep:return
 assert a in instructions,'Native escaped '+hex(a)
 if last_block is not None:edges.add((last_block,a))
 if(a,n) not in block_cache:block_cache[a,n]=sorted(x for x in instructions if a<=x<a+n)
 values=block_cache[a,n];hits.update(values);last_block=values[-1]
def memory_write(cpu,access,a,n,value,user):
 if STACK<=a and a+n<=STACK+0x100000:return
 assert any(start<=a and a+n<=start+length for start,length in regions),'Unwatched native write '+hex(a)
 if frames and frames[-1][0] in owned_functions:writes.append([a,n,value&((1<<(n*8))-1)])
vm.hook_add(UC_HOOK_BLOCK,block);vm.hook_add(UC_HOOK_MEM_WRITE,memory_write)
for fn,body in functions.items():
 vm.hook_add(UC_HOOK_CODE,enter,begin=fn,end=fn)
 for i in body:
  if i['instruction'].lower().startswith('ret'):
   a=int(i['address'],16);vm.hook_add(UC_HOOK_CODE,leave,begin=a,end=a)
for a in deep:vm.hook_add(UC_HOOK_CODE,endpoint,begin=a,end=a)
for a in import_sites:vm.hook_add(UC_HOOK_CODE,imported,begin=a,end=a)
def native(p):
 global params,seen,active_self,last_block
 params=p;seen=0;active_self=SOUND;events.clear();writes.clear();frames.clear()
 for j,(a,n) in enumerate(regions):
  value=(p[P_SEED]+j*29)&255
  if(n,value) not in fill_cache:fill_cache[n,value]=bytes([value])*n
  vm.mem_write(a,fill_cache[n,value])
 for a,value,n in [(0xd3b198,p[P_SELECTED],4),(0xe38dda,p[P_MODE],1),(0xe38dca,p[P_DIRTY],1),(0x6de510,p[P_POPUP],4),(0xe38ef0,p[P_SAVED],4),(0xe38ec8,p[P_VOL0],4),(0xe38ecc,p[P_VOL1],4),(0xe38ec4,p[P_VOL2],4),(0xe38ed0,p[P_VOL3],4),(0xe38e6c,p[P_SPEED],4),(0xe38e70,p[P_SCROLL],4),(0x605a3c,p[P_LOCK],4),(0xe39080,p[P_DIFFICULTY],4),(0xeaac28,p[P_PLAYER],4),(0x7848f8,p[P_QUERYARG],4),(0xe39140,p[P_WINDOW],4),(0x6057bc,p[P_DEF0],4),(0x6057b4,p[P_DEF1],4),(0x6057ac,p[P_DEF2],4),(0x6057a4,p[P_DEF3],4),(0x6057c0,p[P_DEFFLAGS]>>8,1),(0x6057b8,p[P_DEFFLAGS]>>16,1),(0x6057b0,p[P_DEFFLAGS],1),(0x6057a8,p[P_DEFFLAGS]>>24,1),(0x6057a0,p[P_DEFSPEED],4),(0x60579c,p[P_DEFSCROLL],4)]:write(a,value,n)
 put_flags(p[P_FLAGS])
 for j,a in enumerate([SOUND,ALT+16]):
  for off,value in [(0x200,p[P_DISABLED]),(0x1d8,p[P_STREAM0]),(0x1dc,p[P_STREAM1]),(0x1e0,p[P_STREAM2]),(0x14c,0xabc00000+j),(0x154,0xabc00001+j),(0x158,0xabc00002+j)]:write(a+off,value)
 for j in range(5):write(0x5d1960+j*4,10007+j*10009)
 fn=entries[p[P_OP]];top=[p[P_ARGUMENT],p[P_VOL0]] if fn==0x55f639 else [p[P_ARGUMENT]] if fn in members or fn==0x416ba0 else []
 saved=[(UC_X86_REG_EBX,0x11111111),(UC_X86_REG_EBP,0x22222222),(UC_X86_REG_ESI,0x33333333),(UC_X86_REG_EDI,0x44444444)]
 for repeat in range(p[P_REPEATS]):
  sp=STACK+0xf0000;vm.mem_write(sp,b'\xcc'*0x200);write(sp,STOP)
  for j,value in enumerate(top):write(sp+4+j*4,value)
  for reg,value in saved:vm.reg_write(reg,value)
  vm.reg_write(UC_X86_REG_ECX,ALT+16 if p[P_SELF] else SOUND);vm.reg_write(UC_X86_REG_ESP,sp);vm.reg_write(UC_X86_REG_EFLAGS,2);last_block=None
  vm.emu_start(fn,STOP,count=100000)
  assert vm.reg_read(UC_X86_REG_EIP)==STOP and not frames,'Native budget/frame'
  assert vm.reg_read(UC_X86_REG_ESP)==sp+4+(len(top)*4 if fn in members else 0),'Native stack cleanup'
  assert all(vm.reg_read(reg)==value for reg,value in saved),'Native callee-saved registers'
  assert bytes(vm.mem_read(sp+4+len(top)*4,128))==b'\xcc'*128,'Native argument guard'
 h=hashlib.sha256()
 for a,n in regions:h.update(struct.pack('<II',a,n));h.update(vm.mem_read(a,n))
 return {'memory':h.hexdigest(),'events':events.copy(),'writes':writes.copy()}
default=[0]*FIELDS
defaults={'seed':1,'mode':11,'dirty':0xa5,'popup':0x89abcdef,'saved':0x12345678,'vol0':50,'vol1':51,'vol2':52,'vol3':53,'flags':0x01010101,'speed':90,'scroll':50,'difficulty':2,'player':1,'queryarg':0x80000001,'window':0x10203040,'def0':60,'def1':61,'def2':62,'def3':63,'defflags':0x01010101,'defspeed':80,'defscroll':40,'stream0':0x11112222,'stream1':0x33334444,'stream2':0x55556666,'status':4,'result':0xa5a50000,'repeats':1,'mocc':1,'newselected':2,'newmode':53,'newdirty':0x66,'newdifficulty':4,'newplayer':3,'newvol0':70,'newvol1':71,'newvol2':72,'newvol3':73,'newflags':0xff00ff00,'newdisabled':1,'newstream0':0,'newstream1':0,'newstream2':0,'newpopup':0x76543210,'newspeed':20,'newscroll':30,'newlock':1,'newqueryarg':0x7fffffff,'newwindow':0x50607080}
for key,value in defaults.items():default[names.index(key)]=value&MASK
cases=[]
def case(**kw):
 p=default.copy()
 for key,value in kw.items():p[names.index(key)]=value&MASK
 cases.append(tuple(p))
for op in range(16):case(op=op)
for op in range(16,21):case(op=op,argument=1)
inventory=json.loads((ROOT/'metadata/settings-callback-inventory.json').read_text())
for table in inventory['tables']:
 for button in table['items']:
  token=int(button['callbacks'][0],16);case(op=entries.index(token),selected=button['primary'],mode=table['mode'])
selectors=[0,1,2,3,4,5,12,13,14,15,127,128,255,256,257,0x10001,0x1000001,0x7fffffff,0x80000000,MASK]
for op,selected in itertools.product(range(15),selectors):case(op=op,selected=selected,mode=53)
for selected,popup in itertools.product(range(256),[0,0x12345678,MASK]):case(op=2,selected=selected,popup=popup)
for selected in range(1,14):case(op=2,selected=selected,popup=0xa5a5a5a5,repeats=2)
for selected,flag in itertools.product(range(1,5),range(256)):case(op=5,selected=selected,flags=flag*0x01010101)
values=[0,1,2,9,10,11,49,50,89,90,99,100,101,127,128,255,256,65535,MASK,0xfffffffe,0xfffffff5,0x7fffffff,0x7ffffffe,0x80000000,0x80000001]+list(range(0x7ffffff6,0x80000000))+list(range(0x80000000,0x8000000b))
for op,field in [(6,'vol0'),(7,'vol1'),(8,'vol2'),(9,'vol3'),(11,'speed'),(12,'scroll')]:
 for selected,value in itertools.product([0,1,2,256,MASK],values):case(op=op,selected=selected,**{field:value})
for lock,speed,selected in itertools.product([0,1,256,0x1000001,0x80000001,MASK],[0,9,10,95,100,0x7fffffff,0x80000000],[0,1,256]):case(op=11,lock=lock,speed=speed,selected=selected)
for mode,argument,lock,speed in itertools.product([0,1,2,10,12,53,255],[0,1,2,256,0x1000001,MASK],[0,1,0x1000001],[9,10,95,100,0x7fffffff]):case(op=15,mode=mode,argument=argument,lock=lock,speed=speed)
for selected,difficulty,mode,player,result in itertools.product([0,1,256,MASK],[0,1,2,3,4,5,MASK,0x7fffffff,0x80000000],[1,53],range(4),[0,1,MASK,0x80000000]):case(op=3,selected=selected,difficulty=difficulty,mode=mode,player=player,result=result)
for flag in range(256):case(op=4,defflags=flag*0x01010101,def0=0x80000000+flag,def1=0x7fffffff-flag,def2=flag*0x1010101,def3=MASK-flag)
for selected,result in itertools.product([0,1,2,3,4,256,0x1000001,MASK],[0,1,MASK]):case(op=1,selected=selected,result=result)
for result in [0,1,MASK,0x80000000]:case(op=0,result=result)
for op,self,disabled,present,status,argument in itertools.product([16,17,18],[0,1],[0,1,256,0x80000000],[0,1],[0,3,4,5,0x104,0x80000004,MASK],[0,1,100,101,MASK,0x80000000,0x7fffffff]):
 case(op=op,self=self,disabled=disabled,status=status,argument=argument,stream0=0x11112222 if present else 0,stream1=0x33334444 if present else 0,stream2=0x55556666 if present else 0)
for op,self,disabled,mask,channel in itertools.product([19,20],[0,1],[0,1,256,0x80000000],range(8),range(3)):
 case(op=op,self=self,disabled=disabled,argument=channel,vol0=0x80000001,stream0=0x11112222 if mask&1 else 0,stream1=0x33334444 if mask&2 else 0,stream2=0x55556666 if mask&4 else 0)
for op in [4,5,6,7,8,9,10,11,12,13,14]:case(op=op,selected=1,repeats=4)
for channel in range(3):case(op=19,argument=channel,repeats=3)
def mutate(fn,phase,bits=1,**kw):case(mfn=fn,mphase=phase,mbits=bits,**kw)
for phase in [0,1]:
 for op,fn in enumerate(entries):mutate(fn,phase,op=op,argument=1,mode=53)
 for fn in [0x540c90,0x540ed0,0x541160,0x4bc720,0x4cde40]:mutate(fn,phase,op=3,mode=53)
 for fn,op in [(0x56f174,0),(0x56f298,16),(0x56f29c,20),(0x56f2d0,19)]:mutate(fn,phase,op=op,argument=1)
 for fn in [0x540c90,0x540ed0,0x541160,0x4bc720]:
  mutate(fn,phase,bits=2|4|8,op=3,mode=1,newmode=53)
  mutate(fn,phase,bits=2|4|8,op=3,mode=53,newmode=1)
 mutate(0x4cde40,phase,bits=4|8|1024,op=3,mode=53)
 for op in [4,6,7]:mutate(0x55f639,phase,bits=16,op=op,selected=1)
 for selected,next_selected in [(1,2),(1,3),(1,4),(2,4),(2,3)]:mutate(0x55f561,phase,bits=1,op=5,selected=selected,newselected=next_selected)
 for op,self in itertools.product([16,17,18],[0,1]):mutate(0x56f298,phase,bits=32|64,op=op,self=self,argument=45,status=4)
 for channel in range(3):mutate(0x56f2d0,phase,bits=32,op=19,argument=channel,newstream0=0x77770000,newstream1=0x77770001,newstream2=0x77770002)
 for op in [6,7,9,16,17,18]:mutate(0x56f29c,phase,bits=16|64,op=op,selected=1,argument=40)
 for op in [0,1]:mutate(0x56f174,phase,bits=1|2|2048,op=op,selected=1,result=0)
 mutate(0x403320,phase,bits=1|256,op=15,mode=1,argument=0,newselected=0x1000001,newlock=0)
 mutate(0x416ba0,phase,bits=256,op=15,mode=1,argument=0)
 mutate(0x402f70,phase,bits=16|512,op=4)
 mutate(0x55f561,phase,bits=16|512,op=4,defflags=0)
if args.case:cases=[tuple(json.loads(args.case.read_text())['case'])]
elif args.start:cases=cases[args.start:]
if args.limit:cases=cases[:args.limit]
exe=args.exe or ROOT/'build/agent-settings-callbacks/Debug/settings_callbacks_verify.exe';optimized=args.optimized_exe or(None if args.exe else ROOT/'build/agent-settings-callbacks/Release/settings_callbacks_verify.exe')
variants=[(exe,False),(exe,True)]+([(optimized,False),(optimized,True)] if optimized else [])
oracles=[subprocess.Popen([str(path),'--oracle-plain-batch' if plain else '--oracle-batch'],cwd=ROOT,stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True) for path,plain in variants]
report={'format':1,'native_sha256':digest,'original_game_executed':False,'passed':False,'checked':0,'compiled_C_sha256':sha(exe.read_bytes()),'method':'Complete native settings callbacks, speed publisher and sound-member algorithms against compiled traced and plain C. Full watched bytes/guards, owned store order, ordered meaningful calls/state and results, declared helper mutations and native/C saved registers/stack compared.','limitations':['PostMessageA delivery and physical media playback are outside deterministic equivalence. The C uses the existing real platform provider and media bindings; test providers control only the named external calls.','Four difficulty recalculation helpers and4CDE40 remain explicit controls. This proves the controller ordering and reload contract, not their deeper algorithms or complete settings-page drawing/input.','Sound members use explicit C receiver arguments; native ECX/RET4/RET8 and C cdecl adapters are checked separately. Composed accepted561/639 are exercised only for channels0..2, exactly the callback domain. Incidental callback/member EAX is not a semantic result.']}
started=time.monotonic();checked=0;p=None
try:
 assert all(json.loads(o.stdout.readline())=={'fields':FIELDS} for o in oracles)
 for p in cases:
  expected=native(p)
  for number,(o,(path,plain)) in enumerate(zip(oracles,variants)):
   o.stdin.write(' '.join(map(str,p))+'\n');o.stdin.flush();line=o.stdout.readline();assert line,'C exited: '+o.stderr.read();actual=json.loads(line);want=dict(expected,writes=[]) if plain else expected
   if actual!=want:(PHASE/'last-difference.json').write_text(json.dumps({'case':p,'oracle':number,'C':actual,'native':want},indent=2)+'\n')
   assert actual==want,repr({'oracle':number,'memory':[actual['memory'],want['memory']],'event_counts':[len(actual['events']),len(want['events'])],'write_counts':[len(actual['writes']),len(want['writes'])],'first_event_difference':[(j,a,b) for j,(a,b) in enumerate(zip(actual['events'],want['events'])) if a!=b][:1],'first_write_difference':[(j,a,b) for j,(a,b) in enumerate(zip(actual['writes'],want['writes'])) if a!=b][:1]})
  checked+=1
  if checked%500==0:print(json.dumps({'checked':checked,'total':len(cases),'elapsed_seconds':round(time.monotonic()-started,1)}),flush=True)
 if not(args.limit or args.start or args.case):
  assert set(instructions)<=hits,'Uncovered instructions '+repr([hex(a) for a in sorted(set(instructions)-hits)])
  missing=[(a,t) for a,ts in branches.items() for t in ts if(a,t) not in edges];assert not missing,'Uncovered conditional outcomes '+repr(missing)
  assert dispatch_targets<={b for a,b in edges if a==0x402ccf},'Uncovered popup dispatch'
 report.update(passed=True,checked=checked)
except Exception as error:report.update(checked=checked,case=p,error=str(error),native_eip=hex(vm.reg_read(UC_X86_REG_EIP)))
finally:
 for o in oracles:o.stdin.close();o.wait(timeout=10)
prefix='' if args.production else 'artifacts/settings_callbacks_phase/'
sources=[prefix+s for s in ['runtime/settings_callbacks.c','runtime/settings_callbacks_audio.c','runtime/mission_menu.c','src/unit_00027.c','src/unit_00191.c','include/settings_callbacks.h']]+['src/unit_02705.c','src/unit_02708.c','src/unit_02711.c','src/unit_02351.c','runtime/sound_streams.c','include/sound_streams.h','include/sound_initialization.h','include/media_bindings.h','include/menu_input.h','include/standalone_data.h','include/restore_types.h','metadata/settings-callback-inventory.json','tools/verify_settings_callbacks_emulation.py']+['artifacts/settings_callbacks_phase/'+s for s in ['tests/settings_case.h','tests/settings_callbacks.c','tests/settings_actual_trace.c','tests/settings_actual_plain.c','project/CMakeLists.txt','make_test_aliases.py','inspect_native.py','all15-native.json','audio-native.json','speed-caller-native.json','retained_native.json']]
missing_edges=[(a,t) for a,ts in branches.items() for t in ts if(a,t) not in edges];dispatch_seen={b for a,b in edges if a==0x402ccf}
report.update(checked=checked,plain_checked=checked,comparison_checked=checked if optimized else 0,comparison_C_sha256=sha(optimized.read_bytes()) if optimized else None,case_selection={'limit':args.limit,'start':args.start,'single_case':bool(args.case)},elapsed_seconds=round(time.monotonic()-started,2),native_instruction_coverage=[len(hits),len(instructions)],owned_instruction_coverage=[len(hits&owned),len(owned)],native_conditional_outcomes=[len(branches)*2-len(missing_edges),len(branches)*2],native_uncovered_instructions=[hex(a) for a in sorted(set(instructions)-hits)],missing_conditional_outcomes=[[hex(a),hex(b)] for a,b in missing_edges],popup_dispatch={'guard':'unsigned(selected DWORD-1)<=12','table_sha256':sha(dispatch_raw),'targets':[len(dispatch_targets&dispatch_seen),13],'uncovered':[hex(a) for a in sorted(dispatch_targets-dispatch_seen)]},function_instruction_coverage={hex(fn):[len(hits&{int(i['address'],16) for i in body}),len(body)] for fn,body in functions.items()},controlled_endpoints=[hex(a) for a in list(deep)+list(imports)],reached_controls=[hex(a) for a in sorted(controlled_reached)],mutated_helper_phases=[[hex(a),phase] for a,phase in sorted(mutated)],source_hashes={s:sha((ROOT/s).read_bytes()) for s in sources})
(ROOT/'metadata/settings-callbacks-emulation.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v for k,v in report.items() if k not in ['source_hashes','native_uncovered_instructions','missing_conditional_outcomes']},indent=2));sys.exit(0 if report['passed'] else 1)
