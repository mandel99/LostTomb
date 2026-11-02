"""Complete next-direction, ferry search, midpoint and live terrain validator.

Compare native/current C state, ordered writes, logical callbacks and ABI.
Ordinary cases control grid-link, relocation and collision providers; explicit
compositions retain the real route consumer, ferry scan, midpoint or queues.
"""
from verify_actor_movement import *

for id in [1, 255, 1999]:
    WATCH.update(a(id)+off for off in [14, 16, 17, 18, 19, 20, 22, 24, 26,
                                      32, 54, 56, 57, 60, 65, 119, 121])


def checked(token=0x4b3530, terrain=0, cell=0, water_cell=0, road_cell=0,
            region=0, building_kind=1, ferry=1, full=0, prepare=None,
            compose=(), **kw):
    def setup(c, u, state, calls):
        actor=kw.get('actor', 255);p=a(actor)
        direction=b(u, p+17);signed=direction if direction<128 else direction-256
        # Keep the validator's destination mapped even for signed BYTE cases.
        put(u, p+32, 12000-get(u, 0x5f80fc+signed*4))
        word(u, p+60, 0);word(u, p+62, 3)
        u.mem_write(0xc8ad38+500, bytes([2])*500)
        for at, value in [(0xaf598c+12000*4, terrain), (0xbca2d8, 7000),
                          (0x799a9c+264, 2), (0xbcd5e8, 0)]:put(u, at, value)
        word(u, 0xb4e6fc+12000*2, 1);word(u, 0x7999e8+264, building_kind)
        word(u, 0x799a18+264, 1);word(u, 0x799a18+528, 1)
        word(u, 0x799a02+264, 1999);byte(u, 0x799aa2+264, 1)
        byte(u, 0xec7630+12000, cell);byte(u, 0xed4140+12000, water_cell)
        byte(u, 0xebab20+12000, road_cell);byte(u, 0xee0c50+12000, region)
        byte(u, 0x78d7cc, 0);byte(u, p+119, 0xa5);byte(u, p+121, 0x5a)
        for endpoint, nargs in [(0x4b3250, 1), (0x4b5420, 1), (0x4b52f0, 1),
                                (0x4690f0, 2), (0x4b1210, 2), (0x4b3470, 2),
                                (0x51cba0, 4)]:
            def observed(cpu, args, endpoint=endpoint):
                calls.append((endpoint, args, get(cpu, 0xd056f8), bytes(cpu.mem_read(p+16, 108))))
                return ferry if endpoint==0x4b3470 else 8 if endpoint==0x51cba0 else 0
            hook(c, state, endpoint, nargs, observed)
        def relocate(cpu, args):
            calls.append((0x4ad880, get(cpu, args[0]), get(cpu, args[1])))
            put(cpu, args[0], 41);put(cpu, args[1], 42);return 0
        hook(c, state, 0x4ad880, 2, relocate)
        def queue_full(cpu, args):
            self=args[0] if c else cpu.reg_read(UC_X86_REG_ECX)
            calls.append((0x4f6650, self));return full
        def queue_push(cpu, args):
            self, ptr=args if c else (cpu.reg_read(UC_X86_REG_ECX), args[0])
            calls.append((0x4f65b0, self, get(cpu, ptr)));return 1
        if c:
            state['hooks'][sym('restore_actor_walking_queue_full')]=(1, 0, queue_full)
            state['hooks'][sym('restore_actor_walking_queue_push')]=(2, 0, queue_push)
        else:
            hook(c, state, 0x4f6650, 0, queue_full)
            hook(c, state, 0x4f65b0, 1, queue_push, cleanup=4)
        for endpoint in (token, *compose):
            state['hooks'].pop(address(c, endpoint), None)
        if prepare:prepare(c, u, state, calls)
    return case(token, prepare=setup, **kw)


def verify_direction():
    count=0
    for route, index, length in itertools.product([-1, 0, 1], [-1, 0, 3, 32767], [0, 3]):
        def setup(c, u, state, calls):word(u, a(255)+60, index);word(u, a(255)+62, length)
        checked(0x4b33d0, route=route, prepare=setup);count+=1
    for result in [0, 1, 7, 8, 9, 256, 264, 0xffffffff]:
        def setup(c, u, state, calls):hook(c, state, 0x51cba0, 4, lambda cpu, args: result)
        checked(0x4b33d0, route=0, prepare=setup);count+=1
    def changed(c, u, state, calls):
        word(u, a(255)+60, 3)
        def clear(cpu, args):
            calls.append((0x4b3250, args));put(cpu, 0xd056f8, 1);byte(cpu, a(255)+17, 127);return 0
        hook(c, state, 0x4b3250, 1, clear)
    checked(0x4b33d0, prepare=changed);count+=1
    print('PASS next direction, AL fallback, signed route bounds and cached actor through cleanup:', count, flush=True)


def verify_crossing():
    count=0
    for position, first_staff, second_staff in itertools.product([-1, 0, 2, 3], [-1, 0, 1], [-1, 0, 1]):
        def setup(c, u, state, calls):
            put(u, a(255)+32, 12000)
            for i in range(4):word(u, 0xb4e6fc+(12000+i)*2, 2 if i==position else 3)
            word(u, 0x7999e8+2*264, 136);word(u, 0x7999e8+3*264, 1)
            word(u, 0x799a18+264, first_staff);word(u, 0x799a18+528, second_staff)
        checked(0x4b3470, prepare=setup);count+=1
    for step, length in [(-1, 0), (0, 0), (3, 3), (4, 3), (32767, -32768)]:
        def setup(c, u, state, calls):
            put(u, a(255)+32, 12000);word(u, a(255)+60, step);word(u, a(255)+62, length)
            byte(u, 0xc8ad38+499, 2)
        checked(0x4b3470, prepare=setup);count+=1
    print('PASS ferry scanning current tile before remaining steps, counterpart/staffing and signed limits:', count, flush=True)


def verify_midpoint():
    count=0
    for direction, terrain in itertools.product([*range(10), 127, 128, 255], [0, 64, 0x40000000, 68, 4]):
        def setup(c, u, state, calls):
            put(u, a(255)+32, 12000)
            for tile in range(11700, 12300):put(u, 0xaf598c+tile*4, terrain)
        checked(0x4b3c60, direction=direction, prepare=setup);count+=1
    for sprite, kind in itertools.product([*range(15), 255], [4, 77, 78, 92, 93, 100, 101, 102]):
        def setup(c, u, state, calls):
            put(u, a(255)+32, 11999);put(u, 0xaf598c+12000*4, 68)
            byte(u, 0xadc36c+12000, sprite);word(u, 0xb350dc+12000*2, -1)
        checked(0x4b3c60, direction=2, kind=kind, prepare=setup);count+=1
    for endpoint in [0x4b5420, 0x4b52f0, 0x4b1210]:
        for changed_dir in [2, 8, 255]:
            def setup(c, u, state, calls):
                put(u, a(255)+32, 11999)
                def changed(cpu, args):
                    calls.append((endpoint, args));put(cpu, 0xd056f8, 1)
                    byte(cpu, a(255)+17, changed_dir);put(cpu, a(255)+32, 12003)
                    word(cpu, a(255)+20, 0x7fff);word(cpu, a(255)+22, 0x8000)
                    put(cpu, 0xaf598c+12003*4, 68);return 0
                hook(c, state, endpoint, 2 if endpoint==0x4b1210 else 1, changed)
            checked(0x4b3c60, direction=2, prepare=setup);count+=1
    print('PASS midpoint directions, road/bridge heights, collision exclusions, coordinate wrap and callback reloads:', count, flush=True)


def verify_terrain():
    count=0
    for direction, mode, water in itertools.product([0, 7, 8, 9, 127, 128, 255],
            [0, 1, 2, 3, 4, 5, 6, 7, 9, 10, 11, 255], [0, 1]):
        checked(direction=direction, water=water, terrain=4,
            prepare=lambda c,u,state,calls: byte(u,a(255)+114,mode))
        count+=1
    for mode, cell, flags, tick in itertools.product([2, 4, 6], [0, 1, 2, 3, 4, 5, 127, 128, 255],
                                                   [0, 0x400, 0x10004], [0, 1]):
        def setup(c, u, state, calls):byte(u,a(255)+114,mode);byte(u,0x78d7cc,tick)
        checked(cell=cell, road_cell=cell, region=cell, terrain=flags, prepare=setup);count+=1
    for flags, building_kind, gate in itertools.product(
            [0, 4, 8, 64, 0x400, 0x4000, 0x8000, 0x10000000|8, 0x40000008],
            [1, 54, 56, 58, 136, 138, 170, 171, 183, 200, 201, 202, 209], [0, 1]):
        def setup(c, u, state, calls):put(u,0xbcd5e8,gate)
        checked(terrain=flags, building_kind=building_kind, prepare=setup);count+=1
    for region, kind in itertools.product([1, 3], [0, 1, 3, 4, 94, 255]):
        checked(region=region, kind=kind);count+=1
    print('PASS live terrain validator: signed gates, land/water classes, obstruction timers, masks and relocation:', count, flush=True)


def verify_ferries():
    count=0
    for kind, ferry, full in itertools.product([*range(110), 255], [0, 1, 2, 0xffffffff], [0, 1]):
        checked(terrain=8, building_kind=136, kind=kind, ferry=ferry, full=full);count+=1
    for staff1, staff2, action in itertools.product([-1, 0, 1], [-1, 0, 1], [7, 9]):
        def setup(c, u, state, calls):
            word(u,0x799a18+264,staff1);word(u,0x799a18+528,staff2)
        checked(terrain=8, building_kind=136, action=action, prepare=setup);count+=1
    print('PASS all ferry actor/recovery table entries, crossing outcomes, full queue results and staffing/action gates:', count, flush=True)


def verify_mutations():
    count=0
    for region in [1, 3]:
        def setup(c,u,state,calls):
            pointers=[]
            def locate(cpu,args):
                pointers[:]=args;calls.append((0x4ad880,get(cpu,args[0]),get(cpu,args[1])))
                put(cpu,args[0],31);put(cpu,args[1],32);return 0
            def unlink(cpu,args):
                calls.append((0x4b5420,args));put(cpu,pointers[0],33);put(cpu,pointers[1],34)
                put(cpu,0xbca2d8,8000);put(cpu,0xd056f8,1);return 0
            hook(c,state,0x4ad880,2,locate);hook(c,state,0x4b5420,1,unlink)
        checked(region=region,prepare=setup);count+=1
    for answer, full in itertools.product([0,1,2],[0,1]):
        def setup(c,u,state,calls):
            def query(cpu,args):
                calls.append((0x4b3470,args));byte(cpu,0x799aa2+264,7)
                put(cpu,0x799a9c+264,3);byte(cpu,a(255)+10,42);put(cpu,0xd056f8,1)
                return answer
            hook(c,state,0x4b3470,2,query)
        checked(terrain=8,building_kind=136,ferry=answer,full=full,prepare=setup);count+=1
    for staff in [0,1]:
        def setup(c,u,state,calls):
            def full(cpu,args):
                self=args[0] if c else cpu.reg_read(UC_X86_REG_ECX)
                calls.append((0x4f6650,self));word(cpu,0x799a18+264,staff)
                word(cpu,0x799a18+528,staff);put(cpu,0xd056f8,1999);return 0
            if c:state['hooks'][sym('restore_actor_walking_queue_full')]=(1,0,full)
            else:hook(c,state,0x4f6650,0,full)
        checked(terrain=8,building_kind=136,prepare=setup);count+=1
    def argument(c,u,state,calls):
        byte(u,a(1)+56,11)
        def push(cpu,args):
            self,ptr=args if c else (cpu.reg_read(UC_X86_REG_ECX),args[0])
            calls.append((0x4f65b0,self,get(cpu,ptr)));put(cpu,ptr,1);return 0
        if c:state['hooks'][sym('restore_actor_walking_queue_push')]=(2,0,push)
        else:hook(c,state,0x4f65b0,1,push,cleanup=4)
    result=checked(terrain=8,building_kind=136,prepare=argument)
    assert result[1][a(1)+56-0x560000]==7;count+=1
    for stop in [1,2,3]:
        def setup(c,u,state,calls):
            counter=[0]
            def direction(cpu,args):
                counter[0]+=1;calls.append((0x4b33d0,args))
                byte(cpu,a(255)+17,8 if counter[0]==stop else 2);return 0
            hook(c,state,0x4b33d0,1,direction)
        checked(terrain=8,building_kind=136,ferry=2,prepare=setup);count+=1
    print('PASS escaped relocation outputs, cached ferry/queue data, live staffing and queue-owned actor slot:',count,flush=True)


def verify_compositions():
    count=0
    for staff in [0,1]:
        def setup(c,u,state,calls):
            put(u,a(255)+32,11999)
            word(u,0xb4e6fc+11999*2,3);word(u,0x7999e8+3*264,1)
            word(u,0xb4e6fc+12001*2,2);word(u,0x7999e8+528,136)
            word(u,0x799a18+528,staff)
        checked(terrain=8,building_kind=136,compose=(0x4b3470,),prepare=setup);count+=1
    for full in [False,True]:
        def setup(c,u,state,calls):
            queue=0xeaaf68+224
            put(u,queue+4,0);put(u,queue+8,50 if full else 0);put(u,queue+12,51)
            if c:
                for name in ['restore_actor_walking_queue_full','restore_actor_walking_queue_push']:
                    state['hooks'].pop(sym(name))
            else:
                for token in [0x4f6650,0x4f65b0]:state['hooks'].pop(token)
        checked(terrain=8,building_kind=136,prepare=setup);count+=1
    for kind in [4,77,100]:
        def setup(c,u,state,calls):
            put(u,a(255)+32,11999);word(u,0xb350dc+11999*2,255)
            word(u,0xb350dc+12000*2,1999);put(u,a(1999)+32,12000)
            for token in [0x4b5420,0x4b52f0]:state['hooks'].pop(address(c,token))
        checked(0x4b3c60,direction=2,kind=kind,prepare=setup);count+=1
    for steps in [1,9,24]:
        def setup(c,u,state,calls):
            put(u,a(255)+32,11999)
            for tile in range(11700,12300):put(u,0xaf598c+tile*4,64)
            for token in [0x4b33d0,0x4b3530,0x4b3c60,0x4baa10]:state['hooks'].pop(address(c,token))
        checked(0x4b27e0,phase=14,steps=steps,prepare=setup);count+=1
    print('PASS actual ferry scan/queues, grid unlink/link, service dispatcher and movement-step chain:',count,flush=True)


if __name__=='__main__':
    verify_direction();verify_crossing();verify_midpoint();verify_terrain();verify_ferries()
    verify_mutations();verify_compositions()
    print('Verified target SHA256:',target_sha,flush=True)
