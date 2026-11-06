"""Bounded event/calendar/food recipes; no executable is run here."""
import struct
from compiled_flood_regions.cohort7_compiled_acceptance_cases import Case as BaseCase

TARGETS=(0x44ba90,0x452160,0x45d630,0x45de80,0x45e150)
MANAGER=0x785d70;BUILDINGS=0x7999d8;POLICY=0x7900c2
PLAYER=2;OFF=PLAYER*0x49d8;STOCK=0xea60ac+OFF


def signed(value,width=32):
 value&=(1<<width)-1
 return value-(1<<width)if value>>(width-1)else value


class Case(BaseCase):
 def __init__(self,entry,label,args=(),result=None,receiver=0x89abcdef):
  super().__init__(entry,label,receiver,args,named=list(args),result=result)
  self.c['id']=f'c7-event-food/{entry:08x}/'+label
  self.c.pop('expected_invocations')
  self.c.update(compare_x87_values=True,compare_x87_control=True,
   initial_fpcw=0x27f,initial_x87=['0000000000000080ff3f','000000000000008000c0'])


def manager_cases():
 out=[]
 for label,count,flags,months,guards in [
  ('zero-count',0,[],[],[]),('negative-count',-1,[],[],[]),
  ('mixed-skipped-and-real-unknown-kind',3,[1,0,1],[0,0,0],[0,0,0]),
  ('three-real-unknown-kind-noops',3,[0,0,0],[0,0,0],[0,0,0]),
  ('real-rearm-byte-decrement-and-wrap',2,[0x14,0x14],[2,0],[0,0]),
  ('real-rearm-guard-preserves-first',2,[0x14,0x14],[2,3],[1,0])]:
  c=Case(0x44ba90,label,receiver=MANAGER)
  c.block(MANAGER,0x48b8,0);c.put(MANAGER,count)
  c.put(MANAGER+0x48b4,0xa1,1);c.put(MANAGER+0x48b5,0xb2,1)
  c.block(0x78a634,1);c.put(0x78a634,0xc3,1)
  c.block(0x5d9a40,4);c.put(0x5d9a40,0x12345678)
  for i,flag in enumerate(flags):
   row=MANAGER+4+i*124;c.put(row+2,0,1);c.put(row+3,months[i],1)
   c.put(row+0x28,flag);c.put(row+0x32,guards[i],1)
  c.start([(MANAGER,4),(MANAGER+0x48b4,2),(0x78a634,1),(0x5d9a40,4)])
  c.put(MANAGER+0x48b4,0,1);c.put(MANAGER+0x48b5,0,1)
  c.put(0x78a634,0,1);c.put(0x5d9a40,0xffffffff)
  for i,flag in enumerate(flags):
   if flag&4 and not guards[i]and flag&16:
    assert months[i]!=1 # Reset descendants are intentionally outside these fixtures.
    c.put(MANAGER+4+i*124+3,(months[i]-1)&255,1)
  c.c['actual_children']=['0044c660 unknown kind0 returns','0044bc20 rearm; no reset branch']
  out.append(c.end())
 return out


def calendar_cases():
 out=[]
 values=[('cash-zero-resets',0,80,6,0),('cash-positive-resets',5,0xffffffff,0xffffffff,1),
  ('debt-first-month',-1,0,0,0),('month-wrap-zero-no-event',-1,0xffffffff,5,1),
  ('twelfth-month-selected-player',-1,11,0,2),('year-ten-cap',-1,23,9,1),
  ('above-ten-stored-unclamped-index-capped',-1,35,14,0),
  ('negative-month-multiple-and-year-wrap',-1,0xfffffff3,0xffffffff,0),
  ('year-signed-wrap-table-address-wrap',-1,11,0x7fffffff,0)]
 for ordinal,(label,cash,months,years,skill)in enumerate(values):
  c=Case(0x452160,label);off=3*0x49d8
  for at,value in [(0xeaac28,3),(0xea187c+off,cash),(0xea609c+off,months),(0xea60a0+off,years),
   (0xe39080,skill),(0xe950dc,0xcccccccc)]:c.block(at,4);c.put(at,value)
  next_month=(months+1)&0xffffffff
  event=cash<0 and next_month!=0 and signed(next_month)%12==0
  if event:
   next_year=(years+1)&0xffffffff;index=(min(signed(next_year),10)+skill*10)&0xffffffff
   table=(0x5da330+index*4)&0xffffffff;amount=0x81230000+ordinal
   c.block(table,4);c.put(table,amount)
  c.start([(0xeaac28,4),(0xea609c+off,4),(0xea60a0+off,4),(0xe950dc,4)])
  c.endpoint('debt-notification',0x4e96c0,1)
  c.endpoint('debt-message',0x4e0cf0,5)
  if cash>=0:c.put(0xea609c+off,0);c.put(0xea60a0+off,0)
  else:
   c.put(0xea609c+off,next_month)
   if event:
    c.put(0xea60a0+off,next_year);c.event('debt-notification',[amount])
    c.put(0xe950dc,1);c.event('debt-message',[0x79,0,0,0,0])
  out.append(c.end())
 return out


def add_food(c,bound=1,editor=0,population=400):
 c.block(BUILDINGS,4*264,0)
 c.block(0x705eb8+71*40,4);c.put(0x705eb8+71*40,100)
 c.block(0x705eb8+191*40,4);c.put(0x705eb8+191*40,100)
 for at,width,value in [(0x78d790,4,bound),(0x784bbc,4,editor),(0xeaac28,4,3),
  (0xea1894+OFF,4,population),(0xea5ce8+OFF,2,0),(0xea5ce8+3*0x49d8,2,0),
  (0xe38ebc,4,1),(0xe38ea8,1,0xfe),(0x5794e8,4,0xfffffffe),(0x579574,4,0xfffffffe),
  (0x7027b9,1,1),(0x7027ba,1,0),(0x7027bf,1,0),(0xe950dc,4,0xcccccccc)]:
  c.block(at,width);c.put(at,value,width)
 for at,size in [(STOCK,36),(0xea41dc+OFF,8),(0xea4388+OFF,8),(0xea5e1c+OFF,16)]:c.block(at,size)


def food_record(c,id_,kind,grade,stocks,house=False):
 row=BUILDINGS+id_*264;c.put(row,1,1);c.put(row+16,kind,2)
 c.put(row+6,-3,2);c.put(row+8,7,2);c.put(row+64,grade,2);c.put(row+68,0xa5,1)
 for i,value in enumerate(stocks):c.put(row+0x56+i*2,value,2)
 if house:c.put(row+5,1,1);c.put(row+28,1,2);c.put(row+0x54,-3,2)
 return row


def food_cases():
 out=[]
 # Explicit authored vectors/counters; arithmetic/rate children execute normally.
 specs=[
  ('empty',0,0,400,[],[],[0]*9,(0,0,0,0,0,0),0,[]),
  ('negative-bound-editor-override',-1,1,-4,[],[],[0]*9,(0,1,12,0,0,0),0,[]),
  ('non-granary-signed-local-only',1,0,400,[(100,0,[0]*9,True)],[],[0]*9,(0,1,0,0,0,0),0,[]),
  ('granary-road-reject',1,0,400,[(71,100,[20,10,0,0,0,0,0,0,0],False)],[0],[0]*9,(0,0,0,0,0,0),0,[]),
  ('grade49-positive-stock',1,0,400,[(71,49,[20,10,0,0,0,0,0,0,0],False)],[0x100],[0]*9,(0,0,0,0,1,1),1,[]),
  ('grade49-negative-stock',1,0,400,[(71,49,[20,-10,0,0,0,0,0,0,0],False)],[1],[0]*9,(0,0,0,0,1,0),0,[]),
  ('grade50-two-native-tutorial-thresholds',1,0,400,[(71,50,[1000,100,0,0,0,0,800,0,400],False)],[1],
    [1000,100,0,0,0,0,800,0,400],(1300,3,13,1,1,0),0,[2,7]),
  ('grade100-signed-stock-and-zero-need',1,0,0,[(191,100,[1000,-10,20,0,0,0,0,0,0],False)],[1],
    [1000,-10,20,0,0,0,0,0,0],(10,2,1,1,0,0),0,[]),
  ('two-granaries-sum-and-grade99',2,0,400,[(71,99,[10,40,0,0,0,0,0,0,0],False),(191,100,[20,60,5,0,0,0,0,0,0],False)],
    [1,1],[30,100,5,0,0,0,0,0,0],(105,2,1,2,1,0),0,[])]
 for label,bound,editor,population,records,roads,vector,counters,positive,tutorials in specs:
  c=Case(0x45d630,label,[PLAYER]);add_food(c,bound,editor,population)
  for i,record in enumerate(records,1):food_record(c,i,*record)
  if tutorials:c.put(0xe38ebc,0)
  observe=[(0xeaac28,4),(0xea41dc+OFF,4),(0xea438c+OFF,4),(0xea5e20+OFF,4),
   (0xea5e24+OFF,4),(0xea5e28+OFF,4),(0x7027ba,1),(0x7027bf,1)]
  c.start(observe);c.endpoint('granary-road',0x4806a0,2,'full32')
  c.endpoint('tutorial',0x417ae0,1)
  for at in [0xea41dc+OFF,0xea41e0+OFF,0xea4388+OFF,0xea438c+OFF,0xea5e20+OFF,0xea5e24+OFF,0xea5e28+OFF]:c.put(at,0)
  for i in range(9):c.put(STOCK+i*4,0)
  road_index=0;full=0;under100=0;under50=0;withstock=0;running=[0]*9
  for id_,record in enumerate(records,1):
   kind,grade,stocks,house=record
   if house:continue
   row=BUILDINGS+id_*264;c.put(row+68,0,1)
   answer=roads[road_index];road_index+=1;c.event('granary-road',[-3,7],answer)
   if not answer:continue
   c.put(row+68,1,1)
   if grade<100:under100+=1;c.put(0xea5e20+OFF,under100)
   if grade<50:
    under50+=1;c.put(0xea5e24+OFF,under50)
    if sum(stocks[1:])>0:withstock+=1;c.put(0xea5e28+OFF,withstock)
   else:
    for i,value in enumerate(stocks):running[i]+=value;c.put(STOCK+i*4,running[i])
    full+=1;c.put(0xea438c+OFF,full)
    if tutorials:
     for index in tutorials:
      c.event('tutorial',[index],mutations=[(0x7027b8+index,1,1),(0xe950dc,4,1)])
  assert [signed(c.get(STOCK+i*4))for i in range(9)]==vector
  total,types,ratio,full,under100,withstock_expected=counters
  # grade49 cases have one under50 granary; all other fixtures have none.
  low=1 if 'grade49'in label else 0
  expected_with_stock=positive if low else withstock_expected
  for at,value in [(0xea41dc,total),(0xea41e0,types),(0xea4388,ratio),(0xea438c,full),
   (0xea5e20,under100),(0xea5e24,low),(0xea5e28,expected_with_stock),
   (0xea5e1c,signed((population*25)&0xffffffff)//100 if population>=0 else -((-population*25)//100))]:c.put(at+OFF,value)
  c.c['actual_children']=['004cde60 signed percentage','004c5340 flags0=>25','004cde40 wrapped product/100']
  c.c['explicit_food_oracle']=dict(resource_vector=vector,total=total,types=types,ratio=ratio,granaries=full,under100=under100,under50=low,with_stock=expected_with_stock)
  out.append(c.end())
 return out


def granary_fixture(c):
 c.block(BUILDINGS-264,6*264,0);c.block(POLICY,4*196,0)
 for at,size in [(0x784bbc,4),(0x78d9a0,4),(0x78d8d4,12),(0xd3b4e0,8),(0xd3b4f8,8)]:c.block(at,size)
 c.put(0x784bbc,0);c.put(0x78d9a0,0);c.put(0xd3b4e0,0x11223344);c.put(0xd3b4e4,0x55667788)


def granary_record(c,id_,slot,x,y,region=5,network=7):
 row=BUILDINGS+id_*264;c.put(row+193,slot,1);c.put(row+20,network,1)
 c.put(row+38,x,2);c.put(row+40,y,2);c.put(row+32,region,2)
 return row


def source_cases():
 out=[]
 specs=[('no-gather-preserves-output',False,0,[],[]),('editor-preserves-output',True,1,[],[]),
  ('zero-count-publishes-building0',True,0,[],[]),('network-reject-publishes-building0',True,0,[(1,9,10,5,6,500)],[]),
  ('small-stock-double-and-strict-tie',True,0,[(1,11,11,5,7,400),(3,12,10,5,7,401)],[1,11,11,2,0]),
  ('large-stock-wins-unweighted',True,0,[(1,11,11,5,7,400),(3,11,10,5,7,401)],[3,11,10,1,0]),
  ('signed-source-id-and-region-minus1',True,0,[(-1,-8,-9,32767,7,500)],[-1,-8,-9,18,19])]
 for label,gather,editor,sources,expected in specs:
  c=Case(0x45de80,label,[2],0xffffffff);granary_fixture(c)
  dest=granary_record(c,2,1,10,99,-1 if label.startswith('signed')else 5)
  granary_record(c,0,0,-22,-33)
  if gather:c.put(POLICY+196+1,2,1)
  c.put(0x784bbc,editor);c.put(0x78d9a0,len(sources))
  for i,(id_,x,y,region,network,stock)in enumerate(sources):
   row=granary_record(c,id_,2,x,y,region,network);c.put(row+0x58,stock,2);c.put(0x78d8d4+i*2,id_,2)
  c.start([])
  if not gather or editor:answer=0
  else:
   answer=expected[0]if expected else 0
   x,y=(expected[1],expected[2])if expected else(-22,-33)
   c.put(0xd3b4e0,y);c.put(0xd3b4e4,x)
   if expected:c.put(0xd3b4fc,expected[3]);c.put(0xd3b4f8,expected[4])
  c.c['actual_children']=['0045dc50 Chebyshev distance; destination X deliberately supplied twice']
  out.append(c.end(answer))
 return out


def take_cases():
 out=[]
 specs=[('no-eligible-resource',0,1000,[0]*9,0,0),
  ('largest-stock-clamped800',1000,1000,[100,0,0,0,0,900,0,0,0],5,800),
  ('first-equal-stock-and-destination-capacity',1000,300,[100,0,500,500,0,0,0,0,0],2,300),
  ('negative-free-capacity-returns-signed',1000,-7,[100,0,500,0,0,0,0,0,0],2,-7),
  ('negative-source-stocks-ignored',1000,1000,[100,0,-5,0,0,0,0,0,0],0,0),
  ('source-equals-destination',1000,1000,[100,0,500,0,0,0,0,0,0],0,0)]
 for label,counter,capacity,stocks,chosen,amount in specs:
  source=2 if label=='source-equals-destination'else 1
  c=Case(0x45e150,label,[2,source],0xffff);granary_fixture(c)
  c.block(0xeaac28,4);c.put(0xeaac28,PLAYER);c.block(0x78d99c,4);c.put(0x78d99c,0xdeadbeef)
  c.block(STOCK,36,0)
  granary_record(c,2,1,10,99);src=granary_record(c,source,1 if source==2 else 2,11,12)
  if label!='no-eligible-resource':
   for resource in range(1,9):c.put(POLICY+196+resource,2,1)
  for i,value in enumerate(stocks):c.put(src+0x56+i*2,value,2);c.put(STOCK+i*4,counter)
  c.put(BUILDINGS+2*264+0x56,capacity,2)
  c.start([]);c.put(0x78d99c,chosen)
  if amount>0:
   c.put(src+0x56+chosen*2,stocks[chosen]-amount,2)
   c.put(src+0x56,stocks[0]+amount,2);c.put(STOCK+chosen*4,counter-amount)
  c.c['actual_children']=['0045e360 withdrawal; its ignored return does not replace selected amount']
  c.c['logical_result_contract']='Native caller stores AX; low16 signed amount is compared. Actual selected amount is independent of child residual EAX.'
  out.append(c.end(amount))
 return out


def cases(registry=None):
 out=manager_cases()+calendar_cases()+food_cases()+source_cases()+take_cases()
 if registry is None:return out
 members={int(m['address'],16):m for m in registry['members']}
 out=[c for c in out if c['entry']in members]
 for c in out:c['expected_invocations']=1+sum(bool(v['named'])+bool(v['canonical'])for v in members[c['entry']]['providers'].values())
 return out


definitions=cases
