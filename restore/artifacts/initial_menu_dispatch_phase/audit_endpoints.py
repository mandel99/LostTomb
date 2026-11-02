"""Preserve endpoint ABI evidence without asserting downstream equivalence."""
from pathlib import Path
import hashlib,json,re
ROOT=Path(__file__).resolve().parents[2];HERE=Path(__file__).resolve().parent
native=json.loads((HERE/'native-provider.json').read_text());targets={int(i['instruction'].split()[-1],16) for i in native['assembly']['items'] if i['instruction'].startswith('CALL ')}
cache={}
for path in (ROOT/'metadata/native-function-audit-cache').glob('batch-*.json'):
 for entry in json.loads(path.read_text())['entries']:
  if int(entry['requested_address'],16) in targets:cache[int(entry['requested_address'],16)]=(entry,path)
assert set(cache)==targets
sources={}
for path in list((ROOT/'src').glob('unit_*.c'))+list((ROOT/'runtime').glob('*.c')):
 text=path.read_text()
 for token in targets:
  matches=list(re.finditer(r'(?m)^(?!extern\b)([^\n;{}]*\bFUN_'+f'{token:08x}'+r'\([^;{}]*\))\s*\{',text))
  for m in matches:sources.setdefault(token,[]).append({'source':path.relative_to(ROOT).as_posix(),'signature':m[1].strip(),'source_sha256':hashlib.sha256(path.read_bytes()).hexdigest(),'explicit_register_placeholders':sorted(set(re.findall(r'\b(?:in_E\w+|unaff_E\w+|extraout_E\w+)\b',text[m.end():])))})
records=[]
for token in sorted(targets):
 e,path=cache[token];assert e['status']=='exact_ghidra_function' and not e['assembly']['has_more']
 records.append({'entry':f'{token:08x}','native_info':e['info'],'native_instruction_count':e['assembly']['total'],'native_return_sites':[i for i in e['assembly']['items'] if i['instruction'].startswith('RET')],'C_bindings':sources.get(token,[]),'native_evidence':path.relative_to(ROOT).as_posix(),'native_evidence_sha256':hashlib.sha256(path.read_bytes()).hexdigest()})
out={'format':1,'native_sha256':native['native_sha256'],'whole_downstream_algorithms_proved':False,'limitations':['Ghidra signatures and generated C placeholder scans are ABI audit leads; absence of a placeholder alone does not prove no implicit register input. Native actual entry/CFG dataflow and caller role must decide ambiguous entries.','Only50 named boundary calls and their exact native stack argument setup are covered by the dispatcher comparator.'], 'entries':records}
(HERE/'endpoint-audit.json').write_text(json.dumps(out,indent=2)+'\n')
print(json.dumps({'entries':len(records),'with_explicit_register_placeholders':[r['entry'] for r in records if any(b['explicit_register_placeholders'] for b in r['C_bindings'])],'interesting':[{k:r[k] for k in ['entry','native_info','C_bindings']} for r in records if r['entry'] in ['00520ab0','0055ad30','00530d70']]}))
