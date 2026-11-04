"""Native/C building destruction; authenticated REA instruction ownership.
Ordered owned writes, child inputs, full static memory and saved ABI are compared.
Children are controlled unless actual_children explicitly enables real execution.
"""
from verify_options_startup import *
from unicorn import UC_HOOK_MEM_WRITE
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import json
catalogue=json.loads((ROOT/'metadata/building-destruction-native.json').read_text())
assert catalogue['native_sha256']==hashlib.sha256(original.read_bytes()).hexdigest()
rea=json.loads((ROOT/'metadata/building-destruction-rea-evidence.json').read_text());evidence={r['evidence_id']:r for r in rea['records']}
names={0x46d100:'restore_building_destroy',0x4c04a0:'restore_building_destroy_four_parts',0x46d0f0:'restore_building_destruction_root',0x4bfee0:'restore_building_clear_four_part_occupancy',0x4e9170:'restore_population_remove_destroyed'}
frames={a:'restore_god_C_%08x'%a for a in names}
compat={a:'restore_god_building_%08x'%a for a in names}
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
calls={0x4c04a0:1,0x4e9170:2,0x46ac50:1,0x46d0f0:1,0x43fd00:2,0x47c480:3,0x47bf50:7,0x46a400:6,0x5178b0:0,0x4bfee0:1,0x475b50:1,0x4e9350:2}
child_names={**names,0x43fd00:'restore_building_clear_farm_tiles',0x46a400:'restore_building_allocate_0046a400'}
for c,pe in enumerate(images):
 u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(pe.base,(len(pe.data)+4095)&~4095);u.mem_write(pe.base,bytes(pe.data))
 u.mem_map(0x30000000,0x10000);u.mem_map(STOP,4096);u.mem_map(S,0x50000)
 state=dict(collect=False,own=True,writes=[],events=[],compiled=c,pending=set(),returns=set());machines.append((c,u,state))
 def write(uc,access,at,size,value,data):
  if not data['collect'] or not data['own']:return
  data['writes'].append((at,size,value&((1<<(size*8))-1)))
  cfg=data['cfg'];mutation=cfg.get('mutation');record=264
  if mutation=='cached_house' and at==0xbc9694:byte(uc,0x7999dd+record,0)
  if mutation=='cached_view' and at==0x799a24+record:put(uc,0xbca2e0,2)
  if mutation=='width_before_store' and at==0x799a22+data.get('new_id',2)*264:put(uc,data['width_address'],0)
  if mutation=='cohort_read' and at==0xea1894+cfg.get('owner',0)*0x49d8:
   data['sums']+=1
   if data['sums']<100:word(uc,0xea3e34+cfg.get('owner',0)*0x49d8+data['sums']*2,data['sums']-50)
 u.hook_add(UC_HOOK_MEM_WRITE,write,state,begin=0x560000,end=0x126cfff)
 if not c:
  def code(uc,pc,size,data):
   if not data['collect'] or not data['own']:return
   hits.add(pc)
   if last[0] in branches:edges.add((last[0],pc))
   last[0]=pc
  for p in owned.values():u.hook_add(UC_HOOK_CODE,code,state,begin=min(p),end=max(p))
 def returned(uc,pc,size,data):
  if data['collect'] and pc in data['pending']:data['pending'].remove(pc);data['own']=True
 def child(uc,pc,size,data):
  if not data['collect'] or not data['own']:return
  t=data['entries'][pc]
  if t==data['fn']:return
  sp=uc.reg_read(UC_X86_REG_ESP);ret=get(uc,sp);cfg=data['cfg'];mutation=cfg.get('mutation')
  args=tuple(get(uc,sp+4+i*4) for i in range(calls[t]))
  if t==0x47bf50:args=(args[0]&65535,*args[1:])
  data['events'].append((t,args));data['counts'][t]=data['counts'].get(t,0)+1;number=data['counts'][t]
  if t in cfg.get('actual_children',[]):
   data['own']=False;data['pending'].add(ret)
   if ret not in data['returns']:uc.hook_add(UC_HOOK_CODE,returned,data,begin=ret,end=ret);data['returns'].add(ret)
   return
  result=cfg.get('child_result',0);record=264
  if t==0x46ac50:
   if 'release_kind' in cfg:word(uc,0x7999e8+record,cfg['release_kind'])
   if 'release_house' in cfg:byte(uc,0x7999dd+record,cfg['release_house'])
   if 'release_level' in cfg:word(uc,0x7999ea+record,cfg['release_level'])
  if t in [0x46d0f0,0x475b50]:
   result=cfg.get('root',1)
   if mutation=='root_kind':word(uc,0x7999e8+record,209)
  if t==0x4e9350:
   if mutation=='population_owner':put(uc,0xeaac28,3)
   if mutation=='population_cohorts':
    for i in range(100):word(uc,0xea3e34+args[0]*0x49d8+i*2,i-50)
  if t==0x47c480:
   if mutation=='clear_move':word(uc,0x7999de+record,105+number);word(uc,0x7999e0+record,107+number);byte(uc,0x7999db+record,6)
   if mutation=='clear_orientation':byte(uc,0x799a82+record,number%3);word(uc,0x7999de+record,100+number)
   if mutation=='clear_cell':put(uc,0x7999e4+record,1234);put(uc,0xaf598c+1234*4,4)
  if t==0x46a400:
   allocs=cfg.get('allocations');result=allocs[(number-1)%len(allocs)] if allocs else number+1;data['new_id']=result
   for a,v,n in [(0x7999de,args[2],2),(0x7999e0,args[3],2),(0x7999e4,1234+number,4),(0x7999db,1,1),(0x7999e8,99,2),(0x799a25,number+cfg.get('variant',0),1)]:
    {1:byte,2:word,4:put}[n](uc,a+result*264,v)
   byte(uc,0xacf85c+1234+number,(number+cfg.get('variant',0))%4)
   if mutation=='allocator_owner':put(uc,0xeaac28,number%4)
   if mutation=='allocator_root':word(uc,0x7999de+record,100+number);word(uc,0x7999e0+record,100-number)
  if t==0x47bf50:
   if mutation=='publisher_variant':byte(uc,0x799a25+args[0]*264,number+7)
   if mutation=='publisher_width':put(uc,data['width_address'],max(0,3-number))
   if mutation=='publisher_layout':put(uc,data['pattern']+min(number,3)*4,3);put(uc,0xbca2e0,7);byte(uc,0x799a82+record,7)
  uc.reg_write(UC_X86_REG_EAX,result&0xffffffff);uc.reg_write(UC_X86_REG_ESP,sp+4);uc.reg_write(UC_X86_REG_EIP,ret)
 state['entries']={(sym(child_names.get(t,'FUN_%08x'%t)) if c else t):t for t in calls}
 for a in state['entries']:u.hook_add(UC_HOOK_CODE,child,state,begin=a,end=a)

def prepare(u,fn,cfg,state):
 def v(a,x,n=4):u.mem_write(a&0xffffffff,(x&((1<<(n*8))-1)).to_bytes(n,'little'))
 u.mem_write(0x560000,seed);u.mem_write(0x30000000,b'\xcc'*0x10000);u.mem_write(S,bytes(0x50000));u.mem_write(0x7999d8,bytes(4000*264));u.mem_write(0xbcd5f8,bytes(2000*388))
 for a,x in [(0xeaac28,cfg.get('owner',0)),(0xbca2d8,0),(0xbca2d0,228),(0xbca2d4,228),(0xbca2dc,0),(0xbca2e0,cfg.get('view',0)),(0xf56c54,5000),(0xf56c5c,6000),(0xf55b88,0),(0xbc9738,1),(0xbc973c,1),(0x77567c,0),(0x604950,1)]:v(a,x)
 cell=100*228+100
 for a,x,n in [(0x7999d8,1,1),(0x7999db,cfg.get('size',2),1),(0x7999dd,cfg.get('house',0),1),(0x7999de,100,2),(0x7999e0,100,2),(0x7999e4,cell,4),(0x7999e8,cfg.get('kind',1),2),(0x7999ea,cfg.get('level',0),2),(0x7999f4,cfg.get('people',0),2),(0x799a25,cfg.get('variant',0),1),(0x799a82,cfg.get('orientation',0),1)]:v(a+264,x,n)
 for i in range(36):
  x=i%6;y=i//6
  if not cfg.get('native_tables'):v(0x5ccd10+i*8,x);v(0x5ccd14+i*8,y)
  at=(100+y)*228+100+x
  v(0xaf598c+at*4,4 if cfg.get('road')=='all' or (cfg.get('road')=='mixed' and i%2==0) else 8);v(0xacf85c+at,cfg.get('variant',0),1);v(0xb4e6fc+at*2,1,2)
 angle=(cfg.get('view',0)+cfg.get('orientation',0))&0xffffffff
 if (angle if angle<2147483648 else angle-4294967296)>=8:angle=(angle-8)&0xffffffff
 state['pattern']=(0x5df78c+angle*16)&0xffffffff;state['width_address']=0x5df814
 if not cfg.get('native_tables'):
  for i,p in enumerate(cfg.get('parts',[0,1,2,3])):v(state['pattern']+i*4,p)
  for i in range(4):v(0x5df814+i*16,cfg.get('width',i+1));v(0x5df818+i*16,i+1);v(0x5df81c+i*16,-i-2)
 if fn==0x4e9170:
  off=cfg.get('owner',0)*0x49d8
  for i in range(100):v(0xea3e34+off+i*2,cfg.get('cohort',i-20),2)
  v(0xea3f54+off,cfg.get('deaths',0));v(0xea5f0c+off,cfg.get('high',0));return [cfg.get('owner',0),cfg.get('people',1)]
 if fn==0x46d0f0:
  for i,n in enumerate(cfg.get('previous',[2,3,0]),1):v(0x799a10+i*264,n,2)
  return [cfg.get('building',1)]
 return [1,cfg.get('mode',1)] if fn==0x46d100 else [1]

def exercise(fn=0x46d100,**cfg):
 global checked,invocations
 expected=None
 for c,u,state in machines:
  for route in (['native'] if not c else ['named','canonical','frame','compat']):
   state.update(collect=False,own=True,pending=set(),writes=[],events=[],counts={},cfg=cfg,fn=fn,new_id=2,sums=0)
   args=prepare(u,fn,cfg,state);last[0]=None;state['collect']=True;invocations+=1
   pc=fn if not c else sym(names[fn] if route=='named' else 'FUN_%08x'%fn if route=='canonical' else frames[fn] if route=='frame' else compat[fn])
   try:
    if route=='frame':
     frame=S+0x1000;vsp=S+0x3000;initial=[0x11111111,0x22222222,0x33333333,saved[0],vsp,*saved[1:]]
     u.mem_write(frame,struct.pack('<10I',*initial,2,0));put(u,vsp,STOP)
     for i,a in enumerate(args):put(u,vsp+4+i*4,a)
     run(0,u,pc,[frame],budget=3000000);out=struct.unpack('<8I',bytes(u.mem_read(frame,32)));assert out[4]==vsp+4;assert [out[i] for i in [3,5,6,7]]==saved;result=out[0]
    else:result=run(0,u,pc,args,budget=3000000)
   except Exception as e:raise AssertionError(('execute',hex(fn),cfg,route,hex(u.reg_read(UC_X86_REG_EIP)))) from e
   state['collect']=False
   if 'expected_rubble' in cfg:
    count=sum(struct.unpack('<H',u.mem_read(0x7999e8+i*264,2))[0]==99 for i in range(1,4000))
    assert count==cfg['expected_rubble'],(hex(fn),cfg,route,'rubble records',count)
   answer=(tuple(state['writes']),tuple(state['events']),snapshot(u),result if fn==0x46d0f0 else None)
   if expected is None:expected=answer
   elif answer!=expected:
    diffs=[next(((i,a,b) for i,(a,b) in enumerate(zip(x,y)) if a!=b),None) for x,y in zip(expected[:2],answer[:2])]
    memory=next(((hex(0x560000+i),a,b) for i,(a,b) in enumerate(zip(expected[2],answer[2])) if a!=b),None)
    raise AssertionError((hex(fn),cfg,route,'stores/events',diffs,'lengths',[(len(x),len(y)) for x,y in zip(expected[:2],answer[:2])],'memory',memory,'result',(expected[3],answer[3])))
 checked+=1
 if checked%25==0:print('progress',checked,'scenarios',flush=True)

if __name__=='__main__':
 for fn in names:exercise(fn)
 if '--quick' in sys.argv:print('PASS quick',checked,invocations);sys.exit(0)
 for size in [0,1,2,3,4,5,6,7,255]:
  for road in ['none','mixed','all']:exercise(size=size,road=road)
 for kind in [30,31,33,209,75,76,74,181,182,192,202,0,65535]:exercise(kind=kind)
 for cfg in [dict(house=1,people=0),dict(house=1,people=10),dict(house=255,people=-32768),dict(house=1,people=3,mutation='cached_house'),dict(kind=31,root=0),dict(kind=31,root=-1),dict(kind=31,root=2),dict(kind=31,mutation='root_kind'),dict(release_kind=75),dict(release_kind=31),dict(mutation='clear_move'),dict(mutation='clear_cell'),dict(allocations=[0]),dict(allocations=[1]),dict(allocations=[0,1,2]),dict(mutation='allocator_owner'),dict(mutation='allocator_root'),dict(mutation='publisher_variant')]:exercise(**cfg)
 for level in [-32768,-1,0,1,2,32767]:
  for variant in range(4):exercise(release_house=1,release_level=level,variant=variant)
 for mode in [0,1,255,256,257,-1]:exercise(mode=mode)
 for fn in [0x4c04a0,0x4bfee0]:
  for orientation in [0,1,2,7,8,255]:
   for view in [0,2,8,-1]:exercise(fn,orientation=orientation,view=view)
 for cfg in [dict(width=0),dict(width=-1),dict(width=1),dict(parts=[0,0,0,0],mutation='width_before_store'),dict(parts=[0,0,0,0],width=3,mutation='publisher_width'),dict(mutation='publisher_layout'),dict(mutation='cached_view'),dict(mutation='publisher_variant'),dict(mutation='allocator_root'),dict(allocations=[0]),dict(allocations=[1]),dict(mutation='allocator_owner')]:exercise(0x4c04a0,**cfg)
 exercise(0x4bfee0,mutation='clear_orientation')
 for owner in [0,1,3]:
  for cohort in [-32768,-1,0,1,32767]:exercise(0x4e9170,owner=owner,cohort=cohort)
 for cfg in [dict(high=2147483647),dict(high=-1,cohort=-1),dict(high=2950),dict(deaths=2147483647,people=2),dict(people=-2147483648),dict(mutation='population_owner'),dict(mutation='population_cohorts'),dict(mutation='cohort_read')]:exercise(0x4e9170,**cfg)
 for root in [0,1,2,-1,0x12345678]:exercise(0x46d0f0,root=root)
 for cfg in [dict(),dict(previous=[0]),dict(previous=[2,2])]:exercise(0x46d0f0,actual_children=[0x475b50],**cfg)
 for cfg in [dict(people=0),dict(people=32),dict(people=5000,cohort=1)]:exercise(0x4e9170,actual_children=[0x4e9350],**cfg)
 for orientation in [0,1]:exercise(0x4bfee0,orientation=orientation,native_tables=True,actual_children=[0x47c480])
 for cfg in [dict(expected_rubble=4),dict(kind=31,expected_rubble=4),dict(kind=75,expected_rubble=4),dict(house=1,people=16,expected_rubble=4),dict(kind=202,expected_rubble=7),dict(size=6,expected_rubble=36),dict(size=3,road='mixed')]:exercise(native_tables=True,actual_children=list(calls),**cfg)
 for orientation in [0,1]:exercise(0x4c04a0,orientation=orientation,native_tables=True,expected_rubble=7,actual_children=list(calls))
 missing=points-hits;missing_edges={(a,b) for a,ends in branches.items() for b in ends if (a,b) not in edges}
 print('Compared destruction:',checked,'scenarios;',invocations,'invocations;',len(points&hits),'/',len(points),'instructions;',len(edges),'conditional outcomes',flush=True)
 print('Unexecuted instructions:',[hex(a) for a in sorted(missing)],'; edges:',[(hex(a),hex(b)) for a,b in sorted(missing_edges)],flush=True)
 assert not missing and not missing_edges,(missing,missing_edges)
 sources=['CMakeLists.txt','include/building_destruction.h','runtime/building_destruction.c','runtime/god_building_effects.c','runtime/god_building_effects_model.c','runtime/building_allocator.c','runtime/menu_drag_building_release.c','runtime/menu_drag_clear_footprint.c','runtime/menu_drag_road_place.c','runtime/displaced_population.c','runtime/menu_prepare_integer.c','runtime/building_removal.c','src/unit_00985.c','src/unit_00986.c','src/unit_01576.c','src/unit_01581.c','src/unit_02038.c','direct_c/verify_building_destruction.py','metadata/building-destruction-native.json','metadata/building-destruction-rea-evidence.json']
 proof=dict(native_sha256=catalogue['native_sha256'],compiled_C_path=str(target.resolve()),compiled_C_sha256=target_sha,passed=True,checked=checked,invocations=invocations,coverage={hex(a):dict(instructions=len(p),executed=len(p&hits)) for a,p in owned.items()},conditional_edges=len(edges),unexecuted_instructions=[],unexecuted_edges=[],source_hashes={p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest() for p in sources},controlled_algorithm_endpoints=[hex(t) for t in calls],actual_composition_scenarios=17,limitations=['Children are explicitly controlled unless actual_children enables them; complete owned algorithms are verified, not all deeper-child paths.','Seventeen genuine compositions include capped root traversal, demographic removal and footprint clearing; nine execute every direct child of ordinary/farm/refreshed/house/6x6/four-part destruction with native tables. Rubble record counts are asserted in eight whole-destruction cases. No results are substituted within enabled children.','Static memory, ordered owned writes, consumed endpoint inputs, defined result and saved ABI compared. Incidental void-result registers, flags, private scratch, concurrency and live game/UI behavior excluded.'])
 suffix='diagnostic' if target.stem=='restore_link_check' else 'game';(ROOT/f'metadata/building-destruction-{suffix}.json').write_text(json.dumps(proof,indent=2)+'\n')
