"""Current-image storage limit matrix, reused from the saved native protocol.

The actual page-size helper executes. No external endpoint is substituted.
The original table-click/dispatcher routes remain separately historical.
"""
import itertools
import struct

ENTRY = 0x406460


def put(case, at, value, width=4):
    case['stores'].append((at, int(value & ((1 << (8 * width)) - 1)).to_bytes(width, 'little')))


def make_case(index, cfg):
    building, owner, row = 7, cfg.get('owner', 1), cfg.get('row', 0)
    offset = building * 264
    owner_at = 0x7900c2 + owner * 196
    position_at = 0x799ab8 + offset
    watched = [(owner_at - 8, 212), (0x7999e8 + offset, 216),
               (0xe38dc9, 3), (0xd3b194, 8), (0xf53ddc, 296)]
    changes = [(position_at, 4), (owner_at, 196), (0xe38dca, 1)]
    c = dict(id='c7-joint-storage/00406460/%03d' % index, entry=ENTRY,
             receiver=0xa1b2c3d4, arguments=[], named_arguments=[],
             stores=[], watched=watched, write_ranges=changes,
             write_trace_ranges=changes, result_mask=None,
             native_cleanup=0, named_cleanup=0, canonical_cleanup=0,
             canonical_registers_and_flags=False, eflags=0xad7,
             compare_x87_values=True, compare_x87_control=True,
             initial_fpcw=0x27f,
             initial_x87=['0000000000000080ff3f', '000000000000008000c0'],
             endpoints=[], expected_endpoint_counts={})
    for at, value, width in [(0xeadd6c, building, 4), (0x799a99 + offset, owner, 1),
            (0x7999e8 + offset, cfg.get('kind', 70), 2),
            (position_at, cfg.get('position', 1), 4),
            (0xe38e68, cfg.get('mode', 0), 4), (0xf53e6c, 35, 4),
            (0xd3b194, row, 4), (0xd3b198, cfg.get('action', 1), 4),
            (0xe38dca, 0x83, 1)]:
        put(c, at, value, width)
    for resource in range(36):
        put(c, 0xf53ddc + resource * 4, resource)
        put(c, 0xf53e70 + resource * 4, resource)
        put(c, owner_at + resource, cfg.get('policy', 0), 1)
        put(c, 0x7900ec + owner * 196 + resource * 2, cfg.get('limit', 1600), 2)
        put(c, 0x790134 + owner * 196 + resource * 2, cfg.get('limit', 1600), 2)
    position = cfg.get('position', 1)
    pages = {1: 15, 2: 21, 3: 24}.get(cfg.get('mode', 0), 10)
    trace = []
    if position == 0:
        position = 1
        trace.append([position_at, 4, position])
    elif (position if position < 0x80000000 else position - 0x100000000) > pages:
        position = 36 - pages
        trace.append([position_at, 4, position])
    resource = (((position + row) & 0xffffffff) - 1
                if cfg.get('kind', 70) in (72, 193) else row)
    assert 0 <= resource < 36
    final = [(position_at, struct.pack('<I', position).hex()), (0xe38dca, '01')]
    action, policy = cfg.get('action', 1), cfg.get('policy', 0)
    if action in (0, 1):
        limit = cfg.get('limit', 1600)
        limit = max(800, limit - 800) if action == 1 else min(3200, limit + 800)
        at = (0x7900ec if policy == 0 else 0x790134) + owner * 196 + resource * 2
        update = [at, 2, limit & 0xffff]
        redraw_first = policy == 0 or (action == 1 and cfg.get('limit', 1600) - 800 < 800)
        trace += [[0xe38dca, 1, 1], update] if redraw_first else [update, [0xe38dca, 1, 1]]
        final.append((at, struct.pack('<H', limit & 0xffff).hex()))
    else:
        trace.append([0xe38dca, 1, 1])
    c['expected_write_trace'] = trace
    c['authored_final_memory'] = final
    return c


def definitions(registry):
    if not any(int(m['address'], 16) == ENTRY for m in registry['members']):
        return []
    configurations = [dict(action=a, policy=p, limit=n) for a, p, n in itertools.product(
        [0, 1, 2, 0xffffffff], [0, 1, 255],
        [-32768, -1, 0, 799, 800, 1599, 1600, 2400, 3200, 32767])]
    configurations += [dict(mode=m, position=p, kind=k, row=2, action=1, policy=1, limit=2400)
        for m, p, k in itertools.product([0, 1, 2, 3, 4], [0, 1, 10, 25, 0xffffffff], [70, 72, 193])]
    configurations += [dict(owner=o, row=r, policy=0, action=0, limit=1600)
        for o, r in itertools.product([0, 255], [0, 1, 29])]
    assert len(configurations) == 201
    return [make_case(i, cfg) for i, cfg in enumerate(configurations)]
