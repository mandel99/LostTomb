"""Apply three immutable menu phases with exact source and caller guards."""
from pathlib import Path
import hashlib,json
ROOT=Path(__file__).resolve().parents[1]
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
read=lambda p:json.loads((ROOT/p).read_text())
accepted=read('metadata/integration-batch-129-applied.json')
for p,h in accepted['source_and_evidence_hashes'].items():assert sha(ROOT/p)==h,('checkpoint drift',p)
pins={
 'artifacts/menu_keyboard_modal_phase/metadata/apply-map.json':'3382c6bf7e42163ff931c35d895f3ebc495d6b878f0f88dae0f226c21875d118',
 'artifacts/menu_keyboard_modal_phase/metadata/freeze-manifest.json':'60da3f1f0314ffcafc27e9ed6cbe426bd29106fa5e8c94e749c9ca120c58d7fc',
 'artifacts/menu_cursor_phase/metadata/apply-map.json':'063ec148624519ff609b5dfcd5d0d52096ba90279f20a6aacfe3cf639a0735d8',
 'artifacts/menu_cursor_phase/metadata/freeze-manifest.json':'0349ff17ecbf76a2b634cbbb9d23181ff171ff21d2d25bfe92419d87c35523dd',
 'artifacts/training_queries_phase/apply-map.json':'db5017a01457effba6acefbd1f6b110f1b37b68c468ac7191a226783b2ff7e6b',
 'artifacts/training_queries_phase/manifest.json':'32002632024ea611e0d05d4e7b2f0b4d574aa7292fedfe59196fe9a7ee89a517'}
for p,h in pins.items():assert sha(ROOT/p)==h,p
for phase in ['menu_keyboard_modal_phase','menu_cursor_phase']:
 for p,h in read('artifacts/'+phase+'/metadata/freeze-manifest.json')['files'].items():assert sha(ROOT/p)==h,p
frozen=read('artifacts/training_queries_phase/manifest.json')
for p,h in {**frozen['frozen_file_hashes'],**frozen['external_tool_hash']}.items():assert sha(ROOT/p)==h,p
records=[]
for phase in ['menu_keyboard_modal_phase','menu_cursor_phase']:
 for r in read('artifacts/'+phase+'/metadata/apply-map.json')['files']:records.append((r['staged'],r['target'],r['before_sha256'],r['candidate_sha256']))
queries=read('artifacts/training_queries_phase/apply-map.json')
for r in queries['records']:records.append((r['source'],r['destination'],r['before_sha256'],r['candidate_sha256']))
assert sha(ROOT/queries['unchanged_dependency']['path'])==queries['unchanged_dependency']['sha256']
outputs={}
for source,destination,before,after in records:
 p=ROOT/destination;assert (sha(p) if p.exists() else None)==before,destination
 assert sha(ROOT/source)==after,source;assert destination not in outputs,destination
 outputs[destination]=(ROOT/source).read_bytes()
modal=read('artifacts/menu_keyboard_modal_phase/metadata/integration-recipe.json');w=modal['dispatcher'];assert sha(ROOT/w['path'])==w['before_sha256'];data=(ROOT/w['path']).read_bytes()
for r in w['replacements']:
 a,b=r['old'].encode(),r['new'].encode();assert data.count(a)==r['expected_count'];data=data.replace(a,b,1)
assert hashlib.sha256(data).hexdigest()==w['after_sha256'];outputs[w['path']]=data
registry='runtime/menu_callbacks.c';assert sha(ROOT/registry)==modal['callback_registry']['observed_before_sha256'];data=(ROOT/registry).read_bytes()
insertions=[(b'#include "replay_mission.h"\n',b'#include "menu_keyboard_modal.h"\n'),(b'    if(address==0x00418510u){FUN_00418510();return;}\n',b'    if(restore_menu_modal_action(address))return;\n')]
for a,b in insertions:assert data.count(a)==1;data=data.replace(a,a+b,1)
outputs[registry]=data
cmake='\n# Complete modal/window-Escape, cursor selector and real resource-query closure.\n'
cmake+='target_sources(pharaoh_restored PRIVATE runtime/menu_keyboard_modal.c runtime/menu_keyboard_modal_platform.c runtime/menu_cursor_platform.c)\n'
cursor=read('artifacts/menu_cursor_phase/metadata/integration-recipe.json')
recipes=[modal['oracle_CMake'],cursor['oracle_CMake'],frozen['shared_target']]
for recipe in recipes:
 target=recipe.get('target',recipe.get('name'));sources=recipe['sources'];includes=recipe['include_directories'];libs=recipe['link_libraries']
 cmake+='add_executable('+target+'\n  '+'\n  '.join(sources)+')\n'
 cmake+='target_include_directories('+target+' PRIVATE '+' '.join(includes)+')\n'
 cmake+='target_compile_definitions('+target+' PRIVATE _CRT_SECURE_NO_WARNINGS)\n'
 if target=='restore_menu_keyboard_modal_verify':cmake+='target_compile_definitions('+target+' PRIVATE MODAL_KEYBOARD_CONTEXT_SOURCE="${CMAKE_CURRENT_SOURCE_DIR}/src/unit_00274.c")\n'
 options='/W4' if target=='restore_training_queries_verify' else '/W4 /WX'
 cmake+='target_compile_options('+target+' PRIVATE '+options+' "$<$<CONFIG:Debug>:/Od>" "$<$<CONFIG:Release>:/O2>")\n'
 cmake+='target_link_libraries('+target+' PRIVATE '+' '.join(libs)+')\n'
 cmake+='target_link_options('+target+' PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)\n'
 test=target.replace('restore_','standalone_').replace('_verify','')
 cmake+='add_test(NAME '+test+' COMMAND '+target+')\nset_tests_properties('+test+' PROPERTIES LABELS source-only)\n'
outputs['CMakeLists.txt']=(ROOT/'CMakeLists.txt').read_bytes()+cmake.encode()
backup=ROOT/'artifacts/integration-batch-130-before';assert not backup.exists()
for p in ['tools/refresh_menu_native_evidence.py','tools/verify_remaining_menu_click_emulation.py','tools/record_dropdown_actions.py','metadata/dropdown-action-status.json','metadata/settings-callback-status.json']:
 saved=backup/p;saved.parent.mkdir(parents=True,exist_ok=True);saved.write_bytes((ROOT/p).read_bytes())
for name,data in outputs.items():
 p=ROOT/name
 if p.exists():
  saved=backup/name;saved.parent.mkdir(parents=True,exist_ok=True);saved.write_bytes(p.read_bytes())
 p.parent.mkdir(parents=True,exist_ok=True);p.write_bytes(data)
out={'format':1,'complete_menu_reconstruction':False,'predecessor':'metadata/integration-batch-129-applied.json','predecessor_sha256':sha(ROOT/'metadata/integration-batch-129-applied.json'),'frozen_inputs':pins,'written':{p:sha(ROOT/p) for p in outputs},'dispatcher_wiring':w,'registry_insertions':[{'anchor':a.decode(),'inserted':b.decode()} for a,b in insertions],'refresh_product_unchanged':{'path':'src/unit_01867.c','sha256':sha(ROOT/'src/unit_01867.c')},'validation':'Applied only; actual shared Debug/Release and full production native replays required.'}
(ROOT/'metadata/integration-batch-130-application.json').write_text(json.dumps(out,indent=2)+'\n')
print(json.dumps({'applied_files':len(outputs),'new_test_targets':3,'requires_validation':True}))
