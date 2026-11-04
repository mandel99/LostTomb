"""Native/C four-part clearing, linked clearing, farm tiles and removal effects.

Owned algorithms execute; child endpoints are controlled unless a scenario
explicitly enables an actual child. Compare ordered stores, consumed child
inputs, original static memory and returning-call saved ABI.
"""
from verify_options_startup import *
from unicorn import UC_HOOK_MEM_WRITE
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import json

catalogue=json.loads((ROOT/'metadata/building-removal-native.json').read_text())
assert catalogue['native_sha256']==hashlib.sha256(original.read_bytes()).hexdigest()
rea=json.loads((ROOT/'metadata/building-removal-rea-evidence.json').read_text());evidence={r['evidence_id']:r for r in rea['records']}
names={0x4c03d0:'restore_building_clear_four_parts',0x43fd00:'restore_building_clear_farm_tiles',0x4695d0:'restore_building_clear_linked_parts',0x48c0e0:'restore_building_spawn_removal_effects'}
frames={a:'restore_%s_C_%08x'%('god' if a in [0x43fd00,0x4695d0] else 'invasion',a) for a in names}
compat={0x4c03d0:'restore_building_access_remove_special',0x43fd00:'restore_god_building_0043fd00',0x4695d0:'restore_god_building_004695d0'}
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
calls={0x47c2b0:5,0x46d100:2,0x47b310:3,0x47a120:2,0x4767d0:6,0x477500:4,0x4777c0:4,0x475cc0:5,0x4776f0:4,0x477890:4,0x475720:5,0x516f90:5,0x4b4800:5,0x4b4220:6,0x53b090:1}
for c,pe in enumerate(images):
 u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(pe.base,(len(pe.data)+4095)&~4095);u.mem_write(pe.base,bytes(pe.data))
 u.mem_map(0x30000000,0x10000);u.mem_map(STOP,4096);u.mem_map(S,0x50000)
 state=dict(collect=False,own=True,writes=[],events=[],compiled=c,pending=set(),returns=set());machines.append((c,u,state))
 def write(uc,access,at,size,value,data):
  if not data['collect'] or not data['own']:return
  data['writes'].append((at,size,value&((1<<(size*8))-1)))
  cfg=data['cfg'];mutation=cfg.get('mutation')
  if mutation=='farm_flag' and at==0xa9010c+data['cell']:put(uc,0xaf598c+data['cell']*4,0)
  if mutation=='farm_owner' and at==0xea6084+cfg.get('owner',0)*0x49d8:put(uc,0xeaac28,3)
  if mutation=='chain_status' and at==0x7999d8+2*264:word(uc,0x799a10+2*264,0)
 u.hook_add(UC_HOOK_MEM_WRITE,write,state,begin=0x560000,end=0x126cfff)
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
  t=data['entries'][pc];sp=uc.reg_read(UC_X86_REG_ESP);ret=get(uc,sp);cfg=data['cfg']
  args=tuple(get(uc,sp+4+i*4) for i in range(calls[t]))
  if t==0x4b4800:args=(args[0]&255,*args[1:4],args[4]&255)
  if t==0x46d100:args=(args[0],args[1]&255)
  if t==0x516f90:args=(*args[:4],args[4]&255)
  data['events'].append((t,args));data['counts'][t]=data['counts'].get(t,0)+1;number=data['counts'][t];mutation=cfg.get('mutation')
  if t in cfg.get('actual_children',[]):
   data['own']=False;data['pending'].add(ret)
   if ret not in data['returns']:uc.hook_add(UC_HOOK_CODE,returned,data,begin=ret,end=ret);data['returns'].add(ret)
   return
  result=cfg.get('child_result',0)
  if t==0x47c2b0:
   if mutation=='parts_rotation':byte(uc,0x799a82+264,number%3);word(uc,0x7999de+264,10+number);word(uc,0x7999e0+264,-10-number)
   if mutation=='parts_global':put(uc,0xbca2e0,6)
   if mutation=='parts_layout':put(uc,data['pattern']+number*4,(number+1)%4)
   if mutation=='chain_cut':word(uc,0x799a10+args[0]*264,0)
   if mutation=='chain_start_forward':word(uc,0x799a12+264,6)
  if t==0x46d100 and mutation=='chain_cut':word(uc,0x799a10+args[0]*264,0);word(uc,0x799a12+args[0]*264,0)
  if t in [0x47b310,0x47a120]:
   if mutation=='farm_refresh':put(uc,0x77567c,0)
   if mutation=='farm_record':put(uc,0x799aa8+264,0)
  if t==0x4b4800:
   allocations=cfg.get('allocations');result=allocations[(number-1)%len(allocations)] if allocations else number
   if mutation=='allocator_owner' and number==1:put(uc,0xeaac28,1)
   if mutation=='allocator_table':word(uc,0x5dc62c+(number-1)*8,32767);word(uc,0x5dc630+(number-1)*8,-32768)
   if mutation=='allocator_pixel_table':put(uc,0x5dc614+cfg.get('size',3)*4,99999)
  if t==0x4b4220 and mutation=='projection_direction':byte(uc,0x5dc6ac+(number-1)*4,255)
  uc.reg_write(UC_X86_REG_EAX,result&0xffffffff);uc.reg_write(UC_X86_REG_ESP,sp+4);uc.reg_write(UC_X86_REG_EIP,ret)
 state['entries']={(sym('restore_god_building_0046d100' if t==0x46d100 else 'FUN_%08x'%t) if c else t):t for t in calls}
 for a in state['entries']:u.hook_add(UC_HOOK_CODE,child,state,begin=a,end=a)

def prepare(u,fn,cfg,state):
 def v(a,x,n=4):u.mem_write(a&0xffffffff,(x&((1<<(n*8))-1)).to_bytes(n,'little'))
 u.mem_write(0x560000,seed);u.mem_write(0x30000000,b'\xcc'*0x10000);u.mem_write(S,bytes(0x50000));u.mem_write(0x7999d8,bytes(4000*264));u.mem_write(0xbcd5f8,bytes(2000*388))
 v(0xeaac28,cfg.get('owner',0));v(0xbca2d8,cfg.get('base',1000));v(0xbca2d0,228);v(0xbca2d4,228);v(0xbca2dc,0);v(0xbca2e0,cfg.get('view',0));v(0x77567c,cfg.get('refresh',1))
 v(0xbcd5f0,0);v(0xd056fc,123);v(0xd056e0,127);v(0x604950,1);v(0xf5653c,1000);v(0xf55b88,0)
 if cfg.get('full_pool'):
  for actor in range(1,2000):v(0xbcd606+actor*388,1,1)
 for i in range(1,10):
  for a,x,n in [(0x7999db,cfg.get('size',3),1),(0x7999de,cfg.get('x',100)+i,2),(0x7999e0,cfg.get('y',100)-i,2),(0x7999d8,1,1),(0x7999e8,cfg.get('record_kind',31),2),(0x799a82,cfg.get('orientation',0),1)]:v(a+i*264,x,n)
 if fn==0x4c03d0:
  angle=(cfg.get('view',0)+cfg.get('orientation',0))&0xffffffff
  if (angle if angle<2147483648 else angle-4294967296)>=8:angle=(angle-8)&0xffffffff
  state['pattern']=(0x5df78c+angle*16)&0xffffffff
  for i,p in enumerate(cfg.get('parts',[0,1,2,3])):v(state['pattern']+i*4,p)
  for i in range(4):v(0x5df814+i*16,cfg.get('width',i+1));v(0x5df818+i*16,cfg.get('dx',i+1));v(0x5df81c+i*16,cfg.get('dy',-i-2))
  if cfg.get('footprint_fixture'):
   for row in range(90,112):
    for col in range(90,112):
     cell=cfg.get('base',1000)+row*228+col;v(0xb4e6fc+cell*2,1,2);v(0xaf598c+cell*4,8)
  return [1]
 if fn==0x4695d0:
  for i,n in enumerate(cfg.get('previous',[2,3,0]),1):v(0x799a10+i*264,n,2)
  v(0x799a12+264,cfg.get('next_first',4),2);v(0x799a12+4*264,5,2)
  if cfg.get('cycle'):v(0x799a10+2*264,2,2);v(0x799a12+4*264,4,2)
  if cfg.get('footprint_fixture'):
   for building in [2,3,4,5]:
    for row in range(cfg.get('size',3)):
     for col in range(cfg.get('size',3)):
      cell=cfg.get('base',1000)+(100-building+row)*228+100+building+col
      v(0xb4e6fc+cell*2,building,2);v(0xaf598c+cell*4,8)
  return [1,cfg.get('destroy',0)]
 if fn==0x43fd00:
  base=cfg.get('base',1000);cell=cfg.get('cell',base+17*228+13);state['cell']=cell
  v(0x799aa8+264,cell);kind=cfg.get('kind',31);size={30:3,31:2,33:4,209:5}.get(kind,0)
  for p in [0,1,3]:v(0xea6084+p*0x49d8,0xabcd9876)
  for row in range(size):
   for col in range(size):
    at=cell+row*228+col;terrain=64 if cfg.get('mixed') and (row+col)%2==0 else cfg.get('terrain',0)
    for a,x,n in [(0xaf598c+at*4,terrain,4),(0xb7482c+at*4,0xdeadbeef,4),(0xb4e6fc+at*2,2345,2),(0xa9cc1c+at*2,3456,2),(0xb285cc+at,0xa5,1),(0xb67d1c+at,0xb6,1),(0xadc36c+at,255,1),(0xae8e7c+at,255,1),(0xa9010c+at,255,1)]:v(a,x,n)
  return [1,kind]
 size=cfg.get('size',3);v(0x5dc5fc+size*4,cfg.get('offset',2));v(0x5dc614+size*4,cfg.get('pixel',5))
 for i in range(16):
  v(0x5dc62c+i*8,cfg.get('dx',i-8),2);v(0x5dc630+i*8,cfg.get('dy',8-i),2);v(0x5dc6ac+i*4,cfg.get('direction',i%8),1)
 for actor in range(1,20):
  for a,x,n in [(0xbcd642,cfg.get('head_x',32766),2),(0xbcd644,cfg.get('head_y',-32767),2),(0xbcd61c,cfg.get('grid_x',32766),2),(0xbcd61e,cfg.get('grid_y',-32767),2),(0xbcd651,99,1),(0xbcd754,0xdeadbeef,4)]:v(a+actor*388,x,n)
 return [cfg.get('x',100),cfg.get('y',100),size,cfg.get('expanded',1)]

def exercise(fn=0x4c03d0,**cfg):
 global checked,invocations
 expected=None
 for c,u,state in machines:
  for route in (['native'] if not c else ['named','canonical','frame',*(['compat'] if fn in compat else [])]):
   state.update(collect=False,own=True,pending=set(),writes=[],events=[],counts={},cfg=cfg,cell=0)
   args=prepare(u,fn,cfg,state);last[0]=None;state['collect']=True;invocations+=1
   pc=fn if not c else sym(names[fn] if route=='named' else 'FUN_%08x'%fn if route=='canonical' else frames[fn] if route=='frame' else compat[fn])
   try:
    if route=='frame':
     frame=S+0x1000;vsp=S+0x3000;initial=[0x11111111,0x22222222,0x33333333,saved[0],vsp,*saved[1:]]
     u.mem_write(frame,struct.pack('<10I',*initial,2,0));put(u,vsp,STOP)
     for i,a in enumerate(args):put(u,vsp+4+i*4,a)
     run(0,u,pc,[frame],budget=3000000);out=struct.unpack('<8I',bytes(u.mem_read(frame,32)));assert out[4]==vsp+4;assert [out[i] for i in [3,5,6,7]]==saved
    else:run(0,u,pc,args,budget=3000000)
   except Exception as e:raise AssertionError(('execute',hex(fn),cfg,route,hex(u.reg_read(UC_X86_REG_EIP)))) from e
   state['collect']=False;answer=(tuple(state['writes']),tuple(state['events']),snapshot(u))
   if expected is None:expected=answer
   elif answer!=expected:
    diffs=[next(((i,a,b) for i,(a,b) in enumerate(zip(x,y)) if a!=b),None) for x,y in zip(expected[:2],answer[:2])]
    memory=next(((hex(0x560000+i),a,b) for i,(a,b) in enumerate(zip(expected[2],answer[2])) if a!=b),None)
    raise AssertionError((hex(fn),cfg,route,'stores/events',diffs,'lengths',[(len(x),len(y)) for x,y in zip(expected[:2],answer[:2])],'memory',memory))
 checked+=1
 if checked%25==0:print('progress',checked,'scenarios',flush=True)

if __name__=='__main__':
 quick='--quick' in sys.argv
 for fn in names:exercise(fn)
 if quick:print('PASS quick removal:',checked,'scenarios;',invocations,'invocations');sys.exit(0)
 for orientation in [0,1,2,7,8,127,255]:
  for view in [0,2,6,8,-1]:exercise(orientation=orientation,view=view)
 for cfg in [dict(width=0),dict(width=-1),dict(dx=2147483647,dy=-2147483648),dict(x=-32768,y=32767),dict(parts=[3,3,3,3]),dict(mutation='parts_rotation'),dict(mutation='parts_global'),dict(mutation='parts_layout')]:exercise(**cfg)
 for destroy in [0,1,256,-1]:
  for cfg in [dict(),dict(previous=[0],next_first=0),dict(previous=[-1],next_first=-32768),dict(cycle=True),dict(mutation='chain_cut'),dict(mutation='chain_start_forward'),dict(mutation='chain_status')]:exercise(0x4695d0,destroy=destroy,**cfg)
 for kind in [30,31,33,209,32,-1,0x1001e]:
  for terrain,refresh in [(0,0),(0,1),(64,1),(0xffffffff,1)]:exercise(0x43fd00,kind=kind,terrain=terrain,refresh=refresh)
 for cfg in [dict(record_kind=209),dict(record_kind=209,owner=1),dict(record_kind=209,owner=3,mutation='farm_owner'),dict(base=1000,cell=999),dict(kind=0,base=0,cell=0x80000000),dict(mixed=True,mutation='farm_refresh'),dict(terrain=64,mutation='farm_flag'),dict(mutation='farm_record')]:exercise(0x43fd00,**cfg)
 for expanded in [0,1,2,255,256,257,-1]:
  for cfg in [dict(),dict(allocations=[0]),dict(allocations=[0,1,0,2]),dict(allocations=[1])]:exercise(0x48c0e0,expanded=expanded,**cfg)
 for cfg in [dict(owner=0x10001),dict(pixel=0x7fffffff),dict(pixel=-1,dx=-32768,dy=32767),dict(offset=0x7fffffff,x=2147483647,y=-2147483648),dict(direction=2),dict(direction=3),dict(direction=255),dict(mutation='allocator_owner'),dict(mutation='allocator_table'),dict(mutation='allocator_pixel_table'),dict(mutation='projection_direction')]:exercise(0x48c0e0,**cfg)
 for orientation in [0,1]:exercise(0x4c03d0,orientation=orientation,footprint_fixture=True,actual_children=[0x47c2b0])
 exercise(0x4695d0,footprint_fixture=True,actual_children=[0x47c2b0])
 exercise(0x4695d0,destroy=1,footprint_fixture=True,actual_children=[0x46d100])
 farm_children=[0x47a120,0x47b310,0x4767d0,0x477500,0x4777c0,0x475cc0,0x4776f0,0x477890,0x475720,0x516f90]
 for cfg in [dict(kind=31),dict(kind=30,terrain=64),dict(kind=209,record_kind=209)]:exercise(0x43fd00,actual_children=farm_children,**cfg)
 for cfg in [dict(expanded=0),dict(expanded=1),dict(expanded=1,full_pool=True)]:exercise(0x48c0e0,actual_children=[0x4b4800,0x4b4220,0x53b090],**cfg)
 missing=points-hits;missing_edges={(a,b) for a,ends in branches.items() for b in ends if (a,b) not in edges};switch_missing={(a,b) for a,ends in switches.items() for b in ends if (a,b) not in jump_edges}
 cold_edges={(0x48c103,0x48c1ec)}
 print('Compared removal:',checked,'scenarios;',invocations,'invocations;',len(points&hits),'/',len(points),'instructions;',len(edges),'conditional outcomes;',len(jump_edges),'switch arms',flush=True)
 print('Unexecuted instructions:',[hex(a) for a in sorted(missing)],'; edges:',[(hex(a),hex(b)) for a,b in sorted(missing_edges)],flush=True)
 assert not missing and not missing_edges-cold_edges and not switch_missing,(missing,missing_edges-cold_edges,switch_missing)
 sources=['CMakeLists.txt','include/building_removal.h','runtime/building_removal.c','runtime/building_destruction.c','include/building_destruction.h','runtime/building_allocator.c','runtime/displaced_population.c','runtime/menu_drag_building_release.c','runtime/menu_drag_clear_footprint.c','runtime/menu_drag_road_place.c','runtime/city_updates.c','runtime/god_building_effects.c','runtime/god_building_effects_model.c','runtime/invasion_model.c','runtime/building_footprint.c','runtime/menu_drag_actor_create.c','runtime/menu_drag_actor_move.c','runtime/actor_civic_model.c','runtime/actor_civic.c','runtime/mission_unlock.c','src/unit_00588.c','src/unit_00945.c','src/unit_01312.c','src/unit_01580.c','direct_c/verify_building_removal.py','metadata/building-removal-native.json','metadata/building-removal-rea-evidence.json']
 proof=dict(native_sha256=catalogue['native_sha256'],compiled_C_path=str(target.resolve()),compiled_C_sha256=target_sha,passed=True,checked=checked,invocations=invocations,coverage={hex(a):dict(instructions=len(p),executed=len(p&hits)) for a,p in owned.items()},conditional_edges=len(edges),switch_arms=len(jump_edges),unexecuted_instructions=[],unexecuted_edges=[],analytically_unreachable_edges=[[hex(a),hex(b)] for a,b in sorted(cold_edges)],source_hashes={p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest() for p in sources},controlled_algorithm_endpoints=[hex(t) for t in calls],controlled_platform_endpoints=[],actual_composition_scenarios=10,limitations=['Child algorithms are explicitly controlled unless actual_children enables them; this verifies complete owned controller behavior, not every deeper child.','Ten composition cases run real linked destruction, four-part/linked footprint clearing, farm tile clearing with every ordered refresh child, or actor allocation/projection and the sound-disabled sound helper. No child results are substituted in those enabled paths; complete deeper-child coverage and physical output remain separate.','48C103 JLE is infeasible: the low-BYTE flag produces exactly4 or16 before TEST, so the count cannot be nonpositive.','Original static memory, ordered owned writes, consumed endpoint inputs, wrap widths and saved ABI compared. Incidental void-result volatile registers/flags, private scratch, concurrency and live game/UI behavior are excluded.'])
 suffix='diagnostic' if target.stem=='restore_link_check' else 'game';(ROOT/f'metadata/building-removal-{suffix}.json').write_text(json.dumps(proof,indent=2)+'\n')
