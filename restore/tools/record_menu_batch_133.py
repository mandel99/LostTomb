"""Accept qualified whole text and real actor distance code only with fresh evidence."""
from pathlib import Path
from datetime import datetime,timezone
import hashlib,json
ROOT=Path(__file__).resolve().parents[1]
sha=lambda p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest()
read=lambda p:json.loads((ROOT/p).read_text(encoding='utf-8-sig'))
audit=read('metadata/menu-133-wiring-audit.json');assert audit['passed'] and not audit['complete_menu_reconstruction']
for p,h in audit['source_and_evidence_hashes'].items():assert sha(p)==h,('audit freshness',p)
log='artifacts/menu-native-refresh-133.log';suites=[json.loads(s) for s in (ROOT/log).read_text().splitlines() if s.startswith('{"suite":')]
assert len(suites)==len({s['suite'] for s in suites})==96 and sum(s['cases'] for s in suites)==398048
paths=set(audit['source_and_evidence_hashes'])
for s in suites:
 name=s['suite'];p='metadata/'+name.replace('_','-')+'-emulation.json';report=read(p);assert report['passed'] and report['checked']==s['cases']
 for source,h in {**report.get('source_hashes',{}),**report.get('native_exports',{}),**report.get('binary_hashes',{})}.items():assert sha(source)==h,(name,source)
 target='music' if name=='music_manager' else name;binary=f'build/c-only-debug/Debug/restore_{target}_verify.exe';assert sha(binary)==report['compiled_C_sha256'];paths.add(binary)
 if report.get('comparison_C_sha256'):
  binary=f'build/c-only/Release/restore_{target}_verify.exe';assert sha(binary)==report['comparison_C_sha256'];paths.add(binary)
 for v in report.get('variants',{}).values():
  if isinstance(v,dict) and 'path' in v and 'sha256' in v:assert sha(v['path'])==v['sha256'];paths.add(v['path'])
 paths.add(p)
dropdown=read('metadata/dropdown-action-status.json');assert dropdown['current_click_routes_available']==dropdown['proved_callback_bodies']==30 and dropdown['missing_tokens']==[]
for p in ['artifacts/menu-133-return-audit.log','artifacts/menu-133-string-audit.log']:
 d=read(p);assert d['passed'] and d['issues']==[]
assert read('artifacts/menu-133-return-audit.log')['remaining_discarded_guard_sites']==0
assert all(c['CTest_passed']==138 for c in audit['configurations'].values())
app=read('metadata/integration-batch-133-application.json')
paths.update([log,'metadata/menu-133-wiring-audit.json','metadata/dropdown-action-status.json','metadata/settings-callback-status.json','artifacts/menu-133-return-audit.log','artifacts/menu-133-string-audit.log','tools/refresh_menu_native_evidence.py','tools/repair_discarded_returns.py','tools/apply_menu_batch_133.py','tools/freeze_menu_text_glyph.py','README.md',Path(__file__).relative_to(ROOT).as_posix()])
out={'format':1,'generated_at':datetime.now(timezone.utc).isoformat(),'complete_menu_reconstruction':False,'predecessor':{'path':app['predecessor'],'sha256':app['predecessor_sha256']},'scope':'Five complete text functions: byte traversal/substitution, measurement, glyph positioning and centering; exact367 nativeinstructions, typed word color, fullDWORD state and wrapping arithmetic. Real187-instruction actor distance producer with185 feasible instructions and actual22-instruction fill; two exact head-wrap exclusions certified from native counter/noalias invariant, allfive tail-wrap sites execute. No menu dispatcher or callback registry edit.',
 'current_CTest_count':138,'configurations_passed':['Debug','Release'],'native_suites':96,'canonical_native_cases':398048,'new_native_cases':3236,'new_native_C_comparisons':6472,'text':audit['text'],'actor_distance':audit['actor_distance'],'current_top_table_routes':30,'top_table_proved_callback_bodies':30,
 'original_EXE_build_dependency':False,'original_game_process_executed':False,'computer_UI_automation_used':False,'C_sharp_used_as_exact_behavior':False,
 'remaining_obligations':['Deeper53E590 sprite algorithm and physical font pixels are outside the new text proof; independently composed raster/native screen cases are still needed.','Complete6564-instruction4D8310 input controller and its route-member/x87 cluster are unfinished; a native route allocation-size defect requires explicit resolution before production application.','The actor-distance producer is closed; its40C180/40BDA0 LOS and4B6400 action consumers are still under reconstruction, so complete actor-order gameplay is not claimed.','Other deep functions controlled by existing menu/render oracles, actual OS effects, persistence and whole-menu visual fidelity/live playability remain unproved.','Known1768 GetCursorPos(FALSE) qualification remains open.','Frozen map postpasses await coherent guarded application and higher-level closure.'],
 'limitations':audit['limitations'],'source_and_evidence_hashes':{p:sha(p) for p in sorted(paths)}}
(ROOT/'metadata/integration-batch-133-applied.json').write_text(json.dumps(out,indent=2)+'\n')
print(json.dumps({k:out[k] for k in ['current_CTest_count','native_suites','canonical_native_cases','new_native_C_comparisons','complete_menu_reconstruction']}))
