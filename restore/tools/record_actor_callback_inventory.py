"""Retain the actual actor callback reconstruction gap without counting stubs as C."""
from pathlib import Path
from datetime import datetime,timezone
import hashlib,json,re
ROOT=Path(__file__).resolve().parent.parent
def sha(p):return hashlib.sha256((ROOT/p).read_bytes()).hexdigest()
def read(p):return json.loads((ROOT/p).read_text(encoding='utf-8-sig'))
table=read('metadata/actor-callback-table-native.json')
assert len(table['entries'])==110
registry=(ROOT/'runtime/actor_callbacks.c').read_text()
implemented={int(a,16):name for a,name in re.findall(r'case 0x([0-9a-f]+)u:return (\w+);',registry)}
contracts=read('metadata/actor-callback-c-contracts.json')['functions']
expected={0x51f780:'empty_callback',**{int(c['address'],16):c['function'] for c in contracts}}
assert implemented==expected,'Registry differs from reviewed controller manifest'
proofs={};extra_sources=set()
for c in contracts:
 p=c['proof'];proof=proofs.setdefault(p,read(p))
 assert proof['passed'] and proof['checked']==c['cases'],p
 for source,digest in {**proof['source_hashes'],**proof.get('native_exports',{})}.items():
  assert sha(source)==digest,p+' stale input: '+source
 assert sha(c['source'])==proof['source_hashes'][c['source']],c['source']
 assert re.search(r'\bvoid\s+'+re.escape(c['function'])+r'\s*\(\s*void\s*\)',(ROOT/c['source']).read_text()),c['function']
 extra_sources.update([p,c['source']])
assert read('metadata/actor-empty-callback-native.json')['bytes']['hex']=='c3'
entries=[]
for x in table['entries']:
 address=int(x['address'],16)
 entries.append({**x,'implemented_C':address in implemented,'C_function':implemented.get(address),'deep_helpers_proven':False})
missing=sorted({x['address'] for x in entries if not x['implemented_C']},key=lambda x:int(x,16))
assert len({x['address'] for x in entries})==102 and len(missing)+len(implemented)==102
sources=['tools/record_actor_callback_inventory.py','metadata/actor-callback-table-native.json','metadata/actor-empty-callback-native.json','include/actor_update.h','runtime/actor_callbacks.c','runtime/actor_prey.c']
sources+=sorted(extra_sources|{'metadata/actor-callback-c-contracts.json'})
report={'format':2,'generated_at':datetime.now(timezone.utc).isoformat(),'native_sha256':table['native_sha256'],'type_entries':110,'distinct_callback_bodies':102,'implemented_distinct_bodies':len(implemented),'missing_distinct_bodies':len(missing),'all_callbacks_complete':not missing,'entries':entries,'missing_addresses':missing,'controller_proofs':{p:sha(p) for p in proofs},'limitations':['Routed C controllers are declared in the reviewed manifest. Unknown targets abort explicitly. No original machine code is executed.','Each controller retains its documented movement, population, sound or pathfinding boundaries. A routed entry does not establish complete actor gameplay.'],'source_hashes':{p:sha(p) for p in sources}}
(ROOT/'metadata/actor-callback-inventory.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({k:report[k] for k in ['type_entries','distinct_callback_bodies','implemented_distinct_bodies','missing_distinct_bodies','all_callbacks_complete']}))
