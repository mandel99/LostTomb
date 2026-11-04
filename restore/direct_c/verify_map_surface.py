"""Native/C full-map surface passes, ordered memory and child-call oracle.
Controlled child endpoints expose each owned branch; explicit actual_children
scenarios run genuine deeper algorithms without substituting their results.
"""
from verify_options_startup import *
from unicorn import UC_HOOK_MEM_WRITE
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import json
catalogue=json.loads((ROOT/'metadata/map-surface-native.json').read_text())
assert catalogue['native_sha256']==hashlib.sha256(original.read_bytes()).hexdigest()
rea=json.loads((ROOT/'metadata/map-surface-rea-evidence.json').read_text());evidence={r['evidence_id']:r for r in rea['records']}
names={0x476530:'restore_map_refresh_garden_tiles',0x47b1a0:'restore_map_refresh_canal_tiles'}
frames={a:'restore_C_%08x'%a for a in names}
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
calls={0x47cae0:6,0x47bf50:7,0x47db00:1,0x484f00:0,0x484be0:4,0x47e000:0,0x47e750:0,0x51f780:0}
for c,pe in enumerate(images):
 u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(pe.base,(len(pe.data)+4095)&~4095);u.mem_write(pe.base,bytes(pe.data))
 u.mem_map(0x30000000,0x10000);u.mem_map(STOP,4096);u.mem_map(S,0x50000)
 state=dict(collect=False,own=True,writes=[],events=[],compiled=c,pending=set(),returns=set());machines.append((c,u,state))
 def write(uc,access,at,size,value,data):
  if not data['collect'] or not data['own']:return
  data['writes'].append((at,size,value&((1<<(size*8))-1)));cfg=data['cfg'];mutation=cfg.get('mutation');cell=cfg.get('base',1000)
  if mutation=='clear_access' and at==0xb7482c+cell*4 and value==0:byte(uc,0xb285cc+cell,0x56);byte(uc,0xb67d1c+cell,0x23)
  if mutation=='clear_height' and at==0xb7482c+cell*4 and value==0:put(uc,0xbca2d4,1)
  if mutation=='clear_region' and at==0xb7482c+cell*4 and value==0:put(uc,0xbc9b94,2);put(uc,0xbc9804+4,cell+229)
  if mutation=='access_sprite' and at==0xb285cc+cell:byte(uc,0xb67d1c+cell,0x15);put(uc,0xbc9740,123)
  if mutation=='sprite_palette' and at==0xb67d1c+cell:put(uc,0xf5686c,4567);put(uc,0xbc9740,999);put(uc,0xbc9748,87)
  if mutation=='initial_height' and at==0xbc9768 and not data['writes'][:-1]:put(uc,0xbca2d4,0)
  if mutation=='row_width_cache' and at==0xbc9764 and value==0:put(uc,0xbca2d0,1)
  if mutation=='cursor_cache' and at==0xbc9768 and value==cell+1:put(uc,0xbc9768,cell+10)
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
  t=data['entries'][pc];sp=uc.reg_read(UC_X86_REG_ESP);ret=get(uc,sp);cfg=data['cfg'];mutation=cfg.get('mutation')
  args=tuple(get(uc,sp+4+i*4) for i in range(calls[t]))
  if t==0x47bf50:args=(args[0]&65535,*args[1:])
  if t==0x484be0:args=(*args[:3],args[3]&255)
  data['events'].append((t,args));data['counts'][t]=data['counts'].get(t,0)+1;number=data['counts'][t]
  if t in cfg.get('actual_children',[]):
   data['own']=False;data['pending'].add(ret)
   if ret not in data['returns']:uc.hook_add(UC_HOOK_CODE,returned,data,begin=ret,end=ret);data['returns'].add(ret)
   return
  result=0
  if t==0x47cae0:
   results=cfg.get('queries',[0]);result=results[(number-1)%len(results)]
   if mutation=='query_palette':put(uc,0xf565f4,1000+number)
   if mutation=='query_variation':byte(uc,0xacf85c+cfg.get('base',1000),(number+2)%4)
   if mutation=='query_limits':put(uc,0xbc9b94,1);put(uc,0xbca2d4,2)
  if t==0x47bf50:
   if cfg.get('publish'):
    for row in range(args[4]):
     for col in range(args[3]):put(uc,0xb7482c+(cfg.get('base',1000)+args[2]*228+args[1]+row*228+col)*4,args[5])
   if mutation=='publisher_height':put(uc,0xbca2d4,1)
  if t==0x484be0:
   second=args[0]==0x5cbcd8;result=cfg.get('second',1) if second else cfg.get('first',1)
   put(uc,0xbc9740,7 if second else 3);put(uc,0xbc9748,40 if second else 20)
   if mutation=='pattern_cursor':put(uc,0xbc9768,cfg.get('base',1000)+number)
  if t==0x47e000:result=cfg.get('crossing',0)
  if t==0x51f780:
   if mutation=='child_width':put(uc,0xbca2d0,1)
   if mutation=='child_height':put(uc,0xbca2d4,1)
   if mutation=='child_padding':put(uc,0xbca2dc,227)
   if mutation=='child_cursor' and number==1:put(uc,0xbc9768,cfg.get('base',1000)+10);put(uc,0xbc9764,1)
  uc.reg_write(UC_X86_REG_EAX,result&0xffffffff);uc.reg_write(UC_X86_REG_ESP,sp+4);uc.reg_write(UC_X86_REG_EIP,ret)
 state['entries']={(sym('FUN_%08x'%t) if c else t):t for t in calls}
 for a in state['entries']:u.hook_add(UC_HOOK_CODE,child,state,begin=a,end=a)

def prepare(u,fn,cfg):
 u.mem_write(0x560000,seed);u.mem_write(0x30000000,b'\xcc'*0x10000);u.mem_write(S,bytes(0x50000))
 width=cfg.get('width',4);height=cfg.get('height',3);base=cfg.get('base',1000)
 for a,v in [(0xbca2d0,width),(0xbca2d4,height),(0xbca2d8,base),(0xbca2dc,cfg.get('padding',228-max(width,0))),(0xf565f4,cfg.get('palette',700)),(0xf5686c,cfg.get('palette',800)),(0x604950,1)]:put(u,a,v)
 for row in range(8):
  left=cfg.get('left',0);limit=cfg.get('right',width)
  if cfg.get('empty_rows') and row%2:limit=left
  put(u,0xbc9f24+row*4,left);put(u,0xbc9b94+row*4,limit);put(u,0xbc9804+row*4,base+row*228)
  for col in range(16):
   cell=base+row*228+col;flags=cfg.get('terrain',0x20 if fn==0x476530 else 0x4000)
   if cfg.get('mixed'):flags=[0,0x20,0x220,0x420,0x620,0x4000,0x4008][(row+col)%7]
   put(u,0xaf598c+cell*4,flags);put(u,0xb7482c+cell*4,123);byte(u,0xacf85c+cell,cfg.get('variation',(row+col)%4));byte(u,0xb285cc+cell,0xad);byte(u,0xb67d1c+cell,0x82)

def exercise(fn=0x476530,**cfg):
 global checked,invocations
 expected=None
 for c,u,state in machines:
  for route in (['native'] if not c else ['named','canonical','frame']):
   state.update(collect=False,own=True,pending=set(),writes=[],events=[],counts={},cfg=cfg,fn=fn)
   prepare(u,fn,cfg);last[0]=None;state['collect']=True;invocations+=1
   pc=fn if not c else sym(names[fn] if route=='named' else 'FUN_%08x'%fn if route=='canonical' else frames[fn])
   try:
    if route=='frame':
     frame=S+0x1000;vsp=S+0x3000;initial=[0x11111111,0x22222222,0x33333333,saved[0],vsp,*saved[1:]]
     u.mem_write(frame,struct.pack('<10I',*initial,2,0));put(u,vsp,STOP)
     run(0,u,pc,[frame],budget=3000000);out=struct.unpack('<8I',bytes(u.mem_read(frame,32)));assert out[4]==vsp+4;assert [out[i] for i in [3,5,6,7]]==saved
    else:run(0,u,pc,[],budget=3000000)
   except Exception as e:raise AssertionError(('execute',hex(fn),cfg,route,hex(u.reg_read(UC_X86_REG_EIP)))) from e
   state['collect']=False
   if cfg.get('expect_crossing'):assert state['counts'].get(0x47e750,0)>0,(cfg,route,state['counts'])
   if 'expected_image' in cfg:
    for row in range(cfg.get('height',3)):
     for col in range(cfg.get('width',4)):
      cell=cfg.get('base',1000)+row*228+col
      assert get(u,0xb7482c+cell*4)==cfg['expected_image'],(route,cfg,cell,get(u,0xb7482c+cell*4))
   answer=(tuple(state['writes']),tuple(state['events']),snapshot(u))
   if expected is None:expected=answer
   elif answer!=expected:
    diffs=[next(((i,a,b) for i,(a,b) in enumerate(zip(x,y)) if a!=b),None) for x,y in zip(expected[:2],answer[:2])]
    memory=next(((hex(0x560000+i),a,b) for i,(a,b) in enumerate(zip(expected[2],answer[2])) if a!=b),None)
    raise AssertionError((hex(fn),cfg,route,'stores/events',diffs,'lengths',[(len(x),len(y)) for x,y in zip(expected[:2],answer[:2])],'memory',memory))
 checked+=1
 if checked%25==0:print('progress',checked,'scenarios',flush=True)

if __name__=='__main__':
 for fn in names:exercise(fn)
 if '--quick' in sys.argv:print('PASS quick',checked,invocations);sys.exit(0)
 for fn in names:
  for width,height in [(0,0),(0,3),(-1,3),(4,0),(4,-1),(1,1),(4,2),(3,4)]:exercise(fn,width=width,height=height)
  for terrain in [0,8,0x20,0x220,0x420,0x620,0x4000,0x4008,0xffffffff]:exercise(fn,terrain=terrain)
  exercise(fn,mixed=True)
 for cfg in [dict(left=2,right=2),dict(left=3,right=2),dict(left=-2,right=2),dict(empty_rows=True),dict(mixed=True),dict(palette=0xffffffff),dict(mutation='clear_access'),dict(mutation='clear_height'),dict(mutation='clear_region'),dict(mutation='access_sprite')]:exercise(**cfg)
 for queries in [[1],[0,1],[0],[0x100],[0,0x100],[0xffffffff]]:
  for variation in range(4):exercise(queries=queries,variation=variation)
 for cfg in [dict(mutation='query_palette'),dict(queries=[0,1],mutation='query_variation'),dict(mutation='query_limits'),dict(queries=[1],mutation='publisher_height'),dict(queries=[1],publish=True),dict(queries=[0,1],publish=True)]:exercise(**cfg)
 for crossing in [0,1,0x100,0xffffffff]:
  for second in [0,1,0x100,-1]:exercise(0x47b1a0,crossing=crossing,second=second)
 for mutation in ['access_sprite','sprite_palette','initial_height','row_width_cache','cursor_cache','pattern_cursor','child_width','child_height','child_padding','child_cursor']:exercise(0x47b1a0,mutation=mutation)
 for cfg in [dict(width=3,height=3,expected_image=707),dict(width=2,height=2,expected_image=706),dict(width=1,height=1,expected_image=700),dict(width=4,height=4),dict(mixed=True)]:exercise(actual_children=list(calls),**cfg)
 for cfg in [dict(),dict(terrain=0x4004),dict(terrain=0xc000,expect_crossing=True),dict(mixed=True)]:exercise(0x47b1a0,actual_children=list(calls),**cfg)
 missing=points-hits;missing_edges={(a,b) for a,ends in branches.items() for b in ends if (a,b) not in edges}
 print('Compared map surfaces:',checked,'scenarios;',invocations,'invocations;',len(points&hits),'/',len(points),'instructions;',len(edges),'conditional outcomes',flush=True)
 print('Unexecuted instructions:',[hex(a) for a in sorted(missing)],'; edges:',[(hex(a),hex(b)) for a,b in sorted(missing_edges)],flush=True)
 assert not missing and not missing_edges,(missing,missing_edges)
 sources=['CMakeLists.txt','include/map_surface_repair.h','include/surface_passes.h','runtime/map_surface_repair.c','runtime/menu_drag_neighbors.c','runtime/menu_drag_road_place.c','runtime/menu_drag_water_refresh.c','runtime/menu_drag_paving_neighbors.c','src/unit_01142.c','src/unit_01233.c','include/map_refresh_postpass2.h','runtime/menu_drag_surface_passes.c','runtime/menu_drag_vegetation_pass_model.c','runtime/menu_drag_rail_pass_model.c','direct_c/verify_map_surface.py','metadata/map-surface-native.json','metadata/map-surface-rea-evidence.json']
 proof=dict(native_sha256=catalogue['native_sha256'],compiled_C_path=str(target.resolve()),compiled_C_sha256=target_sha,passed=True,checked=checked,invocations=invocations,coverage={hex(a):dict(instructions=len(p),executed=len(p&hits)) for a,p in owned.items()},conditional_edges=len(edges),unexecuted_instructions=[],unexecuted_edges=[],source_hashes={p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest() for p in sources},controlled_algorithm_endpoints=[hex(t) for t in calls],actual_composition_scenarios=9,limitations=['Child algorithms are controlled unless actual_children enables them; complete owned algorithms are verified, not all deeper paths. Nine compositions execute every direct child without substituted results; three assert every resulting image in3x3,2x2 and1x1 fixtures.','51F780 is exactly RET and consumes no arguments: pushed mode values and incidental volatile register results are excluded.','Original static memory, ordered owned writes, consumed child inputs and saved ABI compared. Private scratch, flags, concurrency, physical pixels and full gameplay excluded.'])
 suffix='diagnostic' if target.stem=='restore_link_check' else 'game';(ROOT/f'metadata/map-surface-{suffix}.json').write_text(json.dumps(proof,indent=2)+'\n')
