"""Native/C flat masking and complete sprite dispatch, entirely in Unicorn.

Real addresses/count widths, transparency, row skips and forward overlap are
compared. No raster leaf or clipper is replaced. No host game is launched.
"""
from verify_options_startup import *

SCREEN, SOURCE = 0x35000000, 0x0f6e880e  # Source address from the reported dump.
ARENA = 0x200000
pattern = bytes(range(256))*8192
source_pixels = b''.join(struct.pack('<H', 0xf81f if i % 5 == 0 else
                                   (i*197+33) & 65535) for i in range(65536))
for c, u, state in machines:
    u.mem_map(SCREEN, ARENA)
    u.mem_map(SOURCE & ~65535, 0x40000)


def setup(u, state, width=640, height=480):
    reset(u, state)
    u.mem_write(SCREEN, pattern)
    u.mem_write(SOURCE, source_pixels)
    for at, value in [(0xe92e18, SCREEN), (0xe38d94, width*2),
                      (0xe38d9c, width), (0xe38d98, height),
                      (0xe38dac, width), (0xe38db0, height),
                      (0xd3b558, width), (0xd3b550, height),
                      (0xd3b55c, 0), (0xd3b554, 0)]:
        put(u, at, value)


def direct(token, width, rows, pitch, mask, x=300, y=260, ss=0, ds=None,
           overlap=0, screen_width=640):
    outputs = []
    count = max(1, width)
    if token == 0x4100f0 and width <= 0:
        count = 4 if width & 3 == 0 else 1
    for c, u, state in machines:
        setup(u, state, screen_width)
        for at, value in [(0xe38d94, pitch), (0xd3b5e0, x),
                          (0xd3b5dc, y), (0xd3b5d4, rows),
                          (0xd3b544, width), (0xd3b540, ss),
                          (0xd3b53c, pitch-count*2 if ds is None else ds)]:
            put(u, at, value)
        # The neighboring upper WORD is unrelated to the signed WORD width.
        put(u, 0xd3b5be, 0x6d800000 | (width & 65535))
        source = SCREEN+x*2+y*pitch+overlap if overlap else SOURCE
        before = bytes(u.mem_read(SCREEN, ARENA))
        source_before = bytes(u.mem_read(SOURCE, len(source_pixels)))
        run(c, u, token, [source, mask])
        actual = bytes(u.mem_read(SCREEN, ARENA))
        if not overlap:
            expected = bytearray(before)
            src, dst = 0, x*2+y*pitch
            for row in range(max(1, rows)):
                for col in range(count):
                    if struct.unpack_from('<H', source_before, src)[0] != 0xf81f:
                        value = struct.unpack_from('<H', expected, dst)[0]
                        struct.pack_into('<H', expected, dst, value & (mask & 65535))
                    src += 2
                    dst += 2
                src += ss if token == 0x4101c0 else 0
                dst += pitch-count*2 if ds is None else ds
            assert actual == expected, ('independent pixel result', c, hex(token), width, rows)
        outputs.append((actual, bytes(u.mem_read(SOURCE, len(source_pixels))),
                        bytes(u.mem_read(0xd3b530, 0xb4))))
    assert outputs[0] == outputs[1], ('native/C pixel difference', hex(token), width, rows, mask)


def dispatch():
    """Both actual native callers, real clippers and both flat masking leaves."""
    count = 0
    for token, sw, mask, (x, y) in itertools.product(
            [0x53e590, 0x53f4f0], [640, 800, 1024], [0xf863, 0x7bef],
            [(300, 260), (-3, 260), (635, 260), (300, -2), (300, 478)]):
        outputs = []
        for c, u, state in machines:
            setup(u, state, sw)
            # Embedded, uncompressed, keyed 16x8 image; record layout from SG3.
            image = 7
            record = bytearray(64)
            struct.pack_into('<II', record, 0, SOURCE, 16*8*2)
            struct.pack_into('<HH', record, 20, 16, 8)
            record[0x32] = 0
            u.mem_write(0x10d69f8+image*64, bytes(record))
            put(u, 0x126517c, 25500)
            byte(u, 0xe38dda, 0)
            word(u, 0x70e120, mask)
            args = [image, x, y] + ([mask] if token == 0x53f4f0 else [])
            visits = []
            observers = [u.hook_add(UC_HOOK_CODE,
                lambda uc, pc, length, data: visits.append(pc),
                begin=address(c, leaf), end=address(c, leaf))
                for leaf in [0x4100f0, 0x4101c0]]
            result = run(c, u, token, args,
                         cleanup=0 if c else len(args)*4,
                         receiver=0xf55bb0 if not c else None)
            for observer in observers:
                u.hook_del(observer)
            assert len(visits) == 1, ('mask leaf must execute', c, hex(token), sw, x, y, visits)
            pixels = bytes(u.mem_read(SCREEN, ARENA))
            assert pixels != pattern, ('no visible masked output', c, hex(token), x, y)
            outputs.append((result, pixels, bytes(u.mem_read(0xd3b530, 0xb4))))
        assert outputs[0] == outputs[1], ('complete mask dispatch', hex(token), sw, mask, x, y)
        count += 1
    return count


def fixed_tiles():
    count = 0
    for (token, size), pitch, overlap in itertools.product(
            [(0x410f70, 2), (0x4114a0, 24), (0x411fd0, 32)],
            [1280, 1600, 2048], [0, -4, 2, 4]):
        outputs = []
        for c, u, state in machines:
            setup(u, state)
            x, y = 300, 260
            for at, value in [(0xe38d94, pitch), (0xd3b5e0, x), (0xd3b5dc, y)]:
                put(u, at, value)
            destination = SCREEN+x*2+y*pitch
            source = destination+overlap if overlap else SOURCE
            # Independent access-order model, also covering overlapping inputs.
            expected = bytearray(pattern)
            def load(at, length):
                return bytes(expected[at-SCREEN:at-SCREEN+length]) if overlap else bytes(u.mem_read(at, length))
            if size == 2:
                expected[destination-SCREEN:destination-SCREEN+4] = load(source, 4)
                at = destination-SCREEN+pitch
                expected[at:at+2] = load(source+2, 2)
            else:
                for row in range(size):
                    for col in range(size//2):
                        at = destination-SCREEN+row*pitch+col*4
                        expected[at:at+4] = load(source+(row*size//2+col)*4, 4)
            run(c, u, token, [source])
            actual = bytes(u.mem_read(SCREEN, ARENA))
            assert actual == expected, ('fixed tile pixels', c, hex(token), pitch, overlap)
            outputs.append((actual, bytes(u.mem_read(0xd3b530, 0xb4))))
        assert outputs[0] == outputs[1], ('fixed tile native/C', hex(token), pitch, overlap)
        count += 1
    return count


def fixed_dispatch():
    count = 0
    for sw, (leaf, kind, size) in itertools.product([640, 800, 1024],
            [(0x410f70, 2, 2), (0x4114a0, 12, 24), (0x411fd0, 13, 32)]):
        outputs = []
        for c, u, state in machines:
            setup(u, state, sw)
            image, record = 7, bytearray(64)
            struct.pack_into('<II', record, 0, SOURCE, size*size*2)
            struct.pack_into('<HH', record, 20, size, size)
            record[0x32] = kind
            u.mem_write(0x10d69f8+image*64, bytes(record))
            put(u, 0x126517c, 25500)
            byte(u, 0xe38dda, 0)
            visits = []
            observer = u.hook_add(UC_HOOK_CODE,
                lambda uc, pc, length, data: visits.append(pc),
                begin=address(c, leaf), end=address(c, leaf))
            # Native dispatch selects these fixed blocks only when its
            # horizontal clipping status is nonzero. Execute that real path.
            result = run(c, u, 0x53e590, [image, -1, 260],
                         cleanup=0 if c else 12,
                         receiver=0xf55bb0 if not c else None)
            u.hook_del(observer)
            assert len(visits) == 1, ('fixed leaf must execute', c, hex(leaf), visits)
            outputs.append((result, bytes(u.mem_read(SCREEN, ARENA)),
                            bytes(u.mem_read(0xd3b530, 0xb4))))
        assert outputs[0] == outputs[1], ('fixed tile complete dispatcher', hex(leaf), sw)
        count += 1
    return count


if __name__ == '__main__':
    cases = 0
    for token, sw, width, mask in itertools.product(
            [0x4100f0, 0x4101c0], [640, 800, 1024],
            [1, 3, 4, 5, 8, 255, 256, 300], [0xf863, 0xffff7bef]):
        direct(token, width, 3, sw*2, mask, screen_width=sw)
        cases += 1
    for token in [0x4100f0, 0x4101c0]:
        for width, rows in [(0, 1), (-1, 1), (-4, 1), (4, 0), (4, -1), (4, 300)]:
            direct(token, width, rows, 1280, 0xf863, y=10)
            cases += 1
        for ss, ds in [(258, 1280-8), (-2, 1280-8), (0, -4)]:
            direct(token, 4, 3, 1280, 0xf863, ss=ss, ds=ds)
            cases += 1
        for overlap in [-2, 2]:
            direct(token, 8, 3, 1280, 0xf863, overlap=overlap)
            cases += 1
    print('PASS complete flat masking leaves, independent pixels and ABI:', cases, flush=True)
    print('PASS both complete native sprite callers and real masking/clipping:', dispatch(), flush=True)
    print('PASS three complete fixed tile leaves, native access order and forward overlap:', fixed_tiles(), flush=True)
    print('PASS complete sprite dispatcher reaching each fixed tile leaf:', fixed_dispatch(), flush=True)
    print('Verified target SHA256:', target_sha)
