"""Complete444C20: routes, cooldowns, creation order and city-tick linkage.

Native versus the current Release in Unicorn; no host game is launched.
Normal cases execute the real luxury/eligibility queries and observe actor
creation/message endpoints. Composition cases execute the real actor factory,
reservation queues, profiles, tile links and trader history, or the actual
message publisher in queued mode. Only CRT randomness is controlled there.
"""
from verify_trade_advisor import *


def country(ship=0, fees=(26,), **kw):
    return {'id': 1, 'ship': ship, 'fees': fees, **kw}


def prepare_updates(u, state, countries=(), player=0, cooldowns=(0, 0),
                    paused=0, ports=1, coords=(40, 60), notice=0, extra=0,
                    occupied=0, reserve=None, event_full=False):
    setup(u, state, resources=(19,), mode=1, player=player, countries=countries)
    for owner in (0, 3):
        off = owner*0x49d8
        # Nonzero adjacent bytes catch accidentally widened WORD writes.
        u.mem_write(0xea5ce8+off, b'\xa5'*16)
        word(u, 0xea5cf0+off, cooldowns[0]); word(u, 0xea5cf2+off, cooldowns[1])
        put(u, 0xea5f14+off, 0xdeadbeef); put(u, 0xea6024+off, paused)
        word(u, 0xea3fe8+off, 40+owner); word(u, 0xea3fea+off, 60+owner)
    for d in countries:
        p = 0x77de00+d['id']*106
        byte(u, p+88, d.get('ship', 0)); byte(u, p+4, d.get('category', 0))
        word(u, p+82, d.get('timer', 0)); word(u, p+84, d.get('blocked', 0))
        for i, value in enumerate(d.get('slots', (0, 0, 0))): word(u, p+90+i*2, value)
        for i, value in enumerate(d.get('fees', (26,)), 1):
            byte(u, p+6+i, 1)
            put(u, 0x77646c+(d.get('category', 0)*36+i)*4, value)
    for at, value in [(0x705c1c, ports), (0xe9519c, notice), (0x78d728, extra),
                      (0xbca2d8, 0), (0xd056fc, 0x5678), (0xbcd5f0, 0),
                      (0x77aefc, 0), (0xe951ec, 0), (0xe951f0, 42),
                      (0xe951e8, 0xffffffff), (0xe950e0, 0), (0xe950dc, 0),
                      (0xe951d8, 0), (0xf55b88, 0)]: put(u, at, value)
    word(u, 0x784c8c, coords[0]); word(u, 0x784c8e, coords[1])
    byte(u, 0xe38dda, 2); byte(u, 0xe38dc9, 0); byte(u, 0xe38dca, 0)
    word(u, 0x78d7bc, -1234); byte(u, 0x78d7c0, 9)
    u.mem_write(0xe95228, bytes(1000*48)); u.mem_write(0xe95080, bytes(20*4))
    u.mem_write(0xe209b8+316*80, bytes(80))
    if event_full:
        for i in range(1000): word(u, 0xe95232+i*48, 1)
    u.mem_write(0xbcd5f8, bytes(2000*388)); u.mem_write(0xb350dc, bytes(51984*2))
    u.mem_write(0xeaaf68, bytes(0xeadb28-0xeaaf68))
    u.mem_write(0x9784d8, bytes(0x978d74-0x9784d8))
    u.mem_write(0x77af00, b'\xa5'*(100*88))
    # The controlled factory leaves these bytes untouched; native caller
    # stores must preserve every unrelated byte in each actor record.
    for actor in (0, 501, 502, 503):
        u.mem_write(0xbcd5f8+actor*388, b'\xa5'*388)
        byte(u, 0xbcd606+actor*388, 0)
    for i in range(1, occupied+1): byte(u, 0xbcd606+i*388, 1)
    if reserve == 'ring':
        q = 0xeaaf68
        for at, value in [(q, 1), (q+4, 0), (q+8, 1), (q+12, 2), (q+20, occupied+1)]: put(u, at, value)
    if reserve == 'pool':
        byte(u, 0x9784d8, 1); put(u, 0x9784dc, occupied+1)


def actor_observation(u, countries):
    return (tuple(bytes(u.mem_read(0x77de00+d['id']*106+82, 14)) for d in countries),
            tuple(bytes(u.mem_read(0xbcd5f8+a*388, 388)) for a in (0, 501, 502, 503)))


def compare_updates(real_create=False, real_event=False, query=None,
                    mutation=None, factory_returns=(501, 502, 503), tick=False, **kw):
    results = []
    countries = kw.get('countries', ())
    for c, u, state in machines:
        prepare_updates(u, state, **kw); calls = []; created = []; rng = [1]
        if not real_create:
            def create(uc, args):
                calls.append((0x4b4800, args, actor_observation(uc, countries)))
                result = factory_returns[min(len(created), len(factory_returns)-1)]
                created.append(result)
                if mutation == 'create':
                    owner = 3 if len(created) == 1 else 0
                    put(uc, 0xeaac28, owner)
                    word(uc, 0xea3fe8+owner*0x49d8, -7-len(created))
                    word(uc, 0xea3fea+owner*0x49d8, 11+len(created))
                return result
            hook(c, state, 0x4b4800, 5, create)
        else:
            def random(uc, args):
                rng[0] = (rng[0]*214013+2531011)&0xffffffff
                return (rng[0] >> 16)&32767
            hook(c, state, 0x562348, 0, random)
        if not real_event:
            def event(uc, args):
                calls.append((0x4e0cf0, args, get(uc, 0xe950dc)))
                if mutation == 'message':
                    put(uc, 0xe9519c, 999); put(uc, 0xeaac28, 3)
                return 0
            hook(c, state, 0x4e0cf0, 5, event)
        if query == 'luxury':
            def luxury(uc, args):
                calls.append((0x559fc0, bytes(uc.mem_read(args[0], 64))))
                put(uc, 0xeaac28, 3); put(uc, 0xea5f14, 700)
                word(uc, 0xea5cf0, 2); word(uc, 0xea5cf2, 3)
                return 5
            hook(c, state, 0x559fc0, 1, luxury)
        if isinstance(query, int):
            def eligible(uc, args):
                calls.append((0x447790, args))
                if mutation == 'fee':
                    p = 0x77de00+args[0]*106
                    byte(uc, p+4, 3); put(uc, 0x77646c+(3*36+args[1])*4, 1)
                if mutation == 'ship': byte(uc, 0x77de00+args[0]*106+88, 1)
                return query
            hook(c, state, 0x447790, 2, eligible)
        if tick:
            for at in [0x78d79c, 0x78d7a8, 0x78d7a0, 0x78d7b0, 0x78d7a4]: put(u, at, 0)
            byte(u, 0xeaaf5c, 0); put(u, 0x78d7cc, 32)
            def following(uc, args):
                calls.append((0x46f200, actor_observation(uc, countries))); return 0
            hook(c, state, 0x46f200, 0, following)
        run(c, u, 0x451c50 if tick else 0x444c20)
        results.append((calls, rng[0], snapshot(u)))
    assert results[0] == results[1], (kw, real_create, real_event, query, mutation, tick,
        next(((i, a[:2], b[:2]) for i, (a, b) in enumerate(zip(results[0][0], results[1][0])) if a != b),
             ('call counts', len(results[0][0]), len(results[1][0]))),
        [(hex(0x560000+i), a, b) for i, (a, b) in enumerate(zip(results[0][-1], results[1][-1])) if a != b][:12])
    return results[1][0]


def verify_routes():
    count = 0
    for extra, ship_delay, caravan_delay in itertools.product(
            [-1, 0, 1, 256], [-32768, -1, 0, 1, 2, 32767], [0, 2]):
        compare_updates(extra=extra, cooldowns=(ship_delay, caravan_delay)); count += 1
    for ship, fees in itertools.product([0, 1],
            [(), (-1,), (0,), (1,), (15,), (16,), (25,), (26,), (15, 26), (1, 1, 26)]):
        compare_updates(countries=(country(ship, fees),)); count += 1
    for ship, fee, mask in itertools.product([0, 1], [1, 16, 26], range(8)):
        slots = tuple(600+i if mask & (1 << i) else 0 for i in range(3))
        compare_updates(countries=(country(ship, (fee,), slots=slots),)); count += 1
    for ship, timer, blocked in itertools.product([0, 1], [-32768, -1, 0, 1, 2, 32767], [0, 1]):
        compare_updates(countries=(country(ship, timer=timer, blocked=blocked),)); count += 1
    for ship, paused, delay in itertools.product([0, 1], [-1, 0, 1, 256], [0, 1, 2]):
        compare_updates(countries=(country(ship),), paused=paused, cooldowns=(delay, delay)); count += 1
    print('PASS counters, signed WORD cooldowns, weighted resource demand, slots and timer gates:', count, flush=True)


def verify_order():
    count = 0
    for ports, notice in itertools.product([-1, 0, 1, 256], [-1, 0, 1, 256]):
        compare_updates(countries=(country(1), country(1, id=60)), ports=ports, notice=notice); count += 1
    for coords in [(-1, -1), (-1, 20), (20, -1), (20, 20), (-32768, 32767)]:
        compare_updates(countries=(country(1),), coords=coords); count += 1
    for ship, player in itertools.product([0, 1, 2, 255], [0, 3]):
        compare_updates(countries=(country(ship, id=60),), player=player); count += 1
    for data in [
            (country(0, id=0), country(0, id=60)),
            (country(0, active=0), country(1, id=60)),
            (country(0, enabled=0), country(1, id=60)),
            (country(0), country(1, id=2)),
            (country(0, timer=1), country(1, id=60)),
            (country(0, blocked=1), country(1, id=60))]:
        compare_updates(countries=data); count += 1
    for ship, result in itertools.product([0, 1], [0, 501]):
        compare_updates(countries=(country(ship),), factory_returns=(result,)); count += 1
    for query, mutation in [('luxury', None), (0x100, None), (0x101, None),
                            (0x101, 'fee'), (0x101, 'ship')]:
        compare_updates(countries=(country(),), query=query, mutation=mutation); count += 1
    compare_updates(countries=(country(),), mutation='create'); count += 1
    compare_updates(countries=(country(1), country(0, id=60)), ports=0, mutation='message'); count += 1
    # Both flags set: false AL forces the second query, true AL short-circuits.
    for answer in [0x100, 0x101]:
        compare_updates(countries=(country(first_flags=(1,)),), query=answer); count += 1
    types = luxury_types()
    different = next(v for v in types if v[1] != types[0][1])
    for extra in [-1, 0, 1]:
        compare_updates(countries=(country(subtype=types[0][0], first_flags=(19,), timer=1),
                                   country(id=60, subtype=different[0], first_flags=(19,))), extra=extra); count += 1
    print('PASS port notices, route bounds, first-spawn exit, AL and callback-visible ordering:', count, flush=True)


def verify_compositions():
    count = 0
    for ship, occupied in itertools.product([0, 1], [0, 510, 1998, 1999]):
        compare_updates(countries=(country(ship),), real_create=True, occupied=occupied); count += 1
    for ship, reserve in itertools.product([0, 1], ['ring', 'pool']):
        compare_updates(countries=(country(ship),), real_create=True, occupied=510, reserve=reserve); count += 1
    for notice, rows, full in [(0, 1, False), (0, 3, False), (1, 3, False),
                               (384, 3, False), (0, 1, True)]:
        compare_updates(countries=tuple(country(1, id=i+1) for i in range(rows)),
                        ports=0, notice=notice, real_event=True, event_full=full); count += 1
    for ship, player in itertools.product([0, 1], [0, 3]):
        compare_updates(countries=(country(ship),), real_create=True, player=player, tick=True); count += 1
    compare_updates(countries=(country(1),), ports=0, real_event=True, tick=True); count += 1
    print('PASS genuine factories/reservations/profiles/tile links/history, queued events and phase32:', count, flush=True)
    print('Verified target SHA256:', target_sha, flush=True)


def compare_export(token=0x443330, args=(), buildings=(), towns=(), population=1000,
                   player=0, planned=(0, 0, 0), difference=0, mutation=None,
                   high_water=None, query_result=0):
    results = []
    for c, u, state in machines:
        prepare_updates(u, state, player=player)
        u.mem_write(0x7999d8, bytes(4001*264))
        put(u, 0x78d790, len(buildings) if high_water is None else high_water)
        for i, d in enumerate(buildings, 1):
            p = 0x7999d8+d.get('id', i)*264
            byte(u, p, d.get('state', 1)); word(u, p+16, d.get('kind', 32))
            word(u, p+64, d.get('workers', 1)); word(u, p+18, d.get('model', 13))
            word(u, p+206, d.get('secondary', -2))
        for owner in (0, 3):
            off = owner*0x49d8
            put(u, 0xea1894+off, population+owner*1000)
            for i in range(3): put(u, 0xea5fac+off+i*4, 0)
        put(u, 0x785d70, len(towns))
        for i, d in enumerate(towns):
            p = 0x785d74+i*124
            byte(u, p+2, d.get('active', 1)); byte(u, p+50, d.get('enabled', 1))
            word(u, p+4, d.get('resource', 9)); word(u, p+12, d.get('amount', 123))
            word(u, p+48, d.get('mode', 0))
        for i, kind in enumerate(planned): word(u, 0x784de0+i*2, kind)
        for i in range(35):
            put(u, 0x784dec+i*4, difference+i); put(u, 0x784e7c+i*4, i)
            put(u, 0x775814+i*4, 0xa5a5a5a5)
        for band, index in itertools.product(range(4), range(6)):
            put(u, 0x7758a0+(band*6+index)*4, band*1000+index*99)
        calls = []
        if mutation == 'clamp':
            def clamp(uc, values):
                calls.append(values)
                put(uc, 0xeaac28, 3 if values[0] & 1 else 0)
                put(uc, 0x784dec+values[0]*4, 10*values[0])
                return values[1]
            hook(c, state, 0x559c60, 2, clamp)
        if mutation in ('count', 'planned'):
            def producer(uc, values):
                calls.append(values); return query_result
            hook(c, state, 0x443080 if mutation == 'count' else 0x4f57b0, 1, producer)
        if token == 0x451c50:
            for at in [0x78d79c, 0x78d7a8, 0x78d7a0, 0x78d7b0, 0x78d7a4]: put(u, at, 0)
            byte(u, 0xeaaf5c, 0); put(u, 0x78d7cc, 2)
        value = run(c, u, token, args)
        results.append((value if token not in (0x443330, 0x451c50) else None, calls, snapshot(u)))
    assert results[0] == results[1], (hex(token), args, buildings, towns, population,
        player, planned, difference, mutation, high_water, query_result,
        [(hex(0x560000+i), a, b) for i, (a, b) in enumerate(zip(results[0][-1], results[1][-1])) if a != b][:12])
    if mutation == 'clamp': assert [a[0] for a in results[1][1]] == list(range(1, 36))


def verify_export_queries():
    # Derive the accepted producer kinds and resource-model mapping from the
    # original jump tables, independently of either compiled implementation.
    def raw(a, n): return bytes(images[0].data[a-images[0].base:a-images[0].base+n])
    targets = struct.unpack('<2I', raw(0x44325c, 8))
    kinds = {i+32 for i, b in enumerate(raw(0x443264, 202)) if targets[b] == 0x4431ae}
    kinds.update(range(110, 115))
    count = 0
    for kind in sorted(kinds | {-32768, -1, 0, 31, 33, 109, 115, 234, 32767}):
        compare_export(0x443080, [9], buildings=({'kind': kind},)); count += 1
    model_targets = struct.unpack('<33I', raw(0x4431d8, 132))
    for resource in [0, 1, *range(2, 35), 35, 36, 0xffffffff]:
        target = model_targets[resource-2] if 2 <= resource <= 34 else 0
        model = struct.unpack('<i', raw(target+1, 4))[0] if target and raw(target, 1) == b'\xbe' else -1
        compare_export(0x443080, [resource], buildings=({'model': model}, {'model': -2, 'secondary': model})); count += 1
    for high in [-1, 0, 1, 2, 4000]:
        compare_export(0x443080, [9], buildings=({'id': max(high, 1)},), high_water=high); count += 1
    for state, workers in itertools.product([0, 1, 2, 255], [-32768, 0, 1, 256]):
        compare_export(0x443080, [9], buildings=({'state': state, 'workers': workers},)); count += 1
    for resource, value in itertools.product([0, 9, 10, 12, 24, 25, 26, 28, 30, 36, 0xffffffff],
            [-2147483648, -1, 0, 10000, 10001, 15000, 15001, 30000, 30001, 2147483647]):
        compare_export(0x559c60, [resource, value]); count += 1
    print('PASS whole producer query, PE-derived switches, signed fields, last building and stock clamps:', count, flush=True)


def verify_export_update():
    count = 0
    for population, player in itertools.product([-1, 0, 999, 1000, 2499, 2500, 4999, 5000, 256000], [0, 3]):
        compare_export(population=population, player=player); count += 1
    for resource, mode in itertools.product([-1, 0, 1, 8, 9, 12, 24, 30, 35, 36], [-1, 0, 1, 2, 256]):
        compare_export(towns=({'resource': resource, 'mode': mode},)); count += 1
    for amount, difference in [(-32768, -1), (-1, 0), (0, 2147483647), (32767, 2147483648)]:
        compare_export(towns=({'amount': amount}, {'amount': amount}), difference=difference); count += 1
    for planned in [(1, 22, 24), (2, 23, 25), (3, 4, 5), (6, 7, 8), (9, 10, 11),
                    (12, 13, 14), (15, 16, 17), (18, 19, 20), (26, 27, -1)]:
        compare_export(planned=planned, buildings=({'model': 13}, {'model': 3, 'secondary': 18})); count += 1
    compare_export(mutation='clamp', towns=({'resource': 9}, {'resource': 24})); count += 1
    for mutation, result in itertools.product(['count', 'planned'], [-2147483648, -1, 0, 2147483647]):
        compare_export(mutation=mutation, query_result=result); count += 1
    for player, planned in itertools.product([0, 3], [(0, 0, 0), (1, 22, 24)]):
        compare_export(token=0x451c50, player=player, planned=planned,
                       towns=({'resource': 9}, {'resource': 24, 'mode': 1}),
                       buildings=({'model': 13},)); count += 1
    print('PASS complete export reserve update, real requests/producers/monuments/clamps and phase2:', count, flush=True)
    print('Verified target SHA256:', target_sha, flush=True)


if __name__ == '__main__':
    verify_routes(); verify_order(); verify_compositions()
    verify_export_queries(); verify_export_update()
