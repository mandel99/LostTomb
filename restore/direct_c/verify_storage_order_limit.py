"""Offline native/C storage-limit callback and real two-button dispatch.

Actual 502780 and 425990 children execute. No host program or endpoint stub runs.
Compare ordered static writes, all original static data and the saved C ABI;
the callback has no logical EAX/flags result.
"""
from pathlib import Path
import hashlib
import itertools
import json
import re
import struct
import sys

ROOT = Path(__file__).resolve().parents[1]
sys.path[:0] = [str(ROOT / 'build/offline-verification-deps'), str(ROOT / 'direct_c')]
from native_test_pe import PE
from unicorn import Uc, UC_ARCH_X86, UC_MODE_32, UC_HOOK_CODE, UC_HOOK_MEM_WRITE
from unicorn.x86_const import (UC_X86_REG_EAX, UC_X86_REG_ECX, UC_X86_REG_EDX,
    UC_X86_REG_EBX, UC_X86_REG_ESP, UC_X86_REG_EBP, UC_X86_REG_ESI,
    UC_X86_REG_EDI, UC_X86_REG_EFLAGS, UC_X86_REG_EIP)
from capstone import Cs, CS_ARCH_X86, CS_MODE_32

PIN = 'fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
ENTRY, BUTTONS, SP, STOP = 0x406460, 0x5c81f8, 0x30008000, 0x31000000
STATIC, END = 0x560000, 0x126d000
REGS = [UC_X86_REG_EAX, UC_X86_REG_ECX, UC_X86_REG_EDX, UC_X86_REG_EBX,
        UC_X86_REG_ESP, UC_X86_REG_EBP, UC_X86_REG_ESI, UC_X86_REG_EDI]
SAVED = [0x91a2b3c4, 0x23456789, 0x81234567, 0x456789ab]


def digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def put(u, address, value, width=4):
    u.mem_write(address & 0xffffffff, (value & ((1 << (width * 8)) - 1)).to_bytes(width, 'little'))


def verify():
    original = ROOT / 'build/smoke-game/Pharaoh.exe'
    assert digest(original) == PIN
    native = PE(original)
    seed = bytes(native.data[STATIC - native.base:END - native.base])
    cache = json.loads((ROOT / 'metadata/native-function-audit-cache/batch-0000.json').read_bytes())
    entry = next(item for item in cache['entries'] if int(item['requested_address'], 16) == ENTRY)
    raw = b''.join(bytes.fromhex(item['bytes']) for item in entry['assembly']['items'])
    assert raw == native.data[ENTRY - native.base:ENTRY - native.base + 438]
    instructions = list(Cs(CS_ARCH_X86, CS_MODE_32).disasm(raw, ENTRY))
    points = {item.address for item in instructions}
    branches = {item.address: {int(item.op_str, 16), item.address + item.size}
                for item in instructions if item.mnemonic.startswith('j') and item.mnemonic != 'jmp'}
    assert len(points) == 123
    hits, edges = set(), set()
    artifacts, machines = {}, []
    paths = [('native', original), *[(name, ROOT / ('build/c-only/Release/' + filename)) for name, filename in
             [('game', 'PharaohRestoreC.exe'), ('diagnostic', 'restore_link_check.exe'),
              ('retained', 'restore_cohort3_retained.exe')]]]
    for label, path in paths:
        pe = native if label == 'native' else PE(path)
        assert pe.base <= STATIC and pe.base + len(pe.data) >= END
        u = Uc(UC_ARCH_X86, UC_MODE_32)
        u.mem_map(pe.base, (len(pe.data) + 4095) & ~4095)
        u.mem_write(pe.base, bytes(pe.data))
        u.mem_map(0x30000000, 0x10000)
        u.mem_map(STOP, 4096)
        symbols = {} if label == 'native' else dict(re.findall(r'\s_(\w+)\s+([0-9a-fA-F]{8})\s', path.with_suffix('.map').read_text()))
        resolve = lambda name, symbols=symbols: int(symbols[name], 16)
        state = dict(active=False, writes=[], helpers=0, last=None)

        def written(uc, access, address, size, value, state):
            if state['active']:
                state['writes'].append((address, size, value & ((1 << (8 * size)) - 1)))

        def helper(uc, pc, size, state):
            if state['active']:
                state['helpers'] += 1

        def covered(uc, pc, size, state):
            if not state['active']:
                return
            hits.add(pc)
            if state['last'] in branches:
                edges.add((state['last'], pc))
            state['last'] = pc

        u.hook_add(UC_HOOK_MEM_WRITE, written, state, begin=STATIC, end=END - 1)
        helper_pc = 0x502780 if label == 'native' else resolve('FUN_00502780')
        u.hook_add(UC_HOOK_CODE, helper, state, begin=helper_pc, end=helper_pc)
        if label == 'native':
            u.hook_add(UC_HOOK_CODE, covered, state, begin=ENTRY, end=ENTRY + len(raw) - 1)
        else:
            artifacts[label] = dict(path=path.relative_to(ROOT).as_posix(), sha256=digest(path),
                                    map_path=path.with_suffix('.map').relative_to(ROOT).as_posix(),
                                    map_sha256=digest(path.with_suffix('.map')))
        machines.append((label, u, state, resolve))

    scenarios = []
    for action, policy, limit in itertools.product([0, 1, 2, 0xffffffff], [0, 1, 255],
            [-32768, -1, 0, 799, 800, 1599, 1600, 2400, 3200, 32767]):
        scenarios.append(dict(action=action, policy=policy, limit=limit))
    for mode, position, kind in itertools.product([0, 1, 2, 3, 4], [0, 1, 10, 25, 0xffffffff], [70, 72, 193]):
        scenarios.append(dict(mode=mode, position=position, kind=kind, row=2, action=1, policy=1, limit=2400))
    for owner, row in itertools.product([0, 255], [0, 1, 29]):
        scenarios.append(dict(owner=owner, row=row, policy=0, action=0, limit=1600))
    scenarios.extend(dict(button=True, row=row, action=action, policy=row % 3,
                          limit=[0, 800, 1600, 2400, 3200][row % 5])
                     for row, action in itertools.product(range(30), [0, 1]))
    invocations = 0
    button_cases = 0
    for index, cfg in enumerate(scenarios):
        expected = None
        button = cfg.get('button', False)
        button_cases += bool(button)
        for label, u, state, resolve in machines:
            routes = ['native'] if label == 'native' else (['buttons'] if button else ['named', 'canonical', 'dispatch'])
            for route in routes:
                state.update(active=False, writes=[], helpers=0, last=None)
                u.mem_write(STATIC, seed)
                u.mem_write(0x30000000, b'\xa7' * 0x10000)
                building, owner, row = 7, cfg.get('owner', 1), cfg.get('row', 0)
                offset = building * 264
                for address, value, width in [(0xeadd6c, building, 4), (0x799a99 + offset, owner, 1),
                    (0x7999e8 + offset, cfg.get('kind', 70), 2), (0x799ab8 + offset, cfg.get('position', 1), 4),
                    (0xe38e68, cfg.get('mode', 0), 4), (0xf53e6c, 35, 4),
                    (0xd3b194, row, 4), (0xd3b198, cfg.get('action', 1), 4), (0xe38dca, 0x83, 1)]:
                    put(u, address, value, width)
                for resource in range(36):
                    put(u, 0xf53ddc + resource * 4, resource)
                    put(u, 0xf53e70 + resource * 4, resource)
                    put(u, 0x7900c2 + owner * 196 + resource, cfg.get('policy', 0), 1)
                    put(u, 0x7900ec + owner * 196 + resource * 2, cfg.get('limit', 1600), 2)
                    put(u, 0x790134 + owner * 196 + resource * 2, cfg.get('limit', 1600), 2)
                # The count is the DWORD immediately before the scrolling
                # resource list and remains native35.
                if button:
                    for address, value, width in [(0xe38e34, 8 if cfg['action'] == 1 else 25, 4),
                        (0xe38e30, 8, 4), (0xe38e03, 1, 1), (0xe38df8, 0, 1), (0xe38e00, 0, 1)]:
                        put(u, address, value, width)
                    args = [0, 0, BUTTONS + row * 72, 2]
                    pc = 0x425990 if label == 'native' else resolve('FUN_00425990')
                else:
                    args = [ENTRY] if route == 'dispatch' else []
                    pc = ENTRY if label == 'native' else resolve({'named': 'restore_storage_adjust_limit_00406460',
                        'canonical': 'FUN_00406460', 'dispatch': 'restore_menu_callback'}[route])
                for reg, value in zip(REGS, [0xdeadc0de, 0xa1b2c3d4, 0xfedcba98, SAVED[0], SP, *SAVED[1:]]):
                    u.reg_write(reg, value)
                u.reg_write(UC_X86_REG_EFLAGS, 0xad7)
                put(u, SP, STOP)
                for i, arg in enumerate(args):
                    put(u, SP + 4 + i * 4, arg)
                state['active'] = True
                u.emu_start(pc, STOP, count=2000000)
                state['active'] = False
                assert u.reg_read(UC_X86_REG_EIP) == STOP, (index, label, route, 'did not return')
                assert u.reg_read(UC_X86_REG_ESP) == SP + 4, (index, label, route, 'stack cleanup')
                assert [u.reg_read(REGS[i]) for i in [3, 5, 6, 7]] == SAVED, (index, label, route, 'nonvolatiles')
                if button:
                    assert u.reg_read(UC_X86_REG_EAX) == (1 if cfg['action'] == 1 else 2)
                answer = (tuple(state['writes']), state['helpers'], bytes(u.mem_read(STATIC, len(seed))))
                if expected is None:
                    expected = answer
                else:
                    assert answer == expected, (index, cfg, label, route, 'writes', expected[0], answer[0],
                                               'helper calls', expected[1], answer[1])
                invocations += 1
    missing = points - hits
    missing_edges = {(source, target) for source, targets in branches.items() for target in targets} - edges
    assert not missing and not missing_edges, ('coverage', sorted(map(hex, missing)), sorted(missing_edges))
    source_names = ['CMakeLists.txt', 'include/storage_order_actions.h', 'runtime/storage_order_actions.c',
        'runtime/menu_callbacks.c', 'runtime/small_button_input.c', 'src/unit_02246.c',
        'direct_c/native_test_pe.py', 'direct_c/verify_storage_order_limit.py',
        'metadata/storage-order-limit-rea-records.json', 'metadata/native-function-audit-cache/batch-0000.json']
    receipt = dict(schema='storage-order-limit-finite-comparison-v1', native_sha256=PIN,
        scenarios=len(scenarios), button_table_scenarios=button_cases, invocations=invocations,
        instructions_executed=len(hits), native_instructions=len(points), conditional_edges=len(edges),
        missing_instructions=[], missing_conditional_edges=[], failed_cases=0, compiled_images=artifacts,
        source_hashes={name: digest(ROOT / name) for name in source_names},
        controlled_endpoints=[], host_execution=False, whole_game_equivalence=False,
        qualification='Finite allocated/static single-threaded contexts. All123 callback instructions and both edges of each conditional executed. All60 original button records dispatched through real425990 and menu callbacks. Incidental callback EAX/volatile flags, faults, asynchronous observation and complete UI rendering are not certified.')
    output = ROOT / 'metadata/storage-order-limit-comparison.json'
    output.write_bytes((json.dumps(receipt, indent=2) + '\n').encode())
    print(json.dumps({key: receipt[key] for key in ['scenarios', 'button_table_scenarios', 'invocations',
        'instructions_executed', 'conditional_edges', 'failed_cases', 'host_execution']}))


if __name__ == '__main__':
    verify()
