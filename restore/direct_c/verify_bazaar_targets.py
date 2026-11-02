"""Whole source collection and buyer retry/return native/C comparisons.

Availability, route selection, root resolution and entrance lookup are explicit
boundaries. The seven repaired providers and retained demand selector compose.
No original process, host game, file/save writes or UI automation are used.
"""
from verify_options_startup import *

ACTOR, HOME, SOURCE = 255, 400, 17


def setup(u, state, kind=71, actor=ACTOR, home=HOME, source=SOURCE):
    reset(u, state)
    a, b, s = actor*388, home*264, source*264
    put(u, 0x78d788, 1);put(u, 0xbca2d8, 7000);put(u, 0xeaac28, 0)
    byte(u, 0x7900c0+196, 1);word(u, 0x7900bc+196, source)
    byte(u, 0x7999d8+s, 1);word(u, 0x7999e8+s, kind)
    byte(u, 0x799a1c+s, 1);word(u, 0x7999f8+s, 10)
    byte(u, 0x7999ec+s, 2);byte(u, 0x7999ec+b, 2)
    byte(u, 0x799a99+b, 7)
    for off, value in [(0x7999de, 10), (0x7999e0, 10),
                       (0x7999fe, 12), (0x799a00, 13)]:word(u, off+b, value)
    for off, value in [(0x7999de, 15), (0x7999e0, 12),
                       (0x7999fe, 17), (0x799a00, 18)]:word(u, off+s, value)
    for resource in range(1, 9):
        word(u, 0x799a2e+s+resource*2, 100)
        word(u, 0xea4096+resource*2, 1)
    for i in range(4):word(u, 0x799a76+b+i*2, 5)
    word(u, 0xbcd652+a, home);word(u, 0xbcd624+a, 31);word(u, 0xbcd626+a, 32)
    word(u, 0xbcd632+a, 12);word(u, 0xd04e58+12*2, actor)
    put(u, 0x89b6d4, 0xa5a5a5a5);put(u, 0x96e6b0, 0x5a5a5a5a)
    return a, b, s


def supplier(kind=71, change=None, mutation=None, query=True, actor=ACTOR, home=HOME, source=SOURCE):
    results = []
    for c, u, state in machines:
        a, b, s = setup(u, state, kind, actor, home, source)
        if change:change(u, a, b, s)
        calls = []
        def available(cpu, args):
            calls.append((0x45d510, args))
            if mutation:mutation(cpu, 'available', args, a, b, s)
            return 100
        def route(cpu, args):
            calls.append((0x519e30, args, bytes(cpu.mem_read(args[2], args[3]*4))))
            if mutation:mutation(cpu, 'route', args, a, b, s)
            return args[3] if query else 0
        def root(cpu, args):
            calls.append((0x475b50, args))
            if mutation:mutation(cpu, 'root', args, a, b, s)
            return args[0]+200
        hook(c, state, 0x45d510, 2, available)
        hook(c, state, 0x519e30, 4, route)
        hook(c, state, 0x475b50, 1, root)
        run(c, u, 0x456760, [actor, home])
        results.append((calls, snapshot(u)))
    equal(results, ('supplier', kind, query, actor, home, source))


def verify_supplier():
    cases = 0
    for kind in [0, 71, 191, 72, 193, 115]:
        for query in [False, True]:supplier(kind, query=query);cases+=1
    changes = [
        lambda u,a,b,s:put(u, 0x78d788, 0),
        lambda u,a,b,s:put(u, 0x78d788, 0xffffffff),
        lambda u,a,b,s:byte(u, 0x7900c0+196, 0),
        lambda u,a,b,s:word(u, 0x7900bc+196, 0),
        lambda u,a,b,s:byte(u, 0x7999d8+s, 3),
        lambda u,a,b,s:byte(u, 0x799a1c+s, 0),
        lambda u,a,b,s:word(u, 0x7999f8+s, 0),
        lambda u,a,b,s:word(u, 0x7999f8+s, -1),
        lambda u,a,b,s:byte(u, 0x7999ec+s, 3),
        lambda u,a,b,s:put(u, 0x784bbc, 256),
        lambda u,a,b,s:word(u, 0x7999de+s, 49),
        lambda u,a,b,s:word(u, 0x7999de+s, 50),
        lambda u,a,b,s:word(u, 0x7999e0+s, -30),
        lambda u,a,b,s:word(u, 0x7999de+s, -32768)]
    for kind, change in itertools.product([71, 72], changes):
        supplier(kind, change);cases+=1
    for resource in range(1, 9):
        for kind in [71, 72]:
            def gates(u,a,b,s,resource=resource):
                word(u, 0x799a2e+s+resource*2, 0)
                byte(u, 0x7900c2+7*196+((resource%8)+1), 1)
                word(u, 0x799a2e+b+((resource+1)%8)*2, 20000)
                put(u, 0xea42f8+((resource+2)%8+1)*4, 256)
                word(u, 0xea4096+((resource+3)%8+1)*2, 2)
            supplier(kind, gates);cases+=1
    for stock in [-32768, -1, 0, 19999, 20000, 32767]:
        def cap(u,a,b,s,stock=stock):
            for i in range(12):word(u, 0x799a2e+b+i*2, stock)
            for i in range(4):word(u, 0x799a76+b+i*2, stock)
        supplier(72, cap);cases+=1
    supplier(71, actor=1999, home=3998, source=3999);cases+=1
    def multiple(u,a,b,s):
        put(u, 0x78d788, 3)
        for store in [2,3]:
            byte(u, 0x7900c0+store*196, 1);word(u, 0x7900bc+store*196, SOURCE)
    supplier(71, multiple);supplier(72, multiple);cases+=2
    def live(cpu, event, args, a, b, s):
        if event == 'available':
            byte(cpu, 0x799a99+b, 8)  # cached profile must remain7
            byte(cpu, 0x7900c2+7*196+3, 1)
            word(cpu, 0x7999fe+s, -7);word(cpu, 0x799a00+s, 19)
            word(cpu, 0x7900bc+196, 23)  # cached source remains17
            put(cpu, 0x78d788, 0)  # checked after this source's full scan
            put(cpu, 0xeaac28, 3)
            word(cpu, 0xea4096+3*0x49d8+args[1]*2, 3)
        elif event == 'route':
            word(cpu, 0x7999fe+b, -9);word(cpu, 0x799a00+b, 21)
        elif event == 'root':
            word(cpu, 0xbcd652+a, HOME+1)
    supplier(72, multiple, mutation=live);cases+=1
    print('PASS whole supplier gates,12 resource categories, repeated sources, signed fields and live/cached callback state:', cases, flush=True)


def transitions(token, actor=ACTOR, cargo=(0,0,0,0), entrances=(1,), change=None):
    results = []
    for c, u, state in machines:
        a,b,s = setup(u, state, actor=actor)
        for i, amount in enumerate(cargo):put(u, 0xbcd6b4+a+i*4, amount)
        if change:change(u,a,b,s)
        calls=[]
        def entrance(cpu,args):
            calls.append((args,bytes(cpu.mem_read(0xbcd600+a,96))))
            word(cpu, 0xd3b4e4, -15);word(cpu, 0xd3b4e0, 300)
            word(cpu, 0x7999de+s, 32);word(cpu, 0x7999e0+s, -9)
            return entrances[min(len(calls)-1,len(entrances)-1)]
        hook(c,state,0x480180,3,entrance)
        args = [actor,SOURCE] if token == 0x495c10 else [actor]
        value=run(c,u,token,args)
        results.append((value if token in [0x495c10,0x495d70] else None,calls,snapshot(u)))
    equal(results,('transition',hex(token),actor,cargo,entrances))


def retry(token, indices=(0,1,8,9), committed=(0,0,0,0), stop=0, choice=1, mutate=False, composed=False):
    results=[]
    for c,u,state in machines:
        a,b,s=setup(u,state)
        byte(u,0xbcd68b+a,stop)
        for i,(index,amount) in enumerate(zip(indices,committed)):
            put(u,0xbcd694+a+i*4,index);put(u,0xbcd6b4+a+i*4,amount)
        calls=[];demand_pointer=[None]
        def demand(cpu,pc,size,unused):
            p=get(cpu,cpu.reg_read(UC_X86_REG_ESP)+4);demand_pointer[0]=p
            calls.append(('demand',tuple(struct.unpack('<4i',bytes(cpu.mem_read(p,16))))))
        monitor=u.hook_add(UC_HOOK_CODE,demand,begin=address(c,0x495da0),end=address(c,0x495da0))
        def source(cpu,args):
            calls.append(('sources',args))
            for i in range(12):put(cpu,0x89b6d8+i*4,0)
            put(cpu,0x89b6d8+choice*4,SOURCE)
            if mutate:
                put(cpu,demand_pointer[0],0)
                put(cpu,0xbcd694+a+4,choice)
            return 0
        def entrance(cpu,args):
            calls.append(('entrance',args));word(cpu,0xd3b4e4,44);word(cpu,0xd3b4e0,45);return 1
        if not composed:hook(c,state,0x456760,2,source)
        else:
            hook(c,state,0x519e30,4,lambda cpu,args:1)
            hook(c,state,0x475b50,1,lambda cpu,args:args[0])
        hook(c,state,0x480180,3,entrance)
        run(c,u,token,[ACTOR,HOME,HOME])
        u.hook_del(monitor)
        results.append((calls,snapshot(u)))
    equal(results,('retry',hex(token),indices,committed,stop,choice,mutate,composed))


def verify_buyers():
    cases=0
    for token,actor in itertools.product([0x495c10,0x495d30,0x495d70,0x495de0],[1,255,1999]):
        for cargo in [(0,0,0,0),(1,0,0,0),(0,0,0x100,0),(0,0,0,0xffffffff)]:
            transitions(token,actor,cargo);cases+=1
    for entrances in [(0,0),(0,1),(0,0x100),(0x100,),(0xffffffff,)]:
        transitions(0x495c10,entrances=entrances);cases+=1
    for token,stop,choice in itertools.product([0x495e10,0x495f00],[0,2,3,255],range(12)):
        retry(token,stop=stop,choice=choice);cases+=1
    for token,committed in itertools.product([0x495e10,0x495f00],
            [(399,0,99,100),(0xffffffff,0,0x80000000,0),(800,600,600,600)]):
        retry(token,indices=(0,11,-1,10),committed=committed);cases+=1
    for token in [0x495e10,0x495f00]:
        retry(token,mutate=True);cases+=1
        retry(token,composed=True);cases+=1
    print('PASS full buyer destinations/AL cargo/return cleanup, escaped demands, thresholds and real supplier compositions:',cases)
    print('Verified target SHA256:',target_sha)


if __name__=='__main__':
    verify_supplier();verify_buyers()
