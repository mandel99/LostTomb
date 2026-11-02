"""Native/C shoreline previews and Water/Hide cliffs map footprints.

Whole function execution with controlled projection, placement and final draw
endpoints. Ordered images/coordinates, tint and scratch bytes are compared;
this does not certify live framebuffer pixels or gameplay.
"""
from verify_options_startup import *


def shoreline(kind,result,cash,special,mutate=False,parent=False):
    outputs=[]
    for c,u,state in machines:
        reset(u,state)
        for a,v in [(0xbca304,411),(0xbca308,301),(0x6059e8,kind),
                (0xeaac28,2),(0xea187c+2*0x49d8,cash),(0x70e44c,271),
                (0xbc96dc,6),(0xf56ccc,100000),(0xf56be4,0x23456),
                (0x70e120,0xabcd5555),(0xbca32c,0x12345678),
                (0xbca300,1),(0xe38dcc,0),(0x605994,0),(0x605a3c,0)]:put(u,a,v)
        # Original-sized table: byte address kind*20, library entries*8.
        put(u,0x5d06e4+kind*20,517);put(u,0x5d06e8+kind*20,3)
        put(u,0xf55bb4+517*8,100000 if special else 200000)
        calls=[]
        def observe(cpu,args,t):
            value=0
            if t==0x4f0ba0:
                put(cpu,args[2],600);put(cpu,args[3],450);record=args[:2]
            elif t==0x47cc70:
                value=result;record=args
                if mutate:put(cpu,0x70e44c,285);put(cpu,0xbc96dc,4)
            elif t==0x4893c0:value=0x123456ab;record=args
            elif t==0x487d30:value=0x34567;record=args
            else:
                record=(*args,get(cpu,0x70e120))
                if mutate:
                    put(cpu,0x70e44c,get(cpu,0x70e44c)+13)
                    put(cpu,0xf56be4,get(cpu,0xf56be4)+1)
            calls.append((t,record))
            cpu.reg_write(UC_X86_REG_ECX,0xdeadbeef);cpu.reg_write(UC_X86_REG_EDX,0xbaadf00d)
            return value
        for t,n in [(0x4f0ba0,4),(0x47cc70,4),(0x4893c0,5),(0x487d30,2),
                (0x53e590,3),(0x53f250,3),(0x53f4f0,4)]:
            hook(c,state,t,n,lambda cpu,args,t=t:observe(cpu,args,t),
                cleanup=(16 if t==0x53f4f0 else 12 if t in [0x53e590,0x53f250] else 0) if not c else 0)
        run(c,u,0x43b630 if parent else 0x43dd70,() if parent else [0x87654321,0 if kind in [76,181] else 1])
        outputs.append((calls,get(u,0x70e120),get(u,0xbca32c)))
    assert outputs[0]==outputs[1],('shoreline',kind,result,cash,special,mutate,parent,outputs)


def footprint(token,tile,flags,foot,status,second,mode=0,transparent=0,mutate=False,real_selector=False,parent=0):
    outputs=[]
    for c,u,state in machines:
        reset(u,state)
        building=-1 if tile==51983 else 777;record=building*264
        for a,v in [(0x70e488,tile),(0xaf598c+tile*4,flags),(0xb7482c+tile*4,0x123456),
                (0xd3b5c0,0x1234abcd),(0xf56d14,0x234567),(0xf56d1c,0x345678),
                (0xf56bdc,0x456789),(0xf56b8c,0x56789a),(0xe94e0c,mode),
                (0x70e440,256),(0x70e444,270),(0x70e448,284),(0x70e44c,298),
                (0x70e450,310),(0x70e454,326),(0x70e458,342),(0x70e45c,358)]:put(u,a,v)
        word(u,0xb4e6fc+tile*2,building);byte(u,0xb285cc+tile,foot)
        byte(u,0x7999dd+record,status);byte(u,0x799a4f+record,second)
        word(u,0x7999e8+record,17);word(u,0x799a22+record,1)
        byte(u,0xacf85c+tile,0xdd)
        if parent:
            # Exactly one eligible viewport tile, all other slots are skipped.
            u.mem_write(0x70e494,bytes(231*457*4))
            u.mem_write(0xb67d1c,bytes(51984));byte(u,0xb67d1c+tile,0x40)
            put(u,0x70e494+(4*231+4)*4,tile)
            for a,v in [(0x70e474,12),(0x70e478,8),(0x70e460,-13),(0x70e464,-6),
                    (0x70e490,1000),(0x70e48c,4000),(0x6059d0,0xffffffff)]:put(u,a,v)
            # Set the actual starting cell to our populated row/column.
            put(u,0x70e494+(4*231+4)*4,tile)
            byte(u,0x6de515,parent==2)
            put(u,0xe94e0c,45 if parent==1 else 0)
        calls=[]
        def observe(cpu,args,t):
            calls.append((t,args,get(cpu,0xd3b5c0)))
            if t==0x431690:
                if mutate:
                    # The water renderer must reload the selected cell after
                    # the building filter rather than use its initial index.
                    put(cpu,0x70e488,tile+1);byte(cpu,0xb285cc+tile+1,2)
                    put(cpu,0xb7482c+(tile+1)*4,0x765432)
                value=0x12340001 if mode else 0x12340000
            else:
                value=0
                if mutate:
                    put(cpu,0xd3b5c0,get(cpu,0xd3b5c0)+17)
                    for a in [0x70e440,0x70e44c,0x70e450,0x70e45c]:put(cpu,a,get(cpu,a)+19)
                    put(cpu,0xf56d14,get(cpu,0xf56d14)+23)
            cpu.reg_write(UC_X86_REG_ECX,0xdeadbeef);cpu.reg_write(UC_X86_REG_EDX,0xbaadf00d)
            return value
        endpoints=[(0x53e590,3),(0x53f250,3),(0x432d30,4)]
        if not real_selector:endpoints.append((0x431690,1))
        for t,n in endpoints:
            hook(c,state,t,n,lambda cpu,args,t=t:observe(cpu,args,t),cleanup=12 if not c and t in [0x53e590,0x53f250] else 0)
        args=[450,600,flags,transparent] if token==0x4321a0 else [450,600]
        run(c,u,0x4316f0 if parent else token,() if parent else args)
        outputs.append((calls,get(u,0xd3b5c0),get(u,0x70e488)))
    assert outputs[0]==outputs[1],('footprint',hex(token),tile,hex(flags),foot,status,second,mode,transparent,mutate,real_selector,parent,outputs)
    if parent:assert calls,('viewport fixture did not draw',parent)


if __name__=='__main__':
    count=0
    for kind,result,cash,special in itertools.product([7,90,76,181],[0,1,999],[-5001,-5000,-4999,123456],[False,True]):
        shoreline(kind,result,cash,special);count+=1
    for kind,result,special in itertools.product([7,90,76,181],[0,1],[False,True]):
        shoreline(kind,result,0,special,True,True);count+=1
    print('PASS whole shoreline preview and actual build-mode dispatcher:',count,flush=True)
    count=0
    for f,flags,status,second in itertools.product([0,1,2,3,4,5,6,15],
            [0,0x80,0x2000,0x2080,0x40000000,0x40002000,8,0x88,0x2008,0x4000,1,9,0x20000002],
            [0,1],[0,1]):
        footprint(0x431ce0,48017,flags,f,status,second,1);count+=1
    for f,flags,status,second,alpha in itertools.product([0,1,2,3,4,5,15],
            [0,0x80,0x2000,0x2080],[0,1],[0,1],[0,1]):
        footprint(0x4321a0,51983,flags,f,status,second,transparent=alpha);count+=1
    for f,flags in itertools.product(range(16),[0,0x20000002,0x20000000,2]):
        footprint(0x432620,48017,flags,f,0,0);count+=1
    for token,flags,f in [(0x431ce0,8,3),(0x431ce0,0x40000000,4),
            (0x4321a0,0x80,5),(0x432620,0x20000002,2)]:
        footprint(token,48017,flags,f,1,0,1,mutate=True);count+=1
    for mode,f in itertools.product([0,8,45,51],[0,2,5,15]):
        footprint(0x431ce0,48017,8,f,0,0,mode,real_selector=True);count+=1
    for parent,flags,f in itertools.product([1,2],[0,0x20000002,0x40000000],[0,1,2,5]):
        footprint(0x431ce0,48017,flags,f,0,0,45,real_selector=True,parent=parent);count+=1
    print('PASS whole water/flatten footprints, signed building ids, callback reloads and actual viewport composition:',count,flush=True)
    print('Verified target SHA256:',target_sha)
