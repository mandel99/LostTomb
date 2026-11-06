"""Bounded city/building/grid caller recipes. No PE code is executed here."""
from pathlib import Path
import copy,json,re,sys
sys.dont_write_bytecode=True
OUT=Path(__file__).resolve().parent;ROOT=OUT.parents[2]
sys.path[:0]=[str(ROOT/'direct_c/cohort3_replay')]
from compiled_flood_regions.cohort7_compiled_acceptance_cases import Case as BaseCase,p
INPUTS=json.loads((OUT/'author-inputs.json').read_bytes());TARGETS=tuple(int(m['address'],16)for m in INPUTS['members'])
O,W,H,BOUND=0xbca2d8,0xbca2d0,0xbca2d4,0x78d790
XOUT,YOUT,TILE=0xd3b4e4,0xd3b4e0,0xbc9768
S=0x32001000
def B(i):return 0x7999d8+i*264
def A(i):return 0xbcd600+i*388
def F(i):return 0xbcb9c0+i*144
def signed(v,bits=32):return(v&((1<<bits)-1))-(1<<bits)if v&(1<<(bits-1))else v&((1<<bits)-1)
MASKS={0x446a30:0xffffffff,0x46f190:0xffffffff,0x4867e0:0xffffffff,0x486810:0xffffffff,0x4a2320:0xffffffff,0x4eb800:0xffffffff,0x4f4320:255,0x4f4440:255}
HELPERS={
 'scroll':(0x430cd0,1,'void'),'allocate_group':(0x46a280,1,'void'),
 'allocate_harbor':(0x46b100,1,'al8'),'allocate_dock':(0x46efd0,1,'al8'),
 'lookup_building':(0x475b50,1,'full32'),'remove_building':(0x461b20,1,'void'),
 'neighbors':(0x47ec80,2,'full32'),'equal_neighbors':(0x47ecc0,2,'full32'),
 'select_tile':(0x5190a0,4,'full32'),'market_orientation':(0x477690,2,'full32'),
 'sprite':(0x53e590,3,'void'),'get_draw_height':(0x489050,4,'full32'),'draw_tile':(0x47bf50,7,'void'),
 'actor_eligible':(0x4ea9a0,1,'al8'),'remove_actor':(0x4b3250,1,'void'),
 'list_count':(0x4a34b0,1,'full32'),'position_near':(0x480180,3,'void'),'reroute':(0x4b52f0,1,'void'),
 'follow15':(0x492360,1,'full32'),'follow42':(0x49b3c0,1,'full32'),'follow_other':(0x4a3540,1,'full32'),
 'stop_actor':(0x4b5420,1,'void'),'refresh':(0x4e8e80,1,'void'),
 'advance_building':(0x4f3360,1,'al8'),'building_refresh1':(0x4ee630,1,'void'),'building_refresh2':(0x4ee4a0,1,'void'),
 'choose_access':(0x4f3370,4,'al8'),'move_actor':(0x4b4220,6,'void'),'building_delay':(0x4f4420,1,'full32'),
 'percent':(0x4cde40,2,'full32'),'direction':(0x4467a0,2,'full32'),'formation':(0x4b6ff0,5,'full32'),
 'create_actor':(0x4b4800,5,'full32'),'actor_init':(0x4b5af0,1,'void'),'route_init':(0x4620c0,1,'void'),'spawn_tile':(0x469140,3,'void')}

class Case(BaseCase):
 def __init__(self,entry,label,args=()):
  super().__init__(entry,label,0x89abcdef,arguments=args,named=args,cleanup=24 if entry==0x488840 else 0,result=MASKS.get(entry))
  self.c['id']=f'c8-city-grid25/{entry:08x}/'+label
  member=next(m for m in INPUTS['members']if int(m['address'],16)==entry)
  self.c.update(routes=['native','canonical'],required_retained_route='canonical',routes_reason='Only the existing canonical FUN entry is requested; duplicate namedPC is omitted. Production absence is explicit in the current MAP profile.',
   expected_invocations=1+sum(bool(hit)for hit in member['providers'].values()),budget=4000000)
  self.ro={};self.observations=[];self.actual=[]
  for at in re.findall(r'^#define \w+ .*?0x([0-9a-fA-F]+)u',(ROOT/member['source']).read_text(),re.M):
   a=int(at,16)
   if a>=0x600000:self.seed(a,0)
 def seed(self,at,value,width=4):
  raw=p(value,width)
  try:self.write(at,raw)
  except AssertionError:
   for j,b in enumerate(raw):self.ro[at+j]=b
 def input(self,at,size,fill=0):
  for j in range(size):self.ro[at+j]=fill
 def cell(self,at,value,width=4):self.block(at,width);self.put(at,value,width)
 def val(self,at,width=4):
  try:return self.get(at,width)
  except AssertionError:return int.from_bytes(bytes(self.ro[at+j]for j in range(width)),'little')
 def begin(self,observe=None):
  super().start(observe if observe is not None else [(a+j,min(4,len(data)-j))for a,data in self.mem.items()for j in range(0,min(len(data),16),4)])
  # Read-only seeds never broaden native write permission. Watched initial bytes
  # take priority if an input range aliases an explicit output block.
  items=sorted((a,b)for a,b in self.ro.items()if not any(base<=a<base+len(data)for base,data in self.mem.items()));runs=[]
  for a,b in items:
   if runs and a==runs[-1][0]+len(runs[-1][1]):runs[-1][1].append(b)
   else:runs.append([a,bytearray([b])])
  self.c['stores']=[(a,bytes(b))for a,b in runs]+self.c['stores']
 def call(self,token,*args,result=0,mutations=()):
  if token not in self.eps:
   pc,arity,width=HELPERS[token]
   self.endpoint(token,pc,arity,width,native_cleanup=12 if token=='sprite'else 0,native_receiver=0xf55bb0 if token=='sprite'else None)
  assert len(args)==self.eps[token]['arity'],(token,args)
  # BaseCase's event accepts only numerical words; normalized live stack roles
  # are authored here with the identical pre-call snapshot/response semantics.
  c=self.c;c['authored_endpoint_order'].append(token)
  c['authored_endpoint_observations'].append([[a,w,self.get(a,w)]for a,w in self.observe])
  c['expected_endpoint_counts'][token]+=1;c['expected_endpoint_arguments'][token].append([a&0xffffffff if isinstance(a,int)else a for a in args])
  response=dict(value=result)
  if mutations:response['mutations']=list(mutations)
  self.eps[token]['responses'].append(response)
  for a,w,v in mutations:self.put(a,v,w)
 def finish(self,result=None):return self.end(result)

def buildings(c,count=257):c.seed(BOUND,count);c.input(B(0),264*(count+2))
def actors(c,count=2000):c.input(A(0),388*(count+1))
def rows(c,height=3,row=1,length=3,left=2,origin=0x1000):
 c.seed(W,64);c.seed(H,height);c.seed(O,origin)
 for base in(0xbc9804,0xbc9b94,0xbc9f24):c.input(base,228*4)
 tile=origin+row*228+left
 c.seed(0xbc9804+row*4,tile);c.seed(0xbc9f24+row*4,left);c.seed(0xbc9b94+row*4,left+length)
 return tile

def formation416a20(v):
 c=Case(0x416a20,'wrapped-first-valid-selection'if not v else'signed-coordinate-selection')
 c.seed(0xe38dda,1,1);c.cell(0x578c6c,6 if not v else 2);c.cell(0xe38dca,0x6a,1);c.seed(O,0x1234)
 c.input(F(0),7*144)
 chosen=1 if not v else 4
 for i in(1,4):
  c.seed(F(i)+8,1,1);c.seed(F(i)+0x71,0,1);c.seed(F(i)+0x4a,2,1)
 c.seed(F(chosen)+0x36,12 if not v else -3,2);c.seed(F(chosen)+0x38,3 if not v else 2,2)
 c.begin();c.put(0x578c6c,chosen)
 c.call('scroll',0x1234+(12 if not v else -3)+(3 if not v else 2)*228);c.put(0xe38dca,1,1)
 return c.finish()

def count46f190(v):
 c=Case(0x46f190,'high-building-ID-count'if not v else'active-and-kind-gates');buildings(c)
 for i in(1,257):c.seed(B(i),1,1);c.seed(B(i)+0x10,0xb6 if not v or i==257 else 0xb5,2)
 c.begin();return c.finish(2 if not v else 1)

def group46a320(v):
 c=Case(0x46a320,'high-ID-duplicate-group'if not v else'unassigned-group-allocation');buildings(c)
 c.block(0x9783a4,0x150,0xa5)
 for i in range(11):c.put(0x9783a4+i*28,0,1)
 ids=(1,257)if not v else(256,)
 for i in ids:c.seed(B(i),1,1);c.seed(B(i)+0x10,0x4a,2);c.seed(B(i)+0xcb,1 if not v else 0,1)
 c.begin()
 for a in range(0x9783d8,0x9784f0,28):c.put(a,0)
 if not v:c.put(0x9783a4+28,1,1)
 for i in ids:
  if not v and i==1:c.put(0x9783bc+28,i)
  else:c.call('allocate_group',i)
 return c.finish()

def harbor46b320(v):
 c=Case(0x46b320,'high-ID-claimed-harbor'if not v else'high-ID-unassigned-harbor');buildings(c)
 c.block(0x7900bc,0x9920,0xa5);c.cell(0x78d788,0x1122337f)
 for i in range(200):c.put(0x7900c0+i*196,0,1)
 ids=(1,257)if not v else(256,)
 for i in ids:
  c.seed(B(i),1,1);c.seed(B(i)+0x10,0x47 if not v else 0xc1,2);c.cell(B(i)+0xc1,1 if not v else 0,1)
 c.begin()
 for a in range(0x790180,0x7999dc,196):c.put(a,0,2)
 if not v:c.put(0x7900c0+196,1,1)
 for i in ids:
  if not v and i==1:c.put(0x7900bc+196,i,2)
  else:c.call('allocate_harbor',i,result=2 if not v else 3);c.put(B(i)+0xc1,2 if not v else 3,1)
 c.put(0x78d788,1 if not v else 0)
 return c.finish()

def dock46f030(v):
 c=Case(0x46f030,'high-ID-claimed-dock'if not v else'high-ID-unassigned-dock');buildings(c)
 c.block(0xeaaf68,0x2ca0,0xa5)
 for i in range(51):c.put(0xeaaf68+i*224,0)
 ids=(1,257)if not v else(256,)
 for i in ids:c.seed(B(i),1,1);c.seed(B(i)+0x10,0x88,2);c.cell(B(i)+0xca,1 if not v else 0,1)
 c.begin()
 for a in range(0xeab124,0xeadc04,224):c.put(a,0)
 if not v:c.put(0xeaaf68+224,1)
 for i in ids:
  if not v and i==1:c.put(0xeab044+224,i)
  else:c.call('allocate_dock',i,result=2 if not v else 3);c.put(B(i)+0xca,2 if not v else 3,1)
 return c.finish()

def terrain476750(v):
 c=Case(0x476750,'sprite-range-inclusive-edges'if not v else'signed-row-left-metadata')
 t=rows(c,length=4,left=2 if not v else -2);base=0x4567;c.seed(0xf565f4,base)
 c.block(0xaf598c+t*4-4,24,0xa5);c.block(0xb285cc+t-1,6,0xa5);c.block(0xae8e7c+t-1,6,0xa5)
 for j,n in enumerate((-1,0,7,8)):c.seed(0xb7482c+(t+j)*4,base+n);c.put(0xaf598c+(t+j)*4,0x11220004+j);c.put(0xb285cc+t+j,0xe0+j,1);c.put(0xae8e7c+t+j,9+j,1)
 c.begin()
 for j in(1,2):c.put(0xaf598c+(t+j)*4,c.get(0xaf598c+(t+j)*4)|0x20);c.put(0xb285cc+t+j,c.get(0xb285cc+t+j,1)&0x7f,1);c.put(0xae8e7c+t+j,0,1)
 return c.finish()

def orientation47d720(v):
 c=Case(0x47d720,'rotation0-typed-BYTE-markers'if not v else'rotation4-signed-row-left')
 t=rows(c,length=6,left=2 if not v else -2);c.seed(0xe94e04,0 if not v else 4)
 c.block(0xb67d1c+t-1,8,0xa5)
 for j in range(6):c.seed(0xb285cc+t+j,0x80|j,1);c.put(0xb67d1c+t+j,0x80|0x40|((3,8,16,24,32,41)if not v else(5,1,2,3,4,6))[j],1)
 c.begin()
 for j in range(6):
  old=c.get(0xb67d1c+t+j,1);value=old if j==0 else old&0xbf
  if j<5:value|=0x40
  c.put(0xb67d1c+t+j,value,1)
 return c.finish()

def rotate47fb70(v):
 c=Case(0x47fb70,'clockwise-six-BYTE-states'if not v else'counter-turn-signed-left')
 t=rows(c,length=6,left=2 if not v else -2);c.seed(0xbc96c8,1 if not v else -1);c.block(0xadc36c+t-1,8,0xa5)
 for j in range(6):c.seed(0xaf598c+(t+j)*4,4);c.put(0xadc36c+t+j,j+1,1)
 c.begin()
 for j,value in enumerate((6,5,4,1,2,3)if not v else(4,5,6,3,2,1)):c.put(0xadc36c+t+j,value,1)
 return c.finish()

def rect_flags(entry,v):
 x,y,size,radius=(3,4,2,1)if not v else(-1,0,2,1)
 c=Case(entry,'interior-rectangle'if not v else'negative-edge-clipping',(x,y,size,radius));origin=0x1234
 c.seed(W,64);c.seed(H,64);c.seed(O,origin);c.cell(TILE,0x89abcdef);c.cell(YOUT,0x11223344);c.cell(XOUT,0x55667788)
 left=max(0,x-radius);right=min(63,x+size-1+radius);top=max(0,y-radius);bottom=min(63,y+size-1+radius)
 start=origin+top*228+left;c.block(0xb67d1c+start-1,(bottom-top)*228+right-left+3,0xa5)
 if entry==0x482a90:
  for yy in range(top,bottom+1):
   for xx in range(left,right+1):c.put(0xb67d1c+origin+yy*228+xx,0x25,1)
 c.begin();c.put(YOUT,bottom+1)
 for yy in range(top,bottom+1):
  c.put(XOUT,right+1)
  for xx in range(left,right+1):
   at=0xb67d1c+origin+yy*228+xx;old=c.get(at,1);c.put(at,old|0x80 if entry==0x482a90 else old&0x7f,1)
 c.put(TILE,origin+(bottom+1)*228+left)
 return c.finish()

def dirty47fc40(v):
 actor=257;cx,cy,radius=(2,2,1)if not v else(0,0,2)
 c=Case(0x47fc40,'high-ID-building-radius'if not v else'negative-radius-edge-clipped',(actor,radius));c.seed(O,0x1234);c.seed(W,64);c.seed(H,64)
 c.seed(B(actor)+6,cx,2);c.seed(B(actor)+8,cy,2);c.cell(YOUT,0x11223344);c.cell(XOUT,0x55667788)
 left=max(cx-radius,0);right=min(cx+radius,63);top=max(cy-radius,0);bottom=min(cy+radius,63)
 ids=[]
 for yy in range(top,bottom+1):
  for xx in range(left,right+1):
   value=300 if(xx+yy)%2==0 else 0;c.seed(0xb4e6fc+(0x1234+yy*228+xx)*2,value,2)
   if value:ids.append(value)
 c.cell(B(300)+0x3f,0x6a,1);c.begin();c.put(YOUT,bottom+1);c.put(XOUT,right+1)
 if ids:c.put(B(300)+0x3f,1,1)
 return c.finish()

def mask4de200(v):
 index=v;c=Case(0x4de200,'two-valid-mask-rows'if not v else'signed-source-offset',(index,));origin=0x1234
 c.seed(W,5);c.seed(H,2);c.seed(O,origin);c.seed(0xbca2dc,223);c.seed(0xe94dd4,S);c.seed(0xe94dd8,7)
 sx,sy=(0,0)if not v else(-3,2);c.seed(0x5e2950+index*8,sx);c.seed(0x5e2954+index*8,sy)
 if v:c.seed(0x5e2950+index*32,0);c.seed(0x5e2954+index*32,0)
 c.input(S,4000);pattern=((16,11,4,8,3),(3,4,8,16,11));c.block(0xaf598c+origin*4-4,4*(228+5)+8,0xa5);c.block(0xb285cc+origin-1,228+7,0xa5)
 for yrow,items in enumerate(pattern):
  for xx,code in enumerate(items):c.seed(S+sy*1000+7+sx+yrow*1000+xx,code,1);c.put(0xb285cc+origin+yrow*228+xx,0x81,1)
 c.begin()
 for yy,items in enumerate(pattern):
  for xx,code in enumerate(items):
   tile=origin+yy*228+xx
   if code==3:c.put(0xb285cc+tile,c.get(0xb285cc+tile,1)|0x20,1)
   else:c.put(0xaf598c+tile*4,{16:2,11:1,4:4,8:0x40}[code])
 return c.finish()

def quota461950(v):
 c=Case(0x461950,'positive-quota-early-exhaustion'if not v else'signed-negative-quota',(1 if not v else -1,));buildings(c)
 c.input(0x5da544,16);c.seed(0x5da544,100);c.input(0x5da554,192);c.seed(0x5da554,1);c.seed(0x705740,1)
 c.seed(B(0)+0x40,1,2);c.seed(B(0)+5,0,1)
 for i in(1,257):c.seed(B(i),1,1);c.seed(B(i)+0x10,1,2)
 c.cell(BOUND,257);c.begin()
 c.call('lookup_building',1,result=0);c.call('remove_building',1)
 c.call('lookup_building',257,result=0)
 if v:c.call('remove_building',257)
 return c.finish()

def actor_reset4e7200(v):
 c=Case(0x4e7200,'low-clamp-high-building-ID'if not v else'high-clamp-actor-state-migration',(-5 if not v else 20,));buildings(c)
 c.seed(B(257)+5,1,1);c.block(B(257)+0xe8,24,0xa5)
 c.put(B(257)+0xe8,100);c.put(B(257)+0xf4,1 if not v else 0);c.put(B(257)+0xfc,200);c.cell(B(257)+0x77,9,1)
 for i in range(1,2001):c.seed(A(i)+6,0,1)
 c.block(A(1),388,0xa5);c.seed(A(1)+2,0x5f,1);c.seed(A(1)+6,1,1)
 c.block(A(257),388,0xa5);c.seed(A(257)+2,0x19,1);c.seed(A(257)+6,1,1);c.seed(A(257)+0x30,9 if not v else 10,1)
 c.seed(A(257)+0x24,-2 if v else 5,2);c.seed(A(257)+0x26,7,2)
 c.cell(0x78d7ac,0x11223344);c.cell(0x78d7b0,0x55667788);c.begin()
 inc=2500 if not v else 5000;c.put(B(257)+0xe8,100+inc);c.put(B(257)+0xfc,200+inc);c.put(B(257)+0x77,3 if not v else 0,1)
 c.call('stop_actor',1);c.put(A(257)+0x2c,0,1);c.put(A(257)+0x2d,0,1);c.put(A(257)+0x30,13,1)
 c.put(A(257)+0x1c,-2 if v else 5,2);c.put(A(257)+0x1e,7,2);c.call('remove_actor',257)
 c.put(0x78d7ac,(3 if not v else 9)*0x330);c.put(0x78d7b0,(3 if not v else 9)*0x330);c.call('refresh',-15)
 return c.finish()

def spiral4eb800(v):
 x,y=(-1,2)if v==2 else(5,6);sx,sy,kinds=S,S+4,S+0x100
 c=Case(0x4eb800,('first-linked-actor','excluded-head-follows-WORD-link','negative-x-no-ring')[v],(x,y,1,kinds,2,sx,sy,0,300 if v==1 else 0))
 c.seed(O,0x1234);c.seed(W,64);c.seed(H,64);c.cell(sx,0x89abcdef);c.cell(sy,0xfedcba98);c.seed(kinds,0x19);c.cell(TILE,0x11223344)
 t=0x1234+y*228+x
 if v!=2:
  c.seed(0xb350dc+t*2,300,2);c.seed(A(300)+2,0x19,1);c.seed(A(300),301 if v else 0,2);c.seed(A(301)+2,0x19,1);c.seed(A(301),0,2)
 c.begin();c.put(sx,x);c.put(sy,y);c.put(TILE,t)
 return c.finish(0 if v==2 else 300+v)

def building_inputs(c,id_,tile,kind=3,mask=0,offset=0):
 c.block(B(id_),264,0xa5);c.seed(B(id_)+0xc,tile);c.seed(B(id_)+0x12,offset,2);c.seed(B(id_)+0x55,kind,1);c.seed(B(id_)+0x5b,mask,1)

def corners():return INPUTS['constant_tables']['005f6bdc']['words_signed']

def upgrade4f3690(v):
 x,y=(5,6)if not v else(-1,3);c=Case(0x4f3690,'four-tile-stage3-upgrade'if not v else'four-record-stage10-upgrade',(x,y));c.seed(O,0x1234);t=0x1234+y*228+x;id_=300
 c.seed(0xb4e6fc+t*2,id_,2);c.seed(0xf56b54,0x4567);offsets=corners();base=t-offsets[0]
 building_inputs(c,id_,base,3 if not v else 10,0 if not v else 15,2 if not v else 0)
 if not v:
  for j,delta in enumerate(offsets):
   tile=base+delta;c.cell(0xaf598c+tile*4,0x11223344);c.cell(0xb7482c+tile*4,0x55667788);c.seed(0xacf85c+tile,j+1,1)
 else:
  for i in range(id_+1,id_+4):c.block(B(i),264,0xa5)
 c.begin()
 if not v:
  c.put(B(id_)+0x5b,1,1);c.put(B(id_)+0x55,4,1)
  for j,delta in enumerate(offsets):c.put(0xaf598c+(base+delta)*4,0x10000008);c.put(0xb7482c+(base+delta)*4,0x4567+j+1)
  c.call('building_refresh1',id_-2);c.call('building_refresh2',id_-2)
 else:
  for i in range(id_,id_+4):c.put(B(i)+0x55,11,1);c.put(B(i)+0x5b,15,1)
  c.call('building_refresh2',id_);c.call('building_refresh1',id_)
 return c.finish()

def access4f4320(v):
 actor=7;oldbuilding=300;newbuilding=400+v;x,y=(5,6)if not v else(2,3)
 c=Case(0x4f4320,'private-DWORD-outputs-to-WORD-destination'if not v else'signed-WORD-pixel-inputs',(actor,));c.seed(O,0x1234)
 c.block(A(actor),388,0xa5);c.seed(A(actor)+0x56,oldbuilding,2);c.seed(A(actor)+0x42,-15 if v else 30,2);c.seed(A(actor)+0x44,45,2)
 c.seed(B(oldbuilding)+0x10,0xb7,2);c.seed(B(oldbuilding)+0x55,2,1);c.seed(B(oldbuilding)+0x12,1,2)
 c.seed(0xb4e6fc+(0x1234+y*228+x)*2,newbuilding,2);c.block(B(newbuilding)+0x64-2,6,0xa5)
 c.begin();c.call('choose_access',oldbuilding-1,['stack-output','access-x'],['stack-output','access-y'],1,result=1)
 c.eps['choose_access'].update(pointer_args={1:'access-x',2:'access-y'},writes_args=[[1,4,x],[2,4,y]])
 c.put(A(actor)+0x1c,x,2);c.put(A(actor)+0x1e,y,2)
 c.call('move_actor',actor,-15 if v else 30,45,x*15,y*15,0);c.put(A(actor)+0x3c,0,2)
 c.put(A(actor)+0x56,newbuilding,2);c.put(B(newbuilding)+0x64,actor,2)
 return c.finish(1)

def waiting4f4440(v):
 actor=7;x,y=5,6;oldbuilding=300;target=0x1234+y*228+x
 c=Case(0x4f4440,'WORD-delay-not-reached'if not v else'WORD-delay-reached-real-upgrade-child',(actor,));c.seed(O,0x1234)
 c.block(A(actor),388,0xa5);c.seed(A(actor)+0xc,x,2);c.seed(A(actor)+0xe,y,2);c.seed(A(actor)+0x1c,7,2);c.seed(A(actor)+0x1e,8,2);c.seed(A(actor)+0x2e,4 if not v else 7,2)
 c.seed(0xb4e6fc+target*2,oldbuilding,2);c.block(B(oldbuilding)+0x64-2,6,0xa5);c.seed(B(oldbuilding)+0x64,actor,2)
 if v:
  nexttile=0x1234+8*228+7;c.seed(0xb4e6fc+nexttile*2,301,2);building_inputs(c,301,nexttile,5,0,0);c.actual.append(0x4f3690)
 c.begin();c.call('building_delay',target,result=10 if not v else 8);c.put(A(actor)+0x2e,5 if not v else 8,2)
 if v:
  c.put(B(301)+0x5b,15,1);c.call('building_refresh1',301);c.call('building_refresh2',301);c.put(B(oldbuilding)+0x64,0,2)
 c.c['actual_children']=list(c.actual)
 return c.finish(1 if not v else 0)

def bridge47f200(v):
 x,y=(5,6)if not v else(3,4);origin=0x1234;t=origin+y*228+x;step=228 if not v else -1;end=t+step
 c=Case(0x47f200,'south-bridge-end'if not v else'negative-step-west-bridge-end',(x,y));c.seed(O,origin)
 c.block(0xbc96f4,20,0xa5)
 c.input(0xaf598c+(t-457)*4,4*920);c.seed(0xaf598c+t*4,4)
 c.seed(0xaf598c+(t-228)*4,0 if not v else 4);c.seed(0xaf598c+(t+1)*4,0)
 offsets=INPUTS['constant_tables']['005f80fc']['words_signed']
 for delta in offsets:c.seed(0xee0c50+end+delta,0,1)
 c.begin()
 for a in(0xbc96fc,0xbc96f8,0xbc9700,0xbc96f4):c.put(a,0)
 c.put(0xbc9704,t);c.call('neighbors',t,4,result=3);c.call('neighbors',t,0x10000,result=0)
 c.put(0xbc96f4,4 if not v else 6);c.put(0xbc9700,step);c.put(0xbc96f8,1)
 c.put(0xbc96f8,2);c.put(0xbc96fc,end);c.call('neighbors',end,4,result=3);c.call('neighbors',end,0x10000,result=0)
 return c.finish()

def tile_select(entry,v):
 negative=entry==0x486810 and v
 origin=0x1234;mask=2
 args=(17,19,0,mask)if entry==0x4867e0 else(17,19,0,mask,-3 if negative else 2,-1 if negative else 1,-1 if negative else 5,0 if negative else 2)
 c=Case(entry,'real-child-positive-selector'if entry==0x4867e0 and not v else'no-selection-clears-gate'if entry==0x4867e0 else'positive-DWORD-candidates'if not v else'negative-bounds-signed-IDIV',args)
 c.seed(O,origin);c.seed(W,64);c.seed(H,3)
 for a in(0xbc9804,0xbc9f24,0xbc9b94):c.input(a,912)
 c.seed(0xbc9804+4,origin+228+2);c.seed(0xbc9f24+4,2);c.seed(0xbc9b94+4,5)
 if negative:c.seed(0xbc9f24-4,-4);c.seed(0xbc9b94-4,-1)
 first=origin-231 if negative else origin+228+2;tiles=(first,first+1)if negative else(first,first+2)
 for tile in range(first,first+(2 if negative else 3)):
  c.seed(0xaf598c+tile*4,mask if tile in tiles else 0x80000);c.seed(0xa43eac+tile,-1,1);c.seed(0xb350dc+tile*2,1,2)
 c.seed(A(1)+2,0x19,1);c.cell(0x978d98,1,1);c.cell(XOUT,0x11223344);c.cell(YOUT,0x55667788);c.block(0x96e6b0,16,0xa5)
 c.begin()
 for j,t in enumerate(tiles):c.put(0x96e6b4+j*4,t)
 selected=0 if entry==0x4867e0 and v else 2
 c.call('select_tile',17,19,0x96e6b4,2,result=selected)
 if selected:
  delta=tiles[1]-origin;quot=abs(delta)//228*(-1 if delta<0 else 1);rem=delta-quot*228
  c.put(XOUT,rem);c.put(YOUT,quot)
 else:c.put(0x978d98,0,1)
 if entry==0x4867e0:c.c['actual_children']=[0x486810]
 return c.finish(1 if selected else 0)

def pick_actor4a2320(v):
 selected=300 if not v else 700;current=1;x,y=(-2,3)if not v else(5,6);origin=0x1234
 c=Case(0x4a2320,'high-ID-private-candidate-buffer'if not v else'signed-WORD-distance-gate',(17,19,10 if not v else 5));c.seed(O,origin)
 for i in range(1,2001):c.seed(A(i)+6,0,1)
 c.block(A(selected),388,0);c.seed(A(selected)+6,1,1);c.seed(A(selected)+2,7,1);c.seed(A(selected)+0x30,8,1);c.seed(A(selected)+0x130,0);c.seed(A(selected)+0xc,x,2);c.seed(A(selected)+0xe,y,2)
 c.cell(0xd056f8,current);c.seed(A(current)+0x36,6,2);c.begin()
 c.call('actor_eligible',7,result=1);tile=origin+y*228+x
 c.call('select_tile',17,19,['stack-input','candidate-tiles',p(tile).hex()],1,result=1)
 c.eps['select_tile']['private_input_args']={2:dict(role='candidate-tiles',width=4)}
 if not v:c.call('remove_actor',current)
 return c.finish(selected if not v else 0)

def relocate4a38a0(v):
 parent,child,follower,building=5,7,8,300;xx,yy=(5,6)if not v else(-2,3);origin=0x1234
 c=Case(0x4a38a0,'one-live-child-plus-real-follower'if not v else'signed-output-WORD-propagation',(parent,building));c.seed(O,origin);c.seed(A(parent)+0x114,2)
 slot=0x9784dc+2*44;c.cell(slot,child)
 for i in range(1,2001):c.seed(A(i)+2,0,1)
 for id_ in(child,follower):c.block(A(id_),388,0xa5)
 c.seed(A(child)+6,1,1);c.seed(A(child)+0x30,6,1);c.seed(A(child)+8,9,1);c.seed(A(child)+2,0,1);c.seed(A(follower)+2,0x15,1)
 c.seed(B(building)+6,2,2);c.seed(B(building)+8,3,2);c.cell(XOUT,0x11223344);c.cell(YOUT,0x55667788);c.begin()
 c.call('list_count',parent,result=1);c.put(slot,-1)
 c.call('position_near',2,3,2,mutations=[(XOUT,4,xx),(YOUT,4,yy)])
 for off,value,w in[(0x10,xx,2),(0xc,xx,2),(0x12,yy,2),(0xe,yy,2),(0x18,origin+yy*228+xx,4),(0x42,xx*15,2),(0x44,yy*15,2),(0x30,9,1),(0x31,15,1)]:c.put(A(child)+off,value,w)
 c.call('reroute',child);c.call('follow15',follower,result=child)
 for off,value,w in[(0x10,xx,2),(0xc,xx,2),(0x12,yy,2),(0xe,yy,2),(0x18,origin+yy*228+xx,4),(0x42,xx*15,2),(0x44,yy*15,2)]:c.put(A(follower)+off,value,w)
 c.put(A(child)+0x31,15,1);c.call('reroute',follower)
 return c.finish()

def dispatch446a30(v):
 c=Case(0x446a30,'nonzero-entrance-tile-no-population'if not v else'signed-fallback-entrance-and-counter-wrap',(11,0,9,0x12345678,1));origin=0x1234;c.seed(O,origin)
 c.seed(0x785d70,4);c.block(0x785d74,5*124,0xa5);c.put(0x785d74+124+0x36,0,1)
 c.block(0x775900,2,0);c.put(0x775900,31999 if not v else 32000,2)
 # Native WORD coordinate tables8..15, and three DWORD optional actor groups.
 for a in(0x784bc0,0x784bd0,0x784be0):c.input(a,32)
 c.seed(0x784bc0+8*2,3 if not v else -1,2);c.seed(0x784be0+8*2,4 if not v else 2,2)
 c.seed(0x784c68,-2,2);c.seed(0x784c6a,3,2);c.seed(0x784904,64);c.seed(0x784908,64)
 for a in(0x784dc4,0x784dc8,0x784dcc):c.seed(a,-1)
 c.begin();c.put(0x785d74+124+0x32,1,1);c.put(0x775900,32000 if not v else 1,2)
 return c.finish(origin+(4 if not v else 3)*228+(3 if not v else -2))

def sprite488840(v):
 actor=3;px,py=(100,200)if not v else(-40,300);baseimage=10;param5=20
 c=Case(0x488840,'real-ftol-constant-producer'if not v else'real-ftol-ratio-producer',(actor,px,py,baseimage,param5,1));origin=0x1234;c.seed(O,origin)
 # Both native producers operate on real ST0 and preserve these live caller
 # values. Current C's missing FLD/FILD cannot be concealed by a helper stub.
 c.c.update(compare_x87_values=True,compare_x87_control=True,initial_fpcw=0x27f,
  initial_x87=['00000000000000d00040','00000000000000c0ffbf'],actual_children=[0x56133c])
 c.seed(B(actor)+6,5,2);c.seed(B(actor)+8,6,2);c.seed(B(actor)+3,0,1);c.seed(B(actor)+0x40,1,2);c.seed(0xbca2e0,2)
 # Rotation2 adds the DWORD view offset5CC8C0 before tile origin.
 c.seed(0x5cc8c0,0);tile=origin+6*228+5;c.seed(0xadc36c+tile,1,1)
 c.seed(0xbca34c,500);c.seed(0xd386d4,1 if not v else 0);c.seed(0x78d7c4,0);c.seed(0xacf85c+tile,2,1)
 c.seed(0x78d79c,0);c.seed(0x78d7cc,0);c.seed(0xf560ac,0x4567);c.seed(0xf5609c,0x5678);c.seed(0xf560a4,0x6789)
 c.seed(0x70e45c,8);c.seed(0x70e44c,4);c.seed(0x70e448,3);c.seed(0x126517c,0x10000)
 # All source sprite records have actual64-byte native stride, signed WORD
 # vertical/overlay offsets. The original table elements are explicit inputs.
 overlaymode=3 if not v else 2;overlayindex=2 if not v else 3
 image=(0x4567+2)if not v else(0x5678+(2+overlayindex)%7)
 c.seed(0x10d6a0e+12*64,2,2);c.seed(0x10d6a0e+10*64,1,2);c.seed(0x10d6a1a+image*64,1,2);c.seed(0x10d6a1c+image*64,2,2)
 c.cell(0xbca2e0,2);c.begin()
 points=((0,-2),(1,-1),(2,0),(-1,-1),(0,0),(1,1),(-2,0),(-1,1),(0,2))
 # Native first conversion10*20=200, FILD200/FILD(500+1)*9 ->3.
 for j,(dx,dy)in enumerate(points):
  current=baseimage+(2 if j==0 else 0);vertical=2 if j==0 else 1
  sx=px+dx*8;sy=py+dy*4+6-vertical;c.call('sprite',current,sx,sy)
  if j==overlayindex:c.call('sprite',image,8-1+sx,12-2-5+sy)
 return c.finish()

BUILDERS={0x416a20:formation416a20,0x446a30:dispatch446a30,0x461950:quota461950,0x46a320:group46a320,0x46b320:harbor46b320,0x46f030:dock46f030,0x46f190:count46f190,
 0x476750:terrain476750,0x47d720:orientation47d720,0x47f200:bridge47f200,0x47fb70:rotate47fb70,0x47fc40:dirty47fc40,
 0x482a90:lambda v:rect_flags(0x482a90,v),0x482b50:lambda v:rect_flags(0x482b50,v),0x4867e0:lambda v:tile_select(0x4867e0,v),0x486810:lambda v:tile_select(0x486810,v),
 0x488840:sprite488840,0x4a2320:pick_actor4a2320,0x4a38a0:relocate4a38a0,0x4de200:mask4de200,0x4e7200:actor_reset4e7200,0x4eb800:spiral4eb800,0x4f3690:upgrade4f3690,0x4f4320:access4f4320,0x4f4440:waiting4f4440}
def cases(registry=None):
 selected=TARGETS if registry is None else tuple(a for a in TARGETS if any(int(m['address'],16)==a for m in registry['members']))
 result=[BUILDERS[a](v)for a in selected for v in range(2)]
 if 0x4eb800 in selected:result.append(spiral4eb800(2))
 if 0x488840 in selected:
  base=next(c for c in result if c['entry']==0x488840 and c['id'].endswith('real-ftol-ratio-producer'))
  for pc,precision in((0x0000,24),(0x0200,53),(0x0300,64)):
   for rc,name in((0x0000,'nearest'),(0x0400,'down'),(0x0800,'up'),(0x0c00,'truncate')):
    cw=0x007f|pc|rc
    if cw==0x027f:continue
    c=copy.deepcopy(base);c['id']+=f'/pc{precision}-rc-{name}';c['initial_fpcw']=cw;result.append(c)
 return result
