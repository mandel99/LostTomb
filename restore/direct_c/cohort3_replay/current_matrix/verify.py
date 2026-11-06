"""One explicit current-image binding for source100 and existing47 contracts."""
from pathlib import Path
import argparse,copy,datetime,hashlib,importlib.util,json,sys
sys.dont_write_bytecode=True
OUT=Path(__file__).resolve().parent;R=OUT.parents[2];PUBLIC=R/'direct_c/cohort3_replay'
sys.path[:0]=[str(PUBLIC),str(R/'direct_c')]
from registry import load,save,symbol,digest


def module(name,path):
    spec=importlib.util.spec_from_file_location(name,path);result=importlib.util.module_from_spec(spec)
    sys.path.insert(0,str(path.parent));spec.loader.exec_module(result);return result


def checked_protocol(packet,base,changes):
    folder=PUBLIC/packet
    for item in json.loads((folder/'manifest.json').read_bytes())['files']:
        assert digest(folder/item['path'])==item['sha256'],(packet,item['path'])
    p=json.loads((folder/'protocol.json').read_bytes())
    guards=p.get('strict_source_guards',p.get('source_guards',{}));assert guards,packet
    repair=json.loads((R/'metadata/cohort7-percent-idiv-fault-repair.json').read_bytes())['source_repair']
    for name,old in list(guards.items()):
        now=digest(R/name)
        if now!=old:
            if name=='src/unit_01731.c':assert old==repair['before_sha256']and now==repair['sha256']
            else:assert name in ['direct_c/cohort3_replay/replay.py','direct_c/cohort3_replay/retained_image.cmake'],(packet,name)
            changes.append(dict(packet=packet,path=name,before_sha256=old,sha256=now))
        guards[name]=now
    for item in p.get('evidence_inputs',[]):assert digest(R/item['path'])==item['sha256']
    assert p['native']==base['native'] and digest(R/p['native']['path'])==p['native']['sha256']
    if 'compiled_images'in p:p['compiled_images']=base['compiled_images']
    if packet=='compiled_actor_tail':
        c=next(c for c in p['members']if int(c['address'],16)==0x542ee0)
        c['canonical_images']=dict(game=False,diagnostic=False,retained=True)
    return p


PACKETS=[('compiled_first5_current','cohort7_compiled_first5_cases.py'),
    ('compiled_flood_regions','cohort7_compiled_acceptance_cases.py'),
    ('compiled_animation_current','animation_cases.py'),
    ('compiled_event_constructor','constructor_cases.py'),
    ('compiled_actor_tail','cohort7_compiled_actor_tail_cases.py'),
    ('compiled_event_food','event_food_cases.py'),('compiled_callbacks','callback_cases.py'),
    ('compiled_buyer_chain','buyer_chain_cases.py'),('compiled_final6','remaining_cases.py')]


def bind():
    base=load(True);assert base['compiled_images']==json.loads((OUT/'epoch.json').read_bytes())['compiled_images'];result=copy.deepcopy(base);changes=[];recipes=[];additions=[]
    for packet,filename in PACKETS:
        binding=module('binding_'+packet,PUBLIC/packet/'supplemental_registry.py')
        p=checked_protocol(packet,base,changes)
        def checked(p=p):
            for name,sha in p.get('strict_source_guards',p.get('source_guards',{})).items():assert digest(R/name)==sha,name
            for im in base['compiled_images'].values():
                assert digest(R/im['path'])==im['sha256']and digest(R/im['map_path'])==im['map_sha256']
            return p
        binding.checked_inputs=checked
        additions+=binding.supplemental_members(base)
        recipes.append((packet,module('recipes_'+packet,PUBLIC/packet/filename)))
    from native_test_pe import PE
    pe=PE(R/base['native']['path'])
    food=module('food_source_parser',PUBLIC/'compiled_event_food/supplemental_registry.py')
    for address,source,name,owner,canonical,end in [
            (0x511c50,'runtime/small_button_actions2.c','restore_small_button2_00511c50','small_button_actions2.obj',False,0x511c7f),
            (0x47c480,'runtime/menu_drag_clear_footprint.c','FUN_0047c480','menu_drag_clear_footprint.obj',True,0x47c7ac)]:
        data=(R/source).read_bytes();body=food.source_body(data,name);providers={}
        for label,image in base['compiled_images'].items():
            named=symbol((R/image['map_path']).read_text(),name)
            assert bool(named)==(address==0x47c480 or label=='retained'),(label,name)
            if named:assert owner in named['owner']
            providers[label]=dict(named=named,canonical=copy.deepcopy(named)if canonical else None)
        if address==0x511c50:
            e=json.loads((PUBLIC/'compiled_event_food/message-native-rea.json').read_bytes())
            raw=bytes.fromhex(e['normalized_result']['bytes_hex'])[:48]
        else:
            e=json.loads((PUBLIC/'compiled_clear_footprint/native-evidence.json').read_bytes())['member']
            raw=bytes.fromhex(''.join(i['bytes']for i in e['assembly']['items']))
        assert raw==pe.data[address-pe.base:end+1-pe.base]
        additions.append(dict(address=hex(address),name=name,source=source,source_definition_symbol=name,
            algorithm_source=source,source_sha256=digest(R/source),body_sha256=hashlib.sha256(body).hexdigest(),
            native_ranges=[dict(start=hex(address),end=hex(end))],native_ret_immediates=[0],
            logical_signature='void cdecl0 signed message action'if address==0x511c50 else'void cdecl3 footprint clear',
            providers=providers,rule=None,supplemental_comparison_only=True,new_source_credit=0,new_entry_credit=0))
    assert len(additions)==47 and len({int(m['address'],16)for m in additions})==47
    assert not {int(m['address'],16)for m in additions}&{int(m['address'],16)for m in base['members']}
    result['members']+=additions;result['comparison_entries']=len(result['members'])
    result['current_contract_matrix']=dict(existing_C_entries=47,new_source_credit=0,new_entry_credit=0,known_source_rebindings=changes)
    cases=[]
    for packet,m in recipes:
        selected=m.definitions(result)if hasattr(m,'definitions')else m.cases(result)
        if packet=='compiled_actor_tail':
            selected=[c for c in selected if c['entry']!=0x4f53d0]
            query=module('current_query_recipes',PUBLIC/'compiled_actor_query_repair/cohort7_actor_tail_query_repair_cases.py')
            selected+=query.definitions(result)
        if packet=='compiled_final6':
            repair=module('current_boundary_repair',PUBLIC/'compiled_final6/fixture_repair.py')
            selected=repair.repair_boundary(selected)
        cases+=selected
    cases+=module('current_message_cases',PUBLIC/'compiled_event_food/message_cases.py').definitions(result)
    cases+=module('current_footprint_cases',PUBLIC/'compiled_clear_footprint/footprint_cases.py').definitions(result)
    assert len(cases)==299 and len({c['id']for c in cases})==299
    assert {c['entry']for c in cases}=={int(m['address'],16)for m in additions}
    members={int(m['address'],16):m for m in result['members']}
    for c in cases:
        m=members[c['entry']];requested=set(c.get('routes',['native','named','canonical']))
        count=1
        for p in m['providers'].values():
            count+=bool(p['named'])and'named'in requested
            count+=int(bool(p['canonical'])and'canonical'in requested and bool(Path(m['source']).stem+'.obj'in p['canonical']['owner']or c.get('canonical_owner')and c['canonical_owner']in p['canonical']['owner']))
        c['expected_invocations']=int(count)
    assert sum(c['expected_invocations']for c in cases)==1757
    return result,cases


def audit(cases,report):
    rows={r['id']:r for r in report['cases_detail']};invocations=0;snapshots=0
    assert not report['failed_cases']and report['inputs_stable']
    for c in cases:
        row=rows[c['id']];assert row['passed']
        if 'expected_invocations'in c:assert len(row['routes'])==c['expected_invocations']
        for route in row['routes']:
            assert not route['errors']
            if 'authored_endpoint_order'in c:
                assert [e['token']for e in route['events']]==c['authored_endpoint_order'],(c['id'],'order')
                assert [e['observed']for e in route['events']]==json.loads(json.dumps(c['authored_endpoint_observations'])),(c['id'],'snapshot')
                snapshots+=1
            for token,args in c.get('expected_endpoint_arguments',{}).items():
                assert [e['arguments']for e in route['events']if e['token']==token]==json.loads(json.dumps(args)),(c['id'],token)
            wanted=c.get('expected_masked_result',c.get('expected_eax'))
            if c['result_mask']is not None and wanted is not None:
                assert int(route['logical_result']['value'],16)&c['result_mask']==wanted,(c['id'],'result')
            invocations+=1
    assert invocations==report['invocations']
    return dict(schema='cohort7-current-matrix-authored-checks-v1',cases=len(cases),
        invocations=invocations,authored_snapshot_routes_checked=snapshots,passed=True,
        host_execution=False,whole_game_equivalence=False)


def main():
    ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--scope',choices=['source100','compiled47'],required=True)
    ap.add_argument('--run',action='store_true');args=ap.parse_args()
    for item in json.loads((OUT/'manifest.json').read_bytes())['files']:
        assert digest(OUT/item['path'])==item['sha256'],item['path']
    assert digest(R/'direct_c/cohort3_replay/replay.py')==json.loads((OUT/'epoch.json').read_bytes())['source_replay_sha256']
    if args.scope=='source100':
        registry=load(True);assert registry['compiled_images']==json.loads((OUT/'epoch.json').read_bytes())['compiled_images']
        from cases import definitions
        addresses={int(m['address'],16)for m in json.loads((R/'metadata/cohort7-source-progress.json').read_bytes())['members']}
        cases=[c for c in definitions(registry)if c['entry']in addresses]
        assert len(cases)==606 and len({c['entry']for c in cases})==100
    else:registry,cases=bind()
    if not args.run:print(json.dumps(dict(scope=args.scope,cases=len(cases),entries=len({c['entry']for c in cases}),execution=False)));return
    import replay
    directory=R/'build/actor-animal-following/cohort7-public-current-matrix-runs'/(args.scope+'-'+datetime.datetime.now(datetime.timezone.utc).strftime('%Y%m%d-%H%M%S-%f'))
    directory.mkdir(parents=True,exist_ok=False);save(directory/'registry.json',registry)
    original=replay.load
    if args.scope=='compiled47':replay.load=lambda require_retained=True:bind()[0]
    try:result=replay.execute(registry,cases,directory)
    finally:replay.load=original
    print(json.dumps(dict(result=str(directory/'result.json'),**{k:result[k]for k in ['cases','entries_compared','invocations','failed_cases','inputs_stable']})),flush=True)
    assert not result['failed_cases']and result['inputs_stable']
    save(directory/'authored-checks.json',audit(cases,result))


if __name__=='__main__':main()
