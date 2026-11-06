"""Actual4CABD0 modes/query graph; shared C model owns every private local."""
from compiled_callbacks.callback_cases import Case as CallbackCase,actor

ENTRY=0x4cabd0;TEXT=0x32004000;FONT=0x44556677;OLD_STYLE=0x8123
GEO=0xd3a6cc;OFFSET=0xe20778
MODEL_FIELDS=dict(compiled_frame_symbols=['restore_resource_render_C_004cd510'],
 compiled_frame_dispatcher='restore_resource_render_dispatch',compiled_frame_return_sites=[0x4cda31])


class Case(CallbackCase):
 def __init__(self,label,mode=0,state=0,theme=0,mx=50,my=100,screen_height=2048):
  super().__init__(ENTRY,label);self.c['id']='c7-tooltip-modes/'+label
  self.theme=theme;self.mx=mx;self.my=my;self.screen_height=screen_height
  for at,width,value in[(0xd3a6c0,4,mode),(0xe92dfc,4,theme),(0xe38dda,1,state),
   (0xe20768,4,TEXT),(OFFSET,4,0x55667788),(0xe38da4,4,0),(0xe38da8,4,0),
   (0xe38e28,4,mx),(0xe38e2c,4,my),(0xe38e30,4,mx),(0xe38e34,4,my),
   (0xe38d9c,4,2048),(0xe38d98,4,screen_height),(0xe38dac,4,1024),(0xe38db0,4,768),
   (0xd3b138,4,0),(0xd3b130,4,99),(0xd3b13c,4,99),
   (0x784f14,4,1),(0x784f18,4,0),(0x77591c,4,0),(0xbca2c4,4,1)]:self.block(at,width);self.put(at,value,width)
  self.put(0xf56f84,FONT)
  self.block(GEO,28,0);self.put(0xd3a6d4,10);self.put(0xd3a6d8,20);self.put(0xd3a6e4,17)
  self.block(0xd3b164,8);self.block(0xd3b51c,2);self.put(0xd3b51c,OLD_STYLE,2)
  self.block(TEXT,8,0);self.write(TEXT,b'tooltip\0')
  self.block(0x77de00,106,0);self.put(0x77de02,1,1)
  self.block(0x77f760,98,0);self.put(0x77f761,2,1)
  self.block(0x5d2a3c+2*8,4);self.put(0x5d2a3c+2*8,55)
  self.block(0x5c3ec0,0x500,0)
  for at,x,y in[(0x5c4048,10,20),(0x5c4128,-5,7),(0x5c3ec0,-20,-30)]:self.put(at,x,2);self.put(at+2,y,2)
  self.block(0x5760c0,136,0);self.put(0x5760c0,1);self.put(0x576104,19)
  self.block(0xb350dc+2,2);self.put(0xb350dc+2,1,2)
  self.block(0xd3a4c0,32,0);self.write(0xd3a4c0,b'mode-one\0')
  self.put(0xeaac28,1)
  off=0x49d8
  for at,value in[(0xea446c,0x80000005),(0xea5b3c,0),(0xea5b40,7),(0xea5b44,0xffffffff),(0xea5b48,99),(0xea1878,0x12345678)]:self.block(at+off,4);self.put(at+off,value)

 def begin(self):
  self.start([(0xd3a6d0,4),(0xd3a6cc,4),(0xd3a6d4,4),(0xd3a6d8,4),
   (0xd3b51c,2),(0xd3b138,4),(OFFSET,4)])
  for token,address,arity,kind in [('prepare',0x4cc110,2,'void'),('text',0x4c8070,6,'void'),
   ('line',0x4cd510,5,'void'),('number',0x4ccbb0,6,'void'),('formatted',0x4cce00,7,'void'),
   ('glyph',0x4cc770,5,'void'),('measure',0x4cbf30,2,'full32'),
   ('items',0x559fc0,1,'full32'),('available-a',0x443000,2,'full32'),
   ('available-b',0x443040,2,'full32'),('eligible',0x447790,2,'al8'),('hit',0x4cf620,4,'full32')]:self.endpoint(token,address,arity,kind)
  self.eps['line'].update(MODEL_FIELDS)
  self.eps['items']['pointer_args']={0:'tooltip-items16'}
  self.eps['items']['bind_pointer_roles']=['tooltip-items16']

 def event(self,token,args,value=0,mutations=(),**fields):
  spec=self.eps[token];self.c['authored_endpoint_order'].append(token)
  self.c['authored_endpoint_observations'].append([[at,width,self.get(at,width)]for at,width in self.observe])
  self.c['expected_endpoint_counts'][token]+=1
  self.c['expected_endpoint_arguments'][token].append([x&0xffffffff if isinstance(x,int)else x for x in args])
  response=dict(value=value,**fields)
  if mutations:response['mutations']=list(mutations)
  spec['responses'].append(response)
  for at,width,new in mutations:self.put(at,new,width)

 def box(self,y,x,width,height,border=None,fill=None):
  border=border if border is not None else(0x58a0 if self.theme==2 else 0x2c40)
  fill=fill if fill is not None else(0xbb88 if self.theme==2 else 0x5dc8)
  bx,by=max(0,y),max(0,x);right=bx+min(width,2048-bx)-1;bottom=by+min(height,self.screen_height-by)-1
  for args in[(bx,by,right,by,border),(bx,bottom,right,bottom,border),
   (bx,by,bx,bottom,border),(right,by,right,bottom,border)]:self.event('line',args)
  fx,fy=max(0,y+1),max(0,x+1);fw=min(width-2,2048-fx);fh=min(height-2,self.screen_height-fy)
  for row in range(fy,fy+max(0,fh)):self.event('line',[fx,row,fx+fw-1,row,fill])
  self.c['expected_model_line_calls']=max(0,fh)

 def default_finish(self,group,message):
  self.put(0xd3b168,FONT);self.put(0xd3b164,FONT);self.put(0xd3a6d0,200)
  self.put(0xd3b130,0);self.put(0xd3b13c,30);self.put(0xd3b51c,0x3922 if self.theme==2 else 0x1c82,2)
  self.event('prepare',[group,message]);self.event('text',[TEXT,25,15,180,1,0]);self.put(0xd3a6cc,10)
  y=self.my+20 if self.my<300 else self.my-220;x=self.mx+40 if self.mx<200 else self.mx-62
  self.put(0xd3a6d4,x);self.put(0xd3a6d8,y);self.box(y,x,220,10)
  self.event('text',[TEXT,y+5,x+7,180,0,0])


def mode1_cases():
 out=[]
 for label,first,second,theme,mx,my in [('mode1-one-measure',1,None,0,50,100),('mode1-width-remeasure',3,1,2,500,400)]:
  c=Case(label,mode=1,theme=theme,mx=mx,my=my);c.begin()
  c.put(0xd3b168,FONT);c.put(0xd3b164,FONT);c.put(0xd3a6d0,220)
  c.put(0xd3b130,0);c.put(0xd3b13c,30);c.put(0xd3b51c,0x4208,2)
  c.event('text',[0xd3a4c0,25,15,190,1,0],mutations=[(0xd3b138,4,first)])
  lines=first;width=220;c.put(0xd3a6cc,first*16+10)
  if second is not None:
   width=300;c.put(0xd3a6d0,width);c.put(0xd3b130,0);c.put(0xd3b13c,30)
   c.event('text',[0xd3a4c0,25,15,270,1,0],mutations=[(0xd3b138,4,second)])
   lines=second;c.put(0xd3a6cc,lines*16+10)
  y=my+20 if my<width+20 else my-width-20;x=mx+50 if mx<200 else mx-72
  c.put(0xd3a6d8,y);c.put(0xd3a6d4,x);c.box(y,x,width+20,lines*16+10)
  c.event('text',[0xd3a4c0,y+5,x+7,width-30,0,0]);out.append(c.end())
 return out


def mode2_cases():
 out=[]
 for theme in(0,2):
  c=Case(f'mode2-six-live-DWORD-values-theme{theme}',mode=2,theme=theme,screen_height=76);c.begin()
  y,x=120,60;c.put(0xd3a6d0,180);c.put(0xd3a6cc,120);c.put(0xd3a6d8,y);c.put(0xd3a6d4,x)
  c.box(y,x,180,120)
  front,shadow=(0x3922,0xff3a)if theme==2 else(0x1c82,0x7f9a)
  values=(0x80000005,0,7,0xffffffff,99,0x12345678);labels=(0x87,0x88,0x89,0x8a,0x8b,0x8e)
  for index,(value,label)in enumerate(zip(values,labels)):
   dy=index*14;fmt=0x5e03fc if index in(0,5)else 0x57c048
   c.event('number',[0x44,label,y+5,x+dy+5,FONT,front]);c.event('number',[0x44,label,y+4,x+dy+4,FONT,shadow])
   c.event('formatted',[fmt,0x40,value,y+140,x+dy+5,FONT,front]);c.event('formatted',[fmt,0x40,value,y+139,x+dy+4,FONT,shadow])
  out.append(c.end())
 return out


def mode3_cases():
 out=[]
 for theme,selected in[(0,[]),(2,[0,3])]:
  c=Case(f'mode3-private-array-partial-writes-{len(selected)}-items',mode=3,theme=theme);c.begin()
  writes=[(0,i*4,'01000000')for i in selected]
  c.event('items',[['stack-output','tooltip-items16']],len(selected),writes_arg_bytes=writes)
  width=50+len(selected)*60;y=120 if 100<width+20 else 100-width-20;x=60
  c.put(0xd3a6d0,width);c.put(0xd3a6cc,20);c.put(0xd3a6d8,y);c.put(0xd3a6d4,x)
  style=0x3922 if theme==2 else 0x1c82;c.put(0xd3b51c,style,2);c.box(y,x,width,20);c.put(OFFSET,0)
  if selected:
   c.event('number',[0x17,39,y+5,x+5,FONT,style],mutations=[(OFFSET,4,40)])
   c.event('glyph',[0x5e0400,y+45,x+5,FONT,style],mutations=[(OFFSET,4,45)])
   c.event('number',[0x17,42,y+50,x+5,FONT,style],mutations=[(OFFSET,4,80)])
  c.put(0xd3b51c,OLD_STYLE,2)
  c.c['private_array_oracle']=dict(initialized_words=16,endpoint_writes=selected,unwritten_words_remain_zero=True)
  out.append(c.end())
 return out


def mode4_cases():
 out=[]
 values=[(1,49,3),(2,48,5),(3,48,6),(12,48,18),(13,48,19),(14,48,23),
  (26,48,39),(30,48,56),(33,48,59),(50,48,10),(58,48,48),(0,48,-40),(255,48,245)]
 for ordinal,(kind,group,message)in enumerate(values):
  c=Case(f'mode4-vessel-BYTE-kind{kind}',mode=4,theme=2 if ordinal&1 else 0)
  a=actor(1);c.put(a+10,36,1);c.put(a+11,kind,1)
  if kind==50:
   c.put(a+10,39,1);c.put(a+8,2,2);c.put(actor(2)+10,36,1);c.put(actor(2)+11,kind,1)
  c.begin();c.event('prepare',[group,message]);c.event('measure',[TEXT,FONT],80)
  width=100;y=120;x=60;c.put(0xd3a6d0,width);c.put(0xd3a6cc,20);c.put(0xd3a6d8,y);c.put(0xd3a6d4,x)
  style=0x3922 if c.theme==2 else 0x1c82;c.put(0xd3b51c,style,2);c.box(y,x,width,20)
  c.put(OFFSET,0);c.event('glyph',[TEXT,y+5,x+5,FONT,style]);c.put(0xd3b51c,OLD_STYLE,2)
  out.append(c.end())
 c=Case('mode4-empty-selected-tile',mode=4);c.put(0xb350dc+2,0,2);c.begin();out.append(c.end())
 return out


def query_cases():
 out=[]
 #4C9340 row/grid, each half writes a different BYTE through the actual caller alias.
 for grid in(False,True):
  for half in('a','b'):
   c=Case(f'query20-{("grid"if grid else"row")}-{half}-actual-BYTE-alias',state=20)
   c.put(0x77de05,int(grid),1);c.begin()
   if not grid:c.put(OFFSET,0)
   for resource in range(1,20 if half=='a'else 36):
    c.event('available-a',[0,resource],int(half=='a'and(resource==19 or grid and resource in(1,2))))
    if half=='a'and(resource==19 or grid and resource in(1,2)):
     c.event('eligible',[0,resource],1)
     if grid:point=[134+(135 if resource==2 else 0),931 if resource in(1,2)else 951,26,26]
     else:point=[234,944,19,17]
     c.event('hit',point,0x100 if resource==19 else 0)
     if not grid and resource!=19:c.put(OFFSET,c.get(OFFSET)+32)
   if half=='b':
    if not grid:c.put(OFFSET,0)
    for resource in range(1,20):
     selected=resource==19 or grid and resource in(1,2);c.event('available-b',[0,resource],int(selected))
     if selected:
      c.event('eligible',[0,resource],1)
      point=[414+(135 if resource==2 else 0),931 if resource in(1,2)else 951,26,26]if grid else[234,966,19,17]
      c.event('hit',point,0x100 if resource==19 else 0)
   message=55 if half=='a'else 38;c.default_finish(0x17,message)
   c.c['private_BYTE_oracle']=dict(query='004c9340',initial=0,written=0 if half=='a'else 1,result=19,
    parent_message=message,alias='Actual4CABD0 PUSH-ECX scratch high BYTE passed by LEA; child indirect BYTE store is consumed by parent resource19 branch.')
   out.append(c.end())
 #4C95D0 standard first/second tables and type0 fallback.
 for half,kind,point,message in [('a',1,[233,930,26,26],55),('b',1,[218,917,26,26],38),('zero',0,[203,880,26,26],38)]:
  c=Case(f'query21-{half}-actual-BYTE-alias',state=21);c.put(0x77f760,kind,1);c.begin()
  if half=='b':
   for resource in range(1,36):c.event('available-a',[0,resource],0)
  token='available-b'if half=='b'else'available-a'
  for resource in range(1,20):c.event(token,[0,resource],int(resource==19))
  c.event('hit',point,1);c.default_finish(0x17,message)
  c.c['private_BYTE_oracle']=dict(query='004c95d0',initial=0,written=0 if half=='a'else 1,result=19,parent_message=message,
   alias='Actual query output pointer targets the caller scratch BYTE; resource19 message distinguishes0 from1.')
  out.append(c.end())
 #4C92A0 has no output pointer; parent explicitly sets the same BYTE to1.
 for half in('a','b'):
  c=Case(f'query78-{half}-parent-BYTE-one',state=0x4e)
  if half=='a':c.put(0x5760c0,19)
  c.begin();c.event('hit',[686 if half=='a'else 56,50,24,24],1 if half=='a'else 0)
  if half=='b':c.event('hit',[56,134,24,24],1)
  c.default_finish(0x17,38);c.c['private_BYTE_oracle']=dict(query='004c92a0',parent_initial_override=1,result=19,parent_message=38)
  out.append(c.end())
 c=Case('query20-full-scan-AL-reject-preserves-initial-BYTE',state=20);c.begin();c.put(OFFSET,0)
 for resource in range(1,36):
  c.event('available-a',[0,resource],0x100 if resource==19 else 0)
  if resource==19:c.event('eligible',[0,19],0)
 c.put(OFFSET,0)
 for resource in range(1,36):c.event('available-b',[0,resource],0)
 c.default_finish(0x44,17);c.c['private_BYTE_oracle']=dict(query='004c9340',initial=0,written=None,result=0,parent_message=17)
 out.append(c.end())
 c=Case('query21-unsupported-type-preserves-initial-BYTE',state=21);c.put(0x77f760,2,1);c.begin()
 c.default_finish(0x44,17);c.c['private_BYTE_oracle']=dict(query='004c95d0',initial=0,written=None,result=0,parent_message=17)
 out.append(c.end())
 return out


def cases(registry=None):
 out=mode1_cases()+mode2_cases()+mode3_cases()+mode4_cases()+query_cases()
 if registry is None:return out
 matches=[m for m in registry['members']if int(m['address'],16)==ENTRY]
 if not matches:return[]
 n=1+sum(bool(v['named'])+bool(v['canonical'])for v in matches[0]['providers'].values())
 for c in out:c['expected_invocations']=n
 return out
definitions=cases
