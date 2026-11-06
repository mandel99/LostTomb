"""Same original per-route oracle audit, with the actual qualified route total."""
import json

def audit(cases,report):
    actual={c['id']:c for c in report['cases_detail']};routes=events=0
    for c in cases:
        row=actual[c['id']]
        assert row['passed'],c['id']
        assert len(row['routes'])==c['expected_invocations']
        for r in row['routes']:
            assert r['route']in('native','canonical')and not r['errors'],(c['id'],r['errors'])
            assert[e['token']for e in r['events']]==c['authored_endpoint_order']
            assert[e['observed']for e in r['events']]==json.loads(json.dumps(c['authored_endpoint_observations']))
            for token,expected in c['expected_endpoint_arguments'].items():
                assert[e['arguments']for e in r['events']if e['token']==token]==expected
            if c['result_mask']is not None:
                assert int(r['logical_result']['value'],16)&c['result_mask']==c['expected_masked_result']
            routes+=1;events+=len(r['events'])
    assert routes==sum(c['expected_invocations']for c in cases)
    return dict(routes=routes,endpoint_events=events,independent_endpoint_args_order_and_snapshots=True,
                qualified_route_total=True,old_packet_preserved=True,new_emulation=False)
