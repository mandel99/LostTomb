"""Whole4E81D0 city health against original x86, offline.

Normal fixtures observe quality updates and toast publication. Compositions
execute the actual quality/RNG/infection/cohort/queued-message graph. Named
callback fixtures control only their explicit dependency. No game runs.
"""
from verify_disease_updates import *

for owner in [0, 3]:
    WATCH.update(at+owner*0x49d8 for at in [0xea1884, 0xea1888, 0xea5fc0])
WATCH.add(0xeaac2c)
for id in [1, 2, 3999, 4000]: WATCH.update([0x799a5f+id*264, 0x799ac8+id*264])


def quality_hook(c, state, action):
    if c: state['hooks'][sym('restore_building_quality_004e7f00')]=(2, 0, action)
    else: hook(c, state, 0x4e7f00, 2, action)


def case(rows=None, current=60, population=2000, mission=2, mode=0, flag=0,
         gate=0, table=None, real_quality=False, prepare=None, control=0x37f,
         occupied_fp=False, real_toast=False, calendar=False):
    if rows is None: rows=({'id': 1, 'population': 101},)
    results=[]
    for c, u, state in machines:
        setup_disease(u, state, rows=rows, ids=tuple(row['id'] for row in rows) or (0,), health=current)
        byte(u, 0xe38ea8, mission); put(u, 0xe38ebc, mode); byte(u, 0xeaac2c, flag)
        byte(u, 0xe38dda, 2); put(u, 0xe38ef0, 0)
        u.mem_write(0xbcd5f8, bytes(2000*388))
        for owner in [0, 3]:
            off=owner*0x49d8
            put(u, 0xea1884+off, 37); put(u, 0xea1894+off, population)
            put(u, 0xea5fc0+off, 123); word(u, 0xea5ce8+off, gate)
            put(u, 0xea5f9c+off, 511)
        for i, (threshold, bonus) in enumerate(table or [(0, 0)]*10):
            put(u, 0x5e4b8c+i*8, threshold); put(u, 0x5e4b90+i*8, bonus)
        for row in rows:
            p=0x7999d8+row['id']*264
            for field, value in [(110, row.get('apothecary', 0)),
                                 (120, row.get('physician', 0)), (127, row.get('foods', 0))]: byte(u, p+field, value)
            for field, value in [(232, row.get('plague', 70)), (240, row.get('stage', 2)),
                                 (244, 10), (248, 10), (252, row.get('malaria', 80))]: put(u, p+field, value)
        calls=[]
        if not real_quality:
            def quality(uc, args): calls.append((0x4e7f00, (args[0], args[1]&255))); return 0
            quality_hook(c, state, quality)
        for token, count in [(0x417ae0, 1), (0x4b1df0, 1)]:
            def observed(uc, args, token=token): calls.append((token, args)); return 0
            hook(c, state, token, count, observed)
        if not real_toast:
            def toast(uc, args): calls.append((0x4e2970, args)); return 0
            hook(c, state, 0x4e2970, 1, toast)
        if prepare: prepare(c, u, state, calls)
        if calendar:
            put(u, 0x78d7c4, 15); put(u, 0x78d7c0, 3); put(u, 0x78d7c8, 999)
            byte(u, 0xe38ec2, 0); byte(u, 0x7027b8, 0); byte(u, 0x7027be, 0); byte(u, 0x7027b9, 1)
            endpoints=dict(CALENDAR_ENDPOINTS); del endpoints[0x4e81d0]
            for token, nargs in endpoints.items():
                def observed(uc, args, token=token): calls.append((token, args)); return 0
                hook(c, state, token, nargs, observed)
        u.reg_write(UC_X86_REG_FPCW, control); u.reg_write(UC_X86_REG_FPSW, 3<<11 if occupied_fp else 0)
        u.reg_write(UC_X86_REG_FPTAG, 0xffff)
        if occupied_fp:
            u.reg_write(UC_X86_REG_FP3, (0x8000000000000000, 0x3fff))
            u.reg_write(UC_X86_REG_FP4, (0xc000000000000000, 0x4000))
            u.reg_write(UC_X86_REG_FPTAG, 0xffff & ~(3<<6) & ~(3<<8))
        live_fp=[u.reg_read(UC_X86_REG_FP3), u.reg_read(UC_X86_REG_FP4)] if occupied_fp else []
        state['service_writes'].clear(); run(c, u, 0x452370 if calendar else 0x4e81d0)
        assert u.reg_read(UC_X86_REG_FPCW)==control
        assert u.reg_read(UC_X86_REG_FPSW)&0x3800 == (3<<11 if occupied_fp else 0)
        if occupied_fp: assert [u.reg_read(UC_X86_REG_FP3), u.reg_read(UC_X86_REG_FP4)]==live_fp
        results.append(((calls, list(state['service_writes'])), snapshot(u)))
    same(results, ('health update', rows, current, population, mission, mode, flag, gate, table, real_quality, control, occupied_fp))
    return results[1]


def verify_gates_and_expiry():
    count=0
    for mission, mode in itertools.product([0, 1, 2, 127, 128, 255], [0, 256]):
        case(mission=mission, mode=mode, flag=1); count+=1
    for state, occupied, people in itertools.product([0, 1, 3], [0, 1], [0, -1, 1]):
        result=case(rows=({'id': 3999, 'state': state, 'occupied': occupied, 'population': people},))
        quality=[a for token, a in result[0][0] if token==0x4e7f00]
        assert quality == ([(3999, 0)] if state==1 and occupied and people else []); count+=1
    for timer, infected, status in itertools.product([-2147483648, -1, 0, 1, 2], [0, 1], [1, 3]):
        case(mission=0, rows=({'id': 1, 'state': status, 'infected': infected, 'stage': timer},
             {'id': 3999, 'state': status, 'infected': infected, 'stage': timer},
             {'id': 4000, 'state': status, 'infected': infected, 'stage': timer})); count+=1
    result=case(rows=({'id': 3999}, {'id': 4000}), flag=255)
    assert [a for token, a in result[0][0] if token==0x4e7f00] == [(3999, 255)]; count+=1
    print('PASS signed mission/full mode, exact state/population gates, fixed3999 limit and infection expiry:', count, flush=True)


def verify_weights_and_rounding():
    count=0
    for physician, apothecary, foods in itertools.product([0, 255], [0, 96], [0, 1, 2, 3, 255]):
        case(rows=({'id': 1, 'physician': physician, 'apothecary': apothecary, 'foods': foods},)); count+=1
    for people in [-32768, -1, 1, 99, 100, 32767]:
        case(rows=({'id': 1, 'population': people, 'physician': 1, 'apothecary': 1, 'foods': 3},)); count+=1
    for pc, rc, people, full in itertools.product([0, 0x200, 0x300], [0, 0x400, 0x800, 0xc00], [1, 32767], [0, 1]):
        case(control=0x7f|pc|rc, occupied_fp=True, rows=({'id': 1, 'population': people,
             'physician': full, 'apothecary': 1, 'foods': full*3},)); count+=1
    print('PASS service/food weights, signed WORD populations, inherited x87 precision/rounding and occupied caller stack:', count, flush=True)


def verify_population_bonus_and_notifications():
    count=0
    table=[(100+i*100, i*7-20) for i in range(10)]
    for population in [0, 99, 100, 101, 299, 300, 999, 1000, 0x7fffffff, -2147483648]:
        case(population=population, table=table); count+=1
    edges=[(9, 15), (10, 0), (11, 0), (19, 25), (20, 15), (21, 0),
           (29, 35), (30, 25), (31, 0), (39, 45), (40, 35), (41, 0),
           (101, 101), (-1, -1), (-2147483648, 0x7fffffff), (0x7fffffff, -2147483648)]
    for (current, target), population in itertools.product(edges, [0, 300, 301]):
        def prepare(c, u, state, calls):
            hook(c, state, 0x4cde60, 2, lambda uc, args: target)
            hook(c, state, 0x42fbb0, 0, lambda uc, args: 0)
        case(current=current, population=population, prepare=prepare); count+=1
    print('PASS population bonus table, ramp direction/clamp, decade/low-health notifications and DWORD wrap:', count, flush=True)


def verify_callbacks_and_compositions():
    count=0
    def quality_changes(c, u, state, calls):
        def changed(uc, args):
            calls.append((0x4e7f00, (args[0], args[1]&255)))
            if args[0]==1:
                put(uc, 0xeaac28, 3); byte(uc, 0xeaac2c, 255)
                word(uc, 0x7999f4+264, 7); put(uc, 0x799ac0+264, 1000)
                byte(uc, 0x799a50+264, 255); word(uc, 0x7999f4+2*264, 50)
            return 0
        quality_hook(c, state, changed)
    result=case(prepare=quality_changes, rows=({'id': 1}, {'id': 2}))
    assert [args for token, args in result[0][0] if token==0x4e7f00] == [(1, 0), (2, 255)]; count+=1
    for category, permission in itertools.product([0, 4, 0x104], [0, 256, 257]):
        def switches(c, u, state, calls):
            def ratio(uc, args):
                calls.append((0x4cde60, args)); put(uc, 0xeaac28, 3)
                word(uc, 0xea5ce8+3*0x49d8, 1); put(uc, 0xea5f9c+3*0x49d8, 1000); return 17
            def classify(uc, args): calls.append((0x4c5120, args)); put(uc, 0xeaac28, 0); return category
            def allowed(uc, args): calls.append((0x4c52d0, args)); put(uc, 0xeaac28, 3); return permission
            def percent(uc, args): calls.append((0x4cde40, args)); put(uc, 0xeaac28, 0); return 35
            for token, nargs, action in [(0x4cde60, 2, ratio), (0x4c5120, 1, classify),
                    (0x4c52d0, 2, allowed), (0x4cde40, 2, percent)]: hook(c, state, token, nargs, action)
        case(prepare=switches); count+=1
    for roll in [0, 1, 256, 257, 0xffffffff]:
        def rng_switch(c, u, state, calls):
            def rng(uc, args):
                calls.append((0x42fbb0, args)); put(uc, 0xeaac28, 3)
                put(uc, 0xea1888+3*0x49d8, 20); put(uc, 0xea1884+3*0x49d8, 100); return roll
            def initiate(uc, args):
                calls.append((0x42fcf0, args)); put(uc, 0xeaac28, 0)
                put(uc, 0xea1888, -1); put(uc, 0xea1884, -1); return 1
            hook(c, state, 0x42fbb0, 0, rng); hook(c, state, 0x42fcf0, 1, initiate)
        result=case(prepare=rng_switch)
        assert len([args for token, args in result[0][0] if token==0x42fcf0])==bool(roll&255); count+=1
    def toast_switch(c, u, state, calls):
        hook(c, state, 0x4cde60, 2, lambda uc, args: 100)
        hook(c, state, 0x42fbb0, 0, lambda uc, args: 0)
        def toast(uc, args):
            calls.append((0x4e2970, args)); put(uc, 0xeaac28, 3); put(uc, 0xea1888+3*0x49d8, 250); return 0
        hook(c, state, 0x4e2970, 1, toast)
    result=case(current=9, prepare=toast_switch)
    assert (0x4e2970, (0x63,)) in result[0][0]
    assert struct.unpack_from('<I', result[1], 0xea1888+3*0x49d8-0x560000)[0]==100; count+=1
    for difficulty, current in itertools.product([0, 1, 2], [0, 40, 75, 100]):
        def model(c, u, state, calls):
            byte(u, 0x784cb0, difficulty)
            for level in [0, 13]:
                put(u, 0x70ce1c+level*88, 0); put(u, 0x70ce20+level*88, 0)
        case(current=current, real_quality=True, prepare=model,
             rows=({'id': 1, 'physician': 1, 'apothecary': 96, 'foods': 3}, {'id': 2})); count+=1
    for disease in ['plague', 'malaria']:
        def model(c, u, state, calls):
            byte(u, 0x784cb0, 1)
            for level in [0, 13]:
                put(u, 0x70ce1c+level*88, 0); put(u, 0x70ce20+level*88, 0)
        result=case(current=0, real_quality=True, prepare=model,
            rows=({'id': 1, disease: 1000}, {'id': 2}))
        events=struct.unpack_from('<I', result[1], 0xe951ec-0x560000)[0]
        assert events>0, ('Real disease composition must publish a queued event', disease); count+=1
    print('PASS callback-sensitive owner/flag/population reloads, AL/full-result gates, actual quality/RNG/disease/queued-message compositions:', count, flush=True)


def verify_monthly_caller():
    count=0
    for mission in [0, 2]:
        result=case(mission=mission, calendar=True)
        quality=[args for token, args in result[0][0] if token==0x4e7f00]
        assert quality == ([] if mission==0 else [(1, 0)])
        count+=1
    print('PASS actual monthly calendar reaches new health provider, tutorial exception and ordered later updates:', count, flush=True)


if __name__=='__main__':
    verify_gates_and_expiry(); verify_weights_and_rounding(); verify_population_bonus_and_notifications(); verify_callbacks_and_compositions(); verify_monthly_caller()
    print('Verified target SHA256:', target_sha, flush=True)
