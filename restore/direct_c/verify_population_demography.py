"""Offline original/C births, deaths, age cohorts and population summaries.

Execute actual children; compare ordered owned stores, child inputs, all original
static data and the saved ABI. Actual demographic and numeric children execute;
no child results are replaced and no host game is launched.
"""
from verify_options_startup import *
from unicorn import UC_HOOK_MEM_WRITE,UC_HOOK_INTR
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import json
catalogue=json.loads((ROOT/'metadata/population-demography-native.json').read_text())
assert catalogue['native_sha256']==hashlib.sha256(original.read_bytes()).hexdigest()
names={0x458e20:'restore_population_add_births',0x458f00:'restore_population_remove_deaths',0x4e92c0:'restore_population_age_group',0x4e92f0:'restore_population_remove_age_group',0x4e8650:'restore_population_age_and_deaths',0x4e8720:'restore_population_births',0x4e87a0:'restore_population_rebuild_totals',0x4e9460:'restore_population_working_age',0x458880:'restore_population_rebuild_housing_mix'}
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
calls={0x4e92c0:2,0x4e92f0:3,0x458e20:2,0x458f00:2,0x4cde40:2,0x4cde60:2,0x4e9460:0}
for c,pe in enumerate(images):
 u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(pe.base,(len(pe.data)+4095)&~4095);u.mem_write(pe.base,bytes(pe.data))
 u.mem_map(0x30000000,0x10000);u.mem_map(STOP,4096);u.mem_map(S,0x50000)
 state=dict(collect=False,own=True,writes=[],events=[],pending=set(),returns=set(),compiled=c,last_entry=None);machines.append((c,u,state))
 def write(uc,access,at,size,value,data):
  if data['collect'] and data['own']:
   data['writes'].append((at,size,value&((1<<(size*8))-1)))
   cfg=data['cfg'];off=cfg.get('player',1)*0x49d8;owner=cfg.get('owner',0)*0x49d8;mutation=data['mutation']
   if mutation=='birth_quantity' and at==0xea3f88+off:
    word(uc,0x7999f4+value*264,500);word(uc,0x7999ea+value*264,2)
   if mutation=='death_quantity' and at==0xea3f8c+off:word(uc,0x7999f4+(value&65535)*264,0)
   if mutation in ['birth_cursor','death_cursor'] and at==0x7999f4+264:
    (put if mutation=='birth_cursor' else word)(uc,(0xea3f88 if mutation=='birth_cursor' else 0xea3f8c)+off,3999)
   if mutation=='birth_reset_owner' and at==0xea3f4c+owner and not value:put(uc,0xeaac28,3)
   if mutation=='birth_history_owner' and at==0xea3e34+owner:put(uc,0xeaac28,3)
   if mutation=='death_counter_owner' and at==0xea3f50+owner and value:put(uc,0xeaac28,3)
   if mutation=='totals_history' and at==0xea3f80+off:put(uc,0xea3f74+off,12000)
   if mutation=='totals_population' and at==0xea3f54+off:put(uc,0xea1894+off,1234)
   if mutation=='mix_count' and at==0x78d840 and not value:put(uc,0x78d83c,1)
   if mutation=='mix_signed' and at==0x78d840 and value:word(uc,0x78e178,-1)
   if mutation=='mix_stores' and at in [0xea4394+p*0x49d8 for p in [0,1,3]]:
    data['mix_stores']=data.get('mix_stores',0)+1;put(uc,0xeaac28,1 if data['mix_stores']==1 else 0)
 u.hook_add(UC_HOOK_MEM_WRITE,write,state,begin=0x560000,end=0x126cfff)
 def interrupted(uc,number,data):
  assert data.get('expect_trap'),('unexpected CPU interrupt',number)
  data['trap_seen'].append(number);uc.emu_stop()
 u.hook_add(UC_HOOK_INTR,interrupted,state)
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
  if t==0x4e92c0:
   if mutation in ['owner','working_owners'] and data['counts'][t]<=2:put(uc,0xeaac28,3 if mutation=='owner' or data['counts'][t]==2 else 1)
   if mutation=='coefficient' and data['counts'][t]==1:
    put(uc,0x5e49ac+args[1]*4,100);put(uc,0x5e49d4+(args[1]+5*10)*4,100)
  if mutation=='mix_owner' and t==0x4cde60:put(uc,0xeaac28,3)
  if t not in owned:data['own']=False
  data['pending'].add(ret)
  if ret not in data['returns']:uc.hook_add(UC_HOOK_CODE,returned,data,begin=ret,end=ret);data['returns'].add(ret)
 # Observe each real C algorithm once at its named provider. Canonical and
 # compatibility wrappers can use CALL or a tail jump and are exercised below.
 state['entries']={(sym(names[t] if t in names else 'FUN_%08x'%t) if c else t):t for t in calls}
 for a in state['entries']:u.hook_add(UC_HOOK_CODE,child,state,begin=a,end=a)
 state['callbacks']=(write,interrupted,code if not c else None,child)





frame_names={t:['restore_city_C_%08x'%t] for t in names}
results={0x458e20,0x458f00,0x4e92c0,0x4e9460}

def prepare(u,fn,cfg,state):
 def v(a,x,n=4):u.mem_write(a&0xffffffff,(x&((1<<(n*8))-1)).to_bytes(n,'little'))
 def record(bid,fields):
  for a,x,n in fields:v(a+bid*264,x,n)
 u.mem_write(0x560000,seed);u.mem_write(0x30000000,b'\xcc'*0x10000);u.mem_write(S,bytes(0x50000))
 dense=cfg.get('dense',fn in [0x4e8650,0x4e8720])
 if dense:
  sample=bytearray(264);sample[0]=1;sample[5]=1;struct.pack_into('<h',sample,18,1);struct.pack_into('<h',sample,28,100);struct.pack_into('<h',sample,32,1)
  u.mem_write(0x7999d8,bytes(264)+bytes(sample)*3999)
 else:u.mem_write(0x7999d8,bytes(4000*264))
 player=cfg.get('player',1);owner=cfg.get('owner',0);v(0xeaac28,owner)
 nodes=cfg.get('nodes',[1,2,3]);phases=cfg.get('phases',[cfg.get('phase',1)]*len(nodes));populations=cfg.get('populations',[cfg.get('population',100)]*len(nodes))
 for phase in {*phases,1,2}:v(0x70ce14+phase*88,cfg.get('capacity',200) if phase!=2 else cfg.get('new_capacity',6))
 for i,bid in enumerate(nodes):
  record(bid,[(0x7999d8,cfg.get('state',1),1),(0x7999dd,cfg.get('house',1),1),(0x7999dc,cfg.get('large',0),1),
   (0x7999ea,phases[i],2),(0x7999f4,populations[i],2),(0x7999f6,555,2),(0x7999f8,cfg.get('access',1),2)])
 record(-1,[(0x7999d8,0,1),(0x7999ea,1,2),(0x7999f4,17,2)])
 v(0x78d83c,cfg.get('count',len(nodes)));v(0x78d840,99)
 for i,bid in enumerate(nodes):v(0x78e178+i*2,bid,2)
 for p in {0,1,2,3,owner,player}:
  off=p*0x49d8;ages=cfg.get('ages',[p+1]*100);assert len(ages)==100
  for i,x in enumerate(ages):v(0xea3e34+off+i*2,x,2)
  for a,x in [(0xea1888,cfg.get('health',50+p*10)),(0xea3f88,cfg.get('cursor',0)),(0xea3f8c,cfg.get('cursor',0)),
   (0xea1894,7654321),(0xea1898,8765432),(0xea5f0c,cfg.get('maximum',0)),(0xea3f84,99),
   (0xea3f54,88),(0xea3f74,cfg.get('cumulative',1234)),(0xea3f80,cfg.get('period',4)),
   (0xea3f70,77),(0xea3f50,66),(0xea3f4c,55),(0xea4390,77 if p==0 else 55),(0xea4394,44),(0xea4398,33)]:v(a+off,x)
 group=cfg.get('group',2)
 if fn in [0x4e92c0,0x4e92f0] and 'group_values' in cfg:
  for i,x in enumerate(cfg['group_values']):v(0xea3e34+player*0x49d8+group*20+i*2,x,2)
 if fn in [0x458e20,0x458f00]:return [player,cfg.get('requested',2)]
 if fn==0x4e92c0:return [player,group]
 if fn==0x4e92f0:return [player,group,cfg.get('amount',3)]
 if fn==0x4e87a0:return [player]
 return []

def exercise(fn=0x458e20,**cfg):
 global checked,invocations
 expected=None
 for c,u,state in machines:
  for route in (['native'] if not c else ['named','canonical',*frame_names[fn]]):
   state.update(collect=False,own=True,writes=[],events=[],pending=set(),last_entry=None,expect_trap=False,mix_stores=0,top=fn,mutation=cfg.get('mutation'),counts={},cfg=cfg)
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
   state['collect']=False;answer=(tuple(state['writes']),tuple(state['events']),snapshot(u),result if fn in results else None)
   if expected is None:expected=answer
   elif answer!=expected:
    diffs=[next(((i,a,b) for i,(a,b) in enumerate(zip(x,y)) if a!=b),None) for x,y in zip(expected[:2],answer[:2])]
    memory=next(((hex(0x560000+i),a,b) for i,(a,b) in enumerate(zip(expected[2],answer[2])) if a!=b),None)
    raise AssertionError((hex(fn),cfg,route,'stores/events',diffs,'lengths',[(len(x),len(y)) for x,y in zip(expected[:2],answer[:2])],'memory',memory,'result',(expected[3],answer[3])))
 checked+=1
 if checked%25==0:print('progress',checked,'scenarios',flush=True)
 return expected[3]

def exercise_divide_error(period,cumulative):
 global checked,invocations
 fn=0x4e87a0;cfg=dict(period=period,cumulative=cumulative,ages=[0]*100);expected=None
 for c,unused,template in machines:
  for route in (['native'] if not c else ['named','canonical',*frame_names[fn]]):
   # A divide error leaves hidden x86 exception state in Unicorn. Each fault
   # route needs a new CPU, rather than a second fault on an interrupted CPU.
   pe=images[c];u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(pe.base,(len(pe.data)+4095)&~4095);u.mem_write(pe.base,bytes(pe.data))
   u.mem_map(0x30000000,0x10000);u.mem_map(STOP,4096);u.mem_map(S,0x50000)
   state=dict(collect=False,own=True,writes=[],events=[],pending=set(),returns=set(),last_entry=None,expect_trap=True,trap_seen=[],top=fn,mutation=None,counts={},cfg=cfg,entries=template['entries'])
   writing,interrupting,covering,child_entry=template['callbacks'];u.hook_add(UC_HOOK_MEM_WRITE,writing,state,begin=0x560000,end=0x126cfff);u.hook_add(UC_HOOK_INTR,interrupting,state)
   if not c:
    for points_ in owned.values():u.hook_add(UC_HOOK_CODE,covering,state,begin=min(points_),end=max(points_))
   for entry in state['entries']:u.hook_add(UC_HOOK_CODE,child_entry,state,begin=entry,end=entry)
   args=prepare(u,fn,cfg,state);last[0]=None;state['collect']=True;invocations+=1
   pc=fn if not c else sym(names[fn] if route=='named' else 'FUN_%08x'%fn if route=='canonical' else route)
   if route in frame_names[fn]:
    frame=S+0x1000;vsp=S+0x3000;u.mem_write(frame,struct.pack('<10I',0x11111111,0x22222222,0x33333333,saved[0],vsp,*saved[1:],2,0));put(u,vsp,STOP);put(u,vsp+4,args[0]);args=[frame]
   try:run(0,u,pc,args,budget=50000000)
   except AssertionError:assert state['trap_seen']==[0],(route,state['trap_seen'])
   else:raise AssertionError(('missing native divide error',route,period,cumulative))
   state['collect']=False;answer=(tuple(state['writes']),tuple(state['events']),snapshot(u))
   if expected is None:expected=answer
   else:assert answer==expected,('pre-divide-error state',route,period,cumulative)
 checked+=1

if __name__=='__main__':
 quick='--quick' in sys.argv
 for fn in names:exercise(fn)
 exercise(mutation='birth_quantity',population=5)
 exercise(0x458f00,mutation='death_quantity')
 exercise(0x4e8720,mutation='birth_history_owner')
 exercise(0x4e8650,mutation='death_counter_owner')
 exercise(0x458880,mutation='mix_stores')
 if not quick:
  for fn in [0x458e20,0x458f00]:
   for requested in [-2147483648,-1,0,1,3,4,20]:exercise(fn,requested=requested)
   for cursor in [-2,-1,0,1,3998,3999,4000,2147483647]:exercise(fn,cursor=cursor)
   for cfg in [dict(nodes=[]),dict(state=0),dict(state=2),dict(house=0),dict(house=255),dict(access=0),dict(access=-1),dict(population=0),dict(population=-1),dict(population=32767)]:exercise(fn,**cfg)
  for fn in [0x458e20,0x458f00]:assert exercise(fn,dense=True,requested=300)==300
  assert exercise(dense=True,requested=4000)==3999
  assert exercise(0x458f00,dense=True,requested=16000)==15999
  for cfg in [dict(capacity=100),dict(capacity=99),dict(capacity=0),dict(capacity=2147483648),dict(capacity=2147483647),dict(capacity=4294967295),dict(large=1),dict(large=255),dict(mutation='birth_cursor'),dict(nodes=[3999],cursor=3998)]:exercise(**cfg)
  exercise(0x458f00,mutation='death_cursor');exercise(0x458f00,nodes=[3999],cursor=3998)
  for fn in [0x4e92c0,0x4e92f0]:
   for group in [-1,0,1,9,10,12]:exercise(fn,group=group,group_values=list(range(1,11)))
   for values in [[0]*10,[-1]*10,[32767]*10,[0,1,0,2,0,3,0,4,0,5]]:exercise(fn,group_values=values)
   for player in [0,3]:exercise(fn,player=player)
  for amount in [-1,0,1,10,11,20,21]:exercise(0x4e92f0,amount=amount,group_values=[2]*10)
  for fn in [0x4e8650,0x4e8720]:
   for ages in [[0]*100,[1]*100,[2]*100,[0]*99+[20],[-1]*100]:exercise(fn,ages=ages)
   for mutation in ['owner','coefficient']:exercise(fn,mutation=mutation)
   exercise(fn,dense=False,ages=[0]*100)
  exercise(0x4e8720,mutation='birth_reset_owner')
  for health in [-9,-1,0,9,10,49,50,59,60,99,100]:exercise(0x4e8650,health=health)
  for cfg in [dict(mutation='working_owners'),dict(owner=1),dict(owner=3),dict(ages=[-32768]*100),dict(ages=[32767]*100)]:exercise(0x4e9460,**cfg)
  for cfg in [dict(player=0),dict(player=3),dict(ages=[0]*100),dict(ages=[-32768]*100),dict(ages=[32767]*100),dict(maximum=2147483647),dict(maximum=-1),dict(cumulative=2147483647),dict(cumulative=-2147483648),dict(period=0),dict(period=-2),dict(period=2147483647),dict(mutation='totals_history'),dict(mutation='totals_population')]:exercise(0x4e87a0,**cfg)
  for cfg in [dict(count=0),dict(count=-1),dict(population=0),dict(population=-1),dict(phase=13),dict(phase=14),dict(phase=-1),dict(phases=[1,14,20]),dict(mutation='mix_owner'),dict(mutation='mix_count'),dict(mutation='mix_signed'),dict(owner=3)]:exercise(0x458880,**cfg)
  exercise_divide_error(-1,1234);exercise_divide_error(-2,-2147483648)
 missing=points-hits;missing_edges={(a,b) for a,ends in branches.items() for b in ends if (a,b) not in edges}
 print('Compared population demography:',checked,'scenarios;',invocations,'invocations;',len(points&hits),'/',len(points),'instructions,',len(edges),'conditional outcomes',flush=True)
 if quick:sys.exit(0)
 print('Unexecuted instructions:',[hex(a) for a in sorted(missing)],'; edges:',[(hex(a),hex(b)) for a,b in sorted(missing_edges)],flush=True)
 assert not missing,('instruction coverage',missing)
 assert not missing_edges,('edge coverage',missing_edges)
 sources=['CMakeLists.txt','include/population_demography.h','runtime/population_demography.c','runtime/city_updates.c','runtime/city_updates_model.c','src/unit_00834.c','src/unit_02031.c','src/unit_02045.c','direct_c/verify_population_demography.py','metadata/population-demography-native.json']
 proof=dict(native_sha256=catalogue['native_sha256'],compiled_C_path=str(target.resolve()),compiled_C_sha256=target_sha,passed=True,checked=checked,invocations=invocations,divide_error_cases=2,
  coverage={hex(a):dict(instructions=len(p),executed=len(p&hits)) for a,p in owned.items()},conditional_edges=len(edges),unexecuted_instructions=[],unexecuted_edges=[],
  source_hashes={p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest() for p in sources},commit=None,controlled_algorithm_endpoints=[],controlled_platform_endpoints=[],
  limitations=['Rea authenticates native code and coefficient tables. Actual birth/death/cohort/working-age and numeric children execute. Dense real house records keep composed demographic fixtures finite without replacing child results.',
   'Named/canonical/nine frame adapters; DWORD returns where defined, ordered stores, child inputs, final original static data and saved ABI compared. Includes captured versus fresh owners, cursor/quantity changes, signed cohorts and wrapped totals.',
   'The current MSVC/x86 build also matches original divide-by-zero and signed-overflow CPU interrupts plus all pre-fault data/stores. Returning-call ABI is checked; fault-time private stack/register layout is excluded.',
   'Incidental caller-saved registers/flags, private scratch, concurrent access and host game/UI fidelity are excluded.'])
 suffix='diagnostic' if target.stem=='restore_link_check' else 'game'
 (ROOT/f'metadata/population-demography-{suffix}.json').write_text(json.dumps(proof,indent=2)+'\n')

