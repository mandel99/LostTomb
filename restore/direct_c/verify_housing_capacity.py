"""Offline original/C building timers, housing capacity and migration demand.

Execute actual children; compare ordered owned stores, child inputs, all original
static data and the saved ABI. Actual list, monthly reset, numeric and warning children execute;
no child results are replaced and no host game is launched.
"""
from verify_options_startup import *
from unicorn import UC_HOOK_MEM_WRITE
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import json
catalogue=json.loads((ROOT/'metadata/housing-capacity-native.json').read_text())
assert catalogue['native_sha256']==hashlib.sha256(original.read_bytes()).hexdigest()
names={0x4558b0:'restore_building_decay_timers',0x4583f0:'restore_housing_rebuild_list',0x4e8610:'restore_migration_reset_monthly',0x458440:'restore_housing_rebuild_capacity',0x4e8f80:'restore_migration_calculate_demand',0x4e2fb0:'restore_building_warning_004e2fb0'}
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
calls={0x4e8610:0,0x4583f0:0,0x4cde40:2,0x4e2fb0:1,0x4e2970:1}
for c,pe in enumerate(images):
 u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(pe.base,(len(pe.data)+4095)&~4095);u.mem_write(pe.base,bytes(pe.data))
 u.mem_map(0x30000000,0x10000);u.mem_map(STOP,4096);u.mem_map(S,0x50000)
 state=dict(collect=False,own=True,writes=[],events=[],pending=set(),returns=set(),compiled=c,last_entry=None);machines.append((c,u,state))
 def write(uc,access,at,size,value,data):
  if data['collect'] and data['own']:
   data['writes'].append((at,size,value&((1<<(size*8))-1)))
   cfg=data['cfg'];off=cfg.get('player',1)*0x49d8;owner=cfg.get('owner',0)*0x49d8;mutation=data['mutation']
   if mutation=='timer_bound' and at==0x7999f0+264:put(uc,0x78d790,1)
   if mutation=='list_bound' and at==0x78d83c and not value:put(uc,0x78d790,1)
   if mutation=='capacity_count' and at==0x78d840 and not value:put(uc,0x78d83c,1)
   if mutation=='signed_list' and at==0x78d840 and value:word(uc,0x78e178,-1)
   if mutation=='capacity_owner' and at==0xea18a4+owner:put(uc,0xeaac28,3)
   if mutation=='capacity_population' and at==0xea18a4+owner:word(uc,0x7999f4+264,500);byte(uc,0x799a5f+264,1)
   if mutation=='peak' and at==0x7999f6+264 and value:word(uc,0x7999f4+264,99)
   if mutation=='rate_owner' and at==0xea3f5c+off:put(uc,0xeaac28,3)
   if mutation=='rate_fresh' and at==0xea3f58+off:put(uc,0xea3f9c+off,75)
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
  data['events'].append((t,args))
  data['counts'][t]=data['counts'].get(t,0)+1
  mutation=data['mutation'];cfg=data['cfg']
  if mutation=='rate_calculation' and t==0x4cde40:put(uc,0xeaac28,3);put(uc,0xea3f9c+cfg.get('player',1)*0x49d8,45)
  if t not in owned:data['own']=False
  data['pending'].add(ret)
  if ret not in data['returns']:uc.hook_add(UC_HOOK_CODE,returned,data,begin=ret,end=ret);data['returns'].add(ret)
 # Observe each real C algorithm once at its named provider. Canonical and
 # compatibility wrappers can use CALL or a tail jump and are exercised below.
 state['entries']={(sym(names[t] if t in names else 'FUN_%08x'%t) if c else t):t for t in calls}
 for a in state['entries']:u.hook_add(UC_HOOK_CODE,child,state,begin=a,end=a)




frame_names={t:[] if t==0x4e2fb0 else ['restore_city_C_%08x'%t] for t in names}
frame_names[0x4583f0].append('restore_event_C_004583f0')

def prepare(u,fn,cfg,state):
 def v(a,x,n=4):u.mem_write(a&0xffffffff,(x&((1<<(n*8))-1)).to_bytes(n,'little'))
 def record(bid,fields):
  for a,x,n in fields:v(a+bid*264,x,n)
 u.mem_write(0x560000,seed);u.mem_write(0x30000000,b'\xcc'*0x10000);u.mem_write(S,bytes(0x50000));u.mem_write(0x7999d8,bytes(4000*264))
 player=cfg.get('player',1);owner=cfg.get('owner',0);v(0xeaac28,owner)
 v(0xea624a,cfg.get('monthly',1));v(0xea627c,123456);v(0xea6280,234567)
 v(0x78d790,cfg.get('bound',3));v(0x78d83c,77);v(0x78d840,88)
 u.mem_write(0x78e178,b'\x5a'*8000)
 phase=cfg.get('phase',1);v(0x70ce14+phase*88,cfg.get('capacity',25));v(0x70ce14+88,25 if phase!=1 else cfg.get('capacity',25))
 for bid in cfg.get('nodes',[1,2,3]):
  record(bid,[(0x7999d8,cfg.get('state',1),1),(0x7999dd,cfg.get('house',1),1),(0x7999e8,cfg.get('kind',70),2),
   (0x7999f0,cfg.get('timer',5),2),(0x7999ea,phase,2),(0x7999dc,cfg.get('large',0),1),(0x7999f8,cfg.get('access',1),2),
   (0x7999f4,cfg.get('population_word',3),2),(0x7999f6,555,2),(0x7999fa,cfg.get('peak',1),2),(0x799a5f,cfg.get('blocked',0),1)])
 record(-1,[(0x7999ea,1,2),(0x7999f8,1,2),(0x7999f4,3,2),(0x7999dc,0,1),(0x799a5f,0,1)])
 for p in {0,1,3,owner,player}:
  off=p*0x49d8
  for a,x in [(0xea18a4,1000),(0xea18a8,cfg.get('available',2000)),(0xea1880,cfg.get('sentiment',75)),
   (0xea1894,cfg.get('population',1000)),(0xea3f9c,cfg.get('prior',0)),(0xea3f5c,999),(0xea3f58,888),
   (0xea5eb4,cfg.get('shortage',0)),(0xea5ea4,cfg.get('shortage2',0)),(0xea3fa4,cfg.get('incoming_delay',0)),
   (0xea3fa8,cfg.get('outgoing_delay',0)),(0xea3fb0,cfg.get('migration_mode',0)),(0xea3fac,cfg.get('immigrants',0)),
   (0xea3f98,cfg.get('refused',0)),(0xea5c80,cfg.get('cause',0))]:v(a+off,x)
 v(0xea1894+owner*0x49d8,cfg.get('owner_population',cfg.get('population',1000)))
 v(0x605a3c,cfg.get('scripted',0));v(0xe38ebc,cfg.get('mode',1));v(0xe38ea8,cfg.get('scene',3),1)
 # The actual pending-message provider sees a valid full five-slot queue;
 # it reaches its real capacity exit without font or host-window services.
 v(0xe38ec1,1,1);v(0xe38dda,1,1)
 for i in range(5):v(0xe94e2e+i*120,1,2)
 return [player] if fn==0x4e8f80 else [cfg.get('tool',0)] if fn==0x4e2fb0 else []

def exercise(fn=0x458440,**cfg):
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
 for fn,mutation in [(0x4558b0,'timer_bound'),(0x4583f0,'list_bound'),(0x458440,'capacity_count'),(0x458440,'signed_list'),(0x458440,'capacity_owner'),(0x458440,'capacity_population'),(0x458440,'peak'),(0x4e8f80,'rate_owner'),(0x4e8f80,'rate_fresh'),(0x4e8f80,'rate_calculation')]:exercise(fn,mutation=mutation)
 if not quick:
  for fn in [0x4558b0,0x4583f0,0x458440]:
   for cfg in [dict(bound=0),dict(bound=-1),dict(nodes=[]),dict(state=0),dict(state=2),dict(state=3),dict(state=255),dict(house=0),dict(house=255)]:exercise(fn,**cfg)
  for kind in [-1,0,58,59,60,171,172,173,174]:
   for timer in [-32768,-1,0,1,2,32767]:exercise(0x4558b0,kind=kind,timer=timer)
  exercise(0x4583f0,bound=3999,nodes=[1,3999])
  for monthly in [0,1,256,255]:
   for fn in [0x4e8610,0x458440]:exercise(fn,monthly=monthly)
  for cfg in [dict(access=0),dict(access=-1),dict(large=1),dict(large=255),dict(blocked=1),dict(peak=10),dict(population_word=0),dict(population_word=-1),dict(population_word=-32768),dict(population_word=32767),dict(owner=1),dict(owner=3)]:exercise(0x458440,**cfg)
  for phase in [-1,0,1,9,100]:
   for cap in [0,1,32768,65535,2147483647,2147483648,4294967295]:exercise(0x458440,phase=phase,capacity=cap,large=1)
  for sentiment in [-2147483648,-1,0,20,21,30,31,40,41,49,50,60,61,70,71,2147483647]:exercise(0x4e8f80,sentiment=sentiment)
  for cfg in [dict(player=0),dict(player=3),dict(owner=1),dict(owner=3),dict(owner_population=199999),dict(owner_population=200000),dict(owner_population=200001),dict(shortage=3),dict(shortage=4),dict(shortage=2147483647,shortage2=1),dict(shortage=-1),dict(prior=100),dict(prior=50)]:exercise(0x4e8f80,**cfg)
  for sentiment in [0,45,75]:
   for cfg in [dict(incoming_delay=1),dict(incoming_delay=-1),dict(outgoing_delay=1),dict(outgoing_delay=-1),dict(population=100),dict(population=101),dict(shortage=4)]:exercise(0x4e8f80,sentiment=sentiment,**cfg)
  for cfg in [dict(scripted=1),dict(population=19),dict(population=20),dict(mode=0,scene=1,cause=0),dict(mode=0,scene=1,cause=1),dict(mode=0,scene=1,cause=3),dict(mode=0,scene=1,cause=5),dict(mode=0,scene=1,cause=2),dict(mode=0,scene=-1,cause=2),dict(mode=0,scene=2),dict(shortage=4),dict(shortage=2147483647,shortage2=1)]:exercise(0x4e2fb0,**cfg)
  for mode in [0,1,2,-1]:
   for cfg in [dict(tool=0),dict(tool=10),dict(available=0),dict(available=-1),dict(refused=1),dict(immigrants=5),dict(immigrants=4,prior=79),dict(immigrants=4,prior=80),dict(cause=-124),dict(cause=-131)]:exercise(0x4e2fb0,migration_mode=mode,**cfg)
  exercise(0x4e2fb0,immigrants=5,tool=10)
  exercise(0x4e2fb0,available=0,tool=10)
  exercise(0x4e2fb0,migration_mode=1,available=0,tool=10)
 missing=points-hits;missing_edges={(a,b) for a,ends in branches.items() for b in ends if (a,b) not in edges}
 print('Compared housing capacity:',checked,'scenarios;',invocations,'invocations;',len(points&hits),'/',len(points),'instructions,',len(edges),'conditional outcomes',flush=True)
 if quick:sys.exit(0)
 print('Unexecuted instructions:',[hex(a) for a in sorted(missing)],'; edges:',[(hex(a),hex(b)) for a,b in sorted(missing_edges)],flush=True)
 assert not missing,('instruction coverage',missing)
 assert not missing_edges,('edge coverage',missing_edges)
 sources=['CMakeLists.txt','include/housing_capacity.h','runtime/housing_capacity.c','runtime/building_rect_commit.c','runtime/city_updates.c','runtime/city_updates_model.c','runtime/menu_event_model.c','src/unit_00831.c','src/unit_02027.c','src/unit_02036.c','src/unit_01934.c','direct_c/verify_housing_capacity.py','metadata/housing-capacity-native.json']
 proof=dict(native_sha256=catalogue['native_sha256'],compiled_C_path=str(target.resolve()),compiled_C_sha256=target_sha,passed=True,checked=checked,invocations=invocations,
  coverage={hex(a):dict(instructions=len(p),executed=len(p&hits)) for a,p in owned.items()},conditional_edges=len(edges),unexecuted_instructions=[],unexecuted_edges=[],
  source_hashes={p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest() for p in sources},commit=None,controlled_algorithm_endpoints=[],controlled_platform_endpoints=[],
  limitations=['Rea authenticates all code bytes/instructions. Actual house-list, monthly reset, percentage, warning and pending-message providers execute. A real full five-slot queue keeps warning rendering offline.',
   'Five newly reconstructed functions plus the existing warning provider through its consolidated canonical alias. Named/canonical/six frame adapters; ordered stores, child inputs, final original static data and saved ABI compared. Includes captured bounds/counts/player offsets, fresh quantities, signed types and wrapped totals.',
   'These are void functions. Incidental caller-saved registers/flags, private scratch, concurrent access and host game/UI fidelity are excluded.'])
 suffix='diagnostic' if target.stem=='restore_link_check' else 'game'
 (ROOT/f'metadata/housing-capacity-{suffix}.json').write_text(json.dumps(proof,indent=2)+'\n')

