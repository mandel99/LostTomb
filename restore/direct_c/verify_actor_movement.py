"""Whole cadence/substep/route-slot/route-builder family, offline native/C.

Observe outgoing service, direction, position and path-search/build providers.
All four new bodies run together unless a named mutation fixture controls its
allocator. Compare mapped state, ordered writes, out bytes and callback ABI.
"""
from verify_disease_updates import *

ENDS={token:1 for token in [0x4baa10, 0x4b33d0, 0x4b3530, 0x4b3c60, 0x4b3f00]}
ENDS.update({token:4 for token in [0x517f20, 0x5181c0, 0x518400, 0x5186d0,
    0x518880, 0x519390, 0x5195e0, 0x519840, 0x519be0, 0x51a030, 0x51b040, 0x51d420]})
ENDS.update({0x51d160:2, 0x51ce90:2, 0x51dc10:6, 0x51d980:6, 0x51a420:6, 0x51b400:6})
WATCH.add(0xbcd5e8)
WATCH.update(0xd04e58+i*2 for i in [0, 1, 2, 999, 1000])
for id in [1, 255, 1999]: WATCH.update(a(id)+off for off in [17, 18, 57, 58, 60, 62, 352])


def case(token=0x4a5da0, actor=255, speed=6, steps=1, phase=0, cadence=0,
         direction=0, route=1, terrain=0, water=0, kind=4, action=9, saved=0,
         query=1, built=3, prepare=None, out=S):
    results=[]
    for c, u, state in machines:
        setup_disease(u, state, rows=(), ids=(0,))
        u.mem_write(0xbcd5f8, bytes(2000*388)); u.mem_write(0xd04e58, bytes(1000*2))
        p=a(actor)
        for off, value in [(10, kind), (14, 1), (17, direction), (18, 7), (56, action),
                           (57, phase), (114, terrain), (118, water), (352, cadence)]: byte(u, p+off, value)
        for off, value in [(20, 3), (22, 4), (36, 10), (38, 11), (58, route), (60, 0x7fff), (62, 99)]: word(u, p+off, value)
        put(u, p+348, saved); put(u, p+320, 77); put(u, 0xd056f8, actor)
        put(u, 0x605a3c, 0); word(u, 0x784c8c, -3); word(u, 0x784c8e, 31)
        put(u, 0xbcd5e8, 0xa5a55a5a); u.mem_write(S, b'\xa5'*16)
        calls=[]
        for endpoint, nargs in ENDS.items():
            def observed(uc, args, endpoint=endpoint):
                calls.append((endpoint, args, get(uc, 0xd056f8), bytes(uc.mem_read(p+17, 48))))
                if endpoint in [0x51dc10, 0x51d980, 0x51b400]: return built
                return query if ENDS[endpoint]>=4 else 0
            hook(c, state, endpoint, nargs, observed)
        if prepare: prepare(c, u, state, calls)
        args=[actor, speed] if token==0x4a5da0 else [actor, steps] if token==0x4b27e0 else [actor, out] if token==0x489970 else [actor, 1] if token==0x4b3470 else [actor]
        state['service_writes'].clear(); value=run(c, u, token, args)
        value=value&255 if token==0x489970 else value if token==0x4b3470 else None
        results.append(((value, calls, list(state['service_writes']), bytes(u.mem_read(S, 16))), snapshot(u)))
    same(results, ('movement', hex(token), actor, speed, steps, phase, cadence,
                   direction, route, terrain, water, kind, action, saved, query, built, out))
    return results[1]


def read(data, address): return struct.unpack_from('<I', data, address-0x560000)[0]


def verify_cadence():
    count=0
    for speed, cadence in itertools.product([*range(18), 18, 0xffffffff], [0, 1, 2, 3, 4, 255]):
        case(speed=speed, cadence=cadence); count+=1
    for speed in range(18):
        for cadence in [0, 3]:
            def mutate(c, u, state, calls):
                def position(uc, args):
                    calls.append((0x4b3f00, args, b(uc, a(255)+352)))
                    byte(uc, a(255)+352, 255); put(uc, 0xd056f8, 1); return 0
                hook(c, state, 0x4b3f00, 1, position)
            case(speed=speed, cadence=cadence, prepare=mutate); count+=1
    print('PASS all18 cadence entries/default, BYTE phases and reset/set/increment order around actual substeps:', count, flush=True)


def verify_substeps():
    count=0
    for phase, steps in itertools.product([0, 6, 7, 8, 13, 14, 15, 126, 127, 128, 254, 255], [-1, 0, 1, 3]):
        result=case(0x4b27e0, phase=phase, steps=steps)
        assert read(result[1], 0xbcd5e8)==0; count+=1
    for direction, route in itertools.product([0, 7, 8, 9, 127, 128, 255], [-1, 0, 1]):
        case(0x4b27e0, phase=14, steps=3, direction=direction, route=route); count+=1
    for endpoint in [0x4baa10, 0x4b33d0, 0x4b3530, 0x4b3c60, 0x4b3f00]:
        def prepare(c, u, state, calls):
            def changed(uc, args):
                calls.append((endpoint, args)); put(uc, 0xd056f8, 1)
                word(uc, a(255)+58, 0); byte(uc, a(255)+57, 8); byte(uc, a(255)+17, 255); return 0
            hook(c, state, endpoint, 1, changed)
        case(0x4b27e0, phase=14, steps=3, prepare=prepare); count+=1
    print('PASS cached actor, signed progress/direction, cell and midpoint order, callback mutations, WORD wrap and unconditional global reset:', count, flush=True)


def verify_slots():
    count=0
    for old_slot, owner in itertools.product([-1, 0, 1, 999], [-1, 0, 255, 2000]):
        def prepare(c, u, state, calls): word(u, 0xd04e58+old_slot*2, owner)
        case(0x489970, route=old_slot, prepare=prepare); count+=1
    for owner, backlink, life in itertools.product([-1, 0, 1, 1999, 2000, 32767], [0, 1], [0, 1, 2]):
        def prepare(c, u, state, calls):
            word(u, 0xd04e58+2, owner)
            if 0<owner<2000: word(u, a(owner)+58, backlink); byte(u, a(owner)+14, life)
        case(0x489970, route=0, prepare=prepare); count+=1
    def full(c, u, state, calls):
        for slot in range(1,1000):
            word(u, 0xd04e58+slot*2, slot); word(u, a(slot)+58, slot); byte(u, a(slot)+14, 1)
    result=case(0x489970, actor=1999, route=0, prepare=full)
    assert result[0][0]==0 and struct.unpack_from('<I', result[0][3])[0]==0; count+=1
    for out in [a(255)+58, 0xd04e58+2]:
        def prepare(c, u, state, calls): word(u, 0xd04e58+2, 255)
        case(0x489970, route=1, out=out, prepare=prepare); count+=1
    print('PASS route-slot ownership/reuse/stale backlinks, signed owners, live/aliased out slot and complete999-slot exhaustion:', count, flush=True)


def verify_route_building():
    count=0
    for terrain, query, built in itertools.product([*range(13), 255], [0, 1], [0, 3, 0xffffffff]):
        case(0x4b2d20, terrain=terrain, query=query, built=built); count+=1
    for water, editor, kind, state in itertools.product([1, 2, 3], [0, 1, 256], [4, 78], [0, 9]):
        def prepare(c, u, model, calls): put(u, 0x605a3c, editor)
        case(0x4b2d20, water=water, kind=kind, action=state, prepare=prepare); count+=1
    for first, second in itertools.product([0, 1, -1, 0x7fffffff], [0, 2, -1]):
        def prepare(c, u, state, calls):
            def build(uc, args): calls.append((0x51b400, args)); return first if args[0]==4 else second
            hook(c, state, 0x51b400, 6, build)
        case(0x4b2d20, terrain=4, prepare=prepare); count+=1
    for answer in [0, 1, 256, 257]:
        def prepare(c, u, state, calls):
            def allocator(uc, args): calls.append((0x489970, args[0], get(uc, args[1]))); return answer
            hook(c, state, 0x489970, 2, allocator)
        case(0x4b2d20, prepare=prepare); count+=1
    print('PASS all terrain/water branches, editor/full-result gates, fresh coordinates,4-to8 construction fallback, signed success tests and allocator argument prestate:', count, flush=True)


if __name__=='__main__':
    verify_cadence(); verify_substeps(); verify_slots(); verify_route_building()
    print('Verified target SHA256:', target_sha, flush=True)
