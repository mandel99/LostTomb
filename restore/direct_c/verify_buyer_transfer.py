"""Whole purchase calculators/coordinators/carrier constructors and wrappers.

Original instructions versus current compiled C, offline. Availability, stock
withdrawal and actor allocation are declared controlled endpoints. The native
no-leader check executes with its required zero predecessor word.
"""
from verify_options_startup import *

BUYER, BAZAAR, SOURCE = 255, 400, 17
RESOURCES = [1,2,3,4,5,6,7,8,15,13,17,19]


def setup(u,state,buyer=BUYER,category=0,kind=71,home_stock=0,source_stock=800):
    reset(u,state)
    a,b,s=buyer*388,BAZAAR*264,SOURCE*264
    put(u,0xd056f8,buyer);put(u,0xeaac28,0)
    word(u,0x7999e8+s,kind)
    for resource in range(1,20):word(u,0x799a2e+s+resource*2,source_stock)
    word(u,0x799a2e+b+(category&0xffffffff)*2,home_stock)
    word(u,0xbcd60c+a,12);word(u,0xbcd60e+a,-13)
    word(u,0xbcd652+a,BAZAAR);word(u,0xbcd660+a,0)
    for i,index in enumerate([0,7,8,11]):
        put(u,0xbcd694+a+i*4,index);put(u,0xbcd6b4+a+i*4,10+i)
    return a,b,s


def endpoints(c,u,state,calls,stock=800,mutate=None,fail=False):
    created=[]
    def change(event,args):
        if mutate:mutate(u,event,args,len(created))
        u.reg_write(UC_X86_REG_ECX,0xdeadbeef);u.reg_write(UC_X86_REG_EDX,0xbaadf00d)
    def availability(cpu,args):
        calls.append(('available',args));change('available',args);return stock
    def withdraw(cpu,args,token):
        calls.append((hex(token),args));change('withdraw',args);return 0
    def create(cpu,args):
        # The actual factory consumes BYTE owner and direction.
        logical=(args[0]&255,*args[1:4],args[4]&255)
        previous=created[-1] if created else None
        prior=bytes(cpu.mem_read(0xbcd5f8+previous*388,388)) if previous is not None else None
        calls.append(('create',logical,prior))
        child=0 if fail else 1001+len(created)
        cpu.mem_write(0xbcd5f8+child*388,bytes([0x40+len(created)])*388)
        created.append(child);change('create',logical)
        return child
    hook(c,state,0x45d510,2,availability)
    hook(c,state,0x45b570,4,lambda cpu,args:withdraw(cpu,args,0x45b570))
    hook(c,state,0x45e360,4,lambda cpu,args:withdraw(cpu,args,0x45e360))
    hook(c,state,0x4b4800,5,create)


def quantity(token=0x496550,category=0,home_stock=0,source_stock=800,committed=0,mutate=False):
    results=[]
    for c,u,state in machines:
        a,b,s=setup(u,state,category=category,home_stock=home_stock,source_stock=source_stock)
        calls=[]
        def mutation(cpu,event,args,count):
            if event=='available':word(cpu,0x799a2e+b+category*2,32767)
        endpoints(c,u,state,calls,stock=source_stock,mutate=mutation if mutate else None)
        resource=RESOURCES[category]
        args=[resource,BUYER,BAZAAR,SOURCE,committed,category] if token==0x4963b0 else [BUYER,resource,BAZAAR,SOURCE,committed,category]
        value=run(c,u,token,args)
        results.append((value,calls,snapshot(u)))
    equal(results,('quantity',hex(token),category,home_stock,source_stock,committed,mutate))


def verify_quantities():
    cases=0
    for token in [0x4963b0,0x496460,0x496550]:
        for stock in [-32768,-1,0,1,99,100,199,200,399,400,799,800,32767]:
            quantity(token,source_stock=stock);cases+=1
        limit=400 if token==0x4963b0 else 800
        for need in [-0x80000000,-1,0,1,99,100,101,199,200,399,400,599,600,799,800,0x7fffffff]:
            quantity(token,committed=(limit-need)&0xffffffff);cases+=1
        for stock,committed in [(-32768,0),(32767,0),(-1,-1),(1,0x80000000),(32767,0x7fffffff)]:
            quantity(token,home_stock=stock,committed=committed);cases+=1
        for category in [1,3,7,8,11]:quantity(token,category=category);cases+=1
        quantity(token,mutate=True);cases+=1
    print('PASS all three load calculators: signed WORD/DWORD stock, wrapped demand,100-unit thresholds and callback order:',cases,flush=True)


def transaction(token,category=0,resource=1,loads=1,kind=71,stock=800,ban=0,
                committed=10,slot=0,buyer=BUYER,current=None,mutate=False,fail=False):
    results=[]
    for c,u,state in machines:
        a,b,s=setup(u,state,buyer,category,kind)
        current=buyer if current is None else current
        put(u,0xd056f8,current)
        put(u,0xbcd694+current*388+slot*4,category)
        put(u,0xbcd6b4+current*388+slot*4,committed)
        if category<12:
            put(u,0xea42f8+RESOURCES[category]*4,ban)
        calls=[]
        def mutation(cpu,event,args,count):
            if event=='available':
                put(cpu,0xeaac28,3)
                word(cpu,0x799a2e+b+(category&0xffffffff)*2,0)
            elif event=='withdraw':
                put(cpu,0xeaac28,3)
                word(cpu,0xbcd60c+a,-7);word(cpu,0xbcd60e+a,19)
                word(cpu,0xbcd652+a,401)
                put(cpu,0xbcd6b4+a+slot*4,77)
            elif event=='create':
                put(cpu,0xeaac28,0 if count&1 else 3)
                word(cpu,0xbcd60c+a,30+count);word(cpu,0xbcd60e+a,-40-count)
                word(cpu,0xbcd652+a,400+count)
                put(cpu,0xbcd6b4+a+slot*4,get(cpu,0xbcd6b4+a+slot*4)+1)
        endpoints(c,u,state,calls,stock,mutation if mutate else None,fail)
        if token in [0x496660,0x4968a0]:args=[resource,loads,buyer,BAZAAR,SOURCE,77]
        elif token in [0x495cf0,0x496010]:args=[buyer,BAZAAR,SOURCE,slot]
        else:args=[buyer,BAZAAR,SOURCE,category,committed,slot]
        value=run(c,u,token,args)
        results.append((value,calls,snapshot(u)))
    equal(results,('transaction',hex(token),category,resource,loads,kind,stock,ban,committed,slot,buyer,current,mutate,fail))


def verify_spawns():
    cases=0
    for token,resource in itertools.product([0x496660,0x4968a0],[0,*range(1,21),0xffffffff]):
        transaction(token,resource=resource);cases+=1
    for token,loads in itertools.product([0x496660,0x4968a0],[-0x80000000,-1,0,1,2,3,4,5,7,8,9,0x7fffffff]):
        transaction(token,loads=loads);cases+=1
    for kind in [0,71,191,72,193]:
        transaction(0x4968a0,kind=kind,loads=2);cases+=1
    for token in [0x496660,0x4968a0]:
        transaction(token,loads=9,mutate=True);cases+=1
        transaction(token,loads=2,fail=True);cases+=1
    print('PASS full carrier constructors: resource mapping,1/4/8 creation limits, zero allocations and fresh callback-visible metadata:',cases,flush=True)


def verify_coordinators():
    cases=0
    for token,category,kind in itertools.product([0x496c70,0x496de0],range(12),[71,72]):
        transaction(token,category=category,kind=kind);cases+=1
    for token,ban,category in itertools.product([0x496c70,0x496de0],[1,256],[0,8,11]):
        transaction(token,category=category,ban=ban);cases+=1
    for token,stock in itertools.product([0x496c70,0x496de0],[-1,0,1,99,100,399,400,800]):
        transaction(token,stock=stock,kind=72);cases+=1
    for token in [0x496c70,0x496de0]:
        transaction(token,mutate=True);cases+=1
        transaction(token,fail=True);cases+=1
    for token,buyer,current,slot in itertools.product([0x495cf0,0x496010],[1,255],[1,255],range(4)):
        transaction(token,buyer=buyer,current=current,slot=slot,category=[0,7,8,11][slot]);cases+=1
    print('PASS both full purchases and wrappers: mapped resource bans, real quantities/creation, fresh commitments and current-versus-argument actor:',cases)
    print('Verified target SHA256:',target_sha)


if __name__=='__main__':
    verify_quantities();verify_spawns();verify_coordinators()
