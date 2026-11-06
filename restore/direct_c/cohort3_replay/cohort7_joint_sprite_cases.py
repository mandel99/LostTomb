"""Focused 53E590 lazy-clipped receiver replay; no image is loaded here.

The real 53F790, 53F7D0 and 40FB60 execute.  File/path/color-update endpoints
are controlled.  Non-global receivers intentionally omit the global facade.
"""
import struct
from cohort7_startup_cases import child

ENTRY = 0x53e590
GLOBAL, EXPLICIT = 0xf55bb0, 0x680000
PIXELS, SCREEN, DECOY = 0x32010000, 0x32020010, 0x32030000
SCRATCH_OFFSET, LIMIT_OFFSET, IMAGE_OFFSET = 0x30f5c0, 0x30f5cc, 0x180e48
IMAGE, FILE_INDEX, FD = 7, 2, 7
NAME = b'clip.bin\0'
PATH = ['stack-cstring', 'sprite-path', (b'data\\' + NAME).hex()]
OFFSET = (8 * ((768 - 4) // 2) + (1024 - 4) // 2) * 2
BASE_PIXELS = b''.join(struct.pack('<H', 0x1200 + i) for i in range(64))
FETCH_PIXELS = b''.join(struct.pack('<H', 0xf81f if i % 5 == 0 else
                                    (i * 197 + 33) & 65535) for i in range(16))


def put(case, at, value):
    case['stores'].append((at, struct.pack('<I', value & 0xffffffff)))


def endpoint(token, address, arity, result='void', value=0):
    spec = child(token, address, arity, value)
    spec['result_kind'] = result
    return spec


def path_input(spec):
    spec['private_input_args'] = {0: dict(role='sprite-path', cstring_max=260)}
    return spec


def make_case(receiver, mode):
    explicit = receiver != GLOBAL
    label = ('explicit' if explicit else 'global') + '/' + mode
    watched = [(0xd3b530, 0xb4), (0xe38aa8, 260), (PIXELS, 128),
               (SCREEN - 16, 64), (DECOY, 64)]
    for manager in (GLOBAL, EXPLICIT):
        watched += [(manager + SCRATCH_OFFSET, 24),
                    (manager + IMAGE_OFFSET + IMAGE * 64, 64),
                    (manager + 0x2044 + FILE_INDEX * 260, 260)]
    c = dict(id='c7-joint-sprite/0053e590/' + label, entry=ENTRY,
             receiver=receiver, arguments=[IMAGE, 71, -93],
             named_arguments=[receiver, IMAGE, 71, -93],
             canonical_arguments=[IMAGE, 71, -93], eflags=0xad7,
             stores=[(PIXELS, BASE_PIXELS), (SCREEN - 16, b'\xa5' * 64),
                     (DECOY, b'\x5a' * 64), (0xd3b530, b'\xa5' * 0xb4),
                     (0xe38aa8, b'\xa5' * 260), (0x6018f8, b'data\\\0')],
             watched=watched,
             write_ranges=[(0xd3b530, 0xb4), (0xe38aa8, 260),
                           (PIXELS, 128), (SCREEN - 16, 64)] +
                          [(m + SCRATCH_OFFSET, 4) for m in (GLOBAL, EXPLICIT)],
             result_mask=0xffffffff, expected_eax=4,
             native_cleanup=12, named_cleanup=0, canonical_cleanup=0,
             canonical_registers_and_flags=False,
             compare_x87_values=True, compare_x87_control=True,
             initial_fpcw=0x27f,
             initial_x87=['0000000000000080ff3f', '000000000000008000c0'])
    if explicit:
        c['routes'] = ['native', 'named']
        c['routes_reason'] = ('FUN_0053e590 is the deliberate global-manager '
                              'facade; explicit receiver R is applicable to '
                              'native ECX and restore_sprite_draw only. '
                              'Separate global cases cover all seven routes.')
    for at, value in [(0xe92e1c, PIXELS), (0xe92e18, SCREEN),
                      (0xe38d94, 8), (0xe38d9c, 4), (0xe38d98, 4),
                      (0xe38dac, 4), (0xe38db0, 4),
                      (0xd3b558, 4), (0xd3b550, 4),
                      (0xd3b55c, 0), (0xd3b554, 0)]:
        put(c, at, value)
    for manager in (GLOBAL, EXPLICIT):
        c['stores'].append((manager + SCRATCH_OFFSET, b'\x5a' * 24))
        put(c, manager + SCRATCH_OFFSET, PIXELS if manager == receiver else DECOY)
        put(c, manager + LIMIT_OFFSET, 32)
        record = bytearray(64)
        struct.pack_into('<IIII', record, 0, PIXELS, 128, 0x11223344, 0)
        struct.pack_into('<HH', record, 20, 8, 8)
        record[0x34], record[0x38] = 1, FILE_INDEX
        c['stores'].append((manager + IMAGE_OFFSET + IMAGE * 64, bytes(record)))
        # A wrong manager must also select a visibly different path.
        name = NAME if manager == receiver else b'wrong.bin\0'
        c['stores'].append((manager + 0x2044 + FILE_INDEX * 260,
                            name + b'\xa5' * (260 - len(name))))

    path = path_input(endpoint('path-prepare', 0x4ce310, 2))
    opn = path_input(endpoint('file-open', 0x568eab, 2, 'full32', FD))
    seek = endpoint('file-seek', 0x5662e3, 3, 'full32', 0)
    read = endpoint('file-read', 0x5660ed, 3, 'full32', 0)
    close = endpoint('file-close', 0x5647eb, 1, 'full32', 0)
    refresh = path_input(endpoint('path-refresh', 0x4ce100, 1))
    finish = endpoint('path-finish', 0x4ce2d0, 0)
    update = endpoint('pixel-update', 0x4cdf20, 2)
    update['observe'] = [(GLOBAL + SCRATCH_OFFSET, 4),
                         (EXPLICIT + SCRATCH_OFFSET, 4)]
    passes = 1 if mode == 'direct-success' else 2
    read_args, seek_args, responses = [], [], []
    for pass_index in range(passes):
        seek_args.append([FD, OFFSET, 0])
        successful = mode == 'direct-success' or (mode == 'zero-then-success' and pass_index == 1)
        for row in range(4):
            destination = PIXELS + (pass_index * 4 + row) * 8
            read_args.append([FD, destination, 8])
            seek_args.append([FD, 8, 1])
            response = dict(value=8 if successful else 0)
            if successful:
                payload = FETCH_PIXELS[row * 8:row * 8 + 8]
                response['mutations'] = [(destination + i, 4,
                                          int.from_bytes(payload[i:i + 4], 'little'))
                                         for i in (0, 4)]
            responses.append(response)
    read['responses'] = responses
    updated = mode != 'zero-then-zero'
    c.update(endpoints=[path, opn, seek, read, close, refresh, finish, update],
             expected_endpoint_counts={'path-prepare': 1, 'file-open': passes,
                 'file-seek': passes * 5, 'file-read': passes * 4,
                 'file-close': passes, 'path-refresh': passes - 1,
                 'path-finish': passes - 1, 'pixel-update': int(updated)},
             expected_endpoint_arguments={
                 'path-prepare': [[PATH, 0x5e06e0]],
                 'file-open': [[PATH, 0x8000]] * passes,
                 'file-seek': seek_args, 'file-read': read_args,
                 'file-close': [[FD]] * passes,
                 'path-refresh': [[PATH]] * (passes - 1),
                 'path-finish': [[]] * (passes - 1),
                 'pixel-update': [[PIXELS, 32]] if updated else []})
    # Optional generic native authored-memory assertion; the current shared
    # runner compares these watched bytes across every applicable C route.
    c['authored_final_memory'] = [(SCREEN, (FETCH_PIXELS if mode == 'direct-success'
                                            else BASE_PIXELS[:32]).hex()),
                                  (receiver + SCRATCH_OFFSET,
                                   struct.pack('<I', PIXELS).hex())]
    return c


def definitions(registry):
    if not any(int(m['address'], 16) == ENTRY for m in registry['members']):
        return []
    return [make_case(receiver, mode) for receiver in (GLOBAL, EXPLICIT)
            for mode in ('direct-success', 'zero-then-success', 'zero-then-zero')]
