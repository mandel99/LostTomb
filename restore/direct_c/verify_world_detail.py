"""Whole439F10 and its reviewed metric/image compositions, offline.

Isolated producer cases control its outgoing endpoints; composed cases execute
the complete metric/image/stock models and real queries. Final sprites and
still-open status/food/actor-render boundaries are observed/controlled.
No invented inherited food frame or full pixel/live-game certification.
"""
from verify_options_startup import *

ARITY={0x438e50:4,0x53e590:3,0x4339c0:2,0x53f250:3,0x435050:2,
       0x434aa0:4,0x432320:2,0x51f780:0,0x431690:1,0x432820:2,
       0x40f2c0:2,0x488050:1,0x489500:4,0x438180:1,0x4381a0:1,
       0x4381b0:5,0x442280:4,0x436310:3,0x435e70:3,0x432d30:4,0x432920:1}
for token in [0x435290,0x4353a0,0x435570,0x4354b0,0x435510,0x4355d0,
              0x435630,0x4356a0,0x435700,0x435760,0x4357c0,0x435820,
              0x435880,0x435a70,0x435ac0,0x435b20,0x435b80,0x435a10,
              0x435c40,0x435be0,0x435ca0,0x435d60,0x436140,0x4360a0,0x435d00]:ARITY[token]=3
MEMBER={0x53e590:12,0x53f250:12,0x438180:4,0x4381a0:4,0x488050:4,0x489500:16}


for c,u,state in machines:
    state['detail_controls']={}
    if c:
        def boundary(uc,pc,size,state):
            host_sp=uc.reg_read(UC_X86_REG_ESP);token=get(uc,host_sp+8)
            fn=state['detail_controls'].get(token)
            if fn is None:return
            f=get(uc,host_sp+12);sp=get(uc,f+16)
            args=tuple(get(uc,sp+4+i*4) for i in range(ARITY[token]))
            value=fn(uc,args)
            put(uc,f,value);put(uc,f+4,0xdeadbeef);put(uc,f+8,0xbaadf00d);put(uc,f+32,2)
            put(uc,f+16,sp+4+MEMBER.get(token,0))
            uc.reg_write(UC_X86_REG_ESP,host_sp+4);uc.reg_write(UC_X86_REG_EIP,get(uc,host_sp))
        entry=sym('restore_world_detail_dispatch')
        u.hook_add(UC_HOOK_CODE,boundary,state,begin=entry,end=entry)


def record(image,limit=45000):return 0x10d69f8+(image if image<limit else 0)*64


def prepare(u,state,view,kind,footprint,building=511,image=20000,limit=45000,
            width=800,height=600,origin=(8,4),size=(0,0),actor_mode=0,variant=0):
    reset(u,state);state['detail_controls'].clear()
    for reg,value in [(UC_X86_REG_FPCW,0x37f),(UC_X86_REG_FPSW,0),(UC_X86_REG_FPTAG,0xffff)]:u.reg_write(reg,value)
    u.mem_write(0x70e494,struct.pack('<I',0xfff0000)*(459*231))
    u.mem_write(0xb350dc,bytes(228*228*2));u.mem_write(0xbcd5f8,bytes(2000*388))
    u.mem_write(0x7999d8,bytes(4000*264))
    for at,value in [(0x70e474,origin[0]),(0x70e478,origin[1]),(0x70e460,size[0]),
                     (0x70e464,size[1]),(0x70e48c,400),(0x70e490,width//2),
                     (0x70e44c,15),(0x70e448,30),(0x70e444,45),(0x70e440,60),
                     (0x70e45c,29),(0x70e458,58),(0x70e454,87),(0x70e450,116),
                     (0xe94e0c,view),(0x126517c,limit),(0xe38dac,width),(0xe38db0,height),
                     (0xd3b550,0),(0xd3b554,0),(0xd3b558,width),(0xd3b55c,height),
                     (0xf572ec,40000),(0xf56d34,21000),(0xf566ac,22000),
                     (0xf566fc,20000),(0xf56c0c,image-variant),(0xf56bcc,image-variant)]:put(u,at,value)
    for group in [622,623,624,628,629,631,632,546,547,578,579,580,581]:put(u,0xf55bb4+group*8,25000+group*4)
    for at,value in [(0xf56f3c,27000),(0xf56f44,27100),(0xf56f4c,27200),(0xf56df4,27300),
                     (0xf56dec,27400),(0xf56e0c,27500),(0xf56f54,27600),(0xf56f34,27700),
                     (0xf56cc4,27800),(0xf56ccc,image-variant),(0xf56a84,13198)]:put(u,at,value)
    tile=18000
    row,col=origin[0]-8,origin[1]-4
    if 0<=row<459 and 0<=col<231:put(u,0x70e494+(row*231+col)*4,tile)
    put(u,0xaf598c+tile*4,8);put(u,0xb7482c+tile*4,image)
    word(u,0xb4e6fc+tile*2,building);byte(u,0xb67d1c+tile,64)
    byte(u,0xb285cc+tile,footprint)
    word(u,0x7999e8+building*264,kind);byte(u,0x7999d8+building*264,1)
    word(u,0x799a22+building*264,1);word(u,0x799a14+building*264,300)
    word(u,0x799a18+building*264,1)
    for img in [0,image]:
        word(u,record(img,limit)+30,4);word(u,record(img,limit)+34,17)
        word(u,record(img,limit)+36,-9);word(u,record(img,limit)+22,40)
    if actor_mode:
        word(u,0xb350dc+tile*2,1);word(u,0xbcd600+388,1999)
        byte(u,0xbcd65d+1999*388,actor_mode==2)
        word(u,0xbcd718+1999*388,1 if actor_mode==3 else 0)


def install(c,u,state,calls,composed):
    controlled={0x432920,0x435e70,0x442280,0x438e50}
    if not composed:controlled=set(ARITY)-{0x432d30,0x432920}
    for token in controlled:
        def observe(uc,args,token=token):
            calls.append((token,args))
            if token==0x431690:return 1
            if token==0x40f2c0:return 3
            if token==0x438180:return record(args[0],get(uc,0x126517c))
            if token==0x4381a0:return get(uc,0xf55bb4+args[0]*8)
            return 0x12345678 if token in [0x53e590,0x53f250] else 0
        if c:
            state['detail_controls'][token]=observe
            if re.search(r'\s_FUN_%08x\s'%token,symbols):hook(c,state,token,ARITY[token],observe)
        else:
            def native(uc,args,fn=observe):
                value=fn(uc,args);uc.reg_write(UC_X86_REG_ECX,0xdeadbeef)
                uc.reg_write(UC_X86_REG_EDX,0xbaadf00d);uc.reg_write(UC_X86_REG_EFLAGS,2)
                return value
            hook(c,state,token,ARITY[token],native,cleanup=MEMBER.get(token,0))
    if composed:
        for token,name in [(0x53e590,'restore_sprite_draw'),(0x53f250,'restore_sprite_draw_upper')]:
            if c:
                def draw(uc,args,token=token):
                    assert args[0]==0xf55bb0
                    calls.append((token,args[1:]));return 0x12345678
                state['hooks'][sym(name)]=(4,0,draw)
            else:
                def draw(uc,args,token=token):
                    assert uc.reg_read(UC_X86_REG_ECX)==0xf55bb0
                    calls.append((token,args));return 0x12345678
                hook(c,state,token,3,draw,cleanup=12)


def case(view=1,kind=115,footprint=0,composed=False,**kwargs):
    results=[]
    for c,u,state in machines:
        prepare(u,state,view,kind,footprint,**kwargs);calls=[];install(c,u,state,calls,composed)
        run(c,u,0x439f10)
        results.append((calls,snapshot(u)))
    assert results[0]==results[1],(view,kind,footprint,composed,kwargs,
        results[0][0][:12],results[1][0][:12],
        [(hex(0x560000+i),a,b) for i,(a,b) in enumerate(zip(results[0][-1],results[1][-1])) if a!=b][:12])


def verify():
    count=0
    for view,footprint in itertools.product(range(1,52),[0,1,2,3,4,5,6]):
        case(view=view,footprint=footprint);count+=1
    for kind,variant in itertools.product([30,31,33,34,35,74,75,76,90,106,161,229,230,234],range(4)):
        case(view=8,kind=kind,variant=variant,image=20000+variant);count+=1
    for width,height in [(640,480),(800,600),(1024,768)]:
        for actor_mode in range(4):case(view=27,width=width,height=height,actor_mode=actor_mode);count+=1
    for origin,size in itertools.product([(8,4),(7,3),(466,234),(467,235)],[(0,0),(-18,0),(-17,-7),(14,8)]):
        case(view=27,origin=origin,size=size);count+=1
    print('PASS complete detail producer,51 views, footprints, image kinds, actors and viewport clipping:',count,flush=True)


def verify_compositions():
    count=0
    for view in range(1,52):
        at=struct.unpack_from('<I',seed,0x5d25ac+view*4-0x560000)[0]
        kind=struct.unpack_from('<H',seed,at-0x560000)[0] if at else 115
        case(view=view,kind=kind,composed=True);count+=1
    for kind,footprint in itertools.product([30,31,33,34,35,74,75,76,90,106,161,115],[0,1,2,3,4,5]):
        case(view=8,kind=kind,footprint=footprint,composed=True);count+=1
    print('PASS real terrain/resource/metric/image/stock compositions with declared open status/food boundaries:',count,flush=True)
    print('Verified target SHA256:',target_sha)


def verify_image_limits():
    count=0
    for limit,composed,kind in itertools.product([0,20000,45000],[False,True],[34,115]):
        case(view=8,kind=kind,limit=limit,composed=composed);count+=1
    print('PASS full DWORD image limits, exact-boundary and record0 metadata fallback:',count,flush=True)


if __name__=='__main__':verify();verify_compositions();verify_image_limits()
