"""Pure source/native/recipe audit; optional audit of an already produced report."""
from pathlib import Path
import argparse,hashlib,json,sys
sys.dont_write_bytecode=True
OUT=Path(__file__).resolve().parent;ROOT=OUT.parents[2]
sys.path[:0]=[str(OUT),str(ROOT/'direct_c/cohort3_replay'),str(ROOT/'direct_c')]
import hunter_cases as recipes


def digest(path):return hashlib.sha256(Path(path).read_bytes()).hexdigest()
def packed(value):
 return json.dumps(value,sort_keys=True,separators=(',',':'),default=lambda b:{'bytes_hex':b.hex()}).encode()
def commitment(case):return hashlib.sha256(packed(case)).hexdigest()


def checked_cases():
 protocol=json.loads((OUT/'protocol.json').read_bytes())
 assert 'compiled_images'not in protocol
 for f in json.loads((OUT/'manifest.json').read_bytes())['files']:
  assert digest(OUT/f['path'])==f['sha256'],f['path']
 for path,sha in protocol['source_guards'].items():assert digest(ROOT/path)==sha,path
 from registry import symbol
 ownership=json.loads((OUT/'owner-observations.json').read_bytes())
 for image in ownership['images'].values():
  text=(ROOT/image['map_path']).read_text()
  for name,owner in protocol['expected_provider_owners'].items():
   hit=symbol(text,name);assert hit and owner in hit['owner'],('MAP owner',name)
 header=(ROOT/'include/actor_civic.h').read_bytes()
 assert b'struct restore_civic_frame {uint32_t r[8],flags;restore_civic_call boundary;};'in header
 assert b'RAW(004b45c0);'in(ROOT/'runtime/actor_civic.c').read_bytes()
 assert b'case 0x4a5da0u:FUN_004a5da0((int32_t)A(1),A(2));break;'in(ROOT/'runtime/actor_walking_facades.c').read_bytes()
 for f in protocol['evidence_inputs']:assert digest(ROOT/f['path'])==f['sha256'],f['path']
 assert digest(ROOT/protocol['native']['path'])==protocol['native']['sha256']
 from native_test_pe import PE
 native=PE(ROOT/protocol['native']['path'])
 ev=json.loads((OUT/'native-evidence.json').read_bytes())
 for m in ev['members']:
  assert m['status']=='exact_ghidra_function'and not m['assembly']['has_more']
  for i in m['assembly']['items']:
   at=int(i['address'],16)-native.base;raw=bytes.fromhex(i['bytes'])
   assert native.data[at:at+len(raw)]==raw,i['address']
 table=ev['render_jump_table'];at=int(table['address'],16)-native.base
 assert native.data[at:at+48]==bytes.fromhex(table['bytes_hex'])
 cases=recipes.cases();assert cases==recipes.cases()and len(cases)==30
 assert len({c['id']for c in cases})==30
 assert recipes.cases(dict(members=[]))==[]
 saved=json.loads((OUT/'case-commitments.json').read_bytes())['cases']
 assert {c['id']:commitment(c)for c in cases}==saved
 for c in cases:
  assert c['entry']==0x4a2430 and c['native_cleanup']==c['canonical_cleanup']==c['named_cleanup']==0
  assert c['arguments']==c['named_arguments']==c['canonical_arguments']==[]and c['result_mask']is None
  assert c['canonical_registers_and_flags']is False
  assert len(c['authored_endpoint_order'])==len(c['authored_endpoint_observations'])==sum(c['expected_endpoint_counts'].values())
  assert c['watched']==c['write_ranges']and c['authored_final_memory']
  assert [(at,len(bytes.fromhex(data)))for at,data in c['authored_final_memory']]==c['watched']
  for e in c['endpoints']:
   token=e['token'];count=c['expected_endpoint_counts'][token]
   assert e['result_kind']=='void'and e['native_cleanup']==e['compiled_cleanup']==0
   assert len(e['responses'])==len(c['expected_endpoint_arguments'][token])==c['authored_endpoint_order'].count(token)==count
   assert len(e['observe'])==len(c['authored_endpoint_observations'][0])
   if token=='projectile':
    assert e['compiled_frame_symbols']==['restore_civic_C_004b45c0']
    assert e['compiled_frame_dispatcher']=='restore_actor_civic_dispatch'
    assert e['compiled_frame_return_sites']==[0x4a2925]
   else:assert 'compiled_frame_symbols'not in e
  inputs=dict(c['stores'])
  if any('004a2320'in s for s in c['actual_children']):
   for id_ in range(3,2000):assert inputs[recipes.actor(id_)+14]==b'\0'
   assert all(not any(lo<=recipes.actor(id_)+14<lo+n for lo,n in c['write_ranges'])for id_ in range(3,2000))
 synthetic=dict(members=[dict(address='0x004a2430',providers={k:dict(named=True,canonical=True)for k in('game','diagnostic','retained')})])
 assert sum(c['expected_invocations']for c in recipes.cases(synthetic))==210
 return protocol,cases


def audit(cases,report):
 rows={r['id']:r for r in report['cases_detail']};routes=events=model_visits=0
 for c in cases:
  row=rows[c['id']];assert row['passed'],c['id']
  assert len(row['routes'])==7,(c['id'],'routes')
  for route in row['routes']:
   assert not route['errors'],(c['id'],route['errors'])
   assert [e['token']for e in route['events']]==c['authored_endpoint_order']
   assert [e['observed']for e in route['events']]==json.loads(json.dumps(c['authored_endpoint_observations']))
   for token,wanted in c['expected_endpoint_arguments'].items():
    assert [e['arguments']for e in route['events']if e['token']==token]==wanted
   for event in route['physical_endpoint_arguments']:
    if event['token']=='projectile'and route['image']!='native':
     frame=event['live_model_frame']
     assert frame['return_site']==0x4a2925 and len(frame['registers'])==8
     assert frame['stack_pointer']==frame['registers'][4]
     assert event['physical_C_arguments']==[frame['address']]
     model_visits+=1
   routes+=1;events+=len(route['events'])
 assert routes==210 and model_visits==12
 return dict(routes=routes,endpoint_events=events,actual_live_model_visits=model_visits)


def main():
 ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--report',type=Path);ap.add_argument('--output',type=Path)
 args=ap.parse_args();p,cases=checked_cases()
 result=dict(schema='cohort7-hunter-successful-home-pure-guard-v1',hashes_valid=True,cases=len(cases),
  entries=1,expected_invocations_after_explicit_root_binding=210,replay_invoked=False,
  build_invoked=False,host_launched=False,current_image_binding=False,new_source_credit=0,new_entry_credit=0,
  confirmed_C_gaps=0)
 if args.report:result['archived_report_audit']=dict(report=str(args.report),sha256=digest(args.report),**audit(cases,json.loads(args.report.read_bytes())))
 if args.output:args.output.write_bytes((json.dumps(result,indent=2)+'\n').encode())
 print(json.dumps(result))


if __name__=='__main__':main()
