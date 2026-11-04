"""Native/C processing-kind, permission and occupied-building parent queries."""
from verify_options_startup import *
from unicorn import UC_HOOK_MEM_WRITE
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import json
catalogue=json.loads((ROOT/'metadata/building-processing-native.json').read_text())
assert catalogue['native_sha256']==hashlib.sha256(original.read_bytes()).hexdigest()
rea=json.loads((ROOT/'metadata/building-processing-rea-evidence.json').read_text());evidence={r['evidence_id']:r for r in rea['records']}
names={0x4bf9d0:'restore_city_has_unpermitted_occupied_building',0x459240:'restore_building_resource_is_permitted',0x488030:'restore_building_is_processing_type',0x487f80:'restore_kind_is_processing_type'}
frames={0x4bf9d0:'restore_actor_message_C_004bf9d0',0x459240:'restore_actor_message_C_00459240',0x488030:'restore_building_refresh_dispatch'}
cleanup={0x4bf9d0:0,0x459240:0,0x488030:4,0x487f80:4}
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
calls={0x488030:1,0x4894a0:1,0x459240:1,0x487f80:1}
for c,pe in enumerate(images):
 u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(pe.base,(len(pe.data)+4095)&~4095);u.mem_write(pe.base,bytes(pe.data))
 u.mem_map(0x30000000,0x10000);u.mem_map(STOP,4096);u.mem_map(S,0x50000)
 state=dict(collect=False,own=True,writes=[],events=[],pending=set(),returns=set());machines.append((c,u,state))
 def write(uc,access,at,size,value,data):
  if data['collect'] and data['own']:data['writes'].append((at,size,value&((1<<(size*8))-1)))
 u.hook_add(UC_HOOK_MEM_WRITE,write,state,begin=0x560000,end=0x126cfff)
 if not c:
  def code(uc,pc,size,data):
   if data['collect'] and pc in data['pending']:data['own']=True
   if not data['collect'] or not data['own']:return
   hits.add(pc)
   if last[0] in branches:edges.add((last[0],pc))
   if last[0] in switches:jump_edges.add((last[0],pc))
   last[0]=pc
  for p in owned.values():u.hook_add(UC_HOOK_CODE,code,state,begin=min(p),end=max(p))
 def returned(uc,pc,size,data):
  if data['collect'] and pc in data['pending']:data['pending'].remove(pc);data['own']=True
 def child(uc,pc,size,data):
  if not data['collect'] or not data['own']:return
  t=data['entries'][pc]
  if t==data['fn']:return
  sp=uc.reg_read(UC_X86_REG_ESP);ret=get(uc,sp);cfg=data['cfg'];args=(get(uc,sp+4),)
  data['events'].append((t,args));data['counts'][t]=data['counts'].get(t,0)+1;number=data['counts'][t]
  if cfg.get('actual') or t==0x487f80:
   data['own']=False;data['pending'].add(ret)
   if ret not in data['returns']:uc.hook_add(UC_HOOK_CODE,returned,data,begin=ret,end=ret);data['returns'].add(ret)
   return
  result=cfg.get('class_result',1) if t==0x488030 else cfg.get('occupied_result',1) if t==0x4894a0 else cfg.get('permission_result',0)
  mutation=cfg.get('mutation');record=args[0]*264
  if t==0x488030:
   if mutation=='owner_resource':put(uc,0xeaac28,3);byte(uc,0x799a1b+record,255)
   if mutation=='kind':word(uc,0x7999e8+record,cfg.get('reload_kind',108))
   if mutation=='status':byte(uc,0x7999d8+record,0)
   if mutation=='mode':put(uc,cfg.get('self',0xd05720)+0x32fb4,3)
  if t==0x4894a0 and mutation=='assigned':word(uc,0x799a8c+record,0xffff)
  if t==0x459240 and mutation=='next_status':byte(uc,0x7999d8+(args[0]+1)*264,0)
  if t==0x459240 and cfg.get('permission_sequence'):result=cfg['permission_sequence'][(number-1)%len(cfg['permission_sequence'])]
  uc.reg_write(UC_X86_REG_EAX,result&0xffffffff);uc.reg_write(UC_X86_REG_ESP,sp+4+(cleanup.get(t,4) if not data['compiled'] else 0));uc.reg_write(UC_X86_REG_EIP,ret)
 state['compiled']=c;state['entries']={(sym('FUN_%08x'%t) if c else t):t for t in calls}
 for a in state['entries']:u.hook_add(UC_HOOK_CODE,child,state,begin=a,end=a)

def prepare(u,fn,cfg):
 u.mem_write(0x560000,seed);u.mem_write(0x30000000,b'\xcc'*0x10000);u.mem_write(S,bytes(0x50000));u.mem_write(0x7999d8,bytes(4000*264));u.mem_write(0xaf598c,bytes(51984*4));u.mem_write(0xd05720,bytes(51984*4))
 put(u,0xeaac28,cfg.get('owner',0));put(u,0xbca2d8,1000);put(u,0x78d790,cfg.get('bound',0));put(u,cfg.get('self',0xd05720)+0x32fb4,cfg.get('mode',0))
 for p in [0,1,3]:
  for resource in range(256):word(u,0xea4126+p*0x49d8+resource*2,cfg.get('permission_word',0))
 candidates=cfg.get('candidates',[1])
 for building in candidates:
  r=building*264;x=10+building%5;y=12
  byte(u,0x7999d8+r,cfg.get('status',1));word(u,0x7999e8+r,cfg.get('kind',100));byte(u,0x799a1b+r,cfg.get('resource',7));word(u,0x799a8c+r,cfg.get('assigned',0));word(u,0x7999de+r,x);word(u,0x7999e0+r,y)
  cell=1000+y*228+x
  if cfg.get('occupied',True):put(u,0xaf598c+cell*4,0x10000)
 if fn==0x487f80:return [cfg.get('kind',100)]
 if fn==0x4bf9d0:return []
 return [cfg.get('building',1)]

def exercise(fn=0x4bf9d0,**cfg):
 global checked,invocations
 expected=None
 for c,u,state in machines:
  routes=['native'] if not c else ['named','canonical',*(['frame'] if fn in frames else [])]
  for route in routes:
   state.update(collect=False,own=True,pending=set(),writes=[],events=[],counts={},cfg=cfg,fn=fn);args=prepare(u,fn,cfg);last[0]=None;state['collect']=True;invocations+=1
   pc=fn if not c else sym(names[fn] if route=='named' else 'FUN_%08x'%fn if route=='canonical' else frames[fn])
   try:
    if route=='frame':
     frame=S+0x1000;vsp=S+0x3000;initial=[0x11111111,cfg.get('self',0xd05720),0x33333333,saved[0],vsp,*saved[1:]]
     u.mem_write(frame,struct.pack('<10I',*initial,2,0));put(u,vsp,STOP)
     for i,a in enumerate(args):put(u,vsp+4+i*4,a)
     run(0,u,pc,[0,fn,frame] if fn==0x488030 else [frame],budget=3000000);out=struct.unpack('<8I',bytes(u.mem_read(frame,32)));assert out[4]==vsp+4+cleanup[fn];assert [out[i] for i in [3,5,6,7]]==saved;result=out[0]
    else:
     callargs=([cfg.get('self',0xd05720)] if c and fn==0x4bf9d0 else args)
     result=run(0,u,pc,callargs,cleanup=cleanup[fn] if not c else 0,receiver=cfg.get('self',0xd05720),budget=3000000)
   except Exception as e:raise AssertionError(('execute',hex(fn),cfg,route,hex(u.reg_read(UC_X86_REG_EIP)))) from e
   state['collect']=False
   if 'expected' in cfg:assert result==cfg['expected'],(cfg,route,hex(result))
   answer=(tuple(state['writes']),tuple(state['events']),snapshot(u),result)
   if expected is None:expected=answer
   elif answer!=expected:
    diffs=[next(((i,a,b) for i,(a,b) in enumerate(zip(x,y)) if a!=b),None) for x,y in zip(expected[:2],answer[:2])]
    memory=next(((hex(0x560000+i),a,b) for i,(a,b) in enumerate(zip(expected[2],answer[2])) if a!=b),None)
    raise AssertionError((hex(fn),cfg,route,'stores/events',diffs,'lengths',[(len(x),len(y)) for x,y in zip(expected[:2],answer[:2])],'memory',memory,'EAX',(hex(expected[3]),hex(answer[3]))))
 checked+=1
 if checked%25==0:print('progress',checked,'scenarios',flush=True)

if __name__=='__main__':
 for fn in names:exercise(fn)
 if '--quick' in sys.argv:print('PASS quick',checked,invocations);sys.exit(0)
 for kind in list(range(90,235))+[-2147483648,-32768,-1,0,32767,65535,2147483647]:exercise(0x487f80,kind=kind)
 for kind in [-32768,-1,0,99,100,105,106,108,114,115,196,224,32767,65535]:exercise(0x488030,kind=kind)
 for kind in [0,100,105,106,107,108,109,114,115,196,224,233,234,-1]:
  for word_value in [0,1,65535]:exercise(0x459240,kind=kind,permission_word=word_value)
 for cfg in [dict(class_result=0),dict(class_result=256),dict(mutation='kind',reload_kind=109),dict(mutation='owner_resource',permission_word=1),dict(owner=1,resource=255,permission_word=0x8000),dict(owner=3,resource=0,permission_word=1),dict(owner=0x80000000,permission_word=1)]:exercise(0x459240,**cfg)
 for mode in [0,1,2,3,4,256,0x10001,0xffffffff]:exercise(mode=mode,candidates=[])
 for cfg in [dict(candidates=[]),dict(candidates=[3999]),dict(candidates=[3999],bound=-1),dict(candidates=[1,2,3],permission_sequence=[1,1,0]),dict(status=0),dict(status=3),dict(class_result=0),dict(class_result=256),dict(class_result=0xdeadbe01),dict(occupied_result=0),dict(occupied_result=256),dict(occupied_result=0xbeef0001),dict(assigned=1),dict(assigned=65535),dict(permission_result=1),dict(permission_result=256),dict(permission_result=0x10000),dict(mutation='assigned'),dict(mutation='status'),dict(mutation='mode'),dict(candidates=[1,2],mutation='next_status',permission_sequence=[1]),dict(self=S+0x10000)]:exercise(**cfg)
 for cfg in [dict(),dict(kind=1),dict(occupied=False),dict(assigned=1),dict(permission_word=1),dict(candidates=[3999]),dict(candidates=[1,2,3],permission_word=1)]:exercise(actual=True,**cfg)
 for cfg in [dict(kind=100),dict(kind=1),dict(kind=108),dict(owner=3,resource=255,permission_word=1)]:exercise(0x459240,actual=True,**cfg)
 missing=points-hits;missing_edges={(a,b) for a,ends in branches.items() for b in ends if (a,b) not in edges};switch_missing={(a,b) for a,ends in switches.items() for b in ends if (a,b) not in jump_edges}
 cold={(0x459263,0x459267),(0x459298,0x4592c2)}
 print('Compared processing queries:',checked,'scenarios;',invocations,'invocations;',len(points&hits),'/',len(points),'instructions;',len(edges),'conditional outcomes;',len(jump_edges),'switch targets',flush=True)
 print('Unexecuted instructions:',[hex(a) for a in sorted(missing)],'; edges:',[(hex(a),hex(b)) for a,b in sorted(missing_edges)],flush=True)
 assert not missing and missing_edges==cold and not switch_missing,(missing,missing_edges,switch_missing)
 sources=['CMakeLists.txt','include/building_processing_query.h','runtime/building_processing_query.c','runtime/menu_building_kind.c','runtime/menu_actor_message.c','runtime/building_occupancy_query.c','src/unit_00846.c','src/unit_01275.c','src/unit_01572.c','direct_c/verify_building_processing.py','metadata/building-processing-native.json','metadata/building-processing-rea-evidence.json']
 proof=dict(native_sha256=catalogue['native_sha256'],compiled_C_path=str(target.resolve()),compiled_C_sha256=target_sha,passed=True,checked=checked,invocations=invocations,coverage={hex(a):dict(instructions=len(p),executed=len(p&hits)) for a,p in owned.items()},conditional_edges=len(edges),switch_targets=len(jump_edges),unexecuted_instructions=[],unexecuted_edges=[],analytically_unreachable_edges=[[hex(a),hex(b)] for a,b in sorted(cold)],source_hashes={p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest() for p in sources},controlled_algorithm_endpoints=[hex(t) for t in calls if t!=0x487f80],actual_composition_scenarios=11,limitations=['Controller children are controlled unless actual=True; the487F80 leaf always executes. Eleven parent/permission compositions execute actual reached children without substitutions. Complete owned functions and feasible branches are verified, not all deeper child instructions.','459263 JG cannot take its target after signed kind<106;459298 JNZ cannot take its target because both initial signed comparisons force BL0, including after the child. Both switch targets converge on the same permission lookup.','Full-EAX results, ordered owned stores, consumed child arguments, full static memory, saved ABI, native cleanup through frames and explicit C receivers compared. Private scratch, flags, concurrency and live gameplay excluded.'])
 suffix='diagnostic' if target.stem=='restore_link_check' else 'game';(ROOT/f'metadata/building-processing-{suffix}.json').write_text(json.dumps(proof,indent=2)+'\n')
