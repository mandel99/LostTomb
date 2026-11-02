"""Whole naval controller bodies against the pinned original, without a game.

Actual water/land geometry, building ranking/linkage, formation readiness,
boarding/unlink and landing/reinsert execute. Only movement, route reset,
formation publication and the death-animation endpoint are controlled.
No final rendering or live-game stability claim.
"""
from verify_actor_targets import *


def base(u,ident=511,direction=8):
    u.mem_write(0x7999d8,bytes(4000*264))
    u.mem_write(0xbcb9c8,bytes(50*144))
    tile=actor(u,ident,direction=direction)
    put(u,0xbcd618+ident*388,tile);word(u,0xb350dc+tile*2,ident)
    put(u,0xd056f8,ident);put(u,0xd3b4e4,0x12345);put(u,0xd3b4e0,-0x12345)
    put(u,0x7090ec+77*56,256)
    return tile


def observe(c,state,token,count,calls,fn=None):
    def apply(uc,args):
        calls.append((token,args))
        return fn(uc,args) if fn else 0
    control(c,state,token,count,apply)
    # Some already-reviewed typed providers call this canonical endpoint
    # directly instead of crossing the civic frame boundary.
    if c and re.search(r'\s_FUN_%08x\s'%token,symbols):hook(c,state,token,count,apply)


def check(token,args,setup,death=False,query=False):
    results=[]
    for c,u,state in machines:
        prepare(u,state);setup(u);calls=[]
        for endpoint,n in [(0x4a5da0,2),(0x4b3250,1),(0x4b6400,4),(0x4b7c80,0)]:
            observe(c,state,endpoint,n,calls)
        if death:observe(c,state,0x4b1d50,1,calls)
        value=execute(c,u,token,args)
        results.append((value if query else None,calls,bytes(u.mem_read(S+0x400,0x110)),snapshot(u)))
    assert results[0]==results[1],(hex(token),args,results[0][:2],results[1][:2],
        [(hex(0x560000+i),a,b) for i,(a,b) in enumerate(zip(results[0][-1],results[1][-1])) if a!=b][:12])
    return results[1]


def building(u,ident,x=50,y=50,index=1,rank=0):
    byte(u,0x7999d8+ident*264,1);byte(u,0x7999db+ident*264,2)
    word(u,0x7999de+ident*264,x);word(u,0x7999e0+ident*264,y)
    word(u,0x7999e8+ident*264,74);word(u,0x799a18+ident*264,1)
    byte(u,0x799aa3+ident*264,index);put(u,0x9783b0+index*28,rank)


def formation(u,group,ident,ready=True,aboard=False,length=2):
    byte(u,0xbcd68a+ident*388,group);byte(u,0xbcd728+ident*388,aboard)
    byte(u,0xbcb9cb+group*144,not aboard);byte(u,0xbcb9f0+group*144,length)
    tile=get(u,0xbca2d8)+50+50*228
    for i in range(length):
        member=2+i;actor(u,member,92,direction=8 if ready else 7)
        byte(u,0xbcd5f8+member*388,not aboard)
        byte(u,0xbcd608+member*388,10);byte(u,0xbcd630+member*388,18 if aboard else 10)
        put(u,0xbcd618+member*388,tile)
        word(u,0xbcb9d0+group*144+i*2,member)
        if not aboard:word(u,0xbcd600+member*388,member+1 if i+1<length else ident)
    if length and not aboard:word(u,0xb350dc+tile*2,2)


def verify_departure():
    count=0
    for ident,owner,timer in itertools.product([1,511,1999],[0,1,400,3999],[-1,48,49,50,32767]):
        def setup(u):
            base(u,ident);word(u,0xbcd62e+ident*388,timer)
            byte(u,0xbcd5f9+ident*388,0xa5)
            if owner:building(u,owner)
        check(0x4a4770,[ident,owner],setup);count+=1
    print('PASS departure WORD threshold/wrap, full owner IDs and genuine dock geometry:',count,flush=True)


def verify_arrival_and_assignment():
    count=0
    for ident,owner,direction,action,linked in itertools.product([1,511],[0,3999],[2,8,9,10],[10,11,20],[False,True]):
        def setup(u):
            base(u,ident,direction);byte(u,0xbcd630+ident*388,action)
            word(u,0xbcd652+ident*388,owner)
            if direction==8:word(u,0xbcd61c+ident*388,50)
            if owner:
                building(u,owner);word(u,0x799a8c+owner*264,ident if linked else 0)
        check(0x4a4950,[ident,S+0x400],setup);count+=1
    print('PASS full arrival state/link exclusions, genuine dock claim/release:',count,flush=True)

    count=0
    for ident,direction,layout in itertools.product([1,511,1999],[2,8,9,10],['none','same','better']):
        def setup(u):
            base(u,ident,direction);word(u,0xbcd656+ident*388,400)
            if layout!='none':building(u,400,index=1,rank=1)
            if layout=='better':building(u,3999,index=2,rank=0)
        check(0x4a4c40,[ident,S+0x400],setup);count+=1
    print('PASS assignment with genuine rank/nearest-building/reassignment providers:',count,flush=True)


def verify_boarding_and_landing():
    count=0
    for ident,group,direction,ready,busy in itertools.product([1,511],[0,1,49],[2,8,9,10],[False,True],[False,True]):
        def setup(u):
            base(u,ident,direction)
            if group:formation(u,group,ident,ready=ready)
            put(u,0xbcba4c+group*144,0x1234 if busy else 0)
            word(u,0xbcd62e+ident*388,0x3456)
        check(0x4a4d00,[ident,S+0x400],setup);count+=1
    print('PASS boarding with genuine readiness and member unlink,144-byte formations:',count,flush=True)

    count=0
    for ident,group,direction,point in itertools.product([1,511],[1,49],[2,8,9,10],[(0,0),(56,50),(50,56)]):
        def setup(u):
            base(u,ident,direction);formation(u,group,ident,aboard=True)
            put(u,0xbcba4c+group*144,ident)
            word(u,0xbcd628+ident*388,point[0]);word(u,0xbcd62a+ident*388,point[1])
            u.mem_write(0xee0c50,b'\xff'*(228*228))
            for x,y in [(56,50),(50,56)]:
                tile=get(u,0xbca2d8)+x+y*228;byte(u,0xee0c50+tile,0)
                word(u,0xb4e6fc+tile*2,0)
        check(0x4a4e10,[ident,S+0x400],setup);count+=1
    print('PASS landing with genuine land projection, point selection and member reinsert:',count,flush=True)


def verify_teardown():
    count=0
    for ident,group,timer,carrying,length in itertools.product([1,511],[1,49],[0,2,256],[False,True],[0,1,16]):
        def setup(u):
            base(u,ident);formation(u,group,ident,length=length,aboard=carrying)
            word(u,0xbcd62e+ident*388,timer);put(u,0xbcba4c+group*144,ident)
        check(0x4a46f0,[ident],setup,death=True);count+=1
    print('PASS teardown exact WORD gate, carried member limits and full formation owner:',count,flush=True)


def verify_destination():
    count=0
    offsets=[(0,0,0,0,0,0),(0,-10,-10,0,10,0),(-10,-10,10,10,0,0),
             (10,0,-10,0,0,10),(0,10,0,-10,0,0)]
    for ident,offset,alias in itertools.product([1,511,1999],offsets,[False,True]):
        def setup(u):base(u,ident)
        check(0x4a4850,[S+0x500,S+0x500 if alias else S+0x504,*offset,ident],setup);count+=1
    print('PASS complete three-candidate water destination, ties and aliased outputs:',count,flush=True)


def verify_member_providers():
    count=0
    for group,length,ready,dead in itertools.product([0,1,49],[0,1,16],[False,True],[False,True]):
        def setup(u):
            base(u);formation(u,group,511,length=length,ready=ready)
            if dead and length:byte(u,0xbcd630+2*388,4)
        check(0x4a43f0,[511],setup,query=True);count+=1
    print('PASS canonical readiness query, empty lists, member directions and death gate:',count,flush=True)

    count=0
    for token,group,length,dead in itertools.product([0x4a4470,0x4a44f0],[1,49],[0,1,16],[False,True]):
        def setup(u):
            base(u);formation(u,group,511,length=length,aboard=token==0x4a44f0)
            if dead and length:byte(u,0xbcd630+2*388,4)
        check(token,[group] if token==0x4a4470 else [group,56,50],setup);count+=1
    print('PASS canonical member hide/show with genuine unlink/insert and dead-member exclusions:',count,flush=True)


if __name__=='__main__':
    verify_departure();verify_arrival_and_assignment();verify_boarding_and_landing()
    verify_teardown();verify_destination();verify_member_providers()
    print('Verified target SHA256:',target_sha)
