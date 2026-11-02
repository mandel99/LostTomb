"""Whole earthquake list, terrain predicate, tile destruction and event tick.

Offline native/C comparison. Due checks and message-member graphs execute in
composed cases. Demolition/refresh and final publisher endpoints are controlled;
no host game, physical map rendering or save writes are exercised.
"""
from verify_options_startup import *

SELF=0x785d74
for c,u,state in machines:u.mem_map(0x2fe00000,0x200000)

def prepare(u,state,kind=3,phase=1,timer=0,interval=1,progress=0,quantity=1,
            choice=0,dimensions=(384,300),points=((30,40),(60,70),(90,100),(120,130))):
    reset(u,state);u.mem_write(SP-0x5000,b'\xa5'*0x5000)
    u.mem_write(SELF,bytes(150*124));put(u,0x785d70,4)
    for offset,value in [(12,quantity),(48,phase),(58,progress),(60,interval),(62,timer),(52,-1)]:word(u,SELF+offset,value)
    byte(u,SELF+2,kind);byte(u,SELF+3,5);byte(u,SELF+50,1)
    for i,(x,y) in enumerate(points):word(u,SELF+64+i*4,x);word(u,SELF+66+i*4,y)
    put(u,0xd3b374,choice);put(u,0xbca2d8,0x1234)
    put(u,0xbca2d0,dimensions[0]);put(u,0xbca2d4,dimensions[1])
    word(u,0x7848d8,-331);word(u,SELF+20,4)
    put(u,0x78d7bc,-327);put(u,0x78d7c0,5)
    put(u,0x605a3c,0);word(u,0x784c60,30);word(u,0x784c62,40)
    return 0x1234

def terrain():
    count=0
    for x,y,base in [(30,40,0x1234),(-100,3,0x1000),(0,0,0x1234),
                     (0x7fffffff,0,0x80001235),(0,-1,0x1318),(-1,-1,0x1319)]:
        for flags in [0,1,2,4,0x200,0x206,0x10000,0xfffffdf9,0xffffffff]:
            results=[]
            for c,u,state in machines:
                prepare(u,state);put(u,0xbca2d8,base)
                cell=(base+x+y*228)&0xffffffff;put(u,0xaf598c+cell*4,flags)
                result=run(c,u,0x44bd20,[x,y])
                assert result==int(flags&0x206==0)
                results.append((result,snapshot(u)))
            equal(results,('terrain',x,y,base,flags));count+=1
    print('PASS full DWORD map/terrain, mask and wrapped cell arithmetic:',count,flush=True)
    return count

def destroy_case(building=0,replacement=None,coordinates=(30,40),base=0x1234):
    results=[]
    for c,u,state in machines:
        prepare(u,state);x,y=coordinates;put(u,0xbca2d8,base)
        cell=(base+x+y*228)&0xffffffff;word(u,0xb4e6fc+cell*2,building)
        put(u,0xaf598c+cell*4,0x12345206);calls=[]
        for value in {building,replacement or 0}:
            if value:u.mem_write(0x7999d8+value*264,b'\x19\x28\x37\x46')
        for token,n in [(0x46d100,2),(0x4695d0,2),(0x53b090,1),
                        (0x4798e0,3),(0x476530,0),(0x47b570,0),(0x475720,5),
                        (0x516f90,5),(0x5173a0,4),(0x5178e0,0),(0x48c0e0,4)]:
            def endpoint(cpu,args,t=token):
                calls.append((t,args))
                if t==0x46d100 and replacement is not None:
                    word(cpu,0xb4e6fc+cell*2,replacement)
                    put(cpu,0xbca2d8,0x5678)
                if t==0x47b570:
                    put(cpu,0xbca2d0,513);put(cpu,0xbca2d4,1025)
                return 0x12345678
            hook(c,state,token,n,endpoint)
        run(c,u,0x44bd50,[x,y])
        assert get(u,0xaf598c+cell*4)==0
        expected=building if replacement is None else replacement
        if expected:assert bytes(u.mem_read(0x7999d8+expected*264,4))==b'\x05\x28\x37\x46'
        results.append((tuple(calls),snapshot(u)))
    equal(results,('destroy',building,replacement,coordinates,base))

def destruction():
    count=0
    for building,replacement in [(0,None),(1,None),(3,0),(3,7),(3,-1),(-1,None),(3,None)]:
        for xy,base in [((30,40),0x1234),((-1,-1),0x1319),((0x7fffffff,0),0x80001235)]:
            destroy_case(building,replacement,xy,base);count+=1
    print('PASS tile destruction order, cached cell, signed/reloaded building and BYTE state:',count,flush=True)
    return count

def step_case(allowed=1,mutate=False,due=None,full_tile=False,**settings):
    results=[]
    for c,u,state in machines:
        prepare(u,state,**settings);calls=[]
        if due is not None:
            def due_hook(cpu,args):
                self,ignore=args if c else (cpu.reg_read(UC_X86_REG_ECX),args[0])
                calls.append(('due',self,ignore));return due
            if c:state['hooks'][sym('restore_event_record_due_0044d680')]=(2,0,due_hook)
            else:hook(c,state,0x44d680,1,due_hook,4)
        def allowed_hook(cpu,args):calls.append(('allowed',args));return allowed
        hook(c,state,0x44bd20,2,allowed_hook)
        if not full_tile:
            def destroyed(cpu,args):
                calls.append(('destroy',args))
                if mutate:
                    word(cpu,SELF+64,257);word(cpu,SELF+66,258)
                    put(cpu,0xbca2d8,0x2345)
                return 0x12345678
            hook(c,state,0x44bd50,2,destroyed)
        else:
            for token,n in [(0x46d100,2),(0x4695d0,2),(0x53b090,1),(0x4798e0,3),
                            (0x476530,0),(0x47b570,0),(0x475720,5),
                            (0x516f90,5),(0x5173a0,4),(0x5178e0,0),(0x48c0e0,4)]:
                def observe(cpu,args,t=token):calls.append((t,args));return 0
                hook(c,state,token,n,observe)
            # Clear all four possible map cells and their native blockers.
            for i in range(4):
                x,y=struct.unpack('<hh',u.mem_read(SELF+64+i*4,4))
                for dx,dy in [(0,0),(-1,0),(1,0),(0,-1),(0,1)]:
                    cell=0x1234+x+dx+(y+dy)*228
                    word(u,0xb4e6fc+cell*2,0);put(u,0xaf598c+cell*4,0)
        def publish(cpu,args):
            message,a,b,parameters,force=args
            # Native caller and member helper define0..29, not padding30..31.
            calls.append(('publish',message,a,b,bytes(cpu.mem_read(parameters,30)),force))
            assert message==261 and a==force==0
            return 0
        hook(c,state,0x4e0cf0,5,publish)
        result=run(c,u,0x44be30,[SELF] if c else [],receiver=None if c else SELF)&255
        results.append(((result,tuple(calls)),snapshot(u)))
    equal(results,('step',settings,allowed,mutate,due,full_tile))

def steps():
    count=0
    for kind in range(30):step_case(kind=kind);count+=1
    for phase in [2,-1,3,32767]:step_case(phase=phase);count+=1
    for timer,interval in [(0,1),(0,2),(-1,0),(-32768,-32767),(32767,1),(32766,32767),(0,-1)]:
        for progress,quantity in [(0,1),(24,1),(49,2),(24,0),(24,-1),(32767,1),(32766,32767)]:
            step_case(timer=timer,interval=interval,progress=progress,quantity=quantity);count+=1
    for choice in range(16):
        for allowed in [0,1,256]:step_case(choice=0xabc00000+choice,allowed=allowed);count+=1
        for dimensions,points in [((1,1),((-1,-1),)*4),((384,300),((1000,1000),)*4),
                                  ((0,0),((0,0),)*4),((0x80000000,0x80000000),((32767,32767),)*4)]:
            step_case(choice=choice,dimensions=dimensions,points=points);count+=1
    for due in [None,0,1,256]:
        # The typed C due facade promises AL; a mock must honor that same ABI.
        if due==256:continue
        for mutate in [False,True]:step_case(phase=0,due=due,mutate=mutate);count+=1
    for phase in [0,1]:
        for choice in range(16):step_case(phase=phase,choice=choice,full_tile=True);count+=1
    print('PASS complete event states, all16 directions, timers, clamping and real member/tile compositions:',count,flush=True)
    return count

def list_case(count=4,editor=0,origin=(30,40),success=2,change=None,real=False):
    results=[]
    for c,u,state in machines:
        prepare(u,state);put(u,0x785d70,count);put(u,0x605a3c,editor)
        word(u,0x784c60,origin[0]);word(u,0x784c62,origin[1]);calls=[]
        byte(u,0x78a624,0x37)
        for i in range(12):
            byte(u,SELF+i*124+2,3 if i%2==0 else 1)
            word(u,SELF+i*124+48,2)
        if not real:
            def tick(cpu,args):
                self=args[0] if c else cpu.reg_read(UC_X86_REG_ECX)
                index=(self-SELF)//124;calls.append(index)
                if change is not None:put(cpu,0x785d70,change)
                return int(index==success)
            hook(c,state,0x44be30,1 if c else 0,tick)
        run(c,u,0x44bca0,[0x785d70] if c else [],receiver=None if c else 0x785d70)
        results.append((tuple(calls),snapshot(u)))
    equal(results,('list',count,editor,origin,success,change,real))

def lists():
    count=0
    for n in [-1,0,1,4,12]:
        for success in [-1,0,2,4]:list_case(count=n,success=success);count+=1
    for editor in [1,256]:list_case(editor=editor);count+=1
    for origin in [(-1,40),(30,-1),(-32768,40)]:list_case(origin=origin);count+=1
    for n in [0,1,8,-1]:list_case(change=n,success=-1);count+=1
    list_case(real=True);count+=1
    print('PASS list guards, early success, callback count reload and real event composition:',count,flush=True)
    return count

if __name__=='__main__':
    total=terrain()+destruction()+steps()+lists()
    print('PASS total cases:',total,'target SHA256',target_sha,flush=True)
