"""Fresh draw replay inputs; this module neither loads nor executes an image.

The original draw members and their actual pure virtual predicates execute.
Only projection, occupancy and rendering are controlled endpoints.  No expected
draw coordinates or predicate results are computed here: native/C event order,
raw scalar arguments and watched memory are compared by the shared runner.
"""
import struct

SCRATCH = 0x32000000
SELF = SCRATCH + 0x1000
VTABLE = SCRATCH + 0x100
PIECES = SCRATCH + 0x20000
STRIDE = 52
GRID_START = 0xAF5000
GRID_SIZE = 0x6000
GRID = 0xAF598C
NEIGHBORS = (0xAF55FC, 0xAF5D1C, 0xAF5988, 0xAF5990)
ROTATION = 0xBCA2E0
ORIGIN = 0xBCA2D8
CURSOR = 0xBCA300
MAP_X, MAP_Y = 0xBCA304, 0xBCA308
BLOCK_X, BLOCK_Y = 0xBCA2E8, 0xBCA2EC
DX, DY = 0x70E45C, 0x70E44C
TILE, IMAGE = 0xBC9768, 0xF56BE4
FORWARDERS = {
    0x401420: (0x4C, 16, False),
    0x40D870: (0x4C, 25, False),
    0x416F90: (0x4C, 9, False),
    0x4E6610: (0x4C, 2, False),
    0x5456E0: (0x1E5C, 167, True),
    0x54F410: (0x5C88, 513, True),
    0x555E20: (0x4EE0, 437, True),
    0x558B80: (0x1CBC, 158, True),
}
SHARED = {0x4E5490: False, 0x545FC0: True}
ENTRIES = tuple(FORWARDERS) + tuple(SHARED)


def _u32(value):
    return struct.pack('<I', value & 0xFFFFFFFF)


def _words(values):
    return b''.join(_u32(value) for value in values)


def _piece(x, y, side, last=False):
    # The two unused DWORDs are distinct sentinels. Screen offsets use all
    # four orientations, including negative values, in the 52-byte record.
    return _words([x, y, side, 0xC3170001, 0xC3170002,
                   *([-23, 29, -31, 37] if last else [11, -13, 17, -19]),
                   *([13, -17, 19, -23] if last else [5, -7, 9, -11])])


def _tables():
    stores = []
    watched = []
    for address, payload in [
        (0x5CC8C0, b''.join(_words([r * 228 + c, 0xD19A0000 + r * 6 + c])
                            for r in range(6) for c in range(6))),
        (0x5E44D0, _words([(i * 5 + 3) % 36 for i in range(36)])),
        (0x603258, _words([(i * 7 + 11) % 36 for i in range(36)])),
        (0x5CFCF0, b''.join(_words([100 + i * 7, -90 + i * 11])
                            for i in range(36))),
    ]:
        stores.extend([(address - 16, b'\xA5' * (len(payload) + 32)),
                       (address, payload)])
        watched.append((address - 16, len(payload) + 32))
    return stores, watched


def _terrain(stores, origin, cursor_x, cursor_y, last, selected, special,
             mode, active):
    # Fixture addresses are ordinary map cells, seeded without calculating any
    # expected draw argument. Each side2 square has four distinct map cells.
    for index in active:
        x, y = (8, 4) if index == last else (2, 1)
        tile = origin + (y + cursor_y) * 228 + x + cursor_x
        good = 0 if not special or index == selected else 0x20000002
        if mode == 'invalid':
            good = 1
        for row in range(2):
            for column in range(2):
                stores.append((GRID + (tile + row * 228 + column) * 4,
                               _u32(good)))
        if special and index == selected:
            if mode == 'no-neighbor':
                # Leave only the selected base cell admissible. In a side2
                # square, east/south neighbors overlap two ordinary cells.
                for row in range(2):
                    for column in range(2):
                        stores.append((GRID + (tile + row * 228 + column) * 4,
                                       _u32(1)))
                stores.append((GRID + tile * 4, _u32(0)))
                for plane in NEIGHBORS:
                    stores.append((plane + tile * 4, _u32(1)))
            elif mode != 'invalid':
                # An explicit west hit is outside the square, and the actual
                # native predicate must still execute all four neighbor calls.
                stores.append((NEIGHBORS[2] + tile * 4, _u32(0)))


def _endpoints(special, blocked, phase):
    observed = [(TILE, 4), (ROTATION, 4), (ORIGIN, 4), (CURSOR, 4),
                (DX, 4), (DY, 4), (IMAGE, 4), (BLOCK_X, 4), (BLOCK_Y, 4)]
    if special:
        observed += [(SELF + 0x50, 4)]
    projection_mutations = []
    draw_mutations = []
    if phase == 'projection':
        # The record orientation, dx and dy were captured before projection.
        # Cursor/map globals and the switch rotation are read after it.
        projection_mutations = [
            (ROTATION, 4, 6), (DX, 4, -9), (DY, 4, 31),
            (ORIGIN, 4, 0x900), (CURSOR, 4, 0x900 + 2 * 228 + 9),
            (BLOCK_X, 4, 81), (BLOCK_Y, 4, -77), (IMAGE, 4, 9001)]
    elif phase == 'draw':
        # Repeated assignments happen after each render. The next cell must
        # reread live rotation, dy, image, occupancy coordinates and BC9768;
        # the next piece must recompute BC9768 using the changed map origin.
        draw_mutations = [
            (ROTATION, 4, 6), (DX, 4, -17), (DY, 4, 44),
            (ORIGIN, 4, 0x600), (CURSOR, 4, 0x600 + 228 + 21),
            (BLOCK_X, 4, -91), (BLOCK_Y, 4, 83),
            (IMAGE, 4, 90210), (TILE, 4, 4096)]
    return [
        dict(token='projection', native=0x4F0BA0,
             compiled_symbols=['FUN_004f0ba0'], arity=4,
             native_cleanup=0, compiled_cleanup=0, result_kind='void',
             pointer_args={2: 'screen_x', 3: 'screen_y'},
             writes_args=[(2, 4, 325), (3, 4, -147)],
             mutations=projection_mutations, observe=observed),
        dict(token='occupancy', native=0x46F9A0,
             compiled_symbols=['FUN_0046f9a0'], arity=3,
             native_cleanup=0, compiled_cleanup=0, result_kind='al8',
             value=0x80 if blocked else 0, pointer_args={}, writes_args=[],
             mutations=[], observe=observed),
        dict(token='render', native=0x53F4F0,
             compiled_symbols=['FUN_0053f4f0'], arity=4,
             native_cleanup=16, compiled_cleanup=0, result_kind='full32',
             value=37, native_receiver=0xF55BB0, pointer_args={},
             writes_args=[], mutations=draw_mutations, observe=observed),
    ]


def _case(entry, label, rotation, mode='valid', phase=None, count=None):
    if entry in FORWARDERS:
        offset, number, special = FORWARDERS[entry]
        assert count is None
        pieces = SELF + offset
        object_size = offset + number * STRIDE
        native_args, named_args = [], [SELF]
        cleanup = 0
    else:
        special = SHARED[entry]
        number = 3 if count is None else count
        pieces = PIECES
        object_size = 0x90
        native_args, named_args = [pieces, number], [SELF, pieces, number]
        cleanup = 8
    extent = max(number, 1) * STRIDE
    selected = (number - 1 if rotation in (2, 6) or phase else 0)
    active = [0, number - 1] if number > 1 else ([0] if number == 1 else [])
    stores = [(SELF - 16, b'\xA5' * (object_size + 32)),
              (SELF, bytes(object_size)),
              (VTABLE - 16, b'\xA5' * 64),
              (VTABLE + 16, _u32(0x547DE0 if special else 0x4E5310)),
              (SELF, _u32(VTABLE))]
    watched = [(SELF - 16, object_size + 32), (VTABLE - 16, 64)]
    if entry in SHARED:
        stores += [(pieces - 16, b'\xA5' * (extent + 32)),
                   (pieces, bytes(extent))]
        watched += [(pieces - 16, extent + 32)]
    # All unselected records, including the intermediate511 records of the
    # largest forwarder, remain side0. The loop must reach its real last entry.
    for index in active:
        last = index == number - 1
        stores.append((pieces + index * STRIDE,
                       _piece(8 if last else 2, 4 if last else 1, 2, last)))
    if special:
        stores.append((SELF + 0x50, _u32(selected)))
    globals_ = {ROTATION: rotation, ORIGIN: 0x400,
                CURSOR: 0x400 + 3 * 228 + 5, MAP_X: -1234, MAP_Y: 6789,
                BLOCK_X: 30, BLOCK_Y: -20, DX: 19, DY: 12,
                TILE: 0x6A5B4C3D, IMAGE: 713}
    # Neighboring DWORD canaries overlap; apply all of them before values.
    for address in globals_:
        stores.append((address - 4, b'\xA5' * 12))
        watched += [(address - 4, 12)]
    stores += [(address, _u32(value)) for address, value in globals_.items()]
    table_stores, table_watched = _tables()
    stores += table_stores
    watched += table_watched
    stores.append((GRID_START, _u32(1) * (GRID_SIZE // 4)))
    watched.append((GRID_START, GRID_SIZE))
    if phase == 'projection':
        _terrain(stores, 0x900, 9, 2, number - 1, selected, special, mode, active)
    else:
        _terrain(stores, 0x400, 5, 3, number - 1, selected, special, mode, active)
    if phase == 'draw':
        # Cursor was captured before the first renderer changes CURSOR.
        _terrain(stores, 0x600, 5, 3, number - 1, selected, special, mode, active)
        for row in range(2):
            for column in range(2):
                stores.append((GRID + (4096 + row * 228 + column) * 4,
                               _u32(0)))
    endpoints = _endpoints(special, mode == 'blocked', phase)
    mutations = [item for endpoint in endpoints for item in endpoint['mutations']]
    return dict(
        id=f'draw/{entry:08x}/{label}', entry=entry, receiver=SELF,
        arguments=native_args, named_arguments=named_args,
        eflags=0xAD7 if rotation in (2, 6) else 0x202,
        stores=stores, watched=watched,
        write_ranges=[(TILE, 4)] + [(address, width) for address, width, _ in mutations],
        result_mask=None, native_cleanup=cleanup, named_cleanup=0,
        canonical_cleanup=cleanup, canonical_registers_and_flags=False,
        endpoints=endpoints,
        expected_endpoint_counts={
            'projection': 1, 'occupancy': len(active) * 4,
            'render': 0 if mode == 'blocked' else len(active) * 4},
        features=dict(side=2, count=number, special=special, selected=selected if special else None,
                      active=active, initial_rotation=rotation, terrain=mode,
                      mutation_phase=phase, predicate=hex(0x547DE0 if special else 0x4E5310)))


def definitions(registry):
    present = {int(member['address'], 16) for member in registry['members']}
    assert set(ENTRIES) <= present, 'Missing draw cohort member'
    cases = []
    for entry in ENTRIES:
        for rotation in (0, 2, 4, 6):
            cases.append(_case(entry, f'rotation-{rotation}-valid', rotation))
        cases.append(_case(entry, 'invalid-terrain', 2, 'invalid'))
        cases.append(_case(entry, 'blocked-al80', 4, 'blocked'))
        cases.append(_case(entry, 'projection-mutates-globals', 0, phase='projection'))
        cases.append(_case(entry, 'renderer-mutates-globals', 2, phase='draw'))
        if (entry in FORWARDERS and FORWARDERS[entry][2]) or SHARED.get(entry):
            cases.append(_case(entry, 'selected-without-neighbor', 6, 'no-neighbor'))
    for entry in SHARED:
        cases.append(_case(entry, 'empty', 0, count=0))
        cases.append(_case(entry, 'negative-count', 4, count=-3))
    return cases
