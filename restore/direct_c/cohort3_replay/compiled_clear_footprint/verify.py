from pathlib import Path
import copy,datetime,hashlib,json,sys
sys.dont_write_bytecode=True
OUT=Path(__file__).resolve().parent;R=OUT.parents[2]
sys.path[:0]=[str(OUT),str(R/'direct_c/cohort3_replay'),str(R/'direct_c'),str(R/'direct_c/cohort3_replay/compiled_event_food')]
import footprint_cases,replay
from registry import load,symbol,save,digest
from supplemental_registry import source_body


def extend(base):
    from native_test_pe import PE
    native=PE(R/base['native']['path']);e=json.loads((OUT/'native-evidence.json').read_bytes())
    assert e['native_sha256']==base['native']['sha256']
    member=e['member'];cursor=0x47c480
    for i in member['assembly']['items']:
        raw=bytes.fromhex(i['bytes']);assert int(i['address'],16)==cursor
        assert native.data[cursor-native.base:cursor-native.base+len(raw)]==raw;cursor+=len(raw)
    source='runtime/menu_drag_clear_footprint.c';name='FUN_0047c480';providers={}
    for label,image in base['compiled_images'].items():
        named=symbol((R/image['map_path']).read_text(),name)
        assert named and 'menu_drag_clear_footprint.obj'in named['owner']
        providers[label]=dict(named=named,canonical=copy.deepcopy(named))
    result=copy.deepcopy(base);result['members'].append(dict(address='0x47c480',name=name,
        source=source,source_definition_symbol=name,algorithm_source=source,source_sha256=digest(R/source),
        body_sha256=hashlib.sha256(source_body((R/source).read_bytes(),name)).hexdigest(),
        native_ranges=[dict(start='0x47c480',end=hex(cursor-1))],native_ret_immediates=[0],
        logical_signature='void cdecl3 building,x,y; actual origin, processing classification and monument size children',
        providers=providers,rule=None,case_status='authored_not_executed',supplemental_comparison_only=True,
        new_source_credit=0,new_entry_credit=0))
    result['comparison_entries']=len(result['members']);return result


for item in json.loads((OUT/'manifest.json').read_bytes())['files']:
    assert digest(OUT/item['path'])==item['sha256'],item['path']
base=load(True);assert base['compiled_images']==json.loads((OUT/'image-profile.json').read_bytes());registry=extend(base);cases=footprint_cases.definitions(registry)
assert len(cases)==19 and sum(c['expected_invocations']for c in cases)==133
directory=R/'build/actor-animal-following/cohort7-footprint-public-runs'/datetime.datetime.now(datetime.timezone.utc).strftime('%Y%m%d-%H%M%S-%f')
directory.mkdir(parents=True,exist_ok=False);save(directory/'registry.json',registry)
original=replay.load;replay.load=lambda require_retained=True:extend(original(require_retained))
try:result=replay.execute(registry,cases,directory)
finally:replay.load=original
print(json.dumps(dict(result=str(directory/'result.json'),**{k:result[k]for k in ['cases','entries_compared','invocations','failed_cases','inputs_stable']})))
assert not result['failed_cases']and result['inputs_stable']
rows={row['id']:row for row in result['cases_detail']};inspected=0
for c in cases:
    row=rows[c['id']];assert len(row['routes'])==c['expected_invocations']
    for route in row['routes']:
        assert not route['errors']
        assert [e['token']for e in route['events']]==c['authored_endpoint_order']
        assert [e['observed']for e in route['events']]==json.loads(json.dumps(c['authored_endpoint_observations']))
        for token,expected in c['expected_endpoint_arguments'].items():
            assert [e['arguments']for e in route['events']if e['token']==token]==expected
        inspected+=1
save(directory/'authored-checks.json',dict(cases=19,invocations=inspected,passed=True,host_execution=False))
