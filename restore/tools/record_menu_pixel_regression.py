"""Compare retained hashes while separating current captures from historical images."""
from pathlib import Path
from datetime import datetime,timezone
import hashlib,json
ROOT=Path(__file__).resolve().parent.parent
def read(p):return json.loads((ROOT/p).read_text(encoding='utf-8-sig'))
def sha(p):return hashlib.sha256((ROOT/p).read_bytes()).hexdigest()
binary='build/c-only-debug/Debug/restore_link_check.exe';digest=sha(binary)
baseline_path='artifacts/minimap-prior-frame-hashes.json';baseline=read(baseline_path)
historical={f'artifacts/{name}.bmp' for name in ['menu-name-entry-before-icons',
 'menu-name-entry-before-panels','menu-name-entry-frame','startup-menu-before-font','startup-menu-frame']}
images={};references=set();sources={baseline_path:sha(baseline_path)}
def visit(value):
 if isinstance(value,str) and value.lower().endswith('.bmp'):references.add(value.replace('\\','/'))
 elif isinstance(value,dict):
  p=value.get('path','')
  if isinstance(p,str) and p.lower().endswith('.bmp'):
   p=p.replace('\\','/');assert sha(p)==value['sha256'],'Stale captured image: '+p
   images[p]=value['sha256']
  for v in value.values():visit(v)
 elif isinstance(value,list):
  for v in value:visit(v)
for name in ['menu-dialog','campaign-menu','mission-menu','mission-map','load-save-menu','high-scores-menu']:
 path=f'metadata/{name}-integration.json';proof=read(path)
 assert proof['passed'] and proof['child_exe_sha256']==digest,path+' is stale'
 sources[path]=sha(path);visit(proof)
assert references==images.keys() and len(images)==19
prior={e['path'].replace('\\','/'):e['sha256'] for e in baseline}
assert len(prior)==20 and set(prior)-images.keys()==historical
changed=[p for p,h in prior.items() if sha(p)!=h];assert not changed,changed
sources.update({p:sha(p) for p in set(prior)|images.keys()})
sources['tools/record_menu_pixel_regression.py']=sha('tools/record_menu_pixel_regression.py')
report={'format':2,'generated_at':datetime.now(timezone.utc).isoformat(),'passed':True,
 'compared':20,'changed':changed,'prior_checkpoint':'70 CTests / 101,151 native comparisons',
 'prior_hashes':baseline_path,'child_exe_sha256':digest,
 'method':'All20 retained BMP hashes are unchanged;15 are recreated by the current owned-C flows. Five historical files are retention checks only. Four additional current scores captures are checked against their current integration report, without inventing prior pixel equivalence.',
 'current_owned_C_images':len(images),'current_baseline_images':sorted(set(prior)&images.keys()),
 'retained_historical_images':sorted(historical),'current_images':images,'source_hashes':sources}
(ROOT/'metadata/minimap-menu-frame-regression.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({'passed':True,'current_captures':len(images),'recreated_prior_images':15,'historical_retained_images':5,'changed':changed}))
