"""Whole military advisor background/foreground and real data providers.

Only panel/text/sprite output is controlled. Genuine formation enumeration,
actor eligibility and both event-list traversals execute on native and C.
"""
from verify_kingdom_advisor import *

ENDS = {**ENDS, 0x4d6250: 9, 0x4cd1a0: 5}


def setup(u, state, groups=(1, 7, 49), phase=6, origin=(300, 200), player=3,
          records=(), selection=2, mission=-1, assigned=1, strength=37,
          empire=0, naval=False, city_state=2):
    prepare(u, state, phase=phase, origin=origin, player=player, records=records,
            city_state=city_state)
    u.mem_write(0xbcb9c8, bytes(50*144))
    u.mem_write(0xbcd5f8, bytes(2000*388))
    for at, value in [(0x7056c8, selection), (0xf56434, 21000),
                      (0xf56814, 22000), (0xf56834, 23000),
                      (0xf5642c, 24000), (0xf5643c, 25000),
                      (0xf5690c, 26000), (0xf56f94, 16512),
                      (0xf53dd4, 777), (0xf53f0c, origin[0]),
                      (0xf53f08, origin[1])]:
        put(u, at, value)
    for owner in [0, 3]:
        off = owner*0x49d8
        put(u, 0xea5eb4+off, 256 if empire == 2 else 0)
        put(u, 0xea5ea4+off, 256 if empire == 1 else 0)
        byte(u, 0xea5edf+off, 200)
        byte(u, 0xea5edc+off, 150)
    for i, group in enumerate(groups):
        off = group*144
        byte(u, 0xbcb9c8+off, 1)
        byte(u, 0xbcba0a+off, 255)
        byte(u, 0xbcb9ca+off, 200+i)
        byte(u, 0xbcb9cb+off, i & 1)
        word(u, 0xbcb9cc+off, [13, 11, 25][i % 3])
        byte(u, 0xbcb9f0+off, 200+i)
        word(u, 0xbcb9f4+off, strength)
        word(u, 0xbcba42+off, strength)
        word(u, 0xbcba4a+off, mission)
        byte(u, 0xbcba2f+off, assigned)
        word(u, 0xbcb9d0+off, i+1)
        byte(u, 0xbcd630+(i+1)*388, 18 if naval or i & 1 else 8)


def outputs(c, state, calls, mutate=False):
    for token, count in ENDS.items():
        def draw(uc, args, token=token):
            calls.append((token, args))
            if token in [0x4d62f0, 0x4d6860, 0x4ccc30, 0x4d6250]:
                put(uc, 0xe20778, get(uc, 0xe20778)+17)
            if mutate and token in [0x4d62f0, 0x4d63d0]:
                put(uc, 0xf53f0c, get(uc, 0xf53f0c)+3)
                put(uc, 0xf53f08, get(uc, 0xf53f08)+2)
                put(uc, 0xeaac28, 0)
                put(uc, 0x7056c8, get(uc, 0x7056c8)+1)
            return 0
        hook(c, state, token, count, draw,
             cleanup=12 if not c and token == 0x53e590 else 0)


def compare(token=0x521b50, mutate=False, gate=None, shell=False, buttons=False, **kwargs):
    states = []
    for c, u, state in machines:
        setup(u, state, **kwargs)
        calls = []
        outputs(c, state, calls, mutate)
        if buttons:
            state['hooks'].pop(address(c, 0x4cd1a0))
            hook(c, state, 0x536c30, 6,
                 lambda uc, args: calls.append((0x536c30, args)) or 0)
        if gate is not None:
            def query(uc, args):
                word(uc, 0xbcba4a+args[0]*144, 0)
                return gate
            hook(c, state, 0x40b5e0, 1, query)
        if shell:
            put(u, 0x605a44, 2)
            put(u, 0xe38dac, 600)
            put(u, 0xe38db0, 800)
            byte(u, 0xe38dca, 1)
            for at, value in [(0xf55c0c, 20000), (0xf5691c, 22000), (0xf56914, 21000)]:
                put(u, at, value)
        run(c, u, 0x520ea0 if shell else token)
        states.append((calls, snapshot(u)))
    assert states[0] == states[1], (hex(token), kwargs, mutate, gate, shell,
        states[0][0][:18], states[1][0][:18],
        [(hex(0x560000+i), a, b) for i, (a, b) in enumerate(zip(states[0][-1], states[1][-1]))
         if a != b][:12])
    return states[1][0]


def verify_queries():
    count = 0
    for groups in [(), (1,), (49,), (1, 7, 49), tuple(range(1, 50))]:
        for ordinal in [-1, 0, 1, 2, 3, 49, 50, 256]:
            results = []
            for c, u, state in machines:
                setup(u, state, groups=groups)
                total = run(c, u, 0x4b78f0)
                item = run(c, u, 0x4b78b0, [ordinal])
                results.append((total, item, snapshot(u)))
            assert results[0] == results[1]
            assert total == len(groups)
            assert item == (groups[ordinal-1] if 0 < ordinal <= len(groups) else 0)
            count += 1
    print('PASS genuine formation count/ordinal, sparse records and full ordinal:', count, flush=True)


def verify_panels():
    count = 0
    for phase, origin, groups in itertools.product(
            [-2147483648, -1, 0, 1, 4, 5, 6, 256, 2147483647],
            [(0, 0), (192, 144), (300, 200)], [(), (1, 7, 49)]):
        compare(phase=phase, origin=origin, groups=groups)
        count += 1
    assert compare(phase=1) == [(0x4cd680, (364, 232, 512, 288, 0))]
    print('PASS complete military background, opening stages and full fields:', count, flush=True)
    count = 0
    for mission, strength, assigned in itertools.product(
            [-32768, -1, 0, 32767], [-32768, -1, 0, 37, 32767], [0, 1, 255]):
        compare(mission=mission, strength=strength, assigned=assigned)
        count += 1
    print('PASS real row classes, signed status/strength and assignment side effects:', count, flush=True)
    count = 0
    for mode, city, empire, groups in itertools.product(
            [0, 5, 6, 7], [-1, 0, 49, 50], [0, 1, 2], [(), (1, 7, 49)]):
        compare(records=[dict(kind=37, mode=mode, city=city)], empire=empire, groups=groups)
        count += 1
    print('PASS military events, city bounds, empty/active fleets and empire priorities:', count, flush=True)
    for city_state, naval in itertools.product([1, 2], [False, True]):
        for token in [0x521b50, 0x522a60]:
            compare(token, records=[dict(kind=37)], city_state=city_state, naval=naval)
    for token in [0x521b50, 0x522a60]:
        compare(token, records=[dict(kind=37, mode=m, city=i) for i, m in enumerate([5, 6, 7])])
        compare(token, groups=tuple(range(1, 50)))
    print('PASS both request classes, mixed events and all49 genuine formation records:', 12, flush=True)
    count = 0
    for phase, selection, mission, assigned in itertools.product(
            [4, 5, 6, 256], [0, 1, 2, 3, 19, 20, 256], [-1, 0], [0, 1]):
        compare(0x522a60, phase=phase, selection=selection, mission=mission, assigned=assigned,
                records=[dict(kind=37)])
        count += 1
    print('PASS foreground phase gates, all hover types and dispatch/recall state:', count, flush=True)
    for token, gate in itertools.product([0x521b50, 0x522a60], [0, 1, 256]):
        compare(token, gate=gate, records=[dict(kind=37)])
    for token in [0x521b50, 0x522a60]:
        compare(token, mutate=True, records=[dict(kind=37), dict(kind=37, city=49)])
    print('PASS full eligibility result, post-query status and live layout/selection reloads:', 8, flush=True)
    for groups in [(), (1, 7, 49)]:
        compare(shell=True, groups=groups, records=[dict(kind=37)])
    print('PASS shared advisor frame through genuine military page:', 2, flush=True)
    for selection, assigned in itertools.product([1, 19, 20, 256], [0, 1]):
        calls = compare(0x522a60, selection=selection, assigned=assigned,
                        records=[dict(kind=37)], buttons=True)
        assert any(token == 0x536c30 for token, args in calls)
    print('PASS actual shared button records, signed coordinates and hover forwarding:', 8, flush=True)
    print('Verified target SHA256:', target_sha)


if __name__ == '__main__':
    verify_queries()
    verify_panels()
