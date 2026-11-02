"""Whole entertainment advisor with genuine religion/service-count preludes.

Only final presentation is controlled. The genuine4000-record building scan,
house averages, shortage selection and mood preview all execute before the
opening gate. Isolated report cases are explicitly separate.
"""
from verify_religion_updates import *
from verify_service_advisors import outputs

KINDS = [31, 30, 33, 32, 226]
FIELDS = [0xea5bd0, 0xea5bd8, 0xea5be0, 0xea5be8, 0xea5d38]
CAPACITY = [400, 700, 1200, 5000, 7500]


def compare(phase=5, origin=(300, 200), player=3, counts=(1, 2, 3, 4, 5),
            coverage=(0, 10, 50, 99, 100), stages=(), houses=(),
            performers=100, required=0, mutate=False, shell=False,
            population=1000, isolated_report=None):
    results = []
    for c, u, state in machines:
        setup(u, state, player=player, population=population)
        u.mem_write(0x7999d8, bytes(4000*264))
        for at, value in [(0xf53f48, phase), (0xe38da8, origin[0]),
                          (0xe38da4, origin[1]), (0xf5681c, 20000),
                          (0xf56fb4, 16416), (0xf56f8c, 16448),
                          (0xf56f9c, 16480), (0xe20778, 700)]:
            put(u, at, value)
        for owner in [0, 3]:
            off = owner*0x49d8
            put(u, 0xea5b7c+off, performers)
            put(u, 0xea5bf8+off, required)
            for i, field in enumerate(FIELDS): put(u, field+off, 300+i)
        for i, kind in enumerate(KINDS):
            put(u, 0x70573c+kind*4, counts[i])
            put(u, 0x78d5f0+i*4, coverage[i])
        for i, (kind, flags) in enumerate(stages):
            building = [1, 511, 1000, 2000, 3999][i]
            byte(u, 0x7999d8+building*264, 1)
            word(u, 0x7999e8+building*264, kind)
            for j, flag in enumerate(flags): byte(u, 0x7999d8+building*264+167+j, flag)
        for i, (alive, occupied, level) in enumerate(houses):
            building = 2500+i
            byte(u, 0x7999d8+building*264, alive)
            byte(u, 0x7999d8+building*264+5, occupied)
            for j in range(4): byte(u, 0x7999d8+building*264+128+j, level+j)
        calls = []
        outputs(c, state, calls, mutate)
        if isolated_report is not None:
            def refresh(uc, args):
                # Explicit report-branch isolation only. Real prelude cases
                # above and below execute the actual query, without this hook.
                off = get(uc, 0xeaac28)*0x49d8
                for field, value in zip([0xea5b7c, 0xea5bf8, 0xea5bf4, 0xea5bf0], isolated_report):
                    put(uc, field+off, value)
                return 0
            hook(c, state, 0x4550e0, 0, refresh)
        if shell:
            put(u, 0x605a44, 10)
            put(u, 0xe38dac, 600)
            put(u, 0xe38db0, 800)
            byte(u, 0xe38dca, 1)
            for at, value in [(0xf55c0c, 20000), (0xf5691c, 22000), (0xf56914, 21000)]: put(u, at, value)
        run(c, u, 0x520ea0 if shell else 0x528950)
        results.append((calls, snapshot(u)))
    assert results[0] == results[1], ('entertainment', phase, origin, player,
        counts, coverage, stages, houses, performers, required, mutate, shell,
        isolated_report, results[0][0][:14], results[1][0][:14],
        [(hex(0x560000+i), a, b) for i, (a, b) in enumerate(zip(results[0][-1], results[1][-1])) if a != b][:12])
    return results[1][0]


def report(calls):
    return [args[1] for token, args in calls if token == 0x4d6160][-1]


def verify():
    count = 0
    for phase, origin, player in itertools.product(
            [-2147483648, -1, 0, 1, 4, 5, 6, 256, 2147483647],
            [(0, 0), (192, 144), (300, 200)], [0, 3]):
        compare(phase=phase, origin=origin, player=player,
                stages=[(31, (1,))], houses=[(1, 1, 75)])
        count += 1
    assert compare(phase=1) == [(0x4cd680, (364, 232, 512, 176, 0))]
    print('PASS entertainment opening/full origins/player with genuine city preludes:', count, flush=True)
    count = 0
    for value in [-2147483648, -1, 0, 1, 9, 10, 99, 100, 256, 2147483647]:
        counts = [value, 2, 3, 4, 5]
        calls = compare(counts=counts, coverage=[value]*5)
        expected = [(amount*capacity+2**31) % 2**32-2**31 for amount, capacity in zip(counts, CAPACITY)]
        assert [args[0] for token, args in calls if token == 0x4d6860] == [v & 0xffffffff for v in expected]
        count += 1
    calls = compare(stages=list(zip(KINDS, [(1,), (1, 1), (1, 1, 1), (1,), (1,)])))
    assert [args[0] for token, args in calls if token == 0x4cce90] == [1, 3, 2, 2, 3, 1, 4, 1, 5, 1]
    count += 1
    for kind, flags, expected in [(31, (0,), 11), (30, (1, 0), 12),
                                  (33, (1, 1, 0), 13), (32, (0,), 14),
                                  (226, (0,), 14), (31, (1,), 9)]:
        assert report(compare(stages=[(kind, flags)])) == expected
        count += 1
    for houses, performers, required, expected in [([], 0, 0, 7),
            ([(1, 1, 75)], 0, 0, 8), ([(1, 0, 75), (0, 1, 75)], 0, 0, 7),
            ([(1, 1, 75), (1, 1, -75)], 0, 0, 7), ([], 100, 101, 10)]:
        assert report(compare(houses=houses, performers=performers, required=required)) == expected
        count += 1
    print('PASS distinct service counts, wrapped capacity, signed coverage and all genuine shortage/house reports:', count, flush=True)
    count = 0
    for selected in [-2147483648, -1, 0, 1, 2, 3, 4, 256, 2147483647]:
        calls = compare(isolated_report=(1, 0, 1, selected))
        assert report(calls) == {0: 9, 1: 11, 2: 12, 3: 13}.get(selected, 14)
        count += 1
    for mutate, shell in [(True, False), (False, True), (True, True)]:
        compare(mutate=mutate, shell=shell,
                stages=list(zip(KINDS, [(1,), (1, 0), (1, 1, 0), (0,), (1,)])),
                houses=[(1, 1, 75)])
        count += 1
    print('PASS explicit report isolation, cached staffed counts, callback reloads and genuine shared-frame compositions:', count, flush=True)
    print('Verified target SHA256:', target_sha)


if __name__ == '__main__': verify()
