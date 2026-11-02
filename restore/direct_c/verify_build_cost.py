"""Native/C placement-cost projection, number text, font and clipping.

Only final text raster output is observed; genuine coordinate projection,
number conversion and numeric-label composition execute. This does not claim
pixel fidelity of the unchanged glyph renderer or complete placement logic.
"""
from verify_options_startup import *


def exercise(width,height,orientation,price,cash,player=1,tool=1,blocked=0):
    results=[]
    for c,u,state in machines:
        reset(u,state)
        for at,value in [(0xbca300,tool),(0xe38dcc,blocked),(0xbc9724,price),
                         (0xbca304,30),(0xbca308,32),(0xeaac28,player),
                         (0xea187c+player*0x49d8,cash),(0xf56f84,16416),
                         (0x70e474,2),(0x70e478,3),(0x70e494+(2*231+3)*4,70+80*228),
                         (0xbca2e8,43),(0xbca2ec,51),(0xbca2e0,orientation),
                         (0x70e490,width//2),(0x70e48c,80),(0x70e45c,29),(0x70e44c,15),
                         (0x70e458,30),(0x70e430,width-24),(0x70e434,24),
                         (0x70e438,height-40),(0x70e43c,40),(0xe38dac,width),(0xe38db0,height)]:
            put(u,at,value)
        draws=[]
        def text(cpu,args):
            message=bytes(cpu.mem_read(args[0],256)).split(b'\0',1)[0]
            clip=tuple(get(cpu,at) for at in [0xd3b550,0xd3b554,0xd3b558,0xd3b55c])
            draws.append((message,args[1:],clip));return 0
        hook(c,state,0x4cc770,5,text)
        run(c,u,0x43e1a0)
        results.append((draws,snapshot(u)))
    assert results[0]==results[1],(width,height,orientation,price,cash,tool,blocked,
                                 results[0][0],results[1][0])
    if tool and not blocked and price:
        assert len(draws)==2 and all(d[1][2]==16416 for d in draws)
        if price<1000000000:
            assert draws[0][0]==b'@'+str(price).encode()+b' '
        assert get(u,0xbc9724)==0
    else:assert not draws


def verify():
    count=0
    for (width,height),orientation,price,affordable in itertools.product(
            [(640,480),(800,600),(1024,768)],[0,2,4,6],[1,256,300,25000],[False,True]):
        exercise(width,height,orientation,price,price if affordable else price-1,
                 player=[0,1,3][count%3]);count+=1
    for tool,blocked,price in [(0,0,300),(256,0,300),(1,1,300),(1,256,300),(1,0,0)]:
        exercise(1024,768,0,price,1000,tool=tool,blocked=blocked);count+=1
    exercise(1024,768,0,300,0xffffffff);count+=1
    exercise(1024,768,0,0xffffffff,0);count+=1
    print('PASS cost text/full font token, all orientations/resolutions, signed funds and gates:',count)
    print('Verified target SHA256:',target_sha)


if __name__=='__main__':verify()
