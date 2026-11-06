"""Bounded functional4 recipes authored from native bytes and explicit RAM."""
from compiled_flood_regions.cohort7_compiled_acceptance_cases import Case as BaseCase

TARGETS=(0x409eb0,0x44c1a0,0x4c42b0,0x4e0f90)
MODE=0xd3b198;CAMERA=0xbca2e0;ORIENT=0xe94e04;REFRESH=0xe38dca;TURN=0xbc96c8
COUNT=0x785d70;ROWS=0x785d74;PLAYER=0xeaac28;HISTORY=0xd3b1c4;RNG=0xd3b364
SELECTED=0x6057e4;MODIFIERS=0xe38ef0;KEYDATA=0x32004010
RESOURCE_COUNT=0xf53e6c;RESOURCES=0xf53e70


def signed(v,bits=16):
 v&=(1<<bits)-1
 return v-(1<<bits)if v&(1<<(bits-1))else v
def row_address(row):return ROWS+row*124
def building(id_):return 0x7999d8+id_*264


class Case(BaseCase):
 def __init__(self,entry,label,args=(),result=None):
  super().__init__(entry,label,0x89abcdef,arguments=args,named=list(args),result=result)
  self.c['id']=f'c8-functional4/{entry:08x}/'+label;self.c.pop('expected_invocations')
  self.c.update(budget=5000000,compare_x87_values=True,compare_x87_control=True,initial_fpcw=0x27f,
   initial_x87=['0000000000000080ff3f','000000000000008000c0'])
  if entry in(0x44c1a0,0x4c42b0):self.c['required_retained_route']='canonical'
  self.actual=[]

 def cell(self,at,value,width=4):
  self.block(at,width);self.put(at,value,width)

 def begin(self):self.start([(at+i,4)for at,data in self.mem.items()for i in range(0,len(data)-3,4)])

 def finish(self,result=None):
  self.c['actual_children']=list(dict.fromkeys(self.actual));return self.end(result)


def rotation_cases():
 out=[]
 for label,mode,camera in [('zero-mode-zero-camera',0,0),('mode2-reloads-DWORDs',2,0),
  ('mode1-reloads-DWORDs',1,0),('camera2-single-counterturn',0,2),('camera6-single-turn',0,6),
  ('camera4-double-turn-and-refresh',0,4),('camera00000100-not-BYTE-zero',0,0x100),
  ('mode00000100-not-BYTE-zero',0x100,0)]:
  c=Case(0x409eb0,label)
  for at,value,width in[(MODE,mode,4),(CAMERA,camera,4),(ORIENT,0x89abcd02,4),(TURN,0x55667788,4),(REFRESH,0x6a,1)]:c.cell(at,value,width)
  c.begin()
  for token,pc,arity in[('turn',0x430b20,0),('counterturn',0x430be0,0),('refresh',0x47d6e0,0),('redraw',0x4de080,0),('message',0x4e2970,3)]:c.endpoint(token,pc,arity)
  if mode==0 and camera==0:out.append(c.finish());continue
  if mode in(1,2):
   token='counterturn'if mode==1 else'turn'
   c.event(token,[],mutations=[(MODE,4,0x10203040+mode),(ORIENT,4,0xcafebe00+mode)])
   c.put(TURN,c.get(MODE));c.put(CAMERA,c.get(ORIENT))
  elif mode==0:
   c.put(TURN,1 if camera==2 else 0)
   token='counterturn'if camera==2 else'turn'
   c.event(token,[],mutations=[(ORIENT,4,0x11112222)])
   if camera not in(2,6):c.event('turn',[],mutations=[(ORIENT,4,0x88889999)])
   c.put(CAMERA,c.get(ORIENT))
   if camera not in(2,6):c.event('refresh',[],mutations=[(ORIENT,4,0xabcdef06)])
  else:c.put(TURN,mode);c.put(CAMERA,c.get(ORIENT))
  c.event('refresh',[]);c.event('redraw',[]);c.event('message',[1,0,0]);c.put(REFRESH,1,1)
  out.append(c.finish())
 return out


def council_seed(c,configs,count=257,player=1,history=99,state_a=0x89abcdef,state_b=0x13579bdf,treasury=1000,expense=50):
 c.cell(COUNT,count);c.cell(PLAYER,player)
 ids=sorted(set(configs)|{0})
 # Record0 is a guarded decoy for truncated full-row arguments. Distinct
 # high records may be adjacent; one block preserves their shared borders.
 c.block(ROWS,124,0xa5)
 high=[r for r in ids if r]
 if high:
  start=row_address(min(high))-(0 if min(high)==1 else 8)
  c.block(start,row_address(max(high))+124+8-start,0xa5)
 for r in ids:
  cfg=configs.get(r,{});base=row_address(r)
  for off,width,value in[(0,2,cfg.get('owner',-1)),(2,1,2),(4,2,cfg.get('kind',5)),(12,2,cfg.get('cost',5)),
   (46,2,0x7654),(48,2,cfg.get('flag',0)),(88,2,-1)]:c.put(base+off,value,width)
 c.block(HISTORY,404,0xa5);c.put(HISTORY,history)
 c.block(RNG,36,0xa5)
 for off,value in[(0,0x11112222),(4,0x33334444),(8,0x55556666),(12,0x77778888),(16,0x1234abcd),(20,0x9999aaaa),(28,state_a),(32,state_b)]:c.put(RNG+off,value)
 if any(cfg.get('kind')==36 for cfg in configs.values()):
  for base,value in[(0xea187c+(player*0x49d8&0xffffffff),treasury),(0xea5fc8+(player*0x49d8&0xffffffff),expense)]:c.cell(base&0xffffffff,value)


def advance_rng(c):
 index=c.get(HISTORY);c.put(HISTORY+4+index*4,c.get(0xd3b374))
 c.put(HISTORY,0 if index+1>=100 else index+1)
 a,b=c.get(0xd3b380),c.get(0xd3b384)
 for _ in range(31):
  a=(a>>1)|((((a>>4)^a)&1)<<30)
  b=(b>>1)|((((b>>4)^b)&1)<<30)
 c.put(0xd3b380,a);c.put(0xd3b384,b)
 for at,value in[(0xd3b364,a&0x7fff),(0xd3b368,a&0x7f),(0xd3b36c,a&7),
  (0xd3b370,b&0x7fff),(0xd3b374,b&0x7f),(0xd3b378,b&7)]:c.put(at,value)
 return b


def council(c,row,delivery_mutations=()):
 if row<0 or row>=signed(c.get(COUNT),32):return
 base=row_address(row);kind=signed(c.get(base+4,2));cost=signed(c.get(base+12,2))
 c.put(base+48,3 if c.get(base+48,2)else 2,2)
 if kind==37:
  c.put(base+48,5,2);c.put(base+46,0,2)
  c.actual+=['0044c110 actual ECX=selected record; type2, signedWORD+58=-1 -> EAX0',
   '004b84b0 actual signed owner argument-1 -> earlyRET0']
  c.put(base+46,12,2);return
 b=advance_rng(c);c.put(base+46,(b&3)+1,2)
 if kind==36:
  offset=c.get(PLAYER)*0x49d8&0xffffffff
  for at,delta in[((0xea187c+offset)&0xffffffff,-cost),((0xea5fc8+offset)&0xffffffff,cost)]:c.put(at,c.get(at)+delta)
 else:
  if 'delivery'not in c.eps:c.endpoint('delivery',0x45b4d0,3)
  c.event('delivery',[c.get(PLAYER),kind,cost*100],mutations=delivery_mutations)


def council_cases():
 out=[]
 specs=[('negative-row-no-write',-1,257,36,7,0),('row-equals-count-no-write',256,256,36,7,0),
  ('full-row256-count257-treasury-history-wrap',256,257,36,7,0),
  ('count256-row1-negative-cost',1,256,36,-7,0x8000),
  ('actual-member-query-and-recruit-reject',256,257,37,7,0),
  ('other-kind-controlled-delivery-and-alias',256,257,5,3,1),
  ('row0-signed-kind-and-full-player-param',0,1,-1,-3,0),
  ('treasury-and-expense-wrap32',256,257,36,7,0)]
 for label,row,count,kind,cost,flag in specs:
  c=Case(0x44c1a0,label,[row]);config={max(0,row):dict(kind=kind,cost=cost,flag=flag)}
  council_seed(c,config,count,player=0x100 if row==0 else 1,history=99 if 'history'in label else 0,
   state_a=0xffffffff if cost<0 else 0x89abcdef,state_b=0 if cost<0 else 0x13579bdf,
   treasury=3 if 'wrap32'in label else 1000,expense=0xfffffffe if 'wrap32'in label else 50)
  c.begin();mutations=[(PLAYER,4,2),(row_address(256)+46,2,0x1234)]if 'alias'in label else[]
  council(c,row,mutations);out.append(c.finish())
 return out


def building_seed(c,id_,kind,phase=0,level=2,pop=6000,count=1,resources=(1,)):
 base=building(id_);c.block(base-8,280,0xa5)
 for off,width,value in[(0,1,1),(3,1,phase),(16,2,kind),(18,2,level),(28,2,pop)]:c.put(base+off,value,width)
 for index in range(13):c.put(base+84+index*2,0,2)
 c.cell(RESOURCE_COUNT,count);c.block(RESOURCES,16,0xa5)
 for i,resource in enumerate(resources):c.put(RESOURCES+i*4,resource)
 table=0x70cdf8+level*88;c.block(table,16,0xa5)
 for i,value in enumerate((300,400,500,600)):c.put(table+i*4,value)
 extra=[]
 for id0 in range(4000):
  if id0==id_:continue
  at=building(id0)
  try:c.locate(at,1);c.put(at,0,1)
  except AssertionError:extra.append((at,b'\0'))
 c.begin();c.c['stores']+=extra
 c.actual.append('004c42b0 actual4000-building scan; only selected record may be active')
 return base


def inventory_cases():
 out=[]
 for label,kind,phase in[('house-kind10-WORD-food-and-resource-switch',10,0),('house-kind29-phase2-WORD-wrap',29,2)]:
  c=Case(0x4c42b0,label)
  # Stocks are native inputs; start() must snapshot them before applying oracle.
  pop=12000 if phase else 6000
  base=building_seed(c,256,kind,phase=phase,pop=pop)
  for index in(0,7,8,9,10,11):c.put(base+84+index*2,1,2)
  # Replace the authored input block snapshot after the above explicit seed.
  c.c['stores']=[(at,bytes(data))for at,data in c.mem.items()]+[(building(i),b'\0')for i in range(4000)if i!=256 and not any(b<=building(i)<b+len(d)for b,d in c.mem.items())]
  for index in(0,7):c.put(base+84+index*2,pop*6,2)
  for index,value in[(8,400),(9,300),(10,500),(11,600)]:c.put(base+84+index*2,value*(8 if phase>1 else 2),2)
  out.append(c.finish())
 c=Case(0x4c42b0,'warehouse-query-false-preserves-WORD-guards');base=building_seed(c,1,70)
 c.put(base+86,0x1234,2);c.c['stores'][0]=(base-8,bytes(c.mem[base-8]))
 c.endpoint('eligible',0x5013a0,1,'full32');c.event('eligible',[1],0);out.append(c.finish())
 for label,resources,stocks,values,mutate in[
  ('warehouse-food-floor-and-goods-cap',(1,15),[(0,100),(8,12000)],[1,1],False),
  ('warehouse-zero-stock-and-signed-stock',(2,13),[(1,0),(9,-1)],[1,1],False),
  ('DWORD-count256-callback-reloads-count-and-stock',(1,),[(0,100)],[1],True)]:
  c=Case(0x4c42b0,label);base=building_seed(c,256,70,count=256 if mutate else len(resources),resources=resources)
  for index,stock in stocks:c.put(base+86+index*2,stock,2)
  c.c['stores']=[(at,bytes(data))for at,data in c.mem.items()]+[(building(i),b'\0')for i in range(4000)if i!=256 and not any(b<=building(i)<b+len(d)for b,d in c.mem.items())]
  c.endpoint('eligible',0x5013a0,1,'full32')
  for resource,value in zip(resources,values):
   changes=[(RESOURCE_COUNT,4,1),(base+86,2,3000)]if mutate else[]
   c.event('eligible',[resource],value,mutations=changes)
   index={1:0,2:1,13:9,15:8}[resource];stock=signed(c.get(base+86+index*2,2))
   if value and stock:
    floor=800 if resource==1 else 600 if 1<resource<=8 else 400
    c.put(base+86+index*2,min(20000,max(floor,stock*2)),2)
  out.append(c.finish())
 return out


def key_cases():
 out=[]
 specs=[('unconditional-key84',0x84,0x1300,None),('modifier100-key112',0x112,0x100,0xe5),
  ('modifier100-key111',0x111,0x100,0xe6),('modifier200-key10c',0x10c,0x200,0xe3),
  ('modifier200-key10b',0x10b,0x200,0xe4),('modifier1000-key88',0x88,0x1000,0x10f),
  ('wrong-bit-key112',0x112,0x200,None),('wrong-bit-key88',0x88,0x100,None),
  ('key135-full-row256-actual-council',0x135,0,0xeb),
  ('key13a-callback-reloads-full-row257',0x13a,8,0xeb),('key13a-disabled',0x13a,0,None)]
 for label,key,mod,message in specs:
  c=Case(0x4e0f90,label,[key,KEYDATA],0xffffffff)
  c.cell(MODIFIERS,mod);c.cell(SELECTED,256);c.block(KEYDATA-4,16,0xa5);c.put(KEYDATA,0xc0,2)
  actual=message==0xeb
  if actual:council_seed(c,{256:dict(kind=5,cost=5),257:dict(kind=36,cost=7)},count=258,player=2)
  c.begin();c.endpoint('message',0x4e2970,3)
  if message is not None:
   c.event('message',[message,0,0],mutations=[(SELECTED,4,257)]if key==0x13a else[])
   if actual:c.actual.append('0044c1a0 actual child with full selected row from6057E4 after message');council(c,c.get(SELECTED))
  out.append(c.finish(0 if message is not None else 1))
 return out


def cases(registry=None):
 out=rotation_cases()+council_cases()+inventory_cases()+key_cases()
 if registry is None:return out
 members={int(m['address'],16):m for m in registry['members']};out=[c for c in out if c['entry']in members]
 for c in out:c['expected_invocations']=1+sum(bool(v['named'])+bool(v['canonical'])for v in members[c['entry']]['providers'].values())
 return out


definitions=cases
