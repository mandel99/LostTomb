"""Authored current-image cases for five existing, unselected C entries.

No image is loaded here. Existing file-list recipes supply controlled Win32
imports and allocator endpoint declarations; real41C740/41C8F0 children run.
"""
import struct
from cohort7_joint_filelist_cases import Recipe as FileRecipe
from cohort7_joint_filelist_cases import (SELF, PATH, OLD_OFF, OLD_STR,
    NEW_OFF, NEW_STR, HANDLE, FIND_DATA_NAME, ROLE, OBSERVE, BLOCKS)

TARGETS = (0x4062f0, 0x406620, 0x4192b0, 0x41c8f0, 0x41df70)
OUTPUTS = 0x32008000


def pack(value, width=4):
    return (value & ((1 << (width * 8)) - 1)).to_bytes(width, 'little')


def base(entry, label, blocks, receiver=0x89abcdef, arguments=(), named=None,
         result=None, cleanup=0):
    return dict(id=f'c7-compiled-first5/{entry:08x}/' + label, entry=entry,
        receiver=receiver, arguments=list(arguments),
        named_arguments=list(arguments if named is None else named),
        canonical_arguments=list(arguments if named is None else named),
        eflags=0xad7, initial_eax=0xf1e2d3c4,
        stores=[(p, bytes(data)) for p, data in blocks.items()],
        watched=[(p, len(data)) for p, data in blocks.items()], write_ranges=[],
        native_cleanup=cleanup, named_cleanup=0, canonical_cleanup=0,
        result_mask=result, canonical_registers_and_flags=False,
        endpoints=[], expected_endpoint_counts={},
        expected_endpoint_arguments={}, authored_endpoint_order=[],
        authored_endpoint_observations=[], expected_invocations=7)


def button_cases():
    out = []
    for value in (0, 0xabcdef01):
        blocks = {0xeadcbc: bytearray(b'\xa5' * 12),
                  0xe38dc9: bytearray(b'\xd4' * 3)}
        blocks[0xeadcbc][4:8] = pack(value)
        c = base(0x4062f0, f'DWORD-{value:08x}-BYTE-dirty', blocks,
                 named=[0x4062f0])
        c.update(routes=['native', 'named'],
            routes_reason='Per-entry helper is inlined out of all three images. Named provider is the retained token-dispatch arm; no canonical per-entry symbol is invented.',
            expected_invocations=4,
            write_ranges=[(0xeadcc0, 4), (0xe38dca, 1)])
        blocks[0xeadcbc][4:8] = pack(1)
        blocks[0xe38dc9][1] = 1
        c['authored_final_memory'] = [(p, bytes(data).hex()) for p, data in blocks.items()]
        out.append(c)
    for label, count, owner, offsets in [
            ('zero-count-still-loads-owner', 0, 255, []),
            ('negative-signed-DWORD-count', 0x80000000, 2, []),
            ('offset-DWORD-units-and-duplicate', 3, 2, [1, 197, 1]),
            ('owner-byte-alias-keeps-cached-owner', 2, 255, None)]:
        selected = 3
        owner_at = 0x799a99 + selected * 264
        player_base = 0x7900c2 + owner * 196
        if offsets is None:
            offsets = [(owner_at - player_base) & 0xffffffff, 1]
        targets = [((player_base + off) & 0xffffffff) for off in offsets]
        blocks = {0xeadd6c: bytearray(pack(selected)),
            0xf53dd8: bytearray(pack(count)),
            0xf53e70: bytearray(b''.join(pack(x) for x in offsets) + b'\xa5' * 16),
            owner_at: bytearray([owner]), 0xe38dc9: bytearray(b'\xd4' * 3)}
        for at in targets:
            if at not in blocks:
                blocks[at] = bytearray(b'\xc5')
        c = base(0x406620, label, blocks, named=[0x406620])
        c.update(routes=['native', 'named'],
            routes_reason='Actual compiled algorithm is the retained restore_selection_button_action(406620) arm; no separate canonical symbol survives optimization.',
            expected_invocations=4,
            write_ranges=[(at, 1) for at in set(targets + [0xe38dca])],
            read_ranges=[(owner_at, 1)], expected_reads=[[owner_at, 1]])
        for at in targets:
            blocks[at][0] = 1
        blocks[0xe38dc9][1] = 1
        c['authored_final_memory'] = [(p, bytes(data).hex()) for p, data in blocks.items()]
        out.append(c)
    return out


def summary_cases():
    out = []
    for label in ('signed-nonpositive-slots', 'mixed-BYTE-progress',
                  'three-complete-fresh-render-globals', 'signed-WORD-progress'):
        player = 2
        off = player * 0x49d8
        selected = 0xea5fa0 + off
        progress_at = 0xea5baa + off
        blocks = {0xeaac28: bytearray(pack(player)),
            selected: bytearray(struct.pack('<6I', 31, 32, 33, 1, 2, 3)),
            progress_at: bytearray(pack(16, 2)),
            0xf53db4: bytearray(pack(100)), 0xf53db8: bytearray(pack(200)),
            0xf56f8c: bytearray(pack(0x71234567))}
        for i, (kind, percentage) in enumerate(((0xcf, 100), (0xd0, 99), (0xd2, 0)), 1):
            row = bytearray(b'\xa5' * 264)
            row[16:18], row[0x55] = pack(kind, 2), percentage
            blocks[0x7999d8 + i * 264] = row
        if label == 'signed-nonpositive-slots':
            blocks[selected][12:24] = struct.pack('<3I', 0, 0xffffffff, 0x80000000)
        elif label == 'mixed-BYTE-progress':
            blocks[0x7999d8 + 3 * 264][16:18] = pack(0x123, 2)
        elif label == 'signed-WORD-progress':
            blocks[0x7999d8 + 264][0x55] = 99
            blocks[progress_at][:] = pack(0xffff, 2)
        else:
            blocks[0x7999d8 + 2 * 264][0x55] = 100
        c = base(0x4192b0, label, blocks, result=0xffffffff)
        render = dict(token='render-completed-monument', native=0x4ccbb0,
            compiled_symbols=['FUN_004ccbb0'], arity=6, native_cleanup=0,
            compiled_cleanup=0, result_kind='void', value=0,
            clobber_ecx=0xdead4321, clobber_edx=0xbeef3456,
            observe=[(0xeaac28, 4), (0xf53db4, 4), (0xf53db8, 4), (0xf56f8c, 4)],
            responses=[])
        count = 0
        if label == 'mixed-BYTE-progress':
            count = 1
            c['expected_endpoint_arguments']['render-completed-monument'] = [[198, 31, 475, 300, 0x71234567, 0]]
            render['responses'] = [{}]
        elif label == 'three-complete-fresh-render-globals':
            count = 3
            c['expected_endpoint_arguments']['render-completed-monument'] = [
                [198, 31, 475, 300, 0x71234567, 0],
                [198, 32, 259, 476, 0x81234567, 0],
                [198, 33, 291, 0xc0, 0x92345678, 0]]
            render['responses'] = [dict(mutations=[(0xf53db4, 4, 256),
                (0xf53db8, 4, 0xfffffff0), (0xf56f8c, 4, 0x81234567)]),
                dict(mutations=[(0xf53db4, 4, 0xffffffd0),
                (0xf53db8, 4, 16), (0xf56f8c, 4, 0x92345678)]), {}]
            c['write_ranges'] = [(0xf53db4, 4), (0xf53db8, 4), (0xf56f8c, 4)]
        c['endpoints'] = [render]
        c['expected_endpoint_counts'] = {'render-completed-monument': count}
        c['expected_endpoint_arguments'].setdefault('render-completed-monument', [])
        c['expected_eax'] = count
        for response in render['responses']:
            c['authored_endpoint_order'].append(render['token'])
            c['authored_endpoint_observations'].append([
                [at, width, int.from_bytes(blocks[at], 'little')]
                for at, width in render['observe']])
            for at, width, value in response.get('mutations', []):
                blocks[at][:] = pack(value, width)
        c['authored_final_memory'] = [(p, bytes(data).hex()) for p, data in blocks.items()]
        out.append(c)
    return out


class DirectEnumeration(FileRecipe):
    def __init__(self, label, count=0, capacity=8, used=0, string_capacity=64,
                 alias_outputs=False):
        super().__init__(label, append=1, count=count, capacity=capacity,
                         used=used, string_capacity=string_capacity)
        self.bytes_out = OUTPUTS
        self.files_out = OUTPUTS if alias_outputs else OUTPUTS + 4
        self.mem[OUTPUTS] = bytearray(b'\x9b' * 8)
        self.c['stores'].append((OUTPUTS, bytes(self.mem[OUTPUTS])))
        self.c['watched'] = self.c['watched'] + [(OUTPUTS, 8)]
        self.c['write_ranges'] = self.c['write_ranges'] + [(OUTPUTS, 8)]
        args = [PATH, self.bytes_out, self.files_out]
        self.c.update(id='c7-compiled-first5/0041c8f0/' + label,
            entry=0x41c8f0, arguments=args, named_arguments=[SELF, *args],
            canonical_arguments=[SELF, *args], native_cleanup=12)
        self.observe = OBSERVE + [(OUTPUTS, 4), (OUTPUTS + 4, 4)]
        for spec in self.specs:
            spec['observe'] = self.observe
        self.word(self.bytes_out, 0)
        self.word(self.files_out, 0)

    def event(self, token, arguments):
        self.c['authored_endpoint_order'].append(token)
        self.c['authored_endpoint_observations'].append([
            [at, width, self.read(at, width)] for at, width in self.observe])
        counts = self.c['expected_endpoint_counts']
        counts[token] = counts.get(token, 0) + 1
        self.c['expected_endpoint_arguments'].setdefault(token, []).append(arguments)

    def enumerate(self, names, destinations):
        assert len(names) == len(destinations)
        self.event('find-first', [PATH, ROLE])
        if not names:
            self.first['responses'].append(dict(value=0xffffffff))
            return
        self.first['responses'].append(dict(value=HANDLE,
            writes_arg_bytes=[(1, FIND_DATA_NAME, (names[0] + b'\0').hex())]))
        for i, (name, destination) in enumerate(zip(names, destinations)):
            payload = name + b'\0'
            # Native41C958 publishes file count before41C961 byte count;
            # aliases are intentional and retain that ordered dependence.
            self.word(self.files_out, self.read(self.files_out) + 1)
            self.word(self.bytes_out, self.read(self.bytes_out) + len(payload))
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

    def finish(self, result):
        c = super().finish(result)
        c['authored_final_memory'].append((OUTPUTS, bytes(self.mem[OUTPUTS]).hex()))
        return c


def enumeration_cases():
    out = []
    for label, kwargs, names, destinations, result in [
        ('no-files-initializes-outputs', {}, [], [], 0),
        ('three-files-existing-prefix', dict(count=2, used=7),
            [b'a', b'bb', b'ccc'], [(2, 7), (3, 9), (4, 12)], 1),
        ('offset-capacity-rejected', dict(count=2, capacity=2, used=7),
            [b'ab'], [None], 0),
        ('exact-string-capacity-rejected', dict(string_capacity=3),
            [b'ab'], [None], 0),
        ('one-spare-string-byte-succeeds', dict(string_capacity=4),
            [b'ab'], [(0, 0)], 1),
        ('partial-fill-retains-failed-status', dict(capacity=1),
            [b'a', b'bbb'], [(0, 0), None], 0),
        ('aliased-output-counters-ordered-writes', dict(alias_outputs=True),
            [b'ab'], [(0, 0)], 1)]:
        r = DirectEnumeration(label, **kwargs)
        r.enumerate(names, destinations)
        out.append(r.finish(result))
    return out


def mission_cases():
    out = []
    for label, count in [('zero-count', 0), ('negative-signed-count', 0xffffffff),
                         ('DWORD-mode-flag-array', 3), ('AL-only-above255-count', 257),
                         ('array-store-aliases-live-bound', 3)]:
        self = 0x700978 if label == 'array-store-aliases-live-bound' else SELF
        blocks = {self: bytearray(b'\xa5' * 0x500),
            0x704db0: bytearray(pack(0x100))}
        if self <= 0x700994 < self + 0x500:
            blocks[self][0x700994-self:0x700998-self] = pack(count)
        else:
            blocks[0x700994] = bytearray(pack(count))
        c = base(0x41df70, label, blocks, receiver=self,
            arguments=[PATH, 0x12340101], named=[self, PATH, 0x12340101],
            result=0xff, cleanup=8)
        c['write_ranges'] = [(self, 0x500)]
        rows = 1 if label == 'array-store-aliases-live-bound' else (0 if count & 0x80000000 else count)
        blocks[self][8:12] = pack(rows)
        for i in range(rows):
            blocks[self][28+i*4:32+i*4] = pack(i)
        c['expected_masked_result'] = 1
        c['authored_final_memory'] = [(p, bytes(data).hex()) for p, data in blocks.items()]
        out.append(c)
    # Reuse already-authored connected41C740 scenarios without intercepting it.
    from cohort7_joint_filelist_cases import cases as file_cases
    for old in file_cases()[:2]:
        old['watched'] = list(old['watched'])
        old.update(entry=0x41df70,
            id=old['id'].replace('c7-joint-filelist/0041c740/',
                'c7-compiled-first5/0041df70/real-child-'))
        for at, value in [(0x704db0, 0), (0x700994, 0x12345678)]:
            old['stores'].append((at, pack(value)))
            old['watched'].append((at, 4))
            old['authored_final_memory'].append((at, pack(value).hex()))
        out.append(old)
    return out


def cases(registry=None):
    result = button_cases() + summary_cases() + enumeration_cases() + mission_cases()
    if registry is None:
        return result
    present = {int(m['address'], 16) for m in registry['members']}
    return [c for c in result if c['entry'] in present]


definitions = cases
