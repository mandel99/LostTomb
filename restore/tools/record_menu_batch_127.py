"""Record replay integration after actual builds, native replays and routing audit."""
from pathlib import Path
from datetime import datetime,timezone
import hashlib,json
ROOT=Path(__file__).resolve().parents[1]
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
read=lambda p:json.loads((ROOT/p).read_text())
audit=read('metadata/menu-127-wiring-audit.json');assert audit['passed']
for p,h in audit['source_and_evidence_hashes'].items():assert sha(ROOT/p)==h,p
lines=(ROOT/'artifacts/menu-native-refresh-127.log').read_text(encoding='utf-8',errors='replace').splitlines()
suites=[json.loads(line) for line in lines if line.startswith('{"suite":')]
assert len(suites)==len({s['suite'] for s in suites})==86
assert sum(s['cases'] for s in suites)==351216
for s in suites:
 report=read('metadata/'+s['suite'].replace('_','-')+'-emulation.json')
 assert report['passed'] and report['checked']==s['cases'],s['suite']
 for p,h in {**report.get('source_hashes',{}),**report.get('native_exports',{})}.items():assert sha(ROOT/p)==h,(s['suite'],p)
dropdown=read('metadata/dropdown-action-status.json')
assert dropdown['current_click_routes_available']==30 and dropdown['missing_tokens']==[] and dropdown['proved_callback_bodies']==26 and dropdown['existing_deeper_bodies_outside_callback_proof']==4
returns=json.loads((ROOT/'artifacts/menu-127-return-audit.log').read_text(encoding='utf-8-sig'));assert returns['passed'] and returns['remaining_discarded_guard_sites']==0
strings=json.loads((ROOT/'artifacts/menu-127-string-audit.log').read_text(encoding='utf-8-sig'));assert strings['passed'] and strings['issues']==[]
paths=set(audit['source_and_evidence_hashes'])|{'metadata/menu-127-wiring-audit.json','metadata/dropdown-action-status.json','metadata/settings-callback-status.json','artifacts/menu-native-refresh-127.log','artifacts/menu-127-return-audit.log','artifacts/menu-127-string-audit.log','tools/refresh_menu_native_evidence.py','tools/record_dropdown_actions.py','tools/verify_remaining_menu_click_emulation.py',Path(__file__).relative_to(ROOT).as_posix()}
paths.update('metadata/'+s['suite'].replace('_','-')+'-emulation.json' for s in suites)
out={'format':1,'generated_at':datetime.now(timezone.utc).isoformat(),'complete_menu_reconstruction':False,
 'predecessor':{'path':'metadata/integration-batch-126-applied.json','sha256':sha(ROOT/'metadata/integration-batch-126-applied.json')},
 'scope':'Complete native replay callback418510, corrected whole4181E0 fallback and unchanged real5169C0 profile getter, with exact central route. Legacy stdio/loaders/modal/query remain named deep boundaries.',
 'current_CTest_count':128,'configurations_passed':['Debug','Release'],'native_suites':len(suites),'canonical_native_cases':sum(s['cases'] for s in suites),
 'new_native_cases':10138,'new_native_C_comparisons':40552,'native_instructions':[151,151],'owned_native_instructions':[113,113],'conditional_outcomes':[6,6],'mutated_helper_phases':20,
 'profile_domain_bytes':[0,31],'native_filename_buffer_bytes':100,'maximum_legitimate_filename_bytes_including_NUL':57,
 'new_callback_routes':['00418510'],'current_top_table_routes':30,'remaining_missing_top_table_bodies':[],
 'top_table_proved_callback_bodies':26,'top_table_existing_bodies_under_stronger_review':['00402450','00402540','00402630','00402bb0'],
 'original_EXE_build_dependency':False,'original_game_process_executed':False,'computer_UI_automation_used':False,
 'remaining_obligations':['Four existing load/save/quit callbacks remain outside dedicated whole-body proof; stronger real-helper composition is in progress.','Nested Escape/modal confirmation including OS event pump is under reconstruction.','Full initial/refresh/input dispatch and all downstream screen algorithms require whole-entry verification; routed callbacks alone do not close this obligation.','Physical display switching/message delivery, real I/O and persistence, modal/artwork pixels and deeper gameplay remain separately unproved.','Map refresh and postpass algorithms remain staged/incomplete.','This offline checkpoint does not establish whole-menu visual fidelity or live playability.'],
 'source_and_evidence_hashes':{p:sha(ROOT/p) for p in sorted(paths)}}
(ROOT/'metadata/integration-batch-127-applied.json').write_text(json.dumps(out,indent=2)+'\n')
print(json.dumps({k:out[k] for k in ['current_CTest_count','native_suites','canonical_native_cases','current_top_table_routes','complete_menu_reconstruction']}))
