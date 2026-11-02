"""Validate a real independent-C custom-selector/back integration run."""
from pathlib import Path
from datetime import datetime,timezone
import hashlib,json,re
ROOT=Path(__file__).resolve().parent.parent
def sha(path):return hashlib.sha256((ROOT/path).read_bytes()).hexdigest()
def stamp(path):return {'path':path,'sha256':sha(path)}
digest=sha('build/c-only-debug/Debug/restore_link_check.exe')
log='artifacts/menu-mission-integration.log';trace=(ROOT/log).read_text(errors='replace')
assert 'DEBUG TIMEOUT:' in trace and 'EXIT ' not in trace and 'Unreconstructed' not in trace
exceptions=re.findall(r'EXCEPTION ([0-9a-fA-F]{8})\b',trace)
assert exceptions.count('80000003')==1 and all(x in ['80000003','406d1388'] for x in exceptions)
assert 'family action: custom missions through C buttons' in trace
assert 'mission selector: back through C buttons' in trace
frames=[]
for screen,mode in [('selector',61),('return',48)]:
 manifest=f'metadata/mission-{screen}-frame.json';frame=json.loads((ROOT/manifest).read_text())
 assert frame['child_exe_sha256']==digest and frame['completed_frame_copy'] and frame['mode']==mode
 assert frame['unique_16bit_colors']>100 and f'completed frame copied after rendering: mode={mode}' in trace
 frames.append({'screen':screen,'mode':mode,'manifest':stamp(manifest),'image':stamp(f'artifacts/mission-{screen}.bmp'),'raw_frame_sha256':frame['raw_sha256']})
assert not list((ROOT/'build/debug-startup-game/Maps').glob('*.map'))
profile='build/debug-startup-game/Save/Restore.dat';assert sha(profile)=='7794376f0421356ed16a950de128b5029cc30d679b3f04c3f58760e86d242371'
rea=json.loads((ROOT/'metadata/mission-menu-rea-function.json').read_text())['data']
assert rea['operation']=='analyze_function' and rea['subject']['digest']['sha256']=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
sources=['include/mission_menu.h','runtime/mission_menu.c','runtime/menu_callbacks.c','runtime/startup_probe.c','src/unit_01866.c','src/unit_01867.c','src/unit_01868.c','tools/Debug-Standalone.ps1','tools/Test-MenuIntegration.ps1','tools/capture_restore_framebuffer.py','tools/record_mission_menu.py']
report={'format':1,'generated_at':datetime.now(timezone.utc).isoformat(),'passed':True,'child_exe_sha256':digest,'log':stamp(log),'expected_bounded_timeout':True,
 'original_game_process_executed':False,'computer_UI_automation_used':False,'confirmed_flow':['title','main menu','family registry','saved profile load','family options','custom missions','empty custom selector','back through C small-button dispatcher','family options'],
 'frames':frames,'fixture_profile':stamp(profile),'REA_evidence':stamp('metadata/mission-menu-rea-function.json'),'REA_evidence_id':rea['evidence_id'],
 'limitations':['This live C run uses the isolated empty Maps folder at 1024x768. Nonempty lists/metadata and the other two resolutions are native-contract comparisons.','The chunk/codec pipeline and town connectivity have separate native verification. Top-level map/scenario loading, route rebuilding and independent game preparation remain. This does not establish playable gameplay.'],
 'source_hashes':{s:sha(s) for s in sources}}
(ROOT/'metadata/mission-menu-integration.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({'passed':True,'binary':digest,'flow':report['confirmed_flow']},indent=2))
