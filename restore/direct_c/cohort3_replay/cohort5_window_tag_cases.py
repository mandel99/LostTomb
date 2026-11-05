"""Pure cohort5 window/tag recipes; no image loading or execution.

0041BAE0 uses its actual explicit self and signed DWORD count. Cases cover
nonpositive signed edges, 1..512 allocated rows, the native 100-row boundary,
BYTE flags, surrounding guard fields, and two unrelated live register seeds.
The positive loop is bounded; asynchronous changes to count are not modeled.

005404B0/00540540 execute their reused private append/prepend algorithms.
Mapped lists cover empty/singleton/multiple nodes, stale new-node links,
head-only/tail-only states and nodes reused at head/middle/tail. Payload and
neighbor guards remain watched. All dereferenced nodes are valid allocations;
dangling pointers, list/node storage overlap and concurrency are excluded.
All three logical APIs are void: EAX/flags are deliberately not compared.
"""
import struct

S = 0x32000000
SELF = S + 0x1000
NODES = tuple(S + 0x4000 + i * 0x100 for i in range(6))
OWNERS = {0x41BAE0: 'unit_00260.obj',
          0x5404B0: 'unit_02532.obj', 0x540540: 'unit_02534.obj'}
ENTRIES = tuple(OWNERS)
SEEDS = ((0xA5B6C7D8, 0x202, 19), (0xFEDCBA10, 0xAD7, 137))


def _u32(value):
    return struct.pack('<I', value & 0xFFFFFFFF)


def _pattern(size, salt):
    return bytes((i * 37 + salt) & 255 for i in range(size))


def _case(entry, label, seed, args, named):
    eax, flags, salt = seed
    return dict(id=f'cohort5-window-tag/{entry:08x}/{label}/seed{salt}',
                entry=entry, receiver=SELF, arguments=list(args),
                named_arguments=list(named), initial_eax=eax,
                initial_edx=eax ^ 0x76543210, eflags=flags,
                stores=[], watched=[], write_ranges=[], endpoints=[],
                result_mask=None, native_cleanup=0 if entry == 0x41BAE0 else 4,
                named_cleanup=0, canonical_cleanup=0 if entry == 0x41BAE0 else 4,
                canonical_owner=OWNERS[entry],
                canonical_registers_and_flags=False, budget=200000)


def _guarded(case, address, size, salt):
    # Watch exact guards and all non-output fields; only listed outputs may write.
    case['stores'].append((address - 16, _pattern(size + 32, salt)))
    case['watched'].append((address - 16, size + 32))


def _put(case, address, value):
    case['stores'].append((address, _u32(value)))


def _unlock_cases():
    out = []
    for count in (-0x80000000, -1, 0, 1, 2, 3, 31, 99, 100, 101, 255, 512):
        for seed in SEEDS:
            case = _case(0x41BAE0, f'count{count}', seed, (), (SELF,))
            size = max(0x1904, max(0, count) * 64)
            _guarded(case, SELF, size, seed[2])
            _put(case, SELF + 0x1900, count)
            case['write_ranges'] = [(SELF + 0x3C + i * 64, 1)
                                    for i in range(max(0, count))]
            out.append(case)
    return out


def _tag_cases():
    out = []
    a, b, c, fresh, unused_next, unused_previous = NODES
    # Label, current chain, actual head, actual tail, inserted node, stale links.
    layouts = (
        ('empty-clean', (), 0, 0, fresh, False),
        ('empty-stale-links', (), 0, 0, fresh, True),
        ('singleton-new', (a,), a, a, fresh, True),
        ('three-new', (a, b, c), a, c, fresh, True),
        ('head-only', (a,), a, 0, fresh, True),
        ('tail-only', (a,), 0, a, fresh, True),
        ('reuse-head', (a, b, c), a, c, a, False),
        ('reuse-middle', (a, b, c), a, c, b, False),
        ('reuse-tail', (a, b, c), a, c, c, False),
        ('singleton-reuse', (a,), a, a, a, False),
        ('two-reuse-head', (a, b), a, b, a, False),
        ('two-reuse-tail', (a, b), a, b, b, False),
    )
    for entry in (0x5404B0, 0x540540):
        for label, chain, head, tail, node, stale in layouts:
            for seed in SEEDS:
                case = _case(entry, label, seed, (node,), (SELF, node))
                _guarded(case, SELF, 8, seed[2])
                _put(case, SELF, head)
                _put(case, SELF + 4, tail)
                for i, address in enumerate(NODES):
                    _guarded(case, address, 16, (seed[2] + i * 29) & 255)
                    _put(case, address, 0)
                    _put(case, address + 4, 0)
                for i, address in enumerate(chain):
                    _put(case, address, chain[i + 1] if i + 1 < len(chain) else 0)
                    _put(case, address + 4, chain[i - 1] if i else 0)
                if stale:
                    _put(case, node, unused_next)
                    _put(case, node + 4, unused_previous)
                # Both node links are outputs. Payload/key and unrelated nodes stay guarded.
                case['write_ranges'] = [(SELF, 8), (node, 8)]
                neighbor = tail if entry == 0x5404B0 else head
                if neighbor:
                    case['write_ranges'].append((neighbor if entry == 0x5404B0
                                                 else neighbor + 4, 4))
                out.append(case)
    return out


def definitions(registry):
    present = {int(member['address'], 16) for member in registry['members']}
    assert set(ENTRIES) <= present, ('cohort5 window/tag members absent',
                                     sorted(set(ENTRIES) - present))
    out = _unlock_cases() + _tag_cases()
    assert len(out) == 72
    assert {case['entry'] for case in out} == set(ENTRIES)
    assert len({case['id'] for case in out}) == len(out)
    return out
