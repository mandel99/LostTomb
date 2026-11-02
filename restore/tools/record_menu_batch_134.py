"""Accept actual LOS algorithms without promoting unfinished group action or menu input."""
from pathlib import Path
from datetime import datetime,timezone
import hashlib,json
ROOT=Path(__file__).resolve().parents[1]
sha=lambda p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest()
read=lambda p:json.loads((ROOT/p).read_text(encoding='utf-8-sig'))
audit=read('metadata/menu-134-wiring-audit.json');assert audit['passed'] and not audit['complete_menu_reconstruction'] and not audit['route_builder_application']
for p,h in audit['source_and_evidence_hashes'].items():assert sha(p)==h,('audit freshness',p)
log='artifacts/menu-native-refresh-134.log';suites=[json.loads(s) for s in (ROOT/log).read_text().splitlines() if s.startswith('{"suite":')]
assert len(suites)==len({s['suite'] for s in suites})==97 and sum(s['cases'] for s in suites)==405528
paths=set(audit['source_and_evidence_hashes'])
for s in suites:
 name=s['suite'];path='metadata/'+name.replace('_','-')+'-emulation.json';p=read(path);assert p['passed'] and p['checked']==s['cases']
 for source,h in {**p.get('source_hashes',{}),**p.get('native_exports',{}),**p.get('binary_hashes',{})}.items():assert sha(source)==h,(name,source)
 target='music' if name=='music_manager' else name;binary=f'build/c-only-debug/Debug/restore_{target}_verify.exe';assert sha(binary)==p['compiled_C_sha256'];paths.add(binary)
 if p.get('comparison_C_sha256'):
  binary=f'build/c-only/Release/restore_{target}_verify.exe';assert sha(binary)==p['comparison_C_sha256'];paths.add(binary)
 for v in p.get('variants',{}).values():
  if isinstance(v,dict) and 'path' in v and 'sha256' in v:assert sha(v['path'])==v['sha256'];paths.add(v['path'])
 paths.add(path)
dropdown=read('metadata/dropdown-action-status.json');assert dropdown['current_click_routes_available']==dropdown['proved_callback_bodies']==30 and dropdown['missing_tokens']==[]
for path in ['artifacts/menu-134-return-audit.log','artifacts/menu-134-string-audit.log']:
 p=read(path);assert p['passed'] and p['issues']==[]
assert read('artifacts/menu-134-return-audit.log')['remaining_discarded_guard_sites']==0
assert all(c['CTest_passed']==139 for c in audit['configurations'].values());app=read('metadata/integration-batch-134-application.json')
paths.update([log,'metadata/menu-134-wiring-audit.json','metadata/dropdown-action-status.json','metadata/settings-callback-status.json','artifacts/menu-134-return-audit.log','artifacts/menu-134-string-audit.log','tools/refresh_menu_native_evidence.py','tools/repair_discarded_returns.py','tools/apply_menu_batch_134.py','README.md',Path(__file__).relative_to(ROOT).as_posix()])
out={'format':1,'generated_at':datetime.now(timezone.utc).isoformat(),'complete_menu_reconstruction':False,'complete_actor_order_gameplay_claimed':False,'predecessor':{'path':app['predecessor'],'sha256':app['predecessor_sha256']},'scope':'Whole40BDA0/40C180 LOS algorithms with retained raw80 slopes, ordered reciprocal/intersections, original signed/wrapped metrics, exact flag/classification decisions and real output aliasing. Actual accepted flood/fill and explicit immutable raw80 conversion compose; no controlled game/OS endpoints and no binary32 slope spill. Global implicit-ST0 entry2775, all three menu dispatchers and registry remain unchanged.',
 'current_CTest_count':139,'configurations_passed':['Debug','Release'],'native_suites':97,'canonical_native_cases':405528,'new_native_cases':7480,'new_native_C_comparisons':14960,'whole_function_cases':520,'whole_function_C_comparisons':1040,'primitive_cases':6960,'primitive_C_comparisons':13920,'owned_instruction_coverage':[538,538],'feasible_conditional_outcomes':[146,146],'qualified_excluded_edges':audit['qualified_excluded_edges'],'actual_dependency_coverage':audit['actual_dependency_coverage'],'masked_control_words':24,'controlled_game_or_OS_boundaries':[],
 'current_top_table_routes':30,'top_table_proved_callback_bodies':30,'original_EXE_build_dependency':False,'original_game_process_executed':False,'computer_UI_automation_used':False,'C_sharp_used_as_exact_behavior':False,
 'remaining_obligations':['4B6400 group-action/path graph is under reconstruction; LOS completion does not establish complete actor-order gameplay.','Complete6564-instruction4D8310 input controller and route members remain unproved as a whole. A private capacity repair is being verified separately and is not applied by this checkpoint.','Other menu/render deeper algorithms, full screen composition, actual OS effects, persistence and live playability remain separately unproved.','Known1768 GetCursorPos(FALSE) qualification remains open.','Frozen map postpass phases await coherent guarded application and higher-level closure.'],
 'limitations':audit['limitations'],'source_and_evidence_hashes':{p:sha(p) for p in sorted(paths)}}
(ROOT/'metadata/integration-batch-134-applied.json').write_text(json.dumps(out,indent=2)+'\n')
print(json.dumps({k:out[k] for k in ['current_CTest_count','native_suites','canonical_native_cases','new_native_C_comparisons','complete_menu_reconstruction']}))
