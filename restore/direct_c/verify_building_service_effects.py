"""Health, collapse/fire prevention and complete bazaar distribution offline.

Compare original x86 with Release, all mapped state, ordered scan/maximum
writes and explicit root-query callbacks. Ordinary cases execute the actual
root resolver; composed walker cases execute all new effects without stubs.
"""
from verify_house_services import *

WATCH.update({0x9ab9e0, 0x9ab9e4})
HEALTH = [0x482dd0, 0x482ef0]
RISK = [0x482c10, 0x483000]


def compare(token, args, prepare, after=None, label=None):
    results = []
    for c, u, state in machines:
        calls = []; prepare(c, u, state, calls); state['service_writes'].clear()
        value = run(c, u, token, args)
        if after: after(u, value)
        results.append(((None if token == 0x482ef0 else value,
                         list(state['service_writes']), calls), snapshot(u)))
    same(results, label or (hex(token), args))
    return results[1]


def health(token, value=350, amount=10, **kw):
    def prepare(c, u, state, calls):
        prepare_effect(u, state, **kw)
        for id in [-1, 0, 1, 2, 3, 4, 4000]:
            for at in [0x799ac0, 0x799ad4]: put(u, at+id*264, value)
    return compare(token, [*kw.get('pos', (10, 10)), amount], prepare,
                   label=('health', hex(token), value, amount, kw))


def verify_health():
    count = 0
    for token, value, amount in itertools.product(HEALTH,
            [-2147483648, -1, 0, 350, 1000, 0x7fffffff],
            [-2147483648, -1, 0, 1, 10, 1000, 0x7fffffff]):
        health(token, value=value, amount=amount); count += 1
    for token, kind, status in itertools.product(HEALTH, [-1, 9, 10, 29, 30], [0, 1, 255]):
        health(token, rows=({'id': -1, 'type': kind, 'state': status, 'occupied': 0, 'population': 0},),
               ids=(-1,)); count += 1
    for token, pos in itertools.product(HEALTH, [(0, 0), (31, 31), (40, 10), (10, 40), (-3, 10), (10, -3)]):
        health(token, pos=pos); count += 1
    print('PASS both health fields, signed/wrapped subtraction, exact state/type gates and publication order:', count, flush=True)


def prepare_risk(u, state, value=2500, step=17, **kw):
    prepare_effect(u, state, **kw)
    for id in [-1, 0, 1, 2, 3, 4, 4000]:
        for at in [0x799a1e, 0x799a20]: word(u, at+id*264, value)
        word(u, 0x799a10+id*264, 0)
        byte(u, 0x7999dd+id*264, 1); word(u, 0x7999f4+id*264, 100)
    word(u, 0x799a10+264, 2); word(u, 0x799a10+2*264, 4000)
    put(u, 0x706b70, step); put(u, 0x7078e0, step)
    for yy in range(32):
        for xx in range(32):
            tile=yy*228+xx
            put(u, 0xaf598c+tile*4, 0x4000); word(u, 0xa9cc1c+tile*2, value)


def risk(token, root=None, mutation=None, **kw):
    def prepare(c, u, state, calls):
        prepare_risk(u, state, **kw)
        if root is not None:
            def resolve(uc, args):
                calls.append((args, get(uc, 0xbc9768), get(uc, 0xd3b4e4), get(uc, 0xd3b4e0)))
                if mutation == 'cursor' and len(calls) == 1:
                    for at in [0xbc9768, 0xd3b4e4, 0xd3b4e0]: put(uc, at, get(uc, at)+1)
                if mutation == 'settings':
                    put(uc, 0x706b70, 7+len(calls)); put(uc, 0x7078e0, 11+len(calls))
                    put(uc, 0x9ab9e0, -len(calls)); put(uc, 0x9ab9e4, -len(calls))
                    put(uc, 0xbca2d0, 1); put(uc, 0xbca2d4, 1)
                return root
            hook(c, state, 0x475b50, 1, resolve)
    return compare(token, kw.get('pos', (10, 10)), prepare,
                   label=('risk', hex(token), root, mutation, kw))


def verify_risks():
    count = 0
    for token, value, step in itertools.product(RISK, [-32768, -1, 0, 999, 1000, 2500, 32767],
            [-1, 0, 17, 32768, 65536]):
        risk(token, value=value, step=step); count += 1
    for token, root, mutation in itertools.product(RISK, [-1, 0, 1, 4000, 0x80000001], [None, 'cursor', 'settings']):
        risk(token, root=root, mutation=mutation); count += 1
    for token, pos in itertools.product(RISK, [(0, 0), (31, 31), (40, 10), (10, 40), (-3, 10), (10, -3)]):
        risk(token, pos=pos); count += 1
    print('PASS actual root chains, terrain risk, WORD wrap, maximum1000, full root results and live-cursor reloads:', count, flush=True)


def prepare_market(u, state, source=511, population=10, level=13, size=1,
                   food_types=2, foods=(1, 2), stock=100, stored=0, goods=0,
                   rows=None, ids=None, pos=(10, 10), **kw):
    if rows is None: rows=({'id': 1, 'state': 3, 'population': population, 'kind': level},)
    if ids is None: ids=(1,)
    prepare_effect(u, state, rows=rows, ids=ids, pos=pos, **kw)
    for i, row in enumerate(rows, 1):
        p=0x7999d8+row.get('id', i)*264; byte(u, p+3, size)
        for slot in range(-1, 12): word(u, p+84+slot*2, stored)
    for slot in range(-1, 12):
        word(u, 0x799a2e+source*264+slot*2, stock[slot+1] if isinstance(stock, tuple) else stock)
    for at in [0x799a76, 0x799a78, 0x799a7a, 0x799a7c]: word(u, at+source*264, 0xa5a5)
    put(u, 0xf53dd8, len(foods))
    for i, resource in enumerate(foods): put(u, 0xf53ddc+i*4, resource)
    for model in range(21):
        put(u, 0x70cdf4+model*88, food_types)
        for at in [0x70ce04, 0x70cdf8, 0x70cdfc, 0x70ce00]: put(u, at+model*88, goods)


def market(**kw):
    def prepare(c, u, state, calls): prepare_market(u, state, **kw)
    return compare(0x483910, [kw.get('source', 511), *kw.get('pos', (10, 10))], prepare,
                   label=('market', kw))


def verify_market():
    count = 0
    for resource, stock in itertools.product([*range(20), 23, 256, 0xffffffff], [-1, 0, 1, 9, 19, 20, 100]):
        market(foods=(resource,), stock=stock); count += 1
    for food_types, population in itertools.product([-1, 0, 1, 2, 3, 4, 256], [1, 10, 100, 32767]):
        market(food_types=food_types, population=population, foods=(15, 13, 1, 2, 7)); count += 1
    for level, size in itertools.product(range(20), [0, 1, 2, 255]):
        market(level=level, size=size, goods=3); count += 1
    for goods, size in itertools.product([-2147483648, -1, 0, 1, 32768, 0x40000000, 0x7fffffff], [1, 2, 255]):
        market(goods=goods, size=size, stored=-1); count += 1
    for source, foods in itertools.product([-1, 0, 1, 4000], [(), (0, 1), (1, 1, 2), (15, 13, 1)]):
        market(source=source, foods=foods, goods=2); count += 1
    for status, occupied, population in itertools.product([0, 1, 3], [0, 1], [-1, 0, 1]):
        market(rows=({'id': 4000, 'state': status, 'occupied': occupied, 'population': population},), ids=(4000,)); count += 1
    for pos in [(0, 0), (31, 31), (40, 10), (10, 40), (-3, 10), (10, -3)]:
        market(pos=pos); count += 1
    for resource, final_house, final_source in [(1, 60, 40), (2, 59, 41)]:
        result=market(foods=(resource,)); data=result[1]; slot=resource-1
        assert struct.unpack_from('<h', data, 0x799a2c+264+slot*2-0x560000)[0] == final_house
        assert struct.unpack_from('<h', data, 0x799a2e+511*264+slot*2-0x560000)[0] == final_source
        count += 1
    print('PASS complete bazaar resource tables, variety budget, half portions, stock WORDs, models, timers and source aliasing:', count, flush=True)


def verify_walker_compositions():
    count = 0
    for kind, amount in itertools.product([8, 10, 26, 32, 33], [0, 17, 65536]):
        results=[]
        for c, u, state in machines:
            if kind == 26:
                prepare_market(u, state, goods=2, foods=(1, 2, 7))
            else:
                prepare_risk(u, state, step=amount)
                for at in [0x799ac0, 0x799ad4]: put(u, at+264, 350)
            p=a(255);byte(u, p+10, kind);word(u, p+20, 10);word(u, p+22, 10)
            word(u, p+90, 511);put(u, p+32, 2290);word(u, 0x7999f0+511*264, 290)
            for at in [0x7065f8, 0x707ef8]: put(u, at, amount)
            put(u, 0x5dbf70+kind*4, 2);state['service_writes'].clear()
            assert run(c, u, 0x4baa10, [255]) == 0
            results.append((list(state['service_writes']), snapshot(u)))
        same(results, ('actual walker composition', kind, amount)); count += 1
    print('PASS actual walker service through all five new providers, real root queries, house effects and packed visits:', count, flush=True)
    print('Verified target SHA256:', target_sha, flush=True)


if __name__ == '__main__':
    verify_health(); verify_risks(); verify_market(); verify_walker_compositions()
