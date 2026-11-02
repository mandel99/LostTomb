"""One new known-cache headless batch; staged-only postprocessing."""
import argparse,json,re,subprocess,sys,time
from pathlib import Path
from common import ROOT,PRIVATE,sha,confined,root_snapshot,differences,create_seed
ap=argparse.ArgumentParser();ap.add_argument('--name',default='known4527');ap.add_argument('--run',action='store_true');ap.add_argument('--snapshot-cache',type=Path);a=ap.parse_args()
if not re.fullmatch(r'[a-z][a-z0-9_-]{0,31}',a.name):raise SystemExit('Unsafe private run name')
run=confined(PRIVATE/'runs'/a.name)
if not a.run:print(json.dumps({'run':str(run),'requires_explicit_run':True}));sys.exit(0)
if run.exists():raise SystemExit('Existing private run refused; no overwrite/lock manipulation')
run.mkdir(parents=True);print('SNAPSHOT_START all existing root files outside new v2 output/code',flush=True);previous=json.loads(confined(a.snapshot_cache).read_text()) if a.snapshot_cache else None;before=root_snapshot(previous);(run/'protected-before.json').write_text(json.dumps(before,separators=(',',':'))+'\n');print('SNAPSHOT_COMPLETE',len(before),flush=True);seed=create_seed(run);print('SEED_COMPLETE',len(seed['functions']),seed['native_instructions_pinned'],'pinned instructions;',len(seed['protected_functions']),'protected native addresses',flush=True)
cmd=['powershell.exe','-NoProfile','-File',str(ROOT/'tools/bulk_type_export_v2/Invoke-Batch.ps1'),'-RunRoot',str(run)];(run/'launch.json').write_text(json.dumps({'command':cmd,'tools':{p.name:sha(p) for p in (ROOT/'tools/bulk_type_export_v2').iterdir() if p.is_file()},'root_write_paths_allowed':[str(PRIVATE),str(ROOT/'tools/bulk_type_export_v2')]},indent=2)+'\n');print('HEADLESS_START',str(run),flush=True)
with (run/'headless-stdout.log').open('w') as out,(run/'headless-stderr.log').open('w') as err:r=subprocess.run(cmd,cwd=ROOT,stdout=out,stderr=err)
if r.returncode or not(run/'facts/index.json').exists():
 (run/'failure.json').write_text(json.dumps({'returncode':r.returncode,'existing_project_fallback':False,'logs_retained':True},indent=2)+'\n');raise SystemExit('Private headless failed; inspect new run logs; existing projects untouched')
print('HEADLESS_COMPLETE',flush=True)
from select_candidates import select
selection=select(run,50);print('CANDIDATES_COMPLETE',selection['emitted'],flush=True)
after=root_snapshot(before);(run/'protected-after.json').write_text(json.dumps(after,separators=(',',':'))+'\n');delta=differences(before,after);(run/'external-root-drifts.json').write_text(json.dumps(delta,indent=2)+'\n');(run/'run-summary.json').write_text(json.dumps({'completed':True,'known_functions':len(seed['functions']),'native_instructions_pinned':seed['native_instructions_pinned'],'protected_files_before':len(before),'protected_files_after':len(after),'observed_external_root_drifts':len(delta),'candidate_selection':selection,'all_functions_verified':False,'product_apply_operation_exists':False},indent=2)+'\n');print('BATCH_COMPLETE',len(delta),'observed root changes; no writes outside v2 paths',flush=True)
