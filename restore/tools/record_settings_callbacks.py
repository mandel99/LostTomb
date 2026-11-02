"""Current settings implementation/routing proof; retain the preflight inventory."""
from pathlib import Path
from datetime import datetime,timezone
import hashlib,json,re
ROOT=Path(__file__).resolve().parents[1]
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
p=json.loads((ROOT/'metadata/settings-callbacks-emulation.json').read_text())
assert p['passed'] and p['checked']==p['plain_checked']==p['comparison_checked']==8625
assert p['native_sha256']=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
assert p['case_selection']=={'limit':None,'start':0,'single_case':False}
assert p['native_instruction_coverage']==[531,531] and p['owned_instruction_coverage']==[458,458]
assert p['native_conditional_outcomes']==[108,108] and p['native_uncovered_instructions']==p['missing_conditional_outcomes']==[]
assert p['popup_dispatch']['targets']==[13,13] and len(p['mutated_helper_phases'])==60
for name,digest in p['source_hashes'].items():assert sha(ROOT/name)==digest,name
for config,folder,key in [('Debug','c-only-debug','compiled_C_sha256'),('Release','c-only','comparison_C_sha256')]:assert sha(ROOT/f'build/{folder}/{config}/restore_settings_callbacks_verify.exe')==p[key]
manifest=json.loads((ROOT/'artifacts/settings_callbacks_phase/manifest.json').read_text())
central=(ROOT/'runtime/menu_callbacks.c').read_text()
direct={s.lower() for s in re.findall(r'address==0x([0-9a-fA-F]{8})u',central)}
assert set(manifest['coordinator_routing']['new_no_argument_tokens'])<=direct
inventory=json.loads((ROOT/'metadata/settings-callback-inventory.json').read_text())
callbacks=[x['token'] for x in inventory['callbacks']];assert len(callbacks)==15
assert all(token in direct or token=='0051f780' for token in callbacks)
oldmission=json.loads((ROOT/'artifacts/integration-batch-122-before/metadata/mission-menu-emulation.json').read_text())
mission=json.loads((ROOT/'metadata/mission-menu-emulation.json').read_text())
assert oldmission['passed'] and oldmission['checked']==11000 and mission['passed'] and mission['checked']==11000
for name,digest in mission['source_hashes'].items():assert sha(ROOT/name)==digest,name
assert sha(ROOT/'build/c-only-debug/Debug/restore_mission_menu_verify.exe')==mission['compiled_C_sha256']
out={'format':1,'generated_at':datetime.now(timezone.utc).isoformat(),'complete_settings_pages':False,'descriptor_callback_tokens':callbacks,'descriptor_callbacks_currently_routed':15,'new_routed_callbacks':13,'additional_repaired_caller':'00416ba0','difficulty_native_reload_repair':'00402e50','native_case_count':8625,'native_C_comparisons':34500,'native_instructions':[531,531],'owned_conditional_outcomes':[108,108],'popup_targets':[13,13],'helper_mutation_phases':60,'proof':{'path':'metadata/settings-callbacks-emulation.json','sha256':sha(ROOT/'metadata/settings-callbacks-emulation.json')},'mission_regression':{'passed':True,'cases':11000,'proof':'metadata/mission-menu-emulation.json','sha256':sha(ROOT/'metadata/mission-menu-emulation.json')},'limitations':['Controller inputs and owned state/member algorithms are proved; full settings-page drawing/pixels remain separate.','PostMessage delivery, physical audio, four difficulty recalculations and4CDE40 are qualified deeper/platform boundaries.','Original generated sound members2704/2706 are unchanged; the new settings callers use actual new explicit154/158 receiver members.','The original descriptor inventory is retained as a pre-reconstruction snapshot, not silently relabeled current implementation coverage.']}
(ROOT/'metadata/settings-callback-status.json').write_text(json.dumps(out,indent=2)+'\n')
print(json.dumps({'routed':15,'new_routes':13,'native_cases':8625,'mission_regression_cases':11000,'complete_settings_pages':False}))
