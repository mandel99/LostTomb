"""Apply the frozen whole refresh controller after checkpoint130 acceptance."""
from pathlib import Path
import hashlib,json
ROOT=Path(__file__).resolve().parents[1];PHASE=ROOT/'artifacts/refresh_menu_dispatch_phase'
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
accepted=json.loads((ROOT/'metadata/integration-batch-130-applied.json').read_text())
for p,h in accepted['source_and_evidence_hashes'].items():assert sha(ROOT/p)==h,('checkpoint130 drift',p)
manifest=PHASE/'freeze-manifest.json';assert sha(manifest)=='ddd403e31bc13a45fc51c88838cbc99342ef8f78b2e94e4f5d31e71e9f59271b'
frozen=json.loads(manifest.read_text())
for p,h in frozen['files'].items():assert sha(ROOT/p)==h,('frozen candidate drift',p)
proof=json.loads((ROOT/'metadata/refresh-menu-dispatch-emulation.json').read_text())
assert proof['passed'] and not proof['production'] and proof['complete_case_set'] and proof['checked']==11633 and proof['comparisons']==23266
assert proof['native_instruction_coverage']==[960,960] and proof['controller_instruction_coverage']==[917,917] and proof['conditional_outcomes']==[250,250]
assert len(proof['reached_endpoints'])==96 and len(proof['mutated_endpoint_phases'])==192
for p,h in {**proof['source_hashes'],**proof['binary_hashes']}.items():assert sha(ROOT/p)==h,('extended private proof drift',p)
old='638bc1f8ed206a710ce6c3afa82e324dff781a3a3ecf0528bacd7d9f7ded6b9c';assert sha(ROOT/'src/unit_01867.c')==old
assert not (ROOT/'include/refresh_menu_dispatch.h').exists() and not (ROOT/'runtime/refresh_menu_dispatch_boundary.c').exists()
outputs={p:(PHASE/p).read_bytes() for p in ['src/unit_01867.c','include/refresh_menu_dispatch.h','runtime/refresh_menu_dispatch_boundary.c']}
cmake='''
# Whole4D75D0 native-verified refresh controller. Deep render/media endpoints
# are controlled only in the oracle; the product links its actual C functions.
target_sources(pharaoh_restored PRIVATE runtime/refresh_menu_dispatch_boundary.c)
add_executable(restore_refresh_menu_dispatch_verify
  artifacts/refresh_menu_dispatch_phase/tests/refresh_dispatch.c
  artifacts/refresh_menu_dispatch_phase/tests/actual_mission_buttons.c
  src/unit_01867.c
  src/unit_00286.c src/unit_00293.c src/unit_00295.c
  src/unit_00296.c src/unit_00297.c src/unit_02276.c)
target_include_directories(restore_refresh_menu_dispatch_verify PRIVATE include artifacts/refresh_menu_dispatch_phase/tests)
target_compile_definitions(restore_refresh_menu_dispatch_verify PRIVATE _CRT_SECURE_NO_WARNINGS)
target_compile_options(restore_refresh_menu_dispatch_verify PRIVATE /W4 /WX "$<$<CONFIG:Debug>:/Od>" "$<$<CONFIG:Release>:/O2>")
# Actual295 is MOV EAX,57CD70;RET. Its unchanged DWORD* to byte* return has
# known C4133; this file alone retains the same suppression as private proof.
set_source_files_properties(src/unit_00295.c PROPERTIES COMPILE_OPTIONS /wd4133)
target_link_libraries(restore_refresh_menu_dispatch_verify PRIVATE restore_data advapi32)
target_link_options(restore_refresh_menu_dispatch_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
add_test(NAME standalone_refresh_menu_dispatch COMMAND restore_refresh_menu_dispatch_verify)
set_tests_properties(standalone_refresh_menu_dispatch PROPERTIES LABELS source-only)
'''
outputs['CMakeLists.txt']=(ROOT/'CMakeLists.txt').read_bytes()+cmake.encode()
backup=ROOT/'artifacts/integration-batch-131-before';assert not backup.exists()
protected=['src/unit_01866.c','src/unit_01868.c','runtime/menu_callbacks.c','runtime/mission_menu.c','src/unit_00286.c','src/unit_00293.c','src/unit_00295.c','src/unit_00296.c','src/unit_00297.c','src/unit_02276.c']
protected_hashes={p:sha(ROOT/p) for p in protected}
for p in ['tools/refresh_menu_native_evidence.py','tools/repair_discarded_returns.py','metadata/dropdown-action-status.json','metadata/settings-callback-status.json','metadata/refresh-menu-dispatch-emulation.json']+list(outputs):
 if (ROOT/p).exists():
  dest=backup/p;dest.parent.mkdir(parents=True,exist_ok=True);dest.write_bytes((ROOT/p).read_bytes())
for p,data in outputs.items():dest=ROOT/p;dest.parent.mkdir(parents=True,exist_ok=True);dest.write_bytes(data)
out={'format':1,'complete_menu_reconstruction':False,'predecessor':'metadata/integration-batch-130-applied.json','predecessor_sha256':sha(ROOT/'metadata/integration-batch-130-applied.json'),'frozen_manifest':{'path':manifest.relative_to(ROOT).as_posix(),'sha256':sha(manifest)},'written':{p:sha(ROOT/p) for p in outputs},'protected_unchanged':protected_hashes,'old_controller_sha256':old,'private_proof_backup':'artifacts/integration-batch-131-before/metadata/refresh-menu-dispatch-emulation.json','validation':'Applied candidate only; full actual production replay and Debug/Release builds required.'}
(ROOT/'metadata/integration-batch-131-application.json').write_text(json.dumps(out,indent=2)+'\n')
print(json.dumps({'applied_files':len(outputs),'requires_production_validation':True}))
