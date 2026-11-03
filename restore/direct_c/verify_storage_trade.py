"""Offline original/C trade permissions, storage admission and food destinations.

Execute actual children; compare ordered owned stores, child inputs, all original
static data and the saved ABI. Actual root traversal and native callers execute;
no child results are replaced and no host game is launched.
"""
from verify_options_startup import *
from unicorn import UC_HOOK_MEM_WRITE
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import json
catalogue=json.loads((ROOT/'metadata/storage-trade-native.json').read_text())
assert catalogue['native_sha256']==hashlib.sha256(original.read_bytes()).hexdigest()
names={0x4434e0:'restore_trade_can_export',0x4435f0:'restore_trade_can_import',0x45cf50:'restore_storage_can_export',0x45d180:'restore_storage_can_import',0x45d9e0:'restore_food_delivery_destination'}
owned={};branches={};hits=set();edges=set();last=[None];checked=invocations=0
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
calls={0x4434e0:3,0x4435f0:3,0x45d510:2,0x45b230:3,0x4c5240:1,0x4cde60:2,0x4806a0:2,0x4b28f0:3}
for c,pe in enumerate(images):
 u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(pe.base,(len(pe.data)+4095)&~4095);u.mem_write(pe.base,bytes(pe.data))
 u.mem_map(0x30000000,0x10000);u.mem_map(STOP,4096);u.mem_map(S,0x50000)
 state=dict(collect=False,own=True,writes=[],events=[],pending=set(),returns=set(),compiled=c,last_entry=None);machines.append((c,u,state))
 def write(uc,access,at,size,value,data):
  if data['collect'] and data['own']:
   data['writes'].append((at,size,value&((1<<(size*8))-1)))
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
  mutation=data['mutation'];cfg=data['cfg'];player=cfg.get('player',0);trader=cfg.get('trader',1);root=1
  if mutation=='quota' and t==0x4c5240:
   byte(uc,0x77de04+trader*106,1);word(uc,0xea4096+player*0x49d8+cfg.get('resource',1)*2,3 if data['top']==0x4435f0 else 4)
  if mutation=='phase' and t==0x4434e0:word(uc,0x7999ea+2*264,2)
  if mutation=='percentage' and t==0x4cde60:
   byte(uc,0x799a99+264,2);word(uc,0xbcd66c+255*388,cfg.get('newcargo',7))
  if mutation=='exhaust' and t==0x4435f0 and data['counts'][t]==36:
   for resource in range(36):word(uc,0xea4096+player*0x49d8+resource*2,0)
  if mutation=='bound' and t==0x4cde60:put(uc,0x78d790,1)
  if mutation=='candidates' and t==0x4806a0:
   put(uc,0x96e6b0,1);put(uc,0x972534,0);put(uc,0xbca2d8,1)
  if mutation=='selection' and t==0x4b28f0:
   put(uc,0x972534,0);put(uc,0x972538,0);word(uc,0x7999fe,-123);word(uc,0x799a00,321)
  if t not in owned:data['own']=False
  data['pending'].add(ret)
  if ret not in data['returns']:uc.hook_add(UC_HOOK_CODE,returned,data,begin=ret,end=ret);data['returns'].add(ret)
 # Observe each real C algorithm once at its named provider. Canonical and
 # compatibility wrappers can use CALL or a tail jump and are exercised below.
 state['entries']={(sym(names[t] if t in names else 'FUN_%08x'%t) if c else t):t for t in calls}
 for a in state['entries']:u.hook_add(UC_HOOK_CODE,child,state,begin=a,end=a)

frame_names={t:['restore_'+('transport' if t==0x45d9e0 else 'actor_message')+'_C_%08x'%t] for t in names}
def prepare(u,fn,cfg):
 def v(a,x,n=4):u.mem_write(a&0xffffffff,(x&((1<<(n*8))-1)).to_bytes(n,'little'))
 u.mem_write(0x560000,seed);u.mem_write(0x30000000,b'\xcc'*0x10000);u.mem_write(S,bytes(0x50000))
 u.mem_write(0x7999d8,bytes(4000*264));u.mem_write(0x77de00,bytes(106*3));u.mem_write(0x7900c0,bytes(196*4))
 player=cfg.get('player',0);owner=cfg.get('owner',player);trader=cfg.get('trader',1);resource=cfg.get('resource',1)
 v(0xeaac28,owner)
 for p in {0,3,player,owner}:
  v(0xea187c+p*0x49d8,cfg.get('cash',1000));v(0xea5b34+p*0x49d8,cfg.get('global_cursor',1) if p==owner and p!=player else cfg.get('cursor',1))
  for o in [0xea61f4,0xea61f6,0xea61f8]:v(o+p*0x49d8,0,2)
  for res in set(range(-1,37))|{resource}:
   status=cfg.get('status',1 if fn in [0x4435f0,0x45d180] else 2)
   v(0xea4096+p*0x49d8+res*2,status,2);v(0xea40de+p*0x49d8+res*2,cfg.get('threshold',1),2)
   v(0xea6160+p*0x49d8+res*4,cfg.get('city_stock',0 if fn in [0x4435f0,0x45d180] else 1000))
   v(0x775810+res*4,cfg.get('automatic',100))
   for template in [0,1]:
    v(0x77646c+(res+template*36)*4,cfg.get('quota',25));v(0x77592c+(res+template*36)*4,cfg.get('traded',0) if template==0 else cfg.get('fresh_traded',0))
   for slot in range(4):
    v(0x7900c2+slot*196+res,cfg.get('policy',0),1)
    v(0x7900ec+slot*196+res*2,cfg.get('limit',1000),2);v(0x790134+slot*196+res*2,cfg.get('alt_limit',1000),2)
   for record in [0,1,2]:
    v(0x77de06+record*106+res,cfg.get('exports',1),1);v(0x77de2a+record*106+res,cfg.get('imports',1),1)
 for record in [0,1,2]:v(0x77de04+record*106,0,1)
 for o in [0xea61f4,0xea61f6,0xea61f8]:v(o+owner*0x49d8,cfg.get('adjustment',{}).get(o,0),2)
 if fn in [0x4434e0,0x4435f0]:return [player,trader,resource]
 v(0x7999e8+264,cfg.get('kind',72),2);v(0x799a18+264,100,2);v(0x799a99+264,cfg.get('slot',1),1)
 v(0x705eb8+72*40,100);v(0x705eb8+193*40,100)
 v(0xbcd667+255*388,cfg.get('cargo_byte',0),1);v(0xbcd66c+255*388,cfg.get('cargo',0),2)
 stocks=cfg.get('stocks',[cfg.get('stock',100)]*8);phases=cfg.get('phases',[cfg.get('phase',1)]*8)
 previous=1
 for i in range(cfg.get('length',8)):
  node=2+i;v(0x799a12+previous*264,node,2);previous=node
  v(0x799a14+node*264,stocks[i],2);v(0x7999ea+node*264,phases[i],2)
 v(0x799a12+previous*264,cfg.get('terminal',0),2)
 if 'break_at' in cfg:v(0x799a12+cfg['break_at']*264,cfg.get('break_value',0),2)
 if 'zero_resume' in cfg:v(0x799a12,cfg['zero_resume'],2)
 if 'negative_resume' in cfg:v(0x799a12-264,cfg['negative_resume'],2)
 if 'cursor_policy' in cfg:v(0x7900c2+196*cfg.get('slot',1)+cfg.get('global_cursor',cfg.get('cursor',1)),cfg['cursor_policy'],1)
 if 'resource_policy' in cfg:
  for res,policy in cfg['resource_policy'].items():v(0x7900c2+196*cfg.get('slot',1)+res,policy,1)
 if 'disabled' in cfg:
  for res in cfg['disabled']:v(0xea4096+player*0x49d8+res*2,0,2)
 if fn in [0x45cf50,0x45d180]:return [255,player,1,trader]
 assert fn==0x45d9e0
 u.mem_write(0x7999d8,bytes(4000*264));p=255*388
 for y in range(6,20):
  for x in range(6,20):
   tile=y*228+x;v(0xec7630+tile,0,1);v(0xaf598c+tile*4,0);v(0xb4e6fc+tile*2,0,2)
 for a,x in [(0xeadfd8,0xffffffff),(0xbca2d8,0),(0xbca2d0,228),(0xbca2d4,228),(0xd056f8,255),(0xd3b4e4,13),(0xd3b4e0,11),
             (0x78d790,cfg.get('bound',2)),(0x784bbc,cfg.get('blocked',0)),(0x96e6b0,99),(0xd05628,0xffffffff)]:v(a,x)
 u.mem_write(0xd04e58,bytes(2000))
 for off,value in [(10,4),(14,1),(56,9),(57,14),(114,cfg.get('movement',2)),(118,0)]:v(0xbcd5f8+p+off,value,1)
 for off,value in [(20,10),(22,10),(36,13),(38,11),(58,0),(60,0),(62,0),(116,cfg.get('cargo',3))]:v(0xbcd5f8+p+off,value,2)
 v(0xbcd5f8+p+32,2290);v(0xb350dc+2290*2,255,2)
 for i in range(10):v(0xea5f1c+owner*0x49d8+i*8,i+1)
 v(0xea42f8+owner*0x49d8+resource*4,cfg.get('quota_gate',0))
 for bid in [1,2]:
  r=bid*264
  for a,x,n in [(0x7999d8,cfg.get('state',1),1),(0x7999e8,cfg.get('kind',71),2),(0x799a1c,cfg.get('enabled',1),1),
   (0x7999f8,cfg.get('access',1),2),(0x7999ec,cfg.get('road_network',1),1),(0x799a18,cfg.get('workers',100),2),
   (0x799a99,bid,1),(0x799a2e,cfg.get('free',100),2),(0x799a2e+resource*2,cfg.get('food_stock',0),2),
   (0x799adc,cfg.get('reserved_count',0),2),(0x799ade,cfg.get('reserved',0),2),(0x7999de,12,2),(0x7999e0,11,2),
   (0x7999fe,-20-bid,2),(0x799a00,30+bid,2)]:v(a+r,x,n)
  v(0x705eb8+cfg.get('kind',71)*40,cfg.get('max_workers',100))
 if cfg.get('roads'):
  offsets=struct.unpack_from('<24i',images[0].data,0x5cc9e0+4*96-images[0].base)
  cell=11*228+12+next(x for x in offsets if x);v(0xaf598c+cell*4,0x40);v(0xa76aec+cell,1,1)
 return [cfg.get('force',0),255,resource,cfg.get('network',1)]

def exercise(fn=0x4434e0,**cfg):
 global checked,invocations
 expected=None
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
   state['collect']=False;answer=(tuple(state['writes']),tuple(state['events']),snapshot(u),result)
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
 assert exercise(0x45d9e0,roads=True)>0
 exercise(0x45d180,stock=0,cursor=35,disabled=[35]);exercise(0x45d180,mutation='exhaust',cursor=0)
 if not quick:
  for fn in [0x4434e0,0x4435f0]:
   for status,resource in itertools.product([0,1,2,3,4,5],[-1,0,1,26,27,35,36]):exercise(fn,status=status,resource=resource)
   for trader,traded,quota in itertools.product([0,1],[0,24,25,26],[-1,0,25]):exercise(fn,trader=trader,traded=traded,quota=quota)
   for stock,threshold in itertools.product([-2147483648,-1,0,99,100,101,2147483647],[-32768,-1,0,1,32767]):exercise(fn,city_stock=stock,threshold=threshold)
   for stock in [-1,99,100,101]:exercise(fn,status=4 if fn==0x4434e0 else 3,city_stock=stock)
   for player,owner in [(0,0),(3,3),(0,3),(3,0),(-1,-1)]:exercise(fn,player=player,owner=owner)
   for fresh in [0,25]:exercise(fn,mutation='quota',fresh_traded=fresh)
   for allowed in [0,1]:exercise(fn,exports=allowed,imports=allowed)
   for adjustment in [{0xea61f4:1},{0xea61f6:1},{0xea61f6:1,0xea61f8:1}]:exercise(fn,quota=15,adjustment=adjustment)
  for cash in [-5001,-5000,-4999,0]:exercise(0x4435f0,cash=cash)
  for fn in [0x45cf50,0x45d180]:
   for kind in [0,71,72,193,194]:exercise(fn,kind=kind)
   for cargo in [-32768,-1,0,7,8,255,256,32767]:exercise(fn,cargo=cargo,cargo_byte=cargo)
   for stock in [-32768,-1,0,1,99,100,399,400,401,32767]:exercise(fn,stock=stock)
   for length,terminal in itertools.product([0,1,7,8],[0,-1]):exercise(fn,length=length,terminal=terminal)
   exercise(fn,break_at=3,zero_resume=6,stocks=[0,0,0,0,100,0,0,0])
   exercise(fn,break_at=3,break_value=-1,negative_resume=6,stocks=[0,0,0,0,100,0,0,0])
  exercise(0x45cf50,mutation='phase',phases=[1,2,2,2,2,2,2,2])
  exercise(0x45cf50,phase=27)
  exercise(0x45d180,phase=27)
  for policy in [0,1,2,3,4,255]:exercise(0x45d180,policy=policy)
  for limit in [-1,0,1,99,100,101,800,900]:exercise(0x45d180,limit=limit,alt_limit=limit,stock=100)
  for cursor in [-1,0,1,34,35,36]:exercise(0x45d180,cursor=cursor)
  for owner,cursor_policy in itertools.product([0,3],[1,3]):exercise(0x45d180,player=0,owner=owner,global_cursor=2,cursor_policy=cursor_policy)
  for cargo in [7,8]:exercise(0x45d180,mutation='percentage',newcargo=cargo,slot=1,resource_policy={1:1})
  exercise(0x45d180,stock=0,resource_policy={1:1,2:3},cursor=1)
  for cfg in [dict(blocked=1),dict(resource=0),dict(resource=9),dict(quota_gate=1),dict(quota_gate=1,force=1),dict(bound=0),dict(bound=-1),dict(state=0),dict(state=2),dict(kind=72),dict(kind=191),dict(enabled=0),dict(access=0),dict(access=-1),dict(network=257),dict(road_network=2),dict(workers=99),dict(workers=-1),dict(max_workers=0)]:exercise(0x45d9e0,**cfg)
  for policy,stock,reserved in itertools.product([0,1,2,3,255],[0,999,1000],[-1,0,1,1000]):exercise(0x45d9e0,policy=policy,food_stock=stock,reserved=reserved,reserved_count=1)
  for free in [-1,0,99,100,101]:exercise(0x45d9e0,free=free)
  exercise(0x45d9e0,resource=8,cargo=65535,reserved_count=32767,reserved=0)
  for mutation in ['bound','candidates','selection']:exercise(0x45d9e0,mutation=mutation)
  for movement in [0,1,2,3,4,5,6,9,10]:exercise(0x45d9e0,movement=movement,roads=True)
 missing=points-hits;excluded={(0x45db24,0x45dbb4)}
 # EDX caches policy. The preceding policy gate admits only 0 or 2; the
 # inner JZ consumes 0. SUB EDX,2 therefore leaves zero on this branch.
 assert not excluded&edges
 missing_edges={(a,b) for a,ends in branches.items() for b in ends if (a,b) not in edges and (a,b) not in excluded}
 print('Compared trade/destinations:',checked,'scenarios;',invocations,'invocations;',len(points)-len(missing),'/',len(points),'instructions,',len(edges),'conditional edges',flush=True)
 print('Unexecuted instructions:',[hex(a) for a in sorted(missing)],'; edges:',[(hex(a),hex(b)) for a,b in sorted(missing_edges)],flush=True)
 if quick:sys.exit(0)
 assert not missing,('instruction coverage',missing)
 assert not missing_edges,('edge coverage',missing_edges)
 sources=['CMakeLists.txt','include/storage_trade.h','runtime/storage_trade.c','runtime/menu_actor_message.c','runtime/actor_transport.c','runtime/actor_transport_model.c',
  'src/unit_00619.c','src/unit_00620.c','src/unit_00894.c','src/unit_00896.c','direct_c/verify_storage_trade.py','metadata/storage-trade-native.json']
 proof=dict(native_sha256=catalogue['native_sha256'],compiled_C_path=str(target.resolve()),compiled_C_sha256=target_sha,passed=True,checked=checked,invocations=invocations,
  coverage={hex(a):dict(instructions=len(p),executed=len(p&hits)) for a,p in owned.items()},conditional_edges=len(edges),
  unexecuted_instructions=[hex(a) for a in sorted(missing)],unexecuted_edges=[[hex(a),hex(b)] for a,b in sorted(missing_edges)],
  analytically_unreachable_edges=[dict(source='0x45db24',target='0x45dbb4',reason='EDX is cached policy 0 or 2 after 45DAD7/45DADC; 45DB1F takes policy 0, so SUB EDX,2 is zero. No intervening child calls.')],
  source_hashes={p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest() for p in sources},commit=None,controlled_algorithm_endpoints=[],controlled_platform_endpoints=[],
  limitations=['Actual quota/percentage/capacity/resource/road and route helpers execute. Named, canonical and compatibility adapters; full DWORD results, ordered owned stores, helper inputs, all final original static data and saved ABI compared.',
  'Genuine template/status/phase/cargo/slot/eligibility/bound/candidate/selection input mutations do not replace child results. Real walkable-map/road/route fixtures exercise destination selection and reservations.',
  'Incidental caller-saved registers/flags, private scratch, concurrent access and host game/UI fidelity are excluded.'])
 suffix='diagnostic' if target.stem=='restore_link_check' else 'game'
 (ROOT/f'metadata/storage-trade-{suffix}.json').write_text(json.dumps(proof,indent=2)+'\n')




