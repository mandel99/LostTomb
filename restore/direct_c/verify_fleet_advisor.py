"""Fleet advisor and genuine ship/dock/crew/damage queries, offline.

Only panel/text/sprite outputs are controlled. Geometry and its bounded water
region, ordinal selection and event/request algorithms execute on both sides.
"""
from verify_kingdom_advisor import *

ENDS = {**ENDS, 0x4d6250: 9, 0x4cd1a0: 5}


def lake(u):
    put(u, 0xbca2d8, 43+51*228)
    for at, value in [(0xbca2d0, 100), (0xbca2d4, 100), (0xbca2e8, 43),
                      (0xbca2ec, 51), (0xbca2f0, 142), (0xbca2f4, 150)]:
        put(u, at, value)
    u.mem_write(0xaf598c, bytes(228*228*4))
    u.mem_write(0xed4140, b'\xff'*(228*228))
    for y in range(45, 56):
        at = get(u, 0xbca2d8)+45+y*228
        u.mem_write(0xed4140+at, bytes(11))
        u.mem_write(0xaf598c+at*4, struct.pack('<I', 4)*11)


def setup(u, state, ships=(1, 511, 1999), phase=6, origin=(300, 200),
          player=3, records=(), selection=2, mission=-1, assigned=1,
          crew=0, damage=50, empire=0, city_state=2, extra=0, reported=None, docked=False):
    prepare(u, state, phase=phase, origin=origin, player=player, records=records,
            city_state=city_state)
    u.mem_write(0xbcd5f8, bytes(2000*388))
    u.mem_write(0x7999d8, bytes(4000*264))
    lake(u)
    for at, value in [(0x7056c8, selection), (0xf56694, 21000),
                      (0xf56834, 23000), (0xf5643c, 25000),
                      (0xf5690c, 26000), (0xf56f94, 16512),
                      (0xf53dd0, 777), (0xf53f0c, origin[0]),
                      (0xf53f08, origin[1]), (0xd3b4e4, 0x12345),
                      (0xd3b4e0, -0x12345)]:
        put(u, at, value)
    for owner in [0, 3]:
        off = owner*0x49d8
        put(u, 0xea5eb4+off, 256 if empire == 2 else 0)
        put(u, 0xea5ea4+off, 256 if empire == 1 else 0)
        byte(u, 0xea5ed8+off, len(ships) if reported is None else reported)
        byte(u, 0xea5ed4+off, extra)
    for actor in ships:
        off = actor*388
        for at, value in [(0xbcd602, 78), (0xbcd606, 1), (0xbcd607, player),
                          (0xbcd5fa, assigned), (0xbcd74c, crew)]:
            byte(u, at+off, value)
        word(u, 0xbcd72a+off, mission)
        word(u, 0xbcd652+off, 511)
        word(u, 0xbcd61c+off, 50 if docked else 52)
        word(u, 0xbcd61e+off, 49 if docked else 50)
        put(u, 0xbcd720+off, damage)
        put(u, 0xbcd724+off, 100)
    byte(u, 0x7999db+511*264, 2)
    word(u, 0x7999de+511*264, 50)
    word(u, 0x7999e0+511*264, 50)


def outputs(c, state, calls, mutate=False):
    for token, count in ENDS.items():
        def draw(uc, args, token=token):
            calls.append((token, args))
            if token in [0x4d62f0, 0x4d6250, 0x4ccc30]:
                put(uc, 0xe20778, get(uc, 0xe20778)+17)
            if mutate and token in [0x4d62f0, 0x4d63d0]:
                put(uc, 0xf53f0c, get(uc, 0xf53f0c)+3)
                put(uc, 0xf53f08, get(uc, 0xf53f08)+2)
                put(uc, 0xeaac28, 0)
                put(uc, 0x7056c8, get(uc, 0x7056c8)+1)
            return 0
        hook(c, state, token, count, draw,
             cleanup=12 if not c and token == 0x53e590 else 0)


def compare(token=0x522e10, mutate=False, buttons=False, shell=False, **kwargs):
    states = []
    for c, u, state in machines:
        setup(u, state, **kwargs)
        calls = []
        outputs(c, state, calls, mutate)
        if buttons:
            state['hooks'].pop(address(c, 0x4cd1a0))
            hook(c, state, 0x536c30, 6,
                 lambda uc, args: calls.append((0x536c30, args)) or 0)
        if shell:
            put(u, 0x605a44, 3)
            put(u, 0xe38dac, 600)
            put(u, 0xe38db0, 800)
            byte(u, 0xe38dca, 1)
            for at, value in [(0xf55c0c, 20000), (0xf5691c, 22000), (0xf56914, 21000)]:
                put(u, at, value)
        run(c, u, 0x520ea0 if shell else token)
        states.append((calls, snapshot(u)))
    assert states[0] == states[1], (hex(token), kwargs, mutate, buttons, shell,
        states[0][0][:18], states[1][0][:18],
        [(hex(0x560000+i), a, b) for i, (a, b) in enumerate(zip(states[0][-1], states[1][-1]))
         if a != b][:12])
    return states[1][0]


def verify_ordinal():
    count = 0
    for ships, player, ordinal in itertools.product(
            [(), (1,), (1999,), (1, 511, 1999)], [0, 3, 255, 256], [-1, 0, 1, 2, 3, 4, 256]):
        states = []
        for c, u, state in machines:
            reset(u, state)
            u.mem_write(0xbcd5f8, bytes(2000*388))
            put(u, 0xeaac28, player)
            for actor in ships:
                byte(u, 0xbcd602+actor*388, 78)
                byte(u, 0xbcd606+actor*388, 255)
                byte(u, 0xbcd607+actor*388, player)
            result = run(c, u, 0x4bc620, [ordinal])
            states.append((result, snapshot(u)))
        assert states[0] == states[1], ('ordinal', ships, player, ordinal)
        assert result == (ships[ordinal-1] if player < 256 and 0 < ordinal <= len(ships) else 0)
        count += 1
    print('PASS genuine sparse/full-owner ship ordinal selection and scan limits:', count, flush=True)


def verify_dock():
    count = 0
    for actor, building, layout, point in itertools.product(
            [1, 511, 1999], [0, 511, 3999], [0, 2, 5],
            [(52, 50), (50, 49), (0x12345, -0x12345)]):
        states = []
        for c, u, state in machines:
            setup(u, state, ships=(actor,))
            word(u, 0xbcd652+actor*388, building)
            byte(u, 0x7999db+building*264, layout)
            word(u, 0x7999de+building*264, 50)
            word(u, 0x7999e0+building*264, 50)
            word(u, 0xbcd61c+actor*388, point[0])
            word(u, 0xbcd61e+actor*388, point[1])
            result = run(c, u, 0x522d80, [actor]) & 255
            assert get(u, 0xd3b4e4) == 0x12345 and get(u, 0xd3b4e0) == (-0x12345 & 0xffffffff)
            states.append((result, snapshot(u)))
        assert states[0] == states[1], ('dock', actor, building, layout, point)
        assert result == int(layout in [2, 5] and point == (50, 49))
        count += 1
    # Explicit successful output and callback-mutated actor coordinates.
    for actor, callback_point in itertools.product([1, 511, 1999], [(41, 52), (0, 0)]):
        states = []
        for c, u, state in machines:
            setup(u, state, ships=(actor,))
            word(u, 0x7999de+511*264, 40)
            calls = []
            def geometry(uc, args):
                calls.append(args)
                put(uc, 0xd3b4e4, args[0]+1)
                put(uc, 0xd3b4e0, args[1]+2)
                word(uc, 0xbcd61c+actor*388, callback_point[0])
                word(uc, 0xbcd61e+actor*388, callback_point[1])
                return 1
            hook(c, state, 0x4806c0, 3, geometry)
            result = run(c, u, 0x522d80, [actor]) & 255
            states.append((result, calls, snapshot(u)))
        assert states[0] == states[1]
        assert calls == [(40, 50, 2)] and result == int(callback_point == (41, 52))
        count += 1
    # A failed real geometry query does not write its output. Native522D80
    # still compares the retained coordinates; it does not return that query's
    # Boolean. Keep both matching and nonmatching representable coordinates.
    for point in [(50, 49), (51, 49)]:
        states = []
        for c, u, state in machines:
            setup(u, state, ships=(511,))
            byte(u, 0x7999db+511*264, 0)
            put(u, 0xd3b4e4, 50)
            put(u, 0xd3b4e0, 49)
            word(u, 0xbcd61c+511*388, point[0])
            word(u, 0xbcd61e+511*388, point[1])
            result = run(c, u, 0x522d80, [511]) & 255
            states.append((result, snapshot(u)))
        assert states[0] == states[1] and result == int(point == (50, 49))
        count += 1
    print('PASS genuine dock geometry, full IDs/scratch, failed-output retention and callback reload:', count, flush=True)


def verify_panels():
    count = 0
    for phase, origin, ships in itertools.product(
            [-2147483648, -1, 0, 1, 4, 5, 6, 256, 2147483647],
            [(0, 0), (192, 144), (300, 200)], [(), (1, 511, 1999)]):
        compare(phase=phase, origin=origin, ships=ships)
        count += 1
    assert compare(phase=1) == [(0x4cd680, (364, 232, 512, 288, 0))]
    print('PASS full fleet background/opening, all fonts/images/counts and real providers:', count, flush=True)
    count = 0
    for mission, crew, damage, assigned in itertools.product(
            [-32768, -1, 0, 32767], [0, 1, 255], [0, 15, 16, 30, 31, 50, 51, 70, 71, 90, 91, 100], [0, 1]):
        compare(ships=(511,), mission=mission, crew=crew, damage=damage, assigned=assigned)
        count += 1
    print('PASS all actual crew/damage thresholds, mission status and assignment side effects:', count, flush=True)
    count = 0
    for mode, city, empire, ships in itertools.product(
            [0, 5, 6, 7], [-1, 0, 49, 50], [0, 1, 2], [(), (511,)]):
        compare(records=[dict(kind=37, mode=mode, city=city)], empire=empire, ships=ships, extra=255)
        count += 1
    print('PASS complete event/city/empire priorities and optional summary counts:', count, flush=True)
    count = 0
    for phase, selection, mission, assigned in itertools.product(
            [4, 5, 6, 256], [0, 1, 2, 3, 19, 20, 256], [-1, 0], [0, 1]):
        compare(0x523bb0, phase=phase, selection=selection, mission=mission, assigned=assigned,
                records=[dict(kind=37)])
        count += 1
    print('PASS complete foreground, full hover slots, phase and recall publication:', count, flush=True)
    for city_state, token in itertools.product([1, 2], [0x522e10, 0x523bb0]):
        compare(token, records=[dict(kind=37)], city_state=city_state)
        compare(token, mutate=True, records=[dict(kind=37)], city_state=city_state, extra=255)
    for token in [0x522e10, 0x523bb0]:
        compare(token, ships=(), reported=1)
    print('PASS both genuine request classes, live cursor/layout/player changes and missing ship:', 10, flush=True)
    for city_state in [1, 2]:
        calls = compare(docked=True, city_state=city_state, records=[dict(kind=37)])
        assert sum(token == 0x53e590 and args[0] == 23002 for token, args in calls) == 3
    print('PASS genuine successful dock query through all three rendered ship rows:', 2, flush=True)
    for ships in [(), (1, 511, 1999)]:
        compare(shell=True, ships=ships, records=[dict(kind=37)])
    for selection, assigned in itertools.product([19, 20, 256], [0, 1]):
        calls = compare(0x523bb0, selection=selection, assigned=assigned,
                        records=[dict(kind=37)], buttons=True)
        assert any(token == 0x536c30 for token, args in calls)
    print('PASS shared advisor composition and actual button-record forwarding:', 8, flush=True)
    print('Verified target SHA256:', target_sha)


if __name__ == '__main__':
    verify_ordinal()
    verify_dock()
    verify_panels()
