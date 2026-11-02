"""Complete ratings panel/button/foreground/pillar versus original x86.

Observe presentation only; the real image-tag getter and monument delivery
query run. Explicit callback cases control font lookup or the delivery AL
to exercise native cache/reload contracts. No live-game claim is made.
"""
from verify_kingdom_advisor import *

ENDS = {**ENDS, 0x4d6250: 9, 0x4cce90: 8}
RESOURCE_IDS = [0, 7, 9, 12, 14, 16, 17, 18, 19, 22, 23, 24, 25, 27, 29]


def setup(u, state, phase=5, origin=(300, 200), player=3, hover=0,
          ratings=(40, 50, 60, 70), goals=(50, 50, 50, 50), flags=(1, 1, 1, 1),
          advice=0, reasons=(0, 0, 0, 0), prosperity=0, scenario=0,
          population_goal=1, housing_goal=1, missing=-1, limit=25500,
          minimum=256, buildings=()):
    reset(u, state)
    u.mem_write(0x7999d8, bytes(4002*264))
    for building, kind, active, enabled, occupied in buildings:
        at = 0x7999d8+building*264
        byte(u, at, active); byte(u, at+5, enabled)
        word(u, at+16, kind); word(u, at+28, occupied)
    for owner in [0, 3]:
        off = owner*0x49d8
        put(u, 0xea1894+off, 12345)
        for i, rating in enumerate(ratings): put(u, 0xea5b3c+off+i*4, rating)
        put(u, 0xea5ca0+off, advice); put(u, 0xea5b54+off, prosperity)
        for i, reason in enumerate(reasons): put(u, 0xea5ca4+off+i*4, reason)
    for at, value in [(0xf53f48, phase), (0xe38da8, origin[0]), (0xe38da4, origin[1]),
                      (0xf53f0c, origin[0]), (0xf53f08, origin[1]),
                      (0xeaac28, player), (0x7056c8, hover), (0xf5681c, 20000),
                      (0xf55bc4, 21000), (0xf56a04, 10000), (0x126517c, limit),
                      (0xf56f8c, 16416), (0xf56f9c, 16448), (0xf56fb4, 16480),
                      (0x784c58, population_goal), (0x784c5c, 50000),
                      (0x784c34, housing_goal), (0x784c38, minimum), (0xeadb28, 25600),
                      (0xe20778, 700)]: put(u, at, value)
    word(u, 0x784b60, scenario)
    for i, goal in enumerate(goals): put(u, 0x784c24+i*4, goal)
    for i, flag in enumerate(flags): byte(u, 0x784c3c+i, flag)
    for tag, image in [(0x27b, 17000), (0x27d, 18000), (0x280, 19000)]:
        put(u, 0xf55bb4+tag*8, image)
    for image in [0, *range(10000, 10012)]:
        word(u, 0x10d6a0e+image*64, -7 if image == 0 else 10+image%5)
    for i in range(36):
        put(u, 0x784dec+i*4, 100); put(u, 0x784e7c+i*4, 99 if i == missing else 100)


def observe(c, state, calls, mutation=None, font_callback=False, delivery_callback=False):
    for token, arity in ENDS.items():
        def draw(uc, args, token=token):
            calls.append((token, args))
            if token in [0x4d62f0, 0x4d6250, 0x4d6860, 0x4cce90]:
                put(uc, 0xe20778, get(uc, 0xe20778)+17)
            if mutation == 'geometry':
                for field, delta in [(0xf53f0c, 3), (0xf53f08, 2),
                                     (0xe38da8, 7), (0xe38da4, 5),
                                     (0xf56f8c, 16), (0xf56f9c, 16), (0xf56fb4, 16)]:
                    put(uc, field, get(uc, field)+delta)
            if mutation == 'player':
                put(uc, 0xeaac28, 0 if get(uc, 0xeaac28) else 3)
                off = get(uc, 0xeaac28)*0x49d8
                for i in range(4): put(uc, 0xea5b3c+off+i*4, 80+i)
            if mutation == 'goals':
                for i in range(4):
                    byte(uc, 0x784c3c+i, 0 if bytes(uc.mem_read(0x784c3c+i, 1))[0] else 1)
                    put(uc, 0x784c24+i*4, get(uc, 0x784c24+i*4)+3)
            if mutation == 'hover': put(uc, 0x7056c8, (get(uc, 0x7056c8)+1)%5)
            if mutation == 'pillar' and token == 0x53e590:
                put(uc, 0xf56a04, 10004); put(uc, 0x126517c, 10006)
            return 0x12345678
        hook(c, state, token, arity, draw, cleanup=12 if not c and token == 0x53e590 else 0)
    if font_callback:
        def font(uc, args):
            if not c: assert uc.reg_read(UC_X86_REG_ECX) == 0xf55bb0
            calls.append((0x4381a0, args))
            result = get(uc, 0xf55bb4+args[0]*8)
            for field, delta in [(0xf53f0c, 3), (0xf53f08, 2),
                                 (0xe38da8, 7), (0xe38da4, 5), (0xe20778, 11)]:
                put(uc, field, get(uc, field)+delta)
            for i in range(4): put(uc, 0x784c24+i*4, get(uc, 0x784c24+i*4)+1)
            return result
        hook(c, state, 0x4381a0, 1, font, cleanup=0 if c else 4)
    if delivery_callback:
        def delivery(uc, args):
            calls.append((0x4ea930, args)); put(uc, 0xf53f0c, get(uc, 0xf53f0c)+3)
            return 0x100  # AL0 despite a nonzero full EAX.
        hook(c, state, 0x4ea930, 0, delivery)


def compare(token=0x4f6af0, args=(), mutation=None, font_callback=False,
            delivery_callback=False, shell=False, **kw):
    results = []
    for c, u, state in machines:
        setup(u, state, **kw); calls = []
        observe(c, state, calls, mutation, font_callback, delivery_callback)
        if shell:
            put(u, 0x605a44, 5); put(u, 0xe38dac, 800); put(u, 0xe38db0, 600); byte(u, 0xe38dca, 1)
            for at, v in [(0xf55c0c, 20000), (0xf5691c, 22000), (0xf56914, 21000)]: put(u, at, v)
        value = run(c, u, 0x520ea0 if shell else token, args)
        results.append((value if token in [0x4ea930, 0x4ea280] else None, calls, snapshot(u)))
    assert results[0] == results[1], (hex(token), args, kw, mutation, font_callback, delivery_callback,
        next(((i, a, b) for i, (a, b) in enumerate(zip(results[0][1], results[1][1])) if a != b),
             ('call counts', len(results[0][1]), len(results[1][1]))),
        [(hex(0x560000+i), a, b) for i, (a, b) in enumerate(zip(results[0][-1], results[1][-1])) if a != b][:12])


def verify_panel():
    count = 0
    for phase, origin in itertools.product([0, 1, 2, 3, 4, 5, 6, 256, 0x80000000],
                                           [(300, 200), (-300, -200)]):
        compare(phase=phase, origin=origin); count += 1
    for goals, rating, flags in itertools.product([0, 50, 256], [-1, 0, 49, 50, 90, 91, 100, 101], [0, 1, 255]):
        compare(ratings=(rating,)*4, goals=(goals,)*4, flags=(flags,)*4); count += 1
    for scenario, population, housing in itertools.product([0, 1, 256], [0, 1, 256], [0, 1, 256]):
        compare(scenario=scenario, population_goal=population, housing_goal=housing); count += 1
    for missing in [-1, *RESOURCE_IDS, 1, 30, 35]:
        compare(missing=missing); count += 1
    for advice, reason in itertools.product([-1, 0, 1, 2, 3, 4, 5, 256], [-100, -20, -9, 0, 1, 20, 256]):
        compare(advice=advice, reasons=(reason,)*4, prosperity=reason); count += 1
    for advice, rating in itertools.product(range(1, 5), [90, 91]):
        compare(advice=advice, ratings=(rating,)*4); count += 1
    for mutation, advice in itertools.product(['geometry', 'player', 'goals', 'hover'], range(5)):
        compare(mutation=mutation, advice=advice); count += 1
    for advice, flags in itertools.product(range(5), [0, 1]):
        compare(advice=advice, flags=(flags,)*4, font_callback=True); count += 1
    compare(delivery_callback=True); count += 1
    print('PASS whole ratings panel, all goal/advice branches, live/cached fields and real queries:', count, flush=True)


def verify_foreground():
    count = 0
    for phase, hover in itertools.product([0, 1, 4, 5, 6, 256, 0x80000000], [0, 1, 4, 256]):
        compare(0x4f7770, phase=phase, hover=hover); count += 1
    for column, hover in itertools.product([-1, 0, 1, 2, 3, 4, 256], [0, 1, 4, 256]):
        compare(0x4f7720, [column], hover=hover); count += 1
    compare(0x4f7770, mutation='geometry'); compare(0x4f7770, mutation='hover'); count += 2
    print('PASS complete button/foreground, full-width hover and opening stages:', count, flush=True)


def verify_pillars():
    count = 0
    for amount, complete, limit in itertools.product([-2, -1, 0, 1, 2, 3, 99, 100, 101, 0x7fffffff],
                                                    [0, 1], [0, 10000, 10002, 25500]):
        compare(0x4f77e0, [350, 450, amount, complete], limit=limit); count += 1
    for amount in [0, 1, 3, 100]:
        compare(0x4f77e0, [-300, -200, amount, 1], mutation='pillar'); count += 1
    print('PASS whole pillar, signed height, image-limit fallback and cached middle/live cap:', count, flush=True)


def verify_compositions():
    count = 0
    for player, advice in itertools.product([0, 3], range(5)):
        compare(player=player, advice=advice, shell=True); count += 1
    for missing in [-1, *RESOURCE_IDS, 1, 30, 35]:
        compare(0x4ea930, missing=missing); count += 1
    print('PASS advisor routing and genuine monument-delivery eligibility:', count, flush=True)


def verify_pillar_pixels():
    from verify_workplace_pixels import archive, PIXELS, SCREEN
    # Global tag458 belongs to Pharaoh_General (first_tag270). Derive its
    # actual first image from the SG3 tag order, not from the reconstructed C.
    metadata = (ROOT/'build/debug-startup-game/Data/Pharaoh_General.sg3').read_bytes()
    tags = [struct.unpack_from('<h', metadata, 0x50+i*2)[0] for i in range(300)
            if struct.unpack_from('<h', metadata, 0x50+i*2)[0]-200 > 0]
    image = 11907+tags[458-270]-200-1
    records, pixels = archive('Pharaoh_General', 11907, range(image, image+3))
    for c, u, state in machines:
        u.mem_map(PIXELS, 0x1000000); u.mem_map(SCREEN, 0x200000)
    count = 0
    for width, height in [(640, 480), (800, 600), (1024, 768)]:
        cases = [(width//2-30, height-60, amount, complete)
                 for amount, complete in itertools.product([0, 1, 100], [0, 1])]
        cases += [(-12, 15, 100, 1), (width-25, height-5, 100, 1)]
        for px, py, amount, complete in cases:
            result = []
            for c, u, state in machines:
                reset(u, state); u.mem_write(PIXELS, pixels)
                u.mem_write(SCREEN, b'\x5a'*(width*height*2))
                for index, record in records.items(): u.mem_write(0x10d69f8+index*64, record)
                for at, value in [(0xf56a04, image), (0x126517c, 25500),
                                  (0xe92e18, SCREEN), (0xe38d94, width*2),
                                  (0xe38d9c, width), (0xe38d98, height),
                                  (0xe38dac, width), (0xe38db0, height),
                                  (0xd3b550, height), (0xd3b554, 0),
                                  (0xd3b558, width), (0xd3b55c, 0)]: put(u, at, value)
                byte(u, 0xe38dda, 0)
                run(c, u, 0x4f77e0, [px, py, amount, complete])
                result.append(bytes(u.mem_read(SCREEN, width*height*2)))
            assert result[0] == result[1], ('pillar pixels', width, height, px, py, amount, complete)
            assert result[1] != b'\x5a'*(width*height*2), 'No visible pillar pixels'
            count += 1
    print('PASS actual pillar SG3/raster pixels, three resolutions and edge clipping:', count, flush=True)


def verify_housing_count():
    count = 0
    for minimum, kind in itertools.product([-2147483648, -1, 0, 1, 255, 256, 32758, 2147483647],
                                           [-32768, 9, 10, 11, 265, 266, 32767]):
        records = [(i, kind, 1, 255, -1) for i in [0, 1, 6, 511, 3999, 4000, 4001]]
        compare(0x4f6ad0, minimum=minimum, buildings=records); count += 1
    for active, enabled, occupied in [(0, 1, 1), (1, 0, 1), (1, 1, 0), (1, 1, -1)]:
        compare(0x4ea280, [1], buildings=[(1, 11, active, enabled, occupied)]); count += 1
    print('PASS genuine housing scanner/publication,4000-record bound, signed fields and full minimum:', count, flush=True)


if __name__ == '__main__':
    verify_panel(); verify_foreground(); verify_pillars(); verify_compositions(); verify_pillar_pixels(); verify_housing_count()
    print('Verified target SHA256:', target_sha)
