"""Tax-collector state machine, cell coverage, expiry and actual tax coupling.

Original x86 and current Release run offline. State-machine cases control
eight named movement/road endpoints and compare ordered arguments, entry state
and all game data. Service compositions run the real4BAA10, coverage, packed
cell service, phase48 and tax providers. No game process or save is modified.
"""
from verify_tax_updates import *

ACTORS = (1, 255, 1999)
ENDS = {0x4b17d0: 1, 0x4b1d50: 1, 0x47fe60: 4, 0x4b4220: 6,
        0x4b3ff0: 2, 0x4b1df0: 1, 0x4b1ef0: 2, 0x4a5da0: 2}


def a(id): return 0xbcd5f8+id*388
def h(u, at): return struct.unpack('<h', u.mem_read(at, 2))[0]
def b(u, at): return u.mem_read(at, 1)[0]


def same(results, label):
    assert results[0] == results[1], (label,
        results[0][0], results[1][0],
        [(hex(0x560000+i), x, y) for i, (x, y) in
         enumerate(zip(results[0][-1], results[1][-1])) if x != y][:12])


def prepare_cells(u, state, x=10, y=10, dims=(32, 32), origin=0,
                  rows=None, ids=(1, 1, 2, 3, 4), metric=80):
    if rows is None:
        rows = ({'id': 1}, {'id': 2, 'kind': 14}, {'id': 3, 'population': 0},
                {'id': 4, 'occupied': 0})
    prepare_tax(u, state, rows=rows)
    u.mem_write(0xb4e6fc, bytes(228*228*2))
    for at, value in [(0xbca2d0, dims[0]), (0xbca2d4, dims[1]),
                      (0xbca2d8, origin), (0x9ab9e8, 0x7fffffff),
                      (0xd3b4e0, 0x12345678), (0xd3b4e4, 0x23456789),
                      (0xbc9768, 0x3456789a)]: put(u, at, value)
    for level in range(20): put(u, 0x70ce18+level*88, metric+level)
    # Populate the entire bounded map: edge clipping selects from real cells.
    for yy in range(max(0, dims[1])):
        for xx in range(max(0, dims[0])):
            word(u, 0xb4e6fc+(origin+yy*228+xx)*2, ids[(xx+yy*3) % len(ids)])
    return x, y


def coverage(**kw):
    results = []
    for c, u, state in machines:
        args = prepare_cells(u, state, **kw)
        value = run(c, u, 0x484a90, args)
        results.append((value, snapshot(u)))
    same(results, ('coverage', kw))
    return results[1][0]


def verify_coverage():
    count = 0
    for dims, origin, pos in itertools.product([(1, 1), (17, 11), (228, 228)],
            [0, 7], [(-3, -3), (-1, 0), (0, 0), (1, 1), (10, 8), (230, 230)]):
        coverage(x=pos[0], y=pos[1], dims=dims, origin=origin); count += 1
    for state, occupied, population in itertools.product([0, 1, 3], [0, 1, 255], [-1, 0, 1, 32767]):
        coverage(rows=({'state': state, 'occupied': occupied, 'population': population},),
                 ids=(1,)); count += 1
    for level, metric in itertools.product([0, 13, 14, 19], [-2147483648, -1, 0, 256, 2147483600]):
        coverage(rows=({'kind': level},), ids=(1,), metric=metric); count += 1
    for id in [1, 511, 4000]:
        value = coverage(rows=({'id': id},), ids=(id,))
        assert value == 25; count += 1
    coverage(ids=(-1, 0, 1)); count += 1
    for x, y in [(40, 10), (10, 40), (-3, 10), (10, -3)]:
        coverage(x=x, y=y); count += 1
    print('PASS cell coverage, repeated houses, clipped/empty rectangles, WORD population and full metrics:', count, flush=True)


def verify_decay():
    count = 0
    for status, amount, high in itertools.product([0, 1, 3], [0, 1, 2, 50, 255], [-1, 0, 1, 4000]):
        rows = ({'id': max(1, high), 'state': status, 'paying': amount},)
        results = []
        for c, u, state in machines:
            prepare_tax(u, state, rows=rows, high_water=high)
            p = 0x7999d8+max(1, high)*264
            byte(u, p+77, 0xa5); byte(u, p+79, 0x5a)
            run(c, u, 0x4599a0)
            expected = amount-1 if high >= 1 and status == 1 and amount else amount
            assert b(u, p+78) == expected and b(u, p+77) == 0xa5 and b(u, p+79) == 0x5a
            results.append((None, snapshot(u)))
        same(results, ('decay', status, amount, high)); count += 1
    print('PASS cached count, active-only BYTE decay, high-water record and adjacent-byte guards:', count, flush=True)


def prepare_actor(u, state, action=8, frame=0, wait=1, counter=0, direction=0,
                  orientation=0, home=511, home_state=1, home_link=1, kind=7, on_home=False):
    prepare_tax(u, state)
    u.mem_write(0xbcd5f8, bytes(2000*388))
    put(u, 0xd056f8, 1); put(u, 0xbca2e0, orientation)
    put(u, 0xf55e34, 10000); put(u, 0xf55e3c, 11000)
    put(u, 0xd3b4e4, 0x12348002); put(u, 0xd3b4e0, 0x56787ffd)
    for id in ACTORS:
        p = a(id)
        for off, value in [(1, frame), (10, kind), (11, 0xa5), (12, 9), (13, 0x5a),
                           (14, 1), (17, direction), (56, action), (101, 0), (114, 9)]: byte(u, p+off, value)
        for off, value in [(20, 10), (22, 10), (54, wait), (66, 0x1234),
                           (68, counter), (74, -32768), (76, 32767), (90, home)]: word(u, p+off, value)
        put(u, p+32, 2300+id); put(u, 0x7090ec+kind*56, 0x80000101)
        word(u, 0xb4e6fc+(2300+id)*2, home if on_home else 0)
    p = (0x7999d8+home*264) & 0xffffffff
    byte(u, p, home_state); word(u, p+42, home_link)
    word(u, p+6, -32768); word(u, p+8, 32767); byte(u, p+3, 255)


def actor_observation(u):
    return (get(u, 0xd056f8), tuple(bytes(u.mem_read(a(id), 116)) for id in ACTORS))


def invoke_symbol(u, name, args):
    for r, v in zip(registers, [0x11111111, 0x22222222, 0x33333333, saved[0], SP, *saved[1:]]): u.reg_write(r, v)
    put(u, SP, STOP)
    for i, value in enumerate(args): put(u, SP+4+i*4, value)
    u.reg_write(UC_X86_REG_EFLAGS, 2)
    u.emu_start(sym(name), STOP, count=2000000)
    assert u.reg_read(UC_X86_REG_EIP) == STOP and u.reg_read(UC_X86_REG_ESP) == SP+4
    assert [u.reg_read(registers[i]) for i in [3, 5, 6, 7]] == saved
    return u.reg_read(UC_X86_REG_EAX)


def invoke_callback(c, u):
    if not c: return run(c, u, 0x48c2b0)
    # Use the real callback-address resolver to reach the new public body.
    return invoke_symbol(u, 'restore_actor_callback', [0x48c2b0])


def service(c, u, actor):
    if not c: return run(c, u, 0x4baa10, [actor])
    # O2 inlines the public4BAA10 facade. Enter its genuine routing boundary
    # with an explicit native frame; do not add production test exports.
    sp = SP-0x100
    initial = [0x11111111, 0x22222222, 0x33333333, saved[0], sp, *saved[1:]]
    for i, value in enumerate(initial): put(u, S+i*4, value)
    put(u, S+0x40, 2); put(u, sp, STOP); put(u, sp+4, actor)
    invoke_symbol(u, 'restore_actor_routing_raw', [0x4baa10, S, S+0x40])
    assert get(u, S+16) == sp+4
    assert [get(u, S+i*4) for i in [3, 5, 6, 7]] == saved
    return get(u, S)


def walker(road=1, moved=1, mutate=False, after_action=None, **kw):
    results = []
    for c, u, state in machines:
        prepare_actor(u, state, **kw); calls = []
        for token, nargs in ENDS.items():
            def observe(uc, args, token=token):
                calls.append((token, args, actor_observation(uc)))
                if mutate:
                    next_actor = ACTORS[min(len(calls), 2)]
                    put(uc, 0xd056f8, next_actor)
                    word(uc, a(next_actor)+90, 4000)
                    if after_action is not None: byte(uc, a(next_actor)+56, after_action)
                return road if token == 0x47fe60 else moved if token == 0x4b3ff0 else 0xfedcba98
            hook(c, state, token, nargs, observe)
        invoke_callback(c, u)
        results.append((calls, snapshot(u)))
    same(results, ('walker', kw, road, moved, mutate, after_action))


def verify_walker():
    count = 0
    for action, frame, validity in itertools.product([0, 4, 5, 8, 9, 10, 11, 255], [0, 10, 11, 255], [0, 1, 2]):
        walker(action=action, frame=frame, home_state=0 if validity == 0 else 1,
               home_link=255 if validity == 2 else 1); count += 1
    for wait, road in itertools.product([-32768, -1, 0, 1, 2, 32767], [0, 1, 256]):
        walker(action=8, wait=wait, road=road); count += 1
    for moved, on_home in itertools.product([-1, 0, 1, 256], [False, True]):
        walker(action=9, moved=moved, on_home=on_home); count += 1
    for counter, road in itertools.product([-32768, -1, 0, 510, 511, 32767], [0, 1, 256]):
        walker(action=10, counter=counter, road=road); count += 1
    for direction, kind in itertools.product([0, 7, 8, 9, 10, 127, 128, 255], [0, 7, 255]):
        walker(action=11, direction=direction, kind=kind); count += 1
    for action, orientation, direction in itertools.product([0, 4], [0, 6, 256, 0xffffffff], [0, 8, 255]):
        walker(action=action, orientation=orientation, direction=direction); count += 1
    for action, after_action in itertools.product([4, 5, 8, 9, 10, 11], [None, 4]):
        walker(action=action, counter=511, direction=8, mutate=True, after_action=after_action); count += 1
    for home in [-1, 0, 4000]:
        walker(home=home, action=8); count += 1
    print('PASS complete collector actions, signed/wrapped timers, full query results, images and callback reloads:', count, flush=True)


def verify_service_and_income():
    count = 0
    for owner, age, refresh in itertools.product([0, 3], [0, 49, 50, 51], [False, True]):
        results = []
        for c, u, state in machines:
            prepare_cells(u, state, rows=({'id': 1, 'paying': 0},), ids=(1,), metric=256)
            put(u, 0xeaac28, owner)
            p = a(255); byte(u, p+10, 7); word(u, p+20, 10); word(u, p+22, 10)
            word(u, p+90, 511); put(u, p+32, 2290)
            word(u, 0x7999f0+511*264, 290)
            put(u, 0x978d9c+2290*4, 0xfffffff0)
            put(u, 0x5dbf70+7*4, 2)
            assert service(c, u, 255) == 0
            assert b(u, 0x799a26+264) == 50
            assert b(u, p+102) == (269 & 255)
            assert h(u, 0x7999f0+511*264) == 300
            for n in range(age):
                for at in [0x78d79c, 0x78d7a8, 0x78d7a0, 0x78d7b0, 0x78d7a4]: put(u, at, 0)
                byte(u, 0xeaaf5c, 0); put(u, 0x78d7cc, 48)
                run(c, u, 0x451c50)
            if refresh: service(c, u, 255)
            assert b(u, 0x799a26+264) == (50 if refresh else max(0, 50-age))
            run(c, u, 0x4599d0)
            off = owner*0x49d8
            assert get(u, 0xea44a8+off) == (100 if refresh or age < 50 else 0)
            results.append((None, snapshot(u)))
        same(results, ('actual service/expiry/income', owner, age, refresh)); count += 1
    print('PASS genuine actor service, packed cell update, home count, phase48 expiry, refresh and monthly tax:', count, flush=True)
    print('Verified target SHA256:', target_sha, flush=True)


if __name__ == '__main__':
    verify_coverage(); verify_decay(); verify_walker(); verify_service_and_income()
