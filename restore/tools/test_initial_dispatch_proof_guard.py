"""Ensure a retired caller literal requires complete, fresh native evidence."""
from pathlib import Path
from unittest.mock import patch
import copy,importlib.util,json
ROOT=Path(__file__).resolve().parents[1]
spec=importlib.util.spec_from_file_location('returns_guard',ROOT/'tools/repair_discarded_returns.py');module=importlib.util.module_from_spec(spec);spec.loader.exec_module(module)
target=(ROOT/'metadata/initial-menu-dispatch-emulation.json').resolve();proof=json.loads(target.read_text());original=Path.read_text
assert module.verified_initial_dispatch_supersession(ROOT)['whole_caller_procedure_proven'] is True
mutations={
 'failed_comparison':lambda p:p.update(passed=False),
 'partial_corpus':lambda p:p.update(complete_case_set=False,checked=5794),
 'uncovered_instruction':lambda p:p.update(native_instruction_coverage=[178,179]),
 'uncovered_branch':lambda p:p.update(conditional_outcomes=[7,8]),
 'stale_source':lambda p:p['source_hashes'].update({'src/unit_01866.c':'0'*64}),
 'stale_binary':lambda p:p['variants']['O2_plain'].update(sha256='0'*64),
 'private_binary':lambda p:p['variants']['Debug_trace'].update(path='build/initial-menu-dispatch/Debug/restore_initial_menu_dispatch_verify.exe')}
rejected=[]
for name,mutation in mutations.items():
 candidate=copy.deepcopy(proof);mutation(candidate)
 def read(path,*a,**kw):return json.dumps(candidate) if path.resolve()==target else original(path,*a,**kw)
 try:
  with patch.object(Path,'read_text',read):module.verified_initial_dispatch_supersession(ROOT)
 except ValueError:rejected.append(name)
 else:raise AssertionError('Unqualified proof accepted: '+name)
report={'format':1,'passed':True,'in_memory_only':True,'product_files_modified':False,'rejected':rejected,'checked':len(mutations)}
(ROOT/'metadata/initial-dispatch-proof-negative.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps(report))
