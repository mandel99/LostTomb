"""Accept whole initial dispatcher and stronger proof for all top-table entries."""
from pathlib import Path
from datetime import datetime,timezone
import hashlib,json
ROOT=Path(__file__).resolve().parents[1]
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
read=lambda p:json.loads((ROOT/p).read_text())
audit=read('metadata/menu-128-wiring-audit.json');assert audit['passed']
for p,h in audit['source_and_evidence_hashes'].items():assert sha(ROOT/p)==h,p
lines=(ROOT/'artifacts/menu-native-refresh-128.log').read_text(encoding='utf-8',errors='replace').splitlines()
suites=[json.loads(line) for line in lines if line.startswith('{"suite":')]
assert len(suites)==len({s['suite'] for s in suites})==88
assert sum(s['cases'] for s in suites)==361204
for s in suites:
 report=read('metadata/'+s['suite'].replace('_','-')+'-emulation.json')
 assert report['passed'] and report['checked']==s['cases'],s['suite']
 for p,h in {**report.get('source_hashes',{}),**report.get('native_exports',{})}.items():assert sha(ROOT/p)==h,(s['suite'],p)
dropdown=read('metadata/dropdown-action-status.json')
assert dropdown['current_click_routes_available']==dropdown['proved_callback_bodies']==30 and dropdown['missing_tokens']==[] and dropdown['existing_deeper_bodies_outside_callback_proof']==0
returns=json.loads((ROOT/'artifacts/menu-128-return-audit.log').read_text(encoding='utf-8-sig'));assert returns['passed'] and returns['remaining_discarded_guard_sites']==0
replacement=[x for x in returns['later_complete_reconstructions'] if x['file']=='src/unit_01866.c'];assert len(replacement)==1 and replacement[0]['whole_caller_procedure_proven'] is True
negative=read('metadata/initial-dispatch-proof-negative.json');assert negative['passed'] and negative['checked']==7 and negative['product_files_modified'] is False
strings=json.loads((ROOT/'artifacts/menu-128-string-audit.log').read_text(encoding='utf-8-sig'));assert strings['passed'] and strings['issues']==[]
paths=set(audit['source_and_evidence_hashes'])|{'metadata/menu-128-wiring-audit.json','metadata/dropdown-action-status.json','metadata/settings-callback-status.json','metadata/initial-dispatch-proof-negative.json','artifacts/menu-native-refresh-128.log','artifacts/menu-128-return-audit.log','artifacts/menu-128-string-audit.log','tools/refresh_menu_native_evidence.py','tools/record_dropdown_actions.py','tools/repair_discarded_returns.py','tools/test_initial_dispatch_proof_guard.py',Path(__file__).relative_to(ROOT).as_posix()}
paths.update('metadata/'+s['suite'].replace('_','-')+'-emulation.json' for s in suites)
reference=read('metadata/initial-dispatch-reference-review.json')
assert sha(ROOT/reference['reference_source']['path'])==reference['reference_source']['sha256']
assert reference['C_sharp_used_as_exact_behavior'] is False
paths.update(['metadata/initial-dispatch-reference-review.json','tools/record_initial_dispatch_reference.py'])
initial=read('metadata/initial-menu-dispatch-emulation.json');existing=read('metadata/menu-existing-actions-emulation.json')
out={'format':1,'generated_at':datetime.now(timezone.utc).isoformat(),'complete_menu_reconstruction':False,
 'predecessor':{'path':'metadata/integration-batch-127-applied.json','sha256':sha(ROOT/'metadata/integration-batch-127-applied.json')},
 'scope':'Whole initial4D71C0 dispatcher, corrected full-DWORD alternate mode/flag and ordered data access, plus complete existing load/save/delete/quit entry proof using actual unchanged getter/selector/memset/caret. Deeper renderer and platform algorithms remain qualified.',
 'current_CTest_count':130,'configurations_passed':['Debug','Release'],'native_suites':len(suites),'canonical_native_cases':sum(s['cases'] for s in suites),
 'new_native_cases':9988,'new_native_C_comparisons':31566,
 'initial_dispatch':{'cases':5795,'comparisons':23180,'native_instructions':[179,179],'conditional_outcomes':[8,8],'switch_modes':[88,88],'switch_targets':[47,47],'call_sites':[58,58],'controlled_screen_endpoints':50,'mutated_helper_phases':100,'limitations':initial['limitations']},
 'existing_actions':{'cases':4193,'comparisons':8386,'primary_instructions':[235,235],'reachable_composed_instructions':[423,423],'feasible_conditional_outcomes':[18,18],'mutated_helper_phases':26,'limitations':existing['limitations']},
 'current_top_table_routes':30,'top_table_proved_callback_bodies':30,'remaining_missing_top_table_bodies':[],
 'original_EXE_build_dependency':False,'original_game_process_executed':False,'computer_UI_automation_used':False,
 'remaining_obligations':['Whole refresh4D75D0 and the rest of input4D8310 dispatch require complete reconstruction/verification beyond the selected already-proved blocks.','Fifty initial-screen renderer/controller algorithms are qualified deep endpoints, not fifty finished screens;55AD30/context analysis and whole panel reconstruction are in progress.','Nested Escape/modal confirmation including OS event pump is under reconstruction.','Physical display switching/message delivery, real I/O/persistence, modal/artwork pixels and deeper gameplay remain separately unproved.','Map refresh/postpass phases are staged and further algorithms remain incomplete.','This offline checkpoint does not establish whole-menu visual fidelity or live playability.'],
 'source_and_evidence_hashes':{p:sha(ROOT/p) for p in sorted(paths)}}
(ROOT/'metadata/integration-batch-128-applied.json').write_text(json.dumps(out,indent=2)+'\n')
print(json.dumps({k:out[k] for k in ['current_CTest_count','native_suites','canonical_native_cases','top_table_proved_callback_bodies','complete_menu_reconstruction']}))
