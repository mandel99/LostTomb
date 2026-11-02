"""Whole mood calculation and effect timers against pinned native instructions.

Real RNG, timers and temple classification execute. Tutorial publication and
the three-argument god-effect endpoint are observed in isolated caller cases;
separate no-effect compositions execute the actual god-effect implementation.
No game process, invented RNG policy or inherited message scratch is used.
"""
from verify_kingdom_advisor import *


def seed_for(god):
    return next(seed for seed in range(1000)
                if (((seed*0x343fd+0x269ec3) & 0xffffffff) >> 16 & 0x7fff) % 5 == god)


def setup(u, state, player=3, population=1000, coverage=50, elapsed=12,
          level=50, roles=(1, 2, 1, 2, 1), difficulty=2, counter=0,
          seed_value=1, scenario=1, enabled=1, tutorial=0, flags=0,
          hold=0, temple=0, temple_kind=60, cooldown=0):
    reset(u, state)
    for owner in [0, 3]:
        off = owner*0x49d8
        u.mem_write(0xea1894+off, bytes(0x49d8))
        put(u, 0xea1894+off, population)
        word(u, 0xea5ce8+off, temple)
        put(u, 0xea5f9c+off, 511)
        put(u, 0xea5e48+off, cooldown)
        for i in range(5):
            byte(u, 0xea5c00+off+i, 77+i)
            byte(u, 0xea5c0a+off+i, level if isinstance(level, int) else level[i])
            byte(u, 0xea5c14+off+i, counter)
            byte(u, 0xea6086+off+i, counter)
            put(u, 0xea5c50+off+i*4, elapsed if isinstance(elapsed, int) else elapsed[i])
    for at, value in [(0xeaac28, player), (0xe38ebc, scenario),
                      (0xe39084, enabled), (0xe39080, difficulty),
                      (0x78d7c4, hold), (0x604950, seed_value)]:
        put(u, at, value)
    for at, value in [(0x7027bc, tutorial), (0x7027bd, 0),
                      (0xe38ea8, flags), (0xe38ea9, 0)]:
        byte(u, at, value)
    word(u, 0x7999e8+511*264, temple_kind)
    for i in range(5):
        word(u, 0x7848e2+i*2, roles[i])
        put(u, 0x78d620+i*4, coverage if isinstance(coverage, int) else coverage[i])


def compare(mode=1, genuine_effect=False, mutate_effect=False,
            mutate_tutorial=False, query_result=None, observe_messages=False, **kwargs):
    results = []
    for c, u, state in machines:
        setup(u, state, **kwargs)
        calls = []
        def tutorial(uc, args):
            calls.append((0x417ae0, args))
            if mutate_tutorial:
                put(uc, 0xeaac28, 0)
                word(uc, 0xea5ce8, 256)
            return 0
        hook(c, state, 0x417ae0, 1, tutorial)
        if observe_messages:
            def publish(uc, args):
                calls.append((0x4e0cf0, args))
                assert args[1:] == (0, 0, 0, 0)
                return 0
            hook(c, state, 0x4e0cf0, 5, publish)
        if not genuine_effect:
            def effect(uc, args):
                calls.append((0x4c47a0, args))
                assert args[0] <= 4 and args[1] <= 2 and args[2] == 0xffffffff
                if mutate_effect:
                    put(uc, 0xeaac28, 0)
                    byte(uc, 0xea5c0a, 20)
                return 0
            if c:
                state['hooks'][sym('restore_god_effect_004c47a0')] = (3, 0, effect)
            else:
                hook(c, state, 0x4c47a0, 3, effect)
        if query_result is not None:
            def query(uc, args):
                calls.append((0x4c06e0, args))
                put(uc, 0xeaac28, 0 if get(uc, 0xeaac28) else 3)
                return query_result
            hook(c, state, 0x4c06e0, 2, query)
        run(c, u, 0x4e9890, [mode])
        results.append((calls, snapshot(u)))
    assert results[0] == results[1], (mode, kwargs, genuine_effect,
        mutate_effect, mutate_tutorial, query_result, results[0][0], results[1][0],
        [(hex(0x560000+i), a, b) for i, (a, b) in
         enumerate(zip(results[0][-1], results[1][-1])) if a != b][:12])
    return results[1]


def verify_preview():
    count = 0
    for player, scenario, tutorial, mode in itertools.product(
            [0, 3], [0, 1, 256], [0, 1], [0, 1]):
        compare(mode, player=player, scenario=scenario, tutorial=tutorial)
        count += 1
    for population in [-2147483648, -1, 0, 399, 400, 449, 450, 499,
                       500, 549, 550, 599, 600, 2147483647]:
        compare(0, population=population, coverage=[-1, 0, 50, 100, 256],
                elapsed=[-2147483648, -1, 0, 40, 2147483647], roles=(0, 1, 2, 256, -1))
        count += 1
    result = compare(0, coverage=20, elapsed=0)
    assert result[1][0xea5c00+3*0x49d8-0x560000:0xea5c05+3*0x49d8-0x560000] == bytes([32]*5)
    count += 1
    print('PASS religion preview/monthly entry gates, full player, population boundaries and BYTE wrap:', count, flush=True)


def verify_monthly():
    count = 0
    for difficulty, level in itertools.product(range(5),
            [-128, -1, 0, 10, 11, 20, 21, 30, 31, 49, 50, 51, 69, 70, 79, 80, 89, 90, 100, 127]):
        god = (difficulty+level) % 5
        compare(level=level, coverage=level, difficulty=difficulty, counter=49,
                seed_value=seed_for(god))
        count += 1
    for counter, level in itertools.product([0, 19, 20, 49, 50, 127, 128, 255], [0, 50, 100]):
        compare(counter=counter, level=level, coverage=level)
        count += 1
    for god, cooldown in itertools.product(range(5), [0, 1, -1, -2147483648]):
        compare(seed_value=seed_for(god), cooldown=cooldown, level=20, coverage=20,
                roles=(0, 256, 2, 1, -1))
        count += 1
    for temple, kind in itertools.product([0, 1, 256], [60, 61, 62, 63, 64, -1]):
        compare(temple=temple, temple_kind=kind, roles=(1, 2, 256, 0, -1), coverage=99)
        count += 1
    for hold, enabled, mode in itertools.product([0, 256], [0, 256], [-1, 256]):
        compare(mode, hold=hold, enabled=enabled)
        count += 1
    print('PASS real RNG/all gods, difficulty tables, signed counters, temples, hold and cooldown:', count, flush=True)


def verify_callbacks_and_compositions():
    count = 0
    # Use the authentic phase as well as nonmatching signed BYTE values.
    value = struct.unpack_from('<I', seed, 0x57953c-0x560000)[0]
    for flags, tutorial, done, level in itertools.product(sorted({0, 1, 255, value}), [0, 1], [0, 1], [39, 40]):
        results = []
        for c, u, state in machines:
            setup(u, state, scenario=0, tutorial=tutorial, flags=flags, level=level,
                  coverage=level, hold=1)
            byte(u, 0x7027bd, done)
            calls = []
            hook(c, state, 0x417ae0, 1, lambda uc, args: calls.append(args) or 0)
            run(c, u, 0x4e9890, [1])
            results.append((calls, snapshot(u)))
        assert results[0] == results[1], ('tutorial', flags, tutorial, done, level, value)
        count += 1
    compare(mutate_effect=True)
    compare(temple=256, query_result=0x1003c)
    compare(temple=256, query_result=60)
    result = compare(scenario=0, tutorial=1, flags=value, level=40,
                     coverage=40, mutate_tutorial=True)
    assert (0x417ae0, (5,)) in result[0]
    count += 4
    for god in range(5):
        compare(genuine_effect=True, seed_value=seed_for(god), counter=0,
                level=50, coverage=50, roles=(1, 1, 1, 1, 1))
        count += 1
    print('PASS tutorial gates, callback player reloads, full temple query results and genuine no-effect compositions:', count, flush=True)


def verify_timers():
    count = 0
    for player, value in itertools.product([0, 3], [-2147483648, -32768, -1, 0, 1, 32767, 2147483647]):
        results = []
        for c, u, state in machines:
            setup(u, state, player=player)
            off = player*0x49d8
            for i in range(3): word(u, 0xea61f4+off+i*2, value)
            for field in [0xea5e3c, 0xea6024]: put(u, field+off, value)
            run(c, u, 0x4c51d0)
            results.append(snapshot(u))
        assert results[0] == results[1], ('timers', player, value)
        count += 1
    print('PASS signed WORD/DWORD timers, zero expiry and full player stride:', count, flush=True)


def verify_selection():
    count = 0
    cases = [(tuple(256 if mask & (1 << i) else 0 for i in range(5)),
              [0, 0, 0, 0, 0], [39, -128, 39, 50, 0]) for mask in range(32)]
    cases += [((1, 1, 1, 1, 1), wrath, levels) for wrath, levels in [
        ([0]*5, [40]*5), ([0]*5, [39]*5), ([50, 50, 0, 0, 0], [0]*5),
        ([0, 0, 0, 0, 127], [-128]*5), ([-128, -1, 0, 0, 0], [50]*5),
        ([0, 1, 0, 0, 0], [-128]*5), ([0, 0, 0, -1, 0], [50, 50, -1, -1, 0])]]
    for player, (roles, wrath, levels) in itertools.product([0, 3], cases):
        results = []
        for c, u, state in machines:
            setup(u, state, player=player, roles=roles, level=levels)
            off = player*0x49d8
            for i, value in enumerate(wrath): byte(u, 0xea5c14+off+i, value)
            put(u, 0xea5c78+off, 0x12345678)
            value = run(c, u, 0x4e9f50)
            assert value in [0, 1]
            results.append((value, snapshot(u)))
        assert results[0] == results[1], ('least happy', player, roles, wrath, levels)
        count += 1
    print('PASS complete least-happy query, all god masks, signed priorities/ties and full EAX:', count, flush=True)


def verify_effect_compositions():
    count = 0
    for level, counter, divisor, message in [(100, 49, 2, 0x128),
            (100, 20, 2, 0x14b), (0, 49, 3, 0x14c), (0, 20, 2, 0x14e)]:
        def qualifies(initial):
            values = []
            for _ in range(3):
                initial = (initial*0x343fd+0x269ec3) & 0xffffffff
                values.append(initial >> 16 & 0x7fff)
            return values[0] % 5 == 1 and values[2] % divisor == 0
        initial = next(value for value in range(1000) if qualifies(value))
        calls, memory = compare(genuine_effect=True, observe_messages=True,
            level=level, coverage=level, counter=counter, seed_value=initial,
            roles=(1, 1, 1, 1, 1))
        assert calls == [(0x4e0cf0, (message, 0, 0, 0, 0))], (level, counter, calls)
        count += 1
    print('PASS genuine monthly/RNG/major-minor blessing-curse/timer compositions, message publication observed:', count, flush=True)


if __name__ == '__main__':
    verify_preview()
    verify_monthly()
    verify_callbacks_and_compositions()
    verify_timers()
    verify_selection()
    verify_effect_compositions()
    print('Verified target SHA256:', target_sha)
