"""Whole 4DEBE0 native/C preview with the actual 13-DWORD geometry helper.

Only placement acceptance, screen projection and masked drawing are controlled.
The geometry tables, all 116 pieces and rotation adjustments execute unchanged.
No host game, original executable process or display is started.
"""
from verify_options_startup import *


def exercise(rotation, allowed, scale, mutate='', parent=False, composed=False):
    results=[]
    for c,u,state in machines:
        reset(u,state)
        # Poison the physical stack so a split/uninitialized output record cannot
        # accidentally inherit suitable values from a preceding invocation.
        u.mem_write(SP-0x4000,b'\xa5'*0x4000)
        for a,v in [(0xbca2e0,rotation),(0xbca300,30000),(0xbca304,411),
                    (0xbca308,301),(0x70e45c,scale),(0x70e44c,15),
                    (0xf56be4,0x123456),(0x6059e8,222),(0x605a3c,0),
                    (0xbca2d8,0),(0xbca2e8,0),(0xbca2ec,0)]:put(u,a,v)
        # Populate the real 116-record table's four screen-offset families.
        run(c,u,0x4dee10)
        calls=[];draws=0;queries=0
        if not composed:
            def precheck(cpu,args):
                assert args==(30000,),(c,args)
                if mutate=='accept':put(cpu,0xbca2e0,6);put(cpu,0xbca304,510)
                return 0x12340001 if allowed else 0x12340000
            hook(c,state,0x4def00,1,precheck)
        else:
            # Real placement acceptance also runs; only its map-boundary query
            # is controlled, while the terrain and 13-DWORD geometry stay real.
            u.mem_write(0xaf598c,bytes(51984*4))
            def outside(cpu,args):
                nonlocal queries
                queries+=1
                return 0x12340000 if allowed else 0x12340001
            hook(c,state,0x46f9a0,3,outside)
        def project(cpu,args):
            calls.append(('project',args[:2]))
            put(cpu,args[2],560);put(cpu,args[3],330)
            if mutate=='project':put(cpu,0xbca2e0,4);put(cpu,0x70e45c,41)
            return 0
        def draw(cpu,args):
            nonlocal draws
            calls.append(('draw',args));draws+=1
            if mutate=='draw' and draws==1:
                put(cpu,0xbca2e0,2);put(cpu,0x70e44c,21);put(cpu,0x70e45c,53)
                put(cpu,0xf56be4,0x234567)
            cpu.reg_write(UC_X86_REG_ECX,0xdeadbeef)
            cpu.reg_write(UC_X86_REG_EDX,0xbaadf00d)
            return 0
        hook(c,state,0x4f0ba0,4,project)
        hook(c,state,0x53f4f0,4,draw,cleanup=0 if c else 16)
        run(c,u,0x43b630 if parent else 0x4debe0,budget=5000000)
        assert draws>116,(c,rotation,draws)
        results.append((calls,queries,tuple(get(u,a) for a in
            [0xbca2e0,0xbca304,0x70e45c,0x70e44c,0xf56be4,0xbc9768])))
    assert results[0]==results[1],(rotation,allowed,scale,mutate,parent,composed,
        next(((i,a,b) for i,(a,b) in enumerate(zip(results[0][0],results[1][0])) if a!=b),None),
        [len(r[0]) for r in results],results[0][1:],results[1][1:])


if __name__=='__main__':
    count=0
    for rotation,allowed,scale in itertools.product([0,2,4,6],[False,True],[29,315,0xfffffffd]):
        exercise(rotation,allowed,scale,parent=True);count+=1
    for rotation,mutate in itertools.product([0,2,4,6],['accept','project','draw']):
        exercise(rotation,True,29,mutate=mutate);count+=1
    print('PASS whole monument preview: %d native/C cases, actual 116 geometry records and all four rotations'%count,flush=True)
    count=0
    for rotation,allowed in itertools.product([0,2,4,6],[False,True]):
        exercise(rotation,allowed,29,parent=True,composed=True);count+=1
    print('PASS actual placement-acceptance + preview composition: %d native/C cases'%count,flush=True)
    print('target SHA256',target_sha)
