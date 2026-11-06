from pathlib import Path
import copy,datetime,hashlib,importlib.util,json,sys
sys.dont_write_bytecode=True
OUT=Path(__file__).resolve().parent;R=OUT.parents[2]
P=OUT
M=OUT
sys.path[:0]=[str(P),str(M),str(R/'direct_c/cohort3_replay'),str(R/'direct_c')]
import supplemental_registry as food
import event_food_cases,message_cases,replay
from registry import load,save,symbol,digest


def extend(base):
    out=food.extend_registry(base)
    from native_test_pe import PE
    native=PE(R/base['native']['path'])
    e=json.loads((M/'message-native-rea.json').read_bytes())
    raw=bytes.fromhex(e['normalized_result']['bytes_hex'])[:48]
    assert raw==native.data[0x511c50-native.base:0x511c80-native.base]
    source='runtime/small_button_actions2.c';name='restore_small_button2_00511c50'
    body=food.source_body((R/source).read_bytes(),name);providers={}
    for label,image in base['compiled_images'].items():
        text=(R/image['map_path']).read_text();named=symbol(text,name)
        assert bool(named)==(label=='retained'),(label,name)
        assert not symbol(text,'FUN_00511c50')
        if named:assert 'small_button_actions2.obj'in named['owner']
        providers[label]=dict(named=named,canonical=None)
    out['members'].append(dict(address='0x511c50',name=name,source=source,source_definition_symbol=name,
        algorithm_source=source,source_sha256=digest(R/source),body_sha256=hashlib.sha256(body).hexdigest(),
        native_ranges=[dict(start='0x511c50',end='0x511c7f')],native_ret_immediates=[0],
        logical_signature='void cdecl0; signed DWORD message selector; actual panel boundary controlled',
        providers=providers,rule=None,case_status='authored_not_executed',supplemental_comparison_only=True,
        new_source_credit=0,new_entry_credit=0,native_ownership='Pinned raw48-byte extent; no standalone Ghidra function'))
    out['comparison_entries']=len(out['members'])
    return out


base=load(True);assert base['compiled_images']==json.loads((OUT/'image-profile.json').read_bytes());registry=extend(base)
old=json.loads((OUT/'pre-images.json').read_bytes())
assert all(base['compiled_images'][label]==old[label]for label in ['game','diagnostic'])
# Current images are exactly pinned by image-profile.json.
cases=event_food_cases.cases(registry)+message_cases.definitions(registry)
assert len(cases)==45 and sum(c['expected_invocations']for c in cases)==239
directory=R/'build/actor-animal-following/cohort7-event-food-public-runs'/datetime.datetime.now(datetime.timezone.utc).strftime('%Y%m%d-%H%M%S-%f')
directory.mkdir(parents=True,exist_ok=False);save(directory/'registry.json',registry)
original=replay.load;replay.load=lambda require_retained=True:extend(original(require_retained))
try:result=replay.execute(registry,cases,directory)
finally:replay.load=original
print(json.dumps(dict(result=str(directory/'result.json'),**{k:result[k]for k in ['cases','entries_compared','invocations','failed_cases','inputs_stable']})),flush=True)
assert result['failed_cases']==0 and result['inputs_stable']
rows={c['id']:c for c in result['cases_detail']};checked=0
for c in cases:
    row=rows[c['id']];assert row['passed']and len(row['routes'])==c['expected_invocations']
    for route in row['routes']:
        assert not route['errors']
        assert [e['token']for e in route['events']]==c['authored_endpoint_order']
        assert [e['observed']for e in route['events']]==json.loads(json.dumps(c['authored_endpoint_observations']))
        for token,args in c['expected_endpoint_arguments'].items():
            assert [e['arguments']for e in route['events']if e['token']==token]==json.loads(json.dumps(args))
        checked+=1
save(directory/'authored-checks.json',dict(cases=45,entries=6,invocations=checked,passed=True,
    original_C_bodies_preserved=True,new_source_credit=0,new_entry_credit=0,host_execution=False))
print('Authored route snapshots PASS',checked,flush=True)
