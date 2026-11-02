"""Whole financial panel and bar against pinned original x86.

Real temple-god query and bar composition execute. Only final presentation
is observed, with explicitly separate query/callback mutation cases. No
complete glyph/raster or live-game certification is implied.
"""
from verify_kingdom_advisor import *

ENDS = {**ENDS, 0x4cd510: 5}
FIELDS = [0xea1878, 0xea187c, 0xea44a8, 0xea44b4, 0xea44b8, 0xea5e5c,
          0xea44c0, 0xea44bc, 0xea44d8, 0xea44d4, 0xea5cc4, 0xea5cc8,
          0xea601c, 0xea6020, 0xea4514, 0xea4518, 0xea44e0, 0xea44dc,
          0xea448c, 0xea4488, 0xea44f8, 0xea44fc, 0xea44e8, 0xea44ec,
          0xea4500, 0xea4504, 0xea44f0, 0xea44f4, 0xea5d54, 0xea5d50,
          0xea5fc4, 0xea5fc8, 0xea451c, 0xea4520, 0xea4524, 0xea4528,
          0xea452c, 0xea4530]


def setup(u, state, player=3, origin=(300, 200), phase=5, cash=10000,
          temple=0, kind=61, building=511, rate=10, pattern=0):
    reset(u, state)
    for owner in [0, 3]:
        off = owner*0x49d8
        for i, field in enumerate(FIELDS):
            values = [1000+i+owner*100, 0, -1, -2147483648, 2147483647,
                      0x10000+i*3, -300-i, i*0x1020304]
            put(u, field+off, values[pattern])
        put(u, 0xea187c+off, cash if owner == player else -cash)
        word(u, 0xea5ce8+off, temple)
        put(u, 0xea5f9c+off, building)
    word(u, 0x7999e8+building*264, kind)
    for at, value in [(0xeaac28, player), (0xf53f48, phase),
                      (0xe38da8, origin[0]), (0xe38da4, origin[1]),
                      (0xf5681c, 20000), (0xf56844, 22000),
                      (0xf56fb4, 16416), (0xf56f8c, 16448),
                      (0xf56f9c, 16480), (0xf56fa4, 16512),
                      (0xe20778, 700), (0x784ddc, rate)]:
        put(u, at, value)


def outputs(c, state, calls, mutate=False):
    for token, arity in ENDS.items():
        def draw(uc, args, token=token):
            calls.append((token, args))
            if token in [0x4d62f0, 0x4d6860, 0x4ccc30]:
                put(uc, 0xe20778, get(uc, 0xe20778)+17)
            if mutate and token in [0x4d62f0, 0x4d6860, 0x4ccc30, 0x4cd510]:
                put(uc, 0xf53f0c, get(uc, 0xf53f0c)+3)
                put(uc, 0xf53f08, get(uc, 0xf53f08)+2)
                put(uc, 0xeaac28, 0 if get(uc, 0xeaac28) else 3)
                put(uc, 0xf56f8c, get(uc, 0xf56f8c)+32)
                put(uc, 0xf56f9c, get(uc, 0xf56f9c)+32)
            return 0
        hook(c, state, token, arity, draw,
             cleanup=12 if not c and token == 0x53e590 else 0)


def compare(mutate=False, query_result=None, shell=False, **kwargs):
    results = []
    for c, u, state in machines:
        setup(u, state, **kwargs)
        calls = []
        outputs(c, state, calls, mutate)
        if query_result is not None:
            def query(uc, args):
                calls.append((0x4c5120, args))
                put(uc, 0x784ddc, 99)
                put(uc, 0xeaac28, 0)
                put(uc, 0xe38da8, 400)
                put(uc, 0xe38da4, 240)
                return query_result
            hook(c, state, 0x4c5120, 1, query)
        if shell:
            put(u, 0x605a44, 12)
            put(u, 0xe38dac, 600)
            put(u, 0xe38db0, 800)
            byte(u, 0xe38dca, 1)
            for at, value in [(0xf55c0c, 20000), (0xf5691c, 22000), (0xf56914, 21000)]: put(u, at, value)
        run(c, u, 0x520ea0 if shell else 0x52a250)
        results.append((calls, snapshot(u)))
    assert results[0] == results[1], ('financial', kwargs, mutate, query_result, shell,
        next(((i, a, b) for i, (a, b) in enumerate(zip(results[0][0], results[1][0])) if a != b),
             (len(results[0][0]), len(results[1][0]))),
        [(hex(0x560000+i), a, b) for i, (a, b) in enumerate(zip(results[0][-1], results[1][-1])) if a != b][:12])
    return results[1][0]


def verify_panel():
    count = 0
    for phase, origin, player in itertools.product(
            [-2147483648, -1, 0, 1, 4, 5, 6, 256, 2147483647],
            [(0, 0), (192, 144), (300, 200)], [0, 3]):
        compare(phase=phase, origin=origin, player=player)
        count += 1
    assert compare(phase=1) == [(0x4cd680, (364, 232, 512, 304, 0))]
    print('PASS financial full opening/origin/player/font fields:', count, flush=True)
    count = 0
    for player, pattern in itertools.product([0, 3], range(8)):
        compare(player=player, pattern=pattern)
        count += 1
    for cash in [-2147483648, -10000, -1, 0, 1, 2147483647]:
        calls = compare(cash=cash)
        currency = [args for token, args in calls if token == 0x4ccc30][0]
        assert currency[2] == (abs(cash) & 0xffffffff)
        assert currency[5] == (16512 if cash < 0 else 16480)
        count += 1
    for rate, kind in itertools.product([-2147483648, -1, 0, 4, 5, 6, 256, 2147483647], range(59, 71)):
        compare(rate=rate, kind=kind, temple=1 if kind % 2 else 256)
        count += 1
    for rate in [0, 4, 5, 6, 256, -1]:
        calls = compare(rate=rate, temple=256, kind=61)
        assert [args[0] for token, args in calls if token == 0x4d6860][19] == max(0, rate-5)
        count += 1
    print('PASS all ledger fields, signed debt, wrapped totals and genuine temple tax adjustment:', count, flush=True)
    count = 0
    for result in [0, 1, 257, -1]:
        compare(temple=256, building=3999, query_result=result)
        count += 1
    compare(phase=0, temple=256, query_result=1); count += 1
    for mutate, shell in [(True, False), (False, True), (True, True)]:
        compare(mutate=mutate, shell=shell, temple=256)
        count += 1
    print('PASS cached prelude rate/full query result, live ledger/cursor/layout and shared-shell compositions:', count, flush=True)


def verify_bar():
    count = 0
    for (x, y), width, atlas in itertools.product([(300, 200), (-8, -20), (192, 144)],
            [-2147483648, -1, 0, 1, 2, 3, 6, 7, 34], [20000, 0xfffffff0]):
        results = []
        for c, u, state in machines:
            reset(u, state); put(u, 0xf56844, atlas)
            calls = []; outputs(c, state, calls)
            run(c, u, 0x536ba0, [x, y, width])
            results.append((calls, snapshot(u)))
        assert results[0] == results[1], ('bar', x, y, width, atlas, results[0][0], results[1][0])
        assert len(calls) == max(0, width)
        count += 1
    for c, u, state in machines:
        reset(u, state); put(u, 0xf56844, 20000)
        calls = []
        def sprite(uc, args):
            calls.append(args)
            put(uc, 0xd3b5dc, get(uc, 0xd3b5dc)+1)
            put(uc, 0xf56844, get(uc, 0xf56844)+1)
            return 0
        hook(c, state, 0x53e590, 3, sprite, cleanup=0 if c else 12)
        run(c, u, 0x536ba0, [300, 200, 9])
        if not c: expected = calls, snapshot(u)
        else: assert (calls, snapshot(u)) == expected
    count += 1
    print('PASS complete bar widths/first-last/cycling, wrapped full atlas/coordinates and scratch callbacks:', count, flush=True)


def verify_temple_query():
    count = 0
    for building, kind in itertools.product([1, 511, 3999], [-32768, -1, 0, *range(59, 71), 32767]):
        results = []
        for c, u, state in machines:
            reset(u, state); word(u, 0x7999e8+building*264, kind)
            value = run(c, u, 0x4c5120, [building])
            results.append((value, snapshot(u)))
        assert results[0] == results[1], ('temple query', building, kind)
        count += 1
    print('PASS reused genuine temple-god query, signed WORD, actual264-byte stride and full EAX:', count, flush=True)


if __name__ == '__main__':
    verify_panel()
    verify_bar()
    verify_temple_query()
    print('Verified target SHA256:', target_sha)
