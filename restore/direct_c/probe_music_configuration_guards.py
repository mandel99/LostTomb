"""Retained pre-repair music insertion mismatches, not a passing recovery."""
from verify_sound_device import *
fn=0x5395e0;arity[fn]=6;members.add(fn);returns.add(fn)
cases=[]
for category,count,ambient in [(-1,0,0),(53,0,0),(0,51,0),(0,11,1)]:
 results=[]
 for c,u,state in machines:
  setup(c,u,state,0x539550,dict(self=MQ,poison=0))
  invoke(c,u,state,0x539550,route='canonical')
  row=(MQ+category*0x1b10)&0xffffffff;field=row+(0x1b0c if ambient else 0x1b08)
  put(u,field,count);u.mem_write(S+0x40000,b'probe.wav\0')
  state.update(fn=fn,collect=True)
  result=invoke(c,u,state,fn,[S+0x40000,category,7,20,200,ambient],route='canonical')
  state['collect']=False
  assert not state['faults'],state['faults']
  results.append(dict(result=result,count=get(u,field)))
 assert results[0]==dict(result=1,count=count+1) and results[1]==dict(result=0xffffffff,count=count),results
 cases.append(dict(category=category,old_count=count,ambient=ambient,original=results[0],previous_C=results[1]))
paths=['direct_c/probe_music_configuration_guards.py','direct_c/verify_sound_device.py','direct_c/verify_options_startup.py','build/music-configuration-before/unit_02476.c','include/music_manager.h','metadata/music-configuration-next-native.json','metadata/music-configuration-next-rea-evidence.json']
report=dict(native_sha256=catalogue['native_sha256'],compiled_C_path=str(target.resolve()),compiled_C_sha256=target_sha,concrete_counterexamples=len(cases),complete_reconstruction=False,retained_previous_C=True,cases=cases,source_hashes={p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest() for p in paths},limitations=['Four historical insertion guard mismatches execute actual native and retained pre-repair canonical C with mapped deterministic backing and normal ABI checks. This is not a current passing equivalence corpus.','Physical heap/files/audio, arbitrary unbounded traversal, malformed parser state and OS exception dispatch remain outside the probe.'])
(ROOT/'metadata/music-configuration-next-counterexamples.json').write_text(json.dumps(report,indent=2)+'\n')
print('Confirmed four retained pre-repair native/C music insertion guard counterexamples')
