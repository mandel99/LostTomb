"""Shared offline production matrix; preserves existing C and real frames."""
from pathlib import Path
import argparse,datetime,json,sys,hashlib
sys.dont_write_bytecode=True
OUT=Path(__file__).resolve().parent;R=OUT.parents[2]
sys.path[:0]=[str(OUT),str(R/'direct_c/cohort3_replay'),str(R/'direct_c')]
from compiled_final6 import supplemental_registry as binding
from current_matrix.verify import audit
import replay,production_cases
from registry import load,save

def checks(cases,report):
 result=audit(cases,report);visits=0
 for row in report['cases_detail']:
  c=next(c for c in cases if c['id']==row['id'])
  for route in row['routes']:
   for token,registers in c.get('expected_endpoint_registers',{}).items():
    assert [e['registers']for e in route['events']if e['token']==token]==registers
   visits+=sum('live_model_frame'in e for e in route['physical_endpoint_arguments'])
 assert visits==126,('actual frame endpoint visits',visits)
 result.update(live_frame_endpoint_visits=visits,endpoint_receivers_checked=True)
 return result

def main():
 ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--run',action='store_true');args=ap.parse_args()
 for item in json.loads((OUT/'manifest.json').read_bytes())['files']:
  assert hashlib.sha256((OUT/item['path']).read_bytes()).hexdigest()==item['sha256'],item['path']
 protocol=json.loads((OUT/'protocol.json').read_bytes())
 for f,sha in protocol['source_guards'].items():assert hashlib.sha256((R/f).read_bytes()).hexdigest()==sha,f
 base=load(True);assert base['compiled_images']==protocol['compiled_images']
 from native_test_pe import PE
 native=PE(R/base['native']['path'])
 for record in json.loads((OUT/'expanded-native-evidence.json').read_bytes())['records']:
  for i in record['member']['assembly']['items']:
   at=int(i['address'],16)-native.base;raw=bytes.fromhex(i['bytes']);assert native.data[at:at+len(raw)]==raw
 with binding.temporary_registry(replay,base)as registry:
  cases=production_cases.cases(registry);assert len(cases)==26
  if not args.run:
   print(json.dumps(dict(cases=26,invocations=182,execution=False)));return
  directory=R/'build/actor-animal-following/cohort7-production-public-runs'/datetime.datetime.now(datetime.timezone.utc).strftime('%Y%m%d-%H%M%S-%f')
  directory.mkdir(parents=True,exist_ok=False);save(directory/'registry.json',registry)
  result=replay.execute(registry,cases,directory);save(directory/'authored-checks.json',checks(cases,result))
 print(json.dumps(dict(result=str(directory/'result.json'),cases=26,invocations=182,failed_cases=result['failed_cases'])),flush=True)

if __name__=='__main__':main()
