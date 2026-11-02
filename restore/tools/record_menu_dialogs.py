"""Validate current owned-C dialog runs and retain their bounded integration evidence."""
from pathlib import Path
from datetime import datetime,timezone
import hashlib,json,re
ROOT=Path(__file__).resolve().parent.parent
def sha(path):return hashlib.sha256((ROOT/path).read_bytes()).hexdigest()
def read(path):return json.loads((ROOT/path).read_text(encoding='utf-8-sig'))
def stamp(path):return {'path':path,'sha256':sha(path)}
binary='build/c-only-debug/Debug/restore_link_check.exe';digest=sha(binary)
cases=[]
for name,returned,accepted in [('duplicate',54,1),('cancel',47,0),('delete',47,1)]:
 log=f'artifacts/menu-dialog-{name}.log';trace=(ROOT/log).read_text(errors='replace')
 assert 'DEBUG TIMEOUT:' in trace and 'EXIT ' not in trace and 'Unreconstructed' not in trace,name+' did not remain live through its bounded run'
 exceptions=re.findall(r'EXCEPTION ([0-9a-fA-F]{8})\b',trace)
 assert exceptions.count('80000003')==1 and all(x in ['80000003','406d1388'] for x in exceptions),name+' has unexpected exceptions'
 assert f'family dialog: respond {accepted} through C buttons' in trace and 'completed frame copied after rendering: mode=63' in trace
 frames=[]
 for screen,mode in [('dialog',63),('return',returned)]:
  manifest=f'metadata/{name}-{screen}-frame.json';frame=read(manifest)
  assert frame['child_exe_sha256']==digest and frame['completed_frame_copy'] and frame['mode']==mode,name+' frame is stale/incomplete'
  frames.append({'screen':screen,'manifest':stamp(manifest),'image':stamp(f'artifacts/{name}-{screen}.bmp'),'visually_inspected':True})
 cases.append({'scenario':name,'log':stamp(log),'frames':frames,'expected_bounded_timeout':True})
removed=read('metadata/delete-confirm-filesystem.json');cancelled=read('metadata/delete-cancel-filesystem.json')
assert removed['child_exe_sha256']==cancelled['child_exe_sha256']==digest
assert removed['profile_removed'] and removed['sav_removed'] and removed['other_file_preserved'] and removed['directory_retained']
assert cancelled['profile_preserved'] and cancelled['sav_preserved'] and cancelled['other_file_preserved']
assert removed['other_file_sha256']==cancelled['other_file_sha256']=='041772459fd885d4ea9a994dc6e4b9c1dd1d93e4af042f5a7a441d45c38d7e0e'
assert cancelled['profile_sha256']==sha('artifacts/family-dialog-fixtures/Restore.dat')==sha('build/debug-startup-game/Save/Restore.dat')
assert not list((ROOT/'build/debug-startup-game/Save/Restore').iterdir()),'Owned filesystem fixtures were not cleaned up'
sources=['runtime/menu_dialog.c','runtime/family_delete.c','runtime/startup_probe.c','src/unit_00398.c','src/unit_00399.c','src/unit_00418.c','src/unit_00419.c','src/unit_01669.c','src/unit_01690.c','src/unit_01704.c','src/unit_01705.c','tools/Debug-Standalone.ps1','tools/capture_restore_framebuffer.py']
report={'format':1,'generated_at':datetime.now(timezone.utc).isoformat(),'passed':True,'child_exe_sha256':digest,
 'original_game_process_executed':False,'computer_UI_automation_used':False,
 'method':'Isolated restored C startup, real C menu/small-button dispatch and WM_CHAR name entry; immutable post-render frames read from the owned C child. Actual filesystem effects checked on a known fixture profile and two marker files.',
 'cases':cases,'cancel_filesystem':stamp('metadata/delete-cancel-filesystem.json'),'delete_filesystem':stamp('metadata/delete-confirm-filesystem.json'),
 'fixture_cleanup':'C-created Restore profile restored byte-for-byte from the retained fixture; marker files removed from the isolated Save folder.',
 'limitations':['Visual captures use 1024x768. Native draw-call checks cover the other two viewport offsets. Other modal response callbacks and later menu branches remain incomplete.'],
 'source_hashes':{source:sha(source) for source in sources}}
(ROOT/'metadata/menu-dialog-integration.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({'passed':True,'scenarios':len(cases),'child_exe_sha256':digest},indent=2))
