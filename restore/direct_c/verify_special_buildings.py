"""Whole special-building providers, offline native/C comparison.

Real geometry, category selection and pyramid record/actor traversals execute.
Only sprite output and the separate actor-render endpoint are controlled.
"""
from verify_options_startup import *
from verify_terrain_overlays import setup
from verify_overlay_metrics import sprites


def prepare(u,state,building=511,direction=0):
    setup(u,state,building=building,orientation=direction)
    u.mem_write(0xb4e6fc,bytes(228*228*2));u.mem_write(0xb350dc,bytes(228*228*2))
    u.mem_write(0xbcd5f8,bytes(2000*388))
    for at,value in [(0xf57584,20000),(0xf5758c,21000),(0xf56d74,22000),
                     (0xf56afc,23000),(0xf57334,24000)]:put(u,at,value)
    for i in range(5):
        group=get(u,0x5df778+i*4);put(u,0xf55bb4+group*8,25000+i*32)
        word(u,0x7848e2+i*2,0)


def compare(token,args,init):
    results=[]
    for c,u,state in machines:
        init(u,state);calls=[];sprites(c,state,calls)
        hook(c,state,0x438e50,4,lambda uc,a:calls.append((0x438e50,a)) or 0)
        run(c,u,token,args,cleanup=12 if not c and token==0x401f70 else 0)
        results.append((calls,snapshot(u)))
    assert results[0]==results[1],(hex(token),args,results[0][0][:15],results[1][0][:15],
        [(hex(0x560000+i),a,b) for i,(a,b) in enumerate(zip(results[0][-1],results[1][-1])) if a!=b][:12])
    return results[1][0],results[1][1]


def verify_sun_temple():
    count=0
    for building,stage in itertools.product([1,511,3999],[*range(16),0x10001,-1]):
        def init(u,s):prepare(u,s,building);put(u,0x799a30+building*264,stage)
        calls,_=compare(0x542e10,[building,80,100],init)
        if stage==1:assert calls==[(0x53e590,(22013,140,71))]
        count+=1
    print('PASS sun-temple full DWORD stages, complete selector and building stride:',count,flush=True)


def verify_abu_simbel():
    count=0
    for building,mask,direction in itertools.product([1,511,3999],
            [0,1,4,0x800,0xfff],[0,2,4,6]):
        delta=0 if building==1 else 1
        def init(u,s):
            prepare(u,s,building,direction);word(u,0x7999ea+building*264,delta)
            u.mem_write(0x799a39+(building-delta)*264,
                        bytes(bool(mask&(1<<i)) for i in range(12)))
        calls,_=compare(0x401f70,[building,80,100],init)
        if mask==0xfff and direction in [0,6]:assert len(calls)==12
        if direction in [2,4]:assert not calls
        count+=1
    print('PASS Abu Simbel owned tables, all12 flags, root offsets, orientations and RET12:',count,flush=True)


def verify_annex():
    count=0
    for variant,direction,active in itertools.product(range(4),[0,2,4,6],[1,6]):
        def init(u,s):
            prepare(u,s,direction=direction);byte(u,0x799a82+511*264,variant)
            byte(u,0x7999d8+511*264,active)
        calls,_=compare(0x4c0160,[511],init)
        assert len(calls)==4;count+=1
    for category,index in itertools.product([1,2],range(5)):
        def init(u,s):
            prepare(u,s);word(u,0x7848e2+index*2,category)
            # The primary category wins even if the fallback occurs earlier.
            if category==2 and index:word(u,0x7848e2,1)
        calls,_=compare(0x4c0160,[511],init)
        assert calls[-1][1][0] in [25000+index*32,25001+index*32];count+=1
    print('PASS annex orientation/reversal, active highlight, genuine category priority and projection:',count,flush=True)


def pyramid_init(u,state,shape=1,style=0,part=0,layer=0,stage=0,direction=0,
                 relation='none',with_actors=False,reset_world=True):
    if reset_world:prepare(u,state,direction=direction)
    put(u,0xbca2e0,direction);o=511*264
    put(u,0x5e4ef4,shape);word(u,0x7999e8+o,183);word(u,0x7999ea+o,part)
    put(u,0x7999e4+o,18000)
    byte(u,0x799a2c+o,style);byte(u,0x799a2d+o,0);byte(u,0x799a2f+o,layer)
    byte(u,0x799a31+o,stage);byte(u,0x799a32+o,stage+1)
    put(u,0x799a40+o,24109)
    for field,value in [(0x799a34,-17),(0x799a36,9),(0x799a38,21),(0x799a3a,-7)]:word(u,field+o,value)
    p=512*264
    word(u,0x7999e8+p,183);word(u,0x7999ea+p,1);put(u,0x7999e4+p,18001)
    put(u,0x799a40+p,24109);byte(u,0x799a31+p,stage);byte(u,0x799a32+p,stage+1)
    for at,value in [(0x799a48,240),(0x799a4c,200),(0x799a50,140),(0x799a54,100)]:put(u,at+p,value)
    if relation=='parent':put(u,0x799a44+o,512)
    if relation=='neighbor':
        for table in [0x5700f4,0x5700f8]:
            for camera in range(4):
                delta=struct.unpack('<i',u.mem_read(table+camera*12,4))[0]
                word(u,0xb4e6fc+(18000+delta)*2,512)
    if with_actors:
        for i,(actor,phase) in enumerate([(1,3),(511,0),(1999,1)]):
            word(u,0xb350dc+(18000+[0,1,228][i])*2,actor)
            word(u,0xbcd718+actor*388,phase)


def verify_pyramid():
    count=0
    for shape,direction,stage in itertools.product([1,3,5],[0,2,4,6],[0,1,6,7,10,13,25]):
        def init(u,s):pyramid_init(u,s,shape=shape,direction=direction,stage=stage,
                                  style=stage%3,part=stage%8,layer=stage%2)
        compare(0x4ef550,[20000,80,100,511],init);count+=1
    print('PASS complete pyramid body, three shapes, layers, part/stage selectors and orientations:',count,flush=True)
    count=0;published=0;actor_calls=0
    for shape,relation,direction in itertools.product([1,3,5],['none','parent','neighbor'],[0,2,4,6]):
        def init(u,s):pyramid_init(u,s,shape=shape,direction=direction,stage=13,style=1,
                                  part=5,layer=1,relation=relation,with_actors=True)
        calls,data=compare(0x4ef550,[20000,80,100,511],init)
        if relation=='neighbor' and struct.unpack_from('<I',data,0x799a44+512*264-0x560000)[0]:published+=1
        actor_calls+=sum(token==0x438e50 for token,args in calls);count+=1
    assert published and actor_calls
    print('PASS pyramid parent/neighbor publication and genuine actor-layer traversal:',count,
          'neighbor publications:',published,'actor endpoint calls:',actor_calls,flush=True)
    print('Verified target SHA256:',target_sha)


def verify_compositions():
    from verify_world_detail import prepare as world_prepare,install
    for c,u,state in machines:u.mem_map(0x2fe00000,0x200000)
    count=0
    for (width,height),kind,direction in itertools.product(
            [(640,480),(800,600),(1024,768)],[202,213,230,183],[0,6]):
        results=[]
        for c,u,state in machines:
            world_prepare(u,state,0,kind,2,width=width,height=height)
            put(u,0xbca2e0,direction);o=511*264
            word(u,0x7999de+o,50);word(u,0x7999e0+o,50)
            for at,value in [(0xf57584,20000),(0xf5758c,21000),(0xf56d74,22000),
                             (0xf56afc,23000),(0xf57334,24000)]:put(u,at,value)
            word(u,0x7999ea+o,0);word(u,0x10d6a16+20000*64,0)
            if kind==213:put(u,0x799a30+o,1)
            if kind==230:
                part=1 if direction==6 else 0
                word(u,0x7999ea+o,part)
                u.mem_write(0x799a39+(511-part)*264,bytes([1]*12))
            if kind==183:pyramid_init(u,state,shape=3,style=1,part=5,layer=1,stage=13,
                                       direction=direction,reset_world=False)
            calls=[];install(c,u,state,calls,True)
            run(c,u,0x436b60,[0]);results.append((calls,snapshot(u)))
        assert results[0]==results[1],('city composition',width,kind,direction,
            results[0][0][:15],results[1][0][:15],
            [(hex(0x560000+i),a,b) for i,(a,b) in enumerate(zip(results[0][-1],results[1][-1])) if a!=b][:12])
        if kind==213:assert any(a[0]==22013 for token,a in calls if token==0x53e590)
        if kind==230:assert sum(a[0] in [20000,20001,21000,21001] for token,a in calls if token==0x53e590)>=12
        count+=1
    print('PASS complete city compositions with real spot-position/category/projection providers:',count,flush=True)
    print('Verified target SHA256:',target_sha)


if __name__=='__main__':
    verify_sun_temple();verify_abu_simbel();verify_annex();verify_pyramid()
    verify_compositions()
