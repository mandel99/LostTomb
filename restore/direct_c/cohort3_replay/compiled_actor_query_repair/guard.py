"""Pure guards for derived query fixtures; never invokes replay or a host."""
from pathlib import Path
import argparse,hashlib,json,sys
sys.dont_write_bytecode=True
OUT=Path(__file__).resolve().parent;ROOT=OUT.parents[2]
sys.path[:0]=[str(OUT),str(ROOT/'direct_c/cohort3_replay'),str(ROOT/'direct_c')]
import supplemental_registry as binding
import cohort7_actor_tail_query_repair_cases as recipes


def main():
    ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--report',type=Path);ap.add_argument('--output',type=Path);args=ap.parse_args()
    p=binding.checked_inputs();members=binding.supplemental_members();assert len(members)==1
    original=ROOT/p['original_packet']['path']
    assert binding.digest(original/'manifest.json')==p['original_packet']['manifest_sha256']
    for item in json.loads((original/'manifest.json').read_bytes())['files']:
        assert binding.digest(original/item['path'])==item['sha256']
    evidence=json.loads((OUT/'native-evidence.json').read_bytes())
    helper=next(x for x in evidence['members']if x['requested_address']=='004ef450')
    raw=bytes.fromhex(''.join(i['bytes']for i in helper['assembly']['items']))
    assert len(helper['assembly']['items'])==86 and len(raw)==256
    from native_test_pe import PE
    native=PE(ROOT/p['native']['path'])
    assert raw==native.data[0x4ef450-native.base:0x4ef550-native.base]
    expected_write_sites={'004ef463','004ef47b','004ef51d','004ef52e','004ef536'}
    actual={i['address']for i in helper['assembly']['items']if i['instruction'].startswith('MOV ')and
            ('ptr [0x00eaaf58]'in i['instruction']or i['instruction'].startswith('MOV [0x00eaaf58]')or
             '[EAX*0x4 + 0xeaac44]'in i['instruction']or
             i['instruction'].startswith('MOV dword ptr [ESP + 0x14]'))}
    assert actual==expected_write_sites,actual
    cases=recipes.definitions(dict(members=members));assert len(cases)==len(recipes.cases())==6
    assert recipes.cases(dict(members=[]))==[]
    assert sum(c['expected_invocations']for c in cases)==42
    for c in cases:
        assert c['arguments']==c['named_arguments']==c['canonical_arguments']==[2]
        assert c['result_mask']==255 and c['native_cleanup']==c['named_cleanup']==c['canonical_cleanup']==0
        oracle=c['helper_output_oracle'];prefix=oracle['changed_prefix']
        authored={a:bytes.fromhex(v)for a,v in c['authored_final_memory']}
        region=authored[recipes.OUTPUT_GUARD]
        assert int.from_bytes(region[:4],'little')==0xa1b2c3d4
        assert int.from_bytes(region[-4:],'little')==0xe5f60718
        words=[int.from_bytes(region[4+i*4:8+i*4],'little')for i in range(recipes.OUTPUT_WORDS)]
        assert words==prefix+[recipes.SENTINEL]*(recipes.OUTPUT_WORDS-len(prefix))
        assert int.from_bytes(authored[recipes.COUNT],'little')==oracle['count']
        assert (recipes.OUTPUT_GUARD,584)in c['watched']and(recipes.OUTPUT_GUARD,584)in c['write_ranges']
        assert (recipes.COUNT,4)in c['watched']and(recipes.COUNT,4)in c['write_ranges']
        assert len(c['authored_endpoint_order'])==len(c['authored_endpoint_observations'])==sum(c['expected_endpoint_counts'].values())
        for ep in c['endpoints']:
            token=ep['token'];n=c['expected_endpoint_counts'][token]
            assert len(ep['responses'])==len(c['expected_endpoint_arguments'][token])==c['authored_endpoint_order'].count(token)==n
    reset=cases[1]['helper_output_oracle'];assert reset['count']==2 and reset['changed_prefix']==[16,17]+list(range(4,16))
    inspected=0
    if args.report:
        rows={r['id']:r for r in json.loads(args.report.read_bytes())['cases_detail']}
        for c in cases:
            row=rows[c['id']];assert row['passed']and len(row['routes'])==7,c['id']
            snapshots=json.loads(json.dumps(c['authored_endpoint_observations']))
            for route in row['routes']:
                assert not route['errors']
                assert [e['token']for e in route['events']]==c['authored_endpoint_order']
                assert [e['observed']for e in route['events']]==snapshots
                assert int(route['logical_result']['value'],16)&255==c['expected_masked_result']
                inspected+=1
        assert inspected==42
    result=dict(schema='cohort7-query-repair-private-guard-v1',hashes_valid=True,original_packet_unchanged=True,
        cases=6,planned_invocations=42,watched_output_DWORDs=144,independently_authored_oracles=True,
        inspected_report_invocations=inspected,new_source_credit=0,new_entry_credit=0,
        replay_invoked=False,builds_run=False,host_execution=False,whole_game_equivalence=False)
    if args.output:args.output.write_bytes((json.dumps(result,indent=2)+'\n').encode())
    print(json.dumps(result))


if __name__=='__main__':main()
