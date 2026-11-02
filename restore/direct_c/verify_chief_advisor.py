"""Whole chief advisor with genuine food, request, invasion and flood queries.

Only final presentation is observed. Separate callback cases explicitly
replace a query to test cached fonts or post-call reloads. No live-game or
complete glyph/raster claim is made.
"""
from verify_kingdom_advisor import *

ENDS = {**ENDS, 0x4d6200: 9, 0x4cd290: 8}
FIELDS = [0xea1880, 0xea5db0, 0xea3fb0, 0xea5eb4, 0xea5ea4, 0xea3fac,
          0xea3f98, 0xea18a8, 0xea3f9c, 0xea5c80, 0xea446c, 0xea4474,
          0xea4388, 0xea41dc, 0xea5e1c, 0xea1894, 0xea5fdc, 0xea5fe0,
          0xea5fe8, 0xea5fec, 0xea1888, 0xea44b8, 0xea44b4, 0xea44b0,
          0xea44ac, 0xea187c, 0xea452c, 0xea5fd0, 0xea5fd4, 0xea5ee8]


def setup(u, state, phase=5, player=3, origin=(300, 200), updates=None,
          gods=(50, 50, 50, 50, 50), records=(), flood=80, cycle=15, period=1,
          temple=0, temple_kind=60):
    reset(u, state)
    for reg, value in [(UC_X86_REG_FPCW, 0x37f), (UC_X86_REG_FPSW, 0),
                       (UC_X86_REG_FPTAG, 0xffff)]: u.reg_write(reg, value)
    u.mem_write(0x7999d8, bytes(4000*264))
    u.mem_write(0x77de00, bytes(0x1942))
    u.mem_write(0x785d70, bytes(18628))
    for owner in [0, 3]:
        off = owner*0x49d8
        for field in FIELDS: put(u, field+off, 0)
        put(u, 0xea1894+off, 1000)
        put(u, 0xea5e1c+off, 100)
        word(u, 0xea5ce8+off, temple)
        put(u, 0xea5f9c+off, 511)
        for i, value in enumerate(gods): byte(u, 0xea5c00+off+i, value)
        for field, value in (updates or {}).items(): put(u, field+off, value)
    for at, value in [(0xeaac28, player), (0xf53f48, phase),
                      (0xe38da8, origin[0]), (0xe38da4, origin[1]),
                      (0xf5681c, 20000), (0xf5690c, 22000),
                      (0xf56fb4, 16416), (0xf56f8c, 16448),
                      (0xf56f94, 16464), (0xf56f9c, 16480), (0xf56fa4, 16512),
                      (0x784bbc, 0), (0x78d66c, 1), (0x78d668, 1),
                      (0xe20778, 700), (0xd38374, flood), (0xd38360, cycle),
                      (0xd386d0, period), (0x7848d8, 0), (0x78d7bc, 0), (0x78d7c0, 0)]: put(u, at, value)
    put(u, 0x785d70, len(records))
    word(u, 0x7999e8+511*264, temple_kind)
    for i, data in enumerate(records):
        p = 0x785d74+i*124
        byte(u, p+2, data.get('active', 1)); byte(u, p+0x32, data.get('enabled', 1))
        word(u, p+4, data.get('kind', 1)); word(u, p+0x30, data.get('mode', 0))
        word(u, p+0x14, data.get('year', 0)); byte(u, p+3, data.get('month', 0))
        word(u, p+0x2c, data.get('duration', 0)); put(u, p+0x28, data.get('flags', 0))


def compare(mutate=False, food_callback=False, waiting_callback=False,
            shell=False, force_mode=None, **kwargs):
    results = []
    for c, u, state in machines:
        setup(u, state, **kwargs)
        if force_mode is not None: put(u, 0x784bbc, force_mode)
        calls = []
        for token, arity in ENDS.items():
            def draw(uc, args, token=token):
                calls.append((token, args))
                if token in [0x4d6200, 0x4cd290, 0x4d62f0]:
                    put(uc, 0xe20778, get(uc, 0xe20778)+17)
                if mutate and token == 0x4d6200:
                    put(uc, 0xeaac28, 0 if get(uc, 0xeaac28) else 3)
                    for field in [0xf56f94, 0xf56f9c, 0xf56fa4]: put(uc, field, get(uc, field)+32)
                    put(uc, 0xe38da8, get(uc, 0xe38da8)+3)
                    put(uc, 0xe38da4, get(uc, 0xe38da4)+2)
                return 0xdeadbeef
            hook(c, state, token, arity, draw,
                 cleanup=12 if not c and token == 0x53e590 else 0)
        if food_callback:
            def food(uc, args):
                calls.append((0x447530, args))
                put(uc, 0xeaac28, 0)
                put(uc, 0xea1894, 100)
                put(uc, 0xf56f94, 9999)
                return 151
            hook(c, state, 0x447530, 0, food)
        if waiting_callback:
            def waiting(uc, args):
                p = args[0] if c else uc.reg_read(UC_X86_REG_ECX)
                calls.append((0x44d4c0, (p,)))
                if waiting_callback == 2:
                    word(uc, p+0x30, 7)
                    return 0x101
                return 0x100  # AL0, despite a nonzero full EAX.
            hook(c, state, 0x44d4c0, 1 if c else 0, waiting)
        if shell:
            put(u, 0x605a44, 13); put(u, 0xe38dac, 600); put(u, 0xe38db0, 800)
            byte(u, 0xe38dca, 1)
            for at, value in [(0xf55c0c, 20000), (0xf5691c, 22000), (0xf56914, 21000)]: put(u, at, value)
        run(c, u, 0x520ea0 if shell else 0x52c570)
        results.append((calls, snapshot(u)))
    assert results[0] == results[1], ('chief', kwargs, mutate, food_callback,
        waiting_callback, shell, force_mode,
        next(((i, a, b) for i, (a, b) in enumerate(zip(results[0][0], results[1][0])) if a != b),
             (len(results[0][0]), len(results[1][0]))),
        [(hex(0x560000+i), a, b) for i, (a, b) in enumerate(zip(results[0][-1], results[1][-1])) if a != b][:12])
    return results[1][0]


def row_items(calls, row):
    return [args[1] for token, args in calls if token == 0x4d6200 and args[2] == 0x5c5878+row*28]


def verify():
    count = 0
    for phase, origin, player in itertools.product(
            [-2147483648, -1, 0, 1, 4, 5, 6, 256, 2147483647],
            [(0, 0), (192, 144), (300, 200)], [0, 3]):
        compare(phase=phase, origin=origin, player=player); count += 1
    assert compare(phase=1) == [(0x4cd680, (364, 232, 512, 256, 0))]
    print('PASS whole chief opening/full phase/origin/player/font fields:', count, flush=True)
    count = 0
    for level, modifier in itertools.product([-1, 0, 1, 29, 30, 49, 50, 99, 100, 256], [0, 1, 256]):
        compare(updates={0xea1880: level, 0xea5db0: modifier}); count += 1
    for fields in [{0xea3fb0: v} for v in [-1, 0, 1, 2, 256]] + [
            {0xea5eb4: 4}, {0xea5ea4: 4}, {0xea3fac: 5},
            {0xea18a8: 1, 0xea3f9c: 79, 0xea5c80: 3},
            {0xea18a8: 1, 0xea3f9c: 80}, {0xea18a8: 1, 0xea3f98: 1},
            {0xea3fb0: 1, 0xea18a8: 1, 0xea5c80: 2},
            {0xea5eb4: 2147483647, 0xea5ea4: 1}]:
        compare(updates=fields); count += 1
    for labor, missing in itertools.product([-1, 0, 1, 5, 6, 10, 11, 17, 18, 256], [-1, 0, 1, 5, 6, 10, 11, 20, 21]):
        compare(updates={0xea446c: labor, 0xea4474: missing}); count += 1
    for remaining, amount in itertools.product([-1, 0, 1, 256], [-1, 0, 49, 50, 100, 2147483647]):
        compare(updates={0xea4388: remaining, 0xea41dc: amount}); count += 1
    compare(force_mode=256); count += 1
    print('PASS migration/safety/labor/food-storage branches, signed fields, wrapped ratios and full flags:', count, flush=True)
    count = 0
    for supply in [-1, 0, 70, 71, 85, 86, 95, 96, 110, 111, 150, 151, 256]:
        compare(updates={0xea1894: 400, 0xea5fdc: 1, 0xea5fe0: supply}); count += 1
    for population in [-1, 0, 1, 256]: compare(updates={0xea1894: population}); count += 1
    for health in [-2147483648, -1, 0, 19, 20, 29, 30, 100, 256]:
        compare(updates={0xea1888: health}); count += 1
    for low, high, minimum in itertools.product(range(6), range(6), [19, 20]):
        if low+high>5: continue
        compare(gods=[minimum]*low+[71]*high+[50]*(5-low-high)); count += 1
    compare(gods=[-128, -1, 29, 30, 127]); count += 1
    print('PASS genuine food-capacity calculation and all food/health/religion thresholds:', count, flush=True)
    count = 0
    for cash, previous in itertools.product([-2147483648, -1, 0, 100, 2147483647], [-1, 0, 100]):
        compare(updates={0xea187c: cash, 0xea452c: previous}); count += 1
    for numerator, denominator in itertools.product([-2147483648, -1, 0, 1, 10, 2147483647], [1, 2, 10]):
        compare(updates={0xea44b4: denominator, 0xea44ac: numerator}); count += 1
    for crime, amount in itertools.product([-1, 0, 1, 2, 4, 5, 9, 10, 256], [-1, 0, 256]):
        compare(updates={0xea5fd0: crime, 0xea5fd4: amount}); count += 1
    for flood, cycle, period in itertools.product([-20, -19, 0, 1, 20, 21, 80, 100, 101], [-31, 0, 15, 30, 256], [0, 256]):
        compare(flood=flood, cycle=cycle, period=period); count += 1
    print('PASS budget/treasury/crime and genuine flood-quality/calendar branches:', count, flush=True)
    count = 0
    for kind, mode, enabled in itertools.product([0, 1, 37], [-1, 0, 1, 5, 6, 7, 8, 256], [0, 1]):
        compare(records=[dict(kind=kind, mode=mode, enabled=enabled)]); count += 1
    for n in [0, 1, 2, 3, 4, 5, 6, 20]:
        compare(records=[dict(kind=1, mode=0)]*n); count += 1
    for field in [0xea5eb4, 0xea5ea4, 0xea5ee8]:
        for value in [-1, 0, 256]: compare(updates={field: value}); count += 1
    compare(records=[dict(kind=37, mode=0), dict(kind=37, mode=5), dict(kind=37, mode=6)]); count += 1
    for mutate, shell in [(True, False), (False, True), (True, True)]:
        compare(mutate=mutate, shell=shell, records=[dict(kind=37, mode=1)]); count += 1
    compare(food_callback=True); count += 1
    compare(waiting_callback=True, records=[dict(kind=37, mode=256)]); count += 1
    print('PASS real request/invasion/two-pass list queries, five-item cap, AL results and callback/shared-shell compositions:', count, flush=True)
    print('Verified target SHA256:', target_sha)


def verify_temples_and_query_reload():
    count = 0
    for temple, kind, flood in itertools.product([1, 256], [60, 61], [5, 95, 2147483647]):
        compare(temple=temple, temple_kind=kind, flood=flood)
        count += 1
    calls = compare(waiting_callback=2, records=[dict(kind=37, mode=0)])
    assert row_items(calls, 9)[-1] == 176
    count += 1
    print('PASS genuine temple/flood bonus-clamp/wrap and post-query request-state reload:', count, flush=True)


if __name__ == '__main__':
    verify()
    verify_temples_and_query_reload()
