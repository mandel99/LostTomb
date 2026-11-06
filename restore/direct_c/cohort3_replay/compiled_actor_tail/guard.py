"""Hash/provider/recipe guards; optional check of an already-finished replay."""
from pathlib import Path
import argparse,hashlib,json,struct,sys
sys.dont_write_bytecode=True
OUT=Path(__file__).resolve().parent;ROOT=OUT.parents[2]
sys.path[:0]=[str(OUT),str(ROOT/'direct_c/cohort3_replay'),str(ROOT/'direct_c'),str(ROOT/'build/offline-verification-deps')]
import supplemental_registry as binding
import cohort7_compiled_actor_tail_cases as recipes


def main():
    ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--report',type=Path);ap.add_argument('--output',type=Path);args=ap.parse_args()
    protocol=binding.checked_inputs();members=binding.supplemental_members()
    assert len(members)==8 and all(m['new_source_credit']==m['new_entry_credit']==0 for m in members)
    all_cases=recipes.cases();assert len(all_cases)==43 and len(recipes.cases())==43
    assert len({c['id']for c in all_cases})==43
    assert recipes.definitions(dict(members=[]))==[]
    cases=recipes.definitions(dict(members=members));assert len(cases)==43
    planned=sum(c['expected_invocations']for c in cases);assert planned==protocol['current_planned_invocations']
    from native_test_pe import PE
    from registry import symbol
    from capstone import Cs,CS_ARCH_X86,CS_MODE_32
    native=PE(ROOT/protocol['native']['path'])
    constants=json.loads((OUT/'native-constants.json').read_bytes())
    for r in constants['records']:
        at=int(r['address'],16);raw=bytes(native.data[at-native.base:at-native.base+r['size']])
        assert raw.hex()==r['bytes_hex'] and hashlib.sha256(raw).hexdigest()==r['sha256']
    assert tuple(struct.unpack_from('<8i',native.data,0x5e4e0c-native.base))==recipes.DELTAS
    for label,image in protocol['compiled_images'].items():
        text=(ROOT/image['map_path']).read_text();pe=PE(ROOT/image['path'])
        parent=symbol(text,'FUN_004e7300')['va']
        calls={int(i.op_str,16)for i in Cs(CS_ARCH_X86,CS_MODE_32).disasm(pe.data[parent-pe.base:parent-pe.base+104],parent)
               if i.mnemonic=='call'and i.op_str.startswith('0x')}
        assert {symbol(text,n)['va']for n in('restore_god_effect_004e75a0','restore_god_effect_004e7410')}<=calls
        for c in cases:
            for spec in c['endpoints']:
                assert any(symbol(text,name)for name in spec['compiled_symbols']),(label,c['id'],spec['token'])
    for c in cases:
        assert c['native_cleanup']==c['named_cleanup']==c['canonical_cleanup']==0
        assert c['arguments']==c['named_arguments']==c['canonical_arguments']if c['entry']!=0x542ee0 else c['named_arguments']==[c['initial_eax']]and c['canonical_arguments']==[]
        assert c['result_mask']==({0x4ec4a0:0xffffffff,0x4f5050:255,0x4f53d0:255,0x542ee0:0xffffffff}.get(c['entry']))
        assert len(c['authored_endpoint_order'])==len(c['authored_endpoint_observations'])==sum(c['expected_endpoint_counts'].values())
        for spec in c['endpoints']:
            token=spec['token'];count=c['expected_endpoint_counts'][token]
            assert len(spec['responses'])==len(c['expected_endpoint_arguments'][token])==c['authored_endpoint_order'].count(token)==count
        assert c['authored_final_memory'] and c['compare_x87_values']
    inspected=0
    if args.report:
        rows={r['id']:r for r in json.loads(args.report.read_bytes())['cases_detail']}
        for c in cases:
            row=rows[c['id']];assert row['passed']and len(row['routes'])==c['expected_invocations'],c['id']
            expected=json.loads(json.dumps(c['authored_endpoint_observations']))
            for route in row['routes']:
                assert not route['errors']
                assert [e['token']for e in route['events']]==c['authored_endpoint_order']
                assert [e['observed']for e in route['events']]==expected
                if c['result_mask']is not None:assert int(route['logical_result']['value'],16)&c['result_mask']==c['expected_masked_result']
                inspected+=1
        assert inspected==planned
    result=dict(schema='cohort7-actor-tail-private-guard-v1',hashes_valid=True,current_members=8,
        cases=43,planned_invocations=planned,inspected_report_invocations=inspected,
        new_source_credit=0,new_entry_credit=0,replay_invoked=False,builds_run=False,host_execution=False,whole_game_equivalence=False)
    if args.output:args.output.write_bytes((json.dumps(result,indent=2)+'\n').encode())
    print(json.dumps(result))


if __name__=='__main__':main()
