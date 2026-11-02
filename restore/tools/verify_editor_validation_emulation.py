"""Complete editor-validation graph against pinned x86 and compiled traced/plain C."""
from pathlib import Path
import argparse, hashlib, itertools, json, re, struct, subprocess, sys, time
ROOT=Path(__file__).resolve().parent.parent; PHASE=ROOT/'artifacts/editor_validation_phase'
parser=argparse.ArgumentParser(description=__doc__)
parser.add_argument('--exe',type=Path); parser.add_argument('--optimized-exe',type=Path)
parser.add_argument('--production',action='store_true'); parser.add_argument('--limit',type=int)
parser.add_argument('--start',type=int,default=0); parser.add_argument('--case',type=Path)
args=parser.parse_args(); sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_BLOCK,UC_HOOK_CODE,UC_HOOK_MEM_WRITE,UC_HOOK_MEM_READ
from unicorn.x86_const import *
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
MASK=0xffffffff; ALT=0x31000000; STOP=0x33000000; STACK=0x34000000; CELLS=51984
CLASS_BASE=0x5e4c10; CLASS_SIZE=1100*48+64; sha=lambda b:hashlib.sha256(b).hexdigest()
schema=(PHASE/'tests/editor_case.h').read_text()
names=[x.lower() for x in re.findall(r'\bP_([A-Z0-9_]+)\b',schema.split('enum {',1)[1].split('};',1)[0]) if x!='FIELDS']
FIELDS=len(names); globals().update({'P_'+x.upper():j for j,x in enumerate(names)})
image=(ROOT/'build/smoke-game/Pharaoh.exe').read_bytes(); digest=sha(image)
assert digest=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
u16=lambda p:struct.unpack_from('<H',image,p)[0]; u32=lambda p:struct.unpack_from('<I',image,p)[0]
pe=u32(60); opt=pe+24; sec=opt+u16(pe+20); base=u32(opt+28); size=u32(opt+56)
vm=Uc(UC_ARCH_X86,UC_MODE_32); vm.mem_map(base,(size+4095)&~4095)
for j in range(u16(pe+6)):
 q=sec+j*40; vm.mem_write(base+u32(q+12),image[u32(q+20):u32(q+20)+u32(q+16)])
for a,n in [(ALT,0x10000),(STOP,4096),(STACK,0x100000)]:vm.mem_map(a,n)
def read(a,n=4):return int.from_bytes(vm.mem_read(a&MASK,n),'little')
def write(a,v,n=4):vm.mem_write(a&MASK,(v&((1<<(8*n))-1)).to_bytes(n,'little'))
def signed(v,n=32):return (v&((1<<(n-1))-1))-(v&(1<<(n-1)))
def cstring(a):
 b=bytearray()
 for j in range(256):
  value=read(a+j,1)
  if not value:return bytes(b)
  b.append(value)
 raise AssertionError('Unterminated bounded string')
def string_hash(a):
 h=2166136261
 for value in cstring(a):h=((h^value)*16777619)&MASK
 return h
functions={}
for filename in ['native.json','helper-native.json','selector-native.json']:
 data=json.loads((PHASE/filename).read_text()); assert data['native_sha256']==digest
 for a,f in data['functions'].items():
  if 'assembly' in f:assert not f['assembly']['has_more']
  functions[int(a,16)]=f.get('instructions',f.get('assembly',{}).get('items'))
instructions={int(i['address'],16):i for body in functions.values() for i in body}
for a,i in instructions.items():assert bytes(vm.mem_read(a,len(bytes.fromhex(i['bytes']))))==bytes.fromhex(i['bytes'])
entries=[0x409830,0x4ec360,0x4095c0,0x409650,0x4096e0,0x409760,0x4097c0,0x409820,0x409620,0x4ec410,0x443710,0x4cdff0,0x4ce3a0]
byte_functions=set(entries[1:9]); raw_functions={0x4ec410,0x443710}
owned_functions=set(entries[:7]+[0x409620,0x4ec410]); owned={int(i['address'],16) for fn in owned_functions for i in functions[fn]}
cap=Cs(CS_ARCH_X86,CS_MODE_32); branches={}; decoded={}
for a,i in instructions.items():
 d=next(cap.disasm(bytes.fromhex(i['bytes']),a)); decoded[a]=d
 if d.mnemonic.startswith('j') and d.mnemonic!='jmp':branches[a]=(a+d.size,int(d.op_str,16))
regions=[(0x784b40,0x200),(0xbca2c8,0x40),(0xaf597c,CELLS*4+32),(CLASS_BASE,CLASS_SIZE),(0x77f738,200*98+32),(0x6057b0,0x2b0),(0xe20750,0x30),(0xe20980,0x30),(0xe38dc0,0x100),(0xd3b1a0,0x30),(0xf53f20,0x30),(0xe36ee0,0x30),(ALT,0x10000)]
points=json.loads((PHASE/'tests/points.json').read_text())
filenames=[b'Alpha.map',b'Beta.extra.map',b'.hidden.map',b'no_extension',b'Gamma.MAP',b'zeta.map',b'  spaced.name.map',b'Last.']
profiles=[b'Restore',b'Alpha.Beta',b'',b'City']; deep={0x4cc110,0x4cc160,0x538cd0,0x5169c0,0x41c740,0x41ca10}
events=[]; writes=[]; frames=[]; hits=set(); edges=set(); mutated=set(); controlled_reached=set()
seen=0; params=None; last_block=None; block_cache={}; fill_cache={}; pattern_cache={}
def coordinate(q):return signed(read(q['x'],q['width']),q['width']*8),signed(read(q['y'],q['width']),q['width']*8)
def active(q,x,y):return x>0 or y>0 if q['group'] in [3,4] else x>=0 and y>=0
def cell(base,x,y):return (base+x+y*228)&MASK
def set_coordinate(index,x,y):
 q=points[index]; write(q['x'],x,q['width']); write(q['y'],y,q['width'])
def allowed_address(scene,variant,mode):
 a=((0x5e4c24+(scene&255)*48) if mode else (0x5e4cb4+(signed(variant&65535,16)+(scene&255)*2)*48))&MASK
 assert CLASS_BASE<=a and a+4<=CLASS_BASE+CLASS_SIZE,hex(a)
 return a
def record(index,typ,enabled,shape,extra,codes8,codes6):
 assert index<200; a=0x77f748+index*98; vm.mem_write(a,b'\xa6'*98)
 for off,v in [(0,typ),(1,enabled),(0x18,shape),(0x1a,extra)]:write(a+off,v,1)
 for j in range(14):
  v=((codes8>>(j*4)) if j<8 else (codes6>>((j-8)*4)))&15
  write(a+0x1e+j,params[P_BADCODE] if v==15 else v,1)
def observe(fn,phase,values,result=0):
 global seen
 p=params; q=points[p[P_FOCUS]]; x,y=coordinate(q); tile=cell(read(0xbca2d8),x,y) if active(q,x,y) else MASK
 e=[fn,phase]+list(values)+[0]*(4-len(values))+[result]
 e +=[read(a,n) for a,n in [(0xbca2d8,4),(0x784cb0,4),(0x784b94,2),(0x605a3c,4),(0xe38dda,1),(0xe38dca,1),(0xe20998,4),(0xd3b1c0,1),(0xf53f2c,4),(0xe20768,4),(0xe20764,4),(0x6057d0,4),(0x6059f8,4),(0x6059f4,4),(0x6059f0,4)]]
 e +=[x&MASK,y&MASK,read(0xaf598c+tile*4) if tile<CELLS else 0,read(allowed_address(read(0x784cb0),read(0x784b94,2),0)),read(allowed_address(read(0x784cb0),read(0x784b94,2),1)),read(0xd3b1b3,1),read(0xe36efc),read(0x6057cc),read(0x6057d8),tile]
 assert len(e)==32; events.append(e)
 if fn==p[P_MFN] and phase==p[P_MPHASE]:
  seen+=1
  if seen==p[P_MOCC]:
   n=p[P_MBITS];mutated.add((fn,phase))
   if n&1:write(0xbca2d8,p[P_NEWBASE])
   if n&2:set_coordinate(p[P_NEWPOINT],p[P_NEWX],p[P_NEWY])
   if n&4:
    q=points[p[P_NEWPOINT]]; x,y=coordinate(q);tile=cell(read(0xbca2d8),x,y);assert tile<CELLS;write(0xaf598c+tile*4,p[P_NEWTERRAIN])
   if n&8:
    write(0x784cb0,p[P_NEWSCENE]);write(0x784b94,p[P_NEWVARIANT],2);write(0x605a3c,p[P_NEWEDITOR])
    write(allowed_address(p[P_NEWSCENE],p[P_NEWVARIANT],0),p[P_NEWALLOW0]);write(allowed_address(p[P_NEWSCENE],p[P_NEWVARIANT],1),p[P_NEWALLOW1])
   if n&16:write(0xe20768,p[P_NEWTITLE]);write(0xe20764,p[P_NEWBODY])
   if n&32:
    for a,v,w in [(0xe38dda,p[P_NEWMODE],1),(0xe38dca,p[P_NEWDIRTY],1),(0xd3b1c0,p[P_NEWFLAG],1),(0xe20998,p[P_NEWVALUE],4),(0xf53f2c,p[P_NEWCOUNTER],4)]:write(a,v,w)
   if n&64:record(p[P_NEWRECORD],p[P_NEWREC0],p[P_NEWREC1],p[P_NEWSHAPE],p[P_NEWEXTRAPRESENT],p[P_NEWCODES8],p[P_NEWCODES6])
   if n&128:assert signed(p[P_NEWFILECOUNT])<=8;write(0x6057d0,p[P_NEWFILECOUNT])
def semantic(fn,r):
 if fn in byte_functions:
  r&=255;assert r in [0,1]
 elif fn not in raw_functions:r=0
 if fn==0x443710:assert r<=8
 return r
def enter(cpu,a,n,user):
 sp=cpu.reg_read(UC_X86_REG_ESP)
 values=[read(sp+4),read(sp+8)&255] if a in [0x4ec410,0x4cdff0] else [read(sp+4)] if a in [0x409620,0x4ce3a0] else []
 frames.append((a,sp,values));observe(a,0,values)
def leave(cpu,a,n,user):
 sp=cpu.reg_read(UC_X86_REG_ESP);assert frames and frames[-1][1]==sp,'Native RET frame'
 fn,old,values=frames.pop();observe(fn,1,values,semantic(fn,cpu.reg_read(UC_X86_REG_EAX)))
def endpoint(cpu,a,n,user):
 sp=cpu.reg_read(UC_X86_REG_ESP);p=params;cleanup=0;controlled_reached.add(a)
 if a in [0x4cc110,0x4cc160]:
  values=[read(sp+4),read(sp+8)];assert values[0]==5 and values[1]<256;observe(a,0,values)
  write(0xe20768 if a==0x4cc110 else 0xe20764,ALT+0x6000+values[1]*64);r=0;observe(a,1,values)
 elif a==0x538cd0:
  values=[read(sp+4),read(sp+8),read(sp+12)];assert all(ALT<=v<ALT+0x10000 for v in values[:2]);observe(a,0,values)
  string_hash(values[0]);string_hash(values[1]);r=p[P_RESULT];observe(a,1,values,r)
 elif a==0x5169c0:
  values=[];observe(a,0,values);r=ALT+0x3000;observe(a,1,values,r)
 elif a==0x41c740:
  self=cpu.reg_read(UC_X86_REG_ECX);pattern=read(sp+4);flag=read(sp+8)&255;assert self==0x6057c8 and signed(p[P_FILECOUNT])<=8
  values=[self,string_hash(pattern),flag,0x576260 if pattern==0x576260 else 0];observe(a,0,values)
  for off,v in [(4,ALT+0x1000),(8,p[P_FILECOUNT]),(12,8),(16,ALT+0x2000),(20,512),(24,p[P_FILECOUNT]*64 if signed(p[P_FILECOUNT])>0 else 0)]:write(self+off,v)
  for j,name in enumerate(filenames):write(ALT+0x1000+j*4,j*64);vm.mem_write(ALT+0x2000+j*64,name+b'\0')
  r=p[P_ENUMRET]&255;observe(a,1,values,r);cleanup=8
 elif a==0x41ca10:
  values=[cpu.reg_read(UC_X86_REG_ECX),read(sp+4)];assert values==[0x6057c8,0x562370];observe(a,0,values);r=0;observe(a,1,values);cleanup=4
 else:raise AssertionError(hex(a))
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
 if frames and frames[-1][0] in owned_functions:writes.append([a,n,value&((1<<(8*n))-1)])
def memory_read(cpu,access,a,n,value,user):
 if STACK<=a and a+n<=STACK+0x100000:return
 assert any(s<=a and a+n<=s+z for s,z in regions) or 0x5761c4<=a and a+n<=0x5761e0 or 0x576260<=a and a+n<=0x57626b,'Unwatched native read '+hex(a)
vm.hook_add(UC_HOOK_BLOCK,block);vm.hook_add(UC_HOOK_MEM_WRITE,memory_write);vm.hook_add(UC_HOOK_MEM_READ,memory_read)
for fn,body in functions.items():
 vm.hook_add(UC_HOOK_CODE,enter,begin=fn,end=fn)
 for i in body:
  a=int(i['address'],16)
  if decoded[a].mnemonic.startswith('ret'):vm.hook_add(UC_HOOK_CODE,leave,begin=a,end=a)
for a in deep:vm.hook_add(UC_HOOK_CODE,endpoint,begin=a,end=a)
def native(p):
 global params,seen,last_block
 params=p;seen=0;events.clear();writes.clear();frames.clear()
 for j,(a,n) in enumerate(regions):
  value=(p[P_SEED]+j*29)&255
  if(n,value) not in fill_cache:fill_cache[n,value]=bytes([value])*n
  vm.mem_write(a,fill_cache[n,value])
 if p[P_SEED] not in pattern_cache:
  terrain=struct.pack('<%dI'%CELLS,*[((j*0x9e3779b9)^((j>>3)*0x85ebca6b)^(p[P_SEED]*0x1234567))&MASK for j in range(CELLS)])
  classification=bytearray([(p[P_SEED]+3*29)&255]*CLASS_SIZE)
  for j in range(1100):struct.pack_into('<I',classification,20+j*48,((j*0x9e3779b9)^(p[P_SEED]*0x13579bd))&MASK)
  pattern_cache[p[P_SEED]]=(terrain,bytes(classification))
 terrain,classification=pattern_cache[p[P_SEED]];vm.mem_write(0xaf598c,terrain);vm.mem_write(CLASS_BASE,classification);vm.mem_write(0x77f748,bytes(200*98))
 for j in range(36):
  bit=(p[P_ACTIVE0]>>j)&1 if j<32 else(p[P_ACTIVE1]>>(j-32))&1;set_coordinate(j,3+j*3 if bit else MASK,10 if bit else MASK)
 if p[P_OVERRIDE]!=MASK:set_coordinate(p[P_OVERRIDE],p[P_X],p[P_Y])
 for a,v,n in [(0xbca2d8,p[P_BASE],4),(0x784cb0,p[P_SCENE],4),(0x784b94,p[P_VARIANT],2),(0x605a3c,p[P_EDITOR],4)]:write(a,v,n)
 write(allowed_address(p[P_SCENE],p[P_VARIANT],0),p[P_ALLOW0]);write(allowed_address(p[P_SCENE],p[P_VARIANT],1),p[P_ALLOW1])
 for j,q in enumerate(points):
  x,y=coordinate(q)
  if not active(q,x,y):continue
  g=q['group'];t=p[P_T0] if g==0 else p[P_T1] if g==1 else p[P_TWATER] if g in [2,4,6] else p[P_TPASS]
  bit=(p[P_FAIL0]>>j)&1 if j<32 else(p[P_FAIL1]>>(j-32))&1
  if bit:t=p[P_BADWATER] if g in [2,4,6] else p[P_BADCLASS]
  index=cell(p[P_BASE],x,y);assert index<CELLS,(j,x,y,index);write(0xaf598c+index*4,t)
  other=cell(p[P_NEWBASE],x,y)
  if other<CELLS:write(0xaf598c+other*4,p[P_NEWTERRAIN])
 if p[P_RECORD]!=MASK:record(p[P_RECORD],p[P_REC0],p[P_REC1],p[P_SHAPE],p[P_EXTRAPRESENT],p[P_CODES8],p[P_CODES6])
 for j in range(8):
  if p[P_SPREAD]&(1<<j):record(23*j%200,1,1,0,0,j+1,0)
 for a,v,n in [(0xe38dda,p[P_MODE],1),(0xe38dca,p[P_DIRTY],1),(0xd3b1c0,p[P_FLAG],1),(0xe20998,p[P_VALUE],4),(0xf53f2c,p[P_COUNTER],4),(0xe20768,p[P_TITLE],4),(0xe20764,p[P_BODY],4)]:write(a,v,n)
 assert signed(p[P_FILECOUNT])<=8 and p[P_PROFILE]<4 and p[P_FILENAME]<8
 for a,v in [(0x6057cc,ALT+0x1000),(0x6057d0,p[P_FILECOUNT]),(0x6057d4,8),(0x6057d8,ALT+0x2000),(0x6057dc,512),(0x6057e0,p[P_FILECOUNT]*64 if signed(p[P_FILECOUNT])>0 else 0)]:write(a,v)
 for j,name in enumerate(filenames):write(ALT+0x1000+j*4,j*64);vm.mem_write(ALT+0x2000+j*64,name+b'\0')
 vm.mem_write(ALT+0x3000,profiles[p[P_PROFILE]]+b'\0')
 for j in range(256):vm.mem_write(ALT+0x6000+j*64,('line-5-%d'%j).encode()+b'\0')
 vm.mem_write(ALT+0xe000,b'mutated-title\0');vm.mem_write(ALT+0xe100,b'mutated-body\0')
 fn=entries[p[P_OP]];top=[p[P_TERRAIN],p[P_KIND]] if fn==0x4ec410 else[p[P_TERRAIN]] if fn==0x409620 else[p[P_ARGUMENT],p[P_MAPS]] if fn==0x4cdff0 else[ALT+0x2000+p[P_FILENAME]*64] if fn==0x4ce3a0 else[]
 saved=[(UC_X86_REG_EBX,0x11111111),(UC_X86_REG_EBP,0x22222222),(UC_X86_REG_ESI,0x33333333),(UC_X86_REG_EDI,0x44444444)]
 for repeat in range(p[P_REPEATS]):
  sp=STACK+0xf0000;vm.mem_write(sp,b'\xcc'*0x200);write(sp,STOP)
  for j,value in enumerate(top):write(sp+4+j*4,value)
  for reg,value in saved:vm.reg_write(reg,value)
  for reg,value in [(UC_X86_REG_EAX,0xa5a55a5a),(UC_X86_REG_ECX,0x12345678),(UC_X86_REG_EDX,0x87654321),(UC_X86_REG_ESP,sp),(UC_X86_REG_EFLAGS,2)]:vm.reg_write(reg,value)
  last_block=None;vm.emu_start(fn,STOP,count=100000)
  assert vm.reg_read(UC_X86_REG_EIP)==STOP and not frames,'Native budget/frame'
  assert vm.reg_read(UC_X86_REG_ESP)==sp+4,'Native stack cleanup'
  assert all(vm.reg_read(reg)==v for reg,v in saved),'Native saved registers'
  assert bytes(vm.mem_read(sp+4+len(top)*4,128))==b'\xcc'*128,'Native stack argument guard'
  result=semantic(fn,vm.reg_read(UC_X86_REG_EAX))
 h=hashlib.sha256()
 for a,n in regions:h.update(struct.pack('<II',a,n));h.update(vm.mem_read(a,n))
 return {'return':result,'memory':h.hexdigest(),'events':events.copy(),'writes':writes.copy()}

default=[0]*FIELDS
defaults={'seed':1,'override':MASK,'record':MASK,'scene':2,'newscene':3,'newbase':2280,'newterrain':1,'twater':4,'badclass':4,'rec0':1,'rec1':1,'badcode':255,'mode':53,'dirty':0xa5,'flag':0x5a,'value':0x12345678,'counter':0x87654321,'title':ALT+0xe000,'body':ALT+0xe100,'maps':1,'repeats':1,'mocc':1,'newx':99,'newy':10,'newtitle':ALT+0xe000,'newbody':ALT+0xe100,'newmode':14,'newdirty':0x66,'newflag':0xff,'newvalue':0x89abcdef,'newcounter':0x76543210,'newrec0':1,'newrec1':1}
for key,value in defaults.items():default[names.index(key)]=value&MASK
cases=[]
def case(**kw):
 p=default.copy()
 for key,value in kw.items():p[names.index(key)]=value&MASK
 cases.append(tuple(p))
def point_mask(index,fail=False):return {('fail0' if fail else 'active0') if index<32 else('fail1' if fail else 'active1'):1<<(index if index<32 else index-32)}
def point_case(index,**kw):case(focus=index,**point_mask(index),**kw)
# Complete predicates and every validation failure are exercised with actual terrain and records.
for op in range(13):case(op=op)
for index in range(36):
 for bad in [False,True]:point_case(index,op=0,**(point_mask(index,True) if bad else{}))
 for op in [1,2,3,4,5,6]:point_case(index,op=op)
case(op=0,spread=31);case(op=0,spread=255);case(op=0,active0=MASK,active1=15)
for index,q in enumerate(points):
 op=[1,1,2,3,4,5,6][q['group']]
 for x,y,basevalue in [(-1,-1,0),(-1,0,0),(0,-1,0),(0,0,0),(1,0,0),(0,1,0),(-1,1,500),(1,-1,500),(227,0,0),(0,227,0),(32767,0,0),(1,-32768,228*32768),(32767,1,0),(-32768,-32768,0)]:
  case(op=op,override=index,x=x,y=y,base=basevalue,focus=index)
# Signed DWORD coordinates additionally reach wrapped arithmetic with valid mapped cell results.
for index in range(4,8):
 for x,y,b in [(0x7fffffff,0,0x80000001),(1,0x7fffffff,228),(0x10000,1,0xffff0000),(1,0x10000,(-0x10000*228)&MASK)]:case(op=1,override=index,x=x,y=y,base=b,focus=index)
terrain_values=[0,1,4,0x40000,0x10000000,0x20000000,0x26351277,0x3038d022,MASK]+[1<<j for j in range(32)]
for t in terrain_values:case(op=8,terrain=t)
for terrain,allowed,editor,kind in itertools.product(terrain_values,[0,1,4,0x40000,0x40004,0x26351277,MASK],[0,1,256,0x1000001,MASK],[0,1]):
 case(op=9,terrain=terrain,allow0=allowed,allow1=allowed,editor=editor,kind=kind)
for scene,variant in [(0,0),(1,-1),(127,-254),(128,-256),(255,0),(255,256),(255,257),(255,-256),(0x100,256),(0x10001,0),(0x800000ff,257),(MASK,0)]:
 for kind,terrain in itertools.product([0,1,2,255,256,257,0xffffff00,MASK],[0,4,0x40000,0x12345678]):case(op=9,scene=scene,variant=variant,kind=kind,terrain=terrain,allow0=0x40004,allow1=4,editor=1)
for index in [0,3,4,7,16,17,20,27]:
 op=[1,1,2,3,4,5,6][points[index]['group']]
 for terrain in terrain_values:point_case(index,op=op,t0=terrain,t1=terrain,tpass=terrain)
# Counter uses all200 actual98-byte records; both slot counts and every BYTE shape/code.
for index in range(200):case(op=10,record=index,codes8=0x87654321)
for shape,extra in itertools.product(range(256),[0,1,255]):case(op=10,record=199,shape=shape,extrapresent=extra,codes8=0,codes6=0x876543)
for code in range(256):case(op=10,record=17,codes8=0xf,codes6=0xffffff,badcode=code,shape=1,extrapresent=1)
for typ,enabled in itertools.product([0,1,2,127,128,255,256,257],[0,1,255,256,257]):case(op=10,record=1,rec0=typ,rec1=enabled,codes8=0x87654321)
for spread in range(256):case(op=7,spread=spread)
for op,count,maps in itertools.product([0,11],[-1,0,1,2,8],[0,1,255,256]):case(op=op,filecount=count,maps=maps,profile=1,enumret=0xff)
for profile,filename in itertools.product(range(4),range(8)):case(op=11,maps=0,profile=profile,filecount=filename+1);case(op=12,filename=filename)
def mutate(fn,phase,bits=1,**kw):case(mfn=fn,mphase=phase,mbits=bits,**kw)
for phase in [0,1]:
 for op,fn in enumerate(entries):mutate(fn,phase,op=op)
 for fn in [0x4cc110,0x4cc160,0x538cd0]:mutate(fn,phase,bits=16|32,op=0,active0=1,fail0=1,result=0x80000001)
 for fn in [0x5169c0,0x41c740,0x41ca10]:mutate(fn,phase,bits=32,op=11,maps=0,filecount=3)
 for fn,op,index,next_index in [(0x4ec410,1,0,1),(0x4ec410,1,4,5),(0x409620,3,16,17),(0x409620,5,20,21)]:
  mask=(1<<index)|(1<<next_index);mutate(fn,phase,bits=1|2|4|8,op=op,active0=mask,newpoint=next_index,focus=next_index,newallow0=4,newallow1=4,neweditor=1,newterrain=4)
 for fn in [0x4ec360,0x4095c0,0x409650,0x4096e0,0x409760,0x4097c0,0x409820,0x443710]:mutate(fn,phase,bits=64,op=0,newrecord=199,newcodes8=0x87654321)
 for fn in [0x4cc110,0x4cc160]:mutate(fn,phase,bits=16,op=0,active0=1,fail0=1,newtitle=ALT+0xe100,newbody=ALT+0xe000)
 # List count changes only at explicit enumeration/sort boundaries; never fabricate a bad trim pointer.
 for fn in [0x41c740,0x41ca10]:mutate(fn,phase,bits=128,op=11,maps=1,filecount=3,newfilecount=8)
for op in [0,1,3,5,7,9,10,11,12]:case(op=op,repeats=3,filecount=3,codes8=0x87654321,record=0)
if args.case:cases=[tuple(json.loads(args.case.read_text())['case'])]
elif args.start:cases=cases[args.start:]
if args.limit:cases=cases[:args.limit]
exe=args.exe or ROOT/'build/agent-editor-validation/Debug/editor_validation_verify.exe'
optimized=args.optimized_exe or(None if args.exe else ROOT/'build/agent-editor-validation/Release/editor_validation_verify.exe')
variants=[(exe,False),(exe,True)]+([(optimized,False),(optimized,True)] if optimized else[])
oracles=[subprocess.Popen([str(path),'--oracle-plain-batch' if plain else '--oracle-batch'],cwd=ROOT,stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True) for path,plain in variants]
report={'format':1,'native_sha256':digest,'original_game_executed':False,'passed':False,'checked':0,'compiled_C_sha256':sha(exe.read_bytes()),'method':'Complete editor validation, terrain classifiers and real counter/selector/trim composition against compiled traced and plain C. Full watched bytes/guards, native reads and writes, ordered calls/arguments/state/consumed results, AL/full-EAX domains, declared mutations and native/C stack/saved registers compared.','limitations':['Text lookup4CC110/4CC160, panel538CD0, profile5169C0, filesystem enumeration41C740 and sorting41CA10 are explicit typed deterministic boundaries. This proves validation/error selection and selector ordering/reloads; physical filesystem, language resources and modal rendering are outside this proof.','The selector uses a stable valid list of at most8 prepared entries. Its known invalid-index fallback is infeasible in this domain; unchanged selector is separately covered by the accepted dropdown-start-actions proof.','Helper-exit mutation cases prove boundary robustness and reload order; pure real classifiers do not ordinarily mutate coordinate/classifier state. Byte-return predicates are compared only in AL; terrain4EC410 and counter443710 preserve their complete native EAX contracts.','Incoming caller audits distinguish authenticated recovered/cached CFG calls from unowned raw E8 candidates. Zero provider xrefs does not establish absence of callers.']}
started=time.monotonic();checked=0;p=None
dead_instructions={0x4ce0ad};dead_edges={(0x443740,0x44377e),(0x44375d,0x44377e),(0x4ce090,0x4ce0ad),(0x4ce098,0x4ce0ad)}
try:
 assert all(json.loads(o.stdout.readline())=={'fields':FIELDS} for o in oracles)
 for p in cases:
  expected=native(p)
  for number,(o,(path,plain)) in enumerate(zip(oracles,variants)):
   o.stdin.write(' '.join(map(str,p))+'\n');o.stdin.flush();line=o.stdout.readline();assert line,'C exited: '+o.stderr.read();actual=json.loads(line);want=dict(expected,writes=[]) if plain else expected
   if actual!=want:(PHASE/'last-difference.json').write_text(json.dumps({'case':p,'oracle':number,'C':actual,'native':want},indent=2)+'\n')
   assert actual==want,repr({'oracle':number,'return':[actual['return'],want['return']],'memory':[actual['memory'],want['memory']],'event_counts':[len(actual['events']),len(want['events'])],'first_event_difference':[(j,a,b) for j,(a,b) in enumerate(zip(actual['events'],want['events'])) if a!=b][:1],'write_counts':[len(actual['writes']),len(want['writes'])],'first_write_difference':[(j,a,b) for j,(a,b) in enumerate(zip(actual['writes'],want['writes'])) if a!=b][:1]})
  checked+=1
  if checked%500==0:print(json.dumps({'checked':checked,'total':len(cases),'elapsed_seconds':round(time.monotonic()-started,1)}),flush=True)
 if not(args.limit or args.start or args.case):
  assert set(instructions)-dead_instructions<=hits,'Uncovered instructions '+repr([hex(a) for a in sorted(set(instructions)-dead_instructions-hits)])
  missing=[(a,t) for a,ts in branches.items() for t in ts if(a,t) not in edges and(a,t) not in dead_edges];assert not missing,'Uncovered feasible conditional outcomes '+repr([(hex(a),hex(t)) for a,t in missing])
  assert deep<=controlled_reached,'Unreached boundary';assert len(mutated)==2*(len(functions)+len(deep)),'Unreached mutation phases '+repr(mutated)
 report.update(passed=True,checked=checked)
except Exception as error:report.update(checked=checked,case=p,error=str(error),native_eip=hex(vm.reg_read(UC_X86_REG_EIP)))
finally:
 for o in oracles:
  try:o.stdin.close();o.wait(timeout=5)
  except Exception:o.kill()
prefix='' if args.production else 'artifacts/editor_validation_phase/'
sources=[prefix+s for s in ['runtime/editor_validation.c','src/unit_02079.c','include/editor_validation.h']]+['src/unit_00071.c','src/unit_00621.c','src/unit_01739.c','src/unit_01745.c','include/restore_types.h','include/standalone_data.h','include/file_list.h','include/text_selection.h','include/placement_preview.h','tools/verify_editor_validation_emulation.py']+['artifacts/editor_validation_phase/'+s for s in ['tests/editor_case.h','tests/editor_points.h','tests/points.json','tests/editor_validation.c','tests/editor_actual_trace.c','tests/editor_actual_plain.c','project/CMakeLists.txt','make_test_aliases.py','inspect_native.py','native.json','helper-native.json','selector-native.json']]+['metadata/popup-editor-xrefs-native.json','metadata/popup-editor-relative-calls-native.json']
missing_edges=[(a,t) for a,ts in branches.items() for t in ts if(a,t) not in edges]
report.update(checked=checked,plain_checked=checked,comparison_checked=checked if optimized else 0,comparison_C_sha256=sha(optimized.read_bytes()) if optimized else None,comparison_count=checked*len(variants),case_selection={'limit':args.limit,'start':args.start,'single_case':bool(args.case)},elapsed_seconds=round(time.monotonic()-started,2),native_instruction_coverage=[len(hits),len(instructions)],reachable_instruction_coverage=[len(hits-dead_instructions),len(set(instructions)-dead_instructions)],owned_instruction_coverage=[len(hits&owned),len(owned)],native_conditional_outcomes=[len(branches)*2-len(missing_edges),len(branches)*2],feasible_conditional_outcomes=[len(branches)*2-len(missing_edges),len(branches)*2-len(dead_edges)],native_uncovered_instructions=[hex(a) for a in sorted(set(instructions)-hits)],missing_conditional_outcomes=[[hex(a),hex(b)] for a,b in missing_edges],declared_dead_edges=[[hex(a),hex(b)] for a,b in sorted(dead_edges)],function_instruction_coverage={hex(fn):[len(hits&{int(i['address'],16) for i in body}),len(body)] for fn,body in functions.items()},controlled_endpoints=[hex(a) for a in sorted(deep)],reached_controls=[hex(a) for a in sorted(controlled_reached)],mutated_helper_phases=[[hex(a),phase] for a,phase in sorted(mutated)],source_hashes={s:sha((ROOT/s).read_bytes()) for s in sources})
(ROOT/'metadata/editor-validation-emulation.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({k:v for k,v in report.items() if k not in ['source_hashes','native_uncovered_instructions','missing_conditional_outcomes']},indent=2));sys.exit(0 if report['passed'] else 1)
