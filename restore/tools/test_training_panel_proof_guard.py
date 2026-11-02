"""Reject incomplete/stale evidence before retiring the old panel call literals."""
from pathlib import Path
from unittest.mock import patch
import copy,importlib.util,json
ROOT=Path(__file__).resolve().parents[1]
spec=importlib.util.spec_from_file_location('returns_guard',ROOT/'tools/repair_discarded_returns.py');module=importlib.util.module_from_spec(spec);spec.loader.exec_module(module)
target=(ROOT/'metadata/training-panel-emulation.json').resolve();proof=json.loads(target.read_text());original=Path.read_text
assert module.verified_training_panel_supersession(ROOT)['whole_procedure_proven'] is True
mutations={
 'failed':lambda p:p.update(passed=False),
 'partial':lambda p:p.update(case_selection={'limit':3864,'start':0,'single_case':False}),
 'missing_instruction':lambda p:p.update(native_instruction_coverage=[1000,1001]),
 'missing_conditional_outcome':lambda p:p.update(native_conditional_outcomes=[89,90]),
 'missing_computed_edge':lambda p:p.update(feasible_computed_edges=[14,15]),
 'unqualified_exclusion':lambda p:p['declared_infeasible_computed_edges'].update({'0x1->0x2':'invented'}),
 'stale_source':lambda p:p['source_hashes'].update({'src/unit_02637.c':'0'*64}),
 'stale_binary':lambda p:p.update(comparison_C_sha256='0'*64),
 'private_sources':lambda p:p['source_hashes'].pop('src/unit_02637.c')}
rejected=[]
for name,mutation in mutations.items():
 candidate=copy.deepcopy(proof);mutation(candidate)
 def read(path,*a,**kw):return json.dumps(candidate) if path.resolve()==target else original(path,*a,**kw)
 try:
  with patch.object(Path,'read_text',read):module.verified_training_panel_supersession(ROOT)
 except ValueError:rejected.append(name)
 else:raise AssertionError('Unqualified panel proof accepted: '+name)
report={'format':1,'passed':True,'in_memory_only':True,'product_files_modified':False,'rejected':rejected,'checked':len(mutations)}
(ROOT/'metadata/training-panel-proof-negative.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
