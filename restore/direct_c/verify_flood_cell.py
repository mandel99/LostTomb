"""Whole4BD950 native/C flood transition and actor-link regression.

Controls area refresh, neighboring-water classification and building-query/
draw endpoints. Actor unlink/insert/death and x87 fertility conversion execute.
Final game stability and composed rendering are not certified by this fixture.
"""
from verify_options_startup import *


def exercise(flooding,flags,building=0,kind=71,category=0,query=0,editor=0,
             actor_kind=4,actor_id=9,mask=0,quality=15,active=1,control=0x37f,poison_stride=False):
    results=[]
    for c,u,state in machines:
        reset(u,state)
        u.mem_write(0xb350dc,bytes(228*228*2));u.mem_write(0xbcd5f8,bytes(2000*388))
        tile=12000;word(u,0xb4e6fc+tile*2,building);put(u,0xaf598c+tile*4,flags)
        put(u,S,S+0x100);put(u,S+8,tile);put(u,S+12,30);put(u,S+16,40)
        byte(u,S+24,active);put(u,S+0x108,quality)
        put(u,0xeaac28,1);put(u,0xea6208+0x49d8,editor)
        row=building*264
        byte(u,0x7999d8+row,1);word(u,0x7999e8+row,kind)
        word(u,0x7999de+row,35);word(u,0x7999e0+row,45)
        for y in range(3):
            for x in range(3):put(u,0xaf598c+(35+x+(45+y)*228)*4,8)
        if actor_id:
            word(u,0xb350dc+tile*2,actor_id)
            byte(u,0xbcd602+actor_id*388,actor_kind);byte(u,0xbcd606+actor_id*388,1)
            put(u,0xbcd618+actor_id*388,tile)
            for at,value in [(0xbcd60c,30),(0xbcd60e,40),(0xbcd642,32760),(0xbcd644,32760)]:
                word(u,at+actor_id*388,value)
        if poison_stride:
            # The captured crash read0x4040 as an actor through tile*8, then
            # dereferenced0x02425F00 through the second four-times stride.
            word(u,0xb350dc+tile*8,0x4040)
        calls=[]
        def area(token):
            return lambda cpu,args:calls.append((token,args)) or 0
        for token,count in [(0x517690,4),(0x516f90,5),(0x5173a0,4),(0x4777c0,4),
                            (0x475cc0,5),(0x47b590,4),(0x47bae0,5),(0x47bd50,4)]:
            hook(c,state,token,count,area(token))
        hook(c,state,0x488030,1,lambda cpu,args:calls.append((0x488030,args)) or category,
             cleanup=0 if c else 4)
        def neighbors(cpu,args):
            assert args==(4,)
            calls.append((0x47db00,args))
            for i in range(8):put(cpu,0xbc97c0+i*4,(mask>>i)&1)
            return 0
        hook(c,state,0x47db00,1,neighbors)
        if c:
            def member(cpu,args):
                site,token,frame=args;n=2 if token==0x488e60 else 1
                sp=get(cpu,frame+16);values=tuple(get(cpu,sp+4+i*4) for i in range(n))
                calls.append((token,get(cpu,frame+4),values))
                put(cpu,frame,query);put(cpu,frame+16,sp+4+n*4)
                return 0
            state['hooks'][sym('restore_building_refresh_dispatch')]=(3,0,member)
        else:
            for token,count in [(0x4894d0,1),(0x488e60,2)]:
                hook(c,state,token,count,
                     lambda cpu,args,t=token:calls.append((t,cpu.reg_read(UC_X86_REG_ECX),args)) or query,
                     cleanup=count*4)
        u.reg_write(UC_X86_REG_FPCW,control);u.reg_write(UC_X86_REG_FPSW,0);u.reg_write(UC_X86_REG_FPTAG,0xffff)
        run(c,u,0x4bd950,[S,flooding] if c else [flooding],cleanup=0 if c else 4,receiver=S)
        results.append((calls,snapshot(u),bytes(u.mem_read(S,0x200))))
    assert results[0]==results[1], ('flood cell',flooding,hex(flags),building,kind,category,query,
        editor,actor_kind,actor_id,mask,quality,active,
        'calls',results[0][0],results[1][0],
        [(hex(0x560000+i),a,b) for i,(a,b) in enumerate(zip(results[0][1],results[1][1])) if a!=b][:12])


def verify():
    cases=0
    for flooding,flags,kind in itertools.product([0,1],[0,4,0x40,0x8000004,8,12],[14,28,4,25]):
        exercise(flooding,flags,actor_kind=kind);cases+=1
    for kind,category,query,editor in itertools.product([71,77,88,89,90,93,99,138],[0,1],[0,1],[0,2]):
        exercise(1,12,building=400,kind=kind,category=category,query=query,editor=editor);cases+=1
        exercise(0,4,building=400,kind=kind,category=category,query=query,editor=editor);cases+=1
    for mask in range(256):
        exercise(0,4,actor_kind=25,mask=mask);cases+=1
    for control,quality,active in itertools.product([0x7f,0x27f,0x37f,0x77f,0xb7f,0xf7f],[-2147483648,-1,0,1,15,30,2147483647],[0,1]):
        exercise(1,0,actor_kind=4,quality=quality,active=active,control=control);cases+=1
    exercise(1,0,poison_stride=True);cases+=1
    print('PASS whole flood-cell transition, building branches,256 escape masks and x87 fertility:',cases)
    print('Verified target SHA256:',target_sha)


if __name__=='__main__':verify()
