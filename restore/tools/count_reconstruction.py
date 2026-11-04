"""Structural reconstruction inventory, not proof of behavior or test coverage.

Resolve canonical entries using the current link MAP or their manifest source,
then inspect their own C body and providers. Count native addresses once. Calls to other
FUN_* entries do not certify those callees. Unknown/macro definitions stay
unclassified instead of increasing the ordinary-C count.
"""
from pathlib import Path
from collections import Counter,defaultdict
from datetime import datetime,timezone
import argparse,csv,hashlib,json,re

root=Path(__file__).resolve().parents[1]
parser=argparse.ArgumentParser(description=__doc__)
parser.add_argument('--source-root',type=Path,default=root)
parser.add_argument('--map',type=Path,default=root/'build/c-only/Release/PharaohRestoreC.map')
parser.add_argument('--output',type=Path,default=root/'build/reconstruction-count.json')
parser.add_argument('--native-catalogue',type=Path,default=root/'metadata/compiler-unwind-catalogue.json')
args=parser.parse_args();src=args.source_root.resolve()
units=json.loads((src/'units.json').read_text());mapping=args.map.read_text()
owners={m[1]:m[2] for m in re.finditer(r'\s_(FUN_[0-9a-fA-F]{8})\s+[0-9a-fA-F]{8}\s+f\s+pharaoh_restored:([^\s]+)\.obj',mapping)}
mask=re.compile(r'/\*.*?\*/|//[^\n]*|"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\'',re.S)
head=re.compile(r'^[ \t]*[A-Za-z_][A-Za-z0-9_ \t*]*[ \t*]([A-Za-z_]\w*)[ \t]*\([^;{}]*\)\s*\{',re.M)
definitions=defaultdict(list);file_hashes={};generated_files=set()
for directory,glob in [('runtime','*.c'),('src','*.c'),('include','*.h')]:
 for path in sorted((src/directory).glob(glob)):
  text=path.read_text(encoding='utf-8-sig');clean=mask.sub(lambda m:' '*len(m[0]),text);rel=path.relative_to(src).as_posix()
  clean=re.sub(r'\b[A-Z_][A-Z0-9_]*\((FUN_[0-9a-fA-F]{8})\)',lambda m:m[1]+' '*(len(m[0])-len(m[1])),clean)
  # Attributes precede return types; retain positions for source line evidence.
  clean=re.sub(r'\b__declspec\s*\([^)]*\)',lambda m:' '*len(m[0]),clean)
  file_hashes[rel]=hashlib.sha256(path.read_bytes()).hexdigest()
  if '#include "restore_types.h"' in text:generated_files.add(rel)
  for m in head.finditer(clean):
   p=m.end();depth=1
   while depth and p<len(clean):
    depth+=(clean[p]=='{')-(clean[p]=='}');p+=1
   definitions[m[1]].append((rel,path.stem,text.count('\n',0,m.start())+1,clean[m.end():p-1]))

model=re.compile(r'\b(?:invoke|invoke[0-9]+|mi_alu|mi_shift|mi_imul|restore_unresolved_return)\s*\(|\b(?:f|frame)(?:->|\.)r\s*\[|\b(?:f|frame)->boundary\b')
decompiled=re.compile(r'\b(?:in_(?:EAX|ECX|EDX|EBX|ESI|EDI|EBP|ESP|stack_)[A-Za-z0-9_]*|unaff_\w+|extraout_\w+|[a-zA-Z]+Var\d+|local_[0-9a-f]+)\b')
calls=re.compile(r'\b([A-Za-z_]\w*)\s*\(')

def classify(d,seen=None):
 seen=set() if seen is None else seen
 key=d[:3]
 if key in seen:return 'ordinary_c_structure'
 seen.add(key);body=d[3]
 if model.search(body):return 'instruction_model_or_frame_bridge'
 if re.search(r'\b(?:__asm|asm)\b',body):return 'assembly_boundary'
 if decompiled.search(body):return 'decompiler_style_or_unresolved'
 if d[0] in generated_files:return 'generated_decompiler_source'
 children=[]
 for name in set(calls.findall(body)):
  if name.startswith('FUN_'):continue
  found=definitions.get(name,[]);local=[x for x in found if x[0]==d[0]]
  if len(local)==1:children.append(classify(local[0],seen))
  elif name.startswith('restore_'):
   if len(found)==1:children.append(classify(found[0],seen))
   else:children.append('provider_definition_unknown')
 for status in ['instruction_model_or_frame_bridge','assembly_boundary','decompiler_style_or_unresolved','generated_decompiler_source','provider_definition_unknown']:
  if status in children:return status
 return 'ordinary_c_structure'

native={}
if args.native_catalogue.exists():
 native_input=json.loads(args.native_catalogue.read_text())
 for address in native_input.get('compiler_unwind_addresses','').split():
  assert re.fullmatch(r'0x[0-9a-fA-F]+',address),address
  native[int(address,16)]='Unwind@%08x'%int(address,16)
 for item in native_input.get('procedures',[]):
  if re.fullmatch(r'0x[0-9a-fA-F]+',item['address']):native[int(item['address'],16)]=item['name']
rows=[]
for u in units:
 name=u['name'];owner=owners.get(name);choices=[d for d in definitions.get(name,[]) if d[1]==owner] if owner else []
 if not owner:
  candidates=definitions.get(name,[]);choices=[d for d in candidates if d[0]==u['file']]
  if not choices and len(candidates)==1:choices=candidates
 if len(choices)==1:
  d=choices[0];status=classify(d);file,line=d[0],d[2]
 else:
  status='canonical_definition_unknown';file='';line=0
 native_name=native.get(int(u['provenance']['address'],16),'')
 rows.append(dict(address=u['provenance']['address'],name=name,native_name=native_name,compiler_unwind=native_name.startswith('Unwind@'),status=status,source=file,line=line,linked_object=owner or ''))
counts=Counter(r['status'] for r in rows);linked=len(owners.keys()&{u['name'] for u in units})
report=dict(catalogued_functions=len(units),linked_catalogued_functions=linked,status_counts=dict(counts),
 ordinary_C_percent_of_catalogue=round(100*counts['ordinary_c_structure']/len(units),2),
 ordinary_C_percent_of_linked=round(100*sum(r['status']=='ordinary_c_structure' and bool(r['linked_object']) for r in rows)/linked,2),
 source_root=str(src),map_path=str(args.map.resolve()),map_sha256=hashlib.sha256(args.map.read_bytes()).hexdigest(),
 limitations=['Structural classification only; ordinary C is not proof of correctness, completeness, native equivalence or current validation.',
  'Counts original catalogue entries once; shared helpers, duplicate model copies and added runtime helpers do not inflate the denominator.',
  'Resolve the canonical link object and its own body/providers. Calls to other original FUN_* entries are not treated as proof of their algorithms.',
  'Unknown/macro-generated providers and canonical definitions are separate. Original CRT/runtime entries remain in the catalogue.'],
 unknown_examples=[r for r in rows if 'unknown' in r['status']][:20])
report.update(generated_at=datetime.now(timezone.utc).isoformat(),manifest_sha256=hashlib.sha256((src/'units.json').read_bytes()).hexdigest(),
 tool_sha256=hashlib.sha256(Path(__file__).read_bytes()).hexdigest())
if native:
 non_unwind=[r for r in rows if not r['compiler_unwind']];other_counts=Counter(r['status'] for r in non_unwind)
 report.update(compiler_unwind_functions=len(rows)-len(non_unwind),non_unwind_functions=len(non_unwind),non_unwind_status_counts=dict(other_counts),
  ordinary_C_percent_without_unwind=round(100*other_counts['ordinary_c_structure']/len(non_unwind),2),
  native_catalogue_evidence=json.loads(args.native_catalogue.read_text()).get('evidence_id'))
 report['native_catalogue_sha256']=hashlib.sha256(args.native_catalogue.read_bytes()).hexdigest()
report['source_snapshot_sha256']=hashlib.sha256(json.dumps(file_hashes,sort_keys=True).encode()).hexdigest()
report['limitations']+=['Generated sources that still include restore_types.h are not counted as reconstructed ordinary C, even if a tiny body is syntactically simple.',
 'An absent canonical MAP symbol does not mean the algorithm is missing: unused entries or renamed providers can be omitted. Those rows fall back to their manifest/unique source definition.']
args.output.parent.mkdir(parents=True,exist_ok=True);args.output.write_text(json.dumps(report,indent=2)+'\n')
with args.output.with_suffix('.csv').open('w',newline='',encoding='utf-8-sig') as f:
 writer=csv.DictWriter(f,fieldnames=list(rows[0]));writer.writeheader();writer.writerows(rows)
print(json.dumps({k:report[k] for k in ['catalogued_functions','linked_catalogued_functions','status_counts','ordinary_C_percent_of_catalogue','compiler_unwind_functions','non_unwind_functions','non_unwind_status_counts','ordinary_C_percent_without_unwind'] if k in report},indent=2))
