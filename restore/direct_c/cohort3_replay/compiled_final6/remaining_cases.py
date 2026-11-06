"""Final six bounded focal recipes; raw C frames remain owned by real code."""
from compiled_callbacks.callback_cases import Case as CallbackCase,actor,frame_byte,sprite_value
from compiled_flood_regions.cohort7_compiled_acceptance_cases import Case as BaseCase
TARGETS=(0x488400,0x48e9e0,0x496140,0x4a2430,0x4cabd0,0x53f7d0)
CURRENT=0xd056f8;HOME=0x7999d8+264;MANAGER=0x32018010
NAME=0x32003000;BUFFER=0x32010010;COLOR=0x32012000;DECOY=0x32013000
GLOBAL,EXPLICIT=0xf55bb0,0x680000;SCRATCH=0x30f5c0;ALT=0x5e05ac
PATH=['stack-cstring','sprite-path',b'data\\clip.bin\0'.hex()]


class Case(CallbackCase):
 def __init__(self,entry,label,current=1):
  super().__init__(entry,label,current);self.c['id']=f'c7-final6/{entry:08x}/'+label
 def event(self,token,args,value=0,mutations=(),**fields):
  spec=self.eps[token];self.c['authored_endpoint_order'].append(token)
  self.c['authored_endpoint_observations'].append([[at,width,self.get(at,width)]for at,width in self.observe])
  self.c['expected_endpoint_counts'][token]+=1
  self.c['expected_endpoint_arguments'][token].append([x&0xffffffff if isinstance(x,int)else x for x in args])
  response=dict(value=value,**fields)
  if mutations:response['mutations']=list(mutations)
  spec['responses'].append(response)
  for at,width,new in mutations:self.put(at,new,width)


def actor_cases():
 out=[]
 specs=[('default-return-goal',0,6,0),('state6-transient-image',6,6,0),
  ('timer101-AL-false',7,100,0),('timer101-AL-true',7,100,1),
  ('outbound-direction9-return-goal',8,6,9),('outbound-direction8-buy',8,6,8),
  ('return-direction8-delete',9,6,8),('return-direction9-reset-route',9,6,9),
  ('state5-current-actor-reload',5,6,0)]
 for label,mode,timer,value in specs:
  c=Case(0x496140,label);a=actor(1)
  c.put(a+56,mode,1);c.put(a+54,timer,2);c.put(a+44,-20,2);c.put(a+46,21,2);c.put(a+94,3,2)
  c.put(HOME+44,1,2);c.put(HOME+182,0,2)
  for at in(0xf55d6c,0xf55d74):c.put(at,sprite_value(at))
  c.begin();c.put(a+114,1,1);c.put(a+12,0,1);c.put(a+66,800,2);c.put(a+1,5,1)
  target=1
  if mode==7:
   c.put(a+1,0,1);c.put(a+54,0,2);c.endpoint('timer-action',0x4a35e0,1,'al8')
   c.event('timer-action',[1],value)
   if not value:c.put(a+56,9,1)
  elif mode==5:
   c.endpoint('state5',0x4b17d0,1);c.event('state5',[1],mutations=[(CURRENT,4,2)]);target=2
  elif mode in(8,9):
   c.endpoint('navigation',0x4a5da0,2);c.event('navigation',[1,0x76543210],mutations=[(a+17,1,value)])
   if mode==8 and value==8:
    c.endpoint('buy',0x496060,3);c.event('buy',[1,1,3])
   elif mode==8:
    c.put(a+56,9,1);c.put(a+36,-20,2);c.put(a+38,21,2)
    c.endpoint('reset-route',0x4b3250,1);c.event('reset-route',[1])
   elif value==8:c.put(a+14,2,1)
   else:c.endpoint('reset-route',0x4b3250,1);c.event('reset-route',[1])
  elif mode==0:
   c.put(a+56,9,1);c.put(a+36,-20,2);c.put(a+38,21,2)
   c.endpoint('reset-route',0x4b3250,1);c.event('reset-route',[1])
  elif mode==6:c.put(a+4,0)
  c.image(target,0xf55d6c)
  out.append(c.end())
 return out


def civic_cases():
 out=[]
 for id_,bits in[(1,0x10004),(2,0x10104)]:
  c=Case(0x48e9e0,f'actual-terrain-teardown-current-{id_}',id_)
  c.put(0xaf598c+id_*4,bits);c.begin();c.put(actor(id_)+14,2,1)
  c.c['actual_children']=['004bd250 exact BYTE teardown store']
  c.c['focal_domain']='Entry terrain mask10004 only. Main2099-instruction civic state graph is pending.'
  out.append(c.end())
 for id_,home in[(1,1),(2,-1)]:
  c=Case(0x4a2430,f'actual-signed-home-reject-current-{id_}',id_)
  c.put(actor(id_)+90,home,2)
  if home==1:c.put(HOME,0,1)
  else:c.block(0x7999d8-264,264,0)
  c.begin();a=actor(id_);c.put(a+114,3,1);c.put(a+12,0,1);c.put(a+101,0,1);c.put(a+14,2,1)
  c.c['focal_domain']='Invalid-home early return only; successful-home657-instruction state graph remains pending.'
  out.append(c.end())
 return out


def city_cases():
 out=[]
 for mode in(1,8,0xffffffff):
  c=Case(0x488400,f'full-DWORD-mode-{mode:08x}-early-zero')
  c.block(0x78d7c4,4);c.put(0x78d7c4,mode);c.block(MANAGER-16,64)
  c.c.update(receiver=MANAGER,arguments=[1],named_arguments=[MANAGER,1],canonical_arguments=[MANAGER,1],
   native_cleanup=4,result_mask=0xffffffff)
  c.begin();c.c['focal_domain']='Mode neither0 nor7 returns full EAX0 before member children; production/destruction branches remain pending.'
  out.append(c.end(0))
 return out


def tooltip_cases():
 out=[];TEXT=0x32004000
 for label,mode,mx,my in [('near-default',0,50,100),('far-default',0,500,400),
  ('signed-left-mode45',0x2d,50,100),('center-mode42',0x2a,50,100)]:
  c=Case(0x4cabd0,label)
  for at,width,value in[(0xd3a6c0,4,0),(0xf56f84,4,0x44556677),(0xe92dfc,4,0),
   (0xe38dda,1,mode),(0xe20768,4,TEXT),(0xe38da4,4,0),(0xe38da8,4,0),
   (0xe38e28,4,mx),(0xe38e2c,4,my),(0xe38d9c,4,640),(0xe38d98,4,480),
   (0xd3b138,4,0),(0xd3b130,4,99),(0xd3b13c,4,99)]:
   if at!=0xf56f84:c.block(at,width)
   c.put(at,value,width)
  c.block(0xd3a6cc,28,0);c.put(0xd3a6d4,10);c.put(0xd3a6d8,20);c.put(0xd3a6e4,17)
  c.block(0xd3b164,8);c.block(0xd3b51c,2);c.block(TEXT,4);c.write(TEXT,b'tip\0')
  c.begin();c.endpoint('prepare-text',0x4cc110,2);c.endpoint('text',0x4c8070,6);c.endpoint('line',0x4cd510,5)
  c.put(0xd3b168,0x44556677);c.put(0xd3b164,0x44556677);c.put(0xd3a6d0,200)
  c.put(0xd3b130,0);c.put(0xd3b13c,30);c.put(0xd3b51c,0x1c82,2)
  c.event('prepare-text',[0x44,17]);c.event('text',[TEXT,25,15,180,1,0]);c.put(0xd3a6cc,10)
  y=my+20 if my<300 else my-220
  x=mx+40 if mx<200 else mx-62
  if mode==0x2d:x=mx-52
  elif mode==0x2a:x=mx+40 if mx<200 else mx-72;y=my-110
  c.put(0xd3a6d4,x);c.put(0xd3a6d8,y)
  # Real4CD680 border and4CD9D0 fill execute, isolated at typed line leaf.
  bx,by=max(0,y),max(0,x);right=bx+min(220,640-bx)-1;bottom=by+min(10,480-by)-1
  for args in[(bx,by,right,by,0x2c40),(bx,bottom,right,bottom,0x2c40),
   (bx,by,bx,bottom,0x2c40),(right,by,right,bottom,0x2c40)]:c.event('line',args)
  fx,fy=max(0,y+1),max(0,x+1);width=min(218,640-fx);height=min(8,480-fy)
  for row in range(fy,fy+height):c.event('line',[fx,row,fx+width-1,row,0x5dc8])
  c.event('text',[TEXT,y+5,x+7,180,0,0])
  c.c['actual_children']=['004cd680 border','004cd9d0 fill']
  c.c['focal_domain']='Default-tooltip positioning/render route. Raw mode1..4 and private BYTE output-pointer query paths remain pending.'
  out.append(c.end())
 return out


def sprite_cases():
 out=[]
 for receiver in(GLOBAL,EXPLICIT):
  for mode in('success','partial','zero-success','zero-zero','seek-fallback','negative-read','zero-rows','open-fail'):
   c=Case(0x53f7d0,('global'if receiver==GLOBAL else'explicit')+'/'+mode)
   rows=0 if mode=='zero-rows'else 2
   c.c.update(receiver=receiver,arguments=[NAME,BUFFER,4,rows,0x81234567],
    named_arguments=[receiver,NAME,BUFFER,4,rows,0x81234567],canonical_arguments=[],native_cleanup=20,result_mask=0xffffffff)
   c.block(NAME,9);c.write(NAME,b'clip.bin\0');c.block(BUFFER-16,96)
   c.block(COLOR,16);c.block(DECOY,16)
   for manager in(GLOBAL,EXPLICIT):c.block(manager+SCRATCH,4);c.put(manager+SCRATCH,COLOR if manager==receiver else DECOY)
   c.block(0xd3b5be,2);c.put(0xd3b5be,6,2)
   c.block(0x6018f8,6);c.write(0x6018f8,b'data\\\0')
   c.block(ALT,260);c.write(ALT,b'X:\\\0');c.block(0xe38e62,1);c.put(0xe38e62,ord('D'),1)
   c.begin()
   for token,address,arity,result in [('prepare',0x4ce310,2,'void'),('open',0x568eab,2,'full32'),
    ('seek',0x5662e3,3,'full32'),('read',0x5660ed,3,'full32'),('close',0x5647eb,1,'full32'),
    ('refresh',0x4ce100,1,'void'),('finish',0x4ce2d0,0,'void'),('update',0x4cdf20,2,'void'),('prompt',0x4ce9e0,0,'full32')]:
    c.endpoint(token,address,arity,result)
   for token in('prepare','open','refresh'):c.eps[token]['private_input_args']={0:dict(role='sprite-path',cstring_max=260)}
   c.event('prepare',[PATH,0x5e06e0])
   if mode=='open-fail':
    c.event('open',[PATH,0x8000],-1)
    c.write(ALT,b'D:\\data\\clip.bin\0')
    c.event('open',[ALT,0x8000],-1,private_input_args={})
    c.event('prompt',[],0);c.event('open',[ALT,0x8000],-1,private_input_args={})
    c.event('refresh',[PATH]);c.event('open',[PATH,0x8000],-1);c.event('finish',[]);total=0
   else:
    c.event('open',[PATH,0x8000],7);failed_seek=mode=='seek-fallback'
    c.event('seek',[7,0x81234567,0],-1 if failed_seek else 0)
    cursor=BUFFER;total=0
    passes=2 if mode in('zero-success','zero-zero','negative-read','zero-rows','seek-fallback')else 1
    for pass_ in range(passes):
     if pass_:
      c.event('refresh',[PATH]);c.event('open',[PATH,0x8000],7);c.event('seek',[7,0x81234567,0],0)
     if rows and not(failed_seek and not pass_):
      counts=([4,4]if mode=='success'or pass_ and mode in('zero-success','seek-fallback')else
       [2,1]if mode=='partial'else[-1,-1]if mode=='negative-read'and not pass_ else[0,0])
      for count in counts:
       mutations=[(cursor+i,1,0x20+pass_*16+i)for i in range(max(0,count))]
       c.event('read',[7,cursor,4],count,mutations=mutations);total+=count
       c.event('seek',[7,8,1],0);cursor+=4
     c.event('close',[7],0)
     if pass_:c.event('finish',[])
    if total!=0:c.event('update',[COLOR,total])
   c.c['direct_entry_proof']=True
   c.c['prior_child_packet']='Public c7-joint-sprite enters53E590; it is not a standalone53F7D0 proof.'
   c.c['focal_domain']='Controlled CRT/path/color boundaries, exact caller-supplied receiver/five args. Width4/rows0or2; retry termination declared.'
   out.append(c.end(total))
 return out


def cases(registry=None):
 out=actor_cases()+civic_cases()+city_cases()+tooltip_cases()+sprite_cases()
 if registry is None:return out
 members={int(m['address'],16):m for m in registry['members']};out=[c for c in out if c['entry']in members]
 for c in out:c['expected_invocations']=1+sum(bool(v['named'])+bool(v['canonical'])for v in members[c['entry']]['providers'].values())
 return out
definitions=cases
