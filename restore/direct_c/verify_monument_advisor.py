"""Complete monument-advisor background and genuine36-slot demand query.

Only final presentation is observed. The separately owned foreground is not
certified by this background verification.
"""
from verify_financial_advisor import *


def compare(phase=5, origin=(300, 200), player=3, resources=None,
            query_result=None, shell=False, mutate=False):
    results = []
    for c, u, state in machines:
        setup(u, state, phase=phase, origin=origin, player=player)
        u.mem_write(0x784de8, bytes(36*4))
        for slot, amount in (resources or {}).items(): put(u, 0x784de8+slot*4, amount)
        calls = []; outputs(c, state, calls, mutate)
        if query_result is not None:
            def query(uc, args):
                assert args == (0,)
                calls.append((0x40cc40, args))
                put(uc, 0xf53f0c, 400)
                put(uc, 0xf53f08, 240)
                put(uc, 0xe20778, 1000)
                return query_result
            hook(c, state, 0x40cc40, 1, query)
        if shell:
            put(u, 0x605a44, 14)
            put(u, 0xe38dac, 600); put(u, 0xe38db0, 800)
            byte(u, 0xe38dca, 1)
            for at, value in [(0xf55c0c, 20000), (0xf5691c, 22000), (0xf56914, 21000)]: put(u, at, value)
        run(c, u, 0x520ea0 if shell else 0x52b520)
        results.append((calls, snapshot(u)))
    assert results[0] == results[1], ('monument background', phase, origin,
        resources, query_result, shell, mutate, results[0][0], results[1][0],
        [(hex(0x560000+i), a, b) for i, (a, b) in enumerate(zip(results[0][-1], results[1][-1])) if a != b][:12])
    return calls


def verify():
    count = 0
    for phase, origin, player in itertools.product(
            [-2147483648, -1, 0, 1, 4, 5, 6, 256, 2147483647],
            [(0, 0), (192, 144), (300, 200)], [0, 3]):
        compare(phase=phase, origin=origin, player=player)
        count += 1
    assert compare(phase=1) == [(0x4cd680, (364, 232, 512, 304, 0))]
    for slot in range(36):
        calls = compare(resources={slot: 256})
        assert [args[1] for token, args in calls if token == 0x4d62f0][-1] == (3 if slot else 12)
        count += 1
    for quantity in [-2147483648, -1, 0, 1, 2147483647]:
        compare(resources={i: quantity for i in range(36)})
        count += 1
    for value in [0, 1, 256, -1]:
        compare(query_result=value)
        count += 1
    for mutate, shell in [(True, False), (False, True), (True, True)]:
        compare(mutate=mutate, shell=shell, resources={35: 256})
        count += 1
    print('PASS whole monument background, all resource slots, signed demand, cursor/full result and shared-shell compositions:', count, flush=True)
    count = 0
    for excluded, mode, amount in itertools.product([-1, 0, 1, 35, 36], [0, 1, 256], [-1, 0, 1]):
        results = []
        for c, u, state in machines:
            reset(u, state); put(u, 0x605a3c, mode)
            for i in range(36): put(u, 0x784de8+i*4, amount)
            value = run(c, u, 0x40cc40, [excluded])
            expected = (35 if 0 <= excluded < 36 else 36) if amount > 0 else 0
            assert value == expected
            results.append((value, snapshot(u)))
        assert results[0] == results[1], ('resource query', excluded, mode, amount)
        count += 1
    print('PASS reused complete resource query, excluded-index boundaries, full signed amounts and both mode branches:', count, flush=True)
    print('Verified target SHA256:', target_sha)


if __name__ == '__main__': verify()
