"""Complete health and education advisors with real percentage arithmetic.

Only presentation endpoints are controlled. Isolated query-callback cases
additionally check the eight numeric arguments deferred across4CDE40.
"""
from verify_kingdom_advisor import *

ENDS = {**ENDS, 0x4d6250: 9, 0x4cce90: 8}
TOKENS = [0x527bf0, 0x528360]


def setup(u, state, phase=5, origin=(300, 200), player=3, population=1000,
          health=75, counts=300, coverage=(50, 50, 50, 50, 50), risk=4,
          health_need=0, school_need=0, flags=1, school=50, library=50):
    reset(u, state)
    for at, val in [(0xeaac28, player), (0xf53f48, phase), (0xe38da8, origin[0]),
                    (0xe38da4, origin[1]), (0xf5681c, 20000), (0xf56fb4, 16416),
                    (0xf56f8c, 16448), (0xf56f9c, 16480),
                    (0xe20778, 700), (0xe20774, 500),
                    (0x7057f4, counts), (0x705808, counts), (0x705810, counts)]:
        put(u, at, val)
    for owner in [0, 3]:
        off = owner*0x49d8
        for at, val in [(0xea1894, population), (0xea1888, health), (0xea5fc0, risk),
                        (0xea41e0, counts), (0xea5c8c, health_need), (0xea5c94, school_need),
                        (0xea5bb8, flags), (0xea5bb4, flags), (0xea5bbc, flags),
                        (0xea5fb8, flags), (0xea5b88, flags), (0xea5b8c, flags)]:
            put(u, at+off, val)
    for kind in [206, 49, 47]:
        put(u, 0x70573c+kind*4, counts)
    for i, val in zip([10, 8, 11], coverage):
        put(u, 0x78d5f0+i*4, val)
    put(u, 0x78d604, school)
    put(u, 0x78d608, library)


def outputs(c, state, calls, mutate=False):
    for token, count in ENDS.items():
        def draw(uc, args, token=token):
            calls.append((token, args))
            if token in [0x4d62f0, 0x4d6250, 0x4d6860, 0x4cce90, 0x4d6160]:
                put(uc, 0xe20778, get(uc, 0xe20778)+17)
            if token == 0x4d6160:
                put(uc, 0xe20774, get(uc, 0xe20774)+25)
            if mutate and token in [0x4d62f0, 0x4cce90, 0x4d6250, 0x4d6160]:
                put(uc, 0xf53f0c, get(uc, 0xf53f0c)+3)
                put(uc, 0xf53f08, get(uc, 0xf53f08)+2)
                put(uc, 0xeaac28, 0)
                for at in [0x78d618, 0x78d610, 0x78d61c, 0x78d604, 0x78d608]:
                    put(uc, at, get(uc, at)+7)
            return 0
        hook(c, state, token, count, draw,
             cleanup=12 if not c and token == 0x53e590 else 0)


def compare(token, mutate=False, query_callback=False, shell=False, updates=None, **kwargs):
    states = []
    for c, u, state in machines:
        setup(u, state, **kwargs)
        for at, val in (updates or {}).items():
            put(u, at, val)
        calls = []
        outputs(c, state, calls, mutate)
        if query_callback:
            def query(uc, args):
                calls.append((0x4cde40, args))
                put(uc, 0xf53f0c, get(uc, 0xf53f0c)+10)
                put(uc, 0xf53f08, get(uc, 0xf53f08)+20)
                put(uc, 0xe38da8, 222)
                put(uc, 0xe38da4, 111)
                put(uc, 0xf56f9c, 16512)
                return -100
            hook(c, state, 0x4cde40, 2, query)
        if shell:
            put(u, 0x605a44, 8 if token == 0x527bf0 else 9)
            put(u, 0xe38dac, 600)
            put(u, 0xe38db0, 800)
            byte(u, 0xe38dca, 1)
            for at, val in [(0xf55c0c, 20000), (0xf5691c, 22000), (0xf56914, 21000)]:
                put(u, at, val)
        run(c, u, 0x520ea0 if shell else token)
        states.append((calls, snapshot(u)))
    assert states[0] == states[1], (hex(token), kwargs, mutate, query_callback, shell,
        states[0][0][:18], states[1][0][:18],
        [(hex(0x560000+i), a, b) for i, (a, b) in enumerate(zip(states[0][-1], states[1][-1]))
         if a != b][:12])
    return states[1][0]


def verify_opening():
    count = 0
    for token, phase, origin, player in itertools.product(TOKENS,
            [-2147483648, -1, 0, 1, 4, 5, 6, 256, 2147483647],
            [(0, 0), (192, 144), (300, 200)], [0, 3]):
        compare(token, phase=phase, origin=origin, player=player)
        count += 1
    assert compare(0x527bf0, phase=1) == [(0x4cd680, (364, 232, 512, 224, 0))]
    assert compare(0x528360, phase=1) == [(0x4cd680, (364, 232, 512, 128, 0))]
    print('PASS complete health/education opening, origins/player/fonts and full phase:', count, flush=True)


def verify_health():
    count = 0
    for population, health, coverage in itertools.product(
            [-1, 0, 199, 200, 1000, 2147483647], [-1, 0, 75, 100],
            [0, 1, 50, 99, 100, 101, 256, -1]):
        compare(0x527bf0, population=population, health=health, coverage=[coverage]*3)
        count += 1
    calls = compare(0x527bf0, population=1000, coverage=[50]*3)
    assert [args[0] for token, args in calls if token == 0x4d6860] == [500]*3
    print('PASS genuine health population/percentage, signed coverage and summary thresholds:', count, flush=True)
    count = 0
    for risk, need, flags in itertools.product([-1, 0, 1, 2, 3, 5, 6, 8, 9, 256],
                                              [0, 1, 2, 3, 4, 256], [0, 1, 256]):
        compare(0x527bf0, risk=risk, health_need=need, flags=flags)
        count += 1
    print('PASS all health requirement/risk decisions, DWORD flags and multiline cursor:', count, flush=True)
    for counts in [-2147483648, -1, 0, 300, 2147483647]:
        compare(0x527bf0, counts=counts)
    compare(0x527bf0, query_callback=True)
    print('PASS full building counts and all deferred numeric-render arguments:', 6, flush=True)


def verify_education():
    count = 0
    for counts, coverage in itertools.product([-2147483648, -1, 0, 1, 300, 2147483647],
                                             [-2147483648, -1, 0, 1, 9, 10, 99, 100, 256, 2147483647]):
        compare(0x528360, counts=counts, school=coverage, library=coverage)
        count += 1
    print('PASS school/library DWORD counts, wrapped capacities and signed coverage:', count, flush=True)
    count = 0
    for need, flags, (school, library) in itertools.product([0, 1, 2, 3, 4, 256], [0, 1, 256],
            [(0, 0), (0, 100), (50, 20), (20, 50), (99, 100), (100, 99), (100, 100), (-1, 0)]):
        compare(0x528360, school_need=need, flags=flags, school=school, library=library)
        count += 1
    for school_enabled, library_enabled in itertools.product([0, 256], [0, 256]):
        for c, u, state in machines:
            setup(u, state)
            put(u, 0xea5b88+3*0x49d8, school_enabled)
            put(u, 0xea5b8c+3*0x49d8, library_enabled)
            calls = []
            outputs(c, state, calls)
            run(c, u, 0x528360)
            if not c:
                expected = calls, snapshot(u)
            else:
                assert (calls, snapshot(u)) == expected
        count += 1
    print('PASS every education requirement/default branch, unequal coverages and independent flags:', count, flush=True)


def verify_compositions():
    for token in TOKENS:
        compare(token, mutate=True)
        compare(token, shell=True)
    print('PASS live text/coverage/player mutations and actual advisor frame compositions:', 4, flush=True)
    print('Verified target SHA256:', target_sha)


def verify_independent_fields():
    updates = {0x70573c+206*4: 1024, 0x70573c+49*4: 511, 0x70573c+47*4: 300}
    calls = compare(0x527bf0, coverage=[10, 50, 150], updates=updates)
    assert [args[0] for token, args in calls if token == 0x4d6860] == [100, 500, 1500]
    assert [args[0] for token, args in calls if token == 0x4cce90][:3] == [1024, 511, 300]
    calls = compare(0x528360, updates={0x705808: 5, 0x705810: 7})
    assert [args[0] for token, args in calls if token == 0x4d6860] == [1500, 5600]
    for need, field in enumerate([0xea5bb8, 0xea5bb4, 0xea5bbc, 0xea5fb8], 1):
        for active in [0, 256]:
            fields = {at+3*0x49d8: 256-active for at in [0xea5bb8, 0xea5bb4, 0xea5bbc, 0xea5fb8]}
            fields[field+3*0x49d8] = active
            calls = compare(0x527bf0, health_need=need, updates=fields)
            assert [args[1] for token, args in calls if token == 0x4d6160][-1] == [7, 9, 11, 13][need-1]+int(bool(active))
    print('PASS distinct row sources/capacities and independently toggled health requirements:', 10, flush=True)


if __name__ == '__main__':
    verify_opening()
    verify_health()
    verify_education()
    verify_independent_fields()
    verify_compositions()
