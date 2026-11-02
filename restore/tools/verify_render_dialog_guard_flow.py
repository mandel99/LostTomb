"""Bounded native control-flow review of two discarded renderer returns.

The original executable is read as data and selected instructions execute only
inside Unicorn. This checks two call-site windows and a nine-instruction wrapper,
not complete dialog reconstruction or live rendering. The renderer and refresh
bodies are controlled endpoints. No product source is written or executed.

After complete producer reconstruction, --supersession validates the fresh
compiled proof and writes a separate report. The old 512-case source review
remains historical; its source-pattern checks are not relabeled current.
"""
from pathlib import Path
import argparse
import hashlib
import json
import re
import struct
import sys
import time
from repair_discarded_returns import verified_message_panel_proof, MESSAGE_PANEL_PROOF

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / 'build/offline-verification-deps'))
from unicorn import Uc, UC_ARCH_X86, UC_MODE_32, UC_HOOK_CODE, __version__
from unicorn.x86_const import (
    UC_X86_REG_EAX, UC_X86_REG_EBX, UC_X86_REG_EBP, UC_X86_REG_ECX,
    UC_X86_REG_EDX, UC_X86_REG_ESI, UC_X86_REG_EDI, UC_X86_REG_ESP,
    UC_X86_REG_EIP, UC_X86_REG_EFLAGS,
)

CANONICAL_SHA = 'fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
DOSSIER = 'metadata/render-dialog-discarded-returns-native.json'
SOURCES = ['src/unit_01622.c', 'src/unit_01623.c']
SELF = 'tools/verify_render_dialog_guard_flow.py'
SITES = [
    dict(source=SOURCES[0], function='004c5e60', entry=0x4c5f78,
         call=0x4c5f8f, redraw_compare=0x4c5f94, overwrite=0x4c5fae,
         stop=0x4c5fb3, final_eax=0x99),
    dict(source=SOURCES[1], function='004c6c70', entry=0x4c6d6b,
         call=0x4c6d83, redraw_compare=0x4c6d88, overwrite=0x4c6d96,
         stop=0x4c6d9b, final_eax=0x5c),
]


def supersession(report):
    historical_path = ROOT / 'metadata/render-dialog-guard-review.json'
    historical = json.loads(historical_path.read_text(encoding='utf-8-sig'))
    assert historical.get('passed') is True and historical.get('checked') == 512
    assert historical.get('original_sha256') == CANONICAL_SHA
    assert historical.get('complete_dialog_reconstruction') is False
    assert historical.get('source_hashes', {}).get(DOSSIER) == hashlib.sha256((ROOT / DOSSIER).read_bytes()).hexdigest(), 'Historical native dossier changed'
    proof, summary = verified_message_panel_proof(ROOT)
    current = dict(proof['source_hashes'])
    for relative in [SELF, 'tools/repair_discarded_returns.py', DOSSIER, MESSAGE_PANEL_PROOF, 'metadata/render-dialog-guard-review.json']:
        current[relative] = hashlib.sha256((ROOT / relative).read_bytes()).hexdigest()
    drift = [dict(source=source, historical_sha256=digest,
                  current_sha256=hashlib.sha256((ROOT / source).read_bytes()).hexdigest())
             for source, digest in historical.get('source_hashes', {}).items()
             if hashlib.sha256((ROOT / source).read_bytes()).hexdigest() != digest]
    report.update(format=1, kind='complete-message-panel-proof-supersession', passed=True,
                  checked=0, reconstructed_native_functions=8, complete_dialog_reconstruction=False,
                  historical_review=dict(path='metadata/render-dialog-guard-review.json',
                                         sha256=current['metadata/render-dialog-guard-review.json'],
                                         checked=512, source_patterns_current=False),
                  historical_source_drift=drift, source_shape_checks_performed=False,
                  superseding_native_proof=summary, source_hashes=current,
                  method='Keep the accepted 512-case native-window/source-shape review as historical evidence. Validate the complete handwritten message-panel proof, compiled image, every source hash, native exports and selected probes before superseding its old current-source pattern checks.',
                  limitations=proof.get('limitations', []),
                  original_game_executed=False, original_sha256=CANONICAL_SHA)


def review(original, report):
    image = original.read_bytes()
    assert hashlib.sha256(image).hexdigest() == CANONICAL_SHA
    dossier = json.loads((ROOT / DOSSIER).read_text(encoding='utf-8-sig'))
    assert dossier['native_sha256'] == CANONICAL_SHA
    source_checks = []
    for source in SOURCES:
        compact = re.sub(r'\s+', '', (ROOT / source).read_text())
        lhs = '#defineDAT_00d3b140(*(uint32_t*)0x00d3b140u)'
        redraw = '#defineDAT_00e38dca(*(byte*)0x00e38dcau)'
        condition = ("if((DAT_00d3b140==0)&&(FUN_004dddc0(),"
                     "DAT_00e38dca=='\\x01')){FUN_0051e990();}")
        assert lhs in compact, source + ': lhs must remain a dword'
        assert redraw in compact, source + ': redraw must remain a byte'
        assert condition in compact, source + ': renderer/short-circuit/refresh sequence differs'
        assert 'restore_unresolved_return(0x004dddc0u)' not in compact
        source_checks.append(dict(source=source, dword_lhs=True, byte_redraw=True,
                                  renderer_call_and_short_circuit_retained=True,
                                  discarded_return_guard_absent=True))
    report['source_checks'] = source_checks

    u16 = lambda p: struct.unpack_from('<H', image, p)[0]
    u32 = lambda p: struct.unpack_from('<I', image, p)[0]
    pe = u32(0x3c)
    opt = pe + 24
    table = opt + u16(pe + 20)
    base, size = u32(opt + 28), u32(opt + 56)
    vm = Uc(UC_ARCH_X86, UC_MODE_32)
    vm.mem_map(base, (size + 4095) & ~4095)
    vm.mem_write(base, image[:u32(opt + 60)])
    for i in range(u16(pe + 6)):
        row = table + i * 40
        vm.mem_write(base + u32(row + 12), image[u32(row + 20):u32(row + 20) + u32(row + 16)])
    stack = 0x34008000
    vm.mem_map(0x34000000, 0x10000)

    verified = []
    for name in ['004c5e60', '004c6c70']:
        function = dossier['functions'][name]
        assert function['info']['address'] == name
        assert function['info']['body_start'] == name
        assembly = function['assembly']
        assert not assembly['has_more'] and len(assembly['items']) == assembly['total']
        verified.append((name, assembly))
    wrapper = dossier['facade']
    assert not wrapper['has_more'] and len(wrapper['items']) == wrapper['total'] == 9
    verified.append(('004dddc0', wrapper))
    byte_count = 0
    for name, assembly in verified:
        for ins in assembly['items']:
            raw = bytes.fromhex(ins['bytes'])
            address = int(ins['address'], 16)
            assert bytes(vm.mem_read(address, len(raw))) == raw, ins
            byte_count += len(raw)
    report['native_byte_audit'] = dict(
        instruction_records=sum(len(a['items']) for _, a in verified),
        bytes=byte_count,
        records_by_function={name: len(a['items']) for name, a in verified},
        note='All retained complete caller-body and wrapper instruction bytes match the canonical PE. Only the listed call-site windows and wrapper execute below.')

    # EDI is the zero operand of the second call site's lhs comparison.
    prefix = dossier['functions']['004c6c70']['assembly']['items']
    edi_zero = next(i for i in prefix if i['address'] == '004c6c8b')
    assert edi_zero['bytes'] == '33ff' and edi_zero['instruction'] == 'XOR EDI,EDI'
    # Static prefix contains no later explicit EDI destination before the site.
    edi_writes = [i for i in prefix if 0x4c6c8d <= int(i['address'], 16) < 0x4c6d83
                  and re.match(r'\w+ EDI(?:,|$)', i['instruction'])
                  and not i['instruction'].startswith(('PUSH ', 'CMP ', 'TEST '))]
    assert not edi_writes, edi_writes

    read = lambda a, n=4: int.from_bytes(vm.mem_read(a, n), 'little')

    def write(address, value, length=4):
        vm.mem_write(address, (value & ((1 << (length * 8)) - 1)).to_bytes(length, 'little'))

    state = {}
    executed = set()

    def hook(cpu, address, length, user):
        executed.add(address)
        if address not in [0x430d30, 0x51e990]:
            assert (0x4dddc0 <= address and address + length - 1 <= 0x4ddddc) or any(
                s['entry'] <= address and address + length <= s['stop'] for s in SITES), hex(address)
            return
        sp = cpu.reg_read(UC_X86_REG_ESP)
        if address == 0x430d30:
            args = [read(sp + 4 + 4 * i) for i in range(3)]
            assert args == [0x1234, 0x5678, 0x9abc], args
            state['events'].append('render')
            write(0xe38dca, state['after'], 1)
        else:
            state['events'].append('refresh')
        cpu.reg_write(UC_X86_REG_EAX, state['eax'])
        cpu.reg_write(UC_X86_REG_ECX, 0xaaaa5555)
        cpu.reg_write(UC_X86_REG_EDX, 0xbbbb6666)
        cpu.reg_write(UC_X86_REG_ESP, sp + 4)
        cpu.reg_write(UC_X86_REG_EIP, read(sp))

    vm.hook_add(UC_HOOK_CODE, hook)
    checked = 0
    for site in SITES:
        for lhs in [0, 1, 0x100, 0xffffffff]:
            for initial in [0, 1, 2, 255]:
                for after in [0, 1, 2, 255]:
                    for eax in [0, 1, 0x12345678, 0xffffffff]:
                        report['active_case'] = dict(call=f"0x{site['call']:08x}", lhs=lhs,
                                                   initial_redraw=initial, renderer_redraw=after, renderer_eax=eax)
                        state.clear()
                        state.update(after=after, eax=eax, events=[])
                        for address, value in [(0xd3b140, lhs), (0xd3b124, 0x99),
                                               (0xd3b160, 0x20), (0xd3b15c, 0x40),
                                               (0xbca2bc, 0x1234), (0xbca2c0, 0x5678), (0xbca2d8, 0x9abc)]:
                            write(address, value)
                        # Poison neighboring bytes to distinguish BYTE from DWORD equality.
                        vm.mem_write(0xe38dca, bytes([initial, 0xa5, 0xb6, 0xc7]))
                        vm.mem_write(stack - 128, b'\xcc' * 256)
                        saved = [(UC_X86_REG_EBX, 0x11223344), (UC_X86_REG_EBP, 0x22334455),
                                 (UC_X86_REG_ESI, 0x33445566), (UC_X86_REG_EDI, 0)]
                        for reg, value in saved + [(UC_X86_REG_ESP, stack), (UC_X86_REG_ECX, 0x20),
                                                  (UC_X86_REG_EDX, 0x40), (UC_X86_REG_EFLAGS, 2)]:
                            vm.reg_write(reg, value)
                        vm.emu_start(site['entry'], site['stop'], count=100)
                        expected = [] if lhs else ['render'] + (['refresh'] if after == 1 else [])
                        assert state['events'] == expected, (state['events'], expected)
                        assert vm.reg_read(UC_X86_REG_EIP) == site['stop']
                        assert vm.reg_read(UC_X86_REG_EAX) == site['final_eax']
                        assert vm.reg_read(UC_X86_REG_ESP) == stack
                        assert all(vm.reg_read(reg) == value for reg, value in saved)
                        assert bytes(vm.mem_read(stack, 128)) == b'\xcc' * 128
                        assert bytes(vm.mem_read(stack - 128, 96)) == b'\xcc' * 96
                        assert bytes(vm.mem_read(0xe38dca + 1, 3)) == bytes([0xa5, 0xb6, 0xc7])
                        checked += 1
                        report['checked'] = checked
    assert checked == 512
    report.pop('active_case', None)
    report['executed_native_instruction_addresses'] = [f'0x{a:08x}' for a in sorted(executed)
                                                       if a not in [0x430d30, 0x51e990]]
    report['passed'] = True


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--original', type=Path, default=ROOT / 'build/smoke-game/Pharaoh.exe')
    parser.add_argument('--report', type=Path)
    parser.add_argument('--supersession', action='store_true', help='Validate the complete current message-panel proof and preserve the historical 512-case report')
    args = parser.parse_args()
    historical_path = ROOT / 'metadata/render-dialog-guard-review.json'
    output_path = args.report or (ROOT / 'metadata/render-dialog-guard-supersession.json' if args.supersession else historical_path)
    if args.supersession:
        if output_path.resolve() == historical_path.resolve():parser.error('Supersession must not overwrite the historical 512-case report')
        report = dict(passed=False, checked=0, kind='complete-message-panel-proof-supersession')
        try:supersession(report)
        except Exception as error:report['error'] = f'{type(error).__name__}: {error}'
        output_path.resolve().write_text(json.dumps(report, indent=2) + '\n')
        print(json.dumps({key: report.get(key) for key in ['passed', 'checked', 'historical_review', 'superseding_native_proof', 'error']}, indent=2))
        return 0 if report['passed'] else 1
    if output_path.resolve() == historical_path.resolve() and any('DAT_00d3b140' not in (ROOT / source).read_text() for source in SOURCES):
        parser.error('Producer sources were completely reconstructed; use --supersession. The historical 512-case report is preserved.')
    report = dict(
        format=1, kind='bounded-native-callsite-control-flow-review', passed=False, checked=0,
        reconstructed_native_functions=0, complete_dialog_reconstruction=False,
        original_game_executed=False, original_sha256=CANONICAL_SHA, unicorn_version=__version__,
        source_hashes={p: hashlib.sha256((ROOT / p).read_bytes()).hexdigest()
                       for p in SOURCES + [DOSSIER, SELF]},
        method='Verify current two C conditions and widths by exact normalized source patterns; audit every retained caller/wrapper instruction byte against the canonical PE; execute both original call-site windows and original nine-instruction wrapper in Unicorn for 512 branch scenarios. Compare calls to the source expression control-flow model, with arbitrary controlled renderer EAX values.',
        cases=dict(lhs_dword=[0, 1, 0x100, 0xffffffff], initial_redraw_byte=[0, 1, 2, 255],
                   renderer_redraw_byte=[0, 1, 2, 255], renderer_eax=[0, 1, 0x12345678, 0xffffffff]),
        sites=[{key: f'0x{value:08x}' if isinstance(value, int) else value
                for key, value in site.items()} for site in SITES],
        conclusions=[
            'Renderer call is short-circuited by the complete dword at 0x00d3b140.',
            'Refresh depends only on the post-render byte at 0x00e38dca equaling one.',
            'Renderer EAX is overwritten without a read at 0x004c5fae / 0x004c6d96.',
            'Removing the two nested unresolved-return guards preserves the relevant native dataflow.',
        ],
        limitations=[
            'This is bounded native control-flow and static source-shape evidence, not complete dialog reconstruction or a compiled-C differential.',
            'FUN_00430d30 renderer and FUN_0051e990 refresh bodies are controlled endpoints; their drawing, resource access, and other side effects are outside this review.',
            'The second window begins after the original XOR EDI,EDI. The zero EDI invariant is checked in the retained prefix; preservation across prefix callees relies on the native callee-saved ABI.',
            'No original game process or product executable is launched; no product source is edited by this tool.',
        ],
    )
    started = time.monotonic()
    try:
        review(args.original.resolve(), report)
    except Exception as error:
        report['error'] = f'{type(error).__name__}: {error}'
    report['elapsed_seconds'] = round(time.monotonic() - started, 3)
    output_path.resolve().write_text(json.dumps(report, indent=2) + '\n')
    print(json.dumps({key: report.get(key) for key in ['passed', 'checked', 'native_byte_audit', 'error']}, indent=2))
    return 0 if report['passed'] else 1


if __name__ == '__main__':
    sys.exit(main())
