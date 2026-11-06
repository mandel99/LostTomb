"""UI-origin recipes authored from the pinned load/PUSH sites in native.asm.txt.

Data authoring only. Text endpoints deliberately accumulate a declared DWORD
width, with narrow-view traps; no renderer, SDK, or host behavior is guessed.
"""
from pathlib import Path
import json,re,sys
sys.dont_write_bytecode=True
OUT=Path(__file__).resolve().parent;ROOT=OUT.parents[2]
sys.path[:0]=[str(ROOT/'direct_c/cohort3_replay')]
from compiled_flood_regions.cohort7_compiled_acceptance_cases import Case as BaseCase,p
INPUTS=json.loads((OUT/'author-inputs.json').read_bytes())
TARGETS=tuple(int(m['address'],16)for m in INPUTS['members'])
X,Y,W,H=0xe38da8,0xe38da4,0xe38db0,0xe38dac
WIDTH,SELECT=0xe20778,0x7056c8
PX,PY=0xf53f0c,0xf53f08
C0,C1,C2,C3,C4,C5,C6=0xf56f84,0xf56f8c,0xf56f94,0xf56f9c,0xf56fa4,0xf56fb4,0xf56fbc
PLAYER=1;OFF=PLAYER*0x49d8
FMT=0x57c048
HELPERS={
 'panel':(0x536820,4),'sprite':(0x53e590,3),'prepare':(0x4cf1c0,0),
 'plain':(0x4ccb30,7),'wrapped':(0x4ccfd0,7),'label':(0x4d62f0,8),
 'center':(0x4d6250,9),'paragraph':(0x4d6160,9),'number':(0x4d6860,9),
 'button':(0x4d63d0,8),'inset':(0x4d65b0,6),'numeric_center':(0x4cce90,8),
 'ordinal':(0x4ccc30,7),'currency':(0x4cccb0,6),'string':(0x4cc770,5),
 'rect_fill':(0x4cd9d0,5),'rect':(0x4cd930,4),'box':(0x4cd680,5),
 'key':(0x4cc720,6),'descriptor_text':(0x4d6380,9),'descriptor_button':(0x4cd1a0,5),
 'child52d630':(0x52d630,0),'child52e6f0':(0x52e6f0,0),'child52ec50':(0x52ec50,0),
 'ratio':(0x4cde60,2),'scale':(0x4cde40,2),'eligible':(0x44d500,1),
 'count':(0x426b20,0),'query_setting':(0x4f4ff0,7),'building_status':(0x444a00,2),
 'population':(0x458f80,1)}
RESULTS={k:'full32'for k in('ratio','scale','eligible','count','query_setting','building_status','population')}
TEXT={'plain','wrapped','label','center','paragraph','number','numeric_center','ordinal','currency','string','key','descriptor_text'}
def signed(v,bits=32):
 v&=(1<<bits)-1
 return v-(1<<bits)if v>>(bits-1)else v
def half(v):return abs(v)//2*(-1 if v<0 else 1)

class UI(BaseCase):
 def __init__(self,entry,v,extra=(),arguments=()):
  super().__init__(entry,('negative-x-full-dword-y'if not v else'full-dword-x-negative-y'),0x89abcdef,arguments,named=arguments)
  self.c['id']=f'c8-ui-origin-joint25/{entry:08x}/'+self.c['id'].split('/')[-1]
  self.c.update(required_retained_route='canonical',expected_invocations=4,routes=['native','canonical'],routes_reason='The FUN symbol is the existing canonical C entry in all images; a second named route would execute the identical machine entry.')
  self.v=v;self.step=(17,273)[v];self.ro={}
  self.block(0xe38d90,0xc0,0);self.block(0xd3b560,0x20,0xa5)
  self.block(WIDTH,4,0);self.block(0xf53d00,0x300,0)
  m=next(m for m in INPUTS['members']if int(m['address'],16)==entry)
  # Explicit zero seeds for scalar source views. Array elements used below are
  # separately seeded from native stride calculations, never C pointer units.
  for a in re.findall(r'^#define \w+ .*?0x([0-9a-fA-F]+)u', (ROOT/m['source']).read_text(),re.M):
   at=int(a,16)
   if at>=0x600000:self.seed(at,0)
  self.seed(X,(-512,0x123456)[v]);self.seed(Y,(0x12345,-300)[v])
  self.seed(W,(800,1025)[v]);self.seed(H,(600,769)[v])
  self.seed(PX,(32,0x120)[v]);self.seed(PY,(48,0x130)[v]);self.seed(SELECT,v+1)
  self.seed(0xeaac28,PLAYER)
  for a in(0xf55bfc,0xf55be4,0xf55bdc,0xf55bd4,0xf56494,0xf56974,0xf571fc):self.seed(a,0x3000+v*0x456)
  for a in(C0,C1,C3,C5):self.seed(a,0x1c82)
  for a in(C2,C4,C6):self.seed(a,0x3922)
  self.extra=list(extra)
 def seed(self,at,value,width=4):
  raw=p(value,width)
  try:self.write(at,raw)
  except AssertionError:
   for i,b in enumerate(raw):self.ro[at+i]=b
 def val(self,at,width=4):
  try:return self.get(at,width)
  except AssertionError:return int.from_bytes(bytes(self.ro[at+i]for i in range(width)),'little')
 def begin(self):
  super().start([(X,4),(Y,4),(W,4),(H,4),(PX,4),(PY,4),(WIDTH,4),(0xf53db8,4),(0xf53db4,4),(0xd3b568,4),(0xd3b56c,4),(0xd3b570,4),(0xd3b574,4),*self.extra])
  items=sorted(self.ro.items());runs=[]
  for at,b in items:
   if runs and at==runs[-1][0]+len(runs[-1][1]):runs[-1][1].append(b)
   else:runs.append([at,bytearray([b])])
  self.c['stores'] +=[(a,bytes(b))for a,b in runs]
 def call(self,token,*args,result=0):
  if token not in self.eps:
   address,arity=HELPERS[token]
   self.endpoint(token,address,arity,RESULTS.get(token,'void'),native_cleanup=12 if token=='sprite'else 0,
    native_receiver=0xf55bb0 if token=='sprite'else None)
  assert len(args)==self.eps[token]['arity'],(token,len(args),self.eps[token]['arity'])
  mutations=[(WIDTH,4,self.get(WIDTH)+self.step)]if token in TEXT else []
  self.event(token,args,result,mutations)
 def reset(self):self.put(WIDTH,0)
 def xy(self):return[self.get(X),self.get(Y)]
 def label(self,group,item,x,y,color=None,flags=0):self.call('label',group,item,x,y,self.val(C1)if color is None else color,flags,*self.xy())
 def center(self,group,item,x,y,width,color=None,flags=0,token='center'):self.call(token,group,item,x,y,width,self.val(C1)if color is None else color,flags,*self.xy())
 def number(self,value,x,y,color=None,fmt=FMT,flags=0,base=0x40):self.call('number',value,base,fmt,x,y,self.val(C1)if color is None else color,flags,*self.xy())
 def nc(self,value,x,y,width,color=None,flags=0):self.call('numeric_center',value,0x40,FMT,x,y,width,self.val(C1)if color is None else color,flags)
 def button(self,x,y,width,height,selection,mode=0):self.call('button',mode,x,y,width,height,self.val(SELECT)==selection if selection else 0,*self.xy())
 def finish(self):return self.end()

def early_panels(entry,v):
 c=UI(entry,v);x,y=signed(c.val(X)),signed(c.val(Y))
 if entry==0x4484b0:
  c.seed(0x785d70,20 if not v else 12);c.seed(0x78a620,4 if not v else 1)
 c.begin()
 if entry==0x414c40:
  c.call('panel',x+0x80,y+0xa0,0x18,7)
  c.call('plain',0xb,5,x+0x80,y+0xbc,0x180,c.val(C5),0)
  c.call('plain',0xb,6,x+0x80,y+0xdc,0x180,c.val(C2),0)
 elif entry in(0x417e80,0x417eb0,0x417ee0):
  c.call('prepare');a={0x417e80:0xf55be4,0x417eb0:0xf55bdc,0x417ee0:0xf55bd4}[entry]
  c.call('sprite',c.val(a),x,y)
 elif entry==0x417f10:
  for a,value in[(0xf53db8,x+0x60),(0xf53db4,y+0xa0),(0xd3b56c,half(0x400-c.val(W))),(0xd3b568,half(0x300-c.val(H))),(0xd3b574,c.val(W)),(0xd3b570,c.val(H))]:c.put(a,value)
  c.call('sprite',c.val(0xf55bfc),0,0)
  c.call('panel',x+0x60,y+0xa0,0x1c,0xa)
  c.call('plain',0x3e,8,x+0x80,y+0xb8,0x180,c.val(C5),0)
  c.call('wrapped',0x3e,9,x+0x80,y+0xf0,0x180,c.val(C1),0)
 else:
  if not v:c.call('ratio',4,8,result=50)
  c.call('scale',196,50 if not v else 100,result=98 if not v else 196)
  c.call('sprite',c.val(0xf56494)+0x27,x+0x204,y+0x50+(98 if not v else 196))
 return c.finish()

def panel524840(v):
 c=UI(0x524840,v);a,b=c.val(PX),c.val(PY);c.seed(0xf53f48,6)
 for at,value in[(0xea5cbc,2),(0xea5cc0,765),(0xea5ff4,4567),(0xea620c,v),(0xea450c,3),(0xea4508,4321)]:c.seed(at+OFF,value)
 # Explicit decoys for the extant source's extra pointer-unit multiplication.
 c.seed(0xea5ff4+OFF*4,0x1357);c.seed(0xea620c+OFF*4,0)
 c.begin();c.call('inset',a+0x40,b+0x144,0x20,6,*c.xy())
 c.label(0x20,2,a+0x48,b+0x152,c.val(C6));c.reset()
 c.label(0x34,1,a+0x48,b+0x174,c.val(C3));c.number(765,a+0x50+c.get(WIDTH),b+0x174,c.val(C3),fmt=0x5e03ec)
 c.reset();c.number(4567,a+0xfa,b+0x174,c.val(C3));c.label(0x34,0x4f,a+0xfa+c.get(WIDTH),b+0x174,c.val(C3))
 c.button(a+0x140,b+0x160,0xfa,0x14,1);c.center(0x34,2,a+0x140,b+0x165,0xfa,c.val(C3))
 if not v:c.label(0x34,0x4e,a+0x64,b+0x18e,c.val(C3))
 else:
  c.button(a+0x46,b+0x189,500,0x14,2);c.reset();c.label(0x34,7,a+0x78,b+0x18e,c.val(C3))
  c.number(4321,a+0x78+c.get(WIDTH),b+0x18e,c.val(C3));c.label(0x34,3,a+0x78+c.get(WIDTH),b+0x18e,c.val(C3))
 c.button(a+0x140,b+0x14a,0xfa,0x14,3);c.center(0x34,0x31,a+0x140,b+0x14f,0xfa,c.val(C3))
 for j in range(5):
  eligible=(j+v)%2;c.call('eligible',j,result=eligible)
  if eligible:c.button(a+0x26,b+0x60+j*0x2a,0x230,0x28,j+4)
 return c.finish()

def panel524e20(v):
 c=UI(0x524e20,v);a,b=c.val(PX),c.val(PY)
 for i in range(11):c.seed(0x5d0320+i*4,1000+i*37)
 for at,value in[(0xea5d44,v),(0xea5cbc,2),(0xea450c,3)]:c.seed(at+OFF,value)
 c.begin();c.call('panel',a+0x80,b+0x20,0x18,0x19);c.call('sprite',c.val(0xf571fc)+0x15,a+0x90,b+0x30)
 c.center(0x34,0xf,a+0x90,b+0x30,0x170,c.val(C5));c.call('inset',a+0x90,b+0x50,0x16,0xf,*c.xy())
 for i in range(11):
  c.reset();color=c.val(C4)if c.val(SELECT)==i+2 else c.val(C3)
  c.label(0x34,i+4,a+0xb0,b+0x5a+i*0x14,color)
  c.number(1000+i*37,a+0xb0+c.get(WIDTH),b+0x5a+i*0x14,color,fmt=0x5e03ec)
 c.center(0x34,0x47 if not v else 0x4d,a+0x98,b+0x150,0x150,token='paragraph')
 c.button(a+0xf0,b+0x18b,0xa0,0x14,1);c.center(0xd,4,a+0xb0,b+400,0x120)
 return c.finish()

def panel5250f0(v):
 c=UI(0x5250f0,v);a,b=c.val(PX),c.val(PY);c.seed(0xea5ccc+OFF,6789);c.begin()
 c.call('panel',a+0x40,b+0xa0,0x20,0xa);c.call('sprite',c.val(0xf571fc)+0x15,a+0x60,b+0xb0)
 c.center(0x34,0x10,a+0x50,b+0xb0,0x1f0,c.val(C5));c.call('inset',a+0x70,b+0xd0,0x1a,4,*c.xy())
 for i,amount in enumerate((0,500,2000,5000)):c.nc(amount,a+0x7c+i*0x50,b+0xdd,0x40,c.val(C3))
 c.center(0x34,0x13,a+0x1bc,b+0xdd,0x40,c.val(C3));c.label(0x34,0x11,a+0x80,b+0xf8,c.val(C3))
 c.number(6789,a+0x13c,b+0xf8,c.val(C3));c.center(0xd,4,a+0x150,b+0x120,0xa0);c.center(0x34,0x12,a+0x90,b+0x120,0xa0)
 return c.finish()

def panel5253c0(v):
 c=UI(0x5253c0,v);a,b=c.val(PX),c.val(PY);c.seed(SELECT,3 if not v else 7);c.begin()
 for j in range(5):c.button(a+0x80+j*0x50,b+0xd8,0x40,0x14,j+3)
 c.button(a+0x150,b+0x11b,0xa0,0x14,1);c.button(a+0x90,b+0x11b,0xa0,0x14,2)
 return c.finish()

def panel5255a0(v):
 c=UI(0x5255a0,v);a,b=c.val(PX),c.val(PY);c.seed(0xea5dcc+OFF,567);c.begin()
 c.call('panel',a+0x60,b+0x90,0x1e,0xf);c.call('sprite',c.val(0xf571fc)+0x15,a+0x80,b+0xa0)
 c.center(0x34,0x45,a+0x80,b+0xa0,0x1b0,c.val(C5));c.call('inset',a+0x70,b+0xd0,0x1c,5,*c.xy())
 c.reset();c.label(0x34,0x32,a+0x90,b+0x130);c.call('ordinal',8,4,567,a+0x90+c.get(WIDTH),b+0x130,c.val(C1),0)
 c.center(0xd,4,a+400,b+0x155,0xa0)
 return c.finish()

def panel525720(v):
 c=UI(0x525720,v);a,b=c.val(PX),c.val(PY);c.seed(0xea5cc0+OFF,50 if not v else 300)
 for at,value in[(0xea5de4,100),(0xea5de8,200),(0xea5dec,300),(0xea5dd8,1),(0xea5ddc,2),(0xea5de0,3),(0xea5dc8,2)]:c.seed(at+OFF,value)
 c.begin()
 if not v:c.center(0x34,0x46,a+0xa0,b+0xe0,0x160,c.val(C3),token='paragraph')
 else:
  for j in range(3):
   yy=b+0xda+j*0x14;c.label(0x34,0x3f+j,a+0x80,yy,c.val(C3));color=c.val(C4)if c.val(SELECT)==j+1 else c.val(C3)
   c.reset();c.label(0x34,(0x33,0x37,0x3b)[j]+j+1,a+0xe0,yy,color)
   c.number(100*(j+1),a+0xe0+c.get(WIDTH),yy,color,fmt=0x5f89cc)
  c.center(0x34,0x44,a+0x80,b+0x155,0xf0);c.button(a+0x80,b+0x150,0xf0,0x14,4,mode=1)
 c.button(a+400,b+0x150,0xa0,0x14,5,mode=1)
 return c.finish()

def panel525b90(v):
 c=UI(0x525b90,v);x,y=signed(c.val(X)),signed(c.val(Y));c.seed(0x784f18,1);c.seed(0x77de58+106,v,1);c.begin()
 c.call('panel',x+0x50,y+0x40,0x1e,0xe);c.center(0x8e,0,x+0x50,y+0x50,0x1e0,c.val(C5))
 c.center(0x8e,1,x+0x70,y+(0x78 if v else 0x98),0x1a0,token='paragraph')
 if v:c.center(0x8e,3,x+0x70,y+0xb8,0x1a0,token='paragraph')
 c.label(0x8e,2,x+0x80,y+0x100)
 return c.finish()

def footer52d4e0(v):
 c=UI(0x52d4e0,v);w,h=c.val(W),c.val(H);c.begin()
 c.call('child52d630');c.call('rect_fill',10,h-0x74,w-0x14,0x73,0xf73b);c.call('rect',10,h-0x74,w-0x14,0x73)
 c.label(0x2c,0xde,0xf,h-0x74,c.val(C5))
 for xx in(0xf,0x77):
  for yy in(0x56,0x42,0x2e,0x1a):c.call('rect',xx,h-yy,100,0x12)
 c.call('child52e6f0');c.call('rect_fill',0xdf,h-0x72,w-0xee,0x6d,0xf73b);c.call('rect',0xdf,h-0x72,w-0xee,0x6d);c.call('child52ec50')
 return c.finish()

def footer52db10(v):
 c=UI(0x52db10,v);c.seed(0x784f14,2);c.seed(0x784f18,v);c.seed(0x77de02+106*v,0 if not v else 2,1)
 # Source pointer-unit decoy is explicit and remains a different status; native
 # reads only the BYTE at base+106*index.
 if v:c.seed(0x77de02+424,0,1)
 c.begin();c.center(0x2f,1 if not v else 0x13,half(c.val(W)-0xf0),c.val(H)-0x2e,0xf0)
 return c.finish()

def footer52e610(v):
 c=UI(0x52e610,v);record=1+v;c.seed(0xe38dda,0x14,1);c.seed(0x5d2e88,v)
 c.seed(0xf535d0+v*20,record);c.seed(0x785d70,4)
 c.seed(0x785d74+record*124+2,1,1);c.seed(0x785d74+record*124+0x30,5 if not v else 3,2)
 c.seed(0x785d74+record*248+2,0,1);c.seed(0x785d74+record*248+0x30,0x7654,2)
 c.begin();c.center(0x2f,0x10 if not v else 0x11,half(c.val(W)-0xf0),c.val(H)-0x44,0xf0,token='paragraph')
 return c.finish()

def footer52e6f0(v):
 c=UI(0x52e6f0,v);c.seed(0x775928,1 if not v else 15);c.begin();h=c.val(H);s=c.val(0x775928)
 for j in range(4):c.center(0x2c,j,0xf,h-(0x54-j*0x14),100,flags=0xf800 if s==j+1 else 0)
 for j,item in enumerate((0xa6,0xa7,0xdf,7)):c.center(0x2c,item,0x77,h-(0x54-j*0x14),100,flags=0xf800 if j<2 and s==j+14 else 0)
 return c.finish()

def salary5305e0(v):
 c=UI(0x5305e0,v);x,y=signed(c.val(X)),signed(c.val(Y));salary=-5 if not v else 345;c.seed(0x7848d8,salary,2);c.begin()
 c.call('panel',x+0x80,y+0x2c,0x14,0xa);c.reset();c.center(0x2c,0xd,x+0x8a,y+0x38,0x140,c.val(C5))
 c.center(0xd,3,x+0x80,y+0xb2,0x140);c.button(x+0x9e,y+0x64,100,0x1e,1,mode=1)
 c.label(0x14,int(salary>=0),x+0xa8,y+0x68,c.val(C5));c.button(x+0x116,y+0x64,0x78,0x1e,2,mode=1)
 c.nc(abs(salary),x+0x116,y+0x68,0x78,c.val(C5))
 return c.finish()

def settings530780(v):
 c=UI(0x530780,v);x,y=signed(c.val(X)),signed(c.val(Y))
 for j in range(3):c.seed(0x5c5e20+j*0x1c+2,10+20*j,2)
 for at,value in[(0xd38360,450 if v else 30),(0xd38364,900 if v else 60),(0xd38368,600 if v else 40)]:c.seed(at,value)
 c.begin();c.call('panel',x,y+0x50,0x1e,0x12);c.center(0x2c,0xb2,x,y+0x64,0x1e0,c.val(C5));c.center(0xd,3,x,y+0x138,0x1e0)
 for j in range(3):
  yy=y+0x50+10+20*j;ptr=0x5c5e20+j*0x1c
  c.label(0x2c,(0xb3,0xbc,0xc0)[j],x+0x3c,yy+8,c.val(C2))
  c.call('descriptor_text',0x2c,((0xc8,0xd9,0xdf)if v else(0xac,0xbd,0xc3))[j],ptr,c.val(C1),0,0,0x58,*c.xy())
  c.call('descriptor_button',1,ptr,c.val(SELECT)==j+1,0,0x50)
 return c.finish()

def keypad5309b0(v):
 ax,ay=(-1234,0x23456)if not v else(0x34567,-987)
 c=UI(0x5309b0,v,arguments=(ax,ay));x,y=signed(c.val(X)),signed(c.val(Y));c.seed(0x605978,v);c.seed(0x60597c,98765)
 c.seed(SELECT,1 if not v else 12);c.begin();c.call('panel',ax-0x10,ay-0x10,8,0xe);c.call('rect_fill',ax,ay,0x60,0x1e,0)
 if v:c.nc(98765,ax-8,ay+3,0x60,c.val(C5),0xf800)
 for j in range(10):
  xx=ax+(5,0x23,0x41)[j%3];yy=ay+0x23+(j//3)*0x1e;style=0x2bf if c.val(SELECT)==j+1 else 0
  c.call('box',xx,yy,0x19,0x19,style);c.call('key',0x5f8a00-j*4,xx,yy,0x19,c.val(C5),style)
 for xx,yy,ww,sel,item in[(ax+0x23,ay+0x7d,0x37,11,0x10),(ax+5,ay+0x9b,0x55,12,0x11)]:
  style=0x2bf if c.val(SELECT)==sel else 0;c.call('box',xx,yy,ww,0x19,style);c.center(0x2c,item,xx,yy+6,ww,flags=style)
 return c.finish()

def settings532920(v):
 c=UI(0x532920,v);x,y=signed(c.val(X)),signed(c.val(Y));c.seed(0x7848d8,-12 if not v else 345,2)
 for at,value in[(0x784b62,3),(0x7848f8,1234),(0x784c94,2345),(0x784c98,1000),(0x784c9c,2000),(0x784ca0,3000),(0x784ddc,4567),(0x784f08,2),(0x784f0c,3)]:c.seed(at,value)
 c.begin();c.call('panel',x,y,0x1e,0x22);c.label(0x2c,0x58,x+0x20,y+0xc,c.val(C5));c.center(0xd,3,x+0xc,y+0x1fb,0x1e0)
 for j,(title,sel)in enumerate(zip((0x6c,0x59,0x27,0x44,0x5b,0x5c,0x5d,0xb1,0xca,0xcb),(7,1,2,3,4,5,6,8,9,10))):
  yy=y+0x34+j*0x28;c.label(0x2c,title,x+0x20,yy);c.button(x+0x106,yy,200,0x1e,sel,mode=1)
  if j==0:c.center(0x20,3,x+0x106,yy+8,200)
  elif j==1:c.call('currency',0,-12 if not v else 345,x+0x13a,yy+4,c.val(C5),0)
  elif j in(2,3):c.nc((1234,2345)[j-2],x+0x106,yy+8,200)
  elif j in(4,5,6):
   c.reset();num=(j-3)*1000;c.number(num,x+0x115,yy+8,base=0x2b)
   c.call('currency',0,(-12 if not v else 345)+num,x+0x11f+c.get(WIDTH),yy+8,c.val(C1),0)
  elif j==7:c.reset();c.nc(4567,x+0x106,yy+8,200)
  else:c.center(0x97+j-8,j-6,x+0x106,yy+8,200)
 return c.finish()

def settings532fe0(v):
 c=UI(0x532fe0,v);x,y=signed(c.val(X)),signed(c.val(Y));c.seed(0xe38dda,0x3f,1)
 for j in range(v+1):c.seed(0x5cf594+j*4,j+1);c.seed(0x784ce0+j+1,j%2,1)
 c.begin();c.call('panel',x+0x10,y+0x20,0x26,0x1a);c.label(0x2c,0x2f,x+0x1a,y+0x2a,c.val(C5));c.center(0xd,3,x+0x10,y+0x1a8,0x260)
 c.call('count',result=3+v)
 for j in range(v+1):
  c.call('box',x+0x19,y+0x52+j*0x14,0xbe,0x12,0xfac1 if c.val(SELECT)==j+1 else 0)
  c.center(0x43,j+1,x+0x19,y+0x56+j*0x14,0xbe,c.val(C1)if j%2 else c.val(C4));c.call('count',result=3+v)
 return c.finish()

def settings533470(v):
 c=UI(0x533470,v);x,y=signed(c.val(X)),signed(c.val(Y));c.seed(0x784b60,v);c.seed(0x784cbc+2,0xab,1)
 c.seed(0x7848d8,-5 if not v else 345,2);c.seed(0x784c3c,0x04030201)
 for at,value in[(0x784c24,1001),(0x784c28,1002),(0x784c30,1003),(0x784c34,2),(0x784c38,3),(0x784c48,4),(0x784c4c,500),(0x784c50,6),(0x784c54,600),(0x784c58,8),(0x784c5c,700)]:c.seed(at,value)
 c.begin();c.call('panel',x+0x10,y+0x20,0x26,0x1c);c.label(0x2c,0x30,x+0x1a,y+0x2a,c.val(C5));c.center(0xd,3,x+0x10,y+0x1c8,0x260)
 for j,title in enumerate((0x6b,0xd2,0x32,0x33,0x34,0x35,0x36,0x37,0x38)):
  yy=y+0x65+j*0x28;c.label(0x2c,title,x+0x42,yy)
  leftsel=(15,17,1,3,0,7,9,11,13)[j];rightsel=(16,18,2,4,6,8,10,12,14)[j]
  c.button(x+0x13c,yy-9,0x50,0x1e,leftsel)
  if j==0:c.center(0x12,v,x+0x13c,yy,0x50)
  elif j==1:c.nc(2,x+0x13c,yy,0x50)
  elif j==4:
   c.call('query_setting',0x40,FMT,x+0x13c,yy,0x50,c.val(C1),0,result=17)
   # Native leaves the seven arguments live and pushes only the result. The
   # current compiler's deferred caller cleanup preserves that exact stack.
   c.nc(17,x+0x13c,yy,0x50)
  else:
   value=(0,0,1,2,0,4,4,6,8)[j]if not v else 0
   c.center(0x12,value,x+0x13c,y+0x71d if v and j==3 else yy,0x50)
  c.button(x+0x1a0,yy-9,0xb4,0x1e,rightsel)
  if j==0:c.nc(0xab,x+0x1a0,yy,0xb4)
  elif j==1:c.center(0x1d,23,x+0x1a0,yy,0xb4)
  elif j in(2,3,5,8):c.nc({2:1001,3:1002,5:1003,8:700}[j],x+0x1a0,yy,0xb4)
  elif j==4:c.center(0x2c,0xc9,x+0x1a0,yy,0xb4)
  else:
   c.reset();value=500 if j==6 else 600;c.number(value,x+0x1b4,yy,base=0x2b)
   c.call('currency',0,(-5 if not v else 345)+value,x+0x1be+c.get(WIDTH),yy,c.val(C1),0)
 return c.finish()

def buildings534320(v):
 c=UI(0x534320,v);x,y=signed(c.val(X)),signed(c.val(Y));active=0 if not v else 9;item=1+v
 for j in range(16):c.seed(0x705650+j*4,item if j==active else -1)
 c.seed(0x77591c,3);c.seed(SELECT,active+1);c.begin();c.call('panel',x+0x10,y+0x20,0x1e,0xe);c.center(0x2c,0x53,x+0x10,y+0x30,0x1e0,c.val(C5))
 value=3 if not v else 5;c.call('building_status',3,item,result=value)
 col=active*0x32-(400 if active>=8 else 0);row=0x3c if active<8 else 0x78
 c.call('sprite',c.val(0xf571fc)+item,x+0x10+col+0x28,y+0x20+row)
 c.button(x+0x10+col+0x19,y+0x20+row+0x1e,0x32,0x16,active+1)
 c.center(0x2c,value+0x54,x+0x10+col+0x19,y+0x20+row+0x24,0x32)
 c.center(0xd,1,x+0x10,y+0xe4,0x1e0)
 return c.finish()

def debug534610(v):
 c=UI(0x534610,v);c.seed(0xea181c,1)
 for j in range(8):c.seed(0xea624a+j*0x49d8,1 if j==0 else 0,1)
 for at,value in[(0xea626c,123),(0xea627c,200),(0xea6280,77),(0xea627c+0x9aa*4,4),(0xea627c+0x9ab*4,5),
                  (0x705c1c,2),(0xea5f14+OFF,3),(0xbca2e0,2),(0xe94e0c,1),(0xea5e04+OFF,4),(0xea5e0c+OFF,5),
                  (0xf55ba0,6),(0xf55b9c,7),(0xe951d8,8),(0xe951d4,9),(0xf535bc,10),
                  (0xd3b490,11),(0xd3b480,12),(0xd3b470,13),(0xd3b460,14),(0xe38e34,456),(0xe38e30,-567),
                  (0xe38e40,16),(0xf53f64,17),(0x6059e4,18)]:c.seed(at,value)
 for j in range(10):c.seed(0xea5cd0+OFF+j*2,j-5,2)
 c.begin();yy=0x5e;color=c.val(C0)
 def text(ptr,line,style=0xffff):c.call('string',ptr,c.get(WIDTH)+8,line,color,style)
 def number(value,fmt,line,xx=None,style=0xffff):c.number(value,c.get(WIDTH)+8 if xx is None else xx,line,color,fmt,style)
 c.reset();text(0x5f8bc0,yy-0xe);number(123,FMT,yy-0xe);text(0x5f8bb4,yy-0xe);number(200,FMT,yy-0xe);text(0x5f8ba4,yy-0xe);number(77,FMT,yy-0xe)
 c.reset();text(0x5f8b98,yy);number(4,FMT,yy);text(0x5f8b8c,yy);number(5,FMT,yy)
 c.call('population',PLAYER,result=123+v);style=0xffff if not v else 0xf800;text(0x5f8b7c,yy,style);number(123+v,FMT,yy,style=style)
 c.reset();text(0x5f8b6c,yy+0xe);number(2,0x5f8b68,yy+0xe);text(0x5f8b58,yy+0xe)
 for j in range(10):number(j-5,0x5f8b54,yy+0xe,xx=c.get(WIDTH)+4)
 c.reset();text(0x5f8b40,yy+0x1c);number(3,0x5f8b68,yy+0x1c)
 for delta,value,fmt in[(0x2a,2,0x5f8b30),(0x38,1,0x5f8b24)]:c.reset();number(value,fmt,yy+delta,xx=8)
 for delta,values,formats in[(0x46,(4,5),(0x5f8b0c,0x5f8af8)),(0x54,(6,7),(0x5f8aec,0x5f8ae0)),(0x62,(8,9),(0x5f8ad0,0x5f8ab4))]:
  c.reset();number(values[0],formats[0],yy+delta,xx=8);number(values[1],formats[1],yy+delta,xx=c.get(WIDTH)+0x6c)
 c.reset();number(10,0x5f8a94,yy+0x70,xx=8)
 for delta,values,formats in[(0x7e,(11,12),(0x5f8a84,0x5f8a74)),(0x8c,(13,14),(0x5f8a64,0x5f8a54)),(0x9a,(456,-567),(0x5f8a48,0x5f8a3c)),(0xa8,(16,17),(0x5f8a2c,0x5f8a1c))]:
  c.reset();number(values[0],formats[0],yy+delta,xx=8);number(values[1],formats[1],yy+delta,xx=c.get(WIDTH)+0x6c)
 c.reset();number(18,0x5f8a04,yy+0xb6,xx=8)
 # Each of the remaining seven inactive records resets the shared width before
 # testing its activity BYTE. The native tail therefore leaves a DWORD zero.
 c.reset()
 return c.finish()

BUILDERS={0x524840:panel524840,0x524e20:panel524e20,0x5250f0:panel5250f0,0x5253c0:panel5253c0,0x5255a0:panel5255a0,0x525720:panel525720,0x525b90:panel525b90,
 0x52d4e0:footer52d4e0,0x52db10:footer52db10,0x52e610:footer52e610,0x52e6f0:footer52e6f0,0x5305e0:salary5305e0,0x530780:settings530780,0x5309b0:keypad5309b0,
 0x532920:settings532920,0x532fe0:settings532fe0,0x533470:settings533470,0x534320:buildings534320,0x534610:debug534610}
def cases(registry=None):
 selected=TARGETS if registry is None else tuple(a for a in TARGETS if any(int(m['address'],16)==a for m in registry['members']))
 return[(BUILDERS[a](v)if a in BUILDERS else early_panels(a,v))for a in selected for v in range(2)]
