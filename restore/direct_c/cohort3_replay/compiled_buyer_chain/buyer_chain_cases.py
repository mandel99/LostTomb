"""Bounded buyer/carrier/projectile recipes; no images are bound or run."""
from compiled_flood_regions.cohort7_compiled_acceptance_cases import Case as BaseCase
TARGETS=(0x496c70,0x496de0,0x49b400,0x4a2e90)
CURRENT=0xd056f8;HOME=0x7999d8+264;BUILDINGS=0x7999d8
PLAYER=2;OFF=PLAYER*0x49d8;AVAIL=0xea42f8+OFF
IMAGE=0x12000000
def actor(id_):return 0xbcd5f8+id_*388


class Case(BaseCase):
 def __init__(self,entry,label,args=(),result=None):
  super().__init__(entry,label,0x89abcdef,args,named=list(args),result=result)
  self.c['id']=f'c7-buyer-chain/{entry:08x}/'+label;self.c.pop('expected_invocations')
  self.c.update(compare_x87_values=True,compare_x87_control=True,initial_fpcw=0x27f,
   initial_x87=['0000000000000080ff3f','000000000000008000c0'])
  self.block(actor(0)-8,6*388+16,0)
  self.block(BUILDINGS,4*264,0)
  for at,width,value in [(CURRENT,4,1),(0xeaac28,4,PLAYER),(0xbca2e0,4,3),(0xe94e00,4,0x12345678),(0x784cb0,1,0)]:
   self.block(at,width);self.put(at,value,width)
  for id_ in range(6):
   a=actor(id_)
   for off,width,value in [(1,1,4 if id_!=3 else 7),(4,4,0xdead0000+id_),(10,1,39),(14,1,1),
    (17,1,1),(18,1,6),(20,2,-3),(22,2,7),(54,2,-3),(56,1,0),(57,1,0),
    (90,2,1),(101,1,0),(104,2,0),(122,1,1),(188,4,0x11112222),(384,4,0xaabbccdd)]:self.put(a+off,value,width)
  for at in(0xf55d34,0xf55d44,0xf55cec,0xf55cf4):self.block(at,4);self.put(at,IMAGE+(at&0xffff))
  self.block(0x5d048c-8,32,0)
  for i in range(-8,24):self.put(0x5d048c+i,0x40+i+8,1)

 def begin(self):
  self.start([(CURRENT,4),(actor(1)+14,1),(actor(1)+101,1),(actor(1)+104,2),
   (actor(1)+4,4),(actor(2)+56,1),(actor(3)+56,1),(HOME+0x58,2)])


def buyer_cases():
 out=[]
 for entry in(0x496c70,0x496de0):
  food=entry==0x496c70
  for label in('availability-full-DWORD-block','invalid-category-block','nonpositive-demand-order',
   'zero-stock','real-calculator-and-real-carrier-sender'):
   category=-1 if label=='invalid-category-block'else(0 if food else 8)
   resource=0xffffffff if category==-1 else(1 if food else 15)
   c=Case(entry,label,[1,2,3,category,10,3],0xffffffff)
   c.block(AVAIL-4,84,0)
   blocked='block'in label
   if blocked:c.put((AVAIL+resource*4)&0xffffffff,0x100)
   bazaar=BUILDINGS+2*264;src=BUILDINGS+3*264
   c.put(src+16,71 if food and label=='real-calculator-and-real-carrier-sender'else 73,2)
   if label=='nonpositive-demand-order':c.put(bazaar+0x56+(0 if food else 8)*2,800 if food else 400,2)
   c.put(src+0x56,100,2);c.put(src+0x58,250,2)
   c.block(0xea60ac+OFF,36,0);c.put(0xea60ac+OFF+4,10000)
   c.put(actor(1)+104,0,2);c.put(actor(1)+200,0xfffffff0)
   c.begin();c.endpoint('source-stock',0x45d510,2,'full32')
   c.endpoint('warehouse-withdraw',0x45b570,4,'full32')
   c.endpoint('create-carrier',0x4b4800,5,'full32')
   if blocked:answer=0
   elif label=='nonpositive-demand-order':
    if food:c.event('source-stock',[3,resource],1000)
    answer=0
   elif label=='zero-stock':c.event('source-stock',[3,resource],0);answer=0
   else:
    answer=1;loads=2 if food else 4
    if not food:
     c.event('source-stock',[3,resource],400)
     c.event('warehouse-withdraw',[PLAYER,3,resource,loads*100],0xdeadbeef)
    else:
     # Actual45E360:250>200; returned residual is ignored by sender.
     c.put(src+0x58,50,2);c.put(src+0x56,300,2);c.put(0xea60ac+OFF+4,9800)
    previous=1
    ids=[2,0]if food else[2,3,4,5]
    for child in ids:
     c.event('create-carrier',[PLAYER,66,-3,7,0],child)
     c.put(actor(child)+104,previous,2);c.put(actor(child)+122,category,1)
     c.put(actor(child)+90,1,2);previous=child
    c.put(actor(1)+200,(0xfffffff0+loads*100)&0xffffffff)
   c.c['actual_children']=['4963B0/496460/496550 calculators','496640 zero-predecessor assertion',
    '496660/4968A0 unrolled sender','45E360 food withdrawal on granary success']
   c.c['pending_domain']='Nonzero496640 predecessor is a genuine native infinite loop and is not executed.'
   out.append(c.end(answer))
 return out


def carrier_cases():
 out=[]
 for label,previous,previous_mode,status,kind,root_mode,follow in [
  ('missing-predecessor',0,0,1,39,0,False),('dead-predecessor',2,4,1,39,4,False),
  ('lost-predecessor-returns-WORD-stock',2,0,0,39,0,False),
  ('wrong-predecessor-type-root-waits',2,0,1,40,7,False),
  ('root-wait7-zeroes-frame',2,7,1,39,7,False),('root-stop6-propagates-wait',2,6,1,39,6,False),
  ('follow-reloads-current-actor',2,0,1,39,0,True)]:
  c=Case(0x49b400,label);a=actor(1);c.put(a+104,previous,2)
  c.put(actor(previous)+56,previous_mode,1);c.put(actor(previous)+14,status,1)
  c.put(actor(previous)+10,kind,1)
  root=3 if kind==40 else previous
  if kind==40:c.put(actor(previous)+104,3,2)
  c.put(actor(root)+56,root_mode,1);c.put(actor(root)+10,39,1)
  c.put(HOME+0x58,65500,2);c.begin()
  c.put(a+101,0,1);c.put(a+114,1,1);c.put(a+1,5,1);c.put(a+384,0)
  if previous<=0 or previous_mode==4:c.put(a+14,2,1)
  elif status!=1:c.put(a+14,2,1);c.put(HOME+0x58,64,2)
  elif kind not in(39,66):c.put(a+14,2,1)
  if follow:
   c.endpoint('follow',0x4b22e0,3);c.event('follow',[1,previous,1],mutations=[(CURRENT,4,3)])
  target=3 if follow else 1;ta=actor(target)
  if root_mode==7:c.put(ta+1,0,1)
  if root_mode==6:c.put(ta+101,1,1)
  direction=1-3+8;c.put(0xe94e00,direction)
  c.put(ta+4,IMAGE+(0xf55d34&0xffff)+c.get(ta+1,1)*8+direction)
  c.c['actual_children']=['0049b3c0 bounded root traversal']
  out.append(c.end())
 return out


def projectile_cases():
 out=[]
 for label,arrival,results,prey_mode,age,overlay in [
  ('no-target-not-arrived',0,[0,0,0],0,0,0),
  ('full-DWORD-arrived-releases-shooter',0x100,[0,0,0],0,104,0),
  ('second-query-hit-rearms-shooter',0,[0,2],0,0,0),
  ('already-caught-target-no-hit',0,[2],12,0,0),
  ('signed-age-wrap-and-overlay',0,[0,0,0],0,127,1)]:
  c=Case(0x4a2e90,label);a=actor(1);prey=actor(2);shooter=actor(3)
  c.put(a+90,3,2);c.put(a+1,4,1);c.put(a+57,age,1);c.put(a+14,0,1)
  c.put(prey+56,prey_mode,1);c.put(0x784cb0,overlay,1);c.begin()
  c.endpoint('arrived',0x4b3ff0,2,'full32');c.endpoint('target',0x4b57e0,2,'full32')
  c.put(a+12,1,1);c.put(a+57,(age+1)&255,1);c.put(a+1,0,1)
  if age==104:c.put(a+14,2,1)
  c.event('arrived',[1,3],arrival)
  for kind,result in zip((70,68,69),results):c.event('target',[1,kind],result)
  hit=results[-1]!=0 and prey_mode not in(4,12)
  if hit:
   c.put(prey+56,12,1);c.put(prey+54,0,2);c.put(shooter+94,2,2);c.put(shooter+56,11,1)
   c.put(shooter+36,-3,2);c.put(shooter+38,7,2);c.put(a+14,2,1)
  elif arrival:c.put(shooter+56,8,1);c.put(a+14,2,1)
  direction=1-(3<<2)+32;c.put(0xe94e00,direction)
  if not overlay:c.put(a+4,IMAGE+(0xf55d44&0xffff));c.put(a+384,0)
  else:
   c.put(a+4,IMAGE+(0xf55cec&0xffff)+direction)
   c.put(a+384,IMAGE+(0xf55cf4&0xffff)+direction)
  c.c['actual_children']=['0047d350 ignored no-argument sound leaf on hit']
  out.append(c.end())
 return out


def cases(registry=None):
 out=buyer_cases()+carrier_cases()+projectile_cases()
 if registry is None:return out
 members={int(m['address'],16):m for m in registry['members']};out=[c for c in out if c['entry']in members]
 for c in out:c['expected_invocations']=1+sum(bool(v['named'])+bool(v['canonical'])for v in members[c['entry']]['providers'].values())
 return out
definitions=cases
