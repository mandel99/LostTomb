"""Accept whole-refresh reconstruction only with fresh shared builds and all suites."""
from pathlib import Path
from datetime import datetime,timezone
import hashlib,json
ROOT=Path(__file__).resolve().parents[1]
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
read=lambda p:json.loads((ROOT/p).read_text(encoding='utf-8-sig'))
audit=read('metadata/menu-131-wiring-audit.json');assert audit['passed'] and not audit['complete_menu_reconstruction']
for p,h in audit['source_and_evidence_hashes'].items():assert sha(ROOT/p)==h,('audit freshness',p)
log=ROOT/'artifacts/menu-native-refresh-131.log';suites=[json.loads(line) for line in log.read_text(encoding='utf-8-sig').splitlines() if line.startswith('{"suite":')]
assert len(suites)==len({s['suite'] for s in suites})==93 and sum(s['cases'] for s in suites)==392177
paths=set(audit['source_and_evidence_hashes'])
for s in suites:
 name=s['suite'];path='metadata/'+name.replace('_','-')+'-emulation.json';report=read(path)
 assert report['passed'] and report['checked']==s['cases'],name
 for p,h in {**report.get('source_hashes',{}),**report.get('native_exports',{}),**report.get('binary_hashes',{})}.items():assert sha(ROOT/p)==h,(name,p)
 target='music' if name=='music_manager' else name;binary=f'build/c-only-debug/Debug/restore_{target}_verify.exe';assert sha(ROOT/binary)==report['compiled_C_sha256'],(name,'Debug binary freshness');paths.add(binary)
 if report.get('comparison_C_sha256'):
  binary=f'build/c-only/Release/restore_{target}_verify.exe';assert sha(ROOT/binary)==report['comparison_C_sha256'],(name,'Release binary freshness');paths.add(binary)
 for v in report.get('variants',{}).values():
  if isinstance(v,dict) and 'path' in v and 'sha256' in v:assert sha(ROOT/v['path'])==v['sha256'];paths.add(v['path'])
 paths.add(path)
dropdown=read('metadata/dropdown-action-status.json');assert dropdown['current_click_routes_available']==dropdown['proved_callback_bodies']==30 and dropdown['missing_tokens']==[]
returns=read('artifacts/menu-131-return-audit.log');assert returns['passed'] and returns['remaining_discarded_guard_sites']==0 and returns['issues']==[]
strings=read('artifacts/menu-131-string-audit.log');assert strings['passed'] and strings['issues']==[]
rea=read('metadata/refresh-menu-rea-availability.json');assert rea['target_sha256']=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d' and rea['result']['isError']
assert json.loads(rea['result']['content'][0]['text'])['error']['code']=='provider_timeout'
paths.update(['metadata/menu-131-wiring-audit.json','metadata/dropdown-action-status.json','metadata/settings-callback-status.json','metadata/refresh-menu-rea-availability.json','artifacts/menu-native-refresh-131.log','artifacts/menu-131-return-audit.log','artifacts/menu-131-string-audit.log','tools/refresh_menu_native_evidence.py','tools/repair_discarded_returns.py','tools/apply_menu_batch_131.py','tools/freeze_refresh_menu_dispatch.py',Path(__file__).relative_to(ROOT).as_posix()])
paths.add('README.md')
assert all(x['CTest_passed']==135 for x in audit['configurations'].values())
app=read('metadata/integration-batch-131-application.json');proof=read('metadata/refresh-menu-dispatch-emulation.json')
out={'format':1,'generated_at':datetime.now(timezone.utc).isoformat(),'complete_menu_reconstruction':False,
 'predecessor':{'path':app['predecessor'],'sha256':app['predecessor_sha256']},
 'scope':'Whole917-instruction4D75D0 per-frame menu refresh controller with actual42-instruction six-getter composition and actual bareRET. Correct44 scalar widths, three indexed data bindings, fullDWORD selected city/actor selectors, single entry mode sample, x86 coordinate wrapping and exact typed hit/hover ABI facades. Ninety-six deeper algorithms remain explicit controlled endpoints in the oracle and real C calls in product.',
 'current_CTest_count':135,'configurations_passed':['Debug','Release'],'native_suites':93,'canonical_native_cases':392177,'new_native_cases':11633,'new_native_C_comparisons':23266,
 'controller_instruction_coverage':[917,917],'composed_instruction_coverage':[960,960],'conditional_outcomes':[250,250],'reached_controlled_endpoints':96,'mutated_endpoint_phases':192,'watched_bytes':222728,
 'current_top_table_routes':30,'top_table_proved_callback_bodies':30,'new_callback_routes':[],
 'original_EXE_build_dependency':False,'original_game_process_executed':False,'computer_UI_automation_used':False,'C_sharp_used_as_exact_behavior':False,
 'REA_attempt':'Fresh read_function_instructions startup timed out after330seconds; no new REA instruction or equivalence result claimed. The frozen Ghidra export is authenticated instruction-by-instruction against pinned original PE in the successful native/C replay.',
 'remaining_obligations':['Complete4D8310 input controller is still unproved beyond accepted popup/display/modal blocks; whole6564-instruction native audit is in progress.','Ninety-six deep rendering/audio/video/query/button algorithms in the refresh-controller oracle require their own actual composition proofs; whole-controller coverage does not establish all downstream behavior.','Physical screen composition, OS event/display/audio/video effects, I/O/persistence and live playability remain separately unproved.','Known1768 GetCursorPos(FALSE) early return differs from native consumption of unspecified POINT; its qualification remains open.','Prepared valid city/resource/actor/record domains and explicit synthetic width probes do not authorize arbitrary unsafe original indices or added clamps.','Frozen map refresh/postpasses and actor-order phases await coherent guarded production integration; larger map refresh488E90 and actor path algorithms remain incomplete.'],
 'limitations':proof['limitations'],'source_and_evidence_hashes':{p:sha(ROOT/p) for p in sorted(paths)}}
(ROOT/'metadata/integration-batch-131-applied.json').write_text(json.dumps(out,indent=2)+'\n')
print(json.dumps({k:out[k] for k in ['current_CTest_count','native_suites','canonical_native_cases','new_native_C_comparisons','complete_menu_reconstruction']}))
