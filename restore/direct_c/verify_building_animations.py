"""Complete native/C image-animation family438210..438770.

Only the final sprite endpoint is controlled. Selectors, actor records,
owned offset tables, image limits and metadata reads all execute.
"""
from verify_building_overlays import *

BASES={0xf56c0c:14000,0xf56bcc:15000,0xf56f3c:19000,0xf56f44:19016,
       0xf56f4c:19032,0xf566fc:18000,0xf56e04:19048,0xf56e0c:19064,
       0xf56df4:19080,0xf56dec:19096}
GROUPS={622:20000,629:20032,631:20064,632:20096,623:20128}


def exercise(token,args,limit=25500,actor=0,kind=77,alive=1,action=8):
    results=[]
    for c,u,state in machines:
        reset(u,state)
        for at,value in BASES.items():put(u,at,value)
        for group,image in GROUPS.items():put(u,0xf55bb4+group*8,image)
        put(u,0x126517c,limit)
        word(u,0x10d6a1a,31);word(u,0x10d6a1c,-12)
        for image in [*range(19000,19120),*range(20000,20164)]:
            word(u,0x10d6a1a+image*64,image%17-8)
            word(u,0x10d6a1c+image*64,image%23-11)
        byte(u,0xbcd602+actor*388,kind);byte(u,0xbcd606+actor*388,alive)
        byte(u,0xbcd630+actor*388,action)
        draws=[];sprite_spy(c,state,draws)
        value=run(c,u,token,args)
        results.append((None if token==0x438210 else value,draws,snapshot(u)))
    assert results[0]==results[1],('animation',hex(token),args,limit,actor,kind,alive,action,
                                 results[0][:2],results[1][:2])
    if token!=0x438210:assert results[1][0]==0x12345678


def verify():
    count=0
    for token,frame,limit in itertools.product(
            [0x438470,0x4384d0,0x438590,0x438650,0x4386b0,0x438710,0x438770],
            [1,5],[128,25500]):
        for selector in ([18000,18001] if token==0x438590 else [18000]):
            exercise(token,[selector,frame,80,100],limit);count+=1
    print('PASS seven complete simple/dual-layer/selector animations:',count,flush=True)
    count=0
    for tag,variant,frame,limit in itertools.product(
            [0,0x1019,0x104d,0x104e,0x3019],range(4),[1,5],[128,25500]):
        exercise(0x438210,[14000+variant,frame,80,100,tag],limit);count+=1
    for actor,kind,(alive,action) in itertools.product([1,511,1999],[25,77,78],[(0,8),(1,4),(1,8)]):
        exercise(0x438210,[14000+count%4,5,80,100,0x2000|actor],
                 actor=actor,kind=kind,alive=alive,action=action);count+=1
    print('PASS shipwright tag/actor selection, dead/combat gates and four orientations:',count,flush=True)
    count=0
    for (actor,action),variant,frame,limit in itertools.product(
            [(0,8),(1,8),(1,12),(511,12),(1999,12)],range(4),[1,5],[128,25500]):
        exercise(0x438350,[15000+variant,frame,80,100,actor],limit,actor=actor,action=action);count+=1
    print('PASS fishing-wharf animation states and both four-pair offset blocks:',count)
    print('Verified target SHA256:',target_sha)


if __name__=='__main__':verify()
