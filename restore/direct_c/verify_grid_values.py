"""Whole grid-value selector/scan against the pinned original, offline.

Only the seven-argument numeric text endpoint is controlled. Actor counting
and channel-bit queries execute their genuine providers.
"""
from verify_options_startup import *
from verify_terrain_overlays import setup

for c,u,state in machines:u.mem_map(0x2fe00000,0x200000)


def initialize(u,state,mode,value):
    setup(u,state);put(u,0xe38e94,mode);put(u,0xf56f84,16416)
    u.mem_write(0xbcd5f8,bytes(2000*388));u.mem_write(0xb350dc,bytes(228*228*2))
    for at in [0xb285cc,0xb67d1c,0xae8e7c,0xacf85c,0xadc36c,0xee0c50,
               0xac2d4c,0xab623c,0xed4140,0xec7630,0xa9010c,0xa835fc,
               0xebab20,0xa76aec,0xa69fdc]:byte(u,at+18000,value)
    for at in [0xaf598c,0xb7482c,0xeed760,0x978d9c]:put(u,at+18000*4,value)
    word(u,0xa9cc1c+18000*2,value);byte(u,0x799a0d+511*264,value)
    for at in [0x7999f0,0x7999f8,0x799a1e,0x7999f4]:word(u,at+511*264,value)
    if value:
        word(u,0xb350dc+18000*2,1)
        word(u,0xbcd600+388,511);word(u,0xbcd600+511*388,1999)


def observe(c,state,calls,mutate=False):
    def number(uc,args):
        calls.append(args)
        if mutate and len(calls)==1:
            put(uc,0x70e47c,get(uc,0x70e47c)+1);put(uc,0x70e464,0)
            put(uc,0x70e480,get(uc,0x70e480)+1);put(uc,0x70e458,126)
            put(uc,0x70e44c,21);put(uc,0xf56f84,16672)
        return 0
    hook(c,state,0x4cce00,7,number)


def verify_selector():
    count=0
    for mode,value in itertools.product([*range(29),256],
                                         [0,1,127,128,255,-1,32767,-2147483648]):
        results=[]
        for c,u,state in machines:
            initialize(u,state,mode,value);calls=[];observe(c,state,calls)
            run(c,u,0x43e420,[0x7ffffff0,0x80000010]);results.append((calls,snapshot(u)))
        assert results[0]==results[1],('selector',mode,value,results[0][0],results[1][0])
        if mode==3 and value==32767:
            assert calls==[(32767,32,0x57c048,0x80000008,0x8000001a,16416,0xffff)]
        count+=1
    print('PASS all28 selectors, high DWORD selector, signed grid/record values, real actor queries and font:',count,flush=True)


def verify_scan():
    count=0
    for (width,height),flag_at,flag,rows,columns in itertools.product(
            [(640,480),(1024,768)],[0x78d7d8,0x78d7dc,0x78d7d4],[0,1,256],
            [0,1,3],[-1,0,2]):
        results=[]
        for c,u,state in machines:
            initialize(u,state,3,20000)
            for at in [0x78d7d8,0x78d7dc,0x78d7d4]:put(u,at,0)
            put(u,flag_at,flag);put(u,0x70e474,0);put(u,0x70e478,0)
            put(u,0x70e460,rows);put(u,0x70e464,columns)
            put(u,0x70e490,width//2);put(u,0x70e48c,height//2)
            u.mem_write(0x70e494,struct.pack('<I',0x0fff0000)*(459*231))
            for row in range(5):
                for col in range(5):
                    tile=18000+row*5+col
                    put(u,0x70e494+(row*231+col)*4,tile if col!=1 else 0x0fff0000)
                    put(u,0xb7482c+tile*4,20000+row*5+col)
            calls=[];observe(c,state,calls);run(c,u,0x43e300)
            results.append((calls,snapshot(u)))
        assert results[0]==results[1],('scan',width,flag_at,flag,rows,columns,
                                      results[0][0],results[1][0])
        count+=1
    print('PASS whole viewport scan, all three DWORD enable flags, alternating rows and skipped cells:',count,flush=True)
    for mode in [0,1,3,16,21,27]:
        results=[]
        for c,u,state in machines:
            initialize(u,state,mode,128)
            put(u,0x78d7d8,1);put(u,0x70e474,0);put(u,0x70e478,0)
            put(u,0x70e460,3);put(u,0x70e464,2)
            for row in range(5):
                for col in range(5):put(u,0x70e494+(row*231+col)*4,18000)
            calls=[];observe(c,state,calls,mutate=True);run(c,u,0x43e300)
            results.append((calls,snapshot(u)))
        assert results[0]==results[1],('callback reloads',mode,results[0][0],results[1][0])
    print('PASS genuine leaf compositions and callback row/column/geometry/font reloads:',6,flush=True)
    print('Verified target SHA256:',target_sha)


if __name__=='__main__':verify_selector();verify_scan()
