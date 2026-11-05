"""Fresh pure gameplay recipes: importing does not load or execute an image.

Selected entry bodies and their connected helpers execute. Only declared external
rendering, message, cell-search, work-list, allocation and clock boundaries are
controlled. Native/canonical stack arguments and named C arguments are separate.
"""
import struct

SCRATCH = 0x32000000
SELF = SCRATCH + 0x1000
CALLBACK = 0x31000100
ACTORS, BUILDINGS = 0xBCD5F8, 0x7999D8
INCOMING_EAX = 0xA5B6C7D8
PIN = 'fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
ENTRIES = (0x462180, 0x4620C0, 0x41FFF0, 0x420050, 0x4206F0,
           0x420A30, 0x420D60, 0x421060, 0x421920, 0x4E5B20,
           0x4EFE30, 0x4F01E0, 0x46F200, 0x4A3B00, 0x4E7AC0,
           0x4E7C90, 0x52B6D0, 0x4728A0, 0x55D330, 0x55D460,
           0x55D6D0, 0x55D810, 0x55D840)
CANONICAL_OWNERS = {
    0x462180: 'city_updates.obj',
    0x41FFF0: 'widget_draw.obj', 0x420050: 'widget_draw.obj',
    0x4206F0: 'widget_draw.obj', 0x420A30: 'widget_draw.obj',
    0x420D60: 'widget_draw.obj', 0x421060: 'widget_draw.obj',
    0x421920: 'widget_draw.obj', 0x4E5B20: 'actor_laborer.obj',
    0x46F200: 'menu_building_overlay.obj',
    0x4E7AC0: 'menu_event_actions.obj', 0x4E7C90: 'menu_event_actions.obj',
    0x4728A0: 'map_commit_complete.obj'}


def _u32(v):
    return struct.pack('<I', v & 0xFFFFFFFF)


def _u16(v):
    return struct.pack('<H', v & 0xFFFF)


def _pattern(n):
    cycle = bytes((i * 37 + 19) & 255 for i in range(256))
    return cycle * (n // 256) + cycle[:n % 256]


def _case(entry, label, args=(), named=None, receiver=0x23456789,
          mask=None, cleanup=0, owner=None):
    c = dict(id=f'gameplay/{entry:08x}/{label}', entry=entry,
             receiver=receiver, arguments=list(args),
             named_arguments=list(args if named is None else named),
             eflags=0xAD7, stores=[], watched=[], write_ranges=[],
             result_mask=mask, native_cleanup=cleanup, named_cleanup=0,
             canonical_cleanup=cleanup, canonical_registers_and_flags=False,
             endpoints=[], budget=2000000)
    if owner or entry in CANONICAL_OWNERS:
        c['canonical_owner'] = owner or CANONICAL_OWNERS[entry]
    return c


def _region(c, address, size, fill=None, writable=False):
    c['stores'] += [(address - 16, _pattern(size + 32)),
                    (address, bytes(size) if fill is None else fill)]
    c['watched'].append((address - 16, size + 32))
    if writable:
        c['write_ranges'].append((address, size))


def _word(c, address, value, width=4, writable=False):
    c['stores'].append((address, (value & ((1 << (width * 8)) - 1)).to_bytes(width, 'little')))
    c['watched'].append((address, width))
    if writable:
        c['write_ranges'].append((address, width))


def _ep(token, native, compiled, arity, kind='void', value=None,
        native_cleanup=0, compiled_cleanup=0, **extra):
    e = dict(token=token, native=native, compiled_symbols=list(compiled),
             arity=arity, native_cleanup=native_cleanup,
             compiled_cleanup=compiled_cleanup, result_kind=kind,
             pointer_args={}, writes_args=[], mutations=[], observe=[])
    if kind != 'void':
        assert value is not None
        e['value'] = value
    e.update(extra)
    return e


def _render(observe=()):
    # Native thiscall3/RET12; ordinary C explicitly adds the receiver.
    return _ep('render', 0x53E590, ['restore_sprite_draw'], 3,
               'full32', 1, native_cleanup=12,
               compiled_arity=4, compiled_receiver_arg=0,
               native_receiver=0xF55BB0, observe=list(observe))


def _building(c, index, kind=0, state=1):
    b = BUILDINGS + index * 264
    c['stores'] += [(b, bytes([state])), (b + 16, _u16(kind))]
    return b


def _actor(c, index):
    a = ACTORS + index * 388
    _region(c, a, 388, _pattern(388), True)
    return a


def _building_table(c, writable=False):
    _region(c, BUILDINGS, 4000 * 264, writable=writable)


def _profile_cases():
    out = []
    for actor in (0, 1, 7, 1999):
        for kind in (0, 127):
            c = _case(0x4620C0, f'actor{actor}-kind{kind}', [actor])
            a = _actor(c, actor)
            c['stores'].append((a + 10, bytes([kind])))
            table = 0x7090D0 + kind * 56
            _region(c, table, 56, _pattern(56))
            c['stores'] += [(table, _u32(0x9137AC42)),
                            (table + 28, _u32(0x7F21B3D5))]
            out.append(c)
    return out


WIDGETS = {0x41FFF0: (0x5C2750, 22), 0x420050: (0x596088, 13),
           0x4206F0: (0x59EDF0, 27), 0x420A30: (0x5A8148, 56),
           0x420D60: (0x5AD218, 27), 0x421060: (0x5B0060, 15)}
BUILDING_WIDGET_TABLES = {
    0: 0x5BE600, 1: 0x5BE510, 29: 0x5BE510, 2: 0x5BE540,
    3: 0x5BE570, 4: 0x5BE750, 5: 0x5BE780, 6: 0x5BE630,
    7: 0x5BE7B0, 8: 0x5BE7E0, 9: 0x5BE660, 10: 0x5BE690,
    11: 0x5BE8A0, 12: 0x5BE810, 13: 0x5BE8D0, 14: 0x5BE900,
    15: 0x5BE5A0, 16: 0x5BE5D0, 19: 0x5BE6C0, 20: 0x5BE6F0,
    21: 0x5BE720, 22: 0x5BE840, 23: 0x5BE870, 24: 0x5BE930,
    25: 0x5BE960, 26: 0x5BE990}


def _widget_fixture(c, table, count, orientation, visible=True, building=False):
    coords, libraries, animations = SCRATCH + 0x21000, SCRATCH + 0x22000, SCRATCH + 0x23000
    objects = SCRATCH + 0x10000
    _region(c, table - 8, count * 16 + 8, _pattern(count * 16 + 8))
    _region(c, objects, count * 32, _pattern(count * 32))
    _region(c, 0x5BE9B8, 40, _pattern(40))
    _region(c, coords, 64, _pattern(64))
    _region(c, libraries, 16, _pattern(16))
    _region(c, animations, 48, _pattern(48))
    # Real 41F910/41FAD0 execute using the group0 descriptor and two points.
    c['stores'] += [(0x5BE9B8, b''.join(_u32(v) for v in
                     [coords, 2, libraries, 1, animations, 1, 0, 0, -11, 13])),
                    (libraries, b''.join(_u32(v) for v in [2, 21, 3, 7])),
                    (animations, b''.join(_u32(v) for v in
                     [0, 1, 0, 12, 3, 4, 5, 6, 7, 8, 9, 10]))]
    for point in range(2):
        for rot in range(4):
            c['stores'].append((coords + (point * 4 + rot) * 8,
                               _u32(-17 + point * 23 + rot * 3) +
                               _u32(29 - point * 19 - rot * 5)))
    for i in range(count):
        obj = objects + i * 32
        c['stores'] += [(table + i * 16, _u32(obj)),
                        (obj, _u32(0) + _u32(0) + _u32(6)),
                        (obj + 16, _u16(0 if visible and i in (0, count - 1) else 3)),
                        (obj + 21, bytes([i & 1]))]
        if building:
            c['stores'].append((table - 8 + i * 16, _u32(81 if i != 1 else 82)))
    _word(c, 0xBCA2E0, orientation)
    _word(c, 0xF55BB4 + 2 * 8, 20)
    _word(c, 0x126517C, 200)
    _region(c, 0x10D6A00, 200 * 64, _pattern(200 * 64))
    c['endpoints'].append(_render([(0xBCA2E0, 4)]))


def _widget_cases():
    out = []
    for entry, (table, count) in WIDGETS.items():
        for orientation, visible in ((0, True), (2, True), (4, True), (6, True), (0, False)):
            c = _case(entry, f'rotation{orientation}-visible{int(visible)}', [117, -93])
            _widget_fixture(c, table, count, orientation, visible)
            if entry == 0x4206F0:
                _word(c, 0x704DC0, 2 if orientation in (0, 4) else 0, writable=True)
                _region(c, 0x596150, 0xDE0, _pattern(0xDE0), True)
                _region(c, 0x596F30, 0xDE0, _pattern(0xDE0))
                _region(c, 0x597D10, 0xDE0, _pattern(0xDE0)[::-1])
            out.append(c)
    for kind, table in BUILDING_WIDGET_TABLES.items():
        c = _case(0x421920, f'kind{kind}', [117, -93, 7])
        b = _building(c, 7)
        _region(c, b, 264, _pattern(264))
        c['stores'] += [(b + 104, _u32(kind)), (b + 96, _u32(81)),
                        (b + 144, _u32(3))]
        _widget_fixture(c, table, 3, 2, building=True)
        out.append(c)
    for kind in (17, 18, 0x80000001):
        c = _case(0x421920, f'default{kind:x}', [117, -93, 7])
        b = BUILDINGS + 7 * 264
        _region(c, b, 264, _pattern(264))
        c['stores'].append((b + 104, _u32(kind)))
        out.append(c)
    return out


def _city_cases():
    out = []
    for delay, count, actor_links, latch in ((-3, 0, 0, False), (0, 0, 0, False),
                                             (1, 0, 0, False), (37, 4, 0, False),
                                             (9, 4, 5, False), (9, 4, 5, True)):
        c = _case(0x462180, f'delay{delay}-buildings{count}-actors{actor_links}-latch{int(latch)}')
        _region(c, BUILDINGS, 6 * 264, _pattern(6 * 264), True)
        # All2000 actor activity bytes are controlled; only first5 may be active.
        _region(c, ACTORS, 2001 * 388, writable=True)
        for i in range(1, 6):
            b = _building(c, i, (0xB1, 0xB2, 0xB3, 0xE7, 0x26)[i - 1], 0)
            c['stores'].append((b + 166, b'\x00'))
        # Kind26 traverses the main active default case and reset store.
        c['stores'] += [(BUILDINGS + 4 * 264, b'\x01'),
                        (BUILDINGS + 4 * 264 + 16, _u16(0x26))]
        for i in range(actor_links):
            a = ACTORS + (i + 1) * 388
            c['stores'] += [(a + 10, bytes([0x4F])), (a + 14, b'\x01'),
                            (a + 90, _u16(1)), (a + 0x120, _u16(0x7F82))]
        _word(c, 0x78D790, count)
        _word(c, 0xBCB9AC, delay, writable=True)
        _word(c, 0x78D584, 0x76543210, writable=True)
        _word(c, 0xBCA348, 0xA5, 1, True)
        _word(c, 0xEAAC28, 1)
        _word(c, 0xEA6204 + 0x49D8, 0x7145AC32, writable=True)
        clock = _ep('clock', 0, [], 0, 'full32', 0x81234567,
                    import_name='timeGetTime')
        if latch:
            clock['mutations'] = [(0xBCA348, 1, 1)]
        c['endpoints'].append(clock)
        c['expected_endpoint_counts'] = {'clock': 4}
        out.append(c)
    return out


def _obelisk_cases():
    out = []
    for progress, increment in ((100, 7), (0, 4), (93, 7), (94, 7),
                                (99, 4), (101, 4), (250, 7), (255, 4)):
        c = _case(0x4E5B20, f'progress{progress}-increment{increment}', [7], mask=0xFF)
        b = BUILDINGS + 7 * 264
        _region(c, b, 264, _pattern(264), True)
        c['stores'] += [(b + 85, bytes([progress])), (b + 6, _u16(-23)),
                        (b + 8, _u16(37))]
        c['endpoints'] += [
            _ep('increment', 0x4E5AE0, ['FUN_004e5ae0'], 1, 'full32', increment),
            _ep('refresh', 0x4E5840, ['restore_obelisk_refresh_progress'], 3,
                observe=[(b + 85, 1)])]
        c['expected_endpoint_counts'] = {'increment': int(progress != 100),
                                         'refresh': int(progress != 100)}
        out.append(c)
    return out


def _work_fixture(c, actor=7):
    _building_table(c)
    a = _actor(c, actor)
    root = _building(c, 1, 183)
    c['stores'] += [(root + 84, b'\x00'), (root + 85, b'\x02'),
                    (root + 200, _u16(1))]
    _word(c, 0x56F7D8, 4000)
    _word(c, 0x56F7EC, 4000)
    _word(c, 0x5E4EF4, 1)
    _region(c, 0xEAAC44, 16, _pattern(16), True)
    _word(c, 0xEAAF58, 0, writable=True)
    _region(c, 0x5F3878, 3 * 20, _pattern(3 * 20))
    _word(c, 0x5F5498, 1)
    target = BUILDINGS + 2 * 264
    c['stores'] += [(target + 6, _u16(-17)), (target + 8, _u16(29)),
                    (target + 38, _u16(43)), (target + 40, _u16(-31)),
                    (target + 12, _u32(0x1735))]
    c['write_ranges'] += [(target + 100, 2), (BUILDINGS + 3 * 264 + 100, 2)]
    c['stores'] += [(target + 100, _u16(0)),
                    (BUILDINGS + 3 * 264 + 100, _u16(0))]
    return a, root, target


def _pyramid_cases():
    out = []
    for family, mode, pending, available, claim, active, pieces in (
            (1, 0, 0, 0, 0, 0, 1), (2, 0, 1, 0, 0, 0, 1),
            (2, 0, 0, 0, 0, 0, 1), (2, 1, 0, 0, 0, 0, 1),
            (2, 1, 1, 0, 0, 0, 1), (3, 0, 0, 0, 91, 0, 1),
            (4, 0, 0, 0, 91, 1, 1), (5, 0, 0, 0, 0, 0, 1),
            (1, 0, 0, 1, 0, 0, 2), (1, 0, 0, 0, 0, 0, 0)):
        label = f'f{family}-m{mode}-pending{pending}-available{available}-claim{claim}-active{active}-n{pieces}'
        c = _case(0x4EFE30, label, [0x12345678, 0x9ABCDEF0, 7, mode], [7, mode], mask=0xFFFFFFFF,
                  owner='actor_laborer.obj')
        a, root, target = _work_fixture(c)
        c['stores'] += [(0x5E4EF4, _u32(family)), (target + 100, _u16(claim)),
                        (0x5F3878, _u32(1))]
        collect = _ep('collect', 0x4EF450, ['restore_pyramid_collect_work_pieces'], 1)
        collect['mutations'] = [(0xEAAF58, 4, pieces), (0xEAAC44, 4, 2), (0xEAAC48, 4, 3)]
        c['endpoints'] += [collect,
            _ep('available', 0x4EF100, ['restore_pyramid_available_worker_slot'], 1, 'full32', available),
            _ep('claim-active', 0x4EEA10, ['restore_pyramid_worker_claim_active'], 1, 'al8', active),
            _ep('pending', 0x4EFD90, ['restore_pyramid_piece_has_pending_work'], 1, 'al8', pending)]
        out.append(c)
    # Access-only branch: real signed delta division and real actor conflict scan.
    for chosen, conflict in ((0, False), (0x1300, False), (0x1300, True)):
        c = _case(0x4EFE30, f'access{chosen:x}-conflict{int(conflict)}',
                  [0x12345678, 0x9ABCDEF0, 7, 1], [7, 1], mask=0xFFFFFFFF,
                  owner='actor_laborer.obj')
        a, root, target = _work_fixture(c)
        # Full native conflict-scan input. Restore the current actor's patterned
        # record after clearing all unrelated actors' activity/kind fields.
        c['stores'].append((ACTORS, bytes(2001 * 388)))
        c['stores'] += [(a, _pattern(388)), (a + 10, b'\x63'),
                        (root + 200, _u16(0)), (root + 85, b'\x01')]
        _word(c, 0xBCA2D8, 0x1000)
        _word(c, 0x5F5D08, 19)
        _word(c, 0xEAAE90, 0, writable=True)
        if conflict:
            other = ACTORS + 3 * 388
            c['stores'] += [(other + 10, b'\x63'), (other + 14, b'\x01'),
                            (other + 40, _u32(chosen)), (other + 94, _u16(1))]
        c['endpoints'] += [_ep('access-kind', 0x4ECEA0, ['FUN_004ecea0'], 1, 'al8', 1),
                           _ep('brush', 0x4ED360, ['FUN_004ed360'], 4,
                               mutations=[(0xEAAE90, 4, chosen)])]
        out.append(c)
    for family, available, claim, active, count in ((5, 1, 0, 0, 2), (1, 0, 0, 0, 0),
                                                   (1, 0, 0, 0, 2), (1, 1, 0, 0, 2),
                                                   (1, 1, 91, 0, 2), (1, 1, 91, 1, 2)):
        c = _case(0x4F01E0, f'f{family}-available{available}-claim{claim}-active{active}-n{count}',
                  [0x12345678, 0x9ABCDEF0, 7], [7], mask=0xFFFFFFFF,
                  owner='actor_laborer.obj')
        a, root, target = _work_fixture(c)
        _word(c, 0x570070, count)
        c['stores'] += [(0x5E4EF4, _u32(family)), (target + 100, _u16(claim)),
                        (0x5F3878, _u32(1)), (0x5F388C, _u32(2))]
        c['endpoints'] += [
            _ep('available', 0x4EF100, ['restore_pyramid_available_worker_slot'], 1, 'full32', available),
            _ep('claim-active', 0x4EEA10, ['restore_pyramid_worker_claim_active'], 1, 'al8', active),
            _ep('publish-target', 0x4F3000, ['restore_monument_worker_publish_target_eax'], 2,
                'full32', 0x1735, observe=[(target + 100, 2)])]
        out.append(c)
    return out


def _cadence_cases():
    out = []
    # Divisible wrapped counters and genuine nested step->4B3F00 execution.
    for counter, period in ((-32768, 3), (-1, 2), (0, 3), (1, 2),
                            (99, 2), (100, 101), (101, 2), (32767, 2),
                            (0, -2147483648), (0, 0), (0, 1)):
        for phase in (0, 6):
            c = _case(0x4A3B00, f'clock{counter}-period{period}-phase{phase}',
                      [7, period], [7, period, INCOMING_EAX], mask=0xFFFFFFFF,
                      owner='actor_cadence_facade.obj')
            a = _actor(c, 7)
            c['stores'] += [(a + 0x36, _u16(counter)), (a + 0x39, bytes([phase])),
                            (a + 0x11, b'\x02'), (a + 0x77, b'\x02'),
                            (a + 0x78, b'\x03'), (a + 0x79, b'\x05')]
            _word(c, 0xBCD5E8, 0x73246185, writable=True)
            out.append(c)
    return out


def _cell_search(value):
    return _ep('cell-search', 0x47FE60, ['FUN_0047fe60'], 4, 'full32', value,
               mutations=[(0xD3B4E4, 4, 11), (0xD3B4E0, 4, 17)])


def _patrol_cases():
    out = []
    for kind in (0x2E, 0x2F, 0x3B, 0x3C, 0x40, 0x41, 0xB7,
                 0xDE, 0xE5, 0xEA, 0xEB, 0xEC, 0xFFFF):
        for result in (0, 0x1234):
            c = _case(0x4E7C90, f'kind{kind:x}-cell{result:x}', [9], mask=0xFFFFFFFF)
            b = BUILDINGS + 9 * 264
            _region(c, b, 264, _pattern(264))
            c['stores'] += [(b + 16, _u16(kind)), (b + 38, _u16(-23)),
                            (b + 40, _u16(37))]
            _word(c, 0xD3B4E4, 0x81357246, writable=True)
            _word(c, 0xD3B4E0, 0x92713465, writable=True)
            c['endpoints'].append(_cell_search(result))
            out.append(c)
    for count, kind, result, ring in ((-1, 0, 0, 0), (0, 0, 0, 0),
                                      (3, 0x2E, 0x1234, 0), (3, 0x3C, 0, 0),
                                      (3, 0x3C, 0x1234, 0), (3, 0x3C, 0x1234, 99)):
        c = _case(0x4E7AC0, f'count{count}-kind{kind:x}-cell{result:x}-ring{ring}',
                  [-157], mask=0xFFFFFFFF)
        _region(c, BUILDINGS, 5 * 264, writable=True)
        for i in range(1, 4):
            b = _building(c, i, kind)
            c['stores'] += [(b + 6, _u16(21 + i)), (b + 8, _u16(35 + i)),
                            (b + 38, _u16(11 + i)), (b + 40, _u16(17 + i))]
        a = _actor(c, 5)
        c['stores'].append((a, bytes(388)))
        _word(c, 0x78D790, count)
        _word(c, 0x78D798, 0x87314625, writable=True)
        _region(c, 0xD3B1C4, 0x1D0, _pattern(0x1D0), True)
        c['stores'] += [(0xD3B1C4, _u32(ring)), (0xD3B374, _u32(0x73)),
                        (0xD3B380, _u32(0x4A316B27)), (0xD3B384, _u32(0x5721C49B))]
        _word(c, 0xD3B4E4, 31, writable=True)
        _word(c, 0xD3B4E0, 27, writable=True)
        _word(c, 0xBCA2D0, 200)
        _word(c, 0xBCA2D4, 180)
        _word(c, 0xEAAC28, 1)
        c['endpoints'] += [_cell_search(result),
                           _ep('create-actor', 0x4B4800, ['FUN_004b4800'], 5, 'full32', 5)]
        # Both scans use the actual selected4E7C90, and the successful branch
        # runs the actual existing4B1DF0 service-patrol body.
        out.append(c)
    return out


COMPLETION_QUERY = {
    0xD5: (0x542AD0, 'FUN_00542ad0', None),
    0xDE: (0x4DF040, 'FUN_004df040', None),
    0xE1: (0x40D940, 'restore_monument_base_0040d940', 0x6FD5D8),
    0xE3: (0x417050, 'restore_monument_base_00417050', 0x6FD368),
    0xE4: (0x4E6710, 'restore_monument_base_004e6710', 0x6DE548),
    0xE5: (0x545740, 'restore_monument_progress_00545740', 0x6F9320),
    0xE6: (0x401540, 'FUN_00401540', None),
    0xEA: (0x558BE0, 'restore_monument_progress_00558be0', 0x6F5648),
    0xEB: (0x555E80, 'restore_monument_progress_00555e80', 0x6DE9E0),
    0xEC: (0x54F470, 'restore_monument_progress_0054f470', 0x6E9188)}


def _completion_cases():
    out = []
    choices = [(0xCF, 99, 0, 1), (0xCF, 100, 0, 1), (0xD0, 100, 1, 1),
               (0xD2, 15, 0, 1), (0xD2, 16, 0, 1), (0xD2, 16, 1, 1)]
    choices += [(0xB7, 100, 0, family) for family in (0, 1, 2, 3, 4, 5)]
    choices += [(kind, progress, announced, 1) for kind in COMPLETION_QUERY
                for progress, announced in ((99, 0), (100, 0), (100, 1))]
    for kind, progress, announced, family in choices:
        c = _case(0x46F200, f'kind{kind:x}-progress{progress}-announced{announced}-family{family}')
        _building_table(c, True)
        b = _building(c, 1, kind)
        c['stores'] += [(b + 85, bytes([progress])), (b + 128, bytes([announced])),
                        (b + 104, bytes([announced])), (b + 116, bytes([announced])),
                        (b + 157, bytes([announced])), (b + 96, bytes([announced])),
                        (b + 22, _u16(-1))]
        _word(c, 0x5E4EF4, family)
        _word(c, 0x56F7D8, 4000)
        _word(c, 0x56F7EC, 4000)
        _word(c, 0xEAAC28, 1)
        _word(c, 0xEA5BAA + 0x49D8, progress, 2)
        _word(c, 0xEA41F8 + 0x49D8, announced, writable=True)
        _word(c, 0xE950DC, 0x37246185, writable=True)
        c['endpoints'].append(_ep('publish-message', 0x4E0CF0, ['FUN_004e0cf0'], 5,
            private_input_args={3: dict(role='message_parameters', width=6)},
            observe=[(0xE950DC, 4), (b + 128, 1), (b + 104, 1), (b + 157, 1)]))
        c['endpoints'] += [_ep('reset-selection', 0x547A60, ['restore_remaining_selection_00547a60'], 0),
                           _ep('reset-overlay', 0x4DEA50, ['FUN_004dea50'], 0)]
        if kind == 0xB7:
            c['endpoints'].append(_ep('pyramid-complete', 0x4F6020, ['FUN_004f6020'], 1, 'al8', 1))
        if kind in COMPLETION_QUERY:
            target, compiled, receiver = COMPLETION_QUERY[kind]
            e = _ep('progress', target, [compiled], 2, 'full32', progress,
                    native_cleanup=8 if receiver is not None or target == 0x401540 else 0)
            if receiver is not None:
                e.update(native_receiver=receiver, compiled_arity=3, compiled_receiver_arg=0)
            c['endpoints'].append(e)
        out.append(c)
    return out


def _storage_cases():
    out = []
    # Resource scan with zero and three visible resources, then absent/active
    # slots through simple WORD/BYTE progress families. All renderer arguments
    # and all numeric/text arguments are compared as raw scalar DWORDs.
    for resources, selected_type, present, progress in ((False, 0, 0, 0),
            (True, 0, 0, 0), (True, 1, 0, 0), (True, 1, 1, 8),
            (False, 1, 1, 16), (False, 2, 0, 100), (False, 2, 1, 99),
            (False, 2, 1, 100), (False, 3, 0, 0), (False, 5, 0, 0)):
        c = _case(0x52B6D0, f'resources{int(resources)}-type{selected_type}-present{present}-progress{progress}')
        _word(c, 0xEAAC28, 1)
        _region(c, 0x784DE0, 6)
        _region(c, 0x784DE8, 0x120)
        _region(c, 0xEA6160 + 0x49D8, 36 * 4)
        _region(c, 0xEA5FA0 + 0x49D8, 3 * 4)
        _region(c, 0xEA5FAC + 0x49D8, 3 * 4)
        _region(c, 0x5D19D8, 16)
        if selected_type:
            c['stores'] += [(0x784DE0, _u16(1)), (0x784DE2, _u16(-1)),
                            (0x5D19D8, _u16(0) + _u16(selected_type)),
                            (0xEA5FA0 + 0x49D8, _u32(present)),
                            (0xEA5FAC + 0x49D8, _u32(7))]
        if resources:
            for r, total, committed, required in ((1, 3, 1, 270), (24, 2, 2, 100), (28, 3, 1, 299)):
                c['stores'] += [(0x784DE8 + r * 4, _u32(total)),
                                (0x784E78 + r * 4, _u32(committed)),
                                (0xEA6160 + 0x49D8 + r * 4, _u32(required))]
        _word(c, 0xEA5BAA + 0x49D8, progress, 2)
        _word(c, BUILDINGS + 7 * 264 + 85, progress, 1)
        _word(c, 0xEA61C8 + 0x49D8, 100)
        _word(c, 0xEA61D8 + 0x49D8, 3700)
        for address, value in ((0xF53F0C, 117), (0xF53F08, -93),
            (0xE38DA8, 17), (0xE38DA4, -19), (0xF56F9C, 0x7314),
            (0xF571FC, 0x391), (0x7056C8, 6)):
            _word(c, address, value)
        _word(c, 0xE20778, 0x7318AC42, writable=True)
        _word(c, 0xE20774, 0x92714365, writable=True)
        c['endpoints'] += [_ep('resources-visible', 0x40CC40, ['FUN_0040cc40'], 1, 'full32', int(resources)),
            _render(), _ep('row-highlight', 0x4CD1A0, ['FUN_004cd1a0'], 5),
            _ep('resource-text', 0x4D62F0, ['FUN_004d62f0'], 8,
                mutations=[(0xE20778, 4, 23)]),
            _ep('resource-number', 0x4D6860, ['FUN_004d6860'], 9,
                mutations=[(0xE20778, 4, 37)]),
            _ep('section-title', 0x4D65B0, ['FUN_004d65b0'], 6),
            _ep('selected-text', 0x4D6200, ['FUN_004d6200'], 9,
                mutations=[(0xE20778, 4, 41)]),
            _ep('selected-number', 0x4CD290, ['FUN_004cd290'], 8,
                mutations=[(0xE20778, 4, 43)]),
            _ep('obelisk-cost', 0x4F52B0, ['FUN_004f52b0'], 1, 'full32', 0xCF)]
        out.append(c)
    return out


def _map_cases():
    out = []
    for tool, debt, blocked in ((0, 0, 0), (7, -5000, 0), (7, -5001, 0),
                                 (7, 0, 1), (0x52, 0, 1), (6, 0, 1),
                                 (0x26, 0, 1), (0xCF, 0, 0)):
        c = _case(0x4728A0, f'tool{tool:x}-debt{debt}-blocked{blocked}',
                  [1, 6, -17, 23, 31, -37, tool])
        for address, value in ((0xEAAC28, 1), (0xEA187C + 0x49D8, debt),
                               (0xE38EC1, 0)):
            _word(c, address, value, 1 if address == 0xE38EC1 else 4)
        for address in (0xE94E20, 0xBC9728, 0xBC9718, 0xBC96F8):
            _word(c, address, 0x73214685, writable=True)
        c['endpoints'] += [_ep('floodplain', 0x46FDE0, ['FUN_0046fde0'], 1, 'al8', 0),
            _ep('invalid-drag', 0x4B5830, ['FUN_004b5830'], 4, 'full32', blocked),
            _ep('clear-preview', 0x40F920, ['FUN_0040f920'], 3),
            _ep('restore-map', 0x40F860, ['FUN_0040f860'], 3),
            _ep('restore-map-refresh', 0x485830, ['FUN_00485830'], 0),
            _ep('existing-obelisk', 0x4E59C0, ['FUN_004e59c0'], 0, 'full32', 1)]
        # Actual4E2970 runs its documented disabled-message early exit.
        out.append(c)
    return out


def _sprite_fixture(c, mode=0, frame=0x3F800000, directions=4,
                    mirrored=False, source=True, data=True, angle=0x42B40000):
    src, dat, descriptors = SCRATCH + 0x2000, SCRATCH + 0x3000, SCRATCH + 0x4000
    variants, geometry, vtable = SCRATCH + 0x5000, SCRATCH + 0x6000, SCRATCH + 0x100
    _region(c, SELF, 0x80, _pattern(0x80), True)
    for address, size in ((src, 16), (dat, 0x20), (descriptors, 48),
                          (variants, 3 * 64), (geometry, 3 * 64)):
        _region(c, address, size, _pattern(size))
    c['stores'] += [(SELF + 4, _u32(vtable)), (SELF + 0x34, _u32(0xA731E9F7)),
                    (SELF + 0x38, _u32(src if source else 0)),
                    (src + 8, _u32(dat if data else 0)),
                    (dat + 0x1A, _u16(3)), (dat + 0x1C, _u32(descriptors)),
                    (SELF + 0x40, _u32(descriptors)), (SELF + 0x44, _u32(variants)),
                    (SELF + 0x48, _u32(frame)), (SELF + 0x4C, _u32(0x3F800000)),
                    (SELF + 0x50, _u32(0x3F800000)), (SELF + 0x78, _u16(1))]
    for action in range(3):
        d = descriptors + action * 16
        c['stores'] += [(d, bytes([mode if action == 0 else 4, 0x80 if mirrored else 0,
                                  7 + action, 1 + action, (action + 1) % 3,
                                  0x73, 0x51, 0x92, 0xB4, 0xC6, 0xD8, directions]) +
                            _u32(variants + action * 64))]
        for v in range(8):
            c['stores'] += [(variants + action * 64 + v * 8,
                             _u16(0x7143) + _u16(4 + action) + _u32(geometry + action * 64 + v * 8)),
                            (geometry + action * 64 + v * 8,
                             _u16(0x9271) + _u16(23 + v) + _u16(19 + action) + _u16(0x7314))]
    # Do not watch a per-image relocated function pointer as physical data.
    c['stores'].append((vtable + 4, _pattern(28)))
    c['watched'].append((vtable + 4, 28))
    c['function_pointers'] = [dict(address=vtable, native=CALLBACK, compiled_symbol='FUN_00561a1d')]
    c['endpoints'].append(_ep('angle-callback', CALLBACK, ['FUN_00561a1d'], 0,
        'x87_float32', angle, native_receiver=SELF + 4,
        registers=['eax', 'ecx', 'edx', 'esi', 'edi']))
    _word(c, 0x6044B0, 0)
    return descriptors


def _sprite_cases():
    out = []
    for action, frame in ((0, 0), (1, 0), (2, 1000), (0x12340002, -1),
                           (0x10003, 1000), (2, 99)):
        c = _case(0x55D330, f'action{action:x}-frame{frame}',
                  [action, 0x92713465, frame], [SELF, action, frame, 0],
                  receiver=SELF, cleanup=12, owner='sprite_controller_entries.obj')
        _sprite_fixture(c)
        out.append(c)
    c = _case(0x55D330, 'null-source', [2, 0x92713465, 1000], [SELF, 2, 1000, 0],
              receiver=SELF, cleanup=12, owner='sprite_controller_entries.obj')
    _sprite_fixture(c, source=False)
    out.append(c)
    for action in (0, 1, 2, 0x12340003):
        c = _case(0x55D460, f'action{action:x}', [action], [SELF, action],
                  receiver=SELF, cleanup=4, owner='sprite_controller_entries.obj')
        _sprite_fixture(c, mirrored=True, angle=0x43870000)
        out.append(c)
    for mode in range(8):
        for frame in (0x3F800000, 0x40800000):
            c = _case(0x55D6D0, f'mode{mode}-frame{frame:x}', [], [SELF], receiver=SELF,
                      mask=0xFFFFFFFF, owner='sprite_controller_entries.obj')
            _sprite_fixture(c, mode=mode, frame=frame)
            out.append(c)
    for frame in (0x3F800000, 0x40800000):
        c = _case(0x55D810, f'frame{frame:x}', [], [SELF], receiver=SELF,
                  mask=0xFFFFFFFF, owner='sprite_controller_entries.obj')
        _sprite_fixture(c, mode=3, frame=frame)
        out.append(c)
    for source, data, directions, mirrored, angle in (
            (False, False, 4, False, 0), (True, False, 4, False, 0),
            (True, True, 1, False, 0), (True, True, 1, False, 0x43340000),
            (True, True, 4, False, 0x42B40000), (True, True, 4, False, 0xC2B40000),
            (True, True, 4, True, 0x43870000), (True, True, 4, True, 0x44250000)):
        c = _case(0x55D840, f'source{int(source)}-data{int(data)}-directions{directions}-mirror{int(mirrored)}-angle{angle:x}',
                  [], [SELF], receiver=SELF, mask=0xFFFF, owner='sprite_controller_entries.obj')
        _sprite_fixture(c, source=source, data=data, directions=directions,
                        mirrored=mirrored, angle=angle)
        out.append(c)
    return out


def definitions(registry):
    present = {int(m['address'], 16) for m in registry['members']}
    missing = set(ENTRIES) - present
    assert not missing, ('gameplay registry entries missing', sorted(missing))
    out = []
    for family in (_profile_cases, _widget_cases, _city_cases, _obelisk_cases,
                   _pyramid_cases, _cadence_cases, _patrol_cases,
                   _completion_cases, _storage_cases, _map_cases, _sprite_cases):
        out.extend(family())
    assert {c['entry'] for c in out} == set(ENTRIES)
    assert len({c['id'] for c in out}) == len(out)
    return out
