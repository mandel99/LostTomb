"""Native/C ferry crossing scan, opposite-bank highlights and full preview.

Draw/route endpoints are controlled, with genuine scan/approach/orientation
compositions. No host game or physical renderer is started.
"""
from verify_options_startup import *

names={0x47f3e0:'restore_ferry_scan_crossing',0x43d010:'restore_ferry_image_orientation',
       0x43d030:'restore_ferry_highlight_approaches',0x43d300:'restore_ferry_preview'}


def prepare(u,state):
    reset(u,state);u.mem_write(SP-0x6000,b'\xa5'*0x6000)
    for address,value in [(0xbca304,90),(0xbca308,80),(0xbca2d8,1000),(0xbca2e0,0),
            (0xbca300,19330),(0xbca310,0),(0xbc9800,400),(0xbc96e4,2),
            (0x70e44c,36),(0x70e120,0xa5a51234),(0x70e13c,0x12345678),
            (0x5d1184,257),(0x5d1188,513),(0xf56be4,0x12340000),
            (0xf55bb4+257*8,0x23450000)]:put(u,address,value)
    for i,(x,y) in enumerate([(0,0),(30,15),(60,0),(30,-15)]):put(u,0x5cfcf0+i*8,x);put(u,0x5cfcf4+i*8,y)
    for id in [400,401]:
        r=0x7999d8+id*264;u.mem_write(r,bytes(264));word(u,r+6,84);word(u,r+8,79)
        byte(u,r+3,2);byte(u,r+170,1);put(u,r+196,-1)
    word(u,0xb4e6fc+19330*2,400)


def execute(c,u,token,args=()):
    if not c:return run(c,u,token,args,budget=20000000)
    for r,v in zip(registers,[0x11111111,0x22222222,0x33333333,saved[0],SP,*saved[1:]]):u.reg_write(r,v)
    u.reg_write(UC_X86_REG_EFLAGS,2);put(u,SP,STOP)
    for i,v in enumerate(args):put(u,SP+4+i*4,v)
    u.emu_start(sym(names[token]),STOP,count=20000000)
    assert u.reg_read(UC_X86_REG_EIP)==STOP,(hex(token),c,hex(u.reg_read(UC_X86_REG_EIP)))
    assert u.reg_read(UC_X86_REG_ESP)==SP+4
    assert [u.reg_read(registers[i]) for i in [3,5,6,7]]==saved
    return u.reg_read(UC_X86_REG_EAX)


def endpoint(c,state,token,name,n,action):state['hooks'][sym(name) if c else token]=(n,0,action)


def compare(values,label):
    assert values[0][0]==values[1][0],(label,'trace/result',
        next(((i,a,b) for i,(a,b) in enumerate(zip(values[0][0],values[1][0])) if a!=b),None),
        [len(v[0]) for v in values])
    equal(values,label)


def scan(direction=0,stop=2,flags=1,obstruction=0,linked=0,real_shore=False):
    values=[]
    for c,u,state in machines:
        prepare(u,state);put(u,0xbca310,linked)
        step=[-228,1,228,-1][direction%4];base=19330
        for i in range(44):put(u,0xaf598c+(base+i*step)*4,4);byte(u,0xadc36c+base+i*step,0)
        if stop:put(u,0xaf598c+(base+stop*step)*4,flags);byte(u,0xadc36c+base+stop*step,obstruction)
        if not real_shore:hook(c,state,0x47cbd0,2,lambda cpu,args:direction)
        else:
            for i,offset in enumerate([0,1,228,229]):
                put(u,0x5cccc8+i*4,offset)
                wanted=get(u,0x5dbda8+(direction%4)*16+i*4)
                put(u,0xaf598c+(base+offset)*4,4 if wanted else 0)
        execute(c,u,0x47f3e0,[90,80]);values.append(([],snapshot(u)))
    compare(values,('scan',direction,stop,flags,obstruction,linked,real_shore))


def offset(orientation,camera):
    values=[]
    for c,u,state in machines:
        prepare(u,state);put(u,0xbca2e0,camera)
        value=execute(c,u,0x43d010,[orientation]);values.append(([value],snapshot(u)))
    compare(values,('image offset',orientation,camera))


def providers(c,u,state,calls,fit=0,exists=0,shore=2,route=0,mutate=''):
    points=[];qcount=0
    def project(cpu,args):
        points.append(args[2:]);calls.append(('project',args[:2]))
        put(cpu,args[2],600+(args[0]&255)*2);put(cpu,args[3],300+(args[1]&255)*2)
        if mutate=='project' and len(points)==1:put(cpu,0x70e44c,51);put(cpu,0xbca304,91)
        return 0
    hook(c,state,0x4f0ba0,4,project)
    def existing(cpu,args):
        calls.append(('existing',args))
        if mutate=='existing':word(cpu,0xb4e6fc+19330*2,401)
        return exists
    hook(c,state,0x486cf0,1,existing)
    hook(c,state,0x47cc70,4,lambda cpu,args:calls.append(('fit',args)) or fit)
    def shoreline(cpu,args):
        nonlocal qcount
        qcount+=1;calls.append(('shore',args))
        return shore(qcount,args) if callable(shore) else shore
    hook(c,state,0x47cbd0,2,shoreline)
    def path(cpu,args):
        count=get(cpu,args[8]);has_path=args[7]!=0
        calls.append(('route',args[:7],has_path,count))
        if has_path:
            assert bytes(cpu.mem_read(args[7],1000))==b'\xff'*1000
            put(cpu,args[7],12345)
        put(cpu,args[8],count+1)
        return route
    hook(c,state,0x486a30,9,path)
    def draw(cpu,args,t):
        calls.append((t,args,get(cpu,0x70e120)))
        if mutate=='draw':
            put(cpu,0x70e44c,get(cpu,0x70e44c)+1);put(cpu,0xf56be4,get(cpu,0xf56be4)+7)
            if points:
                x,y=points[-1];put(cpu,x,get(cpu,x)+3);put(cpu,y,get(cpu,y)+5)
        return 0
    for t,n in [(0x53e590,3),(0x53f250,3),(0x53f4f0,4)]:hook(c,state,t,n,lambda cpu,args,t=t:draw(cpu,args,t),cleanup=0 if c else n*4)
    hook(c,state,0x43dd70,2,lambda cpu,args:calls.append(('valid footprint',args)) or 0)
    return points


def approaches(orientation=0,length=1,fit=0,exists=0,route=0,mutate=''):
    values=[]
    for c,u,state in machines:
        prepare(u,state);calls=[]
        # Different captured building ownership permits existing-bank highlights.
        for cell in range(15000,25000):word(u,0xb4e6fc+cell*2,401)
        word(u,0xb4e6fc+19330*2,400)
        providers(c,u,state,calls,fit,exists,((orientation+4)%8)//2,route,mutate)
        value=execute(c,u,0x43d030,[orientation,90,80,700,500,length])&255
        values.append((calls+[('AL',value)],snapshot(u)))
    compare(values,('approaches',orientation,length,fit,exists,route,mutate))


def preview(mode=0,exists=0,fit=0,valid=0,route=0,mutate='',real_scan=False,real_approach=False,direction=0,other=False):
    values=[]
    for c,u,state in machines:
        prepare(u,state);put(u,0xbca310,mode)
        if other:word(u,0xb4e6fc+19330*2,401)
        shore=(lambda n,args:direction if n==1 else (direction+2)%4) if real_scan and real_approach and mode==0 else direction
        calls=[];points=providers(c,u,state,calls,fit,exists,shore,route,mutate)
        if not real_scan:
            def crossing(cpu,args):
                calls.append(('crossing',args));put(cpu,0xbc96e0,direction*2);put(cpu,0xbc96e4,2);return 0
            endpoint(c,state,0x47f3e0,'restore_ferry_scan_crossing',2,crossing)
        else:
            step=[-228,1,228,-1][direction%4]
            put(u,0xaf598c+(19330+2*step)*4,1)
        if not real_approach:
            def approach(cpu,args):
                calls.append(('approach',args))
                if mutate=='approach' and points:
                    x,y=points[-1];put(cpu,x,get(cpu,x)+11);put(cpu,y,get(cpu,y)+13);put(cpu,0x70e44c,71)
                return 0x12340000|valid
            endpoint(c,state,0x43d030,'restore_ferry_highlight_approaches',6,approach)
        execute(c,u,0x43d300);values.append((calls,snapshot(u)))
    compare(values,('preview',mode,exists,fit,valid,route,mutate,real_scan,real_approach,direction,other))


if __name__=='__main__':
    n=0
    for direction,stop in itertools.product(range(4),[2,7,39,40,0]):
        for flags,obstruction,linked in [(1,0,0),(0,0,0),(4,1,0),(0x44,0,0),(0x44,0,1),(0x4c,0,256)]:
            scan(direction,stop,flags,obstruction,linked);n+=1
    for direction in [999,0x80000000,0x80000001]:scan(direction);n+=1
    for direction in range(4):scan(direction,real_shore=True);n+=1
    print('PASS crossing scan: %d native/C complete switch, terminal and obstruction cases'%n,flush=True)
    n=0
    for orientation,camera in itertools.product([0,2,4,6,-2,257],[0,2,4,6,256,257]):offset(orientation,camera);n+=1
    print('PASS orientation: %d native/C DWORD camera and signed arithmetic cases'%n,flush=True)
    n=0
    for orientation,length,fit,exists,route in itertools.product([0,2,4,6],[0,1,2],[0,1],[0,1],[0,1]):
        approaches(orientation,length,fit,exists,route);n+=1
    approaches(length=-1);n+=1
    for mutation in ['project','existing','draw']:approaches(exists=1,route=1,mutate=mutation);n+=1
    print('PASS opposite-bank approaches: %d native/C geometry, tint and callback cases'%n,flush=True)
    n=0
    for mode,exists,fit,valid,route in itertools.product([0,1,256],[0,1],[0,1],[0,1],[0,1,2]):
        preview(mode,exists,fit,valid,route,other=exists!=0);n+=1
    for mutation in ['project','existing','draw','approach']:preview(exists=1,valid=1,mutate=mutation);n+=1
    print('PASS full preview: %d native/C first/second bank, masks, buffers and branch cases'%n,flush=True)
    n=0
    for direction,exists,route in itertools.product(range(4),[0,1],[0,1]):
        preview(exists=exists,route=route,real_scan=True,real_approach=True,direction=direction);n+=1
    print('PASS actual scan/approach/preview compositions: %d native/C cases'%n,flush=True)
    print('target SHA256',target_sha)
