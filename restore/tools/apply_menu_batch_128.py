"""Guarded application of the whole initial dispatcher and four-action proof."""
from pathlib import Path
import hashlib,json
ROOT=Path(__file__).resolve().parents[1];PHASE=ROOT/'artifacts/initial_menu_dispatch_phase'
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
read=lambda p:json.loads((ROOT/p).read_text())
accepted=read('metadata/integration-batch-127-applied.json')
for p,h in accepted['source_and_evidence_hashes'].items():assert sha(ROOT/p)==h,('checkpoint drift',p)
proof=read('artifacts/initial_menu_dispatch_phase/private-emulation.json')
assert proof['passed'] and proof['complete_case_set'] and proof['checked']==5795 and proof['comparisons']==23180
for key,want in {'native_instruction_coverage':[179,179],'conditional_outcomes':[8,8],'switch_mode_coverage':[88,88],'switch_target_coverage':[47,47],'native_call_site_coverage':[58,58],'deep_endpoint_coverage':[50,50],'mutation_phases':100,'source_drift':False}.items():assert proof[key]==want,key
for p,h in proof['source_hashes'].items():assert sha(ROOT/p)==h,p
for v in proof['variants'].values():assert sha(ROOT/v['path'])==v['sha256']
frozen=read('artifacts/menu_existing_actions_phase/manifest.json')
assert sha(ROOT/'artifacts/menu_existing_actions_phase/apply-map.json')=='d2cfa8e85df88d9cd807c7bceb34048d887bd8d6d52995e7d03d5841f3a57c08'
for p,h in {**frozen['frozen_file_hashes'],**frozen['external_tool_hash']}.items():assert sha(ROOT/p)==h,p
for r in read('artifacts/menu_existing_actions_phase/apply-map.json')['unchanged_source_attestations']:assert sha(ROOT/r['path'])==r['before_sha256']==r['candidate_sha256'],r['path']
outputs={}
for path,before in [('src/unit_01866.c','707a3b53d695a2430e1778c664768f737798f81f4d2da9e59295ee46e96bc6fe'),('include/initial_menu_dispatch.h',None),('runtime/initial_menu_dispatch_boundary.c',None)]:
 assert (sha(ROOT/path) if (ROOT/path).exists() else None)==before,path
 outputs[path]=(PHASE/path).read_bytes()
cmake='''
# Whole initial menu dispatcher; renderer algorithms remain qualified boundaries.
target_sources(pharaoh_restored PRIVATE runtime/initial_menu_dispatch_boundary.c)
add_executable(restore_initial_menu_dispatch_verify
  artifacts/initial_menu_dispatch_phase/tests/initial_dispatch.c
  artifacts/initial_menu_dispatch_phase/tests/actual_trace.c
  artifacts/initial_menu_dispatch_phase/tests/actual_plain.c)
target_include_directories(restore_initial_menu_dispatch_verify PRIVATE include artifacts/initial_menu_dispatch_phase/tests)
target_compile_definitions(restore_initial_menu_dispatch_verify PRIVATE _CRT_SECURE_NO_WARNINGS)
target_compile_options(restore_initial_menu_dispatch_verify PRIVATE /W4 /WX "$<$<CONFIG:Debug>:/Od>" "$<$<CONFIG:Release>:/O2>")
target_link_libraries(restore_initial_menu_dispatch_verify PRIVATE restore_data)
target_link_options(restore_initial_menu_dispatch_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
add_test(NAME standalone_initial_menu_dispatch COMMAND restore_initial_menu_dispatch_verify)
set_tests_properties(standalone_initial_menu_dispatch PROPERTIES LABELS source-only)
add_executable(restore_menu_existing_actions_verify
  artifacts/menu_existing_actions_phase/tests/menu_existing_actions.c
  artifacts/menu_existing_actions_phase/tests/existing_actual_entries.c
  artifacts/menu_existing_actions_phase/tests/existing_actual_loadsave.c
  artifacts/menu_existing_actions_phase/tests/existing_actual_helpers.c
  artifacts/menu_existing_actions_phase/tests/unreached_link_sentinels.c)
target_include_directories(restore_menu_existing_actions_verify PRIVATE include artifacts/menu_existing_actions_phase/tests)
target_compile_definitions(restore_menu_existing_actions_verify PRIVATE _CRT_SECURE_NO_WARNINGS)
target_compile_options(restore_menu_existing_actions_verify PRIVATE /W4 /Gy "$<$<CONFIG:Debug>:/Od>" "$<$<CONFIG:Release>:/O2>")
target_link_libraries(restore_menu_existing_actions_verify PRIVATE restore_data advapi32)
target_link_options(restore_menu_existing_actions_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED /OPT:REF /INCREMENTAL:NO)
add_test(NAME standalone_menu_existing_actions COMMAND restore_menu_existing_actions_verify)
set_tests_properties(standalone_menu_existing_actions PROPERTIES LABELS source-only)
'''
outputs['CMakeLists.txt']=(ROOT/'CMakeLists.txt').read_bytes()+cmake.encode()
backup=ROOT/'artifacts/integration-batch-128-before';assert not backup.exists()
for path in ['tools/repair_discarded_returns.py','tools/refresh_menu_native_evidence.py','tools/record_dropdown_actions.py','metadata/dropdown-action-status.json','metadata/settings-callback-status.json']:
 saved=backup/path;saved.parent.mkdir(parents=True,exist_ok=True);saved.write_bytes((ROOT/path).read_bytes())
frozen_initial={p.relative_to(ROOT).as_posix():sha(p) for p in PHASE.rglob('*') if p.is_file() and p.name!='freeze-manifest.json'}
frozen_initial['tools/verify_initial_menu_dispatch_emulation.py']=sha(ROOT/'tools/verify_initial_menu_dispatch_emulation.py')
(PHASE/'freeze-manifest.json').write_text(json.dumps({'format':1,'frozen_files':frozen_initial,'production_applied_by_coordinator':False},indent=2)+'\n')
for path,data in outputs.items():
 p=ROOT/path
 if p.exists():
  saved=backup/path;saved.parent.mkdir(parents=True,exist_ok=True);saved.write_bytes(p.read_bytes())
 p.parent.mkdir(parents=True,exist_ok=True);p.write_bytes(data)
out={'format':1,'complete_menu_reconstruction':False,'predecessor':'metadata/integration-batch-127-applied.json','predecessor_sha256':sha(ROOT/'metadata/integration-batch-127-applied.json'),'initial_frozen_manifest_sha256':sha(PHASE/'freeze-manifest.json'),'existing_actions_apply_map_sha256':sha(ROOT/'artifacts/menu_existing_actions_phase/apply-map.json'),'written':{p:sha(ROOT/p) for p in outputs},'no_central_registry_changes':True,'validation':'Applied only; actual shared Debug/Release and full production native replays required.'}
(ROOT/'metadata/integration-batch-128-application.json').write_text(json.dumps(out,indent=2)+'\n')
print(json.dumps({'applied':len(outputs),'new_test_targets':2,'requires_validation':True}))
