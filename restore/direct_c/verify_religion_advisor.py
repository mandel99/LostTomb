"""Whole religion/festival presentation family against original instructions.

Real classification and least-happy queries execute. Only low-level output
is observed, except explicitly isolated query mutations. This is not a full
glyph/pixel or live-game claim.
"""
from verify_religion_updates import *
import verify_service_advisors as service

ENDS = {**service.ENDS, 0x4d6200: 9, 0x4cf090: 5}


def compare(token=0x5298e0, phase=6, origin=(300, 200), player=3,
            roles=(1, 2, 1, 2, 0), temple_flags=(1, 0, 1, 0, 1),
            level=50, wrath=0, blessing=0, need=0, flag=0,
            festival=0, months=1, since=12, reserved=(0, 0), calendar=0,
            selection=0, size=1, hover=0, eligible=1, cash=10000,
            beer_flag=0, debug=0, mutate=False, query_callback=False,
            shell=False):
    results = []
    for c, u, state in machines:
        setup(u, state, player=player, level=level, roles=roles)
        for owner in [0, 3]:
            off = owner*0x49d8
            for at, value in [(0xea5c90, need), (0xea5bcc, flag),
                              (0xea5d68, festival), (0xea5d6c, months),
                              (0xea5bfc, since), (0xea5d98, reserved[0]),
                              (0xea5d9c, reserved[1]), (0xea5d60, selection),
                              (0xea5d64, size), (0xea187c, cash),
                              (0xea5d84, beer_flag), (0xea5d74, 1024),
                              (0xea5d78, 511), (0xea5d7c, 300), (0xea5d80, 17)]:
                put(u, at+off, value)
            word(u, 0xea6084+off, eligible)
            for god in range(5):
                byte(u, 0xea5c14+off+god, wrath if isinstance(wrath, int) else wrath[god])
                byte(u, 0xea6086+off+god, blessing)
        for god in range(5):
            word(u, 0x784db0+god*2, temple_flags[god])
        for kind in range(60, 75): put(u, 0x705af0+kind*4, 1000+kind)
        for at, value in [(0xf53f48, phase), (0xf53f0c, origin[0]),
                          (0xf53f08, origin[1]), (0xe38da8, origin[0]),
                          (0xe38da4, origin[1]), (0xf5681c, 20000),
                          (0xf55c5c, 21000), (0xf56794, 22000),
                          (0xf56464, 23000), (0xf56fb4, 16416),
                          (0xf56f8c, 16448), (0xf56f94, 16464),
                          (0xf56f9c, 16480), (0x78d7c0, calendar),
                          (0x7056c8, hover), (0xe20778, 700)]:
            put(u, at, value)
        calls = []
        for endpoint, arity in ENDS.items():
            def draw(uc, args, endpoint=endpoint):
                calls.append((endpoint, args))
                if endpoint in [0x4d62f0, 0x4d6250, 0x4d6860, 0x4cce90, 0x4ccc30, 0x4d6160]:
                    put(uc, 0xe20778, get(uc, 0xe20778)+17)
                if endpoint == 0x4d6200:
                    put(uc, 0xeadd44, debug if debug != 2 else args[1] % 2)
                if mutate and endpoint in [0x4d62f0, 0x4d6250, 0x4d63d0, 0x4cf090, 0x4ccc30]:
                    put(uc, 0xf53f0c, get(uc, 0xf53f0c)+3)
                    put(uc, 0xf53f08, get(uc, 0xf53f08)+2)
                    put(uc, 0xeaac28, 0 if get(uc, 0xeaac28) else 3)
                    put(uc, 0x7056c8, get(uc, 0x7056c8)+1)
                return 0
            hook(c, state, endpoint, arity, draw,
                 cleanup=12 if not c and endpoint == 0x53e590 else 0)
        if query_callback:
            def query(uc, args):
                calls.append((0x4c06e0, args))
                put(uc, 0xf53f0c, get(uc, 0xf53f0c)+10)
                put(uc, 0xf53f08, get(uc, 0xf53f08)+20)
                put(uc, 0xf56f9c, get(uc, 0xf56f9c)+32)
                return 60+args[0]+args[1]*5
            hook(c, state, 0x4c06e0, 2, query)
        if shell:
            put(u, 0x605a44, 11)
            put(u, 0xe38dac, 600)
            put(u, 0xe38db0, 800)
            byte(u, 0xe38dca, 1)
            for at, value in [(0xf55c0c, 20000), (0xf5691c, 22000), (0xf56914, 21000)]: put(u, at, value)
        run(c, u, 0x520ea0 if shell else token)
        results.append((calls, snapshot(u)))
    assert results[0] == results[1], ('religion page', hex(token), phase, origin,
        player, roles, level, wrath, blessing, festival, months, since, selection,
        cash, debug, mutate, query_callback, shell,
        next(((i, a, b) for i, (a, b) in enumerate(zip(results[0][0], results[1][0])) if a != b),
             (len(results[0][0]), len(results[1][0]))),
        [(hex(0x560000+i), a, b) for i, (a, b) in enumerate(zip(results[0][-1], results[1][-1])) if a != b][:12])
    return results[1][0]


def verify():
    count = 0
    for phase, origin, player in itertools.product(
            [-2147483648, -1, 0, 1, 4, 5, 6, 256, 2147483647],
            [(0, 0), (192, 144), (300, 200)], [0, 3]):
        compare(phase=phase, origin=origin, player=player)
        count += 1
    assert compare(phase=1) == [(0x4cd680, (364, 232, 512, 304, 0))]
    print('PASS whole religion page opening/full origins/player/fonts:', count, flush=True)
    count = 0
    for level in [-128, -1, 0, 9, 10, 39, 40, 50, 99, 100, 127]:
        compare(level=level, wrath=level, blessing=level)
        count += 1
    for need, flag, wrath in itertools.product([0, 1, 2, 3, 256], [0, 256], [0, 4, 5]):
        compare(need=need, flag=flag, wrath=wrath)
        count += 1
    for mask in range(32):
        compare(roles=tuple(2 if mask & (1 << i) else 0 for i in range(5)),
                temple_flags=(1, 256, 2, 0, -1), wrath=(0, 5, 10, 20, 50))
        count += 1
    for debug in [1, 2]: compare(debug=debug, level=-128, wrath=127, blessing=255); count += 1
    compare(query_callback=True); count += 1
    print('PASS all god/summary decisions, signed moods/stars, WORD availability, diagnostics and deferred query arguments:', count, flush=True)
    count = 0
    for festival, months in itertools.product([1, 2, 3, -1, 256],
            [-2147483648, -1, 0, 1, 2, 3, 4, 31, 2147483647]):
        compare(0x528f10, festival=festival, months=months, calendar=256)
        count += 1
    for since, reserved in itertools.product([-1, 1, 2, 6, 7, 12, 13, 18, 19, 24, 25, 30, 31],
                                             [(0, 0), (0, 1), (1, 0), (256, 256)]):
        compare(0x528f10, since=since, reserved=reserved)
        count += 1
    for phase, hover, eligible in itertools.product([5, 6, 256], [0, 1, 257], [0, 1, 256]):
        compare(0x5292b0, phase=phase, hover=hover, eligible=eligible)
        count += 1
    print('PASS full festival states/month arithmetic, reservation/elapsed thresholds and foreground hover:', count, flush=True)
    count = 0
    for mask, selection in itertools.product([0, 1, 2, 4, 8, 16, 21, 31], [-1, 0, 2, 4, 5, 256]):
        compare(0x529320, roles=tuple(1 if mask & (1 << i) else 0 for i in range(5)), selection=selection)
        count += 1
    for cash, hover, beer in itertools.product([-2147483648, -5001, -5000, -4999, 0, 2147483647],
                                              [0, 6, 7, 8, 262], [0, 256]):
        compare(0x529530, cash=cash, hover=hover, beer_flag=beer)
        count += 1
    for token in [0x5298e0, 0x528f10, 0x529320, 0x529530]:
        compare(token, mutate=True, wrath=50, blessing=50)
        count += 1
    compare(shell=True); compare(shell=True, mutate=True); count += 2
    print('PASS god selection/cost popup, exact debt boundary/cursor, full hover and genuine shared compositions:', count, flush=True)
    print('Verified target SHA256:', target_sha)


if __name__ == '__main__': verify()
