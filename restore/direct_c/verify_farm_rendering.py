"""Native/C farm terrain classification and actual city-layer selection.

The selection cases control only final sprite drawing. Both rectangle queries and crop
selection execute; a deliberately different Y in record4*id catches the old
scaled-pointer bug. Pixel cases use real SG3/555 data and real raster endpoints.
No host game, settings, assets or saves are modified.
"""
from verify_world_detail import *

TILE=18000;X=35;Y=30;WRONG_Y=43
FARMS=[100,101,102,103,104,105,196,224]
for c,u,state in machines:u.mem_map(0x2fe00000,0x200000)

def farm_prepare(u,state,building=9,kind=196,flood=True,wrong_flood=False,growth=0):
    prepare(u,state,0,kind,2,building=building,image=15410,size=(-17,-6))
    base=TILE-X-Y*228
    put(u,0xbca2d8,base)
    word(u,0x7999de+building*264,X);word(u,0x7999e0+building*264,Y)
    byte(u,0x7999db+building*264,3)
    word(u,0x7999e0+building*1056,WRONG_Y)
    word(u,0x799a2c+building*264,growth)
    for row in range(3):
        for col in range(3):
            put(u,0x5cc8c0+row*48+col*8,row*228+col)
            for y,flag in [(Y,flood),(WRONG_Y,wrong_flood)]:
                tile=base+y*228+X+row*228+col
                put(u,0xaf598c+tile*4,8|(0x10000 if flag else 0))
                put(u,0xd05720+tile*4,0)
            word(u,0xb4e6fc+(TILE+row*228+col)*2,building)
    # Every crop uses a distinct tag base, so a barley fallback is observable.
    for group in [369,463,325,282,322,324,384,715]:put(u,0xf55bb4+group*8,20000+group*4)
    byte(u,0xadc36c+TILE,3)
    byte(u,0x605b38,0)
    # Suppress no animation by stubbing a gate: the real farm gate must do it.
    word(u,0x799a18+building*264,1)

def predicates():
    n=0
    for building,kind,flood,wrong in itertools.product([1,3,9,511,999],FARMS,[False,True],[False,True]):
        results=[]
        for c,u,state in machines:
            farm_prepare(u,state,building,kind,flood,wrong)
            query=run(c,u,0x4894a0,[building],cleanup=4 if not c else 0)
            gate=run(c,u,0x488050,[building],cleanup=4 if not c else 0)
            assert (query&255)==int(flood),('query',c,building,kind,flood,wrong,query)
            assert gate==int(flood),('farm gate',c,building,kind,flood,wrong,gate)
            results.append((query,gate,snapshot(u)))
        equal(results,('farm predicates',building,kind,flood,wrong));n+=1
    print('PASS farm query/gate with true and decoy coordinates:',n,flush=True)
    return n

def city_case(kind=196,flood=True,wrong=False,growth=0):
    results=[]
    for c,u,state in machines:
        farm_prepare(u,state,9,kind,flood,wrong,growth)
        draws=[]
        for token,name in [(0x53e590,'restore_sprite_draw'),(0x53f250,'restore_sprite_draw_upper')]:
            if c:
                def draw(cpu,args,token=token):
                    assert args[0]==0xf55bb0;draws.append((token,args[1:]));return 0x12345678
                state['hooks'][sym(name)]=(4,0,draw)
            else:
                def draw(cpu,args,token=token):draws.append((token,args));return 0x12345678
                hook(c,state,token,3,draw,cleanup=12)
        run(c,u,0x436b60,[0])
        results.append((tuple(draws),snapshot(u)))
    if results[0]!=results[1]:
        raise AssertionError(('city farm layers',kind,flood,wrong,growth,results[0][0],results[1][0],
            [(hex(0x560000+i),a,b) for i,(a,b) in enumerate(zip(results[0][1],results[1][1])) if a!=b][:12]))
    groups=dict(zip(FARMS,[369,463,325,282,322,324,384,715]))
    crop_base=20000+groups[kind]*4
    crop_calls=[args for token,args in results[1][0] if crop_base<=args[0]<=crop_base+5]
    assert len(crop_calls)==(9 if flood else 5)
    generic=[args for token,args in results[1][0] if args[0]==15413]
    assert bool(generic)==(not flood)
    return results[1][0]

def city():
    n=0
    for kind,flood,wrong,growth in itertools.product(FARMS,[False,True],[False,True],[0,100,500]):
        city_case(kind,flood,wrong,growth);n+=1
    print('PASS real city farm/crop/animation selection, all crop kinds and growth stages:',n,flush=True)
    return n

def pixels():
    from verify_workplace_pixels import archive,PIXELS,SCREEN
    records,payload=archive('Pharaoh_General',11907,range(13949,13961))
    terrain,terrain_payload=archive('Pharaoh_Terrain',14453,range(15410,15461))
    shift=0x1000000
    records.update({image:struct.pack('<I',struct.unpack_from('<I',record)[0]+shift)+record[4:]
                    for image,record in terrain.items()})
    for c,u,state in machines:
        u.mem_map(PIXELS,0x2000000);u.mem_map(SCREEN+0x1000000,0x200000)
        u.mem_write(PIXELS,payload)
        u.mem_write(PIXELS+shift,terrain_payload)
    n=0;by_kind={}
    for kind,flood,growth in itertools.product([100,196],[False,True],[0,100,200,300,400,500]):
        output=[]
        for c,u,state in machines:
            farm_prepare(u,state,9,kind,flood,not flood,growth)
            for image,record in records.items():u.mem_write(0x10d69f8+image*64,record)
            put(u,0xf55bb4+369*8,13949);put(u,0xf55bb4+384*8,13955)
            for at,value in [(0xe92e18,SCREEN+0x1000000),(0xe38d94,1600),(0xe38d9c,800),
                    (0xe38d98,600),(0xe38dac,800),(0xe38db0,600),(0xd3b550,600),
                    (0xd3b554,0),(0xd3b558,800),(0xd3b55c,0)]:put(u,at,value)
            byte(u,0xe38dda,0)
            u.mem_write(SCREEN+0x1000000,b'\x5a'*(800*600*2))
            run(c,u,0x436b60,[0])
            output.append(bytes(u.mem_read(SCREEN+0x1000000,800*600*2)))
        assert output[0]==output[1],('actual farm pixels',kind,flood,growth)
        assert output[1]!=b'\x5a'*(800*600*2)
        by_kind[kind,flood,growth]=output[1]
        n+=1
    for flood,growth in itertools.product([False,True],[0,100,200,300,400,500]):
        assert by_kind[100,flood,growth]!=by_kind[196,flood,growth],('barley/fig distinction',flood,growth)
    print('PASS actual barley/fig city framebuffer, six stages and both terrains:',n,flush=True)
    return n

if __name__=='__main__':
    total=predicates()+city()+pixels()
    print('PASS total:',total,'target SHA256',target_sha,flush=True)
