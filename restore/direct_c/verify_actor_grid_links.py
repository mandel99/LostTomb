"""Compare the two public actor grid-list bodies with the pinned native PE."""
from pathlib import Path
import hashlib, itertools, json, struct, sys

ROOT = Path(__file__).resolve().parents[1]
sys.path[:0] = [str(ROOT / 'build/offline-verification-deps'), str(ROOT / 'direct_c')]
from native_test_pe import PE
from unicorn import Uc, UC_ARCH_X86, UC_MODE_32, UC_HOOK_CODE, UC_HOOK_MEM_WRITE
from unicorn.x86_const import *
from capstone import Cs, CS_ARCH_X86, CS_MODE_32

original = ROOT / 'build/smoke-game/Pharaoh.exe'
inputs = [Path(__file__), Path(sys.argv[1]), ROOT / 'metadata/actor-walking-grid-native.json',
          ROOT / 'runtime/menu_drag_actor_walk_link.c', ROOT / 'runtime/menu_drag_actor_walk_unlink.c',
          ROOT / 'runtime/actor_walking_facades.c', ROOT / 'include/actor_walking.h']
before = [hashlib.sha256(p.read_bytes()).digest() for p in inputs]
assert hashlib.sha256(original.read_bytes()).hexdigest() == 'fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
native, candidate = PE(original), PE(sys.argv[1])
packet = json.loads((ROOT / 'metadata/actor-walking-grid-native.json').read_text())
assert packet['native_sha256'] == hashlib.sha256(original.read_bytes()).hexdigest()
functions, instructions, branches = {}, {}, set()
cs = Cs(CS_ARCH_X86, CS_MODE_32)
for fn in [0x4b52f0, 0x4b5420]:
    body = packet['functions'][f'{fn:08x}']['assembly']
    assert not body['has_more']
    functions[fn] = set()
    for item in body['items']:
        address, raw = int(item['address'], 16), bytes.fromhex(item['bytes'])
        assert native.data[address-native.base:address-native.base+len(raw)] == raw
        instructions[address] = raw
        functions[fn].add(address)
        decoded = next(cs.disasm(raw, address))
        if decoded.mnemonic.startswith('j') and decoded.mnemonic != 'jmp':
            branches.update([(address, address+decoded.size), (address, int(decoded.op_str, 16))])
assert [len(functions[fn]) for fn in functions] == [55, 62]
assert native.data[0x51f780-native.base] == 0xc3
SP, STOP = 0x30010000, 0x31000000
regions = [(0xbcd5e8, 2000*388+32), (0xb350cc, 51984*2+32)]
regids = [UC_X86_REG_EBX, UC_X86_REG_EBP, UC_X86_REG_ESI, UC_X86_REG_EDI]
preserved = [0x12345678, 0x87654321, 0x55aabbcc, 0xccbbaa55]
hits, edges, runs = set(), set(), 0
machines, states = [], []
for compiled in [False, True]:
    vm = Uc(UC_ARCH_X86, UC_MODE_32)
    vm.mem_map(native.base, (len(native.data)+4095)&~4095)
    vm.mem_write(native.base, bytes(native.data))
    if compiled:
        vm.mem_write(0x401000, bytes(0x16e000))
        vm.mem_map(candidate.base, (len(candidate.data)+4095)&~4095)
        vm.mem_write(candidate.base, bytes(candidate.data))
    vm.mem_map(0x30000000, 0x20000)
    vm.mem_map(STOP, 4096)
    state = {'writes': [], 'calls': 0, 'previous': None}
    callback = candidate.entry('FUN_0051f780') if compiled else 0x51f780
    def code(cpu, address, size, unused, state=state, compiled=compiled, callback=callback):
        if address == callback: state['calls'] += 1
        if not compiled:
            assert address in instructions or address == 0x51f780, hex(address)
            if state['previous'] is not None and (state['previous'], address) in branches:
                edges.add((state['previous'], address))
            state['previous'] = address
            if address in instructions: hits.add(address)
    def write(cpu, access, address, size, value, unused, state=state):
        if any(base <= address < base+length for base, length in regions):
            state['writes'].append((address, size, value & ((1 << (size*8))-1)))
    vm.hook_add(UC_HOOK_CODE, code, begin=candidate.base if compiled else 0x401000,
                end=candidate.base+len(candidate.data)-1 if compiled else 0x56efff)
    vm.hook_add(UC_HOOK_MEM_WRITE, write)
    machines.append(vm); states.append(state)

def run(fn, actor, tile, length, position):
    global runs
    chain = list(range(1000, 1000+length))
    if position is not None: chain.insert(min(position, len(chain)), actor)
    results = []
    for compiled, (vm, state) in enumerate(zip(machines, states)):
        for base, length_bytes in regions:
            vm.mem_write(base, b'\xa5'*16+bytes(length_bytes-32)+b'\xa5'*16)
        def put(address, value, width=4):
            vm.mem_write(address, (value & ((1 << (width*8))-1)).to_bytes(width, 'little'))
        put(0xbcd618+actor*388, tile)
        put(0xbcd682+actor*388, 253, 1)
        if tile >= 0:
            put(0xb350dc+tile*2, chain[0] if chain else 0, 2)
            for index, node in enumerate(chain):
                put(0xbcd600+node*388, chain[index+1] if index+1 < len(chain) else 0, 2)
        vm.mem_write(SP-1024, b'\xcc'*2048)
        put(SP, STOP); put(SP+4, actor); put(SP+8, 0xa5a5a5a5)
        for reg, value in zip(regids, preserved): vm.reg_write(reg, value)
        vm.reg_write(UC_X86_REG_ESP, SP); vm.reg_write(UC_X86_REG_EFLAGS, 2)
        state.update(writes=[], calls=0, previous=None)
        vm.emu_start(candidate.entry(f'FUN_{fn:08x}') if compiled else fn, STOP, count=500000)
        assert vm.reg_read(UC_X86_REG_EIP) == STOP
        assert vm.reg_read(UC_X86_REG_ESP) == SP+4
        assert [vm.reg_read(reg) for reg in regids] == preserved
        assert bytes(vm.mem_read(SP+4, 8)) == struct.pack('<II', actor, 0xa5a5a5a5)
        for base, length_bytes in regions:
            assert bytes(vm.mem_read(base, 16)) == b'\xa5'*16
            assert bytes(vm.mem_read(base+length_bytes-16, 16)) == b'\xa5'*16
        digest = hashlib.sha256(b''.join(bytes(vm.mem_read(a, n)) for a, n in regions)).hexdigest()
        results.append((digest, state['writes'], state['calls']))
    assert results[0] == results[1], (hex(fn), actor, tile, length, position, results[0][1:], results[1][1:])
    runs += 1

for fn, actor, tile in itertools.product(functions, [1, 257, 1999], [-2147483648, -1, 0, 22900, 51983]):
    for length in [0, 1, 2, 20, 21, 256]:
        positions = [None] if tile < 0 else [None, 0, length//2, length]
        for position in dict.fromkeys(positions): run(fn, actor, tile, length, position)
assert hits == set(instructions), sorted(hex(a) for a in set(instructions)-hits)
assert not branches-edges, sorted((hex(a), hex(b)) for a, b in branches-edges)
assert before == [hashlib.sha256(p.read_bytes()).digest() for p in inputs]
print(f'PASS {runs} actor grid-list cases;117/117 instructions,{len(edges)}/{len(branches)} outcomes; ordered BYTE/WORD writes,whole pools+guards,actual RET callback counts,SP/nonvolatiles.')
print('Initially acyclic mapped lists up to256 links; actor-present insertion profiles are mapped extensions,not constructor-valid claims. Native callback actor slot is ignored; actual RET executes. Original text is absent from the C VM.')
