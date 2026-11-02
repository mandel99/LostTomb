"""Whole native/C pyramid parts: footprint, images, workers and entrance.

Actual template/offset tables and the complete seven-function placement
composition execute. Allocation, model creation, tile commit and brush
endpoints are controlled. No host game or physical rendering is exercised.
"""
from verify_options_startup import *


def prepare(u,state):
    reset(u,state);u.mem_write(SP-0x5000,b'\xa5'*0x5000)


def intercept(c,state,token,name,n,action):
    state['hooks'][sym(name) if c else token]=(n,0,action)


def compare(values,label):
    assert values[0][0]==values[1][0],(label,'calls',
        next(((i,a,b) for i,(a,b) in enumerate(zip(values[0][0],values[1][0])) if a!=b),None),
        [len(x[0]) for x in values])
    equal(values,label)


def record(u,building=400,piece=0,phase=0):
    r=0x7999d8+building*264
    u.mem_write(r,b'\xa5'*264)
    word(u,r+6,90);word(u,r+8,90);byte(u,r+3,2);word(u,r+16,183);word(u,r+18,piece)
    put(u,r+12,21610);byte(u,r+84,0);byte(u,r+85,phase)
    byte(u,r+86,1);byte(u,r+87,1);byte(u,r+88,1)
    byte(u,r+89,1);byte(u,r+90,6);byte(u,r+91,5)
    for a,v in [(0xbca2d0,400),(0xbca2d4,400),(0xbca2d8,1000),(0xbca2e0,2),
                (0xf57334,100000),(0xf57364,200000),(0xf5738c,300000),(0xf5736c,400000),
                (0xbc9768,0x12345678)]:put(u,a,v)
    return r


def footprint(x,y,size,width=400,height=400):
    values=[]
    for c,u,state in machines:
        prepare(u,state);r=record(u)
        word(u,r+6,x);word(u,r+8,y);byte(u,r+3,size);put(u,0xbca2d0,width);put(u,0xbca2d4,height)
        run(c,u,0x4ee6c0,[400]);values.append(([],snapshot(u)))
    compare(values,('footprint',x,y,size,width,height))


def image(mode,shape,rotation,phase=0,step=1,layer=1,height=1,workers=(1,6),alternate=0,piece=5,mutate=False,kind=0):
    values=[]
    for c,u,state in machines:
        prepare(u,state);r=record(u,piece=piece,phase=phase)
        put(u,0x5e4ef4,mode);put(u,0xbca2e0,rotation*2)
        put(u,(0x5e6578 if mode==5 else 0x5e4ef8)+(piece+kind*(36 if mode==5 else 144))*8,shape)
        byte(u,r+86,step);byte(u,r+87,layer);byte(u,r+88,height)
        byte(u,r+89,workers[0]);byte(u,r+90,workers[1]);byte(u,0x5e68d8+kind*6+layer,alternate)
        byte(u,0xacf85c+21610,1)
        # Full signed DWORD division must differ from low-WORD/unsigned halves.
        for i in range(96):put(u,0x5f561c+i*8,-11-i*3)
        calls=[]
        def draw(cpu,args,t):
            calls.append((t,args))
            if mutate and len(calls)==1:put(cpu,r+12,22610);put(cpu,0x5e4ef4,5)
            return 0
        for t,n in [(0x47bf50,7),(0x4f3f70,4)]:hook(c,state,t,n,lambda cpu,args,t=t:draw(cpu,args,t))
        run(c,u,0x4edf50,[400,rotation,kind]);values.append((calls,snapshot(u)))
    compare(values,('image',mode,shape,rotation,phase,step,layer,height,workers,alternate,piece,mutate,kind))


def finished(mode,kind,phase,mask=5,mutate=False,match=True,noise=1):
    values=[]
    for c,u,state in machines:
        prepare(u,state);r=record(u,phase=phase)
        put(u,0x5e4ef4,mode);put(u,0xbca2e0,6);byte(u,r+91,mask)
        descriptor=(0x5f6d58 if mode==5 else 0x5f6cd0)+kind*8
        table=get(u,descriptor)
        if phase==3:
            piece=get(u,table) if match else 32000
            word(u,r+18,piece);tile=21610+get(u,table+4);byte(u,0xacf85c+tile,noise)
        calls=[]
        def draw(cpu,args,t):
            calls.append((t,args))
            if mutate and len(calls)==1:
                put(cpu,r+12,22610);byte(cpu,r+85,8);byte(cpu,r+91,15)
            return 0
        for t,n in [(0x47bf50,7),(0x4f3f70,4)]:hook(c,state,t,n,lambda cpu,args,t=t:draw(cpu,args,t))
        run(c,u,0x4f4160,[400,1,kind]);values.append((calls,snapshot(u)))
    compare(values,('finished',mode,kind,phase,mask,mutate,match,noise))


def refresh(mode,kind,rotation,mark=False,mutate=False):
    values=[]
    for c,u,state in machines:
        prepare(u,state);r=record(u,building=300)
        byte(u,r+84,kind);put(u,0x5e4ef4,mode);put(u,0xbca2e0,rotation*2)
        calls=[]
        def draw(cpu,args):
            calls.append(('image',args,bytes(cpu.mem_read(0x7999d8+args[0]*264+84,8))))
            if mutate and len(calls)==1:
                put(cpu,0xbca2e0,6);byte(cpu,r+84,4);put(cpu,0x5e4ef4,5)
                if mark:
                    descriptor=(0x5f6d58 if mode==5 else 0x5f6cd0)+kind*8
                    put(cpu,descriptor,S);put(cpu,descriptor+4,2)
                    put(cpu,S,3);put(cpu,S+8,7)
            return 0
        intercept(c,state,0x4edf50,'restore_pyramid_update_piece_image',3,draw)
        hook(c,state,0x4ecea0,1,lambda cpu,args:calls.append(('allowed',args)) or 0x12340001)
        hook(c,state,0x4ed360,4,lambda cpu,args:calls.append(('brush',args)) or 0)
        run(c,u,0x4f3c90 if mark else 0x4ee4a0,[300],budget=5000000)
        values.append((calls,snapshot(u)))
    compare(values,('refresh',mode,kind,rotation,mark,mutate))


def construction(mode,kind,mutate=False):
    values=[]
    for c,u,state in machines:
        prepare(u,state);r=record(u,building=300)
        put(u,0x5e4ef4,mode);put(u,0x5e6970,kind);put(u,0xeaac28,259);put(u,0x5d17a8,0x123456)
        calls=[];next_id=301
        def allocate(cpu,args):
            nonlocal next_id
            calls.append(('allocate',args));index=next_id;next_id+=1;off=index*264
            word(cpu,0x7999e8+off,args[1]);word(cpu,0x7999de+off,args[2]);word(cpu,0x7999e0+off,args[3])
            put(cpu,0x7999e4+off,1000+args[3]*228+args[2]);return index
        intercept(c,state,0x46a400,'restore_building_allocate_0046a400',6,allocate)
        def update(cpu,args):
            calls.append(('update',args))
            if mutate and args[0]==301:put(cpu,0x5e4ef4,5);put(cpu,0x5e6970,0)
            return 0
        hook(c,state,0x485100,1,update);hook(c,state,0x4f4f80,1,lambda cpu,args:calls.append(('slot',args)) or 0)
        hook(c,state,0x4ecea0,1,lambda cpu,args:calls.append(('allowed',args)) or 0)
        intercept(c,state,0x470990,'restore_placement_create_00470990',8,
                  lambda cpu,args:calls.append(('entrance',args)) or 0)
        # All seven reconstructed functions execute, including image selection,
        # special descriptor lookups, exact map writes and entrance allocation.
        run(c,u,0x4ee780,[259,90,90,0x12345678,300],budget=10000000)
        assert any(call[0]=='entrance' for call in calls)
        values.append((calls,snapshot(u)))
    compare(values,('construction',mode,kind,mutate))


def entrance(mode,kind,building_kind=183,mutate=False,negative=False):
    values=[]
    for c,u,state in machines:
        prepare(u,state);r=record(u,building=300);selected=307
        byte(u,r+84,kind);word(u,0x7999e8+selected*264,building_kind);word(u,0x7999ea+selected*264,7)
        for a,v in [(0x5e4ef4,mode),(0x5d17a8,0x123456),(0xeaac28,259),
                    (0xbca2e8,300),(0xbca2ec,400)]:put(u,a,v)
        if negative:put(u,r+12,-30000)
        calls=[]
        def allocate(cpu,args):
            calls.append(('allocate',args))
            if mutate:put(cpu,0xeaac28,261);put(cpu,0x5d17a8,0x87654321)
            return 500
        intercept(c,state,0x46a400,'restore_building_allocate_0046a400',6,allocate)
        hook(c,state,0x485100,1,lambda cpu,args:calls.append(('update',args)) or 0)
        intercept(c,state,0x470990,'restore_placement_create_00470990',8,
                  lambda cpu,args:calls.append(('create',args)) or 0)
        run(c,u,0x4f5f10,[selected]);values.append((calls,snapshot(u)))
    compare(values,('entrance',mode,kind,building_kind,mutate,negative))


if __name__=='__main__':
    count=0
    for size in range(7):
        for x,y in [(0,0),(90,90),(300,0),(-1,1),(1,-1),(400-size,400-size),(401-size,401-size)]:
            footprint(x,y,size);count+=1
    footprint(90,90,2,512,512);count+=1
    print('PASS footprint: %d native/C bounds, zero-size, WORD ownership and DWORD scratch cases'%count,flush=True)
    count=0
    for mode,shape in itertools.product([0,1,2,3,4,5,257],range(10)):
        for phase in [0,1]:
            image(mode,shape,(mode+shape)%4,phase=phase,alternate=phase);count+=1
    for mode,workers in itertools.product([1,2,3],[(0,0),(1,0),(0,4),(2,3),(4,6),(255,255)]):
        image(mode,5,0,workers=workers,alternate=1);count+=1
    for shape,rotation in itertools.product([0,3,4,7,9],range(4)):
        image(3,shape,rotation,phase=0,step=3,layer=0,height=2);count+=1
    for mode,piece in itertools.product([1,3,5],[0,5]):
        image(mode,5,1,step=0,piece=piece,mutate=True);count+=1
    for mode,kind in [(1,k) for k in range(1,5)]+[(5,1),(5,2)]:
        image(mode,5,2,kind=kind,alternate=1);count+=1
    print('PASS piece images/workers: %d native/C shapes, sprite branches, signed half offsets and reloads'%count,flush=True)
    count=0
    for phase,mask in itertools.product([2,3,4,5,6,7,8,9,10,11,12],[0,5,15]):
        finished(1,0,phase,mask,mutate=True);count+=1
    for mode,kind in [(1,4),(5,0),(5,1),(5,2)]:
        for match,noise in [(True,0),(True,1),(False,1)]:
            finished(mode,kind,3,match=match,noise=noise);count+=1
    print('PASS finished parts: %d native/C descriptor, mask and phase cases'%count,flush=True)
    count=0
    for mode,kind in [(1,i) for i in range(5)]+[(5,i) for i in range(3)]:
        for mark,mutate in itertools.product([False,True],[False,True]):
            refresh(mode,kind,1,mark,mutate);count+=1
    print('PASS whole refresh/special parts: %d native/C complete argument and cached/reloaded table cases'%count,flush=True)
    count=0
    for mode in [0,1,2,3,4,5,257]:
        for kind in range(3 if mode==5 else 5):
            construction(mode,kind);count+=1
    # Root kind2 exists in both families. Kind4 + a synthetic switch to
    # family5 is invalid even in the original (that family has only 3 kinds).
    construction(1,2,True);construction(5,2,True);count+=2
    print('PASS complete seven-function placement composition: %d native/C cases with all parts and entrance'%count,flush=True)
    count=0
    for mode,kind in [(1,i) for i in range(5)]+[(5,i) for i in range(3)]:
        for building_kind in [183,0x1b7]:
            entrance(mode,kind,building_kind,mutate=True,negative=True);count+=1
    print('PASS entrance: %d native/C full kind, owner/size lifetime and signed coordinate cases'%count,flush=True)
    print('target SHA256',target_sha)
