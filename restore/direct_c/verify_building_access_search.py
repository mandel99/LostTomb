"""Native/C access searches, aliased special-entry outputs and removal controller.

All four query algorithms run without controlled endpoints. The removal
controller's deeper children are controlled and their consumed inputs observed.
"""
from verify_options_startup import *
from unicorn import UC_HOOK_MEM_WRITE
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import json

catalogue=json.loads((ROOT/'metadata/building-access-search-native.json').read_text())
assert catalogue['native_sha256']==hashlib.sha256(original.read_bytes()).hexdigest()
rea=json.loads((ROOT/'metadata/building-access-search-rea-evidence.json').read_text())
evidence={r['evidence_id']:r for r in rea['records']}
names={0x481060:'restore_building_find_access',0x481210:'restore_building_find_wide_access',0x487d00:'restore_building_fixed_entry',0x4f30b0:'restore_building_special_entry',0x458270:'restore_building_remove_highest_serial'}
owned={};branches={};hits=set();edges=set();jump_edges=set();last=[None];checked=invocations=0
for f in catalogue['functions']:
 a=int(f['address'],16);raw=bytes.fromhex(f['bytes_hex']);r=evidence[f['bytes_evidence_id']]
 assert r['authority']=='shipped-artifact' and r['subject']['digest']['sha256']==catalogue['native_sha256']
 assert r['normalized_result']['complete'] and r['normalized_result']['bytes_hex']==f['bytes_hex']
 assert raw==bytes(images[0].data[a-images[0].base:a-images[0].base+len(raw)])
 instructions=list(Cs(CS_ARCH_X86,CS_MODE_32).disasm(raw,a));owned[a]={int(p,16) for p in f['instruction_addresses'].split()}
 assert {i.address for i in instructions}==owned[a]
 for i in instructions:
  if i.mnemonic.startswith('j') and i.mnemonic!='jmp':branches[i.address]={int(i.op_str,16),i.address+i.size}
for t in catalogue['tables']:
 a=int(t['address'],16);raw=bytes.fromhex(t['bytes_hex']);assert raw==bytes(images[0].data[a-images[0].base:a-images[0].base+len(raw)])
switches={int(pc,16):set(struct.unpack_from('<%dI'%n,images[0].data,int(at,16)-images[0].base)) for pc,at,n in catalogue['switches']}
points=set.union(*owned.values());machines=[]
calls={0x475b50:1,0x4e0cf0:5,0x4c03d0:1,0x47c2b0:5,0x43fd00:2,0x48c0e0:4,0x4695d0:2,0x516f90:5,0x5173a0:4}
child_names={0x4c03d0:'restore_building_access_remove_special',0x43fd00:'restore_god_building_0043fd00',0x48c0e0:'restore_building_access_refresh_terrain',0x4695d0:'restore_god_building_004695d0'}
for c,pe in enumerate(images):
 u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(pe.base,(len(pe.data)+4095)&~4095);u.mem_write(pe.base,bytes(pe.data))
 u.mem_map(0x30000000,0x10000);u.mem_map(STOP,4096);u.mem_map(S,0x50000)
 state=dict(collect=False,writes=[],events=[],compiled=c,own=True,pending=set(),returns=set());machines.append((c,u,state))
 def write(uc,access,at,size,value,data):
  if not data['collect'] or not data['own']:return
  data['writes'].append((at,size,value&((1<<(size*8))-1)))
  cfg=data['cfg'];mutation=cfg.get('mutation');r=cfg.get('root',2)*264
  if mutation=='bc_kind' and at==0xbc9694:word(uc,0x7999e8+r,33)
  if mutation=='status_kind' and at==0x7999d8+r and value==4:word(uc,0x7999e8+r,202)
 u.hook_add(UC_HOOK_MEM_WRITE,write,state,begin=0x560000,end=0x126cfff)
 u.hook_add(UC_HOOK_MEM_WRITE,write,state,begin=S,end=S+0x3ff)
 if not c:
  def code(uc,pc,size,data):
   if not data['collect']:return
   hits.add(pc)
   if last[0] in branches:edges.add((last[0],pc))
   if last[0] in switches:jump_edges.add((last[0],pc))
   last[0]=pc
  for p in owned.values():u.hook_add(UC_HOOK_CODE,code,state,begin=min(p),end=max(p))
 def returned(uc,pc,size,data):
  if data['collect'] and pc in data['pending']:data['pending'].remove(pc);data['own']=True
 def child(uc,pc,size,data):
  if not data['collect'] or not data['own']:return
  t=data['entries'][pc];sp=uc.reg_read(UC_X86_REG_ESP);ret=get(uc,sp);cfg=data['cfg'];c=data['compiled']
  n=3 if c and t==0x48c0e0 else calls[t]
  args=tuple(get(uc,sp+4+i*4) for i in range(n))
  if c and t==0x48c0e0:args=(*args,1)
  if t==0x4e0cf0:args=(*args[:4],args[4]&255)
  if t==0x516f90:args=(*args[:4],args[4]&255)
  data['events'].append((t,args));mutation=cfg.get('mutation');r=cfg.get('root',2)*264
  if (cfg.get('real_geometry') and t in [0x475b50,0x47c2b0,0x516f90,0x5173a0]) or cfg.get('real_cleanup'):
   data['own']=False;data['pending'].add(ret)
   if ret not in data['returns']:uc.hook_add(UC_HOOK_CODE,returned,data,begin=ret,end=ret);data['returns'].add(ret)
   return
  if t==0x4e0cf0 and mutation=='event_kind':word(uc,0x7999e8+r,202)
  if t==0x4c03d0 and mutation=='special_kind':word(uc,0x7999e8+r,31)
  if t in [0x47c2b0,0x43fd00,0x48c0e0,0x4695d0,0x516f90] and mutation=='reload_geometry':
   word(uc,0x7999de+r,t&32767);word(uc,0x7999e0+r,-(t&32767));byte(uc,0x7999db+r,t&255)
  if t==0x475b50 and mutation=='bound':put(uc,0x78d790,0)
  result=cfg.get('root',2) if t==0x475b50 else 0
  uc.reg_write(UC_X86_REG_EAX,result&0xffffffff);uc.reg_write(UC_X86_REG_ESP,sp+4);uc.reg_write(UC_X86_REG_EIP,ret)
 state['entries']={(sym(child_names.get(t,'FUN_%08x'%t)) if c else t):t for t in calls}
 for a in state['entries']:u.hook_add(UC_HOOK_CODE,child,state,begin=a,end=a)

def prepare(u,fn,cfg):
 def v(a,x,n=4):u.mem_write(a&0xffffffff,(x&((1<<(n*8))-1)).to_bytes(n,'little'))
 u.mem_write(0x560000,seed);u.mem_write(0x30000000,b'\xcc'*0x10000);u.mem_write(S,bytes(0x50000));u.mem_write(0x7999d8,bytes(4000*264))
 base=cfg.get('base',0);v(0xbca2d8,base);v(0xeaac28,cfg.get('owner',0));v(0xd3b4e4,0x12345678);v(0xd3b4e0,0x76543210)
 if fn in [0x481060,0x481210]:
  x=cfg.get('x',100);y=cfg.get('y',100);size=cfg.get('size',3);cell=(base+y*228+x)&0xffffffff
  u.mem_write(0xaf598c,bytes(51984*4));u.mem_write(0xeed760,bytes(51984*4));u.mem_write(0xa76aec,bytes(51984))
  for i in range(24):
   v(0x5cc9e0+size*96+i*4,0 if i==cfg.get('terminator',24) else 1+i*10)
   v(0x5cce98+size*192+i*8,cfg.get('dx',1000+i));v(0x5cce9c+size*192+i*8,cfg.get('dy',-1000-i))
  for p in [0,1,3]:
   for i,color in enumerate(cfg.get('priorities',list(range(1,11)))):v(0xea5f1c+p*0x49d8+i*8,color)
  for shift,index,distance,flags,color in cfg.get('candidates',[(0,0,20,64,1)]):
   at=(cell+1+index*10+shift)&0xffffffff;v(0xeed760+at*4,distance);v(0xaf598c+at*4,flags);v(0xa76aec+at,color,1)
  return [x,y,size]
 if fn==0x487d00:return [cfg.get('x',100),cfg.get('y',100),cfg.get('unused',0x12345678)]
 if fn==0x4f30b0:
  building=5;root=4;target_record=6;variation=cfg.get('variation',1);child_variation=cfg.get('child_variation',2);level=cfg.get('level',3)
  v(0x7999ea+building*264,1,2);v(0x5e4ef4,cfg.get('mode',0));v(0x799a2c+root*264,variation,1)
  v(0x5f5498+variation*128,target_record-root);v(0x5f3878+variation*960,target_record-root)
  v(0x799a2c+target_record*264,child_variation,1);v(0x7999ea+target_record*264,level,2)
  v(0x7999de+target_record*264,cfg.get('x',-123),2);v(0x7999e0+target_record*264,cfg.get('y',456),2)
  v(0x5e4ef8+(child_variation*144+level)*8,cfg.get('direction',4))
  tile,xp,yp=S+0x100,S+0x104,S+0x108;alias=cfg.get('alias')
  if alias=='xy':yp=xp
  if alias=='xt':tile=xp
  if alias=='yt':tile=yp
  if alias=='all':tile=yp=xp
  if alias=='record_y':xp=0x7999e0+target_record*264
  if alias=='record_x':yp=0x7999de+target_record*264
  if alias=='base':tile=0xbca2d8
  return [building,tile,xp,yp]
 statuses=cfg.get('statuses',[1,3,0]);scores=cfg.get('scores',[100,200,300]);v(0x78d790,cfg.get('bound',len(statuses)));v(0xbc9694,999)
 for i in range(0,5):
  v(0x7999d8+i*264,statuses[i-1] if 1<=i<=len(statuses) else 0,1);v(0x7999ee+i*264,scores[i-1] if 1<=i<=len(scores) else 0,2)
  for a,x,n in [(0x7999e8,cfg.get('kind',31),2),(0x7999de,cfg.get('x',-123),2),(0x7999e0,cfg.get('y',456),2),(0x7999db,cfg.get('size',3),1),(0x7999e4,1000+i,4)]:v(a+i*264,x,n)
 if cfg.get('real_geometry'):
  v(0xbca2d0,228);v(0xbca2d4,228);v(0xbca2dc,0)
  root=cfg.get('root',2);v(0x799a10+2*264,root if root!=2 else 0,2)
  x=cfg.get('x',100);y=cfg.get('y',100);size=cfg.get('size',3)
  for row in range(size):
   for col in range(size):
    cell=x+col+(y+row)*228;v(0xb4e6fc+cell*2,root,2);v(0xaf598c+cell*4,8)
    v(0x5cc8c0+row*48+col*8,row*228+col)
 if cfg.get('real_cleanup'):
  root=cfg.get('root',2);tile=cfg.get('x',100)+cfg.get('y',100)*228
  for building in [1,2,3]:v(0x7999d8+building*264,3 if building==root else 0,1)
  v(0x7999e4+root*264,tile);v(0x799aa8+root*264,tile)
  u.mem_write(0xbcd5f8,bytes(2000*388));u.mem_write(0xeaaf68,bytes(0xeadb28-0xeaaf68));u.mem_write(0x9784d8,bytes(50*44))
  v(0xbcd5f0,0);v(0xd056fc,123);v(0xd056e0,127);v(0x604950,1);v(0xf5653c,1000);v(0xf55b88,0)
  v(0xe38dda,0,1);v(0xe38ec1,1,1);v(0x605a0c,1);v(0x605a3c,0);v(0xe38ebc,1);v(0xe38ea8,3,1);v(0xea624a,1,1)
  for i in range(5):v(0xe94e2e+i*120,1,2)
  for i in range(1000):v(0xe95232+i*48,0,2)
  v(0xe951ec,0);v(0xe951f0,1)
 return []

def exercise(fn=0x481060,**cfg):
 global checked,invocations
 expected=None
 for c,u,state in machines:
  for route in (['native'] if not c else ['named','canonical','frame']):
   state.update(collect=False,own=True,pending=set(),writes=[],events=[],cfg=cfg);args=prepare(u,fn,cfg);last[0]=None;state['collect']=True;invocations+=1
   pc=fn if not c else sym(names[fn] if route=='named' else 'FUN_%08x'%fn if route=='canonical' else 'restore_city_C_%08x'%fn)
   try:
    if route=='frame':
     frame=S+0x1000;vsp=S+0x3000;initial=[0x11111111,0x22222222,0x33333333,saved[0],vsp,*saved[1:]]
     u.mem_write(frame,struct.pack('<10I',*initial,2,0));put(u,vsp,STOP)
     for i,a in enumerate(args):put(u,vsp+4+i*4,a)
     run(0,u,pc,[frame],budget=2000000);out=struct.unpack('<8I',bytes(u.mem_read(frame,32)));assert out[4]==vsp+4
     assert [out[i] for i in [3,5,6,7]]==saved;result=out[0]
    else:result=run(0,u,pc,args,budget=2000000)
   except Exception as e:raise AssertionError(('execute',hex(fn),cfg,route,hex(u.reg_read(UC_X86_REG_EIP)))) from e
   state['collect']=False;answer=(tuple(state['writes']),tuple(state['events']),snapshot(u),bytes(u.mem_read(S,0x400)),result if fn!=0x458270 else None)
   if expected is None:expected=answer
   elif answer!=expected:
    diffs=[next(((i,a,b) for i,(a,b) in enumerate(zip(x,y)) if a!=b),None) for x,y in zip(expected[:2],answer[:2])]
    memory=next(((hex(0x560000+i),a,b) for i,(a,b) in enumerate(zip(expected[2],answer[2])) if a!=b),None)
    scratch=next(((hex(S+i),a,b) for i,(a,b) in enumerate(zip(expected[3],answer[3])) if a!=b),None)
    raise AssertionError((hex(fn),cfg,route,'stores/events',diffs,'lengths',[(len(x),len(y)) for x,y in zip(expected[:2],answer[:2])],'memory',memory,'outputs',scratch,'result',(expected[4],answer[4])))
 checked+=1
 if checked%25==0:print('progress',checked,'scenarios',flush=True)

if __name__=='__main__':
 quick='--quick' in sys.argv
 for fn in names:exercise(fn)
 if quick:print('PASS quick access search:',checked,'scenarios;',invocations,'invocations');sys.exit(0)
 for fn in [0x481060,0x481210]:
  for cfg in [dict(candidates=[]),dict(terminator=0),dict(terminator=1),dict(owner=1),dict(owner=3),dict(x=-1,y=100),dict(base=17),dict(dx=2147483647,dy=-2147483648),dict(priorities=[1]*10)]:exercise(fn,**cfg)
  for d in [-2147483648,-1,0,1,65536,99999,100000,100001,2147483647]:
   for flags in [0,64,68]:exercise(fn,candidates=[(0,0,d,flags,255)])
  for color in [0,1,2,10,11,127,255]:exercise(fn,candidates=[(0,0,20,64,color),(0,1,30,64,1)])
  for cfg in [dict(candidates=[(0,0,20,64,1),(0,1,10,64,1)]),dict(candidates=[(0,0,20,0,1),(0,1,20,0,2)]),dict(candidates=[(0,0,30,0,1),(0,1,10,0,2)]),dict(candidates=[(0,0,20,64,255)]),dict(terminator=3,candidates=[(0,4,1,64,1)])]:exercise(fn,**cfg)
 for size in [0,1,3,5]:
  for fn in [0x481060,0x481210]:exercise(fn,size=size)
 for index in range(24):
  for flags in [0,64]:exercise(0x481060,candidates=[(0,index,20,flags,1)])
 for shift in [0,3,6]:
  for index in [0,1,23]:
   for flags in [0,64,68]:exercise(0x481210,candidates=[(shift,index,20,flags,1)])
 for cfg in [dict(candidates=[(0,0,40,0,1),(3,0,30,0,1),(6,0,20,0,1)]),dict(candidates=[(0,0,40,64,3),(3,0,30,64,2),(6,0,20,64,1)])]:exercise(0x481210,**cfg)
 for x,y,base in [(0,0,0),(-32768,32767,17),(-2147483648,2147483647,0xffffffff),(2147483647,-2147483648,0x12345678)]:exercise(0x487d00,x=x,y=y,base=base)
 for mode in [0,5,6]:
  for direction in [0,3,4,5,6,7,8,0xffffffff]:
   for alias in [None,'xy','xt','yt','all','record_x','record_y','base']:exercise(0x4f30b0,mode=mode,direction=direction,alias=alias)
 for cfg in [dict(base=0x12345678),dict(base=0xabcdef00,mode=5),dict(x=-32768,y=32767),dict(variation=255,child_variation=255,level=-1)]:exercise(0x4f30b0,**cfg)
 for cfg in [dict(bound=0),dict(bound=-1),dict(statuses=[0,2,4]),dict(scores=[0,0,0]),dict(scores=[-1,-32768,32767]),dict(scores=[200,200,200]),dict(root=0),dict(root=1),dict(size=255),dict(x=-32768,y=32767),dict(mutation='bound')]:exercise(0x458270,**cfg)
 for kind in [30,31,33,209,202,1]:exercise(0x458270,kind=kind)
 for mutation in ['event_kind','special_kind','bc_kind','status_kind','reload_geometry']:exercise(0x458270,kind=202 if mutation in ['special_kind','bc_kind'] else 31,mutation=mutation)
 for root,size in [(2,1),(2,3),(1,3)]:exercise(0x458270,real_geometry=True,root=root,size=size,x=100,y=100)
 for kind in [31,202,1]:
  exercise(0x458270,real_cleanup=True,real_geometry=True,kind=kind,x=100,y=100)
  for _,u,_ in machines:
   assert get(u,0xe951ec)==1 and get(u,0xbcd5f0)==16
   assert bytes(u.mem_read(0x7999d8+2*264,1))==b'\x04'
 missing=points-hits;missing_edges={(a,b) for a,ends in branches.items() for b in ends if (a,b) not in edges}
 switch_missing={(a,b) for a,ends in switches.items() for b in ends if (a,b) not in jump_edges}
 print('Compared access search:',checked,'scenarios;',invocations,'invocations;',len(points&hits),'/',len(points),'instructions;',len(edges),'conditional outcomes;',len(jump_edges),'switch arms',flush=True)
 print('Unexecuted instructions:',[hex(a) for a in sorted(missing)],'; edges:',[(hex(a),hex(b)) for a,b in sorted(missing_edges)],flush=True)
 assert not missing and not missing_edges and not switch_missing,(missing,missing_edges,switch_missing)
 sources=['CMakeLists.txt','include/building_access_search.h','include/building_access.h','runtime/building_access_search.c','runtime/building_removal.c','runtime/god_building_effects.c','runtime/god_building_effects_model.c','runtime/invasion_model.c','runtime/menu_drag_actor_create.c','runtime/actor_civic.c','runtime/actor_civic_model.c','runtime/city_updates.c','runtime/city_updates_model.c','runtime/menu_prepare_integer.c','runtime/building_footprint.c','runtime/map_commit_connect.c','runtime/map_commit_costs.c','src/unit_00830.c','src/unit_01188.c','src/unit_01189.c','src/unit_01269.c','src/unit_02135.c','direct_c/verify_building_access_search.py','metadata/building-access-search-native.json','metadata/building-access-search-rea-evidence.json','metadata/building-access-search-names.json','metadata/building-serial-producer-rea-evidence.json']
 proof=dict(native_sha256=catalogue['native_sha256'],compiled_C_path=str(target.resolve()),compiled_C_sha256=target_sha,passed=True,checked=checked,invocations=invocations,
  coverage={hex(a):dict(instructions=len(p),executed=len(p&hits)) for a,p in owned.items()},conditional_edges=len(edges),switch_arms=len(jump_edges),unexecuted_instructions=[],unexecuted_edges=[],
  source_hashes={p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest() for p in sources},controlled_algorithm_endpoints=[hex(t) for t in calls],controlled_platform_endpoints=[],
  limitations=['The four queries execute without controlled endpoints. The458270 removal-controller children are controlled to verify complete caller stores, reload phases and consumed inputs; their complete behavior remains separate.',
   'Three additional removal cases execute actual475B50 root selection,47C2B0 footprint clearing and516F90/5173A0 cost updates. Event/god/special-removal children remain controlled; no complete removal-graph claim is made.',
   'Three further458270 cases execute every direct child without replacement for kinds31/202/1, including real farm/four-part/linked cleanup, effects, actors, map refresh and event publication. Each case checks status4,16 allocated actors and one queued event. Sound is disabled; this proves these finite compositions, not all removal states or live playability.',
   'Native481210 fallback double shift,487D00 zero-based result,4F30B0 full-EAX high bits and aliased output/record fields are retained. All original static data plus output memory, ordered stores and saved ABI compared.',
   'Private scratch, incidental void-result volatile registers/flags, concurrency and live game/UI effects are excluded.'])
 suffix='diagnostic' if target.stem=='restore_link_check' else 'game'
 (ROOT/f'metadata/building-access-search-{suffix}.json').write_text(json.dumps(proof,indent=2)+'\n')
