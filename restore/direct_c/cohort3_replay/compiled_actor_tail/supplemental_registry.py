"""Temporary comparison-only bindings; never edits the public source ledger."""
from pathlib import Path
from contextlib import contextmanager
import copy, hashlib, json, re, sys
sys.dont_write_bytecode = True
OUT = Path(__file__).resolve().parent
ROOT = OUT.parents[2]


def digest(path):
    return hashlib.sha256(Path(path).read_bytes()).hexdigest()


def checked_inputs():
    manifest = json.loads((OUT / 'manifest.json').read_bytes())
    for item in manifest['files']:
        assert digest(OUT / item['path']) == item['sha256'], item['path']
    p = json.loads((OUT / 'protocol.json').read_bytes())
    for name, expected in p['strict_source_guards'].items():
        assert digest(ROOT / name) == expected, ('source changed', name)
    assert digest(ROOT / p['native']['path']) == p['native']['sha256']
    for label, image in p['compiled_images'].items():
        for key, hash_key in [('path', 'sha256'), ('map_path', 'map_sha256')]:
            assert digest(ROOT / image[key]) == image[hash_key], (label, key)
    return p


def source_body(data, name):
    matches = list(re.finditer(rb'(?m)^[^\r\n;{}]*\b' +
        re.escape(name.encode()) + rb'\([^;{}]*\)\s*\{', data))
    assert len(matches) == 1, ('source definition', name, len(matches))
    start = matches[0].start()
    token = re.compile(rb'/\*[\s\S]*?\*/|//[^\r\n]*|"(?:\\.|[^"\\])*"|\x27(?:\\.|[^\x27\\])*\x27|[{}]')
    depth = 0
    for t in token.finditer(data, matches[0].end() - 1):
        if t.group() == b'{':
            depth += 1
        elif t.group() == b'}':
            depth -= 1
            if depth == 0:
                return data[start:t.end()]
    raise AssertionError(('unterminated source definition', name))


def supplemental_members(base_registry=None):
    p = checked_inputs()
    sys.path[:0] = [str(ROOT / 'direct_c/cohort3_replay'), str(ROOT / 'direct_c')]
    from registry import symbol
    from native_test_pe import PE
    native = PE(ROOT / p['native']['path'])  # Read mapped PE bytes offline.
    evidence = {x['requested_address']: x for x in
        json.loads((OUT / 'native-evidence.json').read_bytes())['members']}
    if base_registry is not None:
        assert base_registry['native'] == p['native']
        assert set(base_registry['compiled_images']) == set(p['compiled_images'])
        for label, image in p['compiled_images'].items():
            assert all(base_registry['compiled_images'][label][k] == image[k]
                for k in ('path', 'map_path', 'sha256', 'map_sha256'))
    maps = {label: (ROOT / image['map_path']).read_text()
            for label, image in p['compiled_images'].items()}
    rows = []
    for contract in p['members']:
        a = int(contract['address'], 16)
        n = evidence[f'{a:08x}']
        raw = bytes.fromhex(''.join(i['bytes'] for i in n['assembly']['items']))
        cursor = a
        for instruction in n['assembly']['items']:
            assert int(instruction['address'], 16) == cursor
            cursor += len(bytes.fromhex(instruction['bytes']))
        assert cursor - a == contract['native_bytes']
        assert hashlib.sha256(raw).hexdigest() == contract['native_bytes_sha256']
        assert raw == native.data[a-native.base:cursor-native.base]
        source = (ROOT / contract['source']).read_bytes()
        body = source_body(source, contract['source_definition_symbol'])
        providers = {}
        for label, text in maps.items():
            named = symbol(text, contract['provider_symbol'])
            expected=contract['available_images'].get(label,False)
            assert bool(named)==expected,(label,contract['provider_symbol'],'provider availability changed')
            if named:
                assert contract['owner_object'] in named['owner'], (label, contract['provider_symbol'])
                named = dict(named, symbol=contract['provider_symbol'])
            canonical = None
            if contract['canonical_symbol'] is not None:
                canonical = symbol(text, contract['canonical_symbol'])
                assert bool(canonical)==contract.get('canonical_images',contract['available_images']).get(label,False)
                if canonical:
                    assert contract['owner_object'] in canonical['owner']
                    canonical = dict(canonical, symbol=contract['canonical_symbol'])
            else:
                assert symbol(text, f'FUN_{a:08x}') is None
            providers[label] = dict(named=named, canonical=canonical)
        if not contract['current_replay_applicable']:
            continue
        rows.append(dict(address=hex(a), name=contract['source_definition_symbol'],
            source=contract['source'], source_definition_symbol=contract['source_definition_symbol'],
            algorithm_source=contract['source'], source_sha256=digest(ROOT / contract['source']),
            body_sha256=hashlib.sha256(body).hexdigest(),
            native_ranges=[dict(start=hex(a), end=hex(cursor-1))],
            native_ret_immediates=contract['native_ret_immediates'],
            logical_signature=contract['logical_signature'], providers=providers,
            rule=None, case_status='private_supplemental_authored_cases_not_executed',
            supplemental_comparison_only=True, new_source_credit=0, new_entry_credit=0,
            provider_adapter=contract['provider_adapter']))
    return rows


def extend_registry(base_registry):
    assert not any(m.get('supplemental_comparison_only') for m in base_registry['members']), 'already extended'
    additions = supplemental_members(base_registry)
    original = {int(m['address'], 16) for m in base_registry['members']}
    assert not original.intersection(int(m['address'], 16) for m in additions)
    result = copy.deepcopy(base_registry)
    result['members'].extend(additions)
    # Public source-entry accounting stays exactly unchanged. These five are
    # temporary comparison rows for already-present C, never selected entries.
    result['source_entries'] = base_registry['source_entries']
    result['comparison_entries'] = len(result['members'])
    result['supplemental_comparison'] = dict(
        schema='cohort7-actor-tail-temporary-registry-extension-v1',
        original_selected_source_entries=base_registry['source_entries'],
        supplemental_existing_C_members=len(additions), new_source_credit=0, new_entry_credit=0,
        public_ledger_written=False, manifest_sha256=digest(OUT / 'manifest.json'))
    return result


@contextmanager
def temporary_registry(replay_module, base_registry=None):
    """Keep the existing replay's end-of-run load(True) integrity check valid.

    Root may pass its already-loaded base registry. The local loader binding
    is restored on exit; no public Python source or ledger is changed.
    """
    original_load = replay_module.load
    initial = original_load(True) if base_registry is None else base_registry
    replay_module.load = lambda require_retained=True: extend_registry(
        original_load(require_retained))
    try:
        yield extend_registry(initial)
    finally:
        replay_module.load = original_load
