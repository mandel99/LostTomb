"""Whole trade header/list/luxury-count/scrollbar/arrow, native versus C.

Genuine availability, country queries and integer percentage helpers execute.
Drawing and button endpoints are observed. Explicit query callback cases
exercise cached fonts and AL eligibility; no live-game certification.
"""
from verify_kingdom_advisor import *

ENDS = {**ENDS, 0x4d6250: 9, 0x4cce90: 8, 0x425e80: 5, 0x425c40: 4, 0x4256a0: 4}


def setup(u, state, resources=(1,), stage=6, first=1, price=0, mode=0,
          target=7, stock=12345, flag=0, selected=0, stopped=0,
          player=3, countries=(), flags=None, initial_counts=None):
    reset(u, state)
    u.mem_write(0x77de00, bytes(61*106)); u.mem_write(0x785d70, bytes(18628))
    for i in range(4*36): put(u, 0x77646c+i*4, 40)
    for owner in [0, 3]:
        off = owner*0x49d8
        u.mem_write(0xea4096+off, bytes(0x100))
        u.mem_write(0xea41fc+off, bytes(0x30))
        for k in range(37):
            word(u, 0xea4096+off+k*2, mode)
            word(u, 0xea40de+off+k*2, target)
            word(u, 0xea4126+off+k*2, flag)
            put(u, 0xea42f8+off+k*4, stopped)
            put(u, 0xea6160+off+k*4, stock)
            if k in (resources if flags is None else flags):
                byte(u, 0xea41fc+off+k, 1); byte(u, 0xea4205+off+k, 1)
        for k in range(3): word(u, 0xea61f4+off+k*2, 0)
        put(u, 0xea60a4+off, first); byte(u, 0xea623c+off, price)
    for data in countries:
        p = 0x77de00+data.get('id', 1)*106
        byte(u, p, data.get('active', 1)); byte(u, p+2, data.get('type', 1))
        byte(u, p+3, data.get('subtype', 0)); byte(u, p+4, 0); byte(u, p+5, data.get('enabled', 1))
        for k in data.get('first_flags', ()): byte(u, p+42+k, 1)
        for k in data.get('second_flags', ()): byte(u, p+6+k, 1)
    for at, value in [(0xeaac28, player), (0xf53f48, stage), (0xe38da8, 300),
                      (0xe38da4, 200), (0xf53f0c, 300), (0xf53f08, 200),
                      (0xf5681c, 20000), (0xf571fc, 21000), (0xf56494, 22000),
                      (0xf56f8c, 16416), (0xf56f94, 16432), (0xf56f9c, 16448),
                      (0xf56fa4, 16464), (0xf56fb4, 16480), (0xe20778, 700),
                      (0x7056c8, selected), (0xf53e6c, len(resources)), (0xd3b198, 0)]: put(u, at, value)
    u.mem_write(0xf53e70, bytes(35*4))
    for i, k in enumerate(resources): put(u, 0xf53e70+i*4, k)
    word(u, 0x603322, 0xa1a2); word(u, 0x603324, 0xa3a4)
    word(u, 0x603326, 0xa5a6); word(u, 0x603328, 0xa7a8)
    u.mem_write(S, struct.pack('<16I', *[v & 0xffffffff for v in (initial_counts or [0]*16)]))


def observe(c, state, calls, mutation=None, queries=None, al_callback=None):
    for token, arity in ENDS.items():
        def draw(uc, args, token=token):
            calls.append((token, args))
            if token in [0x4d62f0, 0x4d6250, 0x4d6860, 0x4cce90, 0x4ccc30]:
                put(uc, 0xe20778, get(uc, 0xe20778)+17)
            if mutation == 'geometry':
                for field, delta in [(0xf53f0c, 3), (0xf53f08, 2),
                                     (0xe38da8, 7), (0xe38da4, 5),
                                     (0xf56f8c, 16), (0xf56f94, 16), (0xf56f9c, 16), (0xf56fa4, 16)]:
                    put(uc, field, get(uc, field)+delta)
            if mutation == 'player': put(uc, 0xeaac28, 0 if get(uc, 0xeaac28) else 3)
            if mutation == 'list' and token in [0x425e80, 0x425c40, 0x4256a0]:
                put(uc, 0xea60a4+get(uc, 0xeaac28)*0x49d8, 3)
            if mutation == 'mode' and token == 0x4d62f0:
                off = get(uc, 0xeaac28)*0x49d8
                byte(uc, 0xea623c+off, 0 if bytes(uc.mem_read(0xea623c+off, 1))[0] else 1)
            return 0x12345678
        hook(c, state, token, arity, draw, cleanup=12 if not c and token == 0x53e590 else 0)
    if queries is not None:
        for i, token in enumerate([0x443950, 0x443b20, 0x443a20, 0x4439c0]):
            def query(uc, args, i=i, token=token):
                calls.append((token, args))
                put(uc, 0xf56f9c, get(uc, 0xf56f9c)+32)
                if mutation == 'query_rebase':
                    put(uc, 0xeaac28, 0); word(uc, 0xea4096+args[0]*2, 4)
                    return 0x100 if token == 0x443950 else 0
                return (queries >> i) & 1
            hook(c, state, token, 1, query)
    if al_callback is not None:
        def eligible(uc, args):
            calls.append((0x447790, args)); return al_callback
        hook(c, state, 0x447790, 2, eligible)


def compare(token=0x55a0e0, args=(), mutation=None, queries=None, al_callback=None,
            arrow=0, total=None, shell=False, pipeline=False, **kw):
    results = []
    for c, u, state in machines:
        setup(u, state, **kw); calls = []; observe(c, state, calls, mutation, queries, al_callback)
        put(u, 0xd3b198, arrow)
        if total is not None: put(u, 0xf53e6c, total)
        if shell:
            put(u, 0x605a44, 6); put(u, 0xe38dac, 800); put(u, 0xe38db0, 600); byte(u, 0xe38dca, 1)
            for at, value in [(0xf55c0c, 20000), (0xf5691c, 22000), (0xf56914, 21000)]: put(u, at, value)
        if pipeline:
            run(c, u, 0x559e10)
        value = run(c, u, 0x520ea0 if shell else token, args)
        if pipeline and token == 0x40c760: run(c, u, 0x55a0e0)
        results.append((value if token in [0x559fc0, 0x5599c0] else None, calls, snapshot(u), bytes(u.mem_read(S, 64))))
    assert results[0] == results[1], (hex(token), args, kw, mutation, queries, arrow, total, pipeline,
        next(((i, a, b) for i, (a, b) in enumerate(zip(results[0][1], results[1][1])) if a != b),
             ('call counts', len(results[0][1]), len(results[1][1]))),
        [(hex(0x560000+i), a, b) for i, (a, b) in enumerate(zip(results[0][-2], results[1][-2])) if a != b][:12])


def verify_panel():
    count = 0
    for stage, price in itertools.product([0, 1, 4, 5, 6, 256, 0x80000000], [0, 1]):
        compare(0x559e10, stage=stage, price=price); compare(stage=stage, price=price); count += 2
    for resources, first in itertools.product([(), (1,), tuple(range(1, 15)), tuple(range(1, 16)), tuple(range(1, 36))], [-1, 1, 2, 30]):
        compare(resources=resources, first=first); count += 1
    for price in [0, 1]:
        for resource in [1, 10, 19, 24, 25, 26, 27, 28, 30, 35, 36]:
            compare(resources=(resource,), price=price); count += 1
    for resource, mode in itertools.product([1, 19, 24, 28, 36], [-1, 0, 1, 2, 3, 256]):
        compare(resources=(resource,), mode=mode, stopped=1, flag=1, selected=4); count += 1
    for stock, target in [(-2147483648, -32768), (-1, -1), (99, 0), (100, 1), (2147483647, 32767)]:
        compare(resources=(1, 10, 24, 36), stock=stock, target=target, mode=1); count += 1
    for answers in range(16):
        compare(queries=answers); count += 1
    for mutation in ['geometry', 'player', 'list', 'mode']:
        compare(resources=tuple(range(1, 36)), mutation=mutation, first=3); count += 1
    print('PASS trade header/list, rows, all status/price/unit branches and callback reloads:', count, flush=True)


def luxury_types():
    raw = images[0].data
    return [(i, struct.unpack_from('<I', raw, 0x5d2a3c+i*8-images[0].base)[0]-38)
            for i in range(256) if 38 <= struct.unpack_from('<I', raw, 0x5d2a3c+i*8-images[0].base)[0] < 54]


def verify_countries():
    types = luxury_types(); assert len(types) >= 2
    different = next(v for v in types if v[1] != types[0][1])
    one = {'id': 1, 'subtype': types[0][0], 'first_flags': (19,)}
    second = {'id': 60, 'subtype': different[0], 'first_flags': (19,)}
    count = 0
    for mode, countries in itertools.product([0, 1, 2, 3], [(), (one,), (one, {**one, 'id': 2}), (one, second)]):
        compare(0x559fc0, [S], mode=mode, countries=countries); count += 1
        compare(resources=(19,), mode=mode, countries=countries); count += 1
    for player, flag in itertools.product([0, 3], [0x100, 0x101]):
        compare(0x559fc0, [S], mode=1, player=player, countries=(one,), al_callback=flag); count += 1
    for subtype, index in [types[0], different, types[-1]]:
        initial = [0]*16; initial[index] = -1
        compare(0x559fc0, [S], mode=1, countries=({**one, 'subtype': subtype},), initial_counts=initial); count += 1
    for resource, flags, enabled in itertools.product([1, 24], ['first_flags', 'second_flags'], [0, 1]):
        compare(resources=(resource,), countries=({'id': 60, flags: (resource,), 'enabled': enabled},)); count += 1
    print('PASS real country queries, luxury bins/accumulation, AL and records1..60:', count, flush=True)


def verify_scroll_and_compositions():
    count = 0
    for total, first in itertools.product([0, 14, 15, 16, 35], [0, 1, 2, 21, 35]):
        compare(0x55a060, first=first, total=total); count += 1
        for arrow in [0, 1]:
            compare(0x40c760, first=first, total=total, arrow=arrow); count += 1
    for first, arrow in itertools.product([1, 20, 22], [0, 1]):
        compare(0x40c760, resources=tuple(range(1, 36)), first=first, arrow=arrow, pipeline=True); count += 1
    compare(shell=True); count += 1
    for price, player in itertools.product([0, 1], [0, 3]):
        compare(resources=(1, 9, 12, 19, 24, 35), price=price, player=player, pipeline=True); count += 1
    print('PASS native scrollbar/arrow limits and actual availability/router compositions:', count, flush=True)


def verify_callback_router():
    count = 0
    for total, first, arrow in [(n, n-13, a) for n, a in itertools.product([14, 15, 35], [0, 1])] + [(0, -1, 1), (35, 1, 0), (35, 22, 256)]:
        results = []
        for c, u, state in machines:
            setup(u, state, first=first); put(u, 0xf53e6c, total); put(u, 0xd3b198, arrow)
            if not c: run(c, u, 0x40c760)
            else:
                for r, v in zip(registers, [0x11111111, 0x22222222, 0x33333333, saved[0], SP, *saved[1:]]): u.reg_write(r, v)
                u.reg_write(UC_X86_REG_EFLAGS, 2); put(u, SP, STOP); put(u, SP+4, 0x40c760)
                u.emu_start(sym('restore_dropdown_callback'), STOP, count=2000000)
                assert u.reg_read(UC_X86_REG_EIP) == STOP and u.reg_read(UC_X86_REG_ESP) == SP+4
                assert [u.reg_read(registers[i]) for i in [3, 5, 6, 7]] == saved
            results.append(snapshot(u))
        assert results[0] == results[1], ('central scroll callback', total, first, arrow)
        count += 1
    print('PASS actual dropdown/central callback routing into the restored arrow:', count, flush=True)


def verify_policy_reset():
    count = 0
    for resource, mode, availability in itertools.product([1, 19, 35], [0, 1, 2, 3, 4, 256, 0x8001, 0xffff], range(4)):
        country = {'id': 60}
        if availability & 1: country['first_flags'] = (resource,)
        if availability & 2: country['second_flags'] = (resource,)
        compare(0x5599c0, [resource], resources=(resource,), mode=mode, countries=(country,)); count += 1
    for resource, mode in itertools.product([1, 19, 35], [1, 3]):
        compare(0x5599c0, [resource], resources=(resource,), mode=mode, queries=0,
                mutation='query_rebase'); count += 1
    print('PASS policy reset: exact WORD stride, real eligibility, post-query owner and full EAX:', count, flush=True)


if __name__ == '__main__':
    verify_panel(); verify_countries(); verify_scroll_and_compositions(); verify_callback_router(); verify_policy_reset()
    print('Verified target SHA256:', target_sha)
