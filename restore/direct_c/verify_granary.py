"""Offline original/C granary supply, gathering, delivery and withdrawal.

Execute actual children; compare ordered owned stores, child inputs, all original
static data and the saved ABI. Actual route, distance and storage children execute;
no child results are replaced and no host game is launched.
"""
from verify_options_startup import *
from unicorn import UC_HOOK_MEM_WRITE
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import json
catalogue=json.loads((ROOT/'metadata/granary-native.json').read_text())
assert catalogue['native_sha256']==hashlib.sha256(original.read_bytes()).hexdigest()
names={0x45dcc0:'restore_granary_gather_destination',0x45de80:'restore_granary_find_gather_source',0x45e040:'restore_granary_rebuild_supply',0x45e150:'restore_granary_take_gathered',0x45e220:'restore_granary_deliver',0x45e360:'restore_granary_withdraw'}
owned={};branches={};hits=set();edges=set();last=[None];checked=invocations=0
decoder=Cs(CS_ARCH_X86,CS_MODE_32)
for f in catalogue['functions']:
 a=int(f['address'],16);raw=bytes.fromhex(f['bytes_hex']);assert raw==bytes(images[0].data[a-images[0].base:a-images[0].base+len(raw)])
 instructions=list(decoder.disasm(raw,a));owned[a]={int(p,16) for p in f['instruction_addresses'].split()}
 assert {i.address for i in instructions}==owned[a]
 if a==0x45dcc0:
  by_pc={i.address:i for i in instructions};seen=set();todo=[a]
  while todo:
   pc=todo.pop()
   if pc in seen:continue
   assert pc in by_pc,('unbounded DCC0 flow',hex(pc))
   seen.add(pc);i=by_pc[pc]
   if i.mnemonic.startswith('ret'):continue
   if i.mnemonic.startswith('j'):
    todo.append(int(i.op_str,16))
    if i.mnemonic=='jmp':continue
   todo.append(pc+i.size)
  assert seen==owned[a]
 for i in instructions:
  if i.mnemonic.startswith('j') and i.mnemonic!='jmp':branches[i.address]={int(i.op_str,16),i.address+i.size}
for t in catalogue.get('tables',[]):
 a=int(t['address'],16);b=bytes.fromhex(t['bytes_hex']);assert b==bytes(images[0].data[a-images[0].base:a-images[0].base+len(b)])
for f in catalogue.get('auxiliary_functions',[]):
 a=int(f['address'],16);raw=bytes.fromhex(f['bytes_hex']);assert raw==bytes(images[0].data[a-images[0].base:a-images[0].base+len(raw)])
points=set.union(*owned.values());machines=[]
calls={0x45dc50:6,0x45e360:4,0x4cde60:2,0x4b28f0:3}
for c,pe in enumerate(images):
 u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(pe.base,(len(pe.data)+4095)&~4095);u.mem_write(pe.base,bytes(pe.data))
 u.mem_map(0x30000000,0x10000);u.mem_map(STOP,4096);u.mem_map(S,0x50000)
 state=dict(collect=False,own=True,writes=[],events=[],pending=set(),returns=set(),compiled=c,last_entry=None);machines.append((c,u,state))
 def write(uc,access,at,size,value,data):
  if data['collect'] and data['own']:
   data['writes'].append((at,size,value&((1<<(size*8))-1)))
   cfg=data['cfg'];mutation=data['mutation'];resource=cfg.get('resource',1);player=cfg.get('player',0)
   if mutation=='counter' and at in [0x799a2e+264,0x799a2e+264+resource*2]:put(uc,0xea60ac+player*0x49d8+resource*4,9999)
   if mutation=='free' and at==0xea5e54+cfg.get('owner',player)*0x49d8:word(uc,0x799a2e+264,20)
   if mutation=='bound' and data['top']==0x45e040 and at==0x78d8d4 and size==2:put(uc,0x78d790,1)
 u.hook_add(UC_HOOK_MEM_WRITE,write,state,begin=0x560000,end=0x126cfff)
 if not c:
  def code(uc,pc,size,data):
   if not data['collect']:return
   hits.add(pc)
   if last[0] in branches:edges.add((last[0],pc))
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
  if mutation=='amount' and t==0x45e360:word(uc,0x799a2e+args[1]*264+args[2]*2,0);put(uc,0xeaac28,3)
  if mutation=='slot' and t==0x45dc50:byte(uc,0x799a99+264,4)
  if mutation=='bound' and t==0x45dc50:put(uc,0x78d9a0,1)
  if mutation=='percentage' and t==0x4cde60:byte(uc,0x799a99+264,4);put(uc,0x78d790,1)
  if mutation=='selection' and t==0x4b28f0:put(uc,0x972534,0);put(uc,0x972538,0)
  if t not in owned:data['own']=False
  data['pending'].add(ret)
  if ret not in data['returns']:uc.hook_add(UC_HOOK_CODE,returned,data,begin=ret,end=ret);data['returns'].add(ret)
 # Observe each real C algorithm once at its named provider. Canonical and
 # compatibility wrappers can use CALL or a tail jump and are exercised below.
 state['entries']={(sym(names[t] if t in names else 'FUN_%08x'%t) if c else t):t for t in calls}
 for a in state['entries']:u.hook_add(UC_HOOK_CODE,child,state,begin=a,end=a)

frame_names={t:['restore_'+('city' if t==0x45e040 else 'sidebar' if t==0x45e220 else 'transport')+'_C_%08x'%t] for t in names}
def prepare(u,fn,cfg):
 def v(a,x,n=4):u.mem_write(a&0xffffffff,(x&((1<<(n*8))-1)).to_bytes(n,'little'))
 u.mem_write(0x560000,seed);u.mem_write(0x30000000,b'\xcc'*0x10000);u.mem_write(S,bytes(0x50000));u.mem_write(0x7999d8,bytes(4000*264));u.mem_write(0x7900bc,bytes(196*8))
 player=cfg.get('player',0);owner=cfg.get('owner',player);resource=cfg.get('resource',1);building=cfg.get('building',1)
 for p in {0,3,player,owner}:
  for res in range(-1,37):v(0xea60ac+p*0x49d8+res*4,cfg.get('city_stock',10000));v(0xea42f8+p*0x49d8+res*4,0)
  v(0xea5e54+p*0x49d8,cfg.get('produced_count',0xffffffff))
 for slot in range(8):
  for res in range(36):
   v(0x7900c2+slot*196+res,cfg.get('policy',0 if fn==0x45e040 else 2) if slot==1 else cfg.get('source_policy',0),1)
   v(0x7900ec+slot*196+res*2,cfg.get('limit',1000),2);v(0x790134+slot*196+res*2,cfg.get('alt_limit',1000),2)
 v(0xeaac28,owner);v(0x784bbc,cfg.get('blocked',0));v(0xd3b4e0,0x12345678);v(0xd3b4e4,0x87654321)
 def granary(bid,slot,stocks,free,kind=71):
  r=bid*264
  for a,x,n in [(0x7999d8,cfg.get('state',1),1),(0x7999e8,kind,2),(0x799a99,slot,1),(0x7999ec,cfg.get('network_byte',1),1),
   (0x799a1c,cfg.get('enabled',1),1),(0x7999f8,cfg.get('access',1),2),(0x799a18,cfg.get('workers',100),2),
   (0x7999de,12,2),(0x7999e0,11,2),(0x7999fe,10+bid,2),(0x799a00,90+bid,2),(0x799a2e,free,2)]:v(a+r,x,n)
  for res,stock in enumerate(stocks,1):v(0x799a2e+r+res*2,stock,2)
  v(0x705eb8+kind*40,cfg.get('max_workers',100))
 granary(1,1,cfg.get('stocks',[cfg.get('stock',100)]*8),cfg.get('free',1000),cfg.get('kind',71))
 if building!=1:granary(building,1,cfg.get('stocks',[cfg.get('stock',100)]*8),cfg.get('free',1000),cfg.get('kind',71))
 if resource not in range(1,9):v(0x799a2e+building*264+resource*2,cfg.get('stock',100),2)
 if fn==0x45e220:return [building,resource,cfg.get('produced',1),cfg.get('amount',100)]
 if fn==0x45e360:return [player,building,resource,cfg.get('amount',100)]
 for bid,slot in [(20,2),(30,3)]:
  granary(bid,slot,cfg.get('source_stocks',[cfg.get('source_stock',500 if bid==20 else 600)]*8),1000,cfg.get('source_kind',71))
  v(0x7999ec+bid*264,cfg.get('source_network',1),1);v(0x7999fe+bid*264,cfg.get('source_x',12 if bid==20 else 15),2);v(0x799a00+bid*264,cfg.get('source_y',11 if bid==20 else 15),2)
 for res,policy in cfg.get('policies',{}).items():v(0x7900c2+196+res,policy,1)
 for res,policy in cfg.get('source_policies',{}).items():v(0x7900c2+392+res,policy,1);v(0x7900c2+588+res,policy,1)
 ids=cfg.get('supply_ids',[20,30]);v(0x78d9a0,cfg.get('supply_count',len(ids)))
 v(0x78d790,cfg.get('bound',30))
 for i,bid in enumerate(ids):v(0x78d8d4+i*2,bid,2)
 if fn==0x45de80:return [1]
 if fn==0x45e150:return [1,20]
 if fn==0x45e040:
  u.mem_write(0x78d844,b'\xa5'*(144+200));v(0x78d99c,0xdeadbeef);v(0x78d9a0,99)
  if 'mass' in cfg:
   for bid in range(1,cfg['mass']+1):granary(bid,1,[cfg.get('stock',100)]*8,1000,cfg.get('kind',71))
  v(0x78d790,cfg.get('bound',cfg.get('mass',30)));return []
 assert fn==0x45dcc0
 granary(2,2,cfg.get('stocks',[cfg.get('stock',100)]*8),cfg.get('free',1000),cfg.get('kind',71));v(0x7999de+2*264,12,2);v(0x7999e0+2*264,11,2)
 v(0x7900c2+392+resource,cfg.get('policy',2),1)
 v(0x78d790,cfg.get('bound',2));v(0x96e6b0,99);v(0xea42f8+owner*0x49d8+resource*4,cfg.get('quota_gate',0))
 for y in range(6,20):
  for x in range(6,20):
   tile=y*228+x;v(0xec7630+tile,0,1);v(0xaf598c+tile*4,0);v(0xb4e6fc+tile*2,0,2)
 p=255*388
 for a,x in [(0xeadfd8,0xffffffff),(0xbca2d8,0),(0xbca2d0,228),(0xbca2d4,228),(0xd056f8,255)]:v(a,x)
 u.mem_write(0xd04e58,bytes(2000))
 for off,x in [(10,4),(14,1),(56,9),(57,14),(114,cfg.get('movement',2)),(118,0)]:v(0xbcd5f8+p+off,x,1)
 for off,x in [(20,10),(22,10),(36,13),(38,12),(58,0),(60,0),(62,0)]:v(0xbcd5f8+p+off,x,2)
 v(0xbcd5f8+p+32,2290);v(0xb350dc+2290*2,255,2)
 return [255,resource,cfg.get('network',1)]

def exercise(fn=0x45e360,**cfg):
 global checked,invocations
 expected=None;void=fn==0x45e040
 for c,u,state in machines:
  for route in (['native'] if not c else ['named','canonical',*frame_names[fn]]):
   state.update(collect=False,own=True,writes=[],events=[],pending=set(),last_entry=None,top=fn,mutation=cfg.get('mutation'),counts={},cfg=cfg)
   args=prepare(u,fn,cfg);last[0]=None;state['collect']=True;invocations+=1
   pc=fn if not c else sym(names[fn] if route=='named' else 'FUN_%08x'%fn if route=='canonical' else route)
   try:
    if cfg.get('rebuild'):run(0,u,0x45e040 if not c else sym(names[0x45e040]),[],budget=50000000)
    if route in frame_names[fn]:
     frame=S+0x1000;vsp=S+0x3000;initial=[0x11111111,0x22222222,0x33333333,saved[0],vsp,*saved[1:]]
     u.mem_write(frame,struct.pack('<10I',*initial,2,0));put(u,vsp,STOP)
     for i,a in enumerate(args):put(u,vsp+4+i*4,a)
     run(0,u,pc,[frame],budget=50000000)
     out=struct.unpack('<8I',bytes(u.mem_read(frame,32)));assert out[4]==vsp+4
     assert [out[i] for i in [3,5,6,7]]==saved;result=out[0]
    else:result=run(0,u,pc,args,budget=50000000)
   except Exception as e:raise AssertionError(('execute',hex(fn),cfg,route,hex(u.reg_read(UC_X86_REG_EIP)))) from e
   state['collect']=False;answer=(tuple(state['writes']),tuple(state['events']),snapshot(u),0 if void else result)
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
 for fn in names:exercise(fn)
 assert exercise(0x45dcc0)>0
 assert exercise(0x45e150,source_stocks=[10,20,30,40,50,60,70,1200])==800
 assert exercise(0x45de80,rebuild=True)==20
 assert exercise(0x45de80,policies={1:0,2:2})==20
 if not quick:
  for fn in [0x45e220,0x45e360]:
   for amount,stock in itertools.product([-2147483648,-1,0,1,99,100,101,2147483647],[-32768,-1,0,1,100,32767]):exercise(fn,amount=amount,stock=stock)
   for resource in [-1,0,1,8,9,35]:exercise(fn,resource=resource,mutation='counter')
   for player,owner in [(0,3),(3,0),(-1,-1)]:exercise(fn,player=player,owner=owner)
  for kind,policy in itertools.product([0,71,72,191,193],[0,1,2,3,255]):exercise(0x45e220,kind=kind,policy=policy)
  for building in [-1,0]:exercise(0x45e220,building=building)
  for free,amount in itertools.product([-1,0,1,99,100,101,32767],[0,1,100,200]):exercise(0x45e220,free=free,amount=amount)
  for limit in [-1,99,100,101]:exercise(0x45e220,policy=0,limit=limit);exercise(0x45e220,policy=2,alt_limit=limit)
  for produced in [0,1,-1]:exercise(0x45e220,produced=produced,amount=150,free=100,mutation='free')
  for fn in [0x45e040,0x45dcc0]:
   for kind,state,enabled,access in [(0,1,1,1),(71,0,1,1),(71,2,1,1),(71,1,0,1),(71,1,1,0),(71,1,1,-1),(191,1,1,1)]:exercise(fn,kind=kind,state=state,enabled=enabled,access=access)
   for bound in [-1,0,1,2]:exercise(fn,bound=bound)
  for stock,policy in itertools.product([-1,0,12,13,100],[0,1,2,3]):exercise(0x45e040,stock=stock,source_stock=stock,policy=policy,source_policy=policy)
  for mass in [97,98,99,100,150]:
   exercise(0x45e040,mass=mass)
   for _,u,_ in machines:assert get(u,0x78d9a0)==min(mass,98)
  exercise(0x45e040,mass=150,mutation='bound')
  for stock,free in itertools.product([-1,0,1,100,799,800,801,32767],[-1,0,1,100,800,1000]):exercise(0x45e150,source_stock=stock,free=free)
  exercise(0x45e150,source_stocks=[500,500,500,500,500,500,500,500],mutation='amount')
  for policy,source_policy in itertools.product([0,2],[0,2]):exercise(0x45e150,policy=policy,source_policy=source_policy)
  for cfg in [dict(policy=0),dict(blocked=1),dict(supply_count=0),dict(supply_count=-1),dict(source_network=2),dict(source_stock=0),dict(source_policy=2),dict(source_stock=50),dict(source_stock=51),dict(source_x=20000),dict(source_x=12,source_y=11),dict(mutation='slot'),dict(mutation='bound')]:exercise(0x45de80,**cfg)
  for cfg in [dict(blocked=1),dict(resource=0),dict(resource=9),dict(quota_gate=1),dict(network=257),dict(network_byte=2),dict(workers=99),dict(max_workers=0),dict(policy=0),dict(stock=1000),dict(free=100),dict(free=101),dict(movement=6),dict(mutation='percentage'),dict(mutation='selection')]:exercise(0x45dcc0,**cfg)
 blocked_instructions={0x45dead,0x45deae,0x45deb1,0x45deb3,0x45df51,0x45df52,0x45df55,0x45df57,0x45e0ca,0x45e0cb,0x45e0ce,0x45e0d0,0x45e194,0x45e195,0x45e198,0x45e19a}
 excluded={(0x45deab,0x45dead),(0x45df4f,0x45df51),(0x45e0c8,0x45e0ca),(0x45e192,0x45e194)}
 excluded.update((a,b) for a in blocked_instructions if a in branches for b in branches[a])
 assert not blocked_instructions&hits and not excluded&edges
 missing=points-hits-blocked_instructions;missing_edges={(a,b) for a,ends in branches.items() for b in ends if (a,b) not in edges and (a,b) not in excluded}
 print('Compared granaries:',checked,'scenarios;',invocations,'invocations;',len(points&hits),'/',len(points),'instructions,',len(edges),'conditional edges;',len(blocked_instructions),'analytically unreachable instructions',flush=True)
 print('Unexecuted reachable instructions:',[hex(a) for a in sorted(missing)],'; edges:',[(hex(a),hex(b)) for a,b in sorted(missing_edges)],flush=True)
 if quick:sys.exit(0)
 assert not missing,('instruction coverage',missing)
 assert not missing_edges,('edge coverage',missing_edges)
 sources=['CMakeLists.txt','include/granary.h','runtime/granary.c','runtime/actor_transport.c','runtime/actor_transport_model.c','runtime/city_updates.c','runtime/city_updates_model.c','runtime/sidebar_undo_model.c','src/unit_00906.c','direct_c/verify_granary.py','metadata/granary-native.json']
 proof=dict(native_sha256=catalogue['native_sha256'],compiled_C_path=str(target.resolve()),compiled_C_sha256=target_sha,passed=True,checked=checked,invocations=invocations,
  coverage={hex(a):dict(instructions=len(p),executed=len(p&hits),analytically_unreachable=len(p&blocked_instructions)) for a,p in owned.items()},conditional_edges=len(edges),
  unexecuted_instructions=[hex(a) for a in sorted(missing)],unexecuted_edges=[[hex(a),hex(b)] for a,b in sorted(missing_edges)],
  analytically_unreachable_instructions=sorted(map(hex,blocked_instructions)),analytically_unreachable_edges=[[hex(a),hex(b)] for a,b in sorted(excluded)],
  unreachable_reason='Four resource loops initialize EAX=0, increment to 1..8 and exit at 9. No body/helper changes that counter, so inner nonpositive recovery paths cannot execute.',
  source_hashes={p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest() for p in sources},commit=None,controlled_algorithm_endpoints=[],controlled_platform_endpoints=[],
  limitations=['Rea authenticates all native bytes. Connected Ghidra program of the same SHA supplies DE80/E150 Listing evidence; DCC0 lacks a Listing function and is independently decoded from authenticated bytes with complete bounded flow and frame ABI checks.',
  'Actual percentage, route, distance and withdrawal children execute. Named/canonical/frame adapters; full nonvoid DWORD returns, ordered owned stores, helper inputs, all original static data and saved ABI compared. Includes real routing, supply-rebuild composition, cached/fresh inputs, aliasing resource zero, word wrapping and saturated source-list sentinel.',
  'Incidental caller-saved registers/flags, private scratch, concurrent access and host game/UI fidelity are excluded.'])
 suffix='diagnostic' if target.stem=='restore_link_check' else 'game'
 (ROOT/f'metadata/granary-{suffix}.json').write_text(json.dumps(proof,indent=2)+'\n')

