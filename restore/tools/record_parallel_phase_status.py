"""Record component progress without accepting a failing city integration."""
from pathlib import Path
from datetime import datetime, timezone
import hashlib, json, re

ROOT = Path(__file__).resolve().parent.parent

def read(path):
    return json.loads((ROOT / path).read_text(encoding='utf-8-sig'))

def sha(path):
    return hashlib.sha256((ROOT / path).read_bytes()).hexdigest()

def stamp(path):
    return {'path': path, 'sha256': sha(path)}

binary = 'build/c-only-debug/Debug/restore_link_check.exe'
digest = sha(binary)
builds = []
for configuration in ['debug', 'release']:
    path = f'artifacts/menu-routes-{configuration}-build.log'
    log = (ROOT / path).read_text(errors='replace')
    count = re.findall(r'100% tests passed, 0 tests failed out of (\d+)', log)
    assert len(count) == 1 and 'C source build completed.' in log
    builds.append({'configuration': configuration, 'tests_passed': int(count[0]), 'log': stamp(path)})
rows = [json.loads(line) for line in (ROOT / 'artifacts/menu-native-refresh.log').read_text().splitlines() if line.startswith('{')]
assert len(rows) == len({row['suite'] for row in rows})
suites = []
for row in rows:
    name = row['suite']
    path = 'metadata/' + name.replace('_', '-') + '-emulation.json'
    proof = read(path)
    target = 'music' if name == 'music_manager' else name
    image = f'build/c-only-debug/Debug/restore_{target}_verify.exe'
    assert proof['passed'] and proof['checked'] == row['cases']
    assert proof['compiled_C_sha256'] == sha(image)
    for source, expected in proof['source_hashes'].items():
        assert sha(source) == expected, name + ' stale source: ' + source
    suites.append({**stamp(path), 'cases': proof['checked'], 'compiled_C': stamp(image)})
integrations = []
for name in ['menu-dialog', 'campaign-menu', 'mission-menu', 'mission-map', 'load-save-menu', 'high-scores-menu']:
    path = f'metadata/{name}-integration.json'
    proof = read(path)
    assert proof['passed'] and proof['child_exe_sha256'] == digest
    integrations.append(stamp(path))
baseline = read('artifacts/minimap-prior-frame-hashes.json')
assert len(baseline) == 20
assert all(sha(item['path']) == item['sha256'] for item in baseline)
pixel = read('metadata/minimap-menu-frame-regression.json')
assert pixel['passed'] and pixel['child_exe_sha256'] == digest
assert pixel['current_owned_C_images'] == 19 and len(pixel['current_baseline_images']) == 15
assert len(pixel['retained_historical_images']) == 5
city_path = 'metadata/city-confirm-team.json'
city = read(city_path)
assert city['child_exe_sha256'] == digest and city['profile_preserved'] and city['fixture_preserved']
city_passed = city.get('passed', False) and city.get('completed_city_frame_proven', False)
post_path = 'metadata/city-message-dismiss.json'
post = read(post_path) if (ROOT / post_path).exists() else {}
post_current = post.get('child_exe_sha256') == digest
post_passed = post_current and post.get('passed', False) and post.get('completed_gameplay_frame_proven', False)
report = {
    'format': 1, 'generated_at': datetime.now(timezone.utc).isoformat(),
    'kind': 'component-verification-with-explicit-runtime-status',
    'built_Debug_binary': stamp(binary), 'builds': builds,
    'native_cases_passed': sum(row['cases'] for row in rows), 'native_suites': suites,
    'owned_C_menu_scenarios_passed': 8, 'menu_integration_reports': integrations,
    'prior_framebuffers_compared': 20, 'changed_prior_framebuffers': [],
    'current_owned_C_framebuffers_verified': 19, 'recreated_prior_framebuffers': 15,
    'retained_historical_framebuffers': pixel['retained_historical_images'],
    'city_diagnostic': stamp(city_path), 'city_runtime_passed': bool(city_passed),
    'city_exceptions': city['exceptions'],
    'runtime_regression_present': not city_passed,
    'post_modal_diagnostic': stamp(post_path) if post_current else None,
    'post_modal_OK_and_frame_passed': bool(post_passed),
    'runtime_failure_present': not city_passed or (post_current and not post_passed),
    'whole_program_runtime_accepted': False,
    'complete_menu_reconstruction': False, 'standalone_gameplay_proven': False,
    'limitations': ['A component comparison can pass while a deeper real-C runtime path still fails.',
                    'This progress report does not replace the stricter menu-reconstruction checkpoint.',
                    'Only the compiled binaries and current source hashes in the listed native suites are included.'],
}
(ROOT / 'metadata/parallel-phase2-progress.json').write_text(json.dumps(report, indent=2) + '\n')
print(json.dumps({key: report[key] for key in ['native_cases_passed', 'owned_C_menu_scenarios_passed', 'city_runtime_passed', 'runtime_regression_present']}))
