"""Whole4138D0 and its actual display-router caller, offline in Unicorn.

Ordered layout/text/image calls and live settings reads are compared. Drawing
endpoints are controlled here; this does not certify framebuffer pixels or a
live editor session. Original executable and assets are never run or modified.
"""
from verify_options_startup import *

DRAW = {0x536820: 4, 0x4cc540: 0, 0x536a30: 4, 0x4cc720: 6,
        0x537140: 3, 0x536c30: 6, 0x4ccb30: 7, 0x4ccbb0: 6, 0x53e590: 3}
OUTER = {0x4256a0: 4, 0x51efb0: 1, 0x533e70: 0, 0x535080: 0,
         0x414c40: 0, 0x51f780: 0}
MEMBER = {0x53e590: 12}
REGIONS = [(0x7056c8, 4), (0x7848fc, 0x300), (0xe20768, 0x40),
           (0xe38d90, 0x40), (0xf55c94, 4), (0xf56f8c, 20), (0xf57314, 4)]


for c, u, state in machines:
    # The retained controller owns a 64 KiB native scratch frame; provide
    # mapped stack space below the shared entry stack without replacing it.
    u.mem_map(0x2fe00000, 0x200000)
    state['editor_outer'] = False
    if c:
        def boundary(uc, pc, size, state):
            sp = uc.reg_read(UC_X86_REG_ESP)
            token, f = get(uc, sp+8), get(uc, sp+12)
            if not state['editor_outer'] or token not in OUTER:
                return
            frame_sp = get(uc, f+16)
            args = tuple(get(uc, frame_sp+4+i*4) for i in range(OUTER[token]))
            state['calls'].append((token, args))
            put(uc, f, 0)
            put(uc, f+4, 0xdeadbeef)
            put(uc, f+8, 0xbaadf00d)
            put(uc, f+32, 2)
            put(uc, f+16, frame_sp+4)
            uc.reg_write(UC_X86_REG_ESP, sp+4)
            uc.reg_write(UC_X86_REG_EIP, get(uc, sp))
        pc = sym('restore_menu_overlay_dispatch')
        u.hook_add(UC_HOOK_CODE, boundary, state, begin=pc, end=pc)


def exercise(origin, hover, choice, month, toggle, variant=0, outer=False):
    results = []
    for c, u, state in machines:
        reset(u, state)
        state['editor_outer'] = outer
        for at, value in [(0xe38da8, origin[0]), (0xe38da4, origin[1]),
                          (0x7056c8, hover), (0xe20770, 0x1042),
                          (0xf56f8c, 0x12340003), (0xf56f9c, 0x43210002),
                          (0xf55c94, 0x2345), (0xf57314, 0x6789),
                          (0x784cb0, 0xabcdef02), (0x605a3c, 1), (0x605a38, 1)]:
            put(u, at, value)
        byte(u, 0xe38dda, 1)
        byte(u, 0xe3908c, variant & 1)
        word(u, 0x784b5e, choice)
        word(u, 0x7848fc, month)
        byte(u, 0x784cbc, 0xaa)
        byte(u, 0x784cbd, toggle)
        u.mem_write(0x784914, b'Native editor scenario\0')
        def observe(uc, args, token):
            state['calls'].append((token, args))
            uc.reg_write(UC_X86_REG_ECX, 0xdeadbeef)
            uc.reg_write(UC_X86_REG_EDX, 0xbaadf00d)
            if variant:
                if token == 0x536820:
                    put(uc, 0xe38da8, 0x12340010)
                    put(uc, 0xe38da4, 0x54320020)
                if token == 0x4cc720:
                    put(uc, 0xe20770, 137)
                if token in [0x536c30, 0x4ccb30]:
                    put(uc, 0x7056c8, (get(uc, 0x7056c8)+1) & 0xffffffff)
                    put(uc, 0xf56f8c, get(uc, 0xf56f8c)+257)
                    word(uc, 0x7848fc, -123)
                    byte(uc, 0x784cbd, 1)
                    put(uc, 0x784cb0, 0x12340004)
                    word(uc, 0x784b5e, -1 if variant == 1 else 256)
            return 0
        for token, n in DRAW.items():
            hook(c, state, token, n, lambda uc, args, t=token: observe(uc, args, t),
                 cleanup=0 if c else MEMBER.get(token, 0))
        if outer and not c:
            for token, n in OUTER.items():
                hook(c, state, token, n,
                     lambda uc, args, t=token: state['calls'].append((t, args)) or 0)
        run(c, u, 0x4d75d0 if outer else 0x4138d0)
        calls = tuple(state['calls'])
        assert sum(t == 0x536c30 for t, a in calls) == 10
        assert sum(t == 0x53e590 for t, a in calls) == 1
        results.append((calls, tuple(bytes(u.mem_read(a, n)) for a, n in REGIONS)))
    assert results[0] == results[1], ('editor options', origin, hover, choice, month,
                                    toggle, variant, outer,
                                    next(((i, a, b) for i, (a, b) in enumerate(
                                        zip(results[0][0], results[1][0])) if a != b), None))


if __name__ == '__main__':
    origins = [(0, 0), (80, 60), (192, 144), (320, 260), (-8, -12),
               (0xfffffff0, 0xffffffe0)]
    cases = 0
    for origin, hover in itertools.product(origins, [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 257, 0xffffffff]):
        exercise(origin, hover, 18, 511, 1)
        cases += 1
    for choice, month, toggle in [(0, 0, 0), (18, 255, 2), (19, 256, 1),
                                  (255, -1, 255), (256, -32768, 0), (-1, 32767, 1),
                                  (-32768, -123, 1), (32767, 300, 0)]:
        exercise((320, 260), 9, choice, month, toggle)
        cases += 1
    for variant, origin in itertools.product([1, 2], origins):
        exercise(origin, 1, 0, 12, 0, variant)
        cases += 1
    print('PASS whole editor options renderer, full-width fields and callback reloads:', cases, flush=True)
    for variant, origin in itertools.product([0, 1, 2], origins[:4]):
        exercise(origin, 2, 19, 300, 1, variant, outer=True)
    print('PASS real display router composing the complete editor renderer:', 12, flush=True)
    print('Verified target SHA256:', target_sha)
