"""Native/C chief-advisor advice and temple upgrade preview comparisons.

All selection logic executes. Advice composes the real trade-list iterator;
temple cases compose the real upgrade-bit query and optionally root lookup.
Projection, raster/text and other provider queries are controlled endpoints.
No physical rendering or host game is started.
"""
from verify_options_startup import *


def prepare(u,state):
    reset(u,state);u.mem_write(SP-0x5000,b'\xa5'*0x5000)


def compare(values,label):
    assert values[0][0]==values[1][0],(label,'calls',
        next(((i,a,b) for i,(a,b) in enumerate(zip(values[0][0],values[1][0])) if a!=b),None),
        [len(v[0]) for v in values])
    equal(values,label)


def temple(tool=211,rotation=0,problem=0,match=0,count=9,mutate='',real_root=False):
    values=[];root=400
    for c,u,state in machines:
        prepare(u,state);p=2*0x49d8
        for a,v in [(0xeaac28,2),(0xbca300,30000),(0xbca304,310),(0xbca308,280),
                    (0xbca2e0,rotation),(0xbca2d8,1000),(0x5cfce0,count),(0x70e44c,271),
                    (0xf56be4,100000),(0xf56d8c,200000),(0xf56d84,300000),
                    (0xf56dac,400000),(0x70e120,0x76541111),(0xea187c+p,-4999),
                    (0xea5f9c+p,root)]:put(u,a,v)
        word(u,0xea5ce8+p,256);word(u,0xea5cfa+p,2);word(u,0xb4e6fc+30000*2,402 if real_root else root)
        for i in range(3):
            r=0x7999d8+(root+i)*264
            word(u,r+6,20+i*3);word(u,r+8,30+i*2);word(u,r+16,65+i)
            word(u,r+56,root+i-1 if i else 0);word(u,r+58,root+i+1 if i<2 else 0)
            byte(u,r+169,0)
        if match in (0,1):
            at=1000+23+32*228
            put(u,0xb7482c+at*4,(200000 if tool==211 else 300000)+(3 if match else 0))
        if problem==1:word(u,0xea5ce8+p,0)
        elif problem==2:word(u,0x7999e8+root*264,64)
        elif problem==3:word(u,0xea5cfa+p,1)
        elif problem in (4,5):byte(u,0x799a81+root*264,1 if problem==4 else 3)
        elif problem==6:put(u,0xea187c+p,-5000)
        calls=[];projections=0;draws=0;output_ptrs=None
        if not real_root:
            hook(c,state,0x475b50,1,lambda cpu,args:calls.append(('root',args)) or root)
        def project(cpu,args):
            nonlocal projections,output_ptrs
            projections+=1;calls.append(('project',args[:2]));output_ptrs=args[2:]
            put(cpu,args[2],611+projections*13);put(cpu,args[3],401+projections*17)
            if mutate=='projection' and projections==1:
                put(cpu,0xbca2e0,6);put(cpu,0x5cfce0,1)
            return 0
        def draw(cpu,args,t):
            nonlocal draws
            draws+=1;calls.append((t,args,get(cpu,0x70e120)))
            if mutate=='draw':
                put(cpu,0x70e44c,get(cpu,0x70e44c)+7);put(cpu,0xf56be4,get(cpu,0xf56be4)+17)
                put(cpu,0xf56dac,get(cpu,0xf56dac)+29);put(cpu,0xbca2e0,4);put(cpu,0xbca300,31000)
                if output_ptrs:
                    put(cpu,output_ptrs[0],get(cpu,output_ptrs[0])+11)
                    put(cpu,output_ptrs[1],get(cpu,output_ptrs[1])+13)
            return 0
        hook(c,state,0x4f0ba0,4,project)
        for t,n in [(0x53e590,3),(0x53f250,3),(0x53f4f0,4)]:
            hook(c,state,t,n,lambda cpu,args,t=t:draw(cpu,args,t),cleanup=0 if c else n*4)
        run(c,u,0x4c2f50,[tool],budget=3000000)
        values.append((calls,snapshot(u)))
    compare(values,('temple',tool,rotation,problem,match,count,mutate,real_root))


def upgrade(flags,desired):
    values=[]
    for c,u,state in machines:
        prepare(u,state);byte(u,0x799a81+400*264,flags)
        result=run(c,u,0x4c52d0,[400,desired]);values.append((result&255,snapshot(u)))
    equal(values,('upgrade',flags,desired))


DEFAULTS={0xea5db0:1,0xea1880:30,0xea5c80:0,0xea3fb0:0,0xea3f98:0,
          0xea18a8:1,0xea3f9c:0,0xea3fac:0,0xea5eb4:0,0xea5ea4:0,
          0xea446c:0,0xea4474:0,0xea4388:1,0xea41dc:50,0xea5e1c:100,
          0xea1894:100,0xea1888:50,0xea44b8:10,0xea44b4:10,0xea44b0:20,
          0xea44ac:20,0xea187c:100,0xea452c:100,0xea5fd0:5,0xea5ee8:0}


def advice(category,fields=None,moods=(50,50,50,50,50),towns=(),query=100,status=0,
           mutate='',player=2,button=True):
    values=[]
    for c,u,state in machines:
        prepare(u,state)
        for owner in [0,2]:
            for a,v in (DEFAULTS|dict(fields or {})).items():put(u,a+owner*0x49d8,v)
            for i,v in enumerate(moods):byte(u,0xea5c00+owner*0x49d8+i,v)
        for a,v in [(0xeaac28,player),(0xd3b198,category),(0xf53f0c,0x1234),
                    (0xf53f08,0x2345),(0xe38da8,317),(0xe38da4,271),(0xf56f8c,0x123456)]:put(u,a,v)
        for i in range(13):
            byte(u,0x6fdc88+i,1 if button else 0);put(u,0x5f78f0+i*4,0x579000+i*4)
        put(u,0x785d70,len(towns))
        for i,(kind,stage,available) in enumerate(towns):
            r=0x785d74+i*124;u.mem_write(r,bytes(124));byte(u,r+2,1);byte(u,r+50,1)
            word(u,r+4,kind);word(u,r+48,stage);byte(u,r+100,available)
        calls=[]
        def availability(cpu,args):
            receiver=args[0] if c else cpu.reg_read(UC_X86_REG_ECX)
            assert 0x785d74<=receiver<0x785d74+len(towns)*124
            calls.append(('available',receiver))
            if mutate=='town':
                put(cpu,0xeaac28,0);word(cpu,receiver+48,7)
            return 0x12340000|int(cpu.mem_read(receiver+100,1)[0]!=0)
        hook(c,state,0x44d4c0,1 if c else 0,availability)
        def provider(cpu,args,t):
            if t==0x4bd590 and not c:assert cpu.reg_read(UC_X86_REG_ECX)==0xd38360
            calls.append((t,args))
            if mutate=='provider':
                put(cpu,0xeaac28,0);put(cpu,0xea1894,200)
            return status if t==0x446610 else query
        for t in [0x447530,0x446610,0x4bd590]:hook(c,state,t,0,lambda cpu,args,t=t:provider(cpu,args,t))
        def draw(cpu,args,t):
            calls.append((t,args))
            if mutate=='draw':
                for a in [0xf53f0c,0xf53f08,0xe38da8,0xe38da4,0xf56f8c]:put(cpu,a,get(cpu,a)+17)
            return 0
        for t,n in [(0x4cf090,5),(0x536820,4),(0x4d6160,9),(0x4d6250,9),(0x425730,4)]:
            hook(c,state,t,n,lambda cpu,args,t=t:draw(cpu,args,t))
        run(c,u,0x538050,budget=5000000)
        values.append((calls,snapshot(u)))
    compare(values,('advice',category,fields,moods,towns,query,status,mutate,player,button))


if __name__=='__main__':
    count=0
    for flags,desired in itertools.product([0,1,2,3,4,255],[0,1,2,3,255,257,-1]):
        upgrade(flags,desired);count+=1
    print('PASS upgrade query: %d native/C BYTE flags and full DWORD desired values'%count,flush=True)
    count=0
    for tool,rotation,problem in itertools.product([211,212],[0,2,4,6],range(7)):
        temple(tool,rotation,problem);count+=1
    for tool,rotation,match,mutate in itertools.product([211,212],[0,2,4,6],[0,1,2],['projection','draw']):
        temple(tool,rotation,0,match,mutate=mutate,real_root=True);count+=1
    for count_value in [0,-1,1,36]:temple(problem=1,count=count_value)
    count+=4
    print('PASS temple preview: %d native/C chain, projection, scenario and mask cases'%count,flush=True)
    count=0
    for category in list(range(12))+[-1,12,256]:
        for mutate in ['', 'draw']:
            advice(category,mutate=mutate);count+=1
    for reason,level in itertools.product(range(7),[0,49,50,99,100]):
        advice(0,{0xea5db0:reason,0xea1880:level});count+=1
    for variant,phase in itertools.product(range(4),[0,1,2]):
        f={0xea5c80:variant,0xea3fb0:phase};advice(1,f);count+=1
        for extra in [{0xea3fac:5},{0xea5eb4:4},{0xea3f98:1},{0xea3f9c:80}]:
            advice(1,f|extra);count+=1
    for a,b in itertools.product([0,1,5,6,10,11,17,18],[0,5,6,10,11,20,21]):
        advice(2,{0xea446c:a,0xea4474:b});count+=1
    for level,numerator in [(0,50),(1,49),(1,50),(3,100),(1,0x40000000)]:
        advice(3,{0xea4388:level,0xea41dc:numerator});count+=1
    for ratio in [0,70,71,85,86,95,96,110,111,150,151]:advice(4,query=ratio);count+=1
    advice(4,{0xea1894:0});advice(4,query=150,mutate='provider');count+=2
    for level in [-50,-1,0,9,10,99,100]:advice(5,{0xea1888:level});count+=1
    for low,minimum in itertools.product(range(6),[19,20,39]):
        advice(6,moods=tuple([minimum]*low+[50]*(5-low)));count+=1
    for high in range(6):advice(6,moods=tuple([61]*high+[50]*(5-high)));count+=1
    advice(6,moods=(-128,127,40,60,0));count+=1
    for income,denominator,balance in itertools.product([-1,0,1,20],[0,10],[99,100,101]):
        advice(7,{0xea44ac:0,0xea44b0:income,0xea44b4:0,0xea44b8:denominator,0xea187c:balance});count+=1
    for value in [-1,0,1,2,4,5,9,10]:advice(8,{0xea5fd0:value});count+=1
    for town_data in [(),((37,0,0),),((37,0,1),),((37,5,0),),((37,6,0),),((37,7,0),),((0,7,1),(8,5,1))]:
        for status in [0,1]:advice(9,towns=town_data,status=status);count+=1
    for fields in [{0xea5eb4:1},{0xea5ea4:1},{0xea5ee8:-1}]:advice(9,fields);count+=1
    advice(9,towns=((37,0,1),),mutate='town');advice(9,mutate='provider');count+=2
    for total in range(8):
        advice(10,towns=tuple([(0,0,1),(8,0,0)]+[(i+1,0,1) for i in range(total)]));count+=1
    for quality in [-21,-1,0,1,20,100,0x7fffffff]:advice(11,query=quality);count+=1
    advice(0,button=False,player=0);count+=1
    print('PASS chief-advisor advice: %d native/C complete category, trade iterator and drawing cases'%count,flush=True)
    print('target SHA256',target_sha)
