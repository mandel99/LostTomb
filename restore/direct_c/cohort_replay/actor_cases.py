"""Finite cohort2 fixtures for ledger entries0..26; no executable imports.

Native remains the reference. These inputs do not claim complete instruction,
branch, ABI, or gameplay coverage. Member-receiver phases of4BC140/2E0/3B0
remain outside this first finite batch (the scan cases exercise rejection).
"""

GROUP = 'actor'
A = 0xBCD5F8 + 388
L = 0xBCD5F8 + 2 * 388
TILE = 1000 + 11 * 228 + 10
OUT = 0x32002000


def spec(name, arity, result='full32', aliases=(), native_cleanup=0):
    return dict(name=name, arity=arity, result=result, native_cleanup=native_cleanup,
                compiled_cleanup=0, aliases=list(aliases), private_output_args=[])


CHILDREN = {
    0x458C30: spec('FUN_00458c30', 2),
    0x469A50: spec('restore_fireman_find_nearest_fire', 2, aliases=['FUN_00469a50']),
    0x47FE60: spec('FUN_0047fe60', 4),
    0x47D350: spec('FUN_0047d350', 0),
    0x48E5E0: spec('restore_fireman_try_assign_fire', 1, aliases=['FUN_0048e5e0']),
    0x48AE50: spec('restore_actor_find_remaining_delivery_destination', 2,
                   aliases=['FUN_0048ae50']),
    0x48AEF0: spec('restore_actor_apply_cart_delivery', 3, 'void', ['FUN_0048aef0']),
    0x48B4A0: spec('restore_actor_select_cargo_image', 1, 'void', ['FUN_0048b4a0']),
    0x48B320: spec('FUN_0048b320', 1, aliases=['restore_actor_resource_cargo_image_group']),
    0x488030: spec('FUN_00488030', 1, aliases=['restore_building_is_processing_type'],
                   native_cleanup=4),
    0x4894A0: spec('FUN_004894a0', 1, aliases=['restore_building_has_active_occupant'],
                   native_cleanup=4),
    0x493610: spec('restore_actor_update_service_state', 0, 'void', ['FUN_00493610']),
    0x4A5DA0: spec('FUN_004a5da0', 2, 'void'),
    0x4B17D0: spec('FUN_004b17d0', 1, 'void'),
    0x4B1D50: spec('FUN_004b1d50', 1, 'void'),
    0x4B1DF0: spec('restore_actor_prepare_service_patrol', 1, aliases=['FUN_004b1df0']),
    0x4B1EF0: spec('restore_actor_advance_service_patrol', 2, 'void', ['FUN_004b1ef0']),
    0x4B3250: spec('FUN_004b3250', 1, 'void'),
    0x4B3FF0: spec('restore_actor_advance_route_pixels', 2, aliases=['FUN_004b3ff0']),
    0x4B4220: spec('FUN_004b4220', 6, 'void'),
    0x4B4180: spec('restore_actor_update_route_error', 1, 'void', ['FUN_004b4180']),
    0x4B43A0: spec('restore_actor_step_route_pixel_y', 1, 'void', ['FUN_004b43a0']),
    0x4B43E0: spec('restore_actor_step_route_pixel_x', 1, 'void', ['FUN_004b43e0']),
    0x4B5420: spec('restore_actor_unlink_from_grid', 1, aliases=['FUN_004b5420']),
    0x4B52F0: spec('restore_actor_link_to_grid', 1, aliases=['FUN_004b52f0']),
    0x4BAA10: spec('FUN_004baa10', 1),
    0x47E340: spec('FUN_0047e340', 2),
    0x4B3F00: spec('FUN_004b3f00', 1, 'void'),
    0x4B3C60: spec('FUN_004b3c60', 1, 'void'),
    0x4450C0: spec('FUN_004450c0', 5),
    0x45CF50: spec('FUN_0045cf50', 4, aliases=['restore_storage_can_export']),
    0x45D180: spec('FUN_0045d180', 4, aliases=['restore_storage_can_import']),
    0x45F7B0: spec('FUN_0045f7b0', 4, aliases=['restore_industry_city_destination']),
    0x45EAB0: spec('FUN_0045eab0', 3, aliases=['restore_industry_delivery_destination']),
    0x45ACF0: spec('FUN_0045acf0', 4, aliases=['restore_storage_delivery_destination']),
    0x45D9E0: spec('FUN_0045d9e0', 4, aliases=['restore_food_delivery_destination']),
    0x45E360: spec('FUN_0045e360', 4, aliases=['restore_granary_withdraw']),
    0x45E220: spec('FUN_0045e220', 4, aliases=['restore_granary_deliver']),
    0x51CBA0: spec('FUN_0051cba0', 4),
    0x4F01E0: spec('FUN_004f01e0', 3),
    0x420350: spec('FUN_00420350', 3),
    0x4E5C00: spec('FUN_004e5c00', 3),
    0x4F1D10: spec('restore_route_advance_travel', 2, aliases=['FUN_004f1d10']),
}


def b(building):
    return 0x7999D8 + building * 264


def cfg(arguments=(), controlled=(), **values):
    value = dict(arguments=list(arguments), controlled=list(controlled),
                 actor=1, kind=0, state=8, stores=[
                     (0xAF598C + TILE * 4, 0), (0x78D804, 0),
                     (0xD3B4E4, 17), (0xD3B4E0, 19),
                     (0xF55D0C, 12000), (0xF55D14, 13000),
                     (0xF55D1C, 14000), (0xF55D24, 15000),
                     (0xF55D2C, 16000), (0xF55D4C, 17000),
                     (0xF55D54, 18000), (0xF5635C, 19000),
                     (0xF56364, 20000), (0xF56E5C, 21000),
                     (0xF56E64, 22000), (0xF55E44, 23000),
                     (0xF55E4C, 24000), (0xF5603C, 25000),
                     (0xF55F54, 26000)],
                 words=[(A + 0x5A, 1)],
                 bytes=[(0xADC36C + TILE, 0), (0xA76AEC + TILE, 3),
                        (0xEE0C50 + TILE, 0)])
    for key, item in values.items():
        if key in ('stores', 'words', 'bytes'):
            value[key].extend(item)
        else:
            value[key] = item
    return value


CASES = []


def emit(fn, label, configuration, origin='protocol'):
    CASES.append(dict(id='actor/' + label, fn=fn, origin=origin, cfg=configuration))


# Nearest fire: sentinel reset, unassigned priority, reserved near-distance limit.
emit(0x469A50, 'fire-nearest-empty', cfg([10, 11], stores=[(0x78D7FC, 0)]))
emit(0x469A50, 'fire-nearest-unassigned-priority', cfg([10, 11],
    stores=[(0x78D7FC, 2)], words=[(0x78DD90, 2), (0x78DD92, 3),
        (b(2)+0x10, 99), (b(2)+0x20, 1), (b(2)+0x30, 7),
        (b(2)+6, 10), (b(2)+8, 11), (b(3)+0x10, 99), (b(3)+0x20, 1),
        (b(3)+6, 13), (b(3)+8, 15)], bytes=[(b(2), 1), (b(3), 1)]))
emit(0x469A50, 'fire-nearest-reserved-distance2', cfg([10, 11],
    stores=[(0x78D7FC, 1)], words=[(0x78DD90, 2), (b(2)+0x10, 99),
        (b(2)+0x20, 1), (b(2)+0x30, 9), (b(2)+6, 12), (b(2)+8, 10)],
    bytes=[(b(2), 1)]))

# Vagrant: waiting, the51-tick query boundary, and ordinary death rendering.
emit(0x48A720, 'vagrant-wait', cfg(kind=3, delay=0))
emit(0x48A720, 'vagrant-no-destination', cfg(controlled=[0x458C30], kind=3,
    delay=51, results={0x458C30: [0]}, words=[(0xEA3FF0+0x49D8, 77),
                                                        (0xEA3FF2+0x49D8, 88)]))
emit(0x48A720, 'vagrant-death-half-age', cfg(controlled=[0x4B1D50],
    kind=3, state=4, delay=67, rotation=5, direction=1))

# Ghost: valid home, invalid home/frame wrap, and death-base return.
emit(0x48BFF0, 'ghost-valid-home', cfg(controlled=[0x493610], kind=5,
    words=[(b(1)+0x2C, 1)], bytes=[(b(1), 1)]))
emit(0x48BFF0, 'ghost-invalid-home-frame-wrap', cfg(controlled=[0x493610],
    kind=5, animation=11, rotation=6, direction=1))
emit(0x48BFF0, 'ghost-death', cfg(controlled=[0x493610], kind=5,
    state=4, delay=24, words=[(b(1)+0x2C, 1)], bytes=[(b(1), 1)]))

# Explosion: normal lifetime, alternate lifetime, signed BYTE wrap.
for label, age, alternate in [('normal-lifetime', 44, 0),
                              ('alternate-lifetime', 39, 1), ('signed-wrap', 127, 0)]:
    emit(0x48C200, 'explosion-' + label, cfg(controlled=[0x4B3FF0], kind=6,
        stores=[(A+0x15C, alternate), (0xF55BB4+0x246*8, 30000),
                (0xF55BB4+0x24D*8, 31000)],
        bytes=[(A+0x39, age), (A+0x59, 2)], results={0x4B3FF0: [0x87654321]}))

# Engineer: valid idle, road-search failure, arrival -> service preparation.
emit(0x48C6A0, 'engineer-wait-valid-home', cfg(kind=8, delay=2,
    words=[(b(1)+0x2A, 1)], bytes=[(b(1), 1)]))
emit(0x48C6A0, 'engineer-road-failure', cfg(controlled=[0x47FE60], kind=8,
    delay=1, words=[(b(1)+0x2A, 1)], bytes=[(b(1), 1)], results={0x47FE60: [0]}))
emit(0x48C6A0, 'engineer-arrived-service', cfg(controlled=[0x4B3FF0, 0x4B1DF0],
    kind=8, state=9, words=[(b(1)+0x2A, 1), (0xB4E6FC+TILE*2, 2)],
    bytes=[(b(1), 1)], results={0x4B3FF0: [1], 0x4B1DF0: [0x76540011]}))

# Fireman: expired idle search, route arrival, and the active extinguish arm.
emit(0x48DED0, 'fireman-idle-road-failure', cfg(controlled=[0x48E5E0, 0x47FE60],
    kind=10, delay=1, words=[(b(1)+0x2A, 1)], bytes=[(b(1), 1)],
    results={0x48E5E0: [0], 0x47FE60: [0]}))
emit(0x48DED0, 'fireman-arrive-at-fire', cfg(controlled=[0x48E5E0, 0x4A5DA0, 0x4B3250],
    kind=10, state=12, direction=8, words=[(b(1)+0x2A, 1)], bytes=[(b(1), 1)],
    results={0x48E5E0: [0]}))
emit(0x48DED0, 'fireman-extinguish-direction-clamp', cfg(
    controlled=[0x48E5E0, 0x47D350, 0x51CBA0], kind=10, state=13, delay=2,
    words=[(A+0x74, 1), (b(1)+0x2A, 1), (b(2)+0x10, 99),
           (b(2)+6, 10), (b(2)+8, 11)], bytes=[(b(1), 1), (b(2), 1)],
    results={0x48E5E0: [0], 0x47D350: [0x87654321], 0x51CBA0: [0x12345609]}))

# Assignment: global gate, timer just before threshold, accepted distance25.
emit(0x48E5E0, 'fire-assign-global-gate', cfg([1], state=10))
emit(0x48E5E0, 'fire-assign-timer19', cfg([1], state=10,
    stores=[(0x78D804, 1)], bytes=[(A+0x6B, 18)]))
emit(0x48E5E0, 'fire-assign-distance25', cfg([1], [0x469A50, 0x4B3250], state=10,
    stores=[(0x78D804, 1), (0x78D7F8, 25)],
    words=[(b(2)+0x26, 33), (b(2)+0x28, 44)], bytes=[(A+0x6B, 19)],
    results={0x469A50: [2]}))

# Caravan: wait boundary, absent destination, failed buy/sell then reroute.
emit(0x491420, 'caravan-wait20', cfg(kind=19, delay=19))
emit(0x491420, 'caravan-exit-on-query-failure', cfg(controlled=[0x4450C0], kind=19,
    delay=20, results={0x4450C0: [0]}, words=[(0xEA3FF0+0x49D8, 70),
                                                         (0xEA3FF2+0x49D8, 71)]))
emit(0x491420, 'caravan-failed-trades-reroute', cfg(controlled=[0x45CF50, 0x45D180, 0x4450C0],
    kind=19, state=10, delay=10, words=[(A+0x5E, 2)],
    bytes=[(A+0x6E, 1), (A+0x0F, 1)],
    results={0x45CF50: [0], 0x45D180: [0], 0x4450C0: [3]}))

# Pixel caller protocol: leaves are controlled independently of their own tests.
emit(0x4B3FF0, 'pixels-zero-steps', cfg([1, 0], [0x4B5420, 0x4B52F0],
    words=[(A+0x4A, 150), (A+0x4C, 165)],
    results={0x4B5420: [0x11223344], 0x4B52F0: [0x55667788]}))
emit(0x4B3FF0, 'pixels-arrived-first-step', cfg([1, 1], [0x4B5420, 0x4B52F0],
    words=[(A+0x4A, 150), (A+0x4C, 165), (A+0x52, 0), (A+0x54, 0)],
    results={0x4B5420: [0], 0x4B52F0: [0]}))
emit(0x4B3FF0, 'pixels-axis2-diagonal', cfg([1, 1],
    [0x4B5420, 0x4B4180, 0x4B43A0, 0x4B43E0, 0x4B52F0],
    words=[(A+0x4A, 150), (A+0x4C, 165), (A+0x52, 3),
           (A+0x54, 4), (A+0x56, 0)], bytes=[(A+0x58, 2), (A+0x1C, 2)],
    results={0x4B5420: [0], 0x4B52F0: [0]}))

# Error arithmetic includes negative signed error, positive error, zero wrap.
for label, axis, error, component_x, component_y in [
        ('axis1-negative', 1, 0xFFFF, 5, 2),
        ('axis2-nonnegative', 2, 3, 2, 5), ('axis0-zero-wrap', 0, 0, 0, 0)]:
    emit(0x4B4180, 'error-' + label, cfg([1], words=[
        (A+0x52, component_x), (A+0x54, component_y), (A+0x56, error)],
        bytes=[(A+0x58, axis)]))
for fn, field, label in [(0x4B43A0, 0x4C, 'y'), (0x4B43E0, 0x4A, 'x')]:
    for suffix, current, target in [('increase', 14, 15), ('equal', 15, 15),
                                    ('signed-boundary', 0x7FFF, 0x8000)]:
        emit(fn, 'pixel-' + label + '-' + suffix,
             cfg([1], words=[(A+field, current), (A+field+4, target)]))

# Delivery chain short circuits and final fallback, including repeated child.
emit(0x48AE50, 'remaining-first-destination', cfg([1, 3], [0x45F7B0],
    bytes=[(A+0x0B, 12)], results={0x45F7B0: [2]}))
emit(0x48AE50, 'remaining-second-destination', cfg([1, 3], [0x45F7B0, 0x45EAB0],
    bytes=[(A+0x0B, 12)], results={0x45F7B0: [0], 0x45EAB0: [3]}))
emit(0x48AE50, 'remaining-all-fail', cfg([1, 3],
    [0x45F7B0, 0x45EAB0, 0x45ACF0, 0x45D9E0], bytes=[(A+0x0B, 12)],
    results={0x45F7B0: [0], 0x45EAB0: [0], 0x45ACF0: [0], 0x45D9E0: [0, 0]}))

# Warehouse completion: granary accounting, retire, partial reroute.
emit(0x48CAA0, 'warehouse-delivery-complete', cfg([100, 1, 3, 0],
    words=[(A+0x74, 100), (b(2)+0x10, 71), (b(2)+0x104, 2),
           (b(2)+0x106, 300), (A+0x2C, 7), (A+0x2E, 8)]))
emit(0x48CAA0, 'warehouse-delivery-retire-booking', cfg([100, 1, 3, 1],
    words=[(A+0x74, 100), (b(2)+0x10, 204), (b(2)+0xCE, 7),
           (b(2)+0x106, 2)], bytes=[(A+0x0B, 12)]))
emit(0x48CAA0, 'warehouse-delivery-partial-reroute', cfg([40, 1, 3, 0], [0x48AE50],
    words=[(A+0x74, 100)], results={0x48AE50: [3]}))

# Warehouse callback: idle reset, movement arrival, partial pickup return.
emit(0x48CE30, 'warehouseman-idle-first-tick', cfg(controlled=[0x48B4A0],
    kind=9, delay=0, words=[(b(1)+0x2A, 1)], bytes=[(b(1), 1)]))
emit(0x48CE30, 'warehouseman-route-arrival', cfg(controlled=[0x48B4A0, 0x4A5DA0],
    kind=9, state=9, direction=8))
emit(0x48CE30, 'warehouseman-partial-pickup', cfg(controlled=[0x48B4A0, 0x45E360, 0x4B3250],
    kind=9, state=19, delay=4, words=[(b(1)+0x10, 71)],
    bytes=[(A+0x7A, 12)], results={0x45E360: [125]}))

# Service preparation: lower clamp/failure, upper clamp/high-result bits, normal.
emit(0x4B1DF0, 'patrol-prepare-lower-clamp-failure', cfg([1], [0x47FE60],
    words=[(b(1)+6, 0), (b(1)+8, 0)], bytes=[(b(1)+0x34, 0)],
    results={0x47FE60: [0]}))
emit(0x4B1DF0, 'patrol-prepare-upper-clamp-full-result', cfg([1], [0x47FE60],
    stores=[(0xBCA2D0, 20), (0xBCA2D4, 20)],
    words=[(b(1)+6, 30), (b(1)+8, 31)], bytes=[(b(1)+0x34, 6)],
    results={0x47FE60: [0xFEDC0001]}))
emit(0x4B1DF0, 'patrol-prepare-south-wrap', cfg([1], [0x47FE60],
    words=[(b(1)+6, 10), (b(1)+8, 11)], bytes=[(b(1)+0x34, 4)],
    results={0x47FE60: [1]}))

# Service advance: arrival before zero steps, no roads, ordinary phase8 crossing.
emit(0x4B1EF0, 'patrol-advance-arrival-zero-steps', cfg([1, 0], [0x4A5DA0],
    direction=8, bytes=[(A+0x46, 0)]))
emit(0x4B1EF0, 'patrol-advance-no-roads', cfg([1, 1], [0x4BAA10, 0x47E340],
    words=[(A+0x42, 640)], bytes=[(A+0x46, 1), (A+0x39, 14)],
    results={0x4BAA10: [0], 0x47E340: [0]}))
emit(0x4B1EF0, 'patrol-advance-phase8', cfg([1, 1], [0x4B3F00, 0x4B3C60],
    bytes=[(A+0x46, 1), (A+0x39, 7)]))

# Cargo rendering: empty cart, large ordinary cargo, reverse-resource offset.
emit(0x48B4A0, 'cargo-empty', cfg([1], kind=4, words=[(A+0x74, 0)],
    bytes=[(A+0x0B, 1)]))
emit(0x48B4A0, 'cargo-large', cfg([1], [0x48B320], kind=4,
    stores=[(0xF55BB4+0x75*8, 33000)], words=[(A+0x74, 401)],
    bytes=[(A+0x0B, 1)], results={0x48B320: [0x75]}))
emit(0x48B4A0, 'cargo-reverse-resource', cfg([1], [0x48B320], kind=4,
    stores=[(0xF55BB4+0x80*8, 34000)], words=[(A+0x74, 400)],
    bytes=[(A+0x0B, 12)], results={0x48B320: [0x80]}))

# Cart callback: idle rendering, timed delivery, mode-gated early return.
emit(0x48B620, 'cartpusher-idle-first-tick', cfg(controlled=[0x48B4A0], kind=4,
    delay=0, words=[(b(1)+0x2A, 1)], bytes=[(b(1), 1)]))
emit(0x48B620, 'cartpusher-granary-delivery', cfg(controlled=[0x48B4A0, 0x45E220, 0x48AEF0],
    kind=4, state=13, delay=5, words=[(A+0x74, 100)], bytes=[(A+0x0B, 12)],
    results={0x45E220: [100]}))
emit(0x48B620, 'cartpusher-mode1-early-return', cfg(controlled=[0x488030, 0x4894A0, 0x4B3250],
    kind=4, state=15, stores=[(0xD386D4, 1)],
    results={0x488030: [0x12345601], 0x4894A0: [0x76543201]}))

# Cart completion uses the same booking tables and preserves original actor.
emit(0x48AEF0, 'cart-delivery-complete-granary', cfg([100, 1, 3],
    words=[(A+0x74, 100), (b(2)+0x10, 71), (b(2)+0x104, 2),
           (b(2)+0x106, 300), (A+0x2C, 7), (A+0x2E, 8)]))
emit(0x48AEF0, 'cart-delivery-partial-no-target', cfg([40, 1, 3], [0x48AE50],
    words=[(A+0x74, 100)], results={0x48AE50: [0]}))
emit(0x48AEF0, 'cart-delivery-partial-kind95', cfg([40, 1, 3], [0x48AE50],
    words=[(A+0x74, 100), (b(3)+0x10, 95)], results={0x48AE50: [3]}))

for label, resource in [('default0', 0), ('ordinary-last26', 26), ('special31', 31)]:
    emit(0x48B320, 'resource-image-' + label, cfg([resource]))

# Custom finite cardinal offsets keep road probes within initialized tile RAM.
OFFSETS = [-10, -9, 2, 3, 10, 9, -2, -3]
ROAD_STORES = [(0x5F80FC+i*4, value & 0xFFFFFFFF) for i, value in enumerate(OFFSETS)]
ROAD_STORES += [(0xAF598C+(TILE+offset)*4, 0) for offset in OFFSETS]
emit(0x4B21D0, 'road-direction-no-roads', cfg([1], [0x51CBA0],
    stores=ROAD_STORES, results={0x51CBA0: [0]}))
emit(0x4B21D0, 'road-direction-clockwise-tie', cfg([1], [0x51CBA0],
    stores=ROAD_STORES+[(0xAF598C+(TILE+OFFSETS[0])*4, 64),
                       (0xAF598C+(TILE+OFFSETS[2])*4, 64)], results={0x51CBA0: [1]}))
emit(0x4B21D0, 'road-direction-clamp-counterclockwise', cfg([1], [0x51CBA0],
    stores=ROAD_STORES+[(0xAF598C+(TILE+OFFSETS[6])*4, 64)],
    results={0x51CBA0: [0x12345609]}))

emit(0x4A34F0, 'linked-kind85-first', cfg([1, OUT], stores=[(OUT, 0xAAAAAAAA)],
    words=[(A+0x68, 2)], bytes=[(L+0x0A, 85)]))
emit(0x4A34F0, 'linked-kind85-third', cfg([1, OUT], stores=[(OUT, 0xAAAAAAAA)],
    words=[(A+0x68, 2), (L+0x68, 3), (0xBCD5F8+3*388+0x68, 4)],
    bytes=[(0xBCD5F8+4*388+0x0A, 85)]))
emit(0x4A34F0, 'linked-kind85-cycle-exhausted', cfg([1, OUT], stores=[(OUT, 0xAAAAAAAA)],
    words=[(A+0x68, 2), (L+0x68, 2)]))

emit(0x4BC140, 'approach-first-success', cfg([10, 11, 1], [0x4F01E0],
    results={0x4F01E0: [0x80000002]}))
emit(0x4BC140, 'approach-second-success', cfg([10, 11, 1], [0x4F01E0, 0x420350],
    results={0x4F01E0: [0], 0x420350: [0xFFFFFFFF]}))
emit(0x4BC140, 'approach-third-success', cfg([10, 11, 1], [0x4F01E0, 0x420350, 0x4E5C00],
    results={0x4F01E0: [0], 0x420350: [0], 0x4E5C00: [2]}))

# Explicit rejection cases; receiver-acceptance phases are not covered here.
for fn, state in [(0x4BC2E0, 2), (0x4BC3B0, 3)]:
    emit(fn, 'building-state%d-empty' % state, cfg([10, 11, 1]))
    emit(fn, 'building-state%d-filter-rejections' % state, cfg([10, 11, 1],
        stores=[(b(1)+0x60, state+1), (b(3999)+0x60, state+1)],
        words=[(b(1)+0x10, 229), (b(2)+0x10, 100), (b(3999)+0x10, 235)],
        bytes=[(b(1), 1), (b(2), 1), (b(3999), 1)]))

# Closely follow: ordinary direction transfer, tile crossing, travel-state copy.
emit(0x4B23C0, 'follow-phase0-direction-copy', cfg([1, 2, 0, 0],
    bytes=[(L+0x39, 0), (L+0x11, 6), (L+1, 7), (A+0x78, 1)]))
emit(0x4B23C0, 'follow-phase8-terrain-height', cfg([1, 2, 0, 0], [0x4B5420, 0x4B52F0],
    stores=[(0xAF598C+TILE*4, 0x44)], words=[(L+0x14, 10), (L+0x16, 11)],
    bytes=[(L+0x39, 8), (L+1, 5), (0xADC36C+TILE, 2)],
    results={0x4B5420: [0], 0x4B52F0: [0]}))
emit(0x4B23C0, 'follow-travel-copy-after-phase8', cfg([1, 2, 3, 5], [0x4F1D10],
    words=[(L+0x120, 1), (L+0x54, 50), (L+0x52, 3),
           (L+0x4A, 4), (L+0x4E, 6), (L+0x4C, 2), (L+0x50, 8)],
    bytes=[(A+0x39, 8), (L+0x11, 2), (L+1, 9)], results={0x4F1D10: [0xFEDCBA00]}))


def endpoints():
    return CHILDREN


def cases():
    return CASES
