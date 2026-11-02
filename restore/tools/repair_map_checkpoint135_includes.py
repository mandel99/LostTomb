"""Add the missing shared P2 test-data include to both actual P3 test targets."""
from pathlib import Path
from datetime import datetime,timezone
import hashlib,json
ROOT=Path(__file__).resolve().parents[1]
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
receipt_path=ROOT/'artifacts/map_refresh_application_phase/application-receipt.json'
receipt=json.loads(receipt_path.read_text());cmake=ROOT/'CMakeLists.txt'
expected='92d720f804c80aad8b42909197fe35df97f1e1554ebb820dae9131e476d632e1'
assert sha(cmake)==expected and receipt['product_written'] and receipt['applied_not_verified']
change=next(c for c in receipt['changes'] if c['path']=='CMakeLists.txt');assert change['after_sha256']==expected
header=ROOT/'artifacts/map_refresh_postpass2_phase/selector_native_data.h';assert header.is_file()
for cfg in ['debug','release']:
 log=(ROOT/f'artifacts/menu-135-{cfg}-build.log').read_text(errors='replace')
 assert 'error C1083' in log and 'selector_native_data.h' in log
addition=b'''\n# Checkpoint135 build correction: P3 fixtures share P2's authenticated record data.\nforeach(_map135_target IN ITEMS restore_map_refresh_postpass3_verify restore_map_refresh_postpass3_plain_verify)\n target_include_directories(${_map135_target} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/artifacts/map_refresh_postpass2_phase")\nendforeach()\nunset(_map135_target)\n'''
old=cmake.read_bytes();new=old+addition;after=hashlib.sha256(new).hexdigest()
cmake.write_bytes(new)
adjustment={'format':1,'generated_at':datetime.now(timezone.utc).isoformat(),
 'scope':'Build plumbing only: add one existing frozen data-header directory to two verification targets. No algorithm, source mapping, frozen recipe or test corpus changes.',
 'path':'CMakeLists.txt','before_sha256':expected,'after_sha256':after,'appended_text':addition.decode(),
 'header':{'path':header.relative_to(ROOT).as_posix(),'sha256':sha(header)},
 'script':{'path':Path(__file__).relative_to(ROOT).as_posix(),'sha256':sha(Path(__file__))},
 'builds_and_tests_must_be_rerun':True}
adjustment_path=ROOT/'metadata/integration-batch-135-build-adjustments.json'
adjustment_path.write_text(json.dumps(adjustment,indent=2)+'\n')
change['initial_application_after_sha256']=expected;change['after_sha256']=after
change['operation']='append exact aggregate recipe once, then add the qualified P3 test-data include directory'
receipt['post_apply_adjustments']=[{'path':adjustment_path.relative_to(ROOT).as_posix(),'sha256':sha(adjustment_path)}]
receipt['applied_not_verified']=True;receipt_path.write_text(json.dumps(receipt,indent=2)+'\n')
print(json.dumps({'passed':sha(cmake)==after,'CMake_after_sha256':after,'algorithm_changes':0,'affected_verification_targets':2,'accepted_checkpoint135_published':False}))
