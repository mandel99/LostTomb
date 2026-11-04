"""Five complete native/C timed, work-state and message-category functions.

All iterator/calendar children execute. Selector cases may control its weight
builder; actual=True executes it too. Mutation cases are deterministic read
phase probes, not a claim about concurrent gameplay.
"""
from verify_options_startup import *
from unicorn import UC_HOOK_MEM_WRITE,UC_HOOK_MEM_READ
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import json
catalogue=json.loads((ROOT/'metadata/message-category-native.json').read_text())
evidence={r['evidence_id']:r for r in json.loads((ROOT/'metadata/message-category-rea-evidence.json').read_text())['records']}
names={0x44d6d0:'restore_event_is_future_result',0x44d700:'restore_event_within_window_result',0x495b60:'restore_actor_building_work_state',0x53c060:'restore_message_choose_weighted_category',0x53d3a0:'restore_message_build_category_weights'}
owned={};branches={};hits=set();edges=set();jump_edges=set();previous=[None];checked=invocations=actual_cases=0
for f in catalogue['functions']:
 a=int(f['address'],16);raw=bytes.fromhex(f['bytes_hex']);r=evidence[f['bytes_evidence_id']]
 assert r['authority']=='shipped-artifact' and r['subject']['digest']['sha256']==catalogue['native_sha256']
 assert r['normalized_result']['complete'] and r['normalized_result']['bytes_hex']==f['bytes_hex']
 assert raw==bytes(images[0].data[a-images[0].base:a-images[0].base+len(raw)])
 ins=list(Cs(CS_ARCH_X86,CS_MODE_32).disasm(raw,a));owned[a]={int(p,16) for p in f['instruction_addresses'].split()}
 assert {i.address for i in ins}==owned[a]
 for i in ins:
  if i.mnemonic.startswith('j') and i.mnemonic!='jmp':branches[i.address]={int(i.op_str,16),i.address+i.size}
for t in catalogue['tables']:
 a=int(t['address'],16);raw=bytes.fromhex(t['bytes_hex']);assert raw==bytes(images[0].data[a-images[0].base:a-images[0].base+len(raw)])
switches={int(pc,16):set(struct.unpack_from('<%dI'%n,images[0].data,int(at,16)-images[0].base)) for pc,at,n in catalogue['switches']}
points=set.union(*owned.values());Q=S+0x10000;OUT=0xf55ba4;FLAG=OUT+16;TOWN=0x785d70
for c,u,state in machines:
 state.update(collect=False,own=True,returns=set())
 def writes(uc,access,at,size,value,data):
  if data['collect'] and data['own']:
   data['writes'].append((at,size,value&((1<<(8*size))-1)))
   if data['cfg'].get('owner_after') and at==OUT and size==1:put(uc,0xeaac28,data['cfg']['owner_after'])
 u.hook_add(UC_HOOK_MEM_WRITE,writes,state,begin=0x560000,end=0x126cfff)
 def reads(uc,access,at,size,value,data):
  if data['collect'] and at==0xd3b374 and data['cfg'].get('second_weights') is not None:
   uc.mem_write(OUT,bytes(data['cfg']['second_weights']))
 u.hook_add(UC_HOOK_MEM_READ,reads,state,begin=0xd3b374,end=0xd3b377)
 if not c:
  def visit(uc,pc,size,data):
   if data['collect'] and pc in data['pending']:data['own']=True
   if not data['collect'] or not data['own']:return
   hits.add(pc)
   if previous[0] in branches:edges.add((previous[0],pc))
   if previous[0] in switches:jump_edges.add((previous[0],pc))
   previous[0]=pc
  for p in owned.values():u.hook_add(UC_HOOK_CODE,visit,state,begin=min(p),end=max(p))
 def returned(uc,pc,size,data):
  if data['collect'] and pc in data['pending']:data['pending'].remove(pc);data['own']=True
 def child(uc,pc,size,data):
  if not data['collect'] or not data['own']:return
  t=data['entries'][pc]
  if t==data['fn']:return
  sp=uc.reg_read(UC_X86_REG_ESP);ret=get(uc,sp);cfg=data['cfg'];compiled=data['compiled']
  if t==0x44bb90:
   args=tuple(get(uc,sp+4+i*4) for i in range(3)) if compiled else (uc.reg_read(UC_X86_REG_ECX),get(uc,sp+4),get(uc,sp+8))
  elif t in [0x44d700,0x44d6d0]:args=(get(uc,sp+4) if compiled else uc.reg_read(UC_X86_REG_ECX),)
  else:args=(get(uc,sp+4),'private-default-flag')
  data['events'].append((t,args))
  if t==0x53d3a0 and not cfg.get('actual'):
   weights=cfg.get('weights',[8,6,1,0,4,6,8,4]);uc.mem_write(args[0],bytes(weights));byte(uc,get(uc,sp+8),cfg.get('default_flag',0))
   uc.reg_write(UC_X86_REG_EAX,cfg.get('divisor',sum(weights))&0xffffffff);uc.reg_write(UC_X86_REG_ESP,sp+4);uc.reg_write(UC_X86_REG_EIP,ret);return
  data['own']=False;data['pending'].add(ret)
  if ret not in data['returns']:uc.hook_add(UC_HOOK_CODE,returned,data,begin=ret,end=ret);data['returns'].add(ret)
 state['compiled']=c
 entry_names={0x44bb90:'restore_town_next',0x44d700:names[0x44d700],0x44d6d0:names[0x44d6d0],0x53d3a0:'FUN_0053d3a0'}
 state['entries']={(sym(n) if c else t):t for t,n in entry_names.items()}
 for a in state['entries']:u.hook_add(UC_HOOK_CODE,child,state,begin=a,end=a)

def prepare(u,fn,cfg):
 u.mem_write(0x560000,seed);u.mem_write(S,bytes(0x50000));u.mem_write(0x30000000,b'\xcc'*0x10000)
 put(u,0xeaac28,cfg.get('owner',0));put(u,0xbcb9b4,cfg.get('suppress',0));put(u,0xd3b374,cfg.get('rng',0))
 word(u,0x7848d8,cfg.get('base',0));put(u,0x78d7bc,cfg.get('year',0));put(u,0x78d7c0,cfg.get('current_month',0))
 def date(at,values):
  word(u,at+0x14,values.get('offset',0));byte(u,at+3,values.get('month',0));put(u,at+0x28,values.get('flags',0));word(u,at+0x2c,values.get('duration',12))
 date(Q,cfg)
 u.mem_write(TOWN,bytes(18628));records=cfg.get('records',[]);put(u,TOWN,len(records));put(u,0x5d9a40,0)
 for i,r in enumerate(records):
  at=TOWN+4+i*124;byte(u,at+2,r.get('kind',2));date(at,r)
 for owner in [0,1,3]:
  off=owner*0x49d8;vals=dict(pop=40,services=2,demand=100,labor=100,ratings=[100]*5,stock=40,total=100,assigned=0,need=20,city_suppress=0)
  vals.update(cfg);vals.update(cfg.get('city_overrides',{}).get(owner,{}))
  for key,at in [('pop',0xea1888),('services',0xea4388),('stock',0xea5b48),('total',0xea1894),('assigned',0xea4468),('need',0xea5bf4)]:put(u,at+off,vals[key])
  byte(u,0xea5ed8+off,vals['city_suppress']);u.mem_write(0xea5c0a+off,bytes(v&255 for v in vals['ratings']))
  for i in range(10):put(u,0xea43a0+off+i*20,vals['demand'] if i==0 else 0);put(u,0xea439c+off+i*20,vals['labor'] if i==0 else 0)
  for i,v in enumerate(cfg.get('demands',[])):put(u,0xea43a0+off+i*20,v)
  for i,v in enumerate(cfg.get('labors',[])):put(u,0xea439c+off+i*20,v)
 flag=OUT+cfg['alias'] if 'alias' in cfg else FLAG;byte(u,flag,cfg.get('flag_input',1))
 if fn in [0x44d6d0,0x44d700]:return [],Q
 if fn==0x53d3a0:return [OUT,flag],0x22222222
 if fn==0x53c060:return [],0x22222222
 actor=cfg.get('actor',1);building=cfg.get('building',2);record=building*264
 word(u,0xbcd656+actor*388,building);byte(u,0x7999d8+record,cfg.get('status',1));word(u,0x7999e8+record,cfg.get('kind',75))
 for i in range(3):word(u,0x799a86+record+i*2,0)
 for i,(linked,state,status) in enumerate(cfg.get('links',[])):
  word(u,0x799a86+record+i*2,linked)
  if linked:byte(u,0xbcd606+linked*388,status);byte(u,0xbcd630+linked*388,state)
 return [actor],0x22222222

def invoke(u,pc,args,receiver,incoming):
 for r,v in zip(registers,[incoming,receiver,0x33333333,saved[0],SP,*saved[1:]]):u.reg_write(r,v)
 u.reg_write(UC_X86_REG_EFLAGS,2);put(u,SP,STOP)
 for i,v in enumerate(args):put(u,SP+4+i*4,v)
 u.emu_start(pc,STOP,count=2000000)
 assert u.reg_read(UC_X86_REG_EIP)==STOP,(hex(pc),hex(u.reg_read(UC_X86_REG_EIP)))
 assert u.reg_read(UC_X86_REG_ESP)==SP+4
 assert [u.reg_read(registers[i]) for i in [3,5,6,7]]==saved
 return u.reg_read(UC_X86_REG_EAX)

def exercise(fn=0x53d3a0,**cfg):
 global checked,invocations,actual_cases
 expected=None
 for c,u,state in machines:
  for route in (['native'] if not c else ['named','canonical','frame']):
   state.update(collect=False,own=True,pending=set(),writes=[],events=[],cfg=cfg,fn=fn);args,receiver=prepare(u,fn,cfg);previous[0]=None;state['collect']=True;invocations+=1
   pc=fn if not c else sym(names[fn] if route=='named' else 'FUN_%08x'%fn if route=='canonical' else 'restore_actor_message_C_%08x'%fn)
   incoming=cfg.get('incoming',0xa5b6c7d8)
   try:
    if route=='frame':
     frame=S+0x1000;vsp=S+0x3000;initial=[incoming,receiver,0x33333333,saved[0],vsp,*saved[1:]];u.mem_write(frame,struct.pack('<10I',*initial,2,0));put(u,vsp,STOP)
     for i,a in enumerate(args):put(u,vsp+4+i*4,a)
     invoke(u,pc,[frame],receiver,incoming);out=struct.unpack('<8I',bytes(u.mem_read(frame,32)));assert out[4]==vsp+4;assert [out[i] for i in [3,5,6,7]]==saved;result=out[0]
    else:
     callargs=([receiver]+([incoming] if fn==0x44d700 else []) if c and route=='named' and fn in [0x44d6d0,0x44d700] else args)
     result=invoke(u,pc,callargs,receiver,incoming)
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
 if cfg.get('actual'):actual_cases+=1
 if checked%25==0:print('progress',checked,'scenarios',flush=True)

if __name__=='__main__':
 for fn in names:exercise(fn)
 if '--quick' in sys.argv:print('PASS quick',checked,invocations);sys.exit(0)
 for fn,offset,base,delta,month,current_month in itertools.product([0x44d6d0,0x44d700],[-32768,-1,0,32767],[-32768,0,32767],[-1,0,1], [0,255],[0,256]):
  exercise(fn,offset=offset,base=base,year=offset+base+delta,month=month,current_month=current_month)
 for flags,incoming,duration,year in itertools.product([0,1,4,16,17,0x100,0x80000000],[0,0xffffffff],[-32768,-1,0,32767],[0,0x80000000]):exercise(0x44d700,flags=flags,incoming=incoming,duration=duration,year=year)
 for fn in [0x44d6d0,0x44d700]:
  for current_month in [0x7fffffff,0x80000000,0xffffffff]:exercise(fn,month=255,current_month=current_month)
 for kind,status in itertools.product([0,75,192,65535],[0,1,3]):exercise(0x495b60,kind=kind,status=status)
 for slot,state in itertools.product(range(3),range(8,18)):exercise(0x495b60,links=[(0,0,0)]*slot+[(7,state,1)])
 for links in [[(7,9,0)],[(7,10,3)],[(7,0,1),(8,11,1)],[(7,0,1),(8,0,1),(9,16,1)],[(-1,12,1)]]:exercise(0x495b60,links=links)
 exercise(0x495b60,building=-1,kind=192,links=[(7,10,1)])
 for key,values in {'pop':[-1,19,20,29,30,39,40,0x80000000],'services':[-1,0,1,2], 'demand':[0,79,80,89,90,94,95,100,-1,0x7fffffff], 'labor':[0,-1,0x80000000], 'stock':[-1,0,1,19,20,39,40], 'assigned':[-1,0,5,6,10,11,17,18,100,0x7fffffff], 'total':[0,-1,0x80000000], 'need':[-1,0,1,19,20]}.items():
  for value in values:exercise(**{key:value})
 for ratings in [[20,100,100,100,100],[100,21,100,100,100],[100,100,30,100,100],[100,100,100,31,100],[100]*5,[255,100,100,100,100],[127]*5,[128]*5]:exercise(ratings=ratings)
 for records in [[],[dict(flags=0x14)],[dict(flags=0x10)],[dict(flags=0x10),dict(flags=0x14)],[dict(flags=0),dict(flags=0x14)],[dict(flags=0x14),dict(flags=0)],[dict(kind=3),dict(flags=0,duration=-1)],[dict(offset=1),dict(duration=-1)]]:
  exercise(records=records,actual=True)
 for services,suppress,city_suppress in itertools.product([0,1,2],[0,1,256],[0,1,255]):exercise(records=[dict()],services=services,suppress=suppress,city_suppress=city_suppress)
 for alias in [0,1,2,3,4,5,6,7]:
  for cfg in [dict(),dict(demand=0),dict(pop=0),dict(assigned=18),dict(need=0)]:exercise(alias=alias,**cfg)
 for owner in [1,3]:exercise(owner=owner,records=[dict()],actual=True)
 exercise(owner_after=3,city_overrides={3:dict(services=0,stock=0,assigned=18)},actual=True)
 exercise(demand=0,assigned=18);exercise(alias=6,demand=0,need=0)
 for flag_input in [0,1,255]:exercise(flag_input=flag_input)
 exercise(demands=[0xffffffff,2],labors=[0xffffffff,101]);exercise(demands=[21474836,1],labors=[100])
 for weights in [[8,6,1,0,4,6,8,4],[1]*8,[1,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,8],[0,4,0,0,0,0,0,0],[255]*8]:
  for rng in [0,1,7,31,63,64,100,127,255,0xffffffff]:exercise(0x53c060,weights=weights,rng=rng)
 for flag in [1,255]:exercise(0x53c060,default_flag=flag)
 exercise(0x53c060,weights=[8]*8,rng=0xffffffff,second_weights=[0]*8)
 for cfg in [dict(),dict(pop=0),dict(records=[dict()]),dict(records=[dict(flags=0x14)]),dict(demand=0,assigned=18,need=0,rng=0xffffffff),dict(owner=3,stock=0,rng=63)]:exercise(0x53c060,actual=True,**cfg)
 missing=points-hits;missing_edges={(a,b) for a,ends in branches.items() for b in ends if (a,b) not in edges};switch_missing={(a,b) for a,ends in switches.items() for b in ends if (a,b) not in jump_edges}
 cold={(0x53d3f3,0x53d404),(0x53d445,0x53d6bf),(0x53c0dd,0x53c0e4)}
 print('Compared message queries:',checked,'scenarios;',invocations,'invocations;',len(points&hits),'/',len(points),'instructions;',len(edges),'conditional outcomes;',len(jump_edges),'switch targets',flush=True)
 print('Unexecuted instructions:',[hex(a) for a in sorted(missing)],'; edges:',[(hex(a),hex(b)) for a,b in sorted(missing_edges)],flush=True)
 assert not missing and missing_edges==cold and not switch_missing,(missing,missing_edges,switch_missing)
 sources=['CMakeLists.txt','include/message_category.h','runtime/message_category.c','include/town_record_queries.h','runtime/town_record_queries.c','runtime/menu_actor_message.c','src/unit_00720.c','src/unit_00745.c','src/unit_00746.c','src/unit_02506.c','direct_c/verify_message_category.py','metadata/message-category-native.json','metadata/message-category-rea-evidence.json']
 proof=dict(native_sha256=catalogue['native_sha256'],compiled_C_path=str(target.resolve()),compiled_C_sha256=target_sha,passed=True,checked=checked,invocations=invocations,coverage={hex(a):dict(instructions=len(p),executed=len(p&hits)) for a,p in owned.items()},conditional_edges=len(edges),switch_targets=len(jump_edges),unexecuted_instructions=[],unexecuted_edges=[],analytically_unreachable_edges=[[hex(a),hex(b)] for a,b in sorted(cold)],source_hashes={p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest() for p in sources},actual_composition_scenarios=actual_cases,controlled_algorithm_endpoints=['53D3A0 in selector cases without actual=True'],limitations=['All iterator/calendar children execute. Controlled selector weights cover selection separately; actual=True executes the complete reached builder/list/calendar chain. Child instruction coverage is not claimed beyond the five owned methods.','Deterministic memory-hook probes check live owner/weight reloads, not concurrent execution. Private scratch and flags excluded; static memory, ordered owned writes, consumed child arguments, full EAX and saved ABI compared.','Three cold edges: first-bit16 match is already nonnull; loop category index never exceeds7; final selector JLE follows the same failed comparison at category7.','Only normal IDIV returns verified. Division by zero/overflow fault correspondence and complete actor message controller/gameplay remain unverified.'])
 suffix='diagnostic' if target.stem=='restore_link_check' else 'game';(ROOT/f'metadata/message-category-{suffix}.json').write_text(json.dumps(proof,indent=2)+'\n')
