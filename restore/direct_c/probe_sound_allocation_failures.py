"""Retain concrete remaining 55DEA2 divergences; no game/audio process runs.

This is a historical counterexample probe, not a passing recovery oracle.
Only heap allocation/free and Miles handle allocation/release are controlled.
"""
from pathlib import Path
import hashlib,json,shutil,sys
ROOT=Path(__file__).resolve().parents[1]
current=ROOT/'build/c-only/Release/PharaohRestoreC.exe'
retained=ROOT/'build/sound-allocation-before'/current.name
retained.parent.mkdir(exist_ok=True)
if not retained.exists():
 shutil.copy2(current,retained);shutil.copy2(current.with_suffix('.map'),retained.with_suffix('.map'))
sys.argv=[sys.argv[0],str(retained)]
from verify_options_startup import *
OBJ=S+0x1000;HEAP=S+0x10000
def capture(c,u,state,requested=3,fail_heap=0,fail_handle=0):
 u.mem_write(0x560000,seed);u.mem_write(S,b'\xcd'*0x50000);u.mem_write(OBJ,bytes(0x204));put(u,OBJ+0x198,3);put(u,OBJ+0x178,0x12345678)
 state['hooks'].clear();state['calls'].clear();events=[];counts={};cursor=[HEAP]
 def heap(uc,args):
  counts['heap']=counts.get('heap',0)+1;n=counts['heap'];value=0 if n==fail_heap else cursor[0];cursor[0]+=0x1000
  events.append(('allocate',args[0],value));return value
 def free(uc,args):events.append(('free',args[0]));return 0
 hook(c,state,0x561a1d,1,heap);hook(c,state,0x561a2b,1,free)
 for slot,name in [(0x56f2f4,'_AIL_allocate_sample_handle_4'),(0x56f2f0,'_AIL_release_sample_handle_4')]:
  def endpoint(uc,args,slot=slot,name=name):
   counts[slot]=counts.get(slot,0)+1;n=counts[slot]
   value=(0 if n==fail_handle else 0x70000000+n) if slot==0x56f2f4 else 0
   events.append((name,args,value));return value
  pc=sym(name) if c else S+0x49000+(slot&0xfff)
  if not c:put(u,slot,pc)
  state['hooks'][pc]=(1,0 if c else 4,endpoint)
 args=[OBJ,requested] if c else [requested]
 result=run(c,u,0x55dea2,args,cleanup=0 if c else 4,receiver=OBJ)
 return dict(result=result,count=get(u,OBJ+0x198),arrays={hex(a):get(u,OBJ+a) for a in [0x19c,0x1a0,0x1a8,0x1ac]},events=events)

cases=[]
for cfg in [dict(),dict(requested=0),dict(requested=-1),*[dict(fail_heap=n) for n in range(1,5)],dict(fail_handle=1),dict(fail_handle=2),dict(requested=0x20000000,fail_heap=1),dict(requested=0x40000000,fail_heap=1)]:
 answers=[capture(c,u,state,**cfg) for c,u,state in machines]
 differs=answers[0]!=answers[1]
 if not cfg or cfg.get('requested') in [0,-1]:assert not differs,(cfg,answers)
 else:assert differs,(cfg,answers)
 cases.append(dict(inputs=cfg,native=answers[0],previous_C=answers[1],mismatch=differs))
report=dict(native_sha256=hashlib.sha256(original.read_bytes()).hexdigest(),old_C_path=str(retained.resolve()),old_C_sha256=target_sha,cases=cases,matching_controls=3,concrete_counterexamples=8,complete_reconstruction=False,controlled_algorithm_endpoints=[],controlled_heap_endpoints=['00561a1d','00561a2b'],controlled_media_endpoints=['_AIL_allocate_sample_handle_4','_AIL_release_sample_handle_4'],limitations=['Current compatible typed 2D resize still diverges. Three controls match; eight actual original/retained-C paths demonstrate partial-allocation, individual-handle and upper-count-guard differences.','No physical allocation of oversized blocks, audio backend or original game process is used. New C repair and broader callback/overflow-path proof are pending.'])
sources=['direct_c/probe_sound_allocation_failures.py','direct_c/verify_options_startup.py','metadata/sound-allocation-next-native.json','metadata/sound-allocation-next-rea-evidence.json']
report['source_hashes']={p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest() for p in sources}
(ROOT/'metadata/sound-allocation-before-counterexamples.json').write_text(json.dumps(report,indent=2)+'\n')
print('Confirmed remaining 2D array resize: 3 matching controls, 8 concrete counterexamples; original/retained C hashes bound')
