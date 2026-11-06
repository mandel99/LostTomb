"""Authored successful-home 4A2430 state cases. No binding or execution."""
from compiled_callbacks.callback_cases import Case as CallbackCase, actor, frame_byte, sprite_value

ENTRY=0x4a2430;CURRENT=0xd056f8;HOME=0x7999d8+264
THEME=0x784cb0;ORIENTATION=0xe94e00;CAMERA=0xbca2e0
ROUTE_OWNER=0xd04e58+10
PALETTES=((0xdf,0xe0,0xe1,0xdf,0xdf,0xe1,0xe1),
          (0x265,0x266,0x267,0x26b,0x26a,0x269,0x268),
          (0x54,0x55,0x56,0x5a,0x59,0x58,0x57))


def signed(value,bits):
 value&=(1<<bits)-1
 return value-(1<<bits)if value&(1<<(bits-1))else value


class Case(CallbackCase):
 def __init__(self,label,mode,timer=6,theme=0,frame=4):
  super().__init__(ENTRY,label)
  self.c['id']='c7-hunter-main/'+label
  self.block(THEME,4);self.put(THEME,0x55667700|theme)
  self.block(0xbca2d8,4);self.put(0xbca2d8,0x1820)
  self.block(ROUTE_OWNER-2,6,0xa5);self.put(ROUTE_OWNER,1,2)
  for id_ in(1,2):
   a=actor(id_)
   for off,width,value in[(16,1,12),(18,1,5),(58,2,5 if id_==1 else 0),
      (62,2,0),(94,2,2),(312,4,0)]:self.put(a+off,value,width)
  self.put(actor(1)+56,mode,1);self.put(actor(1)+54,timer,2)
  self.put(actor(1)+1,frame,1)
  for off,value in[(38,40),(40,50),(60,65500)]:self.put(HOME+off,value,2)
  for palette in PALETTES:
   for index in palette:
    at=0xf55bb4+index*8;self.put(at,sprite_value(at))
  self.actual=[];self.query_seed=False
  self.c['budget']=4000000

 def begin(self,empty_query=False,empty_queues=False):
  if empty_queues:
   for at in range(0xeaaf68,0xeadb28,224):
    self.block(at,4,0)
  offsets=((1,1),(4,4),(12,1),(14,1),(17,1),(20,2),(22,2),
   (36,2),(38,2),(54,2),(56,1),(58,2),(94,2),(101,1),(114,1),(312,4))
  self.start([(CURRENT,4),(THEME,4),(ORIENTATION,4),(ROUTE_OWNER,2),(HOME+60,2)]+
   [(actor(id_)+off,width)for id_ in(1,2)for off,width in offsets])
  if empty_query:
   # Native4A2320 tests the status BYTE before any other row field. Rows1/2
   # are already explicitly inactive; every other scanned row is seeded0.
   # These input-only cells are deliberately outside allowed write_ranges.
   assert self.get(actor(1)+14,1)==self.get(actor(2)+14,1)==0
   self.c['stores'] += [(actor(id_)+14,b'\0')for id_ in range(3,2000)]
   self.query_seed=True
  a=actor(1)
  self.put(a+114,3,1);self.put(a+12,0,1);self.put(a+101,0,1)
  frame=(self.get(a+1,1)+1)&255
  self.wrap=frame>=12
  self.put(a+1,0 if self.wrap else frame,1)

 def move(self,direction,mutations=()):
  self.endpoint('move',0x4a5da0,2)
  self.event('move',[1,0x76543210],mutations=[(actor(1)+17,1,direction),*mutations])

 def reset(self,id_=None):
  id_=self.get(CURRENT)if id_ is None else id_;a=actor(id_)
  route=signed(self.get(a+58,2),16)
  if route>0:
   assert route==5,'This bounded recipe seeds one route-owner cell'
   if signed(self.get(ROUTE_OWNER,2),16)==id_:self.put(ROUTE_OWNER,0,2)
   self.put(a+58,0,2)
  self.actual.append('004b3250')

 def empty_search(self):
  assert self.query_seed
  self.actual.append('004a2320 (1999 inactive status BYTEs; no candidate)')

 def home_goal(self,id_=1):
  self.put(actor(id_)+36,self.get(HOME+38,2),2)
  self.put(actor(id_)+38,self.get(HOME+40,2),2)

 def render(self,shot_frame=0):
  a=actor(self.get(CURRENT));mode=self.get(a+56,1)
  direction=signed(self.get(a+17,1),8)-signed(self.get(CAMERA),32)
  if direction<0:direction+=8
  self.put(ORIENTATION,direction)
  theme=self.get(THEME,1);walk,death,shoot,pose,timer,pose12,pose_other=PALETTES[0 if theme==0 else 1 if theme==1 else 2]
  group=walk;delta=self.get(a+1,1)*8+direction
  if mode==6:return
  if mode==14:self.put(a+4,0);return
  if mode==4:
   value=signed(self.get(a+54,2),16)
   index=abs(value)//2*(-1 if value<0 else 1)
   group=death;delta=frame_byte(index)
  elif mode==5:group=pose12 if self.get(a+16,1)==12 else pose_other
  elif mode==7:
   group=pose if self.get(a+16,1)==12 else walk;delta=direction
  elif mode==10:group=timer;delta=signed(self.get(a+54,2),16)*8+direction
  elif mode==12:group=pose
  elif mode==15:group=shoot;delta=shot_frame*8+direction
  self.put(a+4,sprite_value(0xf55bb4+group*8)+delta)

 def finish(self,shot_frame=0,early=False):
  if not early:self.render(shot_frame)
  self.c['actual_children']=list(dict.fromkeys(self.actual))
  self.c['focal_domain']='Successful-home4A2430 state graph; declared external child responses only.'
  return self.end()


def simple_cases():
 out=[];a=actor(1)
 c=Case('death-negative-timer-truncates-toward-zero',4,-3,frame=255)
 c.put(a+17,0x80,1);c.begin();c.endpoint('death',0x4b1d50,1)
 c.event('death',[1]);out.append(c.finish())
 for pose,theme in[(12,1),(3,2)]:
  c=Case(f'state5-pose{pose}-theme{theme}',5,theme=theme);c.put(a+16,pose,1)
  c.begin();c.endpoint('state5',0x4b17d0,1);c.event('state5',[1]);out.append(c.finish())
 c=Case('state5-current-reload-to-state6-alternate',5)
 c.put(actor(2)+56,6,1);c.begin();c.endpoint('state5',0x4b17d0,1)
 c.event('state5',[1],mutations=[(CURRENT,4,2)]);out.append(c.finish())
 c=Case('state6-wait-and-signed-orientation-preserves-image',6)
 c.put(a+17,0xfe,1);c.begin();c.put(a+101,1,1);out.append(c.finish())
 for timer in(99,100):
  c=Case(f'state7-timer-{timer}-actual-empty-queues',7,timer,theme=1)
  c.begin(empty_queues=timer==100);c.put(a+1,0,1);c.put(a+54,timer+1,2)
  if timer==100:
   c.put(a+54,0,2);c.put(a+56,8,1);c.actual.append('004a35e0 (50 zero queue counts; AL=false)')
  out.append(c.finish())
 for timer in(2,1):
  c=Case(f'state14-timer-{timer}',14,timer,theme=2)
  c.begin();c.put(a+101,1,1);c.put(a+54,timer-1,2)
  if timer==1:c.put(a+56,8,1);c.put(a+1,0,1)
  out.append(c.finish())
 return out


def waiting_cases():
 out=[];a=actor(1)
 c=Case('state8-positive-timer-restores-fallback-direction',8,2)
 c.begin();c.put(a+54,1,2);c.put(a+17,5,1);out.append(c.finish())
 for near in(False,True):
  c=Case('state8-empty-search-'+('already-home-early-return'if near else'home-goal-actual-route-clear'),8,1)
  if near:c.put(a+20,40,2);c.put(a+22,50,2)
  c.begin(empty_query=True);c.put(a+54,0,2);c.put(a+17,5,1);c.empty_search()
  c.put(a+56,13,1);c.home_goal()
  if near:c.put(a+101,1,1);c.put(a+14,2,1)
  else:c.reset()
  out.append(c.finish(early=near))
 return out


def moving_cases():
 out=[];a=actor(1)
 for direction in(9,10,1,8):
  c=Case(f'state9-movement-direction-{direction}',9,theme=1)
  c.begin(empty_query=direction==8);c.move(direction)
  if direction==9:c.reset();c.put(a+56,8,1);c.put(a+54,20,2)
  elif direction==10:c.put(a+14,2,1)
  elif direction==8:c.empty_search();c.put(a+56,13,1);c.home_goal()
  out.append(c.finish())
 for theme,timer in[(0,0),(1,17),(2,18)]:
  c=Case(f'state10-theme{theme}-timer{timer}',10,timer,theme)
  c.begin();c.put(a+54,timer+1,2)
  if timer+1>(18 if theme else 0):c.home_goal();c.put(a+56,12,1);c.reset()
  out.append(c.finish())
 for mismatch in(False,True):
  c=Case('state11-contact-'+('tile-mismatch'if mismatch else'successful-mode12-capture'),11,theme=2)
  c.put(actor(2)+56,12,1);c.put(actor(2)+14,1,1)
  c.put(actor(2)+32,2 if mismatch else 1)
  c.begin();c.move(8)
  if mismatch:c.put(a+56,8,1);c.put(a+54,20,2)
  else:c.put(actor(2)+14,2,1);c.put(a+56,10,1);c.put(a+54,0,2)
  out.append(c.finish())
 c=Case('state11-blocked-direction9-clears-timer-and-route',11)
 c.begin();c.move(9);c.reset();c.put(a+56,8,1);c.put(a+54,0,2);out.append(c.finish())
 for direction in(8,9):
  c=Case(f'state12-direction{direction}-'+('WORD-inventory-wrap'if direction==8 else'route-clear-retains-mode12'),12,theme=2)
  c.begin();c.move(direction)
  if direction==8:c.put(a+14,2,1);c.put(HOME+60,65500+100,2)
  else:c.reset()
  out.append(c.finish())
 c=Case('state13-arrival-resets-route-deletes-and-zeroes-timer',13)
 c.begin();c.move(8);c.reset();c.put(a+14,2,1);c.put(a+54,0,2);out.append(c.finish())
 c=Case('state13-frame-wrap-actual-empty-search-keeps-mode13',13,frame=11)
 c.begin(empty_query=True);c.move(1);c.empty_search();out.append(c.finish())
 return out


def projectile(c,args):
 c.endpoint('projectile',0x4b45c0,6)
 c.eps['projectile'].update(compiled_frame_symbols=['restore_civic_C_004b45c0'],
  compiled_frame_dispatcher='restore_actor_civic_dispatch',compiled_frame_return_sites=[0x4a2925])
 c.event('projectile',args)


def shooting_cases():
 out=[];a=actor(1);b=actor(2)
 c=Case('state15-oldtimer9-sound-reloads-current-before-projectile',15,9)
 c.put(b+56,15,1);c.put(b+54,3,2);c.put(b+36,-7,2);c.put(b+38,9,2)
 c.begin();c.endpoint('sound',0x53b4a0,3);c.event('sound',[1,16,2],mutations=[(CURRENT,4,2)])
 projectile(c,[2,200,-12,-7,9,74]);c.put(b+54,4,2);out.append(c.finish(9))
 c=Case('state15-three-independent-theme-reloads',15,9)
 c.begin();c.endpoint('sound',0x53b4a0,3)
 c.event('sound',[1,16,2],mutations=[(THEME,1,1),(CURRENT,4,2)])
 c.event('sound',[2,14,2],mutations=[(THEME,1,2),(CURRENT,4,1)])
 c.event('sound',[1,15,2]);projectile(c,[1,200,-12,20,30,74])
 c.put(a+54,10,2);out.append(c.finish(9))
 c=Case('state15-oldtimer12-clamps-render-frame-only',15,12,theme=1)
 c.begin();c.put(a+54,13,2);out.append(c.finish(0))
 c=Case('state15-timer301-transitions-to-wait-without-shot',15,300,theme=2)
 c.begin();c.put(a+54,301,2);c.put(a+56,8,1);out.append(c.finish())
 return out


def cases(registry=None):
 out=simple_cases()+waiting_cases()+moving_cases()+shooting_cases()
 if registry is None:return out
 members={int(m['address'],16):m for m in registry['members']}
 if ENTRY not in members:return []
 for c in out:c['expected_invocations']=1+sum(bool(p['named'])+bool(p['canonical'])for p in members[ENTRY]['providers'].values())
 return out


definitions=cases
