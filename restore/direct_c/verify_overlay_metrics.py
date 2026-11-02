"""Canonical metric/column providers against the pinned original.

Complete algorithms and query dependencies execute; only final sprite calls
are observed. No inherited food frames or pixel-output certification.
"""
from verify_terrain_overlays import setup
from verify_options_startup import *

TWO=[0x432320,0x432820,0x435050]
THREE=[0x435290,0x4353a0,0x4354b0,0x435510,0x435570,0x4355d0,
       0x435630,0x4356a0,0x435700,0x435760,0x4357c0,0x435820,
       0x435880,0x435a10,0x435a70,0x435ac0,0x435b20,0x435b80,
       0x435be0,0x435c40,0x435ca0,0x435d00,0x435d60,0x435de0,
       0x4360a0,0x436140]


def prepare(u,state,kind=115,construct=1,field=100,footprint=2,flags=8,
            limit=45000,height=24,orientation=0,variant=0,built=0,fertility=0,
            workflag=None,edge=64):
    setup(u,state,kind=kind,construct=construct,footprint=footprint,flags=flags,
          orientation=orientation)
    offset=511*264
    for at in range(0x7999f0,0x799ab0,2):word(u,at+offset,field)
    word(u,0x7999ea+offset,1);byte(u,0x799a82+offset,variant)
    word(u,0x799a10+offset,built);byte(u,0x7999db+offset,3)
    byte(u,0xa5d4cc+18000,fertility);byte(u,0xb67d1c+18000,edge)
    if workflag is not None:byte(u,0x799a5f+offset,workflag)
    for at in [0x799abc,0x799ac4,0x799ad4]:put(u,at+offset,field)
    for at,value in [(0xf56754,19000),(0xf56a34,19300),(0xf56d14,19400),
                     (0xf56d1c,19500),(0x126517c,limit)]:put(u,at,value)
    for image in [0,*range(19000,19016)]:word(u,0x10d6a0e+image*64,height)


def sprites(c,state,calls):
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


def compare(token,args,**kw):
    results=[]
    for c,u,state in machines:
        prepare(u,state,**kw);calls=[];sprites(c,state,calls)
        run(c,u,token,args);results.append((calls,snapshot(u)))
    assert results[0]==results[1],(hex(token),args,kw,results[0][0][:12],results[1][0][:12],
        [(hex(0x560000+i),a,b) for i,(a,b) in enumerate(zip(results[0][-1],results[1][-1])) if a!=b][:12])
    return results[1][0]


def verify_columns():
    count=0
    for token,level,palette,limit in itertools.product([0x436180,0x4361a0],
            [-32768,-1,0,1,4,5,6,7,8,9,10,11,2147483647],[0,1,2,256],[128,45000]):
        compare(token,[level,80,100,palette],limit=limit,height=-24 if palette==256 else 24);count+=1
    print('PASS both complete column entries, signed levels, palettes, full IDs/limits and signed heights:',count,flush=True)


def verify_metrics():
    count=0
    for token,construct,field in itertools.product([t for t in THREE if t!=0x435de0],
                                                  [0,1],[0,9,100,700,-1,32767]):
        compare(token,[511,80,100],construct=construct,field=field);count+=1
    calls=compare(0x435c40,[511,80,100],field=700)
    assert len(calls)==11 and calls[0]==(0x53e590,(19002,89,92))
    print('PASS25 metric/icon entries, construction gates and actual signed BYTE/WORD/DWORD reads:',count,flush=True)


def verify_ground():
    count=0
    for token,flags,footprint in itertools.product(TWO,[0,1,8,0x80,0x2080,0x4000,0x20000002],range(7)):
        compare(token,[80,100],flags=flags,footprint=footprint);count+=1
    print('PASS ground/top metric entries, terrain masks and full footprint tables:',count,flush=True)


def verify_special():
    count=0
    for token,variant,direction in itertools.product([0x435290,0x4353a0],
                                                     [0,2,4,6],[0,2,4,6]):
        compare(token,[511,80,100],kind=65,field=801,variant=variant,orientation=direction);count+=1
    print('PASS fire/damage pyramid anchors with genuine projection and orientation:',count,flush=True)
    count=0
    for value in [-2147483648,-1,0,1,2,3,4,8,12,199,200,201,2147483647]:
        compare(0x435de0,[value,0x7ffffff0,0x80000010]);count+=1
    for flag,construct in itertools.product([0,1,2,3,255],[0,1]):
        compare(0x4360a0,[511,80,100],construct=construct,workflag=flag);count+=1
    for fertility in [0,1,9,10,19,20,29,30,49,50,69,70,79,80,89,90,255]:
        compare(0x435050,[80,100],fertility=fertility);count+=1
    for token,built,edge in itertools.product([0x435290,0x4353a0],[0,1],[0x40,0x42,0x50,0x52]):
        compare(token,[511,80,100],kind=82,field=801,built=built,edge=edge);count+=1
    print('PASS tax/icon branches, construction flags, fertility thresholds and built/edge gates:',count,flush=True)
    print('Verified target SHA256:',target_sha)


def verify_fertility():
    count=0
    for fertility in range(256):
        calls=compare(0x434e10,[80,100],fertility=fertility)
        if fertility:assert calls==[(0x53e590,(23000+min(fertility//10+1,9),80,100))]
        count+=1
    for flags,footprint,edge in itertools.product([0,1,8,0x80,0x2080,0x4000,0x20000002],
                                                  range(8),[0,64]):
        compare(0x434e10,[0x7ffffff0,0x80000010],flags=flags,footprint=footprint,edge=edge);count+=1
    print('PASS lower fertility: all256 levels, mask/edge/footprint gates and wrapped coordinates:',count,flush=True)
    print('Verified target SHA256:',target_sha)


def verify_fertility_compositions():
    from verify_world_detail import prepare as world_prepare,install
    count=0
    for (width,height),fertility,footprint in itertools.product(
            [(640,480),(800,600),(1024,768)],[0,80],[0,2,5]):
        results=[]
        for c,u,state in machines:
            world_prepare(u,state,28,115,footprint,width=width,height=height)
            byte(u,0xa5d4cc+18000,fertility);put(u,0xf56ca4,23000)
            calls=[];install(c,u,state,calls,True)
            run(c,u,0x4316f0);run(c,u,0x439f10)
            results.append((calls,snapshot(u)))
        assert results[0]==results[1],('fertility compositions',width,fertility,footprint,
                                      results[0][0][:12],results[1][0][:12])
        if fertility:
            assert any(token==0x53e590 and args[0]==23009 for token,args in calls)
            assert any(token==0x53f250 and args[0]==23009 for token,args in calls)
        count+=1
    print('PASS full ground/detail fertility compositions at three sizes and footprints:',count,flush=True)
    print('Verified target SHA256:',target_sha)


if __name__=='__main__':
    verify_columns();verify_metrics();verify_ground();verify_special()
    verify_fertility();verify_fertility_compositions()
