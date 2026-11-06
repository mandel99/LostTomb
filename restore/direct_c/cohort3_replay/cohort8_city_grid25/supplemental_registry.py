"""Hash-bound comparison-only canonical25; never changes the source ledger."""
from pathlib import Path
from contextlib import contextmanager
import copy,hashlib,json,sys
sys.dont_write_bytecode=True
OUT=Path(__file__).resolve().parent;ROOT=OUT.parents[2]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def checked_inputs():
 for row in json.loads((OUT/'manifest.json').read_bytes())['files']:assert digest(OUT/row['path'])==row['sha256'],row['path']
 p=json.loads((OUT/'protocol.json').read_bytes())
 for f,sha in p['source_guards'].items():assert digest(ROOT/f)==sha,f
 for f in p['evidence_inputs']:assert digest(ROOT/f['path'])==f['sha256'],f['path']
 assert digest(ROOT/p['native']['path'])==p['native']['sha256']
 for label,image in p['compiled_images'].items():
  for key,h in [('path','sha256'),('map_path','map_sha256')]:assert digest(ROOT/image[key])==image[h],(label,key)
 assert json.loads((ROOT/p['image_profile_path']).read_bytes())['compiled_images']==p['compiled_images'],'Explicit derived profile binding required'
 return p
def supplemental_members(base=None):
 p=checked_inputs();sys.path[:0]=[str(ROOT/'direct_c/cohort3_replay'),str(ROOT/'direct_c')]
 from native_test_pe import PE
 from registry import symbol
 if base is not None:assert base['native']==p['native']and base['compiled_images']==p['compiled_images']
 pe=PE(ROOT/p['native']['path']);maps={k:(ROOT/v['map_path']).read_text()for k,v in p['compiled_images'].items()};rows=[]
 for m in p['members']:
  a=int(m['address'],16);assert hashlib.sha256(pe.data[a-pe.base:m['native_end_inclusive']-pe.base+1]).hexdigest()==m['native_contiguous_scope_sha256']
  providers={}
  for label,text in maps.items():
   hit=symbol(text,m['symbol']);assert hit==m['providers'][label]
   if hit:assert m['owner_object']in hit['owner'];hit=dict(hit,symbol=m['symbol'])
   providers[label]=dict(named=None,canonical=hit)
  rows.append(dict(address=m['address'],name=m['symbol'],source=m['source'],source_definition_symbol=m['symbol'],source_sha256=m['source_sha256'],body_sha256=m['source_sha256'],body_hash_scope='Exact existing C source; no new algorithm.',algorithm_source=m['source'],
   providers=providers,rule=None,native_ranges=[dict(start=hex(a),end=hex(m['native_end_inclusive']))],native_ret_immediates=[m['native_cleanup']],logical_signature=m['logical_signature'],case_status='authored_not_executed',supplemental_comparison_only=True,new_source_credit=0,new_entry_credit=0))
 return rows
def extend_registry(base):
 rows=supplemental_members(base);old={int(m['address'],16)for m in base['members']};assert not old.intersection(int(m['address'],16)for m in rows)
 result=copy.deepcopy(base);result['members'].extend(rows);result['comparison_entries']=len(result['members']);result['source_entries']=base['source_entries'];result['city_grid25_comparison_only']=True;return result
@contextmanager
def temporary_registry(replay_module):
 original=replay_module.load;replay_module.load=lambda require_retained=True:extend_registry(original(require_retained))
 try:yield replay_module.load(True)
 finally:replay_module.load=original
