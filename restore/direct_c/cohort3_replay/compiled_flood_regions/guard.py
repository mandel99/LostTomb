"""Pure packet/provider/recipe guards; optional already-completed report check."""
from pathlib import Path
import argparse,json,sys
sys.dont_write_bytecode=True
OUT=Path(__file__).resolve().parent;ROOT=OUT.parents[2]
sys.path[:0]=[str(OUT),str(ROOT/'direct_c/cohort3_replay')]
import supplemental_registry as binding
import cohort7_compiled_acceptance_cases as recipes


def main():
    ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--report',type=Path);ap.add_argument('--output',type=Path);args=ap.parse_args()
    protocol=binding.checked_inputs();members=binding.supplemental_members()
    all_cases=recipes.cases();assert len(all_cases)==43 and len(recipes.cases())==43
    assert len({c['id']for c in all_cases})==43
    assert recipes.definitions(dict(members=[]))==[]
    selected=recipes.definitions(dict(members=members))
    assert len(selected)==protocol['current_available_cases']
    planned=sum(c['expected_invocations']for c in selected)
    assert planned==protocol['current_planned_invocations']
    assert sum('known_current_C_gap'in c for c in all_cases)==3
    for c in all_cases:
        assert c['entry']in recipes.TARGETS
        assert c['result_mask']==(0xffffffff if c['entry']in(0x4bd740,0x4becf0)else None)
        assert c['native_cleanup']=={0x4bd740:20,0x4bd950:4}.get(c['entry'],0)
        assert c['named_cleanup']==c['canonical_cleanup']==0
        if c['entry']not in(0x4be640,0x4becf0):
            assert c['named_arguments']==c['canonical_arguments']==[c['receiver'],*c['arguments']]
        else:assert c['receiver']==0xd05720 and c['named_arguments']==c['canonical_arguments']==[]
        assert len(c['authored_endpoint_order'])==len(c['authored_endpoint_observations'])
        assert len(c['authored_endpoint_order'])==sum(c['expected_endpoint_counts'].values())
        for ep in c['endpoints']:
            token=ep['token'];count=c['expected_endpoint_counts'][token]
            assert len(ep['responses'])==count==c['authored_endpoint_order'].count(token)
            assert len(c['expected_endpoint_arguments'][token])==count
        assert c['authored_final_memory']
    inspected=0
    if args.report:
        rows={r['id']:r for r in json.loads(args.report.read_bytes())['cases_detail']}
        for c in selected:
            row=rows[c['id']];assert row['passed'],c['id']
            assert len(row['routes'])==c['expected_invocations']
            expected=json.loads(json.dumps(c['authored_endpoint_observations']))
            for route in row['routes']:
                assert not route['errors']
                assert [e['token']for e in route['events']]==c['authored_endpoint_order']
                assert [e['observed']for e in route['events']]==expected
                if c['result_mask']is not None:
                    assert int(route['logical_result']['value'],16)&c['result_mask']==c['expected_masked_result']
                inspected+=1
        assert inspected==planned
    result=dict(schema='cohort7-acceptance-private-guard-v1',hashes_valid=True,authored_contracts=8,
        current_members=len(members),current_cases=len(selected),current_planned_invocations=planned,
        pending_cases=43-len(selected),known_current_C_gap_cases=3,inspected_report_invocations=inspected,
        new_source_credit=0,new_entry_credit=0,replay_invoked=False,builds_run=False,host_execution=False,whole_game_equivalence=False)
    if args.output:args.output.write_bytes((json.dumps(result,indent=2)+'\n').encode())
    print(json.dumps(result))


if __name__=='__main__':main()
