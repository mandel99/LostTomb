"""Native/current C land search, occupancy and gradient route construction.

Ordinary searches control only the zero-fill and occupancy endpoints; ordinary
constructors control only geometric preference. Named compositions use actual
providers. No game process or save is started or modified.
"""
from verify_disease_updates import *

QUERIES=[0x517f20,0x5181c0,0x5186d0,0x518880,0x519390,0x5195e0,
         0x519840,0x519be0,0x51a030,0x51b040]
DELTA=[-228,1,228,-1]
WATCH.update([0xeadfd4,0xeae004,0xeae008,0xeae00c,0xf203a0])
WATCH.update(0xeed760+i*4 for i in [2290,2062,2291,2518,2289,2292,2519])


def setup(u,state):
    setup_disease(u,state,rows=(),ids=(0,))
    u.mem_write(0xbcd5f8,bytes(2000*388));u.mem_write(0xb350dc,bytes(51984*2))
    for at in [0xee0c50,0xebab20,0xec7630]:u.mem_write(at,b'\x80'*51984)
    u.mem_write(0xeed760,b'\xa5'*207936)
    put(u,0xbca2d8,0);put(u,0xeadfd4,0xffffffff)


def compare(token,args,prepare=None,control_clear=False,gate=1,preference=None):
    states=[]
    for c,u,state in machines:
        setup(u,state);calls=[]
        if control_clear:
            def clear(cpu,values):
                assert values==(0xeed760,207936,0)
                calls.append((0x40f8f0,values));cpu.mem_write(values[0],bytes(values[1]));return 0
            hook(c,state,0x40f8f0,3,clear)
        if gate is not None:
            def allowed(cpu,values):
                calls.append((0x51aff0,values,get(cpu,0xeae008),get(cpu,0xeae00c)))
                return gate
            hook(c,state,0x51aff0,1,allowed)
        if preference is not None:
            def geometry(cpu,values):calls.append((0x51cba0,values));return preference
            hook(c,state,0x51cba0,4,geometry)
        if prepare:prepare(c,u,state,calls)
        state['service_writes'].clear();value=run(c,u,token,args)
        states.append(((value,calls,list(state['service_writes'])),snapshot(u)))
    same(states,('land path',hex(token),args,gate,preference))
    return states[1]


def query(token=0x517f20,classification=4,direction=1,flags=0,building=0,gate=1,
          prepare=None,clear=True):
    goal=2290+DELTA[direction]
    def cells(c,u,state,calls):
        for base in [0xee0c50,0xebab20,0xec7630]:byte(u,base+goal,classification)
        put(u,0xaf598c+goal*4,flags);word(u,0xb4e6fc+goal*2,building)
        if prepare:prepare(c,u,state,calls)
    x,y=goal%228,goal//228
    return compare(token,[10,10,x,y],cells,control_clear=clear,gate=gate)


def verify_classes():
    count=0
    for token,classification,direction in itertools.product(QUERIES,
            [0,1,2,3,4,5,6,7,127,128,252,253,255],range(4)):
        query(token,classification,direction);count+=1
    for token,flags,building in itertools.product([0x517f20,0x518880,0x519be0],
                                                 [0,4,0x1000,0x10000,0x10004],[-1,0,1]):
        query(token,255,flags=flags,building=building);count+=1
    for token,gate in itertools.product(QUERIES,[0,256]):
        query(token,0,gate=gate);count+=1
    print('PASS all ten query rules, four neighbor positions, signed classes, bridge/building gates and full occupancy returns:',count,flush=True)


def verify_search_state():
    count=0
    for token in QUERIES:
        compare(token,[10,10,10,10],control_clear=True);count+=1
        for pattern in range(4):
            def cells(c,u,state,calls):
                for y in range(8,15):
                    for x in range(8,15):
                        tile=y*228+x
                        value=(x*3+y*5+pattern)%7
                        for base in [0xee0c50,0xebab20,0xec7630]:byte(u,base+tile,value)
            compare(token,[10,10,13,12],cells,control_clear=True);count+=1
    for token in [0x517f20,0x5181c0,0x518880,0x519390,0x5195e0]:
        for answer in [0,1]:
            def callback(c,u,state,calls):
                seen=[0]
                def changed(cpu,args):
                    seen[0]+=1;calls.append((0x51aff0,args,seen[0]))
                    if seen[0]==1:
                        put(cpu,0xeae008,51983);put(cpu,0xeae00c,51982)
                        put(cpu,0xf203a0+51983*4,2291)
                        put(cpu,0xeed760+2290*4,99)
                    return answer
                hook(c,state,0x51aff0,1,changed)
            query(token,0,prepare=callback);count+=1
    print('PASS start-equals-goal, bounded mixed-class maps, queue wrap, live cursors and cached frontier cost:',count,flush=True)


def verify_occupancy():
    count=0
    for token,head,flag13,action in itertools.product([0x51afa0,0x51aff0],[-1,0,1,1999],[0,1,255],[0,5,255]):
        def records(c,u,state,calls):
            word(u,0xb350dc+2290*2,head)
            if head>0:byte(u,a(head)+13,flag13);byte(u,a(head)+56,action)
        compare(token,[2290],records,gate=None);count+=1
    for token,position in itertools.product([0x51afa0,0x51aff0],[0,1,2,3]):
        def records(c,u,state,calls):
            ids=[1,255,1999];word(u,0xb350dc+2290*2,ids[0])
            for i,id in enumerate(ids):
                word(u,a(id)+8,ids[i+1] if i<2 else 0)
                byte(u,a(id)+13,int(token==0x51aff0));byte(u,a(id)+56,5 if i==position else 9)
        compare(token,[2290],records,gate=None);count+=1
    for flag13,action in itertools.product([0,1,255],[0,5,255]):
        def records(c,u,state,calls):
            word(u,0xb350dc+2290*2,255);word(u,a(255)+8,255)
            byte(u,a(255)+13,flag13);byte(u,a(255)+56,action)
        compare(0x51aff0,[2290],records,gate=None);count+=1
    print('PASS both occupancy predicates, signed heads, BYTE fields, linked blockers and native self-link exit:',count,flush=True)


def constructor(mode=8,slot=1,direction=2,weight=1,preference=0,prepare=None):
    dx=[0,1,1,1,0,-1,-1,-1];dy=[-1,-1,0,1,1,1,0,-1]
    def grid(c,u,state,calls):
        u.mem_write(0xeed760,bytes(207936));put(u,0xeed760+2290*4,3)
        put(u,0xeed760+(2290+dx[direction]+dy[direction]*228)*4,weight)
        u.mem_write(0xf533c8,b'\xa5'*500);u.mem_write(0xc8ad38+slot*500,b'\x5a'*500)
        if prepare:prepare(c,u,state,calls)
    return compare(0x51b400,[mode,slot,10,10,10,10],grid,gate=None,preference=preference)


def verify_constructor():
    count=0
    for mode,slot,direction,weight in itertools.product([4,8,0,-1],[1,999],range(8),[0,1,2,-1]):
        constructor(mode,slot,direction,weight,0x12340000|direction);count+=1
    for initial in [-2147483648,-1,0,1]:
        def grid(c,u,state,calls):put(u,0xeed760+2290*4,initial)
        constructor(prepare=grid);count+=1
    for preferred in [*range(9),255,256,257]:
        def grid(c,u,state,calls):
            for direction in range(8):
                delta=[-228,-227,1,229,228,227,-1,-229][direction]
                put(u,0xeed760+(2290+delta)*4,1)
        constructor(preference=preferred,prepare=grid);count+=1
    # A long cardinal snake separates parallel segments by an unused row.
    path=[]
    for row,y in enumerate(range(20,30,2)):
        xs=range(10,211) if row%2==0 else range(210,9,-1)
        if path:path.append((path[-1][0],y-1))
        path.extend((x,y) for x in xs)
    for length in [2,499,500,501]:
        points=path[:length];tx,ty=points[-1]
        def grid(c,u,state,calls):
            u.mem_write(0xeed760,bytes(207936));u.mem_write(0xf533c8,b'\xa5'*500)
            u.mem_write(0xc8ad38+500,b'\x5a'*500)
            for i,(x,y) in enumerate(points):put(u,0xeed760+(y*228+x)*4,i+1)
        result=compare(0x51b400,[4,1,10,20,tx,ty],grid,gate=None,preference=None)
        assert result[0][0]==(length-1 if length<=500 else 0);count+=1
    print('PASS gradient construction, cardinal/diagonal stride, AL preference, signed costs, ties and499/500-step limit:',count,flush=True)


def verify_compositions():
    count=0
    for token in QUERIES:
        query(token,0,gate=None,clear=False);count+=1
    for token,flag13,action in itertools.product([0x517f20,0x5181c0,0x518880,0x519390,0x5195e0],
                                               [0,1,255],[4,5]):
        def records(c,u,state,calls):
            word(u,0xb350dc+2289*2,255);byte(u,a(255)+13,flag13)
            byte(u,a(255)+56,action);word(u,a(255)+8,255)
        # West invokes the genuine51AFF0 provider even in animal mode.
        query(token,0,3,gate=None,prepare=records);count+=1
    for preference in [0,1,257]:
        def changed(c,u,state,calls):
            def geometry(cpu,values):
                calls.append((0x51cba0,values));put(cpu,0xeed760+2290*4,1)
                put(cpu,0xeed760+2291*4,-1);return preference
            hook(c,state,0x51cba0,4,geometry)
        constructor(direction=2,weight=0,prepare=changed);count+=1
    print('PASS actual fill/occupancy providers, self-link exit, faction/action gating and constructor callback mutations:',count,flush=True)


def verify_walk():
    count=0
    for terrain in [0,1,3,4,5,6,7,9,11]:
        states=[]
        for c,u,state in machines:
            setup(u,state)
            for y in range(8,16):
                for x in range(8,16):
                    tile=y*228+x
                    for base in [0xee0c50,0xebab20,0xec7630]:byte(u,base+tile,0)
                    put(u,0xaf598c+tile*4,0);word(u,0xb4e6fc+tile*2,0)
            p=a(255);byte(u,p+10,4);byte(u,p+14,1);byte(u,p+56,9)
            byte(u,p+57,14);byte(u,p+114,terrain);put(u,p+32,2290)
            for off,value in [(20,10),(22,10),(36,13),(38,11)]:word(u,p+off,value)
            word(u,0xb350dc+2290*2,255);put(u,0xd056f8,255)
            u.mem_write(0xd04e58,bytes(2000));put(u,0xbca2d0,228);put(u,0xbca2d4,228)
            history=[];state['service_writes'].clear()
            for tick in range(65):
                run(c,u,0x4a5da0,[255,6])
                history.append(bytes(u.mem_read(p+17,49)))
            assert struct.unpack('<hh',u.mem_read(p+20,4))==(13,11),(c,terrain,history[-1].hex())
            assert get(u,p+32)==11*228+13
            states.append(((history,list(state['service_writes'])),snapshot(u)))
        same(states,('actual route and walk',terrain));count+=1
    print('PASS full C cadence/search/allocation/construction/terrain/grid/collision/position chain to destination:',count,flush=True)


if __name__=='__main__':
    verify_classes();verify_search_state();verify_occupancy();verify_constructor()
    verify_compositions();verify_walk()
    print('Verified target SHA256:',target_sha,flush=True)
