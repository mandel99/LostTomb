"""Whole ambient reset/classifier/controller, with real stream/fade children.

Only seven external Miles endpoints are controlled. All game code runs offline.
"""
import verify_sound_streams as streams
from verify_options_startup import *
from unicorn import UC_HOOK_CODE
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import json,random

catalogue=json.loads((ROOT/'metadata/sound-ambient-native.json').read_text())
evidence={r['evidence_id']:r for r in json.loads((ROOT/'metadata/sound-ambient-rea-evidence.json').read_text())['records']}
assert catalogue['native_sha256']==hashlib.sha256(original.read_bytes()).hexdigest()
owned={};branches={};hits=set();edges=set();switches=set();previous=[None]
checked=invocations=compositions=fp_cases=0
for f in catalogue['functions']:
 a=int(f['address'],16);raw=bytes.fromhex(f['bytes_hex']);ev=evidence[f['bytes_evidence_id']]
 assert ev['authority']=='shipped-artifact' and ev['subject']['digest']['sha256']==catalogue['native_sha256']
 assert ev['normalized_result']['complete'] and ev['normalized_result']['bytes_hex']==f['bytes_hex']
 assert raw==bytes(images[0].data[a-images[0].base:a-images[0].base+len(raw)])
 ins=list(Cs(CS_ARCH_X86,CS_MODE_32).disasm(raw,a));owned[a]={int(p,16) for p in f['instruction_addresses'].split()};assert {i.address for i in ins}==owned[a]
 for i in ins:
  if i.mnemonic.startswith('j') and i.mnemonic!='jmp':branches[i.address]={int(i.op_str,16),i.address+i.size}
for t in catalogue['tables']:
 a=int(t['address'],16);raw=bytes.fromhex(t['bytes_hex']);ev=evidence[t['evidence_id']]
 assert ev['authority']=='shipped-artifact' and ev['subject']['digest']['sha256']==catalogue['native_sha256']
 assert ev['normalized_result']['complete'] and ev['normalized_result']['bytes_hex']==t['bytes_hex']
 assert raw==bytes(images[0].data[a-images[0].base:a-images[0].base+len(raw)])
points=set.union(*owned.values())
names={0x53b670:'restore_sound_reset_ambient_weights',0x53b830:'restore_sound_building_ambient_class',0x53b960:'restore_sound_update_ambient'}
jump_sites={0x53b846,0x53b9ea}
required_switches={(0x53b846,a) for a in [0x53b84d,0x53b853,0x53b859,0x53b85f,0x53b865]}|{(0x53b9ea,a) for a in [0x53bb05,0x53bac3,0x53ba14,0x53bbdd,0x53bc2b,0x53b9f1,0x53ba37,0x53ba5a,0x53baa0,0x53ba7d]}
for c,u,state in machines:
 # The high-byte owner256 fixture deliberately has a readable city field just
 # beyond the original static image; retain its full page and guard bytes.
 try:u.mem_read(0x133f000,4096)
 except streams.UcError:u.mem_map(0x133f000,4096)
 if not c:
  def visit(uc,pc,size,data):
   if not data['collect']:return
   if pc in points:hits.add(pc)
   if previous[0] in branches:edges.add((previous[0],pc))
   if previous[0] in jump_sites:switches.add((previous[0],pc))
   previous[0]=pc
  # Observe actual next PC, including callees, not just the owned ranges.
  u.hook_add(UC_HOOK_CODE,visit,state)
 fade=sym('FUN_0053b790') if c else 0x53b790
 def observe_fade(uc,pc,size,data):
  if data['collect']:data['fade_events'].append(get(uc,uc.reg_read(UC_X86_REG_ESP)+4))
 u.hook_add(UC_HOOK_CODE,observe_fade,state,begin=fade,end=fade)
 def mutation(uc,pc,size,data):
  if not data['collect'] or pc not in data['backend_entries']:return
  slot=data['backend_entries'][pc];cfg=data['cfg']
  # The original backend hook has supplied its result. These deterministic
  # callback writes happen before the actual game caller resumes on both sides.
  if slot==0x56f298 and cfg.get('change_selection_at_status'):
   uc.mem_write(0xf55b50,bytes(48));put(uc,0xf55b50+11*4,9999);put(uc,0xf55b80,11)
  if slot==0x56f28c and cfg.get('change_selection_at_get'):
   uc.mem_write(0xf55b50,bytes(48));put(uc,0xf55b50+10*4,7777);put(uc,0xf55b80,10)
  if slot==0x56f2d0 and cfg.get('change_selection_at_close'):
   put(uc,0xf55b80,0xabcdef01);put(uc,0xf55b50+8,0x7fffffff)
  if slot==0x56f290 and cfg.get('change_selection_at_open'):
   put(uc,0xf55b80,0x87654321);put(uc,0xf55b50+8,0x7fffffff);put(uc,0xeaac28,0x12345678)
 u.hook_add(UC_HOOK_CODE,mutation,state)

def fp_state(u):
 tags=u.reg_read(UC_X86_REG_FPTAG)
 return (u.reg_read(UC_X86_REG_FPCW),u.reg_read(UC_X86_REG_FPSW),tags,
         tuple((i,u.reg_read(UC_X86_REG_FP0+i)) for i in range(8) if (tags>>(i*2))&3!=3))

def prepare(u,state,fn,cfg):
 streams.prepare(u,state,0x55f449,dict(cfg,self=streams.OBJ,channel=2))
 u.mem_write(0x133f000,b'\x6b'*4096)
 byte(u,0xe38ec0,cfg.get('ambient_enabled',1))
 weights=cfg.get('weights')
 if weights is None:
  weights=[0]*12
  if cfg.get('selected',7)>=0:weights[cfg.get('selected',7)]=cfg.get('weight',100)
 assert len(weights)==12
 # Guard,12 weights, cached index and total; stop before sound-enabledF55B88.
 u.mem_write(0xf55b4c,b'\xa5'*60)
 for i,w in enumerate(weights):put(u,0xf55b50+i*4,w)
 put(u,0xf55b80,cfg.get('last',0x10203040));put(u,0xf55b84,cfg.get('total',100))
 owner=cfg.get('owner',0);put(u,0xeaac28,owner)
 pop_at=(0xea1894+owner*0x49d8)&0xffffffff
 if not cfg.get('invalid_population'):put(u,pop_at,cfg.get('population',2000))
 put(u,0x5f9058,cfg.get('metric',10000));put(u,0xe38ed0,cfg.get('cap',100))
 for reg,value in [(UC_X86_REG_FPCW,cfg.get('control',0x37f)),(UC_X86_REG_FPSW,cfg.get('fp_status',0)),(UC_X86_REG_FPTAG,0xffff)]:u.reg_write(reg,value)
 depth=cfg.get('depth',0)
 u.mem_write(S+0x4800,b'\xd9\xe8'*8)
 if depth:u.emu_start(S+0x4800,S+0x4800+depth*2)
 return [cfg.get('type',6)] if fn==0x53b830 else []

def restore_iat(u,c):
 if not c:
  for at in streams.backends:put(u,at,struct.unpack_from('<I',seed,at-0x560000)[0])

def exercise(fn=0x53b960,**values):
 global checked,invocations,compositions,fp_cases
 cfg={'reported_volume':98,**values};expected=None
 for c,u,state in machines:
  for route in (['native'] if not c else ['named','canonical']):
   state.update(collect=False,writes=[],events=[],method_events=[],fade_events=[],faults=[],counts={},cfg=cfg,fn=fn,slot_reads=0,open_returned=False,replacement_applied=False,volume_replaced=False,pending_store=None)
   args=prepare(u,state,fn,cfg);previous[0]=None;state['collect']=True;invocations+=1
   pc=fn if not c else sym(names[fn] if route=='named' else 'FUN_%08x'%fn)
   result=streams.invoke(u,pc,args,streams.OBJ);state['collect']=False;restore_iat(u,c)
   if isinstance(result,tuple):assert state['faults'],('unexpected execution fault',hex(fn),cfg,route,result)
   if cfg.get('expected_fault'):assert state['faults'] and state['faults'][0][1:]==cfg['expected_fault'],(hex(fn),cfg,route,state['faults'])
   elif state['faults']:raise AssertionError(('unexpected memory fault',hex(fn),cfg,route,state['faults']))
   current=(result if fn==0x53b830 else None,tuple(state['writes']),tuple(state['events']),tuple(state['method_events']),tuple(state['fade_events']),snapshot(u)+bytes(u.mem_read(0x133f000,4096)),tuple(state['faults']),fp_state(u))
   if expected is None:expected=current
   elif current!=expected:
    differences=[(i,next(((n,a,b) for n,(a,b) in enumerate(zip(x,y)) if a!=b),None),len(x),len(y)) for i,(x,y) in enumerate(zip(expected[1:5],current[1:5])) if x!=y]
    mem=next(((hex(0x560000+i),a,b) for i,(a,b) in enumerate(zip(expected[5],current[5])) if a!=b),None)
    raise AssertionError((hex(fn),cfg,route,'return',expected[0],current[0],'events',differences,'memory',mem,'faults',expected[6],current[6],'fp',expected[7],current[7]))
 checked+=1
 if fn==0x53b960 and expected[3]:compositions+=1
 if fn==0x53b960 and cfg.get('selected')==2:fp_cases+=1
 if checked%25==0:print('progress',checked,'scenarios',flush=True)

if __name__=='__main__':
 for fn in names:exercise(fn)
 exercise(selected=2);exercise(selected=1);exercise(selected=7,last=7)
 if '--quick' in sys.argv:print('PASS quick',checked,invocations);sys.exit(0)
 for t in [*range(256),-2147483648,-1,256,65542,0x7fffffff,0x80000006,0xffffffff]:exercise(0x53b830,type=t)
 for k in range(4):exercise(0x53b670,weights=[(i*0x12345678+k)&0xffffffff for i in range(12)],last=0xaabbccdd,total=0x12345678)
 for enabled,ambient,disabled,handle,status in itertools.product([0,1,256],[0,1,255],[0,1,256],[0,0x12121212],[0,4,0x104]):
  exercise(audio_enabled=enabled,ambient_enabled=ambient,disabled=disabled,handle=handle,status=status,last=7)
 for selected in range(-1,12):
  for last in [-1,selected,selected+256]:exercise(selected=selected,last=last)
 for volume in [-2147483648,-1,0,94,95,99,100,101,255,0x7fffffff]:
  for cap in [-1,0,99,100,1000]:exercise(last=7,reported_volume=volume,cap=cap)
 for owner,population in itertools.product([0,1,3,256,0x20000000],[-2147483648,-1,0,499,500,1999,2000,4999,5000,7999,8000,0x7fffffff]):exercise(selected=1,owner=owner,population=population)
 for metric in [-2147483648,-1,0,999,1000,65536,0x7fffffff]:exercise(selected=4,metric=metric)
 for weight in [-2147483648,-1,0,1,255,256,0x7fffffff]:
  for i in range(12):
   weights=[0]*12;weights[i]=weight;exercise(weights=weights,last=0xffffffff)
 for weights in [[100]*12,[0]+[100]*11,[0,100,100]+[0]*9,[-1]*12,[-2147483648]*12]:exercise(weights=weights,last=7)
 rng=random.Random(0x53b960)
 for i in range(24):exercise(weights=[rng.choice([-2147483648,-1,0,1,100,0x7fffffff]) for _ in range(12)])
 for priority,total in [(100,300),(100,400),(1,-2147483648),(0x7fffffff,0x7fffffff),(65537,200001),(16777217,50840052)]:
  for precision,rounding,depth in itertools.product([0,2,3],range(4),[0,2,6]):
   exercise(selected=2,weight=priority,total=total,control=0x7f|(precision<<8)|(rounding<<10),depth=depth)
 for depth in [7,8]:exercise(selected=2,depth=depth)
 for status in [0x4500,0x20,0x1]:exercise(selected=2,fp_status=status)
 for cfg in [dict(status_mutation=True),dict(status_mutation=True,status_handle=0),dict(change_selection_at_status=True,status=0),dict(change_selection_at_status=True),dict(change_selection_at_get=True),dict(get_volume_mutation=[100,100,300]),dict(get_volume_mutation=[100,100,300],getter_handle=0)]:exercise(last=7,**cfg)
 for selected in [-1,0,1,2,4,5,11]:
  for cfg in [dict(open_mutation=True),dict(close_mutation=True),dict(loop_mutation=True),dict(set_mutation=True),dict(change_selection_at_close=True),dict(change_selection_at_open=True),dict(opened=0),dict(status_mutation=True)]:exercise(selected=selected,**cfg)
 owner=0x12345678;exercise(selected=1,owner=owner,invalid_population=True,expected_fault=((0xea1894+owner*0x49d8)&0xffffffff,4))
 missing=points-hits;missing_edges={(a,b) for a,targets in branches.items() for b in targets if (a,b) not in edges};missing_switches=required_switches-switches
 print('Compared ambient:',checked,'scenarios;',invocations,'invocations;',len(points&hits),'/',len(points),'instructions;',len(edges),'conditional outcomes;',len(switches),'switch targets;',compositions,'actual compositions;',fp_cases,'FP cases',flush=True)
 print('Unexecuted:',[hex(a) for a in sorted(missing)],'edges:',[(hex(a),hex(b)) for a,b in sorted(missing_edges)],'switches:',[(hex(a),hex(b)) for a,b in sorted(missing_switches)],flush=True)
 assert not missing and not missing_edges and not missing_switches,(missing,missing_edges,missing_switches)
 sources=['CMakeLists.txt','runtime/sound_ambient.c','runtime/sound_ambient_profile.c','include/sound_ambient.h','runtime/sound_fades.c','include/sound_fades.h','runtime/sound_streams.c','include/sound_streams.h','include/sound_initialization.h','include/media_bindings.h','runtime/media_bindings.c','direct_c/verify_sound_ambient.py','direct_c/verify_sound_streams.py','direct_c/verify_options_startup.py','metadata/sound-ambient-native.json','metadata/sound-ambient-rea-evidence.json','metadata/sound-stream-native.json','metadata/sound-stream-rea-evidence.json']+[f'src/unit_{n:05}.c' for n in [2493,2497,2498,2496,2704,2705,2707,2708,2709,2710,2711]]
 proof=dict(native_sha256=catalogue['native_sha256'],compiled_C_path=str(target.resolve()),compiled_C_sha256=target_sha,passed=True,checked=checked,invocations=invocations,coverage={hex(a):dict(instructions=len(p),executed=len(p&hits)) for a,p in owned.items()},conditional_edges=len(edges),switch_targets=len(switches),unexecuted_instructions=[],unexecuted_edges=[],source_hashes={p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest() for p in sources},actual_composition_scenarios=compositions,fp_scenarios=fp_cases,controlled_algorithm_endpoints=[],controlled_backend_endpoints=[name for name,n in streams.backends.values()],limitations=['Three whole owned bodies; actual stream/fade children execute. Only seven Miles backend functions controlled. All static bytes, ordered writes/calls, classification full EAX, saved ABI and matching fault address/size compared. Void APIs exclude incidental EAX.','Masked x87 PC24/53/64, all four RC modes, depths0/2/6 and masked stack-overflow7/8 compare complete CW/SW/tags and live FP80 registers. FIP/FDP relocation and unmasked host exception/trap delivery are excluded.','Classification lowers immutable native dispatch/remap tables. Direction flag is clear at the ABI boundary. Deterministic backend mutations check caching/reloads, not concurrency, physical sound or complete live gameplay.'])
 suffix='diagnostic' if target.stem=='restore_link_check' else 'game';(ROOT/f'metadata/sound-ambient-{suffix}.json').write_text(json.dumps(proof,indent=2)+'\n')
