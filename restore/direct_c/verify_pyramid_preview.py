"""Whole native/C pyramid preview, real layout tables and both record families.

Prechecks, projection, bounds and final drawing are controlled boundaries.
Coordinates, masks, cached state and callback-driven layout changes execute.
"""
from verify_options_startup import *


def exercise(mode,tool,rotation,allowed=True,obstacle=0,debt=False,mutate='',tiles=4,parent=False):
    outputs=[]
    for c,u,state in machines:
        reset(u,state);u.mem_write(0xaf598c,bytes(51984*4))
        for a,v in [(0xbca304,411),(0xbca308,301),(0xbca300,30000),(0xbca2e0,rotation),
                (0x5e4ef4,mode),(0x5e6970,tool),(0x6059e8,183),(0x5cfcdc,tiles),
                (0x70e458,569),(0x70e448,285),(0xeaac28,2),(0xea187c+2*0x49d8,-5000 if debt else 100000),
                (0xbca2d8,13282),(0xbca2e8,300),(0xbca2ec,260),
                (0xf56be4,0x123456),(0xbca300,30000),(0xe38dcc,0),(0x605994,0),(0x605a3c,0)]:put(u,a,v)
        # A terrain obstacle in the first piece plus the separate trailing row.
        if obstacle==1:
            at=((tool*4+rotation//2)*(36 if mode==5 else 144))*16
            index=get(u,(0x5f1d7c if mode==5 else 0x5e697c)+at)
            delta=(get(u,0x56fd10+index*8)+get(u,0x56fd14+index*8)*228) if mode==5 else get(u,0x56fe30+index*4)
            tile=(30000+delta+get(u,0x5cfe10+(rotation//2)*144))&0xffffffff
            assert 0<=tile<51984;put(u,0xaf598c+tile*4,4)
            start=get(u,(0x5700c8 if mode==5 else 0x5700b4)+tool*4)
            put(u,0xaf598c+(30000+start)*4,4)
        calls=[];queries=0;draws=0;projections=0
        def observe(cpu,args,t):
            nonlocal queries,draws,projections
            record=args;value=0
            if t==0x4ecec0:
                put(cpu,args[2],0x12345678);record=args[:2];value=0x12340001 if allowed else 0x12340000
                if mutate=='precheck':put(cpu,0x5e4ef4,5);put(cpu,0x5e6970,1);put(cpu,0xbca2e0,6)
            elif t==0x4f0ba0:
                projections+=1;record=args[:2]
                put(cpu,args[2],args[0]*7-args[1]*3+600);put(cpu,args[3],args[0]+args[1]*2+450)
                if mutate=='projection' and projections==1:
                    put(cpu,0xbca2e0,6);put(cpu,0xbca300,31000)
            elif t==0x46f9a0:
                queries+=1;value=0x12340001 if obstacle==2 else 0x12340000
                if mutate=='query' and queries==1:
                    put(cpu,0x5e4ef4,5);put(cpu,0x5e6970,1);put(cpu,0x5cfcdc,1)
                    put(cpu,0xbca300,31000);put(cpu,0x70e458,584);put(cpu,0x70e448,298)
            elif t==0x4ecea0:value=0x12340001 if allowed else 0x12340000
            elif t==0x4ed360:
                if mutate=='tail':put(cpu,0x5e4ef4,5);put(cpu,0x5e6970,0);put(cpu,0xbca300,31000)
            elif t==0x53f4f0:
                draws+=1
                if mutate=='draw' and draws==1:put(cpu,0x5cfcdc,0);put(cpu,0xbca300,31000)
            calls.append((t,record))
            cpu.reg_write(UC_X86_REG_ECX,0xdeadbeef);cpu.reg_write(UC_X86_REG_EDX,0xbaadf00d)
            return value
        for t,n in [(0x4ecec0,3),(0x4f0ba0,4),(0x46f9a0,3),(0x4ecea0,1),(0x4ed360,4),(0x53f4f0,4)]:
            hook(c,state,t,n,lambda cpu,args,t=t:observe(cpu,args,t),cleanup=16 if not c and t==0x53f4f0 else 0)
        run(c,u,0x43b630 if parent else 0x4edbb0,budget=5000000)
        outputs.append((calls,tuple(get(u,a) for a in [0x5e4ef4,0x5e6970,0xbca300,0xbca2e0,0x5cfcdc])))
    assert outputs[0]==outputs[1],('pyramid',mode,tool,rotation,allowed,obstacle,debt,mutate,tiles,parent,
        next(((i,a,b) for i,(a,b) in enumerate(zip(outputs[0][0],outputs[1][0])) if a!=b),None),outputs[0][1],outputs[1][1])


if __name__=='__main__':
    count=0
    for mode in [0,5]:
        for tool,rotation,allowed,obstacle in itertools.product(range(3 if mode==5 else 5),[0,2,4,6],[False,True],[0,1,2]):
            exercise(mode,tool,rotation,allowed,obstacle,parent=True);count+=1
    for mode,rotation,mutate in itertools.product([0,5],[0,2,4,6],['precheck','projection','query','draw','tail']):
        exercise(mode,0,rotation,mutate=mutate);count+=1
    for mode,tiles in itertools.product([0,5],[-1,0,1,2,4]):
        exercise(mode,1,4,debt=True,tiles=tiles);count+=1
    print('PASS whole pyramid preview and actual build dispatcher: both real layout families, every size/rotation, terrain masks, lowAL prechecks, debt and callbacks:',count,flush=True)
    print('Verified target SHA256:',target_sha)
