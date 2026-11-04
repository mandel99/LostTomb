"""Offline native/C checks for explicit ring-member receivers.

Use restore_link_check.exe to inspect all six diagnostic-retained member APIs;
the game strips unused APIs but retains the actor-enqueue entry. No providers
are replaced. Growth cases have a separate heap-backed oracle which preserves the native
destination-capacity copy, including reads beyond the temporary source extent.
"""
from verify_options_startup import *

Q, BUFFER, INDEX = S+0x1100, S+0x2100, S+0x3100
MEMBERS = [0x4f68a0, 0x4f68b0, 0x4f68f0, 0x4f6900, 0x4f6950, 0x4f6a10]


def execute(c, u, token, self=0, args=()):
    argv = (self, *args) if c and token in MEMBERS+[0x4f6810] else args
    cleanup = 4 if not c and token in MEMBERS[2:]+[0x4f6810] else 0
    for r, v in zip(registers, [0x11111111, self, 0x33333333,
                              saved[0], SP, *saved[1:]]):
        u.reg_write(r, v)
    u.reg_write(UC_X86_REG_EFLAGS, 2)
    put(u, SP, STOP)
    for i, value in enumerate(argv):
        put(u, SP+4+i*4, value)
    u.emu_start(address(c, token), STOP, count=20000000)
    assert u.reg_read(UC_X86_REG_EIP) == STOP, (hex(token), c)
    assert u.reg_read(UC_X86_REG_ESP) == SP+4+cleanup
    assert [u.reg_read(registers[i]) for i in [3, 5, 6, 7]] == saved
    value = u.reg_read(UC_X86_REG_EAX)
    if token in (0x4f68f0, 0x4f6900, 0x4f6810):
        return None  # The native callers discard the unspecified result.
    return value if token in MEMBERS[:2] else value & 255


def fixture(u, state, capacity, head, count, buffer=True, self=Q):
    reset(u, state)
    u.mem_write(self, b'\xa5'*28)
    tail = (head+count-1) % capacity if capacity else 0
    for field, value in [(4, head), (8, tail), (12, count),
                         (16, capacity), (20, BUFFER if buffer else 0)]:
        put(u, self+field, value)
    for i in range(32):
        put(u, BUFFER+i*4, 0x80000000+i)


def capture(u):
    return snapshot(u), bytes(u.mem_read(S+0x1000, 0x3000))


def compare(token, setup, args=(), self=Q):
    results = []
    for c, u, state in machines:
        setup(u, state)
        result = execute(c, u, token, self, args)
        results.append((result, capture(u)))
    assert results[0] == results[1], (hex(token), args)


def verify_members():
    missing = [token for token in MEMBERS if not re.search(
        r'\s_FUN_%08x\s' % token, symbols)]
    if missing:
        print('Not retained in this target:', ', '.join(hex(v) for v in missing))
        return
    cases = 0
    for capacity, alias in itertools.product([0, 1, 2, 5, 11], [4, 8, 12, None]):
        for value in [-17, -1, 0, capacity-1, capacity, capacity*3+2]:
            index = Q+alias if alias else INDEX
            def setup(u, state):
                fixture(u, state, capacity, 0, 0)
                put(u, index, value)
            compare(0x4f6a10, setup, (index,))
            cases += 1
    for token, capacity in itertools.product(MEMBERS[:2], [1, 2, 5, 11]):
        for head, count in itertools.product([-1, 0, capacity-1], [0, 1, capacity, 0xffffffff]):
            compare(token, lambda u, state: fixture(u, state, capacity, head, count))
            cases += 1
    for capacity, head in itertools.product([1, 2, 5, 11], [0, 1]):
        head %= capacity
        for count, wanted in itertools.product([0, 1, capacity], [101, 102, 999]):
            def setup(u, state):
                fixture(u, state, capacity, head, count)
                for i in range(count):
                    put(u, BUFFER+((head+i) % capacity)*4, 101+i//2)
            compare(0x4f6950, setup, (wanted,))
            cases += 1
    for token, capacity in itertools.product(MEMBERS[2:4], [1, 2, 5, 11]):
        for head, count, buffer in itertools.product([0, capacity-1], [0, capacity-1], [False, True]):
            compare(token, lambda u, state: fixture(u, state, capacity, head, count, buffer), (0xdeadbeef,))
            cases += 1
    print('PASS six actual C member APIs, full memory, native/C receiver and cleanup:', cases, flush=True)


def verify_actor_enqueue():
    cases = 0
    for actor, building, lane, action in itertools.product(
            [1, 256, 1999], [1, 400, 3999], [0, 1, 9, 255], [0, 4]):
        self = 0x9783a4+lane*28
        def setup(u, state):
            fixture(u, state, 8, 6, 3, self=self)
            byte(u, 0x799aa3+building*264, lane)
            byte(u, 0xbcd630+actor*388, action)
        compare(0x4a4340, setup, (actor, building), self=0)
        cases += 1
    print('PASS real actor-to-building queue enrollment, high actor/building IDs:', cases, flush=True)


def verify_history_export():
    cases = 0
    for capacity, buffer in itertools.product([0, 1, 5, 200, 201], [False, True]):
        def setup(u, state):
            fixture(u, state, capacity, -17, 7, buffer)
        compare(0x4f6810, setup, (S+0x3300,))
        cases += 1
    for out in [Q, Q+4, BUFFER-20, BUFFER+4]:
        compare(0x4f6810, lambda u, state: fixture(u, state, 5, 3, 2), (out,))
        cases += 1
    print('PASS history snapshot export: capacity cap, absent buffer and forward overlap:', cases, flush=True)


if __name__ == '__main__':
    verify_members()
    verify_actor_enqueue()
    verify_history_export()
    print('Verified target SHA256:', target_sha)
