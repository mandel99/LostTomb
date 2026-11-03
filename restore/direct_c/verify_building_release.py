"""Offline original/C building release and temple-complex terrain clearing.

Execute actual children; compare ordered owned stores, child inputs, all original
static data and the saved ABI. Resident synthetic text/font data, disabled sound
and genuine no-image guards qualify this check; it does not launch the game.
"""
from verify_options_startup import *
from unicorn import UC_HOOK_MEM_WRITE
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import json
catalogue=json.loads((ROOT/'metadata/building-release-native.json').read_text())
assert catalogue['native_sha256']==hashlib.sha256(original.read_bytes()).hexdigest()
names={0x46ac10:'restore_building_release',0x46ac50:'restore_building_release_links',0x4c0920:'restore_temple_complex_clear'}
owned={};branches={};hits=set();edges=set();last=[None];switches=set();checked=invocations=0
decoder=Cs(CS_ARCH_X86,CS_MODE_32)
for f in catalogue['functions']:
 a=int(f['address'],16);raw=bytes.fromhex(f['bytes_hex']);assert raw==bytes(images[0].data[a-images[0].base:a-images[0].base+len(raw)])
 instructions=list(decoder.disasm(raw,a));owned[a]={int(p,16) for p in f['instruction_addresses'].split()}
 assert {i.address for i in instructions}==owned[a]
 for i in instructions:
  if i.mnemonic.startswith('j') and i.mnemonic!='jmp':branches[i.address]={int(i.op_str,16),i.address+i.size}
for t in catalogue['tables']:
 a=int(t['address'],16);b=bytes.fromhex(t['bytes_hex']);assert b==bytes(images[0].data[a-images[0].base:a-images[0].base+len(b)])
# Native 4E5830 only forwards its one argument to 4F4AE0 (Rea catalogue).
# MSVC folds the equivalent C bodies; normalize this one helper identity.
assert sym('FUN_004e5830')==sym('FUN_004f4ae0')
points=set.union(*owned.values());machines=[]
calls={0x46ac50:1,0x4c0920:1,0x46a2e0:1,0x4a36b0:1,0x4b82e0:1,0x4f4ae0:1,0x4e5830:1,0x475b50:1,0x47b590:4,0x47bd50:4,0x4dea50:0,0x40f8f0:3,0x485200:0,0x47a220:3,0x4767d0:6,0x477500:4,0x4777c0:4,0x475cc0:5,0x4776f0:4,0x477890:4}
for c,pe in enumerate(images):
 u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(pe.base,(len(pe.data)+4095)&~4095);u.mem_write(pe.base,bytes(pe.data))
 u.mem_map(0x30000000,0x10000);u.mem_map(STOP,4096);u.mem_map(S,0x50000)
 state=dict(collect=False,own=True,writes=[],events=[],pending=set(),returns=set(),compiled=c,last_entry=None);machines.append((c,u,state))
 def write(uc,access,at,size,value,data):
  if data['collect'] and data['own']:data['writes'].append((at,size,value&((1<<(size*8))-1)))
 u.hook_add(UC_HOOK_MEM_WRITE,write,state,begin=0x560000,end=0x126cfff)
 if not c:
  def code(uc,pc,size,data):
   if not data['collect']:return
   hits.add(pc)
   if last[0] in branches:edges.add((last[0],pc))
   if last[0]==0x46ace7:switches.add(pc)
   last[0]=pc
  for p0 in owned.values():u.hook_add(UC_HOOK_CODE,code,state,begin=min(p0),end=max(p0))
 def returned(uc,pc,size,data):
  if data['collect'] and pc in data['pending']:data['pending'].remove(pc);data['own']=True
 def child(uc,pc,size,data):
  if not data['collect'] or not data['own']:return
  t=data['entries'][pc];t=0x4f4ae0 if t==0x4e5830 else t;sp=uc.reg_read(UC_X86_REG_ESP);ret=get(uc,sp)
  if t==data['top'] and ret==STOP:return
  entry=(t,sp,ret)
  if entry==data['last_entry']:return
  data['last_entry']=entry
  args=tuple(get(uc,sp+4+i*4) for i in range(calls[t]))
  if t==0x40f8f0:args=args[:2]+(args[2]&255,)
  data['events'].append((0x4f4ae0 if t==0x4e5830 else t,args))
  mutation=data['mutation'];r=264
  if mutation=='kind' and t==0x46a2e0:word(uc,0x7999e8+r,182);put(uc,0xeaac28,2)
  if mutation=='phase' and t==0x4f4ae0:word(uc,0x7999ea+r,data['phase'])
  if mutation=='position' and t==0x47b590:word(uc,0x7999de+r,27);word(uc,0x7999e0+r,26)
  if mutation=='player' and t==0x475b50:put(uc,0xeaac28,2)
  if t not in owned:
   data['own']=False;data['pending'].add(ret)
   if ret not in data['returns']:uc.hook_add(UC_HOOK_CODE,returned,data,begin=ret,end=ret);data['returns'].add(ret)
 state['entries']={(sym('FUN_%08x'%t) if c else t):t for t in calls}
 if c:
  for t in names:
   if t in calls:state['entries'][sym(names[t])]=t
 for a in state['entries']:u.hook_add(UC_HOOK_CODE,child,state,begin=a,end=a)

DATA,baseline,STACK,N=0x560000,seed,0x30000000,0xcb10
directions=[-228,-227,1,229,228,227,-1,-229]
def fixture(u,c):
    def put(a,v,n=4):u.mem_write(a,(v&((1<<(n*8))-1)).to_bytes(n,'little'))
    u.mem_write(DATA,baseline);u.mem_write(STACK,b'\xcc'*0x10000);u.mem_write(0xbc9730,bytes(0xd20))
    x,y=c.get('position',(8,8));width,height=c.get('dimensions',(16,16));origin=912;cell=origin+y*228+x
    for a,v in [(0xbca2d0,width),(0xbca2d4,height),(0xbca2d8,origin),(0xbca2dc,228-width),(0xbca2e0,c.get('orientation',0)),(0xbca2e8,c.get('offset_x',0)),(0xbca2ec,c.get('offset_y',0)),(0xbca2f0,500),(0xbca2f4,500),(0xbca2f8,100),(0xbca2fc,0),(0xeaac28,0),(0xeaaf64,0),(0x5e4ef4,5),(0xbc971c,c.get('count',0)),(0xbc9718,c.get('cost',0)),(0xbc9768,cell),(0x705f94,c.get('refund1',25)),(0x7078e4,c.get('refund2',41)),(0x126517c,0),(0x10d69f8,0)]:put(a,v)
    for a,n in [(0x7999d8,4000*264),(0xaf598c,N*4),(0xae8e7c,N),(0xb4e6fc,N*2),(0xb350dc,N*2),(0xd05720,N*4),(0xb7482c,N*4),(0xb285cc,N),(0xb67d1c,N),(0xacf85c,N),(0xa509bc,N),(0xf55bb4,0x2000)]:u.mem_write(a,bytes(n))
    u.mem_write(0xaf598c,struct.pack('<I',c.get('surround',0))*N);u.mem_write(0xa509bc,bytes([c.get('fertility',0)])*N)
    put(0xbc9768,cell)
    for row in range(228):put(0xbc9f24+row*4,0);put(0xbc9b94+row*4,width if row<height else 0);put(0xbc9804+row*4,origin+row*228)
    for j in range(4):put(0x784b64+j*2,1000,2);put(0x784b6c+j*2,1000,2);put(0x784cc0+j*4,1000);put(0x784cd0+j*4,1000)
    if c.get('point'):put(0x784b64,x,2);put(0x784b6c,y,2)
    u.mem_write(0xb285cc,bytes([c.get('access',0xff)])*N);u.mem_write(0xb67d1c,bytes([0x9f])*N)
    put(0xaf598c+cell*4,c.get('flags',0));put(0xb350dc+cell*2,c.get('building_word',0),2)
    pattern=c.get('pattern',0)
    for bit,d in enumerate(directions):
     if pattern&(1<<bit):put(0xaf598c+(cell+d)*4,0x4000)
    for bit,d in enumerate(directions[::2]):
     if c.get('road_pattern',0)&(1<<bit):
      at=0xaf598c+(cell+d)*4;put(at,int.from_bytes(u.mem_read(at,4),'little')|0x8000)
    if c.get('object'):put(0xd05720+cell*4,0x6f0000);put(0x6f0018,c.get('object_flag',0),1)
    # Real sparse map, empty sprite image0: renderer algorithms execute their
    # genuine no-image guards, never an injected rendering result.
    for group in range(0x240):put(0xf55bb4+group*8,1000+(group%200)*10)
    put(0x126517c,0);put(0x10d69f8,0)
    u.mem_write(0x9de62c,bytes(u.mem_read(0xaf598c,N*4)));u.mem_write(0x9d1b1c,bytes(u.mem_read(0xae8e7c,N)))
    u.mem_write(0xa1126c,struct.pack('<I',0x12345678)*N)
    u.mem_write(0xadc36c,bytes(N));u.mem_write(0xba746c,bytes(0x41a));u.mem_write(0xba7888,b'\xa5'*0x21d68)
    u.mem_write(0xbcd5f8,bytes(2000*388));u.mem_write(0xeaaf68,bytes(0x2bc0));u.mem_write(0x9784d8,bytes(0x89c))
    for at,v,n in [(0x5e4ef4,0,4),(0xf55b88,0,4),(0xe38e63,1,1),(0xbc95f0,c.get('reservation_count',0),4),(0xbc96a8,c.get('allow_class',0),4),(0xbc96ac,c.get('allow_special',0),4),(0xbc96b0,c.get('allow_line',0),4),(0xbc96b4,c.get('allow_54',0),4),(0xbc9694,1,4),(0x604950,0,4),(0xd056fc,0,4),(0xbcd5f0,0,4),(0xd3b1c4,0,4)]:put(at,v,n)
    for j in range(100):put(0xd3b1c8+j*4,j);put(0xea3e34+j*2,6 if j in [0,70] else 0,2)
    for at,v in [(0xea3f68,0),(0xea1894,12),(0xea5f0c,12)]:put(at,v)
    kind=c.get('kind')
    if kind is not None:
     base=0x7999d8+264
     for at,v,n in [(base,c.get('status',1),1),(base+5,c.get('occupied',0),1),(base+6,x,2),(base+8,y,2),(base+12,cell,4),(base+16,kind,2),(base+28,c.get('people',0),2),(base+56,c.get('next',0),2),(base+58,0,2),(base+84,0,1),(0x5f5d08,1,4),(0x5cfcdc,0,4)]:put(at,v,n)
     put(0xb4e6fc+cell*2,1,2)
     if c.get('reservation_count'):put(0xba746c,1,2)
     if c.get('next'):
      put(0x7999d8+c['next']*264,1,1);put(0x7999e8+c['next']*264,20,2)
     if kind==183:
      cap=cell+5;put(0xaf598c+(cap+1)*4,4);put(0xaf598c+(cap+229)*4,4)
    if c.get('bridge'):
     put(0xaf598c+cell*4,4);put(0xadc36c+cell,1,1);put(0xb350dc+cell*2,c.get('line_block',0),2)
    if c.get('constructor'):
     base=0x7999d8+2*264;where=origin+5*228+5;typ=c['constructor']
     for at,v,n in [(base,1,1),(base+6,5,2),(base+8,5,2),(base+16,typ,2),(base+58,3,2),(base+208,where,4),(base+212,c.get('direction',0),4),(base+216,1,4),(0x7999d8+3*264,1,1),(0x7999e8+3*264,typ,2)]:put(at,v,n)
    for at,v,n in [(0xe38ec1,1,1),(0xe38dda,5,1),(0xe36c70,0x32001000,4),(0xe34c38+19*8,0x1000,4),(0xe34c38+17*8,0x1000,4),(0xf56f8c,10,4),(0xf56fb4,10,4),(0xf56fbc,20,4),(0xf56f84,30,4),(0x126517c,512,4),(0xe92e0c,0x11223344,4)]:put(at,v,n)
    u.mem_write(0xe94e28,bytes(5*120));u.mem_write(0x10d69f8,bytes(512*64))
    for j in range(512):put(0x10d6a0c+j*64,6,2)
    for ch in [65,66,67,48,49,50]:put(0x5e047c+ch,1,1)
    u.mem_write(0x32000000,bytes(0x10000));u.mem_write(0x32002000,b'A00\0'*300+b'TAIL\0')
    u.mem_write(0x9de62c,bytes(u.mem_read(0xaf598c,N*4)));u.mem_write(0x9d1b1c,bytes(u.mem_read(0xae8e7c,N)))
    for reg,v in [(UC_X86_REG_FPCW,0x37f),(UC_X86_REG_FPSW,0),(UC_X86_REG_FPTAG,0xffff)]:u.reg_write(reg,v)
    if c.get('canal'):
        for at,val in [(0xae8d98,5),(0xae8e7d,6),(0xae8f60,5),(0xae8e7b,6)]:put(at+cell,val,1)
    if c.get('slots'):
        for at,val,n in [(0x799a99+264,2,1),(0x799aa3+264,3,1),(0x799aa2+264,4,1),
                         (0x7900c0+2*196,1,1),(0x9783a4+3*28,1,1),(0xeaaf68+4*224,99,4),(0x799a7e+264,1,1)]:put(at,val,n)
    if c.get('forward'):
        put(0x799a12+264,4,2);put(0x7999d8+4*264,1,1);put(0x7999e8+4*264,20,2)
    if c.get('cycle'):
        put(0x799a10+264,1,2);put(0x799a12+264,1,2)
    if c.get('long_chain'):
        for node in range(1,528):
            put(0x7999d8+node*264,1,1);put(0x7999e8+node*264,20,2);put(0x799a10+node*264,node+1 if node<527 else 0,2)
    if 'marker' in c:put(0xadc36c+cell,c['marker'],1)
    if c.get('mixed'):
        for node,kind in enumerate([65,54],2):
            put(0x7999d8+node*264,1,1);put(0x7999e8+node*264,kind,2);put(0xb4e6fc+(cell+node-1)*2,node,2);put(0xaf598c+(cell+node-1)*4,8)
    if c.get('child_parent'):put(0x799a10+3*264,2,2)
    u.mem_write(0x9de62c,bytes(u.mem_read(0xaf598c,N*4)));u.mem_write(0x9d1b1c,bytes(u.mem_read(0xae8e7c,N)))
    return [c.get('preview',0),*c.get('bounds',(x,y,x,y))]


def prepare(u,c,fn):
 fixture(u,dict(position=(24,24),dimensions=(48,48),kind=c.get('kind',20)))
 building=c.get('building',1);record=building*264;player=c.get('player',0);off=player*0x49d8
 put(u,0xeaac28,player);put(u,0xbcb9b0,c.get('counter',10));put(u,0xbc9690,c.get('tool',0));put(u,0xbc9728,765)
 # Deliberately preserve nonzero surrounding record bytes through link release.
 u.mem_write(0x7999d8+record,b'\xa5'*264)
 for a,v,n in [(0x7999d8+record,1,1),(0x7999de+record,24,2),(0x7999e0+record,24,2),(0x7999e4+record,6408,4),
  (0x7999e8+record,c.get('kind',20),2),(0x7999ea+record,c.get('phase',0),2),(0x799a10+record,c.get('previous',0),2),(0x799a12+record,0,2),
  (0x799a28+record,c.get('group',0),2),(0x799a82+record,c.get('view',0),1),(0x799a99+record,c.get('slot',0),1),
  (0x799aa2+record,c.get('queue',0),1),(0x799aa3+record,c.get('list',0),1),(0x799a9c+record,c.get('peer',0),4)]:
  (byte if n==1 else word if n==2 else put)(u,a,v)
 for a in [0xea5e68,0xea5d18,0xea3ffc]:put(u,a+off,6408 if c.get('match',True) else 6409)
 for a in [0xea5ce8,0xea5cfa,0xea5cf6]:word(u,a+off,0x1234)
 for a in [0xea5f9c,0xea6010,0xea6014,0xea6244,0xea5fac,0xea5fb0,0xea5fb4]:put(u,a+off,0)
 word(u,0xea5baa+off,c.get('sun',-123))
 for a in [0xea5f9c,0xea5fa0,0xea5fa4,0xea5fa8]:put(u,a+off,0xabcdef01)
 slot=c.get('slot',0);queue=c.get('queue',0);listing=c.get('list',0)
 if slot:byte(u,0x7900c0+slot*196,1)
 if queue:put(u,0xeaaf68+queue*224,0x12345678)
 if 0<listing<=10:
  byte(u,0x9783a4+listing*28,1);put(u,0x9783bc+listing*28,0x12345678)
 if c.get('registered') is not None:put(u,0xea5fac+off+c['registered']*4,building)
 if c.get('peer',0)>0:
  peer=c['peer'];put(u,0x799a9c+peer*264,building);byte(u,0x799aa2+peer*264,0)
 if c.get('group',0)>0:
  group=c['group'];u.mem_write(0xbcb9c8+group*144,bytes(144));byte(u,0xbcb9c8+group*144,1)
  byte(u,0xbcb9c8+group*144+66,1);word(u,0xbcb9c8+group*144+4,13)
 if fn==0x4c0920 or c.get('kind',20) in range(65,70):
  view=c.get('view',0);p=0x5e00cc+(view>>1)*8
  if view not in [0,2,4,6]:
   put(u,p,0);put(u,p+4,0)
  x=24+get(u,p);y=24+get(u,p+4)
  for row in range(7):
   for col in range(13):
    px,py=(x+col,y+row) if view==0 else (x-row,y-col) if view==2 else (x-col,y-row) if view==4 else (x+row,y+col)
    cell=(912+py*228+px)&0xffffffff;i=row*13+col
    owner=building if c.get('owner','same')=='same' else 2 if c['owner']=='child' else 3
    word(u,0xb4e6fc+cell*2,owner);word(u,0xa9cc1c+cell*2,0xbeef)
    byte(u,0xb285cc+cell,(i*19+7)&255);byte(u,0xb67d1c+cell,(i*23+9)&255)
    byte(u,0xadc36c+cell,0xab);byte(u,0xae8e7c+cell,6);byte(u,0xacf85c+cell,i&255)
    flags=c.get('terrain',0x108) if c.get('terrain') is not None else [8,12,0x1000c,0x8008][i%4]
    put(u,0xaf598c+cell*4,flags);byte(u,0xa9010c+cell,0xe7)
  word(u,0x799a10+2*264,building);word(u,0x799a10+3*264,0)
  if c.get('mask') is not None:
   for i in range(91):put(u,0x5df9b0+i*20,c['mask'])
 return [building]

def exercise(fn=0x46ac50,**cfg):
 global checked,invocations
 expected=None
 for c,u,state in machines:
  for route in (['native'] if not c else ['named','canonical']):
   state.update(collect=False,own=True,writes=[],events=[],pending=set(),last_entry=None,top=fn,mutation=cfg.get('mutation'),phase=cfg.get('mutation_phase',0))
   args=prepare(u,cfg,fn);last[0]=None;state['collect']=True;invocations+=1
   pc=fn if not c else sym(names[fn] if route=='named' else 'FUN_%08x'%fn)
   try:run(0,u,pc,args,budget=100000000)
   except Exception as e:raise AssertionError(('execute',hex(fn),cfg,route,hex(u.reg_read(UC_X86_REG_EIP)))) from e
   state['collect']=False;answer=(tuple(state['writes']),tuple(state['events']),snapshot(u))
   if expected is None:expected=answer
   elif answer!=expected:
    diffs=[next(((i,a,b) for i,(a,b) in enumerate(zip(x,y)) if a!=b),None) for x,y in zip(expected[:2],answer[:2])]
    memory=next(((hex(0x560000+i),a,b) for i,(a,b) in enumerate(zip(expected[2],answer[2])) if a!=b),None)
    raise AssertionError((hex(fn),cfg,route,'stores/events',diffs,'lengths',[(len(x),len(y)) for x,y in zip(expected[:2],answer[:2])],'memory',memory))
 checked+=1
 if checked%25==0:print('progress',checked,'scenarios',flush=True)

if __name__=='__main__':
 quick='--quick' in sys.argv
 for fn,cfg in [(0x46ac50,{}),(0x46ac10,dict(tool=9)),(0x46ac50,dict(kind=210,registered=1)),(0x46ac50,dict(kind=136,peer=2)),(0x46ac50,dict(kind=65)),(0x4c0920,dict(kind=99,terrain=None)),(0x4c0920,dict(view=4,owner='child'))]:
  exercise(fn,**cfg)
 if not quick:
  for kind in [-32768,-1,0,20,49,50,57,64,65,66,67,68,69,70,74,77,78,79,94,95,136,138,182,183,187,188,189,207,208,209,210,213,215,222,225,227,228,229,230,234,235,236,237,32767]:
   exercise(kind=kind)
  for kind in [50,95,187,188,189]:
   for player,match in itertools.product([0,1,3],[False,True]):exercise(kind=kind,player=player,match=match)
  for slot in [0,1,255]:exercise(slot=slot)
  for queue in [0,1,9]:exercise(queue=queue)
  for listing in [0,1,10,11,255]:exercise(list=listing)
  for peer in [0,-1,2]:exercise(kind=136,peer=peer)
  for group in [0,-1,1]:exercise(kind=57,group=group)
  for kind in [183,210,213,222,225,227]:
   for registered in [0,1,2]:exercise(kind=kind,registered=registered)
  for kind,phase in [(228,1),(229,166),(230,15),(234,157),(235,436),(236,512)]:
   exercise(kind=kind,phase=phase)
   exercise(kind=kind,phase=0,mutation='phase',mutation_phase=phase)
  for fn in [0x46ac10,0x46ac50]:
   exercise(fn,kind=138,mutation='position');exercise(fn,kind=215)
   exercise(fn,kind=20,list=1,slot=2,queue=3,mutation='kind')
   exercise(fn,kind=65,mutation='player')
  for view in [0,2,4,6,1,255]:
   for owner in ['same','child','other']:
    exercise(0x4c0920,kind=65,view=view,owner=owner,terrain=None)
   exercise(0x4c0920,building=0,kind=99,view=view,terrain=None)
   exercise(0x4c0920,kind=99,view=view,mask=0)
  exercise(0x4c0920,kind=65,mask=1,terrain=None)
  exercise(0x46ac10,kind=65,view=6,owner='child',terrain=None,tool=9)
 missing=points-hits;missing_edges={(a,b) for a,ends in branches.items() for b in ends if (a,b) not in edges}
 print('PASS release:',checked,'scenarios;',invocations,'invocations;',len(points)-len(missing),'/',len(points),'instructions,',len(edges),'conditional edges;',len(switches),'switch targets',flush=True)
 print('Unexecuted instructions:',[hex(a) for a in sorted(missing)],'; edges:',[(hex(a),hex(b)) for a,b in sorted(missing_edges)],flush=True)
 if quick:sys.exit(0)
 assert not missing,('instruction coverage',missing)
 assert not missing_edges,('edge coverage',missing_edges)
 table=bytes.fromhex(catalogue['tables'][0]['bytes_hex']);expected_switches=set(struct.unpack('<20I',table[:80]))
 assert switches==expected_switches,('switch targets',expected_switches-switches,switches-expected_switches)
 sources=['CMakeLists.txt','include/building_release_model.h','include/monument_clear_model.h','runtime/menu_drag_building_release.c','runtime/menu_drag_monument_clear.c','direct_c/verify_building_release.py','metadata/building-release-native.json']
 proof=dict(native_sha256=catalogue['native_sha256'],compiled_C_path=str(target.resolve()),compiled_C_sha256=target_sha,passed=True,checked=checked,invocations=invocations,
  coverage={hex(a):dict(instructions=len(p),executed=len(p&hits)) for a,p in owned.items()},conditional_edges=len(edges),switch_targets=[hex(p) for p in sorted(switches)],
  unexecuted_instructions=[hex(a) for a in sorted(missing)],unexecuted_edges=[[hex(a),hex(b)] for a,b in sorted(missing_edges)],
  source_hashes={p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest() for p in sources},commit=None,controlled_algorithm_endpoints=[],controlled_platform_endpoints=[],
  limitations=['Actual cleanup, unlink, root query, group release, terrain and refresh children. Owned ordered stores, helper inputs, all final original static data and saved ABI compared.',
  'Resident synthetic text/font data, disabled audio and genuine no-image renderer guards; no pixel/audio/UI fidelity or host game execution claim. ICF-folded 4E5830/4F4AE0 helper identity is normalized; native forwarding is authenticated in the catalogue. Incidental void registers/flags and private stack scratch excluded.'])
 suffix='diagnostic' if target.stem=='restore_link_check' else 'game'
 (ROOT/f'metadata/building-release-{suffix}.json').write_text(json.dumps(proof,indent=2)+'\n')
