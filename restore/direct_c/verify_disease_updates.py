"""Disease, its RNG and canonical population providers against original x86.

Offline state/ordered-write/callback comparisons. Real cohort removal runs
normally. Controlled fixtures name their population, tutorial, actor/patrol or
message boundary. No game process, file save or installed asset is changed.
"""
from verify_building_service_effects import *

FLAGS = [0x70e138, 0x70e134, 0x70e130]
SOURCES = [0x70e12c, 0x70e128, 0x70e124]
WATCH.update({*FLAGS, *SOURCES, 0xd3b1c4, 0xd3b384, 0xd3b378, 0xd3b380,
              0xd3b364, 0xd3b36c, 0xd3b374, 0xd3b368, 0xd3b370})


def setup_disease(u, state, player=0, health=60, month=3, rows=None, ids=None,
                  pos=(10, 10), **kw):
    if rows is None:
        rows = ({'id': 1, 'population': 100}, {'id': 2, 'population': 50},
                {'id': 3, 'population': -1}, {'id': 4, 'population': 0})
    if ids is None: ids = (1, 1, 2, 3, 4, 0)
    prepare_effect(u, state, rows=rows, ids=ids, pos=pos, **kw)
    put(u, 0xeaac28, player); put(u, 0x78d7c0, month)
    for owner in [0, 3]:
        off=owner*0x49d8
        u.mem_write(0xea3e34+off, struct.pack('<h', 100)*100)
        put(u, 0xea1894+off, 10000); put(u, 0xea5f0c+off, 9999)
        put(u, 0xea1888+off, health); put(u, 0xea61f0+off, 10)
    for i, row in enumerate(rows, 1):
        id=row.get('id', i); p=0x7999d8+id*264
        word(u, p+16, row.get('type', 13)); word(u, p+6, pos[0]); word(u, p+8, pos[1])
        put(u, p+12, 2300+id); byte(u, p+135, row.get('infected', 0))
        for at, value in [(232, 70), (236, row.get('timer', 2)), (240, 99), (252, 80)]: put(u, p+at, value)
    put(u, 0xe38ebc, 1); byte(u, 0xe38ea8, 0); put(u, 0x579558, 0); byte(u, 0x7027be, 0)
    for at in [*FLAGS, *SOURCES]: put(u, at, 0)
    for i in range(100): put(u, 0xd3b1c8+i*4, [0, 9, 10, 50, 51, 63][i % 6])
    put(u, 0xd3b1c4, 0); put(u, 0xd3b380, 0x13579bdf); put(u, 0xd3b384, 0x2468ace0)
    put(u, 0xd3b374, 0x37)


def run_case(token, args=(), prepare=None, al_only=False, **kw):
    results=[]
    for c, u, state in machines:
        setup_disease(u, state, **kw); calls=[]
        # Tutorial is the default declared boundary, except a fixture may
        # replace it with a callback that changes cursor/settings state.
        def tutorial(uc, a): calls.append((0x417ae0, a)); return 0
        hook(c, state, 0x417ae0, 1, tutorial)
        if prepare: prepare(c, u, state, calls)
        state['service_writes'].clear()
        value=run(c, u, token, args)
        if al_only: value &= 255
        elif token not in [0x42fbb0, 0x430060, 0x42fca0, 0x4baa10]: value=None
        results.append(((value, calls, list(state['service_writes'])), snapshot(u)))
    same(results, ('disease', hex(token), args, kw))
    return results[1]


def verify_population():
    count=0
    for owner, amount, cursor in itertools.product([0, 3], [0, 1, 2, 7, 100], [0, 99]):
        def prepare(c, u, state, calls): put(u, 0xd3b1c4, cursor)
        run_case(0x4e9400, [owner, amount], prepare); count+=1
    for owner, amount in itertools.product([0, 3], [-120, -1, 0, 1, 77]):
        run_case(0x4e90f0, [owner, amount]); count+=1
    def remove(c, u, state, calls):
        def altered(uc, a):
            calls.append((0x4e9350, a)); put(uc, 0xeaac28, 3)
            word(uc, 0xea3e34, -32768); word(uc, 0xea3e34+2, 32767); return 0
        hook(c, state, 0x4e9350, 2, altered)
    run_case(0x4e90f0, [0, -2147483648], remove); count+=1
    print('PASS canonical population add/delta, real removal, cohort/history wrap and cached owner:', count, flush=True)


def verify_rng():
    count=0; high_returns=0
    for low, high, health in itertools.product([0, 1, 0x13579bdf, 0xffffffff],
            [0, 16, 0x2468ace0, 0xffffffff], [-2147483648, 0, 40, 0x7fffffff]):
        def prepare(c, u, state, calls):
            put(u, 0xd3b380, low); put(u, 0xd3b384, high)
        result=run_case(0x42fbb0, prepare=prepare, health=health)
        high_returns += bool(result[0][0] & 0xffffff00); count+=1
    for cursor in [-1, 0, 99, 110, 111, 0x40000000]:
        def prepare(c, u, state, calls): put(u, 0xd3b1c4, cursor)
        run_case(0x42fbb0, prepare=prepare, player=3); count+=1
    def upper_byte(c, u, state, calls): put(u, 0xd3b384, 0x6000)
    result=run_case(0x42fbb0, prepare=upper_byte, health=0)
    assert result[0][0] == 0x101
    high_returns += bool(result[0][0] & 0xffffff00); count+=1
    assert high_returns, 'Fixtures must exercise native quotient high bytes'
    print('PASS complete31-round disease RNG, history-before-seed aliases, signed health and full EAX:', count, flush=True)


def verify_infection():
    count=0
    for id, people, mode in itertools.product([-1, 0, 1, 4000], [-1, 0, 10], [0, 256]):
        def prepare(c, u, state, calls): put(u, 0xe38ebc, mode)
        run_case(0x42fb30, [id], prepare,
            rows=({'id': id, 'population': people},), ids=(id,)); count+=1
    for mission, reference, gate in [(-1, -1, 0), (128, -128, 0), (128, 128, 0), (0, 0, 1)]:
        def prepare(c, u, state, calls):
            put(u, 0xe38ebc, 0); byte(u, 0xe38ea8, mission); put(u, 0x579558, reference); byte(u, 0x7027be, gate)
        run_case(0x42fb30, [1], prepare); count+=1
    def changed(c, u, state, calls):
        def population(uc, a):
            calls.append((0x4e90f0, a)); put(uc, 0xeaac28, 3); put(uc, 0xe38ebc, 0); return 0
        hook(c, state, 0x4e90f0, 2, population)
    run_case(0x42fb30, [1], changed); count+=1
    print('PASS actual infection/cohorts, signed population, post-adjustment mode and tutorial BYTE gate:', count, flush=True)


def verify_spread_and_latency():
    count=0
    for pos, dims in itertools.product([(10, 10), (0, 0), (31, 31), (40, 10), (10, 40), (-3, 10), (10, -3)], [(1, 1), (32, 32)]):
        run_case(0x42ff20, pos, pos=pos, dims=dims); count+=1
    for kind, status, population, flag in itertools.product([9, 10, 29, 30], [0, 3], [-1, 0, 1], [0, 1]):
        run_case(0x42ff20, [10, 10], rows=({'id': -1, 'type': kind, 'state': status, 'population': population,
                     'infected': flag},), ids=(-1,)); count+=1
    def cursor(c, u, state, calls):
        put(u, 0xe38ebc, 0)
        def tutorial(uc, a):
            calls.append((0x417ae0, a))
            if len(calls)==1:
                for at in [0xbc9768, 0xd3b4e4, 0xd3b4e0]: put(uc, at, get(uc, at)+1)
                put(uc, 0xbca2d0, 1); put(uc, 0xbca2d4, 1)
            return 0
        hook(c, state, 0x417ae0, 1, tutorial)
    run_case(0x42ff20, [10, 10], cursor); count+=1
    for health, month in itertools.product([-2147483648, -1, 0, 60, 80, 99, 100, 120, 0x7fffffff], [0, 9, 11, 256]):
        run_case(0x430060, [1], health=health, month=month); count+=1
    for pos in [(0, 0), (31, 31), (40, 10), (10, 40), (-3, 10), (10, -3)]:
        run_case(0x430060, [1], pos=pos); count+=1
    for timer, people, status in itertools.product([-2147483648, -1, 0, 1, 2], [-1, 0, 1], [0, 3]):
        run_case(0x430250, rows=({'id': 4000, 'state': status, 'population': people, 'timer': timer},), ids=(4000,)); count+=1
    for final_limit in [0, 2]:
        def limit(c, u, state, calls):
            put(u, 0x78d790, 1)
            def population(uc, a): calls.append((0x4e90f0, a)); put(uc, 0x78d790, final_limit); return 0
            hook(c, state, 0x4e90f0, 2, population)
        run_case(0x430250, prepare=limit, rows=({'id': 1, 'timer': 1}, {'id': 2, 'timer': 1}), ids=(1, 2)); count+=1
    run_case(0x430040, [1]); count+=1
    print('PASS live spread cursor, source/radius/month rules, per-cell distances, live high-water latency and malaria:', count, flush=True)


def verify_spawn_and_messages():
    count=0
    for building, allocated in itertools.product([0, 1, 4000], [0, 1, 1999, 0x80000001]):
        def prepare(c, u, state, calls):
            def actor(uc, a): calls.append((0x4b4800, (a[0]&255, *a[1:]))); return allocated
            def patrol(uc, a): calls.append((0x4b1df0, a)); return 0
            hook(c, state, 0x4b4800, 5, actor); hook(c, state, 0x4b1df0, 1, patrol)
        run_case(0x42fe90, [building], prepare, al_only=True,
                 rows=({'id': building, 'population': 10},), ids=(building,)); count+=1
    for bits in range(8):
        def prepare(c, u, state, calls):
            for i in range(3): put(u, FLAGS[i], (bits >> i)&1); put(u, SOURCES[i], i+1)
            def message(uc, a): calls.append(a); return 0
            hook(c, state, 0x4e0cf0, 5, message)
        run_case(0x4302b0, prepare=prepare); count+=1
    def mutate(c, u, state, calls):
        put(u, FLAGS[0], 256); put(u, SOURCES[0], 1)
        def message(uc, a):
            calls.append(a); n=len(calls)
            put(uc, FLAGS[n-1], 1)
            if n<3: put(uc, FLAGS[n], 1); put(uc, SOURCES[n], n+1)
            return 0
        hook(c, state, 0x4e0cf0, 5, message)
    run_case(0x4302b0, prepare=mutate); count+=1
    def queued(c, u, state, calls):
        # reset() poisons the settings block with A5. Its enabled notification
        # filters would request toast text from an uninitialized language pack.
        # Use explicit preferences and queued UI mode; run the real filter,
        # publisher and event-slot allocator, without a message/text hook.
        put(u, 0xe38ef0, 0); byte(u, 0xe38dda, 2)
        for i in range(3): put(u, FLAGS[i], 1); put(u, SOURCES[i], i+1)
        for message in [0x146, 0x147, 0x148]: u.mem_write(0xe209b8+message*80, bytes(80))
    result=run_case(0x4302b0, prepare=queued); count+=1
    memory=result[1]
    def read(at): return struct.unpack_from('<I', memory, at-0x560000)[0]
    assert [read(at) for at in FLAGS] == [0, 0, 0]
    assert read(0xe951ec) == 3
    assert [read(0xe95080+i*4) for i in range(3)] == [42, 43, 44]
    for i, message in enumerate([0x147, 0x146, 0x148]):
        event=0xe95228+i*48
        assert struct.unpack_from('<h', memory, event+10-0x560000)[0] == message
        assert read(event) == 13 and read(event+4) == 2301+i
    def actor_real(c, u, state, calls):
        u.mem_write(0xbcd5f8, bytes(2000*388))
        def patrol(uc, a): calls.append((0x4b1df0, a)); return 0
        hook(c, state, 0x4b1df0, 1, patrol)
    run_case(0x42fe90, [1], actor_real, al_only=True); count+=1
    print('PASS disease walker success/failure/fields, real actor allocation, ordered flags and genuine queued messages:', count, flush=True)
    print('Verified target SHA256:', target_sha, flush=True)


def verify_selectors_and_callers():
    count=0
    rows=({'id': 1, 'population': 10}, {'id': 2, 'population': -1},
          {'id': 4000, 'population': 20})
    def observe_selected(c, u, state, calls):
        def initiate(uc, args): calls.append((0x42fe90, args)); return int(args[0] != 0)
        hook(c, state, 0x42fe90, 1, initiate)
    for scores in [(-2147483648, -1, 0), (0, 0, 0), (1, 1, 1),
                   (1, 2, 0), (0, 1, 2), (0x7fffffff, 0, 0x7fffffff)]:
        def prepare(c, u, state, calls):
            for row, score in zip(rows, scores): put(u, 0x799ad4+row['id']*264, score)
            observe_selected(c, u, state, calls)
        # O2 folds the ranked helper into its sole real caller. Observe the
        # selected ID at that caller's initiation boundary, without adding an
        # artificial production reference merely to keep a test entry alive.
        result=run_case(0x42fcf0, [0], prepare=prepare, al_only=True, rows=rows, ids=(1, 2, 4000))
        expected=0; maximum=0
        for row, score in zip(rows, scores):
            if score>maximum: expected=row['id']; maximum=score
        assert result[0][1] == [(0x42fe90, (expected,))]; count+=1
    for status, occupied, population in itertools.product([0, 3], [0, 1], [0, -1]):
        result=run_case(0x42fcf0, [0], observe_selected, al_only=True,
            rows=({'id': 4000, 'state': status, 'occupied': occupied, 'population': population},), ids=(4000,))
        expected=4000 if status and occupied and population else 0
        assert result[0][1] == [(0x42fe90, (expected,))]; count+=1
    for random, scores, cursor in itertools.product([0, 1, 256, 257], [0, 10], [0, 99, 110, 111]):
        def prepare(c, u, state, calls):
            for row in rows: put(u, 0x799ad4+row['id']*264, scores)
            put(u, 0xd3b1c4, cursor); u.mem_write(0xbcd5f8, bytes(2000*388))
            def patrol(uc, args): calls.append((0x4b1df0, args)); return 0
            hook(c, state, 0x4b1df0, 1, patrol)
        result=run_case(0x42fcf0, [random], prepare, al_only=True,
                        rows=rows, ids=(1, 2, 4000))
        assert result[0][0] == (1 if scores or random & 255 else 0); count+=1
    for random in [0, 1, 256, 257]:
        run_case(0x42fcf0, [random], al_only=True, rows=(), ids=(0,)); count+=1
    print('PASS canonical ranked/reservoir selection, signed scores, ties, final record, BYTE choice and real infection/actor composition:', count, flush=True)
    count=0
    for pos, tutorial_mode in itertools.product([(10, 10), (0, 0), (31, 31)], [0, 1]):
        def prepare(c, u, state, calls):
            p=a(255); byte(u, p+10, 98); word(u, p+20, pos[0]); word(u, p+22, pos[1])
            word(u, p+90, 511); put(u, p+32, 2290)
            put(u, 0xe38ebc, tutorial_mode); put(u, 0x5dbf70+98*4, 2)
        run_case(0x4baa10, [255], prepare, pos=pos); count+=1
    print('PASS real disease-walker service dispatcher, infection and cohort adjustment at map edges:', count, flush=True)
    print('Verified target SHA256:', target_sha, flush=True)


if __name__ == '__main__':
    verify_population(); verify_rng(); verify_infection(); verify_spread_and_latency(); verify_spawn_and_messages(); verify_selectors_and_callers()
