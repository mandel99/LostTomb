"""Offline original/C migration allocation, figures and monthly coordination.

Execute actual children; compare ordered owned stores, child inputs, all original
static data and the saved ABI. Actual allocation, cohort, demand, cleanup and event children execute;
no child results are replaced and no host game is launched.
"""
from verify_options_startup import *
from unicorn import UC_HOOK_MEM_WRITE
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import json
catalogue=json.loads((ROOT/'metadata/migration-flow-native.json').read_text())
assert catalogue['native_sha256']==hashlib.sha256(original.read_bytes()).hexdigest()
names={0x458540:'restore_migration_update',0x458950:'restore_migration_allocate_immigrants',0x458b60:'restore_migration_allocate_emigrants',0x458d00:'restore_migration_create_immigrants',0x458d80:'restore_migration_create_emigrants'}
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
calls={0x4e8f80:1,0x458950:1,0x458b60:1,0x458d00:2,0x458d80:2,0x4b4800:5,0x4e90f0:2,0x46cf40:1,0x4e8650:0,0x4e8720:0,0x4e87a0:1,0x458880:0,0x4e0cf0:5}
for c,pe in enumerate(images):
 u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(pe.base,(len(pe.data)+4095)&~4095);u.mem_write(pe.base,bytes(pe.data))
 u.mem_map(0x30000000,0x10000);u.mem_map(STOP,4096);u.mem_map(S,0x50000)
 state=dict(collect=False,own=True,writes=[],events=[],pending=set(),returns=set(),compiled=c,last_entry=None);machines.append((c,u,state))
 def write(uc,access,at,size,value,data):
  if not data['collect']:return
  cfg=data['cfg'];mutation=data['mutation'];owner=cfg.get('owner',0)*0x49d8
  if 'rate' in cfg and not data['own'] and at==0xea3f5c+owner and data['top']==0x458540:put(uc,0xea3f9c+owner,cfg['rate'])
  if not data['own']:return
  data['writes'].append((at,size,value&((1<<(size*8))-1)))
  if mutation=='palette' and at==0x799a06+264:byte(uc,0x799a25+264,255)
  if mutation=='cleanup_count' and at==0x799a06+264 and not value:put(uc,0x78d83c,1)
  if mutation in ['third_count','emigration_count'] and at==0x78d840 and not value:put(uc,0x78d83c,0 if mutation=='third_count' else 3)
  if mutation=='reset_owner' and at==0xea3f90+owner and not value:put(uc,0xeaac28,1)
  if mutation=='notification_owner' and at==0xe951c5:put(uc,0xeaac28,3)
  if mutation=='notification_population' and at==0xe951c5:put(uc,0xea1894+owner,500)
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
  if t==0x4b4800:args=(args[0]&255,*args[1:4],args[4]&255)
  if t==0x458d00:args=(args[0],args[1]&255)
  data['events'].append((t,args))
  data['counts'][t]=data['counts'].get(t,0)+1
  mutation=data['mutation'];cfg=data['cfg']
  if t==0x4b4800 and data['counts'][t]==1:
   if mutation=='cursor':put(uc,0x78d840,2)
   if mutation=='bound':put(uc,0x78d83c,1)
   if mutation=='allocator_owner':put(uc,0xeaac28,3);word(uc,0xea3fe8,17);word(uc,0xea3fea,18)
  if t==0x4e90f0 and mutation=='depart_population':put(uc,0xeaac28,3);word(uc,0x7999f4+264,5)
  if t==0x46cf40 and mutation=='cleanup_owner':put(uc,0xeaac28,3);word(uc,0x7999de+264,14);word(uc,0x7999e0+264,12)
  if t==0x4e8f80 and mutation=='demand_owner':put(uc,0xeaac28,1)
  if t==0x4e0cf0 and mutation=='notification_owner' and data['counts'][t]==1:put(uc,0xeaac28,1)
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
 u.mem_write(0x560000,seed);u.mem_write(0x30000000,b'\xcc'*0x10000);u.mem_write(S,bytes(0x50000))
 u.mem_write(0x7999d8,bytes(4000*264));u.mem_write(0xbcd5f8,bytes(2000*388));u.mem_write(0xeaaf68,bytes(0xeadb28-0xeaaf68));u.mem_write(0x9784d8,bytes(50*44));u.mem_write(0xb350dc,bytes(51984*2))
 owner=cfg.get('owner',0);v(0xeaac28,owner);v(0xbca2d8,0);v(0xbca2d0,228);v(0xbca2d4,228);v(0xbcd5f0,0);v(0xd056fc,123);v(0xd056e0,127);v(0x604950,cfg.get('random',1));v(0xf5653c,1000)
 nodes=cfg.get('nodes',[1,2,3]);v(0x78d83c,cfg.get('count',len(nodes)));v(0x78d840,99);v(0x78d790,max(nodes) if nodes else 0);v(0x78d78c,0)
 phases=cfg.get('phases',[cfg.get('phase',0)]*len(nodes));populations=cfg.get('populations',[cfg.get('population_word',6)]*len(nodes));spaces=cfg.get('spaces',[cfg.get('space',10)]*len(nodes))
 for i,bid in enumerate(nodes):
  x=10+bid*2;y=11;v(0x78e178+i*2,bid,2)
  record(bid,[(0x7999d8,1,1),(0x7999dd,1,1),(0x7999dc,cfg.get('large',0),1),(0x7999db,2 if cfg.get('large',0) else 1,1),
   (0x7999e8,10+phases[i],2),(0x7999ea,phases[i],2),(0x7999f4,populations[i],2),(0x7999f6,spaces[i],2),(0x7999f8,cfg.get('access',1),2),
   (0x7999de,x,2),(0x7999e0,y,2),(0x7999e4,x+y*228,4),(0x799a06,cfg.get('pending',0),2),(0x799a25,cfg.get('appearance',3),1)])
  v(0x70ce14+phases[i]*88,200)
 if cfg.get('full_pool'):
  for i in range(1,2000):v(0xbcd606+i*388,1,1)
 if cfg.get('valid_pending'):
  pending=cfg.get('pending',5);v(0xbcd606+pending*388,1,1)
 for p in {0,1,3,owner}:
  off=p*0x49d8
  for i,x in enumerate(cfg.get('ages',[1]*100)):v(0xea3e34+off+i*2,x,2)
  for a,x in [(0xea1880,cfg.get('sentiment',45)),(0xea1888,50),(0xea1894,cfg.get('population',1000)),(0xea5ce8,0),
   (0xea5eb4,0),(0xea5ea4,0),(0xea3f9c,0),(0xea3fa4,0),(0xea3fa8,0),(0xea3fb0,0),
   (0xea3f64,cfg.get('small_in',0)),(0xea3f60,cfg.get('small_out',0)),(0xea3f58,0),(0xea3f5c,0),
   (0xea3f90,cfg.get('prior_in',7)),(0xea3f94,cfg.get('prior_out',9)),(0xea3fac,20),(0xea3f98,11),(0xea18a8,200),
   (0xea3f84,cfg.get('annual',0)),(0xea3f80,4),(0xea3f74,1000),(0xea3f88,0),(0xea3f8c,0),
   (0xea3fe8,12+p),(0xea3fea,11+p),(0xea5c80,2)]:v(a+off,x)
  for i in range(10):v(0xea5f1c+off+i*8,i+1)
 for i in range(100):v(0xd3b1c8+i*4,i)
 v(0xd3b1c4,0)
 if cfg.get('mutation')=='notification_owner':v(0xea1894+0x49d8,25000);v(0xea1894+3*0x49d8,0)
 v(0xe38dda,0,1);v(0xe38ec1,1,1);v(0x605a0c,1);v(0x605a3c,0);v(0xe38ebc,1);v(0xe38ea8,3,1);v(0xea624a,1,1)
 for i in range(5):v(0xe94e2e+i*120,1,2)
 free=cfg.get('queue_free',0)
 for i in range(1000):v(0xe95232+i*48,0 if i>=1000-free else 1,2)
 v(0xe951ec,1000-free);v(0xe951f0,1001-free);v(0xf55b88,0)
 for i in range(9):v(0xe951c5+i,cfg.get('milestones',0),1)
 if fn in [0x458950,0x458b60]:return [cfg.get('requested',6)]
 if fn in [0x458d00,0x458d80]:return [cfg.get('building',1),cfg.get('people',4)]
 return []

def exercise(fn=0x458950,**cfg):
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
 exercise(0x458540,sentiment=75)
 exercise(0x458540,sentiment=0)
 exercise(0x458540,annual=1)
 exercise(0x458540,population=25000)
 exercise(0x458d80,people=6)
 exercise(mutation='cursor')
 exercise(0x458d00,mutation='palette')
 if not quick:
  for fn in [0x458950,0x458b60]:
   for requested in [-1,0,1,3,4,5,8,9,20,300]:exercise(fn,requested=requested)
   for cfg in [dict(count=0),dict(count=-1),dict(nodes=[]),dict(owner=1),dict(owner=3),dict(population_word=0),dict(population_word=-1),dict(access=0),dict(access=-1),dict(pending=5),dict(pending=5,valid_pending=True),dict(full_pool=True)]:exercise(fn,**cfg)
  for cfg in [dict(space=0),dict(space=-1),dict(space=1),dict(space=7),dict(space=8),dict(space=32767,requested=300),dict(mutation='bound'),dict(mutation='cleanup_count',pending=5),dict(mutation='third_count',space=3),dict(mutation='allocator_owner'),dict(prior_in=2147483647)]:exercise(**cfg)
  for cfg in [dict(phases=[0,1,9]),dict(phase=10),dict(phase=-1),dict(population_word=1),dict(population_word=4),dict(population_word=5),dict(mutation='emigration_count',count=0),dict(mutation='bound'),dict(mutation='cursor'),dict(mutation='depart_population')]:exercise(0x458b60,**cfg)
  for people in [0,1,127,128,255,256,300,-1]:exercise(0x458d00,people=people)
  for appearance in [0,127,128,255]:exercise(0x458d00,appearance=appearance)
  for cfg in [dict(full_pool=True),dict(mutation='allocator_owner'),dict(building=0)]:exercise(0x458d00,**cfg)
  for people in [-1,0,1,5,6,7,256,300]:exercise(0x458d80,people=people)
  for cfg in [dict(full_pool=True),dict(mutation='depart_population'),dict(mutation='cleanup_owner',people=6),dict(large=1,people=6)]:exercise(0x458d80,**cfg)
  for sentiment in [0,25,35,45,55,65,75]:exercise(0x458540,sentiment=sentiment)
  for rate in [9,17,25,-9,-17,-25]:
   for accumulated in [0,1,3,2147483647]:exercise(0x458540,rate=rate,**({'small_in':accumulated} if rate>0 else {'small_out':accumulated}))
  for population in [499,500,999,1000,1999,2000,2999,3000,4999,5000,9999,10000,14999,15000,19999,20000,24999,25000]:exercise(0x458540,population=population)
  for cfg in [dict(population=25000,milestones=1),dict(population=25000,milestones=255),dict(mutation='notification_owner'),dict(population=25000,mutation='notification_population'),dict(mutation='demand_owner'),dict(mutation='reset_owner'),dict(annual=1,ages=[0]*100)]:exercise(0x458540,**cfg)
  for population,free in [(500,1000),(25000,1000),(25000,1)]:
   exercise(0x458540,population=population,queue_free=free)
   for _,u,_ in machines:
    assert get(u,0xe951ec)==(1 if population==500 else 9) if free==1000 else get(u,0xe951ec)==1000
 missing=points-hits;missing_edges={(a,b) for a,ends in branches.items() for b in ends if (a,b) not in edges}
 cold={a for a in owned[0x458950] if 0x4589c4<=a<0x458a2c}
 cold_edges={(a,b) for a,ends in branches.items() if a in cold for b in ends}|{(0x4589be,0x458a2c),(0x4589c2,0x4589c4)}
 print('Compared migration flow:',checked,'scenarios;',invocations,'invocations;',len(points&hits),'/',len(points),'instructions,',len(edges),'conditional outcomes',flush=True)
 if quick:sys.exit(0)
 print('Unexecuted instructions:',[hex(a) for a in sorted(missing)],'; edges:',[(hex(a),hex(b)) for a,b in sorted(missing_edges)],flush=True)
 assert not missing-cold,('instruction coverage',missing-cold)
 assert not missing_edges-cold_edges,('edge coverage',missing_edges-cold_edges)
 sources=['CMakeLists.txt','include/migration_flow.h','runtime/migration_flow.c','runtime/city_updates.c','runtime/city_updates_model.c','src/unit_00835.c','src/unit_00836.c','src/unit_00838.c','src/unit_00839.c','direct_c/verify_migration_flow.py','metadata/migration-flow-native.json']
 proof=dict(native_sha256=catalogue['native_sha256'],compiled_C_path=str(target.resolve()),compiled_C_sha256=target_sha,passed=True,checked=checked,invocations=invocations,
  coverage={hex(a):dict(instructions=len(p),executed=len(p&hits)) for a,p in owned.items()},conditional_edges=len(edges),unexecuted_instructions=[hex(a) for a in sorted(missing-cold)],unexecuted_edges=[[hex(a),hex(b)] for a,b in sorted(missing_edges-cold_edges)],
  analytically_unreachable_instructions=[hex(a) for a in sorted(cold)],analytically_unreachable_edges=[[hex(a),hex(b)] for a,b in sorted(cold_edges)],
  source_hashes={p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest() for p in sources},commit=None,controlled_algorithm_endpoints=[],controlled_platform_endpoints=[],
  limitations=['Rea authenticates every native instruction. Positive immigration requests exhaust cleanup and jump past the empty-house block; nonpositive requests fail its remaining gate. That block and two gate outcomes are analytically unreachable.',
   'Actual actor allocation/RNG/linking, population updates, house cleanup, demographic/demand and event providers execute. Valid full, empty and last-slot notification queues are compared with sound disabled and the UI inactive, including actual enqueue counters. Demand-rate input changes happen before the actual demand provider consumes the rate; no results are replaced.',
   'Named/canonical/five frame adapters; ordered stores, consumed helper inputs, all final original static data and returning-call saved ABI compared. BYTE actor-owner/direction and immigrant payload inputs are normalized to their consumed widths.',
   'Void returns exclude incidental caller-saved registers/flags. Private scratch, concurrent access and host game/UI fidelity are excluded.'])
 suffix='diagnostic' if target.stem=='restore_link_check' else 'game'
 (ROOT/f'metadata/migration-flow-{suffix}.json').write_text(json.dumps(proof,indent=2)+'\n')

