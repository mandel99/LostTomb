"""Offline QuickTime support comparison; never launches a host executable."""
from pathlib import Path
import argparse, copy, datetime, hashlib, json, re, sys
sys.dont_write_bytecode = True
OUT = Path(__file__).resolve().parent
ROOT = next(p for p in OUT.parents if (p/'direct_c/cohort3_replay/registry.py').exists())
sys.path[:0] = [str(OUT), str(ROOT/'direct_c/cohort3_replay'), str(ROOT/'direct_c')]
from registry import load, save, symbol
import core_cases, window_cases


def sha(path):
    return hashlib.sha256(Path(path).read_bytes()).hexdigest()


def source_body(data, name):
    matches = list(re.finditer(rb'(?m)^[^\r\n;{}]*\b'+re.escape(name.encode())+rb'\([^;{}]*\)\s*\{', data))
    assert len(matches) == 1, (name, len(matches))
    start = matches[0].start()
    token = re.compile(rb'/\*[\s\S]*?\*/|//[^\r\n]*|"(?:\\.|[^"\\])*"|\x27(?:\\.|[^\x27\\])*\x27|[{}]')
    depth = 0
    for t in token.finditer(data, matches[0].end()-1):
        if t.group() == b'{': depth += 1
        elif t.group() == b'}':
            depth -= 1
            if depth == 0: return data[start:t.end()]
    raise AssertionError(('unterminated source', name))


def bind(base):
    p = json.loads((OUT/'protocol.json').read_bytes())
    for name, expected in p['guarded_files'].items():
        assert sha(ROOT/name) == expected, name
    assert base['native'] == p['native']
    assert base['compiled_images'] == p['compiled_images']
    from native_test_pe import PE
    pe = PE(ROOT/p['native']['path'])
    maps = {k:(ROOT/v['map_path']).read_text() for k,v in p['compiled_images'].items()}
    evidence = json.loads((OUT/'native-evidence.json').read_bytes())
    ev = {int(m['requested_address'],16):m for m in evidence['members']}
    for span in evidence['bounded_data_and_helper_spans']:
        at = span['address']-pe.base
        raw = bytes.fromhex(span['hex'])
        assert pe.data[at:at+len(raw)] == raw
    result = copy.deepcopy(base)
    for c in p['members']:
        a = int(c['address'],16)
        if a in ev:
            e = ev[a]
            assert e['status'] == 'exact_ghidra_function' and not e['assembly']['has_more']
            cursor = a; chunks = []
            for i in e['assembly']['items']:
                raw = bytes.fromhex(i['bytes'])
                assert int(i['address'],16) == cursor
                assert pe.data[cursor-pe.base:cursor-pe.base+len(raw)] == raw
                chunks.append(raw); cursor += len(raw)
            assert hashlib.sha256(b''.join(chunks)).hexdigest() == c['native_bytes_sha256']
        else:
            # This historical WNDPROC extent includes branch tables/data. It is
            # a pinned raw extent, never a fabricated Ghidra instruction list.
            cursor = c['native_end_inclusive']+1
            assert hashlib.sha256(pe.data[a-pe.base:cursor-pe.base]).hexdigest() == c['native_bytes_sha256']
        body = source_body((ROOT/c['source']).read_bytes(),c['source_definition_symbol'])
        assert hashlib.sha256(body).hexdigest() == c['body_sha256']
        providers = {}
        for label,t in maps.items():
            named = symbol(t,c['provider_symbol'])
            assert named and c['owner_object'] in named['owner'], (label,c['provider_symbol'])
            # The named Qt provider IS its canonical FUN export. Do not execute
            # the identical PC twice as if it were a second implementation.
            providers[label] = dict(named=dict(named,symbol=c['provider_symbol']),canonical=None)
        assert not any(int(m['address'],16)==a for m in result['members'])
        result['members'].append(dict(address=hex(a),name=c['source_definition_symbol'],
            source=c['source'],source_definition_symbol=c['source_definition_symbol'],
            algorithm_source=c.get('algorithm_source',c['source']),source_sha256=sha(ROOT/c['source']),
            body_sha256=c['body_sha256'],native_ranges=[dict(start=hex(a),end=hex(cursor-1))],
            native_ret_immediates=c['native_ret_immediates'],logical_signature=c['logical_signature'],
            providers=providers,rule=None,case_status='supplemental_authored_recipes',
            supplemental_comparison_only=True,new_source_credit=0,new_entry_credit=0))
    result['comparison_entries'] = len(result['members'])
    result['supplemental_comparison'] = dict(schema='quicktime-support-v1',new_source_credit=0,
        new_entry_credit=0,public_selected_entries_unchanged=True,protocol_sha256=sha(OUT/'protocol.json'))
    return result


def inspect(registry, cases, report):
    assert report['inputs_stable'] and report['failed_cases']==0
    rows = {r['id']:r for r in report['cases_detail']}
    count = 0
    for c in cases:
        row = rows[c['id']]
        assert row['passed'] and len(row['routes']) == c['expected_invocations'], c['id']
        expected_order = c.get('authored_endpoint_order')
        for r in row['routes']:
            assert not r['errors']
            if expected_order is not None:
                assert [e['token'] for e in r['events']] == expected_order
                assert [e['observed'] for e in r['events']] == json.loads(json.dumps(c['authored_endpoint_observations']))
            for token,expected in c['expected_endpoint_arguments'].items():
                assert [e['arguments'] for e in r['events'] if e['token']==token] == json.loads(json.dumps(expected)), (c['id'],token)
            for token,expected in c.get('expected_endpoint_registers',{}).items():
                assert [e['registers'] for e in r['events'] if e['token']==token] == expected
            if 'expected_selector_sequence' in c:
                assert [e['registers']['eax'] for e in r['events'] if e['token']=='qt-dispatch'] == c['expected_selector_sequence']
            if c.get('expected_opaque_argument_words') is not None:
                assert r['physical_argument_words'] == c['expected_opaque_argument_words']
            if c['result_mask'] is not None:
                wanted = c.get('expected_masked_result',c.get('expected_eax'))
                assert int(r['logical_result']['value'],16)&c['result_mask'] == wanted
            count += 1
    return dict(schema='quicktime-support-authored-checks-v1',cases=len(cases),invocations=count,
        entries=len({c['entry'] for c in cases}),all_authored_checks_passed=True,
        host_execution=False,whole_game_equivalence=False)


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--run',action='store_true')
    ap.add_argument('--scope',choices=['core','window','all'],default='all')
    ap.add_argument('--output-dir',type=Path,default=ROOT/'build/actor-animal-following/cohort7-quicktime-support-runs')
    args = ap.parse_args()
    base = load(True); registry = bind(base)
    cases = (core_cases.cases(registry) if args.scope!='window' else []) + (window_cases.definitions(registry) if args.scope!='core' else [])
    for c in cases:
        member = next(m for m in registry['members'] if int(m['address'],16)==c['entry'])
        c['expected_invocations'] = 1+sum(bool(v['named']) for v in member['providers'].values())
    if not args.run:
        print(json.dumps(dict(cases=len(cases),entries=len({c['entry'] for c in cases}),invocations=sum(c['expected_invocations'] for c in cases),execution=False)))
        return
    import replay
    directory = args.output_dir/datetime.datetime.now(datetime.timezone.utc).strftime('%Y%m%d-%H%M%S-%f')
    directory.mkdir(parents=True,exist_ok=False)
    save(directory/'registry.json',registry)
    original = replay.load
    replay.load = lambda require_retained=True: bind(original(require_retained))
    try:
        report = replay.execute(registry,cases,directory)
    finally:
        replay.load = original
    print(json.dumps({k:report[k] for k in ['cases','entries_compared','invocations','failed_cases','inputs_stable']}),flush=True)
    if report['failed_cases'] or not report['inputs_stable']:
        print(str(directory/'result.json'));sys.exit(1)
    checks = inspect(registry,cases,report)
    save(directory/'authored-checks.json',checks)
    print(json.dumps(dict(result=str(directory/'result.json'),authored_checks=checks)))


if __name__=='__main__': main()
