"""Whole sentiment/population-count family against original x86, offline.

Default fixtures control the labor query and toast publication. Actual house
counts, census removal, food percentage/classifiers and all four new C bodies
execute unless a named callback fixture explicitly substitutes a dependency.
"""
from verify_disease_updates import *

FIELDS=[0xea1880, 0xea5da8, 0xea5db0, 0xea3f78, 0xea3f7c, 0xea5eac,
        0xea5eb0, 0xea5ea8, 0xea5da4, 0xea5efb]
for owner in [0, 3]: WATCH.update(at+owner*0x49d8 for at in FIELDS)
for id in [1, 2, 3999, 4000]: WATCH.update(0x7999d8+id*264+off for off in [188, 194, 228])


def case(token=0x4e8840, args=(), rows=None, player=0, population=1000,
         tax=0, coverage=100, rate=10, wage=0, unemployment=10, toggle=1,
         mission=2, mode=0, sentiment=50, previous=50, reason=3, gate=0,
         difficulty=2, base=60, high_water=None, real_labor=False, prepare=None,
         steps=1, calendar=None):
    if rows is None: rows=({'id': 1, 'population': 600}, {'id': 2, 'population': 400})
    results=[]
    for c, u, state in machines:
        setup_disease(u, state, player=player, rows=rows, ids=tuple(row['id'] for row in rows) or (0,))
        if high_water is not None: put(u, 0x78d790, high_water)
        byte(u, 0xe38ea8, mission); put(u, 0xe38ebc, mode); put(u, 0xe39080, difficulty)
        for owner in [0, 3]:
            off=owner*0x49d8
            for at, value in [(0xea1894, population), (0xea1880, sentiment), (0xea5da8, previous),
                    (0xea5db0, reason), (0xea44a8, coverage), (0xea1878, rate), (0xea4478, wage),
                    (0xea447c, 0), (0xea446c, unemployment), (0xea5f9c, 511)]: put(u, at+off, value)
            byte(u, 0xea5efb+off, toggle); word(u, 0xea5ce8+off, gate)
        for i in range(26*11): put(u, 0xea1384+i*4, tax)
        for i in range(5): put(u, 0x5d199c+i*4, base)
        for level in range(20):
            put(u, 0x70cdf4+level*88, int(level>=2)); put(u, 0x70ce08+level*88, 5)
        for row in rows:
            p=0x7999d8+row['id']*264; word(u, p+18, row.get('level', 13))
            for off, value in [(188, row.get('hunger', 0)), (194, row.get('happiness', 50)), (127, row.get('foods', 1))]: byte(u, p+off, value)
            put(u, p+228, row.get('crime', 100))
            level=row.get('level', 13)
            if 'required' in row: put(u, 0x70cdf4+level*88, row['required'])
        calls=[]
        if not real_labor:
            hook(c, state, 0x4c38f0, 0, lambda uc, a: calls.append((0x4c38f0, a)) or 0)
        hook(c, state, 0x4e2970, 1, lambda uc, a: calls.append((0x4e2970, a)) or 0)
        if prepare: prepare(c, u, state, calls)
        if calendar is not None:
            put(u, 0x78d7c4, calendar); put(u, 0x78d7c0, 3); put(u, 0x78d7c8, 999)
            byte(u, 0xe38ec2, 0); byte(u, 0x7027b8, 0); byte(u, 0x7027be, 0); byte(u, 0x7027b9, 1)
            for endpoint, nargs in CALENDAR_ENDPOINTS.items():
                if endpoint in [0x4e8840, 0x4e8db0]: continue
                def observed(uc, a, endpoint=endpoint): calls.append((endpoint, a)); return 0
                hook(c, state, endpoint, nargs, observed)
        state['service_writes'].clear()
        for _ in range(steps): value=run(c, u, 0x452370 if calendar is not None else token, args)
        if token not in [0x458f80, 0x4e8ec0]: value=None
        results.append(((value, calls, list(state['service_writes'])), snapshot(u)))
    same(results, ('sentiment', hex(token), args, rows, player, population, tax, coverage, rate,
                   wage, unemployment, toggle, mission, mode, sentiment, previous, reason, gate, difficulty, base, high_water))
    return results[1]


def verify_house_counts():
    count=0
    for status, occupied in itertools.product([0, 1, 2, 3, 4, 5, 6, 255], [0, 1]):
        case(0x458f80, [3], rows=({'id': 1, 'state': status, 'occupied': occupied, 'population': -1},)); count+=1
    for level, player in itertools.product([-1, 0, 1, 2, 3, 4, 9, 10, 13, 14, 19], [0, 3]):
        case(0x458f80, [player], player=3-player, rows=({'id': 4000, 'level': level, 'population': 100},)); count+=1
    for people in [-32768, -1, 0, 1, 32767]:
        result=case(0x458f80, [0], rows=({'id': 1, 'population': people},))
        assert result[0][0]==people&0xffffffff; count+=1
    for limit in [-1, 0, 1, 3999, 4000]:
        case(0x458f80, [0], rows=({'id': 1}, {'id': 3999}, {'id': 4000}), high_water=limit); count+=1
    print('PASS exact count-state exclusions, explicit owner, signed levels/populations and cached high-water limit:', count, flush=True)


def verify_food_factor():
    count=0
    for percentage, wealth in itertools.product([9, 10, 25, 26, 39, 40, 56, 57], [0, 1, 2]):
        def prepare(c, u, state, calls):
            put(u, 0xea5eac, percentage*10); put(u, 0xea3f7c, wealth==2); put(u, 0xea5eb0, wealth==1)
        case(0x4e8ec0, prepare=prepare); count+=1
    for percentage in [-2147483648, -1, 0, 0x7fffffff]:
        def prepare(c, u, state, calls):
            def ratio(uc, args):
                calls.append((0x4cde60, args)); put(uc, 0xeaac28, 3)
                put(uc, 0xea3f7c+3*0x49d8, 1); return percentage
            hook(c, state, 0x4cde60, 2, ratio)
        case(0x4e8ec0, prepare=prepare); count+=1
    print('PASS real food percentages/wealth thresholds, signed full returns and post-query city reload:', count, flush=True)


def verify_house_adjustments():
    count=0
    for sentiment, crime in itertools.product([-2147483648, -1, 0, 79, 80, 81, 100, 0x7fffffff],
                                              [-2147483648, -1, 0, 1, 999, 1000, 0x7fffffff]):
        case(0x4e8db0, sentiment=sentiment, rows=({'id': 1, 'crime': crime},)); count+=1
    for status, occupied, people in [(0, 1, 1), (3, 1, 1), (1, 0, 1), (1, 1, 0), (1, 1, -1)]:
        case(0x4e8db0, rows=({'id': 3999, 'state': status, 'occupied': occupied, 'population': people}, {'id': 4000})); count+=1
    for category, permission in itertools.product([0, 1, 4], [0, 256, 257]):
        def prepare(c, u, state, calls):
            def classify(uc, args): calls.append((0x4c5120, args)); put(uc, 0xeaac28, 3); return category
            def allowed(uc, args): calls.append((0x4c52d0, args)); return permission
            hook(c, state, 0x4c5120, 1, classify); hook(c, state, 0x4c52d0, 2, allowed)
        case(0x4e8db0, gate=1, prepare=prepare); count+=1
    print('PASS complete house adjustment, cached signed/wrapped city delta, clamp, fixed scan and temple callback gates:', count, flush=True)


def verify_sentiment_factors():
    count=0
    # Preserve signed band arithmetic without supplying a malformed coverage
    # that makes the original dereference outside its mapped tax table domain.
    for coverage, rate in itertools.product([-100, -1, 0, 1, 9, 10, 99, 100, 101, 1000], [0, 10, 25]):
        def prepare(c, u, state, calls):
            for i in range(26*11): put(u, 0xea1384+i*4, i%9-4)
        case(coverage=coverage, rate=rate, prepare=prepare); count+=1
    for wage in [-2147483648, -17, -8, -3, -2, -1, 0, 1, 2, 4, 5, 7, 8, 0x7fffffff]: case(wage=wage); count+=1
    for unemployment in [-1, 4, 5, 10, 11, 17, 18, 25, 26, 0x7fffffff]: case(unemployment=unemployment); count+=1
    for population in [0, 199, 200, 299, 300, 301]: case(population=population); count+=1
    for foods, hunger, required in itertools.product([0, 1, 2, 3], [0, 1, 2, 3, 254, 255], [0, 1]):
        case(rows=({'id': 1, 'population': 600, 'foods': foods, 'hunger': hunger, 'required': required},
                   {'id': 2, 'population': 400, 'level': 1})); count+=1
    for happiness, tax in itertools.product([0, 1, 99, 100, 101, 127, 128, 255], [-129, -1, 0, 1, 129]):
        case(tax=tax, rows=({'id': 1, 'population': 1000, 'happiness': happiness},)); count+=1
    for mission, mode in itertools.product([0, 1, 2, 127, 128, 255], [0, 256]):
        case(mission=mission, mode=mode, previous=90); count+=1
    case(rows=()); count+=1
    case(rows=({'id': 3999}, {'id': 4000})); count+=1
    print('PASS tax bands, labor/jobless factors, small-city exceptions, hunger BYTE wrap, food averages, happiness clamp and tutorial messages:', count, flush=True)


def read(data, at): return struct.unpack_from('<I', data, at-0x560000)[0]


def verify_callbacks_and_causes():
    count=0
    def labor_switch(c, u, state, calls):
        def query(uc, args): calls.append((0x4c38f0, args)); put(uc, 0xeaac28, 3); return 2
        hook(c, state, 0x4c38f0, 0, query)
    case(prepare=labor_switch); count+=1
    def removal_switch(c, u, state, calls):
        def removal(uc, args):
            calls.append((0x4e9350, args)); put(uc, 0xeaac28, 3); word(uc, 0xea3e34, -12); return 0
        hook(c, state, 0x4e9350, 2, removal)
    case(prepare=removal_switch, population=1001); count+=1
    for percentage in [0, 10, 26, 40, 56, 57]:
        def food_switch(c, u, state, calls):
            def ratio(uc, args):
                calls.append((0x4cde60, args)); put(uc, 0xeaac28, 3)
                put(uc, 0xea3f7c+3*0x49d8, 1); byte(uc, 0x799a9a+264, 12); return percentage
            hook(c, state, 0x4cde60, 2, ratio)
        case(prepare=food_switch, rows=({'id': 1, 'population': 1000, 'level': 1},)); count+=1
    for category, permission in itertools.product([0, 4, 0x104], [0, 256, 257]):
        def temple_switch(c, u, state, calls):
            def classify(uc, args):
                calls.append((0x4c5120, args)); put(uc, 0xeaac28, 3); put(uc, 0xea5f9c+3*0x49d8, 1000); return category
            def allowed(uc, args): calls.append((0x4c52d0, args)); return permission
            hook(c, state, 0x4c5120, 1, classify); hook(c, state, 0x4c52d0, 2, allowed)
        case(gate=1, prepare=temple_switch); count+=1
    def toasts_switch(c, u, state, calls):
        n=[0]
        def toast(uc, args):
            calls.append((0x4e2970, args)); n[0]+=1
            if n[0]==1: put(uc, 0xeaac28, 3); put(uc, 0xea1880+3*0x49d8, 73)
            else: put(uc, 0xeaac28, 0); put(uc, 0xea1880, 23)
            return 0
        hook(c, state, 0x4e2970, 1, toast)
    result=case(tax=-5, previous=100, prepare=toasts_switch,
        rows=({'id': 1, 'population': 1000, 'happiness': 0, 'foods': 0},))
    assert [args for token, args in result[0][1] if token==0x4e2970] == [(0x69,), (0x77,)]
    assert read(result[1], 0xea5da8)==23; count+=1
    for options, expected in [({'unemployment': 26}, 2), ({'tax': -4}, 3), ({'wage': -20}, 4),
            ({'unemployment': 26, 'tax': -3, 'wage': -6,
              'rows': ({'id': 1, 'population': 1000, 'foods': 0, 'hunger': 2},)}, 1),
            ({'tax': 1, 'wage': 8, 'unemployment': 0, 'reason': 4}, 4)]:
        result=case(**options); assert read(result[1], 0xea5db0)==expected; count+=1
    result=case(rows=({'id': 1, 'population': 100, 'level': 1}, {'id': 2, 'population': 900, 'level': 14}))
    assert read(result[1], 0xea5db0)==5; count+=1
    case(steps=3); count+=1
    print('PASS callback-sensitive city/cohort/food/temple reloads, cached secondary toast, strict cause ties/retention and alternating food toggle:', count, flush=True)


def verify_real_integrations():
    count=0
    for gate, kind, variant in itertools.product([0, 1], [60, 61, 64], [0, 1]):
        def temple(c, u, state, calls):
            word(u, 0x7999e8+511*264, kind); byte(u, 0x799a80+511*264, variant)
        case(gate=gate, prepare=temple, real_labor=True); count+=1
    for day in [7, 15]:
        result=case(calendar=day)
        assert read(result[1], 0xea5da8)==read(result[1], 0xea1880); count+=1
    print('PASS actual labor/temple query graph and genuine half-month/monthly calendar sentiment/house adjustments:', count, flush=True)
    print('Verified target SHA256:', target_sha, flush=True)


if __name__=='__main__':
    verify_house_counts(); verify_food_factor(); verify_house_adjustments(); verify_sentiment_factors(); verify_callbacks_and_causes(); verify_real_integrations()
