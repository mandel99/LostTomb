"""Eight pure ABI-preflight recipes; shared cohort3 replay owns execution.

No image loading, build, emulation, installation or metadata writes occur here.
These bounded cases do not constitute the cohort semantic/gameplay comparison.
"""
import struct

SP, CONTEXT = 0x30008000, 0x32030000
OFFSETS = dict(edi=0, esi=4, ebp=8, ebx=16, edx=20, ecx=24, eax=28, eflags=32)
ENTRIES = {0x004F56D0, 0x0053FA40}
COMMON_GAPS = [
    'Necessary ABI preflight only; full semantic corpus and indirect consumers remain untested.',
    'Mapped nonfaulting synchronous contexts; no exceptions, guard faults, asynchronous observers or host execution.',
    'AL logical result is compared for4F56D0; its unconsumed high EAX bits and arithmetic flags are not promoted to a C result contract.',
]


def u16(value):
    return struct.pack('<H', value & 0xFFFF)


def u32(value):
    return struct.pack('<I', value & 0xFFFFFFFF)


def words(values):
    return b''.join(u32(value) for value in values)


def snapshot_case(entry, label, ordinal, receiver, arguments, stores,
                  watched, writes, result_mask, **extra):
    # Every actual register has an explicit nonzero arbitrary input. The saved
    # ESP is the native anchor before PUSHFD, not the separated C machine SP.
    eax = (0xA7C39E51 ^ (ordinal * 0x01020304)) & 0xFFFFFFFF
    edx = (0x6D2B7943 ^ (ordinal * 0x1020304)) & 0xFFFFFFFF
    ebx = (0x13579BDF ^ ordinal) & 0xFFFFFFFF
    ebp = (0x2468ACE1 ^ (ordinal << 4)) & 0xFFFFFFFF
    esi = (0x3579BDF1 ^ (ordinal << 8)) & 0xFFFFFFFF
    edi = (0x468ACE13 ^ (ordinal << 12)) & 0xFFFFFFFF
    flags = 0xAD7 if ordinal & 1 else 0x202  # DF clear on both routes.
    snapshot = words([edi, esi, ebp, SP - 4, ebx, edx, receiver, eax, flags])
    cleanup = 12 if entry == 0x0053FA40 else 0
    item = dict(
        id=f'c6-overlay-abi/{entry:08x}/{label}', entry=entry,
        receiver=receiver, arguments=list(arguments), named_arguments=[CONTEXT],
        initial_eax=eax, initial_edx=edx, initial_ebx=ebx,
        initial_ebp=ebp, initial_esi=esi, initial_edi=edi, eflags=flags,
        stores=[*stores, (CONTEXT, snapshot)], watched=list(watched),
        write_ranges=[*writes, (CONTEXT, 36)], result_mask=result_mask,
        result_memory={'named': [CONTEXT + OFFSETS['eax'], 4]},
        native_cleanup=cleanup, canonical_cleanup=cleanup, named_cleanup=0,
        canonical_registers_and_flags=False,
        context_outputs={'named': {name: [CONTEXT + OFFSETS[name], 4]
                                  for name in ('ebx', 'ebp', 'esi', 'edi')}},
        context_compare=['ebx', 'ebp', 'esi', 'edi'],
        machine_sp={'named': SP - 0x20000}, borrowed_native_stack=True,
        # A nonempty live x87 value must survive these integer-only paths.
        initial_x87=['00000000000000a0ff3f'], compare_x87_values=True,
        compare_x87_control=True, budget=250000,
        endpoints=[], gap_domains=list(COMMON_GAPS),
    )
    item.update(extra)
    return item


def summary_boundary(building):
    # Only this external typed query is controlled.4F56D0,4F5BD0 and4F5770
    # execute their actual complete native/C frame algorithms. Output[7]
    # is the exact field consumed after the complex-summary query.
    payload = [0] * 12
    payload[7] = 9
    return dict(
        token='overlay-abi/complex-summary48', native=0x004DF040,
        compiled_symbols=['FUN_004df040'], arity=2,
        native_cleanup=0, compiled_cleanup=0, result_kind='full32', value=47,
        pointer_args={1: 'complex-summary48'},
        writes_arg_bytes=[(1, 0, words(payload).hex())],
        observe=[(SP + 0x70, 4), (SP + 0x74, 4), (SP + 0x78, 4)],
    )


def resource_cases():
    out = []
    group, building = 7, 37
    descriptor = 0x005D19C8 + group * 16
    record = 0x007999D8 + building * 264
    variants = [
        ('skip-zero-and20', (0, 20, 0), 0xDEADBEEF, 4, 0),
        ('high-building-slot-reject', (30, 0, 0), 0, 4, 0),
        ('ordinary-slot70-accept', (30, 0, 0), 222, 4, 1),
        ('ordinary-slot70-reject', (30, 0, 0), 222, 2, 0),
    ]
    for ordinal, (label, resources, kind, requested, expected) in enumerate(variants):
        # The real slots are SP+70/+74/+78; only SP+4 is the declared group
        # argument. Never move the high slots onto the named C control stack.
        high_slots = words([requested, 0x7B, building])
        desc = bytearray(16)
        for index, resource in enumerate(resources):
            desc[6 + index * 2:8 + index * 2] = u16(resource)
        rec = bytearray(264)
        rec[16:18] = u16(kind)
        stores = [
            (descriptor, bytes(desc)), (record, bytes(rec)),
            (SP + 0x70, high_slots),
            (0x00EAAC28, u32(0)),
            (0x00EA6160 + 30 * 4, u32(3)),
            (0x00EA42F8 + 30 * 4, u32(0)),
        ]
        endpoints = [summary_boundary(building)] if kind == 222 else []
        endpoint_counts = {'overlay-abi/complex-summary48': 1} if endpoints else {}
        expected_args = ({'overlay-abi/complex-summary48':
                          [[building, ['stack-output', 'complex-summary48']]]}
                         if endpoints else {})
        out.append(snapshot_case(
            0x004F56D0, label, ordinal, 0x32001000 + ordinal * 0x100,
            [group], stores,
            [(descriptor, 16), (record, 264), (SP + 0x70, 12)], [], 0xFF,
            endpoints=endpoints, expected_endpoint_counts=endpoint_counts,
            expected_endpoint_arguments=expected_args,
            expected_masked_result=expected,
            native_case_domain=dict(
                descriptors=list(resources), actual_caller_slots={
                    'SP+0x70': requested, 'SP+0x74': 0x7B,
                    'SP+0x78': building}, building_kind=kind,
                children='Actual4F5BD0 and4F5770; typed4DF040 query boundary only'),
            gap_domains=[*COMMON_GAPS,
                'Resource25 arm consuming SP+0x74 remains unvisited: its actual4F1310 pyramid-summary frame graph is not replaced by a typed stub.',
                'Complex-summary contents are controlled at its external typed boundary; this preflight does not verify4DF040 semantics.'],
        ))
    return out


def sprite_cases():
    out = []
    # Two nonoverlapping complete receiver extents fit inside the shared
    # original-address RAM mapping. Seed only fields read by these rejection
    # paths; the record0/record1 windows and next_image are fully allocated.
    receivers = (0x00650000, 0x00980000)
    image_offset, next_image_offset = 0x180E48, 0x30F5CC
    manager = 0x00F55BB0
    variants = [
        ('receiverA-zero-image', receivers[0], 0, 2, -3, 0),
        ('receiverA-record1-empty', receivers[0], 1, 2, -3, 0),
        ('receiverB-limit-fallback-record0', receivers[1], 1, 1, -7, -11),
        ('receiverB-hard-image-bound', receivers[1], 25500, 2, -7, -11),
    ]
    for index, (label, receiver, image, limit, length0, length1) in enumerate(variants):
        records = bytearray(128)
        records[:4] = u32(length0)
        records[64:68] = u32(length1)
        # A fixed global receiver would publish different metadata, detecting
        # the previous hardcoded RESTORE_SPRITE_LIBRARIES facade.
        global_records = bytearray(128)
        global_records[:4] = u32(-31)
        global_records[64:68] = u32(-37)
        stores = [
            (receiver + next_image_offset, u32(limit)),
            (receiver + image_offset, bytes(records)),
            (manager + next_image_offset, u32(2)),
            (manager + image_offset, bytes(global_records)),
            (0x00D3B578, bytes([0x6D]) * 0x44),
        ]
        out.append(snapshot_case(
            0x0053FA40, label, index + 4, receiver,
            [image, 137 + index * 17, (-91 - index * 13) & 0xFFFFFFFF], stores,
            [(receiver + next_image_offset, 4),
             (receiver + image_offset, 128), (0x00D3B578, 0x44)],
            [(0x00D3B578, 0x44)], 0xFFFFFFFF,
            expected_eax=0,
            native_case_domain=dict(
                receiver=receiver, receiver_extent=0x30F5D8,
                mapped_input_fields=[[receiver + next_image_offset, 4],
                                     [receiver + image_offset, 128]],
                next_image=limit, image=image,
                metadata_lengths=[length0, length1],
                native_cleanup=12, all_helpers='None reached on selected rejection paths'),
            gap_domains=[*COMMON_GAPS,
                'Only zero/high image rejection, next_image fallback and nonpositive metadata length are visited.',
                'Positive metadata, sprite pixel decoding/clipping, format strings, library I/O and deeper helpers remain outside this ABI preflight.'],
        ))
    return out


def definitions(registry):
    present = {int(member['address'], 16) for member in registry['members']}
    out = [item for item in resource_cases() + sprite_cases() if item['entry'] in present]
    assert len({item['id'] for item in out}) == len(out)
    assert len(out) <= 8
    assert {item['entry'] for item in out} == ENTRIES & present
    return out
