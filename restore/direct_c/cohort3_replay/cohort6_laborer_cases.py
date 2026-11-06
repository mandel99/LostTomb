"""Finite139-case cohort6 labourer recipes; shared replay owns execution.

Pure definitions only. No build, image loading, native/C execution, public
source changes, metadata writes, or Git operations occur in this module.
Every selected raw body and reached child frame algorithm executes. There are
no endpoint stubs, generated expected gameplay algorithms, or fake FP inputs.
"""
import struct

SP, S, CONTEXT = 0x30008000, 0x32000000, 0x32048000
SELF, VTABLE, OBJECTS, OUTPUT = S + 0x1000, S + 0x80, S + 0x3000, S + 0x40000
BUILDING_BASE, ACTOR_BASE = 0x7999D8, 0xBCD600
ROOT, BUILDING, ACTOR = 23, 25, 41
OFFSETS = dict(edi=0, esi=4, ebp=8, ebx=16, edx=20, ecx=24, eax=28, eflags=32)
IMMUTABLE_INPUTS = {
    'runtime/actor_laborer_model.c': 'e7cd153ef2a8e5186974a33266826225139902a0d33352139d2722fb26981191',
    'runtime/actor_laborer.c': '6525218d6b37456c47349a3df4bf79d84e13bd5050df02c361d7a2528308dcda',
}
NATIVE_PIN = 'fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'

# Native selectors stay native on every route: these slots are read by the
# existing raw dispatcher, not called through compiled hardware function PCs.
VTABLE_METHODS = {0x18: 0x004E6BD0, 0x24: 0x005477E0, 0x38: 0x00545F60}
METHOD_CLEANUP = {
    0x401BE0: 4, 0x40ECF0: 4, 0x40F0D0: 12, 0x417640: 4, 0x417940: 12,
    0x4BEEB0: 4, 0x4E6BD0: 4, 0x4E6E70: 12,
    0x545740: 8, 0x545BC0: 4, 0x546790: 12, 0x547870: 8,
    0x547B60: 4, 0x54F470: 8, 0x54FB20: 4, 0x555E80: 8,
    0x556410: 4, 0x558BE0: 8, 0x5590A0: 4,
}
# Address, exact descriptor table, native dispatch category. The25 bodies
# share one authenticated algorithm with these distinct table specializations.
FAMILIES = (
    (0x4233F0, 0x5BE508, 1), (0x423530, 0x5BE538, 2),
    (0x423670, 0x5BE568, 3), (0x4237B0, 0x5BE748, 4),
    (0x4238F0, 0x5BE778, 5), (0x423A30, 0x5BE598, 15),
    (0x423B70, 0x5BE5C8, 16), (0x423CB0, 0x5BE5F8, 0),
    (0x423DF0, 0x5BE628, 6), (0x423F30, 0x5BE7A8, 7),
    (0x424070, 0x5BE7D8, 8), (0x4241B0, 0x5BE658, 9),
    (0x4242F0, 0x5BE688, 10), (0x424430, 0x5BE898, 11),
    (0x424570, 0x5BE808, 12), (0x4246B0, 0x5BE8C8, 13),
    (0x4247F0, 0x5BE8F8, 14), (0x424930, 0x5BE6B8, 19),
    (0x424A70, 0x5BE6E8, 20), (0x424BB0, 0x5BE718, 21),
    (0x424CF0, 0x5BE838, 22), (0x424E30, 0x5BE868, 23),
    (0x424F70, 0x5BE928, 24), (0x4250B0, 0x5BE958, 25),
    (0x4251F0, 0x5BE988, 26),
)
ENTRIES = set(METHOD_CLEANUP) | {row[0] for row in FAMILIES} | {0x4223F0, 0x547E20}
COMMON_GAPS = [
    'Finite mapped/nonfaulting synchronous inputs only; no complete gameplay, concurrency, throwing or guard-fault claim.',
    'Constructed vtables select actual recognized same-signature native methods; every reached native/C child executes. Other virtual targets and exhaustive original class configurations remain unvisited.',
    'Native selectors in raw-dispatch vtable slots are intentionally identical across images; no compiled-PC relocation is applied.',
    'Unconsumed volatile registers and undefined flags are not invented logical API outputs. Nonvolatile registers, native RETn, mapped effects and live x87/CW are compared.',
]


def u16(value):
    return struct.pack('<H', value & 0xFFFF)


def u32(value):
    return struct.pack('<I', value & 0xFFFFFFFF)


def words(values):
    return b''.join(u32(value) for value in values)


def set16(data, offset, value):
    data[offset:offset + 2] = u16(value)


def set32(data, offset, value):
    data[offset:offset + 4] = u32(value)


def building_address(index):
    return BUILDING_BASE + index * 264


def actor_address(index):
    return ACTOR_BASE + index * 388


def receiver_stores(receiver=SELF, extent=0x200):
    obj = bytearray(extent)
    obj[:4] = u32(VTABLE)
    vt = bytearray(0x40)
    for offset, target in VTABLE_METHODS.items():
        set32(vt, offset, target)
    return [(receiver, bytes(obj)), (VTABLE, bytes(vt))]


def record_array(root=ROOT, count=40, stage=100, progress=0, flags=1):
    data = bytearray(count * 264)
    for index in range(count):
        offset = index * 264
        set32(data, offset + 0x58, stage)
        set32(data, offset + 0x60, progress)
        set32(data, offset + 0x64, flags)
        # Six relative-neighbor slots are individually readable; negative
        # values are native no-neighbor sentinels, not null host pointers.
        for neighbor in range(6):
            set32(data, offset + 0x6C + neighbor * 4, -1)
    return data


def actors():
    data = bytearray(3 * 388)
    for index, (kind, state) in enumerate(((0x51, 9), (0x6C, 12), (0x55, 9))):
        base = index * 388
        data[base + 2] = kind
        data[base + 6] = 1
        data[base + 8] = state
        data[base + 0x30] = state
    return data


def captured_case(entry, label, ordinal, arguments, stores=(), watched=(),
                  writes=(), receiver=SELF, result_mask=None, domain=None,
                  gaps=(), fpcw=0x37F, **extra):
    # Actual initial hardware GPRs and the9-word PUSHAD/PUSHFD image agree.
    # Original return/argument slots remain at SP, while the named helper's
    # physical C control frame is below the whole64KB native workspace.
    eax = (0xD3C597A1 ^ (ordinal * 0x010203)) & 0xFFFFFFFF
    edx = (0xA16F3C95 ^ (ordinal * 0x010305)) & 0xFFFFFFFF
    ebx = (0x13579BDF ^ (ordinal << 2)) & 0xFFFFFFFF
    ebp = (0x2468ACE1 ^ (ordinal << 4)) & 0xFFFFFFFF
    esi = (0x3579BDF1 ^ (ordinal << 6)) & 0xFFFFFFFF
    edi = (0x468ACE13 ^ (ordinal << 8)) & 0xFFFFFFFF
    flags = 0xAD7 if ordinal & 1 else 0x202
    snapshot = words([edi, esi, ebp, SP - 4, ebx, edx, receiver, eax, flags])
    cleanup = METHOD_CLEANUP.get(entry, 12 if entry == 0x547E20 else 0)
    item = dict(
        id=f'c6-laborer/{entry:08x}/{label}', entry=entry,
        receiver=receiver, arguments=list(arguments), named_arguments=[CONTEXT],
        initial_eax=eax, initial_edx=edx, initial_ebx=ebx,
        initial_ebp=ebp, initial_esi=esi, initial_edi=edi, eflags=flags,
        stores=[*( (address,bytes(payload)) for address,payload in stores), (CONTEXT, snapshot)], watched=list(watched),
        write_ranges=[*writes, (CONTEXT, 36)], result_mask=result_mask,
        result_memory={'named': [CONTEXT + 28, 4]},
        native_cleanup=cleanup, named_cleanup=0, canonical_cleanup=cleanup,
        canonical_registers_and_flags=False,
        context_outputs={'named': {name: [CONTEXT + OFFSETS[name], 4]
                                  for name in ('ebx', 'ebp', 'esi', 'edi')}},
        context_compare=['ebx', 'ebp', 'esi', 'edi'],
        machine_sp={'named': SP - 0x20000}, borrowed_native_stack=True,
        initial_fpcw=fpcw,
        initial_x87=['00000000000000a0ff3f', '00000000000000a000c0'],
        compare_x87_values=True, compare_x87_control=True,
        endpoints=[], budget=12000000,
        native_case_domain=domain or {}, gap_domains=[*COMMON_GAPS, *gaps],
    )
    item.update(extra)
    return item


def increment_cases():
    out = []
    specs = (
        (0x401BE0, (0, 55, 56), 16, 0x54),
        (0x40ECF0, (0, 20, 21), 25, 0x58),
        (0x417640, (0, 18, 19), 9, 0x58),
        (0x4E6BD0, (0, 8, 9), 1, 0x58),
    )
    for family, (entry, levels, count, field) in enumerate(specs):
        for variant, level in enumerate(levels):
            receiver = SP + 0x200 if variant == 2 else SELF
            records = record_array(count=max(count + 4, 32), stage=100)
            # Root delta2 exercises the caller's true building-relative field.
            argument = BUILDING if entry != 0x4E6BD0 else ROOT
            set16(records, (argument - ROOT) * 264 + 0x12,
                  argument - ROOT if entry != 0x4E6BD0 else 0)
            set32(records, field, level)
            if field == 0x54:
                set32(records, 0x58, 100)
            stores = [*receiver_stores(receiver), (building_address(ROOT), bytes(records))]
            watched = [(receiver, 0x200), (VTABLE, 0x40),
                       (building_address(ROOT), len(records))]
            out.append(captured_case(
                entry, f'level-{level}', family * 3 + variant, [argument],
                stores, watched, [(building_address(ROOT), len(records))],
                receiver=receiver,
                domain=dict(field_offset=field, level=level, root=ROOT,
                            building=argument, expected_virtual_extent=count,
                            receiver_alias='original-SP+200' if receiver != SELF else 'mapped-object',
                            virtual18='Actual4E6BD0, with bounded stage9 guard/recursion'),
                gaps=['Construction/material metadata uses pinned native seed; no exhaustive phase table or original-class-vtable claim.'],
            ))
    return out


def worker_25_cases():
    out = []
    for variant, (label, kind, counter) in enumerate((
            ('other-worker-state', 0x4F, 19),
            ('counter-100-no-advance', 0x55, 99),
            ('counter-101-with-incomplete-neighbor', 0x55, 100))):
        records = record_array(count=29, stage=0)
        set16(records, (BUILDING - ROOT) * 264 + 0x12, BUILDING - ROOT)
        # At least one following part remains incomplete after the real
        # bounded refresh child. The final coordinate-getter path is separate.
        actor = bytearray(388)
        actor[2], actor[6], actor[0x30] = kind, 1, 9
        set16(actor, 0x2E, counter)
        stores = [*receiver_stores(), (building_address(ROOT), bytes(records)),
                  (actor_address(ACTOR), bytes(actor))]
        watched = [(SELF, 0x200), (VTABLE, 0x40),
                   (building_address(ROOT), len(records)), (actor_address(ACTOR), 388)]
        out.append(captured_case(
            0x40F0D0, label, 20 + variant, [BUILDING, ACTOR, 17], stores,
            watched, [(building_address(ROOT), len(records)), (actor_address(ACTOR), 388)],
            domain=dict(root=ROOT, building=BUILDING, actor=ACTOR,
                        actor_kind=kind, incoming_counter=counter,
                        real_child='4E6BD0 via native virtual18, no controlled endpoint'),
            gaps=['All25-parts-complete transition,4ECF0 follow-up and virtual20 coordinate getter remain unvisited.'],
        ))
    return out


def descriptor_table(base, count, phase=2, claimed=ACTOR, remaining=3):
    table = bytearray(count * 16)
    object_bytes = bytearray(count * 32)
    for index in range(count):
        at = index * 16
        set32(table, at, phase + index)
        set32(table, at + 4, 0x55)
        set32(table, at + 8, OBJECTS + index * 32)
        set32(object_bytes, index * 32 + 12, remaining)
        set16(object_bytes, index * 32 + 18, claimed if index == 0 else ACTOR + 1)
    return [(base, bytes(table)), (OBJECTS, bytes(object_bytes))]


def base_assignment_cases():
    out = []
    # Actual child4E20/4F1120 descriptor loops execute, using mapped claimed
    # objects to avoid inventing a raw-frame construction/registration endpoint.
    for family, (entry, table, count) in enumerate((
            (0x417940, 0x5AD210, 27), (0x4E6E70, 0x5B0058, 15))):
        for variant, label in enumerate(('receiver-guard', 'claimed-elsewhere', 'claimed-decrement')):
            phase = 0 if entry == 0x417940 else 2
            records = record_array(count=5, stage=phase)
            set16(records, (BUILDING - ROOT) * 264 + 0x12, BUILDING - ROOT)
            obj = bytearray(0x200)
            set32(obj, 0, VTABLE)
            set32(obj, 0x58 + (BUILDING - ROOT) * 52, 1 if variant == 0 else 0)
            actor = bytearray(388)
            actor[2], actor[6], actor[0x30] = 0x55, 1, 9
            claimed = ACTOR if variant == 2 else ACTOR + 1
            stores = [*receiver_stores(), (SELF, bytes(obj)),
                      (building_address(ROOT), bytes(records)),
                      (actor_address(ACTOR), bytes(actor)),
                      *descriptor_table(table, count, phase, claimed)]
            watched = [(SELF, 0x200), (VTABLE, 0x40),
                       (building_address(ROOT), len(records)), (actor_address(ACTOR), 388),
                       (table, count * 16), (OBJECTS, count * 32)]
            out.append(captured_case(
                entry, label, 30 + family * 3 + variant, [BUILDING, ACTOR, 23],
                stores, watched, [(actor_address(ACTOR), 388), (OBJECTS, count * 32),
                                  (building_address(ROOT), len(records))],
                domain=dict(relative_part=BUILDING - ROOT, phase=phase,
                            guarded=variant == 0, descriptor_table=table,
                            descriptor_entries=count, first_claim=claimed,
                            real_child='420E20' if entry == 0x417940 else '421120'),
                gaps=['Unclaimed descriptor construction/registration and depleted-object follow-up are not visited; no raw-frame endpoint replaces them.'],
            ))
    return out


def progress_cases():
    out = []
    counts = {0x545740: 9, 0x558BE0: 10, 0x555E80: 21, 0x54F470: 31}
    for family, entry in enumerate((0x546790, *counts)):
        variants = [
            ('fractional-cw-nearest', 2, 0x37F, OUTPUT),
            ('saturated-cw-down', 4, 0x77F, OUTPUT),
            ('caller-stack-output-cw-up', 1, 0xB7F, SP + 0x180),
        ]
        if entry == 0x546790:
            variants.append(('null-output-live-x87', 3, 0x37F, 0))
        for variant, (label, level, cw, output) in enumerate(variants):
            count = counts.get(entry, 3)
            records = record_array(count=count + 4, progress=level)
            set16(records, (BUILDING - ROOT) * 264 + 0x12, BUILDING - ROOT)
            set32(records, 0x98, 0x13579BDF)
            for index in range(count):
                set32(records, index * 264 + 0x5C, ACTOR + index % 3)
            stores = [*receiver_stores(), (building_address(ROOT), bytes(records)),
                      (actor_address(ACTOR), actors())]
            watched = [(SELF, 0x200), (VTABLE, 0x40),
                       (building_address(ROOT), len(records)), (actor_address(ACTOR), 3 * 388)]
            writes = [(building_address(ROOT), len(records))]
            if output:
                stores.append((output, bytes([0xA6]) * 28))
                watched.append((output, 28))
                writes.append((output, 28))
            arguments = [BUILDING, count, output] if entry == 0x546790 else [BUILDING, output]
            out.append(captured_case(
                entry, label, 40 + family * 4 + variant, arguments,
                stores, watched, writes, result_mask=0xFFFFFFFF, fpcw=cw,
                domain=dict(root=ROOT, building=BUILDING, count=count,
                            summed_levels=level, output=output, inherited_cw=cw,
                            live_x87=['+1.25', '-2.5'],
                            real_virtual24='5477E0 worker predicate; native root argument retained',
                            worker_types=[0x51, 0x6C, 0x55]),
                gaps=['Only positive counts3/9/10/21/31 and finite integer levels are visited; zero-count FP exceptions and unsupported worker states remain outside this domain.'],
            ))
    return out


def stage_cases():
    out = []
    counts = {0x545BC0: 167, 0x5590A0: 158, 0x556410: 437, 0x54FB20: 513}
    for family, entry in enumerate((0x547870, *counts)):
        for variant, level in enumerate((0, 2, 4)):
            count = counts.get(entry, 5)
            records = record_array(count=count + 4, stage=100, progress=0, flags=1)
            set16(records, (BUILDING - ROOT) * 264 + 0x12, BUILDING - ROOT)
            offset = (BUILDING - ROOT) * 264
            set32(records, offset + 0x60, level)
            if level == 2:
                # Actual transition-to3 traverses all six slots; two valid
                # relative neighbors and four sentinels exercise both arms.
                for neighbor, relative in enumerate((-1, 1, -1, 3, -1, -1)):
                    set32(records, offset + 0x6C + neighbor * 4, relative)
            stores = [*receiver_stores(), (building_address(ROOT), bytes(records))]
            watched = [(SELF, 0x200), (VTABLE, 0x40),
                       (building_address(ROOT), len(records))]
            arguments = [BUILDING, count] if entry == 0x547870 else [BUILDING]
            out.append(captured_case(
                entry, f'progress-{level}', 65 + family * 3 + variant,
                arguments, stores, watched, [(building_address(ROOT), len(records))],
                domain=dict(root=ROOT, building=BUILDING, progress=level,
                            virtual_extent=count, refresh_stage=100,
                            completion_flag=1, real_virtual18='4E6BD0 stage guard'),
                gaps=['Completion flag remains set;4EA930, subtype dispatch, final progress recomputation and footprint completion are not visited.'],
            ))
    return out


def refresh_group_cases():
    out = []
    for variant, delta in enumerate((0, 2, -1)):
        argument = ROOT + delta
        records = record_array(root=ROOT - 2, count=18, stage=100)
        offset = (argument - (ROOT - 2)) * 264
        set16(records, offset + 0x12, delta)
        receiver = SP + 0x200 if variant == 2 else SELF
        stores = [*receiver_stores(receiver), (building_address(ROOT - 2), bytes(records))]
        out.append(captured_case(
            0x547B60, f'relative-{delta}', 85 + variant, [argument],
            stores, [(receiver, 0x200), (VTABLE, 0x40),
                     (building_address(ROOT - 2), len(records))],
            [(building_address(ROOT - 2), len(records))], receiver=receiver,
            domain=dict(building=argument, signed_relative=delta, root=ROOT,
                        real_virtual38='545F60 count9', real_virtual18='4E6BD0 stage100'),
            gaps=['Only the actual nine-record count getter is selected; other count getters and completion callbacks remain unvisited.'],
        ))
    return out


def assignment_scan_cases():
    out = []
    for variant, (mode, maximum) in enumerate(((1, 0), (2, 3), (0, 2))):
        records = record_array(root=1, count=max(maximum, 1), stage=100)
        # Status0 makes every scanned record inactive. No routing/selection
        # callback is invented to fake a successful complex assignment.
        actor = bytearray(388)
        actor[2], actor[6], actor[0x30] = 0x55, 1, 9
        stores = [*receiver_stores(), (SELF + 0x32FB4, u32(mode)),
                  (0x78D790, u32(maximum)), (0x78D7F0, u32(0x13579BDF)),
                  (building_address(1), bytes(records)),
                  (actor_address(ACTOR), bytes(actor))]
        watched = [(SELF, 0x200), (SELF + 0x32FB4, 4),
                   (0x78D790, 4), (0x78D7F0, 4),
                   (building_address(1), len(records)), (actor_address(ACTOR), 388)]
        out.append(captured_case(
            0x4BEEB0, f'mode-{mode}-inactive-count-{maximum}', 90 + variant,
            [ACTOR], stores, watched,
            [(0x78D7F0, 4), (actor_address(ACTOR), 388)],
            domain=dict(receiver_mode=mode, maximum_building=maximum,
                        active_records=0, actor=ACTOR,
                        selection_packet='Actual local packet; no eligible entries'),
            gaps=['Eligible-building collection, priority/distance sorting, routing, retries and every successful assignment branch remain outside these three inactive/empty scan cases.'],
        ))
    return out


def part_fixture(table, category, variant):
    phase = 2
    records = record_array(count=5, stage=100)
    offset = (BUILDING - ROOT) * 264
    set16(records, offset + 0x12, 0)
    set32(records, offset + 0x60, phase)
    set32(records, offset + 0x68, category)
    set32(records, offset + 0x5C, ACTOR)
    set32(records, offset + 0x80 + phase * 4, 1 if variant == 1 else 3)
    if variant == 2:
        # Claimed actor matches the first descriptor, but its phase differs.
        descriptor_phase = phase + 1
    else:
        descriptor_phase = phase
    actor = bytearray(388)
    actor[2], actor[6], actor[0x30] = 0x55, 1, 9
    stores = [*receiver_stores(), (building_address(ROOT), bytes(records)),
              (actor_address(ACTOR), bytes(actor)),
              *descriptor_table(table, 3, descriptor_phase, ACTOR)]
    watched = [(SELF, 0x200), (building_address(ROOT), len(records)),
               (actor_address(ACTOR), 388), (table, 48), (OBJECTS, 96)]
    writes = [(building_address(ROOT), len(records)), (actor_address(ACTOR), 388)]
    return stores, watched, writes


def specialized_cases():
    out = []
    labels = ('claimed-work-remaining', 'depleted-real4223F0', 'claimed-phase-mismatch')
    for family, (entry, table, category) in enumerate(FAMILIES):
        for variant, label in enumerate(labels):
            stores, watched, writes = part_fixture(table, category, variant)
            out.append(captured_case(
                entry, label, 100 + family * 3 + variant, [BUILDING, ACTOR, 27],
                stores, watched, writes,
                domain=dict(descriptor_table=table, category=category,
                            native_specialization=entry, descriptor_entries=3,
                            actor=ACTOR, claimed=True, phase=2,
                            remaining=1 if variant == 1 else 3,
                            descriptor_phase=3 if variant == 2 else 2,
                            child='Actual4223F0 on depletion; unrecognized building kind0 has no final refresh'),
                gaps=['Unclaimed-object allocation/registration and successful final class-specific refresh remain unvisited. All25 actual descriptor-table specializations execute; no family stub is used.'],
            ))
    return out


def direct_caller_cases():
    out = []
    for variant, (category, remaining) in enumerate(((1, 3), (1, 0), (30, 0))):
        table = FAMILIES[0][1]
        stores, watched, writes = part_fixture(table, category, 0)
        stores.append((building_address(BUILDING) + 0x88, u32(remaining)))
        out.append(captured_case(
            0x4223F0, f'category-{category}-remaining-{remaining}', 180 + variant,
            [BUILDING], stores, watched, writes,
            domain=dict(category=category, phase=2, remaining=remaining,
                        actual_descriptor_table=table, building_kind=0),
            gaps=['Only category1 and out-of-range30 are exercised; class-specific fallback refresh is excluded by building kind0. AL is not promoted to an invented public result width.'],
        ))
    selected = (FAMILIES[0], FAMILIES[14], FAMILIES[-1])
    for variant, (entry, table, category) in enumerate(selected):
        stores, watched, writes = part_fixture(table, category, variant)
        out.append(captured_case(
            0x547E20, f'category-{category}-real-{entry:08x}', 185 + variant,
            [BUILDING, ACTOR, 29], stores, watched, writes,
            domain=dict(category=category, real_specialized_child=entry,
                        descriptor_table=table,
                        connected_path=('claimed-remaining', 'depleted4223F0', 'phase-mismatch')[variant]),
            gaps=['Three real dispatch categories are tested here; remaining specialized entries are exercised directly by the parameterized25-family cases.'],
        ))
    return out


def definitions(registry):
    present = {int(member['address'], 16) for member in registry['members']}
    all_cases = (increment_cases() + worker_25_cases() + base_assignment_cases()
                 + progress_cases() + stage_cases() + refresh_group_cases()
                 + assignment_scan_cases() + specialized_cases() + direct_caller_cases())
    assert len(all_cases) == 139
    assert {item['entry'] for item in all_cases} == ENTRIES
    assert len(ENTRIES) == 46
    out = [item for item in all_cases if item['entry'] in present]
    assert len({item['id'] for item in out}) == len(out)
    assert {item['entry'] for item in out} == ENTRIES & present
    assert all(not item['endpoints'] for item in out)
    return out
