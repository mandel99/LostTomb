"""Whole weighted single/multiple-goal search against original instructions.

Only ordinary fill and optional actor-gate endpoints are controlled. Compare
all mapped state, watched stores, callback cursors, full results and native ABI.
"""
from verify_actor_land_paths import *

WATCH.add(0xeadfd8)


def checked(multiple=False,owner=0,limit=100,direction=1,classification=None,
            building=0,gate=None,goals=None,count=None,list_address=S,
            prepare=None,clear=True):
    goal=2290+DELTA[direction]
    if goals is None:goals=(goal,)
    if count is None:count=len(goals)
    def grid(c,u,state,calls):
        put(u,0xeadfd8,0xffffffff)
        if classification is None:
            for y in range(8,15):
                for x in range(8,15):byte(u,0xec7630+y*228+x,(x*3+y*5)%5)
        else:
            byte(u,0xec7630+goal,classification)
        word(u,0xb4e6fc+goal*2,building)
        for i,tile in enumerate(goals):put(u,list_address+i*4,tile)
        if gate is not None:
            def allowed(cpu,values):
                calls.append((0x51afa0,values,get(cpu,0xeae008),get(cpu,0xeae00c)))
                return gate
            hook(c,state,0x51afa0,1,allowed)
        if prepare:prepare(c,u,state,calls)
    token=0x51a9d0 if multiple else 0x51a420
    args=[10,10,list_address,count,owner,limit] if multiple else [10,10,goal%228,goal//228,owner,limit]
    return compare(token,args,grid,control_clear=clear,gate=None)


def verify_classes():
    count=0
    for multiple,owner,classification,direction,building in itertools.product(
            [False,True],[0,7],[0,1,2,3,4,5,127,128,255],range(4),[0,7]):
        result=checked(multiple,owner,direction=direction,classification=classification,building=building)
        if not owner and classification in [0,1,2,3,4]:
            goal=2290+DELTA[direction]
            distance=struct.unpack_from('<I',result[1],0xeed760+goal*4-0x560000)[0]
            assert distance=={0:2,1:12,2:2,3:42,4:2}[classification]
        count+=1
    for multiple,owner,building in itertools.product([False,True],[-1,65543],[-1,7]):
        checked(multiple,owner,classification=1,building=building);count+=1
    for multiple,gate in itertools.product([False,True],[0,1,256]):
        checked(multiple,classification=0,gate=gate);count+=1
    print('PASS weighted costs, owner FIFO rules, signed classes/WORD owner comparisons, four neighbors and full gate results:',count,flush=True)


def verify_limits_and_goals():
    count=0
    for multiple,owner,limit in itertools.product([False,True],[0,7],[-2147483648,-1,0,1,2,3,7,100,2147483647]):
        checked(multiple,owner,limit=limit);count+=1
    for owner,limit in itertools.product([0,7],[-1,0,100]):
        result=checked(True,owner,limit,goals=(2519,2290,2290))
        assert result[0][0]==3;count+=1
    for multiple,limit in itertools.product([False,True],[-1,0]):
        result=checked(multiple,owner=7,limit=limit,classification=0)
        assert result[0][0]==1;count+=1
    for goals in [(),(2291,),(2518,2291),(2291,2518,2291),(2300,2518)]:
        for owner in [0,7]:checked(True,owner,goals=goals);count+=1
    for multiple in [False,True]:
        def alternatives(c,u,state,calls):
            byte(u,0xec7630+2291,3);byte(u,0xec7630+2062,0)
        result=checked(multiple,limit=1,classification=3,prepare=alternatives)
        assert result[0][0]==int(not multiple);count+=1
    def negative(c,u,state,calls):put(u,S,2290)
    result=checked(True,count=-1,goals=(),list_address=S+8,prepare=negative)
    assert result[0][0]==0;count+=1
    print('PASS signed limits, owner-mode limit bypass, reverse/duplicate/empty/live goal lists and discovered-versus-selected success:',count,flush=True)


def verify_frontier_selection():
    count=0
    scenarios=[
        (51982,2,[(51983,2300,9),(0,2301,8),(1,2302,2)],2302),
        (51980,1,[(51981,2300,9),(51982,2301,1),(51983,2302,-1),(0,2303,-2)],2301),
        (51982,2,[(51983,2300,9),(0,2301,1),(1,2302,0)],2302),
        (0,4,[(1,2300,9),(2,2301,1),(3,2302,-1)],2301),
        (0,3,[(1,2300,0),(2,2301,-1)],2300)]
    for multiple,(head,tail,rows,target) in itertools.product([False,True],scenarios):
        def seed(c,u,state,calls):
            seen=[0]
            def gate(cpu,args):
                seen[0]+=1;calls.append((0x51afa0,args,seen[0]))
                if seen[0]==1:
                    put(cpu,0xeae00c,head);put(cpu,0xeae008,tail)
                    for index,tile,cost in rows:
                        put(cpu,0xf203a0+index*4,tile);put(cpu,0xeed760+tile*4,cost)
                return 0
            hook(c,state,0x51afa0,1,gate)
        if multiple:
            result=checked(True,classification=128,goals=(target,),prepare=seed)
            assert result[0][0]==1
        else:
            def grid(c,u,state,calls):
                put(u,0xeadfd8,0xffffffff);seed(c,u,state,calls)
            compare(0x51a420,[10,10,target%228,target//228,0,100],grid,control_clear=True,gate=None)
        count+=1
    def alias(c,u,state,calls):
        seen=[0]
        def gate(cpu,args):
            seen[0]+=1;calls.append((0x51afa0,args,seen[0]))
            if seen[0]==1:
                put(cpu,0xeae00c,51982);put(cpu,0xeae008,2)
                for index,tile,cost in [(51983,2300,9),(0,2301,8),(1,2302,2)]:
                    put(cpu,0xf203a0+index*4,tile);put(cpu,0xeed760+tile*4,cost)
            return 0
        hook(c,state,0x51afa0,1,gate)
    result=checked(True,limit=1,classification=128,goals=(),count=1,
                   list_address=0xf203a4,prepare=alias)
    assert result[0][0]==0;count+=1
    print('PASS contiguous/wrapped priority scans, early-minimum differences, cached minima and goal/queue alias ordering:',count,flush=True)


def verify_mutations():
    count=0
    for multiple,owner in itertools.product([False,True],[0,7]):
        def changed(c,u,state,calls):
            def gate(cpu,args):
                calls.append((0x51afa0,args));put(cpu,0xeed760+2290*4,99)
                put(cpu,0xeed760+args[0]*4,777)
                byte(cpu,0xec7630+args[0],1);word(cpu,0xb4e6fc+args[0]*2,owner)
                return 1
            hook(c,state,0x51afa0,1,gate)
        checked(multiple,owner,classification=128,prepare=changed);count+=1
    for multiple in [False,True]:
        checked(multiple,classification=0,clear=False);count+=1
    print('PASS cached expansion costs, no post-callback visited recheck, fresh classification/owner and actual fill:',count,flush=True)


if __name__=='__main__':
    verify_classes();verify_limits_and_goals();verify_frontier_selection();verify_mutations()
    print('Verified target SHA256:',target_sha,flush=True)
