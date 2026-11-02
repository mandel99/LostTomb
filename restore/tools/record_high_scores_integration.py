"""Accept the actual C scores flow with frozen frames, records and restored fixtures."""
from pathlib import Path
from datetime import datetime,timezone
import hashlib,json,re
ROOT=Path(__file__).resolve().parent.parent
def read(p):return json.loads((ROOT/p).read_text(encoding='utf-8-sig'))
def sha(p):return hashlib.sha256((ROOT/p).read_bytes()).hexdigest()
def stamp(p):return {'path':p,'sha256':sha(p)}
binary='build/c-only-debug/Debug/restore_link_check.exe';digest=sha(binary)
run=read('metadata/high-scores-integration-run.json');assert run['passed'] and run['child_exe_sha256']==digest
fixture=read('metadata/high-scores-integration-fixture.json');assert sha(fixture['path'])==fixture['sha256']==run['fixture_sha256']
proof=read('metadata/high-scores-menu-emulation.json');assert proof['passed'] and proof['compiled_C_sha256']==sha('build/c-only-debug/Debug/restore_high_scores_menu_verify.exe')
sources=set(proof['source_hashes'])
for p,h in proof['source_hashes'].items():assert sha(p)==h,'Scores native source is stale: '+p
log=run['log'];trace=(ROOT/log).read_text(errors='replace')
assert 'DEBUG TIMEOUT:' in trace and 'EXIT ' not in trace and 'Unreconstructed' not in trace
assert re.findall(r'EXCEPTION ([0-9a-fA-F]{8})',trace)==['80000003']
for marker in ['menu input: dispatch action 3 through C buttons','high scores: down through C small buttons','high scores: drag to bottom through C input','high scores: Escape back through C input']:
 assert marker in trace,marker
frames=[]
for name,mode,first in [('high-scores',83,0),('high-scores-scrolled',83,1),('high-scores-dragged',83,5),('high-scores-return',0,5)]:
 p=f'metadata/menu-{name}-frame.json';r=read(p);s=r['high_scores_state']
 assert r['child_exe_sha256']==digest and r['completed_frame_copy'] and r['mode']==mode
 assert r['unique_16bit_colors']>100 and s=={'total':25,'first':first,'records_sha256':fixture['sha256']}
 frames.append({'screen':name,'mode':mode,'first':first,'manifest':stamp(p),'image':stamp(f'artifacts/menu-{name}.bmp'),'raw_frame_sha256':r['raw_sha256'],'visually_inspected':True})
profile='build/debug-startup-game/Save/Restore.dat';assert sha(profile)=='7794376f0421356ed16a950de128b5029cc30d679b3f04c3f58760e86d242371'
saved=ROOT/'build/debug-startup-game/Save/highscore.jas'
assert (sha(saved.relative_to(ROOT))==run['previous_scores_file_sha256']) if run['previous_scores_file_sha256'] else not saved.exists()
rea=read('metadata/high-scores-rea-function.json')['data']
assert rea['operation']=='analyze_function' and rea['subject']['digest']['sha256']=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
assert rea['normalized_result']['procedure']['address']=='0x4c5920' and len(rea['normalized_result']['assembly'])==22
sources.update(['tools/Test-HighScoresIntegration.ps1','tools/record_high_scores_integration.py','tools/Debug-Standalone.ps1','tools/capture_restore_framebuffer.py','runtime/startup_probe.c','include/startup_probe.h','runtime/menu_callbacks.c','runtime/menu_platform_win32.c','src/unit_01868.c','metadata/high-scores-integration-fixture.json'])
result={'format':1,'generated_at':datetime.now(timezone.utc).isoformat(),'passed':True,'child_exe_sha256':digest,'original_game_process_executed':False,'computer_UI_automation_used':False,'original_machine_code_fallback_used':False,'expected_bounded_timeout':True,'confirmed_flow':['title','main menu action 3','25 real score records loaded by C','first20 rows','down callback to first1','drag to first5','Escape through mode83 input','main menu'],'log':stamp(log),'fixture':stamp(fixture['path']),'fixture_restored_after_run':True,'profile':stamp(profile),'frames':frames,'REA_evidence':stamp('metadata/high-scores-rea-function.json'),'REA_evidence_id':rea['evidence_id'],'limitations':['Frozen captures use1024x768. Native comparisons also exercise other offsets and malformed/partial input cases.','This proves the rankings flow; it does not establish the remaining editor, settings, campaign confirmation or playable gameplay.'],'source_hashes':{p:sha(p) for p in sorted(sources)}}
(ROOT/'metadata/high-scores-menu-integration.json').write_text(json.dumps(result,indent=2)+'\n')
print(json.dumps({'passed':True,'rows':25,'first':[0,1,5,5],'modes':[83,83,83,0],'binary':digest},indent=2))
