"""Record reproducible launch evidence without promoting incomplete gameplay to complete."""
from pathlib import Path
import hashlib,json,re,datetime
ROOT=Path(__file__).resolve().parent.parent
def read(p):return (ROOT/p).read_text(encoding='utf-8-sig',errors='replace')
def digest(p):return hashlib.sha256((ROOT/p).read_bytes()).hexdigest()
def fatal_exceptions(text):
 events=list(re.finditer(r'^EXCEPTION ([0-9a-f]{8}) at.*$',text,re.M))
 failures=[]
 for i,event in enumerate(events):
  block=text[event.end():events[i+1].start() if i+1<len(events) else len(text)]
  if event[1] in ['c0000005','80000003'] and not any(label in block for label in ['INITIAL LOADER BREAKPOINT','OBSERVED BREAKPOINT']):
   failures.append(event[1])
 return failures
previous=json.loads(read('metadata/debug-session.json'))
log=read('artifacts/startup-menu-final.log')
units=json.loads(read('units.json'))
sources={u['file']:digest(u['file']) for u in units}
for folder in ['include','runtime']:
 for file in (ROOT/folder).glob('*.h' if folder=='include' else '*.c'):
  sources[file.relative_to(ROOT).as_posix()]=digest(file.relative_to(ROOT))
for file in ['tools/debug_child.c','tools/Debug-Standalone.ps1','tools/Run-Standalone.ps1','tools/prepare_c.py','tools/repair_discarded_returns.py','tools/record_startup_launch_repair.py','tools/capture_restore_framebuffer.py','tools/audit_standalone_readiness.mjs','CMakeLists.txt']:
 sources[file]=digest(file)
frame=json.loads(read('metadata/startup-menu-frame.json')) if (ROOT/'metadata/startup-menu-frame.json').exists() else {}
visual=json.loads(read('metadata/startup-menu-visual-review.json'))
images={}
for name,path in [('debug','build/c-only-debug/Debug/restore_link_check.exe'),('release','build/c-only/Release/restore_link_check.exe'),('debugger','build/c-only-debug/Debug/restore_debug.exe')]:
 images[name]={'file':path,'sha256':digest(path),'bytes':(ROOT/path).stat().st_size}
report={'format':2,'generated_at':datetime.datetime.now(datetime.timezone.utc).isoformat(),'scope':'Bounded owned-child launch of independent reconstructed C through Bink, title and main menu',
 'original_game_executed':False,'hybrid_executed':False,'restored_C_startup_executed':True,'entry':'0x00413ed0','full_game_loop_reached':'STARTUP frame mode=' in log,
 'gameplay_tested':False,'playable_full_game_established':False,'build':dict(previous['build'],images=images),
 'reproduce':{'working_directory':str(ROOT/'build/debug-startup-game'),'command':'.\\tools\\Debug-Standalone.ps1 -ContinueFromTitle -TimeoutSeconds 180',
 'child_compatibility':'16BITCOLOR HIGHDPIAWARE DWM8And16BitMitigation','RESTORE_TRACE_STARTUP':'1','diagnostic_input':'One left-release flag after a real title frame; recovered C dispatcher performs the transition. No desktop/window input is sent.',
 'normal_manual_launcher':'.\\tools\\Run-Standalone.ps1','original_installation_modified':False},
 'evidence':{'log':'artifacts/startup-menu-final.log','log_sha256':digest('artifacts/startup-menu-final.log'),'framebuffer':frame,
 'bink_opened':bool(re.search(r'STARTUP BinkOpen .*movie=(?!00000000)[0-9a-f]+',log)),
 'bink_completed':bool(re.search(r'STARTUP Bink completed decoded=3281 mode=68',log)),
 'modes':[int(v) for v in re.findall(r'STARTUP frame mode=(\d+)',log)],
 'fatal_exceptions':fatal_exceptions(log),
 'capture_matches_debug_image':frame.get('child_exe_sha256')==images['debug']['sha256'],
 'visual_review':dict(visual,image_hash_matches=digest(visual['image'])==visual['sha256']),
 'run_ended_by_bound':'DEBUG TIMEOUT:' in log},
 'repairs':['52-byte startup geometry records and eight explicit receivers; 18 full-width entry globals',
 '317 discarded-result return traps removed while retaining every call and its arguments',
 'DirectDraw pointers and WINAPI IsLost calls; 96 frame-entry combinations tested',
 'Directory buffer and drive environment name reconstructed; long-path/error cases tested',
 'Five stream methods and 83 caller arguments reconstructed; five sample/cache methods and four public callers receive their objects',
 'Full-width simulation RNG and clock state; three explicit object receivers in simulation callbacks. Active flood/building bodies remain otherwise unreviewed',
 'RLE color fill and compressed-row skip reconstructed; menu, style-table and font bindings repaired',
 'Glyph map now uses bytes; image dimensions use 64-byte records; text selection reuses verified byte scanning'],
 'verification':{'debug_log':'artifacts/startup-debug-final-tests.log','release_log':'artifacts/startup-release-final-tests.log','source_only_tests_per_configuration':37,
 'native_comparisons':{p:json.loads(read('metadata/'+p)).get('checked') for p in ['startup-geometry-emulation.json','sound-streams-emulation.json','tick-timer-emulation.json','sprite-pixels-emulation.json','sprite-render-emulation.json','menu-render-emulation.json']},
 'native_limitations':'Native comparisons use controlled endpoints where declared and never launch the original game; sample playback has source-only lifecycle checks and live C execution, not a complete differential proof.'},
 'reference_project':{'path':str(ROOT.parent/'PharaohRewrite'),'used':['docs/INTRO_MENU_MAP.cs.md','docs/MENU_FONT_UTF8_PLAN.cs.md','src/Pharaoh.Desktop/LegacyBitmapFont.cs','analysis/ghidra/flood-cycle-004be770-live.md'],'modified':False,'linked_into_C':False},
 'last_startup_failure':{'stage':'standalone C Bink/title/menu launch; see evidence for bounded outcome','address':None,'resolved':not fatal_exceptions(log)},
 'remaining_obligations':['Restore menu action callback 0x00417ff0 and other numeric callback tables before claiming interactive menu behavior.','Mission loading, simulation, other raster variants, exception handling and cleanup still need review.'],
 'source_hashes':sources}
report['startup_rendering_complete']=report['evidence']['bink_completed'] and 0 in report['evidence']['modes'] and not report['evidence']['fatal_exceptions'] and report['evidence']['capture_matches_debug_image'] and visual['passed'] and report['evidence']['visual_review']['image_hash_matches']
report['evidence']['release_short_run']={'log':'artifacts/startup-release-menu.log','sha256':digest('artifacts/startup-release-menu.log'),'intro_deliberately_cancelled_after_one_decoded_frame':True,'menu_reached':'STARTUP frame mode=0' in read('artifacts/startup-release-menu.log'),'fatal_exceptions':fatal_exceptions(read('artifacts/startup-release-menu.log')),'limit_seconds':15,'last_sample':'Miles opens the menu music stream at the time limit; no Release framebuffer was accepted.'}
report['assets_manifests']=['metadata/intro-local-asset.json','metadata/music-local-assets.json','metadata/sound-effect-local-assets.json','build/c-only-debug/Debug/media-dependencies.json','build/c-only/Release/media-dependencies.json']
(ROOT/'metadata/startup-launch-repair.json').write_text(json.dumps(report,indent=2)+'\n')
(ROOT/'metadata/debug-session.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({k:report[k] for k in ['full_game_loop_reached','playable_full_game_established','evidence','verification','remaining_obligations']},indent=2))
