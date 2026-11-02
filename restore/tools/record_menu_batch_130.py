"""Record batch130 only after actual wiring/build/native evidence is current."""
from pathlib import Path
from datetime import datetime,timezone
import argparse,hashlib,json
SCRIPT=Path(__file__).resolve()
ROOT=next(p for p in SCRIPT.parents if (p/'metadata/integration-batch-130-application.json').is_file())
PRIVATE=SCRIPT.parent!=ROOT/'tools'
parser=argparse.ArgumentParser(description=__doc__)
parser.add_argument('--audit',type=Path,default=(SCRIPT.parents[1]/'menu-130-wiring-audit.json' if PRIVATE else ROOT/'metadata/menu-130-wiring-audit.json'))
parser.add_argument('--output',type=Path,default=(SCRIPT.parents[1]/'integration-batch-130-applied.json' if PRIVATE else ROOT/'metadata/integration-batch-130-applied.json'))
args=parser.parse_args()
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
read=lambda p:json.loads((ROOT/p).read_text(encoding='utf-8-sig'))
audit=json.loads(args.audit.read_text());assert audit['passed'] and audit['complete_menu_reconstruction'] is False
for p,h in audit['source_and_evidence_hashes'].items():assert sha(ROOT/p)==h,('audit freshness',p)
assert audit['dispatcher_bytewise_replacements']==audit['registry_bytewise_insertions']==2
assert audit['qualified_new_proofs']['queries']['ordinary_outcomes']==[191,193] and audit['known_1768_GetCursorPos_FALSE_gap']['closed'] is False
log=ROOT/'artifacts/menu-native-refresh-130.log';suites=[json.loads(line) for line in log.read_text().splitlines() if line.startswith('{"suite":')]
assert len(suites)==len({s['suite'] for s in suites})==92 and sum(s['cases'] for s in suites)==380544
paths=set(audit['source_and_evidence_hashes']);suite_summary=[]
for s in suites:
 name=s['suite'];path='metadata/'+name.replace('_','-')+'-emulation.json';report=read(path)
 assert report['passed'] and report['checked']==s['cases'],name
 consumed={**report.get('source_hashes',{}),**report.get('native_exports',{})}
 for p,h in consumed.items():assert sha(ROOT/p)==h,(name,p)
 paths.update(consumed)
 target='music' if name=='music_manager' else name;binary=f'build/c-only-debug/Debug/restore_{target}_verify.exe'
 assert sha(ROOT/binary)==report['compiled_C_sha256'],(name,'Debug binary freshness');paths.add(binary)
 if report.get('comparison_C_sha256'):
  binary=f'build/c-only/Release/restore_{target}_verify.exe';assert sha(ROOT/binary)==report['comparison_C_sha256'],(name,'Release binary freshness');paths.add(binary)
 for variant in report.get('variants',{}).values():
  if isinstance(variant,dict) and 'path' in variant and 'sha256' in variant:assert sha(ROOT/variant['path'])==variant['sha256'],(name,'variant freshness');paths.add(variant['path'])
 paths.add(path);suite_summary.append({'suite':name,'cases':s['cases'],'rerun':s['rerun']})
dropdown=read('metadata/dropdown-action-status.json');assert dropdown['current_click_routes_available']==dropdown['proved_callback_bodies']==30 and dropdown['missing_tokens']==[]
returns=read('artifacts/menu-130-return-audit.log');assert returns['passed'] and returns['remaining_discarded_guard_sites']==0 and returns['issues']==[]
strings=read('artifacts/menu-130-string-audit.log');assert strings['passed'] and strings['issues']==[]
paths.update(['metadata/dropdown-action-status.json','metadata/settings-callback-status.json','artifacts/menu-native-refresh-130.log','artifacts/menu-130-return-audit.log','artifacts/menu-130-string-audit.log','tools/refresh_menu_native_evidence.py','tools/repair_discarded_returns.py',SCRIPT.relative_to(ROOT).as_posix(),args.audit.resolve().relative_to(ROOT).as_posix()])
app=read('metadata/integration-batch-130-application.json');modal=read('metadata/menu-keyboard-modal-emulation.json');cursor=read('metadata/menu-cursor-emulation.json');queries=read('metadata/training-queries-emulation.json')
assert all(row['CTest_passed']==134 for row in audit['configurations'].values())
out={'format':1,'generated_at':datetime.now(timezone.utc).isoformat(),'complete_menu_reconstruction':False,'predecessor':{'path':app['predecessor'],'sha256':app['predecessor_sha256']},'application':{'path':'metadata/integration-batch-130-application.json','sha256':sha(ROOT/'metadata/integration-batch-130-application.json')},'scope':'Qualified modal/window Escape controller415instructions and exact32-instruction mode54 dispatcher wiring; full99-instruction cursor selector with real WINAPI provider outside finite oracle; real nine-function resource-query closure517instructions/506reachable, eight complete replacements. No claim that complete input dispatcher, refresh controller, pixels or whole menu is proved.','current_CTest_count':134,'configurations_passed':['Debug','Release'],'native_suites':92,'canonical_native_cases':380544,'new_native_cases':15475,'new_native_C_comparisons':30950,'qualified_new_proofs':audit['qualified_new_proofs'],'application_written_verified':audit['application_written_verified'],'dispatcher_bytewise_replacements':2,'registry_bytewise_insertions':2,'new_modal_action_token':'00538920','current_top_table_routes':30,'top_table_proved_callback_bodies':30,'source_only_build':True,'original_EXE_build_dependency':False,'original_game_process_executed':False,'computer_UI_automation_used':False,'known_1768_GetCursorPos_FALSE_gap':audit['known_1768_GetCursorPos_FALSE_gap'],'refresh_product_unchanged':audit['refresh_product_unchanged'],'remaining_obligations':['Existing1768 GetCursorPos(FALSE) early return differs from native undefined POINT consumption; this gap remains open.','Whole4D75D0 refresh controller and remaining4D8310 input dispatcher beyond accepted blocks require separate complete proofs.','Modal proof covers declared selected54 frame and valid selected indices; invalid native private-stack indices and excluded noalias count edge retain their exact qualifications.','Physical cursor resources/SetCursor effects, OS event delivery, render/text rasterization and complete screen composition are not established by finite API boundaries.','Nine real query functions are closed independently; complete resource-panel raster/text and every external consumer remain separate requirements.','Deeper gameplay, I/O/persistence and menu visual fidelity/live playability are not established by this offline checkpoint.'],'limitations':{'modal':modal['limitations'],'cursor':cursor['limitations'],'queries':queries['limitations']},'native_refresh_suites':suite_summary,'source_and_evidence_hashes':{p:sha(ROOT/p) for p in sorted(paths)}}
args.output.parent.mkdir(parents=True,exist_ok=True);args.output.write_text(json.dumps(out,indent=2)+'\n')
print(json.dumps({k:out[k] for k in ['current_CTest_count','native_suites','canonical_native_cases','new_native_C_comparisons','complete_menu_reconstruction']}))
