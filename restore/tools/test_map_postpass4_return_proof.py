"""Reject stale, private, partial or incomplete substitutes for the new guard proof."""
from pathlib import Path
import copy,json
from map_postpass4_return_proof import verified_postpass4_supersession,PROOF
ROOT=Path(__file__).resolve().parents[1]
proof=json.loads((ROOT/PROOF).read_text())
assert verified_postpass4_supersession(ROOT)['whole_group_instruction_coverage']==[384,384]
def edit(path,value):
 def change(p):
  for key in path[:-1]:p=p[key]
  p[path[-1]]=value
 return change
negative=[
 ('private',edit(['production'],False)),('wrong_count',edit(['checked'],4673)),
 ('partial',edit(['complete_cases'],4673)),('missing_variant_comparison',edit(['comparisons'],18695)),
 ('wrong_original',edit(['native_sha256'],'0'*64)),
 ('caller_instruction_gap',edit(['coverage','0047e870'],[197,198])),
 ('helper_instruction_gap',edit(['coverage','0047eb60'],[84,85])),
 ('missing_counter_wrap',edit(['counter_changes','wrap'],103)),
 ('missing_branch',edit(['branches',next(iter(proof['branches']))],0)),
 ('private_binary',edit(['variants','O2_plain','path'],'artifacts/map_refresh_postpass4_phase/build/O2/map_post4_plain.exe')),
 ('stale_binary',edit(['variants','O2_trace','sha256'],'0'*64)),
 ('stale_source',edit(['source_hashes','src/unit_01152.c'],'0'*64)),
 ('wrong_corpus',edit(['corpus_sha256'],'0'*64)),
 ('missing_source',lambda p:p['source_hashes'].pop('src/unit_01153.c')),
 ('missing_variant',lambda p:p['variants'].pop('Debug_plain'))]
rejected=[]
for name,change in negative:
 candidate=copy.deepcopy(proof);change(candidate)
 try:verified_postpass4_supersession(ROOT,candidate)
 except (AssertionError,KeyError):rejected.append(name)
 else:raise AssertionError('Unqualified guard retirement accepted: '+name)
assert len(rejected)==15
print(json.dumps({'passed':True,'positive_proof':1,'unqualified_reports_rejected':len(rejected),'cases':rejected,'product_sources_written':False}))
