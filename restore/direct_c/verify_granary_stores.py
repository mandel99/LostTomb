"""Compare the native/C granary renderer's stock reads and draw protocol.

Controls only the resource enumeration and final sprite endpoint. Cases keep
the native coordinate reads within its eight owned pairs. Larger inherited
caller-frame reads remain a separate world-renderer obligation.
"""
from verify_options_startup import *


def verify():
    cases = 0
    for building, resource, quantity, visible in itertools.product(
            [6, 400], [1, 8], [0, 1, 600, 601, 1000, 1001, 3200, 3201, 65535], [False, True]):
        states = []
        for c, u, state in machines:
            reset(u, state)
            tile = 18000
            byte(u, 0xb67d1c+tile, 64 if visible else 0)
            word(u, 0x799a2e+building*264+resource*2, quantity)
            put(u, 0xf5648c, 13994)
            draws = []
            def resources(uc, args):
                put(uc, 0xf53dd8, 1)
                put(uc, 0xf53ddc, resource)
                return 0
            hook(c, state, 0x5596d0, 0, resources)
            hook(c, state, 0x53e590, 3, lambda uc, args: draws.append(args) or 1,
                 cleanup=0 if c else 12)
            value = run(c, u, 0x442280, [building, tile, 80, 100])
            states.append((value, draws, snapshot(u)))
        assert states[0] == states[1], (building, resource, quantity, visible)
        if visible and quantity == 600:
            assert states[1][1] == [(13994+resource-1, 190, 27)]
        cases += 1
    print('PASS WORD stock, BYTE tile flags, DWORD sprite IDs and pile thresholds:', cases)
    print('Verified target SHA256:', target_sha)


if __name__ == '__main__':
    verify()
