"""Actual licensed stock/worker sprites through native and Release raster paths.

No sprite, clipping or raster endpoint is substituted. SG3 records and their
555 payloads are read without modifying the installed assets. This focuses on
the reported hunting-stock and granary-worker layers, not a live-game claim.
"""
from verify_options_startup import *

PIXELS, SCREEN = 0x34000000, 0x35000000
DATA = ROOT/'build/debug-startup-game/Data'


def archive(name, first, wanted):
    metadata = (DATA/(name+'.sg3')).read_bytes()
    pixels = (DATA/(name+'.555')).read_bytes()
    skip = 200 if metadata[0x2a8:0x2b2] == b'system.bmp' else 0
    offset = 4
    records = {}
    count = struct.unpack_from('<I', metadata, 16)[0]
    for i in range(count):
        record = bytearray(metadata[0x9f28+i*64:0x9f28+(i+1)*64])
        length = struct.unpack_from('<I', record, 4)[0]
        image = first+i-skip
        if image in wanted:
            assert not record[0x34], 'This regression requires an embedded sprite'
            assert offset+length <= len(pixels)
            struct.pack_into('<I', record, 0, PIXELS+offset)
            records[image] = bytes(record)
        if not record[0x34]:
            offset += length
    assert set(records) == set(wanted)
    return records, pixels


def exercise(token, args, records, pixels, width, height):
    output = []
    for c, u, state in machines:
        reset(u, state)
        u.mem_write(PIXELS, pixels)
        u.mem_write(SCREEN, b'\x5a'*(width*height*2))
        for image, record in records.items():
            u.mem_write(0x10d69f8+image*64, record)
        for at, value in [(0x126517c, 25500), (0xf56a84, 13198),
                          (0xe92e18, SCREEN), (0xe38d94, width*2),
                          (0xe38d9c, width), (0xe38d98, height),
                          (0xe38dac, width), (0xe38db0, height),
                          (0xd3b550, height), (0xd3b554, 0),
                          (0xd3b558, width), (0xd3b55c, 0)]:
            put(u, at, value)
        byte(u, 0xe38dda, 0)
        word(u, 0x7999e8+9*264, 115)
        word(u, 0x799a14+9*264, args[-1] if token == 0x436310 else 0)
        actual_args = args[:-1] if token == 0x436310 else args
        run(c, u, token, actual_args)
        output.append(bytes(u.mem_read(SCREEN, width*height*2)))
    assert output[0] == output[1], (hex(token), args, width, height,
        [(i, a, b) for i, (a, b) in enumerate(zip(*output)) if a != b][:12])
    changed = sum(a != 0x5a for a in output[1])
    if token == 0x436310 and args[-1] == 0:
        assert changed == 0
    else:
        assert changed > 0, ('No visible pixels', hex(token), args)


def verify():
    for c, u, state in machines:
        u.mem_map(0x2fe00000, 0x200000)
        u.mem_map(PIXELS, 0x1000000)
        u.mem_map(SCREEN, 0x200000)
    stock = archive('Pharaoh_General', 11907, range(13198, 13203))
    worker = archive('SprAmbient', 15831, range(18477, 18501))
    cases = 0
    for width, height in [(640, 480), (800, 600), (1024, 768)]:
        for amount in [0, 1, 100, 199, 200, 300, 500]:
            for x, y in [(width//2-120, 250), (-65, 20), (width-70, height-30)]:
                exercise(0x436310, [9, x, y, amount], *stock, width, height)
                cases += 1
        for frame in [1, 9, 18, 24]:
            for x, y in [(width//2-120, 250), (-108, 25), (width-120, height-10)]:
                exercise(0x4381b0, [18476+frame, x, y, 113, 3],
                         *worker, width, height)
                cases += 1
    print('PASS actual hunting-stock/granary-worker framebuffer bytes, three sizes and clipping:', cases)
    print('Verified target SHA256:', target_sha)
    verify_building_layers()
    verify_animation_transitions()


def verify_building_layers():
    """Real normal-view building upper, animation and stock layers together.

    Granary food piles are the declared controlled boundary; their inherited
    caller-frame obligation is independent of the worker anchor reported here.
    """
    from verify_world_detail import prepare
    general, payload = archive('Pharaoh_General', 11907,
                               [12757, 13991, *range(13198, 13203)])
    ambient, worker_payload = archive('SprAmbient', 15831, range(18477, 18501))
    # Keep both archives in one mapped source arena with distinct pointers.
    shift = 0xc00000
    ambient = {image: struct.pack('<I', struct.unpack_from('<I', record)[0]+shift)+record[4:]
               for image, record in ambient.items()}
    cases = 0
    for (width, height), kind, frame, amount in itertools.product(
            [(640, 480), (800, 600), (1024, 768)], [115, 71], [0, 1, 9, 18], [100, 300]):
        output = []
        for c, u, state in machines:
            image, footprint = (12757, 1) if kind == 115 else (13991, 3)
            prepare(u, state, 0, kind, footprint, building=9, image=image,
                    width=width, height=height)
            u.mem_write(PIXELS, payload)
            u.mem_write(PIXELS+shift, worker_payload)
            u.mem_write(SCREEN, b'\x5a'*(width*height*2))
            for image, record in {**general, **ambient}.items():
                u.mem_write(0x10d69f8+image*64, record)
            for at, value in [(0xf56a84, 13198), (0xf56f34, 18477),
                              (0xe92e18, SCREEN), (0xe38d94, width*2),
                              (0xe38d9c, width), (0xe38d98, height),
                              (0xe38dac, width), (0xe38db0, height),
                              (0xd3b550, height), (0xd3b554, 0),
                              (0xd3b558, width), (0xd3b55c, 0)]:
                put(u, at, value)
            byte(u, 0xe38dda, 0)
            word(u, 0x799a14+9*264, amount)
            byte(u, 0xadc36c+18000, frame)
            hook(c, state, 0x442280, 4, lambda uc, args: 1)
            run(c, u, 0x436b60, [0])
            output.append(bytes(u.mem_read(SCREEN, width*height*2)))
        assert output[0] == output[1], ('building layers', width, kind, frame, amount)
        assert output[1] != b'\x5a'*(width*height*2)
        cases += 1
    print('PASS building upper/worker/stock pixels together, real footprints and three sizes:', cases)


def verify_animation_transitions():
    """Real staffing/frame query and SG3 anchors across the wrap boundary.

    Observe final drawing only; the actual city renderer and animation query
    execute. Food piles remain the separate declared boundary above.
    """
    from verify_world_detail import prepare, record
    from verify_building_overlays import sprite_spy
    metadata = (DATA/'SprAmbient.sg3').read_bytes()
    general = (DATA/'Pharaoh_General.sg3').read_bytes()
    cases = 0
    for (width, height), frame, due, staffed in itertools.product(
            [(640, 480), (800, 600), (1024, 768)],
            [0, 6, 23, 24, 0x81, 0x98], [0, 1], [0, 1]):
        results = []
        for c, u, state in machines:
            prepare(u, state, 0, 71, 3, building=9, image=13991,
                    width=width, height=height)
            # first_image=15831 for SprAmbient; the SG3 record is zero based.
            for image in range(18477, 18501):
                start = 0x9f28+(image-15831)*64
                u.mem_write(record(image), metadata[start:start+64])
            base = 0x9f28+(13991-11907+200)*64
            u.mem_write(record(13991), general[base:base+64])
            put(u, 0xf55bb4+624*8, 18477)
            word(u, 0x799a18+9*264, staffed)
            byte(u, 0xadc36c+18000, frame)
            speed = general[base+0x30]
            byte(u, 0x605b38+speed, due)
            draws = []
            sprite_spy(c, state, draws)
            # This test observes drawing arguments, including the building top.
            hook(c, state, 0x53f250, 3, lambda uc, args: 0,
                 cleanup=0 if c else 12)
            hook(c, state, 0x442280, 4, lambda uc, args: 1)
            run(c, u, 0x436b60, [0])
            results.append((draws, snapshot(u)))
        assert results[0] == results[1], ('animation transition', width,
            frame, due, staffed, results[0][0], results[1][0])
        workers = [call for call in draws if 18477 <= call[0] <= 18500]
        if not staffed:
            assert not workers
        elif workers:
            image, x, y = workers[0]
            dx, dy = struct.unpack_from('<hh', metadata,
                0x9f28+(image-15831)*64+34)
            assert (x, y) == (width//2-240+113-dx, 265+3-dy)
        cases += 1
    print('PASS actual granary frame/staffing transitions, SG3 anchors and three sizes:',
          cases, flush=True)


if __name__ == '__main__':
    verify()
