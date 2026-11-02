"""Annual closing, projections and real calendar/menu finance integration.

Offline original-x86/current-Release comparisons; no game or file save runs.
The actual calendar composition controls unrelated simulation/save endpoints,
while monthly wages/salary and every annual finance rollover execute.
"""
from verify_financial_updates import *

YEAR_FIELDS = [0xea5cc4, 0xea5cc8, 0xea601c, 0xea6020, 0xea5d58, 0xea5d5c,
               0xea5d54, 0xea44c0, 0xea44d8, 0xea4500, 0xea44e8, 0xea44f8,
               0xea448c, 0xea44e0, 0xea4478, 0xea4464, 0xea4484, 0xea5f10,
               0xea44e4, 0xea4510, 0xea44ac, 0xea44b0, 0xea5d4c, 0xea5e18,
               0xea5ff4, 0xea5fcc, 0xea3fac, 0xea5fd0, 0xea5fd4, 0xea5bfc,
               0xea4508, 0xea620c, 0xea5ccc]
CALENDAR_ENDPOINTS = {token: 0 for token in [
    0x4e81d0, 0x4599d0, 0x4474e0, 0x45e420,
    0x4464f0, 0x44ba90, 0x4ea740, 0x4b7960, 0x4e1850, 0x47b570,
    0x517370, 0x4e1640, 0x4e7700, 0x4bc7a0, 0x4e8630,
    0x4448e0, 0x455530, 0x4c5300, 0x4e9700, 0x4ea000, 0x4e8840, 0x4e8db0]}
CALENDAR_ENDPOINTS.update({0x475cc0: 5, 0x46e180: 1, 0x4d4e20: 1})


def prepare_year(u, state, player=0, income=10000, expense=1000, cash=10000,
                 population=2000, mode=0, profit=0, month=3, gate=0, rate=10, updates=None):
    prepare_debt(u, state, player=player, cash=cash, population=population, mode=mode)
    for owner in (0, 3):
        off = owner*0x49d8
        for at in YEAR_FIELDS: put(u, at+off, 0)
        put(u, 0xea44c0+off, income); put(u, 0xea4500+off, expense)
        put(u, 0xea4528+off, profit); put(u, 0xea5d54+off, 77+owner)
        put(u, 0xea5d5c+off, 0xffffffff)
        word(u, 0xea5ce8+off, gate); put(u, 0xea5f9c+off, 511)
        for at, value in (updates or {}).items(): put(u, at+off, value)
    put(u, 0x78d7c0, month); put(u, 0x78d790, 0); put(u, 0x784ddc, rate)
    u.mem_write(0x7999d8+511*264, bytes(264)); word(u, 0x7999e8+511*264, 60)


def finance_state(u):
    return tuple(tuple(get(u, at+owner*0x49d8) for at in
        [0xea187c, 0xea5d54, 0xea4514, 0xea451c, 0xea452c]) for owner in (0, 3))


def compare_year(token=0x45a5d0, mutation=None, query_result=200,
                 calendar=False, menu=False, temple_result=4, **kw):
    results = []
    for c, u, state in machines:
        prepare_year(u, state, **kw); calls = []; queries = [0]
        if mutation == 'percentage':
            def percentage(uc, args):
                queries[0] += 1
                calls.append((0x4cde40, args, finance_state(uc)))
                put(uc, 0xeaac28, 3 if queries[0] & 1 else 0)
                put(uc, 0xea44c0+3*0x49d8, 999999)
                put(uc, 0xea5d54+3*0x49d8, 123)
                return query_result
            hook(c, state, 0x4cde40, 2, percentage)
        if mutation == 'reserve':
            def reserve(uc, args):
                calls.append((0x45a7b0, finance_state(uc)))
                put(uc, 0xeaac28, 3); put(uc, 0xea187c+3*0x49d8, 1234)
                return 0
            hook(c, state, 0x45a7b0, 0, reserve)
        if mutation == 'temple':
            def temple(uc, args):
                calls.append((0x4c5120, args)); put(uc, 0xeaac28, 3)
                put(uc, 0xea44e4+3*0x49d8, 0xfedcba98)
                put(uc, 0x784ddc, 99); return temple_result
            hook(c, state, 0x4c5120, 1, temple)
        if calendar:
            put(u, 0x78d7c4, 15); put(u, 0x78d7c0, 11); put(u, 0x78d7c8, 999)
            byte(u, 0xe38ec2, 1); byte(u, 0xe3908c, 0)
            byte(u, 0x7027b8, 0); byte(u, 0x7027be, 0); byte(u, 0x7027b9, 1)
            put(u, 0xbca2d0, 128); put(u, 0xbca2d4, 128)
            u.mem_write(0xbcd5f8, bytes(2000*388))
            for owner in (0, 3):
                put(u, 0xea609c+owner*0x49d8, 0); put(u, 0xea60a0+owner*0x49d8, 0)
            for endpoint, arity in CALENDAR_ENDPOINTS.items():
                def observe(uc, args, endpoint=endpoint):
                    calls.append((endpoint, args, finance_state(uc)))
                    return 1
                if c and endpoint == 0x4599d0 and re.search(r'\s_restore_calendar_C_004599d0\s', symbols):
                    # MSVC inlines the public finance facade into the calendar;
                    # observe its actual model boundary, not a nonexistent export.
                    def model(uc, args, observe=observe):
                        f = args[0]; sp = get(uc, f+16); value = observe(uc, ())
                        put(uc, f, value); put(uc, f+16, sp+4); return value
                    state['hooks'][sym('restore_calendar_C_004599d0')] = (1, 0, model)
                else: hook(c, state, endpoint, arity, observe)
        if menu and c:
            for r, v in zip(registers, [0x11111111, 0x22222222, 0x33333333, saved[0], SP, *saved[1:]]): u.reg_write(r, v)
            u.reg_write(UC_X86_REG_EFLAGS, 2); put(u, SP, STOP); put(u, SP+4, 0x40af90)
            u.emu_start(sym('restore_menu_economy_action'), STOP, count=2000000)
            assert u.reg_read(UC_X86_REG_EIP) == STOP and u.reg_read(UC_X86_REG_ESP) == SP+4
            assert [u.reg_read(registers[i]) for i in [3, 5, 6, 7]] == saved
        else: run(c, u, 0x452370 if calendar else 0x40af90 if menu else token)
        results.append((calls, snapshot(u)))
    assert results[0] == results[1], (hex(token), kw, mutation, query_result, calendar, menu,
        results[0][0], results[1][0],
        [(hex(0x560000+i), a, b) for i, (a, b) in enumerate(zip(results[0][-1], results[1][-1])) if a != b][:12])
    if calendar:
        assert sum(call[0] == 0x4d4e20 for call in results[1][0]) == 1
        assert [call[1] for call in results[1][0] if call[0] == 0x46e180] == [(1,)]


def verify_closing():
    count = 0
    for population, relation, mode in itertools.product(
            [-1, 500, 501, 1000, 1001, 2000, 2001, 3000, 3001, 5000, 5001],
            [-1, 0, 1], [0, 10]):
        compare_year(population=population, income=1000+relation, expense=1000, mode=mode); count += 1
    for cash, income, expense, player in itertools.product(
            [-2147483648, -1, 0, 1, 2147483647], [0, 0x7fffffff, 0x80000000, 0xffffffff], [0, 1000], [0, 3]):
        compare_year(cash=cash, income=income, expense=expense, player=player); count += 1
    for amount in [-1, 0, 200, 0x7fffffff, 0x80000000]:
        compare_year(mutation='percentage', query_result=amount); count += 1
    for profit in [-1, 0, 1, 0x7fffffff]:
        compare_year(profit=profit, mutation='percentage'); count += 1
    compare_year(mutation='reserve'); count += 1
    compare_year(updates={0xea44c0: 0x7fffffff, 0xea44d8: 1, 0xea5cc8: 123,
                          0xea6020: 456, 0xea4500: 0xffffffff, 0xea44e8: 1}); count += 1
    print('PASS annual transfer/tribute, population thresholds, signed totals, overflow and owner changes:', count, flush=True)


def verify_projections():
    count = 0
    for month, workers, wages in itertools.product([-1, 0, 11, 12, 256, 0x7fffffff],
            [-1, 0, 120, 0x7fffffff], [1, 119, 120, 0x80000000]):
        compare_year(0x45a8c0, month=month, updates={0xea4478: workers, 0xea4464: wages, 0xea4484: 12345}); count += 1
    for gate, player in itertools.product([0, 1, 256, 0xffff], [0, 3]):
        compare_year(0x45a930, gate=gate, player=player, updates={0xea44e4: 0xabcdef01}); count += 1
    compare_year(0x45a930, gate=256, mutation='temple'); count += 1
    for value, player in itertools.product([-1, 0, 1, 0x80000000], [0, 3]):
        compare_year(0x45a970, player=player, updates={0xea4510: value}); count += 1
    print('PASS wage product/divisions, DWORD month, query/WORD gate, fresh owner and salary snapshot:', count, flush=True)


def verify_integrations():
    count = 0
    update = {0xea44ac: 3000, 0xea44b0: 4000, 0xea4478: 120, 0xea4464: 100,
              0xea4484: 1000, 0xea44d4: 100, 0xea44dc: 200, 0xea44e4: 300,
              0xea4510: 50, 0xea44f4: 20, 0xea44fc: 10, 0xea5fc8: 5,
              0xea5cc8: 40, 0xea6020: 30, 0xea4508: 10, 0xea620c: 1}
    for player, cash, mode in itertools.product([0, 3], [-1, 10000], [0, 10]):
        compare_year(player=player, cash=cash, mode=mode, updates=update, calendar=True); count += 1
    for player, amount in itertools.product([0, 3], [0, 500, 0xffffffff]):
        compare_year(player=player, updates={0xea5ccc: amount}, menu=True); count += 1
    print('PASS actual annual rollover chain/calendar/year/save ordering and menu donation finance:', count, flush=True)
    print('Verified target SHA256:', target_sha, flush=True)


def verify_interest():
    count = 0
    for cash, rate, gate in itertools.product([-2147483648, -10000, -1, 0, 1],
            [-1, 0, 4, 5, 10, 0x7fffffff, 0x80000000], [0, 1]):
        compare_year(0x45a350, cash=cash, rate=rate, gate=gate); count += 1
    for result, rate in itertools.product([0, 1, 4, 0x101], [4, 10, 0x80000000]):
        compare_year(0x45a350, cash=-10000, rate=rate, gate=256,
                     mutation='temple', temple_result=result); count += 1
    compare_year(0x45a350, cash=-10000, mutation='percentage'); count += 1
    print('PASS monthly interest, cached rate/full query return, NEG/division/wrap and fresh payment owner:', count, flush=True)


if __name__ == '__main__':
    verify_closing(); verify_projections(); verify_interest(); verify_integrations()
