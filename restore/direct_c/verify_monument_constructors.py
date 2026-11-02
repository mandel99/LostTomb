"""Complete native/C monument construction, without launching a host game.

Execute the actual native array-element constructor, base constructor, record
copy and destructor. Capture only incoming private rectangle prestate for the
explicit C body. Public wrappers define those native-uninitialized words as
zero; comparisons mask precisely those words until actual geometry derives
the piece fields. All other object and global bytes remain compared.
"""
from verify_options_startup import *

OBJECT=S+0x1000;SCRATCH=S+0x22000
CLASSES=[(0x40d280,0x6fd5d8,0x4c,25,0x40d7e0,0x413c80),
 (0x401030,0x6de650,0x4c,16,0x4013f0,0x413e00),
 (0x416c20,0x6fd368,0x4c,9,0x416f60,0x413cc0),
 (0x4e6400,0x6de548,0x4c,2,0x4e6590,0x413e40),
 (0x5430e0,0x6f9320,0x1e5c,167,0x5456a0,0x413d00),
 (0x5567d0,0x6f5648,0x1cbc,158,0x558b40,0x413d40),
 (0x54fef0,0x6de9e0,0x4ee0,437,0x555de0,0x413dc0),
 (0x5480e0,0x6e9188,0x5c88,513,0x54f3d0,0x413d80)]
ORDER=[0,2,4,5,7,6,1,3]
for c,u,state in machines:
    u.mem_map(0,4096)

def prepare(u,state,pattern=0xa5):
    reset(u,state);put(u,0,0x12345678)
    u.mem_write(SP-0x6000,bytes([pattern])*0x6000)
    u.mem_write(OBJECT,bytes([pattern^0x3c])*0xd000)
    put(u,0x70e44c,271);put(u,0x70e45c,283)

def random_endpoint(c,state,mode):
    calls=[]
    if mode=='real':return calls
    def random(cpu,args):
        i=len(calls);calls.append(i)
        if mode=='mutation':
            put(cpu,OBJECT+0x54,0x12345678+i)
            return (i*57+31)&0x7fff
        return [0,1,4,5,32767,0xffffffff,0x80000000,0x7fffffff][i%8]
    hook(c,state,0x562348,0,random)
    return calls

def capture_private(u):
    captured=[]
    def observe(cpu,pc,size,data):
        if pc==0x4e5120:
            p=cpu.reg_read(UC_X86_REG_ECX)
            captured.append(bytes(cpu.mem_read(p,52)))
    handle=u.hook_add(UC_HOOK_CODE,observe)
    return captured,handle

def mask(u,object,definition,geometry):
    # Base record's nine inherited, uninitialized temporary words.
    u.mem_write(object+0x1c,bytes(36))
    if not geometry:
        _,_,offset,count,_,_=definition
        for i in range(count):u.mem_write(object+offset+i*52+16,bytes(36))

def one(index,pattern=0xa5,mode='sequence',public=False,geometry=False,global_thunk=False):
    definition=CLASSES[index];token,target,offset,count,project,thunk=definition
    object=target if global_thunk else OBJECT
    results=[];native_frames=None
    for c,u,state in machines:
        prepare(u,state,pattern);put(u,0x604950,0x12345678+pattern)
        calls=random_endpoint(c,state,mode)
        if not c:
            frames,handle=capture_private(u)
            value=run(0,u,thunk if global_thunk else token,receiver=None if global_thunk else object)
            u.hook_del(handle)
            assert len(frames)==count+1
            assert all(p[16:]==frames[1][16:] for p in frames[1:])
            native_frames=frames[:2]
        elif public or global_thunk:
            value=run(1,u,thunk if global_thunk else token,[] if global_thunk else [object])
        else:
            u.mem_write(SCRATCH,b''.join(native_frames))
            value=run(0,u,sym('restore_monument_construct_body_%08x'%token),[object,SCRATCH])
            u.mem_write(SCRATCH,bytes(104))
        assert value==object and get(u,0)==0x12345678
        if geometry:run(c,u,project,[object] if c else [],receiver=None if c else object)
        if public or global_thunk:mask(u,object,definition,geometry)
        results.append(((value,tuple(calls),bytes(u.mem_read(OBJECT,0xd000))),snapshot(u)))
    if results[0]!=results[1]:
        a,b=results[0],results[1]
        region=0x560000 if a[-1]!=b[-1] else OBJECT
        x,y=(a[-1],b[-1]) if region==0x560000 else (a[0][-1],b[0][-1])
        raise AssertionError((hex(token),pattern,mode,public,geometry,global_thunk,
            'calls',len(a[0][1]),len(b[0][1]),
            'diff',[(hex(region+i),v,w) for i,(v,w) in enumerate(zip(x,y)) if v!=w][:16]))

def helpers():
    n=0
    for delta,pattern in itertools.product([-52,-36,-20,-16,-4,0,4,8,16,20,32,36,48,52],[0,0xa5]):
        results=[]
        for c,u,state in machines:
            prepare(u,state,pattern);p=S+0x18000
            u.mem_write(p-256,bytes((i*31+pattern)&255 for i in range(1024)))
            value=run(c,u,0x4e5140,[p+delta,p] if c else [p],cleanup=0 if c else 4,receiver=None if c else p+delta)
            assert value==p+delta
            results.append((bytes(u.mem_read(S,0x20000)),snapshot(u)))
        equal(results,('record copy overlap',delta,pattern));n+=1
    for args in [(0,0,1,7),(-1,0x7fffffff,0x80000000,255),(31,-32,5,512)]:
        results=[]
        for c,u,state in machines:
            prepare(u,state)
            value=run(c,u,0x4e5120,([OBJECT] if c else [])+list(args),cleanup=0 if c else 16,receiver=None if c else OBJECT)
            assert value==OBJECT
            results.append((bytes(u.mem_read(OBJECT,64)),snapshot(u)))
        equal(results,('record arguments and return',args));n+=1
    for pattern in [0,0x5a,0xff]:
        results=[]
        for c,u,state in machines:
            prepare(u,state,pattern)
            value=run(0,u,sym('restore_monument_zero_record'),[OBJECT]) if c else run(0,u,0x4e50f0,receiver=OBJECT)
            assert value==OBJECT
            results.append((bytes(u.mem_read(OBJECT,64)),snapshot(u)))
        equal(results,('record zero',pattern));n+=1
    for token,pattern,public in itertools.product([0x4e5210,0x545f80],[0,0x5a,0xff],[False,True]):
        results=[];incoming=None
        for c,u,state in machines:
            prepare(u,state,pattern)
            if not c:
                frames,handle=capture_private(u)
                value=run(0,u,token,receiver=OBJECT);u.hook_del(handle)
                assert len(frames)==1;incoming=frames[0]
            elif public:value=run(1,u,token,[OBJECT])
            else:
                u.mem_write(SCRATCH,incoming)
                run(0,u,sym('restore_monument_class_base'),[OBJECT,SCRATCH,int(token==0x545f80)])
                value=OBJECT
            assert value==OBJECT and get(u,0)==0x12345678
            if public:u.mem_write(OBJECT+0x1c,bytes(36))
            results.append((bytes(u.mem_read(OBJECT,96)),snapshot(u)))
        equal(results,('whole base constructor',hex(token),pattern,public));n+=1
    print('PASS complete record initialization/copy/argument helpers, including overlap:',n,flush=True)
    return n

def initialize_all(seed=1,mode='real'):
    results=[]
    for c,u,state in machines:
        prepare(u,state);put(u,0x604950,seed)
        calls=random_endpoint(c,state,mode)
        if c:run(0,u,sym('restore_monument_constructors_initialize'))
        else:
            for index in ORDER:run(0,u,CLASSES[index][5])
        for definition in CLASSES:
            token,object,offset,count,project,thunk=definition
            run(c,u,project,[object] if c else [],receiver=None if c else object)
            mask(u,object,definition,True)
        assert get(u,0)==0x12345678
        results.append((tuple(calls),snapshot(u)))
    equal(results,('all constructor initialization and actual geometry',seed,mode))

def constructors():
    n=0
    for index,pattern in itertools.product(range(8),[0,0x5a,0xa5,0xff]):
        one(index,pattern);n+=1
    for index in range(8):
        one(index,mode='real');n+=1
        one(index,mode='mutation');n+=1
        one(index,public=True);n+=1
        one(index,public=True,geometry=True);n+=1
        one(index,global_thunk=True,geometry=True);n+=1
    for seed in [1,0xdeadbeef,0xffffffff]:initialize_all(seed);n+=1
    initialize_all(mode='sequence');n+=1
    print('PASS eight full constructors, explicit private prestate, public/static receivers, real random and geometry:',n,flush=True)
    return n

if __name__=='__main__':
    print('PASS total:',helpers()+constructors(),'target SHA256',target_sha,flush=True)
