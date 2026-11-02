"""Native/C cell targeting and complete movement/combat state machines.

Queries execute without substituted providers. Movement-selector cases control
4A5DA0; combat cases additionally observe route reset, projectile publication,
sound and map-effect endpoints. The root state machines and their genuine
cell/nearest-target/direction queries execute. This is not a live-game proof.
Unreferenced typed facades may be discarded: those cases invoke the actual
compiled complete civic body with an explicit original frame instead.
"""
from verify_options_startup import *


for c,u,state in machines:
    state['controls']={}
    if c:
        def boundary(uc,pc,size,state):
            real_sp=uc.reg_read(UC_X86_REG_ESP)
            token=get(uc,real_sp+8)
            controlled=state['controls'].get(token)
            if controlled is None:return
            count,fn=controlled
            frame=get(uc,real_sp+12);sp=get(uc,frame+16)
            args=tuple(get(uc,sp+4+i*4) for i in range(count))
            put(uc,frame,fn(uc,args)&0xffffffff);put(uc,frame+16,sp+4)
            uc.reg_write(UC_X86_REG_ESP,real_sp+4)
            uc.reg_write(UC_X86_REG_EIP,get(uc,real_sp))
        at=sym('restore_actor_civic_dispatch')
        u.hook_add(UC_HOOK_CODE,boundary,state,begin=at,end=at)


def prepare(u,state):
    reset(u,state);state['controls'].clear()
    u.mem_write(0xb350dc,bytes(228*228*2))
    u.mem_write(0xbcd5f8,bytes(2000*388))
    put(u,0xbca2d8,43+51*228)
    for at,value in [(0xbca2d0,100),(0xbca2d4,100),(0xbca2e8,43),
                     (0xbca2ec,51),(0xbca2f0,142),(0xbca2f4,150)]:put(u,at,value)
    # A bounded navigable lake. An all-zero whole-map mask makes the real
    # 51CE90 flood search traverse beyond the intended fixture domain.
    u.mem_write(0xed4140,b'\xff'*(228*228))
    for y in range(45,56):
        at=get(u,0xbca2d8)+45+y*228
        u.mem_write(0xed4140+at,bytes(11))
        u.mem_write(0xaf598c+at*4,struct.pack('<I',4)*11)
    for reg,value in [(UC_X86_REG_FPCW,0x37f),(UC_X86_REG_FPSW,0),(UC_X86_REG_FPTAG,0xffff)]:
        u.reg_write(reg,value)
    put(u,S+0x400,0x12345678)


def actor(u,ident,kind=77,x=50,y=50,direction=0):
    for at,value in [(0xbcd602,kind),(0xbcd606,1),(0xbcd609,direction),
                     (0xbcd60a,direction),(0xbcd630,10)]:byte(u,at+ident*388,value)
    for at,value in [(0xbcd60c,x),(0xbcd60e,y),(0xbcd61c,x+2),
                     (0xbcd61e,y),(0xbcd628,x),(0xbcd62a,y)]:word(u,at+ident*388,value)
    put(u,0xbcd720+ident*388,1000)
    return get(u,0xbca2d8)+x+y*228


def execute(c,u,token,args):
    if not c or re.search(r'\s_FUN_%08x\s'%token,symbols):
        return run(c,u,token,args)
    # The actual C body uses this real memory frame; no locals or inherited
    # tables are fabricated. Keep host and modeled stacks disjoint.
    f=S+0x100;real_sp=SP-0x1000
    values=[0x11111111,0x22222222,0x33333333,saved[0],SP,*saved[1:]]
    for i,value in enumerate(values):put(u,f+4*i,value)
    put(u,f+32,2);put(u,f+36,sym('restore_actor_civic_dispatch'))
    put(u,SP,STOP)
    for i,value in enumerate(args):put(u,SP+4+4*i,value)
    put(u,real_sp,STOP);put(u,real_sp+4,f)
    for r,value in zip(registers,values):u.reg_write(r,value)
    u.reg_write(UC_X86_REG_ESP,real_sp);u.reg_write(UC_X86_REG_EFLAGS,2)
    u.emu_start(sym('restore_civic_C_%08x'%token),STOP,count=2000000)
    assert u.reg_read(UC_X86_REG_EIP)==STOP,(hex(token),c,hex(u.reg_read(UC_X86_REG_EIP)))
    assert u.reg_read(UC_X86_REG_ESP)==real_sp+4
    assert get(u,f+16)==SP+4
    assert [get(u,f+4*i) for i in [3,5,6,7]]==saved
    return get(u,f)


def control(c,state,token,count,fn):
    if c:state['controls'][token]=(count,fn)
    else:hook(c,state,token,count,fn)


def endpoints(c,state,calls,movement=True):
    entries={0x4b3250:1,0x4b45c0:6,0x48c0e0:4,0x53b090:1}
    if movement:entries[0x4a5da0]=2
    for token,count in entries.items():
        def observe(uc,args,token=token):calls.append((token,args));return 0
        control(c,state,token,count,observe)


def compare(token,args,setup,query=False,controlled=False):
    results=[]
    for c,u,state in machines:
        prepare(u,state);setup(u)
        calls=[]
        if controlled:endpoints(c,state,calls)
        value=execute(c,u,token,args)
        results.append((value if query else None,calls,get(u,S+0x400),snapshot(u)))
    equal(results,(hex(token),args))
    return results[1]


def verify_queries():
    count=0
    for token,kinds in [(0x4a59c0,{25,77,78}),(0x4a5a20,{92,93,100,101})]:
        for kind in range(256):
            def setup(u):
                word(u,0xb350dc+51983*2,1)
                word(u,0xbcd600+388,511)
                byte(u,0xbcd602+388,kind)
                byte(u,0xbcd602+511*388,kind)
                # Adjacent bytes distinguish a BYTE kind from a DWORD load.
                byte(u,0xbcd603+511*388,0xa5)
            value=compare(token,[51983,1],setup,query=True)[0]
            assert value==(511 if kind in kinds else 0),(hex(token),kind,value)
            count+=1
        for length,excluded in [(0,0),(1,1),(25,1),(1999,1),(1999,1999)]:
            def setup(u):
                word(u,0xb350dc+18000*2,1 if length else 0)
                for ident in range(1,length+1):
                    word(u,0xbcd600+ident*388,ident+1 if ident<length else 0)
                    byte(u,0xbcd602+ident*388,9 if ident<length else min(kinds))
            compare(token,[18000,excluded],setup,query=True);count+=1
    print('PASS both complete cell queries, all256 kinds, exclusion and1999-node chains:',count,flush=True)

    count=0;deltas=[-228,-227,1,229,228,227,-1,-229]
    for ident,direction,where in itertools.product([1,511,1999],[-128,-1,*range(11),127],['current','neighbor','none']):
        def setup(u):
            tile=actor(u,ident,77,direction=direction)
            other=2 if ident!=2 else 3
            actor(u,other,92);word(u,0xb350dc+tile*2,ident)
            if where=='current':word(u,0xbcd600+ident*388,other)
            if where=='neighbor' and direction in range(8):word(u,0xb350dc+(tile+deltas[direction])*2,other)
        value=compare(0x4a5a80,[ident],setup,query=True)[0]
        assert value==(2 if direction in range(8) and where!='none' else 0)
        count+=1
    print('PASS all eight neighbors, current-cell priority and signed invalid directions:',count,flush=True)

    count=0
    for ident,size,mode,layout in itertools.product([1,511],[-3,0,1,2,3,18],[0,1,256],['empty','nearest','tie']):
        def setup(u):
            tile=actor(u,ident)
            if layout=='empty':return
            kind=92 if mode==0 else 77
            for candidate,x,y in [(1998,49,50),(1999,51,50)]:
                actor(u,candidate,kind,x,y)
                word(u,0xb350dc+(tile+x-50+(y-50)*228)*2,candidate)
            if layout=='nearest':
                actor(u,1997,kind);word(u,0xb350dc+tile*2,1997)
        compare(0x4a5560,[ident,size,mode],setup,query=True);count+=1
    print('PASS genuine square search, both kind sets, full mode and scan-order ties:',count,flush=True)


def verify_movement():
    count=0
    vectors=[(0,0,speed,0,0,False) for speed in [0,4,9,20,256]]
    vectors += [(phase,1,4,timer,steps,False) for phase,timer,steps in
                [(0,0,0),(0,255,0),(0,0,2),(0,0,256),(1,198,0),
                 (1,199,0),(2,198,0),(2,199,0),(255,199,0)]]
    vectors += [(0,0,4,0,0,True)]
    for ident,(phase,mode,speed,timer,steps,arrived) in itertools.product([1,511,1999],vectors):
        def setup(u):
            actor(u,ident,77,direction=8 if arrived else 2)
            byte(u,0xbcd74c+ident*388,phase)
            put(u,0xbcd730+ident*388,timer);put(u,0xbcd72c+ident*388,steps)
            for at,value in [(0x7090ec,speed),(0x7090f4,2),(0x7090f8,256)]:put(u,at+77*56,value)
            if arrived:word(u,0xbcd61c+ident*388,50)
        compare(0x4a5680,[ident,mode],setup,controlled=True);count+=1
    print('PASS complete movement phases, DWORD timers/models and speed selection:',count,flush=True)

    count=0
    for ident,direction in itertools.product([1,511],range(8)):
        def setup(u):
            tile=actor(u,ident,77,direction=direction)
            put(u,0xbcd618+ident*388,tile);word(u,0xb350dc+tile*2,ident)
            put(u,0x7090ec+77*56,4)
        compare(0x4a5680,[ident,0],setup);count+=1
    print('PASS movement composition with genuine routing and cell-link providers:',count,flush=True)


def verify_combat():
    count=0
    for ident,direction,cooldown,fatigue,random in itertools.product([1,511],[0,3,7,8,9,10],[0,1,16,256,-1],[0,256],[0,2]):
        def setup(u):
            tile=actor(u,ident,77,direction=direction);actor(u,1999,92,direction=8)
            byte(u,0xbcd60a+1999*388,4)
            word(u,0xb350dc+tile*2,ident);word(u,0xbcd600+ident*388,1999)
            byte(u,0xbcd74c+ident*388,2)
            put(u,0xbcd75c+ident*388,cooldown);put(u,0xbcd72c+ident*388,fatigue)
            put(u,0xbcd748+ident*388,35);put(u,0xbcd750+ident*388,70)
            put(u,0x7090f4+77*56,2);put(u,0xd3b374,random)
        compare(0x4a6160,[ident,S+0x400],setup,controlled=True);count+=1
    print('PASS whole melee movement/query/damage/cooldown/direction composition:',count,flush=True)


def verify_combat_states():
    count=0
    for token,ident,phase,action in itertools.product([0x4a6300,0x4a6d40],[1,511],[0,9,11,16,21,22,256],[10,23]):
        def setup(u):
            actor(u,ident,77,direction=8);actor(u,1999,92,x=52,y=50,direction=4)
            word(u,0xbcd67a+ident*388,1999);byte(u,0xbcd630+ident*388,action)
            put(u,0xbcd754+ident*388,phase);byte(u,0xbcd74c+ident*388,2)
            put(u,0x7090e4+77*56,256);put(u,0x7090e8+77*56,0)
            byte(u,0xbcd663+ident*388,1)
        compare(token,[ident,S+0x400],setup,controlled=True);count+=1
    print('PASS complete melee/ranged target-state switches and projectile publication:',count,flush=True)


def verify_arrival():
    count=0
    for ident,arrived,linked,action in itertools.product([1,511,1999],[False,True],[False,True],[10,13,20,23]):
        def setup(u):
            actor(u,ident,direction=2);byte(u,0xbcd630+ident*388,action)
            if arrived:word(u,0xbcd61c+ident*388,50)
            word(u,0xbcd652+ident*388,3999 if linked else 0)
            word(u,0x799a8c+3999*264,ident)
        compare(0x4a6890,[ident,S+0x400],setup,controlled=True);count+=1
    print('PASS arrival with genuine actor/building linkage, action exclusions and movement flag:',count,flush=True)

    count=0
    for token,ident,phase,timer,present in itertools.product([0x4a6970,0x4a4a80],[1,511],[0,9,11,16,256],[0,74,75,32767],[False,True]):
        def setup(u):
            tile=actor(u,ident,direction=8)
            word(u,0xbcd61c+ident*388,50)
            put(u,0xbcd754+ident*388,phase);word(u,0xbcd62e+ident*388,timer)
            if present:
                actor(u,1999,92,x=51,y=50)
                word(u,0xb350dc+(tile+1)*2,1999)
        compare(token,[ident,S+0x400],setup,controlled=True);count+=1
    print('PASS retarget timer/WORD wrap and genuine search/direction/destination selection:',count,flush=True)

    count=0
    for token,(dx,dy) in itertools.product([0x4a6970,0x4a4a80],
                                          [(0,-1),(1,-1),(1,0),(1,1),(0,1),(-1,1),(-1,0),(-1,-1)]):
        def setup(u):
            tile=actor(u,511,direction=8);word(u,0xbcd61c+511*388,50)
            put(u,0xbcd754+511*388,16);word(u,0xbcd62e+511*388,74)
            actor(u,1999,92,x=50+dx,y=50+dy)
            word(u,0xb350dc+(tile+dx+dy*228)*2,1999)
        compare(token,[511,S+0x400],setup,controlled=True);count+=1
    print('PASS all retarget direction-table branches with genuine water reachability:',count,flush=True)

    count=0
    for timer in [0,18,19,20,32767,-1]:
        def setup(u):actor(u,511);word(u,0xbcd62e+511*388,timer)
        compare(0x4a4f60,[511],setup);count+=1
    for effect in [-1,0,1,2,3,4,5,6,256]:
        compare(0x4a5880,[effect],lambda u:None,controlled=True);count+=1
    print('PASS periodic genuine direction query and all sound-selector branches:',count,flush=True)


def verify_fill():
    count=0
    for length,value,alignment in itertools.product([0,1,2,3,4,5,7,8,31,256,0x32c40],
                                                    [0,255,0x123400a5],[0,1,3]):
        states=[]
        for c,u,state in machines:
            prepare(u,state);u.mem_write(S+0x1000,b'\x6d'*(length+16))
            run(c,u,0x40f8f0,[S+0x1004+alignment,length,value])
            data=bytes(u.mem_read(S+0x1000,length+16))
            assert data==b'\x6d'*(4+alignment)+bytes([value&255])*length+b'\x6d'*(12-alignment)
            states.append((data,snapshot(u)))
        assert states[0]==states[1],('fill',length,value,alignment)
        count+=1
    print('PASS complete native byte fill, unaligned spans, tails and large map buffers:',count,flush=True)
    for c,u,state in machines:
        prepare(u,state);before=snapshot(u)
        run(c,u,0x40f8f0,[0,0,0x123400a5])
        assert snapshot(u)==before
    print('PASS zero-length null-destination fill without memory access',flush=True)


if __name__=='__main__':
    verify_queries();verify_movement();verify_combat();verify_combat_states();verify_arrival();verify_fill()
    print('Verified target SHA256:',target_sha)
