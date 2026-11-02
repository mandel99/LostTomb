"""Record qualified full actor-order controller with fresh broad evidence."""
from pathlib import Path
from datetime import datetime,timezone
import hashlib,json
ROOT=Path(__file__).resolve().parents[1]
sha=lambda p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest()
read=lambda p:json.loads((ROOT/p).read_text(encoding='utf-8-sig'))
audit=read('metadata/menu-132-wiring-audit.json');assert audit['passed'] and not audit['complete_menu_reconstruction'] and not audit['complete_actor_order_gameplay_claimed']
for p,h in audit['source_and_evidence_hashes'].items():assert sha(p)==h,('audit freshness',p)
log='artifacts/menu-native-refresh-132.log';suites=[json.loads(line) for line in (ROOT/log).read_text().splitlines() if line.startswith('{"suite":')]
assert len(suites)==len({s['suite'] for s in suites})==94 and sum(s['cases'] for s in suites)==394812
paths=set(audit['source_and_evidence_hashes'])
for s in suites:
 name=s['suite'];path='metadata/'+name.replace('_','-')+'-emulation.json';report=read(path);assert report['passed'] and report['checked']==s['cases']
 for p,h in {**report.get('source_hashes',{}),**report.get('native_exports',{}),**report.get('binary_hashes',{})}.items():assert sha(p)==h,(name,p)
 target='music' if name=='music_manager' else name;binary=f'build/c-only-debug/Debug/restore_{target}_verify.exe';assert sha(binary)==report['compiled_C_sha256'];paths.add(binary)
 if report.get('comparison_C_sha256'):
  binary=f'build/c-only/Release/restore_{target}_verify.exe';assert sha(binary)==report['comparison_C_sha256'];paths.add(binary)
 for v in report.get('variants',{}).values():
  if isinstance(v,dict) and 'path' in v and 'sha256' in v:assert sha(v['path'])==v['sha256'];paths.add(v['path'])
 paths.add(path)
dropdown=read('metadata/dropdown-action-status.json');assert dropdown['current_click_routes_available']==dropdown['proved_callback_bodies']==30 and dropdown['missing_tokens']==[]
for path in ['artifacts/menu-132-return-audit.log','artifacts/menu-132-string-audit.log']:
 proof=read(path);assert proof['passed'] and proof['issues']==[]
assert read('artifacts/menu-132-return-audit.log')['remaining_discarded_guard_sites']==0
assert all(x['CTest_passed']==136 for x in audit['configurations'].values())
app=read('metadata/integration-batch-132-application.json');proof=read('metadata/menu-window-cursor-consumer-emulation.json')
paths.update([log,'metadata/menu-132-wiring-audit.json','metadata/dropdown-action-status.json','metadata/settings-callback-status.json','artifacts/menu-132-return-audit.log','artifacts/menu-132-string-audit.log','tools/refresh_menu_native_evidence.py','tools/repair_discarded_returns.py','tools/apply_menu_batch_132.py','README.md',Path(__file__).relative_to(ROOT).as_posix()])
out={'format':1,'generated_at':datetime.now(timezone.utc).isoformat(),'complete_menu_reconstruction':False,'complete_actor_order_gameplay_claimed':False,'predecessor':{'path':app['predecessor'],'sha256':app['predecessor_sha256']},
 'scope':'Whole215-instruction40BA40 actor-order commit and23-instruction4B5520 linked-actor lookup. Correct fullDWORD actor/cursor fields, signedWORD coordinates/head/links/tags and388-byte records. Actual unchanged route/direction/scaling and selected cursor paths composed. Existing mode31 callsite, three menu dispatchers and central registry unchanged.',
 'current_CTest_count':136,'configurations_passed':['Debug','Release'],'native_suites':94,'canonical_native_cases':394812,'new_native_cases':2635,'new_native_C_comparisons':5270,'owned_instruction_coverage':[238,238],'owned_conditional_outcomes':[32,32],'dispatch_indices':list(range(12)),'full_helper_instructions':168,'cursor_context_coverage':[19,99],'separate_full_cursor_coverage':[99,99],'watched_bytes':2864,'negative_proof_checks':11,
 'old_cursor_consumer_fact_explicitly_superseded':True,'known_GetCursorPos_FALSE_gap_closed':False,'current_top_table_routes':30,'top_table_proved_callback_bodies':30,
 'original_EXE_build_dependency':False,'original_game_process_executed':False,'computer_UI_automation_used':False,'C_sharp_used_as_exact_behavior':False,
 'remaining_obligations':['Three real40C180/40BDA0/4B6400 movement/action algorithms are controlled boundaries here; full actor-order gameplay is still unproved.','Other4B5520 callers81/40B630 and1862/4D6AA0 have not been fully verified by this scope.','Complete6564-instruction4D8310 input dispatcher, its route members and exact x87 geometry are under separate reconstruction.','Ninety-six deeper functions in the accepted refresh-controller oracle, physical screens/OS effects, persistence and live playability remain separately unproved.','Known1768 GetCursorPos(FALSE) behavior qualification remains open; no unspecified POINT value is invented.','Map refresh/postpass/flood phases remain staged pending coherent guarded integration and higher-level closures.'],
 'limitations':proof['limitations'],'source_and_evidence_hashes':{p:sha(p) for p in sorted(paths)}}
(ROOT/'metadata/integration-batch-132-applied.json').write_text(json.dumps(out,indent=2)+'\n')
print(json.dumps({k:out[k] for k in ['current_CTest_count','native_suites','canonical_native_cases','new_native_C_comparisons','complete_menu_reconstruction']}))
