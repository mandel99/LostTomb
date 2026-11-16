"""Offline startup inventory. No game execution; no equivalence claim."""
import hashlib
import json
import pathlib
import re
import struct

ROOT = pathlib.Path(__file__).resolve().parents[1]
EVIDENCE = ROOT / 'evidence'
PIN = 'fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
ORIGINAL = pathlib.Path(r'C:\Program Files (x86)\GOG.com\Pharaoh\Pharaoh.exe')
EXPORT = EVIDENCE / 'ghidra-typed-pseudocode-20261116-after-java-types.c'


def digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def main():
    binary = ORIGINAL.read_bytes()
    assert hashlib.sha256(binary).hexdigest() == PIN, 'Wrong original executable'
    pe = struct.unpack_from('<I', binary, 60)[0]
    entry = 0x400000 + struct.unpack_from('<I', binary, pe + 40)[0]
    text = EXPORT.read_text(encoding='utf-8')
    markers = list(re.finditer(r'/\* ([0-9a-f]{8}) [^\n]*\*/', text))
    bodies = {int(m[1], 16): text[m.end():markers[i+1].start() if i+1 < len(markers) else len(text)]
              for i, m in enumerate(markers)}
    owners = {int(f['address'], 16): f for f in json.loads((EVIDENCE / 'function-map.json').read_text())['functions']}
    stages = [
        (entry, 'PE/CRT entry'), (0x413ed0, 'Application startup and message loop'),
        (0x4147f0, 'Settings defaults, INI and INF'), (0x4cec40, 'Language catalogs'),
        (0x4268c0, 'Startup drive probe'), (0x4149d0, 'Default names'),
        (0x4cf960, 'Window, geometry and platform initialization'),
        (0x4d0060, 'Graphics device initialization'), (0x426b40, 'DirectDraw enable'),
        (0x414420, 'Asset setup and loading progress'),
        (0x540c90, 'Building models'), (0x540ed0, 'Figure models'),
        (0x541160, 'Tax/sentiment models'), (0x41ae40, 'Campaign file'),
        (0x41ba20, 'Campaign period availability'),
        (0x41ba80, 'Played campaign query'), (0x41bab0, 'Any campaign period open'),
        (0x541310, 'Trade recommendations'), (0x44efa0, 'Event reason phrases'),
        (0x5394f0, 'Sound initialization'), (0x540b50, 'State initialization before intro'),
        (0x51e930, 'Sidebar geometry'), (0x4cf8e0, 'Speed measurement'),
        (0x413580, 'Bink intro playback'), (0x413690, 'Bink frame loop'),
        (0x4143c0, 'Surface/frame gate'), (0x4d6b60, 'Main frame dispatcher'),
        (0x4d8310, 'Input and window transitions'), (0x520e00, 'Logo screen'),
        (0x4180c0, 'Front menu rendering'), (0x417ff0, 'Front menu actions'),
        (0x4cb6a0, 'Open main menu'), (0x4cb4d0, 'Main menu background'),
        (0x4cb5a0, 'Main menu foreground')]
    rows = []
    for address, name in stages:
        owner = owners.get(address)
        rows.append(dict(address=f'0x{address:08X}', stage=name,
            native_body_available=address in bodies,
            clean_owner=owner['name'] if owner else None,
            clean_source=owner['source'] if owner else None,
            host_connected=None if address == entry else False))
    bootstrap = ROOT / 'game/platform/windows_bootstrap.c'
    host_calls = sorted(set(re.findall(r'\b(pharaoh_\w+)\s*\(', bootstrap.read_text())))
    # This audit's negative reachability conclusion is valid only for the
    # current host, which has no game calls. Wiring requires a new graph audit.
    assert not host_calls, 'Host changed: perform an updated startup graph audit'
    direct_calls = {}
    for address in (entry, 0x413ed0, 0x4cf960, 0x414420, 0x4143c0, 0x4d6b60):
        body = bodies[address]
        direct_calls[f'0x{address:08X}'] = re.findall(r'\bFUN_([0-9a-f]{8})\s*\(', body)
    startup = bodies[0x413ed0]
    assert startup.index('FUN_00540b50(') < startup.index('FUN_00413580(')
    report = dict(status='incomplete_not_end_to_end_verified', game_launched=False,
        original_sha256=PIN, pe_entry=f'0x{entry:08X}',
        evidence_sha256=digest(EXPORT), bootstrap_sha256=digest(bootstrap),
        function_map_sha256=digest(EVIDENCE / 'function-map.json'),
        host_game_calls=host_calls, stages=rows,
        rea_startup_function_dossier=dict(
            bundle='rea-app-startup-044.json',
            bundle_sha256=digest(EVIDENCE / 'rea-app-startup-044.json'),
            evidence_id='ev_581816a555c45f24663157d0a98af494c5e7f7ca3c2a8f3f217b81a5aa03e75d',
            native_body_ranges=[['0x00413ED0','0x00413F9D'],['0x00414000','0x00414394'],['0x0041439E','0x004143BD']],
            non_contiguous=True, direct_callees=40),
        native_direct_calls_in_lexical_order=direct_calls,
        findings=[
            'Current host only registers/creates/shows a 960x640 window and dispatches Windows messages.',
            'No clean settings, text, asset, graphics, audio, video, frame or menu owner is invoked by the current host.',
            'Native 0x00540B50 is called by application startup before intro video; excluding it as level-only was incorrect.',
            'Native idle loop calls the frame gate when active, or WaitMessage otherwise; current host only uses GetMessage.',
            'Rendering owners alone do not supply menu opening, input handling, transition state or backend implementations.',
            'Existing SG3 and focused C tests are component evidence, not evidence of the complete startup cycle.'
        ], limitations=[
            'The rebuilt EXE has compiler-generated CRT startup; absence of a mapped original CRT owner is not a missing host entry point.',
            'The original drive probe is recorded as native behavior, not a requirement to reintroduce a mandatory CD check.',
            'Selected stages are an audit index, not an exhaustive transitive call graph.',
            'Native calls listed in lexical order are not a runtime trace; conditionals, indirect calls and external APIs need separate coverage.',
            'Owner presence does not prove behavior, backend integration or that the linker retained the owner in the EXE.',
            'No original or restored host was run; no end-to-end native replay was performed.'
        ], reference_comparison=dict(
            commit='a012b1a137b1d5307a2382bfea8efacbbc8dbe43',
            urls=['https://github.com/dalerank/tutankhamun/blob/a012b1a137b1d5307a2382bfea8efacbbc8dbe43/src/app/startup.c',
                  'https://github.com/dalerank/tutankhamun/blob/a012b1a137b1d5307a2382bfea8efacbbc8dbe43/src/game/game/startup_load.c'],
            observations=['Reference corroborates settings/text/assets/model ordering and early game_state_init.',
                          'Reference app_startup uses a no-display platform setup and adds external config loading; it is not a 1:1 WinMain replacement.'],
            implementation_copied=False))
    path = EVIDENCE / 'application-startup-audit.json'
    path.write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(json.dumps(dict(status=report['status'], stages=len(rows),
        stages_with_clean_owner=sum(r['clean_owner'] is not None for r in rows),
        host_game_calls=host_calls, report=str(path))))


if __name__ == '__main__':
    main()
