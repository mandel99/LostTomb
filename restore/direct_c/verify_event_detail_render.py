"""Whole native/C event detail and three rectangle-text helpers, offline.

Member predicates, dependency lookup and selection validation execute. UI
endpoints are controlled; composed cases execute real rectangle-text helpers.
No host game is launched and no game files or saves are changed.
"""
from verify_options_startup import *

for c,u,state in machines:u.mem_map(0x2fe00000,0x200000)

def record(index):return 0x785d74+index*124

def prepare(u,state,kind=1,flags=0,selected=0,count=150,subtype=1,hover=1,
            ranges=(-1,2,8),links=(1,2,3,4),choices=(1,2,3),style=4):
    reset(u,state);u.mem_write(SP-0x5000,b'\xa5'*0x5000)
    u.mem_write(0x785d74,bytes(150*124))
    for i in range(150):
        p=record(i);word(u,p,100+i);byte(u,p+2,1+i%29);put(u,p+40,1)
    index=selected if 0<=selected<150 else 0;p=record(index)
    byte(u,p+2,kind);byte(u,p+3,2);byte(u,p+54,style);byte(u,p+96,subtype)
    put(u,p+40,flags)
    for offset in [14,22,30,90]:
        for i,v in enumerate(ranges):word(u,p+offset+i*2,v)
    for offset,value in zip([36,38,82,84],links):word(u,p+offset,value)
    for offset,value in zip([6,8,10],choices):word(u,p+offset,value)
    word(u,p+44,1);word(u,p+56,2)
    for i in range(4):byte(u,p+106+i,250-i)
    for at,value in [(0x78a61c,selected),(0x785d70,count),(0x7056c8,hover),
                     (0xe38da8,257),(0xe38da4,-513),(0xf56f8c,0x12340200),
                     (0xf56fb4,0x23450300),(0xe20778,0x12345678),
                     (0xeaac28,1),(0x605a3c,0)]:put(u,at,value)
    word(u,0x7848d8,-331);byte(u,0x785d6c,31);byte(u,0xe38dda,31)
    return p

def detail(full=False,mutation='',erase=False,**settings):
    results=[]
    for c,u,state in machines:
        self=prepare(u,state,**settings);calls=[]
        endpoints={0x536820:4,0x4cd1a0:5,0x4cd240:7,0x4ccbb0:6,
                   0x4ccb30:7,0x4cce00:7}
        if full:endpoints.update({0x4cce90:8,0x4cccb0:6})
        else:endpoints.update({0x4cd290:8,0x4cd1f0:7,0x4cd2e0:8,0x4cd340:7})
        def observe(cpu,args,token):
            calls.append((token,args))
            put(cpu,0xe20778,get(cpu,0xe20778)+17)
            if mutation=='ui':
                put(cpu,0xe38da8,get(cpu,0xe38da8)+257)
                put(cpu,0xe38da4,get(cpu,0xe38da4)-513)
                put(cpu,0xf56f8c,get(cpu,0xf56f8c)+257)
                put(cpu,0x7056c8,(get(cpu,0x7056c8)+1)%31)
                put(cpu,0x78a61c,149)  # Native retains its actual entry record.
            elif mutation=='ranges' and token in (0x4cd2e0,0x4cce90):
                for offset in [14,22,30,90]:
                    for i in range(3):word(cpu,self+offset+i*2,257+i)
            return 0
        for token,n in endpoints.items():hook(c,state,token,n,lambda cpu,args,t=token:observe(cpu,args,t))
        if not erase:
            def erase_stub(cpu,args):
                if c:assert args[0]==0x785d70;args=args[1:]
                else:assert cpu.reg_read(UC_X86_REG_ECX)==0x785d70
                calls.append(('erase',args));return 1
            if c:state['hooks'][sym('restore_event_list_erase_00448e30')]=(2,0,erase_stub)
            else:hook(c,state,0x448e30,1,erase_stub,cleanup=4)
        try:run(c,u,0x44aaf0,budget=5000000)
        except Exception as error:
            raise AssertionError(('detail failed',c,hex(u.reg_read(UC_X86_REG_EIP)),settings,full,mutation)) from error
        results.append((tuple(calls),snapshot(u)))
    assert results[0][0]==results[1][0],('calls',settings,full,mutation,
        next(((i,a,b) for i,(a,b) in enumerate(zip(results[0][0],results[1][0])) if a!=b),None),
        [len(r[0]) for r in results])
    equal(results,('detail state',settings,full,mutation))

def rectangle_helpers():
    n=0
    for token,origin,point,shift in itertools.product([0x4cd1f0,0x4cd2e0,0x4cd340],
             [(257,-513),(-2147483648,2147483647),(0,0)],
             [(-32768,32767,32767),(-257,513,-300)],[(4,7),(-513,257)]):
        results=[]
        for c,u,state in machines:
            reset(u,state);put(u,0xe38da8,origin[0]);put(u,0xe38da4,origin[1])
            for i,v in enumerate(point):word(u,S+0x1000+i*2,v)
            args=[0x80000101,0x80000040]
            if token==0x4cd2e0:args+=[0x57c048]
            args+=[S+0x1000,0x12340200,0x2345abcd,*shift]
            endpoint={0x4cd1f0:0x4ccbb0,0x4cd2e0:0x4cce90,0x4cd340:0x4cccb0}[token]
            calls=[];hook(c,state,endpoint,8 if token==0x4cd2e0 else 6,
                lambda cpu,args:calls.append(args) or 0)
            run(c,u,token,args);results.append((tuple(calls),snapshot(u)))
        equal(results,('rectangle helper',hex(token),origin,point,shift));n+=1
    print('PASS complete rectangle helpers, signed bounds and32-bit position wrap:',n,flush=True)
    return n

if __name__=='__main__':
    detail()
    if '--one' in sys.argv:print('PASS one complete detail case');exit()
    n=1
    for kind,flags in itertools.product(range(30),[0,1,16,17]):detail(kind=kind,flags=flags);n+=1
    for selected in [-2147483648,-1,1,128,149,150,256]:detail(selected=selected);n+=1
    ranges=[(-1,2,8),(-1,8,2),(-1,-1,-1),(5,-1,9),(5,0,9),
            (-1,-32768,32767),(256,-1,257),(32767,32767,32767)]
    for kind,values in itertools.product([1,2,3,19,20,23],ranges):detail(kind=kind,ranges=values);n+=1
    for hover in range(31):detail(hover=hover);n+=1
    for links in [(-1,-1,-1,-1),(0,0,0,0),(100,100,100,100),
                  (149,149,149,149),(150,150,150,150),(256,256,256,256)]:
        detail(links=links);n+=1
    for kind,subtype,style in itertools.product([1,19,20,23],[0,2,3,4,5,255],[4,5,255]):
        detail(kind=kind,subtype=subtype,style=style);n+=1
    for kind,mutation in itertools.product(range(1,30),['ui','ranges']):
        detail(kind=kind,mutation=mutation);n+=1
    print('PASS whole detail, all kinds, range/selection/dependency fields and callback reloads:',n,flush=True)
    composed=0
    for kind,mutation in itertools.product([1,2,3,13,19,20,23,29],['','ui','ranges']):
        detail(kind=kind,mutation=mutation,full=True);composed+=1
    for selected,count in [(0,1),(0,3),(2,3)]:
        detail(kind=0,selected=selected,count=count,erase=True,full=True);composed+=1
    print('PASS real rectangle helpers and real record erasure compose with detail:',composed,flush=True)
    helpers=rectangle_helpers()
    print('PASS total cases:',n+composed+helpers,'target SHA256',target_sha,flush=True)
