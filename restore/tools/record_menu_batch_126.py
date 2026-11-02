"""Record batch126 only after complete source-only and native verification."""
from pathlib import Path
from datetime import datetime,timezone
import hashlib,json
ROOT=Path(__file__).resolve().parents[1]
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
read=lambda p:json.loads((ROOT/p).read_text())
audit=read('metadata/menu-126-wiring-audit.json');assert audit['passed']
for p,h in audit['source_and_evidence_hashes'].items():assert sha(ROOT/p)==h,p
lines=(ROOT/'artifacts/menu-native-refresh-126.log').read_text(encoding='utf-8',errors='replace').splitlines()
suites=[json.loads(line) for line in lines if line.startswith('{"suite":')]
assert len(suites)==len({x['suite'] for x in suites})==85,('incomplete native refresh',len(suites))
assert sum(x['cases'] for x in suites)==341078
for s in suites:
 report=read('metadata/'+s['suite'].replace('_','-')+'-emulation.json')
 assert report['passed'] and report['checked']==s['cases'],s['suite']
 for p,h in {**report.get('source_hashes',{}),**report.get('native_exports',{})}.items():assert sha(ROOT/p)==h,(s['suite'],p)
dropdown=read('metadata/dropdown-action-status.json');assert dropdown['current_click_routes_available']==29 and dropdown['missing_tokens']==['00418510']
returns=json.loads((ROOT/'artifacts/menu-126-return-audit.log').read_text(encoding='utf-8-sig'));assert returns['passed'] and returns['remaining_discarded_guard_sites']==0
strings=json.loads((ROOT/'artifacts/menu-126-string-audit.log').read_text(encoding='utf-8-sig'));assert strings['passed'] and strings['issues']==[]
paths=set(audit['source_and_evidence_hashes'])|{'metadata/menu-126-wiring-audit.json','metadata/dropdown-action-status.json','metadata/settings-callback-status.json','artifacts/menu-native-refresh-126.log','artifacts/menu-126-return-audit.log','artifacts/menu-126-string-audit.log','tools/refresh_menu_native_evidence.py','tools/record_dropdown_actions.py','tools/verify_remaining_menu_click_emulation.py',Path(__file__).relative_to(ROOT).as_posix()}
paths.update('metadata/'+s['suite'].replace('_','-')+'-emulation.json' for s in suites)
out={'format':1,'generated_at':datetime.now(timezone.utc).isoformat(),'complete_menu_reconstruction':False,
 'predecessor':{'path':'metadata/integration-batch-125-applied.json','sha256':sha(ROOT/'metadata/integration-batch-125-applied.json')},
 'scope':'Complete editor validation with native classifiers/counter/selector, and display confirmation callbacks/timed input/renderer/framebuffer clear. Exact narrow input wiring; other modes preserved. Deeper OS/modal/rendering/gameplay boundaries remain qualified.',
 'current_CTest_count':127,'configurations_passed':['Debug','Release'],'native_suites':len(suites),'canonical_native_cases':sum(s['cases'] for s in suites),
 'new_native_cases':10128,'new_native_C_comparisons':32556,
 'editor_validation':{'cases':6150,'comparisons':24600,'reachable_native_instructions':[571,571],'owned_instructions':[408,408],'feasible_conditional_outcomes':[138,138],'dead_edges':read('metadata/editor-validation-emulation.json')['declared_dead_edges']},
 'display_confirmation':{'cases':3978,'comparisons':7956,'owned_native_instructions':[126,126],'owned_conditional_outcomes':[10,10],'actual_640x480_RGB565_fill':True},
 'new_callback_routes':['00409830','00402da0','00402de0','00402e10'],'current_top_table_routes':29,'remaining_top_table_bodies':['00418510'],
 'original_EXE_build_dependency':False,'original_game_process_executed':False,'computer_UI_automation_used':False,
 'remaining_obligations':['Replay418510 is frozen, not applied at this checkpoint.','Nested Escape/modal confirmation including OS event pump is under reconstruction.','Physical display switching and message delivery, language/modal/artwork pixels, deeper file operations and gameplay remain separately unproved.','Map refresh and postpass algorithms remain staged/incomplete.','This offline checkpoint does not establish whole-menu visual fidelity or live playability.'],
 'source_and_evidence_hashes':{p:sha(ROOT/p) for p in sorted(paths)}}
(ROOT/'metadata/integration-batch-126-applied.json').write_text(json.dumps(out,indent=2)+'\n')
print(json.dumps({k:out[k] for k in ['current_CTest_count','native_suites','canonical_native_cases','current_top_table_routes','complete_menu_reconstruction']}))
