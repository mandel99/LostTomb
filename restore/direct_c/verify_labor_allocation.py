"""Offline native/C labor allocation. Actual child algorithms execute.

Compare ordered owned stores, consumed direct child inputs, original static data,
stack cleanup and saved registers through named, canonical and frame entries.
"""
from verify_options_startup import *
from unicorn import UC_HOOK_MEM_WRITE
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
import json

catalogue=json.loads((ROOT/'metadata/labor-allocation-native.json').read_text())
assert catalogue['native_sha256']==hashlib.sha256(original.read_bytes()).hexdigest()
rea=json.loads((ROOT/'metadata/labor-allocation-rea-evidence.json').read_text())
reads={r['parameters']['address']:r for r in rea['records'] if r['operation']=='read_bytes'}
for f in catalogue['functions']:
 r=reads[f['address']]
 assert r['authority']=='shipped-artifact' and r['confidence']=='observed'
 assert r['subject']['digest']['sha256']==catalogue['native_sha256']
 assert r['normalized_result']['complete'] and r['normalized_result']['bytes_hex']==f['bytes_hex']
names={0x459070:'restore_housing_displace_overcrowding',0x459100:'restore_labor_rebuild_demand',
       0x459350:'restore_labor_update_unemployment',0x459400:'restore_labor_allocate_groups',
       0x4595a0:'restore_labor_rebuild_shares',0x459670:'restore_labor_assign_buildings'}
owned={};branches={};hits=set();edges=set();last=[None];checked=invocations=0
decoder=Cs(CS_ARCH_X86,CS_MODE_32)
for f in catalogue['functions']:
 a=int(f['address'],16);raw=bytes.fromhex(f['bytes_hex'])
 assert raw==bytes(images[0].data[a-images[0].base:a-images[0].base+len(raw)])
 instructions=list(decoder.disasm(raw,a));owned[a]={int(p,16) for p in f['instruction_addresses'].split()}
 assert {i.address for i in instructions}==owned[a]
 for i in instructions:
  if i.mnemonic.startswith('j') and i.mnemonic!='jmp':branches[i.address]={int(i.op_str,16),i.address+i.size}
for t in catalogue.get('tables',[]):
 a=int(t['address'],16);raw=bytes.fromhex(t['bytes_hex'])
 assert raw==bytes(images[0].data[a-images[0].base:a-images[0].base+len(raw)])
points=set.union(*owned.values())
calls={0x459020:4,0x488050:1,0x459240:1,0x4cde40:2,0x4cde60:2,0x459400:0,
       0x4e0cf0:5,0x43f060:3,0x4a36b0:1}
machines=[]
for c,pe in enumerate(images):
 u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(pe.base,(len(pe.data)+4095)&~4095);u.mem_write(pe.base,bytes(pe.data))
 u.mem_map(0x30000000,0x10000);u.mem_map(STOP,4096);u.mem_map(S,0x50000)
 state=dict(collect=False,own=True,writes=[],events=[],pending=set(),returns=set(),compiled=c,last_entry=None)
 machines.append((c,u,state))
 def write(uc,access,at,size,value,data):
  if not data['collect'] or not data['own']:return
  data['writes'].append((at,size,value&((1<<(size*8))-1)))
  mutation=data['mutation']
  if mutation=='displace_cursor' and at==0x78d840:put(uc,0x78d840,2)
  if mutation=='displace_bound' and at==0x78d840:put(uc,0x78d83c,1)
  if mutation=='demand_owner' and at==0x799a1a+264:put(uc,0xeaac28,1)
  if mutation=='share_coverage' and at==0x7999f2+264 and not value:word(uc,0x7999f0+264,25)
  if mutation=='share_owner' and at==0x7999f2+264 and not value:put(uc,0xeaac28,1)
  if mutation=='assign_bound' and at==0x799a18+264:put(uc,0x78d790,1)
  if mutation=='assign_owner' and at==0x799a18+264:put(uc,0xeaac28,1)
 u.hook_add(UC_HOOK_MEM_WRITE,write,state,begin=0x560000,end=0x126cfff)
 if not c:
  def code(uc,pc,size,data):
   if not data['collect']:return
   hits.add(pc)
   if last[0] in branches:edges.add((last[0],pc))
   last[0]=pc
  for p in owned.values():u.hook_add(UC_HOOK_CODE,code,state,begin=min(p),end=max(p))
 def returned(uc,pc,size,data):
  if data['collect'] and pc in data['pending']:
   data['pending'].remove(pc);data['own']=True;data['last_entry']=None
 def child(uc,pc,size,data):
  if not data['collect'] or not data['own']:return
  t=data['entries'][pc];sp=uc.reg_read(UC_X86_REG_ESP);ret=get(uc,sp)
  if t==data['top']:return
  entry=(t,sp,ret)
  if entry==data['last_entry']:return
  data['last_entry']=entry
  args=tuple(get(uc,sp+4+i*4) for i in range(calls[t]))
  if t==0x43f060:args=(args[0],args[1]&255,args[2])
  data['events'].append((t,args));data['counts'][t]=data['counts'].get(t,0)+1
  mutation=data['mutation']
  if t==0x459020 and data['counts'][t]==1:
   if mutation=='displace_population':word(uc,0x7999f4+264,2)
   if mutation=='displace_owner':put(uc,0xeaac28,1)
  if t==0x488050 and mutation=='gate_owner':put(uc,0xeaac28,1)
  if t==0x4cde60 and mutation=='ratio_owner':put(uc,0xeaac28,1)
  if t not in owned:data['own']=False
  data['pending'].add(ret)
  if ret not in data['returns']:uc.hook_add(UC_HOOK_CODE,returned,data,begin=ret,end=ret);data['returns'].add(ret)
 state['entries']={(sym(names[t] if t in names else 'restore_actor_building_gate' if t==0x488050 else 'FUN_%08x'%t) if c else t):t for t in calls}
 for a in state['entries']:u.hook_add(UC_HOOK_CODE,child,state,begin=a,end=a)

frames={t:['restore_city_C_%08x'%t] for t in names}
for t in [0x459400,0x4595a0,0x459670]:frames[t].append('restore_labor_C_%08x'%t)

def prepare(u,fn,cfg):
 def v(a,x,n=4):u.mem_write(a&0xffffffff,(x&((1<<(n*8))-1)).to_bytes(n,'little'))
 u.mem_write(0x560000,seed);u.mem_write(0x30000000,b'\xcc'*0x10000);u.mem_write(S,bytes(0x50000))
 u.mem_write(0x7999d8,bytes(4000*264));u.mem_write(0xbcd5f8,bytes(2000*388));u.mem_write(0x9784d8,bytes(50*44))
 owner=cfg.get('owner',0);v(0xeaac28,owner);v(0xbca2d8,0);v(0xbca2d0,228);v(0xbca2d4,228)
 v(0xbcd5f0,0);v(0xd056fc,123);v(0xd056e0,127);v(0x604950,1);v(0xf5653c,1000)
 if cfg.get('gate'):u.mem_write(0xaf598c,struct.pack('<I',0x10000)*51984)
 groups=cfg.get('groups',[0,1,2,3,4,5,6,7,8,9]);kinds=cfg.get('kinds',list(range(30,30+len(groups))))
 count=len(groups);v(0x78d790,cfg.get('bound',count));v(0x78d83c,cfg.get('count',count));v(0x78d840,99)
 for p in {0,1,3,owner}:
  off=p*0x49d8
  for i in range(100):v(0xea3e34+off+i*2,20,2)
  for a,x in [(0xea4398,cfg.get('workers',15)),(0xea4464,0),(0xea4468,0),(0xea446c,0),
              (0xea4470,cfg.get('old_unemployment',0)),(0xea4474,cfg.get('old_shortage',0)),(0xea1894,2000)]:v(a+off,x)
  for i in range(10):
   for a,key,default in [(0xea439c,'demand',10),(0xea43a0,'allocated',5),(0xea43a4,'coverage_total',100),
                          (0xea43a8,'buildings',1),(0xea43ac,'priorities',0)]:
    values=cfg.get(key,[default]*10);v(a+off+i*20,values[i])
  for i in range(36):v(0xea4126+off+i*2,cfg.get('blocked',0),2)
 for i,(group,kind) in enumerate(zip(groups,kinds),1):
  r=i*264;v(0x78e178+(i-1)*2,i,2);v(0x5d06ec+kind*20,group)
  v(0x705eb8+kind*40,cfg.get('max_workers',10))
  for a,x,n in [(0x7999d8,cfg.get('status',1),1),(0x7999db,1,1),(0x7999dd,1,1),
                (0x7999e8,kind,2),(0x7999ea,0,2),(0x7999de,10+i*2,2),(0x7999e0,11,2),
                (0x7999e4,10+i*2+11*228,4),(0x7999f0,cfg.get('coverage',50),2),(0x7999f2,cfg.get('share',50),2),
                (0x7999f4,cfg.get('population',10),2),(0x7999f6,cfg.get('space',-3),2),(0x7999f8,1,2),
                (0x799a10,cfg.get('special',0),2),(0x799a18,cfg.get('staff',7),2),(0x799a1b,1,1),
                (0x799a9c,cfg.get('linked',0),4)]:v(a+r,x,n)
 for i in range(100):v(0xd3b1c8+i*4,i)
 v(0xd3b1c4,0);v(0x78d7bc,cfg.get('time',100));v(0x7848d8,cfg.get('threshold',50),2);v(0xe95144,cfg.get('cooldown',0))
 v(0xe38dda,0,1);v(0xe38ec1,1,1);v(0x605a0c,1);v(0x605a3c,0);v(0xe38ebc,1);v(0xe38ea8,3,1);v(0xea624a,1,1)
 for i in range(5):v(0xe94e2e+i*120,1,2)
 free=cfg.get('queue_free',0)
 for i in range(1000):v(0xe95232+i*48,0 if i>=1000-free else 1,2)
 v(0xe951ec,1000-free);v(0xe951f0,1001-free);v(0xf55b88,0)

def exercise(fn=0x459400,**cfg):
 global checked,invocations
 expected=None
 for c,u,state in machines:
  for route in (['native'] if not c else ['named','canonical',*frames[fn]]):
   state.update(collect=False,own=True,writes=[],events=[],pending=set(),last_entry=None,top=fn,mutation=cfg.get('mutation'),counts={},cfg=cfg)
   prepare(u,fn,cfg);last[0]=None;state['collect']=True;invocations+=1
   pc=fn if not c else sym(names[fn] if route=='named' else 'FUN_%08x'%fn if route=='canonical' else route)
   try:
    if route in frames[fn]:
     frame=S+0x1000;vsp=S+0x3000;initial=[0x11111111,0x22222222,0x33333333,saved[0],vsp,*saved[1:]]
     u.mem_write(frame,struct.pack('<10I',*initial,2,0));put(u,vsp,STOP)
     run(0,u,pc,[frame],budget=50000000)
     out=struct.unpack('<8I',bytes(u.mem_read(frame,32)));assert out[4]==vsp+4
     assert [out[i] for i in [3,5,6,7]]==saved
    else:run(0,u,pc,[],budget=50000000)
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
 if quick:print('PASS quick labor:',checked,'scenarios;',invocations,'invocations');sys.exit(0)
 for fn in names:
  for cfg in [dict(owner=1),dict(owner=3),dict(bound=0,count=0),dict(bound=-1,count=-1),dict(status=0),dict(status=2)]:exercise(fn,**cfg)
 for fn in [0x459070]:
  for space in [0,1,-1,-10,-11,-32768]:exercise(fn,space=space)
  for mutation in ['displace_cursor','displace_bound','displace_population','displace_owner']:exercise(fn,mutation=mutation)
 for fn in [0x459100,0x4595a0,0x459670]:
  for coverage in [0,-1,1,32767,-32768]:exercise(fn,coverage=coverage)
  for cfg in [dict(groups=[-1,0,1,2,3,8,9]),dict(blocked=1),dict(special=1),dict(groups=[0,1,2,3,8],kinds=[30,31,33,112,205]),dict(groups=[0,1],kinds=[100,101],gate=True),dict(mutation='gate_owner')]:exercise(fn,**cfg)
 for priorities in [[0]*10,list(range(1,10))+[0],[1]*10,[10]*10,[-1]*10,[9,8,7,6,5,4,3,2,1,0]]:
  for workers in [0,1,2,8,15,100,101]:exercise(priorities=priorities,workers=workers)
 for demand in [[0]*10,[-1]*10,[1]*10,[0]*9+[100],[1,2,3,4,5,6,7,8,9,10]]:
  for workers in [0,1,5,100]:exercise(demand=demand,workers=workers)
 exercise(mutation='ratio_owner')
 for cfg in [dict(allocated=[0]*10),dict(allocated=[10]*10),dict(allocated=[100]*10),dict(share=0),dict(share=-1),dict(share=32767),dict(max_workers=0),dict(groups=[2,2,2,2],kinds=[30,31,33,32],special=1),dict(groups=[8,8,8],kinds=[112,205,111],blocked=1),dict(groups=[3],kinds=[136]),dict(groups=[3],kinds=[136],linked=1),dict(groups=[3],kinds=[136],share=0),dict(mutation='assign_bound'),dict(mutation='assign_owner')]:exercise(0x459670,**cfg)
 for mutation in ['share_coverage','share_owner','ratio_owner']:exercise(0x4595a0,mutation=mutation)
 exercise(0x459100,mutation='demand_owner')
 for workers,old in [(1,0),(15,0),(15,100),(100,0),(100,1),(101,99),(200,200),(200,0)]:exercise(0x459350,workers=workers,old_unemployment=old)
 exercise(0x459350,demand=[-1]*10,old_unemployment=120)
 for cfg in [dict(old_shortage=1),dict(time=49),dict(cooldown=1),dict(queue_free=1000),dict(queue_free=1)]:exercise(0x459350,**cfg)
 missing=points-hits;missing_edges={(a,b) for a,ends in branches.items() for b in ends if (a,b) not in edges}
 print('Compared labor:',checked,'scenarios;',invocations,'invocations;',len(points&hits),'/',len(points),'instructions;',len(edges),'conditional outcomes',flush=True)
 print('Unexecuted instructions:',[hex(a) for a in sorted(missing)],'; edges:',[(hex(a),hex(b)) for a,b in sorted(missing_edges)],flush=True)
 assert not missing,('instruction coverage',missing)
 cold_edges={(0x45951c,0x459531)}
 assert not missing_edges-cold_edges,('edge coverage',missing_edges-cold_edges)
 sources=['CMakeLists.txt','include/labor_allocation.h','runtime/labor_allocation.c','runtime/city_updates.c','runtime/city_updates_model.c','runtime/menu_labor_model.c','src/unit_00848.c','direct_c/verify_labor_allocation.py','metadata/labor-allocation-native.json','metadata/labor-allocation-rea-evidence.json']
 proof=dict(native_sha256=catalogue['native_sha256'],compiled_C_path=str(target.resolve()),compiled_C_sha256=target_sha,passed=True,checked=checked,invocations=invocations,
  coverage={hex(a):dict(instructions=len(p),executed=len(p&hits)) for a,p in owned.items()},conditional_edges=len(edges),unexecuted_instructions=[],unexecuted_edges=[],
  analytically_unreachable_edges=[[hex(a),hex(b)] for a,b in sorted(cold_edges)],
  source_hashes={p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest() for p in sources},controlled_algorithm_endpoints=[],controlled_platform_endpoints=[],
  limitations=['Actual child algorithms execute; proof covers six owned functions and their consumed child inputs, not complete child coverage.',
   '45951C to 459531 is infeasible: every update to register ESI is tested at 45950D and exits on <=0; skipped slots do not change ESI. The final loop test cannot newly see <=0 under this sequential execution contract.',
   'Void returns exclude incidental caller-saved registers/flags. Private scratch, concurrent access and live game/UI fidelity are excluded.'])
 suffix='diagnostic' if target.stem=='restore_link_check' else 'game'
 (ROOT/f'metadata/labor-allocation-{suffix}.json').write_text(json.dumps(proof,indent=2)+'\n')
