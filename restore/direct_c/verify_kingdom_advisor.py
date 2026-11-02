"""Whole kingdom advisor and explicit event getter, native/C offline.

List iteration, eligibility, country lookup, margins and rating prelude run.
Only low-level panel/text/sprite output is controlled; no full pixel proof.
"""
from verify_options_startup import *

ENDS={0x4cd680:5,0x536820:4,0x53e590:3,0x4d62f0:8,0x4cc770:5,
      0x4d6860:9,0x4d6160:9,0x4d65b0:6,0x4d63d0:8,0x4ccc30:7}
for c,u,state in machines:u.mem_map(0x2fe00000,0x200000)


def prepare(u,state,records=(),phase=5,origin=(300,200),player=0,locale=0,
            rating=75,cash=10000,stock=10000,city_state=2):
    reset(u,state);u.mem_write(0x785d70,bytes(18628))
    for at,value in [(0xeaac28,player),(0xf53f48,phase),(0xe38da8,origin[0]),
                     (0xe38da4,origin[1]),(0xf5681c,20000),(0xf56b04,21000),
                     (0xf571fc,22000),(0xf56fb4,16416),(0xf56f8c,16448),(0xf56f9c,16480),
                     (0x78d7bc,100),(0x78d7c0,5),
                     (0xea5b48+player*0x49d8,rating),(0xea187c+player*0x49d8,cash),
                     (0xea5cc0+player*0x49d8,27)]:put(u,at,value)
    for k in range(38):put(u,0xea6160+player*0x49d8+k*4,stock)
    byte(u,0xe39088,locale);word(u,0x7848d8,100)
    for i in range(50):byte(u,0x7770f0+i*324,city_state)
    byte(u,0x77de00,1);byte(u,0x77de03,200);byte(u,0x77de04,0)
    put(u,0x785d70,len(records))
    for i,data in enumerate(records):
        p=0x785d74+i*124
        byte(u,p+2,data.get('active',1));byte(u,p+0x32,data.get('enabled',1))
        word(u,p+0x30,data.get('mode',0));word(u,p+4,data.get('kind',1))
        word(u,p+12,data.get('quantity',2));word(u,p+0x2e,data.get('months',9))
        word(u,p+0x1c,data.get('city',0));word(u,p+0x2c,30)
        byte(u,p+0x61,data.get('delay',0))


def outputs(c,state,calls,mutate=False):
    for token,n in ENDS.items():
        def draw(uc,args,token=token):
            calls.append((token,args))
            if token in [0x4d62f0,0x4cc770,0x4d6860,0x4ccc30]:
                put(uc,0xe20778,get(uc,0xe20778)+17)
            if mutate and token==0x4d62f0:
                put(uc,0xf53f0c,get(uc,0xf53f0c)+3)
                put(uc,0xf53f08,get(uc,0xf53f08)+2)
            return 0
        hook(c,state,token,n,draw,cleanup=12 if not c and token==0x53e590 else 0)


def compare(mutate=False,shell=False,**kw):
    states=[]
    for c,u,state in machines:
        prepare(u,state,**kw);calls=[];outputs(c,state,calls,mutate)
        if shell:
            put(u,0x605a44,4);put(u,0xe38dac,800);put(u,0xe38db0,600);byte(u,0xe38dca,1)
            for at,value in [(0xf55c0c,20000),(0xf5691c,22000),(0xf56914,21000)]:put(u,at,value)
            for i in range(13):
                index=get(u,0x5f833c+i*36);byte(u,0x6fdc88+index,i&1)
        run(c,u,0x520ea0 if shell else 0x523ea0);states.append((calls,snapshot(u)))
    assert states[0]==states[1],(kw,mutate,states[0][0][:18],states[1][0][:18],
        [(hex(0x560000+i),a,b) for i,(a,b) in enumerate(zip(states[0][-1],states[1][-1])) if a!=b][:12])
    return states[1][0]


def verify_getter():
    for value in [-32768,-1,0,1,12,32767]:
        results=[]
        for c,u,state in machines:
            reset(u,state);p=S+0x2000;u.mem_write(p,bytes([0xa5]*124));word(u,p+0x2e,value)
            result=run(c,u,0x44c100,[p] if c else [],receiver=None if c else p)
            results.append((result,bytes(u.mem_read(p,124))))
        assert results[0]==results[1] and results[1][0]==value&0xffffffff
    print('PASS explicit getter receiver, signed WORD limits and adjacent-byte preservation:',6,flush=True)


def verify():
    count=0
    for phase,origin,player in itertools.product([-1,0,1,4,5,6,256],
            [(0,0),(192,144),(300,200)],[0,3]):
        compare(phase=phase,origin=origin,player=player);count+=1
    assert compare(phase=1)==[(0x4cd680,(364,232,512,304,0))]
    print('PASS whole opening stages, full origins/fonts/player stride and empty-state layout:',count,flush=True)
    count=0
    for kind,quantity in itertools.product(range(37),[-1,0,1,2,200]):
        compare(records=[dict(kind=kind,quantity=quantity)],player=3);count+=1
    print('PASS all37 goods/money request kinds, units/plurals, quantities and genuine deadlines:',count,flush=True)
    count=0
    for kind,stock,cash in itertools.product([1,10,24,28,35,36],[0,100,100000],[0,10000]):
        compare(records=[dict(kind=kind,quantity=100,months=-1)],stock=stock,cash=cash);count+=1
    print('PASS treasury/inventory availability, bulk conversion and deadline formatting:',count,flush=True)
    count=0
    for city,strength,locale,state in itertools.product([-1,0,49,50],[45,46,88,89],[0,1],[1,2]):
        compare(records=[dict(kind=37,quantity=strength,city=city)],locale=locale,city_state=state);count+=1
    print('PASS royal requests, city bounds/status, both language groups and strength thresholds:',count,flush=True)
    for records in [[],[dict(kind=1)]*5,[dict(kind=1)]*8,
                    [dict(kind=1,mode=2)]*3,
                    [dict(kind=1,enabled=0),dict(kind=36),dict(kind=37),dict(kind=28)],
                    [dict(kind=1,active=0),dict(kind=24),dict(kind=37,delay=255)]]:
        compare(records=records,mutate=True)
    print('PASS genuine list filtering/IDs, five-row limit and live layout reloads:',6,flush=True)
    print('Verified target SHA256:',target_sha)


PAGES=[0x521080,0x521b50,0x522e10,0x523ea0,0x4f6af0,0x559e10,0x525cb0,
       0x527bf0,0x528360,0x528950,0x5298e0,0x52a250,0x52c570,0x52b520]


def verify_shell():
    count=0
    for selection,(width,height),background in itertools.product(range(15),
            [(640,480),(800,600),(1024,768)],[0,1,2]):
        states=[]
        for c,u,state in machines:
            prepare(u,state);put(u,0xe38dac,width);put(u,0xe38db0,height)
            put(u,0x605a44,selection);byte(u,0xe38dca,background)
            for at,value in [(0xf55c0c,20000),(0xf5691c,22000),(0xf56914,21000)]:put(u,at,value)
            for i in range(13):
                index=get(u,0x5f833c+i*36);byte(u,0x6fdc88+index,255 if i&1 else 0)
            calls=[];outputs(c,state,calls)
            run(c,u,0x520f40);states.append((calls,snapshot(u)))
        assert states[0]==states[1],('shell',selection,width,background,
                                      states[0][0][:15],states[1][0][:15])
        count+=1
    print('PASS complete advisor shell, all15 highlights, three sizes, real buttons and13 availability fields:',count,flush=True)
    for selection in [0,1,2,14]:
        states=[]
        for c,u,state in machines:
            prepare(u,state);put(u,0x605a44,selection);calls=[];outputs(c,state,calls)
            def buttons(uc,args):
                calls.append((0x425730,args));put(uc,0x605a44,7)
                put(uc,0xe38da8,700);put(uc,0xe38da4,500);return 0
            hook(c,state,0x425730,4,buttons)
            run(c,u,0x520f40);states.append((calls,snapshot(u)))
        assert states[0]==states[1],('shell reload',selection,states[0][0],states[1][0])
    print('PASS post-button selection reload and preserved native origin:',4,flush=True)


def verify_routes():
    count=0
    for selection,change in itertools.product([*range(16),260,0xffffffff],[False,True]):
        states=[]
        for c,u,state in machines:
            prepare(u,state);put(u,0x605a44,selection);calls=[]
            def header(uc,args):
                calls.append(0x520f40)
                if change:put(uc,0x605a44,4)
                return 0
            hook(c,state,0x520f40,0,header)
            for page in PAGES:
                hook(c,state,page,0,lambda uc,args,page=page:calls.append(page) or 0)
            run(c,u,0x520ea0);states.append((calls,snapshot(u)))
        assert states[0]==states[1],('advisor route',selection,change,states[0][0],states[1][0])
        expected=4 if change else selection
        assert calls==[0x520f40]+([PAGES[expected-1]] if 1<=expected<=14 else [])
        count+=1
    print('PASS all14 page routes, invalid full DWORD selections and post-shell reload:',count,flush=True)
    for phase,records in itertools.product([1,5],[[],[dict(kind=1)],
            [dict(kind=36),dict(kind=37),dict(kind=28)]]):
        compare(shell=True,phase=phase,records=records,player=3)
    print('PASS real advisor-shell/kingdom/list/query compositions:',6,flush=True)
    print('Verified target SHA256:',target_sha)


if __name__=='__main__':verify_getter();verify();verify_shell();verify_routes()
