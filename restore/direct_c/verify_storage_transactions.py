"""Offline original/C transactions, gathering priorities and storage sources.

Execute actual children; compare ordered owned stores, child inputs, all original
static data and the saved ABI. Actual root traversal and native callers execute;
no child results are replaced and no host game is launched.
"""
from verify_options_startup import *
from unicorn import UC_HOOK_MEM_WRITE
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import json
catalogue=json.loads((ROOT/'metadata/storage-transactions-native.json').read_text())
assert catalogue['native_sha256']==hashlib.sha256(original.read_bytes()).hexdigest()
names={0x45d010:'restore_storage_export',0x45d370:'restore_storage_import',0x45d4d0:'restore_trade_charge_import',0x45b880:'restore_storage_rank_gathering',0x45bb50:'restore_storage_find_source',0x45dc50:'restore_storage_distance'}
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
points=set.union(*owned.values());machines=[]
calls={0x4434e0:3,0x4435f0:3,0x45d510:2,0x45b570:4,0x45b2a0:4,0x45d4d0:3,0x4cde40:2,0x45bb50:3,0x45dc50:6}
for c,pe in enumerate(images):
 u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(pe.base,(len(pe.data)+4095)&~4095);u.mem_write(pe.base,bytes(pe.data))
 u.mem_map(0x30000000,0x10000);u.mem_map(STOP,4096);u.mem_map(S,0x50000)
 state=dict(collect=False,own=True,writes=[],events=[],pending=set(),returns=set(),compiled=c,last_entry=None);machines.append((c,u,state))
 def write(uc,access,at,size,value,data):
  if data['collect'] and data['own']:
   if at>=S and not S+0x100<=at<S+0x120:return
   data['writes'].append((at,size,value&((1<<(size*8))-1)))
   if data['mutation']=='price' and at==0xea187c+data['cfg'].get('player',0)*0x49d8:
    put(uc,0x5d2c4c+data['cfg'].get('resource',1)*8,987)
 u.hook_add(UC_HOOK_MEM_WRITE,write,state,begin=0x560000,end=0x126cfff)
 u.hook_add(UC_HOOK_MEM_WRITE,write,state,begin=S+0x100,end=S+0x11f)
 if not c:
  def code(uc,pc,size,data):
   if not data['collect']:return
   hits.add(pc)
   if last[0] in branches:edges.add((last[0],pc))
   if last[0]==0x45bbc8:jump_edges.add(pc)
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
  if t==0x45bb50:args=args[:2]+(args[2]&255,)
  data['events'].append((t,args))
  data['counts'][t]=data['counts'].get(t,0)+1
  mutation=data['mutation'];cfg=data['cfg'];player=cfg.get('player',0);resource=cfg.get('resource',1)
  if mutation=='phase' and t in [0x4434e0,0x45d510]:word(uc,0x7999ea+2*264,2 if t==0x4434e0 else 3)
  if mutation=='withdraw' and t==0x45b570:
   for node in range(2,10):word(uc,0x799a14+node*264,0)
   put(uc,0xeaac28,3);put(uc,0xea5e3c+3*0x49d8,1)
  if mutation=='percentage' and t==0x4cde40:
   put(uc,0x5d2c4c+resource*8,41 if data['counts'][t]==1 else 99);put(uc,0xeaac28,3)
  if mutation=='cursor' and t==0x4435f0 and data['counts'][t]==1:put(uc,0xea5b34+player*0x49d8,2);put(uc,0xeaac28,3)
  if mutation=='import_failure' and t==0x45b2a0 and data['counts'][t]==1:
   word(uc,0x799a12+264,0)
  if mutation=='gather' and t==0x45d510:
   byte(uc,0x799a99+264,3);put(uc,0xd3b4e4,123);put(uc,0xd3b4e0,456)
   if data['counts'][t]==2:
    word(uc,0x799a14+2*264,20);word(uc,0x790134+196+args[1]*2,200)
  if mutation=='bound' and t==0x45dc50:put(uc,0x78d788,1)
  if mutation=='quota' and t==0x45dc50:put(uc,0xea42f8+get(uc,0xeaac28)*0x49d8+resource*4,1)
  if t not in owned:data['own']=False
  data['pending'].add(ret)
  if ret not in data['returns']:uc.hook_add(UC_HOOK_CODE,returned,data,begin=ret,end=ret);data['returns'].add(ret)
 # Observe each real C algorithm once at its named provider. Canonical and
 # compatibility wrappers can use CALL or a tail jump and are exercised below.
 state['entries']={(sym(names[t] if t in names else 'FUN_%08x'%t) if c else t):t for t in calls}
 for a in state['entries']:u.hook_add(UC_HOOK_CODE,child,state,begin=a,end=a)

frame_names={t:['restore_'+('civic' if t in [0x45d010,0x45d370,0x45d4d0] else 'transport' if t==0x45b880 else 'routing')+'_C_%08x'%t] for t in names}
def prepare(u,fn,cfg):
 def v(a,x,n=4):u.mem_write(a&0xffffffff,(x&((1<<(n*8))-1)).to_bytes(n,'little'))
 u.mem_write(0x560000,seed);u.mem_write(0x30000000,b'\xcc'*0x10000);u.mem_write(S,bytes(0x50000))
 u.mem_write(0x7999d8,bytes(4000*264));u.mem_write(0x77de00,bytes(106*3));u.mem_write(0x7900bc,bytes(196*8))
 player=cfg.get('player',0);owner=cfg.get('owner',player);resource=cfg.get('resource',1);v(0xeaac28,owner)
 for p in {0,3,player,owner}:
  for at,value in [(0xea187c,cfg.get('cash',10000)),(0xea44d4,cfg.get('export_ledger',100)),(0xea44dc,cfg.get('import_ledger',200)),
                   (0xea5b34,cfg.get('primary',1)),(0xea5b38,cfg.get('secondary',1)),(0xea5e3c,cfg.get('bonus',0))]:v(at+p*0x49d8,value)
  for at in [0xea61f4,0xea61f6,0xea61f8]:v(at+p*0x49d8,0,2)
  for res in range(-1,37):
   v(0xea4096+p*0x49d8+res*2,cfg.get('status',2 if fn==0x45d010 else 1),2);v(0xea40de+p*0x49d8+res*2,100,2)
   v(0xea6160+p*0x49d8+res*4,cfg.get('city_stock',20000 if fn==0x45d010 else 0));v(0xea60d0+p*0x49d8+res*4,5000)
   v(0xea42f8+p*0x49d8+res*4,0);v(0x77646c+res*4,25);v(0x77592c+res*4,0)
   v(0x5d2c48+res*8,cfg.get('buy_price',30));v(0x5d2c4c+res*8,cfg.get('sell_price',20))
   for slot in range(8):
    v(0x7900c2+slot*196+res,cfg.get('policy',0),1)
    v(0x7900ec+slot*196+res*2,cfg.get('limit',3200),2);v(0x790134+slot*196+res*2,cfg.get('alt_limit',1000),2)
   for trader in [0,1,2]:v(0x77de06+trader*106+res,1,1);v(0x77de2a+trader*106+res,1,1)
 for trader in [0,1,2]:v(0x77de04+trader*106,0,1)
 for res in cfg.get('disabled',[]):v(0xea4096+player*0x49d8+res*2,0,2)
 def store(bid,slot,kind=72,quantity=100,phase=1,nodes=None):
  r=bid*264
  for a,x,n in [(0x7999d8,1,1),(0x7999e8,kind,2),(0x799a99,slot,1),(0x799a18,100,2),(0x799a22,0,2),
   (0x7999ec,1,1),(0x7999de,10+bid,2),(0x7999e0,10,2),(0x7999f8,1,2),(0x7999fe,-bid,2),(0x799a00,bid+1,2)]:v(a+r,x,n)
  if nodes is None:nodes=list(range(2,10)) if bid==1 else list(range(100+bid*10,108+bid*10))
  previous=bid
  for i,node in enumerate(nodes):
   v(0x799a12+previous*264,node,2);v(0x799a10+node*264,bid,2);previous=node
   for a,x,n in [(0x7999ea,cfg.get('phases',[phase]*8)[i],2),(0x799a14,cfg.get('stocks',[quantity]*8)[i],2),(0x7999e4,100+bid*10+i,4)]:v(a+node*264,x,n)
  v(0x799a12+previous*264,cfg.get('terminal',0),2)
  for res in range(36):v(0x799a2e+r+res*2,cfg.get('food_stock',400),2)
 store(1,1,cfg.get('kind',72),cfg.get('stock',100),cfg.get('phase',resource),list(range(2,2+cfg.get('length',8))))
 if 'break_at' in cfg:v(0x799a12+cfg['break_at']*264,cfg.get('break_value',0),2)
 if 'zero_resume' in cfg:v(0x799a12,cfg['zero_resume'],2)
 if 'negative_resume' in cfg:v(0x799a12-264,cfg['negative_resume'],2)
 for res,limit in cfg.get('limits',{}).items():v(0x7900ec+196+res*2,limit,2)
 v(0xf566b4,900);v(0xf566bc,1000);v(0xf57204,2000)
 if fn in [0x45d010,0x45d370,0x45d4d0]:return [player,1,resource if fn==0x45d4d0 else cfg.get('trader',1)]
 if fn==0x45dc50:return cfg.get('distance_args',[10,20,13,15,1,3])
 assert fn in [0x45bb50,0x45b880]
 for i,bid in enumerate(cfg.get('sources',[20,30]),2):
  store(bid,i,cfg.get('source_kind',72),cfg.get('source_stock',100),cfg.get('source_phase',resource),list(range(100+bid*10,100+bid*10+cfg.get('source_length',8))))
  v(0x7900bc+i*196,cfg.get('source_id',bid),2);v(0x7900c0+i*196,cfg.get('source_enabled',1),1)
  v(0x7999d8+bid*264,cfg.get('source_state',1),1);v(0x7999ec+bid*264,cfg.get('source_network',1),1)
  v(0x7999de+bid*264,cfg.get('source_x',10+i),2);v(0x7999e0+bid*264,cfg.get('source_y',12),2)
  v(0x7999f8+bid*264,cfg.get('source_region',1),2)
  for res in range(36):v(0x7900c2+i*196+res,cfg.get('source_policy',0),1)
 if 'source_resume' in cfg:
  v(0x799a12+(0 if cfg.get('terminal',0)==0 else -264),1000,2);v(0x7999ea+1000*264,resource,2);v(0x799a14+1000*264,100,2)
 v(0x78d788,cfg.get('bound',3));v(0xd3b4e4,0x12345678);v(0xd3b4e0,0x87654321)
 v(0xea42f8+owner*0x49d8+resource*4,cfg.get('quota_gate',0))
 if cfg.get('self'):v(0x7900bc+2*196,1,2)
 if cfg.get('equal_distance'):
  v(0x7999de+20*264,12,2);v(0x7999de+30*264,12,2)
 if fn==0x45bb50:return [1,resource,cfg.get('require_stock',0)]
 listed=cfg.get('list',[1,2,3])
 for a in [0xf53dd8,0xf53e6c]:
  v(a,cfg.get('count',len(listed)))
  for i,res in enumerate(listed):v(a+4+i*4,res)
 for res in listed:
  v(0x7900c2+196+res,cfg.get('gather_policy',2),1)
  v(0x799a2e+264+res*2,cfg.get('resource_stocks',{}).get(res,cfg.get('food_stock',400)),2)
 for slot in [2,3]:
  for res in range(36):v(0x7900c2+slot*196+res,0,1)
 initial=cfg.get('outputs',[11,22,100,100,33,44]);ptrs=[S+0x100+i*4 for i in range(6)]
 if 'aliases' in cfg:ptrs=[S+0x100+i*4 for i in cfg['aliases']]
 for p,x in zip(ptrs,initial):v(p,x)
 return [1,*ptrs]

def exercise(fn=0x45d010,**cfg):
 global checked,invocations
 expected=None;void=fn in [0x45b880,0x45d4d0]
 for c,u,state in machines:
  for route in (['native'] if not c else ['named','canonical',*frame_names[fn]]):
   state.update(collect=False,own=True,writes=[],events=[],pending=set(),last_entry=None,top=fn,mutation=cfg.get('mutation'),counts={},cfg=cfg)
   args=prepare(u,fn,cfg);last[0]=None;state['collect']=True;invocations+=1
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
   state['collect']=False;answer=(tuple(state['writes']),tuple(state['events']),snapshot(u),bytes(u.mem_read(S+0x100,32)),0 if void else result)
   if expected is None:expected=answer
   elif answer!=expected:
    diffs=[next(((i,a,b) for i,(a,b) in enumerate(zip(x,y)) if a!=b),None) for x,y in zip(expected[:2],answer[:2])]
    memory=next(((hex(0x560000+i),a,b) for i,(a,b) in enumerate(zip(expected[2],answer[2])) if a!=b),None)
    raise AssertionError((hex(fn),cfg,route,'stores/events',diffs,'lengths',[(len(x),len(y)) for x,y in zip(expected[:2],answer[:2])],'memory',memory,'outputs',(expected[3].hex(),answer[3].hex()),'result',(expected[4],answer[4])))
 checked+=1
 if checked%25==0:print('progress',checked,'scenarios',flush=True)
 return expected[4]

if __name__=='__main__':
 quick='--quick' in sys.argv
 for fn in names:exercise(fn,stock=0 if fn==0x45d370 else 100)
 assert exercise(0x45d370,stock=0)==1
 assert exercise(0x45d370,stock=0,phase=0,limits={1:0})==2
 exercise(0x45d010,bonus=1,mutation='percentage');exercise(0x45d370,primary=35,stock=0)
 exercise(0x45b880,kind=71,resource_stocks={1:600,2:100,3:200},source_kind=71)
 exercise(0x45b880,kind=71,alt_limit=400,source_kind=71)
 exercise(0x45b880,list=[1,0,2],stock=1,source_phase=0)
 for _,u,_ in machines:assert (get(u,S+0x100),get(u,S+0x108),get(u,S+0x110))==(0,0,20)
 if not quick:
  for kind in [0,71,72,191,193,194]:
   for fn in [0x45d010,0x45d370]:exercise(fn,kind=kind,stock=0 if fn==0x45d370 else 100)
  for stock in [-1,0,1,12,13,99,100,399,400]:exercise(stock=stock)
  for phases in [[1,2,1,2,1,2,1,2],[1,2,3,4,5,6,7,8],[0]*8,[27]*8]:exercise(phases=phases)
  for length,terminal in itertools.product([0,1,7,8],[0,-1]):exercise(length=length,terminal=terminal)
  for bonus,price in itertools.product([-1,0,1],[-1,0,1,25,2147483647]):exercise(bonus=bonus,sell_price=price)
  for mutation in ['phase','withdraw','percentage','price']:exercise(mutation=mutation,bonus=1)
  exercise(status=0);exercise(disabled=list(range(36)))
  exercise(break_at=3,zero_resume=6);exercise(break_at=3,break_value=-1,negative_resume=6)
  for player,owner in [(0,3),(3,0),(-1,-1)]:
   for fn in [0x45d010,0x45d370,0x45d4d0]:exercise(fn,player=player,owner=owner,stock=0 if fn==0x45d370 else 100)
  for resource,price,cash in itertools.product([-1,0,1,35],[0,30,0xffffffff],[0,-2147483648,2147483647]):exercise(0x45d4d0,resource=resource,buy_price=price,cash=cash,mutation='price')
  for primary,secondary in itertools.product([-1,0,1,34,35,36],[0,35]):exercise(0x45d370,primary=primary,secondary=secondary,stock=0)
  for limit in [0,1,99,100,101]:exercise(0x45d370,stock=0,limit=limit,alt_limit=limit)
  exercise(0x45d370,stock=0,disabled=list(range(36)))
  exercise(0x45d370,stock=0,disabled=[1,2,3,4]);exercise(0x45d370,stock=0,mutation='cursor')
  exercise(0x45d370,stock=0,mutation='import_failure')
  exercise(0x45d370,stock=0,phase=0,limits={1:0})
  exercise(0x45d370,stock=0,phase=0,limits={1:0},secondary=35,disabled=[2])
  exercise(0x45d370,stock=399,phases=[1,2,3,4,5,6,7,8],limit=3200)
  for kind in [-1,0,70,71,72,73,183,191,193,213,222,225,227,228,229,230,234,235,236,237]:exercise(0x45bb50,kind=kind,source_network=2,policy=2)
  for resource,source_kind in itertools.product([-1,0,1,8,9,35],[0,71,72,191,193]):exercise(0x45bb50,resource=resource,source_kind=source_kind)
  for require,stock,policy in itertools.product([0,1,255,256],[0,1,49,50,100],[0,2]):exercise(0x45bb50,source_stock=stock,require_stock=require,source_policy=policy)
  for cfg in [dict(bound=0),dict(bound=-1),dict(source_enabled=0),dict(source_id=0),dict(source_state=0),dict(self=True,sources=[20]),dict(quota_gate=1),dict(source_network=2),dict(equal_distance=True),dict(source_x=20000),dict(source_region=12000),dict(source_kind=71,food_stock=399),dict(source_kind=71,source_policy=2),dict(mutation='bound')]:exercise(0x45bb50,**cfg)
  for length,terminal in itertools.product([0,1,7],[0,-1]):exercise(0x45bb50,source_length=length,terminal=terminal)
  for terminal in [0,-1]:exercise(0x45bb50,source_length=1,terminal=terminal,source_resume=True)
  for kind in [71,72,191,193]:
   for policy in [0,1,2,3]:exercise(0x45b880,kind=kind,gather_policy=policy,source_kind=71 if kind==71 else 72)
  for count in [-1,0]:exercise(0x45b880,count=count)
  for limit in [0,1,800,1000]:exercise(0x45b880,alt_limit=limit)
  exercise(0x45b880,list=[1,0,2],stock=1)
  exercise(0x45b880,kind=71,list=[1,0,2],food_stock=10,source_kind=71)
  exercise(0x45b880,mutation='gather');exercise(0x45b880,mutation='quota')
  for outputs in [[1,2,0,0,3,4],[1,2,100,200,3,4],[1,2,200,100,3,4]]:exercise(0x45b880,outputs=outputs)
  for aliases in [[0,0,0,0,0,0],[0,1,2,3,0,1],[2,3,0,1,4,5]]:exercise(0x45b880,aliases=aliases)
  for x1,y1,x2,y2 in itertools.product([-2147483648,-1,0,2147483647],repeat=4):exercise(0x45dc50,distance_args=[x1,y1,x2,y2,-1,2147483647])
  for r1,r2 in itertools.product([-2147483648,-1,0,2147483647],repeat=2):exercise(0x45dc50,distance_args=[10,20,13,15,r1,r2])
 excluded={(0x45d3eb,0x45d3e1)}
 assert not excluded&edges
 missing=points-hits;missing_edges={(a,b) for a,ends in branches.items() for b in ends if (a,b) not in edges and (a,b) not in excluded}
 expected_jumps=set(struct.unpack_from('<3I',bytes.fromhex(catalogue['tables'][0]['bytes_hex'])))
 print('Compared storage transactions:',checked,'scenarios;',invocations,'invocations;',len(points)-len(missing),'/',len(points),'instructions,',len(edges),'conditional edges; switch targets',sorted(map(hex,jump_edges)),flush=True)
 print('Unexecuted instructions:',[hex(a) for a in sorted(missing)],'; edges:',[(hex(a),hex(b)) for a,b in sorted(missing_edges)],flush=True)
 if quick:sys.exit(0)
 assert not missing,('instruction coverage',missing)
 assert not missing_edges,('edge coverage',missing_edges)
 assert expected_jumps==jump_edges
 sources=['CMakeLists.txt','include/storage_transactions.h','runtime/storage_transactions.c','runtime/actor_civic.c','runtime/actor_civic_model.c','runtime/actor_transport.c','runtime/actor_transport_model.c','runtime/actor_walking_facades.c','runtime/actor_routing_model.c','direct_c/verify_storage_transactions.py','metadata/storage-transactions-native.json']
 proof=dict(native_sha256=catalogue['native_sha256'],compiled_C_path=str(target.resolve()),compiled_C_sha256=target_sha,passed=True,checked=checked,invocations=invocations,
  coverage={hex(a):dict(instructions=len(p),executed=len(p&hits)) for a,p in owned.items()},conditional_edges=len(edges),switch_targets=sorted(map(hex,jump_edges)),
  unexecuted_instructions=[hex(a) for a in sorted(missing)],unexecuted_edges=[[hex(a),hex(b)] for a,b in sorted(missing_edges)],
  analytically_unreachable_edges=[dict(source='0x45d3eb',target='0x45d3e1',reason='The outer import query loop admits counters 1..35 only; its successful-query exit cannot have counter >=36.')],
  source_hashes={p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest() for p in sources},commit=None,controlled_algorithm_endpoints=[],controlled_platform_endpoints=[],
  limitations=['Actual permission, stock, delivery, withdrawal, price, source and distance children execute. Named/canonical/frame adapters; full nonvoid DWORD results, ordered owned global/output stores, child inputs, all final original static data, output values and saved ABI compared.',
  'Real input mutations cover price snapshots, phase reloads, failed withdrawals, current city and independent cursors, cached slot/fresh amount/capacity, live source bounds and source policy. Pointer outputs may alias.',
  'Gathering resource-list members are valid indices 0..35, remain initialized members across its two passes, and ratio divisors are nonzero. Incidental caller-saved state, undefined private scratch, concurrent access and host game/UI fidelity are excluded.'])
 suffix='diagnostic' if target.stem=='restore_link_check' else 'game'
 (ROOT/f'metadata/storage-transactions-{suffix}.json').write_text(json.dumps(proof,indent=2)+'\n')

