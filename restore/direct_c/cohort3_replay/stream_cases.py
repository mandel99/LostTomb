"""Pure stream replay recipes. Importing this module does not execute an image.

All selected constructors/destructors, their static children, and buffer vtable
deleting destructors execute. Only allocation, free and an explicitly supplied
cdecl release callback are controlled boundaries. Native and named receiver
arguments are separate. The shared runner owns memory/register/stack comparison.
"""
import struct

SCRATCH = 0x32000000
SELF = SCRATCH + 0x1000
FRAME = SCRATCH + 0x2080
BUFFER = SCRATCH + 0x3000
NEW_BUFFER = SCRATCH + 0x4000
PAYLOAD = SCRATCH + 0x5000
SECOND_PAYLOAD = SCRATCH + 0x6000
TABLE = SCRATCH + 0x100
SECOND_TABLE = SCRATCH + 0x140
RELEASE_TARGET = 0x31000100
EXTENT = 0x180
PIN = 'fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'

IOS_CTOR, IOS_DTOR, IOS_INIT = 0x5605FA, 0x560648, 0x560671
FILE_CTOR, BUFFER_CTOR = 0x5606A0, 0x560F02
FILE_DTOR, BUFFER_DTOR, PENDING = 0x5606E2, 0x560F4B, 0x561090
FSTREAM_CTOR, OSTREAM_CTOR = 0x5609DA, 0x560D22
ISTREAM_CTOR, IOSTREAM_CTOR = 0x5611C8, 0x561278
OSTREAM_DTOR, ISTREAM_DTOR = 0x560D87, 0x56123E
IOSTREAM_DTOR, FSTREAM_DTOR, COMPLETE_DTOR = 0x5612ED, 0x560A9B, 0x53DFD0
SCALAR_DELETES = {0x56062C: 'ios', 0x5606C6: 'filebuf', 0x560F2F: 'buffer'}
VIRTUAL_DELETES = {0x560A70: 20, 0x560CF7: 8, 0x56119D: 12, 0x56124D: 20}
CONDITIONAL_CLEANUPS = {0x56E190: 20, 0x56E1C0: 8,
                        0x56E1E4: 12, 0x56E208: 20}
CLEANUPS = tuple(CONDITIONAL_CLEANUPS) + (0x56B7D0, 0x56E17C, 0x56E222, 0x56E238)
ENTRIES = (IOS_CTOR, IOS_DTOR, IOS_INIT, FILE_CTOR, BUFFER_CTOR,
           FILE_DTOR, BUFFER_DTOR, PENDING, FSTREAM_CTOR, OSTREAM_CTOR,
           ISTREAM_CTOR, IOSTREAM_CTOR, OSTREAM_DTOR, ISTREAM_DTOR,
           IOSTREAM_DTOR, FSTREAM_DTOR, COMPLETE_DTOR,
           *SCALAR_DELETES, *VIRTUAL_DELETES, *CLEANUPS)


def _u32(value):
    return struct.pack('<I', value & 0xFFFFFFFF)


def _pattern(length):
    # Nonzero, nonuniform untouched bytes make byte-vs-DWORD stores visible.
    return bytes((i * 29 + 7) & 255 for i in range(length))


def _words(fixture, address, values):
    fixture['stores'].extend((address + offset, _u32(value))
                             for offset, value in values.items())


def _ios(fixture, address, owned=0, buffer=0, state=0xC31A5AF7):
    _words(fixture, address, {0: 0x570534, 4: buffer, 8: state,
                             28: owned, 36: 0xB4628CA0})


def _buffer(fixture, address, kind='buffer', owned=0, file_owned=0,
            payload=PAYLOAD, pending='none'):
    _words(fixture, address, {
        0: 0x57053C if kind == 'filebuf' else 0x570598,
        4: owned, 8: 0x82A31947, 12: 0xF1E2D3C4, 16: payload,
        20: 0xA1B2C3D4, 24: 0x80000001, 28: 0x80000001,
        32: 0x75642310, 36: 0xC49176A3, 40: 0x80000001, 44: 0x80000001,
        48: file_owned, 52: 0x39A7D261, 56: 0x8E5B2AC4,
        60: 0xA9B8C7D6, 64: 0x10293847, 68: 0})
    if pending == 'read':
        _words(fixture, address, {40: 0x7FFFFFFF, 44: 0x80000000})
    elif pending == 'write':
        _words(fixture, address, {24: 0x7FFFFFFF, 28: 0x80000000,
                                  40: 0xFFFFFFFF, 44: 0x80000000})


def _layout(fixture, root, displacement, combined=False):
    # These are vbtable displacements, not code pointers or vtable slots.
    _words(fixture, TABLE, {0: 0xD24A6C81, 4: displacement})
    _words(fixture, SECOND_TABLE,
           {0: 0x4916AD83, 4: displacement - 12})
    _words(fixture, root, {0: TABLE})
    if combined:
        _words(fixture, root, {12: SECOND_TABLE})


def _fixture(root=SELF):
    stores = [(FRAME - 0x80, _pattern(0x100)),
              (root - 16, _pattern(EXTENT + 32)),
              (BUFFER - 16, _pattern(0xB0)),
              (NEW_BUFFER - 16, _pattern(0xB0)),
              (PAYLOAD - 16, _pattern(0xA0)),
              (SECOND_PAYLOAD - 16, _pattern(0xA0)),
              (TABLE - 16, _pattern(0x80))]
    watched = [(root - 16, EXTENT + 32), (FRAME - 0x80, 0x100),
               (BUFFER - 16, 0xB0), (NEW_BUFFER - 16, 0xB0),
               (PAYLOAD - 16, 0xA0), (SECOND_PAYLOAD - 16, 0xA0),
               (TABLE - 16, 0x80)]
    # Native bytes for these vbtables are saved in the constructor packet's
    # offline-byte-binding.json and authenticated in the cohort REA records.
    stores += [(0x570570, bytes.fromhex('00000000080000000000000014000000')),
               (0x57058C, bytes.fromhex('0000000008000000')),
               (0x5705CC, bytes.fromhex('000000000c000000')),
               (0x5705DC, bytes.fromhex('00000000080000000000000014000000'))]
    watched += [(0x570570, 16), (0x57058C, 8),
                (0x5705CC, 8), (0x5705DC, 16)]
    fixture = dict(root=root, stores=stores, watched=watched,
                   write_ranges=[(root, EXTENT), (BUFFER, 0x90),
                                 (NEW_BUFFER, 0x90), (TABLE, 8),
                                 (SECOND_TABLE, 8), (0, 4)],
                   function_pointers=[
                       dict(address=0x57053C, native=0x5606C6,
                            compiled_symbol='FUN_005606c6'),
                       dict(address=0x570598, native=0x560F2F,
                            compiled_symbol='FUN_00560f2f')])
    _buffer(fixture, BUFFER)
    _buffer(fixture, NEW_BUFFER)
    return fixture


def _observe(root, ios=None):
    addresses = [(root, 4), (root + 4, 4), (root + 16, 4),
                 (root + 48, 4), (TABLE + 4, 4), (SECOND_TABLE + 4, 4),
                 (BUFFER, 4), (BUFFER + 4, 4), (BUFFER + 16, 4),
                 (NEW_BUFFER, 4), (NEW_BUFFER + 4, 4)]
    if ios is not None:
        addresses += [(ios, 4), (ios + 4, 4), (ios + 8, 4),
                      (ios + 28, 4), (ios + 36, 4)]
    # Remove duplicate observations, retaining event order.
    return list(dict.fromkeys(addresses))


def _free(fixture, ios=None, mutations=()):
    return dict(token='free', native=0x561A2B,
                compiled_symbols=['FUN_00561a2b'], arity=1,
                native_cleanup=0, compiled_cleanup=0, result_kind='void',
                pointer_args={}, writes_args=[], mutations=list(mutations),
                observe=_observe(fixture['root'], ios),
                clobber_ecx=0xD1C2B3A4, clobber_edx=0x9A8B7C6D)


def _allocator(fixture, value, ios):
    return dict(token='allocate', native=0x561A1D,
                compiled_symbols=['FUN_00561a1d'], arity=1,
                native_cleanup=0, compiled_cleanup=0, result_kind='full32',
                value=value, pointer_args={}, writes_args=[], mutations=[],
                observe=_observe(fixture['root'], ios),
                clobber_ecx=0x84C2A731, clobber_edx=0x72B493D1)


def _release(fixture, mutations=()):
    # This callback is a fixture-supplied external function, not a stream body.
    # The allocator symbol is just an existing compiled executable boundary;
    # these callback-only cases never allocate. Its return value is unobserved.
    return dict(token='release', native=RELEASE_TARGET,
                compiled_symbols=['FUN_00561a1d'], arity=1,
                native_cleanup=0, compiled_cleanup=0, result_kind='void',
                native_receiver=PAYLOAD, pointer_args={}, writes_args=[],
                mutations=list(mutations), observe=_observe(fixture['root']),
                clobber_ecx=0x925C7AE1, clobber_edx=0xF3816B24)


def _split_watch(watched, address, width):
    result = []
    for start, length in watched:
        end = start + length
        if address >= end or address + width <= start:
            result.append((start, length))
        else:
            if start < address:
                result.append((start, address - start))
            if address + width < end:
                result.append((address + width, end - address - width))
    return result


def _case(entry, label, fixture, receiver, arguments=(), result=None,
          full_result=False, free_count=0, free_mutations=(), ios=None,
          allocate=None, callback=False, release_mutations=(), features=None):
    endpoints = [_free(fixture, ios, free_mutations)]
    counts = {'free': free_count}
    if allocate is not None:
        endpoints.append(_allocator(fixture, allocate, ios))
        counts['allocate'] = 1
    if callback:
        assert allocate is None
        slot = fixture['root'] + 68
        fixture['function_pointers'].append(
            dict(address=slot, native=RELEASE_TARGET,
                 compiled_symbol='FUN_00561a1d'))
        # Relocated code slots have their own unchanged-value check in runner.
        # Every other object and canary byte remains in ordinary comparison.
        fixture['watched'] = _split_watch(fixture['watched'], slot, 4)
        endpoints.append(_release(fixture, release_mutations))
        counts['release'] = 1
    cleanup = len(arguments) * 4
    if entry in CLEANUPS:
        assert not arguments
        named = [FRAME]
    else:
        named = [receiver, *arguments]
    case = dict(id=f'stream/{entry:08x}/{label}', entry=entry,
                receiver=receiver, arguments=list(arguments),
                named_arguments=named,
                eflags=0xAD7 if sum(label.encode()) & 1 else 0x202,
                stores=fixture['stores'], watched=fixture['watched'],
                write_ranges=fixture['write_ranges'],
                result_mask=0xFFFFFFFF if full_result else None,
                native_cleanup=cleanup, named_cleanup=0,
                canonical_cleanup=cleanup, canonical_registers_and_flags=False,
                endpoints=endpoints, expected_endpoint_counts=counts,
                function_pointers=fixture['function_pointers'],
                initial_fs0=0xFFFFFFFF,
                features=features or {})
    if full_result:
        assert result is not None
        case['expected_eax'] = result & 0xFFFFFFFF
    if entry in CLEANUPS:
        case['initial_ebp'] = FRAME
    return case


def _simple_constructs():
    out = []
    for entry in (IOS_CTOR, FILE_CTOR, BUFFER_CTOR):
        for label in ('sentinels-a', 'sentinels-b'):
            fixture = _fixture()
            if label.endswith('b'):
                fixture['stores'].append((SELF, b'\x5A' * EXTENT))
            out.append(_case(entry, label, fixture, SELF,
                             result=SELF, full_result=True,
                             features=dict(untouched_bytes=True)))
    return out


def _ios_cases():
    out = []
    profiles = [('unowned', 0, BUFFER, 'buffer', 0, 0),
                ('owned-null', 0x80000000, 0, 'buffer', 0, 0),
                ('owned-buffer', 2, BUFFER, 'buffer', 0, 1),
                ('owned-filebuf', 0x80000000, BUFFER, 'filebuf', 0, 1),
                ('owned-filebuf-both', 1, BUFFER, 'filebuf', 1, 3)]
    for label, owned, old, kind, payload_owned, frees in profiles:
        fixture = _fixture()
        _ios(fixture, SELF, owned, old)
        _buffer(fixture, BUFFER, kind, payload_owned, payload_owned)
        mutations = [(SELF + 4, 4, NEW_BUFFER), (SELF + 8, 4, 0x73AC15E6)]
        out.append(_case(IOS_DTOR, label, fixture, SELF, free_count=frees,
                         free_mutations=mutations, ios=SELF,
                         features=dict(owned=owned, old_buffer=old,
                                       nested_kind=kind, callback_mutation=True)))
        for incoming in (0, NEW_BUFFER):
            fixture = _fixture()
            _ios(fixture, SELF, owned, old)
            _buffer(fixture, BUFFER, kind, payload_owned, payload_owned)
            out.append(_case(IOS_INIT, f'{label}/incoming-{incoming:x}',
                             fixture, SELF, [incoming], result=incoming,
                             full_result=True, free_count=frees,
                             free_mutations=mutations, ios=SELF,
                             features=dict(owned=owned, old_buffer=old,
                                           incoming=incoming, nested_kind=kind,
                                           live_state_after_callback=True)))
    # No low-bit coercion: a high ownership DWORD still calls virtual delete.
    fixture = _fixture()
    _ios(fixture, SELF, 0, BUFFER)
    out.append(_case(IOS_INIT, 'unowned-same-buffer', fixture, SELF, [BUFFER],
                     result=BUFFER, full_result=True, ios=SELF))
    return out


def _buffer_cases():
    out = []
    pending_rows = [
        ('read-unsigned', 0x7FFFFFFF, 0x80000000, 0xFFFFFFFF, 0, 0xFFFFFFFF),
        ('read-zero', 0, 1, 0, 0xFFFFFFFF, 0xFFFFFFFF),
        ('write-unsigned', 0xFFFFFFFF, 0x80000000,
         0x80000000, 0x7FFFFFFF, 0xFFFFFFFF),
        ('write-equal', 0x80000000, 0x80000000, 0xFFFFFFFF, 0xFFFFFFFF, 0),
        ('write-less', 0xFFFFFFFF, 0x80000000, 0x7FFFFFFF, 0x80000000, 0),
        ('zero', 0, 0, 0, 0, 0)]
    for label, cursor, end, put, base, expected in pending_rows:
        for flags in (0x202, 0xAD7):
            fixture = _fixture()
            _buffer(fixture, SELF)
            _words(fixture, SELF, {24: base, 28: put, 40: cursor, 44: end})
            case = _case(PENDING, f'{label}/{flags:x}', fixture, SELF,
                         result=expected, full_result=True,
                         features=dict(unsigned_comparisons=True))
            case['eflags'] = flags
            out.append(case)
    profiles = [('unowned', 0, PAYLOAD, 'none', 0),
                ('owned-null', 2, 0, 'none', 0),
                ('owned-high', 0x80000000, PAYLOAD, 'none', 1),
                ('owned-read', 2, PAYLOAD, 'read', 1),
                ('owned-write', 1, PAYLOAD, 'write', 1)]
    for label, owned, payload, pending, frees in profiles:
        fixture = _fixture()
        _buffer(fixture, SELF, owned=owned, payload=payload, pending=pending)
        out.append(_case(BUFFER_DTOR, label, fixture, SELF, free_count=frees,
                         features=dict(owned=owned, payload=payload,
                                       pending=pending)))
    return out


def _filebuf_cases():
    out = []
    profiles = [
        ('neither', 0, 0, PAYLOAD, False, (), (), 0),
        ('file-owned-null', 0, 0x80000000, 0, False, (), (), 0),
        ('file-owned', 0, 2, PAYLOAD, False, (), (), 1),
        ('base-owned', 2, 0, PAYLOAD, False, (), (), 1),
        ('both-owned', 1, 0x80000000, PAYLOAD, False, (), (), 2),
        ('free-mutes-base', 1, 2, PAYLOAD, False,
         ((SELF + 4, 4, 0), (SELF + 16, 4, SECOND_PAYLOAD)), (), 1),
        ('release', 0, 0x80000000, PAYLOAD, True, (), (), 0),
        ('release-changes-base', 0, 2, PAYLOAD, True, (),
         ((SELF + 4, 4, 2), (SELF + 16, 4, SECOND_PAYLOAD),
          (SELF + 48, 4, 0)), 1)]
    for label, owned, file_owned, payload, callback, free_mut, release_mut, frees in profiles:
        fixture = _fixture()
        _buffer(fixture, SELF, 'filebuf', owned, file_owned, payload, 'read')
        out.append(_case(FILE_DTOR, label, fixture, SELF, free_count=frees,
                         free_mutations=free_mut, callback=callback,
                         release_mutations=release_mut,
                         features=dict(base_owned=owned, file_owned=file_owned,
                                       payload=payload, release_callback=callback,
                                       callback_reloads=bool(free_mut or release_mut))))
    return out


def _scalar_cases():
    out = []
    for entry, kind in SCALAR_DELETES.items():
        for flags in (0, 1, 2, 3, 0x80000001):
            fixture = _fixture()
            if kind == 'ios':
                _ios(fixture, SELF, 2, BUFFER)
                nested_frees = 1
            else:
                _buffer(fixture, SELF, kind, owned=2,
                        file_owned=2 if kind == 'filebuf' else 0,
                        pending='write')
                nested_frees = 2 if kind == 'filebuf' else 1
            out.append(_case(entry, f'flags-{flags:x}', fixture, SELF, [flags],
                             result=SELF, full_result=True,
                             free_count=nested_frees + (flags & 1),
                             ios=SELF if kind == 'ios' else None,
                             features=dict(flags=flags, destructor=kind)))
    return out


def _stream_constructs():
    out = []
    constructors = [(OSTREAM_CTOR, 8, False), (ISTREAM_CTOR, 12, False),
                    (IOSTREAM_CTOR, 20, True)]
    for entry, default, combined in constructors:
        rows = [(flag, incoming, False, False)
                for flag in (1, 0x80000000) for incoming in (0, NEW_BUFFER)]
        rows += [(0, 0, False, False), (0, NEW_BUFFER, False, False),
                 (0, 0, True, False), (0, NEW_BUFFER, True, False),
                 (0, NEW_BUFFER, True, True)]
        for flag, incoming, owned, mutate in rows:
            fixture = _fixture()
            displacement = default if flag else 0x70
            _layout(fixture, SELF, displacement, combined)
            ios = SELF + displacement
            _ios(fixture, ios, 2 if owned else 0, BUFFER if owned else 0)
            frees = (2 if combined and incoming else 1) if owned else 0
            mutations = ([(TABLE + 4, 4, 0x90),
                          (ios + 8, 4, 0x5963ACF7)] if mutate else [])
            label = f'vb-{flag:x}/buffer-{incoming:x}/owned-{int(owned)}/mutate-{int(mutate)}'
            out.append(_case(entry, label, fixture, SELF, [incoming, flag],
                             result=SELF, full_result=True, free_count=frees,
                             free_mutations=mutations, ios=ios,
                             features=dict(virtual_base_flag=flag,
                                           displacement=displacement,
                                           old_owned=owned, incoming=incoming,
                                           mutate_vbtable=mutate)))
    for flag in (0, 1, 0x80000000):
        for allocation in (0, BUFFER):
            fixture = _fixture()
            displacement = 20 if flag else 0x70
            _layout(fixture, SELF, displacement, True)
            ios = SELF + displacement
            _ios(fixture, ios)
            out.append(_case(FSTREAM_CTOR,
                             f'vb-{flag:x}/allocation-{allocation:x}', fixture,
                             SELF, [flag], result=SELF, full_result=True,
                             allocate=allocation, ios=ios,
                             features=dict(virtual_base_flag=flag,
                                           displacement=displacement,
                                           allocation=allocation,
                                           nested_filebuf=bool(allocation))))
    return out


def _stream_destroys():
    out = []
    rows = [(OSTREAM_DTOR, 8, False), (ISTREAM_DTOR, 12, False),
            (IOSTREAM_DTOR, 20, True), (FSTREAM_DTOR, 20, True)]
    for entry, adjustment, combined in rows:
        for displacement in (adjustment, 0x70):
            fixture = _fixture()
            _layout(fixture, SELF, displacement, combined)
            _ios(fixture, SELF + displacement, 2, BUFFER)
            out.append(_case(entry, f'displacement-{displacement:x}', fixture,
                             SELF + adjustment,
                             ios=SELF + displacement,
                             features=dict(receiver_adjustment=adjustment,
                                           displacement=displacement,
                                           ios_not_destroyed_here=True)))
    for displacement in (20, 0x70):
        for owned in (0, 2):
            fixture = _fixture()
            _layout(fixture, SELF, displacement, True)
            _ios(fixture, SELF + displacement, 0, 0)
            # Complete destruction always passes actual self+20 to ios_destroy;
            # a custom vbtable does not change that separate explicit argument.
            _ios(fixture, SELF + 20, owned, BUFFER)
            out.append(_case(COMPLETE_DTOR,
                             f'displacement-{displacement:x}/owned-{owned}',
                             fixture, SELF, free_count=1 if owned else 0,
                             ios=SELF + 20,
                             features=dict(displacement=displacement,
                                           actual_ios_argument=SELF + 20,
                                           owned=owned)))
    for entry, adjustment in VIRTUAL_DELETES.items():
        for flags in (0, 1, 2, 3, 0x80000001):
            fixture = _fixture()
            displacement = adjustment if flags in (0, 3) else 0x70
            combined = adjustment == 20
            _layout(fixture, SELF, displacement, combined)
            _ios(fixture, SELF + displacement)
            _ios(fixture, SELF + adjustment, 2, BUFFER)
            out.append(_case(entry, f'flags-{flags:x}/displacement-{displacement:x}',
                             fixture, SELF + adjustment, [flags],
                             result=SELF, full_result=True,
                             free_count=1 + (flags & 1), ios=SELF + adjustment,
                             features=dict(flags=flags, displacement=displacement,
                                           actual_ios_argument=SELF + adjustment,
                                           complete_allocation=SELF)))
    return out


def _cleanup_cases():
    out = []
    for entry, adjustment in CONDITIONAL_CLEANUPS.items():
        for flags in (0, 1, 2, 0x80000001):
            fixture = _fixture()
            _ios(fixture, SELF + adjustment, 0x80000000, BUFFER)
            _words(fixture, FRAME, {-20: SELF, -16: flags})
            out.append(_case(entry, f'constructed-{flags:x}', fixture,
                             0x22739A41, free_count=flags & 1,
                             ios=SELF + adjustment,
                             features=dict(parent_ebp=FRAME, constructed=flags,
                                           ios_adjustment=adjustment)))
    for label, owned, payload, pending, frees in [
            ('unowned', 0, PAYLOAD, 'none', 0),
            ('owned-null', 2, 0, 'read', 0),
            ('owned-buffer', 0x80000000, PAYLOAD, 'write', 1)]:
        fixture = _fixture()
        _buffer(fixture, SELF, owned=owned, payload=payload, pending=pending)
        _words(fixture, FRAME, {-16: SELF})
        out.append(_case(0x56E17C, label, fixture, 0x22739A41,
                         free_count=frees,
                         features=dict(parent_ebp=FRAME, buffer=SELF)))
    for owned in (0, 2):
        root = FRAME - 84
        fixture = _fixture(root)
        _layout(fixture, root, 20, True)
        _ios(fixture, root + 20, owned, BUFFER)
        out.append(_case(0x56B7D0, f'inline-owned-{owned}', fixture, 0x22739A41,
                         free_count=1 if owned else 0, ios=root + 20,
                         features=dict(parent_ebp=FRAME, inline_object=root,
                                       owned=owned)))
    for entry in (0x56E222, 0x56E238):
        for displacement in (20, 0x70):
            fixture = _fixture()
            _layout(fixture, SELF, displacement, True)
            _ios(fixture, SELF + displacement)
            _words(fixture, FRAME,
                   {-20: SELF} if entry == 0x56E222 else {-16: SELF + 20})
            out.append(_case(entry, f'displacement-{displacement:x}', fixture,
                             0x22739A41, ios=SELF + displacement,
                             features=dict(parent_ebp=FRAME,
                                           child_argument=SELF + 12,
                                           displacement=displacement)))
    return out


def definitions(registry):
    present = {int(member['address'], 16) for member in registry['members']}
    assert len(set(ENTRIES)) == 32 and set(ENTRIES) <= present
    out = []
    for author in (_simple_constructs, _ios_cases, _buffer_cases, _filebuf_cases,
                   _scalar_cases, _stream_constructs, _stream_destroys,
                   _cleanup_cases):
        out.extend(author())
    assert {case['entry'] for case in out} == set(ENTRIES)
    assert len({case['id'] for case in out}) == len(out)
    return out
