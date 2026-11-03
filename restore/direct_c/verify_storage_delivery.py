"""Offline original/C storage resource sums, capacity, pile frames and deliveries.

Execute actual children; compare ordered owned stores, child inputs, all original
static data and the saved ABI. Actual root traversal and native callers execute;
no child results are replaced and no host game is launched.
"""
from verify_options_startup import *
from unicorn import UC_HOOK_MEM_WRITE
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import json
catalogue=json.loads((ROOT/'metadata/storage-delivery-native.json').read_text())
assert catalogue['native_sha256']==hashlib.sha256(original.read_bytes()).hexdigest()
names={0x45b230:'restore_storage_limit_exceeded',0x45d510:'restore_storage_resource_amount',0x45b490:'restore_storage_pile_frame',0x45b2a0:'restore_storage_deliver'}
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
points=set.union(*owned.values());machines=[]
calls={0x475b50:1,0x45b230:3,0x45d510:2,0x45b490:1}
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
  mutation=data['mutation'];root=data['root'];resource=data['resource'];target=data['target']
  if mutation=='slot' and t==0x45b230 and data['counts'][t]==1:byte(uc,0x799a99+root*264,2)
  if mutation=='rule' and t==0x45d510:word(uc,0x7900ec+data['slot']*196+resource*2,0)
  if mutation=='image' and t==0x45b490:put(uc,0x7999e4+target*264,9999);put(uc,0xf566bc,1234)
  if mutation=='chain' and t==0x45b490:word(uc,0x799a12+root*264,0)
  if t not in owned:data['own']=False
  data['pending'].add(ret)
  if ret not in data['returns']:uc.hook_add(UC_HOOK_CODE,returned,data,begin=ret,end=ret);data['returns'].add(ret)
 # Observe each real C algorithm once at its named provider. Canonical and
 # compatibility wrappers can use CALL or a tail jump and are exercised below.
 state['entries']={(sym(names[t] if t in names else 'FUN_%08x'%t) if c else t):t for t in calls}
 for a in state['entries']:u.hook_add(UC_HOOK_CODE,child,state,begin=a,end=a)



frame_names={0x45b230:['restore_actor_message_C_0045b230'],0x45d510:['restore_actor_message_C_0045d510','restore_transport_C_0045d510'],0x45b490:['restore_placement_support_C_0045b490'],0x45b2a0:['restore_god_C_0045b2a0']}
def prepare(u,fn,cfg):
 def v(a,x,n=4):u.mem_write(a&0xffffffff,(x&((1<<(n*8))-1)).to_bytes(n,'little'))
 u.mem_write(0x560000,seed);u.mem_write(0x30000000,b'\xcc'*0x10000);u.mem_write(S,bytes(0x50000))
 u.mem_write(0x7999d8,bytes(4000*264))
 root=cfg.get('root',1);resource=cfg.get('resource',1);slot=cfg.get('slot',1);length=cfg.get('length',8);player=cfg.get('player',0)
 for a,x,n in [(0x799a99+root*264,slot,1),(0x799a18+root*264,cfg.get('active',1),2),(0x799a22+root*264,cfg.get('blocked',0),2),
  (0xf566bc,cfg.get('image_base',1000),4),(0xf57204,cfg.get('image_alt',2000),4),(0xeaac28,3,4)]:
  v(a,x,n)
 if cfg.get('root_cycle'):v(0x799a10+root*264,root,2)
 phases=cfg.get('phases',[cfg.get('phase',resource)]*8);stocks=cfg.get('stocks',[cfg.get('stock',0)]*8);previous=root
 for i in range(length):
  node=2+i;v(0x799a12+previous*264,node,2);v(0x799a10+node*264,root,2);previous=node
  for a,x,n in [(0x7999ea+node*264,phases[i],2),(0x799a14+node*264,stocks[i],2),(0x7999e4+node*264,100+i,4),(0xb7482c+(100+i)*4,0xdead0000+i,4)]:
   v(a,x,n)
 v(0x799a12+previous*264,cfg.get('terminal',0),2)
 if cfg.get('cycle'):
  v(0x799a12+root*264,2,2);v(0x799a12+2*264,2,2)
 for st in {slot,2}:
  v(0x7900c2+st*196+resource,cfg.get('policy',0),1)
  v(0x7900ec+st*196+resource*2,cfg.get('second_maximum',20) if st==2 and cfg.get('mutation')=='slot' else cfg.get('maximum',3200),2)
  v(0x790134+st*196+resource*2,cfg.get('alternate_maximum',100),2)
 counter=player*0x49d8+resource*4
 v(0xea60d0+counter,cfg.get('available',500));v(0xea6160+counter,cfg.get('used',100))
 if fn==0x45b490:return [cfg.get('amount',0)]
 if fn==0x45d510:return [root,resource]
 if fn==0x45b230:return [root,cfg.get('amount',0),resource]
 if fn==0x45b2a0:return [player,root,resource,cfg.get('amount',1)]
 raise AssertionError(fn)

def exercise(fn=0x45b2a0,**cfg):
 global checked,invocations
 expected=None
 for c,u,state in machines:
  routes=['native'] if not c else ['named','canonical',*frame_names[fn]]
  if c and fn==0x45b2a0:routes+=['restore_god_building_0045b2a0']
  for route in routes:
   state.update(collect=False,own=True,writes=[],events=[],pending=set(),last_entry=None,top=fn,mutation=cfg.get('mutation'),counts={},
    root=cfg.get('root',1),resource=cfg.get('resource',1),slot=cfg.get('slot',1),target=2)
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

if __name__=='__main__':
 quick='--quick' in sys.argv
 for fn,cfg in [(0x45b2a0,{}),(0x45b230,dict(amount=257)),(0x45d510,dict(stock=100)),(0x45b490,dict(amount=400)),
  (0x45b2a0,dict(amount=3,stocks=[399,398,3,0,0,0,0,0])),(0x45b2a0,dict(amount=2,mutation='chain')),
  (0x45b2a0,dict(amount=30,maximum=50,mutation='slot'))]:exercise(fn,**cfg)
 if not quick:
  for length in range(9):
   for terminal in [0,-1]:exercise(0x45d510,length=length,terminal=terminal,stock=10)
  for phase,resource,stock in itertools.product([0,1,2,-1],[-1,0,1,65535],[-32768,-1,400]):
   exercise(0x45d510,phase=phase,resource=resource,stock=stock)
  exercise(0x45d510,cycle=True,stock=50)
  for amount in [-2147483648,-32768,-1,0,1,199,200,201,299,300,399,400,401,2147483647]:exercise(0x45b490,amount=amount)
  for policy,maximum,amount in itertools.product([0,1,255],[-32768,-1,0,1,3200,32767],[-1,0,1,255,256,257,2147483647,-2147483648]):
   exercise(0x45b230,policy=policy,maximum=maximum,alternate_maximum=maximum,amount=amount,stock=1)
  for slot,resource in itertools.product([0,1,255],[-1,0,1,30,31,34,65535,65536]):
   exercise(0x45b230,slot=slot,resource=resource,stock=400,amount=257)
  exercise(0x45b230,amount=10,maximum=100,mutation='rule')
  for active,blocked in itertools.product([-32768,-1,0,1,32767],[0,1,65535]):exercise(active=active,blocked=blocked)
  for policy,limit,amount in itertools.product([0,1],[-1,0,1,10],[0,1,2,20]):
   exercise(policy=policy,maximum=limit,alternate_maximum=limit,amount=amount)
  for amount in [-2147483648,-1,0,1,2,10,100,399,400,401]:
   exercise(amount=amount)
  for stocks in [[199,0,0,0,0,0,0,0],[299,0,0,0,0,0,0,0],[399,0,0,0,0,0,0,0],
   [400]*8,[-1]*8,[10,10,0,0,0,0,0,0],[-1,0,3,0,0,0,0,0]]:
   exercise(stocks=stocks,amount=2)
  for phase in [0,2,-1]:exercise(phase=phase,stock=100,amount=2)
  for length in [0,1,7]:exercise(length=length,amount=1)
  for resource in [-1,0,30,31,34,65535,65536]:exercise(resource=resource,amount=1)
  for player in [-1,0,3]:exercise(player=player,amount=1,available=0,used=0xffffffff)
  exercise(root=0,amount=1);exercise(root_cycle=True)
  exercise(amount=1,mutation='image');exercise(amount=30,maximum=50,mutation='slot')
  exercise(amount=10,maximum=100,mutation='rule')
  exercise(amount=3200,phase=0);exercise(amount=2147483647,stocks=[1,0,0,0,0,0,0,0])
 missing=points-hits;missing_edges={(a,b) for a,ends in branches.items() for b in ends if (a,b) not in edges}
 excluded={(int(x['branch'],16),int(x['target'],16)) for x in catalogue['analytical_exclusions']}
 print('Compared storage:',checked,'scenarios;',invocations,'invocations;',len(points)-len(missing),'/',len(points),'instructions,',len(edges),'conditional edges',flush=True)
 print('Unexecuted instructions:',[hex(a) for a in sorted(missing)],'; edges:',[(hex(a),hex(b)) for a,b in sorted(missing_edges)],flush=True)
 if quick:sys.exit(0)
 assert not missing,('instruction coverage',missing)
 assert missing_edges==excluded,('edge coverage',missing_edges,excluded)
 print('PASS complete storage comparison; one authenticated analytically unreachable branch',flush=True)
 sources=['CMakeLists.txt','include/storage_delivery.h','runtime/storage_delivery.c','src/unit_00883.c','src/unit_00884.c','runtime/actor_transport.c',
  'runtime/placement_support.c','runtime/god_building_effects.c','runtime/god_building_effects_model.c','runtime/menu_actor_message.c','runtime/placement_support_model.c','runtime/actor_transport_model.c',
  'direct_c/verify_storage_delivery.py','metadata/storage-delivery-native.json']
 proof=dict(native_sha256=catalogue['native_sha256'],compiled_C_path=str(target.resolve()),compiled_C_sha256=target_sha,passed=True,checked=checked,invocations=invocations,
  coverage={hex(a):dict(instructions=len(p),executed=len(p&hits)) for a,p in owned.items()},conditional_edges=len(edges),
  unexecuted_instructions=[hex(a) for a in sorted(missing)],unexecuted_edges=[[hex(a),hex(b)] for a,b in sorted(missing_edges)],analytical_exclusions=catalogue['analytical_exclusions'],
  source_hashes={p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest() for p in sources},commit=None,controlled_algorithm_endpoints=[],controlled_platform_endpoints=[],
  limitations=['Actual root, capacity, stock-sum and pile-frame children. Named, canonical and legacy frame adapters compared through real algorithm execution; full DWORD return values, ordered owned stores, helper inputs, all final original data and saved ABI.',
  'Compatibility adapters expose standard C return/data effects and preserve saved registers/stack; incidental caller-saved registers/flags and private scratch are excluded. Input mutations do not replace any child result. No host game, concurrent access or UI fidelity claim.'])
 suffix='diagnostic' if target.stem=='restore_link_check' else 'game'
 (ROOT/f'metadata/storage-delivery-{suffix}.json').write_text(json.dumps(proof,indent=2)+'\n')
