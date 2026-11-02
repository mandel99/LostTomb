"""Complete service dispatcher, eleven native-equivalent visits and packed cells.

Offline original-x86/Release comparisons. The eleven new visit providers run
in dispatcher cases; other service/classifier endpoints are explicitly
controlled there. Final packed-cell writes run unless explicitly observed.
"""
from verify_tax_collector import *

VISITS = [(0x484450, 0x799a46), (0x484120, 0x799a4a), (0x484230, 0x799a4b),
          (0x484340, 0x799a4d), (0x484560, 0x799a4e), (0x484780, 0x799a50),
          (0x4833c0, 0x799a44), (0x4834d0, 0x799a45), (0x4835e0, 0x799a47),
          (0x4836f0, 0x799a49), (0x483800, 0x799a5e)]
CONTROLLED = {token: 2 for token in [0x484990, 0x484a90, 0x483e90, 0x42ff20,
    0x484890, 0x483160, 0x482c10, 0x483000, 0x484670]}
CONTROLLED.update({token: 3 for token in [0x483910, 0x482ef0, 0x482dd0, 0x483fb0, 0x483260]})
CONTROLLED.update({0x4c5120: 1, 0x4c52d0: 2})


def verify_pattern():
    def body(at): return bytes(images[0].data[at-images[0].base:at-images[0].base+270])
    reference = body(0x4834d0)
    assert hashlib.sha256(reference).hexdigest() == '5b41fad16ddeafe42f097b4d0289e3158310e7e88a7e4246da6a10861940287a'
    assert reference[215:222] == bytes.fromhex('c681459a790060')
    for token, field in VISITS:
        assert body(token) == reference[:217]+struct.pack('<I', field)+reference[221:]
    print('PASS eleven complete native270-byte bodies; only the field operand differs:', len(VISITS), flush=True)


def verify_visits():
    count = 0
    for (token, field), pos in itertools.product(VISITS,
            [(10, 10), (0, 0), (31, 31), (-3, 10), (40, 10), (10, 40), (10, -3)]):
        results = []
        for c, u, state in machines:
            args = prepare_cells(u, state, x=pos[0], y=pos[1],
                rows=({'id': 1, 'state': 3}, {'id': 2, 'population': -1},
                      {'id': 3, 'occupied': 0}, {'id': 4000}), ids=(1, 1, 2, 3, 4000))
            value = run(c, u, token, args)
            results.append((value, snapshot(u)))
        same(results, ('visit', hex(token), pos)); count += 1
    for token, field in VISITS:
        results = []
        for c, u, state in machines:
            prepare_cells(u, state, rows=({'id': 4000, 'state': 3},), ids=(4000,))
            p = field+4000*264
            byte(u, p-1, 0xa5); byte(u, p+1, 0x5a)
            value = run(c, u, token, [10, 10])
            assert value == 25 and b(u, p) == 96 and b(u, p-1) == 0xa5 and b(u, p+1) == 0x5a
            results.append((value, snapshot(u)))
        same(results, ('visit field guards', hex(token))); count += 1
    print('PASS real visit fields, repeated houses, inactive records, map edges/empty rectangles and BYTE guards:', count, flush=True)


def verify_packed():
    count = 0
    for channel, packed, tile in itertools.product([0, 1, 4, 5, 10, 11, 31, 32, 255, 256, 0xffffffff],
            [0, 1, 6, 7, 0x7fff, 0xffff, 0x12345678, 0x80000000, 0xffffffff], [0, 51983]):
        results = []
        shift = (channel*3) & 31; value = (packed >> shift) & 7
        expected = packed if value == 7 else ((packed & ((7 << shift) ^ 0xffff)) | ((value+1) << shift)) & 0xffffffff
        for c, u, state in machines:
            reset(u, state); p = 0x978d9c+tile*4
            put(u, p-4, 0xa5a55a5a); put(u, p, packed); put(u, p+4, 0x5a5aa5a5)
            assert run(c, u, 0x47e230, [channel, tile]) == value
            run(c, u, 0x47e250, [channel, tile])
            assert get(u, p) == expected and get(u, p-4) == 0xa5a55a5a and get(u, p+4) == 0x5a5aa5a5
            result = run(c, u, 0x47e230, [channel, tile])
            results.append((result, snapshot(u)))
        same(results, ('packed', channel, hex(packed), tile)); count += 1
    print('PASS packed get/increment, x86 shift masking, XOR FFFF, saturation, high bits and adjacent cells:', count, flush=True)


def prepare_dispatch(u, state, kind=5, actor=255, home=511, counter=290, strength=10,
                     home_kind=60, gate=1, action=12, selected_flag=0):
    prepare_cells(u, state, rows=({'id': 1, 'state': 3},), ids=(1,))
    u.mem_write(0xbcd5f8, bytes(2000*388))
    off = actor*388; p = 0xbcd5f8+off
    byte(u, p+10, kind); byte(u, p+56, action); byte(u, p+102, strength)
    for at, value in [(20, 10), (22, 10), (90, home), (94, 1000)]: word(u, p+at, value)
    put(u, p+32, 2290); put(u, 0xd056f8, 1999)
    for id in [home, 1000, 1500]:
        word(u, 0x7999f0+id*264, counter); byte(u, 0x799a80+id*264, selected_flag if id == home else 1-selected_flag)
    word(u, 0x7999e8+home*264, home_kind)
    for owner, major in [(0, 1500), (3, 1000)]:
        word(u, 0xea5ce8+owner*0x49d8, gate); put(u, 0xea5f9c+owner*0x49d8, major)
    for k in range(256): put(u, 0x5dbf70+k*4, k % 5)
    for at, value in [(0x7065f8, 11), (0x706760, 22), (0x706918, 33),
                      (0x706968, 44), (0x707b88, 55), (0x707ef8, 66)]: put(u, at, value)
    put(u, 0x978d9c+2290*4, 0xa5a50000)
    return p


def dispatch(result=25, metric=269, categories=(1, 1, 1), permitted=1, mutation=False,
             observe_packed=False, **kw):
    results = []
    for c, u, state in machines:
        p = prepare_dispatch(u, state, **kw); calls = []; classifiers = [0]
        endpoints = dict(CONTROLLED)
        if observe_packed: endpoints[0x47e250] = 2
        for token, nargs in endpoints.items():
            def controlled(uc, args, token=token):
                calls.append((token, args, bytes(uc.mem_read(p, 116)), get(uc, 0xeaac28)))
                if mutation:
                    put(uc, 0xd056f8, 1); put(uc, 0xeaac28, 3)
                    word(uc, p+20, -7-len(calls)); word(uc, p+22, 5+len(calls))
                    word(uc, p+90, 1500); put(uc, p+32, 2291)
                    byte(uc, p+10, 255); byte(uc, p+102, 31)
                put(uc, 0x9ab9dc, metric); put(uc, 0x9ab9e0, metric); put(uc, 0x9ab9e8, metric)
                if token == 0x4c5120:
                    n = classifiers[0]; classifiers[0] += 1
                    return categories[min(n, len(categories)-1)]
                if token == 0x4c52d0: return permitted
                return result
            hook(c, state, token, nargs, controlled)
        value = run(c, u, 0x4baa10, [kw.get('actor', 255)])
        assert value == 0
        results.append((calls, snapshot(u)))
    same(results, ('dispatcher', kw, result, metric, categories, permitted, mutation))


def verify_dispatch():
    count = 0
    for kind in range(256): dispatch(kind=kind); count += 1
    for kind, actor, home, counter in itertools.product([5, 40, 255], [1, 1999], [0, 511], [-32768, 299, 32767]):
        dispatch(kind=kind, actor=actor, home=home, counter=counter); count += 1
    for result, counter in itertools.product([-2147483648, -1, 0, 1, 65535, 65536, 0x7fffffff], [-32768, -1, 299, 300, 32767]):
        dispatch(result=result, counter=counter); count += 1
    for kind, strength, metric in itertools.product([8, 33], [0, 9, 10, 11, 255], [-2147483648, -1, 0, 255, 256, 0x7fffffff]):
        dispatch(kind=kind, strength=strength, metric=metric, result=metric); count += 1
    for kind, action, selected_flag in itertools.product([16, 17], [0, 12, 13, 255], [0, 1]):
        dispatch(kind=kind, action=action, selected_flag=selected_flag); count += 1
    for home_kind, gate in itertools.product(range(59, 71), [0, 1]):
        dispatch(kind=27, home_kind=home_kind, gate=gate); count += 1
    for categories, permitted in itertools.product([(0, 0, 0), (1, 1, 1), (3, 3, 3),
            (4, 4, 4), (-1, -1, -1), (1, 3, 1), (1, 1, 4)], [0, 1, 256, 257]):
        dispatch(kind=27, categories=categories, permitted=permitted); count += 1
    for kind in [5, 7, 8, 10, 26, 27, 32, 33, 64, 87, 88, 89, 98]:
        dispatch(kind=kind, mutation=True, categories=(3, 3, 3), observe_packed=True); count += 1
    print('PASS all256 kinds, live actor/home/cell reloads, WORD counters, BYTE strength and priest query order:', count, flush=True)
    print('Verified target SHA256:', target_sha, flush=True)


if __name__ == '__main__':
    verify_pattern(); verify_visits(); verify_packed(); verify_dispatch(); verify_service_and_income()
