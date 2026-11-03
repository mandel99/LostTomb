"""Offline original/C building bounds and warehouse stock summaries.

Execute actual children; compare ordered owned stores, child inputs, all original
static data and the saved ABI. Actual root, tutorial and road children execute;
no child results are replaced and no host game is launched.
"""
from verify_options_startup import *
from unicorn import UC_HOOK_MEM_WRITE
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import json
catalogue=json.loads((ROOT/'metadata/warehouse-stock-native.json').read_text())
assert catalogue['native_sha256']==hashlib.sha256(original.read_bytes()).hexdigest()
names={0x45aa30:'restore_building_rebuild_bounds',0x45aaa0:'restore_warehouse_update_stock',0x45aae0:'restore_warehouse_rebuild_stock'}
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
switches={int(pc,16):set(struct.unpack_from('<%dI'%n,images[0].data,int(at,16)-images[0].base)) for pc,at,n in catalogue.get('switches',[])}
points=set.union(*owned.values());machines=[]
calls={0x480180:3,0x475b50:1,0x417ae0:1,0x45aae0:1}
for c,pe in enumerate(images):
 u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(pe.base,(len(pe.data)+4095)&~4095);u.mem_write(pe.base,bytes(pe.data))
 u.mem_map(0x30000000,0x10000);u.mem_map(STOP,4096);u.mem_map(S,0x50000)
 state=dict(collect=False,own=True,writes=[],events=[],pending=set(),returns=set(),compiled=c,last_entry=None);machines.append((c,u,state))
 def write(uc,access,at,size,value,data):
  if data['collect'] and data['own']:
   data['writes'].append((at,size,value&((1<<(size*8))-1)))
   cfg=data['cfg'];off=cfg.get('player',1)*0x49d8
   if data['mutation']=='high' and at==0x78d790 and value:put(uc,0x78d78c,3999)
   if data['mutation']=='phase' and at==0x799a1c+2*264:word(uc,0x7999ea+2*264,9);word(uc,0x799a14+2*264,123)
   if data['mutation']=='quantity' and at==0xea60d0+off+cfg.get('resource',1)*4 and value:word(uc,0x799a14+2*264,555)
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
  if mutation=='bound' and t==0x480180:put(uc,0x78d790,1)
  if mutation=='access' and t==0x475b50:byte(uc,0x799a1c+264,0)
  if mutation=='bin' and t==0x475b50:word(uc,0x7999ea+args[0]*264,9);word(uc,0x799a14+args[0]*264,123)
  if mutation=='tutorial' and t==0x417ae0:put(uc,0xe38ebc,1);byte(uc,0xe38ea8,127)
  if t not in owned:data['own']=False
  data['pending'].add(ret)
  if ret not in data['returns']:uc.hook_add(UC_HOOK_CODE,returned,data,begin=ret,end=ret);data['returns'].add(ret)
 # Observe each real C algorithm once at its named provider. Canonical and
 # compatibility wrappers can use CALL or a tail jump and are exercised below.
 state['entries']={(sym(names[t] if t in names else 'FUN_%08x'%t) if c else t):t for t in calls}
 for a in state['entries']:u.hook_add(UC_HOOK_CODE,child,state,begin=a,end=a)



frame_names={t:['restore_city_C_%08x'%t] for t in names}

def prepare(u,fn,cfg,state):
 def v(a,x,n=4):u.mem_write(a&0xffffffff,(x&((1<<(n*8))-1)).to_bytes(n,'little'))
 def record(bid,fields):
  for a,x,n in fields:v(a+bid*264,x,n)
 u.mem_write(0x560000,seed);u.mem_write(0x30000000,b'\xcc'*0x10000);u.mem_write(S,bytes(0x50000));u.mem_write(0x7999d8,bytes(4000*264))
 if fn==0x45aa30:
  u.mem_write(0x7900c0,bytes(200*196));v(0x78d790,7777);v(0x78d788,7777);v(0x78d78c,cfg.get('high',0))
  for i,flag in cfg.get('buildings',[(1,1),(2,2),(3999,255)]):v(0x7999d8+i*264,flag,1)
  for i,flag in cfg.get('slots',[(1,1),(2,2),(199,255)]):v(0x7900c0+i*196,flag,1)
  return []
 player=cfg.get('player',1);off=player*0x49d8;v(0xeaac28,player);v(0xbca2d8,0);v(0xe38ebc,cfg.get('mode',1));v(0xea624a,cfg.get('monthly',1));v(0x78d790,cfg.get('bound',9))
 for p in {0,1,player}:
  for i in range(10):v(0xea5f1c+p*0x49d8+i*8,i+1)
 for i in range(36):v(0xea60d0+off+i*4,10000+i);v(0xea6160+off+i*4,20000+i)
 # Genuine orientation-four or fallback orientation-three road lookup.
 for y in range(6,20):
  for x in range(6,20):v(0xaf598c+(y*228+x)*4,0)
 orientation=cfg.get('orientation',4);style=cfg.get('roads','primary')
 if style!='none':
  primary=struct.unpack_from('<24i',images[0].data,0x5cc9e0+orientation*96-images[0].base)
  fallback=struct.unpack_from('<24i',images[0].data,0x5cc9e0+3*96-images[0].base)
  step=next(x for x in (fallback if style=='fallback' else primary) if x and (style!='fallback' or x not in primary))
  tile=11*228+12+step;v(0xaf598c+tile*4,0x40);v(0xa76aec+tile,1,1)
 record(1,[(0x7999d8,cfg.get('root_state',1),1),(0x7999e8,cfg.get('kind',72),2),(0x7999de,12,2),(0x7999e0,11,2),(0x7999db,orientation,1),(0x799a1c,cfg.get('root_enabled',9),1)])
 stocks=cfg.get('stocks',[cfg.get('stock',100)]*8);resource=cfg.get('resource',1)
 for i in range(8):
  record(i+2,[(0x7999d8,cfg.get('state',1),1),(0x7999e8,cfg.get('bin_kind',73),2),(0x799a10,cfg.get('parent',1),2),(0x799a1c,9,1),(0x7999ea,resource,2),(0x799a14,stocks[i],2)])
 if cfg.get('mode',1)==0:
  v(0xe38dda,0,1);v(0x605a0c,1);v(0x605a3c,0)
  for i in range(1000):v(0xe95232+i*48,1,2)
  tutorial=cfg.get('tutorial',9);scene={9:0x5795ac,10:0x5795c8,13:0x57961c}[tutorial]
  v(0xe38ea8,cfg.get('scene',get(u,scene)),1);v(0x7027c1,cfg.get('stage9',0),1);v(0x7027c2,cfg.get('stage10',0),1);v(0x7027c5,cfg.get('stage13',0),1);v(0x7027c4,cfg.get('previous',1),1)
 if cfg.get('blocked_tutorial'):v(0xe38ebc,1)
 return [player] if fn==0x45aae0 else []

def exercise(fn=0x45aae0,**cfg):
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
   state['collect']=False;answer=(tuple(state['writes']),tuple(state['events']),snapshot(u),None)
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
 exercise(roads='fallback');exercise(roads='none');exercise(0x45aa30,mutation='high',buildings=[(5,1)])
 for tutorial,resource in [(9,13),(10,15),(13,12)]:exercise(mode=0,tutorial=tutorial,resource=resource)
 if not quick:
  for buildings in [[],[(0,1)],[(1,1)],[(3999,1)],[(1,0)],[(2,2),(50,255)],[(20,3)]]:
   for high in [-1,0,1,2000,3999,4000]:exercise(0x45aa30,buildings=buildings,high=high)
  for slots in [[],[(0,1)],[(1,1)],[(199,1)],[(2,0),(10,2),(150,255)]]:exercise(0x45aa30,slots=slots)
  for cfg in [dict(bound=0),dict(bound=-1),dict(bound=1),dict(player=0),dict(player=3),dict(state=0),dict(state=2),dict(bin_kind=72),dict(root_state=0),dict(root_state=2),dict(kind=193),dict(kind=70),dict(kind=70,root_enabled=0),dict(kind=70,root_enabled=3),dict(parent=0)]:exercise(**cfg)
  for resource in [-1,0,1,12,13,15,35,36,40]:exercise(resource=resource)
  for stock in [-32768,-1,0,1,199,200,299,300,399,400,401,32767]:exercise(stock=stock)
  for mutation in ['bound','access','bin','phase','quantity']:exercise(mutation=mutation)
  for tutorial,resource,threshold in [(9,13,200),(10,15,300),(13,12,800)]:
   for amount in [0,threshold-1,threshold,threshold+1]:exercise(mode=0,tutorial=tutorial,resource=resource,stocks=[amount]+[0]*7)
   for cfg in [dict(previous=0),dict(scene=127),{f'stage{tutorial}':1},dict(mutation='tutorial')]:exercise(mode=0,tutorial=tutorial,resource=resource,**cfg)
   exercise(mode=0,tutorial=tutorial,resource=resource,blocked_tutorial=True)
  for monthly in [0,1,256]:exercise(0x45aaa0,monthly=monthly)
 missing=points-hits;missing_edges={(a,b) for a,ends in branches.items() for b in ends if (a,b) not in edges}
 print('Compared warehouse stock:',checked,'scenarios;',invocations,'invocations;',len(points&hits),'/',len(points),'instructions,',len(edges),'conditional edges',flush=True)
 if quick:sys.exit(0)
 print('Unexecuted instructions:',[hex(a) for a in sorted(missing)],'; edges:',[(hex(a),hex(b)) for a,b in sorted(missing_edges)],flush=True)
 assert not missing,('instruction coverage',missing)
 assert not missing_edges,('edge coverage',missing_edges)
 sources=['CMakeLists.txt','include/warehouse_stock.h','runtime/warehouse_stock.c','runtime/city_updates.c','runtime/city_updates_model.c','src/unit_00880.c','direct_c/verify_warehouse_stock.py','metadata/warehouse-stock-native.json']
 proof=dict(native_sha256=catalogue['native_sha256'],compiled_C_path=str(target.resolve()),compiled_C_sha256=target_sha,passed=True,checked=checked,invocations=invocations,
  coverage={hex(a):dict(instructions=len(p),executed=len(p&hits)) for a,p in owned.items()},conditional_edges=len(edges),unexecuted_instructions=[],unexecuted_edges=[],
  source_hashes={p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest() for p in sources},commit=None,controlled_algorithm_endpoints=[],controlled_platform_endpoints=[],
  limitations=['Rea authenticates all code bytes/instructions. Actual road, root and tutorial/event children execute. Occupied real message slots keep the tutorial pipeline offline.',
   'Named/canonical/three frame adapters, ordered stores, child inputs, final original static data and saved ABI compared. Includes signed stock/type/capacity arithmetic, real fallback access, live bounds and genuine input changes at child/store boundaries.',
   'These are void functions. Incidental caller-saved registers/flags, private scratch, concurrent access and host game/UI fidelity are excluded.'])
 suffix='diagnostic' if target.stem=='restore_link_check' else 'game'
 (ROOT/f'metadata/warehouse-stock-{suffix}.json').write_text(json.dumps(proof,indent=2)+'\n')

