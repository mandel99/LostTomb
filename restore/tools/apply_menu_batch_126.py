"""Guarded coordinator application of frozen editor and display phases."""
from pathlib import Path
import hashlib,json
ROOT=Path(__file__).resolve().parents[1]
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
read=lambda p:json.loads((ROOT/p).read_text())
pins={
 'artifacts/editor_validation_phase/apply-map.json':'fdb8cd6df48b6e248571854729bc5a16c17202cb12cee5bcebe02bfc13684ce2',
 'artifacts/display_confirmation_phase/metadata/apply-map.json':'78ff801a2a0f71e1b57998f91700d8e9a9d49dad24020c466375067013c23f1b',
 'artifacts/display_confirmation_phase/metadata/freeze-manifest.json':'98e02694b1548c814bed067e47aab72563ee037fc0dbe2eed41ee86e88966e40',
 'CMakeLists.txt':'799f7f753578cd3477b45161e563fe4a8b4111c643fd0d61af882d0eddc54f8e',
 'runtime/menu_callbacks.c':'68dbd3b45a89b69f3ec2a805fb6cf4b528a595a3a0730b33e23eba5cebb08e9d'}
for p,h in pins.items():assert sha(ROOT/p)==h,('precondition',p)
editor=read('artifacts/editor_validation_phase/manifest.json')
for p,h in {**editor['frozen_file_hashes'],**editor['external_tool_hash']}.items():assert sha(ROOT/p)==h,p
display=read('artifacts/display_confirmation_phase/metadata/freeze-manifest.json')
for p,h in display['files'].items():assert sha(ROOT/'artifacts/display_confirmation_phase'/p)==h,p
records=[]
for r in read('artifacts/editor_validation_phase/apply-map.json')['records']:
 records.append((r['source'],r['destination'],r['before_sha256'],r['candidate_sha256']))
for r in read('artifacts/display_confirmation_phase/metadata/apply-map.json')['files']:
 records.append((r['staged'],r['target'],r['before_sha256'],r['candidate_sha256']))
outputs={}
for source,dest,before,after in records:
 assert sha(ROOT/source)==after,source
 assert (sha(ROOT/dest) if (ROOT/dest).exists() else None)==before,dest
 outputs[dest]=(ROOT/source).read_bytes()
recipe=read('artifacts/display_confirmation_phase/metadata/integration-recipe.json')
w=recipe['root_owned_input_wiring'];path=w['path'];assert sha(ROOT/path)==w['expected_before_sha256']
data=(ROOT/path).read_bytes()
for r in w['replacements']:
 a,b=r['expected'].encode(),r['replacement'].encode();assert data.count(a)==r['expected_count'];data=data.replace(a,b)
assert hashlib.sha256(data).hexdigest()==w['expected_after_sha256'];outputs[path]=data
path='runtime/menu_callbacks.c';data=(ROOT/path).read_bytes()
changes=[(b'#include "selection_popup.h"\n',b'#include "editor_validation.h"\n#include "display_confirmation.h"\n'),
 (b'    if(restore_selection_popup_action(address))return;\n',b'    if(address==0x00409830u){FUN_00409830();return;}\n    if(restore_display_confirmation_action(address))return;\n')]
for a,b in changes:assert data.count(a)==1;data=data.replace(a,a+b,1)
outputs[path]=data
cmake='''
# Frozen complete editor validation and display confirmation algorithms.
target_sources(pharaoh_restored PRIVATE runtime/editor_validation.c
  runtime/display_confirmation.c runtime/display_confirmation_clock.c)
add_executable(restore_editor_validation_verify
  artifacts/editor_validation_phase/tests/editor_validation.c
  artifacts/editor_validation_phase/tests/editor_actual_trace.c
  artifacts/editor_validation_phase/tests/editor_actual_plain.c)
target_include_directories(restore_editor_validation_verify PRIVATE include artifacts/editor_validation_phase/tests)
target_compile_definitions(restore_editor_validation_verify PRIVATE _CRT_SECURE_NO_WARNINGS)
target_compile_options(restore_editor_validation_verify PRIVATE /W4 "$<$<CONFIG:Debug>:/Od>" "$<$<CONFIG:Release>:/O2>")
target_link_libraries(restore_editor_validation_verify PRIVATE restore_data advapi32)
target_link_options(restore_editor_validation_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
add_test(NAME standalone_editor_validation COMMAND restore_editor_validation_verify)
set_tests_properties(standalone_editor_validation PROPERTIES LABELS source-only)
add_executable(restore_display_confirmation_verify tests/display_confirmation.c runtime/display_confirmation.c
  src/unit_00025.c src/unit_00026.c src/unit_02436.c src/unit_01766.c src/unit_00119.c
  src/unit_00416.c src/unit_00417.c src/unit_01836.c src/unit_01857.c src/unit_01858.c src/unit_01775.c)
target_include_directories(restore_display_confirmation_verify PRIVATE include)
target_compile_definitions(restore_display_confirmation_verify PRIVATE _CRT_SECURE_NO_WARNINGS)
target_compile_options(restore_display_confirmation_verify PRIVATE /W4 /WX "$<$<CONFIG:Debug>:/Od>" "$<$<CONFIG:Release>:/O2>")
target_link_libraries(restore_display_confirmation_verify PRIVATE restore_data)
target_link_options(restore_display_confirmation_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
add_test(NAME standalone_display_confirmation COMMAND restore_display_confirmation_verify)
set_tests_properties(standalone_display_confirmation PROPERTIES LABELS source-only)
'''
outputs['CMakeLists.txt']=(ROOT/'CMakeLists.txt').read_bytes()+cmake.encode()
backup=ROOT/'artifacts/integration-batch-126-before';assert not backup.exists()
# All guards and exact output construction finish before the first mutation.
for name,data in outputs.items():
 p=ROOT/name
 if p.exists():
  saved=backup/name;saved.parent.mkdir(parents=True,exist_ok=True);saved.write_bytes(p.read_bytes())
 p.parent.mkdir(parents=True,exist_ok=True);p.write_bytes(data)
out={'format':1,'complete_menu_reconstruction':False,'frozen_inputs':pins,
 'written':{p:sha(ROOT/p) for p in outputs},'narrow_caller_wiring':w,
 'central_registry_insertions':[{'anchor':a.decode(),'inserted':b.decode()} for a,b in changes],
 'validation':'Applied only; build and native replay still required.'}
(ROOT/'metadata/integration-batch-126-application.json').write_text(json.dumps(out,indent=2)+'\n')
print(json.dumps({'applied':len(outputs),'requires_validation':True}))
