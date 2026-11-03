"""Offline original/C building information, house reasons and neighboring influence.

Execute actual children; compare ordered owned stores, child inputs, all original
static data and the saved ABI. Actual root traversal and native callers execute;
no child results are replaced and no host game is launched.
"""
from verify_options_startup import *
from unicorn import UC_HOOK_MEM_WRITE
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import json
catalogue=json.loads((ROOT/'metadata/building-information-native.json').read_text())
assert catalogue['native_sha256']==hashlib.sha256(original.read_bytes()).hexdigest()
names={0x454190:'restore_house_explain_evolution',0x45d570:'restore_eight_link_status',0x475b50:'restore_building_root',0x4b7590:'restore_has_active_formation',0x50f0f0:'restore_house_find_negative_influence'}
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
points=set.union(*owned.values());machines=[]
calls={0x475b50:1}
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
   if last[0]==0x454310:switches.add(pc)
   last[0]=pc
  for p0 in owned.values():u.hook_add(UC_HOOK_CODE,code,state,begin=min(p0),end=max(p0))
 def returned(uc,pc,size,data):
  if data['collect'] and pc in data['pending']:data['pending'].remove(pc);data['own']=True;data['last_entry']=None
 def child(uc,pc,size,data):
  if not data['collect'] or not data['own']:return
  t=data['entries'][pc];sp=uc.reg_read(UC_X86_REG_ESP);ret=get(uc,sp)
  if t==data['top']:return
  entry=(t,sp,ret)
  if entry==data['last_entry']:return
  data['last_entry']=entry
  args=tuple(get(uc,sp+4+i*4) for i in range(calls[t]))
  data['events'].append((t,args))
  if data['mutation']=='selected':put(uc,0xeadd6c,4)
  if t not in owned:data['own']=False
  data['pending'].add(ret)
  if ret not in data['returns']:uc.hook_add(UC_HOOK_CODE,returned,data,begin=ret,end=ret);data['returns'].add(ret)
 state['entries']={(sym('FUN_%08x'%t) if c else t):t for t in calls}
 if c:
  for t in calls:state['entries'][sym(names[t])]=t
 for a in state['entries']:u.hook_add(UC_HOOK_CODE,child,state,begin=a,end=a)


def prepare(u,fn,cfg):
 def v(a,x,n=4):u.mem_write(a&0xffffffff,(x&((1<<(n*8))-1)).to_bytes(n,'little'))
 u.mem_write(0x560000,seed);u.mem_write(0x30000000,b'\xcc'*0x10000)
 u.mem_write(0x7999d8,bytes(4000*264));u.mem_write(0xb4e6fc,bytes(0xcb10*2))
 building=cfg.get('building',1);r=building*264
 if fn==0x454190:
  level=cfg.get('level',0);model=0x70cdcc+level*88;v(0x7999ea+r,level,2);v(0x799a96+r,cfg.get('desire',10),1)
  v(0x7999db+r,cfg.get('merged',0),1);v(0x799a56+r,cfg.get('blocked',0),1);v(0x799a5d+r,0xa5,1)
  for shift in [0,88]:
   u.mem_write(model+shift,bytes(88));v(model+shift,cfg.get('lower',-100));v(model+shift+4,cfg.get('upper',0))
  for k,x in cfg.get('cur',{}).items():v(model+k,x)
  for k,x in cfg.get('nxt',{}).items():v(model+88+k,x)
  for a,x in cfg.get('bytes',{}).items():v(a+r,x,1)
  for a,x in cfg.get('words',{}).items():v(a+r,x,2)
  foodlist=cfg.get('foodlist',[]);v(0xf53dd8,cfg.get('food_count',len(foodlist)))
  for i,x in enumerate(foodlist):v(0xf53ddc+i*4,x)
  for i,x in cfg.get('stocks',{}).items():v(0x799a2c+r+i*2,x,2)
  player=cfg.get('player',0);v(0xeaac28,player);v(0xea5f14+player*0x49d8,cfg.get('capacity',2));v(0xeadcd4,cfg.get('influence',0))
  return [building]
 if fn in [0x475b50,0x46d0f0,0x45b2a0]:
  length=cfg.get('length',0)
  for i in range(1,length+1):v(0x799a10+(building+i-1)*264,building+i,2)
  v(0x799a10+(building+length)*264,cfg.get('terminal',0),2)
  if cfg.get('cycle'):v(0x799a10+building*264,building,2)
  if fn==0x45b2a0 and cfg.get('delivery'):
   root=building+length;v(0x799a18+root*264,1,2);u.mem_write(0x7900c0,bytes(196));v(0x7900ee,100,2);previous=root
   for node in range(600,608):
    v(0x799a12+previous*264,node,2);v(0x7999ea+node*264,0,2);v(0x799a14+node*264,0,2);previous=node
  return [building] if fn!=0x45b2a0 else [0,building,1,1]
 if fn==0x45d570:
  length=cfg.get('length',8)
  for i in range(length):
   node=building+i+1;v(0x799a12+(building+i)*264,node,2);v(0x7999ea+node*264,cfg.get('phase',1),2);v(0x799a14+node*264,cfg.get('amount',400),2)
  v(0x799a12+(building+length)*264,cfg.get('terminal',0),2)
  if 'zero_phase' in cfg:v(0x7999ea+(building+cfg['zero_phase'])*264,0,2)
  if 'last_amount' in cfg:v(0x799a14+(building+length)*264,cfg['last_amount'],2)
  if cfg.get('cycle'):
   v(0x799a12+building*264,building,2);v(0x7999ea+building*264,1,2);v(0x799a14+building*264,cfg.get('amount',400),2)
  return [building]
 if fn==0x4b7590:
  u.mem_write(0xbcb9c8,bytes(50*144));at=0xbcba9a+cfg.get('row',0)*144
  v(at-66,cfg.get('active',1),1);v(at,cfg.get('flag',1),1);v(at+4,cfg.get('value',1),2)
  return []
 if fn==0x50f0f0:
  x,y=cfg.get('position',(16,16));w,h=cfg.get('dimensions',(32,32));v(0xbca2d0,w);v(0xbca2d4,h);v(0xbca2d8,912);v(0xeadd6c,1)
  v(0x7999d8+264,1,1);v(0x7999de+264,x,2);v(0x7999e0+264,y,2);v(0x7999e8+264,cfg.get('selected_kind',20),2)
  v(0x7999de+4*264,x+1,2);v(0x7999e0+4*264,y+1,2);v(0x7999e8+4*264,20,2)
  items=cfg.get('items',[(cfg.get('dx',0),cfg.get('dy',0),2,cfg.get('kind',40),cfg.get('effect',-10))])
  for dx,dy,node,kind,effect in items:
   v(0xb4e6fc+(912+(y+dy)*228+x+dx)*2,node,2)
   if node<=0 or node==1:continue
   r=node*264;v(0x7999d8+r,cfg.get('state',1),1);v(0x7999dd+r,cfg.get('occupied',0),1);v(0x7999e8+r,kind,2)
   v(0x705ea8+kind*40,effect);v(0x705eb0+kind*40,cfg.get('change',1));v(0x705eb4+kind*40,cfg.get('range',6))
   v(0x799a10+r,cfg.get('parent',3) if kind==54 else 0,2)
  return []
 raise AssertionError(fn)

def exercise(fn=0x454190,**cfg):
 global checked,invocations
 expected=None;void=fn in [0x454190,0x50f0f0]
 for c,u,state in machines:
  for route in (['native'] if not c else (['named','canonical','menu'] if fn in names else ['canonical','god_frame'])):
   state.update(collect=False,own=True,writes=[],events=[],pending=set(),last_entry=None,top=fn,mutation=cfg.get('mutation'))
   args=prepare(u,fn,cfg);last[0]=None;state['collect']=True;invocations+=1
   pc=fn if not c else sym(names[fn] if route=='named' else 'restore_god_building_%08x'%fn if route=='god_frame' else 'FUN_%08x'%fn)
   try:
    if route=='menu':
     frame=S+0x2000;virtual_sp=S+0x3000;initial=[0x11111111,0x22222222,0x33333333,saved[0],virtual_sp,*saved[1:]]
     u.mem_write(frame,struct.pack('<10I',*initial,2,0));put(u,virtual_sp,STOP)
     for i,a in enumerate(args):put(u,virtual_sp+4+i*4,a)
     run(0,u,sym('restore_menu_input_boundary_dispatch'),[0,fn,frame],budget=3000000)
     out=struct.unpack('<8I',bytes(u.mem_read(frame,32)));assert out[4]==virtual_sp+4
     assert [out[i] for i in [3,5,6,7]]==saved
     result=out[0]
    else:result=run(0,u,pc,args,budget=3000000)
   except Exception as e:raise AssertionError(('execute',hex(fn),cfg,route,hex(u.reg_read(UC_X86_REG_EIP)))) from e
   state['collect']=False;answer=(tuple(state['writes']),tuple(state['events']),snapshot(u),0 if void else result)
   if expected is None:expected=answer
   elif answer!=expected:
    diffs=[next(((i,a,b) for i,(a,b) in enumerate(zip(x,y)) if a!=b),None) for x,y in zip(expected[:2],answer[:2])]
    memory=next(((hex(0x560000+i),a,b) for i,(a,b) in enumerate(zip(expected[2],answer[2])) if a!=b),None)
    raise AssertionError((hex(fn),cfg,route,'stores/events',diffs,'lengths',[(len(x),len(y)) for x,y in zip(expected[:2],answer[:2])],'memory',memory,'result',(expected[3],answer[3])))
 checked+=1
 if checked%50==0:print('progress',checked,'scenarios',flush=True)

if __name__=='__main__':
 quick='--quick' in sys.argv
 for fn,cfg in [(0x454190,{}),(0x475b50,dict(length=524)),(0x45d570,{}),(0x4b7590,dict(row=48)),(0x50f0f0,dict(kind=54,dx=3)),(0x454190,dict(nxt={8:10},bytes={0x799a58:1}))]:exercise(fn,**cfg)
 if not quick:
  for phase in ['cur','nxt']:
   for water in [0,1,2,3]:
    for well,fountain in [(0,0),(1,0),(0,1),(1,1)]:exercise(**{phase:{12:water}},bytes={0x799a0d:well,0x799a17:fountain})
   for required in [1,9,10,24,25,49,50,79,80,128]:
    for have in [0,1,127,-1]:exercise(**{phase:{8:required}},bytes={0x799a58:have})
   for required,have in itertools.product([1,2,3,4],[0,1,2,3,4]):
    exercise(**{phase:{40:required}},foodlist=list(range(1,have+1)),stocks={i:1 for i in range(have)})
   for required in [1,2,3,4]:
    for flags in [(0,0),(1,0),(0,1)]:exercise(**{phase:{20:required}},bytes={0x799a4a:flags[0],0x799a4b:flags[1]})
   for required in [1,2,3,4]:exercise(**{phase:{16:required}})
   for offset in [28,32,36]:
    for required in [0,1,2]:
     for flag in [0,1]:exercise(**{phase:{offset:required}},bytes={0x799a46:flag})
   for j,at in enumerate([0x799a3e,0x799a40,0x799a42,0x799a3c]):
    for value in [-1,0,1,2,5,2147483647]:
     for merged in [0,1]:exercise(**{phase:{44+j*4:value}},merged=merged,words={at:0})
   for capacity in [0,1,2,3]:exercise(**{phase:{52:2}},words={0x799a42:2},capacity=capacity,player=3)
  for phase in ['cur','nxt']:
   for offset,at in [(28,0x799a4d),(32,0x799a48),(36,0x799a5a)]:
    for have in [128,255]:exercise(**{phase:{offset:256 if offset!=36 else 0}},bytes={at:have})
   for at,offset in [(0x799a3e,44),(0x799a40,48),(0x799a42,52),(0x799a3c,56)]:
    for have in [-32768,-1]:exercise(**{phase:{offset:0}},words={at:have})
  for mode in [0,1,2]:
   for value in [-1,0,1,2,5,2147483647]:
    exercise(cur={44+j*4:value for j in range(4)},nxt={44+j*4:value for j in range(4)},merged=mode,
     words={at:32767 for at in [0x799a3e,0x799a40,0x799a42,0x799a3c]})
  for level in [-1,0,18,19,20]:
   for desire in [-128,-100,0,10,127]:exercise(level=level,desire=desire)
  for influence in [0,1]:exercise(lower=0,upper=20,influence=influence)
  for blocked in [0,1,2]:exercise(blocked=blocked)
  for resource in [-1,*range(21),0xffffffff]:
   exercise(foodlist=[resource],stocks={i:1 for i in range(-1,12)})
  exercise(food_count=-1);exercise(food_count=0)
  exercise(foodlist=[1,2,3],stocks={0:0,1:1,2:0})
  exercise(foodlist=[1,17,19,0],stocks={0:-1,10:1,11:0})
  for length in [0,1,8,524,525,526]:
   for terminal in [-1,0]:exercise(0x475b50,length=length,terminal=terminal)
  exercise(0x475b50,cycle=True)
  for length in range(9):exercise(0x45d570,length=length)
  for n in range(1,9):exercise(0x45d570,zero_phase=n)
  for amount in [-32768,-1,0,399,400,401]:exercise(0x45d570,amount=amount)
  for tail_amount in [399,400,401]:exercise(0x45d570,last_amount=tail_amount)
  exercise(0x45d570,cycle=True);exercise(0x45d570,length=0,terminal=-1);exercise(0x45d570,phase=-1)
  for row,active,flag,value in itertools.product([0,48,49],[0,1,2],[0,1,255],[-1,0,1]):exercise(0x4b7590,row=row,active=active,flag=flag,value=value)
  for dx,dy in itertools.product([-6,-1,0,1,6],repeat=2):exercise(0x50f0f0,dx=dx,dy=dy)
  for state in [0,1,6]:exercise(0x50f0f0,state=state)
  for kind in [-1,19,20,21,54]:
   for occupied in [0,1]:exercise(0x50f0f0,kind=kind,occupied=occupied)
  for effect,change,reach in itertools.product([-10,0,10],[-2,1,2147483647],[0,1,6]):exercise(0x50f0f0,effect=effect,change=change,range=reach,dx=3,dy=2)
  for position in [(0,0),(31,31),(-10,16),(45,16),(16,-10),(16,45)]:exercise(0x50f0f0,position=position,items=[])
  for node in [-1,0,1]:exercise(0x50f0f0,items=[(0,0,node,40,-10)])
  exercise(0x50f0f0,items=[(-1,0,2,40,-10),(1,0,3,41,-20),(2,0,5,42,-20)])
  exercise(0x50f0f0,kind=54,mutation='selected')
  exercise(0x50f0f0,items=[(0,0,2,54,-10),(2,1,5,41,-20)],mutation='selected')
  for fn in [0x46d0f0,0x45b2a0]:
   for length in [0,1,524,525]:exercise(fn,length=length)
  for length in [0,1,5]:exercise(0x45b2a0,length=length,delivery=True)
 missing=points-hits;missing_edges={(a,b) for a,ends in branches.items() for b in ends if (a,b) not in edges}
 print('Compared information:',checked,'scenarios;',invocations,'invocations;',len(points)-len(missing),'/',len(points),'instructions,',len(edges),'conditional edges;',len(switches),'switch targets',flush=True)
 print('Unexecuted instructions:',[hex(a) for a in sorted(missing)],'; edges:',[(hex(a),hex(b)) for a,b in sorted(missing_edges)],flush=True)
 if quick:sys.exit(0)
 assert not missing,('instruction coverage',missing)
 assert not missing_edges,('edge coverage',missing_edges)
 table=bytes.fromhex(catalogue['tables'][0]['bytes_hex']);assert switches==set(struct.unpack('<19I',table))
 print('PASS complete building-information comparison and coverage',flush=True)
 sources=['CMakeLists.txt','include/menu_prepare_integer.h','runtime/menu_prepare_integer.c','runtime/god_building_effects.c','direct_c/menu_input_boundary.c','src/unit_00884.c','direct_c/verify_building_information.py','metadata/building-information-native.json']
 proof=dict(native_sha256=catalogue['native_sha256'],compiled_C_path=str(target.resolve()),compiled_C_sha256=target_sha,passed=True,checked=checked,invocations=invocations,
  coverage={hex(a):dict(instructions=len(p),executed=len(p&hits)) for a,p in owned.items()},conditional_edges=len(edges),switch_targets=[hex(p) for p in sorted(switches)],
  unexecuted_instructions=[hex(a) for a in sorted(missing)],unexecuted_edges=[[hex(a),hex(b)] for a,b in sorted(missing_edges)],
  source_hashes={p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest() for p in sources},commit=None,controlled_algorithm_endpoints=[],controlled_platform_endpoints=[],
  limitations=['Actual native/C root traversals and raw caller compositions. Ordered owned stores, real helper inputs, all final original static data, full DWORD nonvoid outputs and saved ABI compared.',
  '454190 and 50F0F0 are void contracts authenticated by caller/decompiler evidence; incidental registers/flags and private stack scratch excluded. No concurrent access, host game launch or UI fidelity claim.'])
 suffix='diagnostic' if target.stem=='restore_link_check' else 'game'
 (ROOT/f'metadata/building-information-{suffix}.json').write_text(json.dumps(proof,indent=2)+'\n')
