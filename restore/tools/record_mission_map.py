"""Record a nonempty mission preview from the owned independent C process."""
from pathlib import Path
from datetime import datetime,timezone
import hashlib,json,re
ROOT=Path(__file__).resolve().parent.parent
def read(path):return json.loads((ROOT/path).read_text(encoding='utf-8-sig'))
def sha(path):return hashlib.sha256((ROOT/path).read_bytes()).hexdigest()
def stamp(path):return {'path':path,'sha256':sha(path)}
binary='build/c-only-debug/Debug/restore_link_check.exe';digest=sha(binary)
native=read('metadata/mission-map-emulation.json');assert native['passed'] and native['checked']==2
fixture=next(x for x in native['maps'] if x['path']=='artifacts/mission-map-fixtures/mission-52.map')
frame=read('metadata/map-selector-frame.json');state=frame['live_mission_state']
assert frame['child_exe_sha256']==digest and frame['completed_frame_copy'] and frame['mode']==61 and frame['current_mode']==61
assert frame['unique_16bit_colors']>1000 and state['list_count']==1 and state['selected_index']==0 and state['selector_kind']==0
assert state['file_version']==160 and state['declared_chunks']==24
assert state['scenario_info_sha256']==next(x['sha256'] for x in fixture['chunks'] if x['index']==14)
assert state['nonempty_routes']==fixture['nonempty_routes']==4 and state['route_points']==fixture['route_points']==240
assert bytes.fromhex(state['file_label_hex']).split(b'\0',1)[0]==b'restore-native-v160'
assert bytes.fromhex(state['scenario_info_title_hex']).split(b'\0',1)[0]==b'The Battle of Actium'
log='artifacts/menu-map-integration.log';trace=(ROOT/log).read_text(errors='replace')
assert 'DEBUG TIMEOUT:' in trace and 'EXIT ' not in trace and 'Unreconstructed' not in trace
exceptions=re.findall(r'EXCEPTION ([0-9a-fA-F]{8})\b',trace)
assert exceptions.count('80000003')==1 and all(x in ['80000003','406d1388'] for x in exceptions)
assert 'family action: custom missions through C buttons' in trace and 'completed frame copied after rendering: mode=61' in trace
assert not list((ROOT/'build/debug-startup-game/Maps').glob('*.map')),'Fixture cleanup is incomplete'
profile='build/debug-startup-game/Save/Restore.dat';assert sha(profile)=='7794376f0421356ed16a950de128b5029cc30d679b3f04c3f58760e86d242371'
rea=read('metadata/mission-route-rea-function.json')['data']
assert rea['operation']=='analyze_function' and rea['subject']['digest']['sha256']==native['native_sha256']
sources=['tools/Debug-Standalone.ps1','tools/Test-MenuIntegration.ps1','tools/capture_restore_framebuffer.py','tools/record_mission_map.py',
 'runtime/startup_probe.c','runtime/mission_menu.c','src/unit_00309.c','src/unit_00311.c','src/unit_01830.c']
for name in ['mission-map','mission-routes','mission-loaders','numeric-text']:
 proof=read(f'metadata/{name}-emulation.json');assert proof['passed']
 for source,h in proof['source_hashes'].items():assert sha(source)==h,'Native source evidence is stale: '+source
 sources.extend(proof['source_hashes'])
report={'format':1,'generated_at':datetime.now(timezone.utc).isoformat(),'passed':True,'child_exe_sha256':digest,
 'original_game_process_executed':False,'computer_UI_automation_used':False,'original_machine_code_fallback_used':False,
 'confirmed_flow':['title','main menu','family registry','profile load','family options','custom missions','single real MAP decoded by C','nonempty mission preview'],
 'log':stamp(log),'expected_bounded_timeout':True,'frame':stamp('metadata/map-selector-frame.json'),'image':stamp('artifacts/map-selector.bmp'),
 'live_mission_state':state,'visually_inspected':True,'fixture_native_proof':stamp('metadata/mission-map-emulation.json'),'fixture':fixture['path'],'fixture_sha256':fixture['sha256'],
 'fixture_removed_after_run':True,'profile':stamp(profile),'REA_evidence':stamp('metadata/mission-route-rea-function.json'),'REA_evidence_id':rea['evidence_id'],
 'limitations':['The local fixture is a v160 MAP derived from original mission data, not a separately authored custom map.','This captures the actual C preview at 1024x768. Other resolutions have native draw-call coverage; Go to city, briefing and gameplay remain unproved.','Old-format compatibility helpers and builtin campaign preparation remain separate work.'],
 'source_hashes':{s:sha(s) for s in sorted(set(sources))}}
(ROOT/'metadata/mission-map-integration.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({'passed':True,'title':'The Battle of Actium','routes':state['nonempty_routes'],'route_points':state['route_points'],'binary':digest},indent=2))
