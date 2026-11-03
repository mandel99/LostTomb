"""Offline original/C storage withdrawals, ordered removal and the city cursor.

Execute actual children; compare ordered owned stores, child inputs, all original
static data and the saved ABI. Actual root traversal and native callers execute;
no child results are replaced and no host game is launched.
"""
from verify_options_startup import *
from unicorn import UC_HOOK_MEM_WRITE
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import json
catalogue=json.loads((ROOT/'metadata/storage-withdrawal-native.json').read_text())
assert catalogue['native_sha256']==hashlib.sha256(original.read_bytes()).hexdigest()
names={0x45b570:'restore_storage_withdraw',0x45b700:'restore_storage_remove_any',0x45b4d0:'restore_storage_withdraw_city'}
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
calls={0x45b570:4,0x45b490:1,0x47d350:0}
for c,pe in enumerate(images):
 u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(pe.base,(len(pe.data)+4095)&~4095);u.mem_write(pe.base,bytes(pe.data))
 u.mem_map(0x30000000,0x10000);u.mem_map(STOP,4096);u.mem_map(S,0x50000)
 state=dict(collect=False,own=True,writes=[],events=[],pending=set(),returns=set(),compiled=c,last_entry=None);machines.append((c,u,state))
 def write(uc,access,at,size,value,data):
  if data['collect'] and data['own']:
   data['writes'].append((at,size,value&((1<<(size*8))-1)))
   if data['mutation']=='owner' and at==0xb7482c+100*4 and not data['changed']:
    put(uc,0xeaac28,3);data['changed']=True
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
  mutation=data['mutation'];target=data['target']
  if mutation=='image' and t==0x45b490:
   word(uc,0x799a14+target*264,300);put(uc,0x7999e4+target*264,9999);put(uc,0xf566bc,1234)
  if mutation=='postfx' and t==0x47d350:
   word(uc,0x799a14+target*264,300);put(uc,0x7999e4+target*264,9999);put(uc,0xf566bc,1234);word(uc,0x7999ea+target*264,34);put(uc,0xeaac28,3)
  if mutation=='cursor' and t==0x45b570 and data['counts'][t]==1:
   word(uc,0xea3fc2+data['player']*0x49d8,3000)
  if mutation=='kind' and t==0x45b570 and data['counts'][t]==1:word(uc,0x7999e8+args[1]*264,20)
  if t not in owned:data['own']=False
  data['pending'].add(ret)
  if ret not in data['returns']:uc.hook_add(UC_HOOK_CODE,returned,data,begin=ret,end=ret);data['returns'].add(ret)
 # Observe each real C algorithm once at its named provider. Canonical and
 # compatibility wrappers can use CALL or a tail jump and are exercised below.
 state['entries']={(sym(names[t] if t in names else 'FUN_%08x'%t) if c else t):t for t in calls}
 for a in state['entries']:u.hook_add(UC_HOOK_CODE,child,state,begin=a,end=a)




frame_names={0x45b570:['restore_placement_support_C_0045b570'],0x45b700:['restore_god_C_0045b700'],0x45b4d0:['restore_placement_support_C_0045b4d0']}
def prepare(u,fn,cfg):
 def v(a,x,n=4):u.mem_write(a&0xffffffff,(x&((1<<(n*8))-1)).to_bytes(n,'little'))
 u.mem_write(0x560000,seed);u.mem_write(0x30000000,b'\xcc'*0x10000);u.mem_write(S,bytes(0x50000));u.mem_write(0x7999d8,bytes(4000*264))
 root=cfg.get('root',1);resource=cfg.get('resource',1);player=cfg.get('player',0)
 v(0xf566b4,900);v(0xf566bc,1000);v(0xf57204,2000);v(0xeaac28,cfg.get('owner',player))
 all_resources={resource,-1,0,1,2,30,31,34,65535,65536};all_resources.update(cfg.get('phases',[]))
 for owner in {player,cfg.get('owner',player),0,3}:
  for res in all_resources:
   v(0xea60d0+owner*0x49d8+res*4,cfg.get('available',500));v(0xea6160+owner*0x49d8+res*4,cfg.get('used',100))
 def warehouse(w,nodes,stocks,phases,kind=72,state=1,image_offset=0):
  v(0x7999e8+w*264,kind,2);v(0x7999d8+w*264,state,1);previous=w
  for i,node in enumerate(nodes):
   v(0x799a12+previous*264,node,2);previous=node
   for a,x,n in [(0x7999d8+node*264,1,1),(0x7999e8+node*264,73,2),(0x7999ea+node*264,phases[i],2),
    (0x799a14+node*264,stocks[i],2),(0x7999e4+node*264,100+image_offset+i,4),(0xb7482c+(100+image_offset+i)*4,0xdead0000+i,4)]:
    v(a,x,n)
  v(0x799a12+previous*264,cfg.get('terminal',0),2)
 stocks=cfg.get('stocks',[cfg.get('stock',100)]*8);phases=cfg.get('phases',[cfg.get('phase',resource)]*8)
 if fn==0x45b4d0:
  stores=cfg.get('stores',[(root,stocks)])
  for i,(w,quantities) in enumerate(stores):warehouse(w,list(range(1000+i*10,1008+i*10)),quantities,phases,cfg.get('kind',72),cfg.get('state',1),i*20)
  if cfg.get('many_empty'):
   for w in range(1,4000):v(0x7999d8+w*264,1,1);v(0x7999e8+w*264,72,2);v(0x799a14+w*264,0,2)
  v(0xea3fc2+player*0x49d8,cfg.get('cursor',0),2)
  return [player,resource,cfg.get('amount',1)]
 nodes=list(range(2,2+cfg.get('length',8)))
 warehouse(root,nodes,stocks,phases,cfg.get('kind',72),cfg.get('state',1))
 if cfg.get('cycle'):v(0x799a12+2*264,2,2)
 if 'break_at' in cfg:v(0x799a12+cfg['break_at']*264,cfg.get('break_value',0),2)
 if 'zero_resume' in cfg:v(0x799a12,cfg['zero_resume'],2)
 if 'negative_resume' in cfg:v(0x799a12-264,cfg['negative_resume'],2)
 if fn==0x45b570:return [player,root,resource,cfg.get('amount',1)]
 if fn==0x45b700:return [root,cfg.get('amount',1)]
 raise AssertionError(fn)

def exercise(fn=0x45b570,**cfg):
 global checked,invocations
 expected=None;void=fn==0x45b700
 for c,u,state in machines:
  routes=['native'] if not c else ['named','canonical',*frame_names[fn]]
  if c and void:routes+=['restore_god_building_0045b700']
  for route in routes:
   state.update(collect=False,own=True,writes=[],events=[],pending=set(),last_entry=None,top=fn,mutation=cfg.get('mutation'),counts={},changed=False,
    root=cfg.get('root',1),resource=cfg.get('resource',1),player=cfg.get('player',0),target=2)
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
   state['collect']=False;answer=(tuple(state['writes']),tuple(state['events']),snapshot(u),0 if void else result)
   if expected is None:expected=answer
   elif answer!=expected:
    diffs=[next(((i,a,b) for i,(a,b) in enumerate(zip(x,y)) if a!=b),None) for x,y in zip(expected[:2],answer[:2])]
    memory=next(((hex(0x560000+i),a,b) for i,(a,b) in enumerate(zip(expected[2],answer[2])) if a!=b),None)
    raise AssertionError((hex(fn),cfg,route,'stores/events',diffs,'lengths',[(len(x),len(y)) for x,y in zip(expected[:2],answer[:2])],'memory',memory,'result',(expected[3],answer[3])))
 checked+=1
 if checked%25==0:print('progress',checked,'scenarios',flush=True)

if __name__=='__main__':
 quick='--quick' in sys.argv
 for fn,cfg in [(0x45b570,{}),(0x45b570,dict(amount=10,stocks=[1,2,3,4,5,6,7,8])),(0x45b700,{}),
  (0x45b700,dict(stock=1,amount=3,mutation='owner')),(0x45b700,dict(stock=200,amount=1,mutation='postfx')),
  (0x45b4d0,dict(stores=[(1,[2,0,0,0,0,0,0,0]),(20,[5,0,0,0,0,0,0,0])],amount=4))]:exercise(fn,**cfg)
 if not quick:
  for fn in [0x45b570,0x45b700]:
   for kind in [0,20,72,193,194]:
    for amount in [-2147483648,-1,0,1,100,800,2147483647]:exercise(fn,kind=kind,amount=amount)
   for stock in [-32768,-1,0,1,199,200,299,300,399,400,401,32767]:
    for amount in [1,stock if stock>0 else 1,stock+1 if stock>0 else 2]:exercise(fn,stock=stock,amount=amount)
   for resource in [-1,0,1,30,31,34,65535,65536]:exercise(fn,resource=resource,phase=resource,amount=1)
   for length in [0,1,7,8]:
    for terminal in [0,-1]:exercise(fn,length=length,terminal=terminal,stock=1,amount=20)
   exercise(fn,cycle=True,stock=5,amount=11)
   exercise(fn,stocks=[10,10,0,0,0,0,0,0],amount=15)
   exercise(fn,phases=[1,2,1,2,1,2,1,2],stocks=[1,2,3,4,5,6,7,8],amount=10)
   exercise(fn,break_at=3,zero_resume=6,stocks=[0,0,0,0,100,0,0,0],amount=5)
   exercise(fn,break_at=3,break_value=-1,negative_resume=6,stocks=[0,0,0,0,100,0,0,0],amount=5)
   for player in [-1,0,3]:exercise(fn,player=player,owner=player,amount=10,available=0xfffffffe,used=1)
  exercise(stock=200,amount=1,mutation='image')
  for phase in [0,2,-1]:exercise(phase=phase,resource=1,amount=5)
  for resource in [1,31]:exercise(0x45b700,resource=resource,stock=200,amount=1,mutation='postfx')
  for amount in [-2147483648,-1,0,1,2,4,100]:
   for cursor in [-1,0,1,3998,3999,4000,32767]:
    exercise(0x45b4d0,cursor=cursor,amount=amount,stores=[(1,[2,0,0,0,0,0,0,0]),(20,[5,0,0,0,0,0,0,0])])
  for kind,state in itertools.product([20,72,193],[0,1,6]):exercise(0x45b4d0,kind=kind,state=state,amount=10)
  for player in [-1,0,3]:exercise(0x45b4d0,player=player,cursor=3998,stores=[(3999,[2,0,0,0,0,0,0,0]),(1,[5,0,0,0,0,0,0,0])],amount=4)
  for mutation in ['cursor','kind']:exercise(0x45b4d0,stores=[(1,[0]*8),(20,[5,0,0,0,0,0,0,0])],amount=3,mutation=mutation)
  exercise(0x45b4d0,cursor=-1,stores=[(0,[3,0,0,0,0,0,0,0])],amount=2)
  exercise(0x45b4d0,many_empty=True,stock=0,amount=1,cursor=3999)
 missing=points-hits;missing_edges={(a,b) for a,ends in branches.items() for b in ends if (a,b) not in edges}
 print('Compared withdrawal:',checked,'scenarios;',invocations,'invocations;',len(points)-len(missing),'/',len(points),'instructions,',len(edges),'conditional edges',flush=True)
 print('Unexecuted instructions:',[hex(a) for a in sorted(missing)],'; edges:',[(hex(a),hex(b)) for a,b in sorted(missing_edges)],flush=True)
 if quick:sys.exit(0)
 assert not missing,('instruction coverage',missing)
 assert not missing_edges,('edge coverage',missing_edges)
 print('PASS complete withdrawal comparison and coverage',flush=True)
 sources=['CMakeLists.txt','include/storage_delivery.h','runtime/storage_delivery.c','runtime/placement_support.c','runtime/placement_support_model.c',
  'runtime/god_building_effects.c','runtime/god_building_effects_model.c','src/unit_00888.c','direct_c/verify_storage_withdrawal.py','metadata/storage-withdrawal-native.json']
 proof=dict(native_sha256=catalogue['native_sha256'],compiled_C_path=str(target.resolve()),compiled_C_sha256=target_sha,passed=True,checked=checked,invocations=invocations,
  coverage={hex(a):dict(instructions=len(p),executed=len(p&hits)) for a,p in owned.items()},conditional_edges=len(edges),
  unexecuted_instructions=[hex(a) for a in sorted(missing)],unexecuted_edges=[[hex(a),hex(b)] for a,b in sorted(missing_edges)],
  source_hashes={p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest() for p in sources},commit=None,controlled_algorithm_endpoints=[],controlled_platform_endpoints=[],
  limitations=['Actual single-storage withdrawal, pile-frame and zero-leaf helpers. Named, canonical and compatibility adapters; full DWORD nonvoid results, ordered owned stores, helper inputs, all final original static data and saved ABI compared.',
  'Native 47D350 is authenticated XOR EAX,EAX; RET: its extra pushed caller slots are unused and normalized to a nullary C call. Genuine quantity, image, player, cursor and kind input changes do not replace helper results.',
  '45B700 is void: incidental caller-saved registers/flags, private scratch, concurrent access and host game/UI fidelity are excluded.'])
 suffix='diagnostic' if target.stem=='restore_link_check' else 'game'
 (ROOT/f'metadata/storage-withdrawal-{suffix}.json').write_text(json.dumps(proof,indent=2)+'\n')
