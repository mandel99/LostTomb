"""Validate the bounded real-C family-to-campaign selection integration."""
from pathlib import Path
from datetime import datetime,timezone
import hashlib,json,re
ROOT=Path(__file__).resolve().parent.parent
def sha(path):return hashlib.sha256((ROOT/path).read_bytes()).hexdigest()
def stamp(path):return {'path':path,'sha256':sha(path)}
binary='build/c-only-debug/Debug/restore_link_check.exe';digest=sha(binary)
log='artifacts/menu-campaign-integration.log';trace=(ROOT/log).read_text(errors='replace')
assert 'DEBUG TIMEOUT:' in trace and 'EXIT ' not in trace and 'Unreconstructed' not in trace
exceptions=re.findall(r'EXCEPTION ([0-9a-fA-F]{8})\b',trace)
assert exceptions.count('80000003')==1 and all(x in ['80000003','406d1388'] for x in exceptions)
assert 'family selection: confirm through C buttons' in trace and 'family action: history through C buttons' in trace
frames=[]
for screen,mode in [('family',47),('options',48),('campaign',23)]:
 manifest=f'metadata/menu-{screen}-frame.json';frame=json.loads((ROOT/manifest).read_text())
 assert frame['child_exe_sha256']==digest and frame['completed_frame_copy'] and frame['mode']==mode
 assert frame['unique_16bit_colors']>100 and 'completed frame copied after rendering: mode='+str(mode) in trace
 frames.append({'screen':screen,'mode':mode,'manifest':stamp(manifest),'image':stamp(f'artifacts/menu-{screen}-frame.bmp'),'raw_frame_sha256':frame['raw_sha256']})
profile='build/debug-startup-game/Save/Restore.dat'
assert sha(profile)=='7794376f0421356ed16a950de128b5029cc30d679b3f04c3f58760e86d242371'
assert not list((ROOT/'build/debug-startup-game/Save/Restore').iterdir())
rea=json.loads((ROOT/'metadata/campaign-selector-rea-function.json').read_text())['data']
assert rea['operation']=='analyze_function' and rea['subject']['digest']['sha256']=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
sources=['runtime/campaign_menu.c','runtime/family_options.c','runtime/menu_callbacks.c','runtime/startup_probe.c','src/unit_00261.c','src/unit_00262.c','src/unit_00263.c','src/unit_00308.c','src/unit_01868.c','tools/Debug-Standalone.ps1','tools/capture_restore_framebuffer.py','tools/record_campaign_menu.py']
report={'format':1,'generated_at':datetime.now(timezone.utc).isoformat(),'passed':True,'child_exe_sha256':digest,'log':stamp(log),
 'expected_bounded_timeout':True,'original_game_process_executed':False,'computer_UI_automation_used':False,
 'confirmed_flow':['title','main menu','family registry','saved profile load','family options','history action through C dispatcher','campaign/era selector'],
 'frames':frames,'fixture_profile':stamp(profile),'REA_evidence':stamp('metadata/campaign-selector-rea-function.json'),'REA_evidence_id':rea['evidence_id'],
 'limitations':['Only the 1024x768 real-C framebuffer is captured. Native renderer calls cover 640x480 and 800x600 too.','Mission confirmation, movie playback on this path and later mission preparation are controlled endpoints in native comparisons. Independent gameplay is not proved.'],
 'source_hashes':{s:sha(s) for s in sources}}
(ROOT/'metadata/campaign-menu-integration.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({'passed':True,'flow':report['confirmed_flow'],'binary':digest},indent=2))
