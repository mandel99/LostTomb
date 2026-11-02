"""Whole native/C mausoleum placement, image, rotation and completion routines.

The actual 13-DWORD geometry copier executes. Allocation, actor routing and
terrain refresh are controlled boundaries; image cases additionally compose
the actual image selector while controlling only its sprite-library loader.
This checks data/calls, not host gameplay or physical rendering.
"""
from verify_options_startup import *


def prepare(u,state):
    reset(u,state)
    u.mem_write(SP-0x5000,b'\xa5'*0x5000)


def canonical_hook(c,state,token,name,count,action):
    state['hooks'][sym(name) if c else token]=(count,0,action)


def compare(outputs,label):
    assert outputs[0][0]==outputs[1][0],(label,'calls',
        next(((i,a,b) for i,(a,b) in enumerate(zip(outputs[0][0],outputs[1][0])) if a!=b),None),
        [len(v[0]) for v in outputs])
    equal(outputs,label)


def placement(rotation,loaded,origin=(90,80),mutate=False):
    outputs=[];root=300
    for c,u,state in machines:
        prepare(u,state)
        for a,v in [(0xbca2e0,rotation),(0x702798,0xa5a5c123),(0xbca2d8,1000),(0xeaac28,259)]:put(u,a,v)
        u.mem_write(0x7999d8+root*264,b'\xa5'*(120*264))
        calls=[];next_id=root+1
        hook(c,state,0x4f4f80,1,lambda cpu,args:calls.append(('slot',args)) or 0x12345678)
        def allocate(cpu,args):
            nonlocal next_id
            calls.append(('allocate',args))
            index=next_id;next_id+=1
            # This is an allocator-owned boundary, including x/y/tile fields.
            off=index*264;word(cpu,0x7999de+off,args[2]);word(cpu,0x7999e0+off,args[3])
            put(cpu,0x7999e4+off,1000+args[3]*228+args[2]);return index
        canonical_hook(c,state,0x46a400,'restore_building_allocate_0046a400',6,allocate)
        def refresh(cpu,args):
            calls.append(('image',args,bytes(cpu.mem_read(0x7999d8+args[0]*264,264))))
            return 0
        def update(cpu,args):
            calls.append(('update',args))
            if mutate and args[0]==root:
                put(cpu,0xbca2e0,6);put(cpu,0xeaac28,261)
            return 0x87654321
        hook(c,state,0x4dfbb0,1,refresh);hook(c,state,0x485100,1,update)
        run(c,u,0x4df9d0,[*origin,root,loaded],budget=5000000)
        assert next_id==root+116
        outputs.append((calls,snapshot(u)))
    compare(outputs,('place',rotation,loaded,origin,mutate))


def image(piece,rotation,variant,real_image=False):
    outputs=[];building=400;off=building*264
    for c,u,state in machines:
        prepare(u,state)
        for a,v in [(0xbca2e0,rotation),(0xbca2d8,1000),(0x799a48+off,4),
                    (0x799a30+off,variant%3),(0xe94e18,1),(0x784cbc,3),
                    (0xf56b54,40000),(0xf5736c,50000),(0xf57464,60000),
                    (0xf56d54,70000),(0xf56d5c,71000),(0xf56d64,72000),(0xf56d6c,73000),
                    (0xf57444,74000),(0xf5744c,75000),(0xf57454,76000),(0xf5745c,77000),
                    (0xf56d74,78000)]:put(u,a,v)
        word(u,0x7999ea+off,piece);word(u,0x7999de+off,80);word(u,0x7999e0+off,90)
        word(u,0x7999ee+off,-1 if variant%2 else 1)
        calls=[]
        if real_image:
            def load(cpu,args):
                if c:assert args[0]==0xf55bb0;args=args[1:]
                else:assert cpu.reg_read(UC_X86_REG_ECX)==0xf55bb0
                calls.append(('library',args));return 1
            hook(c,state,0x53dd90,3 if c else 2,load,cleanup=0 if c else 8)
        else:
            def query(cpu,args):
                assert args[0]==building
                put(cpu,args[1],0x12340001 if variant else 0x12340000)
                return [12345,0,40000,50000,60000,70000][variant]
            hook(c,state,0x4df360,2,query)
        for token,n in [(0x47bf50,7),(0x475bf0,4),(0x4767d0,6),(0x477500,4)]:
            hook(c,state,token,n,lambda cpu,args,t=token:calls.append((t,args)) or 0)
        # BYTE side maps and the two WORD ownership arrays must not be aliased
        # as DWORDs. Seed the whole neighborhood with recognizable high bits.
        for tile in range(21000,23000):
            put(u,0xaf598c+tile*4,0xefffffff)
            byte(u,0xb285cc+tile,0xff);byte(u,0xb67d1c+tile,0xa5)
            byte(u,0xacf85c+tile,tile&7);word(u,0xb4e6fc+tile,0x7654)
        run(c,u,0x4dfbb0,[building],budget=5000000)
        outputs.append((calls,snapshot(u)))
    compare(outputs,('image',piece,rotation,variant,real_image))


def position(kind,state_value,rotation=2,related_mode=0,mutate=False,worker=600):
    outputs=[];building=400;off=building*264;actor=worker*388;other=777
    for c,u,state in machines:
        prepare(u,state)
        for a,v in [(0xbca2e0,rotation),(0xbca2d8,1000),(0x799a48+off,4),
                    (0x799a40+off,-300),(0x799a44+off,400),
                    (0x799a34+off,worker),(0xd056f8,910)]:put(u,a,v)
        word(u,0x7999ea+off,80);byte(u,0xbcd602+actor,kind);byte(u,0xbcd630+actor,state_value)
        word(u,0xbcd63c+actor,0x7654);word(u,0xbcd63c+other*388,0x6543)
        word(u,0xbcd660+910*388,0 if related_mode==0 else other)
        byte(u,0xbcd602+other*388,85 if related_mode!=2 else 86)
        byte(u,0xbcd630+other*388,26 if related_mode!=3 else 27)
        calls=[]
        def callback(cpu,args,t):
            calls.append((t,args))
            if mutate and t==0x4b5420:
                word(cpu,0x7999de+off,-302);word(cpu,0x7999e0+off,-200)
                put(cpu,0x7999e4+off,0x123456)
            if mutate and t==0x4b52f0:
                byte(cpu,0xbcd630+actor,10)
                word(cpu,0x7999fe+off,32000);word(cpu,0x799a00+off,-30000)
                put(cpu,0x7999e4+off,0x89abcd)
            if t==0x4b3250:word(cpu,0xbcd63c+args[0]*388,0x1111)
            return 0
        for t in [0x4b5420,0x4b52f0,0x4b3250,0x4dfbb0]:
            hook(c,state,t,1,lambda cpu,args,t=t:callback(cpu,args,t))
        run(c,u,0x4dfd90,[building])
        outputs.append((calls,snapshot(u)))
    compare(outputs,('position',kind,state_value,rotation,related_mode,mutate,worker))


def completion(piece,kind,instant=False):
    outputs=[];root=300;building=root+piece
    for c,u,state in machines:
        prepare(u,state);word(u,0x7999ea+building*264,piece);word(u,0x7999e8+building*264,kind)
        calls=[]
        hook(c,state,0x4dfbb0,1,lambda cpu,args:calls.append(args) or 0)
        run(c,u,0x4df2c0 if instant else 0x4e0c80,[building],budget=5000000)
        assert len(calls)==(116 if kind==222 else 0)
        if calls:assert calls[0]==(root,) and calls[-1]==(root+115,)
        outputs.append((calls,snapshot(u)))
    compare(outputs,('complete',piece,kind,instant))


if __name__=='__main__':
    count=0
    for rotation,loaded in itertools.product([0,2,4,6],[0,1,0x100,0x101]):
        placement(rotation,loaded);count+=1
    for rotation in [0,2,4,6]:
        placement(rotation,1,(-35000,40000),True);count+=1
    print('PASS placement: %d native/C cases, all 116 parts and allocation/link ordering'%count,flush=True)
    count=0
    for piece,rotation,variant in itertools.product([0,80],[0,2,4,6],range(6)):
        image(piece,rotation,variant);count+=1
    print('PASS whole image/map update: %d native/C footprint, terrain, image and BYTE/WORD cases'%count,flush=True)
    count=0
    by_kind={}
    for index in range(116):
        kind=struct.unpack_from('<I',images[0].data,0x5e2ae4-images[0].base+index*52)[0]&255
        by_kind.setdefault(kind,index)
    for piece,rotation in itertools.product(by_kind.values(),[0,2,4,6]):
        image(piece,rotation,1,real_image=True);count+=1
    print('PASS actual image-selector composition: %d native/C cases, all 14 part subtypes'%count,flush=True)
    count=0
    for kind,state_value in itertools.product([79,81,85,86],[0,9,10,11,26,27,28]):
        position(kind,state_value,related_mode=1);count+=1
    for rotation,mode in itertools.product([0,2,4,6],range(4)):
        position(81,10,rotation,mode,True);count+=1
    for kind,state_value in [(79,10),(85,28),(81,11),(81,9),(85,27)]:
        position(kind,state_value,related_mode=1,mutate=True);count+=1
    position(79,9,worker=0);count+=1
    print('PASS part/worker repositioning: %d native/C states, related carriers and post-callback reloads'%count,flush=True)
    count=0
    for piece,kind,instant in itertools.product([0,7,115],[222,0x1de,221],[False,True]):
        completion(piece,kind,instant);count+=1
    print('PASS complete/refresh: %d native/C cases, whole WORD kinds and all 116 parts'%count,flush=True)
    print('target SHA256',target_sha)
