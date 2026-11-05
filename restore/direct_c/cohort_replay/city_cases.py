"""Pure finite city inputs for cohort 2 members 69:100.

Names describe fixture intent, not observed coverage. No PE, runner, native
execution, or author-computed expected result is imported here.
"""
GROUP = 'city'

_SIGNATURES = {
    0x40f8f0: (3, 'void'), 0x469140: (3, 'void'),
    0x4b7510: (2, 'void'), 0x47c2b0: (5, 'void'),
    0x475b50: (1, 'full32'), 0x474380: (1, 'full32'),
    0x4c03d0: (1, 'void'), 0x46d0f0: (1, 'full32'),
    0x43fd00: (2, 'void'), 0x459ec0: (1, 'full32'),
    0x459ef0: (1, 'full32'), 0x4cde40: (2, 'full32'),
    0x4e2970: (1, 'void'), 0x4684e0: (0, 'void'),
    0x48c0e0: (4, 'void'), 0x4695d0: (2, 'void'),
    0x497b90: (0, 'void'), 0x47afd0: (3, 'void'),
    0x47bae0: (5, 'void'), 0x517370: (0, 'void'),
    0x517660: (0, 'void'), 0x5178e0: (0, 'void'),
    0x4696b0: (1, 'al8'), 0x46d100: (2, 'void'),
    0x53b090: (1, 'void'), 0x469d50: (0, 'full32'),
    0x469e40: (2, 'void'), 0x516f90: (5, 'void'),
    0x5173a0: (4, 'void'), 0x480060: (4, 'full32'),
    0x4b5420: (1, 'void'), 0x4b52f0: (1, 'void'),
    0x4b3250: (1, 'void'), 0x488050: (1, 'full32'),
    0x4802c0: (2, 'full32'), 0x4cde60: (2, 'full32'),
    0x4c06e0: (2, 'full32'), 0x4e94a0: (1, 'full32'),
    0x4e94d0: (1, 'full32'), 0x4e7e60: (4, 'void'),
    0x45a140: (1, 'full32'), 0x4689e0: (2, 'void'),
    0x417ae0: (1, 'void'), 0x4e0cf0: (5, 'void'),
    0x468b20: (1, 'void'), 0x480180: (3, 'full32'),
    0x467220: (1, 'void'), 0x4b4800: (5, 'full32'),
    0x4b1df0: (1, 'void'), 0x4bc140: (3, 'full32'),
    0x4bc2e0: (3, 'full32'), 0x4bc3b0: (3, 'full32'),
    0x4816b0: (4, 'full32'), 0x47bf50: (7, 'void'),
}
CHILDREN = {
    token: dict(name='FUN_%08x' % token, arity=arity,
                native_cleanup=0, compiled_cleanup=0, result=result,
                arg_masks={}, private_output_args=[])
    for token, (arity, result) in _SIGNATURES.items()
}
_ALIASES = {
    0x469140: ['restore_actor_tile_heat_overflow_raw', 'restore_city_apply_disaster_at_cell'],
    0x4b7510: ['restore_city_disaster_actors_004b7510', 'restore_actor_mark_sentries_near_cell'],
    0x497b90: ['restore_city_disaster_refresh_00497b90', 'restore_actor_refresh_sentries_on_marked_tiles'],
    0x475b50: ['restore_building_root'],
    0x459ec0: ['restore_building_is_kind_77_to_79'],
    0x4c03d0: ['restore_building_clear_four_parts'],
    0x4695d0: ['restore_building_clear_linked_parts'],
    0x48c0e0: ['restore_building_spawn_removal_effects'],
    0x4696b0: ['restore_building_allows_disaster_spread'],
    0x46d100: ['restore_god_building_0046d100'],
    0x488050: ['restore_actor_building_gate'],
    0x4684e0: ['restore_city_rebuild_building_requirements'],
    0x4689e0: ['restore_city_color_road_region'],
    0x468b20: ['restore_city_mark_linked_fire'],
    0x467220: ['restore_city_ensure_labor_seeker'],
}
for _token, _aliases in _ALIASES.items():
    CHILDREN[_token]['aliases'] = _aliases
CHILDREN[0x488050]['native_cleanup'] = 4
CHILDREN[0x4b4800]['arg_masks'] = {0: 255, 4: 255}
# Complete310B native469E40 consumes argument1 only at469F08 as BYTE.
# See endpoint-input-width-proof.json; physical DWORDs remain recorded.
CHILDREN[0x469e40]['arg_masks'] = {1: 255}

CASES = []
P = 0x49d8
B1 = 264
A7 = 7 * 388
A9 = 9 * 388


def base(arguments=(), controlled=()):
    return dict(setup='minimal', arguments=list(arguments),
                controlled=list(controlled), results={}, words=[], bytes=[],
                stores=[(0xeaac28, 1), (0x78d790, 0),
                        (0xbca2d8, 1000), (0xbca2d0, 1),
                        (0xbca2d4, 1), (0xbca2dc, 227),
                        (0xd056f8, 7), (0xd3b4e4, 20), (0xd3b4e0, 21),
                        (0xd3b384, 0), (0xd3b380, 0),
                        (0xd3b374, 0x12345678), (0xd3b1c4, 99)])


def add(label, fn, cfg, origin='protocol'):
    if origin == 'actual-composition':
        cfg.pop('controlled', None)
        cfg.pop('results', None)
    CASES.append(dict(id='city/' + label, fn=fn, origin=origin, cfg=cfg))


def building(cfg, n=1, kind=70, state=1):
    off = n * 264
    cfg['bytes'] += [(0x7999d8 + off, state), (0x7999d9 + off, 255),
                     (0x7999db + off, 2)]
    cfg['words'] += [(0x7999de + off, 10), (0x7999e0 + off, 11),
                     (0x7999e8 + off, kind), (0x7999f0 + off, 60),
                     (0x799a18 + off, 100)]
    cfg['stores'] += [(0x7999e4 + off, 1000)]
    return off


def terrain(cfg, allowed=()):
    """A closed 51984-cell terrain; later patches are ordered region writes."""
    cfg.setdefault('regions', []).append((0xee0c50, b'\xff' * 51984))
    cfg['regions'] += [(0xee0c50 + tile, bytes([kind & 255]))
                       for tile, kind in allowed]


# 69. Only the returning zero-link domain is finite in the original.
for actor in (1, 1999):
    c = base([actor])
    c['words'] += [(0xbcd65e + actor * 388, 0xdead),
                   (0xbcd660 + actor * 388, 0),
                   (0xbcd662 + actor * 388, 0xbeef)]
    add('no-predecessor/%d' % actor, 0x496640, c, 'actual-composition')

# 70. Signed WORD post-increment, including wrapped 7FFF and negative division.
for label, tile, initial, limit in [('equal', 1000, 9, 10),
                                   ('negative-relative-overflow', 771, 10, 10),
                                   ('signed-wrap', 1000, 0x7fff, 0)]:
    c = base([tile, limit], [0x469140])
    c['words'] += [(0xa9cc1c + tile * 2, initial)]
    add('heat/' + label, 0x4690f0, c)

# 71. The fill endpoint is controlled; its destination is explicitly reset.
for label, generation, cells in [('isolated', 7, []),
                                  ('two-cells', 0xffffffff, [(1001, 0)]),
                                  ('signed-terrain', 17, [(772, 2), (1001, 0x80)])]:
    c = base([0, 0], [0x40f8f0])
    c['stores'] += [(0xeadfd4, generation)]
    c['regions'] = [(0xeed760, b'\0' * 0x32c40)]
    terrain(c, cells)
    add('road-distance/' + label, 0x519a30, c)

# 72. Full empty-cell path, actor-mark condition, protected early return,
# and ordinary demolition with a bounded per-player counter.
disaster_children = [0x4b7510, 0x47c2b0, 0x475b50, 0x474380,
                     0x459ec0, 0x459ef0, 0x48c0e0, 0x4695d0,
                     0x497b90, 0x47afd0, 0x47bae0, 0x517370,
                     0x517660, 0x5178e0]
for label, occupant, marked, protected in [('empty', 0, 0, 0),
                                          ('empty-marked', -1, 0x4000, 0),
                                          ('protected', 1, 0, 1),
                                          ('ordinary-building', 1, 0, 0)]:
    c = base([10, 11, 1000], disaster_children)
    building(c)
    c['words'] += [(0xb4e6fc + 2000, occupant)]
    c['stores'] += [(0xaf598c + 4000, marked), (0xea5ec8 + P, 11)]
    c['results'] = {0x475b50: [1], 0x474380: [0x6abc0000 | protected],
                    0x459ec0: [0], 0x459ef0: [0]}
    add('disaster/' + label, 0x469140, c)

# 73. Selector exclusions and the signed-WORD upper-DWORD result domain.
for kind in (183, 236, 182, 0xffff):
    c = base([1])
    c['words'] += [(0x7999e8 + B1, kind)]
    add('spread-selector/type-%04x' % kind, 0x4696b0, c,
        'actual-composition')

# 74. Phase reset, phase32, expiry33, and a random/spread pass with no occupants.
for label, phase, suppressed in [('negative-phase', 0xffff, 0),
                                  ('phase32-suppressed', 31, 1),
                                  ('expired', 32, 0),
                                  ('spread-empty', 7, 0)]:
    c = base([], [0x47c2b0, 0x517370, 0x517660])
    building(c, kind=99)
    c['stores'] += [(0x78d790, 1), (0x78d808, 0)]
    c['words'] += [(0x799a22 + B1, phase)]
    c['bytes'] += [(0x799a95 + B1, suppressed), (0x799a25 + B1, 0),
                   (0x784cb0, 0)]
    c['regions'] = [(0xb4e6fc, b'\0' * (51984 * 2))]
    add('burning/' + label, 0x469730, c)

# 75. Real native x87/FTOL path is retained; only the migration endpoints are
# controlled. The original immutable multiplier is not replaced by a fixture.
for label, timer, population, count in [('before-tick', 8, 100000, 4),
                                       ('zero-quota', 9, 0, 1),
                                       ('four-requests', 9, 100000, 4),
                                       ('signed-timer-wrap', 0x7fffffff, 100000, 4)]:
    c = base([], [0x469d50, 0x469e40])
    c['stores'] += [(0x978d80, timer), (0xea1894 + P, population),
                    (0x78d790, count), (0x70ce0c + 4 * 88, 123)]
    for n in range(1, count + 1):
        off = building(c, n=n)
        c['bytes'] += [(0x7999dd + off, 1)]
        c['words'] += [(0x7999f4 + off, 1), (0x7999ea + off, 4)]
        c['stores'] += [(0x799abc + off, 1000)]
    c['results'] = {0x469d50: [0x12340001, 0, 1, 1]}
    add('migration/' + label, 0x469b90, c)

# 76. First match is cached; callback changes are visible to later rectangles.
for label, count, match in [('empty', 0, False), ('no-match', 2, False),
                             ('first-match-reloads', 2, True)]:
    c = base([70], [0x47c2b0, 0x53b090, 0x516f90, 0x5173a0])
    c['stores'] += [(0x78d790, count)]
    for n in range(1, count + 1):
        building(c, n=n, kind=70 if match else 71)
    if match:
        c['stores'] += [(0x7999e4 + B1, 0xfedcba98)]
        c['child_mutations'] = {(0x47c2b0, 1): [(0x7999de + B1, 0xfffdfffe)]}
        c['child_bytes'] = {(0x53b090, 1): [(0x7999db + B1, 3)]}
    add('remove-kind/' + label, 0x469f80, c)

# 77. Both sentry repair branches, plus the unmarked skip.
for label, marked, found in [('unmarked', 0, 0),
                             ('return-to-building', 1, 0),
                             ('new-tile', 1, 1)]:
    c = base([], [0x480060, 0x4b5420, 0x4b52f0, 0x4b3250])
    building(c)
    c['bytes'] += [(0xbcd602 + A7, 42), (0xebab20 + 1000, marked)]
    c['words'] += [(0xbcd60c + A7, 10), (0xbcd60e + A7, 11),
                   (0xbcd656 + A7, 1), (0xbcd624 + A7, 31),
                   (0xbcd626 + A7, 32)]
    c['stores'] += [(0xbcd618 + A7, 1000)]
    c['results'] = {0x480060: [found]}
    add('refresh-sentry/' + label, 0x497b90, c)

# 78. Actor zero is in this native fixed scan; use actor7 for populated records.
for label, x, y in [('radius-one', 11, 12), ('radius-two', 12, 11),
                    ('signed-coordinate', -32768, -32768)]:
    c = base([x, y])
    c['bytes'] += [(0xbcd606 + A7, 1), (0xbcd602 + A7, 42),
                   (0xbcd630 + A7, 8)]
    c['words'] += [(0xbcd60c + A7, 10 if x >= 0 else 0x8000),
                   (0xbcd60e + A7, 11 if y >= 0 else 0x8001)]
    add('mark-sentry/' + label, 0x4b7510, c, 'actual-composition')

# 79. Four independent DWORD stores, including high bits.
for value in (1, 0xffffffff):
    c = base()
    c['stores'] += [(at, value ^ n) for n, at in enumerate(
        [0x78d5c8, 0x78d5cc, 0x78d5dc, 0x78d5e0])]
    add('clear-counters/%08x' % value, 0x46a060, c, 'actual-composition')

# 80. Distinct connected count groups, with explicit root/gate endpoints.
for label, kinds in [('empty', []), ('services', [102, 76, 95]),
                      ('root-and-house', [65, 31, 70]),
                      ('production-and-monument', [90, 140, 75])]:
    c = base([], [0x475b50, 0x488050, 0x4802c0])
    c['stores'] += [(0x78d790, len(kinds)), (0x705c1c, 0)]
    for n, kind in enumerate(kinds, 1):
        off = building(c, n=n, kind=kind)
        c['bytes'] += [(0x799a0d + off, 1),
                       (0x799a7f + off, 1), (0x799a80 + off, 255)]
        if label == 'root-and-house' and kind == 70:
            c['bytes'] += [(0x7999dd + off, 1)]
            c['words'] += [(0x7999f4 + off, 1)]
    c['results'] = {0x475b50: [1, 2], 0x488050: [1], 0x4802c0: [1]}
    add('building-counts/' + label, 0x4677c0, c)

# 81. A nonzero active denominator keeps the real native average finite.
for label, modes, percentages in [
        ('one-category', [1, 0, 0, 0, 0], [101, 100, 99, 0, 0xffffffff, 37, 101, 100]),
        ('two-category', [2, 1, 0, 0, 0], [10, 20, 30, 40, 50, 99, 101, 0xffffffff, 75]),
        ('all-categories', [1, 2, 1, 2, 1], [100] * 12)]:
    c = base([], [0x4cde60, 0x4c06e0, 0x4e94a0, 0x4e94d0, 0x4e7e60])
    c['stores'] += [(0xea1894 + P, 1000), (0x70573c + 2 * 4, 3),
                    (0x70573c + 3 * 4, 5), (0x70573c + 4 * 4, 7)]
    c['words'] += [(0x7848e2 + i * 2, mode) for i, mode in enumerate(modes)]
    c['results'] = {0x4cde60: percentages, 0x4c06e0: [2, 3, 4] * 5,
                    0x4e94a0: [333], 0x4e94d0: [444]}
    c['child_mutations'] = {(0x4e7e60, 1): [(0x78d614, 11),
        (0x78d61c, 22), (0x78d618, 33), (0x78d610, 44)]}
    add('service-coverage/' + label, 0x468170, c)

# 82. Inactive, house-skip, and full DWORD requirement publication.
for label, count, house in [('empty', 0, False), ('house', 1, True),
                             ('requirements', 2, False)]:
    c = base([], [0x45a140])
    c['stores'] += [(0x78d790, count)]
    for n in range(1, count + 1):
        off = building(c, n=n)
        c['stores'] += [(0x799a90 + off, 0xdeadbeef)]
        c['bytes'] += [(0x7999dd + off, int(house))]
    c['results'] = {0x45a140: [0xfedcba98, 0x01234567]}
    add('requirements/' + label, 0x4684e0, c)

# 83. Flood endpoint publishes size/tile; ranking includes equal-size insertion.
for label, height, sizes in [('height-zero', 0, []),
                              ('ranked', 1, [5, 3]), ('tied', 1, [5, 5])]:
    c = base([], [0x40f8f0, 0x4689e0])
    c['stores'] += [(0xbca2d4, height), (0xbca2d0, 3),
                    (0xaf598c + 1000 * 4, 0x40),
                    (0xaf598c + 1001 * 4, 0),
                    (0xaf598c + 1002 * 4, 0x40)]
    c['regions'] = [(0xa76aec, b'\0' * 51984),
                    (0xa69fdc, b'\0' * 51984)]
    c['child_mutations'] = {(0x4689e0, i + 1):
        [(0x78c400, size), (0xbc9768, 1000 + 2 * i)]
        for i, size in enumerate(sizes)}
    add('road-regions/' + label, 0x468860, c)

# 84. Signed WORD damage threshold and distinct collapse/fire callbacks.
hazard_children = [0x475b50, 0x417ae0, 0x47c2b0, 0x48c0e0,
                   0x4695d0, 0x46d100, 0x517370, 0x517660]
for label, frozen, count, damage, fire in [('frozen', 1, 1, 0, 0),
                                         ('no-buildings', 0, 0, 0, 0),
                                         ('collapse-threshold', 0, 1, 999, 0),
                                         ('fire-threshold', 0, 1, 0, 999)]:
    c = base([], hazard_children)
    building(c)
    c['stores'] += [(0x978d70, frozen), (0x78d790, count),
                    (0xe92e0c, 15001), (0xe950ec, 1), (0xe950f0, 0),
                    (0xea5c84 + P, 0xffffffff), (0xea5c88 + P, 0xffffffff),
                    (0x705ebc + 70 * 40, int(bool(fire)))]
    c['words'] += [(0x799a1e + B1, damage), (0x799a20 + B1, fire),
                   (0x705ec0 + 70 * 40, int(bool(damage)))]
    c['bytes'] += [(0xacf85c + 1000, 7), (0x7027bb, 0),
                   (0x7027b8, 0), (0x784cb0, 0)]
    c['stores'] += [(0xaf598c + 1000 * 4, 0)]
    c['results'] = {0x475b50: [1]}
    add('hazards/' + label, 0x468b70, c)

# 85. Timer boundary and the original XOR-FFFF mask on packed groups.
for label, timer, values in [('before-tick', 18, [0xffffffff]),
                              ('packed-groups', 19, [0, 7, 0xffffffff]),
                              ('timer-wrap', 0x7fffffff, [0x49249249])]:
    c = base()
    c['stores'] += [(0xbca344, timer), (0xbca2d4, 1),
                    (0xbc9f24, 0), (0xbc9b94, len(values)),
                    (0xbc9804, 1000)]
    c['stores'] += [(0x978d9c + (1000 + i) * 4, value)
                    for i, value in enumerate(values)]
    add('overlay-decay/' + label, 0x47e290, c, 'actual-composition')

# 86. Closed tiny floods exercise isolated, linear, and queued neighbors.
for label, cells, region in [('isolated', [], 1),
                              ('two-cells', [(1001, 1)], 0x1ff),
                              ('queued-branches', [(772, 0), (1001, 2), (1228, 0)], 3)]:
    c = base([1000, region], [0x40f8f0])
    c['stores'] += [(0x78c408, 255), (0x78c400, 1), (0x78c404, 0),
                    (0xaf598c + 1001 * 4, 0x400)]
    c['regions'] = [(0xa76aec, b'\0' * 51984),
                    (0xa69fdc, b'\0' * 51984), (0x78c414, b'\0' * 4000)]
    terrain(c, cells)
    add('color-region/' + label, 0x4689e0, c)

# 87. Acyclic signed links and a link reloaded after the destructive child.
for label, links, mutate in [('single', [(1, 0)], False),
                             ('chain', [(1, 2), (2, 0xffff)], False),
                             ('child-shortens-chain', [(1, 2), (2, 0)], True)]:
    c = base([7], [0x475b50, 0x46d100])
    c['results'] = {0x475b50: [1]}
    c['words'] += [(0x799a12 + n * 264, link) for n, link in links]
    if mutate:
        c['child_mutations'] = {(0x46d100, 1): [(0x799a12 + B1, 0)]}
    add('linked-fire/' + label, 0x468b20, c)


worker_children = [0x480180, 0x467220, 0x4cde60, 0x4b4800,
                   0x4b1df0, 0x4bc140, 0x4bc2e0, 0x4bc3b0, 0x4b3250]


def worker(percentage=100, found=1):
    c = base([1], worker_children)
    building(c)
    c['stores'] += [(0x705eb8 + 70 * 40, 200)]
    c['words'] += [(0x799a2c + B1, 400), (0x799a14 + B1, 199),
                   (0x799aa4 + B1, 100), (0xbcd60c + A7, 0xfffe),
                   (0xbcd60e + A7, 0xfffd), (0xbcd63c + A9, 0xabcd)]
    c['bytes'] += [(0x799a7e + B1, 0), (0x799a0a + B1, 17)]
    c['results'] = {0x480180: [found], 0x4cde60: [percentage],
                    0x4b4800: [7], 0x4bc140: [1],
                    0x4bc2e0: [1], 0x4bc3b0: [1]}
    return c


# 88. The delay is signed and checked at399/400; success reloads current actor.
for label in ['no-road-negative-stock', 'delay399', 'spawn-success', 'spawn-failed']:
    c = worker(1)
    if label == 'no-road-negative-stock':
        c['words'] += [(0x7999f0 + B1, 0xffff)]
        c['results'][0x480180] = [0]
    elif label == 'delay399':
        c['words'] += [(0x799a2c + B1, 399)]
    elif label == 'spawn-success':
        c['child_mutations'] = {(0x4b3250, 1):
            [(0xd056f8, 9), (0x799a14 + B1, 50)]}
    else:
        c['results'][0x4bc140] = [0]
    add('delayed-worker/' + label, 0x4612c0, c)

# 89/90. The minimum is25, and the count is reloaded after patrol.
for fn, stem in [(0x461490, 'service-worker'), (0x4615d0, 'stock-reset-worker')]:
    for label, percentage, found in [('no-road', 100, 0), ('percentage24', 24, 1),
                                     ('percentage25', 25, 1), ('patrol-count-wrap', 75, 1)]:
        c = worker(percentage, found)
        if label == 'patrol-count-wrap':
            c['child_bytes'] = {(0x4b1df0, 1): [(0x799a7e + B1, 255)]}
        add(stem + '/' + label, fn, c)

# 91. Ordered first/second stock attempts and the both-failed actor state.
for label in ['percentage24', 'primary-success', 'fallback-success', 'both-failed']:
    c = worker(24 if label == 'percentage24' else 50)
    if label in ['fallback-success', 'both-failed']:
        c['results'][0x4bc2e0] = [0]
    if label == 'both-failed':
        c['results'][0x4bc3b0] = [0]
    if label == 'primary-success':
        c['child_mutations'] = {(0x4b3250, 1):
            [(0xd056f8, 9), (0x799aa4 + B1, 50)]}
    add('dual-stock-worker/' + label, 0x461720, c)

# 92. Gate, stale link, creation, and restoration of cached spawn coordinates.
for label in ['gate', 'stale-link', 'create-and-restore']:
    c = base([1], [0x488050, 0x4b4800, 0x4b1df0])
    building(c)
    c['stores'] += [(0xea1894 + P, 100)]
    c['results'] = {0x488050: [1 if label == 'gate' else 0], 0x4b4800: [7]}
    if label == 'stale-link':
        c['words'] += [(0x799a04 + B1, 7), (0xbcd652 + A7, 2)]
        c['bytes'] += [(0xbcd606 + A7, 0), (0xbcd602 + A7, 4)]
    if label == 'create-and-restore':
        c['child_mutations'] = {(0x4b1df0, 1):
            [(0xd3b4e4, 0x12345678), (0xd3b4e0, 0x87654321)]}
    add('labor-seeker/' + label, 0x467220, c)

# 93. Cached tile survives the query; signed fertility uses >=30 here.
for label, fertility, found in [('missing-terrain', 31, 0),
                                ('threshold30', 30, 1), ('negative-fertility', 255, 1)]:
    c = base([1], [0x4816b0, 0x47bf50])
    building(c)
    c['stores'] += [(0xf56644, 0xfffffffe)]
    c['bytes'] += [(0xac2d4c + 1000, fertility)]
    c['results'] = {0x4816b0: [found]}
    if label == 'threshold30':
        c['child_mutations'] = {(0x4816b0, 1): [(0x7999e4 + B1, 1200)]}
        c['bytes'] += [(0xac2d4c + 1200, 255)]
    add('masked-image/' + label, 0x467310, c)

# 94. This separate facade has logical AL output and a strict >30 threshold.
for label, state, fertility in [('inactive', 0, 31), ('threshold30', 1, 30),
                                ('threshold31', 1, 31)]:
    c = base([1], [0x47bf50])
    building(c, state=state)
    c['stores'] += [(0xf56584, 0x12345678), (0xf564cc, 0x87654321)]
    c['bytes'] += [(0xac2d4c + 1000, fertility)]
    add('fertility-image/' + label, 0x4673c0, c)

# 95. Native WORD subtraction wraps, including stock below its reservation.
for label, kind, value, reserved in [('unreserved', 70, 199, 0),
                                     ('kind204-underflow', 204, 1, 1),
                                     ('ordinary-exact', 70, 100, 1)]:
    c = base([1])
    c['words'] += [(0x7999e8 + B1, kind), (0x799a2c + B1, 0xffff),
                   (0x7999ea + B1, reserved), (0x799aa6 + B1, reserved),
                   (0x799a14 + B1, value), (0x799aa4 + B1, value)]
    add('reset-reservations/' + label, 0x455c30, c, 'actual-composition')

# 96. A bounded house list, next-level cap, and radius inclusion boundary.
for label, count, house_x in [('empty', 0, 10), ('in-radius', 1, 11),
                              ('outside-radius', 1, 12)]:
    c = base([2, 1])
    building(c, n=2)
    c['stores'] += [(0x78d790, count)]
    if count:
        off = building(c, n=1)
        c['bytes'] += [(0x7999dd + off, 1)]
        c['words'] += [(0x7999f4 + off, 1), (0x7999de + off, house_x),
                       (0x7999ea + off, 19)]
        for at, value in [(0x70ce04, 1), (0x70cdf8, 0),
                           (0x70cdfc, 0x40000000), (0x70ce00, 2)]:
            c['stores'].append((at + 19 * 88, value))
    add('house-needs/' + label, 0x455cc0, c, 'actual-composition')

# 97. Fixed4000 scan: absent, signed-free, and occupied/stale-link paths.
for label, actor_kind in [('none', 25), ('signed-free', 25),
                          ('stale', 77), ('occupied-then-free', 78)]:
    c = base([actor_kind])
    if label != 'none':
        wanted = 76 if actor_kind == 25 else 181 if actor_kind == 77 else 182
        building(c, kind=wanted)
        c['words'] += [(0x799a8e + B1, 0xffff if label == 'signed-free' else 7)]
    if label in ['stale', 'occupied-then-free']:
        c['bytes'] += [(0xbcd606 + A7, 1), (0xbcd602 + A7, actor_kind)]
        c['words'] += [(0xbcd652 + A7, 2 if label == 'stale' else 1)]
    if label == 'occupied-then-free':
        building(c, n=2, kind=182)
    add('vacant-service/' + label, 0x460ea0, c, 'actual-composition')

# 98. BYTE type indexes, all DWORD fields, and exact child result publication.
for kind in (0, 79, 255):
    c = base([7], [0x4cde40])
    c['bytes'] += [(0xbcd602 + A7, kind)]
    for n, at in enumerate([0x7090d0, 0x7090ec, 0x7090f4,
                             0x7090fc, 0x709100, 0x7090d4]):
        c['stores'].append((at + kind * 56, 0x80000000 + n * 0x10203))
    c['results'] = {0x4cde40: [0xfedcba98]}
    add('initialize-type/%d' % kind, 0x461f90, c)

# 99. Native top-level RET4 is independently checked by the common runner.
for label, percentage, timer in [('period-boundary', 100, 2),
                                 ('period-spawn', 100, 3),
                                 ('byte-wrap', 100, 255),
                                 ('low-staffing-period', 1, 47)]:
    c = worker(percentage)
    c['bytes'] += [(0x799a0a + B1, timer)]
    add('dispatch85/' + label, 0x4bed70, c)


def endpoints():
    return CHILDREN


def cases():
    return CASES
