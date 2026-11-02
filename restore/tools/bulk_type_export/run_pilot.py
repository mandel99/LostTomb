"""Explicitly private headless run plus guarded staging emitter."""
import argparse,json,re,subprocess,sys
from pathlib import Path
from emit_candidates import ROOT,PRIVATE,PIN,sha,protection,emit
ap=argparse.ArgumentParser();ap.add_argument('--name',default='guarded');ap.add_argument('--run',action='store_true');a=ap.parse_args()
if not re.fullmatch(r'[a-z][a-z0-9_-]{0,31}',a.name):raise SystemExit('Unsafe private run name')
run=PRIVATE/'runs'/a.name;facts=run/'typed-facts.json';command=['powershell.exe','-NoProfile','-File',str(ROOT/'tools/bulk_type_export/Invoke-TypedPilot.ps1'),'-ProjectRoot',str(run/'project'),'-Output',str(facts),'-InputFile',str(ROOT/'build/smoke-game/Pharaoh.exe')]
if not a.run:print(json.dumps({'private_run':str(run),'command':command,'run_requires_explicit_flag':True}));sys.exit(0)
if run.exists():raise SystemExit('Private run already exists; choose a new name. No lock removal/overwrite.')
assert sha(ROOT/'build/smoke-game/Pharaoh.exe')==PIN;before=protection();run.mkdir(parents=True);(run/'protected-before.json').write_text(json.dumps(before,indent=2)+'\n')
with (run/'headless-stdout.log').open('w') as out,(run/'headless-stderr.log').open('w') as err:result=subprocess.run(command,cwd=ROOT,stdout=out,stderr=err)
if result.returncode or not facts.exists():raise SystemExit('Private headless failed; inspect '+str(run)+'. Existing projects were not touched.')
r=emit(facts,run/'candidates');after=protection();drift=[p for p,h in before.items() if after.get(p)!=h];summary={'passed':not drift,'private_project':str(run/'project'),'facts':str(facts),'facts_sha256':sha(facts),'protected_source_drift':drift,'protected_files':len(before),'staged':len(r['staged']),'quarantined':len(r['quarantined']),'native_instructions_authenticated':r['native_instructions_authenticated'],'script_hashes':{p.name:sha(p) for p in (ROOT/'tools/bulk_type_export').glob('*') if p.is_file()},'product_files_written':[]};(run/'run-summary.json').write_text(json.dumps(summary,indent=2)+'\n');print(json.dumps(summary));sys.exit(0 if summary['passed'] else 1)
