"""Whole typed ground/desirability/resource overlays versus pinned native.

Only final sprites,432920 status publication and declared building/quantity
queries are controlled. Pyramid coordinate projection and x87 conversion run.
No pixel/live-game proof or certification of every downstream query policy.
"""
from verify_options_startup import *


def setup(u,state,kind=115,building=511,flags=8,footprint=0,construct=0,view=0,orientation=0):
    reset(u,state);u.mem_write(0x7999d8,bytes(4000*264))
    for at,value in [(0x70e488,18000),(0xaf598c+18000*4,flags),(0xb7482c+18000*4,20000),
                     (0xf56bdc,21000),(0xf56b8c,22000),(0xf56ca4,23000),(0xe94e0c,view),
                     (0x70e45c,29),(0x70e458,58),(0x70e454,87),(0x70e450,116),
                     (0x70e44c,15),(0x70e448,30),(0x70e444,45),(0x70e440,60),
                     (0xf55bb4+554*8,24000),(0xf55bb4+555*8,25000),(0xbca2e0,orientation),
                     (0x70e474,8),(0x70e478,4),(0x70e494+(8*231+4)*4,18000),
                     (0x70e490,400),(0x70e48c,300),(0xbca2e8,43),(0xbca2ec,51)]:put(u,at,value)
    word(u,0xb4e6fc+18000*2,building);byte(u,0xb67d1c+18000,64)
    byte(u,0xb285cc+18000,footprint);word(u,0x7999e8+building*264,kind)
    byte(u,0x7999dd+building*264,construct)
    word(u,0x7999de+building*264,50);word(u,0x7999e0+building*264,50)
    byte(u,0xacf85c+18000,7)
    for reg,value in [(UC_X86_REG_FPCW,0x37f),(UC_X86_REG_FPSW,0),(UC_X86_REG_FPTAG,0xffff)]:u.reg_write(reg,value)


def spies(c,state,calls,root=511,show=0,quantity=800):
    roots=iter(root if isinstance(root,tuple) else (root,)*20)
    for token,n in [(0x475b50,1),(0x431690,1),(0x45d510,2),(0x432920,1),
                    (0x53e590,3),(0x53f250,3)]:
        def fn(uc,args,token=token):
            calls.append((token,args))
            if token==0x475b50:return next(roots)
            if token==0x431690:return show
            if token==0x45d510:return quantity
            return 0x12345678
        hook(c,state,token,n,fn,cleanup=12 if not c and token in [0x53e590,0x53f250] else 0)


def compare(token,args,init,root=511,show=0,quantity=800):
    results=[]
    for c,u,state in machines:
        init(u,state);calls=[];spies(c,state,calls,root,show,quantity)
        run(c,u,token,args)
        results.append((calls,u.reg_read(UC_X86_REG_FPCW),snapshot(u)))
    assert results[0]==results[1],(hex(token),args,root,show,quantity,
        results[0][0][:15],results[1][0][:15],
        [(hex(0x560000+i),a,b) for i,(a,b) in enumerate(zip(results[0][-1],results[1][-1])) if a!=b][:12])


def verify_desirability():
    count=0
    for appeal,upper,construct in itertools.product([-128,-11,-10,-9,-1,0,9,10,69,70,127],[0,1,256],[0,1]):
        def init(u,s):setup(u,s,construct=construct);byte(u,0xac2d4c+18000,appeal)
        compare(0x433760,[80,100,upper],init);count+=1
    for flags,footprint,upper,edge in itertools.product([1,0x4000,0x100,0],[0,1,2,3,4,5,6,15],[0,1],[0,64]):
        def init(u,s):setup(u,s,flags=flags,footprint=footprint);byte(u,0xb67d1c+18000,edge)
        compare(0x433760,[0x7ffffff0,0x80000010,upper],init);count+=1
    print('PASS complete desirability, signed appeal, renderer BYTE, map flags and footprint shifts:',count,flush=True)


def verify_resources():
    count=0
    values=[-32768,-1,0,1,355,356,711,712,800,1066,1067,1777,1778,3000,32767]
    for kind,value in itertools.product([70,71,72,73,190,191,193,115],values):
        for construct in ([0,1] if kind==115 else [0]):
            def init(u,s):
                setup(u,s,kind=kind,construct=construct)
                word(u,0x799a2e+511*264+2,value);word(u,0x799a2e+511*264+14,value+101)
                word(u,0x799a2c+511*264+14,value)
            compare(0x434aa0,[80,100,1,7],init,root=512,quantity=value);count+=1
    for flags,footprint,edge in itertools.product([0,1,0x100,0x4000],[0,1,2,3,4,5,6,15],[0,64]):
        def init(u,s):setup(u,s,flags=flags,footprint=footprint);byte(u,0xb67d1c+18000,edge)
        compare(0x434aa0,[0x7ffffff0,0x80000010,1,7],init);count+=1
    for value,cw in itertools.product([0x12345678,-0x12345678,2147483647,-2147483648],
                                      [0x27f,0x37f,0x77f,0xb7f,0xf7f]):
        def init(u,s):setup(u,s,kind=72);u.reg_write(UC_X86_REG_FPCW,cw)
        compare(0x434aa0,[80,100,1,7],init,quantity=value);count+=1
    print('PASS all resource selectors, WORD/DWORD quantities, construction scale and native x87 modes:',count,flush=True)


def verify_tiles():
    count=0
    for footprint,construct,tint in itertools.product(range(8),[0,1],[0,257]):
        def init(u,s):setup(u,s,footprint=footprint,construct=construct)
        compare(0x432d30,[511,80,100,tint],init);count+=1
    for kind,footprint,direction in itertools.product([30,31,33,72,73,209],[0,3],[0,2,4,6]):
        def init(u,s):setup(u,s,kind=kind,footprint=footprint,orientation=direction,flags=0x60)
        compare(0x432d30,[511,80,100,257],init);count+=1
    for footprint,construct,current,remote,show in itertools.product(range(7),[0,1],[0,1],[0,1],[0,1]):
        def init(u,s):
            setup(u,s,footprint=footprint,construct=construct,view=31)
            put(u,0x799ad8+511*264,current);put(u,0x799ad8+512*264,remote)
            byte(u,0x799a9b+511*264,1)
        compare(0x432d30,[511,80,100,257],init,root=512,show=show);count+=1
    for footprint in range(3):
        def init(u,s):
            setup(u,s,footprint=footprint,view=31);put(u,0x799ad8+513*264,1)
        compare(0x432d30,[511,80,100,257],init,root=(512,513));count+=1
    for variant,direction,built in itertools.product([0,2,4,6],[0,2,4,6],[0,1]):
        def init(u,s):
            setup(u,s,kind=65,orientation=direction);byte(u,0x799a82+511*264,variant)
            word(u,0x799a10+511*264,built)
        compare(0x432d30,[511,80,100,257],init);count+=1
    compare(0x432d30,[0,80,100,257],lambda u,s:setup(u,s));count+=1
    print('PASS full tile tables, problem-marker asymmetry, query reload and genuine pyramid projection:',count,flush=True)


def verify_predicate():
    count=0
    for mode,kind,layout in itertools.product([0,8,51,256],[-1,0,71,300],['null','empty','hit','miss']):
        results=[]
        for c,u,state in machines:
            setup(u,state,kind=kind,view=mode)
            put(u,0x5d25ac+mode*4,0 if layout=='null' else S+0x1000)
            put(u,S+0x1000,0 if layout=='empty' else kind if layout=='hit' else kind+1)
            put(u,S+0x1004,0)
            value=run(c,u,0x431690,[511]);results.append((value,snapshot(u)))
        assert results[0]==results[1],('predicate',mode,kind,layout,results[0][0],results[1][0])
        count+=1
    print('PASS canonical predicate with true partial-AL/full-EAX result:',count,flush=True)


def status_case(kind=115,construction=0,active=1,materials=1,remaining=0,stage=30,
                delay=0,delay2=0,material_flag=0,fire=0,damage=0,required=10,
                workers=10,access=1,blocked=0,gate=0,classify=0,available=0,
                stock=1,cart='none',tag=0,boat=0,marker=0):
    states=[]
    for c,u,state in machines:
        setup(u,state,kind=kind,construct=construction)
        o=511*264
        for at,value in [(0x7999d8,active),(0x799a5d,stage),(0x799a5f,material_flag),(0x799a9b,marker)]:byte(u,at+o,value)
        for at,value in [(0x7999f4,materials),(0x7999f2,access),(0x799a1e,fire),(0x799a20,damage),
                         (0x799a18,workers),(0x799a14,stock),(0x799a8c,tag)]:word(u,at+o,value)
        for at,value in [(0x799ac4,remaining),(0x799ad4,delay),(0x799abc,delay2),(0x799ad8,0xa5a5a5a5)]:put(u,at+o,value)
        put(u,0x705eb8+kind*40,required)
        if cart!='none':
            word(u,0x799a02+o,1999);byte(u,0xbcd602+1999*388,9 if cart=='wrong' else 4)
            byte(u,0xbcd630+1999*388,9 if cart=='busy' else 8)
            byte(u,0xbcd65e+1999*388,0 if cart=='empty' else 255)
        calls=[]
        for token,n in [(0x488030,1),(0x488050,1),(0x459ff0,1),(0x459240,1),(0x460fb0,1),(0x454190,1)]:
            def fn(uc,args,token=token):
                calls.append((token,(get(uc,args[0]),) if token==0x460fb0 else args))
                if token==0x460fb0:put(uc,args[0],25)
                return {0x488030:classify,0x488050:gate,0x459ff0:available,
                        0x459240:blocked,0x460fb0:boat,0x454190:0}[token]
            if c and token==0x488050:state['hooks'][sym('restore_actor_building_gate')]=(1,0,fn)
            else:hook(c,state,token,n,fn,cleanup=4 if not c and token in [0x488030,0x488050] else 0)
        run(c,u,0x432920,[511]);states.append((calls,snapshot(u)))
    assert states[0]==states[1],('status',kind,construction,active,materials,remaining,stage,
        delay,delay2,material_flag,fire,damage,required,workers,access,blocked,gate,classify,
        available,stock,cart,tag,boat,marker,states[0][0],states[1][0],
        [(hex(0x560000+i),a,b) for i,(a,b) in enumerate(zip(states[0][-1],states[1][-1])) if a!=b][:12])


def verify_status():
    count=0
    for kind in range(256):status_case(kind=kind,required=0,stock=0);count+=1
    for active,materials,remaining in itertools.product([0,1],[0,1],[-1,0,1,2,3,256]):
        status_case(construction=1,active=active,materials=materials,remaining=remaining);count+=1
    for flag,delay,delay2,stage in itertools.product([0,1,2],[800,801],[800,801],[29,30,70,72,73]):
        status_case(construction=1,material_flag=flag,delay=delay,delay2=delay2,stage=stage);count+=1
    for fire,damage,marker in itertools.product([0,800,801,32767,-32768],[0,800,801,32767,-32768],[0,1]):
        status_case(fire=fire,damage=damage,marker=marker,required=0);count+=1
    for required,workers,(access,gate,blocked) in itertools.product([0,10,256],[-1,0,9,10,257],
                                                                  [(0,0,0),(0,1,0),(1,0,0),(1,0,256)]):
        status_case(required=required,workers=workers,access=access,gate=gate,blocked=blocked);count+=1
    for kind,cart,classify in itertools.product([74,106,115],['none','wrong','busy','empty','waiting'],[0,1,256]):
        status_case(kind=kind,cart=cart,classify=classify,stock=0);count+=1
    for tag,stock,boat in itertools.product([0,0x1019,0x101a,0x2001],[-1,0,1],[0,1,256]):
        status_case(kind=74,tag=tag,stock=stock,boat=boat);count+=1
    print('PASS whole status publisher, all256 kinds, signed thresholds, sticky latch, carts and ship tags:',count,flush=True)


if __name__=='__main__':
    verify_desirability();verify_resources();verify_tiles();verify_predicate();verify_status()
    print('Verified target SHA256:',target_sha)
