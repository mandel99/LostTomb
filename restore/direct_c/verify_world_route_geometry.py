"""Whole native/C world-route editing, hit testing and city binding, offline.

Only allocation/free endpoints are controlled. Public entries share the
existing controller providers. Native comparisons use healthy allocations;
C-only checks cover the loader's defined guards and diagonal-capacity repair.
No host game, rendering, data or save writes occur.
"""
from verify_options_startup import *

SELF=0x776fb0+3*324;HEAP=0x33000000
for c,u,state in machines:u.mem_map(HEAP,0x200000)

def prepare(u,state,points=(),spacing=5,capacity=50000,samples=(),control=0x37f):
    reset(u,state);u.mem_write(SP-0x5000,b'\xa5'*0x5000)
    u.mem_write(HEAP,b'\xa5'*0x200000);u.mem_write(SELF,b'\xa5'*324)
    u.mem_write(0x77f748,bytes(200*98))
    byte(u,SELF,spacing);put(u,SELF+4,HEAP);put(u,SELF+0x134,len(samples))
    put(u,SELF+0x138,capacity);byte(u,SELF+0x141,len(points))
    for i,(x,y) in enumerate(points):
        u.mem_write(SELF+8+i*6,struct.pack('<hhH',x,y,0x9080+i))
    for i,(x,y,segment) in enumerate(samples):
        u.mem_write(HEAP+i*6,struct.pack('<hhBB',x,y,segment,0xcc))
    for reg,value in [(UC_X86_REG_FPCW,control),(UC_X86_REG_FPSW,0),(UC_X86_REG_FPTAG,0xffff)]:u.reg_write(reg,value)
    put(u,0x126517c,3)
    for i,(w,h) in enumerate([(10,10),(20,30),(-5,-7)]):
        word(u,0x10d6a0c+i*64,w);word(u,0x10d6a0e+i*64,h)

def compare(result,label):
    if result[0][0]!=result[1][0]:raise AssertionError((label,'return/trace',result[0][0],result[1][0]))
    equal(result,label)

def erase_case(points,index,spacing=5,capacity=50000,pointer=True,mutate=False,control=0x37f,entry=0x4dd930,occupied=False):
    results=[]
    for c,u,state in machines:
        prepare(u,state,points,spacing,capacity,control=control);calls=[]
        if occupied:
            u.reg_write(UC_X86_REG_FPSW,3<<11)
            u.reg_write(UC_X86_REG_FP3,(0x8000000000000000,0x3fff))
            u.reg_write(UC_X86_REG_FP4,(0xc000000000000000,0x4000))
            u.reg_write(UC_X86_REG_FPTAG,0xffff&~(3<<6)&~(3<<8))
        live_fp=[u.reg_read(UC_X86_REG_FP3),u.reg_read(UC_X86_REG_FP4)] if occupied else []
        if not pointer:put(u,SELF+4,0)
        def free(cpu,args):
            calls.append(('free',args))
            if mutate:byte(cpu,SELF+0x141,1)
            return 0
        def malloc(cpu,args):calls.append(('malloc',args));return HEAP+0x100000
        hook(c,state,0x561a2b,1,free);hook(c,state,0x561a1d,1,malloc)
        if c:
            # Production's existing relocation pass routes controller CRT
            # calls to the independent host allocator. Same logical boundary.
            state['hooks'][sym('restore_heap_free')]=(1,0,free)
            state['hooks'][sym('restore_heap_allocate')]=(1,0,malloc)
        args=([SELF] if c else [])+([index] if entry==0x4dd930 else [])
        try:
            result=run(c,u,entry,args,cleanup=4 if not c and entry==0x4dd930 else 0,
                       receiver=None if c else SELF,budget=20000000)&255
        except Exception as error:
            raise AssertionError(('route execution',c,hex(entry),points,index,spacing,capacity,
                                  pointer,mutate,control,occupied,hex(u.reg_read(UC_X86_REG_EIP)))) from error
        assert u.reg_read(UC_X86_REG_FPCW)==control
        assert u.reg_read(UC_X86_REG_FPSW)&0x3800==(3<<11 if occupied else 0)
        if occupied:assert [u.reg_read(UC_X86_REG_FP3),u.reg_read(UC_X86_REG_FP4)]==live_fp
        # Include both the retained and replacement allocation, preserving pads.
        data=bytes(u.mem_read(HEAP,0x200000))
        results.append(((result,tuple(calls),data),snapshot(u)))
    compare(results,('route rebuild/removal',entry,points,index,spacing,capacity,pointer,mutate,control,occupied))

def erases():
    n=0
    shapes=[[],[(0,0)],[(0,0),(10,0)],[(0,0),(10,0),(20,0)],
            [(0,0),(0,0),(0,0)],[(0,0),(10,10),(20,20),(30,30)],
            [(-40,25),(17,-30),(-100,-80),(80,100)],
            [(-32768,-32768),(0,0),(32767,32767)],
            [(i*7,(-1 if i%2 else 1)*i*3) for i in range(50)]]
    for points in shapes:
        for index in sorted(set([-1,0,len(points)//2,len(points)-1,len(points)])):
            for spacing in [0,5,255]:erase_case(points,index,spacing);n+=1
    for points in [shapes[3],shapes[5],shapes[6]]:
        for capacity,pointer,mutate in [(-1,True,False),(0,True,False),(0,False,False),(0,True,True),(1,True,False),(100,True,False)]:
            erase_case(points,1,capacity=capacity,pointer=pointer,mutate=mutate);n+=1
        erase_case(points,1,spacing=1);n+=1
    # Every rounding mode with extended and53-bit precision. Native helper
    # truncates its return while preserving the incoming control word.
    for control in [0x37f,0x77f,0xb7f,0xf7f,0x27f,0x67f,0xa7f,0xe7f]:
        for points in [shapes[5],shapes[6],shapes[7]]:
            erase_case(points,1,spacing=7,control=control);n+=1
        erase_case(shapes[6],1,spacing=7,control=control,occupied=True);n+=1
    print('PASS complete point removal, allocation/reload paths and native x87 sampling:',n,flush=True)
    return n

def rebuilds():
    n=0
    for points in [[],[(0,0)],[(0,0),(10,10)],[(10,20),(-30,-40),(50,60)]]:
        for spacing in [0,5,255]:
            for capacity,mutate in [(50000,False),(0,False),(0,True)]:
                erase_case(points,0,spacing,capacity,mutate=mutate,entry=0x4d4720);n+=1
    for control in [0x37f,0x77f,0xb7f,0xf7f,0x27f,0x67f,0xa7f,0xe7f]:
        erase_case([(13,-17),(-40,55),(80,-120)],0,spacing=7,control=control,entry=0x4d4720);n+=1
    print('PASS shared mission-load rebuild, complete city scan and allocation count reload:',n,flush=True)
    return n

def hit_case(x,y,points=(),samples=(),alias=None):
    results=[]
    for c,u,state in machines:
        prepare(u,state,points,samples=samples)
        output=S+0x1000 if alias is None else SELF+alias
        if alias is None:byte(u,output,0xaa)
        if alias==0x134:put(u,SELF+0x134,256+len(samples))
        result=run(c,u,0x4ddbd0,[SELF,x,y,output] if c else [x,y,output],
                   cleanup=0 if c else 12,receiver=None if c else SELF)
        results.append(((result,bytes(u.mem_read(output,1))),snapshot(u)))
    compare(results,('hit',x,y,points,samples,alias))

def hits():
    n=0
    for x,y in itertools.product([-9,-8,-1,0,8,9,17,18,32767,-32768,0x7fffffff,-0x80000000],[-9,-8,0,8,9]):
        hit_case(x,y,points=[(0,0),(10,10)],samples=[(20,20,255),(0,0,37)]);n+=1
    for points,samples in [([],[]),([],[(0,0,255)]),([(100,100)],[(0,0,128)]),([(0,0),(0,0)],[(0,0,4)])]:
        for alias in [None,0x141,0x134]:hit_case(0,0,points,samples,alias);n+=1
    print('PASS vertex priority, inclusive hit bounds, segment bytes, signed wrap and output aliases:',n,flush=True)
    return n

def bind_case(points,cities,icon_count=3):
    results=[]
    for c,u,state in machines:
        prepare(u,state,points);put(u,0x126517c,icon_count)
        for index,kind,active,x,y,icon in cities:
            p=0x77f748+index*98;byte(u,p,kind);byte(u,p+1,active)
            word(u,p+4,x);word(u,p+6,y);word(u,p+12,icon)
        result=run(c,u,0x407f90,[SELF] if c else [],receiver=None if c else SELF)&255
        results.append((result,snapshot(u)))
    equal(results,('bind',points,cities,icon_count))

def bindings():
    n=0
    for a,b in itertools.product([-1,0,10,11,20,30],repeat=2):
        bind_case([(a,b),(a,b)],[(0,1,1,0,0,0),(199,1,1,0,0,1)]);n+=1
    for count in [-1,0,1,3,256]:
        for icon in [0,1,2,3,256,65535]:
            bind_case([(10,10),(20,30)],[(1,1,1,0,0,icon)],count);n+=1
    for kind,active in itertools.product([0,1,2],[0,1,255]):
        bind_case([(0,0)],[(5,kind,active,0,0,0)]);n+=1
    bind_case([],[(0,1,1,0,0,1)]);n+=1
    bind_case([(-32768,-32768),(32767,32767)],[(0,1,1,-32768,-32768,0),(199,1,1,32767,32767,0)]);n+=1
    print('PASS complete200-city scan, icon bounds/fallback, signed extents and later match priority:',n,flush=True)
    return n

def leaves():
    n=0
    for count in [0,1,50,255]:
        for token in [0x4dd8d0,0x4dd8e0,0x4dd900,0x4dd910,0x4dd920]:
            results=[]
            for c,u,state in machines:
                prepare(u,state);byte(u,SELF+0x141,count)
                word(u,SELF+0x13c,-32768);word(u,SELF+0x13e,32767)
                args=[SELF] if c else []
                if token==0x4dd900:args+=[0x12345678]
                value=run(c,u,token,args,cleanup=4 if not c and token==0x4dd900 else 0,receiver=None if c else SELF)
                results.append(((None if token==0x4dd900 else value),snapshot(u)))
            equal(results,('leaf',token,count));n+=1
    print('PASS five explicit-receiver leaf methods and preserved neighboring fields:',n,flush=True)
    return n

def safe_loader_cases():
    c,u,state=machines[1];n=0
    for mode in ['diagonal','allocation failure','malformed count']:
        points=[(0,0),(500,500)] if mode=='diagonal' else [(0,0),(10,10)]
        prepare(u,state,points,spacing=1,capacity=0);calls=[]
        put(u,SELF+4,0)
        if mode=='malformed count':byte(u,SELF+0x141,51)
        def malloc(cpu,args):calls.append(args[0]);return 0 if mode=='allocation failure' else HEAP
        hook(1,state,0x561a1d,1,malloc)
        state['hooks'][sym('restore_heap_allocate')]=(1,0,malloc)
        result=run(1,u,0x4d4720,[SELF],budget=2000000)&255
        if mode=='diagonal':
            assert result==1 and calls==[708*6] and get(u,SELF+0x134)==708
            assert struct.unpack('<hh',u.mem_read(HEAP+707*6,4))==(499,499)
        elif mode=='allocation failure':assert result==0 and get(u,SELF+4)==0
        else:assert result==0 and not calls
        n+=1
    print('PASS C-only loader capacity, allocation-failure and malformed-count contracts:',n,flush=True)
    return n

if __name__=='__main__':
    total=leaves()+bindings()+hits()+erases()+rebuilds()
    print('PASS total cases:',total,'target SHA256',target_sha,flush=True)
    safe_loader_cases()
