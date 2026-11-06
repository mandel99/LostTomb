"""Bounded 41C740 cases for the existing shared offline cohort replay.

The real 41C8F0 enumerator executes. Only its Win32 enumeration imports and
the caller's allocate/delete endpoints are controlled. This module loads no
image and performs no execution. Its authored transcript is also checked by
guard.py against an optional completed shared report.
"""
import struct

ENTRY = 0x41c740
SELF, PATH = 0x32001000, 0x32002000
OLD_OFF, NEW_OFF = 0x32003000, 0x32004000
OLD_STR, NEW_STR = 0x32005000, 0x32006000
HANDLE = 0x4444
FIND_DATA_NAME = 44
ROLE = ['stack-output', 'file-find-data']
BLOCKS = [(SELF, 28), (PATH, 32), (OLD_OFF, 64), (NEW_OFF, 512),
          (OLD_STR, 128), (NEW_STR, 2048)]
OBSERVE = [(SELF + i, 4) for i in range(0, 28, 4)] + [
    (p + i, 4) for p in (OLD_OFF, NEW_OFF, OLD_STR, NEW_STR)
    for i in range(0, 32, 4)]


def endpoint(token, address, arity, result='void'):
    return dict(token=token, native=address,
                compiled_symbols=[f'FUN_{address:08x}'], arity=arity,
                native_cleanup=0, compiled_cleanup=0, result_kind=result,
                value=0, observe=OBSERVE,
                clobber_ecx=0xdead4321, clobber_edx=0xbeef3456)


def win32(token, name, arity):
    return dict(token=token, import_name=name, arity=arity,
                native_cleanup=arity * 4, compiled_cleanup=arity * 4,
                result_kind='full32', value=0, observe=OBSERVE,
                clobber_ecx=0xdead4321, clobber_edx=0xbeef3456)


class Recipe:
    """Authored memory/transcript for these finite scenarios, not an emulator."""
    def __init__(self, label, append=0, count=2, capacity=8,
                 used=7, string_capacity=64):
        self.mem = {p: bytearray(bytes([fill]) * n)
                    for (p, n), fill in zip(BLOCKS,
                        (0xa5, 0xa5, 0xc5, 0xa7, 0xd4, 0xb6))}
        self.word(SELF, 0x11223344)
        for off, value in [(4, OLD_OFF), (8, count), (12, capacity),
                           (16, OLD_STR), (20, string_capacity), (24, used)]:
            self.word(SELF + off, value)
        for i, value in enumerate((0, 5, 9, 13)):
            self.word(OLD_OFF + i * 4, value)
        self.write(OLD_STR, b'old0\0x\0TAIL\0')
        self.write(PATH, b'offline/*.dat\0')
        self.c = dict(id='c7-joint-filelist/0041c740/' + label,
            entry=ENTRY, receiver=SELF, arguments=[PATH, append],
            named_arguments=[SELF, PATH, append],
            canonical_arguments=[SELF, PATH, append],
            eflags=0xad7, initial_eax=0xf1e2d3c4,
            stores=[(p, bytes(self.mem[p])) for p, _ in BLOCKS],
            watched=BLOCKS,
            write_ranges=[(p, n) for p, n in BLOCKS if p != PATH],
            result_mask=0xff, native_cleanup=8, named_cleanup=0,
            canonical_cleanup=0, canonical_registers_and_flags=False,
            endpoints=[], expected_endpoint_counts={},
            expected_endpoint_arguments={}, authored_endpoint_order=[],
            authored_endpoint_observations=[], expected_invocations=7)
        self.first = win32('find-first', 'FindFirstFileA', 2)
        self.next = win32('find-next', 'FindNextFileA', 2)
        self.close = win32('find-close', 'FindClose', 1)
        self.first['pointer_args'] = {1: 'file-find-data'}
        self.next['pointer_args'] = {1: 'file-find-data'}
        self.close['value'] = 1
        self.alloc = endpoint('allocate', 0x561a1d, 1, 'full32')
        self.delete = endpoint('delete', 0x561a2b, 1)
        for spec in (self.first, self.next, self.alloc):
            spec['responses'] = []
        self.specs = [self.first, self.next, self.close,
                      self.alloc, self.delete]
        if append & 255 == 0:
            self.word(SELF + 8, 0)
            self.word(SELF + 24, 0)

    def locate(self, at, width):
        for p, data in self.mem.items():
            if p <= at and at + width <= p + len(data):
                return data, at - p
        raise AssertionError(('authored memory outside fixture', hex(at), width))

    def write(self, at, payload):
        data, offset = self.locate(at, len(payload))
        data[offset:offset + len(payload)] = payload

    def word(self, at, value):
        self.write(at, struct.pack('<I', value & 0xffffffff))

    def read(self, at, width=4):
        data, offset = self.locate(at, width)
        return int.from_bytes(data[offset:offset + width], 'little')

    def event(self, token, arguments):
        c = self.c
        c['authored_endpoint_order'].append(token)
        c['authored_endpoint_observations'].append([
            [at, width, self.read(at, width)] for at, width in OBSERVE])
        counts = c['expected_endpoint_counts']
        counts[token] = counts.get(token, 0) + 1
        c['expected_endpoint_arguments'].setdefault(token, []).append(arguments)

    def enumerate(self, names, writes):
        """Explicit per-name destinations; None means rejected capacity."""
        assert len(names) == len(writes)
        self.event('find-first', [PATH, ROLE])
        if not names:
            self.first['responses'].append(dict(value=0xffffffff))
            return
        self.first['responses'].append(dict(value=HANDLE,
            writes_arg_bytes=[(1, FIND_DATA_NAME, (names[0] + b'\0').hex())]))
        for i, (name, destination) in enumerate(zip(names, writes)):
            payload = name + b'\0'
            if destination is not None:
                index, offset = destination
                self.word(self.read(SELF + 4) + index * 4, offset)
                self.write(self.read(SELF + 16) + offset, payload)
                self.word(SELF + 8, index + 1)
                self.word(SELF + 24, offset + len(payload))
            self.event('find-next', [HANDLE, ROLE])
            response = dict(value=int(i + 1 < len(names)))
            if i + 1 < len(names):
                response['writes_arg_bytes'] = [(1, FIND_DATA_NAME,
                    (names[i + 1] + b'\0').hex())]
            self.next['responses'].append(response)
        self.event('find-close', [HANDLE])

    def allocate(self, size, value):
        self.event('allocate', [size])
        self.alloc['responses'].append(dict(value=value))

    def release(self, pointer):
        # The endpoint records a release but deliberately keeps backing bytes
        # mapped. It never calls the host allocator or suppresses repeats.
        self.event('delete', [pointer])

    def finish(self, result):
        self.c['expected_masked_result'] = result
        self.c['endpoints'] = self.specs
        for spec in self.specs:
            self.c['expected_endpoint_counts'].setdefault(spec['token'], 0)
            self.c['expected_endpoint_arguments'].setdefault(spec['token'], [])
        self.c['authored_final_memory'] = [
            (p, bytes(self.mem[p]).hex()) for p, _ in BLOCKS]
        return self.c


def cases(registry=None):
    if registry is not None and not any(
            int(m['address'], 16) == ENTRY for m in registry['members']):
        return []
    out = []

    r = Recipe('nonappend-repeated-offset-delete', append=0x12340000,
               capacity=0)
    r.enumerate([b'ab'], [None])
    r.release(OLD_OFF)                 # 41C859: old pointer stays published.
    r.word(SELF + 12, 0)               # 41C861: only capacity is cleared.
    r.allocate(404, NEW_OFF)
    r.release(OLD_OFF)                 # 41C8AA: second delete, no copies.
    r.word(SELF + 4, NEW_OFF)
    r.word(SELF + 12, 101)
    r.enumerate([b'ab'], [(0, 0)])
    out.append(r.finish(1))

    r = Recipe('nonappend-offset-allocation-failure', capacity=0)
    r.enumerate([b'ab'], [None])
    r.release(OLD_OFF)
    r.word(SELF + 12, 0)
    r.allocate(404, 0)
    out.append(r.finish(0))

    r = Recipe('append-offset-copy', append=0x12340101, capacity=2)
    r.enumerate([b'a', b'bb', b'ccc'], [None, None, None])
    r.allocate(420, NEW_OFF)
    r.write(NEW_OFF, bytes(r.mem[OLD_OFF][:8]))
    r.release(OLD_OFF)
    r.word(SELF + 4, NEW_OFF)
    r.word(SELF + 12, 105)
    r.enumerate([b'a', b'bb', b'ccc'], [(2, 7), (3, 9), (4, 12)])
    out.append(r.finish(1))

    r = Recipe('append-string-DWORD-and-tail-copy', append=1,
               count=1, string_capacity=8)
    r.enumerate([b'abcde', b'wxyz'], [None, None])
    r.allocate(1019, NEW_STR)
    r.write(NEW_STR, bytes(r.mem[OLD_STR][:7]))
    r.release(OLD_STR)
    r.word(SELF + 16, NEW_STR)
    r.word(SELF + 20, 1019)
    r.enumerate([b'abcde', b'wxyz'], [(1, 7), (2, 13)])
    out.append(r.finish(1))

    for failure in (False, True):
        r = Recipe('nonappend-string-' +
                   ('allocation-failure' if failure else 'growth-clears-pointer'),
                   string_capacity=2)
        r.enumerate([b'abcd'], [None])
        r.release(OLD_STR)             # 41C7CA: unlike offsets, clear pointer.
        r.word(SELF + 16, 0)
        r.word(SELF + 20, 0)
        r.allocate(1005, 0 if failure else NEW_STR)
        if not failure:
            r.word(SELF + 16, NEW_STR)
            r.word(SELF + 20, 1005)
            r.enumerate([b'abcd'], [(0, 0)])
        out.append(r.finish(0 if failure else 1))

    r = Recipe('append-rejected-but-total-count-fits-no-growth', append=1,
               capacity=2)
    r.enumerate([b'ab'], [None])
    out.append(r.finish(0))

    r = Recipe('nonappend-first-enumeration-success')
    r.enumerate([b'ab'], [(0, 0)])
    out.append(r.finish(1))

    r = Recipe('nonappend-no-files')
    r.enumerate([], [])
    out.append(r.finish(0))
    return out


definitions = cases
