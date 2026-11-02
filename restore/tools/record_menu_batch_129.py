"""Record the whole resource-panel reconstruction with current proof freshness."""
from pathlib import Path
from datetime import datetime,timezone
import hashlib,json
ROOT=Path(__file__).resolve().parents[1]
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
read=lambda p:json.loads((ROOT/p).read_text())
audit=read('metadata/menu-129-wiring-audit.json');assert audit['passed']
for p,h in audit['source_and_evidence_hashes'].items():assert sha(ROOT/p)==h,p
lines=(ROOT/'artifacts/menu-native-refresh-129.log').read_text(encoding='utf-8',errors='replace').splitlines()
suites=[json.loads(line) for line in lines if line.startswith('{"suite":')]
assert len(suites)==len({s['suite'] for s in suites})==89 and sum(s['cases'] for s in suites)==365069
for s in suites:
 report=read('metadata/'+s['suite'].replace('_','-')+'-emulation.json')
 assert report['passed'] and report['checked']==s['cases'],s['suite']
 for p,h in {**report.get('source_hashes',{}),**report.get('native_exports',{})}.items():assert sha(ROOT/p)==h,(s['suite'],p)
dropdown=read('metadata/dropdown-action-status.json');assert dropdown['current_click_routes_available']==dropdown['proved_callback_bodies']==30 and dropdown['missing_tokens']==[]
returns=json.loads((ROOT/'artifacts/menu-129-return-audit.log').read_text(encoding='utf-8-sig'));assert returns['passed'] and returns['remaining_discarded_guard_sites']==0
replacements=[x for x in returns['later_complete_reconstructions'] if x['file']=='src/unit_02637.c'];assert len(replacements)==7 and all(x['whole_procedure_proven'] for x in replacements)
negative=read('metadata/training-panel-proof-negative.json');assert negative['passed'] and negative['checked']==9 and negative['product_files_modified'] is False
strings=json.loads((ROOT/'artifacts/menu-129-string-audit.log').read_text(encoding='utf-8-sig'));assert strings['passed'] and strings['issues']==[]
reference=read('metadata/training-panel-reference-review.json');assert reference['C_sharp_used_as_exact_behavior'] is False
for r in reference['references']:assert sha(ROOT/r['path'])==r['sha256'],r['path']
paths=set(audit['source_and_evidence_hashes'])|{'metadata/menu-129-wiring-audit.json','metadata/dropdown-action-status.json','metadata/settings-callback-status.json','metadata/training-panel-proof-negative.json','metadata/training-panel-reference-review.json','artifacts/menu-native-refresh-129.log','artifacts/menu-129-return-audit.log','artifacts/menu-129-string-audit.log','tools/refresh_menu_native_evidence.py','tools/repair_discarded_returns.py','tools/test_training_panel_proof_guard.py','tools/record_training_panel_reference.py',Path(__file__).relative_to(ROOT).as_posix()}
paths.update('metadata/'+s['suite'].replace('_','-')+'-emulation.json' for s in suites)
proof=read('metadata/training-panel-emulation.json')
out={'format':1,'generated_at':datetime.now(timezone.utc).isoformat(),'complete_menu_reconstruction':False,
 'predecessor':{'path':'metadata/integration-batch-128-applied.json','sha256':sha(ROOT/'metadata/integration-batch-128-applied.json')},
 'scope':'Whole55AD30 resource panel algorithm, historically named training_panel in local artifacts. Correct selected-city/resource addressing, WORD/DWORD fields, labels, formatting, measurement reloads and highlights; exact caller context nonconsumption and full native CFG/GPR/stack proof. Five queries and seven drawing/text algorithms remain named deep boundaries.',
 'current_CTest_count':131,'configurations_passed':['Debug','Release'],'native_suites':len(suites),'canonical_native_cases':sum(s['cases'] for s in suites),
 'new_native_cases':3865,'new_native_C_comparisons':15460,'native_instructions':[1001,1001],'conditional_outcomes':[90,90],'feasible_computed_edges':[15,15],'qualified_infeasible_computed_edge':'0055b343->0055b359; all instruction bytes remain covered through other paths','mutated_helper_phases':26,
 'incoming_GPR_inputs':[],'ignored_context_values':[0,1,256,257,0x80000000,0xffffffff],
 'new_callback_routes':[],'current_top_table_routes':30,'top_table_proved_callback_bodies':30,
 'original_EXE_build_dependency':False,'original_game_process_executed':False,'computer_UI_automation_used':False,
 'refresh_dispatch_preparation':{'production_changed':False,'C_equivalence_verified':False,'authenticated_native_instructions':917,'candidate_compiles':['Debug /Od /W4 /WX','Release /O2 /W4 /WX'],'reviewed_direct_bindings':44,'reviewed_indexed_bindings':3,'unaccepted_working_directory':'artifacts/refresh_menu_dispatch_phase','qualification':'Working staging only, deliberately outside accepted source hash closure. Actual-source native/C full controller comparison and guarded application remain required.'},
 'remaining_obligations':['Whole refresh4D75D0 candidate is not applied/proved; remaining input4D8310 dispatch also needs complete verification beyond accepted blocks.','Five availability queries are being reconstructed as a real9-function pure closure; they are still controlled boundaries in this accepted panel proof.','Seven deeper drawing/text algorithms, physical pixels and full screen composition remain separate requirements.','Nested Escape/modal/window event pump is under reconstruction, with actual window prelude and repeated-Escape filtering.','Real OS message/display/audio/video effects, I/O/persistence and deeper gameplay remain separately unproved.','Map refresh and postpass phases remain staged; further closure work continues.','This offline checkpoint does not establish whole-menu visual fidelity or live playability.'],
 'limitations':proof['limitations'],'source_and_evidence_hashes':{p:sha(ROOT/p) for p in sorted(paths)}}
(ROOT/'metadata/integration-batch-129-applied.json').write_text(json.dumps(out,indent=2)+'\n')
print(json.dumps({k:out[k] for k in ['current_CTest_count','native_suites','canonical_native_cases','new_native_C_comparisons','complete_menu_reconstruction']}))
