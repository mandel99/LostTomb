"""Native/C region image passes, complete owned instructions and ordered stores."""
from verify_options_startup import *
from unicorn import UC_HOOK_MEM_WRITE
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import json
catalogue=json.loads((ROOT/'metadata/map-region-native.json').read_text())
assert catalogue['native_sha256']==hashlib.sha256(original.read_bytes()).hexdigest()
rea=json.loads((ROOT/'metadata/map-region-rea-evidence.json').read_text());evidence={r['evidence_id']:r for r in rea['records']}
names={0x475720:'restore_map_refresh_paved_region',0x47bae0:'restore_map_refresh_channel_region'}
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
calls={0x475670:1,0x47bf50:7,0x47e870:0,0x484be0:4,0x47b920:0,0x51c620:1}
for c,pe in enumerate(images):
 u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(pe.base,(len(pe.data)+4095)&~4095);u.mem_write(pe.base,bytes(pe.data))
 u.mem_map(0x30000000,0x10000);u.mem_map(STOP,4096);u.mem_map(S,0x50000)
 state=dict(collect=False,own=True,writes=[],events=[],pending=set(),returns=set());machines.append((c,u,state))
 def write(uc,access,at,size,value,data):
  if not data['collect'] or not data['own']:return
  data['writes'].append((at,size,value&((1<<(size*8))-1)));cfg=data['cfg'];mutation=cfg.get('mutation');cell=data['first']
  if mutation=='clear_width' and at==0xb7482c+cell*4 and value==0:put(uc,0xbca2d0,8)
  if mutation=='clear_base' and at==0xb7482c+cell*4 and value==0:put(uc,0xbca2d8,cfg.get('base',1000)+1)
  if mutation=='clear_access' and at==0xb7482c+cell*4 and value==0:byte(uc,0xb285cc+cell,0x56);byte(uc,0xb67d1c+cell,0x23)
  if mutation=='image_access' and at==0xb7482c+cell*4:byte(uc,0xb285cc+cell,0x56);byte(uc,0xb67d1c+cell,0x23)
  if mutation=='access_pattern' and at==0xb285cc+cell:put(uc,0xbc9744,9);byte(uc,0xb67d1c+cell,0x15)
  if mutation=='sprite_pattern' and at==0xb67d1c+cell:put(uc,0xbc9744,11)
  if mutation=='initial_bounds' and at==0xbc9760 and len(data['writes'])==1:put(uc,0xbca2d0,1);put(uc,0xbca2d4,1)
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
  if t==0x475670:
   result=cfg.get('neighbor',0)
   if mutation=='neighbor_variation':byte(uc,0xacf85c+args[0],1-cfg.get('variation',0));put(uc,0xf5695c,9000)
   if mutation=='child_bounds':put(uc,0xbca2d0,2);put(uc,0xbca2d4,2);put(uc,0xbca2dc,226)
  if t==0x47bf50 and cfg.get('publish'):
   for row in range(args[4]):
    for col in range(args[3]):put(uc,0xb7482c+(cfg.get('base',1000)+args[2]*228+args[1]+row*228+col)*4,args[5])
  if t==0x47e870 and mutation=='child_road':put(uc,0xaf598c+get(uc,0xbc9768)*4,0x140)
  if t==0x484be0:
   put(uc,0xbc9740,cfg.get('frame',3));put(uc,0xbc9744,cfg.get('pattern',0));put(uc,0xbc9748,cfg.get('offset',5));result=cfg.get('match',1)
   if mutation=='pattern_cursor' and number==1:put(uc,0xbc9768,get(uc,0xbc9768)+1)
   if mutation=='pattern_palette':put(uc,0xf56b84,9000)
  if t==0x47b920:
   result=cfg.get('crossing',0)
   if mutation=='cross_cursor' and number==1:put(uc,0xbc9768,get(uc,0xbc9768)+1);put(uc,0xbc9748,80)
  if t==0x51c620:
   result=cfg.get('mapped_image',args[0]+100)
   if mutation=='overlay_cursor' and number==1:put(uc,0xbc9768,get(uc,0xbc9768)+1)
  uc.reg_write(UC_X86_REG_EAX,result&0xffffffff);uc.reg_write(UC_X86_REG_ESP,sp+4);uc.reg_write(UC_X86_REG_EIP,ret)
 state['entries']={(sym('FUN_%08x'%t) if c else t):t for t in calls}
 for a in state['entries']:u.hook_add(UC_HOOK_CODE,child,state,begin=a,end=a)

def prepare(u,state,fn,cfg):
 u.mem_write(0x560000,seed);u.mem_write(0x30000000,b'\xcc'*0x10000);u.mem_write(S,bytes(0x50000))
 base=cfg.get('base',1000);width=cfg.get('width',6);height=cfg.get('height',5);bounds=cfg.get('bounds',[1,1,4,3]);left,top,right,bottom=bounds
 for a,v in [(0xbca2d0,width),(0xbca2d4,height),(0xbca2d8,base),(0xbca2dc,cfg.get('padding',228-max(width,0))),(0xf5695c,cfg.get('palette',5000)),(0xf56b84,cfg.get('palette',7000)),(0xbca328,777),(0x604950,1)]:put(u,a,v)
 for row in range(10):
  for col in range(12):
   cell=base+row*228+col;flags=cfg.get('terrain',0x40 if fn==0x475720 else 0x100)
   if cfg.get('mixed'):flags=[0,0x40,0x100,0x104,0x108,0x140,0x10100][(row+col)%7]
   image=cfg.get('image',123 if fn==0x475720 else (cfg.get('palette',7000)+cfg.get('old_offset',0))&0xffffffff)
   put(u,0xaf598c+cell*4,flags);put(u,0xb7482c+cell*4,image);byte(u,0xb285cc+cell,cfg.get('access',0xad));byte(u,0xb67d1c+cell,0x82)
   byte(u,0xacf85c+cell,cfg.get('variation',(row+col)%2));byte(u,0xae8e7c+cell,cfg.get('animation',0));word(u,0xb4e6fc+cell*2,cfg.get('building',0))
 if cfg.get('building'):word(u,(0x7999e8+cfg['building']*264)&0xffffffff,cfg.get('kind',1))
 first_x=max(0,min(left,right)-(fn==0x47bae0));first_y=max(0,min(top,bottom)-(fn==0x47bae0));state['first']=base+first_y*228+first_x
 return [left,top,right,bottom,cfg.get('flag',1)]

def exercise(fn=0x475720,**cfg):
 global checked,invocations
 expected=None
 for c,u,state in machines:
  for route in (['native'] if not c else ['named','canonical','frame']):
   state.update(collect=False,own=True,pending=set(),writes=[],events=[],counts={},cfg=cfg,fn=fn)
   args=prepare(u,state,fn,cfg);last[0]=None;state['collect']=True;invocations+=1
   pc=fn if not c else sym(names[fn] if route=='named' else 'FUN_%08x'%fn if route=='canonical' else frames[fn])
   try:
    if route=='frame':
     frame=S+0x1000;vsp=S+0x3000;initial=[0x11111111,0x22222222,0x33333333,saved[0],vsp,*saved[1:]]
     u.mem_write(frame,struct.pack('<10I',*initial,2,0));put(u,vsp,STOP)
     for i,a in enumerate(args):put(u,vsp+4+i*4,a)
     run(0,u,pc,[frame],budget=3000000);out=struct.unpack('<8I',bytes(u.mem_read(frame,32)));assert out[4]==vsp+4;assert [out[i] for i in [3,5,6,7]]==saved
    else:run(0,u,pc,args,budget=3000000)
   except Exception as e:raise AssertionError(('execute',hex(fn),cfg,route,hex(u.reg_read(UC_X86_REG_EIP)))) from e
   state['collect']=False
   if fn==0x47bae0:assert get(u,0xbca328)==0
   if 'expected_origin_image' in cfg:assert get(u,0xb7482c+state['first']*4)==cfg['expected_origin_image'],(cfg,route,get(u,0xb7482c+state['first']*4))
   if 'required_child' in cfg:assert state['counts'].get(cfg['required_child'],0)>0,(cfg,route,state['counts'])
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
  for bounds in [[4,3,1,1],[-3,-2,4,3],[1,1,100,100],[10,10,12,12],[-10,-10,-4,-4],[0,0,0,0]]:exercise(fn,bounds=bounds)
  for width,height in [(0,5),(6,0),(-1,5),(6,-1),(1,1)]:exercise(fn,width=width,height=height)
  for flag in [0,1,256,-1]:exercise(fn,flag=flag)
  for terrain in [0,0x40,0x100,0x104,0x108,0x140,0x10100,0x10140,0xffffffff]:exercise(fn,terrain=terrain)
  exercise(fn,mixed=True)
 for cfg in [dict(flag=0,image=0),dict(image=0,access=0),dict(access=127),dict(building=2,kind=138),dict(building=2,kind=1),dict(building=-1,kind=138),dict(building=-1,kind=1),dict(building=2,kind=138,image=0),dict(mutation='clear_width'),dict(mutation='clear_base'),dict(mutation='clear_access'),dict(mutation='image_access'),dict(mutation='child_bounds')]:exercise(**cfg)
 for neighbor in [0,1,256,-1]:
  for variation in [0,1,2,255]:exercise(neighbor=neighbor,variation=variation)
 exercise(neighbor=1,mutation='neighbor_variation');exercise(neighbor=1,publish=True)
 for offset in [-2147483648,-1,0,47,48,49,2147483647]:exercise(0x47bae0,old_offset=offset,flag=0)
 for animation in [0,15,16,255]:exercise(0x47bae0,animation=animation)
 for pattern in [0,1,256,0xffffffff]:
  for crossing in [0,1,256,-1]:exercise(0x47bae0,terrain=0x140,pattern=pattern,crossing=crossing)
 for cfg in [dict(mutation='child_road'),dict(mutation='pattern_cursor'),dict(mutation='pattern_palette'),dict(terrain=0x140,mutation='cross_cursor'),dict(terrain=0x10100,mutation='overlay_cursor'),dict(terrain=0x10100,mapped_image=0xffffffff),dict(mutation='access_pattern'),dict(mutation='sprite_pattern'),dict(mutation='image_access'),dict(mutation='initial_bounds')]:exercise(0x47bae0,**cfg)
 for cfg in [dict(),dict(flag=0,image=0),dict(building=2,kind=138,expected_origin_image=123),dict(bounds=[0,0,0,0],width=1,height=1,expected_origin_image=5000)]:exercise(actual_children=list(calls),**cfg)
 for cfg in [dict(),dict(terrain=0x140,required_child=0x47b920),dict(terrain=0x10100,required_child=0x51c620),dict(mixed=True)]:exercise(0x47bae0,actual_children=list(calls),**cfg)
 missing=points-hits;missing_edges={(a,b) for a,ends in branches.items() for b in ends if (a,b) not in edges}
 print('Compared map regions:',checked,'scenarios;',invocations,'invocations;',len(points&hits),'/',len(points),'instructions;',len(edges),'conditional outcomes',flush=True)
 print('Unexecuted instructions:',[hex(a) for a in sorted(missing)],'; edges:',[(hex(a),hex(b)) for a,b in sorted(missing_edges)],flush=True)
 assert not missing and not missing_edges,(missing,missing_edges)
 sources=['CMakeLists.txt','include/map_region_repair.h','runtime/map_region_repair.c','runtime/menu_drag_surface_passes.c','runtime/menu_drag_canal_region.c','runtime/menu_drag_paving_pass_model.c','runtime/menu_drag_canal_region_model.c','runtime/menu_drag_paving_neighbors.c','runtime/menu_drag_road_place.c','include/map_refresh_postpass2.h','include/map_refresh_postpass4.h','src/unit_01233.c','src/unit_01110.c','src/unit_01152.c','src/unit_02327.c','direct_c/verify_map_region.py','metadata/map-region-native.json','metadata/map-region-rea-evidence.json']
 proof=dict(native_sha256=catalogue['native_sha256'],compiled_C_path=str(target.resolve()),compiled_C_sha256=target_sha,passed=True,checked=checked,invocations=invocations,coverage={hex(a):dict(instructions=len(p),executed=len(p&hits)) for a,p in owned.items()},conditional_edges=len(edges),unexecuted_instructions=[],unexecuted_edges=[],source_hashes={p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest() for p in sources},controlled_algorithm_endpoints=[hex(t) for t in calls],actual_composition_scenarios=8,limitations=['Children are controlled unless actual_children enables them; complete owned algorithms are verified, not all deeper paths. Eight compositions execute all reached direct children with no substituted results, including special-building preservation, single-tile output, the47B920 branch and the51C620 image transform.','Ordered owned stores, child inputs, full static memory and saved ABI compared. Void-result registers, flags, private scratch, concurrency and physical rendering excluded.'])
 suffix='diagnostic' if target.stem=='restore_link_check' else 'game';(ROOT/f'metadata/map-region-{suffix}.json').write_text(json.dumps(proof,indent=2)+'\n')
