"""Offline original/C city food/industry summaries and storage destinations.

Execute actual children; compare ordered owned stores, child inputs, all original
static data and the saved ABI. Actual supply, tutorial, route and road children execute;
no child results are replaced and no host game is launched.
"""
from verify_options_startup import *
from unicorn import UC_HOOK_MEM_WRITE
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import json
catalogue=json.loads((ROOT/'metadata/city-supply-native.json').read_text())
assert catalogue['native_sha256']==hashlib.sha256(original.read_bytes()).hexdigest()
names={0x45acf0:'restore_storage_delivery_destination',0x45d5d0:'restore_city_update_food',0x45d630:'restore_city_rebuild_food',0x45e6c0:'restore_city_update_industry',0x45e6e0:'restore_city_rebuild_industry'}
owned={};branches={};hits=set();edges=set();last=[None];checked=invocations=0; jump_edges=set()
decoder=Cs(CS_ARCH_X86,CS_MODE_32)
for f in catalogue['functions']:
 a=int(f['address'],16);raw=bytes.fromhex(f['bytes_hex']);assert raw==bytes(images[0].data[a-images[0].base:a-images[0].base+len(raw)])
 instructions=list(decoder.disasm(raw,a));owned[a]={int(p,16) for p in f['instruction_addresses'].split()}
 assert {i.address for i in instructions}==owned[a]
 for i in instructions:
  if i.mnemonic.startswith('j') and i.mnemonic!='jmp':branches[i.address]={int(i.op_str,16),i.address+i.size}
for t in catalogue.get('tables',[]):
 a=int(t['address'],16);b=bytes.fromhex(t['bytes_hex']);assert b==bytes(images[0].data[a-images[0].base:a-images[0].base+len(b)])
for f in catalogue.get('auxiliary_functions',[]):
 a=int(f['address'],16);raw=bytes.fromhex(f['bytes_hex']);assert raw==bytes(images[0].data[a-images[0].base:a-images[0].base+len(raw)])
switches={int(pc,16):set(struct.unpack_from('<%dI'%n,images[0].data,int(at,16)-images[0].base)) for pc,at,n in catalogue['switches']}
points=set.union(*owned.values());machines=[]
calls={0x475b50:1,0x45d510:2,0x480180:3,0x4806a0:2,0x4cde60:2,0x4b28f0:3,0x4c5340:1,0x4cde40:2,0x417ae0:1,0x45d630:1,0x45e6e0:1}
for c,pe in enumerate(images):
 u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(pe.base,(len(pe.data)+4095)&~4095);u.mem_write(pe.base,bytes(pe.data))
 u.mem_map(0x30000000,0x10000);u.mem_map(STOP,4096);u.mem_map(S,0x50000)
 state=dict(collect=False,own=True,writes=[],events=[],pending=set(),returns=set(),compiled=c,last_entry=None);machines.append((c,u,state))
 def write(uc,access,at,size,value,data):
  if data['collect'] and data['own']:
   data['writes'].append((at,size,value&((1<<(size*8))-1)))
   if data['mutation']=='secondary' and value and at==0xea54d4+data['cfg'].get('player',1)*0x49d8+8*4:
    word(uc,0x799aa6+264,0);word(uc,0x799aa4+264,700)
   if data['mutation']=='editor' and at==0x799a2e+264:
    put(uc,0x784bbc,0);put(uc,0x78d790,1)
 u.hook_add(UC_HOOK_MEM_WRITE,write,state,begin=0x560000,end=0x126cfff)
 if not c:
  def code(uc,pc,size,data):
   if not data['collect']:return
   hits.add(pc)
   if last[0] in branches:edges.add((last[0],pc))
   if last[0] in switches:jump_edges.add((last[0],pc))
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
  data['counts'][t]=data['counts'].get(t,0)+1
  mutation=data['mutation'];cfg=data['cfg']
  if mutation=='bound' and t in [0x480180,0x4806a0]:put(uc,0x78d790,1)
  if mutation=='kind' and t in [0x480180,0x4806a0]:word(uc,0x7999e8+264,cfg.get('newkind',47))
  if mutation=='slot' and t==0x45d510:
   byte(uc,0x799a99+264,4);word(uc,0x7900ec+196+cfg.get('resource',1)*2,0)
  if mutation=='root' and t==0x4cde60:word(uc,0x799a10+2*264,10)
  if mutation=='finalroad' and t==0x4b28f0:
   byte(uc,0x799a1c+264,2)
   for at in data['road_cells']:put(uc,0xaf598c+at*4,0)
  if mutation=='tutorial' and t==0x417ae0:put(uc,0xe38ebc,1)
  if mutation=='population' and t==0x4c5340:put(uc,0xea1894+cfg.get('player',1)*0x49d8,2000)
  if t not in owned:data['own']=False
  data['pending'].add(ret)
  if ret not in data['returns']:uc.hook_add(UC_HOOK_CODE,returned,data,begin=ret,end=ret);data['returns'].add(ret)
 # Observe each real C algorithm once at its named provider. Canonical and
 # compatibility wrappers can use CALL or a tail jump and are exercised below.
 state['entries']={(sym(names[t] if t in names else 'FUN_%08x'%t) if c else t):t for t in calls}
 for a in state['entries']:u.hook_add(UC_HOOK_CODE,child,state,begin=a,end=a)


frame_names={0x45acf0:['restore_transport_C_0045acf0'],0x45d5d0:['restore_city_C_0045d5d0','restore_advisor_C_0045d5d0'],0x45d630:['restore_city_C_0045d630','restore_advisor_C_0045d630'],0x45e6c0:['restore_city_C_0045e6c0'],0x45e6e0:['restore_city_C_0045e6e0']}

def prepare(u,fn,cfg,state):
 def v(a,x,n=4):u.mem_write(a&0xffffffff,(x&((1<<(n*8))-1)).to_bytes(n,'little'))
 def record(bid,fields):
  for a,x,n in fields:v(a+bid*264,x,n)
 u.mem_write(0x560000,seed);u.mem_write(0x30000000,b'\xcc'*0x10000);u.mem_write(S,bytes(0x50000));u.mem_write(0x7999d8,bytes(4000*264))
 player=cfg.get('player',1);off=player*0x49d8;v(0xeaac28,player)
 for owner in {0,1,player}:
  for i in range(10):v(0xea5f1c+owner*0x49d8+i*8,i+1)
 v(0xbca2d8,0);v(0xe38ebc,cfg.get('mode',1));v(0x784bbc,cfg.get('editor',0));v(0xea624a,cfg.get('monthly',1));v(0xea1894+off,cfg.get('population',1000));v(0xea5ce8+off,0)
 for a in [0xea41dc,0xea41e0,0xea4388,0xea438c,0xea5e20,0xea5e24,0xea5e28]:v(a+off,7654321)
 for base,count in [(0xea60ac,9),(0xea54d4,27),(0xea5574,27)]:
  for i in range(count):v(base+off+i*4,123456+i)
 # All actual movement/search children execute within this bounded district.
 for base in [0xee0c50,0xebab20,0xec7630]:u.mem_write(base,b'\x80'*51984)
 for y in range(6,20):
  for x in range(6,20):
   tile=y*228+x
   for base in [0xee0c50,0xebab20,0xec7630]:v(base+tile,0,1)
   v(0xaf598c+tile*4,0);v(0xb4e6fc+tile*2,0,2)
 for a,x in [(0xeadfd8,0xffffffff),(0xbca2d0,228),(0xbca2d4,228),(0xd056f8,255)]:v(a,x)
 u.mem_write(0xd04e58,bytes(2000));p=255*388
 for a,x in [(10,4),(14,1),(56,9),(57,14),(114,cfg.get('movement',2)),(118,0)]:v(0xbcd5f8+p+a,x,1)
 for a,x in [(20,10),(22,10),(36,13),(38,12),(58,0),(60,0),(62,0)]:v(0xbcd5f8+p+a,x,2)
 v(0xbcd5f8+p+32,2290);v(0xb350dc+2290*2,255,2);state['road_cells']=[]
 def road(x,y,orientation):
  if not cfg.get('roads',True):return
  offsets=struct.unpack_from('<24i',images[0].data,0x5cc9e0+orientation*96-images[0].base)
  cell=y*228+x+next(k for k in offsets if k);state['road_cells'].append(cell);v(0xaf598c+cell*4,0x40);v(0xa76aec+cell,1,1)
 if fn==0x45acf0:
  resource=cfg.get('resource',1);nodes=cfg.get('nodes',list(range(2,10)));stock=cfg.get('stock',0);v(0x78d790,cfg.get('bound',max(nodes) if nodes else 0));v(0x96e6b0,99)
  for bid,x,y in [(1,12,11),(10,16,14)]:
   record(bid,[(0x7999d8,1,1),(0x7999e8,72,2),(0x799a1c,cfg.get('root_enabled',1),1),(0x7999de,x,2),(0x7999e0,y,2),(0x7999fe,x+1,2),(0x799a00,y+1,2),(0x799a99,1,1),(0x799a18,cfg.get('workers',100),2)])
   road(x,y,3)
  v(0x705eb8+72*40,100);v(0x7900c2+196+resource,cfg.get('policy',0),1)
  for base in [0x7900ec,0x790134]:v(base+196+resource*2,cfg.get('capacity',3200),2)
  for i,bid in enumerate(nodes):
   record(bid,[(0x7999d8,cfg.get('state',1),1),(0x7999e8,cfg.get('kind',73),2),(0x799a1c,cfg.get('enabled',1),1),(0x7999f8,cfg.get('access',1),2),(0x7999ec,cfg.get('network_byte',1),1),(0x7999ea,cfg.get('phase',resource),2),(0x799a14,stock,2),(0x799a10,cfg.get('parent',1),2),(0x799a12,nodes[i+1] if i+1<len(nodes) else 0,2)])
  v(0x799a12+264,nodes[0] if nodes else 0,2)
  return [cfg.get('source',0),255,resource,cfg.get('network',1)]
 if fn in [0x45d630,0x45d5d0]:
  for i in [1,2]:
   kind=cfg.get('kind',71 if i==1 else 191);x=11+i;y=11
   record(i,[(0x7999d8,cfg.get('state',1),1),(0x7999e8,kind,2),(0x7999de,x,2),(0x7999e0,y,2),(0x799a18,cfg.get('workers',100),2),(0x799a1c,9,1)])
   v(0x705eb8+kind*40,100)
   stocks=cfg.get('foods',[0,100,200,0,0,0,0,0])
   v(0x799a2e+i*264,1000,2)
   for res,value in enumerate(stocks,1):v(0x799a2e+i*264+res*2,value,2)
   road(x,y,4)
  record(3,[(0x7999d8,1,1),(0x7999e8,70,2),(0x7999dd,cfg.get('house_active',1),1),(0x7999f4,cfg.get('house_workers',1),2)])
  for i,value in enumerate(cfg.get('house_foods',[0,0,0,100,0,0,0,0])):v(0x799a2c+3*264+i*2,value,2)
  v(0x78d790,cfg.get('bound',3))
  if cfg.get('mode',1)==0:
   # Occupied real message slots keep the actual tutorial pipeline offline.
   v(0xe38dda,0,1);v(0x605a0c,1);v(0x605a3c,0)
   for i in range(1000):v(0xe95232+i*48,1,2)
   v(0x7027b9,cfg.get('previous',1),1);v(0x7027ba,cfg.get('stage2',0),1);v(0x7027bf,cfg.get('stage7',0),1)
   scene=0x579574 if cfg.get('tutorial',2)==7 else 0x5794e8
   v(0xe38ea8,cfg.get('scene',get(u,scene)),1)
  return [player] if fn==0x45d630 else []
 for i in [1,2]:
  x=11+i;y=11;kind=cfg.get('kind',32 if i==1 else 204)
  record(i,[(0x7999d8,cfg.get('state',1),1),(0x7999e8,kind,2),(0x7999de,x,2),(0x7999e0,y,2),(0x7999db,cfg.get('orientation',4),1),(0x799a1c,9,1),(0x7999ea,cfg.get('type',8),2),(0x799a14,cfg.get('stock',100),2),(0x799aa6,cfg.get('secondary',14),2),(0x799aa4,cfg.get('secondary_stock',250),2)])
  road(x,y,cfg.get('orientation',4))
 v(0x78d790,cfg.get('bound',2))
 return [player] if fn==0x45e6e0 else []

def exercise(fn=0x45acf0,**cfg):
 global checked,invocations
 expected=None
 for c,u,state in machines:
  for route in (['native'] if not c else ['named','canonical',*frame_names[fn]]):
   state.update(collect=False,own=True,writes=[],events=[],pending=set(),last_entry=None,top=fn,mutation=cfg.get('mutation'),counts={},cfg=cfg)
   args=prepare(u,fn,cfg,state);last[0]=None;state['collect']=True;invocations+=1
   pc=fn if not c else sym(names[fn] if route=='named' else 'FUN_%08x'%fn if route=='canonical' else route)
   try:
    if route in frame_names[fn]:
     frame=S+0x1000;vsp=S+0x3000;initial=[0x11111111,0x22222222,0x33333333,saved[0],vsp,*saved[1:]]
     u.mem_write(frame,struct.pack('<10I',*initial,2,0));put(u,vsp,STOP)
     for i,a in enumerate(args):put(u,vsp+4+i*4,a)
     run(0,u,pc,[frame],budget=50000000)
     out=struct.unpack('<8I',bytes(u.mem_read(frame,32)));assert out[4]==vsp+4
     assert [out[i] for i in [3,5,6,7]]==saved;result=out[0]
    else:result=run(0,u,pc,args,budget=50000000)
   except Exception as e:raise AssertionError(('execute',hex(fn),cfg,route,hex(u.reg_read(UC_X86_REG_EIP)))) from e
   state['collect']=False;answer=(tuple(state['writes']),tuple(state['events']),snapshot(u),result if fn==0x45acf0 else None)
   if expected is None:expected=answer
   elif answer!=expected:
    diffs=[next(((i,a,b) for i,(a,b) in enumerate(zip(x,y)) if a!=b),None) for x,y in zip(expected[:2],answer[:2])]
    memory=next(((hex(0x560000+i),a,b) for i,(a,b) in enumerate(zip(expected[2],answer[2])) if a!=b),None)
    raise AssertionError((hex(fn),cfg,route,'stores/events',diffs,'lengths',[(len(x),len(y)) for x,y in zip(expected[:2],answer[:2])],'memory',memory,'result',(expected[3],answer[3])))
 checked+=1
 if checked%25==0:print('progress',checked,'scenarios',flush=True)
 return expected[3]

if __name__=='__main__':
 quick='--quick' in sys.argv
 for fn in names:
  value=exercise(fn)
  if fn==0x45acf0:assert value>0
 for cfg in [dict(root_enabled=2),dict(policy=2,source=1),dict(mutation='finalroad'),dict(mutation='slot')]:exercise(**cfg)
 exercise(0x45d630,mode=0,foods=[0,0,0,0,0,400,0,400])
 exercise(0x45d630,mode=0,tutorial=7,foods=[0,0,0,0,0,800,0,0])
 exercise(0x45e6e0,mutation='secondary')
 if not quick:
  for cfg in [dict(resource=21),dict(resource=10),dict(resource=28),dict(nodes=[]),dict(bound=-1),dict(state=0),dict(state=2),dict(kind=72),dict(enabled=0),dict(access=0),dict(access=-1),dict(network=257),dict(network_byte=2),dict(source=1),dict(policy=1),dict(policy=3),dict(policy=2),dict(capacity=0),dict(capacity=-1),dict(stock=400),dict(stock=399),dict(stock=-1),dict(phase=0,stock=400),dict(phase=2),dict(phase=-1),dict(root_enabled=0,roads=False),dict(root_enabled=2,roads=False),dict(movement=6),dict(mutation='root'),dict(mutation='bound')]:exercise(**cfg)
  for workers in [-1,0,49,99,100,101]:exercise(workers=workers)
  exercise(stock=400,capacity=4000)
  for policy in [0,2]:
   for cap in [0,799,800,801,32767,-32768]:exercise(policy=policy,capacity=cap,stock=100)
  for fn in [0x45d630,0x45e6e0]:
   for cfg in [dict(bound=0),dict(bound=-1),dict(state=0),dict(state=2),dict(roads=False),dict(player=0),dict(player=3),dict(mutation='bound'),dict(mutation='kind')]:exercise(fn,**cfg)
  for cfg in [dict(workers=w) for w in [-1,0,49,50,99,100,101]]+[dict(kind=70),dict(kind=190),dict(house_active=0),dict(house_workers=0),dict(editor=1),dict(population=0),dict(population=-1),dict(population=2147483647),dict(mutation='population'),dict(foods=[0]*8),dict(foods=[-100]*8),dict(workers=49,foods=[0]*8),dict(foods=[0]*8,house_foods=[100]*8)]:exercise(0x45d630,**cfg)
  for tutorial in [2,7]:
   for amount in [0,199,200,399,400,799,800]:
    foods=[0]*8;foods[7 if tutorial==2 else 5]=amount
    exercise(0x45d630,mode=0,tutorial=tutorial,foods=foods)
   for cfg in [dict(previous=0),dict(stage2=1),dict(stage7=1),dict(scene=127),dict(mutation='tutorial')]:exercise(0x45d630,mode=0,tutorial=tutorial,foods=[0,0,0,0,0,800,0,400],**cfg)
  for kind in [-1,0,31,32,33,47,51,53,74,84,85,86,87,110,111,112,113,114,115,177,178,184,187,188,189,190,194,203,204,205,226,231,232,233,234]:exercise(0x45e6e0,kind=kind)
  for kind in [32,47,74,187,233]:
   for stock in [-32768,-1,0,199,200,299,300,499,500,32767]:exercise(0x45e6e0,kind=kind,stock=stock,secondary_stock=stock)
  for type_ in [-1,0,1,26,27,40]:exercise(0x45e6e0,type=type_,secondary=type_)
  for kind in [31,47,84,233]:exercise(0x45e6e0,mutation='kind',newkind=kind)
  for fn in [0x45d5d0,0x45e6c0]:
   for monthly in [0,1,256]:exercise(fn,monthly=monthly)
  for cfg in [dict(monthly=0,bound=0),dict(monthly=0,bound=-1),dict(monthly=0,kind=70,editor=1),dict(monthly=0,kind=190,editor=1),dict(monthly=0,kind=70,editor=1,mutation='editor'),dict(monthly=0,kind=70,editor=1,state=0),dict(editor=1)]:exercise(0x45d5d0,**cfg)
 missing=points-hits;missing_edges={(a,b) for a,ends in branches.items() for b in ends if (a,b) not in edges};missing_jumps={(pc,target) for pc,targets in switches.items() for target in targets}-jump_edges
 print('Compared city supply:',checked,'scenarios;',invocations,'invocations;',len(points&hits),'/',len(points),'instructions,',len(edges),'conditional edges;',len(jump_edges),'switch edges',flush=True)
 if quick:sys.exit(0)
 print('Unexecuted instructions:',[hex(a) for a in sorted(missing)],'; edges:',[(hex(a),hex(b)) for a,b in sorted(missing_edges)],'; switch edges:',[(hex(a),hex(b)) for a,b in sorted(missing_jumps)],flush=True)
 # The food loop indexes exactly 0..7: five switch targets and its JA arm are unreachable.
 cold={0x45d732,0x45d737,0x45d739,0x45d73e,0x45d740,0x45d745,0x45d747,0x45d74c,0x45d74e}
 cold_edges={(0x45d6f4,0x45d74e)};cold_jumps={(0x45d6f6,t) for t in [0x45d732,0x45d739,0x45d740,0x45d747,0x45d74e]}
 assert not missing-cold,('instruction coverage',missing-cold)
 assert not missing_edges-cold_edges,('edge coverage',missing_edges-cold_edges)
 assert not missing_jumps-cold_jumps,('switch coverage',missing_jumps-cold_jumps)
 sources=['CMakeLists.txt','include/city_supply.h','runtime/city_supply.c','include/industry_logistics.h','runtime/industry_logistics.c','runtime/actor_transport.c','runtime/actor_transport_model.c','runtime/city_updates.c','runtime/city_updates_model.c','runtime/menu_advisor_model.c','src/unit_00910.c','direct_c/verify_city_supply.py','metadata/city-supply-native.json']
 proof=dict(native_sha256=catalogue['native_sha256'],compiled_C_path=str(target.resolve()),compiled_C_sha256=target_sha,passed=True,checked=checked,invocations=invocations,
  coverage={hex(a):dict(instructions=len(p),executed=len(p&hits)) for a,p in owned.items()},conditional_edges=len(edges),switch_edges=len(jump_edges),
  unexecuted_instructions=[hex(a) for a in sorted(missing-cold)],unexecuted_edges=[[hex(a),hex(b)] for a,b in sorted(missing_edges-cold_edges)],unexecuted_switch_edges=[[hex(a),hex(b)] for a,b in sorted(missing_jumps-cold_jumps)],
  analytically_unreachable_instructions=[hex(a) for a in sorted(cold)],analytically_unreachable_edges=[[hex(a),hex(b)] for a,b in sorted(cold_edges)],analytically_unreachable_switch_edges=[[hex(a),hex(b)] for a,b in sorted(cold_jumps)],
  source_hashes={p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest() for p in sources},commit=None,controlled_algorithm_endpoints=[],controlled_platform_endpoints=[],
  limitations=['Rea authenticates native instructions and tables. Food-loop index is provably limited to 0..7; nine instructions, one conditional outcome and five switch targets are unreachable.',
   'Actual road, staffing, stock, root, route, consumption and tutorial/event children execute. A full real message queue keeps notifications offline. Named/canonical/seven frame adapters; ordered stores, child inputs, all original static data and saved ABI compared. Only the destination has a defined DWORD return.',
   'Incidental caller-saved registers/flags, private scratch, concurrent access and host game/UI fidelity are excluded.'])
 suffix='diagnostic' if target.stem=='restore_link_check' else 'game'
 (ROOT/f'metadata/city-supply-{suffix}.json').write_text(json.dumps(proof,indent=2)+'\n')

