"""Native/C cell counts and load-time actor-chain cleanup.

The footprint publisher is controlled to isolate485F70's argument ordering
and link repair. Actual building-footprint drawing and live stability are
outside this regression. No user save is modified.
"""
from verify_options_startup import *


def verify():
    cases = 0
    for tile, length in itertools.product([0, 18000, 51983], [0, 1, 3, 25, 200, 1999]):
        results = []
        for c, u, state in machines:
            reset(u, state)
            u.mem_write(0xb350dc, bytes(228*228*2))
            u.mem_write(0xbcd5f8, bytes(2000*388))
            word(u, 0xb350dc+tile*2, 1 if length else 0)
            for actor in range(1, length+1):
                word(u, 0xbcd600+actor*388, actor+1 if actor<length else 0)
            value = run(c, u, 0x4b54f0, [tile])
            assert value == length
            results.append((value, snapshot(u)))
        assert results[0] == results[1], ('cell count', tile, length)
        cases += 1
    print('PASS exact cell chain counts:', cases, flush=True)

    cases = 0
    for actor, loop_length, active in itertools.product([1, 9, 511, 1997], [0, 1, 3], [0, 1]):
        results = []
        for c, u, state in machines:
            reset(u, state)
            u.mem_write(0x7999d8, bytes(4000*264))
            u.mem_write(0xbcd5f8, bytes(2000*388))
            put(u, 0xeaac28, 257)
            # Distinguish BYTE truncation and a four-times-scaled lookup.
            put(u, 0x5d06dc+71*20, 0x12345678)
            for building in [1, 6, 3999]:
                row = 0x799af0+building*264-264
                byte(u, row-16, 1)
                word(u, row, 71)
                word(u, row-10, 38)
                word(u, row-8, 59)
            byte(u, 0xbcd606+actor*388, active)
            for index in range(loop_length):
                word(u, 0xbcd600+(actor+index)*388,
                     actor+index+1 if index+1<loop_length else actor)
            calls = []
            hook(c, state, 0x470990, 8, lambda uc, args: calls.append(args) or 0)
            run(c, u, 0x485f70)
            assert calls == [(257, 71, b, 38, 59, 0x12345678, 0, 0) for b in [1, 6, 3999]]
            if active and loop_length:
                assert bytes(u.mem_read(0xbcd600+(actor+loop_length-1)*388, 2)) == b'\0\0'
            results.append((calls, snapshot(u)))
        assert results[0] == results[1], ('load repair', actor, loop_length, active)
        cases += 1
    print('PASS load cleanup, self/multiple-node loops and footprint publication:', cases)
    print('Verified target SHA256:', target_sha)


def verify_relocation():
    cases=0
    for left,top,width,height in [(0,0,8,8),(43,51,8,8)]:
        for mode in ['current','north','south','west','east','occupied','distant','none','alias']:
            results=[]
            for c,u,state in machines:
                reset(u,state)
                u.mem_write(0xb350dc,bytes(228*228*2))
                u.mem_write(0xed4140,b'\xff'*(228*228))
                for at,v in [(0xbca2d8,left+top*228),(0xbca2e8,left),(0xbca2ec,top),
                             (0xbca2f0,left+width-1),(0xbca2f4,top+height-1)]:put(u,at,v)
                x=y=3
                choices={'current':(3,3),'north':(3,2),'south':(3,4),'west':(2,3),
                         'east':(4,3),'occupied':(3,2),'distant':(6,6),'alias':(2,2)}
                if mode in choices:
                    xx,yy=choices[mode];tile=left+xx+(top+yy)*228
                    byte(u,0xed4140+tile,0)
                    if mode=='occupied':word(u,0xb350dc+tile*2,9)
                put(u,S+0x100,x);put(u,S+0x104,y)
                args=[S+0x100,S+0x100 if mode=='alias' else S+0x104]
                value=run(c,u,0x4467a0,args)&255
                results.append((value,bytes(u.mem_read(S+0x100,8)),snapshot(u)))
            assert results[0]==results[1],('nearest cell',left,top,mode)
            assert results[1][0]==(mode not in ['none','occupied'])
            cases+=1
    print('PASS complete nearest-cell search, clipped bounds, alias and no-space cases:',cases,flush=True)

    cases=0
    for actor,mode,pixel in itertools.product([1,9,511,1999],['passable','blocked','full'],[450,32760]):
        results=[]
        for c,u,state in machines:
            reset(u,state)
            u.mem_write(0xb350dc,bytes(228*228*2));u.mem_write(0xbcd5f8,bytes(2000*388))
            u.mem_write(0xed4140,b'\xff'*(228*228))
            left,top=43,51;tile=left+3+(top+3)*228
            for at,v in [(0xbca2d8,left+top*228),(0xbca2e8,left),(0xbca2ec,top),
                         (0xbca2f0,left+7),(0xbca2f4,top+7)]:put(u,at,v)
            put(u,0xbcd618+actor*388,tile)
            word(u,0xbcd60c+actor*388,3);word(u,0xbcd60e+actor*388,3)
            word(u,0xbcd642+actor*388,pixel);word(u,0xbcd644+actor*388,pixel)
            predecessor=2 if actor==1 else 1
            successor=3 if actor!=3 else 4
            word(u,0xb350dc+tile*2,predecessor)
            word(u,0xbcd600+predecessor*388,actor)
            word(u,0xbcd600+actor*388,successor)
            if mode=='passable':byte(u,0xed4140+tile,0)
            elif mode=='blocked':byte(u,0xed4140+tile-228,0)
            run(c,u,0x4be4a0,[actor],cleanup=0 if c else 4)
            results.append(snapshot(u))
        assert results[0]==results[1],('relocation',actor,mode,pixel)
        cases+=1
    print('PASS composed relocation with genuine search/unlink/insert and WORD wrap:',cases)


if __name__ == '__main__':
    verify()
    verify_relocation()
