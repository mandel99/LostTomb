"""Offline original/C industrial destinations, reservations and delivery.

Execute actual children; compare ordered owned stores, child inputs, all original
static data and the saved ABI. Actual production-permission, route and road children execute;
no child results are replaced and no host game is launched.
"""
from verify_options_startup import *
from unicorn import UC_HOOK_MEM_WRITE
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import json
catalogue=json.loads((ROOT/'metadata/industry-logistics-native.json').read_text())
assert catalogue['native_sha256']==hashlib.sha256(original.read_bytes()).hexdigest()
names={0x45eab0:'restore_industry_delivery_destination',0x45f450:'restore_industry_producer_destination',0x45f7b0:'restore_industry_city_destination',0x460890:'restore_industry_deliver'}
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
for f in catalogue['functions']:
 if f['address']!='0x45f450':continue
 ins=list(decoder.disasm(bytes.fromhex(f['bytes_hex']),int(f['address'],16)));by_pc={i.address:i for i in ins};seen=set();todo=[int(f['address'],16)]
 while todo:
  pc=todo.pop()
  if pc in seen:continue
  assert pc in by_pc,('unbounded producer flow',hex(pc))
  seen.add(pc);i=by_pc[pc]
  if i.mnemonic.startswith('ret'):continue
  if i.mnemonic.startswith('j'):
   todo.extend(switches[pc] if pc in switches else [int(i.op_str,16)])
   if i.mnemonic=='jmp':continue
  todo.append(pc+i.size)
 assert seen==owned[int(f['address'],16)]
points=set.union(*owned.values());machines=[]
calls={0x459240:1,0x4b28f0:3,0x480180:3}
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
  mutation=data['mutation'];cfg=data['cfg'];resource=cfg.get('resource',10)
  if mutation=='permission' and t==0x459240:
   word(uc,0x7999e8+args[0]*264,187);word(uc,0x799a18+args[0]*264,1);word(uc,0x7999ea+args[0]*264,8)
  if mutation=='bound' and t==0x459240:put(uc,0x78d790,1)
  if mutation=='selection' and t==0x4b28f0:
   put(uc,0x972534,0);put(uc,0x972538,0);word(uc,0x7999fe,-5);word(uc,0x799a00,7)
  if mutation=='dual' and t==0x4b28f0:
   for node in range(1,4):word(uc,0x7999e8+node*264,204);word(uc,0x799aa6+node*264,MATERIAL.get(resource,-1))
  if mutation=='road' and t==0x480180:
   target=cfg.get('target',1);put(uc,0xea5d1c+cfg.get('owner',0)*0x49d8,20)
   word(uc,0x7999ea+target*264,-1);word(uc,0x799aa6+target*264,MATERIAL.get(resource,-1))
  if t not in owned:data['own']=False
  data['pending'].add(ret)
  if ret not in data['returns']:uc.hook_add(UC_HOOK_CODE,returned,data,begin=ret,end=ret);data['returns'].add(ret)
 # Observe each real C algorithm once at its named provider. Canonical and
 # compatibility wrappers can use CALL or a tail jump and are exercised below.
 state['entries']={(sym(names[t] if t in names else 'FUN_%08x'%t) if c else t):t for t in calls}
 for a in state['entries']:u.hook_add(UC_HOOK_CODE,child,state,begin=a,end=a)

MATERIAL={2:21,8:22,9:13,10:8,11:5,12:7,13:24,14:9,15:19,16:1,17:16,18:10,20:4,21:15,22:11,23:12,24:3,25:6,28:14,29:17,30:18,31:23,32:20,33:25,34:26}
frame_names={t:['restore_transport_C_%08x'%t] for t in names}
def prepare(u,fn,cfg):
 def v(a,x,n=4):u.mem_write(a&0xffffffff,(x&((1<<(n*8))-1)).to_bytes(n,'little'))
 u.mem_write(0x560000,seed);u.mem_write(0x30000000,b'\xcc'*0x10000);u.mem_write(S,bytes(0x50000));u.mem_write(0x7999d8,bytes(4000*264))
 resource=cfg.get('resource',10);type_=MATERIAL.get(resource,-1);owner=cfg.get('owner',0);v(0xeaac28,owner)
 for p in {0,3,owner}:
  for res in range(-1,37):v(0xea42f8+p*0x49d8+res*4,0);v(0xea4126+p*0x49d8+res*2,0,2)
  for i in range(10):v(0xea5f1c+p*0x49d8+i*8,i+1)
  v(0xea5d1c+p*0x49d8,cfg.get('target',1))
 v(0xea42f8+owner*0x49d8+resource*4,cfg.get('quota',0));v(0x7058b8,cfg.get('active',1));v(0xbcb9b4,cfg.get('walkers',1))
 nodes=cfg.get('nodes',[1,2]);stocks=cfg.get('stocks',[cfg.get('stock',0)]*len(nodes));pending=cfg.get('reservations',[cfg.get('pending',0)]*len(nodes))
 for i,bid in enumerate(nodes):
  record=bid*264
  for a,x,n in [(0x7999d8,cfg.get('state',1),1),(0x7999e8,cfg.get('kind',32),2),(0x799a1c,cfg.get('enabled',1),1),
   (0x7999f8,cfg.get('access',1),2),(0x7999ec,cfg.get('network_byte',1),1),(0x7999ea,cfg.get('primary_type',type_),2),
   (0x799aa6,cfg.get('secondary_type',0),2),(0x799a14,stocks[i],2),(0x799aa4,cfg.get('secondary_stock',0),2),
   (0x799adc,pending[i],2),(0x799ade,cfg.get('secondary_pending',0),2),(0x799a18,cfg.get('workers',1),2),
   (0x7999fe,cfg.get('x',13),2),(0x799a00,cfg.get('y',12),2),(0x7999de,12,2),(0x7999e0,11,2),(0x7999db,4,1),(0x799a1b,1,1)]:v(a+record,x,n)
 v(0xea4126+owner*0x49d8+2,cfg.get('blocked',0),2)
 if fn==0x460890:return [cfg.get('building',1),cfg.get('amount',100),resource]
 if fn==0x45f7b0:
  # Actual orientation-four road lookup; input coordinates remain native inputs.
  v(0xbca2d8,0)
  if cfg.get('roads',True):
   offsets=struct.unpack_from('<24i',images[0].data,0x5cc9e0+4*96-images[0].base)
   cell=11*228+12+next(x for x in offsets if x);v(0xaf598c+cell*4,0x40);v(0xa76aec+cell,1,1)
  return [0x11223344,0x55667788,resource,cfg.get('network',1)]
 v(0x78d790,cfg.get('bound',max(nodes) if nodes else 0));v(0x96e6b0,99);v(0xbca2d8,0);v(0xd3b4e4,0x12345678);v(0xd3b4e0,0x87654321)
 # A real bounded walkable district keeps empty-goal searches finite and small.
 # All three native movement classifications execute; no path result is replaced.
 for base in [0xee0c50,0xebab20,0xec7630]:u.mem_write(base,b'\x80'*51984)
 for y in range(6,20):
  for x in range(6,20):
   tile=y*228+x
   for base in [0xee0c50,0xebab20,0xec7630]:v(base+tile,0,1)
   v(0xaf598c+tile*4,0);v(0xb4e6fc+tile*2,0,2)
 p=255*388
 for a,x in [(0xeadfd8,0xffffffff),(0xbca2d0,228),(0xbca2d4,228),(0xd056f8,255)]:v(a,x)
 u.mem_write(0xd04e58,bytes(2000))
 for off,x in [(10,4),(14,1),(56,9),(57,14),(114,cfg.get('movement',2)),(118,0)]:v(0xbcd5f8+p+off,x,1)
 for off,x in [(20,10),(22,10),(36,13),(38,12),(58,0),(60,0),(62,0)]:v(0xbcd5f8+p+off,x,2)
 v(0xbcd5f8+p+32,2290);v(0xb350dc+2290*2,255,2)
 return [255,resource,cfg.get('network',1)]

def exercise(fn=0x45eab0,**cfg):
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
 for fn in names:assert exercise(fn)>0
 exercise(stocks=[100,0,0],reservations=[0,1,0],nodes=[1,2,3])
 exercise(kind=204,secondary_type=8,secondary_pending=1)
 assert exercise(kind=204,primary_type=0,secondary_type=8,secondary_pending=1)>0
 assert exercise(kind=204,pending=1)>0
 exercise(0x45f7b0,mutation='road')
 if not quick:
  for resource in range(-1,37):
   for fn in [0x45eab0,0x45f450,0x45f7b0]:
    value=exercise(fn,resource=resource)
    if resource in MATERIAL and fn in [0x45eab0,0x45f450]:assert value>0
  for kind in [-1,0,31,32,33,47,51,53,74,84,85,86,87,110,111,112,113,114,115,177,178,184,187,188,189,190,194,203,204,205,226,231,232,233,234]:
   for fn in [0x45eab0,0x45f450]:exercise(fn,kind=kind)
  for fn in [0x45eab0,0x45f450]:
   for cfg in [dict(quota=1),dict(state=0),dict(state=2),dict(enabled=0),dict(access=0),dict(access=-1),dict(primary_type=0),dict(network=257),dict(network_byte=2),dict(bound=0),dict(bound=-1),dict(movement=6),dict(mutation='selection')]:exercise(fn,**cfg)
  for kind,cap in [(32,300),(47,500),(74,200),(187,100000)]:
   for secondary in [False,True]:
    for stock,pending in [(0,0),(0,-1),(0,1),(0,32767),(min(cap-100,32767),0),(min(cap-99,32767),0),(10000,0),(10001,0)]:
     exercise(kind=kind,secondary_type=8 if secondary else 0,**({'secondary_stock':stock,'secondary_pending':pending} if secondary else {'stock':stock,'pending':pending}))
  for kind in [187,188,189]:
   for workers in [-1,0,1]:exercise(kind=kind,workers=workers)
  for resource,type_ in MATERIAL.items():exercise(resource=resource,kind=204,secondary_type=type_)
  for type_ in [-1,0,*range(1,28)]:exercise(kind=204,secondary_type=type_,secondary_pending=1)
  for secondary_stock,secondary_pending in itertools.product([0,100,201,32767],[-1,0,1,4]):exercise(kind=204,secondary_type=8,secondary_stock=secondary_stock,secondary_pending=secondary_pending)
  for cfg in [dict(stocks=[0,100,0],reservations=[2,0,1]),dict(stocks=[100,0,100],reservations=[0,1,0]),dict(stocks=[0,0,0],reservations=[2,1,2]),dict(stocks=[10000,10000,10000],reservations=[0,0,0])]:exercise(nodes=[1,2,3],**cfg)
  for mutation in ['permission','bound','dual']:exercise(mutation=mutation)
  exercise(blocked=1);exercise(0x45f450,blocked=1)
  for kind in [0,32,47,74,84,187]:
   for secondary in [False,True]:
    base=dict(resource=10,kind=kind,primary_type=0 if secondary else 8,secondary_type=8 if secondary else 0)
    for stock,pending in [(0,-1),(0,0),(0,1),(0,32767),(199,0),(200,0),(299,0),(300,0),(499,0),(500,0),(32767,0)]:
     exercise(0x45f7b0,**base,**({'secondary_stock':stock,'secondary_pending':pending} if secondary else {'stock':stock,'pending':pending}))
  for secondary in [False,True]:exercise(0x45f7b0,resource=28,primary_type=0 if secondary else 14,secondary_type=14 if secondary else 0)
  for cfg in [dict(quota=1),dict(active=0),dict(active=-1),dict(walkers=0),dict(walkers=-1),dict(roads=False),dict(network=257),dict(primary_type=0),dict(secondary_type=8),dict(mutation='road')]:exercise(0x45f7b0,**cfg)
  for resource in [10,28]:
   for kind,cap in [(0,200),(32,300),(47,500),(74,200),(84,100000),(187,100000)]:
    for stock,amount in [(-32768,2147483647),(-1,-2147483648),(0,-1),(0,0),(0,100),(0,100001),(199,201),(32767,2147483647),(cap-1,2),(cap,1)]:
     if stock<=32767:exercise(0x460890,resource=resource,kind=kind,amount=amount,stock=stock,secondary_stock=stock)
  for building in [-1,0]:exercise(0x460890,building=building)
 missing=points-hits;missing_edges={(a,b) for a,ends in branches.items() for b in ends if (a,b) not in edges};missing_jumps={(pc,target) for pc,targets in switches.items() for target in targets} -jump_edges
 print('Compared industrial logistics:',checked,'scenarios;',invocations,'invocations;',len(points&hits),'/',len(points),'instructions,',len(edges),'conditional edges;',len(jump_edges),'switch edges',flush=True)
 if quick:sys.exit(0)
 print('Unexecuted instructions:',[hex(a) for a in sorted(missing)],'; edges:',[(hex(a),hex(b)) for a,b in sorted(missing_edges)],'; switch edges:',[(hex(a),hex(b)) for a,b in sorted(missing_jumps)],flush=True)
 assert not missing,('instruction coverage',missing)
 assert not missing_edges,('edge coverage',missing_edges)
 assert not missing_jumps,('switch coverage',missing_jumps)
 sources=['CMakeLists.txt','include/industry_logistics.h','runtime/industry_logistics.c','runtime/actor_transport.c','runtime/actor_transport_model.c','direct_c/verify_industry_logistics.py','metadata/industry-logistics-native.json']
 proof=dict(native_sha256=catalogue['native_sha256'],compiled_C_path=str(target.resolve()),compiled_C_sha256=target_sha,passed=True,checked=checked,invocations=invocations,
  coverage={hex(a):dict(instructions=len(p),executed=len(p&hits)) for a,p in owned.items()},conditional_edges=len(edges),switch_edges=len(jump_edges),
  unexecuted_instructions=[hex(a) for a in sorted(missing)],unexecuted_edges=[[hex(a),hex(b)] for a,b in sorted(missing_edges)],unexecuted_switch_edges=[[hex(a),hex(b)] for a,b in sorted(missing_jumps)],
  source_hashes={p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest() for p in sources},commit=None,controlled_algorithm_endpoints=[],controlled_platform_endpoints=[],
  limitations=['Rea authenticates all native bytes/tables. Producer body is decoded with complete bounded switch flow where Listing metadata is absent; delivery includes the authenticated trailing RET beyond its truncated Listing body.',
  'Actual production-permission, route and road children execute. Named/canonical/frame adapters; full DWORD results, ordered owned stores, helper inputs, all original static data and saved ABI compared. Includes genuine permission/bound/selection/type/city-target changes and real road/route fixtures.',
  'Incidental caller-saved registers/flags, private scratch, concurrent access and host game/UI fidelity are excluded.'])
 suffix='diagnostic' if target.stem=='restore_link_check' else 'game'
 (ROOT/f'metadata/industry-logistics-{suffix}.json').write_text(json.dumps(proof,indent=2)+'\n')

