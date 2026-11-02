"""Whole native/C event descriptions, labels, range query and list renderer.

Real member leaves and language selection execute. Only sprintf, final text,
panel/button and scrollbar endpoints are controlled. No host game runs.
"""
from verify_options_startup import *

SELF=S+0x1000
def string(u,p):
    out=bytearray()
    while True:
        b=bytes(u.mem_read(p,128));end=b.find(b'\0')
        if end>=0:return bytes(out)+b[:end]
        out.extend(b);p+=128

def setup(u,state,kind=1,flags=0,date=(-1,2,4),quantity=(-1,5,8),choices=(1,2,3),subtype=1):
    reset(u,state);u.mem_write(SP-0x5000,b'\xa5'*0x5000)
    u.mem_write(SELF,bytes(124));u.mem_write(0x785d74,bytes(150*124))
    u.mem_write(0x785c40,b'\xcc'*256)
    word(u,SELF,257);byte(u,SELF+2,kind);byte(u,SELF+3,subtype);put(u,SELF+40,flags)
    for start,values in [(6,choices),(14,quantity),(22,date)]:
        for i,v in enumerate(values):word(u,SELF+start+i*2,v)
    put(u,0xe20768,S+0x400);put(u,0x785d70,3)
    for i in range(150):
        p=0x785d74+i*124;word(u,p,i+20);byte(u,p+2,1);byte(u,p+96,1)
        for offset in [36,38,82,84]:word(u,p+offset,-1)

def endpoints(c,u,state,real=False,mutate=False,long_choices=False):
    calls=[]
    if real:
        data=(ROOT/'build/debug-startup-game/Pharaoh_Text.eng').read_bytes()
        u.mem_write(0xe34c38,data[28:8028]);u.mem_write(S+0x10000,data[8028:])
        put(u,0xe36c70,S+0x10000)
    else:
        def select(cpu,args):
            group,entry=args;value=('g%de%d'%(group,entry)).encode()
            if long_choices and group not in (20,25,156):value+=b'X'*40
            # Stable pointers per actual call, retaining all earlier strings.
            p=S+0x4000+len(calls)*128
            cpu.mem_write(p,value+b'\0');put(cpu,0xe20768,p)
            calls.append(('select',args));return 0
        hook(c,state,0x4cc110,2,select)
    def sprintf(cpu,args):
        destination,fmt=args;pattern=string(cpu,fmt);values=[];logged=[]
        sp=cpu.reg_read(UC_X86_REG_ESP)
        for i,spec in enumerate(re.findall(rb'%[0-9]*[ds]',pattern)):
            value=get(cpu,sp+12+i*4)
            value=string(cpu,value) if spec.endswith(b's') else (value^0x80000000)-0x80000000
            values.append(value);logged.append(value)
        rendered=pattern%tuple(values)
        cpu.mem_write(destination,rendered+b'\0')
        calls.append(('format',fmt,tuple(logged),rendered))
        if mutate and fmt==0x5da168:
            put(cpu,0xe38da8,get(cpu,0xe38da8)+257)
            put(cpu,0xe38da4,get(cpu,0xe38da4)-513)
            put(cpu,0xf56f94,get(cpu,0xf56f94)+257)
        return len(rendered)
    hook(c,state,0x561a36,2,sprintf)
    return calls

def description(real=False,relation=None,count=3,long_choices=False,**settings):
    results=[]
    for c,u,state in machines:
        setup(u,state,**settings);put(u,0x785d70,count)
        if relation:
            index,offset,kind,subtype=relation;p=0x785d74+index*124
            word(u,p+offset,257);byte(u,p+2,kind);byte(u,p+96,subtype)
        calls=endpoints(c,u,state,real,long_choices=long_choices)
        try:result=run(c,u,0x448f60,[SELF] if c else [],receiver=None if c else SELF)
        except Exception as error:
            raise AssertionError(('formatter failed','C' if c else 'native',hex(u.reg_read(UC_X86_REG_EIP)),settings)) from error
        assert result==0x785c40 and get(u,0xe20768)==S+0x400
        assert u.mem_read(0x785c68,1)==b'\0'
        results.append(((tuple(calls),string(u,result)),snapshot(u)))
    equal(results,('event description',settings,real,relation,count))
    return results[1][0][1]

def labels_and_query():
    n=0
    for kind in [-2147483648,-1,*range(31),256,2147483647]:
        results=[]
        for c,u,state in machines:
            setup(u,state);calls=endpoints(c,u,state)
            value=run(c,u,0x448f20,[kind]);text=string(u,value)
            assert get(u,0xe20768)==S+0x400
            results.append(((tuple(calls),text),snapshot(u)))
        equal(results,('kind label',kind));n+=1
    for kind in range(256):
        values=[]
        for c,u,state in machines:
            setup(u,state,kind=kind)
            values.append(run(c,u,0x449620,[SELF] if c else [],receiver=None if c else SELF))
        assert values[0]==values[1],('quantity table',kind,values);n+=1
    print('PASS kind bounds/selection restoration and full-byte quantity table:',n,flush=True)
    return n

def list_draw(count=15,scroll=0,selected=257,hover=13,mutate=False,real=False):
    results=[]
    for c,u,state in machines:
        setup(u,state)
        for at,value in [(0x785d70,count),(0x78a620,scroll),(0x78a61c,selected),
                         (0x7056c8,hover),(0xe38da8,257),(0xe38da4,-513),
                         (0xf56f8c,0x12340100),(0xf56f94,0x23450200),(0xf56f9c,0x34560300)]:put(u,at,value)
        for i in range(150):
            p=0x785d74+i*124;word(u,p,257 if i==0 else -32768+i)
            byte(u,p+2,1+i%29);byte(u,p+3,i%6)
            for offset in [6,8,10,14,16,18,22,24,26]:word(u,p+offset,-1)
        calls=endpoints(c,u,state,real,mutate)
        for i in range(12):word(u,0x5d9a90+i*28,-257+i);word(u,0x5d9a92+i*28,513+i*16)
        def panel(cpu,args,token):
            calls.append((token,args))
            if mutate:
                put(cpu,0x7056c8,(get(cpu,0x7056c8)+1)%15)
                put(cpu,0xf56f8c,get(cpu,0xf56f8c)+257)
            return 0
        for token,n in [(0x536820,4),(0x4cd1a0,5),(0x4cd240,7),(0x425730,4),(0x4484b0,0)]:
            hook(c,state,token,n,lambda cpu,args,t=token:panel(cpu,args,t))
        def draw(cpu,args):
            calls.append(('draw',string(cpu,args[0]),args[1:]));return 0
        hook(c,state,0x4cc770,5,draw)
        run(c,u,0x448640,budget=5000000)
        results.append((tuple(calls),snapshot(u)))
    equal(results,('event list',count,scroll,selected,hover,mutate,real))
    return sum(x[0]=='draw' for x in results[1][0])

if __name__=='__main__':
    print('description sample:',description(),flush=True)
    if '--one' in sys.argv:exit()
    n=labels_and_query()
    cases=1
    for kind,flags in itertools.product(range(30),[0,1,16,17,0x100]):
        description(kind=kind,flags=flags);cases+=1
    ranges=[(-1,2,4),(-1,4,2),(-1,-1,-1),(5,-1,8),(5,0,8),
            (256,-1,257),(-1,-32768,32767),(32767,32767,32767)]
    for date,quantity in itertools.product(ranges,ranges):
        description(date=date,quantity=quantity);cases+=1
    for relation in [(i,offset,kind,sub) for i in [0,2] for offset,kind,sub in
                     [(36,2,0),(38,1,0),(38,23,0),(38,2,0),(82,23,0),
                      (84,1,1),(84,1,2),(84,1,3),(84,23,1)]]:
        description(flags=1,relation=relation);cases+=1
    for count in [-1,0,1]:description(flags=1,count=count);cases+=1
    for choices in [(-1,-1,-1),(0,0,0),(1,-1,2),(2,2,2),(0,1,0)]:
        description(choices=choices);cases+=1
    for kind in range(30):description(kind=kind,real=True,choices=(-1,0,1));cases+=1
    assert len(description(long_choices=True))==40;cases+=1
    print('PASS complete formatter, signed ranges, flags, dependencies and real catalog:',cases,flush=True)
    rows=0
    for count,scroll in itertools.product([0,1,12,15,150,256],[-1,0,1,128,257]):
        list_draw(count=count,scroll=scroll);rows+=1
    for selected,hover,mutate,real in itertools.product([257,-32767,65535],[0,12,13,14],[False,True],[False,True]):
        list_draw(selected=selected,hover=hover,mutate=mutate,real=real);rows+=1
    assert list_draw(count=12)==12;rows+=1
    print('PASS12-row renderer, full scroll/font/ID fields and formatter composition:',rows,flush=True)
    print('PASS total cases:',n+cases+rows,'target SHA256',target_sha,flush=True)
