"""Replay all eight retained allocation failure settings against current C."""
import verify_sound_allocation as oracle
from verify_sound_allocation import *
before=json.loads((ROOT/'metadata/sound-allocation-before-counterexamples.json').read_text())
cases=[case for case in before['cases'] if case['mismatch']];assert len(cases)==8
replayed=[]
for case in cases:
 cfg={**case['inputs'],'old_count':3,'old_mask':0}
 oracle.exercise(**cfg)
 answers=[]
 for c,u,state in machines:
  answers.append(dict(result=u.reg_read(UC_X86_REG_EAX),count=get(u,OBJ+0x198),arrays=[get(u,OBJ+off) for off in [0x19c,0x1a0,0x1a8,0x1ac]]))
 assert answers[0]==answers[1],(cfg,answers)
 replayed.append(dict(inputs=case['inputs'],original=answers[0],current_C=answers[1]))
sources=['direct_c/verify_sound_allocation_regressions.py','direct_c/verify_sound_allocation.py','direct_c/verify_sound_stopping.py','direct_c/verify_sound_playback.py','direct_c/verify_options_startup.py','metadata/sound-allocation-before-counterexamples.json']
proof=dict(passed=True,native_sha256=catalogue['native_sha256'],compiled_C_path=str(target.resolve()),compiled_C_sha256=target_sha,replayed_counterexamples=8,cases=replayed,source_hashes={p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest() for p in sources},limitations=['All eight historical requested/failure settings replay with initial sample_count3 and empty array fields. The current shared oracle compares actual original and current C full memory, ordered stores/calls, EAX and ABI.','The mapped test allocator uses its own guard-page addresses; historical physical pointer addresses are not compared across fixtures. Physical allocator/audio behavior remains outside the test.'])
suffix='diagnostic' if target.stem=='restore_link_check' else 'game';(ROOT/f'metadata/sound-allocation-regressions-{suffix}.json').write_text(json.dumps(proof,indent=2)+'\n')
print('PASS all 8 retained failure settings against original and current C; full state/calls/ABI comparison')
