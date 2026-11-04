"""Native/C first-blocker access flood with authenticated192-instruction corpus."""
from verify_options_startup import *
from unicorn import UC_HOOK_MEM_WRITE
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import json
catalogue=json.loads((ROOT/'metadata/access-repair-flood-native.json').read_text())
assert catalogue['native_sha256']==hashlib.sha256(original.read_bytes()).hexdigest()
rea=json.loads((ROOT/'metadata/access-repair-flood-rea-evidence.json').read_text());evidence={r['evidence_id']:r for r in rea['records']}
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
points=set.union(*owned.values());machines=[];delta=[-228,1,228,-1]
for c,pe in enumerate(images):
 u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(pe.base,(len(pe.data)+4095)&~4095);u.mem_write(pe.base,bytes(pe.data))
 u.mem_map(0x30000000,0x10000);u.mem_map(STOP,4096);u.mem_map(S,0x50000)
 state=dict(collect=False,own=True,writes=[],events=[],pending=set(),returns=set());machines.append((c,u,state))
 def write(uc,access,at,size,value,data):
  if not data['collect'] or not data['own']:return
  data['writes'].append((at,size,value&((1<<(size*8))-1)));cfg=data['cfg'];start=data['start']
  if 'wrap' in cfg and not data['mutated'] and at==0xeed760+(start+delta[cfg['wrap']])*4:
   data['mutated']=True;put(uc,0xeae008,51983);put(uc,0xeae00c,51983)
  if cfg.get('reload_start') and at==0xeae004:put(uc,0xeadfd4,1234)
  if cfg.get('end_success') and at==0xeae00c and value==1:put(uc,0xeed760+data['target']*4,7)
 u.hook_add(UC_HOOK_MEM_WRITE,write,state,begin=0x560000,end=0x126cfff)
 if not c:
  def code(uc,pc,size,data):
   if not data['collect'] or not data['own']:return
   hits.add(pc)
   if last[0] in branches:edges.add((last[0],pc))
   last[0]=pc
  u.hook_add(UC_HOOK_CODE,code,state,begin=min(points),end=max(points))
 def returned(uc,pc,size,data):
  if data['collect'] and pc in data['pending']:data['pending'].remove(pc);data['own']=True
 def clear(uc,pc,size,data):
  if not data['collect'] or not data['own']:return
  sp=uc.reg_read(UC_X86_REG_ESP);ret=get(uc,sp);args=tuple(get(uc,sp+4+i*4) for i in range(3));args=(*args[:2],args[2]&255)
  assert args==(0xeed760,0x32c40,0);data['events'].append((0x40f8f0,args))
  if data['cfg'].get('actual_clear'):
   data['own']=False;data['pending'].add(ret)
   if ret not in data['returns']:uc.hook_add(UC_HOOK_CODE,returned,data,begin=ret,end=ret);data['returns'].add(ret)
   return
  uc.mem_write(args[0],bytes(args[1]));uc.reg_write(UC_X86_REG_EAX,0);uc.reg_write(UC_X86_REG_ESP,sp+4);uc.reg_write(UC_X86_REG_EIP,ret)
 a=sym('FUN_0040f8f0') if c else 0x40f8f0;u.hook_add(UC_HOOK_CODE,clear,state,begin=a,end=a)

def prepare(u,state,cfg):
 u.mem_write(0x560000,seed);u.mem_write(0x30000000,b'\xcc'*0x10000);u.mem_write(S,bytes(0x50000))
 u.mem_write(0xee0c50,b'\xff'*51984);u.mem_write(0xaf598c,bytes(51984*4));u.mem_write(0xf203a0,bytes(51984*4));u.mem_write(0xeed760,b'\xa5'*(51984*4))
 base=cfg.get('base',1000);x=cfg.get('x',5);y=cfg.get('y',5);start=(base+y*228+x)&0xffffffff
 tx=cfg.get('tx',-2);ty=cfg.get('ty',-2);target=(base+ty*228+tx)&0xffffffff
 state.update(start=start,target=target,mutated=False);put(u,0xbca2d8,base);put(u,0xeadfd4,cfg.get('serial',7))
 byte(u,0xee0c50+start,0)
 for i,d in enumerate(delta):
  neighbor=start+d;byte(u,0xee0c50+neighbor,cfg.get('cost',0) if cfg.get('open',True) else 255)
  put(u,0xaf598c+neighbor*4,cfg.get('terrain',0) if i==cfg.get('blocker',-1) else 0)
  byte(u,0xb285cc+neighbor,0xad);byte(u,0xb67d1c+neighbor,0x82)
 if cfg.get('region'):
  for row in range(3,10):
   for col in range(3,10):byte(u,0xee0c50+base+row*228+col,0)
 return [x,y,tx,ty]

def exercise(**cfg):
 global checked,invocations
 expected=None
 for c,u,state in machines:
  for route in (['native'] if not c else ['named','canonical','frame']):
   state.update(collect=False,own=True,pending=set(),writes=[],events=[],cfg=cfg);args=prepare(u,state,cfg);last[0]=None;state['collect']=True;invocations+=1
   pc=0x517c20 if not c else sym('restore_access_clear_first_blocker' if route=='named' else 'FUN_00517c20' if route=='canonical' else 'restore_city_C_00517c20')
   try:
    if route=='frame':
     frame=S+0x1000;vsp=S+0x3000;initial=[0x11111111,0x22222222,0x33333333,saved[0],vsp,*saved[1:]]
     u.mem_write(frame,struct.pack('<10I',*initial,2,0));put(u,vsp,STOP)
     for i,a in enumerate(args):put(u,vsp+4+i*4,a)
     run(0,u,pc,[frame],budget=3000000);out=struct.unpack('<8I',bytes(u.mem_read(frame,32)));assert out[4]==vsp+4;assert [out[i] for i in [3,5,6,7]]==saved;result=out[0]
    else:result=run(0,u,pc,args,budget=3000000)
   except Exception as e:raise AssertionError(('execute',cfg,route,hex(u.reg_read(UC_X86_REG_EIP)))) from e
   state['collect']=False;assert len(state['events'])==1
   if 'expected' in cfg:assert result==cfg['expected'],(cfg,route,result)
   answer=(tuple(state['writes']),tuple(state['events']),snapshot(u),result)
   if expected is None:expected=answer
   elif answer!=expected:
    diffs=[next(((i,a,b) for i,(a,b) in enumerate(zip(x,y)) if a!=b),None) for x,y in zip(expected[:2],answer[:2])]
    memory=next(((hex(0x560000+i),a,b) for i,(a,b) in enumerate(zip(expected[2],answer[2])) if a!=b),None)
    raise AssertionError((cfg,route,'stores/events',diffs,'lengths',[(len(x),len(y)) for x,y in zip(expected[:2],answer[:2])],'memory',memory,'result',(expected[3],answer[3])))
 checked+=1
 if checked%25==0:print('progress',checked,'scenarios',flush=True)

if __name__=='__main__':
 exercise()
 if '--quick' in sys.argv:print('PASS quick',checked,invocations);sys.exit(0)
 for cfg in [dict(open=False,expected=0),dict(tx=5,ty=5,expected=1),dict(tx=6,ty=5,expected=1),dict(region=True,tx=9,ty=9,expected=1),dict(serial=0xffffffff),dict(end_success=True,expected=1),dict(reload_start=True),dict(x=-1,y=2),dict(base=1200,tx=0,ty=0)]:exercise(**cfg)
 for direction in range(4):
  for terrain in [0x20,0x100,0x4000,0x4120,0xffffffff,0x80000]:exercise(open=False,blocker=direction,terrain=terrain,expected=0)
  exercise(wrap=direction)
 for cost in [1,127,128,252,255]:exercise(cost=cost)
 for cfg in [dict(),dict(open=False,blocker=0,terrain=0x20),dict(open=False,blocker=1,terrain=0x100),dict(open=False,blocker=2,terrain=0x4000),dict(open=False,blocker=3,terrain=0x4120),dict(region=True,tx=9,ty=9,expected=1)]:exercise(actual_clear=True,**cfg)
 missing=points-hits;missing_edges={(a,b) for a,ends in branches.items() for b in ends if (a,b) not in edges}
 print('Compared repair flood:',checked,'scenarios;',invocations,'invocations;',len(points&hits),'/',len(points),'instructions;',len(edges),'conditional outcomes',flush=True)
 print('Unexecuted instructions:',[hex(a) for a in sorted(missing)],'; edges:',[(hex(a),hex(b)) for a,b in sorted(missing_edges)],flush=True)
 assert not missing and not missing_edges,(missing,missing_edges)
 sources=['CMakeLists.txt','include/access_repair_flood.h','include/city_updates.h','runtime/access_repair_flood.c','src/unit_00119.c','runtime/city_updates.c','runtime/city_updates_model.c','direct_c/verify_access_repair_flood.py','metadata/access-repair-flood-native.json','metadata/access-repair-flood-rea-evidence.json']
 proof=dict(native_sha256=catalogue['native_sha256'],compiled_C_path=str(target.resolve()),compiled_C_sha256=target_sha,passed=True,checked=checked,invocations=invocations,coverage={hex(a):dict(instructions=len(p),executed=len(p&hits)) for a,p in owned.items()},conditional_edges=len(edges),unexecuted_instructions=[],unexecuted_edges=[],source_hashes={p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest() for p in sources},controlled_algorithm_endpoints=['0x40f8f0'],actual_composition_scenarios=6,limitations=['Only the memory-clear child is controlled except six actual-clear cases; the complete owned flood, ring cursors and terrain clearing execute.','Ordered stores, full static memory, full-EAX Boolean, consumed child inputs and saved ABI compared. Private scratch, flags, concurrency and live gameplay excluded.'])
 suffix='diagnostic' if target.stem=='restore_link_check' else 'game';(ROOT/f'metadata/access-repair-flood-{suffix}.json').write_text(json.dumps(proof,indent=2)+'\n')
