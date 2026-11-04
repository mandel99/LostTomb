"""Native/C dynamic-history constructors, reserve, copy and growth.

Controls only allocation/free with identical readable padded backing. The
literal copy reads destination-capacity words, including padding. This proves
normal-return behavior and allocation order; it does not certify memory safety
or C++ exception unwinding. Gameplay now shares this literal growth chain through its typed adapter.
"""
import sys,struct
sys.path.insert(0,'restore/direct_c')
from verify_options_startup import *
Q,T,BUF,BUF2=S+0x1000,S+0x1100,S+0x2000,S+0x2200
GDT,TEB=0x34000000,0x34001000
for c,u,state in machines:
 u.mem_map(GDT,0x2000)
 for selector,access in [(8,0x9b),(16,0x93)]:
  u.mem_write(GDT+selector,struct.pack('<HHBBBB',0xffff,0,0,access,0xcf,0))
 u.mem_write(GDT+24,struct.pack('<HHBBBB',0xfff,TEB&65535,(TEB>>16)&255,0x93,0x40,(TEB>>24)&255))
 u.reg_write(UC_X86_REG_GDTR,(0,GDT,0x3f,0))
 u.reg_write(UC_X86_REG_CS,8)
 for selector_reg in [UC_X86_REG_DS,UC_X86_REG_ES,UC_X86_REG_SS]:u.reg_write(selector_reg,16)
 u.reg_write(UC_X86_REG_FS,24)
def object(u,q,capacity,buffer=BUF):
 u.mem_write(q,b'\xa5'*28)
 for off,value in [(4,1),(8,capacity-1),(12,capacity),(16,capacity),(20,buffer)]:put(u,q+off,value)
def setup(c,u,state):
 reset(u,state);put(u,TEB,0xfeedbabe);events=[];bump=[S+0x10000]
 u.mem_write(S+0x10000,b'\xdb'*0x10000)
 for i in range(100):put(u,BUF+i*4,0x12340000+i)
 for i in range(100):put(u,BUF2+i*4,0x56780000+i)
 def alloc(uc,args):
  address=bump[0];bump[0]+=0x1000;events.append(('alloc',args,address));return address
 def free(uc,args):events.append(('free',args));return 0
 hook(c,state,0x561a1d,1,alloc);hook(c,state,0x561a2b,1,free)
 return events
def call(c,u,token,args=(),self=Q):
 member=token!=0x451a70
 argv=(self,*args) if c and member else args
 cleanup=4 if not c and member and token not in [0x4f6670,0x4f66f0,0x4f6760] else 0
 for r,v in zip(registers,[0x11111111,self,0x33333333,saved[0],SP,*saved[1:]]):u.reg_write(r,v)
 u.reg_write(UC_X86_REG_EFLAGS,2);put(u,SP,STOP)
 for i,value in enumerate(argv):put(u,SP+4+i*4,value)
 u.emu_start(address(c,token),STOP,count=30000000)
 assert u.reg_read(UC_X86_REG_EIP)==STOP,(hex(token),c)
 assert u.reg_read(UC_X86_REG_ESP)==SP+4+cleanup
 assert [u.reg_read(registers[i]) for i in [3,5,6,7]]==saved
 assert get(u,TEB)==0xfeedbabe
 value=u.reg_read(UC_X86_REG_EAX)
 return value&255 if token==0x4f6700 else value if token in [0x4f6670,0x4f6690,0x4f66c0,0x4f6a50] else None
cases=[]
for token in [0x4f6670,0x4f66f0,0x4f6760]:
 for buffer in [0,BUF]:cases.append((token,(),5,5,buffer,'plain'))
for token in [0x4f6690,0x4f6700]:
 for old,new in [(0,0),(5,3),(5,5),(5,8),(0,20),(5,-1),(0,0x40000001)]:
  cases.append((token,(new&0xffffffff,),old,5,BUF,'plain'))
for token in [0x4f66c0,0x4f6790]:
 for old,source in [(0,0),(2,5),(5,5),(5,2)]:
  cases.append((token,(T,),old,source,BUF,'plain'))
cases.append((0x4f6790,(Q,),5,5,BUF,'plain'))
cases.append((0x4f6790,(T,),5,5,BUF+4,'overlap'))
for old,new,head in [(5,5,1),(5,3,4),(5,15,1),(5,15,4)]:
 cases.append((0x4f6a50,(new,),old,5,BUF,str(head)))
cases.append((0x451a70,(),0,0,0,'plain'))
for token,args,old,source,buffer,kind in cases:
 results=[]
 for c,u,state in machines:
  events=setup(c,u,state);object(u,Q,old,buffer);object(u,T,source,BUF if kind=='overlap' else BUF2)
  if token==0x4f6a50:put(u,Q+4,int(kind))
  if token==0x451a70:u.mem_write(0x9783c0,b'\xa5'*280)
  value=call(c,u,token,args)
  results.append((value,events,bytes(u.mem_read(S,0x20000)),snapshot(u)))
 assert results[0]==results[1],(hex(token),args,old,source,kind,results[0][:2],results[1][:2],
  [(hex(S+i),a,b) for i,(a,b) in enumerate(zip(results[0][2],results[1][2])) if a!=b][:8],
  [(hex(0x560000+i),a,b) for i,(a,b) in enumerate(zip(results[0][3],results[1][3])) if a!=b][:8])

print('PASS',len(cases),'complete constructor/reserve/copy/grow/array-init cases; real class callees; only allocation/free controlled')
print('Verified target SHA256:',target_sha)

