"""Accept the composed map backlog only after shared builds and full replays."""
from pathlib import Path
from datetime import datetime,timezone
import hashlib,json,re,xml.etree.ElementTree as ET
from map_postpass4_return_proof import verified_postpass4_supersession
ROOT=Path(__file__).resolve().parents[1]
sha=lambda p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest()
read=lambda p:json.loads((ROOT/p).read_text(encoding='utf-8-sig'))
APP='artifacts/map_refresh_application_phase/'
PLAN='artifacts/map_refresh_integration_phase/'
receipt=read(APP+'application-receipt.json');audit=read(APP+'post-apply-wiring-audit.json');plan=read(PLAN+'integration-manifest.json')
assert audit['passed'] and audit['ready_for_root_checkpoint_review'] and not audit['dry_run']
assert audit['current_production_proofs_complete'] and set(audit['configured'])=={'Debug','Release'}
assert set(audit['CTest_execution_evidence'])=={'Debug','Release'} and all(x['passed_CTests']==164 for x in audit['CTest_execution_evidence'].values())
assert receipt['predecessor']['sha256']==sha('metadata/integration-batch-134-applied.json')
assert sha(PLAN+'integration-manifest.json')=='26fb92cfb97e75a69f85847bee68643fe72694cbe8cfac9da130444dfc12b2ee'
assert sha(APP+'application-freeze.json')=='7948338af93d2ea5d1e642ba784d10d64c02ddcb065519a4236928a3d0187062'
paths={APP+'application-receipt.json',APP+'post-apply-wiring-audit.json',APP+'application-freeze.json',PLAN+'integration-manifest.json',PLAN+'integration-freeze.json',Path(__file__).relative_to(ROOT).as_posix()}
for frozen in [read(APP+'application-freeze.json'),read(PLAN+'integration-freeze.json')]:
 for p,h in frozen['files'].items():assert sha(p)==h,('frozen drift',p);paths.add(p)
for change in receipt['changes']:
 assert sha(change['path'])==change['after_sha256'],change['path'];paths.add(change['path'])
 if change['before_exists']:
  before=receipt['snapshot']+'/before/'+change['path'];assert sha(before)==change['before_sha256'];paths.add(before)
assert len(receipt['changes'])==43 and len(plan['mappings'])==42
adjustment=read('metadata/integration-batch-135-build-adjustments.json')
assert adjustment['before_sha256']=='92d720f804c80aad8b42909197fe35df97f1e1554ebb820dae9131e476d632e1'
assert adjustment['after_sha256']==sha('CMakeLists.txt')=='3addbc8039e57ce7176040f54512980c294598b3fa17cbcbf41df5b423c273a2'
assert sha(adjustment['header']['path'])==adjustment['header']['sha256'] and sha(adjustment['script']['path'])==adjustment['script']['sha256']
for entry in receipt['post_apply_adjustments']:assert sha(entry['path'])==entry['sha256'];paths.add(entry['path'])
paths.update([adjustment['header']['path'],adjustment['script']['path']])
def check_proof(path,expected):
 p=read(path);assert p['passed'] and p['checked']==expected,path
 for source,h in {**p.get('source_hashes',{}),**p.get('native_exports',{}),**p.get('binary_hashes',{})}.items():assert sha(source)==h,(path,source)
 for source,h in p.get('artifact_hashes_before_run',{}).items():assert sha(source)==h,(path,source)
 for variant in p.get('variants',{}).values():
  if isinstance(variant,dict) and 'path' in variant and 'sha256' in variant:assert sha(variant['path'])==variant['sha256'];paths.add(variant['path'])
 paths.add(path);return p
base_log='artifacts/menu-native-refresh-135.log';rows=[json.loads(s) for s in (ROOT/base_log).read_text().splitlines() if s.startswith('{"suite":')]
assert len(rows)==len({x['suite'] for x in rows})==97 and sum(x['cases'] for x in rows)==405528
for row in rows:
 name=row['suite'];p=check_proof('metadata/'+name.replace('_','-')+'-emulation.json',row['cases'])
 target='music' if name=='music_manager' else name;binary='build/c-only-debug/Debug/restore_'+target+'_verify.exe'
 assert sha(binary)==p['compiled_C_sha256'];paths.add(binary)
 if p.get('comparison_C_sha256'):
  binary='build/c-only/Release/restore_'+target+'_verify.exe';assert sha(binary)==p['comparison_C_sha256'];paths.add(binary)
map_summary=read('metadata/map-refresh-integration-replay-all.json')
assert map_summary['passed'] and map_summary['production'] and map_summary['suite_count']==15
assert map_summary['canonical_suite_cases']==141189 and map_summary['variant_comparisons']==435842
assert map_summary['integration_manifest_sha256']==sha(PLAN+'integration-manifest.json')
assert not map_summary['whole_map_refresh_complete'] and not map_summary['root134_broader_suite_counts_included']
for suite,actual in zip(plan['production_replays'],map_summary['suites']):
 assert actual['phase']==suite['phase'] and actual['report']==suite['output'] and sha(actual['report'])==actual['sha256']
 p=check_proof(actual['report'],suite['checked']);assert p['production'] and p.get('comparisons',p.get('variant_comparisons'))==suite['comparisons']
 assert not any(p.get(key) for key in ['source_drift','binary_drift','source_drift_at_completion'])
assert len(map_summary['suites'])==len(plan['production_replays'])==15
for cfg,directory in [('Debug','c-only-debug'),('Release','c-only')]:
 config=audit['configured'][cfg];assert config['build']=='build/'+directory
 assert config['configured_CTest_count']==164 and config['product_optimization']=='Disabled'
 assert sha(config['normal_product_library'])==config['normal_product_library_sha256'];paths.add(config['normal_product_library'])
 assert sha(config['normal_product_project'])==config['normal_product_project_sha256'];paths.add(config['normal_product_project'])
 cache='build/'+directory+'/CMakeCache.txt';text=(ROOT/cache).read_text();paths.add(cache)
 for key in ['RESTORE_ORIGINAL_EXE','RESTORE_GAME_EXE']:assert re.findall(r'^'+key+r':[^=]*=(.*)$',text,re.M)==['']
 assert re.search(r'^RESTORE_BUILD_HYBRID:BOOL=OFF$',text,re.M)
 link='build/'+directory+'/'+cfg+'/restore_link_check.exe';assert (ROOT/link).is_file();paths.add(link)
 for target,entry in config['verification_targets'].items():
  assert sha(entry['project'])==entry['project_sha256'] and sha(entry['exe'])==entry['exe_sha256'];paths.update([entry['project'],entry['exe']])
 junit='artifacts/menu-135-'+cfg.lower()+'-tests.xml';tree=ET.parse(ROOT/junit);assert len(tree.findall('.//testcase'))==164 and not tree.findall('.//failure') and not tree.findall('.//error') and not tree.findall('.//skipped');paths.add(junit)
 supplement='artifacts/menu-135-'+cfg.lower()+'-rebuild-tests.xml';tree=ET.parse(ROOT/supplement);assert len(tree.findall('.//testcase'))==4 and not tree.findall('.//failure') and not tree.findall('.//error');paths.add(supplement)
 paths.add('artifacts/menu-135-'+cfg.lower()+'-rebuild-tests.log')
 for target in ['restore_initial_menu_dispatch_verify','restore_menu_window_cursor_consumer_verify','restore_menu_text_glyph_verify','restore_refresh_menu_dispatch_verify']:paths.add('artifacts/menu-135-'+cfg.lower()+'-'+target+'-rebuild.log')
 for log in ['artifacts/menu-135-'+cfg.lower()+'-build.log','artifacts/menu-135-'+cfg.lower()+'-first-build.log']:paths.add(log)
for path in ['artifacts/menu-135-return-audit.log','artifacts/menu-135-string-audit.log']:
 p=read(path);assert p['passed'] and p['issues']==[];paths.add(path)
assert read('artifacts/menu-135-return-audit.log')['remaining_discarded_guard_sites']==0
guard=verified_postpass4_supersession(ROOT);paths.update(guard['verified_source_and_evidence_hashes'])
guard_tests=read('artifacts/menu-135-return-guard-tests.log');assert guard_tests['passed'] and guard_tests['positive_proof']==1 and guard_tests['unqualified_reports_rejected']==15
paths.update(['tools/verify_discarded_returns_135.py','tools/map_postpass4_return_proof.py','tools/test_map_postpass4_return_proof.py','artifacts/menu-135-return-guard-tests.log','artifacts/menu-native-refresh-135-first.log'])
dropdown=read('metadata/dropdown-action-status.json');assert dropdown['current_click_routes_available']==dropdown['proved_callback_bodies']==30 and dropdown['missing_tokens']==[]
paths.update([base_log,'artifacts/menu-135-map-production-replay.log','metadata/map-refresh-integration-replay-all.json','metadata/dropdown-action-status.json','metadata/settings-callback-status.json','metadata/discarded-return-verification.json','metadata/string-binding-verification.json','tools/refresh_menu_native_evidence.py','tools/repair_discarded_returns.py','tools/repair_string_bindings.mjs','README.md'])
out={'format':1,'generated_at':datetime.now(timezone.utc).isoformat(),'complete_menu_reconstruction':False,'whole_map_refresh_complete':False,
 'predecessor':receipt['predecessor'],'scope':'Compose the immutable map-neighbor, refresh geometry/arithmetic and postpass1..8 backlog as28 actual canonical C functions,12headers and2typed numeric runtimes. Real actual-source production Debug/Release trace/plain comparisons; the P3 test-header include is an explicitly recorded build-only correction.',
 'current_CTest_count':164,'configurations_passed':['Debug','Release'],'native_suites':112,'canonical_native_cases':546717,
 'recompiled_verification_target_smoke_supplements':{'Debug':4,'Release':4},'postpass4_historical_call_guard_supersession':{'whole_native_instructions':384,'full_cases':4674,'trace_plain_C_comparisons':18696,'negative_gate_cases_rejected':15},
 'base_native_suites':97,'base_canonical_cases':405528,'new_native_suites':15,'new_native_cases':141189,'new_native_C_comparisons':435842,
 'new_numbered_function_sources':28,'new_headers':12,'new_numeric_runtime_sources':2,'new_CTests':25,
 'counts_deduplicate_game_scenarios':False,'native_only_evidence_counted_as_C_comparisons':False,
 'current_top_table_routes':30,'top_table_proved_callback_bodies':30,'original_EXE_build_dependency':False,'original_game_process_executed':False,'computer_UI_automation_used':False,'C_sharp_used_as_exact_behavior':False,
 'remaining_obligations':['The complete4D8310 input controller remains a separately staged C candidate; its private pilots are not included in this shared-build case count.',
  'Frozen actor path/action graphs require their separate guarded application and shared production replay.',
  'Map nodes475CC0,488E90 and canonical489030, canonical implicit-ST0/56133C, FPSW/unmasked-trap qualifications remain open.',
  'Deeper menu/render algorithms, physical screen composition, actual OS/persistence effects and complete usable menu flow remain unproved.',
  'Known1768 GetCursorPos(FALSE) qualification remains open.'],
 'map_qualifications':audit['qualifications'],'higher_open_graph':audit['higher_open_graph'],
 'source_and_evidence_hashes':{p:sha(p) for p in sorted(paths)}}
(ROOT/'metadata/integration-batch-135-applied.json').write_text(json.dumps(out,indent=2)+'\n')
print(json.dumps({key:out[key] for key in ['current_CTest_count','native_suites','canonical_native_cases','new_native_C_comparisons','complete_menu_reconstruction']}))
