"""Native/C building-access controllers and exact neighbor query.

The three owned algorithms execute. Controller child endpoints are explicitly
controlled to exercise every caller branch and publication order; the481420
child executes its actual source. This is not a proof of the deeper repairs.
"""
from verify_options_startup import *
from unicorn import UC_HOOK_MEM_WRITE
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import json

catalogue=json.loads((ROOT/'metadata/building-access-native.json').read_text())
assert catalogue['native_sha256']==hashlib.sha256(original.read_bytes()).hexdigest()
rea=json.loads((ROOT/'metadata/building-access-rea-evidence.json').read_text())
evidence={r['evidence_id']:r for r in rea['records']}
for f in catalogue['functions']:
 r=evidence[f['bytes_evidence_id']];assembly=evidence[f['evidence_id']]
 assert r['authority']=='shipped-artifact' and r['confidence']=='observed'
 assert r['subject']['digest']['sha256']==catalogue['native_sha256']
 assert r['normalized_result']['complete'] and r['normalized_result']['bytes_hex']==f['bytes_hex']
 assert [p.split(':')[0] for p in assembly['normalized_result'].splitlines()]==f['instruction_addresses'].split()
names={0x457a40:'restore_building_refresh_route_flags',0x457af0:'restore_building_update_access',0x481420:'restore_building_has_access_neighbor'}
owned={};branches={};hits=set();edges=set();last=[None];checked=invocations=0
for f in catalogue['functions']:
 a=int(f['address'],16);raw=bytes.fromhex(f['bytes_hex']);assert raw==bytes(images[0].data[a-images[0].base:a-images[0].base+len(raw)])
 instructions=list(Cs(CS_ARCH_X86,CS_MODE_32).disasm(raw,a));owned[a]={int(p,16) for p in f['instruction_addresses'].split()}
 assert {i.address for i in instructions}==owned[a]
 for i in instructions:
  if i.mnemonic.startswith('j') and i.mnemonic!='jmp':branches[i.address]={int(i.op_str,16),i.address+i.size}
points=set.union(*owned.values());machines=[]
calls={0x51ce90:2,0x481420:3,0x517a40:4,0x47fe60:4,0x459020:4,0x487d00:3,
 0x542f40:3,0x40eb90:3,0x401af0:3,0x417490:3,0x4e6ae0:3,0x4e0c50:3,
 0x545a30:3,0x558f00:3,0x556230:3,0x54f8a0:3,0x481210:3,0x475b50:1,0x4f30b0:4,
 0x481060:3,0x4e2970:3,0x430cd0:1,0x517c20:4,0x517370:0,0x517660:0,0x5178e0:0,
 0x47b1a0:0,0x47bae0:5,0x4767d0:6,0x477500:4,0x476530:0,0x458270:0,0x4e0cf0:5}
member={0x40eb90,0x401af0,0x417490,0x4e6ae0,0x545a30,0x558f00,0x556230,0x54f8a0}
special_names={0x40eb90:'restore_monument_entrance_0040eb90',0x401af0:'restore_monument_entrance_00401af0',0x417490:'restore_monument_entrance_00417490',0x4e6ae0:'restore_monument_entrance_004e6ae0',
 0x545a30:'restore_complex_position_00545a30',0x558f00:'restore_complex_position_00558f00',0x556230:'restore_complex_position_00556230',0x54f8a0:'restore_complex_position_0054f8a0',
 0x458270:'restore_building_access_remove_disconnected',0x4f30b0:'restore_building_access_special_query',0x481420:'restore_building_has_access_neighbor'}
for c,pe in enumerate(images):
 u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(pe.base,(len(pe.data)+4095)&~4095);u.mem_write(pe.base,bytes(pe.data))
 u.mem_map(0x2ff00000,0x110000);u.mem_map(STOP,4096);u.mem_map(S,0x50000)
 state=dict(collect=False,writes=[],events=[],compiled=c,returns=set(),pending=set(),own=True);machines.append((c,u,state))
 def write(uc,access,at,size,value,data):
  if not data['collect'] or not data['own']:return
  data['writes'].append((at,size,value&((1<<(size*8))-1)))
  mutation=data['cfg'].get('mutation')
  if mutation=='store_bound' and at==0x7999f8+264:put(uc,0x78d790,1)
  if mutation=='store_owner' and at==0x7999f8+264:put(uc,0xeaac28,1)
  if mutation=='store_distance' and at==0x7999ec+264:
   word(uc,0xeed760+3000*4,5678);word(uc,0xeed760+(12+13*228)*4,5678)
 u.hook_add(UC_HOOK_MEM_WRITE,write,state,begin=0x560000,end=0x126cfff)
 if not c:
  def code(uc,pc,size,data):
   if not data['collect']:return
   hits.add(pc)
   if last[0] in branches:edges.add((last[0],pc))
   last[0]=pc
  for p in owned.values():u.hook_add(UC_HOOK_CODE,code,state,begin=min(p),end=max(p))
 def returned(uc,pc,size,data):
  if data['collect'] and pc in data['pending']:data['pending'].remove(pc);data['own']=True
 def child(uc,pc,size,data):
  if not data['collect'] or not data['own']:return
  t=data['entries'][pc];sp=uc.reg_read(UC_X86_REG_ESP);ret=get(uc,sp);cfg=data['cfg'];c=data['compiled']
  if t==data['top']:return
  n=4 if c and t==0x40eb90 else calls.get(t,4)
  args=tuple(get(uc,sp+4+i*4) for i in range(n))
  if t=='neighbor':t=args[0];args=args[1:]
  if c and t==0x40eb90:args=args[1:]
  observed=args
  if t==0x40eb90:observed=((get(uc,sp+4) if c else uc.reg_read(UC_X86_REG_ECX)),*args)
  if t==0x4f30b0:observed=(args[0],'tile',*args[2:])
  if t==0x4e2970:observed=(args[0],)
  data['events'].append((t,observed));data['counts'][t]=data['counts'].get(t,0)+1
  if t in cfg.get('actual_children',[]) or t==0x481420 or (t==0x51ce90 and cfg.get('real_flood')) or (t in [0x481060,0x481210,0x487d00,0x4f30b0] and cfg.get('real_search')):
   data['own']=False;data['pending'].add(ret)
   if ret not in data['returns']:uc.hook_add(UC_HOOK_CODE,returned,data,begin=ret,end=ret);data['returns'].add(ret)
   return
  mutation=cfg.get('mutation');number=data['counts'][t]
  if mutation=='child_bound' and t in [0x47fe60,0x481060]:put(uc,0x78d790,1)
  if mutation=='child_owner' and t in [0x47fe60,0x481060,0x517c20]:put(uc,0xeaac28,1)
  if mutation=='child_kind' and t==0x481060:word(uc,0x7999e8+264,72)
  if mutation=='child_dimensions' and t==0x517370:put(uc,0xbca2d0,13);put(uc,0xbca2d4,17)
  result=cfg.get('result',1)
  if t==0x517a40:
   recover=cfg.get('recover',0)
   if recover and number==recover+1:
    for p in [0,1,3]:put(uc,0xeed760+(4000+p)*4,cfg.get('recovery_distance',1))
  if t in [0x481060,0x481210,0x487d00]:
   result=cfg.get('query_result',3001);put(uc,0xd3b4e4,12);put(uc,0xd3b4e0,13)
  if t==0x47fe60:result=cfg.get('house_result',1);put(uc,0xbc9768,3000)
  if t==0x475b50:result=cfg.get('root',2)
  if t in [0x542f40,0x40eb90,0x401af0,0x417490,0x4e6ae0,0x4e0c50,0x545a30,0x558f00,0x556230,0x54f8a0]:
   put(uc,args[1],cfg.get('out_x',12));put(uc,args[2],cfg.get('out_y',13))
  if t==0x4f30b0:
   put(uc,args[1],3000);put(uc,args[2],12);put(uc,args[3],13)
  if mutation=='evict_population' and t==0x459020:word(uc,0x7999f4+264,99)
  cleanup=12 if not c and t in member else 0
  uc.reg_write(UC_X86_REG_EAX,result&0xffffffff);uc.reg_write(UC_X86_REG_ESP,sp+4+cleanup);uc.reg_write(UC_X86_REG_EIP,ret)
 state['entries']={}
 for t in calls:
  if c and t in [0x481060,0x481210,0x487d00]:continue
  name=special_names.get(t,'FUN_%08x'%t)
  state['entries'][sym(name) if c else t]=t
 if c:state['entries'][sym('restore_building_access_neighbor_query')]='neighbor'
 for a in state['entries']:u.hook_add(UC_HOOK_CODE,child,state,begin=a,end=a)

frames={0x457a40:'restore_city_C_00457a40',0x457af0:'restore_city_C_00457af0',0x481420:'restore_placement_support_C_00481420'}
def prepare(u,fn,cfg):
 def v(a,x,n=4):u.mem_write(a&0xffffffff,(x&((1<<(n*8))-1)).to_bytes(n,'little'))
 u.mem_write(0x560000,seed);u.mem_write(0x30000000,b'\xcc'*0x10000);u.mem_write(S,bytes(0x50000));u.mem_write(0x7999d8,bytes(4000*264))
 v(0xeaac28,cfg.get('owner',0));v(0x78d790,cfg.get('bound',len(cfg.get('kinds',[75,192]))));v(0xbca2d8,cfg.get('base',0));v(0xbca2d0,228);v(0xbca2d4,228)
 v(0x784c8c,12,2);v(0x784c8e,13,2);v(0xbc9694,777);v(0xd3b4e4,123);v(0xd3b4e0,234)
 u.mem_write(0xeed760,bytes(51984*4));u.mem_write(0xaf598c,bytes(51984*4));u.mem_write(0xa76aec,bytes(51984))
 if cfg.get('real_flood'):
  u.mem_write(0xed4140,b'\xff'*51984)
  for y in range(10,17):
   for x in range(10,17):v(0xed4140+y*228+x,0,1)
  if cfg.get('start_blocked'):v(0xed4140+13*228+12,252,1)
 for p in [0,1,3]:
  off=p*0x49d8
  for a,x,n in [(0xea3fe8,12+p,2),(0xea3fea,13+p,2),(0xea3ff0,14+p,2),(0xea3ff2,15+p,2),(0xea3ff4,4000+p,4),(0xea5ee4,cfg.get('registered',0),4)]:v(a+off,x,n)
  v(0xeed760+(4000+p)*4,cfg.get('exit_distance',1))
 for tile in [2999,3000,3001,12+13*228]:v(0xeed760+tile*4,cfg.get('distance',12345));v(0xa76aec+tile,67,1)
 for i,kind in enumerate(cfg.get('kinds',[75,192]),1):
  for a,x,n in [(0x7999d8,cfg.get('status',1),1),(0x7999db,cfg.get('size',3),1),(0x7999dd,cfg.get('house',0),1),(0x7999e8,kind,2),
   (0x7999de,12+i,2),(0x7999e0,13,2),(0x7999e4,3000+i,4),(0x7999f4,cfg.get('people',4),2),(0x7999f8,77,2),
   (0x7999fc,cfg.get('timer',0),2),(0x7999fe,98,2),(0x799a00,99,2),(0x799a0d,99,1)]:v(a+i*264,x,n)
 size=cfg.get('size',3);table=0x5cc9e0+(size&0xffffffff)*96
 for i in range(24):v(table+i*4,0 if i==cfg.get('terminator',24) else 1+i)
 cell=cfg.get('base',0)+13*228+12
 if fn in [0x481420,0x457a40]:
  for i in range(24):
   v(0xaf598c+(cell+1+i)*4,4 if i==cfg.get('neighbor',0)+(fn==0x457a40) else 0)
   v(0xeed760+(cell+1+i)*4,cfg.get('neighbor_distance',1))
 if cfg.get('real_repair'):
  width=16;height=16;base=1000;target=base+5*228+5
  v(0x78d790,0);v(0xbca2d0,width);v(0xbca2d4,height);v(0xbca2d8,base);v(0xbca2dc,228-width);v(0xbca2e0,0)
  for address in [0xee0c50,0xec7630,0xebab20,0xed4140]:u.mem_write(address,b'\xff'*51984)
  u.mem_write(0xd05720,bytes(51984*4))
  for row in range(height):
   v(0xbc9f24+row*4,0);v(0xbc9b94+row*4,width);v(0xbc9804+row*4,base+row*228)
   for col in range(width):
    cell=base+row*228+col;flags=0x80000 if cfg.get('blocked') else 0
    if cfg.get('surfaces') and row in [8,9,10] and col in [8,9,10]:flags|=0x20
    if cfg.get('surfaces') and row==12 and col in [8,9,10]:flags|=0x4000
    v(0xaf598c+cell*4,flags);v(0xb4e6fc+cell*2,0,2);v(0xb7482c+cell*4,0)
    if cfg.get('initial_open'):v(0xee0c50+cell,0,1)
  if cfg.get('region_tiles'):
   v(0xf5695c,5000);v(0xf56b84,7000)
   for row in [8,9]:
    for col in [8,9]:
     cell=base+row*228+col;v(0xaf598c+cell*4,0x140);v(0xb285cc+cell,128,1);v(0xae8e7c+cell,0,1);v(0xb7482c+cell*4,7123)
  v(0xea3fe8,2,2);v(0xea3fea,2,2);v(0xea3ff0,5,2);v(0xea3ff2,5,2);v(0xea3ff4,target)
  v(0xf55b88,0);v(0xe38dda,0,1);v(0xe38ec1,1,1);v(0x605a0c,1);v(0x605a3c,0);v(0xe38ebc,1);v(0xe38ea8,3,1);v(0xea624a,1,1)
  for i in range(5):v(0xe94e2e+i*120,1,2)
  for i in range(1000):v(0xe95232+i*48,0,2)
  v(0xe951ec,0);v(0xe951f0,1);v(0x604950,1);v(0xf565f4,700);v(0xf5686c,800)
 if fn==0x481420:return [12,13,size]
 return []

def exercise(fn=0x457af0,**cfg):
 global checked,invocations
 expected=None
 for c,u,state in machines:
  for route in (['native'] if not c else ['named','canonical','frame']):
   state.update(collect=False,own=True,writes=[],events=[],pending=set(),cfg=cfg,counts={},top=fn)
   args=prepare(u,fn,cfg);last[0]=None;state['collect']=True;invocations+=1
   pc=fn if not c else sym(names[fn] if route=='named' else 'FUN_%08x'%fn if route=='canonical' else frames[fn])
   try:
    if route=='frame':
     frame=S+0x1000;vsp=S+0x3000;initial=[0x11111111,0x22222222,0x33333333,saved[0],vsp,*saved[1:]]
     u.mem_write(frame,struct.pack('<10I',*initial,2,0));put(u,vsp,STOP)
     for i,a in enumerate(args):put(u,vsp+4+i*4,a)
     run(0,u,pc,[frame],budget=100000000 if cfg.get('real_repair') else 10000000);out=struct.unpack('<8I',bytes(u.mem_read(frame,32)))
     assert out[4]==vsp+4;assert [out[i] for i in [3,5,6,7]]==saved;result=out[0]
    else:result=run(0,u,pc,args,budget=100000000 if cfg.get('real_repair') else 10000000)
   except Exception as e:raise AssertionError(('execute',hex(fn),cfg,route,hex(u.reg_read(UC_X86_REG_EIP)))) from e
   state['collect']=False
   if 'expected_repairs' in cfg:
    assert state['counts'].get(0x517370,0)==cfg['expected_repairs'],(cfg,route,state['counts'])
    assert state['counts'].get(0x517a40,0)==cfg['expected_repairs']+1,(cfg,route,state['counts'])
    assert state['counts'].get(0x458270,0)==(cfg['expected_repairs']==15),(cfg,route,state['counts'])
    assert state['counts'].get(0x4e0cf0,0)==(cfg['expected_repairs']==1),(cfg,route,state['counts'])
   answer=(tuple(state['writes']),tuple(state['events']),snapshot(u),result if fn==0x481420 else None)
   if expected is None:expected=answer
   elif answer!=expected:
    diffs=[next(((i,a,b) for i,(a,b) in enumerate(zip(x,y)) if a!=b),None) for x,y in zip(expected[:2],answer[:2])]
    memory=next(((hex(0x560000+i),a,b) for i,(a,b) in enumerate(zip(expected[2],answer[2])) if a!=b),None)
    raise AssertionError((hex(fn),cfg,route,'stores/events',diffs,'lengths',[(len(x),len(y)) for x,y in zip(expected[:2],answer[:2])],'memory',memory,'result',(expected[3],answer[3])))
 checked+=1
 if checked%25==0:print('progress',checked,'scenarios',flush=True)

if __name__=='__main__':
 quick='--quick' in sys.argv
 for fn in names:exercise(fn)
 if quick:print('PASS quick access:',checked,'scenarios;',invocations,'invocations');sys.exit(0)
 for fn in names:
  for cfg in [dict(owner=1),dict(owner=3),dict(status=0),dict(status=3),dict(bound=0),dict(bound=-1),dict(house=1),dict(base=17)]:exercise(fn,**cfg)
 for cfg in [dict(kinds=[75,192,74]),dict(terminator=0),dict(neighbor=-1),dict(neighbor_distance=0),dict(neighbor_distance=65536),dict(mutation='child_bound')]:exercise(0x457a40,**cfg)
 for cfg in [dict(),dict(start_blocked=True),dict(neighbor=-1)]:exercise(0x457a40,real_flood=True,**cfg)
 for size in [0,1,3,5]:
  for cfg in [dict(terminator=0),dict(neighbor=-1),dict(neighbor=23),dict(neighbor_distance=0),dict(neighbor_distance=65536)]:exercise(0x481420,size=size,**cfg)
 for kind in [72,193,210,213,225,230,227,228,222,229,234,235,236,65,66,67,68,69,156,157,158,159,160,73,183,74]:
  exercise(kinds=[kind]);exercise(kinds=[kind],result=0,query_result=0)
 for cfg in [dict(registered=1),dict(query_result=1),dict(query_result=65537),dict(result=256),dict(result=0x12340001),dict(distance=65536),dict(mutation='child_bound'),dict(mutation='child_owner'),dict(mutation='child_kind'),dict(mutation='store_bound'),dict(mutation='store_owner')]:exercise(kinds=[72,193],**cfg)
 for house_result in [0,1]:
  for people in [0,4,-1]:
   for timer in [0,4,5,8,9,32767,-1]:exercise(kinds=[10,11],house=1,house_result=house_result,people=people,timer=timer,distance=0)
 exercise(kinds=[11,11],house=1,distance=0)
 exercise(kinds=[11,10],house=1,people=0,distance=0)
 for cfg in [dict(distance=65536),dict(distance=1),dict(mutation='evict_population',house_result=0,timer=5),dict(mutation='child_owner'),dict(mutation='child_bound')]:exercise(kinds=[10,11],house=1,**cfg)
 for kind in [225,230,227,228,229,234,235,236]:exercise(kinds=[kind],mutation='store_distance',out_x=12,out_y=13)
 for cfg in [dict(recover=1),dict(recover=2),dict(recover=15),dict(recover=0),dict(recover=2,mutation='child_owner'),dict(recover=1,mutation='child_dimensions')]:exercise(kinds=[],exit_distance=0,**cfg)
 for cfg in [dict(kinds=[72,193]),dict(kinds=[65,160]),dict(kinds=[210]),dict(kinds=[183]),dict(kinds=[72,65,210,183]),dict(kinds=[72,193],mutation='child_owner')]:exercise(real_search=True,**cfg)
 for cfg in [dict(initial_open=True,expected_repairs=0),dict(expected_repairs=1),dict(blocked=True,expected_repairs=15),dict(surfaces=True,expected_repairs=1),dict(region_tiles=True,expected_repairs=1)]:exercise(kinds=[],real_repair=True,actual_children=list(calls),**cfg)
 missing=points-hits;missing_edges={(a,b) for a,ends in branches.items() for b in ends if (a,b) not in edges}
 print('Compared access:',checked,'scenarios;',invocations,'invocations;',len(points&hits),'/',len(points),'instructions;',len(edges),'conditional outcomes',flush=True)
 print('Unexecuted instructions:',[hex(a) for a in sorted(missing)],'; edges:',[(hex(a),hex(b)) for a,b in sorted(missing_edges)],flush=True)
 assert not missing,('instruction coverage',missing)
 assert not missing_edges,('edge coverage',missing_edges)
 sources=['CMakeLists.txt','include/building_access.h','include/building_access_search.h','runtime/building_access.c','runtime/building_access_search.c','include/access_repair_flood.h','runtime/access_repair_flood.c','runtime/map_surface_repair.c','runtime/map_region_repair.c','include/map_region_repair.h','runtime/menu_drag_surface_passes.c','runtime/map_commit_connect.c','runtime/map_commit_costs.c','runtime/menu_drag_canal_region.c','runtime/menu_drag_canal_region_model.c','runtime/menu_drag_terrain_graph.c','runtime/menu_drag_water_refresh.c','runtime/menu_drag_road_refresh.c','runtime/menu_drag_paving_pass_model.c','runtime/menu_drag_paving_neighbors.c','include/event_publisher.h','src/unit_01911.c','src/unit_02296.c','src/unit_00119.c','runtime/city_updates.c','runtime/city_updates_model.c','runtime/placement_support.c','runtime/placement_support_model.c','direct_c/verify_building_access.py','metadata/building-access-native.json','metadata/building-access-rea-evidence.json']
 proof=dict(native_sha256=catalogue['native_sha256'],compiled_C_path=str(target.resolve()),compiled_C_sha256=target_sha,passed=True,checked=checked,invocations=invocations,
  coverage={hex(a):dict(instructions=len(p),executed=len(p&hits)) for a,p in owned.items()},conditional_edges=len(edges),unexecuted_instructions=[],unexecuted_edges=[],
  source_hashes={p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest() for p in sources},controlled_algorithm_endpoints=[hex(t) for t in calls if t!=0x481420],controlled_platform_endpoints=[],real_repair_composition_scenarios=5,
  limitations=['Controlled child algorithms isolate complete caller behavior; their results, output memory and owner/count mutations are explicit. This does not certify their whole behavior or end-to-end map repair.',
   'Three457A40 cases additionally execute the actual51CE90 flood and481420 neighbor child without result substitution; they compare all original static data. This does not cover every deeper-child instruction.',
   'Six457AF0 cases additionally execute actual481060/481210/487D00/4F30B0 access searches without replacing results, including their global outputs and the parent publications. Other cases control deeper map repair children.',
   'Five complete457AF0 compositions execute all reached direct children without substituted results: already connected, repair success, success with active surface passes, success with active channel/road region tiles, and all15 exhausted attempts. They assert flood/pass counts, success notification and final removal dispatch. No buildings are active in these finite16x16 repair fixtures.',
   'Native object receivers are retained where consumed; unused message arguments and private output-pointer addresses are normalized. Void returns exclude incidental volatile registers/flags.',
   'Original static memory, ordered owned stores, consumed child inputs and saved ABI compared. Concurrency and live screen behavior are outside this offline proof.'])
 suffix='diagnostic' if target.stem=='restore_link_check' else 'game'
 (ROOT/f'metadata/building-access-{suffix}.json').write_text(json.dumps(proof,indent=2)+'\n')
