"""Shared current-image civic graph matrix; no host executable launch."""
from pathlib import Path
import argparse,datetime,json,sys,hashlib
sys.dont_write_bytecode=True
OUT=Path(__file__).resolve().parent;R=OUT.parents[2]
sys.path[:0]=[str(OUT),str(R/'direct_c/cohort3_replay'),str(R/'direct_c')]
from compiled_final6 import supplemental_registry as binding
from current_matrix.verify import audit
import civic_cases,replay
from registry import load,save

def checks(cases,report):
 result=audit(cases,report)
 visits=sum('live_model_frame'in e for row in report['cases_detail']for route in row['routes']for e in route['physical_endpoint_arguments'])
 assert visits==12
 result['live_frame_endpoint_visits']=visits
 return result

def main():
 ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--run',action='store_true');args=ap.parse_args()
 for item in json.loads((OUT/'manifest.json').read_bytes())['files']:
  assert hashlib.sha256((OUT/item['path']).read_bytes()).hexdigest()==item['sha256'],item['path']
 p=json.loads((OUT/'protocol.json').read_bytes())
 for n,sha in p['source_guards'].items():assert hashlib.sha256((R/n).read_bytes()).hexdigest()==sha,n
 assert hashlib.sha256((OUT/'civic_cases.py').read_bytes()).hexdigest()==p['authored_recipe_sha256']
 base=load(True);assert base['compiled_images']==p['compiled_images']
 from native_test_pe import PE
 native=PE(R/base['native']['path'])
 for key in ['native_evidence','predicate_evidence']:
  ev=p[key];path=R/ev['path'];assert hashlib.sha256(path.read_bytes()).hexdigest()==ev['sha256']
  data=json.loads(path.read_bytes());member=next(m for m in data.get('members',data.get('entries',[]))if m['requested_address']==ev['address'])
  for i in member['assembly']['items']:
   at=int(i['address'],16)-native.base;raw=bytes.fromhex(i['bytes']);assert native.data[at:at+len(raw)]==raw
 for address,payload in p['ROM_inputs'].items():
  at=int(address,16)-native.base;raw=bytes.fromhex(payload);assert native.data[at:at+len(raw)]==raw
 with binding.temporary_registry(replay,base)as registry:
  cases=civic_cases.cases(registry);assert len(cases)==43
  if not args.run:
   print(json.dumps(dict(cases=43,invocations=301,execution=False)));return
  directory=R/'build/actor-animal-following/cohort7-civic-main-public-runs'/datetime.datetime.now(datetime.timezone.utc).strftime('%Y%m%d-%H%M%S-%f')
  directory.mkdir(parents=True,exist_ok=False);save(directory/'registry.json',registry)
  report=replay.execute(registry,cases,directory);save(directory/'authored-checks.json',checks(cases,report))
 print(json.dumps(dict(result=str(directory/'result.json'),cases=43,invocations=301,failed_cases=report['failed_cases'])),flush=True)

if __name__=='__main__':main()
