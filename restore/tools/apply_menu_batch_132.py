"""Guarded application of full40BA40 actor order and linked-actor lookup."""
from pathlib import Path
import hashlib,json
ROOT=Path(__file__).resolve().parents[1]
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
read=lambda p:json.loads((ROOT/p).read_text())
accepted=read('metadata/integration-batch-131-applied.json')
for p,h in accepted['source_and_evidence_hashes'].items():assert sha(ROOT/p)==h,('checkpoint131 drift',p)
phase=ROOT/'artifacts/menu_window_cursor_consumer_phase'
manifest=phase/'metadata/freeze-manifest.json';mapping=phase/'metadata/apply-map.json'
assert sha(manifest)=='17f9085908c29f0f53e0a3e7e757799f8b932a928fe158c2740b8db037e7daed'
assert sha(mapping)=='8b6a77e39d22657c9bcbd22f89bb183478ba21e26930b1dedaa870030aea89b4'
frozen=json.loads(manifest.read_text());m=json.loads(mapping.read_text())
for p,h in frozen['files'].items():assert sha(ROOT/p)==h,('frozen consumer drift',p)
assert sha(ROOT/m['recipe'])==m['recipe_sha256'] and sha(ROOT/m['stage_proof'])==m['stage_proof_sha256']
proof=read(m['stage_proof']);assert proof['passed'] and not proof['production'] and proof['complete_case_set'] and proof['checked']==2635 and proof['comparisons']==5270
assert proof['qualified_controller']['executed']==proof['qualified_controller']['instructions']==238 and proof['qualified_controller']['edges_executed']==32
for p,h in {**proof['source_hashes'],**proof['native_exports']}.items():assert sha(ROOT/p)==h,('private proof drift',p)
outputs={}
for r in m['files']:
 source,dest=ROOT/r['staged'],ROOT/r['target'];assert (sha(dest) if dest.exists() else None)==r['before_sha256'],r['target']
 assert sha(source)==r['candidate_sha256'] and r['target'] not in outputs
 outputs[r['target']]=source.read_bytes()
recipe=read(m['recipe'])['oracle_CMake'];target=recipe['target']
cmake='\n# Whole actor-order controller and real linked actor lookup; no dispatcher edits.\n'
cmake+='add_executable('+target+'\n  '+'\n  '.join(recipe['sources'])+')\n'
cmake+='target_include_directories('+target+' PRIVATE '+' '.join(recipe['include_directories'])+')\n'
cmake+='target_compile_definitions('+target+' PRIVATE\n  '+'\n  '.join(recipe['compile_definitions'])+')\n'
cmake+='target_compile_options('+target+' PRIVATE /W4 /WX "$<$<CONFIG:Debug>:/Od>" "$<$<CONFIG:Release>:/O2>")\n'
cmake+='target_link_libraries('+target+' PRIVATE restore_data)\n'
cmake+='target_link_options('+target+' PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)\n'
cmake+='add_test(NAME standalone_menu_window_cursor_consumer COMMAND '+target+')\nset_tests_properties(standalone_menu_window_cursor_consumer PROPERTIES LABELS source-only)\n'
outputs['CMakeLists.txt']=(ROOT/'CMakeLists.txt').read_bytes()+cmake.encode()
backup=ROOT/'artifacts/integration-batch-132-before';assert not backup.exists()
for p in list(outputs)+['tools/verify_menu_cursor_emulation.py','tools/refresh_menu_native_evidence.py','metadata/menu-cursor-emulation.json','metadata/dropdown-action-status.json','metadata/settings-callback-status.json','README.md']:
 if (ROOT/p).exists():dest=backup/p;dest.parent.mkdir(parents=True,exist_ok=True);dest.write_bytes((ROOT/p).read_bytes())
protected=['src/unit_01866.c','src/unit_01867.c','src/unit_01868.c','runtime/menu_callbacks.c','src/unit_01769.c','src/unit_01768.c','src/unit_01427.c','src/unit_02334.c','src/unit_01731.c']
unchanged={p:sha(ROOT/p) for p in protected}
for p,data in outputs.items():dest=ROOT/p;dest.parent.mkdir(parents=True,exist_ok=True);dest.write_bytes(data)
out={'format':1,'complete_menu_reconstruction':False,'predecessor':'metadata/integration-batch-131-applied.json','predecessor_sha256':sha(ROOT/'metadata/integration-batch-131-applied.json'),'frozen_inputs':{manifest.relative_to(ROOT).as_posix():sha(manifest),mapping.relative_to(ROOT).as_posix():sha(mapping)},'written':{p:sha(ROOT/p) for p in outputs},'protected_unchanged':unchanged,'cursor_gap_snapshot_supersession':read(m['recipe'])['proof_supersession'],'validation':'Applied only; full actual shared Debug/O2 replay and explicit cursor gap-note retirement required.'}
(ROOT/'metadata/integration-batch-132-application.json').write_text(json.dumps(out,indent=2)+'\n')
print(json.dumps({'written_files':len(outputs),'new_CTest_targets':1,'validation_required':True}))
