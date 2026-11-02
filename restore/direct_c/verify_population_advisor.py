"""Whole population advisor/hover, three graphs, scale and date queries.

Presentation endpoints are observed; history-ring, scale, calendar and food
queries execute. Explicit controlled-graph cases isolate the panel's reports
and callback ordering. This does not certify live glyph/pixel output.
"""
from verify_chief_advisor import setup as chief_setup
from verify_kingdom_advisor import *

ENDS = {**ENDS, 0x4d6250: 9, 0x4cce90: 8, 0x4cccb0: 6,
        0x4cd510: 5, 0x4cd9d0: 5}
GRAPHS = [0x527360, 0x527710, 0x5278e0]


def setup(u, state, selection=6, length=40, cursor=7, maximum=95,
          population=400, supply=80, updates=None, calendar=(-4000, -3500, 1), **kw):
    fields = {0xea1894: population, 0xea5fdc: 1, 0xea5fe0: supply,
              0xea18a8: 20, 0xea438c: 17, 0xea4388: 0,
              0xea41dc: 0, 0xea5e1c: 100, **(updates or {})}
    chief_setup(u, state, updates=fields, **kw)
    for owner in [0, 3]:
        off = owner*0x49d8
        u.mem_write(0xea18ac+off, bytes(2400*4))
        u.mem_write(0xea3e34+off, bytes(100*2+20*4))
        put(u, 0xea3e30+off, length); put(u, 0xea3e2c+off, cursor)
        # Spread samples across the ring including both sides of its wrap.
        for i, sample in [(0, maximum), (6, 13), (19, -7), (39, 100),
                          (99, 17), (199, 23), (399, 31), (2399, maximum)]:
            put(u, 0xea18ac+off+i*4, sample if sample < 0 else min(sample, maximum))
        for i in [0, 1, 49, 98, 99]: word(u, 0xea3e34+off+i*2, maximum)
        word(u, 0xea3e34+off+40, -32768)
        for i in [0, 1, 10, 18, 19]: put(u, 0xea3efc+off+i*4, maximum)
        put(u, 0xea3efc+off+20, -1)
    for at, value in [(0x605a40, selection), (0xf55c5c, 20000),
                      (0xf56f84, 16400), (0xf56904, 23000),
                      (0xd3b550, 1234), (0xe38dac, 1024),
                      (0x78d7bc, calendar[1]), (0x78d7c0, calendar[2])]: put(u, at, value)
    word(u, 0x7848d8, calendar[0])


def observe(c, state, calls, mutation=None):
    for token, arity in ENDS.items():
        def draw(uc, args, token=token):
            calls.append((token, args))
            if token in [0x4d62f0, 0x4d6250, 0x4d6860, 0x4ccc30, 0x4cccb0]:
                put(uc, 0xe20778, get(uc, 0xe20778)+17)
            if mutation == 'geometry':
                for field, delta in [(0xf53f0c, 3), (0xf53f08, 2),
                                     (0xe38da8, 7), (0xe38da4, 5),
                                     (0xf56f84, 16), (0xf56f94, 16), (0xf56f9c, 16),
                                     (0xf56fa4, 16), (0xf56904, 32)]:
                    put(uc, field, get(uc, field)+delta)
            if mutation == 'player':
                put(uc, 0xeaac28, 0 if get(uc, 0xeaac28) else 3)
            if mutation == 'shift' and token == 0x53e590:
                put(uc, 0xf53dc8, (get(uc, 0xf53dc8)+1)%4)
            if mutation == 'scale' and token == 0x4cce90:
                put(uc, 0xf53dcc, 0xfffffffd)
            if mutation == 'reasons' and token == 0x4d62f0 and args[0] == 0x37:
                items = [21, 22, 23, 24, 32, 33]
                if args[1] in items:
                    put(uc, 0xea5c80+get(uc, 0xeaac28)*0x49d8, items.index(args[1])+1)
            if mutation == 'singular' and token == 0x4d6860:
                put(uc, 0xea3fac+get(uc, 0xeaac28)*0x49d8, 1)
            if mutation == 'selection' and token in [0x4d6250, 0x4d63d0]:
                put(uc, 0x605a40, (get(uc, 0x605a40)+1)%6)
            if mutation == 'hover': put(uc, 0x7056c8, 2)
            return 0xdeadbeef
        hook(c, state, token, arity, draw,
             cleanup=12 if not c and token == 0x53e590 else 0)


def compare(token=0x525cb0, args=(), mutation=None, controlled=False,
            force_mode=0, shell=False, **kw):
    results = []
    for c, u, state in machines:
        setup(u, state, **kw); put(u, 0x784bbc, force_mode)
        calls = []; observe(c, state, calls, mutation)
        if controlled:
            for graph in GRAPHS:
                def endpoint(uc, args, graph=graph):
                    calls.append((graph, args))
                    if mutation == 'geometry':
                        put(uc, 0xf53f0c, get(uc, 0xf53f0c)+3)
                        put(uc, 0xf53f08, get(uc, 0xf53f08)+2)
                    return 0
                hook(c, state, graph, 3, endpoint)
        if shell:
            put(u, 0x605a44, 7); put(u, 0xe38db0, 600); byte(u, 0xe38dca, 1)
            for at, v in [(0xf55c0c, 20000), (0xf5691c, 22000), (0xf56914, 21000)]: put(u, at, v)
        value = run(c, u, 0x520ea0 if shell else token, args)
        results.append((value if token in [0x4e9760, 0x4e97c0] else None, calls, snapshot(u)))
    assert results[0] == results[1], (hex(token), args, kw, mutation, controlled,
        next(((i, a, b) for i, (a, b) in enumerate(zip(results[0][1], results[1][1])) if a != b),
             ('call counts', len(results[0][1]), len(results[1][1]))),
        [(hex(0x560000+i), a, b) for i, (a, b) in enumerate(zip(results[0][-1], results[1][-1])) if a != b][:12])


def verify_panel():
    count = 0
    for phase, origin in itertools.product([0, 1, 2, 3, 4, 5, 6, 256, 0x80000000],
                                           [(300, 200), (-300, -200)]):
        compare(phase=phase, origin=origin, controlled=True); count += 1
    for selection, mutation in itertools.product([-1, 0, 1, 2, 3, 4, 5, 6, 256],
                                                 [None, 'geometry', 'selection']):
        compare(selection=selection, mutation=mutation, controlled=True); count += 1
    for supply, balance in itertools.product([-1, 0, 70, 71, 85, 86, 95, 96, 110, 111, 150, 151, 256],
                                             [-1, 0, 1]):
        compare(supply=supply, updates={0xea3f9c: balance}, controlled=True); count += 1
    for population in [0, 1, 1000]:
        compare(population=population, supply=1, controlled=True); count += 1
    for mode, stored, workers, capacity in itertools.product([0, 256], [0, 1], [0, 50, 51], [-1, 1]):
        compare(force_mode=mode, updates={0xea4388: stored, 0xea41dc: workers,
                                          0xea5e1c: capacity*2}, controlled=True); count += 1
    for state, reason in itertools.product([0, 1, 2, 3, 256], [-1, 0, 1, 2, 3, 4, 5, 6]):
        compare(updates={0xea3fb0: state, 0xea5c80: reason}, controlled=True); count += 1
    for extra in [{0xea5eb4: 4}, {0xea5eb4: 3}, {0xea5eb4: 0x7fffffff, 0xea5ea4: 2},
                  {0xea3fac: 4}, {0xea3fac: 5}, {0xea3f98: 1}, {0xea18a8: 0},
                  {0xea3f9c: 79}, {0xea3f9c: 80, 0xea3fac: 1},
                  {0xea3f9c: 80, 0xea3fac: 2}]:
        compare(updates=extra, controlled=True); count += 1
    for mutation in ['geometry', 'player', 'reasons', 'singular']:
        for state in [0, 1]:
            compare(updates={0xea3fb0: state, 0xea5c80: 0,
                             0xea3f9c: 80 if mutation == 'singular' else 0},
                    mutation=mutation, controlled=True); count += 1
    print('PASS whole population panel, six layouts, genuine food, storage and migration:', count, flush=True)


def verify_graphs():
    count = 0
    for graph, mode, maximum in itertools.product(GRAPHS, [0, 1], [0, 100, 101, 400, 401, 25600, 25601]):
        compare(graph, [mode, 350, 200], maximum=maximum); count += 1
    for length, mode in itertools.product([-1, 20, 21, 40, 41, 100, 101, 200, 201, 2401], [0, 1]):
        compare(0x527360, [mode, -300, 400], length=length, cursor=2399); count += 1
    for graph, mutation in itertools.product(GRAPHS, ['geometry', 'player', 'shift', 'scale']):
        compare(graph, [1, 350, 200], mutation=mutation); count += 1
    for maximum in [-2147483648, -1, 0, 100, 101, 200, 201, 400, 401, 800, 801,
                    1600, 1601, 3200, 3201, 6400, 6401, 12800, 12801, 25600, 25601, 2147483647]:
        compare(0x527ad0, [maximum]); count += 1
    print('PASS three complete graphs, real ring queries, scale boundaries and callback reloads:', count, flush=True)


def verify_dates_and_hover():
    count = 0
    for length, distance in itertools.product([0, 500], [-2147483648, -13, -12, -11, -1, 0, 20, 40, 100, 200, 400]):
        compare(0x4e97c0, [distance], length=length); count += 1
    for length, cursor, distance, index in itertools.product([20, 2500], [0, 2399], [20, 400], [0, 19]):
        compare(0x4e9760, [distance, index], length=length, cursor=cursor); count += 1
    for hover, old, mutation in itertools.product([-1, 0, 1, 2, 3, 256], [0, 1, 256], [None, 'geometry', 'hover']):
        results = []
        for c, u, state in machines:
            setup(u, state); put(u, 0x7056c8, hover); put(u, 0x5f825c, old)
            calls = []; observe(c, state, calls, mutation); run(c, u, 0x5271f0)
            results.append((calls, snapshot(u)))
        assert results[0] == results[1], ('hover', hover, old, mutation)
        count += 1
    print('PASS full-width calendar/ring values and complete hover repaint:', count, flush=True)


def verify_compositions():
    count = 0
    for selection, player in itertools.product(range(6), [0, 3]):
        compare(selection=selection, player=player); count += 1
    for selection in range(6):
        compare(selection=selection, shell=True, mutation='geometry'); count += 1
    print('PASS actual advisor routing plus panel with all genuine graph/food/calendar algorithms:', count, flush=True)


def verify_calendar_boundaries():
    count = 0
    for (start, year), month, length in itertools.product(
            [(32767, 50000), (-32768, -50000), (0, 0)],
            [0, 1, 12, 256, -2147483648], [0, 500]):
        compare(0x4e97c0, [20], length=length, calendar=(start, year, month)); count += 1
    print('PASS signed WORD year, full DWORD calendar and wrapped previous-month boundaries:', count, flush=True)


if __name__ == '__main__':
    verify_panel(); verify_graphs(); verify_dates_and_hover(); verify_compositions(); verify_calendar_boundaries()
    print('Verified target SHA256:', target_sha)
