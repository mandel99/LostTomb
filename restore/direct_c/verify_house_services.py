"""Seven complete house-service effects and real dispatcher compositions.

Offline pinned original/Release. Compare all mapped data and ordered writes
to the public scan coordinates/tile and crime maximum. No service effect is
substituted. The priest composition controls only the two classifiers in its
explicit query case; normal compositions use the actual classifier providers.
"""
from verify_actor_services import *
from unicorn import UC_HOOK_MEM_WRITE

TOKENS = [0x484990, 0x483e90, 0x483fb0, 0x484890, 0x483160, 0x483260, 0x484670]
VOID = {0x483260, 0x484670}
WATCH = {0xd3b4e0, 0xd3b4e4, 0xbc9768, 0x9ab9dc}

for c, u, state in machines:
    state['service_writes'] = []
    def on_write(uc, access, address, size, value, data):
        if address in WATCH:
            data['service_writes'].append((address, size, value & ((1 << (size*8))-1)))
    u.hook_add(UC_HOOK_MEM_WRITE, on_write, state)


def prepare_effect(u, state, pos=(10, 10), dims=(32, 32), origin=0, ids=None,
                   rows=None, crime=350, minimum=100, gate=0):
    if rows is None:
        rows = ({'id': 1, 'state': 3, 'population': 100, 'type': 10},
                {'id': 2, 'state': 0, 'population': 0, 'type': 29},
                {'id': 3, 'occupied': 0, 'population': -1, 'type': 30},
                {'id': 4, 'state': 0, 'occupied': 0, 'type': 88},
                {'id': 4000, 'type': 89}, {'id': -1, 'state': 255, 'type': 29})
    if ids is None: ids = (1, 1, 2, 3, 4, 4000, -1, 0)
    prepare_cells(u, state, x=pos[0], y=pos[1], dims=dims, origin=origin,
                  rows=rows, ids=ids)
    for i, d in enumerate(rows, 1):
        id = d.get('id', i); p = 0x7999d8+id*264
        word(u, p+16, d.get('type', 10))
        for off in [133, 135, 136, 194]: byte(u, p+off, 0xa5)
        put(u, p+228, crime)
    for level in range(20): put(u, 0x70ce0c+level*88, minimum)
    put(u, 0x78d7b0, gate); put(u, 0x9ab9dc, 0x7fffffff)
    state['service_writes'].clear()


def effect(token=0x484990, deity=0, amount=11, **kw):
    results = []
    for c, u, state in machines:
        prepare_effect(u, state, **kw)
        args = [*kw.get('pos', (10, 10))]
        if token == 0x483fb0: args.append(deity)
        if token == 0x483260: args.append(amount)
        value = run(c, u, token, args)
        if token == 0x484890: assert value == 1
        if token == 0x483160: assert value == 0
        results.append(((None if token in VOID else value, list(state['service_writes'])), snapshot(u)))
    same(results, ('effect', hex(token), kw, deity, amount))
    return results[1]


def verify_effects():
    count = 0
    for token, dims, origin, pos in itertools.product(TOKENS, [(1, 1), (32, 32)], [0, 7],
            [(10, 10), (0, 0), (31, 31), (40, 10), (10, 40), (-3, 10), (10, -3)]):
        effect(token, dims=dims, origin=origin, pos=pos); count += 1
    for state, occupied, population in itertools.product([0, 1, 3], [0, 1, 255], [-1, 0, 1]):
        for token in [0x484990, 0x483e90, 0x483fb0, 0x484670]:
            effect(token, rows=({'id': 4000, 'state': state, 'occupied': occupied, 'population': population},),
                   ids=(4000,)); count += 1
    for deity in [-2147483648, -1, 0, 1, 2, 3, 4, 5, 256, 0x7fffffff]:
        result = effect(0x483fb0, deity=deity, rows=({'id': 1, 'state': 0},), ids=(1,))
        assert result[0][0] == 25; count += 1
    for gate in [-2147483648, -1, 0, 1, 256]:
        result = effect(0x483e90, gate=gate, rows=({'id': 1},), ids=(1,))
        if gate > 0: assert result[0] == (0, [])
        count += 1
    for token, kind, state in itertools.product([0x483160, 0x484890],
            [-1, 9, 10, 29, 30, 88, 89, 90], [0, 3]):
        effect(token, rows=({'id': -1, 'state': state, 'type': kind, 'occupied': 0},
                            {'id': 1, 'state': state, 'type': kind, 'occupied': 0}), ids=(-1, 1)); count += 1
    print('PASS seven effects, exact publication order, clipping/origin, occupancy gates, religion selector and water suppression:', count, flush=True)


def verify_crime():
    count = 0
    for crime, amount, minimum in itertools.product([-2147483648, -1, 0, 350, 1001, 0x7fffffff],
            [-2147483648, -1, 0, 10, 1000, 0x7fffffff], [-1, 100, 1001]):
        effect(0x483260, rows=({'id': 1, 'occupied': 0, 'population': 0},), ids=(1,),
               crime=crime, amount=amount, minimum=minimum); count += 1
    # Independent expected result:25 cell visits, each reducing the same record.
    result = effect(0x483260, rows=({'id': 1, 'occupied': 0},), ids=(1,), crime=350, amount=11, minimum=100)
    game = result[1]
    assert struct.unpack_from('<I', game, 0x799abc+264-0x560000)[0] == 100
    assert struct.unpack_from('<I', game, 0x9ab9dc-0x560000)[0] == 350
    count += 1
    print('PASS crime pre-change maximum, repeated reductions, signed limits, DWORD wrap and empty-house effects:', count, flush=True)


def verify_real_dispatch():
    count = 0
    for kind, home_kind, gate in itertools.product([5, 87, 27, 64, 88, 89], [60, 64, 69], [0, 1]):
        results = []
        for c, u, state in machines:
            p = prepare_dispatch(u, state, kind=kind, home_kind=home_kind, gate=gate)
            put(u, 0x78d7b0, 0); word(u, 0x7999e8+1500*264, home_kind)
            byte(u, 0x799a5f+264, 0xa5); put(u, 0x799abc+264, 350)
            for level in range(20): put(u, 0x70ce0c+level*88, 100)
            state['service_writes'].clear()
            value = run(c, u, 0x4baa10, [255]); assert value == 0
            results.append((list(state['service_writes']), snapshot(u)))
        same(results, ('actual dispatcher', kind, home_kind, gate)); count += 1
    for category in [1, 3, 4]:
        results = []
        for c, u, state in machines:
            prepare_dispatch(u, state, kind=27, home_kind=60, gate=1)
            put(u, 0x799abc+264, 350); byte(u, 0x799a5f+264, 0xa5)
            for level in range(20): put(u, 0x70ce0c+level*88, 100)
            hook(c, state, 0x4c5120, 1, lambda uc, args: category)
            hook(c, state, 0x4c52d0, 2, lambda uc, args: 1)
            state['service_writes'].clear()
            assert run(c, u, 0x4baa10, [255]) == 0
            if category in [1, 3]: assert get(u, 0x799abc+264) == 100
            results.append((list(state['service_writes']), snapshot(u)))
        same(results, ('priest actual secondary effect', category)); count += 1
    print('PASS actual dispatcher and new effects together, real classifiers plus three explicit priest-query fixtures:', count, flush=True)
    print('Verified target SHA256:', target_sha, flush=True)


if __name__ == '__main__':
    verify_effects(); verify_crime(); verify_real_dispatch(); verify_visits()
