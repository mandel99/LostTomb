"""Inventory every procedure without equating compilation or test inclusion with review."""
from pathlib import Path
from datetime import datetime,timezone
from collections import Counter,defaultdict
import hashlib,json,re
ROOT=Path(__file__).resolve().parent.parent
def read(p):return json.loads((ROOT/p).read_text(encoding='utf-8-sig'))
def sha(p):return hashlib.sha256((ROOT/p).read_bytes()).hexdigest()
def normal(p):return p.replace('\\','/').removeprefix('./')
units=read('units.json');assert len(units)==4402
checkpoint=read('metadata/menu-reconstruction-progress.json')
approvals=read('metadata/full-function-review-approvals.json')
canonical='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
assert approvals['native_sha256']==canonical
evidence=defaultdict(list);reports={};suite_facts=[]
for item in checkpoint['native_suites']:
 path=item['path'];proof=read(path);reports[path]=proof
 stale=[normal(p) for p,h in {**proof.get('source_hashes',{}),**proof.get('native_exports',{})}.items() if not (ROOT/p).is_file() or sha(p)!=h]
 binary=item['compiled_C']['path']
 fresh=proof.get('passed') is True and not stale and (ROOT/binary).is_file() and sha(binary)==proof.get('compiled_C_sha256')
 suite_facts.append({'path':path,'fresh':fresh,'stale_inputs':stale,'accepted_checkpoint_hash_matches':sha(path)==item['sha256'],'checked':proof.get('checked',0)})
 if fresh:
  for p in proof.get('source_hashes',{}):evidence[normal(p)].append(path)
approved={int(x['address'],16):x for x in approvals['functions']}
patterns={
 'unresolved_return':r'\brestore_unresolved_return\s*\(',
 'register_input':r'\bin_(?:EAX|EBX|ECX|EDX|ESI|EDI|EBP|ESP|ST\d+)\b',
 'unaffiliated_register':r'\bunaff_[A-Za-z0-9_]+\b',
 'detached_stack_input':r'\bin_stack_[A-Za-z0-9_]+\b',
 'implicit_call_output':r'\bextraout_[A-Za-z0-9_]+\b',
 'untyped_byte_binding':r'^#define\s+\w+\s+\(\*\(undefined\s*\*\)',
 'exception_chain_model':r'\bExceptionList\b'}
entries=[];approval_failures=[]
for unit in units:
 address=int(unit['provenance']['address'],16);source=normal(unit['file']);text=(ROOT/source).read_text(encoding='utf-8-sig')
 markers={k:len(re.findall(v,text,re.MULTILINE)) for k,v in patterns.items()};digest=sha(source)
 status='not_reviewed';review=None
 if evidence[source]:status='comparison_scope_needs_review'
 if address in approved:
  review=approved[address];fail=[]
  if review['source']!=source or review['source_sha256']!=digest:fail.append('approved_source_changed')
  for requirement in review['evidence']:
   p=requirement['path'];proof=reports.get(p,{})
   if p not in evidence[source]:fail.append('evidence_not_fresh:'+p)
   if proof.get('checked')!=requirement['checked'] or proof.get(requirement['coverage_field'])!=requirement['coverage']:fail.append('coverage_contract_changed:'+p)
   if proof.get('controlled_algorithm_endpoints')!=[]:fail.append('controlled_dependency:'+p)
  if any(markers[k] for k in ['unresolved_return','register_input','unaffiliated_register','detached_stack_input','implicit_call_output']):fail.append('unresolved_semantic_marker')
  if fail:status='review_invalidated';approval_failures.append({'address':hex(address),'reasons':fail})
  else:status='reviewed_function_contract'
 entries.append({'address':f'0x{address:08x}','name':unit['name'],'source':source,'source_sha256':digest,'status':status,
  'current_marker_counts':markers,'fresh_comparison_suites':evidence[source],
  'historical_generation_flags':{k:unit.get(k) for k in ['unresolved_void_results','unresolved_stack_layout','register_inputs']},
  'review_scope':review.get('scope') if review else None})
assert len({e['address'] for e in entries})==len(entries)
counts=Counter(e['status'] for e in entries);markers=Counter()
for e in entries:markers.update(e['current_marker_counts'])
extra=approvals.get('additional_native_entries',[])
known={int(e['address'],16) for e in entries};assert all(int(e['address'],16) not in known for e in extra)
report={'format':1,'generated_at':datetime.now(timezone.utc).isoformat(),'native_sha256':canonical,
 'requested_completion_condition':'Review and reconstruct every project procedure and discovered native callable entry, with fresh source/ABI/algorithm evidence; compilation alone is insufficient.',
 'project_procedures':len(entries),'additional_catalogued_native_entries':len(extra),'status_counts':dict(counts),'marker_totals':dict(markers),
 'all_project_function_contracts_reviewed':counts['reviewed_function_contract']==len(entries),
 'complete_executable_review':False,'native_catalogue_closed':approvals['native_catalogue_closed'],'approval_failures':approval_failures,
 'scope_notes':['A source appearing in a fresh comparison does not by itself prove that its entire function was executed or rewritten.',
 'Markers are review leads, not automatic findings of a bug. Historical generation flags never override current evidence.',
 'Function-contract approval covers the documented valid inputs and composed dependencies; it does not establish complete gameplay.',
 'The native catalogue is still open: callback/vtable leaves may be absent from the4402 generated units.'],
 'comparison_suites':suite_facts,'additional_native_entries':extra,'functions':entries,
 'inputs':{p:sha(p) for p in ['units.json','metadata/menu-reconstruction-progress.json','metadata/full-function-review-approvals.json','tools/record_full_function_review.py']}}
(ROOT/'metadata/full-function-review-status.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({k:report[k] for k in ['project_procedures','additional_catalogued_native_entries','status_counts','marker_totals','all_project_function_contracts_reviewed','complete_executable_review','approval_failures']},indent=2))
