"""Record the real C load/save selector, row selection and cancel integration."""
from pathlib import Path
from datetime import datetime,timezone
import hashlib,json,re
ROOT=Path(__file__).resolve().parent.parent
def read(p):return json.loads((ROOT/p).read_text(encoding='utf-8-sig'))
def sha(p):return hashlib.sha256((ROOT/p).read_bytes()).hexdigest()
def stamp(p):return {'path':p,'sha256':sha(p)}
binary='build/c-only-debug/Debug/restore_link_check.exe';digest=sha(binary)
native=read('metadata/load-save-menu-emulation.json');assert native['passed'] and native['checked']==1968
assert native['compiled_C_sha256']==sha('build/c-only-debug/Debug/restore_load_save_menu_verify.exe')
for source,h in native['source_hashes'].items():assert sha(source)==h,'Native evidence is stale: '+source
frames=[]
for name,mode in [('load-save',13),('load-save-selected',13),('return',48)]:
 path=f'metadata/load-{name}-frame.json';frame=read(path)
 assert frame['child_exe_sha256']==digest and frame['completed_frame_copy'] and frame['mode']==mode
 assert frame['unique_16bit_colors']>1000
 if mode==13:
  state=frame['live_save_state'];assert state['count']==state['total']==1 and state['selected']==state['first']==0
  assert state['operation']==1 and state['return_mode']==48
  assert bytes.fromhex(state['filename_hex']).split(b'\0',1)[0]==b'restore-native'
 frames.append({'stage':name,'manifest':stamp(path),'image':stamp(f'artifacts/load-{name}.bmp'),'raw_frame_sha256':frame['raw_sha256'],'visually_inspected':True})
assert frames[0]['raw_frame_sha256']!=frames[1]['raw_frame_sha256'],'Selected text was not rendered before the capture'
log='artifacts/menu-load-integration.log';trace=(ROOT/log).read_text(errors='replace')
for marker in ['family action: load game through C buttons','load/save: select first row through C buttons','load/save: cancel through C buttons','DEBUG TIMEOUT:']:assert marker in trace,marker
assert 'Unreconstructed' not in trace and 'EXIT ' not in trace
assert re.findall(r'EXCEPTION ([0-9a-fA-F]{8})\b',trace)==['80000003']
profile='build/debug-startup-game/Save/Restore.dat'
assert sha(profile)=='7794376f0421356ed16a950de128b5029cc30d679b3f04c3f58760e86d242371'
assert not list((ROOT/'build/debug-startup-game/Save/Restore').iterdir()),'SAV fixture cleanup is incomplete'
rea=read('metadata/load-save-rea-function.json')['data'];assert rea['operation']=='analyze_function' and rea['subject']['digest']['sha256']==native['native_sha256']
sources=set(native['source_hashes'])|{'runtime/menu_callbacks.c','runtime/startup_probe.c','include/startup_probe.h','src/unit_01868.c','tools/Debug-Standalone.ps1','tools/Test-MenuIntegration.ps1','tools/capture_restore_framebuffer.py','tools/record_load_save_menu.py'}
report={'format':1,'generated_at':datetime.now(timezone.utc).isoformat(),'passed':True,'child_exe_sha256':digest,
 'original_game_process_executed':False,'computer_UI_automation_used':False,'original_machine_code_fallback_used':False,
 'confirmed_flow':['title','main menu','family registry','family options with verified SAV fixture','load game selector','actual row callback populates filename','cancel through C small-button callback','family options'],
 'log':stamp(log),'frames':frames,'fixture_removed_after_run':True,'profile':stamp(profile),
 'REA_evidence':stamp('metadata/load-save-rea-function.json'),'REA_evidence_id':rea['evidence_id'],
 'limitations':['Real C captures use 1024x768; native draw-call comparisons cover the other viewport offsets.','The confirm controller is natively checked, with deep city loading/saving/deletion controlled. This integration selects and cancels; it does not prove independent gameplay.'],
 'source_hashes':{s:sha(s) for s in sorted(sources)}}
(ROOT/'metadata/load-save-menu-integration.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({'passed':True,'native_cases':native['checked'],'flow':report['confirmed_flow'],'binary':digest},indent=2))
