"""Whole native/C stock overlays and animation-anchor metadata.

Only the final sprite endpoint is controlled. Stock selection/division,
rotation, frame metadata and coordinates execute. No pixel-output claim.
"""
from verify_options_startup import *


def sprite_spy(c,state,draws):
    if c:
        def draw(cpu,args):
            assert args[0]==0xf55bb0
            draws.append(args[1:]);return 0x12345678
        state['hooks'][sym('restore_sprite_draw')]=(4,0,draw)
    else:
        def draw(cpu,args):
            assert cpu.reg_read(UC_X86_REG_ECX)==0xf55bb0
            draws.append(args);return 0x12345678
        hook(c,state,0x53e590,3,draw,cleanup=12)


def stock(building,kind,amount,secondary=0,orientation=0,variant=0):
    results=[]
    for c,u,state in machines:
        reset(u,state)
        word(u,0x7999e8+building*264,kind)
        word(u,0x799a14+building*264,amount)
        word(u,0x799aa4+building*264,secondary)
        byte(u,0x799a82+building*264,variant);put(u,0xbca2e0,orientation)
        for index,at in enumerate([0xf565fc,*range(0xf56a4c,0xf56ab4,8),
                                   0xf572c4,0xf572cc,0xf572d4,0xf572f4]):
            put(u,at,20000+index*32)
        put(u,0xf56a84,13198)
        draws=[];sprite_spy(c,state,draws)
        run(c,u,0x436310,[building,80,100])
        results.append((draws,snapshot(u)))
    assert results[0]==results[1],('stock',building,kind,amount,secondary,orientation,variant,
                                 results[0][0],results[1][0])
    if kind==115 and amount==300:assert draws==[(13200,141,113)]


def worker(image,limit,dx,dy):
    results=[]
    for c,u,state in machines:
        reset(u,state);put(u,0x126517c,limit)
        word(u,0x10d6a1a,31);word(u,0x10d6a1c,-12)
        if image<limit:
            word(u,0x10d6a1a+image*64,dx);word(u,0x10d6a1c+image*64,dy)
        draws=[];sprite_spy(c,state,draws)
        value=run(c,u,0x4381b0,[image,80,100,113,3])
        results.append((value,draws,snapshot(u)))
    assert results[0]==results[1],('worker',image,limit,dx,dy,results[0][:2],results[1][:2])
    assert results[1][0]==0x12345678
    if image==18477 and limit==50000 and (dx,dy)==(17,-9):
        assert draws==[(18477,176,112)]


def verify():
    def raw(at,n):return bytes(images[0].data[at-images[0].base:at-images[0].base+n])
    targets=struct.unpack('<24I',raw(0x436a28,96))
    kinds=[kind+32 for kind,index in enumerate(raw(0x436a88,202)) if targets[index]!=0x436a1d]
    count=0
    for kind,amount,secondary in itertools.product(kinds,[0,1,600],[0,600]):
        stock(6,kind,amount,secondary);count+=1
    for orientation,variant in itertools.product([0,2,4,6],range(4)):
        stock(400,74,300,200,orientation,variant);count+=1
    for building,amount in itertools.product([1,6,400,3999],[-32768,-1,0,1,99,100,199,200,300,499,500,32767]):
        stock(building,115,amount);count+=1
    stock(6,71,300);count+=1
    print('PASS complete stock-overlay switch, WORD stocks, high building IDs and rotated variants:',count,flush=True)
    count=0
    for image,limit,offset in itertools.product([0,1,18477,50000],[128,50000],[(17,-9),(-31,24),(0,0)]):
        worker(image,limit,*offset);count+=1
    print('PASS full image limit,64-byte metadata, signed offsets and record0 fallback:',count)
    print('Verified target SHA256:',target_sha)


def verify_city_compositions():
    """Exercise436B60 and real stock/frame/anchor providers together.

    Food-pile rendering remains a separate boundary. This checks the granary
    worker, not inherited food-pile caller frames or physical output pixels.
    """
    from verify_world_detail import prepare,install,record
    archive=(ROOT/'build/debug-startup-game/Data/SprAmbient.sg3').read_bytes()
    for c,u,state in machines:u.mem_map(0x2fe00000,0x200000)
    count=0
    for (width,height),kind,amount,frame in itertools.product(
            [(640,480),(800,600),(1024,768)],[115,71],[0,100,300],[0,1,3,8]):
        results=[]
        for c,u,state in machines:
            prepare(u,state,0,kind,2,width=width,height=height)
            word(u,0x799a14+511*264,amount);byte(u,0xadc36c+18000,frame)
            put(u,0xf55bb4+624*8,18477)
            for image in range(18477,18481):
                dx,dy=struct.unpack_from('<hh',archive,40680+(image-15831)*64+34)
                word(u,record(image)+34,dx);word(u,record(image)+36,dy)
            calls=[];install(c,u,state,calls,True)
            run(c,u,0x436b60,[0]);results.append((calls,snapshot(u)))
        assert results[0]==results[1],('city composition',width,kind,amount,frame,
                                      results[0][0],results[1][0])
        x=width//2-240;y=265
        if kind==115 and amount:
            assert (0x53e590,({100:13198,300:13200}[amount],x+61,y+13)) in calls
        if kind==71 and frame:
            image=18476+min(frame,4)
            dx,dy=struct.unpack_from('<hh',archive,40680+(image-15831)*64+34)
            assert (0x53e590,(image,x+113-dx,y+3-dy)) in calls
        count+=1
    print('PASS normal city stock/worker compositions, actual SG3 anchors, three sizes and frame limits:',count,flush=True)
    print('Verified target SHA256:',target_sha)


if __name__=='__main__':verify();verify_city_compositions()
