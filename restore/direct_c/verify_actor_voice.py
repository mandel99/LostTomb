"""Whole figure-code query, selection, voice-record copy and filename dispatch.

The four owned functions may execute as a real chain. Deeper sound controller
methods are controlled; no host sound device or original process is launched.
"""
from verify_options_startup import *
from unicorn import UC_HOOK_MEM_WRITE,UC_HOOK_MEM_READ
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import json
catalogue=json.loads((ROOT/'metadata/actor-voice-native.json').read_text())
evidence={r['evidence_id']:r for r in json.loads((ROOT/'metadata/actor-voice-rea-evidence.json').read_text())['records']}
assert catalogue['native_sha256']==hashlib.sha256(original.read_bytes()).hexdigest()
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
 a=int(t['address'],16);assert bytes.fromhex(t['bytes_hex'])==bytes(images[0].data[a-images[0].base:a-images[0].base+t['returned_bytes']])
switches={int(pc,16):set(struct.unpack_from('<%dI'%n,images[0].data,int(at,16)-images[0].base)) for pc,at,n in catalogue['switches']}
points=set.union(*owned.values());TABLE=0x6012ec;PATH=0xf55a20;VOLUME=0xe38ecc;IMAGE=0xeadce0;MESSAGE=0xeadcdc;NAME=S+0x20000
names={0x53d280:'restore_actor_figure_code',0x53d0b0:'restore_figure_selection_result_0053d0b0',0x53bfe0:'restore_actor_voice_variant_result',0x539a70:'restore_figure_voice_00539a70'}
entries={0x53b4a0:'figure_sound',0x4c5120:'FUN_004c5120',0x53bfe0:names[0x53bfe0],0x539a70:'FUN_00539a70',0x55f639:'FUN_0055f639',0x55f32f:'restore_sound_music_volume_0055f32f',0x55f449:'FUN_0055f449'}
def cstring(u,at,limit=256):
 raw=bytes(u.mem_read(at,limit));assert b'\0' in raw,('unterminated test name',hex(at));return raw.split(b'\0')[0]
for c,u,state in machines:
 state.update(collect=False,own=True,returns=set(),compiled=c)
 def writes(uc,access,at,size,value,data):
  if not data['collect'] or not data['own']:return
  data['writes'].append((at,size,value&((1<<(8*size))-1)));cfg=data['cfg']
  if cfg.get('tail_volume') is not None and size==1 and at==data['tail_address']:put(uc,VOLUME,cfg['tail_volume'])
  if cfg.get('word_volume') is not None and size==4 and at==data['word_address']:put(uc,VOLUME,cfg['word_volume'])
 u.hook_add(UC_HOOK_MEM_WRITE,writes,state,begin=0x560000,end=0x126cfff)
 def reads(uc,access,at,size,value,data):
  if data['collect'] and data['own'] and at==TABLE+data['cfg'].get('kind',1)*4 and data['cfg'].get('building_at_lookup') is not None:
   word(uc,0xbcd652+data['record'],data['cfg']['building_at_lookup'])
 u.hook_add(UC_HOOK_MEM_READ,reads,state,begin=TABLE,end=TABLE+1023)
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
  sp=uc.reg_read(UC_X86_REG_ESP);ret=get(uc,sp);cfg=data['cfg'];compiled=data['compiled'];count={0x53b4a0:3,0x4c5120:1,0x53bfe0:2,0x539a70:1,0x55f639:2,0x55f32f:1,0x55f449:3}[t]
  args=tuple(get(uc,sp+4+i*4) for i in range(count+(1 if compiled and t in [0x55f639,0x55f32f,0x55f449] else 0)))
  if t in [0x55f639,0x55f32f,0x55f449] and not compiled:args=(uc.reg_read(UC_X86_REG_ECX),*args)
  if t==0x539a70:observed=(cstring(uc,args[0]),bytes(uc.mem_read(args[0],32)))
  elif t==0x55f449:observed=(args[0],cstring(uc,args[1]),*args[2:])
  else:observed=args
  data['events'].append((t,observed));data['counts'][t]=data['counts'].get(t,0)+1
  actual=cfg.get('actual') and t in [0x53b4a0,0x4c5120,0x53bfe0,0x539a70]
  if actual:
   # The owned voice chain remains observable. Nonowned dependencies are
   # excluded from owned stores/coverage until their real return.
   if t not in owned:
    data['own']=False;data['pending'].add(ret)
    if ret not in data['returns']:uc.hook_add(UC_HOOK_CODE,returned,data,begin=ret,end=ret);data['returns'].add(ret)
   return
  value=cfg.get('training_result',0) if t==0x4c5120 else cfg.get('voice_result',0xdeadbeef) if t in [0x53bfe0,0x539a70] else cfg.get('stream_result',0x89abcdef) if t==0x55f449 else 0
  if t==0x4c5120:
   if cfg.get('training_sequence'):value=cfg['training_sequence'][(data['counts'][t]-1)%len(cfg['training_sequence'])]
   if cfg.get('training_images'):put(uc,IMAGE,cfg['training_images'][(data['counts'][t]-1)%len(cfg['training_images'])])
  if t==0x53b4a0 and cfg.get('sound_mutation'):
   byte(uc,0xbcd602+data['record'],cfg['sound_mutation']);put(uc,0xeadce4,99)
  if t==0x55f639 and cfg.get('volume_after_first') is not None:put(uc,VOLUME,cfg['volume_after_first'])
  if t==0x55f32f and cfg.get('path_after_second') is not None:uc.mem_write(PATH,cfg['path_after_second'].encode()+b'\0')
  cleanup=count*4 if not compiled and t in [0x55f639,0x55f32f,0x55f449] else 0
  uc.reg_write(UC_X86_REG_EAX,value&0xffffffff);uc.reg_write(UC_X86_REG_ESP,sp+4+cleanup);uc.reg_write(UC_X86_REG_EIP,ret)
 state['entries']={(sym(name) if c else t):t for t,name in entries.items()}
 for a in state['entries']:u.hook_add(UC_HOOK_CODE,child,state,begin=a,end=a)

def prepare(u,fn,cfg,state):
 u.mem_write(0x560000,seed);u.mem_write(S,bytes(0x50000));u.mem_write(0x30000000,b'\xcc'*0x10000);u.mem_write(PATH,bytes(256))
 actor=cfg.get('actor',1);record=(actor*388)&0xffffffff;state['record']=record;kind=cfg.get('kind',1);secondary=cfg.get('secondary_kind',2)
 byte(u,(0xbcd602+record)&0xffffffff,kind);byte(u,(0xbcd603+record)&0xffffffff,secondary);word(u,(0xbcd652+record)&0xffffffff,cfg.get('building',7));byte(u,(0xbcd675+record)&0xffffffff,cfg.get('variant',0))
 if 'code' in cfg:put(u,TABLE+kind*4,cfg['code'])
 if 'secondary_code' in cfg:put(u,TABLE+secondary*4,cfg['secondary_code'])
 put(u,0x784cb0,cfg.get('scenario_index',0));put(u,0x5e4c20+(cfg.get('scenario_index',0)&255)*48,cfg.get('scenario_type',68))
 put(u,0xeadce4,cfg.get('selected',0));put(u,(0xeadd24+cfg.get('selected',0)*4)&0xffffffff,actor);put(u,IMAGE,0xa5a55a5a);put(u,MESSAGE,0x5a5aa5a5)
 put(u,0xf55b88,cfg.get('audio_enabled',0));byte(u,0xe38e65,cfg.get('voice_enabled',1));byte(u,0xe38e63,0);put(u,VOLUME,cfg.get('volume',51))
 prefix=cfg.get('prefix','audio/wavs/').encode();name=cfg.get('filename','unit.wav').encode();u.mem_write(0x5f90a4,prefix+b'\0');u.mem_write(NAME,name+b'\0');name_at=cfg.get('name_at',NAME)
 if name_at!=NAME:u.mem_write(name_at,name+b'\0')
 state['tail_address']=PATH+len(prefix)+((len(name)+1)&~3);state['word_address']=PATH+len(prefix)
 if cfg.get('custom_record'):
  bank=cfg.get('bank',1);variant=cfg.get('variant',0);source=(0x60110c+variant*24 if bank==108 else 0x5fb16c+(variant-20+bank*20)*24)&0xffffffff
  u.mem_write(source,name+b'\0')
 if fn==0x53d280:return [actor]
 if fn==0x53d0b0:return []
 if fn==0x53bfe0:return [cfg.get('bank',1),cfg.get('variant',0)]
 return [name_at]

def invoke(u,pc,args,incoming):
 for r,v in zip(registers,[incoming,0x22222222,0x33333333,saved[0],SP,*saved[1:]]):u.reg_write(r,v)
 u.reg_write(UC_X86_REG_EFLAGS,2);put(u,SP,STOP)
 for i,v in enumerate(args):put(u,SP+4+i*4,v)
 u.emu_start(pc,STOP,count=4000000)
 assert u.reg_read(UC_X86_REG_EIP)==STOP,(hex(pc),hex(u.reg_read(UC_X86_REG_EIP)))
 assert u.reg_read(UC_X86_REG_ESP)==SP+4
 assert [u.reg_read(registers[i]) for i in [3,5,6,7]]==saved
 return u.reg_read(UC_X86_REG_EAX)

def exercise(fn=0x53d280,**cfg):
 global checked,invocations,actual_cases
 expected=None
 for c,u,state in machines:
  routes=['native'] if not c else ['result','canonical',*(['frame'] if fn==0x53d280 else ['named'] if fn in [0x53bfe0,0x53d0b0] else [])]
  for route in routes:
   state.update(collect=False,own=True,pending=set(),writes=[],events=[],counts={},cfg=cfg,fn=fn);args=prepare(u,fn,cfg,state);previous[0]=None;state['collect']=True;invocations+=1;incoming=cfg.get('incoming',0xa5b6c7d8)
   pc=fn if not c else sym(names[fn] if route=='result' else 'FUN_%08x'%fn if route=='canonical' else 'restore_menu_overlay_C_0053d280' if route=='frame' else 'restore_actor_voice_variant' if fn==0x53bfe0 else 'restore_figure_selection_0053d0b0')
   try:
    if route=='frame':
     frame=S+0x1000;vsp=S+0x3000;initial=[incoming,0x22222222,0x33333333,saved[0],vsp,*saved[1:]];u.mem_write(frame,struct.pack('<10I',*initial,2,0));put(u,vsp,STOP);put(u,vsp+4,args[0]);invoke(u,pc,[frame],incoming);out=struct.unpack('<8I',bytes(u.mem_read(frame,32)));assert out[4]==vsp+4;assert [out[i] for i in [3,5,6,7]]==saved;result=out[0]
    else:result=invoke(u,pc,[*args,incoming] if route=='result' and fn==0x53bfe0 else args,incoming)
   except Exception as e:raise AssertionError(('execute',hex(fn),cfg,route,hex(u.reg_read(UC_X86_REG_EIP)))) from e
   state['collect']=False
   compare_result=not(c and fn in [0x53d0b0,0x53bfe0] and route in ['canonical','named'])
   if 'expected' in cfg and compare_result:assert result==cfg['expected']&0xffffffff,(cfg,route,hex(result))
   if 'first_volume' in cfg:assert next(args[-1] for t,args in state['events'] if t==0x55f639)==cfg['first_volume'],(cfg,route,state['events'])
   answer=(tuple(state['writes']),tuple(state['events']),snapshot(u),result)
   if expected is None:expected=answer
   elif answer[:3]!=expected[:3] or (compare_result and result!=expected[3]):
    diffs=[next(((i,a,b) for i,(a,b) in enumerate(zip(x,y)) if a!=b),None) for x,y in zip(expected[:2],answer[:2])];memory=next(((hex(0x560000+i),a,b) for i,(a,b) in enumerate(zip(expected[2],answer[2])) if a!=b),None)
    raise AssertionError((hex(fn),cfg,route,'stores/events',diffs,'lengths',[(len(x),len(y)) for x,y in zip(expected[:2],answer[:2])],'memory',memory,'EAX',(hex(expected[3]),hex(answer[3]))))
 checked+=1
 if cfg.get('actual'):actual_cases+=1
 if checked%25==0:print('progress',checked,'scenarios',flush=True)

def verify_overflow_copy_extent():
 global checked,invocations
 for length in [32,40]:
  expected=b'X'*length+b'\0'
  for c,u,state in machines:
   state['collect']=False;prepare(u,0x53bfe0,dict(bank=1,custom_record=True,filename='X'*length),state)
   pc=0x53bfe0 if not c else sym(names[0x53bfe0]);stop=0x539a70 if not c else sym('FUN_00539a70')
   for r,v in zip(registers,[0xa5b6c7d8,0x22222222,0x33333333,saved[0],SP,*saved[1:]]):u.reg_write(r,v)
   u.reg_write(UC_X86_REG_EFLAGS,2);put(u,SP,STOP);put(u,SP+4,1);put(u,SP+8,0);put(u,SP+12,0xa5b6c7d8)
   if not state.get('probe_hook'):
    def stop_probe(cpu,at,size,data):
     if data.get('probe_stop'):cpu.emu_stop()
    state['probe_hook']=u.hook_add(UC_HOOK_CODE,stop_probe,state,begin=stop,end=stop)
   state['probe_stop']=True;u.emu_start(pc,STOP,count=100000);state['probe_stop']=False
   assert u.reg_read(UC_X86_REG_EIP)==stop,(length,c,hex(u.reg_read(UC_X86_REG_EIP)),hex(stop))
   target=get(u,u.reg_read(UC_X86_REG_ESP)+4)
   assert bytes(u.mem_read(target,length+1))==expected,('untruncated overflow copy',length,c)
   invocations+=1
  checked+=1

if __name__=='__main__':
 for fn in names:exercise(fn)
 if '--quick' in sys.argv:print('PASS quick',checked,invocations);sys.exit(0)
 for kind in range(256):exercise(kind=kind)
 for kind in range(109):exercise(0x53d0b0,kind=kind)
 for code in [-2147483648,-5,-4,-3,-2,-1,0,1,200,201,230,0x7fffffff]:exercise(code=code)
 for code in [-2147483648,-5,-4,-3,-2,-1,0,1,200,201,230,0x7fffffff]:
  exercise(code=-4,secondary_code=code);exercise(0x53d0b0,code=-4,secondary_code=code)
 for result in [0,1,2,3,4,5,0xffffffff,0x80000000]:
  for code,secondary in [(-3,0),(-4,-3)]:
   exercise(code=code,secondary_code=secondary,training_result=result);exercise(0x53d0b0,code=code,secondary_code=secondary,training_result=result)
 for scenario in [0,67,68,69,70,71,0x10044,0xffffffff]:
  exercise(code=-2,scenario_type=scenario,scenario_index=0x123456ff);exercise(0x53d0b0,code=-2,scenario_type=scenario,scenario_index=255)
 for building in [-32768,-1,0,32767]:exercise(code=-3,building=building)
 exercise(code=-3,building=7,building_at_lookup=-1);exercise(0x53d0b0,code=-3,building=7,building_at_lookup=-1)
 exercise(0x53d0b0,code=-3,secondary_code=-3,training_sequence=[5,2],training_images=[-4,-1])
 exercise(0x53d0b0,code=-4,secondary_code=-3,training_result=5,training_images=[-1])
 exercise(0x53d0b0,kind=83,sound_mutation=2,secondary_code=230)
 for actor in [0,-1,-2147483648,0x40000001]:exercise(0x53d0b0,actor=actor,code=230)
 for actor in [0,-1,0x40000001]:exercise(actor=actor,code=230)
 for selected in [-1,0,1,255]:exercise(0x53d0b0,selected=selected,code=230)
 for bank,variant,incoming in itertools.product([0,-1,-2147483648],[0,-1],[0,0xffffffff]):exercise(0x53bfe0,bank=bank,variant=variant,incoming=incoming)
 for bank in [*range(1,53),108]:exercise(0x53bfe0,bank=bank,variant=0)
 for bank,variant in [(1,19),(51,19),(108,19),(0x40000001,0),(1,-1)]:exercise(0x53bfe0,bank=bank,variant=variant)
 for length in range(0,12):exercise(0x53bfe0,filename='x'*length,custom_record=True)
 for enabled,gate in [(0,0),(0,1),(1,0),(1,1),(256,1),(1,255)]:exercise(0x539a70,audio_enabled=enabled,voice_enabled=gate)
 for prefix_length,name_length in itertools.product(range(0,5),range(0,8)):exercise(0x539a70,audio_enabled=1,prefix='p'*prefix_length,filename='x'*name_length)
 exercise(0x539a70,audio_enabled=1,volume=-1,stream_result=0)
 exercise(0x539a70,audio_enabled=1,stream_result=0xffffffff,volume_after_first=0x12345678,path_after_second='changed.wav')
 exercise(0x539a70,audio_enabled=1,tail_volume=999,first_volume=51)
 exercise(0x539a70,audio_enabled=1,word_volume=777,tail_volume=999,first_volume=777)
 exercise(0x539a70,audio_enabled=1,name_at=PATH+1,prefix='audio/',filename='old')
 for cfg in [dict(),dict(code=201),dict(code=230),dict(code=-3),dict(code=-4,secondary_code=-3),dict(code=-2,scenario_type=69),dict(code=230,variant=255),dict(kind=83,code=230)]:exercise(0x53d0b0,actual=True,**cfg)
 for cfg in [dict(code=230,audio_enabled=1),dict(code=-3,audio_enabled=1),dict(code=230,audio_enabled=1,tail_volume=999)]:exercise(0x53d0b0,actual=True,**cfg)
 for cfg in [dict(),dict(bank=108),dict(bank=1,variant=19,audio_enabled=1)]:exercise(0x53bfe0,actual=True,**cfg)
 exercise(code=-3,actual=True);exercise(code=-4,secondary_code=-3,actual=True)
 verify_overflow_copy_extent()
 missing=points-hits;missing_edges={(a,b) for a,ends in branches.items() for b in ends if (a,b) not in edges};switch_missing={(a,b) for a,ends in switches.items() for b in ends if (a,b) not in jump_edges}
 print('Compared actor voice:',checked,'scenarios;',invocations,'invocations;',len(points&hits),'/',len(points),'instructions;',len(edges),'conditional outcomes;',len(jump_edges),'switch targets',flush=True)
 print('Unexecuted instructions:',[hex(a) for a in sorted(missing)],'; edges:',[(hex(a),hex(b)) for a,b in sorted(missing_edges)],';switches',[(hex(a),hex(b)) for a,b in sorted(switch_missing)],flush=True)
 assert not missing and not missing_edges and not switch_missing,(missing,missing_edges,switch_missing)
 sources=['CMakeLists.txt','include/figure_menu_actions.h','runtime/figure_menu_actions.c','runtime/menu_building_overlay.c','runtime/menu_building_overlay_model.c','include/menu_building_overlay.h','include/actor_predicate.h','include/sound_streams.h','include/sound_initialization.h','src/unit_02501.c','src/unit_02504.c','src/unit_02505.c','direct_c/verify_actor_voice.py','direct_c/verify_options_startup.py','metadata/actor-voice-native.json','metadata/actor-voice-rea-evidence.json']
 proof=dict(native_sha256=catalogue['native_sha256'],compiled_C_path=str(target.resolve()),compiled_C_sha256=target_sha,passed=True,checked=checked,invocations=invocations,coverage={hex(a):dict(instructions=len(p),executed=len(p&hits)) for a,p in owned.items()},conditional_edges=len(edges),switch_targets=len(jump_edges),unexecuted_instructions=[],unexecuted_edges=[],source_hashes={p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest() for p in sources},actual_composition_scenarios=actual_cases,unsafe_copy_extent_probes=[32,40],unsafe_copy_probe_stops_before_corrupted_return=True,controlled_algorithm_endpoints=['53B4A0 and4C5120 unless actual=True','Owned53BFE0/539A70 children unless actual=True','55F639/55F32F/55F449 sound controller methods in all enabled-voice cases'],limitations=['The four owned functions execute as a real chain in actual=True cases; controlled isolated calls exercise all switch targets and native load phases. Real sound/training children execute in actual=True, with incidental sound disabled.','Enabled voice assembly, exact NUL-inclusive DWORD/BYTE copies,32-byte zero padding, receiver/volume reloads and native full-EAX result routes verified. Named/canonical void routes check effects, not their incidental EAX.','Ordered owned static stores, consumed child inputs/filename bytes, complete original static-memory range and saved ABI checked. Private stack addresses normalized; flag/incidental volatile-register equivalence, post-overflow stack/return behavior beyond the two explicit untruncated-copy probes, concurrency, physical sound and live UI excluded.'])
 suffix='diagnostic' if target.stem=='restore_link_check' else 'game';(ROOT/f'metadata/actor-voice-{suffix}.json').write_text(json.dumps(proof,indent=2)+'\n')
