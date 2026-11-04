"""Retained pre-device-recovery counterexamples; not a passing recovery oracle."""
from pathlib import Path
import sys,json,hashlib
ROOT=Path(__file__).resolve().parents[1]
old=json.loads((ROOT/'metadata/sound-allocation-before-counterexamples.json').read_text())
sys.argv=[sys.argv[0],old['old_C_path']]
import verify_sound_device as oracle
from verify_sound_device import *
assert target_sha==old['old_C_sha256']
alias=[]
for c,u,state in machines:
 cfg=dict(heap_results=[HEAP+0x10000,HEAP+0x11000,HEAP+0x12000,Q+0x1ac])
 setup(c,u,state,0x55e6c4,cfg);state['collect']=True;result=invoke(c,u,state,0x55e6c4,route='canonical');state['collect']=False
 assert result==('fault',) and state['faults'],(c,result,state['faults'])
 alias.append(state['faults'][0])
assert alias[0][1:]==(3,4) and alias[1][1:]==(11,4),alias
prefix=[]
for c,u,state in machines:
 cfg=dict(stop_at_version=True,old_pool=True,old_count=2)
 setup(c,u,state,0x5394f0,cfg);state['collect']=True;result=invoke(c,u,state,0x5394f0,route='canonical');state['collect']=False
 assert result==('prefix',),result
 snap=state['version_snapshot'];prefix.append(dict(handles=struct.unpack_from('<I',snap,0x19c)[0],cache=struct.unpack_from('<I',snap,0x1ac)[0],sample_count=struct.unpack_from('<I',snap,0x198)[0],counter=get(u,0x12663b0)))
assert prefix[0]['handles']==HANDLES and prefix[0]['cache']==CACHE and prefix[1]['handles']==prefix[1]['cache']==0,prefix
report=dict(native_sha256=catalogue['native_sha256'],old_C_path=str(target.resolve()),old_C_sha256=target_sha,alias_fault={'original':alias[0],'previous_C':alias[1]},pre_version_state={'original':prefix[0],'previous_C':prefix[1]},concrete_counterexamples=2,complete_reconstruction=False,limitations=['Actual native/currently retained previous C execute. One full failing initializer path proves a different fault address after cache-pointer aliasing.','The boot example stops at the first version-query entry and compares constructed object state, proving the extra late constructor; it does not claim normal-return ABI for this intentional prefix.','Only declared OS/CRT/Miles/heap services are controlled; no game algorithm result is replaced.'])
report['source_hashes']={p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest() for p in ['direct_c/probe_sound_device_before.py','direct_c/verify_sound_device.py','direct_c/verify_options_startup.py','metadata/sound-device-native.json','metadata/sound-device-rea-evidence.json']}
(ROOT/'metadata/sound-device-before-counterexamples.json').write_text(json.dumps(report,indent=2)+'\n')
print('Confirmed two retained device/boot counterexamples: alias fault3 versus11; late constructor clears the pre-version handle/cache fields')
