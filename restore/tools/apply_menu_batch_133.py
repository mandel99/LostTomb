"""Apply frozen complete text and actor distance producer with exact source guards."""
from pathlib import Path
import hashlib,json
ROOT=Path(__file__).resolve().parents[1]
sha=lambda p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest()
read=lambda p:json.loads((ROOT/p).read_text())
accepted=read('metadata/integration-batch-132-applied.json')
for p,h in accepted['source_and_evidence_hashes'].items():assert sha(p)==h,('checkpoint132 drift',p)
pins={'artifacts/menu_text_glyph_phase/freeze-manifest.json':'89f505c7c42416485a368859662455b50af7c219ce9992bd869d986fb1509544','artifacts/menu_text_glyph_phase/apply-map.json':'6657f35afebe1f6bc0bf5e8fcdee204b566b752a58bff4c86b29063064812d05','artifacts/actor_order_flood_phase/metadata/freeze-manifest.json':'0a522b77312cc85b76677a8dc2ec704089cf622ceba52a8d6c6c3a48faab0f46','artifacts/actor_order_flood_phase/metadata/apply-map.json':'3f65fb812f94643bc354537e7abceba6b4d74b053f610904a66737529d7829f5'}
for p,h in pins.items():assert sha(p)==h,p
for path in ['artifacts/menu_text_glyph_phase/freeze-manifest.json','artifacts/actor_order_flood_phase/metadata/freeze-manifest.json']:
 for p,h in read(path)['files'].items():assert sha(p)==h,('phase drift',p)
text_proof=read('artifacts/menu_text_glyph_phase/private-emulation.json');flood_proof=read('artifacts/actor_order_flood_phase/metadata/actor-order-flood-emulation.json')
for proof,count in [(text_proof,3062),(flood_proof,174)]:
 assert proof['passed'] and not proof['production'] and proof['complete_case_set'] and proof['checked']==count and proof['comparisons']==count*2
 for p,h in {**proof['source_hashes'],**proof.get('native_exports',{}),**proof.get('binary_hashes',{})}.items():assert sha(p)==h,p
assert text_proof['native_instruction_coverage']==[367,367] and text_proof['conditional_outcomes']==[78,78]
assert flood_proof['qualified_controller']['executed']==185 and flood_proof['qualified_controller']['edges_executed']==62 and flood_proof['controlled_deep_boundaries']==[]
records=read('artifacts/menu_text_glyph_phase/apply-map.json')['files']+read('artifacts/actor_order_flood_phase/metadata/apply-map.json')['files'];outputs={}
for r in records:
 p=ROOT/r['target'];assert (sha(r['target']) if p.exists() else None)==r['before_sha256'],r['target']
 assert sha(r['staged'])==r['candidate_sha256'] and r['target'] not in outputs;outputs[r['target']]=(ROOT/r['staged']).read_bytes()
cmake='''
# Complete five-function text algorithms and actual actor-order flood producer.
add_executable(restore_menu_text_glyph_verify
  artifacts/menu_text_glyph_phase/tests/text_glyph.c
  src/unit_01667.c src/unit_01691.c src/unit_01692.c src/unit_01693.c src/unit_01694.c)
target_include_directories(restore_menu_text_glyph_verify PRIVATE include artifacts/menu_text_glyph_phase/tests)
target_compile_definitions(restore_menu_text_glyph_verify PRIVATE _CRT_SECURE_NO_WARNINGS)
target_compile_options(restore_menu_text_glyph_verify PRIVATE /W4 /WX "$<$<CONFIG:Debug>:/Od>" "$<$<CONFIG:Release>:/O2>")
target_link_libraries(restore_menu_text_glyph_verify PRIVATE restore_data advapi32)
target_link_options(restore_menu_text_glyph_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
add_test(NAME standalone_menu_text_glyph COMMAND restore_menu_text_glyph_verify)
set_tests_properties(standalone_menu_text_glyph PROPERTIES LABELS source-only)
'''
recipe=read('artifacts/actor_order_flood_phase/metadata/integration-recipe.json')['oracle_CMake'];target=recipe['target']
cmake+='add_executable('+target+'\n  '+'\n  '.join(recipe['sources'])+')\n'
cmake+='target_include_directories('+target+' PRIVATE '+' '.join(recipe['include_directories'])+')\n'
cmake+='target_compile_definitions('+target+' PRIVATE\n  '+'\n  '.join(recipe['compile_definitions'])+')\n'
cmake+='target_compile_options('+target+' PRIVATE /W4 /WX "$<$<CONFIG:Debug>:/Od>" "$<$<CONFIG:Release>:/O2>")\n'
cmake+='target_link_libraries('+target+' PRIVATE restore_data)\n'
cmake+='target_link_options('+target+' PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)\n'
cmake+='add_test(NAME standalone_actor_order_flood COMMAND '+target+')\nset_tests_properties(standalone_actor_order_flood PROPERTIES LABELS source-only)\n'
outputs['CMakeLists.txt']=(ROOT/'CMakeLists.txt').read_bytes()+cmake.encode()
backup=ROOT/'artifacts/integration-batch-133-before';assert not backup.exists()
for p in list(outputs)+['tools/refresh_menu_native_evidence.py','tools/repair_discarded_returns.py','README.md','metadata/dropdown-action-status.json','metadata/settings-callback-status.json']:
 if (ROOT/p).exists():saved=backup/p;saved.parent.mkdir(parents=True,exist_ok=True);saved.write_bytes((ROOT/p).read_bytes())
unchanged={p:sha(p) for p in ['src/unit_01866.c','src/unit_01867.c','src/unit_01868.c','runtime/menu_callbacks.c','src/unit_00085.c','src/unit_01455.c','src/unit_00119.c']}
for p,data in outputs.items():dest=ROOT/p;dest.parent.mkdir(parents=True,exist_ok=True);dest.write_bytes(data)
out={'format':1,'complete_menu_reconstruction':False,'predecessor':'metadata/integration-batch-132-applied.json','predecessor_sha256':sha('metadata/integration-batch-132-applied.json'),'frozen_inputs':pins,'written':{p:sha(p) for p in outputs},'protected_unchanged':unchanged,'validation':'Applied only; complete shared Debug/Release tests and full production native comparisons are still required.'}
(ROOT/'metadata/integration-batch-133-application.json').write_text(json.dumps(out,indent=2)+'\n')
print(json.dumps({'applied_files':len(outputs),'new_test_targets':2,'requires_production_validation':True}))
