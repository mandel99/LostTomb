from pathlib import Path
import json
ROOT=next(p for p in Path(__file__).resolve().parents if(p/'runtime/actor_civic.c').is_file())
def scopes():
 p=ROOT/'metadata/actor-cohort-2.json';v=json.loads(p.read_bytes());out={}
 for m in v['members']:
  out[int(m['address'],16)]=dict(kind=m.get('registry_kind'),direct_children=[])
 return out,{p}
