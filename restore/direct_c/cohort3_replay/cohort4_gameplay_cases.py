"""Cohort4 finite gameplay cases; pure recipes, never launches a host game.

The cached pinned instructions and cohort4 source/actor-widget/completion
contracts determine result widths. Full volatile state is compared only for
the three compound templates which promise its reconstruction. Placement
predicates execute at real native vtable tokens; the C dispatch reads those
same tokens. The sole placement endpoint is the external border predicate.

Scope cases supply the actual parent EBP and execute the real nonthrowing
array destructor/empty callbacks. Trade-progress search and month-estimate
boundaries are controlled independently; route-month cases execute the real
lookup, scale and enabled/count logic. x87 route samples execute real code.
Ordinary nonfaulting memory, finite inputs and nonthrowing callbacks only.
Indirect consumers, exceptional unwinds, asynchronous memory changes and the
optional compound C output-pointer slots remain unproved. No equivalence of
the whole game or of incidental actor/widget volatile flags is asserted.
"""
import struct

from gameplay_cases import _ep, _pattern, _region, _u16, _u32, _word

SCRATCH = 0x32000000
SELF, OUT, VTABLE = SCRATCH + 0x1000, SCRATCH + 0x18000, SCRATCH + 0x200
FRAME = 0x30009000
LIVE_EAX = 0xA5B6C7D8
BUILDINGS, ACTORS = 0x7999D8, 0xBCD5F8

# check, existing, piece offset/count, kind, real predicate token.
CONTEXTS = (
    (0x401400, 0x401430, 0x4C, 16, 0xE6, 0x401440),
    (0x40D850, 0x40D880, 0x4C, 25, 0xE1, 0x4E5310),
    (0x416F70, 0x416FA0, 0x4C, 9, 0xE3, 0x4E5310),
    (0x4E65F0, 0x4E6650, 0x4C, 2, 0xE4, 0x4E6620),
    (0x5456C0, 0x545700, 0x1E5C, 167, 0xE5, 0x547DE0),
    (0x54F3F0, 0x54F430, 0x5C88, 513, 0xEC, 0x547DE0),
    (0x555E00, 0x555E40, 0x4EE0, 437, 0xEB, 0x547DE0),
    (0x558B60, 0x558BA0, 0x1CBC, 158, 0xEA, 0x547DE0),
)
SCOPES = (
    (0x56B3E8, 0x4C, 16, 'unit_03031.obj'),
    (0x56B4B8, 0x4C, 25, 'unit_03050.obj'),
    (0x56B5C8, 0x4C, 9, 'unit_03078.obj'),
    (0x56B6D8, 0x4C, 2, 'unit_03094.obj'),
    (0x56B818, 0x1E5C, 167, 'unit_03105.obj'),
    (0x56BDA8, 0x5C88, 513, 'unit_03275.obj'),
    (0x56CE08, 0x4EE0, 437, 'unit_03791.obj'),
    (0x56DC08, 0x1CBC, 158, 'unit_04231.obj'),
)
OWNERS = {
    0x4454A0: 'unit_00651.obj', 0x449390: 'unit_00693.obj',
    0x449750: 'unit_00713.obj', 0x44C110: 'unit_00729.obj',
    0x44C9A0: 'unit_00738.obj',
    0x56B6B0: 'unit_03092.obj', 0x56E1AA: 'unit_04396.obj',
}
ENTRIES = tuple([0x41B9D0, 0x4477D0, 0x560734, 0x4E5330, 0x5463E0,
                 0x4E5790, 0x547BA0, 0x56B6B0, 0x56E1AA,
                 0x4E52D0, 0x4E5310, 0x401440, 0x547DE0, 0x4E6620,
                 0x4493B0, 0x4493D0, 0x4493E0,
                 0x41F910, 0x41FAD0, 0x4454A0, 0x445D10, 0x448960,
                 0x449390, 0x449750, 0x44C110, 0x44C780, 0x44C9A0,
                 0x56B650] +
                [a for row in CONTEXTS for a in row[:2]] +
                [row[0] for row in SCOPES])


def _case(entry, label, args=(), named=None, receiver=SELF, mask=0xFFFFFFFF,
          cleanup=0, owner=None):
    c = dict(id=f'cohort4-gameplay/{entry:08x}/{label}', entry=entry,
             receiver=receiver, initial_eax=LIVE_EAX, eflags=0xAD7,
             arguments=list(args), named_arguments=list(args if named is None else named),
             stores=[], watched=[], write_ranges=[], endpoints=[],
             result_mask=mask, native_cleanup=cleanup, canonical_cleanup=cleanup,
             named_cleanup=0, canonical_registers_and_flags=False, budget=2000000)
    if owner or entry in OWNERS:
        c['canonical_owner'] = owner or OWNERS[entry]
    return c


def _put(c, at, value, width=4):
    c['stores'].append((at, (value & ((1 << (width * 8)) - 1)).to_bytes(width, 'little')))


def _compound_cases():
    out = []
    # First/last/duplicate hits, exhausted count, signed nonpositive counts.
    for count, keys, key in ((-1, (), 7), (0, (), 7), (1, (7,), 7),
                             (1, (8,), 7), (4, (1, 3, 5, 7), 7),
                             (4, (1, 7, 7, 2), 7),
                             (4, (0xFFFFFFFF, 0x80000000, 0, 1), 0x80000000),
                             (4, (1, 2, 3, 4), 99)):
        c = _case(0x41B9D0, f'count{count}-key{key:x}-keys' + '-'.join(f'{k:x}' for k in keys),
                  [key], [SELF, key, 0], cleanup=4)
        _region(c, SELF, 0x1904, _pattern(0x1904))
        _put(c, SELF + 0x1900, count)
        for i, value in enumerate(keys):
            _put(c, SELF + 12 + i * 64, value)
        c['expected_eax'] = next((i for i, value in enumerate(keys) if value == key), 0xFFFFFFFF)
        c['canonical_registers_and_flags'] = True
        out.append(c)
    # Low32 multiplication, signed quotient, truncation boundaries and clamp.
    for factor, amount in ((0, 0xFFFFFFFF), (1, 0), (1, 399), (1, 400),
                            (1, 799), (1, 800), (255, 0xFFFFFFFF),
                            (255, 0x80000001), (17, 0x13579BDF),
                            (255, 0x7FFFFFFF), (1, 0x80000000)):
        c = _case(0x4477D0, f'factor{factor}-amount{amount:x}', named=[SELF, 0, 0])
        _region(c, SELF, 0x140, _pattern(0x140))
        _put(c, SELF, factor, 1); _put(c, SELF + 0x134, amount)
        product = (factor * amount) & 0xFFFFFFFF
        signed = product if product < 0x80000000 else product - 0x100000000
        quotient = abs(signed) // 400 * (-1 if signed < 0 else 1)
        c['expected_eax'] = max(1, quotient)
        c['canonical_registers_and_flags'] = True
        out.append(c)
    for value, clear in ((0, 0), (0xFFFFFFFF, 0xFFFFFFFF),
                          (0x80000001, 0x13579BDF), (0x7135AC42, 1)):
        c = _case(0x560734, f'value{value:x}-clear{clear:x}', named=[SELF])
        _region(c, SELF, 0x40, _pattern(0x40), True)
        _put(c, SELF + 0x10, value); _put(c, SELF + 0x30, clear)
        c['expected_eax'] = value
        c['canonical_registers_and_flags'] = True
        out.append(c)
    return out


def _placement_fixture(c, offset, count, predicate, mode, selected=0):
    """Only first and last pieces have area; the real loop retains all count iterations."""
    extent = offset + max(1, count) * 52
    _region(c, SELF, max(0x100, extent), writable=False)
    _region(c, VTABLE, 0x40)
    _put(c, SELF, VTABLE); _put(c, VTABLE + 12, 0x4E52D0)
    _put(c, VTABLE + 16, predicate); _put(c, SELF + 0x50, selected)
    for at, value in ((0xBCA2D8, 0x5000), (0xBCA2E8, -17), (0xBCA2EC, 23)):
        _word(c, at, value)
    _word(c, 0xBC9768, 0x92714365, writable=True)
    _region(c, 0x5CC8C0, 36 * 8)
    for row in range(6):
        for col in range(6):
            _put(c, 0x5CC8C0 + (row * 6 + col) * 8, row * 228 + col)
    center = c['arguments'][0]
    _region(c, 0xAF598C + (center - 230) * 4, 4096, _pattern(4096))
    for i in range(max(0, count)):
        p = SELF + offset + i * 52
        x, y = (5, 2) if i == count - 1 and i else (0, 0)
        size = (2 if i == 0 else 1) if i in (0, count - 1) else 0
        if mode == 'adjacent-reject':
            size = 0
        _put(c, p, x); _put(c, p + 4, y); _put(c, p + 8, size)
        _put(c, p + 12, 9 if i == selected else 7)
        accepted = (0 if predicate == 0x4E5310 else
                    (0 if i == selected else 0x20000002) if predicate in (0x401440, 0x547DE0) else
                    (0 if i == 1 else 2))
        for row in range(size):
            for col in range(size):
                at = 0xAF598C + (center + y * 228 + x + row * 228 + col) * 4
                _put(c, at, 1 if mode == 'terrain-reject' and i == 0 and row == col == 0 else accepted)
    if predicate == 0x547DE0:
        for delta in (-228, 228, -1, 1):
            # Adjacent acceptance is OR, and all four native calls execute.
            _put(c, 0xAF598C + (center + delta) * 4, 2)
        if mode != 'adjacent-reject':
            _put(c, 0xAF598C + (center - 228) * 4, 0)
            # Restore selected area cells which overlap adjacent cells.
            for row in range(2):
                for col in range(2):
                    _put(c, 0xAF598C + (center + row * 228 + col) * 4,
                         1 if mode == 'terrain-reject' and row == col == 0 else 0)
    c['endpoints'].append(_ep('border', 0x46F9A0, ['FUN_0046f9a0'], 3,
                              'full32', 0xA7310001 if mode == 'border-reject' else 0xA7310000))
    return SELF + offset


def _placement_cases():
    out = []
    # Eight wrappers execute the complete shared body with the native fixed count.
    for entry, _, offset, count, _, predicate in CONTEXTS:
        complex_ = predicate == 0x547DE0
        for mode in ('accept', 'terrain-reject', 'border-reject') + (('adjacent-reject',) if complex_ else ()):
            cell = 0x5000 + 7 * 228 + 13
            c = _case(entry, mode, [cell], [SELF, cell], cleanup=4)
            _placement_fixture(c, offset, count, predicate, mode)
            out.append(c)
    # Direct members additionally exercise zero/negative counts and the high index byte.
    for entry, predicate in ((0x4E5330, 0x4E5310), (0x5463E0, 0x547DE0)):
        for count, mode in ((-257, 'accept'), (0, 'accept'), (1, 'accept'),
                             (3, 'terrain-reject'), (257, 'accept'),
                             (3, 'border-reject')):
            cell, pieces = 0x5000 + 7 * 228 + 13, SELF + 0x100
            c = _case(entry, f'count{count}-{mode}', [cell, pieces, count],
                      [SELF, cell, pieces, count], cleanup=12)
            _placement_fixture(c, 0x100, count, predicate, mode)
            out.append(c)
        for delta in (-1, -229):
            cell, pieces, count = 0x5000 + delta, SELF + 0x100, 3
            c = _case(entry, f'negative-cell-delta{delta}', [cell, pieces, count],
                      [SELF, cell, pieces, count], cleanup=12)
            _placement_fixture(c, 0x100, count, predicate, 'accept')
            out.append(c)
    return out


def _existing_fixture(c, count, kind, scenario):
    _region(c, SELF, 0x80, _pattern(0x80))
    _region(c, VTABLE, 0x40)
    _put(c, SELF, VTABLE); _put(c, VTABLE + 12, 0x4E52D0)
    _region(c, BUILDINGS, max(3, count) * 264)
    if count > 1:
        first = BUILDINGS + 264
        # An inactive matching record does not call the virtual query.
        _put(c, first + 16, kind, 2)
        if scenario == 'active-mismatch':
            _put(c, first, 1, 1); _put(c, first + 16, -257, 2)
        elif scenario == 'match-last':
            last = BUILDINGS + (count - 1) * 264
            _put(c, first, 1, 1); _put(c, first + 16, -257, 2)
            _put(c, last, 1, 1); _put(c, last + 16, kind, 2)
        elif scenario == 'match-first':
            _put(c, first, 1, 1)


def _existing_activity_cases():
    out = []
    for _, entry, _, count, kind, _ in CONTEXTS:
        for scenario in ('inactive', 'active-mismatch', 'match-first', 'match-last'):
            c = _case(entry, scenario, named=[SELF, LIVE_EAX])
            _existing_fixture(c, count, kind, scenario)
            out.append(c)
    for count, kind, scenario in ((-1, 0xE6, 'inactive'), (0, 0xE6, 'inactive'),
                                   (1, 0xE6, 'inactive'), (4, -257, 'match-last'),
                                   (4, 0xE6, 'active-mismatch'), (4, 0xE6, 'match-first')):
        c = _case(0x4E5790, f'count{count}-kind{kind}-{scenario}', [SELF, kind, count],
                  [SELF, kind, count, LIVE_EAX], cleanup=12)
        _existing_fixture(c, count, kind, scenario)
        out.append(c)
    for table, count, count_token in ((0x570160, 9, 0x545F60), (0x570268, 10, 0x559460),
                                       (0x570228, 21, 0x5567B0), (0x5701E8, 31, 0x54FED0)):
        for scenario in ('root-busy', 'idle', 'active-first', 'active-last', 'invalid-worker'):
            building, root = 263, 261
            c = _case(0x547BA0, f'table{table:x}-{scenario}', [building], [SELF, building], cleanup=4)
            _region(c, SELF, 0x80)
            _put(c, SELF, table)
            _word(c, table + 0x38, count_token); _word(c, table + 0x24, 0x5477E0)
            _region(c, BUILDINGS + root * 264, (count + 3) * 264, writable=True)
            _put(c, BUILDINGS + building * 264 + 18, 2, 2)
            _put(c, BUILDINGS + root * 264 + 152, 0x12345678 if scenario == 'root-busy' else 0)
            _region(c, ACTORS + 7 * 388, 388)
            if scenario in ('active-first', 'active-last', 'invalid-worker'):
                index = building + (count - 1 if scenario == 'active-last' else 0)
                _put(c, 0x799A34 + index * 264, 7)
                _put(c, ACTORS + 7 * 388 + 14, int(scenario != 'invalid-worker'), 1)
                _put(c, ACTORS + 7 * 388 + 10, 0x51, 1)
                _put(c, ACTORS + 7 * 388 + 56, 9, 1)
            out.append(c)
    return out


def _free_endpoint():
    return _ep('free', 0x561A2B, ['FUN_00561a2b'], 1)


def _scope_cases():
    out = []
    for entry, offset, count, owner in SCOPES:
        for shift in (0, 0x200):
            frame, obj = FRAME + shift, SELF + shift
            c = _case(entry, f'parent-shift{shift:x}', named=[frame], mask=None, owner=owner)
            c.update(initial_ebp=frame, initial_fs0=0xA731E9F7)
            c['write_ranges'].append((0, 4))
            _region(c, frame - 0x60, 0x80, _pattern(0x80))
            _put(c, frame - 0x44, obj)
            _region(c, obj, offset + count * 52, _pattern(offset + count * 52))
            out.append(c)
    for entry, offset in ((0x56B6B0, 4), (0x56E1AA, 8)):
        for allocation in (0, SCRATCH + 0x21000, 0xFFFFFFFF):
            c = _case(entry, f'allocation{allocation:x}', named=[FRAME], mask=None)
            c['initial_ebp'] = FRAME
            _region(c, FRAME - 16, 48, _pattern(48))
            _put(c, FRAME + offset, allocation)
            c['endpoints'] = [_free_endpoint()]
            c['expected_endpoint_counts'] = {'free': 1}
            out.append(c)
    return out


def _predicate_table_cases():
    out = []
    for terrain in (0, 0x01002890, 1, 2, 0x20000002, 0xFFFFFFFF):
        for piece in (-1, 0, 1, 2):
            for entry in (0x4E52D0, 0x4E5310, 0x401440, 0x547DE0, 0x4E6620):
                named = ([] if entry == 0x4E52D0 else
                         [terrain, LIVE_EAX] if entry == 0x4E5310 else
                         [terrain, piece] if entry == 0x4E6620 else
                         [SELF, terrain, piece, LIVE_EAX])
                c = _case(entry, f'terrain{terrain:x}-piece{piece}', [terrain, piece], named, cleanup=8)
                _region(c, SELF - 64, 320, _pattern(320))
                _put(c, SELF + 0x50, 1)
                for i in range(-1, 3):
                    _put(c, SELF + 0x58 + i * 52, 9 if i in (-1, 1) else 7)
                out.append(c)
    for entry, base, width in ((0x4493B0, 0x5D9C58, 1), (0x4493D0, 0x5D9C98, 4),
                                (0x4493E0, 0x5D9D10, 4)):
        for index, value in ((0, 0), (1, 1), (29, 0x80000081), (127, 0xFFFFFFFF), (255, 0x7135AC42)):
            c = _case(entry, f'index{index}-value{value:x}', named=[SELF])
            _region(c, SELF, 0x80, _pattern(0x80))
            _put(c, SELF + 2, index, 1)
            _word(c, base + index * width, value, width)
            c['expected_eax'] = value & ((1 << (width * 8)) - 1)
            out.append(c)
    return out


def _widget_fixture(c, orientation=0, frame=6, side=0, kind=0, motion=0,
                    source=0, destination=1, image=0, duration=12, limit=200):
    coords, sequence, motions = SCRATCH + 0x21000, SCRATCH + 0x22000, SCRATCH + 0x23000
    _region(c, SELF, 32, _pattern(32))
    _put(c, SELF, kind); _put(c, SELF + 4, motion); _put(c, SELF + 8, frame)
    _put(c, SELF + 16, 0, 2); _put(c, SELF + 21, side, 1)
    _region(c, OUT, 32, _pattern(32), True)
    _region(c, 0x5BE9B8, 40)
    c['stores'].append((0x5BE9B8, b''.join(_u32(v) for v in
                        (coords, 2, sequence, 1, motions, 1, 0, 0, -11, 13))))
    _region(c, coords, 64)
    for point in range(2):
        for direction in range(4):
            c['stores'].append((coords + (point * 4 + direction) * 8,
                                _u32(-17 + point * 23 + direction * 3) +
                                _u32(29 - point * 19 - direction * 5)))
    _region(c, sequence, 16)
    c['stores'].append((sequence, b''.join(_u32(v) for v in (2, 21, 3, 7))))
    _region(c, motions, 48)
    c['stores'].append((motions, b''.join(_u32(v) for v in
                        (source, destination, image, duration, 3, 4, 5, 6, 7, 8, 9, 10))))
    _word(c, 0xBCA2E0, orientation); _word(c, 0x126517C, limit)
    _word(c, 0xF55BB4 + 2 * 8, 20)
    _region(c, 0x10D6A00, 200 * 64)
    for index in range(200):
        _put(c, 0x10D6A1A + index * 64, -13 + index % 17, 2)
        _put(c, 0x10D6A1C + index * 64, 19 - index % 23, 2)


def _widget_cases():
    out = []
    vectors = [('start', dict(frame=0)), ('middle', dict(frame=6)),
               ('end', dict(frame=12)), ('reverse', dict(side=1)),
               ('orientation-minus1', dict(orientation=-1)),
               ('orientation6', dict(orientation=6)), ('duration1', dict(duration=1, frame=1)),
               ('fallback-image', dict(limit=20)), ('negative-kind', dict(kind=-1)),
               ('kind253', dict(kind=253)), ('negative-motion', dict(motion=-1)),
               ('motion-bound', dict(motion=1)), ('negative-source', dict(source=-1)),
               ('destination-bound', dict(destination=2)), ('negative-image', dict(image=-1)),
               ('negative-frame', dict(frame=-1)), ('frame-bound', dict(frame=13))]
    for entry in (0x41F910, 0x41FAD0):
        for label, changes in vectors + [('hidden', {}), ('alias-all', {}), ('alias-x-image', {}), ('alias-y-image', {})]:
            pointers = ([OUT] * 3 if label == 'alias-all' else
                        [OUT, OUT + 4, OUT] if label == 'alias-x-image' else
                        [OUT, OUT + 4, OUT + 4] if label == 'alias-y-image' else
                        [OUT, OUT + 4, OUT + 8])
            args = pointers + ([3] if entry == 0x41FAD0 else [])
            c = _case(entry, label, args, [SELF] + args, mask=None,
                      cleanup=16 if entry == 0x41FAD0 else 12)
            _widget_fixture(c, **changes)
            if label == 'hidden':
                _put(c, SELF + 16, 3, 2)
            out.append(c)
        if entry == 0x41FAD0:
            for direction in range(4):
                c = _case(entry, f'direction{direction}', [OUT, OUT + 4, OUT + 8, direction],
                          [SELF, OUT, OUT + 4, OUT + 8, direction], mask=None, cleanup=16)
                _widget_fixture(c, orientation=4, side=1)
                out.append(c)
    return out


def _actor_simple_cases():
    out = []
    for allocation in (0, SCRATCH + 0x24000):
        c = _case(0x4454A0, f'allocation{allocation:x}', named=[SELF], mask=None)
        _region(c, SELF, 324, _pattern(324), True)
        _put(c, SELF + 4, allocation); _put(c, SELF + 0x138, 0x7314AC42)
        c['endpoints'] = [_free_endpoint()]
        c['expected_endpoint_counts'] = {'free': int(bool(allocation))}
        out.append(c)
    for seed in (bytes(124), _pattern(124), b'\xff' * 124):
        c = _case(0x448960, f'seed{seed[0]:02x}', named=[SELF])
        _region(c, SELF, 124, seed, True)
        c['expected_eax'] = SELF
        out.append(c)
    for first, second in ((0, 0), (0, 255), (1, 0), (128, 0), (255, 1), (255, 255)):
        c = _case(0x449390, f'first{first}-second{second}', named=[SELF])
        _region(c, SELF, 124, _pattern(124)); _put(c, SELF + 2, 7, 1)
        _word(c, 0x5D9C18 + 7, first, 1); _word(c, 0x5D9C38 + 7, second, 1)
        c['expected_eax'] = int(first != 0 and second == 0)
        out.append(c)
    for values in ((-1, -1, -1), (-1, 2, 3), (-1, -1, 3), (1, -1, 3),
                   (1, 2, -1), (1, 2, 3), (-32768, -2, 32767),
                   (0x1234FFFF, 0x56780002, 0x9ABC0003)):
        c = _case(0x44C780, '-'.join(f'{v & 0xffffffff:x}' for v in values), values,
                  [SELF] + list(values), mask=None, cleanup=12)
        _region(c, SELF, 16, _pattern(16), True)
        out.append(c)
    for step, start, end, first in ((0, (1, 2), (8, 11), 0),
                                     (1, (7, -3), (7, -3), 0),
                                     (1, (-17, 29), (6, 10), 0),
                                     (3, (30, -40), (-9, 12), 3),
                                     (7, (-32768, 32767), (-32760, 32760), 1),
                                     (255, (8, 11), (1, 2), 0)):
        c = _case(0x445D10, f'step{step}-start{start}-end{end}-first{first}', [0, 1, first],
                  [SELF, 0, 1, first], cleanup=12)
        _region(c, SELF, 324, _pattern(324)); _put(c, SELF, step, 1)
        _put(c, SELF + 4, SCRATCH + 0x20000)
        for i, point in enumerate((start, end)):
            _put(c, SELF + 8 + i * 6, point[0], 2)
            _put(c, SELF + 10 + i * 6, point[1], 2)
        _region(c, SCRATCH + 0x20000, 1024, _pattern(1024), True)
        out.append(c)
    # EH tail receiver is the actual EBP-150h local; named is this same ABI tail.
    for allocation in (0, SCRATCH + 0x24000):
        c = _case(0x56B650, f'allocation{allocation:x}', mask=None)
        c['initial_ebp'] = FRAME
        _region(c, FRAME - 0x150, 324, _pattern(324), True)
        _put(c, FRAME - 0x150 + 4, allocation)
        c['endpoints'] = [_free_endpoint()]
        c['expected_endpoint_counts'] = {'free': int(bool(allocation))}
        out.append(c)
    return out


def _search_endpoint(value=-1, reverse=None):
    ep = _ep('route-search', 0x447430, ['FUN_00447430'], 2, 'full32', value,
             pointer_args={'1': 'reversed-byte'})
    if reverse is not None:
        ep['writes_args'] = [(1, 1, reverse)]
    return ep


def _route_fixture(c, kind=2, index=3, count=2, enabled=1, factor=1, amount=800):
    _region(c, SELF, 124, _pattern(124))
    _put(c, SELF + 2, kind, 1); _put(c, SELF + 0x58, index, 2)
    _put(c, SELF + 0x1C, 3, 2)
    _word(c, 0x5D9C18 + kind, 1, 1); _word(c, 0x5D9C38 + kind, 0, 1)
    row = 0x776FB0 + 3 * 324
    _region(c, row, 324, _pattern(324), True)
    _put(c, row, factor, 1); _put(c, row + 0x134, amount)
    _put(c, row + 0x141, count, 1); _put(c, row + 0x142, enabled, 1)
    _region(c, 0x77DE00, 61 * 106)
    _put(c, 0x77DE00 + 7 * 106, 1, 1)
    _put(c, 0x77DE04 + 7 * 106, 3, 1)
    return row


def _route_cases():
    out = []
    vectors = [('negative-route', dict(index=-1)), ('route-bound', dict(index=50)),
               ('zero-vertices', dict(count=0)), ('one-vertex', dict(count=1)),
               ('disabled', dict(enabled=0)), ('enabled', {}),
               ('enabled255', dict(enabled=255)), ('clamp', dict(amount=0)),
               ('negative-product', dict(factor=255, amount=0xFFFFFFFF)),
               ('wrap-product', dict(factor=17, amount=0x13579BDF)),
               ('trade-search-fail', dict(kind=1)), ('trade-search-success', dict(kind=1)),
               ('trade-inactive-first', dict(kind=1)), ('trade-inactive-second', dict(kind=1))]
    for label, changes in vectors:
        c = _case(0x44C110, label, named=[SELF])
        _route_fixture(c, **changes)
        if label == 'trade-inactive-first':
            _put(c, 0x5D9C18 + 1, 0, 1)
        if label == 'trade-inactive-second':
            _put(c, 0x5D9C38 + 1, 128, 1)
        c['endpoints'] = [_search_endpoint(7, 1) if label == 'trade-search-success' else _search_endpoint()]
        c['expected_endpoint_counts'] = {'route-search': int(label in ('trade-search-fail', 'trade-search-success'))}
        out.append(c)
    for years, month, current_year, current_month, lead, route_index, amount in (
            (0, 0, 0, 0, 0, -1, 800), (1, 6, 0, 0, 3, 3, 800),
            (-3, -17, 0, 0, 255, 3, 0), (32767, 32767, 0, 0, 0, 3, 400),
            (-32768, -32768, 0x80000000, 0x7FFFFFFF, 255, -1, 800)):
        c = _case(0x44C9A0, f'y{years}-m{month}-cy{current_year:x}-cm{current_month:x}-lead{lead}-route{route_index}', named=[SELF])
        _route_fixture(c, index=route_index, amount=amount)
        _put(c, SELF + 0x14, years, 2); _put(c, SELF + 0x2C, month, 2)
        _put(c, SELF + 3, 7, 1); _put(c, SELF + 0x61, lead, 1)
        _word(c, 0x7848D8, -3, 2); _word(c, 0x78D7BC, current_year)
        _word(c, 0x78D7C0, current_month)
        out.append(c)
    return out


def _trade_cases():
    out = []
    vectors = [('wrong-kind', dict(kind=2)), ('inactive', dict(active=0)),
               ('wrong-code', dict(code=36)), ('mode4', dict(mode=4)),
               ('negative-mode', dict(mode=-257)), ('mode8', dict(mode=8)),
               ('mode5', dict(mode=5)), ('mode6', dict(mode=6)), ('mode7', dict(mode=7)),
               ('negative-progress', dict(progress=-17)), ('above100', dict(progress=32767)),
               ('months-zero', dict(months=0)), ('reverse', dict(reverse=1)),
               ('search-failure', dict(search=-1, reverse=None)),
               ('search-failure-pointer-low-byte', dict(search=-1, reverse=None, pointer_delta=1)),
               ('route-high-byte', dict(index=7)), ('special255', dict(special=255)),
               ('alias-route-percentage', dict(alias='route-percentage')),
               ('alias-special-unused', dict(alias='special-unused')),
               ('alias-all', dict(alias='all'))]
    for label, changes in vectors:
        v = dict(kind=1, active=1, code=37, mode=6, progress=3, months=10,
                 reverse=0, search=3, index=3, special=1, alias='none', pointer_delta=0)
        v.update(changes)
        base = OUT + v['pointer_delta']
        pointers = ([base] * 4 if v['alias'] == 'all' else
                    [base, base, base + 8, base + 12] if v['alias'] == 'route-percentage' else
                    [base, base + 4, base + 8, base + 8] if v['alias'] == 'special-unused' else
                    [base, base + 4, base + 8, base + 12])
        c = _case(0x449750, label, pointers, [SELF] + pointers, cleanup=16)
        _region(c, SELF, 124, _pattern(124)); _region(c, OUT, 32, _pattern(32), True)
        for offset, value, width in ((2, v['kind'], 1), (0x32, v['active'], 1),
                                      (4, v['code'], 2), (0x30, v['mode'], 2),
                                      (0x2E, v['progress'], 2), (0x1C, v['index'], 2)):
            _put(c, SELF + offset, value, width)
        # Aliased route/percentage can select any clamped index0..100.
        _region(c, 0x7770F0, 101 * 324)
        for index in range(101):
            _put(c, 0x7770F0 + index * 324, v['special'], 1)
        c['endpoints'] = [_search_endpoint(v['search'], v['reverse']),
            _ep('route-months', 0x44C110, ['restore_event_route_months_0044c110'], 0,
                'full32', v['months'], compiled_arity=1, compiled_receiver_arg=0,
                native_receiver=SELF)]
        proceeds = v['kind'] == 1 and v['active'] != 0 and v['code'] == 37
        c['expected_endpoint_counts'] = {'route-search': int(proceeds), 'route-months': int(proceeds)}
        out.append(c)
    return out


def definitions(registry):
    present = {int(m['address'], 16) for m in registry['members']}
    assert set(ENTRIES) <= present, ('cohort4 gameplay entries missing', sorted(set(ENTRIES) - present))
    out = []
    for family in (_compound_cases, _placement_cases, _existing_activity_cases,
                   _scope_cases, _predicate_table_cases, _widget_cases,
                   _actor_simple_cases, _route_cases, _trade_cases):
        out.extend(family())
    assert len(ENTRIES) == len(set(ENTRIES)) == 52
    assert {c['entry'] for c in out} == set(ENTRIES)
    assert len({c['id'] for c in out}) == len(out)
    return out
