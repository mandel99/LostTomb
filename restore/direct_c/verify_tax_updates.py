"""Complete monthly tax and forecast, original x86 versus current Release.

Actual percentage helpers execute unless an explicit callback case controls
them. Calendar/menu compositions run all finance providers, while unrelated
simulation and the file-save endpoint are controlled. No game process runs.
"""
from verify_financial_year import *

TAX_FIELDS = [0xea4490, 0xea4494, 0xea4498, 0xea449c, 0xea44a0, 0xea44a4,
              0xea44a8, 0xea44ac, 0xea44b0, 0xea44b4, 0xea44b8,
              0xea44bc, 0xea44c4, 0xea44c8, 0xea44cc, 0xea44d0, 0xea5e5c]


def prepare_tax(u, state, rows=(), high_water=None, rate=10, difficulty=2,
                month=3, player=0, metrics=None):
    prepare_year(u, state, player=player, month=month)
    u.mem_write(0x7999d8, bytes(4001*264))
    for owner in (0, 3):
        off = owner*0x49d8
        for i, at in enumerate(TAX_FIELDS): put(u, at+off, 10+i+owner)
        put(u, 0xea1878+off, rate); put(u, 0xea187c+off, 10000)
        u.mem_write(0xea3ef8+off, b'\xa5'*88)
        put(u, 0xea5ccc+off, 100)
    for i, value in enumerate([50, 75, 100, 125, 150]): put(u, 0x5d1974+i*4, value)
    put(u, 0xe39080, difficulty)
    if difficulty == 256: put(u, 0x5d1974+difficulty*4, 137)
    put(u, 0x78d790, max((d.get('id', i) for i, d in enumerate(rows, 1)), default=0)
        if high_water is None else high_water)
    for kind in range(20): put(u, 0x70ce18+kind*88, (metrics or {}).get(kind, 15+kind*5))
    for i, d in enumerate(rows, 1):
        p = 0x7999d8+d.get('id', i)*264
        byte(u, p, d.get('state', 1)); byte(u, p+5, d.get('occupied', 1))
        word(u, p+18, d.get('kind', 13)); word(u, p+28, d.get('population', 100))
        byte(u, p+78, d.get('paying', 1)); byte(u, p+136, d.get('bonus', 0))
        put(u, p+140, d.get('history', 10))


def observation(u, rows):
    return (tuple(tuple(get(u, at+owner*0x49d8) for at in TAX_FIELDS) for owner in (0, 3)),
            tuple(bytes(u.mem_read(0x7999d8+d.get('id', i)*264, 144)) for i, d in enumerate(rows, 1)))


def compare_tax(token=0x4599d0, mutation=None, query_result=33, calendar=None,
                menu=None, **kw):
    rows = kw.get('rows', ())
    results = []
    for c, u, state in machines:
        prepare_tax(u, state, **kw); calls = []; queries = [0]; ratios = [0]
        if mutation:
            def percentage(uc, args):
                queries[0] += 1; n = queries[0]
                calls.append((0x4cde40, args, observation(uc, rows)))
                if mutation == 'owner': put(uc, 0xeaac28, 3 if n & 1 else 0)
                if mutation == 'house' and n <= 2:
                    p = 0x7999d8+n*264
                    word(uc, p+18, 14 if n == 1 else 13)
                    word(uc, p+28, 27+n); byte(uc, p+78, 0 if n == 1 else 255)
                if mutation in ('extend', 'truncate') and n == 1:
                    put(uc, 0x78d790, 2 if mutation == 'extend' else 0)
                if mutation == 'globals':
                    put(uc, 0xe39080, n % 5); put(uc, 0x78d7c0, 256+n)
                    put(uc, 0xea1878, 5+n); put(uc, 0xea1878+3*0x49d8, 9+n)
                return query_result
            hook(c, state, 0x4cde40, 2, percentage)
            if mutation == 'owner':
                def ratio(uc, args):
                    ratios[0] += 1; n = ratios[0]
                    calls.append((0x4cde60, args, observation(uc, rows)))
                    put(uc, 0xeaac28, 3 if n & 1 else 0)
                    put(uc, 0xea4494, 700+n); put(uc, 0xea449c+3*0x49d8, 500+n)
                    return 100+n
                hook(c, state, 0x4cde60, 2, ratio)
        if calendar is not None:
            put(u, 0x78d7c4, 15); put(u, 0x78d7c0, calendar); put(u, 0x78d7c8, 999)
            byte(u, 0xe38ec2, 1); byte(u, 0xe3908c, 0)
            byte(u, 0x7027b8, 0); byte(u, 0x7027be, 0); byte(u, 0x7027b9, 1)
            put(u, 0xbca2d0, 128); put(u, 0xbca2d4, 128)
            u.mem_write(0xbcd5f8, bytes(2000*388))
            for owner in (0, 3):
                put(u, 0xea609c+owner*0x49d8, 0); put(u, 0xea60a0+owner*0x49d8, 0)
            for endpoint, arity in CALENDAR_ENDPOINTS.items():
                if endpoint == 0x4599d0: continue
                def observe(uc, args, endpoint=endpoint):
                    calls.append((endpoint, args, get(uc, 0xea187c), observation(uc, rows))); return 1
                hook(c, state, endpoint, arity, observe)
        if menu is not None:
            put(u, 0xd3b198, menu)
            if c:
                for r, v in zip(registers, [0x11111111, 0x22222222, 0x33333333, saved[0], SP, *saved[1:]]): u.reg_write(r, v)
                u.reg_write(UC_X86_REG_EFLAGS, 2); put(u, SP, STOP); put(u, SP+4, 0x40a260)
                u.emu_start(sym('restore_menu_economy_action'), STOP, count=2000000)
                assert u.reg_read(UC_X86_REG_EIP) == STOP and u.reg_read(UC_X86_REG_ESP) == SP+4
                assert [u.reg_read(registers[i]) for i in [3, 5, 6, 7]] == saved
            else: run(c, u, 0x40a260)
        else: run(c, u, 0x452370 if calendar is not None else token)
        results.append((calls, snapshot(u)))
    assert results[0] == results[1], (hex(token), kw, mutation, query_result, calendar, menu,
        next(((i, a[:2], b[:2]) for i, (a, b) in enumerate(zip(results[0][0], results[1][0])) if a != b),
             ('call counts', len(results[0][0]), len(results[1][0]))),
        [(hex(0x560000+i), a, b) for i, (a, b) in enumerate(zip(results[0][-1], results[1][-1])) if a != b][:12])
    if calendar is not None: assert sum(call[0] == 0x4d4e20 for call in results[1][0]) == 1
    off = kw.get('player', 0)*0x49d8
    return dict(cash=get(u, 0xea187c+off), low=get(u, 0xea44a0+off), total=get(u, 0xea44a8+off),
                history=get(u, 0x7999d8+264+140))


def verify_collection():
    count = 0
    for kind, paying, bonus in itertools.product(range(20), [0, 1], [0, 1]):
        compare_tax(rows=({'kind': kind, 'paying': paying, 'bonus': bonus},)); count += 1
    for state, occupied, paying in itertools.product([0, 1, 2, 255], [0, 1, 255], [0, 255]):
        compare_tax(rows=({'state': state, 'occupied': occupied, 'paying': paying},)); count += 1
    for population, metric in itertools.product([-32768, -1, 0, 1, 32767], [-1, 0, 0x7fffffff, 0x80000000]):
        compare_tax(rows=({'population': population},), metrics={13: metric}); count += 1
    for high in [-1, 0, 1, 4000]:
        compare_tax(rows=({'id': max(high, 1)},), high_water=high); count += 1
    for player, rate, difficulty in itertools.product([0, 3], [-1, 0, 10, 25, 256], [0, 2, 4, 256]):
        compare_tax(rows=({'kind': 13}, {'kind': 14, 'paying': 0}),
                    player=player, rate=rate, difficulty=difficulty); count += 1
    sample = compare_tax(rows=({},)); count += 1
    assert sample == dict(cash=10800, low=100, total=100, history=8000010), sample
    print('PASS complete monthly tax, all house levels/flags, signed population, history, high water and money:', count, flush=True)


def verify_projection_and_callbacks():
    count = 0
    for kind, paying, month in itertools.product([0, 13, 14, 19], [0, 1], [-1, 0, 11, 12, 256]):
        compare_tax(0x459d20, rows=({'kind': kind, 'paying': paying, 'bonus': 1},), month=month); count += 1
    for token, mutation in itertools.product([0x4599d0, 0x459d20], ['owner', 'house', 'globals']):
        compare_tax(token, rows=({'kind': 13, 'bonus': 1}, {'kind': 14, 'bonus': 1}), mutation=mutation); count += 1
    for mutation, high in [('extend', 1), ('truncate', 2)]:
        compare_tax(rows=({'kind': 13, 'bonus': 1}, {'kind': 14}),
                    high_water=high, mutation=mutation); count += 1
    for value in [-2147483648, -1, 0, 1, 2147483647]:
        compare_tax(rows=({'bonus': 1},), mutation='owner', query_result=value); count += 1
    print('PASS forecast DWORD month/no bonus, cached classifications, live limits, owner/difficulty/query order:', count, flush=True)


def verify_integrations():
    rows = ({'kind': 1, 'population': 20}, {'kind': 13, 'population': 50, 'bonus': 1},
            {'kind': 14, 'population': 100, 'paying': 0}, {'kind': 19, 'population': 200})
    count = 0
    for player, month in itertools.product([0, 3], [0, 11]):
        compare_tax(rows=rows, player=player, calendar=month); count += 1
    for rate, action in itertools.product([-1, 0, 25, 26], [0, 1, 256]):
        compare_tax(rows=rows, rate=rate, menu=action); count += 1
    print('PASS actual calendar/month/year financial chain and tax-rate menu routing; no tax substitution:', count, flush=True)
    print('Verified target SHA256:', target_sha, flush=True)


if __name__ == '__main__':
    verify_collection(); verify_projection_and_callbacks(); verify_integrations()
