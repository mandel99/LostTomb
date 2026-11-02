"""Whole native/C Sphinx ghost: three footprint scans and painter orders.

Projection and sprite endpoints are controlled. Bounds queries may be real
native/C geometry or controlled callbacks that stress cached inputs/reloads.
No host execution, save/asset writes or physical framebuffer claim.
"""
from verify_options_startup import *


def exercise(rotation=0,obstacle=0,group=0,force=0,cash=0,count=36,
             wide=False,mutate='',real_bounds=False,parent=False,base=30000):
    outputs=[]
    for c,u,state in machines:
        reset(u,state)
        for a,v in [(0xbca304,411),(0xbca308,301),(0xbca300,base),(0x6059e8,210),
                (0x70e44c,271 if wide else 15),(0x70e448,285 if wide else 30),
                (0x70e458,569 if wide else 62),(0x70e45c,283 if wide else 31),
                (0xbca2e0,rotation),(0xeaac28,2),(0xea187c+2*0x49d8,cash),
                (0x5cfcec,count),(0x70e120,0xabcd5555),(0xf56d54,100000),
                (0xf56d5c,200000),(0xf56d64,300000),(0xf56be4,400000),
                (0x605994,0),(0xe38dcc,0),(0xbca2f8,114),(0xbca2fc,114),
                (0xbca2e8,0),(0xbca2f4,227),(0xbca2f0,227)]:put(u,a,v)
        word(u,0xea5baa+2*0x49d8,force)
        if wide:
            for i in range(16):put(u,0x7756ac+i*4,257+i if i%2 else -257-i)
        # Initialize all orientations so a callback can change the next group.
        for rot in [0,1,2,3]:
            for b in [base,base+get(u,0x5cdfa8+rot*4),base+get(u,0x5cdfb8+rot*4)]:
                for i in range(36):
                    tile=(b+get(u,0x5cfe10+rot*144+i*4))&0xffffffff
                    put(u,0xaf598c+tile*4,0)
        tile=(base+(get(u,(0x5cdfa8 if group==1 else 0x5cdfb8)+(rotation//2)*4) if group else 0)+
              get(u,0x5cfe10+(rotation//2)*144))&0xffffffff
        if obstacle==1:put(u,0xaf598c+tile*4,1)
        calls=[];queries=0;draws=0
        def observe(cpu,args,t):
            nonlocal queries,draws
            value=0;record=args
            if t==0x4f0ba0:
                put(cpu,args[2],600);put(cpu,args[3],450);record=args[:2]
            elif t==0x46f9a0:
                queries+=1;cell=(args[0]+args[1]*228)&0xffffffff
                value=0x12340001 if obstacle==2 and cell==tile else 0x12340000
                if mutate=='query' and queries==1:
                    put(cpu,0xbca2e0,6);put(cpu,0x5cfcec,2)
                    for a in [0x70e44c,0x70e448,0x70e458,0x70e45c]:put(cpu,a,get(cpu,a)+13)
                elif mutate=='flags':
                    put(cpu,0xaf598c+cell*4,get(cpu,0xaf598c+cell*4)^1)
            else:
                draws+=1;record=(*args,get(cpu,0x70e120))
                if mutate in ['draw','query']:
                    for a in [0x70e44c,0x70e448,0x70e458,0x70e45c]:put(cpu,a,get(cpu,a)+13)
                    for a in [0xf56d54,0xf56d5c,0xf56d64,0xf56be4]:put(cpu,a,get(cpu,a)+17)
                    put(cpu,0xbca2e0,6)
            calls.append((t,record))
            cpu.reg_write(UC_X86_REG_ECX,0xdeadbeef);cpu.reg_write(UC_X86_REG_EDX,0xbaadf00d)
            return value
        endpoints=[(0x4f0ba0,4),(0x53e590,3),(0x53f250,3),(0x53f4f0,4)]
        if not real_bounds:endpoints.append((0x46f9a0,3))
        for t,n in endpoints:
            hook(c,state,t,n,lambda cpu,args,t=t:observe(cpu,args,t),cleanup=(16 if t==0x53f4f0 else 12 if t in [0x53e590,0x53f250] else 0) if not c else 0)
        run(c,u,0x43b630 if parent else 0x441490)
        outputs.append((calls,get(u,0x70e120)))
    assert outputs[0]==outputs[1],('Sphinx',rotation,obstacle,group,force,cash,count,wide,mutate,real_bounds,parent,base,
        next(((i,a,b) for i,(a,b) in enumerate(zip(outputs[0][0],outputs[1][0])) if a!=b),None),outputs[0][1],outputs[1][1])


if __name__=='__main__':
    count=0
    for rotation,wide,obstacle,group in itertools.product([0,2,4,6],[False,True],[0,1,2],[0,1,2]):
        exercise(rotation,obstacle,group,wide=wide,parent=True);count+=1
    for rotation,force,cash in itertools.product([0,2,4,6],[0,1,256],[-5001,-5000,-4999,123456]):
        exercise(rotation,force=force,cash=cash);count+=1
    for rotation,mutate,obstacle,group in itertools.product([0,2,4,6],['draw','query','flags'],[0,1,2],[0,1,2]):
        exercise(rotation,obstacle,group,mutate=mutate);count+=1
    for n in [-1,0,1,2,35,36]:
        for rotation in [0,2,4,6]:exercise(rotation,count=n);count+=1
    print('PASS whole Sphinx ghost and build dispatcher: three groups, signed coefficients, counts, flags and callback reloads:',count,flush=True)
    count=0
    for rotation,base in itertools.product([0,2,4,6],[30000,25992]):
        exercise(rotation,real_bounds=True,base=base,parent=True);count+=1
    print('PASS Sphinx with actual46F9A0/46F930 geometry queries:',count,flush=True)
    print('Verified target SHA256:',target_sha)
