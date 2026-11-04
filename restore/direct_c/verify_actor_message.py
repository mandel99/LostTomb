"""Whole53C100 native/C decision tree, exact stores and real child closures."""
from verify_options_startup import *
from unicorn import UC_HOOK_MEM_WRITE,UC_HOOK_MEM_READ
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import json
catalogue=json.loads((ROOT/'metadata/actor-message-native.json').read_text())
evidence={r['evidence_id']:r for r in json.loads((ROOT/'metadata/actor-message-rea-evidence.json').read_text())['records']}
assert catalogue['native_sha256']==hashlib.sha256(original.read_bytes()).hexdigest()
f=catalogue['functions'][0];raw=bytes.fromhex(f['bytes_hex']);ev=evidence[f['bytes_evidence_id']]
assert ev['authority']=='shipped-artifact' and ev['subject']['digest']['sha256']==catalogue['native_sha256']
assert ev['normalized_result']['complete'] and ev['normalized_result']['bytes_hex']==f['bytes_hex']
assert raw==bytes(images[0].data[0x53c100-images[0].base:0x53c100-images[0].base+len(raw)])
instructions=list(Cs(CS_ARCH_X86,CS_MODE_32).disasm(raw,0x53c100));points={i.address for i in instructions}
assert len(points)==976 and points=={int(line.split(':')[0],16) for line in f['assembly'].splitlines()}
branches={i.address:{int(i.op_str,16),i.address+i.size} for i in instructions if i.mnemonic.startswith('j') and i.mnemonic!='jmp'}
for t in catalogue['tables']:
 a=int(t['address'],16);assert bytes.fromhex(t['bytes_hex'])==bytes(images[0].data[a-images[0].base:a-images[0].base+t['returned_bytes']])
switches={pc:set(struct.unpack_from('<%dI'%n,images[0].data,at-images[0].base)) for pc,at,n in [(0x53c1c6,0x53cef8,37),(0x53c342,0x53cff8,6),(0x53c376,0x53d010,2),(0x53c8f0,0x53d028,5),(0x53cea4,0x53d03c,2)]}
hits=set();edges=set();jump_edges=set();previous=[None];checked=invocations=actual_cases=0;TOWN=0x785d70;GLOBAL=0xeadcdc
calls={0x44bb90:'restore_town_next',0x44d700:'restore_event_within_window_result',0x4bf9d0:'FUN_004bf9d0',0x495b60:'FUN_00495b60',0x45cf50:'FUN_0045cf50',0x45d180:'FUN_0045d180',0x4c5120:'FUN_004c5120',0x4c52d0:'FUN_004c52d0',0x53c060:'FUN_0053c060'}
for c,u,state in machines:
 state.update(collect=False,own=True,returns=set(),compiled=c)
 def writes(uc,access,at,size,value,data):
  if not data['collect'] or not data['own']:return
  data['writes'].append((at,size,value&((1<<(8*size))-1)));cfg=data['cfg'];record=data['record']
  if cfg.get('cached_result') and at==GLOBAL and value==2:
   data['pending_global']=True
 u.hook_add(UC_HOOK_MEM_WRITE,writes,state,begin=0x560000,end=0x126cfff)
 def reads(uc,access,at,size,value,data):
  if data['collect'] and data['own'] and at==0xbcd674+data['record'] and data['cfg'].get('counter_reload') is not None:
   data['counter_reads']+=1
   if data['counter_reads']==2:byte(uc,at,data['cfg']['counter_reload'])
 def after_store(uc,pc,size,data):
  if data['collect'] and data['pending_global']:
   data['pending_global']=False;put(uc,GLOBAL,0xffffffff);byte(uc,0xbcd602+data['record'],5)
 u.hook_add(UC_HOOK_MEM_READ,reads,state,begin=0xbcd674,end=0xbcd674+8*388)
 u.hook_add(UC_HOOK_CODE,after_store,state)
 if not c:
  def visit(uc,pc,size,data):
   if data['collect'] and pc in data['pending']:data['own']=True
   if not data['collect'] or not data['own']:return
   hits.add(pc)
   if previous[0] in branches:edges.add((previous[0],pc))
   if previous[0] in switches:jump_edges.add((previous[0],pc))
   previous[0]=pc
  u.hook_add(UC_HOOK_CODE,visit,state,begin=0x53c100,end=0x53cef7)
 def returned(uc,pc,size,data):
  if data['collect'] and pc in data['pending']:data['pending'].remove(pc);data['own']=True
 def child(uc,pc,size,data):
  if not data['collect'] or not data['own']:return
  t=data['entries'][pc];sp=uc.reg_read(UC_X86_REG_ESP);ret=get(uc,sp);cfg=data['cfg'];compiled=data['compiled']
  if t==0x44bb90:args=tuple(get(uc,sp+4+i*4) for i in range(3)) if compiled else (uc.reg_read(UC_X86_REG_ECX),get(uc,sp+4),get(uc,sp+8))
  elif t in [0x44d700,0x4bf9d0]:args=(get(uc,sp+4) if compiled else uc.reg_read(UC_X86_REG_ECX),)
  else:args=tuple(get(uc,sp+4+i*4) for i in range(4 if t in [0x45cf50,0x45d180] else 2 if t==0x4c52d0 else 0 if t==0x53c060 else 1))
  data['events'].append((t,args))
  if cfg.get('actual') or t in [0x44bb90,0x44d700]:
   data['own']=False;data['pending'].add(ret)
   if ret not in data['returns']:uc.hook_add(UC_HOOK_CODE,returned,data,begin=ret,end=ret);data['returns'].add(ret)
   return
  keys={0x4bf9d0:'processing_result',0x495b60:'work_result',0x45cf50:'export_result',0x45d180:'import_result',0x4c5120:'training_result',0x4c52d0:'phase_result',0x53c060:'choice'}
  value=cfg.get(keys[t],0)
  if t==0x45cf50 and cfg.get('reload_trade'):
   put(uc,0xeaac28,3);word(uc,0xbcd656+data['record'],-1);byte(uc,0xbcd666+data['record'],255)
  if t==0x4bf9d0 and cfg.get('reload_state'):byte(uc,0xbcd630+data['record'],25)
  if t==0x4c5120 and cfg.get('reload_kind') is not None:byte(uc,0xbcd602+data['record'],cfg['reload_kind'])
  uc.reg_write(UC_X86_REG_EAX,value&0xffffffff);uc.reg_write(UC_X86_REG_ESP,sp+4);uc.reg_write(UC_X86_REG_EIP,ret)
 state['entries']={(sym(name) if c else t):t for t,name in calls.items()}
 for a in state['entries']:u.hook_add(UC_HOOK_CODE,child,state,begin=a,end=a)

def prepare(u,cfg):
 u.mem_write(0x560000,seed);u.mem_write(S,bytes(0x50000));u.mem_write(0x30000000,b'\xcc'*0x10000);u.mem_write(0x7999d8,bytes(4000*264))
 actor=cfg.get('actor',1);record=(388*(actor if actor>0 else 1))&0xffffffff
 u.mem_write(0xbcd600,bytes(8*388));byte(u,0xbcd602+record,cfg.get('kind',1));byte(u,0xbcd606+record,1);byte(u,0xbcd630+record,cfg.get('state',0));byte(u,0xbcd674+record,cfg.get('counter',0));byte(u,0xbcd65e+record,cfg.get('amount',0))
 byte(u,0xbcd677+record,cfg.get('resource',7));byte(u,0xbcd666+record,cfg.get('trader',0));word(u,0xbcd656+record,cfg.get('building',2));word(u,0xbcd652+record,cfg.get('training_building',2))
 for at,key in [(0xbcd61c,'x'),(0xbcd61e,'y'),(0xbcd624,'target_x'),(0xbcd626,'target_y')]:word(u,at+record,cfg.get(key,0))
 word(u,0xbcd67a+record,cfg.get('first_link',2));word(u,0xbcd67c+record,cfg.get('second_link',3));byte(u,0xbcd602+cfg.get('first_link',2)*388,cfg.get('first_kind',0));byte(u,0xbcd602+cfg.get('second_link',3)*388,cfg.get('second_kind',0))
 byte(u,0x7999d8+2*264,1);word(u,0x7999e8+2*264,cfg.get('building_kind',75))
 for i in range(3):word(u,0x799a86+2*264+i*2,0)
 for i,(linked,state,status) in enumerate(cfg.get('work_links',[])):word(u,0x799a86+2*264+i*2,linked);byte(u,0xbcd606+linked*388,status);byte(u,0xbcd630+linked*388,state)
 put(u,0xeaac28,cfg.get('owner',0));put(u,0xd05720+0x32fb4,cfg.get('processing_mode',3));put(u,0xbcb9b4,0);put(u,0xd3b374,cfg.get('rng',0));put(u,0xd38374,cfg.get('time',0));byte(u,0xd386dc,cfg.get('enabled',0))
 resource=cfg.get('resource',7);put(u,0x77af04+resource*88,cfg.get('trade_second',0));put(u,0x77af00+resource*88,cfg.get('trade_first',0))
 word(u,0x7848d8,0);put(u,0x78d7bc,0);put(u,0x78d7c0,0);u.mem_write(TOWN,bytes(18628));records=cfg.get('records',[]);put(u,TOWN,len(records));put(u,0x5d9a40,0)
 for i,values in enumerate(records):
  at=TOWN+4+i*124;byte(u,at+2,values.get('kind',2));word(u,at+0x14,values.get('offset',0));byte(u,at+3,values.get('month',0));put(u,at+0x28,values.get('flags',0));word(u,at+0x2c,values.get('duration',12))
 for owner in [0,1,3]:
  off=owner*0x49d8;vals=dict(city_mode=0,problem_a=0,problem_b=0,population=40,stock=40,need=60,happiness=60,workforce_a=50,workforce_b=0,workforce_c=50,workforce_d=0,issue_a=0,issue_b=0,issue_c=0)
  vals.update(cfg);vals.update(cfg.get('city_overrides',{}).get(owner,{}))
  fields={'city_mode':0xea5db0,'problem_a':0xea446c,'problem_b':0xea4474,'population':0xea1888,'stock':0xea5b48,'need':0xea5bf4,'happiness':0xea1880,'workforce_a':0xea4494,'workforce_b':0xea4490,'workforce_c':0xea449c,'workforce_d':0xea4498,'issue_a':0xea5ea4,'issue_b':0xea6210,'issue_c':0xea6090}
  for key,at in fields.items():put(u,at+off,vals[key])
  put(u,0xea4388+off,2);put(u,0xea1894+off,100);put(u,0xea4468+off,0);byte(u,0xea5ed8+off,0);u.mem_write(0xea5c0a+off,bytes([100]*5))
  for i in range(10):put(u,0xea43a0+off+i*20,100 if i==0 else 0);put(u,0xea439c+off+i*20,100 if i==0 else 0)
 return actor,record

def invoke(u,pc,args,incoming):
 for r,v in zip(registers,[incoming,0x22222222,0x33333333,saved[0],SP,*saved[1:]]):u.reg_write(r,v)
 u.reg_write(UC_X86_REG_EFLAGS,2);put(u,SP,STOP)
 for i,v in enumerate(args):put(u,SP+4+i*4,v)
 u.emu_start(pc,STOP,count=4000000)
 assert u.reg_read(UC_X86_REG_EIP)==STOP,(hex(pc),hex(u.reg_read(UC_X86_REG_EIP)))
 assert u.reg_read(UC_X86_REG_ESP)==SP+4
 assert [u.reg_read(registers[i]) for i in [3,5,6,7]]==saved
 return u.reg_read(UC_X86_REG_EAX)

def exercise(**cfg):
 global checked,invocations,actual_cases
 expected=None
 for c,u,state in machines:
  for route in (['native'] if not c else ['result','named','canonical','frame']):
   state.update(collect=False,own=True,pending=set(),writes=[],events=[],cfg=cfg,counter_reads=0,pending_global=False);actor,record=prepare(u,cfg);state['record']=record;previous[0]=None;state['collect']=True;invocations+=1
   incoming=cfg.get('incoming',0xa5b6c7d8);names={'result':'restore_actor_update_message_result','named':'restore_actor_update_message','canonical':'FUN_0053c100','frame':'restore_actor_message_C_0053c100'}
   pc=sym(names[route]) if c else 0x53c100
   try:
    if route=='frame':
     frame=S+0x1000;vsp=S+0x3000;initial=[incoming,0x22222222,0x33333333,saved[0],vsp,*saved[1:]];u.mem_write(frame,struct.pack('<10I',*initial,2,0));put(u,vsp,STOP);put(u,vsp+4,actor)
     invoke(u,pc,[frame],incoming);out=struct.unpack('<8I',bytes(u.mem_read(frame,32)));assert out[4]==vsp+4;assert [out[i] for i in [3,5,6,7]]==saved;result=out[0]
    else:result=invoke(u,pc,[actor,incoming] if route=='result' else [actor],incoming)
   except Exception as e:raise AssertionError(('execute',cfg,route,hex(u.reg_read(UC_X86_REG_EIP)))) from e
   state['collect']=False
   if 'expected' in cfg:assert get(u,GLOBAL)==cfg['expected']&0xffffffff,(cfg,route,hex(get(u,GLOBAL)))
   answer=(tuple(state['writes']),tuple(state['events']),snapshot(u),result)
   if expected is None:expected=answer
   elif answer[:3]!=expected[:3] or (route in ['result','frame'] and result!=expected[3]):
    diffs=[next(((i,a,b) for i,(a,b) in enumerate(zip(x,y)) if a!=b),None) for x,y in zip(expected[:2],answer[:2])];memory=next(((hex(0x560000+i),a,b) for i,(a,b) in enumerate(zip(expected[2],answer[2])) if a!=b),None)
    raise AssertionError((cfg,route,'stores/events',diffs,'lengths',[(len(x),len(y)) for x,y in zip(expected[:2],answer[:2])],'memory',memory,'EAX',(hex(expected[3]),hex(answer[3]))))
 checked+=1
 if cfg.get('actual'):actual_cases+=1
 if checked%25==0:print('progress',checked,'scenarios',flush=True)

if __name__=='__main__':
 for kind in range(256):exercise(kind=kind)
 if '--quick' in sys.argv:print('PASS all BYTE kinds',checked,invocations);sys.exit(0)
 for actor,incoming in itertools.product([0,-1,-2147483648],[0,0xffffffff,0x12345678]):exercise(actor=actor,incoming=incoming)
 exercise(actor=0x40000001,kind=1)
 for kind,counter in itertools.product([1,3,23,24,66,19,21,10,88],[0,1,2,3,4,12,13,254,255]):exercise(kind=kind,counter=counter)
 for kind,state in itertools.product([4,9,25,38,20,19,21,85,78,77,42,75,80,79,26,39,10,88],range(0,32)):exercise(kind=kind,state=state)
 for kind in [4,9,38]:
  for x,y,tx,ty in [(30,40,0,0),(0,0,30,40),(0,0,0,0),(0,30,40,0),(40,0,0,30),(0,24,24,0),(0,25,25,0),(-32768,32767,32767,-32768)]:exercise(kind=kind,state=9 if kind!=38 else 11,x=x,y=y,target_x=tx,target_y=ty)
 for kind,amount in itertools.product([4,9,7,8,33,88,89],[0,1,2,6,7,99,100,255]):exercise(kind=kind,state=8,amount=amount)
 for first,second in [(23,0),(0,23),(22,0),(0,22),(0,0)]:exercise(kind=88,state=5,first_kind=first,second_kind=second)
 for kind,trade_second,trade_first in itertools.product([19,20],[0,1],[0,1]):exercise(kind=kind,state=11 if kind==19 else 13,trade_second=trade_second,trade_first=trade_first)
 for result in [0,1,2,3,256,0xffffffff]:exercise(kind=20,state=10,work_result=result)
 for export_result,import_result in itertools.product([0,1,256],[0,1,256]):exercise(kind=19,state=10,export_result=export_result,import_result=import_result)
 exercise(kind=21,state=10,reload_trade=True)
 for kind,processing_result in itertools.product([85],[0,1,256,0xdeadbe01]):exercise(kind=kind,state=30,processing_result=processing_result)
 exercise(kind=85,state=30,reload_state=True)
 for category in [0,1,2,3,4,5,0xffffffff]:exercise(kind=27,training_result=category)
 for cfg in [dict(time=59,enabled=1),dict(time=60,enabled=1),dict(time=-1,enabled=255),dict(stock=19,training_result=1),dict(problem_b=11,training_result=2),dict(records=[dict()],training_result=3),dict(phase_result=1,training_result=4),dict(phase_result=256,training_result=4),dict(happiness=59,training_result=4),dict(happiness=60,need=59,training_result=4),dict(happiness=60,need=60,training_result=4),dict(training_result=5,reload_kind=1)]:exercise(kind=27,**cfg)
 for kind,key,values in [(2,'city_mode',[-1,0,1,2,3,4]),(5,'problem_a',[5,6,-1]),(5,'problem_b',[5,6,-1]),(32,'population',[39,40,-1]),(34,'population',[29,30,-1]),(42,'issue_a',[-2147483648,-1,0,1,10,11,30,31]),(89,'issue_c',[0,1,-1])]:
  for value in values:exercise(kind=kind,**{key:value})
 for kind,issue_a,issue_b,state,active in itertools.product([77,78],[0,1],[0,1],[0,10,12],[False,True]):exercise(kind=kind,issue_a=issue_a,issue_b=issue_b,state=state,records=[dict()] if active else [])
 for workforce in [(49,0,51,0),(50,0,50,0),(51,0,49,0),(-1,0,100,0),(0x7fffffff,1,1,0),(0,0,-1,0)]:exercise(kind=7,workforce_a=workforce[0],workforce_b=workforce[1],workforce_c=workforce[2],workforce_d=workforce[3])
 for choice in range(10):exercise(kind=15,choice=choice)
 for records in [[dict(kind=3)],[dict(offset=1),dict(duration=-1)],[dict(flags=0x14),dict()],[dict()]]:exercise(kind=42,issue_a=20,records=records)
 for cfg in [dict(kind=1,counter_reload=9),dict(kind=66,counter_reload=9),dict(kind=66,counter=1,cached_result=True)]:exercise(**cfg)
 for owner in [1,3,0x80000000]:exercise(kind=5,owner=owner,problem_b=6)
 actual=[dict(kind=85,state=30),dict(kind=20,state=10),dict(kind=20,state=10,work_links=[(4,10,1)]),dict(kind=19,state=10),dict(kind=21,state=10),dict(kind=27),dict(kind=15),dict(kind=7,amount=0),dict(kind=5),dict(kind=32),dict(kind=85,processing_mode=0),dict(kind=42,issue_a=20,records=[dict()]),dict(kind=10,state=0,records=[dict(flags=0x14),dict()])]
 for cfg in actual:exercise(actual=True,**cfg)
 missing=points-hits;missing_edges={(a,b) for a,ends in branches.items() for b in ends if (a,b) not in edges};switch_missing={(a,b) for a,ends in switches.items() for b in ends if (a,b) not in jump_edges}
 dead={0x53cbb5,0x53cbba,0x53cca9,0x53ccae};cold={(0x53cbb3,0x53cbb5),(0x53cca7,0x53cca9),(0x53cd0c,0x53cd2d)}
 print('Compared actor message:',checked,'scenarios;',invocations,'invocations;',len(points&hits),'/',len(points),'instructions;',len(edges),'conditional outcomes;',len(jump_edges),'switch targets',flush=True)
 print('Unexecuted instructions:',[hex(a) for a in sorted(missing)],'; edges:',[(hex(a),hex(b)) for a,b in sorted(missing_edges)],';switches',[(hex(a),hex(b)) for a,b in sorted(switch_missing)],flush=True)
 assert missing==dead and missing_edges==cold and not switch_missing,(missing,missing_edges,switch_missing)
 sources=['CMakeLists.txt','include/actor_message.h','runtime/actor_message.c','src/unit_02503.c','include/message_category.h','runtime/message_category.c','include/menu_actor_message.h','runtime/menu_actor_message.c','include/town_menu.h','src/unit_00720.c','runtime/town_record_queries.c','runtime/building_processing_query.c','runtime/storage_trade.c','include/actor_predicate.h','direct_c/verify_actor_message.py','direct_c/verify_options_startup.py','metadata/actor-message-native.json','metadata/actor-message-rea-evidence.json']
 proof=dict(native_sha256=catalogue['native_sha256'],compiled_C_path=str(target.resolve()),compiled_C_sha256=target_sha,passed=True,checked=checked,invocations=invocations,coverage={'0x53c100':dict(instructions=len(points),executed=len(points&hits),analytically_unreachable=len(dead))},conditional_edges=len(edges),switch_targets=len(jump_edges),unexecuted_instructions=[],unexecuted_edges=[],analytically_unreachable_instructions=[hex(a) for a in sorted(dead)],analytically_unreachable_edges=[[hex(a),hex(b)] for a,b in sorted(cold)],source_hashes={p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest() for p in sources},actual_composition_scenarios=actual_cases,limitations=['All BYTE actor kinds and five switch tables covered; four native instructions behind impossible BYTE>=500 comparisons cannot execute. Two guards and a repeated cached-nonzero test supply the three cold edges.','Iterator/calendar children always execute. Other direct children are controlled except actual=True, whose complete reached children execute. Deeper child instruction coverage is not claimed.','Named result and raw frame compare full EAX including inherited nonpositive-actor returns. Canonical and named void interfaces compare effects because the native caller discards EAX. Ordered owned stores, child inputs, full static memory and saved ABI checked; private scratch, incidental flags/volatile registers, concurrency, invalid IDIV and live gameplay excluded.'])
 suffix='diagnostic' if target.stem=='restore_link_check' else 'game';(ROOT/f'metadata/actor-message-{suffix}.json').write_text(json.dumps(proof,indent=2)+'\n')
