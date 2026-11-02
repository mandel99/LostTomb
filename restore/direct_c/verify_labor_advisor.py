"""Whole labor background, category foreground and priority popup, offline.

Genuine temple classifiers and wage bonus execute. Only panel/text/sprite
output is controlled; this does not certify final pixels or every labor action.
"""
from verify_options_startup import *
from verify_kingdom_advisor import ENDS

ENDS={**ENDS,0x4d6250:9,0x4cf090:5}
TOKENS=[0x521080,0x5215d0,0x521800,0x5219c0]


def prepare(u,state,player=0,phase=6,origin=(300,200),field=500,bonus=False,
            hovered=2,selection=3,minimum=4,popup=0):
    reset(u,state)
    for at,value in [(0xeaac28,player),(0xf53f48,phase),(0xe38da8,origin[0]),
                     (0xe38da4,origin[1]),(0xf53f0c,origin[0]),(0xf53f08,origin[1]),
                     (0xf5681c,20000),(0xf5670c,21000),(0xf56fb4,16416),
                     (0xf56f8c,16448),(0xf56f9c,16480),(0xf56fa4,16512),
                     (0x605988,hovered),(0x7056c8,selection),(0x605984,minimum)]:put(u,at,value)
    byte(u,0xe38dda,popup)
    for owner in [0,3]:
        off=owner*0x49d8
        word(u,0xea5ce8+off,256 if bonus else 0);put(u,0xea5f9c+off,511)
        for at in [0xea4464,0xea4468,0xea446c,0xea4478,0xea447c,0xea5f10]:put(u,at+off,field+owner)
        for i in range(9):
            needed=field+i*100
            put(u,0xea439c+off+i*20,needed)
            put(u,0xea43a0+off+i*20,needed if i&1 else needed-1)
            put(u,0xea43ac+off+i*20,0 if i%3 else i+1)
    word(u,0x7999e8+511*264,61);byte(u,0x799a81+511*264,2)


def outputs(c,state,calls,mutate=False):
    for token,n in ENDS.items():
        def draw(uc,args,token=token):
            calls.append((token,args))
            if token in [0x4d62f0,0x4d6250,0x4d6860,0x4ccc30]:put(uc,0xe20778,get(uc,0xe20778)+17)
            if mutate and token in [0x4d62f0,0x4d6250,0x4cd680]:
                put(uc,0xf53f0c,get(uc,0xf53f0c)+3);put(uc,0xf53f08,get(uc,0xf53f08)+2)
                put(uc,0xeaac28,3);put(uc,0x7056c8,get(uc,0x7056c8)+1)
            return 0
        hook(c,state,token,n,draw,cleanup=12 if not c and token==0x53e590 else 0)


def compare(token,mutate=False,**kw):
    states=[]
    for c,u,state in machines:
        prepare(u,state,**kw);calls=[];outputs(c,state,calls,mutate)
        run(c,u,token);states.append((calls,snapshot(u)))
    assert states[0]==states[1],(hex(token),kw,mutate,states[0][0][:18],states[1][0][:18],
        [(hex(0x560000+i),a,b) for i,(a,b) in enumerate(zip(states[0][-1],states[1][-1])) if a!=b][:12])
    return states[1][0]


def verify_bonus():
    count=0
    for player,flag,kind,annex in itertools.product([0,3],[0,1,256,65535],
                                                  [60,61,62,65,66,202],[0,2,3]):
        states=[]
        for c,u,state in machines:
            prepare(u,state,player=player)
            word(u,0xea5ce8+player*0x49d8,flag)
            word(u,0x7999e8+511*264,kind);byte(u,0x799a81+511*264,annex)
            result=run(c,u,0x4c38f0);states.append((result,snapshot(u)))
        assert states[0]==states[1],('bonus',player,flag,kind,annex)
        assert states[1][0]==(0xfffffffe if flag and kind in [61,66] and annex in [2,3] else 0)
        count+=1
    print('PASS genuine wage bonus, WORD existence, player stride, all classification/annex gates:',count,flush=True)
    for first,second in itertools.product([0,1,257],[0,1,256]):
        states=[]
        for c,u,state in machines:
            prepare(u,state,bonus=True);put(u,0xea5f9c+3*0x49d8,512);calls=[]
            def kind(uc,args):
                calls.append((0x4c5120,args));put(uc,0xeaac28,3);return first
            def annex(uc,args):calls.append((0x4c52d0,args));return second
            hook(c,state,0x4c5120,1,kind);hook(c,state,0x4c52d0,2,annex)
            result=run(c,u,0x4c38f0);states.append((result,calls,snapshot(u)))
        assert states[0]==states[1],('query protocol',first,second,states[0][:2],states[1][:2])
        if first==1:assert calls==[(0x4c5120,(511,)),(0x4c52d0,(512,2))]
    print('PASS full first query, AL second query and actual player/building reload protocol:',9,flush=True)


def verify_panels():
    count=0
    for phase,origin,player in itertools.product([-1,0,1,4,5,6,256],
            [(0,0),(192,144),(300,200)],[0,3]):
        compare(0x521080,phase=phase,origin=origin,player=player,bonus=True);count+=1
    for field,bonus in itertools.product([-2147483648,-1,0,255,500,2147483647],[False,True]):
        compare(0x521080,field=field,bonus=bonus,player=3);count+=1
    print('PASS complete labor background, animation, signed amounts/fonts and real temple bonus:',count,flush=True)
    count=0
    for phase,hovered,selection,popup in itertools.product([5,6,256],[-1,0,8,256],
                                                           [1,2,10,256],[0,1,2]):
        calls=compare(0x5215d0,phase=phase,hovered=hovered,selection=selection,popup=popup,player=3)
        if phase>5:
            assert sum(token==0x4d63d0 for token,args in calls)==9
            assert {args[5] for token,args in calls if token==0x4d6860}=={16480,16512}
        count+=1
    print('PASS all nine category rows, full selections, distinct unmet font and phase gates:',count,flush=True)
    count=0
    for minimum,selection in itertools.product([-1,0,1,8,9,256],[0,1,2,10,256]):
        compare(0x521800,minimum=minimum,selection=selection)
        calls=compare(0x5219c0,minimum=minimum,selection=selection)
        assert len(calls)==10;count+=2
    print('PASS full priority-popup thresholds, nine slots/reset row and selection overlays:',count,flush=True)
    for token in TOKENS:compare(token,mutate=True,bonus=True)
    print('PASS live text cursor, origin/player/selection changes across drawing callbacks:',4,flush=True)
    print('Verified target SHA256:',target_sha)


if __name__=='__main__':verify_bonus();verify_panels()
