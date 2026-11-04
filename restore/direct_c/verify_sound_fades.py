"""Ambient/sample receivers and three whole fade functions, real stream children.

Reuse the already verified seven-Miles-endpoint fixture. Algorithm children
execute; only external backend results and documented mutation phases vary.
"""
import verify_sound_streams as streams
from verify_options_startup import *
from unicorn import UC_HOOK_CODE
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import json
catalogue=json.loads((ROOT/'metadata/sound-fade-native.json').read_text())
evidence={r['evidence_id']:r for r in json.loads((ROOT/'metadata/sound-fade-rea-evidence.json').read_text())['records']}
assert catalogue['native_sha256']==hashlib.sha256(original.read_bytes()).hexdigest()
owned={};branches={};hits=set();edges=set();previous=[None];checked=invocations=compositions=0
for f in catalogue['functions']:
 a=int(f['address'],16);raw=bytes.fromhex(f['bytes_hex']);ev=evidence[f['bytes_evidence_id']]
 assert ev['authority']=='shipped-artifact' and ev['subject']['digest']['sha256']==catalogue['native_sha256']
 assert ev['normalized_result']['complete'] and ev['normalized_result']['bytes_hex']==f['bytes_hex']
 assert raw==bytes(images[0].data[a-images[0].base:a-images[0].base+len(raw)])
 ins=list(Cs(CS_ARCH_X86,CS_MODE_32).disasm(raw,a));owned[a]={int(p,16) for p in f['instruction_addresses'].split()};assert {i.address for i in ins}==owned[a]
 for i in ins:
  if i.mnemonic.startswith('j') and i.mnemonic!='jmp':branches[i.address]={int(i.op_str,16),i.address+i.size}
points=set.union(*owned.values())
names={0x55f3eb:'restore_sound_set_ambient_volume',0x55f67e:'restore_sound_set_sample_volume',0x53b690:'restore_sound_fade_to_stop',0x53b6f0:'restore_sound_fade_out_step',0x53b790:'restore_sound_fade_in_step'}
members={0x55f3eb,0x55f67e};fades=set(names)-members
for c,u,state in machines:
 if not c:
  def visit(uc,pc,size,data):
   if not data['collect']:return
   hits.add(pc)
   if previous[0] in branches:edges.add((previous[0],pc))
   previous[0]=pc
  for p in owned.values():u.hook_add(UC_HOOK_CODE,visit,state,begin=min(p),end=max(p))
 def stop_prefix(uc,pc,size,data):
  if data['collect'] and data.get('prefix_limit') and data['counts'].get(0x56f29c,0)>=data['prefix_limit']:uc.emu_stop()
 u.hook_add(UC_HOOK_CODE,stop_prefix,state)

def prepare(u,state,fn,cfg):
 sound_cfg=dict(cfg)
 if fn in fades:sound_cfg['self']=streams.OBJ
 channel=2 if fn==0x55f3eb else cfg.get('channel',0)
 sound_cfg['channel']=0 if cfg.get('unmapped_channel') else channel
 streams.prepare(u,state,0x55f639,sound_cfg)
 if cfg.get('unmapped_channel'):state['slot']=(streams.OBJ+0x1d8+(channel&0xffffffff)*4)&0xffffffff
 put(u,0xf55b8c,cfg.get('fade0',0xa5b6c7d8));put(u,0xf55b90,cfg.get('fade1',0x12345678))
 for at,key,default in [(0xe38ec8,'cap0',100),(0xe38ecc,'cap1',300),(0xe38ed0,'cap2',65536)]:put(u,at,cfg.get(key,default))
 return [cfg.get('volume',51)] if fn in members else [channel]

def reset_state(state,fn,cfg):
 state.update(collect=False,writes=[],events=[],method_events=[],faults=[],counts={},cfg=cfg,fn=fn,slot_reads=0,open_returned=False,replacement_applied=False,volume_replaced=False,pending_store=None,prefix_limit=0)

def restore_iat(u,c):
 if not c:
  for at in streams.backends:put(u,at,struct.unpack_from('<I',seed,at-0x560000)[0])

def answer(u,state):
 return (tuple(state['writes']),tuple(state['events']),tuple(state['method_events']),snapshot(u),bytes(u.mem_read(S+0xf000,0x5000)),tuple(state['faults']))

def compare(expected,current,fn,cfg,route):
 if current==expected:return
 diff=[next(((i,a,b) for i,(a,b) in enumerate(zip(x,y)) if a!=b),None) for x,y in zip(expected[:3],current[:3])]
 memory=next(((hex(0x560000+i),a,b) for i,(a,b) in enumerate(zip(expected[3],current[3])) if a!=b),None)
 raise AssertionError((hex(fn),cfg,route,'stores/backend/methods',diff,'lengths',[(len(a),len(b)) for a,b in zip(expected[:3],current[:3])],'memory',memory,'faults',expected[-1],current[-1]))

def exercise(fn=0x53b6f0,**values):
 global checked,invocations,compositions
 cfg={'reported_volume':11,**values};expected=None
 for c,u,state in machines:
  for route in (['native'] if not c else ['named','canonical',*(['legacy'] if fn==0x55f3eb else [])]):
   reset_state(state,fn,cfg);args=prepare(u,state,fn,cfg);previous[0]=None;state['collect']=True;invocations+=1
   pc=fn if not c else sym(names[fn] if route=='named' else 'restore_settings_sound_55f3eb' if route=='legacy' else 'FUN_%08x'%fn)
   callargs=[state['self'],*args] if c and fn in members else args
   result=streams.invoke(u,pc,callargs,state['self'],4 if not c and fn in members else 0);state['collect']=False;restore_iat(u,c)
   if isinstance(result,tuple):assert state['faults'],('unexpected execution fault',fn,cfg,route,result)
   if cfg.get('expected_fault'):assert state['faults'] and state['faults'][0][1:]==cfg['expected_fault'],(fn,cfg,route,state['faults'])
   elif state['faults']:raise AssertionError(('unexpected memory fault',fn,cfg,route,state['faults']))
   current=answer(u,state)
   if expected is None:expected=current
   else:compare(expected,current,fn,cfg,route)
 checked+=1
 if fn in fades and expected[2]:compositions+=1
 if checked%25==0:print('progress',checked,'scenarios',flush=True)

def large_fade_prefix():
 global checked,invocations
 fn=0x53b690;cfg=dict(channel=0,reported_volume=0x7fffffff);expected=None
 for c,u,state in machines:
  for route in (['native'] if not c else ['named','canonical']):
   reset_state(state,fn,cfg);args=prepare(u,state,fn,cfg);state['prefix_limit']=3;state['collect']=True;previous[0]=None;pc=fn if not c else sym(names[fn] if route=='named' else 'FUN_0053b690')
   for r,v in zip(registers,[0xa5b6c7d8,state['self'],0x33333333,saved[0],SP,*saved[1:]]):u.reg_write(r,v)
   u.reg_write(UC_X86_REG_EFLAGS,2);put(u,SP,STOP);put(u,SP+4,args[0]);u.emu_start(pc,STOP,count=200000)
   state['collect']=False;state['prefix_limit']=0;restore_iat(u,c);invocations+=1
   volumes=[args[1] for name,args in state['events'] if name=='_AIL_set_stream_volume_8'];assert volumes==[0x7ffffffa,0x7ffffff5,0x7ffffff0],(c,route,volumes)
   current=answer(u,state)
   if expected is None:expected=current
   else:compare(expected,current,fn,cfg,route)
 checked+=1

if __name__=='__main__':
 for fn in names:exercise(fn)
 if '--quick' in sys.argv:print('PASS quick',checked,invocations);sys.exit(0)
 for fn in members:
  for volume,disabled,status,handle in itertools.product([-2147483648,-1,0,255,300,0x7fffffff],[0,1,256],[0,4,0x104],[0,0x12121212]):exercise(fn,volume=volume,disabled=disabled,status=status,handle=handle)
  exercise(fn,self=S+0x10000,volume=-1);exercise(fn,self=0,expected_fault=(0x158 if fn==0x55f3eb else 0x150,4))
 for cfg in [dict(second_handle=0x55555555),dict(status_mutation=True),dict(status_mutation=True,status_handle=0)]:exercise(0x55f3eb,**cfg)
 for fn in fades:
  for channel,enabled,handle,status in itertools.product([-1,0,1,2,3,0x40000001],[0,1,256],[0,0x12121212],[0,4,0x104]):exercise(fn,channel=channel,audio_enabled=enabled,handle=handle,status=status)
 for channel in [0,1,2,3,-1]:
  for volume in [-2147483648,-2147483647,-1,0,1,4,5,6,11,0x7fffffff]:exercise(0x53b6f0,channel=channel,reported_volume=volume)
  for volume in [-2147483648,-1,0,1,4,5,6,11,27]:exercise(0x53b690,channel=channel,reported_volume=volume)
  for volume in [-2147483648,-1,0,1,255,300,0x7fffffff]:exercise(0x53b790,channel=channel,reported_volume=volume)
 for channel,cap in itertools.product([0,1,2],[-1,0,10,15,16,255,300,65536]):exercise(0x53b790,channel=channel,cap0=cap,cap1=cap,cap2=cap,reported_volume=11)
 for fn in fades:
  exercise(fn,status_mutation=True)
  exercise(fn,channel=1,close_mutation=True)
  exercise(fn,channel=2,getter_handle=0,get_volume_mutation=[1000,1000,1000])
  exercise(fn,channel=0,set_mutation=True,set_handle=0)
  exercise(fn,channel=0,second_handle=0x77777777)
  exercise(fn,channel=0,unmapped_channel=True,audio_enabled=0)
  channel=0x12345678;exercise(fn,channel=channel,unmapped_channel=True,expected_fault=((0xe92dd0+channel*4)&0xffffffff,4))
 for channel in [0,1,2]:exercise(0x53b790,channel=channel,reported_volume=100,get_volume_mutation=[300,400,500])
 exercise(0x53b790,channel=0x40000001,reported_volume=400,cap1=1)
 large_fade_prefix()
 missing=points-hits;missing_edges={(a,b) for a,targets in branches.items() for b in targets if (a,b) not in edges}
 print('Compared sound fades:',checked,'scenarios;',invocations,'invocations;',len(points&hits),'/',len(points),'instructions;',len(edges),'conditional outcomes;',compositions,'actual child compositions',flush=True)
 print('Unexecuted instructions:',[hex(a) for a in sorted(missing)],'; edges:',[(hex(a),hex(b)) for a,b in sorted(missing_edges)],flush=True)
 assert not missing and not missing_edges,(missing,missing_edges)
 sources=['CMakeLists.txt','runtime/sound_fades.c','include/sound_fades.h','runtime/settings_callbacks_audio.c','include/settings_callbacks.h','runtime/sound_streams.c','include/sound_streams.h','include/sound_initialization.h','include/media_bindings.h','runtime/media_bindings.c','direct_c/verify_sound_fades.py','direct_c/verify_sound_streams.py','direct_c/verify_options_startup.py','metadata/sound-fade-native.json','metadata/sound-fade-rea-evidence.json','metadata/sound-stream-native.json','metadata/sound-stream-rea-evidence.json']+[f'src/unit_{n:05}.c' for n in [2706,2712,2494,2495,2496,2704,2705,2707,2708,2709,2710,2711]]
 proof=dict(native_sha256=catalogue['native_sha256'],compiled_C_path=str(target.resolve()),compiled_C_sha256=target_sha,passed=True,checked=checked,invocations=invocations,coverage={hex(a):dict(instructions=len(p),executed=len(p&hits)) for a,p in owned.items()},conditional_edges=len(edges),unexecuted_instructions=[],unexecuted_edges=[],source_hashes={p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest() for p in sources},actual_composition_scenarios=compositions,controlled_algorithm_endpoints=[],controlled_backend_endpoints=[name for name,n in streams.backends.values()],stopped_large_positive_fade_prefix=dict(initial=0x7fffffff,steps=3,values=[0x7ffffffa,0x7ffffff5,0x7ffffff0],full_completion_not_executed=True),limitations=['Five whole owned bodies; all reached stream-method algorithms execute with only seven Miles backend endpoints controlled. Legacy ambient helper, named APIs and canonical aliases compared.','All new original APIs are void: consumed memory/backend/method effects and saved ABI are checked, not incidental EAX. Full static memory, private receiver fixture, exact owned writes and matching fault address/size are compared.','Immediate fades use finite normal-return volumes; INT_MAX is checked as a stopped three-call prefix. Its429496730-step full run is not claimed. No production loop limit/clamp is added. Deterministic callback probes do not prove concurrency, physical sound or complete gameplay.'])
 suffix='diagnostic' if target.stem=='restore_link_check' else 'game';(ROOT/f'metadata/sound-fade-{suffix}.json').write_text(json.dumps(proof,indent=2)+'\n')
