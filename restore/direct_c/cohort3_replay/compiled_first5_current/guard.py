"""Private hash/provider/recipe guards; optionally inspect a completed report.

This never invokes replay, a build, Unicorn, the host game or Git.
"""
from pathlib import Path
import argparse, importlib.util, json, sys
sys.dont_write_bytecode = True
OUT = Path(__file__).resolve().parent
ROOT = OUT.parents[2]
sys.path[:0] = [str(OUT), str(ROOT / 'direct_c/cohort3_replay')]
import supplemental_registry as bindings
import cohort7_compiled_first5_cases as recipes


def normal(value):
    return json.loads(json.dumps(value))


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--report', type=Path)
    parser.add_argument('--output', type=Path)
    args = parser.parse_args()
    protocol = bindings.checked_inputs()
    members = bindings.supplemental_members()
    assert len(members) == 5 and all(m['new_entry_credit'] == 0 for m in members)
    cases = recipes.cases()
    assert len(cases) == 24 and len({c['id'] for c in cases}) == 24
    assert len(recipes.cases()) == 24, 'recipe construction must not mutate shared fixtures'
    assert recipes.cases(dict(members=[])) == []
    assert len(recipes.definitions(dict(members=members))) == 24
    counts = {}
    for c in cases:
        counts[c['entry']] = counts.get(c['entry'], 0) + 1
        assert c['entry'] in recipes.TARGETS
        assert c['expected_invocations'] == (4 if c['entry'] in (0x4062f0, 0x406620) else 7)
        assert c['result_mask'] == ({0x4062f0: None, 0x406620: None,
            0x4192b0: 0xffffffff, 0x41c8f0: 255, 0x41df70: 255}[c['entry']])
        assert c['native_cleanup'] == ({0x41c8f0: 12, 0x41df70: 8}.get(c['entry'], 0))
        assert c['named_cleanup'] == c['canonical_cleanup'] == 0
        if c['entry'] in (0x41c8f0, 0x41df70):
            assert c['named_arguments'] == c['canonical_arguments'] == [c['receiver'], *c['arguments']]
        assert len(c['authored_endpoint_order']) == len(c['authored_endpoint_observations'])
        assert len(c['authored_endpoint_order']) == sum(c['expected_endpoint_counts'].values())
        for token, count in c['expected_endpoint_counts'].items():
            assert c['authored_endpoint_order'].count(token) == count
            assert len(c['expected_endpoint_arguments'][token]) == count
        assert c['authored_final_memory']
    assert counts == {0x4062f0: 2, 0x406620: 4, 0x4192b0: 4, 0x41c8f0: 7, 0x41df70: 7}
    planned = sum(c['expected_invocations'] for c in cases)
    assert planned == 150
    inspected = 0
    if args.report:
        report = json.loads(args.report.read_bytes())
        rows = {r['id']: r for r in report['cases_detail']}
        for c in cases:
            row = rows[c['id']]
            assert row['passed'], c['id']
            assert len(row['routes']) == c['expected_invocations'], c['id']
            for route in row['routes']:
                assert not route['errors'], (c['id'], route['image'], route['route'])
                assert [e['token'] for e in route['events']] == c['authored_endpoint_order']
                assert [e['observed'] for e in route['events']] == normal(c['authored_endpoint_observations'])
                if c['result_mask'] is not None:
                    expected = c.get('expected_masked_result', c.get('expected_eax'))
                    assert int(route['logical_result']['value'], 16) & c['result_mask'] == expected
                inspected += 1
        assert inspected == planned
    result = dict(schema='cohort7-first5-private-guard-v1', hashes_valid=True,
        supplemental_members=5, authored_cases=24, planned_invocations=planned,
        inspected_report_invocations=inspected, new_source_credit=0, new_entry_credit=0,
        replay_invoked=False, builds_run=False, host_execution=False,
        whole_game_equivalence=False)
    if args.output:
        args.output.write_bytes((json.dumps(result, indent=2) + '\n').encode())
    print(json.dumps(result))


if __name__ == '__main__':
    main()
