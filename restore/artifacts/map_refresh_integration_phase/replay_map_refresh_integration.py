"""List or run the exact production replay plan, after root applies all mappings."""
from pathlib import Path
import argparse,hashlib,json,subprocess,sys
HERE=Path(__file__).resolve().parent;ROOT=HERE.parent.parent
ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--debug-dir',type=Path,required=True);ap.add_argument('--release-dir',type=Path,required=True);ap.add_argument('--run',action='store_true');ap.add_argument('--phase',action='append');args=ap.parse_args()
manifest=HERE/'integration-manifest.json';sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest();before=sha(manifest);plan=json.loads(manifest.read_text());completed=[]
if args.run:
 own_freeze=json.loads((HERE/'integration-freeze.json').read_text())
 for name,want in own_freeze['files'].items():assert sha(ROOT/name)==want,name
if args.run:subprocess.run([sys.executable,str(HERE/'audit_map_refresh_integration.py'),'--check-applied'],cwd=ROOT,check=True)
for suite in plan['production_replays']:
 if args.phase and suite['phase'] not in args.phase:continue
 argv=[part.replace('{python}',sys.executable).replace('{debug}',str(args.debug_dir.resolve())).replace('{release}',str(args.release_dir.resolve())) for part in suite['argv_template']]
 if not args.run:print(subprocess.list2cmdline(argv));continue
 print('REPLAY '+suite['phase']+' '+suite['script'],flush=True);subprocess.run(argv,cwd=ROOT,check=True)
 path=ROOT/suite['output'];report=json.loads(path.read_text());comparisons=report.get('comparisons',report.get('variant_comparisons'))
 assert report['passed'] and report['production'] and report['checked']==suite['checked'] and comparisons==suite['comparisons'],(path,report.get('checked'),comparisons)
 assert not report.get('source_drift',[]) and not report.get('binary_drift',[]) and not report.get('source_drift_at_completion',[]),path
 completed.append({'phase':suite['phase'],'script':suite['script'],'report':suite['output'],'sha256':sha(path),'checked':report['checked'],'comparisons':comparisons})
if args.run:
 assert sha(manifest)==before
 subprocess.run([sys.executable,str(HERE/'audit_map_refresh_integration.py'),'--check-applied'],cwd=ROOT,check=True)
 summary={'passed':True,'production':True,'whole_map_refresh_complete':False,'integration_manifest_sha256':before,'suites':completed,'suite_count':len(completed),'canonical_suite_cases':sum(x['checked'] for x in completed),'variant_comparisons':sum(x['comparisons'] for x in completed),'original_game_executed':False,'counts_deduplicate_game_scenarios':False,'root134_broader_suite_counts_included':False}
 (ROOT/'metadata'/('map-refresh-integration-replay-'+('-'.join(args.phase) if args.phase else 'all')+'.json')).write_text(json.dumps(summary,indent=2)+'\n')
 print(json.dumps({k:v for k,v in summary.items() if k!='suites'}))
