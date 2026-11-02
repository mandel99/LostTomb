"""Native/C allocation, repair slots and god effects through shared entries.

Whole decision/data logic executes. Reservation lookup and repair-slot
allocation are genuine compositions; quality, neighbor, message and effect
providers are controlled. God scratch is given the same explicit prestate
on both sides, including through the public entry, without changing live C.
"""
from verify_options_startup import *


def prepare(u,state):
    reset(u,state);u.mem_write(SP-0x5000,b'\xa5'*0x5000)


def boundary(c,state,token,name,n,action,cleanup=0):
    state['hooks'][sym(name) if c else token]=(n,0 if c else cleanup,action)


def execute(c,u,token,args=(),name=None):
    if not c or name is None:return run(c,u,token,args,budget=10000000)
    for r,v in zip(registers,[0x11111111,0x22222222,0x33333333,saved[0],SP,*saved[1:]]):u.reg_write(r,v)
    u.reg_write(UC_X86_REG_EFLAGS,2);put(u,SP,STOP)
    for i,v in enumerate(args):put(u,SP+4+i*4,v)
    u.emu_start(sym(name),STOP,count=10000000)
    assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==SP+4
    assert [u.reg_read(registers[i]) for i in [3,5,6,7]]==saved
    return u.reg_read(UC_X86_REG_EAX)


def compare(values,label):
    assert values[0][0]==values[1][0],(label,'trace/return',values[0][0],values[1][0])
    equal(values,label)


def debug_string(c,u,state,calls):
    slot=sym('_imp__OutputDebugStringA@4') if c else 0x56f0f8
    original=get(u,slot)
    put(u,slot,S+0x49000)
    state['hooks'][S+0x49000]=(1,4,lambda cpu,args:calls.append(('debug',args)) or 0)
    return slot,original


def allocate(kind=70,selected=300,contiguous=0,occupied=(),reserved=(),angle=0,rotation=0,
             serial=0x1234ffff,mutate='',gate=False,ferry=0,canonical=False):
    values=[]
    for c,u,state in machines:
        prepare(u,state)
        # Free record states, while preserving recognizable untouched fields.
        for i in range(1,4001):
            byte(u,0x7999d8+i*264,0);byte(u,0x799a7e+i*264,0xa5)
        for i in occupied:byte(u,0x7999d8+i*264,1)
        for a,v in [(0xeaac28,2),(0xbca2d8,1000),(0x78d7b4,serial),(0x78d790,17),
                    (0xbc9714,angle),(0xbca2e0,rotation),(0xbca310,ferry),(0xbc9800,211),
                    (0xbc968c,1),(0xbc9694,1),(0xbc95f0,1 if reserved else 0)]:put(u,a,v)
        u.mem_write(0xba746c,bytes(1050));u.mem_write(0x9783c0,bytes(280))
        for i,value in enumerate(reserved):word(u,0xba746c+i*2,value)
        byte(u,0xea1875+2*0x49d8,0x9a);byte(u,0xacf85c+21610,0xff)
        calls=[]
        def quality(cpu,args):
            calls.append(('quality',args))
            if mutate=='quality':word(cpu,0x7999e8+args[0]*264,136)
            return 0
        boundary(c,state,0x4e7f00,'restore_building_quality_004e7f00',2,quality)
        def neighbor(cpu,args):
            calls.append(('neighbor',args))
            if mutate=='neighbor':put(cpu,0xbca310,2)
            return 0x12345678
        hook(c,state,0x481000,3,neighbor)
        def building_gate(cpu,args):
            calls.append(('gate',args))
            if not c:assert cpu.reg_read(UC_X86_REG_ECX)==0xbca350
            if mutate=='gate':word(cpu,0x7999e8+args[0]*264,199)
            return int(gate) if c else 0x12340000|int(gate)
        boundary(c,state,0x488050,'restore_actor_building_gate',1,building_gate,cleanup=4)
        hook(c,state,0x4e2970,1 if c else 3,
             lambda cpu,args:calls.append(('message',args[:1])) or 0)
        debug_slot,debug_original=debug_string(c,u,state,calls)
        result=execute(c,u,0x46a400,[2,kind,90,90,contiguous,selected],
                       name='restore_building_allocate_0046a400' if canonical else None)
        put(u,debug_slot,debug_original);values.append(((result,calls),snapshot(u)))
    compare(values,('allocate',kind,selected,contiguous,occupied,reserved,angle,rotation,serial,mutate,gate,ferry,canonical))


def repair(first_free):
    values=[]
    for c,u,state in machines:
        prepare(u,state);u.mem_write(0x9783c0,b'\xa5'*280)
        for i in range(10):byte(u,0x9783c0+i*28,0 if i==first_free else 1)
        calls=[];debug_slot,debug_original=debug_string(c,u,state,calls)
        run(c,u,0x46a280,[300]);put(u,debug_slot,debug_original);values.append((calls,snapshot(u)))
    compare(values,('repair',first_free))


GOD_ENDPOINTS={0x4f5340:('restore_monument_effect_level_004f5340',0),
 0x4f3aa0:('restore_monument_effect_004f3aa0',1),0x4c3950:('restore_god_building_004c3950',0),
 0x4c3ac0:('restore_god_building_004c3ac0',0),0x4c3ed0:('restore_god_building_004c3ed0',0),
 0x4c4020:('restore_god_building_004c4020',1),0x4c41b0:('restore_god_building_004c41b0',0),
 0x4c42b0:('restore_god_building_004c42b0',0),0x4c45e0:('restore_god_building_004c45e0',0),
 0x4ea000:('restore_god_city_004ea000',0),0x4e6eb0:('restore_god_city_004e6eb0',0),
 0x4e6ed0:('restore_god_city_004e6ed0',0),0x4e70b0:('restore_god_city_004e70b0',1),
 0x4b4710:('restore_god_city_004b4710',0)}


# Seed the owned public C wrapper's scratch at the explicit core boundary.
# This is verification input; production retains its native unspecified bytes.
for c,u,state in machines:
    if c:
        def seed_scratch(cpu,pc,size,s):
            if pc==sym('restore_god_effect_core_004c47a0') and s.get('god_pattern') is not None:
                cpu.mem_write(get(cpu,cpu.reg_read(UC_X86_REG_ESP)+16),s['god_pattern'])
        u.hook_add(UC_HOOK_CODE,seed_scratch,state)


def god_effect(god=0,effect=1,blessing=0,curse=0,elapsed=12,rng=(0,),level=(0,),enabled=256,
               query=0,mutate='',public=True,source=0x12345678,pattern=0):
    values=[]
    scratch=bytes(((i*7+13) if pattern else 0xa5)&255 for i in range(30))
    for c,u,state in machines:
        prepare(u,state);put(u,0xeaac28,2)
        for player in [0,2]:
            offset=player*0x49d8+(god&0xffffffff)
            byte(u,(0xea6086+offset)&0xffffffff,blessing);byte(u,(0xea5c14+offset)&0xffffffff,curse)
            byte(u,(0xea5c0a+offset)&0xffffffff,100)
            put(u,(0xea5c50+player*0x49d8+(god&0xffffffff)*4)&0xffffffff,elapsed)
            put(u,0xea5c54+player*0x49d8,0x87654321)
            put(u,0xea5c58+player*0x49d8,0x98765432);put(u,0xea5c5c+player*0x49d8,0xa9876543)
        word(u,(0x7848e2+god*2)&0xffffffff,enabled)
        put(u,0x705df4,256);put(u,0x705c10,0);put(u,0xe950dc,0x12345678)
        calls=[];random_index=0;level_index=0
        def random(cpu,args):
            nonlocal random_index
            value=rng[random_index%len(rng)];random_index+=1
            calls.append(('rand',value));return value
        hook(c,state,0x562348,0,random)
        def publish(cpu,args):
            # Normalize the private scratch pointer to its complete contents.
            packet=bytes(cpu.mem_read(args[3],30)) if args[3] else None
            calls.append(('publish',args[:3],packet,args[4]))
            if mutate=='publish':put(cpu,0xeaac28,0)
            return 0
        hook(c,state,0x4e0cf0,5,publish)
        def endpoint(cpu,args,t):
            nonlocal level_index
            calls.append((t,args))
            if t==0x4f5340:
                value=level[min(level_index,len(level)-1)];level_index+=1;return value
            if t in [0x4c3950,0x4c3ed0,0x4c41b0]:return query
            if mutate=='city' and t==0x4ea000:put(cpu,0xeaac28,0)
            return 0
        for t,(name,n) in GOD_ENDPOINTS.items():
            boundary(c,state,t,name,n,lambda cpu,args,t=t:endpoint(cpu,args,t))
        def flood(cpu,args):
            if c:assert args[0]==0xd38360;amount=args[1]
            else:assert cpu.reg_read(UC_X86_REG_ECX)==0xd38360;amount=args[0]
            calls.append(('flood',amount));return 0
        boundary(c,state,0x4bd5e0,'restore_god_settings_004bd5e0',2 if c else 1,flood,cleanup=4)
        for t,n in [(0x4e96c0,1),(0x42fcf0,1)]:hook(c,state,t,n,lambda cpu,args,t=t:calls.append((t,args)) or 0)
        state['god_pattern']=scratch
        if not c:u.mem_write(SP-32,scratch)
        args=[god,source,effect]
        if c and not public:args.append(S+0x1000)
        execute(c,u,0x4c47a0,args,name=None if public else 'restore_god_effect_core_004c47a0')
        state['god_pattern']=None;values.append((calls,snapshot(u)))
    compare(values,('god',god,effect,blessing,curse,elapsed,rng,level,enabled,query,mutate,public,source,pattern))


if __name__=='__main__':
    count=0
    for kind in range(241):allocate(kind);count+=1
    print('PASS allocator public entry: %d native/C building kinds and complete record writes'%count,flush=True)
    count=0
    for selected,contiguous,occupied,reserved in [
        (0,0,(),()),(0,1,(1,2),()),(0,2,(2,),()),(0,4,(3,6),()),
        (0,3,(),(2,)),(300,0,(300,),()),(300,0,(),(300,)),(3999,0,(),()),
        (0,0,tuple(range(1,4000)),()),(0,0,(1,),(2,3)),(0,-1,(),())]:
        allocate(selected=selected,contiguous=contiguous,occupied=occupied,reserved=reserved);count+=1
    for angle,rotation in itertools.product([-5,-1,0,3,4,15,16,0x10001],[0,2,4,6]):
        allocate(41,angle=angle,rotation=rotation);count+=1
    for kind,mutation in [(10,'quality'),(136,'neighbor'),(70,'gate')]:
        allocate(kind,mutate=mutation);count+=1
    for ferry in [0,1,2,3]:allocate(136,ferry=ferry);count+=1
    allocate(70,gate=True);allocate(74,canonical=True);count+=2
    print('PASS allocator search/ABI: %d native/C contiguous, reservation, failure, orientation and callback cases'%count,flush=True)
    for first_free in range(11):repair(first_free)
    print('PASS all ten repair slots and the full-queue path: 11 native/C cases',flush=True)
    count=0
    for god,effect,rng in itertools.product(range(5),[1,2,3,4],[(0,),(1,),(2,),(1,0),(1,1)]):
        god_effect(god,effect,rng=rng);count+=1
    print('PASS god public entry: %d native/C complete effect and RNG branches'%count,flush=True)
    count=0
    for god,effect,level,pattern in itertools.product([-1,0,1,2,3,4],[1,2],[(1,2),(2,1)],[0,1]):
        god_effect(god,effect,level=level,pattern=pattern);count+=1
    for effect,blessing,curse,elapsed in itertools.product([-1,0,5],[19,20,49,50,-128],[19,20,50],[3,4,14,15]):
        god_effect(0,effect,blessing,curse,elapsed,public=False);count+=1
    for god,query in itertools.product([2,3],[0,1]):
        god_effect(god,3,query=query);god_effect(god,4,query=query);count+=2
    for god,effect in itertools.product(range(5),[1,2,3,4]):
        god_effect(god,effect,mutate='publish',rng=(0,));count+=1
    god_effect(4,2,mutate='city');count+=1
    for enabled,god in [(0,0),(1,5),(1,6)]:god_effect(god,1,enabled=enabled);count+=1
    for rng in [(-1,),(-2,),(-3,),(-4,)]:god_effect(0,2,rng=rng);count+=1
    print('PASS god scratch/thresholds: %d native/C explicit 30-byte prestate, signed mood and reload cases'%count,flush=True)
    print('target SHA256',target_sha)
