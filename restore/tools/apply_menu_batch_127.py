"""Apply the immutable replay-mission phase only to exact accepted predecessors."""
from pathlib import Path
import hashlib,json
ROOT=Path(__file__).resolve().parents[1]
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
read=lambda p:json.loads((ROOT/p).read_text())
accepted=read('metadata/integration-batch-126-applied.json')
for p,h in accepted['source_and_evidence_hashes'].items():assert sha(ROOT/p)==h,('checkpoint drift',p)
assert sha(ROOT/'artifacts/replay_mission_phase/apply-map.json')=='6c41db12039c0b0ec2dceb016fac38224d4b1d1e3c3455f4327d10f4ee9d37ae'
frozen=read('artifacts/replay_mission_phase/manifest.json')
for p,h in {**frozen['frozen_file_hashes'],**frozen['external_tool_hash']}.items():assert sha(ROOT/p)==h,p
records=read('artifacts/replay_mission_phase/apply-map.json')['records'];outputs={}
for r in records:
 p=ROOT/r['destination'];assert (sha(p) if p.exists() else None)==r['before_sha256'],p
 assert sha(ROOT/r['source'])==r['candidate_sha256'];outputs[r['destination']]=(ROOT/r['source']).read_bytes()
changes=[(b'#include "display_confirmation.h"\n',b'#include "replay_mission.h"\n'),(b'    if(restore_display_confirmation_action(address))return;\n',b'    if(address==0x00418510u){FUN_00418510();return;}\n')]
path='runtime/menu_callbacks.c';data=(ROOT/path).read_bytes()
assert sha(ROOT/path)=='d7484ed726680ea5edfd4fdf175e91666e229ed8d6f007b92058f24488d4b529'
for a,b in changes:assert data.count(a)==1;data=data.replace(a,a+b,1)
outputs[path]=data
assert sha(ROOT/'CMakeLists.txt')=='17a66209e9bb4426026e687804f8a8f5a0da45f8d0ebd253863fddecd8fa734c'
cmake='''
# Complete native replay callback, fallback and real profile getter composition.
target_sources(pharaoh_restored PRIVATE runtime/replay_mission.c)
add_executable(restore_replay_mission_verify
  artifacts/replay_mission_phase/tests/replay_mission.c
  artifacts/replay_mission_phase/tests/replay_actual_trace.c
  artifacts/replay_mission_phase/tests/replay_actual_plain.c)
target_include_directories(restore_replay_mission_verify PRIVATE include artifacts/replay_mission_phase/tests)
target_compile_definitions(restore_replay_mission_verify PRIVATE _CRT_SECURE_NO_WARNINGS)
target_compile_options(restore_replay_mission_verify PRIVATE /W4 "$<$<CONFIG:Debug>:/Od>" "$<$<CONFIG:Release>:/O2>")
target_link_libraries(restore_replay_mission_verify PRIVATE restore_data advapi32)
target_link_options(restore_replay_mission_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
add_test(NAME standalone_replay_mission COMMAND restore_replay_mission_verify)
set_tests_properties(standalone_replay_mission PROPERTIES LABELS source-only)
'''
outputs['CMakeLists.txt']=(ROOT/'CMakeLists.txt').read_bytes()+cmake.encode()
backup=ROOT/'artifacts/integration-batch-127-before';assert not backup.exists()
for path in ['tools/refresh_menu_native_evidence.py','tools/verify_remaining_menu_click_emulation.py','tools/record_dropdown_actions.py','metadata/dropdown-action-status.json','metadata/settings-callback-status.json']:
 saved=backup/path;saved.parent.mkdir(parents=True,exist_ok=True);saved.write_bytes((ROOT/path).read_bytes())
for path,data in outputs.items():
 p=ROOT/path
 if p.exists():
  saved=backup/path;saved.parent.mkdir(parents=True,exist_ok=True);saved.write_bytes(p.read_bytes())
 p.parent.mkdir(parents=True,exist_ok=True);p.write_bytes(data)
out={'format':1,'complete_menu_reconstruction':False,'predecessor':'metadata/integration-batch-126-applied.json','predecessor_sha256':sha(ROOT/'metadata/integration-batch-126-applied.json'),
 'apply_map_sha256':sha(ROOT/'artifacts/replay_mission_phase/apply-map.json'),'written':{p:sha(ROOT/p) for p in outputs},'registry_insertions':[{'anchor':a.decode(),'inserted':b.decode()} for a,b in changes],
 'validation':'Applied only; actual product build and native replay required.'}
(ROOT/'metadata/integration-batch-127-application.json').write_text(json.dumps(out,indent=2)+'\n')
print(json.dumps({'applied_files':len(outputs),'requires_validation':True}))
