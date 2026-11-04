"""Three complete occupancy-query algorithms; full native EAX and RET widths."""
from verify_options_startup import *
from unicorn import UC_HOOK_MEM_WRITE
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import json
catalogue=json.loads((ROOT/'metadata/building-occupancy-native.json').read_text())
assert catalogue['native_sha256']==hashlib.sha256(original.read_bytes()).hexdigest()
rea=json.loads((ROOT/'metadata/building-occupancy-rea-evidence.json').read_text());evidence={r['evidence_id']:r for r in rea['records']}
names={0x4893c0:'restore_region_has_blocking_tile',0x489470:'restore_region_has_active_occupant',0x4894a0:'restore_building_has_active_occupant'}
frames={a:'restore_actor_message_C_%08x'%a for a in names}
cleanup={0x4893c0:0,0x489470:16,0x4894a0:4}
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
for c,pe in enumerate(images):
 u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(pe.base,(len(pe.data)+4095)&~4095);u.mem_write(pe.base,bytes(pe.data))
 u.mem_map(0x30000000,0x10000);u.mem_map(STOP,4096);u.mem_map(S,0x50000)
 state=dict(collect=False,writes=[]);machines.append((c,u,state))
 def write(uc,access,at,size,value,data):
  if not data['collect']:return
  data['writes'].append((at,size,value&((1<<(size*8))-1)))
  if data['cfg'].get('mutation')=='origin' and at==0xbc9768:put(uc,0xbca2d8,1234)
  if data['cfg'].get('mutation')=='predicate' and at==0xbc9768:put(uc,0xaf598c+value*4,0x10000)
 u.hook_add(UC_HOOK_MEM_WRITE,write,state,begin=0x560000,end=0x126cfff)
 if not c:
  def code(uc,pc,size,data):
   if not data['collect']:return
   hits.add(pc)
   if last[0] in branches:edges.add((last[0],pc))
   last[0]=pc
  for p in owned.values():u.hook_add(UC_HOOK_CODE,code,state,begin=min(p),end=max(p))

def prepare(u,state,fn,cfg):
 u.mem_write(0x560000,seed);u.mem_write(0x30000000,b'\xcc'*0x10000);u.mem_write(S,bytes(0x50000));u.mem_write(0xaf598c,bytes(51984*4));u.mem_write(0xd05720,bytes(51984*4))
 base=cfg.get('base',1000);x=cfg.get('x',10);y=cfg.get('y',12);put(u,0xbca2d8,base)
 word(u,0x7999de+264,x);word(u,0x7999e0+264,y)
 for row in range(6):
  for col in range(6):
   i=row*6+col;tile=(base+y*228+x+get(u,0x5cc8c0+row*48+col*8))&0xffffffff
   if i==cfg.get('terrain_index',-1):put(u,0xaf598c+tile*4,cfg.get('terrain',0x10000))
   if i==cfg.get('object_index',-1):put(u,0xd05720+tile*4,S+0x10000);byte(u,S+0x10018,cfg.get('object_state',0))
 if fn==0x4894a0:return [1]
 args=[x,y,cfg.get('width',3),cfg.get('height',3)]
 if fn==0x4893c0:args.append(cfg.get('mask',0x10000))
 return args

def exercise(fn=0x4893c0,**cfg):
 global checked,invocations
 expected=None
 for c,u,state in machines:
  for route in (['native'] if not c else ['named','canonical','frame',*(['compat'] if fn==0x4894a0 else [])]):
   state.update(collect=False,writes=[],cfg=cfg);args=prepare(u,state,fn,cfg);last[0]=None;state['collect']=True;invocations+=1
   pc=fn if not c else sym(names[fn] if route=='named' else 'FUN_%08x'%fn if route=='canonical' else frames[fn] if route=='frame' else 'restore_actor_building_query')
   try:
    if route=='frame':
     frame=S+0x1000;vsp=S+0x3000;initial=[0x11111111,0x22222222,0x33333333,saved[0],vsp,*saved[1:]]
     u.mem_write(frame,struct.pack('<10I',*initial,2,0));put(u,vsp,STOP)
     for i,a in enumerate(args):put(u,vsp+4+i*4,a)
     run(0,u,pc,[frame],budget=3000000);out=struct.unpack('<8I',bytes(u.mem_read(frame,32)));assert out[4]==vsp+4+cleanup[fn];assert [out[i] for i in [3,5,6,7]]==saved;result=out[0]
    else:result=run(0,u,pc,args,cleanup=cleanup[fn] if not c else 0,budget=3000000,receiver=cfg.get('receiver',0xdeadbeef))
   except Exception as e:raise AssertionError(('execute',hex(fn),cfg,route,hex(u.reg_read(UC_X86_REG_EIP)))) from e
   state['collect']=False;answer=(tuple(state['writes']),snapshot(u),result if route!='compat' else result&255)
   if 'expected' in cfg:assert result&255==cfg['expected'],(cfg,route,hex(result))
   if expected is None:expected=answer
   else:
    want=expected if route!='compat' else (*expected[:2],expected[2]&255)
    if answer!=want:
     memory=next(((hex(0x560000+i),a,b) for i,(a,b) in enumerate(zip(want[1],answer[1])) if a!=b),None)
     raise AssertionError((hex(fn),cfg,route,'writes',(want[0],answer[0]),'memory',memory,'EAX',(hex(want[2]),hex(answer[2]))))
 checked+=1
 if checked%25==0:print('progress',checked,'scenarios',flush=True)

if __name__=='__main__':
 for fn in names:exercise(fn)
 if '--quick' in sys.argv:print('PASS quick',checked,invocations);sys.exit(0)
 for fn in names:
  for index in [0,1,2,6,7,8,12,13,14]:
   for state in [0,1,255]:exercise(fn,object_index=index,object_state=state,expected=state==0)
  for index in [0,7,14]:exercise(fn,terrain_index=index,expected=1)
  for x,y in [(-1,1),(32767,-143),(-32768,144)]:exercise(fn,x=x,y=y,object_index=0,expected=1)
  for cfg in [dict(mutation='origin'),dict(mutation='predicate',expected=1),dict(receiver=0),dict(receiver=0xffffffff)]:exercise(fn,**cfg)
 for fn in [0x4893c0,0x489470]:
  for width,height in [(0,0),(-1,3),(3,-1),(3,-2147483648),(0,257),(1,1),(6,6)]:exercise(fn,width=width,height=height)
 for mask in [0,1,64,0x10000,0x10040,0xffffffff]:
  for cfg in [dict(object_index=0,object_state=0),dict(terrain_index=0,terrain=64),dict(terrain_index=0,terrain=0x10000)]:exercise(mask=mask,**cfg)
 for index in range(36):exercise(width=6,height=6,terrain_index=index,terrain=64,mask=64,expected=1)
 missing=points-hits;missing_edges={(a,b) for a,ends in branches.items() for b in ends if (a,b) not in edges}
 print('Compared occupancy:',checked,'scenarios;',invocations,'invocations;',len(points&hits),'/',len(points),'instructions;',len(edges),'conditional outcomes',flush=True)
 print('Unexecuted instructions:',[hex(a) for a in sorted(missing)],'; edges:',[(hex(a),hex(b)) for a,b in sorted(missing_edges)],flush=True)
 assert not missing and not missing_edges,(missing,missing_edges)
 sources=['CMakeLists.txt','include/building_occupancy_query.h','runtime/building_occupancy_query.c','runtime/menu_actor_message.c','runtime/menu_drag_actor_teardown.c','src/unit_01295.c','src/unit_01296.c','src/unit_01297.c','direct_c/verify_building_occupancy.py','metadata/building-occupancy-native.json','metadata/building-occupancy-rea-evidence.json']
 proof=dict(native_sha256=catalogue['native_sha256'],compiled_C_path=str(target.resolve()),compiled_C_sha256=target_sha,passed=True,checked=checked,invocations=invocations,coverage={hex(a):dict(instructions=len(p),executed=len(p&hits)) for a,p in owned.items()},conditional_edges=len(edges),unexecuted_instructions=[],unexecuted_edges=[],source_hashes={p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest() for p in sources},controlled_algorithm_endpoints=[],limitations=['All three actual query algorithms execute with no substituted children; full-EAX results compared except the explicitly8-bit compatibility facade.','Saved registers, native RET4/RET16 through frames, C cdecl aliases, cursor store and full static data compared. Private scratch, flags, concurrency and live UI excluded.'])
 suffix='diagnostic' if target.stem=='restore_link_check' else 'game';(ROOT/f'metadata/building-occupancy-{suffix}.json').write_text(json.dumps(proof,indent=2)+'\n')
