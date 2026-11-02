"""Original locust spawning/action versus typed Release C, offline.

Sound is always an observed boundary. Ordinary cases also control farm
classification, allocation and four movement/route dependencies. Explicit
compositions run the actual classifier, allocator or actor controller.
"""
from verify_disease_updates import *

WATCH.update([0x78d79c, 0xf55b94, 0xd056f8])
WATCH.update(0xd3b1c8+i*4 for i in range(100))


def member(c, state, token, count, action):
    if c: hook(c, state, token, count+1, lambda uc, args: action(uc, args[0], args[1:]))
    else: hook(c, state, token, count,
               lambda uc, args: action(uc, uc.reg_read(UC_X86_REG_ECX), args), cleanup=count*4)


def case(spawn=False, actor=255, action=8, frame=0, timer=1, direction=0, route=1,
         farm=1, farm_state=1, workers=0, laborer=1999, labor_kind=85, labor_life=1,
         period=2632, month=2, flood=5, width=32, height=17, cursor=0,
         low=0x13579bdf, high=0x2468ace0, kind=100, progress=2000, sound=(0, 0),
         allocation=None, real_classifier=False, real_allocation=False,
         prepare=None, entry=None):
    results=[]
    for c, u, state in machines:
        setup_disease(u, state, rows=({'id': farm, 'type': kind, 'population': 0},), ids=(farm,))
        u.mem_write(0xbcd5f8, bytes(2000*388)); u.mem_write(0xb350dc, bytes(228*228*2))
        p=0x7999d8+farm*264; byte(u, p, farm_state); word(u, p+6, 10); word(u, p+8, 11)
        word(u, p+46, 123); word(u, p+32, 456); word(u, p+64, workers)
        byte(u, p+167, 99); word(u, p+180, laborer); word(u, p+84, progress)
        byte(u, a(laborer)+10, labor_kind); byte(u, a(laborer)+14, labor_life)
        p=a(actor); byte(u, p+10, 107); byte(u, p+14, 1); byte(u, p+56, action)
        byte(u, p+1, frame); byte(u, p+17, direction); word(u, p+54, timer)
        word(u, p+58, route); word(u, p+92, farm); put(u, p+384, 0x12345678)
        word(u, p+68, 99); put(u, 0x7090ec+107*56, 17)
        for at, value in [(0xd056f8, actor), (0x78d79c, -1 if spawn else period), (0x78d7c0, month),
                (0xd386d0, flood), (0xbca2d0, width), (0xbca2d4, height),
                (0xd3b1c4, cursor), (0xd3b380, low), (0xd3b384, high),
                (0xf5713c, 20000), (0xf5714c, 21000), (0xf55b94, 0)]: put(u, at, value)
        calls=[]; created=[0]
        for token, nargs in [(0x55f5a1, 1), (0x55f561, 1), (0x55f449, 3)]:
            def audio(uc, self, args, token=token):
                calls.append((token, self, args, get(uc, 0xf55b94)))
                if token==0x55f5a1: return sound[0 if args[0]==0 else 1]
                return 0
            member(c, state, token, nargs, audio)
        if not real_classifier:
            def classifier(uc, args): calls.append((0x488030, args)); return int(args[0]==farm)
            hook(c, state, 0x488030, 1, classifier, cleanup=0 if c else 4)
        if not real_allocation:
            def create(uc, args):
                calls.append((0x4b4800, (args[0]&255, *args[1:]))); created[0]+=1
                return allocation if allocation is not None else created[0]
            hook(c, state, 0x4b4800, 5, create)
        for token, nargs in [(0x4b1d50, 1), (0x4b2d20, 1), (0x4b3250, 1), (0x4a5da0, 2), (0x4b5420, 1)]:
            def observed(uc, args, token=token):
                selected=get(uc, 0xd056f8)
                calls.append((token, args, selected, bytes(uc.mem_read(a(selected), 116))))
                return 0
            hook(c, state, token, nargs, observed)
        if prepare: prepare(c, u, state, calls)
        state['service_writes'].clear()
        token=entry or (0x4e7700 if spawn else 0x4b03b0)
        run(c, u, token, [actor] if token==0x4bf920 else (), cleanup=4 if token==0x4bf920 and not c else 0)
        results.append(((calls, list(state['service_writes'])), snapshot(u)))
    same(results, ('locust', spawn, actor, action, frame, timer, direction, route, farm,
        farm_state, workers, laborer, labor_kind, labor_life, period, month, flood,
        width, height, cursor, low, high, kind, progress, sound, allocation, real_classifier, real_allocation, entry))
    return results[1]


def verify_spawn():
    count=0
    for period in [-2147483648, -1, 0, 1, 2632, 0x7fffffff]:
        def prepare(c, u, state, calls): put(u, 0x78d79c, period)
        case(spawn=True, prepare=prepare); count+=1
    for flood, month in [(1, 10), (5, 2), (13, 10), (5, 3), (0, 9), (0, 0), (256, 2)]:
        case(spawn=True, flood=flood, month=month); count+=1
    for width, cursor in itertools.product([4, 7, 32, 228], [0, 99, 110, 111]):
        result=case(spawn=True, width=width, cursor=cursor)
        assert sum(call[0]==0x4b4800 for call in result[0][0])==9; count+=1
    for allocation in [0, 1, 1999, 0x80000001]: case(spawn=True, allocation=allocation); count+=1
    for workers, labor_kind, labor_life in itertools.product([-1, 0, 1], [85, 107], [1, 2]):
        case(spawn=True, workers=workers, labor_kind=labor_kind, labor_life=labor_life); count+=1
    for sound in itertools.product([0, 1, 256], repeat=2): case(spawn=True, sound=sound); count+=1
    print('PASS monthly/flood gates, nine spawns/fields, distinct RNG publication, history aliases, allocation results, worker cleanup and sound order:', count, flush=True)


def verify_action():
    count=0
    for action, actor in itertools.product([0, 4, 8, 9, 10, 11, 255], [1, 255, 1998]):
        case(action=action, actor=actor); count+=1
    for frame, farm_state in itertools.product([0, 4, 5, 6, 254, 255], [0, 1, 3]):
        case(frame=frame, farm_state=farm_state); count+=1
    for timer, route in itertools.product([-32768, -1, 0, 1, 2, 32767], [-1, 0, 1]):
        case(timer=timer, route=route); count+=1
    for action, direction in itertools.product([9, 11], [0, 7, 8, 9, 10, 11, 255]):
        case(action=action, direction=direction); count+=1
    for period, progress in itertools.product([-2147483648, -1, 0, 999, 1000, 1001, 2632, 0x7fffffff], [-32768, -1, 0, 1632, 32767]):
        case(action=10, period=period, progress=progress); count+=1
    for width, cursor in itertools.product([4, 7, 32, 228], [0, 99, 110, 111]):
        case(action=10, period=1000, width=width, cursor=cursor); count+=1
    print('PASS complete locust action states, signed WORD timers, movement outcomes, fixed entry farm, progress clamp, RNG exit coordinates and sprites:', count, flush=True)


def verify_callbacks():
    count=0
    for action, direction in itertools.product([4, 8, 9, 11], [8, 9, 10]):
        def prepare(c, u, state, calls):
            p=0x7999d8+2*264; byte(u, p, 1); word(u, p+6, 55); word(u, p+8, 66)
            def moved(uc, args, token):
                calls.append((token, args)); put(uc, 0xd056f8, 1)
                p=a(1); byte(uc, p+56, 10); byte(uc, p+1, 5); byte(uc, p+17, direction)
                word(uc, p+58, 1); word(uc, p+92, 2); byte(uc, p+10, 106)
                return 0
            for token, nargs in [(0x4b1d50, 1), (0x4b2d20, 1), (0x4a5da0, 2)]:
                hook(c, state, token, nargs, lambda uc, args, token=token: moved(uc, args, token))
            def clear(uc, args):
                calls.append((0x4b3250, args)); put(uc, 0xd056f8, 1998)
                byte(uc, a(1998)+56, 10); byte(uc, a(1998)+1, 4); return 0
            hook(c, state, 0x4b3250, 1, clear)
        case(action=action, direction=direction, prepare=prepare); count+=1
    for result in [0, 1, 256, 257]:
        def classifier(c, u, state, calls):
            def query(uc, args): calls.append((0x488030, args)); return result
            hook(c, state, 0x488030, 1, query, cleanup=0 if c else 4)
        output=case(spawn=True, prepare=classifier)
        assert sum(call[0]==0x4b4800 for call in output[0][0]) == (9 if result&255 else 0); count+=1
    def creation_changes(c, u, state, calls):
        n=[0]
        def create(uc, args):
            n[0]+=1; k=n[0]; calls.append((0x4b4800, (args[0]&255, *args[1:])))
            put(uc, 0xeaac28, 3 if k&1 else 0); put(uc, 0xbca2d0, 7 if k&1 else 32)
            put(uc, 0xd3b384, k*0x12345); put(uc, 0xd3b380, k*0x54321)
            put(uc, 0xd3b1c4, 110 if k&1 else 99); put(uc, 0xd3b374, 100+k)
            word(uc, 0x7999de+264, k+10); word(uc, 0x7999e0+264, 11-k)
            return 100+k
        hook(c, state, 0x4b4800, 5, create)
    case(spawn=True, prepare=creation_changes); count+=1
    for final_limit in [0, 2]:
        def farm_limit(c, u, state, calls):
            def query(uc, args): calls.append((0x488030, args)); put(uc, 0x78d790, final_limit); return 1
            hook(c, state, 0x488030, 1, query, cleanup=0 if c else 4)
        case(spawn=True, prepare=farm_limit); count+=1
    for first_match in [False, True]:
        def flood_changed(c, u, state, calls):
            put(u, 0x78d790, 0); n=[0]
            def flood(uc, args):
                n[0]+=1; calls.append((0x4becf0, args)); put(uc, 0x78d7c0, 10); put(uc, 0x78d79c, 9)
                return 12 if first_match or n[0]==1 else 0
            # Nonmatching first result12 must use a different live month.
            if not first_match:
                def flood(uc, args):
                    n[0]+=1; calls.append((0x4becf0, args)); put(uc, 0x78d7c0, 10); return 1 if n[0]==1 else 0
            hook(c, state, 0x4becf0, 0, flood)
        result=case(spawn=True, prepare=flood_changed)
        assert sum(call[0]==0x4becf0 for call in result[0][0]) == (1 if first_match else 2); count+=1
    def cleanup_changed(c, u, state, calls):
        def cleanup(uc, args):
            calls.append((0x4b5420, args)); word(uc, 0x7999d8+264+180, 123); put(uc, 0x78d790, 0); return 0
        hook(c, state, 0x4b5420, 1, cleanup)
    case(spawn=True, workers=1, prepare=cleanup_changed); count+=1
    for farm in [-1, 0, 4000]: case(farm=farm, action=9, direction=10); count+=1
    print('PASS callback-selected actors, cached farm/speed, AL classification, live farm count/flood month, constructor-mutated RNG/coordinates and post-cleanup clearing:', count, flush=True)


def verify_integrations():
    count=0
    for kind in [99, 100, 101, 105, 106, 196, 224]:
        case(spawn=True, kind=kind, real_classifier=True, real_allocation=True); count+=1
    for action in [4, 8, 9, 10, 11]:
        def controller(c, u, state, calls):
            byte(u, a(1999)+14, 0)
            assert get(u, 0x5dc28c+107*4)==0x4b03b0
            for token, nargs in [(0x489a50, 1), (0x489a80, 0)]:
                def observed(uc, args, token=token): calls.append((token, args)); return 0
                hook(c, state, token, nargs, observed)
        case(action=action, prepare=controller, entry=0x489b70); count+=1
    def calendar(c, u, state, calls):
        put(u, 0x78d7c4, 15); put(u, 0x78d7c0, 1); put(u, 0x78d7c8, 999)
        byte(u, 0xe38ec2, 0); byte(u, 0x7027b8, 0); byte(u, 0x7027be, 0); byte(u, 0x7027b9, 1)
        for endpoint, nargs in CALENDAR_ENDPOINTS.items():
            if endpoint==0x4e7700: continue
            def observed(uc, args, endpoint=endpoint): calls.append((endpoint, args)); return 0
            hook(c, state, endpoint, nargs, observed)
    result=case(spawn=True, prepare=calendar, entry=0x452370, flood=5)
    assert sum(call[0]==0x4b4800 for call in result[0][0])==9; count+=1
    print('PASS actual classifier/allocation, genuine actor-controller callback and monthly calendar spawning:', count, flush=True)
    print('Verified target SHA256:', target_sha, flush=True)


def verify_farm_arrival():
    count=0
    for period, classification, source in itertools.product([-2147483648, -1, 0, 1, 2632], [0, 1, 256, 257], [-1, 0, 2]):
        def prepare(c, u, state, calls):
            word(u, a(255)+90, source); word(u, a(255)+94, 1)
            word(u, 0x705eb8+100*40, 42)
            def query(uc, args): calls.append((0x488030, args)); return classification
            hook(c, state, 0x488030, 1, query, cleanup=0 if c else 4)
        case(period=period, prepare=prepare, entry=0x4bf920); count+=1
    for counter, destination in itertools.product([0, 254, 255], [1, 4000]):
        def prepare(c, u, state, calls):
            word(u, a(255)+90, destination); word(u, a(255)+94, destination)
            byte(u, 0x799a7e+destination*264, counter); word(u, 0x7999e8+destination*264, 100)
            word(u, 0x705eb8+100*40, -7)
            hook(c, state, 0x488030, 1, lambda uc, args: 1, cleanup=0 if c else 4)
        case(period=0, prepare=prepare, entry=0x4bf920); count+=1
    def mutated(c, u, state, calls):
        word(u, a(255)+90, 2); word(u, a(255)+94, 1)
        def query(uc, args):
            calls.append((0x488030, args)); put(uc, 0x78d79c, 0); put(uc, 0xd056f8, 1)
            word(uc, a(255)+90, 3); word(uc, a(255)+94, 4)
            word(uc, 0x7999e8+264, 101); word(uc, 0x705eb8+101*40, 77); return 1
        hook(c, state, 0x488030, 1, query, cleanup=0 if c else 4)
    case(period=123, prepare=mutated, entry=0x4bf920); count+=1
    print('PASS complete farm arrival, cached signed references, AL gate, live period, BYTE wrap, alias order, model staffing and native RET4:', count, flush=True)
    print('Verified target SHA256:', target_sha, flush=True)


def verify_arrival_caller():
    count=0
    for period in [0, 1]:
        def laborer(c, u, state, calls):
            p=a(255); byte(u, p+10, 85); word(u, p+90, 1); word(u, p+94, 1)
            put(u, 0x7090ec+85*56, 17); word(u, 0x705eb8+100*40, 42)
            # The routing boundary now calls the canonical typed movement
            # provider, already observed by case() on both machines.
        result=case(action=10, direction=8, period=period, prepare=laborer, entry=0x4a98b0)
        memory=result[1]
        assert memory[a(255)+14-0x560000]==2
        assert struct.unpack_from('<H', memory, 0x799a18+264-0x560000)[0] == (42 if period==0 else 0)
        count+=1
    print('PASS actual laborer arrival caller/continuation through typed cdecl provider and native RET4 adapter:', count, flush=True)
    print('Verified target SHA256:', target_sha, flush=True)


if __name__=='__main__':
    verify_spawn(); verify_action(); verify_callbacks(); verify_integrations(); verify_farm_arrival(); verify_arrival_caller()
