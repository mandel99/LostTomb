"""Executable rejection/guard checks, not native-function acceptance."""
import copy,json
from pathlib import Path
from select_candidates import check,Rejected
from common import PRIVATE,confined
DT={'display':'undefined4','name':'undefined4','size_bytes':4,'java_class':'ghidra.program.model.data.Undefined4DataType'}
def fixture(partial=False):
 a='00401000';ins=[{'address':a,'bytes':'b001' if partial else 'b801000000','instruction':'MOV AL,0x1' if partial else 'MOV EAX,0x1','raw_pcode':[],'flow_targets':[],'native_jump':False,'native_call':False},{'address':'00401002' if partial else '00401005','bytes':'c3','instruction':'RET','raw_pcode':[],'flow_targets':[],'native_jump':False,'native_call':False}];f={'entry':a,'status':'facts_complete','logical_prototype':{'return_datatype':DT,'calling_model_advisory':'__stdcall','parameters':[]},'native_instructions':ins,'native_callsites':[],'native_return_sites':[ins[-1]],'raw_pseudocode':'undefined4 FUN_00401000(void)\n{\n return 1;\n}\n','clang_tokens':[]};s={'entry':a,'unit':'src/unit_fake.c','protected_function':False,'expected_instructions':[[q['address'],q['bytes']] for q in ins]};return f,s
passed=[]
f,s=fixture();check(f,s);passed.append('simple_fullEAX_leaf_eligible')
for label,change in [('protected',lambda f,s:s.update(protected_function=True)),('native_call',lambda f,s:f.update(native_callsites=[{'address':'00401000'}])),('native_RET_cleanup',lambda f,s:f['native_return_sites'][0].update(instruction='RET 0x4')),('unresolved_register',lambda f,s:f.update(raw_pseudocode='undefined4 FUN_00401000(void){return in_EAX;}')),('pointer_projection',lambda f,s:f.update(raw_pseudocode='undefined4 FUN_00401000(void){return *(int *)0x12345678;}')),('register_parameter',lambda f,s:f['logical_prototype']['parameters'].append({'datatype':DT,'storage':[{'address':'register:4','size_bytes':4}]}))]:
 f,s=fixture();change(f,s)
 try:check(f,s)
 except Rejected:passed.append(label);continue
 raise AssertionError('Unsafe candidate accepted: '+label)
f,s=fixture(True)
try:check(f,s)
except Rejected:passed.append('partial_AL_return_rejected')
else:raise AssertionError('PartialAL accepted')
for target in [PRIVATE.parent,PRIVATE.parent.parent/'src',PRIVATE/'../outside']:
 try:confined(target)
 except ValueError:passed.append('output_guard:'+str(target));continue
 raise AssertionError('Unsafe output accepted')
PRIVATE.mkdir(exist_ok=True);(PRIVATE/'policy-guard-tests.json').write_text(json.dumps({'passed':True,'checked':len(passed),'cases':passed,'synthetic_policy_fixtures_only':True,'no_native_function_acceptance_claimed':True},indent=2)+'\n');print('V2policy/CLI guards',len(passed),'passed')
