"""Native/C monument footprint, resource consumption and whole refresh.

The complete shape masks and tile updates execute. Provider-controlled cases
check receiver/flags and callback lifetimes; compositions also use actual image
queries, activity and rectangle stamping. No host game or renderer runs.
"""
from verify_options_startup import *

receivers=[0x6f9320,0x6f5648,0x6de9e0,0x6e9188]
image_queries=[0x545760,0x558c00,0x555ea0,0x54f490]
piece_counts=[0x545f60,0x559460,0x5567b0,0x54fed0]


def prepare(u,state):
    reset(u,state);u.mem_write(SP-0x5000,b'\xa5'*0x5000)
    for reg,value in [(UC_X86_REG_FPCW,0x37f),(UC_X86_REG_FPSW,0),(UC_X86_REG_FPTAG,0xffff)]:u.reg_write(reg,value)
    for i in range(36):put(u,0x5cc8c0+i*8,(i//6)*228+i%6);put(u,0x5cc8c4+i*8,0x12345603+i)
    for a,v in [(0xbca2d8,1000),(0xbca2d0,400),(0xbca2d4,400),(0xe94e0c,0),
                (0xf56b54,0x12340000),(0xf572ec,0x23450000),(0xe94e04,0)]:put(u,a,v)
    for i,self in enumerate(receivers):
        table=S+0x1000+i*0x100;put(u,self,table);put(u,self+80,7);put(u,self+84,0xa5a5a5a5)
        put(u,table+0x1c,image_queries[i]);put(u,table+0x24,0x5477e0);put(u,table+0x38,piece_counts[i])
    for start,n,pattern in [(0xb285cc+16000,12000,b'\x9f'),(0xb67d1c+16000,12000,b'\x8b'),
            (0xacf85c+16000,12000,b'\xfd'),(0xb7482c+16000*4,12000*4,b'\xa5'),
            (0xaf598c+16000*4,12000*4,b'\x96'),(0xb4e6fc+16000*2,12000*2,b'\xa5')]:
        u.mem_write(start,pattern*n)


def record(u,id=400,piece=1,size=2,shape=1,rotation=0,completed=0):
    r=0x7999d8+id*264;u.mem_write(r,bytes(264))
    word(u,r+6,90);word(u,r+8,80);word(u,r+16,229);word(u,r+18,piece)
    byte(u,r+3,size);put(u,r+104,shape);put(u,r+144,rotation)
    put(u,0x799a30+(id-piece)*264,completed)
    return r


def endpoint(c,state,token,name,n,action,cleanup=0):
    state['hooks'][sym(name) if c else token]=(n,0 if c else cleanup,action)


def commit(c,u,self,id):
    return run(c,u,0x5468d0,[self,id] if c else [id],cleanup=0 if c else 4,receiver=None if c else self)


def consume_resource(c,u):
    if not c:return run(c,u,0x492fc0)
    # MSVC inlines the tiny address wrapper into its public C callers, so
    # this Release retains the named canonical body rather than that alias.
    for r,v in zip(registers,[0x11111111,0x22222222,0x33333333,saved[0],SP,*saved[1:]]):u.reg_write(r,v)
    u.reg_write(UC_X86_REG_EFLAGS,2);put(u,SP,STOP)
    u.emu_start(sym('restore_monument_consume_resource'),STOP,count=2000000)
    assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==SP+4
    assert [u.reg_read(registers[i]) for i in [3,5,6,7]]==saved


def providers(c,u,state,calls,r,self,flags,image,mutate='',real_image=False,real_activity=False,real_stamp=False):
    if not real_activity:
        def activity(cpu,args):
            receiver,building=args if c else (cpu.reg_read(UC_X86_REG_ECX),args[0])
            calls.append(('activity',receiver,building))
            if mutate=='activity':word(cpu,r+18,2);byte(cpu,r+3,3);put(cpu,self+80,2)
            return 0x123456ab
        endpoint(c,state,0x547ba0,'restore_placement_activity_00547ba0',2 if c else 1,activity,4)
    if not real_image:
        def query(cpu,args):
            receiver,building,pointer=args if c else (cpu.reg_read(UC_X86_REG_ECX),*args)
            calls.append(('image',receiver,building));put(cpu,pointer,flags)
            if mutate=='image':
                word(cpu,r+6,100);word(cpu,r+8,81);byte(cpu,r+3,5);put(cpu,0xbca2d8,2000)
            return image
        if c:state['hooks'][sym('restore_placement_image_query')]=(3,0,query)
        else:
            put(u,get(u,self)+0x1c,S+0x49000);state['hooks'][S+0x49000]=(2,8,query)
    def variant(cpu,args):
        calls.append(('variant',args))
        if mutate=='variant':put(cpu,0xf572ec,get(cpu,0xf572ec)+256);put(cpu,0xbca2d8,777)
        return (args[0]%9)-3
    hook(c,state,0x475fd0,1,variant)
    def stamp(cpu,args):
        calls.append(('stamp',args))
        if mutate=='stamp':
            word(cpu,r+6,101);word(cpu,r+8,82);put(cpu,0xbca2d8,2000)
            put(cpu,r+104,22);put(cpu,r+144,3)
        return 0
    if not real_stamp:hook(c,state,0x47bf50,7,stamp)
    else:hook(c,state,0x4767d0,6,lambda cpu,args:calls.append(('redraw bounds',args)) or 0)


def compare(values,label):
    assert values[0][0]==values[1][0],(label,'calls',values[0][0],values[1][0])
    equal(values,label)


def footprint(shape=1,rotation=0,size=2,piece=1,completed=0,terminal=7,mode=0,
        flags=0,image=0x34560000,mutate='',real_image=False,real_activity=False,real_stamp=False,cls=0):
    values=[]
    for c,u,state in machines:
        prepare(u,state);r=record(u,piece=piece,size=size,shape=shape,rotation=rotation,completed=completed)
        self=receivers[cls];word(u,r+16,[229,234,235,236][cls]);put(u,self+80,terminal);put(u,0xe94e0c,mode)
        calls=[];providers(c,u,state,calls,r,self,flags,image,mutate,real_image,real_activity,real_stamp)
        commit(c,u,self,400);values.append((calls,snapshot(u)))
    compare(values,('footprint',shape,rotation,size,piece,completed,terminal,mode,flags,image,mutate,real_image,real_activity,real_stamp,cls))


def refresh(resource=False,count=6,mutate=False,real=False,query=0,material=0):
    values=[]
    for c,u,state in machines:
        prepare(u,state);put(u,0x78d790,count)
        for id,kind in enumerate([0,229,234,235,236,0x1e5,229,234]):
            r=record(u,id=id,piece=1,size=1,shape=27);word(u,r+16,kind)
        calls=[]
        if resource:
            for i in range(36):put(u,0x784e78+i*4,0);put(u,0x5d2c48+i*8,0)
            if material:
                for i in range(36):
                    put(u,0x784e78+i*4,1 if material!=3 else -1)
                    put(u,0x5d2c48+i*8,256 if material==2 else i*17)
                if material==4:
                    put(u,0x5d2c48+1*8,-1);put(u,0x5d2c48+30*8,-2)
            hook(c,state,0x4ea930,0,lambda cpu,args:calls.append(('activity query',)) or (0x12340000|query))
        if not real:
            def observe(cpu,args):
                receiver,building=args if c else (cpu.reg_read(UC_X86_REG_ECX),args[0])
                calls.append(('footprint',receiver,building))
                if mutate and building==1:
                    put(cpu,0x78d790,3);word(cpu,0x7999e8+2*264,236)
                return 0
            endpoint(c,state,0x5468d0,'restore_monument_commit_footprint',2 if c else 1,observe,4)
        else:
            # Both loops compose the complete new footprint body; image and
            # activity providers remain controlled at their documented ABI.
            for self in receivers:
                providers(c,u,state,calls,0,self,1,0x34560000)
                if not c:put(u,get(u,self)+0x1c,S+0x49000)
        if resource:consume_resource(c,u)
        else:run(c,u,0x547a60)
        values.append((calls,snapshot(u)))
    compare(values,('refresh',resource,count,mutate,real,query,material))


if __name__=='__main__':
    n=0
    for shape,rotation in itertools.product(list(range(31))+[257],[0,1,2,3,4,256]):
        footprint(shape,rotation);n+=1
    print('PASS shapes: %d native/C every footprint type, orientation and full DWORD selector'%n,flush=True)
    n=0
    for size,flags,image in itertools.product(range(7),[1,0x101,2],[0,0x12340000,0x34560000]):
        footprint(size=size,flags=flags,image=image);n+=1
    for completed,terminal,mode in itertools.product([1,257],[1,7,0x10001],[0,51,307]):
        footprint(completed=completed,terminal=terminal,mode=mode);n+=1
    for size in [0,1,3,6]:footprint(size=size,completed=1,mutate='variant');n+=1
    for mutation in ['activity','image','stamp']:
        footprint(piece=0,mutate=mutation,flags=1 if mutation=='image' else 0);n+=1
    print('PASS tile/flag/finished branches: %d native/C cached size, root, image and callback cases'%n,flush=True)
    n=0
    for cls,shape,flags in itertools.product(range(4),[1,2,19,22,26,27],[0,1]):
        footprint(cls=cls,shape=shape,flags=flags,real_stamp=True);n+=1
    print('PASS actual rectangle stamping: %d native/C full map write compositions'%n,flush=True)
    n=0
    for cls,piece in itertools.product(range(4),[0,1,3]):
        footprint(cls=cls,piece=piece,real_image=True,real_activity=True,real_stamp=True);n+=1
    print('PASS actual image/activity/stamping chain: %d native/C complete provider compositions'%n,flush=True)
    n=0
    for resource,count,query in itertools.product([False,True],[-1,0,1,5,6,7],[0,1]):
        refresh(resource,count,query=query);n+=1
    for resource in [False,True]:
        refresh(resource,mutate=True);n+=1
        refresh(resource,real=True);n+=1
    for material,query in itertools.product([1,2,3,4],[0,1]):refresh(True,query=query,material=material);n+=1
    print('PASS resource and whole-refresh loops: %d native/C bounds, classes, material ranking and compositions'%n,flush=True)
    print('target SHA256',target_sha)
