"""Apply complete native-verified LOS algorithms and immutable typed math dependency."""
from pathlib import Path
import hashlib,json
ROOT=Path(__file__).resolve().parents[1]
sha=lambda p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest()
read=lambda p:json.loads((ROOT/p).read_text())
accepted=read('metadata/integration-batch-133-applied.json')
for p,h in accepted['source_and_evidence_hashes'].items():assert sha(p)==h,('checkpoint133 drift',p)
phase='artifacts/actor_order_los_phase/'
pins={phase+'metadata/apply-map.json':'c31f4db5c36772b010d4262f7e114f0dd49332a17dea185ff024247085d79ba2',phase+'metadata/freeze-manifest.json':'3002556fc2ec9d34e9ce634dfdea5e84f3e21a1bfc7813acd13f66623ed613e4','artifacts/menu_full_input_phase/numeric-freeze.json':'b0921c1a86d6468c0bce576574d2be365ecf8ec8c862846f24f98213b2689cd1'}
for p,h in pins.items():assert sha(p)==h,p
for manifest in [phase+'metadata/freeze-manifest.json','artifacts/menu_full_input_phase/numeric-freeze.json']:
 for p,h in read(manifest)['files'].items():assert sha(p)==h,('frozen dependency drift',p)
mapping=read(phase+'metadata/apply-map.json');assert sha(mapping['recipe'])==mapping['recipe_sha256'] and sha(mapping['stage_proof'])==mapping['stage_proof_sha256']
proof=read(mapping['stage_proof']);assert proof['passed'] and not proof['production'] and proof['complete_case_set'] and proof['checked']==7480 and proof['comparisons']==14960
q=proof['qualified_controller'];assert q['complete'] and q['native_instructions']==q['executed']==538 and q['feasible_edges']==q['edges_executed']==146 and proof['complete_actor_order_gameplay_claimed'] is False
assert set(proof['actual_dependency_coverage'])=={'0040f8f0','0051ce90','0056133c'}
for p,h in {**proof['source_hashes'],**proof['native_exports']}.items():assert sha(p)==h,('staged proof drift',p)
outputs={}
for r in mapping['files']+mapping['shared_numeric_files']:
 assert (sha(r['target']) if (ROOT/r['target']).exists() else None)==r['before_sha256'],r['target']
 assert sha(r['staged'])==r['candidate_sha256'] and r['target'] not in outputs;outputs[r['target']]=(ROOT/r['staged']).read_bytes()
recipe=read(mapping['recipe']);oracle=recipe['oracle_CMake'];target=oracle['target']
cmake='\n# Actual LOS algorithms retain x87 extended slopes; no implicit-ST0/global2775 replacement.\n'
cmake+='target_sources(pharaoh_restored PRIVATE '+' '.join(recipe['main_CMake']['new_runtime_sources'])+')\n'
cmake+='add_executable('+target+'\n  '+'\n  '.join(oracle['sources'])+')\n'
cmake+='target_include_directories('+target+' PRIVATE '+' '.join(oracle['include_directories'])+')\n'
cmake+='target_compile_definitions('+target+' PRIVATE\n  '+'\n  '.join(oracle['compile_definitions'])+')\n'
cmake+='target_compile_options('+target+' PRIVATE /W4 /WX "$<$<CONFIG:Debug>:/Od>" "$<$<CONFIG:Release>:/O2>")\n'
cmake+='target_link_libraries('+target+' PRIVATE restore_data)\n'
cmake+='target_link_options('+target+' PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)\n'
cmake+='add_test(NAME standalone_actor_order_los COMMAND '+target+')\nset_tests_properties(standalone_actor_order_los PROPERTIES LABELS source-only)\n'
outputs['CMakeLists.txt']=(ROOT/'CMakeLists.txt').read_bytes()+cmake.encode()
backup=ROOT/'artifacts/integration-batch-134-before';assert not backup.exists()
for p in list(outputs)+['tools/refresh_menu_native_evidence.py','tools/repair_discarded_returns.py','README.md','metadata/dropdown-action-status.json','metadata/settings-callback-status.json']:
 if (ROOT/p).exists():saved=backup/p;saved.parent.mkdir(parents=True,exist_ok=True);saved.write_bytes((ROOT/p).read_bytes())
unchanged={p:sha(p) for p in ['src/unit_01866.c','src/unit_01867.c','src/unit_01868.c','runtime/menu_callbacks.c','src/unit_00085.c','src/unit_01455.c','src/unit_02335.c','src/unit_00119.c','src/unit_02775.c']}
for p,b in outputs.items():dest=ROOT/p;dest.parent.mkdir(parents=True,exist_ok=True);dest.write_bytes(b)
out={'format':1,'complete_menu_reconstruction':False,'predecessor':'metadata/integration-batch-133-applied.json','predecessor_sha256':sha('metadata/integration-batch-133-applied.json'),'frozen_inputs':pins,'written':{p:sha(p) for p in outputs},'protected_unchanged':unchanged,'scope':'Whole40BDA0/40C180 with actual flood/fill/raw80 conversion, no controlled game/OS endpoints. Only immutable numeric dependency copied; no route builder or whole input dispatcher application.','validation':'Applied only; full shared build/tests and actual production native replay required.'}
(ROOT/'metadata/integration-batch-134-application.json').write_text(json.dumps(out,indent=2)+'\n')
print(json.dumps({'applied_files':len(outputs),'new_CTest_targets':1,'production_validation_required':True}))
