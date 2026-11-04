"""Verify current source/binary bindings for the finite labor/access recovery.

This checks retained oracle reports and lightweight source audits; it does not
rerun or replace the differential tests.
Run the twenty-nine differential suites for both executables before refreshing it.
"""
from pathlib import Path
from datetime import datetime,timezone
import hashlib,json,subprocess,sys

ROOT=Path(__file__).resolve().parents[1]
def digest(path):return hashlib.sha256(path.read_bytes()).hexdigest()

_cmake_scope=None
def cmake_scope():
 global _cmake_scope
 if _cmake_scope is None:
  j=json.loads((ROOT/'metadata/music-cmake-scope-audit.json').read_text())
  assert j['passed'] and j['sole_changed_target']=='restore_music_tick_verify' and j['incremental_build_exit']==0
  assert digest(ROOT/'CMakeLists.txt')==j['current_CMake_sha256']
  for p,sha in j['source_hashes'].items():assert digest(ROOT/p)==sha,('CMake scope audit',p)
  for item in j['unchanged_production_artifacts'].values():
   p=Path(item['compiled_C_path']);assert digest(p)==item['binary_sha256'] and digest(p.with_suffix('.map'))==item['map_sha256']
  _cmake_scope=j
 return _cmake_scope

def source_matches(source,sha,allow_scope=True):
 current=digest(ROOT/source)
 if current==sha:return True
 if not allow_scope or source not in ['CMakeLists.txt','metadata/audio-global-startup-audit.json']:return False
 j=cmake_scope()
 if source=='CMakeLists.txt':return current==j['current_CMake_sha256'] and sha in j['prior_CMake_sha256']
 audit=j['compatible_startup_audit']
 return current==audit['current_sha256'] and sha in audit['prior_sha256'] and audit['only_changed_field']=='source_hashes.CMakeLists.txt'

def main():
 for audit in ['audit_building_access_returns.py','audit_processing_receiver.py']:
  subprocess.run([sys.executable,str(ROOT/'direct_c'/audit)],check=True)
 proofs={};files={}
 for suite in ['labor-allocation','building-access','building-access-search','building-removal','building-destruction','map-surface','access-repair-flood','map-region','building-occupancy','ring-growth','building-processing','message-category','actor-message','actor-voice','sound-stream','sound-fade','sound-ambient','sound-sample','byte-string-compare','sound-file-lifecycle','sound-bank','sound-playback','sound-stop','sound-allocation','sound-device','music-configuration','actor-reserved','actor-scenario-spawn','actor-command-transition']:
  for suffix in ['diagnostic','game']:
   rel=f'metadata/{suite}-{suffix}.json';p=ROOT/rel;j=json.loads(p.read_text())
   assert j['passed'] and not j['unexecuted_instructions'] and not j['unexecuted_edges'],rel
   assert digest(ROOT/'build/smoke-game/Pharaoh.exe')==j['native_sha256'],rel
   assert digest(Path(j['compiled_C_path']))==j['compiled_C_sha256'],(rel,'stale executable')
   for source,sha in j['source_hashes'].items():assert source_matches(source,sha),(rel,'stale source',source)
   if suite=='actor-reserved':
    assert set(j['coverage'])=={'0x4bb090','0x4b4760','0x4b8c90','0x4b4800','0x40f8f0','0x448a60','0x44bb90'}
    assert j['actual_scenarios']>0 and j['protocol_scenarios']>0 and j['synthetic_memory_scenarios']>0
    assert j['controlled_algorithm_endpoints_by_lane']['actual']==[] and j['controlled_platform_endpoints']==[]
   if suite=='actor-scenario-spawn':
    assert set(j['coverage'])=={'0x4bb3f0','0x4bbc10','0x4b6ff0','0x4ec410'}
    assert j['actual_scenarios']==405 and j['protocol_scenarios']==137 and j['synthetic_memory_scenarios']==5
    assert j['conditional_edges']==126 and j['controlled_platform_endpoints']==[]
   if suite=='actor-command-transition':
    assert set(j['coverage'])=={'0x4ec990','0x4ec7c0','0x4ec4a0','0x4ec0f0','0x4eae20'}
    assert j['checked']==1353 and j['invocations']==3976 and j['conditional_edges']==138
    assert j['actual_scenarios']==1256 and j['protocol_scenarios']==95 and j['synthetic_memory_scenarios']==2
    assert j['controlled_algorithm_endpoints_by_lane']['actual']==[] and j['controlled_platform_endpoints']==[]
    assert j['analytically_unreachable_edges']==[['0x4ec137','0x4ec193'],['0x4ec1a9','0x4ec20a'],['0x4ec225','0x4ec281'],['0x4ec297','0x4ec2f4'],['0x4ec5eb','0x4ec5ed'],['0x4ec947','0x4ec949']]
   coverage=j['coverage']
   qualified=sum(c.get('analytically_unreachable',0) for c in coverage.values())
   if qualified:
    if suite=='actor-command-transition':
     assert qualified==3 and set(j['analytically_unreachable_instructions'])=={'0x4ec5ed','0x4ec5ee','0x4ec5f1'},rel
    else:
     assert suite=='actor-message' and qualified==4,rel
     assert set(j['analytically_unreachable_instructions'])=={'0x53cbb5','0x53cbba','0x53cca9','0x53ccae'},rel
   disabled=sum(c.get('user_disabled_CD',0) for c in coverage.values())
   if disabled:
    assert suite=='music-configuration' and disabled==10 and j['explicit_CD_policy_scenarios']==3,rel
    assert set(j['user_disabled_CD_instructions'])=={'0x539470','0x539472','0x539473','0x539479','0x53947d','0x53947e','0x539483','0x539484','0x539489','0x53948c'},rel
    assert j['user_disabled_CD_edges']==[['0x53946e','0x539470']]
    assert j['analytically_unreachable_edges']==[['0x53930e','0x539324'],['0x539388','0x53939e'],['0x53939c','0x53937e']]
   assert all(c['instructions']==c['executed']+c.get('analytically_unreachable',0)+c.get('user_disabled_CD',0) for c in coverage.values()),rel
   proofs[rel]=dict(scenarios=j['checked'],invocations=j['invocations'],binary_sha256=j['compiled_C_sha256'],native_instructions=sum(c['instructions'] for c in coverage.values()),native_instructions_executed=sum(c['executed'] for c in coverage.values()),analytically_unreachable_instructions=qualified,user_disabled_CD_instructions=disabled,explicit_CD_policy_scenarios=j.get('explicit_CD_policy_scenarios',0))
   files[rel]=digest(p)
 settings=json.loads((ROOT/'metadata/settings-callbacks-emulation.json').read_text())
 assert settings['passed'] and settings['case_selection']==dict(limit=None,start=0,single_case=False)
 assert not settings['native_uncovered_instructions'] and not settings['missing_conditional_outcomes']
 assert settings['native_instruction_coverage'][0]==settings['native_instruction_coverage'][1]
 assert settings['owned_instruction_coverage'][0]==settings['owned_instruction_coverage'][1]
 assert digest(ROOT/'build/smoke-game/Pharaoh.exe')==settings['native_sha256']
 assert settings['comparison_checked']==settings['checked']==settings['plain_checked']
 assert digest(ROOT/'build/c-only/Debug/restore_settings_callbacks_verify.exe')==settings['compiled_C_sha256']
 assert digest(ROOT/'build/c-only/Release/restore_settings_callbacks_verify.exe')==settings['comparison_C_sha256']
 for source,sha in settings['source_hashes'].items():assert source_matches(source,sha),('settings',source)
 ground=json.loads((ROOT/'metadata/ground-render-emulation.json').read_text())
 assert ground['passed'] and ground['case_selection']==dict(limit=None,start=0,single_case=False)
 assert not ground['native_uncovered_instructions']
 for key in ['native_instruction_coverage','helper_instruction_coverage']:
  assert ground[key][0]==ground[key][1],('ground',key)
 # The complete sprite dispatcher includes explicitly excluded file loading;
 # only its reached inline-image paths belong to the ground composition.
 assert ground['sprite_dispatch_instruction_coverage']==[246,1076]
 assert digest(ROOT/'build/smoke-game/Pharaoh.exe')==ground['native_sha256']
 assert digest(ROOT/'build/c-only/Release/restore_ground_render_verify.exe')==ground['compiled_C_sha256']
 assert 'src/unit_02493.c' in ground['source_hashes'] and 'runtime/sound_ambient_profile.c' in ground['source_hashes']
 for source,sha in ground['source_hashes'].items():assert source_matches(source,sha),('ground',source)
 sound_tables={}
 for suffix,binary in [('debug','build/c-only/Debug/restore_sound_files_verify.exe'),('release','build/c-only/Release/restore_sound_files_verify.exe')]:
  rel='metadata/sound-file-lookup-emulation'+('-release' if suffix=='release' else '')+'.json'
  table=json.loads((ROOT/rel).read_text())
  assert table['passed'] and table['checked']==5600,(suffix,'full legacy table corpus')
  assert digest(ROOT/'build/smoke-game/Pharaoh.exe')==table['original_sha256']
  assert Path(table['compiled_C_path']).resolve()==(ROOT/binary).resolve()
  assert digest(ROOT/binary)==table['compiled_C_sha256'],suffix
  for source,sha in table['source_hashes'].items():assert source_matches(source,sha),('sound table',suffix,source)
  sound_tables[suffix]=table['checked']
 sound_initialization={}
 for suffix,config in [('debug','Debug'),('release','Release')]:
  rel='metadata/sound-initialization-emulation'+('-release' if suffix=='release' else '')+'.json'
  report=json.loads((ROOT/rel).read_text());binary=ROOT/f'build/c-only/{config}/restore_sound_verify.exe'
  assert report['passed'] and report['checked']==391,(suffix,'full expanded sound initialization corpus')
  assert digest(ROOT/'build/smoke-game/Pharaoh.exe')==report['original_sha256']
  assert Path(report['compiled_C_path']).resolve()==binary.resolve() and digest(binary)==report['compiled_C_sha256']
  for source,sha in report['source_hashes'].items():assert source_matches(source,sha),('sound initialization',suffix,source)
  sound_initialization[suffix]=report['checked']
 host=json.loads((ROOT/'metadata/array-lifecycle-host.json').read_text())
 assert host['passed'] and host['native_OS_exception_dispatch_proven'] is False and len(host['contracts'])==6
 for config,item in host['variants'].items():
  binary=ROOT/f'build/c-only/{config}/restore_array_lifecycle_verify.exe'
  assert item['normal_exit']==0 and item['controlled_terminate_exit']==77 and item['contracts']==6
  assert Path(item['compiled_C_path']).resolve()==binary.resolve() and digest(binary)==item['compiled_C_sha256']
 for source,sha in host['source_hashes'].items():assert source_matches(source,sha),('array host',source)
 playback_host=json.loads((ROOT/'metadata/sound-playback-host.json').read_text())
 assert playback_host['passed'] and set(playback_host['variants'])=={'Debug','Release'}
 for config,item in playback_host['variants'].items():
  binary=ROOT/f'build/c-only/{config}/restore_sound_samples_verify.exe'
  assert item['exit_code']==0 and item['checks']==160
  assert Path(item['compiled_C_path']).resolve()==binary.resolve() and digest(binary)==item['compiled_C_sha256']
 for source,sha in playback_host['source_hashes'].items():assert source_matches(source,sha),('playback host',source)
 allocation_host=json.loads((ROOT/'metadata/sound-allocation-host.json').read_text())
 assert allocation_host['passed'] and allocation_host['expectations_derived_from_native'] and set(allocation_host['variants'])=={'Debug','Release'}
 for config,item in allocation_host['variants'].items():
  binary=ROOT/f'build/c-only/{config}/restore_sound_verify.exe'
  assert item['exit_code']==0 and item['checks']==265 and item['scenarios']==18
  assert Path(item['compiled_C_path']).resolve()==binary.resolve() and digest(binary)==item['compiled_C_sha256']
 for source,sha in allocation_host['source_hashes'].items():assert source_matches(source,sha),('allocation host',source)
 for suffix in ['diagnostic','game']:
  regression=json.loads((ROOT/f'metadata/sound-allocation-regressions-{suffix}.json').read_text())
  assert regression['passed'] and regression['replayed_counterexamples']==8 and len(regression['cases'])==8
  assert digest(Path(regression['compiled_C_path']))==regression['compiled_C_sha256']
  assert digest(ROOT/'build/smoke-game/Pharaoh.exe')==regression['native_sha256']
  assert all(case['original']==case['current_C'] for case in regression['cases'])
  for source,sha in regression['source_hashes'].items():assert source_matches(source,sha),('allocation regression',suffix,source)
 stop_before=json.loads((ROOT/'metadata/sound-stop-before-counterexample.json').read_text())
 assert stop_before['behavioral_mismatch_proven'] and digest(Path(stop_before['old_C_path']))==stop_before['old_C_sha256']
 assert digest(ROOT/'build/smoke-game/Pharaoh.exe')==stop_before['native_sha256']
 assert stop_before['original_counts']=={'0x56f2cc':1,'0x56f2c8':0,'0x56f2c4':0}
 assert stop_before['old_C_counts']=={'0x56f2cc':3,'0x56f2c8':3,'0x56f2c4':0}
 allocation_before=json.loads((ROOT/'metadata/sound-allocation-before-counterexamples.json').read_text())
 assert allocation_before['complete_reconstruction'] is False and allocation_before['matching_controls']==3 and allocation_before['concrete_counterexamples']==8
 assert digest(Path(allocation_before['old_C_path']))==allocation_before['old_C_sha256']
 assert digest(ROOT/'build/smoke-game/Pharaoh.exe')==allocation_before['native_sha256']
 assert sum(case['mismatch'] for case in allocation_before['cases'])==8
 for source,sha in allocation_before['source_hashes'].items():assert source_matches(source,sha),('allocation counterexample',source)
 startup=json.loads((ROOT/'metadata/audio-global-startup-audit.json').read_text())
 assert startup['passed'] and startup['owned_entries']==13 and startup['owned_native_instructions']==474
 assert startup['normal_exit_cleanup_order']==['music','sound','arena']
 assert digest(ROOT/'build/smoke-game/Pharaoh.exe')==startup['native_sha256']
 for item in startup['variants'].values():assert digest(Path(item['compiled_C_path']))==item['compiled_C_sha256']
 for source,sha in startup['source_hashes'].items():assert source_matches(source,sha),('audio startup',source)
 device_before=json.loads((ROOT/'metadata/sound-device-before-counterexamples.json').read_text())
 assert device_before['complete_reconstruction'] is False and device_before['concrete_counterexamples']==2
 assert digest(Path(device_before['old_C_path']))==device_before['old_C_sha256']
 assert digest(ROOT/'build/smoke-game/Pharaoh.exe')==device_before['native_sha256']
 assert device_before['alias_fault']['original'][1:]==[3,4] and device_before['alias_fault']['previous_C'][1:]==[11,4]
 original=device_before['pre_version_state']['original'];previous=device_before['pre_version_state']['previous_C']
 assert original['handles'] and original['cache'] and previous['handles']==previous['cache']==0
 for source,sha in device_before['source_hashes'].items():assert source_matches(source,sha),('device counterexamples',source)
 music_next=json.loads((ROOT/'metadata/music-configuration-next-counterexamples.json').read_text())
 assert music_next['complete_reconstruction'] is False and music_next['concrete_counterexamples']==4
 assert digest(Path(music_next['compiled_C_path']))==music_next['compiled_C_sha256']
 assert digest(ROOT/'build/smoke-game/Pharaoh.exe')==music_next['native_sha256']
 for case in music_next['cases']:
  assert case['original']==dict(result=1,count=case['old_count']+1)
  assert case['previous_C']==dict(result=0xffffffff,count=case['old_count'])
 for source,sha in music_next['source_hashes'].items():assert source_matches(source,sha),('unresolved music counterexamples',source)
 for suffix in ['diagnostic','game']:
  regression=json.loads((ROOT/f'metadata/music-configuration-regressions-{suffix}.json').read_text())
  assert regression['passed'] and regression['replayed_counterexamples']==4 and len(regression['cases'])==4
  assert digest(Path(regression['compiled_C_path']))==regression['compiled_C_sha256']
  assert digest(ROOT/'build/smoke-game/Pharaoh.exe')==regression['native_sha256']
  assert all(case['original']==case['current_C'] for case in regression['cases'])
  for source,sha in regression['source_hashes'].items():assert source_matches(source,sha),('music regression',suffix,source)
 music_host=json.loads((ROOT/'metadata/music-configuration-host.json').read_text())
 assert music_host['passed'] and set(music_host['variants'])=={'Debug','Release'}
 for config,variant in music_host['variants'].items():
  for family,item in variant.items():
   assert digest(Path(item['compiled_C_path']))==item['compiled_C_sha256'] and item['exit_code']==0
   assert item['native_cases']=={'music-manager':316,'campaign-parser':333,'music-tick':1668}[family]
 for path,sha in music_host['native_input_reports'].items():assert digest(ROOT/path)==sha,('music host native input',path)
 for source,sha in music_host['source_hashes'].items():assert source_matches(source,sha),('music host',source)
 cmake_scope()
 extra=['metadata/SOUND_FADE_RECONSTRUCTION.md','metadata/sound-fade-native.json','metadata/sound-fade-rea-evidence.json','metadata/settings-callbacks-emulation.json','build/build-sound-fades-final.log','build/build-settings-fade-debug.log','build/verify-settings-sound-publication.log','metadata/SOUND_STREAM_RECONSTRUCTION.md','metadata/sound-stream-native.json','metadata/sound-stream-rea-evidence.json','build/build-sound-streams.log','metadata/sound-fade-next-xrefs.json','metadata/sound-fade-next-native.json','metadata/sound-fade-next-rea-evidence.json','metadata/ACTOR_VOICE_RECONSTRUCTION.md','metadata/actor-voice-native.json','metadata/actor-voice-rea-evidence.json','metadata/actor-voice-callers-native.json','metadata/actor-voice-overflow-native.json','build/actor-voice-bank56-native-fault.log','build/build-actor-voice.log','metadata/sound-stream-next-native.json','metadata/sound-stream-next-rea-evidence.json','metadata/actor-voice-next-native.json','metadata/actor-voice-next-rea-evidence.json','metadata/ACTOR_MESSAGE_RECONSTRUCTION.md','metadata/actor-message-native.json','metadata/actor-message-rea-evidence.json','metadata/actor-message-decompile-reference.json','build/build-actor-message-final.log','metadata/actor-message-controller-next-native.json','metadata/actor-message-controller-next-rea-evidence.json','metadata/MESSAGE_CATEGORY_RECONSTRUCTION.md','metadata/message-category-native.json','metadata/message-category-rea-evidence.json','build/build-message-category-final.log','metadata/building-access-unused-return-audit.json','metadata/building-access-search-names.json','metadata/LABOR_AND_ACCESS_RECONSTRUCTION.md','metadata/BUILDING_REMOVAL_RECONSTRUCTION.md','metadata/BUILDING_DESTRUCTION_RECONSTRUCTION.md','metadata/building-removal-caller-bytes.json','metadata/building-removal-caller-windows.json','metadata/building-destruction-next-native.json','metadata/building-destruction-native.json','metadata/building-destruction-rea-evidence.json','metadata/MAP_ACCESS_REPAIR_RECONSTRUCTION.md','metadata/map-surface-native.json','metadata/map-surface-rea-evidence.json','metadata/access-repair-flood-native.json','metadata/access-repair-flood-rea-evidence.json','metadata/MAP_REGION_RECONSTRUCTION.md','metadata/map-region-native.json','metadata/map-region-rea-evidence.json','metadata/BUILDING_OCCUPANCY_AND_RING_RECONSTRUCTION.md','metadata/building-occupancy-native.json','metadata/building-occupancy-rea-evidence.json','metadata/ring-growth-native.json','metadata/ring-growth-rea-evidence.json','metadata/BUILDING_PROCESSING_QUERY_RECONSTRUCTION.md','metadata/building-processing-native.json','metadata/building-processing-rea-evidence.json','metadata/processing-receiver-native.json','direct_c/audit_processing_receiver.py','tools/repair_member_receivers.py','tools/prepare_c.py','src/unit_02503.c','metadata/timed-record-next-native.json','metadata/timed-record-next-rea-evidence.json','metadata/actor-message-query-next-native.json','metadata/actor-message-query-next-rea-evidence.json','metadata/ring-growth-next-native.json','metadata/ring-growth-next-rea-evidence.json','metadata/building-query-next-native.json','metadata/building-query-next-rea-evidence.json','metadata/map-region-next-native.json','metadata/map-region-next-rea-evidence.json','metadata/access-surface-next-native.json','metadata/access-surface-next-rea-evidence.json','direct_c/audit_building_access_returns.py','direct_c/verify_labor_access_current.py','tools/repair_discarded_returns.py','src/unit_02216.c','build/build-processing-query-final.log','build/building-access-caller-syntax.log','build/reconstruction-count.json','README.md']
 extra+=['metadata/sound-ambient-next-native.json','metadata/sound-ambient-next-rea-evidence.json','metadata/SOUND_AMBIENT_RECONSTRUCTION.md','metadata/sound-ambient-native.json','metadata/sound-ambient-rea-evidence.json','metadata/ground-render-emulation.json','build/build-sound-ambient-final.log','build/build-ground-ambient-release.log','build/verify-ground-ambient.log','metadata/sound-sample-next-native.json','metadata/sound-sample-next-rea-evidence.json']
 extra+=['metadata/SOUND_SAMPLE_RECONSTRUCTION.md','metadata/sound-sample-native.json','metadata/sound-sample-rea-evidence.json','metadata/sound-file-lookup-emulation.json','metadata/sound-file-lookup-emulation-release.json','build/build-sound-samples-byte-final.log','build/build-sound-file-byte-debug-final.log','build/build-sound-file-debug-final.log','build/verify-sound-file-byte-debug.log','metadata/sound-sample-caller-audit.json','metadata/sound-sample-callers-rea-evidence.json','build/verify-sound-file-byte-release.log','metadata/sound-file-lifecycle-next-native.json','metadata/sound-file-lifecycle-next-rea-evidence.json']
 extra+=['metadata/byte-string-compare-native.json','include/byte_string_compare.h']
 extra+=['metadata/SOUND_FILE_LIFECYCLE_RECONSTRUCTION.md','metadata/sound-file-lifecycle-native.json','metadata/sound-file-lifecycle-rea-evidence.json','metadata/array-lifecycle-host.json','metadata/sound-initialization-emulation.json','metadata/sound-initialization-emulation-release.json','build/build-sound-lifecycle-final.log','build/build-sound-lifecycle-debug-final.log','build/verify-sound-initialization-lifecycle-debug.log','build/verify-sound-initialization-lifecycle-release.log','metadata/sound-bank-next-native.json','metadata/sound-bank-next-rea-evidence.json']
 extra+=['metadata/SOUND_BANK_RECONSTRUCTION.md','metadata/sound-bank-native.json','metadata/sound-bank-rea-evidence.json','metadata/sound-bank-load-order.json','metadata/sound-playback-next-native.json','metadata/sound-playback-next-rea-evidence.json','build/build-sound-banks-final.log','build/verify-sound-banks-quick.log','build/verify-sound-banks-diagnostic.log','build/verify-sound-banks-game.log','runtime/sound_bank_loads.inc']
 extra+=['metadata/SOUND_PLAYBACK_RECONSTRUCTION.md','metadata/SOUND_STOP_RECONSTRUCTION.md','metadata/sound-playback-native.json','metadata/sound-playback-rea-evidence.json','metadata/sound-reset-native.json','metadata/sound-stop-native.json','metadata/sound-stop-rea-evidence.json','metadata/sound-stop-before-counterexample.json','metadata/sound-playback-host.json','metadata/sound-allocation-next-native.json','metadata/sound-allocation-next-rea-evidence.json','build/build-sound-playback-stop-final.log','build/build-sound-playback-stop-debug-final.log','build/verify-sound-stop-before-counterexample.log']
 extra+=['metadata/sound-allocation-before-counterexamples.json','build/verify-sound-allocation-before-counterexamples.log']
 extra+=['metadata/SOUND_ALLOCATION_RECONSTRUCTION.md','metadata/sound-allocation-native.json','metadata/sound-allocation-rea-evidence.json','metadata/sound-allocation-host.json','metadata/sound-allocation-regressions-diagnostic.json','metadata/sound-allocation-regressions-game.json','metadata/sound-device-next-native.json','metadata/sound-device-next-rea-evidence.json','metadata/sound-boot-next-native.json','build/build-sound-allocation-final.log','build/build-sound-allocation-debug-final.log','build/verify-sound-allocation-initialization-Debug.log','build/verify-sound-allocation-initialization-Release.log']
 extra+=['metadata/SOUND_DEVICE_RECONSTRUCTION.md','metadata/sound-device-native.json','metadata/sound-device-rea-evidence.json','metadata/audio-global-native-additions.json','metadata/audio-global-startup-audit.json','metadata/sound-device-before-counterexamples.json','metadata/music-configuration-next-native.json','metadata/music-configuration-next-rea-evidence.json','build/build-sound-device-final.log','build/build-sound-device-debug-final.log','build/verify-sound-device-initialization-Debug.log','build/verify-sound-device-initialization-Release.log','build/verify-sound-device-diagnostic.log','build/verify-sound-device-game.log']
 extra+=['metadata/music-configuration-next-counterexamples.json','build/probe-music-configuration-guards.log']
 extra+=['metadata/MUSIC_CONFIGURATION_RECONSTRUCTION.md','metadata/music-configuration-native.json','metadata/music-configuration-rea-evidence.json','metadata/music-configuration-host.json','metadata/music-configuration-regressions-diagnostic.json','metadata/music-configuration-regressions-game.json','metadata/music-cmake-scope-audit.json','direct_c/verify_music_cmake_scope.py','build/verify-music-cmake-scope.log','build/build-music-configuration-final.log','build/build-music-configuration-debug-final.log','build/verify-music-configuration-diagnostic.log','build/verify-music-configuration-game.log','metadata/actor-mission-reserved-next-native.json','metadata/actor-mission-reserved-next-rea-evidence.json']
 extra+=['metadata/ACTOR_RESERVED_RECONSTRUCTION.md','metadata/actor-reserved-native.json','metadata/actor-reserved-rea-evidence.json','metadata/actor-reserved-iterator-evidence.json','metadata/actor-reserved-iterator-rea-evidence.json','metadata/actor-reserved-constructor-evidence.json','metadata/actor-reserved-caller-native.json','metadata/actor-reserved-caller-audit.json','direct_c/audit_actor_reserved_callers.py','tools/actor_reserved_project/CMakeLists.txt','metadata/actor-mission-spawn-next-native.json','metadata/actor-mission-spawn-next-rea-evidence.json','metadata/reconstruction-count.json','build/build-actor-reserved-final.log','build/build-actor-reserved-strict.log','build/verify-actor-reserved-diagnostic.log','build/verify-actor-reserved-game.log']
 subprocess.run([sys.executable,str(ROOT/'direct_c/audit_actor_reserved_callers.py')],check=True)
 caller=json.loads((ROOT/'metadata/actor-reserved-caller-audit.json').read_text());assert caller['passed']
 for source,sha in caller['source_hashes'].items():assert source_matches(source,sha),('actor reserved callers',source)
 extra+=['metadata/ACTOR_SCENARIO_SPAWN_RECONSTRUCTION.md','metadata/actor-scenario-spawn-native.json','metadata/actor-scenario-spawn-rea-evidence.json','metadata/actor-scenario-spawn-context.json','metadata/actor-scenario-spawn-caller-bytes.json','metadata/actor-scenario-spawn-caller-audit.json','direct_c/audit_actor_scenario_spawn_callers.py','tools/actor_scenario_spawn_project/CMakeLists.txt','metadata/actor-command-transition-next-native.json','metadata/actor-command-transition-next-rea-evidence.json','build/mission-spawn/build-final.log','build/mission-spawn/build-strict.log','build/mission-spawn/verify-diagnostic.log','build/mission-spawn/verify-game.log']
 subprocess.run([sys.executable,str(ROOT/'direct_c/audit_actor_scenario_spawn_callers.py')],check=True)
 scenario_callers=json.loads((ROOT/'metadata/actor-scenario-spawn-caller-audit.json').read_text());assert scenario_callers['passed'] and scenario_callers['direct_callsites']==2
 for source,sha in scenario_callers['source_hashes'].items():assert source_matches(source,sha),('scenario callers',source)
 for suffix in ['diagnostic','game']:
  rel=f'metadata/actor-scenario-spawn-composition-{suffix}.json';composition=json.loads((ROOT/rel).read_text())
  assert composition['passed'] and composition['actual_compositions']==12 and composition['actual_parent_invocations']==72
  assert composition['width_cache_scenarios']==4 and composition['width_cache_invocations']==12 and composition['controlled_endpoints_in_compositions']==[]
  assert digest(Path(composition['compiled_C_path']))==composition['compiled_C_sha256']
  assert digest(ROOT/'build/smoke-game/Pharaoh.exe')==composition['native_sha256']
  for source,sha in composition['source_hashes'].items():assert source_matches(source,sha),('scenario composition',suffix,source)
  extra.append(rel)
 extra.append('direct_c/verify_actor_scenario_spawn_composition.py')
 command_audit=json.loads((ROOT/'metadata/actor-command-transition-audit.json').read_text())
 assert command_audit['passed'] and len(command_audit['analytically_unreachable_instructions'])==3
 for source,sha in command_audit['source_hashes'].items():assert source_matches(source,sha),('command transition audit',source)
 for suffix in ['diagnostic','game']:
  rel=f'metadata/actor-command-transition-composition-{suffix}.json';composition=json.loads((ROOT/rel).read_text())
  assert composition['passed'] and composition['checked']==67 and composition['invocations']==201
  assert composition['actual_scenarios']==66 and composition['synthetic_memory_scenarios']==1 and composition['protocol_scenarios']==0
  assert composition['controlled_algorithm_endpoints']==composition['controlled_platform_endpoints']==[]
  assert digest(Path(composition['compiled_C_path']))==composition['compiled_C_sha256']
  assert digest(ROOT/'build/smoke-game/Pharaoh.exe')==composition['native_sha256']
  for source,sha in composition['source_hashes'].items():assert source_matches(source,sha),('command composition',suffix,source)
  extra.append(rel)
 extra+=['direct_c/verify_actor_command_transition_composition.py','direct_c/audit_actor_command_transition.py','metadata/actor-command-transition-audit.json','metadata/actor-command-transition-native.json','metadata/actor-command-transition-additions-rea-records.json','metadata/actor-command-transition-callers-rea-records.json','metadata/ACTOR_COMMAND_TRANSITION_RECONSTRUCTION.md','tools/actor_command_transition_project/CMakeLists.txt','metadata/actor-command-nullary-next-rea-record.json','build/actor-command-transition/main-build.log','build/actor-command-transition/strict.log','build/actor-command-transition/game-final.log','build/actor-command-transition/diagnostic-final.log']
 for suffix in ['diagnostic','game']:
  rel=f'metadata/actor-command-parent-{suffix}.json';parent=json.loads((ROOT/rel).read_text())
  assert parent['passed'] and parent['checked']==14 and parent['invocations']==28
  assert parent['controlled_algorithm_endpoints']==parent['controlled_platform_endpoints']==[]
  assert digest(Path(parent['compiled_C_path']))==parent['compiled_C_sha256']
  assert digest(ROOT/'build/smoke-game/Pharaoh.exe')==parent['native_sha256']
  for source,sha in parent['source_hashes'].items():assert source_matches(source,sha),('retained command parent',suffix,source)
  extra.append(rel)
 extra+=['direct_c/verify_actor_command_parent.py','metadata/actor-command-nullary-next-context-rea-records.json','metadata/actor-command-nullary-next-caller-refs-rea-record.json','metadata/actor-command-nullary-next-notes.json']
 for rel in extra:files[rel]=digest(ROOT/rel)
 result=dict(generated_at=datetime.now(timezone.utc).isoformat(),scope='Release labor, building queries, removal, ring, map repair, message categories, the actor-message controller actor voice routing, sound streams, fades, ambient selection, sample loading and file registration, unsigned-byte comparison, array/file-table lifecycles and building/UI/actor sample banks, 2D playback/cache, explicit-receiver stopping and faithful 2D sample-pool allocation, PCM initialization/shutdown, whole sound boot and sound/music global CRT lifetimes, music configuration/insertion/population selection and byte-set tokenization, reserved-actor initialization/reset/allocation, ordered byte-fill and town iterator, scenario point/group spawning with real group allocation, terrain checks and native RNG ordering, actor command-state transitions and actual callback target search; explicit user-disabled CD fallback; finite genuine compositions; not complete gameplay',complete_game_reconstruction=False,proofs=proofs,files=files,remaining_tracked_region_models=[],next_native_targets=['0041cd00','0049e8b0','004eaec0'],next_scope='Replace the remaining nullary actor command and civic target-search callers with named C; preserve actual allocation, RNG, transition cycles and native return contracts; remaining catalogue/exception dispatch',verification_configuration='Release',host_game_process_launched=False,scenario_adjacent_compositions_per_binary=12,scenario_width_cache_cases_per_binary=4,actor_command_additional_compositions_per_binary=67,settings_production_checked=settings['checked'],settings_debug_release_trace_plain_passed=True,ground_production_checked=ground['checked'],ground_production_passed=True,sound_table_debug_release_checked=sound_tables,sound_initialization_debug_release_checked=sound_initialization,sound_allocation_host_checks_per_configuration=265,historical_allocation_counterexamples_fixed_in_current_C=8,historical_device_boot_counterexamples=2,historical_music_insertion_counterexamples_fixed=4,user_disabled_CD_native_instructions=10,audio_CRT_subset_startup_audit_passed=True,C_seh_contracts_per_configuration=6,native_OS_exception_dispatch_proven=False,real_highest_serial_removal_cases=dict(kinds=[31,202,1],controlled_results=False,final_building_status=4,allocated_effect_actors=16,queued_events=1,sound_enabled=False),real_destruction=dict(composition_scenarios=17,whole_destruction_scenarios=9,controlled_results=False,native_tables=True,asserted_rubble_counts=[4,7,36]),real_access_repair=dict(composition_scenarios=5,controlled_results=False,fixture_dimensions=[16,16],active_buildings=0,checked_attempt_counts=[0,1,15],success_notification=True,exhausted_removal_dispatch=True),processing_receiver_audit_command='python direct_c/audit_processing_receiver.py',processing_receiver_audit_passed=True,processing_historical_receiver_calls_repaired=1,ring_growth_gameplay_unified=True,ring_growth_normal_return_only=True,caller_abort_placeholders_removed=5,caller_syntax_check='passed',caller_audit_command='python direct_c/audit_building_access_returns.py',build_command='cmake --build build/c-only --config Release --target restore_link_check restore_game --parallel 8',remaining_scope=['Parent actor/building query models and broader states beyond finite compositions','Original heap layout and C++ exception/unwind paths beyond normal-return ring verification','Remaining instruction models and decompiler placeholders across the catalogue','End-to-end usable gameplay, enabled sound and physical screen behavior'])
 (ROOT/'metadata/labor-and-access-current.json').write_text(json.dumps(result,indent=2)+'\n')
 print('PASS58 paired reports plus settings, ground, Debug/Release sound tables/initialization/playback and C SEH contracts: current sources/binaries, exact music-tick-only CMake scope audit; complete_game_reconstruction=false')

if __name__=='__main__':main()
