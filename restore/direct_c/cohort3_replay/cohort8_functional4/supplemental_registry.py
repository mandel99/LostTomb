"""Explicit comparison-only current-profile extension; no public ledger edits."""
from pathlib import Path
from contextlib import contextmanager
import copy,hashlib,json,sys
sys.dont_write_bytecode=True
OUT=Path(__file__).resolve().parent;ROOT=OUT.parents[2]


def digest(path):return hashlib.sha256(Path(path).read_bytes()).hexdigest()


def checked_inputs():
 for row in json.loads((OUT/'manifest.json').read_bytes())['files']:
  assert digest(OUT/row['path'])==row['sha256'],row['path']
 p=json.loads((OUT/'protocol.json').read_bytes())
 for path,sha in p['source_guards'].items():assert digest(ROOT/path)==sha,path
 for row in p['evidence_inputs']:assert digest(ROOT/row['path'])==row['sha256'],row['path']
 assert digest(ROOT/p['native']['path'])==p['native']['sha256']
 for label,image in p['compiled_images'].items():
  for key,hashkey in [('path','sha256'),('map_path','map_sha256')]:assert digest(ROOT/image[key])==image[hashkey],(label,key)
 current=json.loads((ROOT/p['image_profile_input']['path']).read_bytes())['compiled_images']
 assert current==p['compiled_images'],'Explicit profile rebind required after image change'
 return p


def supplemental_members(base=None):
 p=checked_inputs()
 sys.path[:0]=[str(ROOT/'direct_c/cohort3_replay'),str(ROOT/'direct_c')]
 from registry import symbol
 from native_test_pe import PE
 if base is not None:
  assert base['native']==p['native']and base['compiled_images']==p['compiled_images']
 native=PE(ROOT/p['native']['path']);maps={k:(ROOT/v['map_path']).read_text()for k,v in p['compiled_images'].items()}
 rows=[]
 for contract in p['members']:
  a=int(contract['address'],16);raw=native.data[a-native.base:contract['native_end_inclusive']-native.base+1]
  assert hashlib.sha256(raw).hexdigest()==contract['native_contiguous_scope_sha256']
  providers={}
  for label,text in maps.items():
   hit=symbol(text,contract['symbol']);expected=contract['applicable_canonical_images'][label]
   assert bool(hit)==expected,(label,contract['symbol'],'availability')
   if hit:
    assert contract['owner_object']in hit['owner'];hit=dict(hit,symbol=contract['symbol'])
   providers[label]=dict(named=copy.deepcopy(hit)if contract['typed_named_API']else None,canonical=hit)
  rows.append(dict(address=contract['address'],name=contract['symbol'],source=contract['source'],
   source_definition_symbol=contract['symbol'],source_sha256=digest(ROOT/contract['source']),
   body_sha256=digest(ROOT/contract['source']),body_hash_scope='Exact source; native bridge canonical is macro-generated.',
   algorithm_source=contract['algorithm_source'],providers=providers,rule=None,
   native_ranges=[dict(start=hex(a),end=hex(contract['native_end_inclusive']))],native_ret_immediates=[0],
   logical_signature=contract['logical_signature'],case_status='authored_not_executed',
   supplemental_comparison_only=True,new_source_credit=0,new_entry_credit=0))
 return rows


def extend_registry(base):
 additions=supplemental_members(base);existing={int(m['address'],16)for m in base['members']}
 assert not existing.intersection(int(m['address'],16)for m in additions)
 result=copy.deepcopy(base);result['members'].extend(additions)
 result['source_entries']=base['source_entries'];result['comparison_entries']=len(result['members'])
 result['functional4_comparison_only']=dict(entries=4,new_source_credit=0,manifest_sha256=digest(OUT/'manifest.json'))
 return result


@contextmanager
def temporary_registry(replay_module,base_registry=None):
 original=replay_module.load;base=original(True)if base_registry is None else base_registry
 replay_module.load=lambda require_retained=True:extend_registry(original(require_retained))
 try:yield extend_registry(base)
 finally:replay_module.load=original
