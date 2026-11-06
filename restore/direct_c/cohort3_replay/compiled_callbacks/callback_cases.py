"""Bounded no-argument actor callback recipes; no image binding/execution."""
from compiled_flood_regions.cohort7_compiled_acceptance_cases import Case as BaseCase

TARGETS=(0x489ca0,0x48a260,0x493ec0,0x494010,0x496f20,0x49b390)
CURRENT=0xd056f8;CAMERA=0xbca2e0;ORIENTATION=0xe94e00;HOME=0x7999d8+264
SPEED=0x76543210;FRAME=0x5d048c
GROUPS=((0xec,0xed),(0xf9,0xfa),(0xe2,0xe3),(0xe8,0xe9),(0xf7,0xf8))
FIXED=(0xf56324,0xf5632c,0xf55d7c,0xf55d84,0xf55d5c,0xf55d64,
       0xf55cfc,0xf55d04,0xf5613c,0xf571bc,0xf56144,0xf571c4,0xf56e14)


def actor(id_):return 0xbcd5f8+id_*388
def frame_byte(index):return(0x30+index+16)&255
def sprite_value(at):return 0x10000000+(at-0xf55bb4)*13
def orient(direction,camera=3):
 if 127<direction<=255:direction-=256
 value=direction-camera
 return value+8 if value<0 else value


class Case(BaseCase):
 def __init__(self,entry,label,current=1):
  super().__init__(entry,label,0x89abcdef,named=[],result=None)
  self.c['id']=f'c7-callbacks/{entry:08x}/'+label;self.c.pop('expected_invocations')
  self.c.update(compare_x87_values=True,compare_x87_control=True,initial_fpcw=0x27f,
   initial_x87=['0000000000000080ff3f','000000000000008000c0'])
  self.block(actor(1)-8,2*388+16,0xa5);self.block(HOME,264,0)
  for at,width,value in [(CURRENT,4,current),(CAMERA,4,3),(ORIENTATION,4,0x12345678),
   (0xbcb9a0,4,0x778899aa),(0xeaac28,4,2),(0x978d70,4,1),
   (0xd3b4e0,4,0x11112222),(0xd3b4e4,4,0x33334444),
   (0x78d7ac,4,100),(0x78d7b0,4,0),(0xea3fe8+2*0x49d8,4,0x0008fff9)]:
   self.block(at,width);self.put(at,value,width)
  self.block(0x5dc23c,16,0)
  for i in range(16):self.put(0x5dc23c+i,0x10+i,1)
  self.block(FRAME-16,144,0)
  for i in range(-16,128):self.put(FRAME+i,frame_byte(i),1)
  self.block(0xf55bb4,0x1700,0xa5)
  for at in FIXED+tuple(0xf55bb4+g*8 for pair in GROUPS for g in pair):self.put(at,sprite_value(at))
  self.block(0xf53dd8,20,0);self.put(0xf53dd8,3)
  for i,value in enumerate((1,15,99)):self.put(0xf53ddc+i*4,value)
  for id_ in (1,2):
   a=actor(id_)
   for off,width,value in [(1,1,4 if id_==1 else 9),(4,4,0xdead0000+id_),
    (10,1,7 if id_==1 else 8),(12,1,0x66),(14,1,0),(17,1,1 if id_==1 else 6),
    (20,2,200),(22,2,-12),(32,4,id_),(36,2,20),(38,2,30),(54,2,6),
    (56,1,0),(57,1,7),(64,1,1),(66,2,0x1234),(68,2,150),
    (74,2,-4),(76,2,5),(90,2,1),(92,2,1),(100,1,80),(101,1,0x99),
    (108,1,0x88),(109,1,0x77),(114,1,0x55),(384,4,0xcafe0000+id_)]:self.put(a+off,value,width)
   self.block(0x7090ec+(7 if id_==1 else 8)*56,4);self.put(0x7090ec+(7 if id_==1 else 8)*56,SPEED+id_-1)
   self.block(0xaf598c+id_*4,4,0);self.block(0xadc36c+id_,1,0)
  for off,width,value in [(0,1,1),(3,1,3),(4,1,0),(5,1,1),(6,2,-3),(8,2,7),
   (16,2,60),(18,2,2),(28,2,90),(30,2,10),(32,2,5),(42,2,1),(46,2,1),(74,2,0),(135,1,0),(180,2,1)]:self.put(HOME+off,value,width)
  self.block(0x70ce14+2*88,4);self.put(0x70ce14+2*88,100)

 def begin(self):
  self.start([(CURRENT,4),(actor(1)+56,1),(actor(1)+68,2),(actor(1)+114,1),
   (actor(1)+4,4),(actor(2)+56,1),(actor(2)+4,4),(ORIENTATION,4),
   (HOME+28,2),(HOME+46,2)])

 def state_boundary(self):self.endpoint('state',0x493610,0)

 def image(self,id_,table,direction=None,frame=None):
  a=actor(id_);direction=orient(self.get(a+17,1)if direction is None else direction)
  self.put(ORIENTATION,direction)
  if frame is None:self.put(a+4,sprite_value(table)+self.get(a+1,1)*8+direction)
  else:self.put(a+4,sprite_value(table)+frame_byte(frame))


def seed_small(c,rate):
 a=actor(1);c.put(a+114,1,1);c.put(a+12,0,1);c.put(a+66,rate,2)


def trivial_cases():
 out=[]
 for id_ in(1,2):
  c=Case(0x49b390,f'current-{id_}-exact-three-BYTE-stores',id_);c.begin();a=actor(id_)
  c.put(a+114,0,1);c.put(a+101,1,1);c.put(a+12,1,1);out.append(c.end())
 return out


def guard_cases():
 out=[]
 for label,active,frame,timer,mode,real,switch in [
  ('actual-state-default',1,10,6,3,True,False),('invalid-home-still-renders',0,11,6,0,False,False),
  ('negative-death-timer-and-frame-wrap',1,255,-3,4,False,False),
  ('state-global-reload-to-alternate-actor',1,4,6,0,False,True)]:
  c=Case(0x496f20,label);a=actor(1);c.put(HOME,active,1)
  c.put(a+1,frame,1);c.put(a+54,timer,2);c.put(a+56,mode,1)
  if mode==4:c.put(a+17,0x80,1)
  c.begin();seed_small(c,128)
  if not active:c.put(a+14,2,1)
  c.put(a+1,0 if(frame+1)&255>=12 else(frame+1)&255,1)
  if real:c.put(a+56,1,1);c.put(a+68,128,2)
  else:
   c.state_boundary();c.event('state',[],mutations=[(CURRENT,4,2)]if switch else[])
  target=2 if switch else 1
  c.image(target,0xf5632c if mode==4 else 0xf56324,frame=-1 if mode==4 else None)
  c.c['actual_children']=['00493610 default branch']if real else[]
  out.append(c.end())
 return out


def resource_cases():
 out=[]
 for label,mode,timer,stock,valid,real,switch in [
  ('actual-state-default-and-invalid-building',0,150,0,False,True,False),
  ('actual-resource-queries-zero-reset-duty',1,96,0,True,False,False),
  ('actual-resource-query-positive-retains-duty',1,100,7,True,False,False),
  ('state-global-reload-after-original-home-query',1,95,0,True,False,True)]:
  c=Case(0x493ec0,label);a=actor(1);c.put(a+56,mode,1);c.put(a+68,timer,2)
  c.put(HOME,1 if valid else 0,1);c.put(HOME+16,70,2);c.put(HOME+0x56,stock,2)
  # Both real query helpers otherwise see zero WORD stocks (including index-1).
  c.begin();seed_small(c,384)
  if not valid:c.put(a+14,2,1)
  c.put(a+1,5,1)
  if mode==1 and timer>=96 and stock==0:c.put(a+68,384,2)
  if real:c.put(a+56,1,1);c.put(a+68,384,2)
  else:
   c.state_boundary();c.event('state',[],mutations=[(CURRENT,4,2)]if switch else[])
  c.image(2 if switch else 1,0xf55d7c)
  c.c['actual_children']=['004578b0','004579d0']if mode==1 else['00493610 default branch']
  out.append(c.end())
 return out


def classifier_cases():
 out=[]
 for label,active,guard,owner in [('building-inactive',0,0,1),('nonzero-WORD-guard',1,0x100,1),('wrong-primary-owner',1,0,2)]:
  c=Case(0x494010,label);c.put(HOME,active,1);c.put(HOME+74,guard,2);c.put(HOME+42,owner,2)
  c.begin();seed_small(c,384);c.put(actor(1)+14,2,1);out.append(c.end())
 for category in range(5):
  c=Case(0x494010,f'actual-classifier-group-{category}-state8-skips-state-child')
  a=actor(1);c.put(HOME+16,60+category,2);c.put(a+56,8,1);c.put(a+1,11,1)
  c.begin();seed_small(c,384);c.put(a+1,0,1);c.put(a+56,1,1)
  c.image(1,0xf55bb4+GROUPS[category][0]*8);c.c['actual_children']=['004c5120'];out.append(c.end())
 c=Case(0x494010,'actual-classifier-unsigned-default-and-death-image');a=actor(1)
 c.put(HOME+16,999,2);c.put(a+56,4,1);c.put(a+54,-3,2)
 c.begin();seed_small(c,384);c.put(a+1,5,1);c.state_boundary();c.event('state',[])
 c.image(1,0xf55bb4+GROUPS[0][1]*8,frame=-1);c.c['actual_children']=['004c5120'];out.append(c.end())
 c=Case(0x494010,'actual-state-default-before-actual-classifier');a=actor(1);c.put(a+56,0,1)
 c.begin();seed_small(c,384);c.put(a+1,5,1);c.put(a+56,1,1);c.put(a+68,384,2)
 c.image(1,0xf55bb4+GROUPS[0][0]*8);c.c['actual_children']=['00493610 default branch','004c5120'];out.append(c.end())
 return out


def human_cases():
 out=[]
 for entry in(0x489ca0,0x48a260):
  immigrant=entry==0x489ca0
  specs=[('land-default',0,False,6),('water-default',0,True,6),
   ('water-death-short-real-AL-boundary',4,True,32),('water-death-long-clamp',4,True,80),
   ('land-death-negative-timer',4,False,-3),('state5-global-actor-reload',5,False,6),
   ('state8-timer-not-triggered',8,False,5 if immigrant else 0),
   ('state9-home-reject'if immigrant else'state9-arrival-player-target',9,False,6),
   ('state10-capacity-clamp-and-publish'if immigrant else'state10-navigation-deletes',10,False,6)]
  for label,mode,on_water,timer in specs:
   c=Case(entry,label);a=actor(1);c.put(a+56,mode,1);c.put(a+54,timer,2)
   c.put(0xaf598c+4,4 if on_water else 0)
   if mode==4 and on_water and timer<33:c.put(0x78d7b0,1);c.put(0x78d7ac,201)
   if not immigrant and mode==9:c.put(a+64,0,1)
   c.begin();c.put(a+114,0,1);c.put(a+384,0)
   c.put(a+1,5,1);target=1
   if mode==0:c.put(a+56,8,1)
   elif mode==4:
    c.endpoint('death',0x4b1d50,1);c.event('death',[1])
   elif mode==5:
    c.endpoint('state5',0x4b17d0,1);c.event('state5',[1],mutations=[(CURRENT,4,2)])
    target=2
   elif mode==8:
    c.put(a+101,1,1);c.put(a+1,0,1);c.put(a+54,timer+(-1 if immigrant else 1),2)
   elif mode==9 and immigrant:
    c.put(a+101,0,1);c.endpoint('navigation',0x4a5da0,2)
    c.event('navigation',[1,SPEED],mutations=[(a+17,1,10)])
    c.put(HOME+46,0,2);c.put(HOME+32,0,2);c.put(a+14,2,1)
   elif mode==9:
    c.put(a+12,1,1);c.put(a+101,1,1);c.endpoint('arrived',0x4b3ff0,2,'full32')
    c.event('arrived',[1,1],1);c.put(a+56,10,1);c.put(a+36,-7,2);c.put(a+38,8,2)
    c.put(a+68,0,2);c.put(a+57,15,1);c.put(a+101,0,1)
   elif immigrant:
    c.put(a+12,1,1);c.put(a+101,1,1);c.endpoint('arrived',0x4b3ff0,2,'full32');c.event('arrived',[1,1],1)
    c.put(a+14,2,1);c.put(a+100,10,1);c.put(HOME+28,100,2);c.put(HOME+30,0,2)
    c.endpoint('publish-population',0x4e90f0,2);c.event('publish-population',[2,10])
    c.put(HOME+46,0,2);c.put(a+101,1,1)
   else:
    c.put(a+12,0,1);c.put(a+101,0,1);c.endpoint('navigation',0x4a5da0,2)
    c.event('navigation',[1,SPEED],mutations=[(a+17,1,9)]);c.put(a+14,2,1)
   a=actor(target);direction=orient(c.get(a+17,1));c.put(ORIENTATION,direction)
   current_mode=c.get(a+56,1)
   if current_mode==4:
    if on_water:
     if timer<33:c.put(a+4,sprite_value(0xf571c4)+timer//3+direction//8)
     else:c.put(a+4,sprite_value(0xf56e14)+22)
     if not immigrant:c.put(a+384,0)
    else:c.put(a+4,sprite_value(0xf55d64 if immigrant else 0xf55d04)+frame_byte(-1))
   elif on_water:
    c.put(a+4,sprite_value(0xf5613c)+(c.get(a+1,1)//3)*8+direction);c.put(a+384,0)
   else:
    c.put(a+4,sprite_value(0xf55d5c if immigrant else 0xf55cfc)+c.get(a+1,1)*8+direction)
    if immigrant:c.put(a+384,0)
   if not immigrant and current_mode==10:
    reverse=direction+4
    if reverse>=8:reverse-=8
    c.put(0xbcb9a0,reverse);c.put(a+108,0x10+reverse*2,1);c.put(a+109,0x11+reverse*2,1)
   c.c['actual_children']=['0047a1c0 AL-only boundary']if on_water and(timer<33 or current_mode!=4)else[]
   out.append(c.end())
 return out


def cases(registry=None):
 out=trivial_cases()+guard_cases()+resource_cases()+classifier_cases()+human_cases()
 if registry is None:return out
 members={int(m['address'],16):m for m in registry['members']};out=[c for c in out if c['entry']in members]
 for c in out:c['expected_invocations']=1+sum(bool(v['named'])+bool(v['canonical'])for v in members[c['entry']]['providers'].values())
 return out


definitions=cases
