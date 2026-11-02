"""Native/C groundwater grass selections with real neighborhood/context code.

Image selection controls only the unrelated queued-building flush. Preparation
checks both controlled dispatch and a composition with actual recursive flood
repair and redraw helpers. This checks image IDs and terrain state, not final
pixels or the user's complete scene.
"""
from verify_options_startup import *

NEIGHBORS = [-228, -227, 1, 229, 228, 227, -1, -229]
BASE, CELL = 10000, 10000+10*228+10


def exercise(growth, neighbor_growth, block, orientation, random, water):
    output = []
    for c, u, state in machines:
        reset(u, state)
        for at, size in [(0xaf598c, 51984*4), (0xb7482c, 51984*4),
                         (0xd05720, 51984*4), (0xb4e6fc, 51984*2)]:
            u.mem_write(at, bytes(size))
        u.mem_write(0xa509bc, bytes([neighbor_growth])*51984)
        u.mem_write(0xacf85c, bytes([random])*51984)
        for at, value in [(0xbca2d0, 40), (0xbca2d4, 40), (0xbca2d8, BASE),
                          (0xbca2dc, 188), (0xbca2e0, orientation),
                          (0xbc9768, 12345), (0xaf598c+CELL*4, 128)]:
            put(u, at, value)
        for group in range(0x240):
            put(u, 0xf55bb4+group*8, 10000+group*100)
        for n in NEIGHBORS:
            put(u, 0xaf598c+(CELL+n)*4, 128)
        if block:
            put(u, 0xaf598c+(CELL+1)*4, block)
        if water:
            put(u, 0xaf598c+(CELL-228)*4, 0x10000)
        byte(u, 0xa509bc+CELL, growth)
        hook(c, state, 0x4f6130, 0, lambda uc, args: 0)
        u.reg_write(UC_X86_REG_FPCW, 0x37f)
        u.reg_write(UC_X86_REG_FPSW, 0)
        u.reg_write(UC_X86_REG_FPTAG, 0xffff)
        run(c, u, 0x4767d0, [10, 10, 10, 10, 1, 1])
        output.append((get(u, 0xb7482c+CELL*4), bytes(u.mem_read(0xbc9690, 0x140)),
                       bytes(u.mem_read(0xaf598c+CELL*4-1000, 2000)),
                       bytes(u.mem_read(0xa509bc+CELL-256, 512))))
    assert output[0] == output[1], ('grass selection', growth, neighbor_growth,
                                   hex(block), orientation, random, water,
                                   output[0][0], output[1][0])
    return output[1][0]


def generate(climate, distance, terrain):
    output = []
    for c, u, state in machines:
        reset(u, state)
        u.mem_write(0xaf598c, bytes(51984*4))
        u.mem_write(0xd05720, bytes(51984*4))
        u.mem_write(0xa509bc, bytes([255])*51984)
        for at, value in [(0xbca2d0, 40), (0xbca2d4, 40), (0xbca2d8, BASE),
                          (0xbca2dc, 188), (0xbca2e0, 2), (0xbc9768, 12345),
                          (0xaf598c+CELL*4, 128)]:
            put(u, at, value)
        for offset in [-228, 0, 228]:
            put(u, 0xaf598c+(CELL-distance+offset)*4, terrain)
        byte(u, 0x784cb0, climate)
        u.reg_write(UC_X86_REG_FPCW, 0x37f)
        u.reg_write(UC_X86_REG_FPSW, 0)
        u.reg_write(UC_X86_REG_FPTAG, 0xffff)
        run(c, u, 0x487450, [10, 10, 10, 10])
        output.append((bytes(u.mem_read(0xa509bc, 51984)),
                       bytes(u.mem_read(0xaf598c, 51984*4)),
                       bytes(u.mem_read(0xbc9690, 0x150)),
                       bytes(u.mem_read(0x5cbfb8, 26*16))))
    assert output[0] == output[1], ('whole grass generation', climate, distance, hex(terrain),
        [(i, a, b) for i, (a, b) in enumerate(zip(output[0][0], output[1][0])) if a != b][:8])


def prepare_rectangle(bounds, wrapper=False, composed=False, mutate=False):
    output = []
    for c, u, state in machines:
        reset(u, state)
        u.mem_write(0xaf598c, struct.pack('<I', 4)*51984)
        for at, value in [(0xbca2d0, 14), (0xbca2d4, 12),
                          (0xbca2d8, BASE), (0xbca2dc, 214)]:
            put(u, at, value)
        # Four genuine dispatch cases at nonzero physical map origin.
        for y in range(12):
            for x in range(14):
                flags = [4, 0x10000, 0x40, 0][(x+y) % 4]
                put(u, 0xaf598c+(BASE+y*228+x)*4, flags)
        if not composed:
            def observe(uc, args, token):
                state['calls'].append((token, args))
                uc.reg_write(UC_X86_REG_ECX, 0xdeadbeef)
                uc.reg_write(UC_X86_REG_EDX, 0xbaadf00d)
                if mutate:
                    put(uc, 0xbca2d8, 20000)
                    put(uc, 0xbca2d0, 1000)
                    put(uc, 0xbca2dc, 215)
                return 0
            for token, n in [(0x477f60, 0), (0x477f70, 2), (0x478090, 2), (0x478270, 2)]:
                hook(c, state, token, n, lambda uc, args, t=token: observe(uc, args, t))
        # Actual redraw executes software x87 helpers and needs more instructions
        # than the controlled dispatch cases; retain a finite execution bound.
        run(c, u, 0x478420 if wrapper else 0x478330, [] if wrapper else list(bounds),
            budget=30000000 if composed else 2000000)
        output.append((tuple(state['calls']), bytes(u.mem_read(0xaf598c, 51984*4)),
                       bytes(u.mem_read(0xbca2d0, 16))))
    assert output[0] == output[1], ('terrain preparation rectangle', bounds, wrapper, composed, mutate,
        next(((i, a, b) for i, (a, b) in enumerate(zip(output[0][0], output[1][0])) if a != b), None))


if __name__ == '__main__':
    count = 0
    for growth, neighbor in itertools.product([0, 1, 40, 41, 87, 88, 90, 91, 95, 96, 100, 128, 140, 141, 255], [40, 88, 100]):
        exercise(growth, neighbor, 0, 0, 7, False)
        count += 1
    for block, orientation, water in itertools.product([1, 0x10000000, 8, 0x800], [0, 2, 4, 6], [False, True]):
        exercise(90, 88, block, orientation, 23, water)
        count += 1
    print('PASS real grass image selection, growth stages, neighbors, orientation and context:', count)
    count = 0
    for climate, distance, terrain in itertools.product([0, 1, 2], [1, 3, 7], [4, 0x10000]):
        generate(climate, distance, terrain)
        count += 1
    print('PASS complete grass growth and wet edge generation with real helpers:', count)
    count = 0
    for bounds, mutate in itertools.product([(0, 0, 3, 3), (8, 5, 10, 8), (-2, -1, 2, 3), (10, 8, 30, 30), (3, 3, 1, 1)], [False, True]):
        prepare_rectangle(bounds, mutate=mutate)
        count += 1
    prepare_rectangle((), wrapper=True)
    count += 1
    print('PASS complete terrain preparation rectangle and whole-map wrapper:', count)
    prepare_rectangle((8, 5, 10, 8), composed=True)
    print('PASS terrain preparation with actual recursive flood repair and redraw helpers: 1')
    print('Verified target SHA256:', target_sha)
