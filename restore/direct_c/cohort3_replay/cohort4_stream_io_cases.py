"""Pure cohort4 stream-I/O recipes; importing does not execute any image.

The 22 selected native entries execute with their real ECX receivers and native
RET byte counts. Named C entrypoints receive explicit receiver arguments. Real
stream children, memcpy/memmove and strlen execute. Allocation/free, sprintf and
fixture-supplied virtual callbacks are the only controlled boundaries. Sprintf
is controlled because this packet tests integer format/prefix construction and
stream dispatch, not the already separate general CRT formatting subsystem.

Native/C comparison is finite: ordinary valid objects, selected wrapping/signed
arithmetic, null fields, callback mutation and selected EOF/error paths. No claim
is made for all pointer aliases, non-progressing read loops, unbounded counts,
all resolver tokens or actual filesystem/console behavior. Incidental EAX from
logical-void 560C26 is deliberately excluded. Raw controlled callback addresses
have the same identity in all images; numeric original stream tokens exercise
the compiled resolver before actual canonical stream children execute.
"""
import struct

SCRATCH = 0x32000000
SELF, BUFFER, TIE, TIE_BUFFER = (SCRATCH + x for x in (0x1000, 0x2000, 0x3000, 0x4000))
DATA, NEW_DATA, TEXT, PREFIX, DEST = (SCRATCH + x for x in (0x5000, 0x6000, 0x7000, 0x8000, 0x9000))
VBTABLE, VTABLE, TIE_VBTABLE, TIE_VTABLE = (SCRATCH + x for x in (0x100, 0x200, 0x300, 0x400))
IOS = SELF + 0x80
SECOND_IOS = SELF + 0xC0
SYNC, OVERFLOW, UNDERFLOW, SEEK, PUTBACK, ALLOCATE, RELEASE, SECOND_UNDERFLOW = (
    0x31000200 + x * 0x10 for x in range(8))
GROW, SET_GROWTH, FILE_OVERFLOW, FILE_UNDERFLOW, FILE_SEEK = (
    0x56073C, 0x560845, 0x560855, 0x5608CC, 0x56090C)
CHARACTER, PUT, INTEGER, PREPARE, SUFFIX, STRING, FLUSH, FORMAT = (
    0x560AAE, 0x560B2A, 0x560B4C, 0x560BFD, 0x560C26, 0x560CA6, 0x560CCF, 0x560D96)
SETBUF, WRITE, READ, ALLOCATE_BUFFER, SET, DEFAULT_SEEK, SEEK_POSITION, PUTBACK_BUFFER, PUTBACK_DIRECT = (
    0x560F6F, 0x560FA3, 0x560FF9, 0x5610A7, 0x5610D4, 0x561104, 0x56110A, 0x56111C, 0x561178)
ENTRIES = (GROW, SET_GROWTH, FILE_OVERFLOW, FILE_UNDERFLOW, FILE_SEEK,
           CHARACTER, PUT, INTEGER, PREPARE, SUFFIX, STRING, FLUSH, FORMAT,
           SETBUF, WRITE, READ, ALLOCATE_BUFFER, SET, DEFAULT_SEEK,
           SEEK_POSITION, PUTBACK_BUFFER, PUTBACK_DIRECT)


def _u32(value):
    return struct.pack('<I', value & 0xFFFFFFFF)


def _pattern(n):
    return bytes((i * 37 + 13) & 255 for i in range(n))


def _words(fixture, address, values):
    fixture['stores'].extend((address + off, _u32(value)) for off, value in values.items())


def _endpoint(token, native, symbols, arity, value=0, kind='full32', cleanup=0,
              receiver=None, mutations=(), registers=()):
    spec = dict(token=token, native=native, compiled_symbols=list(symbols), arity=arity,
                native_cleanup=cleanup, compiled_cleanup=cleanup, result_kind=kind,
                pointer_args={}, writes_args=[], mutations=list(mutations),
                observe=[(IOS + 8, 4), (IOS + 0x30, 4), (SECOND_IOS + 8, 4),
                         (VBTABLE + 4, 4), (BUFFER + 0x1C, 4), (BUFFER + 0x28, 4)],
                clobber_ecx=0x9182A3B4, clobber_edx=0xE1D2C3B4)
    if kind != 'void':
        spec['value'] = value
    if receiver is not None:
        spec['native_receiver'] = receiver
        spec['registers'] = list(registers or ['ecx'])
    elif registers:
        spec['registers'] = list(registers)
    return spec


def _callback(token, address, arity, value=0, receiver=BUFFER, mutations=(),
              kind='full32', cleanup=None, registers=()):
    spec = _endpoint(token, address, [], arity, value, kind,
                     arity * 4 if cleanup is None else cleanup,
                     receiver, mutations, registers)
    spec['controlled_address'] = address
    return spec


def _fixture():
    stores = [(at - 16, _pattern(n + 32)) for at, n in [
        (SELF, 0x180), (BUFFER, 0x90), (TIE, 0x180), (TIE_BUFFER, 0x90),
        (DATA, 0x200), (NEW_DATA, 0x300), (TEXT, 0x100), (PREFIX, 0x100),
        (DEST, 0x200), (VBTABLE, 0x340)]]
    fixture = dict(stores=stores, watched=[(at, len(raw)) for at, raw in stores],
                   write_ranges=[(SELF, 0x180), (BUFFER, 0x90), (TIE, 0x180),
                                 (TIE_BUFFER, 0x90), (DATA, 0x200), (NEW_DATA, 0x300),
                                 (DEST, 0x200), (VBTABLE, 0x340)], endpoints=[], counts={})
    _words(fixture, VBTABLE, {0: 0x6E5D4C3B, 4: 0x80})
    _words(fixture, TIE_VBTABLE, {0: 0x19283746, 4: 0x80})
    _words(fixture, SELF, {0: VBTABLE})
    _words(fixture, TIE, {0: TIE_VBTABLE})
    for ios, buffer in [(IOS, BUFFER), (SECOND_IOS, BUFFER), (TIE + 0x80, TIE_BUFFER)]:
        _words(fixture, ios, {4: buffer, 8: 0, 0x20: 0, 0x24: 0, 0x2C: ord('.'), 0x30: 0})
    for buffer, table in [(BUFFER, VTABLE), (TIE_BUFFER, TIE_VTABLE)]:
        _words(fixture, buffer, {0: table, 4: 0, 8: 0, 0xC: 0xFFFFFFFF,
                                0x10: DATA, 0x14: DATA + 64,
                                0x18: DATA + 32, 0x1C: DATA + 32, 0x20: DATA + 64,
                                0x24: DATA, 0x28: DATA + 2, 0x2C: DATA + 8,
                                0x30: 0, 0x34: 16, 0x40: 0, 0x44: 0})
        # Numeric native tokens remain unchanged in data. C virtual dispatch
        # resolves them to the actual compiled canonical stream adapters.
        _words(fixture, table, {0: 0x560F2F, 4: SYNC, 0xC: DEFAULT_SEEK,
                               0x14: WRITE, 0x1C: FILE_OVERFLOW,
                               0x20: FILE_UNDERFLOW, 0x24: PUTBACK})
    fixture['stores'] += [(TEXT, b'a\x80z\0'), (PREFIX, b'+\0')]
    return fixture


def _add(fixture, spec, count):
    assert spec['token'] not in fixture['counts']
    fixture['endpoints'].append(spec)
    fixture['counts'][spec['token']] = count


def _heap(fixture, allocation=None, alloc_count=0, free_count=0,
          alloc_mutations=(), free_mutations=()):
    if allocation is None:
        allocation = NEW_DATA
    _add(fixture, _endpoint('allocate', 0x561A1D, ['FUN_00561a1d'], 1,
                            allocation, mutations=alloc_mutations), alloc_count)
    _add(fixture, _endpoint('free', 0x561A2B, ['FUN_00561a2b'], 1,
                            kind='void', mutations=free_mutations), free_count)


def _case(entry, label, fixture, args=(), receiver=BUFFER, expected=None,
          mask=0xFFFFFFFF, named=None, cleanup=None, features=None):
    cleanup = len(args) * 4 if cleanup is None else cleanup
    case = dict(id=f'stream-io/{entry:08x}/{label}', entry=entry, receiver=receiver,
                arguments=list(args), named_arguments=[receiver, *args] if named is None else list(named),
                eflags=0xAD7 if sum(label.encode()) & 1 else 0x202,
                stores=fixture['stores'], watched=fixture['watched'],
                write_ranges=fixture['write_ranges'], endpoints=fixture['endpoints'],
                expected_endpoint_counts=fixture['counts'], result_mask=mask,
                native_cleanup=cleanup, named_cleanup=0, canonical_cleanup=cleanup,
                canonical_owner='crt_stream_io_entries.obj', canonical_registers_and_flags=False,
                features=features or {})
    if expected is not None:
        case['expected_eax'] = expected & 0xFFFFFFFF
    return case


def _growth_cases():
    out = []
    for label, growth, first, last, custom, owned, allocation in [
            ('default-failure', 16, DATA, DATA + 64, False, 0, 0),
            ('default-copy-relocate', 16, DATA, DATA + 64, False, 0, NEW_DATA),
            ('owned-second-release', 16, DATA, DATA + 64, False, 1, NEW_DATA),
            ('zero-growth', 0, DATA, DATA + 64, False, 0, NEW_DATA),
            ('negative-growth', 0xFFFFFFFF, DATA, DATA + 64, False, 0, NEW_DATA),
            ('empty-extent', 4, DATA, DATA, False, 0, NEW_DATA),
            ('reversed-extent', 7, DATA + 16, DATA, False, 0, NEW_DATA),
            ('custom-normal', 16, DATA, DATA + 64, True, 0, NEW_DATA),
            ('custom-signed-sum-overflow', 0x7FFFFFFF, DATA, DATA + 64, True, 0, 0),
            ('custom-negative-growth', 0x80000000, DATA, DATA + 64, True, 0, 0)]:
        f = _fixture()
        _words(f, BUFFER, {4: owned, 0x10: first, 0x14: last, 0x34: growth})
        if custom:
            capacity = max(last - first, 0)
            signed_growth = growth if growth < 0x80000000 else growth - 0x100000000
            inc = growth if signed_growth > 1 else 1
            total = (capacity + inc) & 0xFFFFFFFF
            signed_total = total if total < 0x80000000 else total - 0x100000000
            live_ecx = last if signed_growth > signed_total else (capacity + inc) & 0xFFFFFFFF
            _words(f, BUFFER, {0x40: ALLOCATE, 0x44: RELEASE})
            _add(f, _callback('custom-allocate', ALLOCATE, 1, allocation,
                              receiver=live_ecx, cleanup=0, registers=['eax', 'ecx']), 1)
            _add(f, _callback('custom-release', RELEASE, 1, receiver=RELEASE,
                              kind='void', cleanup=0, registers=['eax', 'ecx']), int(allocation != 0))
            _heap(f)
        else:
            _heap(f, allocation, 1, (1 + int(owned != 0)) if allocation else 0)
        out.append(_case(GROW, label, f, expected=1 if allocation else 0xFFFFFFFF,
                         features=dict(growth=growth, old_extent=[first, last], custom_allocator=custom)))
    f = _fixture()
    _words(f, BUFFER, {0x40: ALLOCATE, 0x44: RELEASE})
    _add(f, _callback('custom-allocate', ALLOCATE, 1, NEW_DATA, receiver=80, cleanup=0,
                      registers=['eax', 'ecx'], mutations=[(BUFFER + 0x10, 4, DATA + 4),
                                                           (BUFFER + 0x14, 4, DATA + 20)]), 1)
    _add(f, _callback('custom-release', RELEASE, 1, receiver=RELEASE, kind='void', cleanup=0,
                      registers=['eax', 'ecx'], mutations=[(BUFFER + 0x1C, 4, DATA + 39),
                                                           (BUFFER + 0x28, 4, DATA + 7)]), 1)
    _heap(f)
    out.append(_case(GROW, 'callbacks-mutate-live-fields', f, expected=1,
                     features=dict(copy_extent_reloaded=True, release_receiver_is_target=True)))
    f = _fixture()
    delta = NEW_DATA - DATA
    _words(f, BUFFER, {0x18: DATA + 32, 0x1C: DATA + 35, 0x20: -delta})
    _heap(f, NEW_DATA, 1, 1)
    out.append(_case(GROW, 'output-last-wraps-to-null', f, expected=1))
    return out


def _file_cases():
    out = []
    for char in (0, 0x12345680, 0xFFFFFFFF):
        f = _fixture()
        _heap(f)
        out.append(_case(FILE_OVERFLOW, f'direct-{char:x}', f, [char], expected=1))
    for label, grow, current, end, allocation in [
            ('full-disabled', 0, DATA + 64, DATA + 64, None),
            ('full-allocate-fails', 1, DATA + 64, DATA + 64, 0),
            ('grow-restore-current', 1, DATA + 64, DATA + 64, NEW_DATA),
            ('grow-initialize-null-output', 1, 0, 0, NEW_DATA)]:
        f = _fixture()
        _words(f, BUFFER, {0x1C: current, 0x20: end, 0x30: grow})
        _heap(f, allocation, int(grow != 0), int(bool(allocation)))
        out.append(_case(FILE_OVERFLOW, label, f, [0xA5B6C7E1],
                         expected=1 if allocation else 0xFFFFFFFF))
    for label, current, end, output, old_first, expected in [
            ('available-high-byte', DATA + 2, DATA + 8, DATA + 32, DATA, None),
            ('at-end-no-output', DATA + 8, DATA + 8, DATA + 8, DATA, 0xFFFFFFFF),
            ('past-end-no-output', DATA + 9, DATA + 8, DATA + 8, DATA, 0xFFFFFFFF),
            ('extend-from-output', DATA + 8, DATA + 8, DATA + 20, DATA, None),
            ('rebase-cursor', DATA + 12, DATA + 8, DATA + 24, DATA + 4, None),
            ('unsigned-reject-high-current', 0xF0000000, DATA + 8, DATA + 8, DATA, 0xFFFFFFFF)]:
        f = _fixture()
        _words(f, BUFFER, {0x24: old_first, 0x28: current, 0x2C: end, 0x1C: output})
        f['stores'].append((DATA + 2, b'\xE7'))
        out.append(_case(FILE_UNDERFLOW, label, f, expected=expected))
    for mode in (0, 1, 2, 3, 0x80000003):
        for origin in (0, 1, 2, 3):
            f = _fixture()
            # Keep input end >= output so the real underflow does not extend it.
            _words(f, BUFFER, {0x18: DATA, 0x1C: DATA + 3, 0x20: DATA + 8,
                              0x24: DATA, 0x28: DATA + 2, 0x2C: DATA + 8})
            offset = 0xFFFFFFFF if origin == 2 else 1
            _heap(f)
            out.append(_case(FILE_SEEK, f'mode-{mode:x}/origin-{origin}', f,
                             [offset, origin, mode]))
    for label, offset, origin, mode, growth, allocation in [
            ('input-below-first', 0xFFFFFFFF, 0, 1, 0, None),
            ('input-above-end', 65, 0, 1, 0, None),
            ('output-below-first', 0xFFFFFFFF, 0, 2, 0, None),
            ('output-above-end-no-growth', 80, 0, 2, 0, None),
            ('output-growth-failure', 80, 0, 2, 1, 0),
            ('output-growth-keeps-old-candidate', 80, 0, 2, 1, NEW_DATA),
            ('null-output-overflow-fails', 0, 0, 2, 0, None)]:
        f = _fixture()
        _words(f, BUFFER, {0x18: DATA, 0x30: growth})
        if label.startswith('null-output'):
            _words(f, BUFFER, {0x1C: 0, 0x20: 0})
        _heap(f, allocation, int(growth != 0), int(bool(allocation)))
        out.append(_case(FILE_SEEK, label, f, [offset, origin, mode]))
    return out


def _small_buffer_cases():
    out = []
    for value in (0, 1, 0x80000000, 0xFFFFFFFF):
        f = _fixture()
        out.append(_case(SET_GROWTH, f'growth-{value:x}', f,
                         [0x13579BDF, value], named=[BUFFER, value], expected=BUFFER,
                         features=dict(ignored_native_first_argument=0x13579BDF)))
    for label, first, buffer, count in [('already-set', DATA, NEW_DATA, 16),
                                       ('nonempty', 0, NEW_DATA, 7),
                                       ('null-buffer', 0, 0, 7),
                                       ('zero-count', 0, NEW_DATA, 0),
                                       ('both-null', 0, 0, 0),
                                       ('wrapping-last', 0, 0xFFFFFFFE, 5)]:
        f = _fixture()
        _words(f, BUFFER, {0x10: first, 4: 0x80000000, 8: 0xA1B2C3D4})
        out.append(_case(SETBUF, label, f, [buffer, count], expected=0 if first else BUFFER))
    for ignored in ([0, 0, 0], [0xA1B2C3D4, 0xFFFFFFFF, 0x80000001]):
        f = _fixture()
        out.append(_case(DEFAULT_SEEK, f'ignored-{ignored[0]:x}', f, ignored,
                         named=[], expected=0xFFFFFFFF))
    for label, owned, old, first, last in [
            ('unowned', 0, DATA, NEW_DATA, NEW_DATA + 37),
            ('owned-null', 2, 0, NEW_DATA, NEW_DATA + 37),
            ('owned-release', 0x80000000, DATA, NEW_DATA, NEW_DATA + 37),
            ('same-pointer-owned', 1, DATA, DATA, DATA + 64),
            ('null-replacement', 1, DATA, 0, 0)]:
        f = _fixture()
        _words(f, BUFFER, {4: owned, 0x10: old})
        _heap(f, free_count=int(bool(owned and old)),
              free_mutations=[(BUFFER + 0x28, 4, DATA + 9)])
        out.append(_case(SET, label, f, [first, last, 7], expected=last))
    for allocation in (0, NEW_DATA):
        f = _fixture()
        _heap(f, allocation, 1)
        out.append(_case(ALLOCATE_BUFFER, f'allocate-{allocation:x}', f,
                         expected=1 if allocation else 0xFFFFFFFF))
    f = _fixture()
    _words(f, BUFFER, {4: 2})
    _heap(f, NEW_DATA, 1, 1)
    out.append(_case(ALLOCATE_BUFFER, 'allocation-replaces-owned', f, expected=1))
    for offset, mode in [(0, 0), (0xFFFFFFFF, 0x80000003), (0x11223344, 2)]:
        f = _fixture()
        _words(f, VTABLE, {0xC: SEEK})
        _add(f, _callback('seek', SEEK, 3, 0xA5B6C780), 1)
        out.append(_case(SEEK_POSITION, f'offset-{offset:x}/mode-{mode:x}', f,
                         [offset, mode], expected=0xA5B6C780,
                         features=dict(forwarded_origin=0)))
    # Real canonical default-seek dispatch also exercises known-token resolving.
    f = _fixture()
    out.append(_case(SEEK_POSITION, 'actual-default-seek', f, [19, 3], expected=0xFFFFFFFF))
    return out


def _putback_cases():
    out = []
    for entry in (PUT, PUTBACK_DIRECT, PUTBACK_BUFFER):
        for value in (0, 0x1234567F, 0x89ABCD80, 0xFFFFFFFE):
            f = _fixture()
            expected = value & 255
            if entry in (PUTBACK_DIRECT, PUTBACK_BUFFER) and expected >= 128:
                expected |= 0xFFFFFF00
            out.append(_case(entry, f'direct-{value:x}', f, [value], expected=expected))
    for value, kind, result in [(0x12345680, 'full32', 0xA1B2C380),
                                 (0x89ABCDE1, 'full32', 0xFFFFFFFF),
                                 (0x7F, 'al8', 0xEA)]:
        f = _fixture()
        _words(f, BUFFER, {0x1C: DATA + 64})
        _words(f, VTABLE, {0x1C: OVERFLOW})
        _add(f, _callback('overflow', OVERFLOW, 1, result, kind=kind), 1)
        expected = ((VTABLE & 0xFFFFFF00) | result) if kind == 'al8' else result
        out.append(_case(PUT, f'fallback-{kind}-{value:x}', f, [value], expected=expected,
                         features=dict(full_original_argument_forwarded=True, callback_result_kind=kind)))
    for value in (0x1234567F, 0x89ABCD80, 0xFFFFFFFF):
        f = _fixture()
        _words(f, BUFFER, {0x28: DATA})
        _add(f, _callback('putback', PUTBACK, 1, 0xA1B2C3D4), 1)
        out.append(_case(PUTBACK_DIRECT, f'fallback-sign-byte-{value:x}', f, [value], expected=0xA1B2C3D4))
    for label, seek_result, unbuffered, end in [
            ('seek-fails', 0xFFFFFFFF, 0, DATA + 8),
            ('seek-unbuffered-success', 7, 1, DATA + 8),
            ('seek-no-input-end', 0, 0, 0),
            ('seek-buffered-insert', 0, 0, DATA + 8)]:
        f = _fixture()
        _words(f, BUFFER, {8: unbuffered, 0x28: DATA, 0x2C: end})
        _words(f, VTABLE, {0xC: SEEK})
        _add(f, _callback('seek', SEEK, 3, seek_result), 1)
        out.append(_case(PUTBACK_BUFFER, label, f, [0x123456E1],
                         expected=0xFFFFFFFF if seek_result == 0xFFFFFFFF else 0x123456E1,
                         features=dict(seek_arguments=[0xFFFFFFFF, 1, 1], overlap_move=True)))
    # Overlapping source/destination is exercised by the actual memmove above.
    return out


def _transfer_cases():
    out = []
    for label, count, unbuffered, current, end, callback_result, calls, expected in [
            ('zero', 0, 0, DATA + 32, DATA + 64, None, 0, 0),
            ('buffered-direct', 5, 0, DATA + 32, DATA + 64, None, 0, 5),
            ('unbuffered-success', 4, 1, DATA + 32, DATA + 64, 1, 4, 4),
            ('unbuffered-error', 4, 1, DATA + 32, DATA + 64, 0xFFFFFFFF, 1, 0),
            ('buffered-short', 5, 0, DATA + 62, DATA + 64, 0xFFFFFFFF, 1, 2),
            ('full-callback-success', 4, 0, DATA + 64, DATA + 64, 0xABCDEF80, 4, 4)]:
        f = _fixture()
        _words(f, BUFFER, {8: unbuffered, 0x1C: current, 0x20: end})
        if callback_result is not None:
            _words(f, VTABLE, {0x1C: OVERFLOW})
            _add(f, _callback('overflow', OVERFLOW, 1, callback_result), calls)
        out.append(_case(WRITE, label, f, [TEXT, count], expected=expected))
    # The source aliases output, so later bytes observe earlier direct writes.
    f = _fixture()
    _words(f, BUFFER, {0x1C: DATA + 1, 0x20: DATA + 16})
    out.append(_case(WRITE, 'direct-source-output-overlap', f, [DATA, 5], expected=5))
    for label, count, unbuffered, cache, result, calls, expected in [
            ('buffered-zero', 0, 0, 0xFFFFFFFF, None, 0, 0),
            ('buffered-full', 4, 0, 0xFFFFFFFF, None, 0, 4),
            ('buffered-short-eof', 11, 0, 0xFFFFFFFF, None, 0, 6),
            ('unbuffered-count-zero-prefetch', 0, 1, 0xFFFFFFFF, 0xA1B2C380, 1, 0),
            ('unbuffered-count-zero-cached', 0, 1, 0xE7, 0xFFFFFFFF, 0, 0),
            ('unbuffered-initial-eof', 4, 1, 0xFFFFFFFF, 0xFFFFFFFF, 1, 0),
            ('unbuffered-repeat-and-prefetch', 3, 1, 0xFFFFFFFF, 0xA1B2C380, 4, 3),
            ('unbuffered-cached-then-eof', 5, 1, 0xABCDEF80, 0xFFFFFFFF, 1, 1)]:
        f = _fixture()
        _words(f, BUFFER, {8: unbuffered, 0xC: cache, 0x1C: DATA + 8})
        if result is not None:
            _words(f, VTABLE, {0x20: UNDERFLOW})
            _add(f, _callback('underflow', UNDERFLOW, 0, result), calls)
        out.append(_case(READ, label, f, [DEST, count], expected=expected))
    f = _fixture()
    _words(f, BUFFER, {8: 1, 0xC: 0xFFFFFFFF})
    _words(f, VTABLE, {0x20: UNDERFLOW})
    _add(f, _callback('underflow-first', UNDERFLOW, 0, 0xE7,
                      mutations=[(VTABLE + 0x20, 4, SECOND_UNDERFLOW)]), 1)
    _add(f, _callback('underflow-eof', SECOND_UNDERFLOW, 0, 0xFFFFFFFF), 1)
    out.append(_case(READ, 'unbuffered-vtable-reload-prefetch', f, [DEST, 3], expected=1))
    return out


def _ostream_control_cases():
    out = []
    for state in (0, 1, 2, 0x80000000):
        f = _fixture()
        _words(f, IOS, {8: state})
        out.append(_case(PREPARE, f'state-{state:x}', f, receiver=SELF, expected=int(state == 0)))
    for value in (0, 0xFFFFFFFF):
        f = _fixture()
        _words(f, IOS, {0x20: TIE})
        _add(f, _callback('sync-tie', SYNC, 0, value, receiver=TIE_BUFFER,
                          mutations=[(IOS + 8, 4, 0x80000000)]), 1)
        out.append(_case(PREPARE, f'actual-tie-sync-{value:x}', f, receiver=SELF, expected=1))
    for value in (0, 0xA5B6C780, 0xFFFFFFFF):
        for move_base in (False, True):
            f = _fixture()
            _words(f, IOS, {8: 0x80000001})
            mutations = [(VBTABLE + 4, 4, 0xC0), (SECOND_IOS + 8, 4, 0x40000000)] if move_base else []
            _add(f, _callback('sync', SYNC, 0, value, mutations=mutations), 1)
            out.append(_case(FLUSH, f'sync-{value:x}/move-base-{int(move_base)}', f,
                             receiver=SELF, expected=SELF))
    for flags, sync_result, stdio_result in [(0, 0, 0), (0x2000, 0, 0),
                                             (0x2000, 0xFFFFFFFF, 0),
                                             (0x4000, 0, 0), (0x4000, 0, 0xFFFFFFFF),
                                             (0x6000, 0xFFFFFFFF, 0xFFFFFFFF)]:
        f = _fixture()
        _words(f, IOS, {8: 0x80000001, 0x24: flags, 0x30: 19})
        _add(f, _callback('sync', SYNC, 0, sync_result), int(bool(flags & 0x2000)))
        _add(f, _endpoint('stdio-flush', 0x562BEA, ['FUN_00562bea'], 1, stdio_result),
             2 if flags & 0x4000 else 0)
        out.append(_case(SUFFIX, f'flags-{flags:x}/sync-{sync_result:x}/stdio-{stdio_result:x}', f,
                         receiver=SELF, mask=None,
                         features=dict(logical_result='void', stdio_arguments=[0x6049C0, 0x6049E0])))
    f = _fixture()
    _words(f, IOS, {0x24: 0x2000, 0x30: 19})
    _words(f, SECOND_IOS, {8: 0x80000000, 0x24: 0x4000, 0x30: 23})
    _add(f, _callback('sync', SYNC, 0, 0xFFFFFFFF,
                      mutations=[(VBTABLE + 4, 4, 0xC0)]), 1)
    _add(f, _endpoint('stdio-flush', 0x562BEA, ['FUN_00562bea'], 1, 0xFFFFFFFF), 2)
    out.append(_case(SUFFIX, 'sync-mutates-vbtable-and-later-flags', f, receiver=SELF, mask=None))
    return out


def _ostream_output_cases():
    out = []
    for value, width in [(0, 0), (0x12345680, 0), (0xFFFFFFFF, 0),
                         (0, 4), (0x12345680, 4), (ord('Z'), 1)]:
        f = _fixture()
        _words(f, IOS, {0x30: width})
        _heap(f)
        out.append(_case(CHARACTER, f'value-{value:x}/width-{width}', f, [value],
                         receiver=SELF, expected=SELF))
    for result in (0xFFFFFFFF, 1):
        f = _fixture()
        _words(f, BUFFER, {0x1C: DATA + 64})
        _words(f, VTABLE, {0x1C: OVERFLOW})
        _add(f, _callback('overflow', OVERFLOW, 1, result), 2 if result == 0xFFFFFFFF else 1)
        out.append(_case(CHARACTER, f'explicit-double-overflow-{result:x}', f,
                         [0xABCDEFE1], receiver=SELF, expected=SELF))
    for entry, args in [(CHARACTER, [ord('z')]), (INTEGER, [42]), (STRING, [0])]:
        f = _fixture()
        _words(f, IOS, {8: 0x80000000, 0x30: 11})
        out.append(_case(entry, 'prepare-fails-without-dereference', f, args,
                         receiver=SELF, expected=SELF))
    for label, text, width, flags in [('normal', TEXT, 0, 0), ('empty', PREFIX + 1, 4, 2),
                                      ('right-padding', TEXT, 7, 0), ('left-padding', TEXT, 7, 2)]:
        f = _fixture()
        _words(f, IOS, {0x30: width, 0x24: flags})
        out.append(_case(STRING, label, f, [text], receiver=SELF, expected=SELF))
    # This endpoint returns a full sprintf count and initializes every output
    # byte including its terminator. Format is a private native/C stack string.
    for label, value, flags, converted in [
            ('zero-suppresses-prefix', 0, 0x4E0, b'0'),
            ('positive-decimal', 123, 0, b'123'),
            ('show-positive', 123, 0x400, b'123'),
            ('negative', 0xFFFFFF85, 0x400, b'-123'),
            ('int-min', 0x80000000, 0, b'-2147483648'),
            ('octal', 0x123, 0x20, b'443'),
            ('octal-showbase', 0x123, 0xA0, b'443'),
            ('hex-lower', 0xABCD, 0x40, b'abcd'),
            ('hex-lower-showbase', 0xABCD, 0xC0, b'abcd'),
            ('hex-upper-showbase', 0xABCD, 0x2C0, b'ABCD')]:
        f = _fixture()
        _words(f, IOS, {0x24: flags, 0x30: 0})
        spec = _endpoint('sprintf', 0x561A36, ['FUN_00561a36'], 3, len(converted))
        spec['pointer_args'] = {0: 'integer-text'}
        spec['private_input_args'] = {1: dict(role='integer-format', width=3)}
        spec['writes_arg_bytes'] = [[0, 0, (converted + b'\0').hex()]]
        _add(f, spec, 1)
        out.append(_case(INTEGER, label, f, [value], receiver=SELF, expected=SELF,
                         features=dict(controlled_sprintf_output=converted.hex(), flags=flags)))
    return out


def _format_cases():
    out = []
    for label, flags, width, prefix, text in [
            ('right-pad', 0, 8, PREFIX, TEXT),
            ('internal-pad', 8, 8, PREFIX, TEXT),
            ('left-pad', 2, 8, PREFIX, TEXT),
            ('both-adjust-bits', 10, 8, PREFIX, TEXT),
            ('no-padding', 0, 4, PREFIX, TEXT),
            ('empty-prefix', 0, 6, PREFIX + 1, TEXT),
            ('empty-text', 8, 4, PREFIX, TEXT + 3),
            ('both-empty-zero-width', 0, 0, PREFIX + 1, TEXT + 3),
            ('signed-negative-padding', 0, 0x80000008, PREFIX, TEXT),
            ('signed-negative-internal', 8, 0x80000008, PREFIX, TEXT)]:
        f = _fixture()
        _words(f, IOS, {0x24: flags, 0x30: width})
        out.append(_case(FORMAT, label, f, [prefix, text], receiver=SELF, expected=SELF))
    # Real bulk-write and overflow children implement failed partial transfers.
    for label, flags, width, room in [('prefix-fails', 0, 0, 0),
                                      ('text-short', 2, 0, 2),
                                      ('fill-fails', 0, 7, 0),
                                      ('internal-fill-fails', 8, 7, 0),
                                      ('left-fill-fails', 2, 7, 0)]:
        f = _fixture()
        _words(f, IOS, {0x24: flags, 0x30: width})
        _words(f, BUFFER, {0x1C: DATA + 64 - room})
        _heap(f)
        out.append(_case(FORMAT, label, f, [PREFIX, TEXT], receiver=SELF, expected=SELF))
    f = _fixture()
    _words(f, IOS, {0x24: 8, 0x30: 7})
    _words(f, BUFFER, {0x1C: DATA + 64})
    _words(f, VTABLE, {0x1C: OVERFLOW})
    _add(f, _callback('overflow', OVERFLOW, 1, 0xFFFFFFFF,
                      mutations=[(VBTABLE + 4, 4, 0xC0), (SECOND_IOS + 0x24, 4, 2)]), 4)
    # Prefix fails, then newly loaded LEFT flag causes three final fill calls;
    # text also fails, so five total callbacks are observed (1+1+3).
    f['counts']['overflow'] = 5
    out.append(_case(FORMAT, 'callback-mutates-base-and-alignment', f, [PREFIX, TEXT],
                     receiver=SELF, expected=SELF,
                     features=dict(later_flags_reloaded=True, callback_errors_reload_state=True)))
    return out


def definitions(registry):
    present = {int(member['address'], 16) for member in registry['members']}
    assert len(set(ENTRIES)) == 22 and set(ENTRIES) <= present
    out = []
    for author in (_growth_cases, _file_cases, _small_buffer_cases, _putback_cases,
                   _transfer_cases, _ostream_control_cases, _ostream_output_cases, _format_cases):
        out.extend(author())
    assert {case['entry'] for case in out} == set(ENTRIES)
    assert len({case['id'] for case in out}) == len(out)
    return out
