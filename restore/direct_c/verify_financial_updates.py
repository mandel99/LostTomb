"""Whole debt/ledger updates against original x86, without a game process.

Actual percentage/ledger/reserve/reputation and town iteration execute.
Messages and scheduling are controlled in isolated cases. Compositions run
the queued publisher or actual scheduling/copy/reset/append graph; the latter
declares immediate invasion execution as an observed external boundary.
"""
from verify_kingdom_advisor import *

FIELDS = [0xea187c, 0xea1894, 0xea5d08, 0xea5d0c, 0xea609c, 0xea60a0,
          0xea5b48, 0xea5b40, 0xea5ca8, 0xea5cbc, 0xea5cc8, 0xea6020,
          0xea44bc, 0xea44d4, 0xea5fc8, 0xea44f4, 0xea4504, 0xea44ec,
          0xea44fc, 0xea4488, 0xea44dc, 0xea4514, 0xea4518, 0xea451c,
          0xea4520, 0xea4524, 0xea4528, 0xea452c, 0xea4530, 0xea5d50]


def prepare_debt(u, state, player=0, cash=-1, used=0, warned=0, aid=1000,
                 difficulty=2, mode=0, rating=50, population=2000,
                 records=(), updates=None):
    reset(u, state)
    for reg, value in [(UC_X86_REG_FPCW, 0x37f), (UC_X86_REG_FPSW, 0),
                       (UC_X86_REG_FPTAG, 0xffff)]: u.reg_write(reg, value)
    for owner in (0, 3):
        off = owner*0x49d8
        for i, at in enumerate(FIELDS): put(u, at+off, 100+i*13+owner)
        for at, value in [(0xea187c, cash), (0xea1894, population), (0xea5d08, used),
                          (0xea5d0c, warned), (0xea5cbc, mode), (0xea5b48, rating)]: put(u, at+off, value)
        for at, value in (updates or {}).items(): put(u, at+off, value)
    for at, value in [(0xeaac28, player), (0x784c94, aid), (0xe39080, difficulty),
                      (0xe950dc, 0), (0xe950e0, 0), (0xe951ec, 0), (0xe951f0, 42),
                      (0xe951e8, 0xffffffff), (0xf55b88, 0), (0x78d7bc, 1000),
                      (0x78d7c0, 3), (0x7848d8, 900), (0xd3b1c4, 0),
                      (0xd3b380, 0x1357), (0xd3b384, 0x2468)]: put(u, at, value)
    for i, value in enumerate([50, 75, 100, 125, 150]): put(u, 0x5d1960+i*4, value)
    if difficulty == 256: put(u, 0x5d1960+difficulty*4, 175)
    byte(u, 0xe38dda, 2); byte(u, 0xe38dc9, 0); byte(u, 0xe38dca, 0)
    u.mem_write(0xe95228, bytes(1000*48)); u.mem_write(0xe95080, bytes(20*4))
    for message in [115, 116, 117]: u.mem_write(0xe209b8+message*80, bytes(80))
    u.mem_write(0x785d70, bytes(18628)); put(u, 0x785d70, len(records))
    for i, data in enumerate(records):
        p = 0x785d74+i*124
        word(u, p, i); byte(u, p+2, data.get('type', 2)); byte(u, p+3, 9)
        for start, value in [(4, data.get('kind', 3)), (12, 100), (20, 6), (28, 2), (88, 0)]:
            word(u, p+start, value); word(u, p+start+2, value)
            word(u, p+start+4, -1); word(u, p+start+6, -1)
        for at in [36, 38, 82, 84]: word(u, p+at, -1)
        put(u, p+40, data.get('flags', 16)); byte(u, p+50, 1)


def observe_schedule(c, state, calls, mutation):
    def action(uc, self, template, reason):
        calls.append((0x44c490, self, template, reason, bytes(uc.mem_read(self, 124))))
        if mutation == 'schedule':
            put(uc, self+40, 0xaabb0010); byte(uc, self+50, 255); byte(uc, self+3, 2)
            put(uc, 0xeaac28, 3)
        return 0
    if c:
        state['hooks'][sym('restore_event_schedule_0044c490')] = (3, 0,
            lambda uc, args: action(uc, *args))
    else:
        hook(c, state, 0x44c490, 2,
             lambda uc, args: action(uc, uc.reg_read(UC_X86_REG_ECX), *args), cleanup=8)


def observe_invasion(c, state, calls):
    if c:
        def action(uc, args):
            f = args[0]; sp = get(uc, f+16); self = get(uc, f+4)
            calls.append((0x451540, self)); put(uc, f, 0); put(uc, f+16, sp+4)
            return 0
        state['hooks'][sym('restore_event_C_00451540')] = (1, 0, action)
    else:
        hook(c, state, 0x451540, 0,
             lambda uc, args: calls.append((0x451540, uc.reg_read(UC_X86_REG_ECX))) or 0)


def compare_debt(token=0x4460b0, real_schedule=False, real_message=False,
                 mutation=None, tick=False, **kw):
    results = []
    for c, u, state in machines:
        prepare_debt(u, state, **kw); calls = []; percentages = [0]
        if not real_schedule: observe_schedule(c, state, calls, mutation)
        else: observe_invasion(c, state, calls)
        if not real_message:
            def message(uc, args):
                calls.append((0x4e0cf0, args, get(uc, 0xe950dc)))
                if mutation == 'message':
                    put(uc, 0xeaac28, 3); put(uc, 0xea5b48+3*0x49d8, 0)
                return 0
            hook(c, state, 0x4e0cf0, 5, message)
        if mutation in ('percent', 'reserve'):
            def percentage(uc, args):
                percentages[0] += 1; n = percentages[0]
                calls.append((0x4cde40, args, tuple(get(uc, 0xea5d50+p*0x49d8) for p in (0, 3))))
                owner = 3 if n & 1 else 0
                put(uc, 0xeaac28, owner)
                if mutation == 'percent':
                    put(uc, 0xea187c, 700); put(uc, 0xea5cc8+3*0x49d8, 3000)
                    put(uc, 0x784c94, 2000+n); put(uc, 0xe39080, n % 5)
                return n*200
            hook(c, state, 0x4cde40, 2, percentage)
        if tick:
            for at in [0x78d79c, 0x78d7a8, 0x78d7a0, 0x78d7b0, 0x78d7a4]: put(u, at, 0)
            byte(u, 0xeaaf5c, 0); put(u, 0x78d7cc, 4)
        run(c, u, 0x451c50 if tick else token)
        results.append((calls, snapshot(u)))
    assert results[0] == results[1], (hex(token), kw, real_schedule, real_message, mutation, tick,
        results[0][0], results[1][0],
        [(hex(0x560000+i), a, b) for i, (a, b) in enumerate(zip(results[0][-1], results[1][-1])) if a != b][:12])
    return results[1][0]


def verify_debt():
    count = 0
    for cash, used, warned, player in itertools.product(
            [-2147483648, -1, 0, 1, 2147483647], [0, 1, 256], [0, 1], [0, 3]):
        compare_debt(cash=cash, used=used, warned=warned, player=player); count += 1
    for aid, difficulty, mode in itertools.product([-2147483648, -1, 0, 1, 1000], [0, 2, 4, 256], [0, 10, 256]):
        compare_debt(aid=aid, difficulty=difficulty, mode=mode); count += 1
    for rating, records in itertools.product([0, 1, 256], [(), ({'flags': 0},),
            ({'flags': 0}, {'flags': 16}), ({'type': 1}, {'kind': 2}),
            ({'flags': 20}, {'flags': 16}), ({'kind': 3},), ({'kind': 0x10003},)]):
        compare_debt(cash=1, rating=rating, records=records); count += 1
    for mutation in ['percent', 'message', 'schedule']:
        compare_debt(mutation=mutation, rating=0, records=({'kind': 2},)); count += 1
    print('PASS debt state/reset/grant/warnings, full fields, event receiver, stop order and callback reloads:', count, flush=True)


def verify_ledgers():
    count = 0
    for cash, profit, population in itertools.product([-1, 0, 1], [-1, 0, 1, 10000],
            [-1, 500, 501, 1000, 1001, 2000, 2001, 3000, 3001, 5000, 5001]):
        compare_debt(0x45a7b0, cash=cash, population=population, updates={0xea4528: profit}); count += 1
    for player, value in itertools.product([0, 3], [-2147483648, -1, 0, 2, 3, 4, 2147483647]):
        compare_debt(0x46e840, player=player, updates={0xea5b40: value}); count += 1
    for player, value in itertools.product([0, 3], [0, 1, 0x7fffffff, 0x80000000, 0xffffffff]):
        compare_debt(0x45a990, player=player, updates={at: value for at in FIELDS}); count += 1
    compare_debt(0x45a7b0, cash=1, updates={0xea4528: 10000}, mutation='reserve'); count += 1
    print('PASS ledger wrap, reserve thresholds/transient query stores and reputation sign test:', count, flush=True)


def verify_compositions():
    count = 0
    for mode, used, warned in [(0, 0, 0), (10, 0, 0), (0, 1, 0), (0, 1, 1)]:
        compare_debt(real_message=True, mode=mode, used=used, warned=warned); count += 1
    for kind, mutation in itertools.product([2, 3], [None, 'message']):
        compare_debt(cash=-1, rating=0, records=({'kind': kind},),
                     real_schedule=True, mutation=mutation); count += 1
    for player, cash in itertools.product([0, 3], [-1, 0]):
        compare_debt(player=player, cash=cash, real_message=True, real_schedule=True,
                     rating=0, records=({'kind': 3},), tick=True); count += 1
    print('PASS genuine queued publishers, scheduling/copy/reset/append graph and city phase4:', count, flush=True)
    print('Verified target SHA256:', target_sha, flush=True)


if __name__ == '__main__':
    verify_debt(); verify_ledgers(); verify_compositions()
