"""Replay the four retained insertion mismatches against original/current C."""
from verify_music_configuration import *
before=json.loads((ROOT/'metadata/music-configuration-next-counterexamples.json').read_text())
assert before['retained_previous_C'] and before['concrete_counterexamples']==4
assert hashlib.sha256(Path(before['compiled_C_path']).read_bytes()).hexdigest()==before['compiled_C_sha256']
cases=[]
for case in before['cases']:
 exercise(category=case['category'],count=case['old_count'],ambient=case['ambient'],text=b'probe.wav')
 _,u,state=machines[-1];row=(MQ+case['category']*0x1b10)&0xffffffff
 actual=dict(result=u.reg_read(UC_X86_REG_EAX),count=get(u,row+(0x1b0c if case['ambient'] else 0x1b08)))
 assert actual==case['original'],(case,actual)
 cases.append(dict(category=case['category'],old_count=case['old_count'],ambient=case['ambient'],original=case['original'],current_C=actual,previous_C=case['previous_C']))
paths=['direct_c/verify_music_configuration_regressions.py','direct_c/verify_music_configuration.py','direct_c/verify_options_startup.py','metadata/music-configuration-next-counterexamples.json','metadata/music-configuration-native.json','metadata/music-configuration-rea-evidence.json','runtime/music_tracks.c','include/music_configuration.h','include/music_manager.h','src/unit_02476.c']
report=dict(passed=True,replayed_counterexamples=4,native_sha256=catalogue['native_sha256'],compiled_C_path=str(target.resolve()),compiled_C_sha256=target_sha,cases=cases,source_hashes={p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest() for p in paths},limitations=['Four complete native/current named/canonical C insertion scenarios compare full state, ordered stores, meaningful results and normal ABI through the primary oracle. This replay does not widen its finite scope or certify CD/physical audio behavior.'])
suffix='diagnostic' if target.stem=='restore_link_check' else 'game';(ROOT/f'metadata/music-configuration-regressions-{suffix}.json').write_text(json.dumps(report,indent=2)+'\n')
print('PASS all four retained music insertion counterexamples against original/current named/canonical C')
