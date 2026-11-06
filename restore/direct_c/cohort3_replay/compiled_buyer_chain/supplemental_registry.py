"""Deferred buyer/chain binding; root supplies final images explicitly."""
from pathlib import Path
from contextlib import contextmanager
import copy,hashlib,json,re,sys
sys.dont_write_bytecode=True
OUT=Path(__file__).resolve().parent;ROOT=OUT.parents[2]


def digest(path):return hashlib.sha256(Path(path).read_bytes()).hexdigest()


def checked_inputs():
 for item in json.loads((OUT/'manifest.json').read_bytes())['files']:
  assert digest(OUT/item['path'])==item['sha256'],item['path']
 p=json.loads((OUT/'protocol.json').read_bytes())
 assert 'compiled_images'not in p,'Authoring packet must not silently acquire a PE binding'
 for file,sha in p['source_guards'].items():assert digest(ROOT/file)==sha,('source changed',file)
 for item in p['evidence_inputs']:assert digest(ROOT/item['path'])==item['sha256'],item['path']
 assert digest(ROOT/p['native']['path'])==p['native']['sha256']
 return p


def source_body(data,name):
 matches=list(re.finditer(rb'(?m)^[^\r\n;{}]*\b'+re.escape(name.encode())+rb'\([^;{}]*\)\s*\{',data))
 assert len(matches)==1,('source definition',name,len(matches))
 token=re.compile(rb'/\*[\s\S]*?\*/|//[^\r\n]*|"(?:\\.|[^"\\])*"|\x27(?:\\.|[^\x27\\])*\x27|[{}]');depth=0
 for t in token.finditer(data,matches[0].end()-1):
  if t.group()==b'{':depth+=1
  elif t.group()==b'}':
   depth-=1
   if not depth:return data[matches[0].start():t.end()]
 raise AssertionError(('unterminated definition',name))


def supplemental_members(base_registry):
 """Call only after root explicitly supplies its final hash-bound registry."""
 p=checked_inputs();assert base_registry['native']==p['native']
 sys.path[:0]=[str(ROOT/'direct_c/cohort3_replay'),str(ROOT/'direct_c')]
 from registry import symbol
 from native_test_pe import PE
 native=PE(ROOT/p['native']['path']);maps={}
 for label,image in base_registry['compiled_images'].items():
  for filekey,hashkey in [('path','sha256'),('map_path','map_sha256')]:
   assert digest(ROOT/image[filekey])==image[hashkey],(label,filekey)
  maps[label]=(ROOT/image['map_path']).read_text()
 evidence={e['requested_address']:e for e in json.loads((OUT/'native-evidence.json').read_bytes())['members']}
 rows=[]
 for c in p['members']:
  a=int(c['address'],16);n=evidence[f'{a:08x}'];raw=bytes.fromhex(''.join(i['bytes']for i in n['assembly']['items']))
  assert len(raw)==c['native_bytes']and hashlib.sha256(raw).hexdigest()==c['native_bytes_sha256']
  assert raw==native.data[a-native.base:a-native.base+len(raw)]
  body=source_body((ROOT/c['source']).read_bytes(),c['symbol']);providers={}
  for label,text in maps.items():
   named=symbol(text,c['symbol']);expected=c['expected_images'][label]
   assert bool(named)==expected,(label,c['symbol'],'retention/provider availability')
   if named:
    assert c['owner_object']in named['owner'];named=dict(named,symbol=c['symbol'])
   canonical=None
   if c['canonical_symbol']is not None:
    canonical=symbol(text,c['canonical_symbol']);assert canonical and c['owner_object']in canonical['owner']
    canonical=dict(canonical,symbol=c['canonical_symbol'])
   else:assert symbol(text,f'FUN_{a:08x}')is None
   providers[label]=dict(named=named,canonical=canonical)
  rows.append(dict(address=hex(a),name=c['symbol'],source=c['source'],source_definition_symbol=c['symbol'],
   algorithm_source=c['algorithm_source'],source_sha256=digest(ROOT/c['source']),body_sha256=hashlib.sha256(body).hexdigest(),
   native_ranges=[dict(start=hex(a),end=hex(a+len(raw)-1))],native_ret_immediates=[0],
   logical_signature=c['logical_signature'],providers=providers,rule=None,
   case_status='private_authored_not_executed',supplemental_comparison_only=True,new_source_credit=0,new_entry_credit=0))
 return rows


def extend_registry(base):
 additions=supplemental_members(base);original={int(m['address'],16)for m in base['members']}
 assert not original.intersection(int(m['address'],16)for m in additions)
 result=copy.deepcopy(base);result['members'].extend(additions)
 result['source_entries']=base['source_entries'];result['comparison_entries']=len(result['members'])
 result['buyer_chain_supplemental']=dict(members=4,new_source_credit=0,new_entry_credit=0,manifest_sha256=digest(OUT/'manifest.json'))
 return result


@contextmanager
def temporary_registry(replay_module,base_registry=None):
 original=replay_module.load;base=original(True)if base_registry is None else base_registry
 replay_module.load=lambda require_retained=True:extend_registry(original(require_retained))
 try:yield extend_registry(base)
 finally:replay_module.load=original
