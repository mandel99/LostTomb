"""Verify the private route repair; keep healthy native equivalence separate."""
from pathlib import Path
from datetime import datetime, timezone
import hashlib
import json
import subprocess
import xml.etree.ElementTree as ET

ROOT = Path(__file__).resolve().parents[1]
PHASE = ROOT / 'artifacts/menu_route_capacity_phase'
BUILD = ROOT / 'build/menu-route-capacity'
NUMERIC = ROOT / 'artifacts/menu_full_input_phase'
NS = {'m': 'http://schemas.microsoft.com/developer/msbuild/2003'}
PIN = 'fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'

def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()

def rel(path):
    return path.relative_to(ROOT).as_posix()

def read(path):
    return json.loads(path.read_text(encoding='utf-8-sig'))

inputs = read(PHASE / 'inputs.json')
assert not inputs['product_applied'] and not inputs['whole_route_cluster_proven']
assert sha(ROOT / inputs['source_reference']['path']) == inputs['source_reference']['sha256']
assert sha(ROOT / inputs['numeric_freeze']['path']) == inputs['numeric_freeze']['sha256']
numeric = read(ROOT / inputs['numeric_freeze']['path'])
for path, digest in numeric['files'].items():
    assert sha(ROOT / path) == digest, ('immutable numeric drift', path)

healthy = read(PHASE / 'healthy-emulation.json')
assert healthy['passed'] and healthy['complete_case_set']
assert healthy['checked'] == healthy['complete_cases'] == 5441
assert healthy['comparisons'] == 10882 and healthy['native_sha256'] == PIN
assert healthy['native_instruction_coverage'] == [411, 411]
assert healthy['owned_instruction_coverage'] == [396, 396]
assert healthy['qualified_conditional_outcomes'] == [88, 88]
assert not healthy['whole_menu_input_proven']
assert not healthy['capacity_repair_domain_original_comparison_claimed']
assert healthy['uncovered_native_instructions'] == []
for path, digest in healthy['source_hashes'].items():
    assert sha(ROOT / path) == digest, ('healthy proof stale', path)
for variant in healthy['variants'].values():
    assert sha(ROOT / variant['path']) == variant['sha256']
qualification = read(PHASE / 'healthy-edge-qualification.json')
assert qualification['excluded_edges'] == healthy['qualified_excluded_edges']
assert qualification['native_sha256'] == PIN
assert sha(PHASE / 'route-members-native.json') == qualification['native_packet_sha256']

sources = set(PHASE.glob('include/*.h')) | set(PHASE.glob('runtime/*.c'))
sources |= {PHASE / 'tests/capacity.c', NUMERIC / 'runtime/menu_full_input_numeric.c',
            NUMERIC / 'include/menu_full_input_numeric.h'}
tracked = sources | {Path(__file__), PHASE / 'inputs.json',
                    PHASE / 'project/CMakeLists.txt', PHASE / 'healthy-emulation.json'}
tracked |= {ROOT / path for path in numeric['files']}
tracked |= {ROOT / path for path in healthy['source_hashes']}
tracked |= {ROOT / variant['path'] for variant in healthy['variants'].values()}
tracked.add(ROOT / inputs['source_reference']['path'])
tracked.add(ROOT / inputs['numeric_freeze']['path'])
protected = {ROOT / 'CMakeLists.txt', ROOT / 'runtime/central_dispatch.c'}
protected |= {ROOT / f'src/unit_{unit:05d}.c' for unit in [68, *range(1868, 1876)]}
# The private proof must not silently install a partially qualified caller ABI.
protected = {path for path in protected if path.exists()}
before = {rel(path): sha(path) for path in tracked | protected}
variants = {}
for cfg, directory in [('Debug', 'c-only-debug'), ('Release', 'c-only')]:
    project = BUILD / 'menu_route_capacity_verify.vcxproj'
    library_project = BUILD / 'menu_route_capacity.vcxproj'
    for path, expected_sources in [
        (project, {PHASE / 'tests/capacity.c'}),
        (library_project, {PHASE / 'runtime/menu_route_capacity.c',
                           PHASE / 'runtime/menu_full_input_routes.c',
                           NUMERIC / 'runtime/menu_full_input_numeric.c'})]:
        tree = ET.parse(path)
        groups = [group for group in tree.findall('m:ItemDefinitionGroup', NS)
                  if "=='%s|Win32'" % cfg in group.get('Condition', '')]
        assert len(groups) == 1
        cc = groups[0].find('m:ClCompile', NS)
        get = lambda name: cc.findtext('m:' + name, default='', namespaces=NS)
        assert get('Optimization') == ('Disabled' if cfg == 'Debug' else 'MaxSpeed')
        assert get('RuntimeLibrary') == ('MultiThreadedDebug' if cfg == 'Debug' else 'MultiThreaded')
        assert get('WarningLevel') == 'Level4' and get('TreatWarningAsError') == 'true'
        actual = {Path(node.get('Include')).resolve()
                  for node in tree.findall('.//m:ClCompile[@Include]', NS)}
        assert actual == expected_sources
    binary = BUILD / cfg / 'menu_route_capacity_verify.exe'
    library = BUILD / cfg / 'menu_route_capacity.lib'
    data = ROOT / 'build' / directory / cfg / 'restore_data.lib'
    assert binary.stat().st_mtime_ns >= library.stat().st_mtime_ns
    for path in sources:
        assert binary.stat().st_mtime_ns >= path.stat().st_mtime_ns, ('stale capacity build', path)
    dependencies = {rel(path): sha(path) for path in [project, library_project, binary, library, data]}
    result = subprocess.run([str(binary)], cwd=ROOT, text=True, capture_output=True, timeout=60)
    (PHASE / (cfg.lower() + '-test-final.log')).write_text(result.stdout + result.stderr)
    assert result.returncode == 0, (cfg, result.stdout, result.stderr)
    observed = json.loads(result.stdout)
    assert observed == {'passed': True, 'planner_cases': 5616, 'builder_cases': 216,
                        'masked_CW': 24, 'whole_route_cluster_proven': False,
                        'original_game_executed': False}
    for path, digest in dependencies.items():
        assert sha(ROOT / path) == digest
    before.update(dependencies)
    variants[cfg] = observed | {'binary': rel(binary), 'binary_sha256': sha(binary)}
for path, digest in before.items():
    assert sha(ROOT / path) == digest, ('source changed during capacity verification', path)

report = {
    'format': 1, 'passed': True, 'generated_at': datetime.now(timezone.utc).isoformat(),
    'product_applied': False, 'whole_menu_input_proven': False,
    'whole_route_cluster_native_equivalence_claimed': False,
    'healthy_native_cases': 5441, 'healthy_native_C_comparisons': 10882,
    'owned_native_instructions': [396, 396], 'feasible_conditional_outcomes': [88, 88],
    'variants': variants, 'original_game_executed': False,
    'original_failing_allocation_inputs_executed': False,
    'intentional_repairs': [
        'Precompute the exact sample count using native binary32 lengths and the integer-distance termination predicate, including the original pre-segment early return.',
        'Retain the native allocation request when sufficient; enlarge only when the sampled path needs more capacity.',
        'Allocation failure leaves a null allocation, zero capacity and zero sample count, and returns failure.'
    ],
    'capacity_argument': {
        'prepared_domain': 'At most50 six-byte vertices with signed-WORD coordinates and BYTE step; stop_after is the original sum of maximum-axis deltas plus1.',
        'segment_samples': 'For positive finite length and nonzero step: floor(floor(binary32_length)/step)+1; otherwise0.',
        'length_bound': 'Signed-WORD endpoint differences bound the rounded length below2^17 under each of the24 masked control words.',
        'total_bound': 'At most49 segments times131072 samples; six bytes per sample fits uint32_t.',
        'test_independence': 'The reference iterates the original FCOM distance loop; the planner decodes binary32 and uses integer division.'
    },
    'limitations': [
        'Native comparison covers prepared well-allocated states only; allocation size/state differences in the repaired domain are intentional and are not called native equivalence.',
        'Native ECX receiver/RET cleanup and C explicit-receiver cdecl are separately checked; caller adaptation is still required before production application.',
        'The complete4D8310 input controller, physical menu rendering and gameplay remain unproved.'
    ],
    'protected_product_hashes': {rel(path): before[rel(path)] for path in sorted(protected)},
    'source_and_evidence_hashes': before
}
(PHASE / 'repair-verification.json').write_text(json.dumps(report, indent=2) + '\n')
# Product baselines and shared build outputs are historical context in the
# report. A later guarded integration may change them. Freeze the private
# proposal and immutable numeric dependency, not those mutable destinations.
frozen = {path: digest for path, digest in before.items()
          if path.startswith(('artifacts/menu_route_capacity_phase/',
                              'artifacts/menu_full_input_phase/'))
          or path == rel(Path(__file__))
          or path.startswith('build/menu-route-capacity/')}
frozen[rel(PHASE / 'repair-verification.json')] = sha(PHASE / 'repair-verification.json')
(PHASE / 'freeze-manifest.json').write_text(json.dumps({
    'format': 1, 'scope': 'Private route capacity repair plus separately qualified healthy native comparison.',
    'product_applied': False, 'whole_menu_input_proven': False, 'files': frozen
}, indent=2) + '\n')
print(json.dumps({'passed': True, 'healthy_cases': 5441, 'healthy_comparisons': 10882,
                  'planner_cases_per_build': 5616, 'builder_cases_per_build': 216,
                  'product_applied': False, 'whole_menu_input_proven': False}))
