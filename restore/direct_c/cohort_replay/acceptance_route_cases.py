"""Pure finite fixtures for cohort2 ledger members [27:69].

Native is the reference. These cases exercise selected paths, not complete
instruction coverage or gameplay equivalence. No PE, emulator or runner import.
Child arity counts native stack slots; receiver/incoming_eax describe explicit
named-C inputs. Protocol result queues contain complete physical EAX words.
"""
from copy import deepcopy
import struct

GROUP = 'cohort2-acceptance-route'
S = 0x32000000
RECEIVER = S + 0x1000
VTABLE = S + 0x2000
TARGETS = S + 0x3000
ACTOR = 1
BUILDING = 2
CELL = 1000 + 11 * 228 + 10
DIST = 0xeed760


def _child(name, arity, result='full32', cleanup=0, receiver=False,
           aliases=(), incoming_eax=False):
    result = dict(name=name, arity=arity, result=result,
                  native_cleanup=cleanup, compiled_cleanup=0,
                  aliases=list(aliases), private_output_args=[], arg_masks={})
    if receiver:
        result['receiver'] = True
    if incoming_eax:
        result['incoming_eax'] = True
    return result


CHILDREN = {
    # Every fill in this module executes its real composed implementation.
    0x40f8f0: _child('FUN_0040f8f0', 3, 'void', aliases=('restore_memory_fill_bytes',)),
    0x51aff0: _child('FUN_0051aff0', 1),
    0x51cba0: _child('FUN_0051cba0', 4),
    0x47e230: _child('FUN_0047e230', 2),
    0x45bb50: _child('FUN_0045bb50', 3),
    0x422a90: _child('FUN_00422a90', 2),
    0x420430: _child('FUN_00420430', 2,
        aliases=('restore_actor_approach_kind213_stage_worker_available_raw',)),
    0x420910: _child('FUN_00420910', 2),
    0x420f50: _child('FUN_00420f50', 2),
    0x421230: _child('FUN_00421230', 2),
    0x420c40: _child('FUN_00420c40', 2),
    0x4e0530: _child('FUN_004e0530', 1, 'al8'),
    0x542110: _child('FUN_00542110', 1, 'al8'),
    # These scanners call the existing full-EAX cdecl RAW surfaces.
    0x5421d0: _child('FUN_005421d0', 3, aliases=(
        'restore_actor_approach_kind213_accept_raw',)),
    0x4e05f0: _child('FUN_004e05f0', 3, aliases=(
        'restore_actor_approach_kind222_accept_raw',)),
    0x401ed0: _child('restore_actor_find_approach_kind230_mode', 4,
        cleanup=16, receiver=True, aliases=('restore_actor_member_find_kind230_raw',)),
    0x401d40: _child('restore_actor_kind230_accept', 4, 'al8',
        cleanup=16, receiver=True, aliases=(
            'restore_acceptance_kind230_check_raw',
            'restore_acceptance_kind230_check_logical')),
    0x547c10: _child('restore_actor_member_accept_building_core', 3,
        cleanup=12, receiver=True),
    0x4176a0: _child('restore_actor_accept_base_kind227', 3, 'al8',
        cleanup=12, receiver=True),
    0x4e6c10: _child('restore_actor_accept_base_kind228', 3, 'al8',
        cleanup=12, receiver=True),
    0x40ed70: _child('restore_actor_accept_base_kind225', 3, 'al8',
        cleanup=12, receiver=True),
    0x417590: _child('restore_monument_base_worker_result', 1,
        cleanup=4, incoming_eax=True),
    0x4e6b20: _child('restore_monument_base_worker_result', 1,
        cleanup=4, incoming_eax=True),
    0x40ec20: _child('restore_monument_base_worker_result', 1,
        cleanup=4, incoming_eax=True),
    0x401b40: _child('restore_monument_worker_query_00401b40', 1,
        cleanup=4, incoming_eax=True),
    0x5477e0: _child('restore_monument_worker_active', 1, 'al8', cleanup=4),
    0x4f0190: _child('restore_route_approach_004f0190', 3),
    0x4f01c0: _child('restore_route_approach_004f01c0', 3),
    0x420350: _child('FUN_00420350', 3),
    0x4e5c00: _child('FUN_004e5c00', 3),
    0x542a80: _child('restore_actor_find_approach_kind213', 3,
        aliases=('restore_route_approach_00542a80',)),
    0x4e0c00: _child('restore_actor_find_approach_kind222', 3,
        aliases=('restore_route_approach_004e0c00',)),
}

for _token, _name in {
    0x4178e0: 'restore_actor_find_approach_kind227',
    0x4e6e10: 'restore_actor_find_approach_kind228',
    0x40f070: 'restore_actor_find_approach_kind225',
    0x401f30: 'restore_actor_find_approach_kind230',
    0x545be0: 'restore_actor_find_approach_kind229',
    0x5590c0: 'restore_actor_find_approach_kind234',
    0x556430: 'restore_actor_find_approach_kind235',
    0x54fb40: 'restore_actor_find_approach_kind236',
}.items():
    CHILDREN[_token] = _child(_name, 3, cleanup=12, receiver=True)


def endpoints():
    return deepcopy(CHILDREN)


CASES = []


def _case(fn, label, cfg, protocol=False, group='acceptance'):
    cfg = deepcopy(cfg)
    if protocol:
        cfg.setdefault('controlled', [])
    CASES.append(dict(id='%s/%06x/%s' % (group, fn, label), fn=fn,
                      origin='protocol' if protocol else 'actual-composition', cfg=cfg))


def _actor(field, actor=ACTOR):
    return field + actor * 388


def _building(field, building=BUILDING):
    return field + building * 264


def _member(arguments, query=0x5477e0, kind=81):
    return dict(arguments=list(arguments), receiver=RECEIVER, actor=ACTOR,
                kind=kind, stores=[(RECEIVER, VTABLE), (VTABLE + 36, query),
                                   (RECEIVER + 88, 0)],
                words=[(_building(0x7999ea), 0)])


def _scan_record(cfg, building, kind, active=1, phase=0):
    cfg.setdefault('bytes', []).append((_building(0x7999e0, building), active))
    cfg.setdefault('words', []).append((_building(0x7999e8, building), kind))
    cfg.setdefault('stores', []).append((_building(0x799a38, building), phase))


def _destination(cfg, building=3):
    cfg.setdefault('words', []).extend([
        (_building(0x7999fe, building), 0x1234),
        (_building(0x799a00, building), 0x5678)])
    cfg.setdefault('stores', []).append((_building(0x7999e4, building), 0x10203040))


# Three virtual scans retain ECX and test only low AL, even with nonzero EAX.
for _fn, _kind, _accept in [
    (0x4178e0, 227, 0x4176a0), (0x4e6e10, 228, 0x4e6c10),
    (0x40f070, 225, 0x40ed70)]:
    _cfg = _member([0x12345678, 0x87654321, ACTOR])
    _cfg['stores'].append((VTABLE + 44, _accept))
    _case(_fn, 'empty-scan', _cfg)
    _scan_record(_cfg, 1, _kind, active=2)
    _scan_record(_cfg, 2, _kind)
    _scan_record(_cfg, 3, _kind)
    _cfg.update(controlled=[_accept], results={_accept: [0xa1b2c300, 0xd4e5f601]})
    _case(_fn, 'inactive-skip-low-al-reject-then-accept', _cfg, True)

for _value in [0, 0x89abcdef]:
    _cfg = _member([0xfedc1234, 0x87654321, ACTOR])
    _cfg.update(controlled=[0x401ed0], results={0x401ed0: [_value]})
    _case(0x401f30, 'forward-mode-zero-%08x' % _value, _cfg, True)
    _cfg = _member([BUILDING, ACTOR, 0x7f123401])
    _cfg.update(controlled=[0x547c10], results={0x547c10: [_value]})
    _case(0x559110, 'forward-all-mode-bits-%08x' % _value, _cfg, True)

for _fn, _kind, _accept in [(0x542a80, 213, 0x5421d0), (0x4e0c00, 222, 0x4e05f0)]:
    _cfg = dict(arguments=[0x11223344, 0x55667788, ACTOR])
    _case(_fn, 'empty-scan', _cfg)
    _scan_record(_cfg, 1, _kind)
    _scan_record(_cfg, 2, _kind)
    _cfg.update(controlled=[_accept], results={_accept: [0xf1e2d300, 0xa5b6c701]})
    _case(_fn, 'full-eax-nonzero-al-zero-then-success', _cfg, True)

_chain = [0x4f0190, 0x420350, 0x4e5c00, 0x542a80, 0x40f070, 0x401f30,
          0x4e6e10, 0x545be0, 0x5590c0, 0x556430, 0x54fb40, 0x4178e0, 0x4e0c00]
for _label, _hit, _value in [('first-child', 0, 0x12345600),
                            ('member-child', 6, 0x87654321),
                            ('final-child', 12, 0xfedcba98)]:
    _case(0x4bc1e0, _label, dict(arguments=[0x12345678, 0x87654321, ACTOR],
        controlled=_chain, results={t: [_value if i == _hit else 0]
                                   for i, t in enumerate(_chain)}), True)
for _value in [0, 0xa5b6c700]:
    _case(0x4bc480, 'full-result-%08x' % _value, dict(
        arguments=[0x13572468, 0x24681357, ACTOR], controlled=[0x4f01c0],
        results={0x4f01c0: [_value]}), True)

for _fn, _kind in [(0x545be0, 229), (0x5590c0, 234),
                    (0x556430, 235), (0x54fb40, 236)]:
    _cfg = _member([0xdeadbeef, 0x12345678, ACTOR])
    _case(_fn, 'empty-scan', _cfg)
    if _kind == 236:
        _scan_record(_cfg, 1, _kind, phase=4)
    _scan_record(_cfg, 2, _kind, phase=3)
    _scan_record(_cfg, 3, _kind, phase=0xffffffff)
    _cfg.update(controlled=[0x547c10], results={0x547c10: [0xaabbcc00, 0xddeeff01]})
    _case(_fn, 'ordered-low-al-selection-and-phase-filter', _cfg, True)

_cfg = _member([0xfedc1234, 0x89abcdef, ACTOR, 0x76543201])
_case(0x401ed0, 'empty-scan', _cfg)
_scan_record(_cfg, 1, 230)
_scan_record(_cfg, 2, 230)
_cfg.update(controlled=[0x401d40], results={0x401d40: [0x11112200, 0x33334401]})
_case(0x401ed0, 'preserve-mode-and-low-al-selection', _cfg, True)

# Core member: an occupied worker rejects with the vtable high24 still live.
_cfg = _member([BUILDING, ACTOR, 1])
_cfg['stores'].append((_building(0x799a34), 3))
_cfg['bytes'] = [(_actor(0xbcd602, 3), 81), (_actor(0xbcd606, 3), 1),
                 (_actor(0xbcd630, 3), 9)]
_case(0x547c10, 'occupied-query-preserves-vtable-eax', _cfg)

_cfg = _member([BUILDING, ACTOR, 0x01000101], kind=85)
_cfg['stores'].extend([(_building(0x799a70), 399), (RECEIVER + 76, 0)])
_cfg['words'].extend([(0xd3b4e4, 0x1234), (0xd3b4e0, 0x5678)])
_destination(_cfg)
_cfg.update(controlled=[0x45bb50], results={0x45bb50: [3]})
_case(0x547c10, 'laborer-special-resource-commit', _cfg, True)

_cfg = _member([BUILDING, ACTOR, 1], kind=81)
_cfg['stores'].append((_building(0x799a70), 100))
_destination(_cfg)
_cfg.update(controlled=[0x422a90], results={0x422a90: [3]})
_case(0x547c10, 'ordinary-stock-100-commit', _cfg, True)

# Two acceptance algorithms, with explicit descriptor branches and bounded
# external worker queries. No external algorithm is claimed by these protocols.
_cfg = dict(arguments=[BUILDING, ACTOR, 0x100], kind=79,
            stores=[(0x5e2ae4, 4), (_building(0x799a3c), 0)],
            words=[(_building(0x7999ea), 0)])
_case(0x4e05f0, 'type79-before-phase1-rejected', _cfg)
_cfg = dict(arguments=[BUILDING, ACTOR, 1], kind=85,
            stores=[(0x5e2ae4, 4), (_building(0x799a3c), 0),
                    (_building(0x799a30), 0)],
            words=[(_building(0x7999ea), 0)], controlled=[0x4e0530],
            results={0x4e0530: [0x6a7b8c00]})
_destination(_cfg, BUILDING)
_case(0x4e05f0, 'laborer-unoccupied-initial-commit', _cfg, True)

_cfg = dict(arguments=[BUILDING, ACTOR, 1], kind=81,
            stores=[(0x601de4, 0), (_building(0x799a3c), 0)],
            words=[(_building(0x7999ea), 0)], controlled=[0x542110, 0x420430],
            results={0x542110: [0x13572400], 0x420430: [BUILDING]})
_destination(_cfg, BUILDING)
_case(0x5421d0, 'descriptor-zero-stage-worker-commit', _cfg, True)
for _commit in [0x100, 0x101]:
    _cfg = dict(arguments=[BUILDING, ACTOR, _commit], kind=85,
                stores=[(0x601de4, 1), (_building(0x799a3c), 0),
                        (_building(0x799a30), 0)],
                words=[(_building(0x7999ea), 0)], controlled=[0x542110],
                results={0x542110: [0xaabbcc00]})
    _destination(_cfg, BUILDING)
    _case(0x5421d0, 'initial-laborer-commit-low-byte-%02x' % (_commit & 255), _cfg, True)

_cfg = _member([BUILDING, ACTOR, 1, 1], query=0x401b40)
_cfg['stores'].append((RECEIVER + 88, 0xffffffff))
_case(0x401d40, 'receiver-negative-state-rejected', _cfg)
for _kind, _mode in [(81, 0), (79, 1)]:
    _cfg = _member([BUILDING, ACTOR, 0x101, _mode], query=0x401b40, kind=_kind)
    _destination(_cfg, BUILDING)
    _cfg.update(controlled=[0x420c40], results={0x420c40: [BUILDING]})
    _case(0x401d40, 'commit-kind%d-mode%d' % (_kind, _mode), _cfg, True)

# Existing availability descriptors: all pointers refer to a bounded scratch
# record. A mismatch forces the whole native fixed-size table scan.
for _fn, _first, _rows in [(0x420430, 0x596088, 13),
                           (0x420910, 0x59edf0, 27),
                           (0x420f50, 0x5ad218, 27),
                           (0x421230, 0x5b0060, 15)]:
    for _match in [True, False]:
        _record = S + 0x4000
        _raw = b''.join(struct.pack('<IIII', 7, 81 if _match else 79, _record, 0)
                        for _ in range(_rows))
        _cfg = dict(arguments=[BUILDING, ACTOR], kind=81,
                    stores=[(_building(0x799a30), 7), (_record + 12, 1)],
                    words=[(_record + 18, 0), (_building(0x7999ea), 0)],
                    regions=[(_first - 8, _raw)])
        _case(_fn, 'descriptor-first-match' if _match else 'descriptor-full-miss', _cfg)

# Full register queries use deliberately nonzero incoming high24. The active
# helper at5477E0 has only its AL result compared by the retained root API.
for _fn in [0x417590, 0x4e6b20, 0x40ec20, 0x401b40, 0x5477e0]:
    _cfg = dict(arguments=[3], incoming_eax=0xa1b2c3f4,
                stores=[(_building(0x799a34, 3), 0)],
                words=[(_building(0x7999ea, 3), 0)])
    _case(_fn, 'empty-worker-nonzero-incoming-high24', _cfg)
    _worker_kind = 108 if _fn == 0x5477e0 else (81 if _fn == 0x401b40 else 85)
    _state = {0x417590: 34, 0x4e6b20: 35, 0x40ec20: 31,
              0x401b40: 11, 0x5477e0: 12}[_fn]
    _cfg['stores'] = [(_building(0x799a34, 3), 2)]
    _cfg['bytes'] = [(_actor(0xbcd606, 2), 1), (_actor(0xbcd602, 2), _worker_kind),
                     (_actor(0xbcd630, 2), 7), (_actor(0xbcd608, 2), _state)]
    _case(_fn, 'suspended-worker-effective-terminal-state', _cfg)
    _cfg['bytes'] = [(_actor(0xbcd606, 2), 1), (_actor(0xbcd602, 2), 1),
                     (_actor(0xbcd630, 2), 9)]
    _case(_fn, 'unsupported-worker-clears-building-slot', _cfg)

for _fn, _query in [(0x4176a0, 0x417590), (0x4e6c10, 0x4e6b20)]:
    _cfg = _member([BUILDING, ACTOR, 1], query=_query, kind=85)
    _cfg['stores'].append((RECEIVER + 88, 1))
    _case(_fn, 'receiver-reserved-reject', _cfg)
    _cfg = _member([BUILDING, ACTOR, 0x100], query=_query, kind=85)
    _cfg['stores'].extend([(_building(0x799a34), 2), (_building(0x799a30), 18)])
    _cfg['bytes'] = [(_actor(0xbcd602, 2), 81), (_actor(0xbcd630, 2), 10),
                     (_actor(0xbcd602, 3), 0)]
    _cfg['words'].append((_actor(0xbcd660, 2), 3))
    _case(_fn, 'query-clears-stale-follower-without-commit', _cfg)
    _cfg['arguments'][-1] = 0x101
    _cfg['words'].extend([(0xd3b4e4, 0x1234), (0xd3b4e0, 0x5678)])
    _cfg.update(controlled=[0x45bb50], results={0x45bb50: [3]})
    _case(_fn, 'laborer-links-delivery-leader', _cfg, True)

_cfg = _member([BUILDING, ACTOR, 1], query=0x40ec20, kind=85)
_cfg['stores'].append((RECEIVER + 88, 25))
_case(0x40ed70, 'receiver-state-above24-reject', _cfg)
_cfg = _member([BUILDING, ACTOR, 1], query=0x40ec20, kind=85)
_cfg['stores'].append((_building(0x799a30), 0))
_destination(_cfg, BUILDING)
_case(0x40ed70, 'initial-laborer-claims-root', _cfg)
_cfg = _member([BUILDING, ACTOR, 1], query=0x40ec20, kind=81)
_cfg['stores'].append((_building(0x799a30), 3))
_destination(_cfg, BUILDING)
_cfg.update(controlled=[0x420910], results={0x420910: [BUILDING]})
_case(0x40ed70, 'ordinary-worker-claims-phase3-root', _cfg, True)
for _value in [0x12345600, 0x89abcd01]:
    _cfg = _member([BUILDING, ACTOR, 0xfedcba01])
    _cfg.update(controlled=[0x401d40], results={0x401d40: [_value]})
    _case(0x401eb0, 'forces-mode-zero-low-al-%02x' % (_value & 255), _cfg, True)


def _route_cfg(arguments, classification=0xee0c50, default=127):
    return dict(arguments=list(arguments), stores=[(0xbca2d8, 1000),
                 (0xeadfd4, 0xfffffffe)],
                regions=[(classification, bytes([default]) * 51984)])


def _open(cfg, classification, cells, value=0):
    # setup writes regions after scalar BYTE stores: edit the initialization
    # region itself, so the bounded open cells survive final initialization.
    for i, (at, raw) in enumerate(cfg.get('regions', [])):
        if at == classification:
            raw = bytearray(raw)
            for cell in cells:
                raw[cell] = value
            cfg['regions'][i] = (at, bytes(raw))
            return
    cfg.setdefault('bytes', []).extend((classification + c, value) for c in cells)


# Service exits: initial snapshots, WORD building classification, then bounded
# cost pointers supplied by the external channel query.
_flag_bases = [0xaf55fc, 0xaf5990, 0xaf5d1c, 0xaf5988]
_cfg = dict(arguments=[CELL, ACTOR], kind=81,
            stores=[(b + CELL * 4, 0x440 if i < 2 else 0)
                    for i, b in enumerate(_flag_bases)])
_case(0x47e340, 'two-exits-no-cost-query', _cfg, group='route')
_cfg = dict(arguments=[CELL, ACTOR], kind=81,
            stores=[(b + CELL * 4, 0x448 if i == 0 else (0x440 if i == 1 else 0))
                    for i, b in enumerate(_flag_bases)],
            words=[(0xb4e534 + CELL * 2, BUILDING), (_building(0x7999e8), 0xab)])
_case(0x47e340, 'blocked-building-kind-prunes-road', _cfg, group='route')
_cfg = dict(arguments=[CELL, ACTOR], kind=81,
            stores=[(b + CELL * 4, 0x440) for b in _flag_bases] +
                   [(S + 0x5000 + i * 4, cost) for i, cost in enumerate([4, 7, 4, 5])],
            controlled=[0x47e230],
            results={0x47e230: [4, 7, 4, 5]})
_case(0x47e340, 'four-exits-equal-minimum-cost-survive', _cfg, True, 'route')

_cfg = _route_cfg([10, 11], 0xed4140, 255)
_case(0x51d160, 'blocked-start', _cfg, group='route')
_open(_cfg, 0xed4140, [CELL])
_case(0x51d160, 'enclosed-single-cell', _cfg, group='route')
_open(_cfg, 0xed4140, [CELL - 228])
_case(0x51d160, 'two-cell-eight-neighbor-flood', _cfg, group='route')

_cfg = _route_cfg([10, 11, 10, 11])
_case(0x51d420, 'start-is-target', _cfg, group='route')
_cfg = _route_cfg([10, 11, 11, 11])
_cfg.update(controlled=[0x51aff0], results={0x51aff0: [0]})
_case(0x51d420, 'all-occupancy-neighbors-rejected', _cfg, True, 'route')
_cfg = _route_cfg([10, 11, 11, 11])
_open(_cfg, 0xee0c50, [CELL + 1], 6)
_case(0x51d420, 'classification-six-neighbor-reached', _cfg, group='route')

_cfg = _route_cfg([10, 11, TARGETS, 2])
_cfg['regions'].append((TARGETS, struct.pack('<II', CELL, CELL)))
_case(0x51d6d0, 'duplicate-start-candidates-last-index', _cfg, group='route')
_cfg = _route_cfg([10, 11, TARGETS, 0])
_case(0x51d6d0, 'zero-candidates-enclosed-search', _cfg, group='route')
_cfg = _route_cfg([10, 11, TARGETS, 1])
_cfg['regions'].append((TARGETS, struct.pack('<I', CELL + 1)))
_open(_cfg, 0xee0c50, [CELL + 1], 6)
_case(0x51d6d0, 'reachable-classification-six-candidate', _cfg, group='route')

for _fn in [0x51d980, 0x51dc10]:
    for _cost in [0, 1]:
        _cfg = dict(arguments=[1, 10, 10, 10, 11, 0],
                    stores=[(0xbca2d8, 1000), (DIST + CELL * 4, _cost)])
        _case(_fn, 'start-cost-%d' % _cost, _cfg, group='route')
    _cfg = dict(arguments=[1, 10, 10, 10, 11, 0], stores=[
        (0xbca2d8, 1000), (0xd3b374, 2), (DIST + CELL * 4, 2),
        (DIST + (CELL - 228) * 4, 1)],
        regions=[(DIST, bytes(207936))])
    # fixture_setup writes regions last; put the two live costs after the zero
    # region inside its bytes, preserving the real pinned neighbor offset table.
    _raw = bytearray(207936)
    struct.pack_into('<I', _raw, CELL * 4, 2)
    struct.pack_into('<I', _raw, (CELL - 228) * 4, 1)
    _cfg['regions'] = [(DIST, bytes(_raw))]
    _case(_fn, 'one-hop-north-route-copy', _cfg, group='route')

for _fn, _classification in [(0x518dd0, 0xee0c50), (0x519e30, 0xee0c50),
                              (0x51a220, 0xebab20), (0x51b220, 0xec7630)]:
    _cfg = _route_cfg([10, 11, TARGETS, 2], _classification)
    _cfg['regions'].append((TARGETS, struct.pack('<II', CELL, CELL)))
    _case(_fn, 'duplicate-start-target-last-index', _cfg, group='route')
    _cfg = _route_cfg([10, 11, TARGETS, 1], _classification)
    _cfg['regions'].append((TARGETS, struct.pack('<I', CELL - 228)))
    _open(_cfg, _classification, [CELL - 228], 7 if _fn == 0x51b220 else 2)
    # Destructible search accepts every nonnegative class, so its enclosure
    # requires signed-negative cells rather than the other searchers'127.
    if _fn == 0x51b220:
        _cfg['regions'][0] = (_classification, bytes([255]) * 51984)
        _open(_cfg, _classification, [CELL - 228], 7)
    _case(_fn, 'one-hop-north-target', _cfg, group='route')

_cfg = _route_cfg([10, 11, TARGETS, 0])
_case(0x5190a0, 'zero-target-count', _cfg, group='route')
_cfg = _route_cfg([10, 11, TARGETS, 2])
_cfg['regions'].append((TARGETS, struct.pack('<II', CELL - 228, CELL - 228)))
_open(_cfg, 0xee0c50, [CELL - 228])
_case(0x5190a0, 'duplicate-north-marker-distance-written', _cfg, group='route')
_cfg = _route_cfg([10, 11, TARGETS, 1])
_cfg['regions'].append((TARGETS, struct.pack('<I', CELL - 1)))
_open(_cfg, 0xee0c50, [CELL - 1])
_case(0x5190a0, 'west-marker-return-without-distance-write', _cfg, group='route')


def cases():
    return deepcopy(CASES)
