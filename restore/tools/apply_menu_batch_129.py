"""Apply the frozen whole training-panel reconstruction to exact predecessors."""
from pathlib import Path
import hashlib,json
ROOT=Path(__file__).resolve().parents[1]
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
read=lambda p:json.loads((ROOT/p).read_text())
accepted=read('metadata/integration-batch-128-applied.json')
for p,h in accepted['source_and_evidence_hashes'].items():assert sha(ROOT/p)==h,('checkpoint drift',p)
map_path='artifacts/training_panel_phase/apply-map.json'
assert sha(ROOT/map_path)=='ed9aa8d98a10281b4886149eed0d9f657680671cd2b992df7dbce7e7f845f6f5'
frozen=read('artifacts/training_panel_phase/manifest.json')
for p,h in {**frozen['frozen_file_hashes'],**frozen['external_tool_hash']}.items():assert sha(ROOT/p)==h,p
outputs={}
for r in read(map_path)['records']:
 p=ROOT/r['destination'];assert (sha(p) if p.exists() else None)==r['before_sha256'],p
 assert sha(ROOT/r['source'])==r['candidate_sha256'];outputs[r['destination']]=(ROOT/r['source']).read_bytes()
cmake='''
# Complete native1001-instruction training panel; explicit query/render boundaries.
add_executable(restore_training_panel_verify
  artifacts/training_panel_phase/tests/training_panel.c
  artifacts/training_panel_phase/tests/training_actual_trace.c
  artifacts/training_panel_phase/tests/training_actual_plain.c)
target_include_directories(restore_training_panel_verify PRIVATE include artifacts/training_panel_phase/tests)
target_compile_definitions(restore_training_panel_verify PRIVATE _CRT_SECURE_NO_WARNINGS)
target_compile_options(restore_training_panel_verify PRIVATE /W4 "$<$<CONFIG:Debug>:/Od>" "$<$<CONFIG:Release>:/O2>")
target_link_libraries(restore_training_panel_verify PRIVATE restore_data advapi32)
target_link_options(restore_training_panel_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
add_test(NAME standalone_training_panel COMMAND restore_training_panel_verify)
set_tests_properties(standalone_training_panel PROPERTIES LABELS source-only)
'''
outputs['CMakeLists.txt']=(ROOT/'CMakeLists.txt').read_bytes()+cmake.encode()
backup=ROOT/'artifacts/integration-batch-129-before';assert not backup.exists()
for path in ['tools/refresh_menu_native_evidence.py','metadata/dropdown-action-status.json','metadata/settings-callback-status.json','metadata/training-panel-emulation.json']:
 p=ROOT/path
 if p.exists():
  saved=backup/path;saved.parent.mkdir(parents=True,exist_ok=True);saved.write_bytes(p.read_bytes())
for path,data in outputs.items():
 p=ROOT/path
 if p.exists():
  saved=backup/path;saved.parent.mkdir(parents=True,exist_ok=True);saved.write_bytes(p.read_bytes())
 p.parent.mkdir(parents=True,exist_ok=True);p.write_bytes(data)
out={'format':1,'complete_menu_reconstruction':False,'predecessor':'metadata/integration-batch-128-applied.json','predecessor_sha256':sha(ROOT/'metadata/integration-batch-128-applied.json'),'apply_map_sha256':sha(ROOT/map_path),'written':{p:sha(ROOT/p) for p in outputs},'protected_unchanged':{p:sha(ROOT/p) for p in ['src/unit_01866.c','src/unit_01867.c','src/unit_01868.c','runtime/menu_callbacks.c','runtime/initial_menu_dispatch_boundary.c']},'validation':'Applied only; actual shared Debug/Release and full native replay required.'}
(ROOT/'metadata/integration-batch-129-application.json').write_text(json.dumps(out,indent=2)+'\n')
print(json.dumps({'applied':len(outputs),'new_test_targets':1,'requires_validation':True}))
