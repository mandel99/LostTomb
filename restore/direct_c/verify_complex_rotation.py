"""Whole native/C complex preview and public building-rotation entry.

Projection and sprite endpoints are controlled for preview. Rotation controls
its genuine building/member descendants (including explicit ECX/RET4), while
all original selection, record iteration and image choices execute in C.
"""
from verify_options_startup import *


def complex_preview(rotation=0,kind=40,obstacle=0,group=0,cash=0,count=0,mutate=False,parent=False):
    outputs=[]
    for c,u,state in machines:
        reset(u,state)
        for a,v in [(0xbca304,411),(0xbca308,301),(0xbca300,30000),(0x6059e8,kind),
                (0x70e44c,271),(0x70e448,285),(0x70e458,569),(0x70e45c,283),
                (0xbca2e0,rotation),(0xbcb9b4,count),(0xeaac28,2),(0xea187c+2*0x49d8,cash),
                (0x70e120,0xabcd5555),(0xf5662c,100000),(0xf56be4,200000),
                (0x605994,0),(0xe38dcc,0)]:put(u,a,v)
        bases=[30000,30000+get(u,0x5cdf78+(rotation//2)*4)]
        sizes=[get(u,0x5d0b50),get(u,0x5d0b14)]
        for base,size in zip(bases,sizes):
            for i in range(get(u,0x5cfcd4+size*4)):
                tile=(base+get(u,0x5cfe10+(rotation//2)*144+i*4))&0xffffffff
                put(u,0xaf598c+tile*4,0);put(u,0xd05720+tile*4,0);word(u,0xb350dc+tile*2,0)
        tile=(bases[group]+get(u,0x5cfe10+(rotation//2)*144))&0xffffffff
        if obstacle==1:put(u,0xaf598c+tile*4,1)
        elif obstacle==2:put(u,0xd05720+tile*4,S+0x1000);byte(u,S+0x1018,0)
        elif obstacle==3:word(u,0xb350dc+tile*2,777)
        elif obstacle==4:put(u,0xd05720+tile*4,S+0x1000);byte(u,S+0x1018,1)
        calls=[]
        def observe(cpu,args,t):
            value=0;record=args
            if t==0x4f0ba0:put(cpu,args[2],600);put(cpu,args[3],450);record=args[:2]
            else:
                record=(*args,get(cpu,0x70e120))
                if mutate:
                    for a in [0x70e44c,0x70e448,0x70e458,0x70e45c]:put(cpu,a,get(cpu,a)+13)
                    put(cpu,0xbca2e0,6);put(cpu,0x6059e8,45)
                    put(cpu,0xf5662c,get(cpu,0xf5662c)+17);put(cpu,0xf56be4,get(cpu,0xf56be4)+19)
            calls.append((t,record))
            cpu.reg_write(UC_X86_REG_ECX,0xdeadbeef);cpu.reg_write(UC_X86_REG_EDX,0xbaadf00d)
            return value
        for t,n in [(0x4f0ba0,4),(0x53e590,3),(0x53f250,3),(0x53f4f0,4)]:
            hook(c,state,t,n,lambda cpu,args,t=t:observe(cpu,args,t),cleanup=(16 if t==0x53f4f0 else 12 if t in [0x53e590,0x53f250] else 0) if not c else 0)
        run(c,u,0x43b630 if parent else 0x43d720)
        outputs.append((calls,get(u,0x70e120)))
    assert outputs[0]==outputs[1],('complex preview',rotation,kind,obstacle,group,cash,count,mutate,parent,
        next(((i,a,b) for i,(a,b) in enumerate(zip(outputs[0][0],outputs[1][0])) if a!=b),None),outputs)


COUNTS={0x47bf50:7,0x47a1c0:1,0x47d1f0:4,0x47cf00:4,0x487ec0:1,
    0x475b50:1,0x440060:3,0x440540:3,0x4c0d50:2,0x541920:1,
    0x40dca0:1,0x4017a0:1,0x4e69c0:1,0x5468d0:1,0x4bfb30:1,0x4dfd90:1,0x4ee560:0}
MEMBERS={0x40dca0,0x4017a0,0x4e69c0,0x5468d0}


def rotation_refresh(kind,rotation,variant=0,building=3,root=True,water=False,active=1,mutate=False):
    outputs=[]
    for c,u,state in machines:
        reset(u,state)
        u.mem_write(0x7999d8,bytes(264*4001))
        record=building*264
        for a,v in [(0x78d790,building),(0xbca2e0,rotation),(0x799ab0+record,1),
                (0x799aa8+record,30000)]:put(u,a,v)
        byte(u,0x7999d8+record,active);byte(u,0x7999db+record,2)
        word(u,0x7999e8+record,kind);word(u,0x7999de+record,-300);word(u,0x7999e0+record,320)
        word(u,0x7999ea+record,variant);byte(u,0x799a82+record,variant)
        word(u,0x799a12+record,building)
        for i,a in enumerate([0xf56d54,0xf56d5c,0xf56d64,0xf56c0c,0xf56bcc,0xf56bc4,
                0xf56c1c,0xf56cc4,0xf56c14,0xf56bf4,0xf5724c,0xf5726c,0xf5725c,
                0xf57254,0xf5727c,0xf57274,0xf5730c,0xf572fc,0xf57304,
                0xf57374,0xf5737c,0xf57384,0xf56b3c]):put(u,a,100000+100*i)
        if 0<=kind<240:
            put(u,0x5d06e4+kind*20,517);put(u,0xf55bb4+517*8,90000)
        calls=[]
        def observe(cpu,t,args,receiver=0):
            if t in MEMBERS:assert receiver in [0x6fd5d8,0x6de650,0x6fd368,0x6de548,0x6f9320,0x6f5648,0x6de9e0,0x6e9188]
            calls.append((t,args,receiver if t in MEMBERS else 0))
            if mutate:
                put(cpu,0xbca2e0,6);put(cpu,0x78d790,building+1)
            return (0x12340001 if water else 0x12340000) if t==0x47a1c0 else (building if root else building-1) if t==0x475b50 else 0x12345678
        if not c:
            for t,n in COUNTS.items():
                def native(cpu,args,t=t):
                    receiver=cpu.reg_read(UC_X86_REG_ECX);value=observe(cpu,t,args,receiver)
                    cpu.reg_write(UC_X86_REG_ECX,0xdeadbeef);cpu.reg_write(UC_X86_REG_EDX,0xbaadf00d)
                    return value
                hook(c,state,t,n,native,cleanup=4 if t in MEMBERS else 0)
        else:
            def child(cpu,args):
                site,t,frame=args;regs=list(struct.unpack('<8I',bytes(cpu.mem_read(frame,32))))
                sp=regs[4];argv=tuple(get(cpu,sp+4+i*4) for i in range(COUNTS[t]))
                regs[0]=observe(cpu,t,argv,regs[1]);regs[1]=0xdeadbeef;regs[2]=0xbaadf00d
                regs[4]=sp+4+(4 if t in MEMBERS else 0);cpu.mem_write(frame,struct.pack('<8I',*regs));return 0
            state['hooks'][sym('restore_building_create_dispatch')]=(3,0,child)
            def raw_child(cpu,args,t):
                frame=args[0];sp=get(cpu,frame+16)
                argv=tuple(get(cpu,sp+4+i*4) for i in range(COUNTS[t]))
                value=observe(cpu,t,argv)
                put(cpu,frame,value);put(cpu,frame+16,sp+4);put(cpu,frame+4,0xdeadbeef);put(cpu,frame+8,0xbaadf00d);return 0
            for t in [0x4ee560,0x4dfd90]:
                state['hooks'][sym('restore_rotation_C_%08x'%t)]=(1,0,lambda cpu,args,t=t:raw_child(cpu,args,t))
        run(c,u,0x46d840,budget=10000000)
        outputs.append((calls,get(u,0xbca2e0),get(u,0x78d790)))
    assert outputs[0]==outputs[1],('public rotation',kind,rotation,variant,building,root,water,active,mutate,outputs)


if __name__=='__main__':
    count=0
    for rotation,kind in itertools.product([0,2,4,6],[40,44,45,7]):
        complex_preview(rotation,kind);count+=1
        complex_preview(rotation,kind,mutate=True);count+=1
    for rotation,obstacle,group in itertools.product([0,2,4,6],range(1,5),[0,1]):
        complex_preview(rotation,40,obstacle,group,mutate=True,parent=True);count+=1
    for cash,limit in itertools.product([-5001,-5000,-4999],[5,6,256]):
        complex_preview(2,45,cash=cash,count=limit);count+=1
    print('PASS whole complex preview, both groups, painter order, blocked cells and live callbacks:',count,flush=True)
    kinds=[-1,0,29,30,31,33,209,40,44,45,*range(140,150),41,42,43,58,200,170,201,171,202,56,
           210,74,76,181,182,90,75,192,136,*range(65,70),213,215,218,219,220,222,225,227,228,229,230,234,235,236,214]
    count=0
    for kind,rot in itertools.product(kinds,[0,2,4,6]):
        rotation_refresh(kind,rot,0x32);count+=1
    for kind,variant,water,root in itertools.product([30,31,33,209,41,42,43,74,76,181,182,75,136,210],
            [0,1,2,3,4,15],[False,True],[False,True]):
        rotation_refresh(kind,2,variant,root=root,water=water);count+=1
    for kind in [76,181,225,227,229,230,234,235,236]:
        rotation_refresh(kind,4,building=511,mutate=True);count+=1
        rotation_refresh(kind,4,building=4000,active=0);count+=1
    print('PASS complete public rotation path: all type branches, full building count, member receivers and callbacks:',count,flush=True)
    print('Verified target SHA256:',target_sha)
