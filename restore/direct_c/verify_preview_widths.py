"""Whole native/C preview siblings and terrain rectangle after linked-body triage.

Final draws and input/projection boundaries are controlled. Actual terrain,
object pointers, occupancy and statue rotation rules execute in both images.
"""
from verify_options_startup import *


def preview(token,variant,result=0,cash=123456,mutate=False,parent=False):
    outputs=[]
    for c,u,state in machines:
        reset(u,state)
        for a,v in [(0xbca304,411),(0xbca308,301),(0x6059e8,129),
                (0xeaac28,2),(0xea187c+2*0x49d8,cash),(0x70e44c,271),
                (0xbc96dc,variant),(0xbc96d4,variant),(0xf56be4,0x23456),
                (0x70e120,0xabcd5555),(0x5d06e4+129*20,517),
                (0x5d06e8+129*20,19),(0xf55bb4+517*8,100000),
                (0xbca300,1),(0xe38dcc,0),(0x605994,0),(0x605a3c,0)]:put(u,a,v)
        calls=[]
        def observe(cpu,args,t):
            value=0;record=args
            if t==0x4f0ba0:put(cpu,args[2],600);put(cpu,args[3],450);record=args[:2]
            elif t in [0x47d060,0x47d350]:
                value=result
                if t==0x47d350:record=()
                if mutate:put(cpu,0x70e44c,285)
            else:
                record=(*args,get(cpu,0x70e120))
                if mutate:put(cpu,0x70e44c,get(cpu,0x70e44c)+13);put(cpu,0xf56be4,get(cpu,0xf56be4)+1)
            calls.append((t,record))
            cpu.reg_write(UC_X86_REG_ECX,0xdeadbeef);cpu.reg_write(UC_X86_REG_EDX,0xbaadf00d)
            return value
        for t,n in [(0x4f0ba0,4),(0x47d060,3),(0x47d350,0 if c else 3),
                (0x53e590,3),(0x53f250,3),(0x53f4f0,4)]:
            hook(c,state,t,n,lambda cpu,args,t=t:observe(cpu,args,t),
                cleanup=(16 if t==0x53f4f0 else 12 if t in [0x53e590,0x53f250] else 0) if not c else 0)
        run(c,u,0x43b630 if parent else token,() if parent or token==0x43e090 else [517])
        outputs.append((calls,get(u,0x70e120)))
    assert outputs[0]==outputs[1],('preview sibling',hex(token),variant,result,cash,mutate,parent,outputs)


def rectangle(width,height,bits,hit,object_kind):
    outputs=[]
    for c,u,state in machines:
        reset(u,state)
        base=13282+20*228+20
        put(u,0xbca2d8,13282);put(u,0xbc9768,0x12345678)
        for row in range(6):
            for col in range(6):
                tile=base+row*228+col;put(u,0xaf598c+tile*4,0);put(u,0xd05720+tile*4,0)
        if hit>=0:
            row,col=divmod(hit,6);tile=base+row*228+col
            put(u,0xaf598c+tile*4,bits if not object_kind else 0)
            if object_kind:
                put(u,0xd05720+tile*4,S+0x1000);byte(u,S+0x1018,0 if object_kind==1 else 1)
        value=run(c,u,0x4893c0,[20,20,width,height,bits])
        outputs.append((value,get(u,0xbc9768)))
    assert outputs[0]==outputs[1],('terrain rectangle',width,height,hex(bits),hit,object_kind,outputs)


def statue(size,rotation,variant,obstacle=0,cash=0,key=0,mutate=False,parent=False):
    outputs=[]
    for c,u,state in machines:
        reset(u,state)
        base=30000;kind=40+size;count=size*size
        for a,v in [(0xbca304,411),(0xbca308,301),(0xbca300,base),(0x6059e8,kind),
                (0xbca2d0,640),(0xbca2d4,480),(0x605a3c,0),(0xbc9714,variant),
                (0xbca2e0,rotation),(0xeaac28,2),(0xea187c+2*0x49d8,cash),
                (0x5d06dc+kind*20,size),(0x5d06e4+kind*20,517),
                (0xf55bb4+517*8,100000),(0xf5730c,200000),
                (0xf57304,300000),(0xf572fc,400000),(0xf56be4,500000),
                (0x70e44c,271),(0x70e120,0xabcd5555),(0xe38dcc,0),(0x605994,0)]:put(u,a,v)
        for i in range(count):
            delta=get(u,0x5cfe10+(rotation//2)*144+i*4);tile=(base+delta)&0xffffffff
            put(u,0xaf598c+tile*4,0);put(u,0xd05720+tile*4,0);word(u,0xb350dc+tile*2,0)
        delta=get(u,0x5cfe10+(rotation//2)*144);tile=(base+delta)&0xffffffff
        if obstacle==1:put(u,0xaf598c+tile*4,0x10000)
        elif obstacle==2:put(u,0xaf598c+tile*4,1)
        elif obstacle==3:word(u,0xb350dc+tile*2,777)
        elif obstacle==4:put(u,0xd05720+tile*4,S+0x1000);byte(u,S+0x1018,0)
        elif obstacle==5:put(u,0xbca304,2)
        calls=[]
        def observe(cpu,args,t):
            value=0;record=args
            if t==0x4f0ba0:put(cpu,args[2],600);put(cpu,args[3],450);record=args[:2]
            elif t==0:key_result=key;value=key_result
            else:
                record=(*args,get(cpu,0x70e120))
                if mutate:
                    put(cpu,0x70e44c,get(cpu,0x70e44c)+13)
                    put(cpu,0xbca2e0,6);put(cpu,0xf56be4,get(cpu,0xf56be4)+1)
                    put(cpu,0xaf598c+tile*4,1)
            calls.append((t,record))
            cpu.reg_write(UC_X86_REG_ECX,0xdeadbeef);cpu.reg_write(UC_X86_REG_EDX,0xbaadf00d)
            return value
        for t,n in [(0x4f0ba0,4),(0x53e590,3),(0x53f250,3),(0x53f4f0,4)]:
            hook(c,state,t,n,lambda cpu,args,t=t:observe(cpu,args,t),cleanup=(16 if t==0x53f4f0 else 12 if t in [0x53e590,0x53f250] else 0) if not c else 0)
        slot=sym('_imp__GetAsyncKeyState@4') if c else 0x56f1e4
        put(u,slot,S+0x49000);state['hooks'][S+0x49000]=(1,4,lambda cpu,args:observe(cpu,args,0))
        run(c,u,0x43b630 if parent else 0x441f80)
        outputs.append((calls,get(u,0x70e120),get(u,0xbc9714)))
    assert outputs[0]==outputs[1],('statue',size,rotation,variant,obstacle,cash,key,mutate,parent,outputs)


if __name__=='__main__':
    count=0
    for token,variant,result,cash in itertools.product([0x43df10,0x43e090],[0,1,2,3,256,-1],[0,1,999],[-5000,-4999]):
        preview(token,variant,result,cash,True);count+=1
    preview(0x43e090,256,parent=True);count+=1
    print('PASS whole3x3 shoreline/single previews and single-mode dispatcher:',count,flush=True)
    count=0
    for width,height,bits,hit,obj in itertools.product([0,1,2,6,-1],[0,1,2,6,-1],[0x10000,0x80],[-1,0,7,35],[0,1,2]):
        rectangle(width,height,bits,hit,obj);count+=1
    print('PASS whole terrain rectangle: live object pointers, full tile indices and retained highEAX:',count,flush=True)
    count=0
    for size,rotation,variant in itertools.product([1,2,3],[0,2,4,6],range(16)):
        statue(size,rotation,variant);count+=1
    for size,rotation,obstacle,cash in itertools.product([1,2,3],[0,2,4,6],range(1,6),[0,-5000]):
        statue(size,rotation,15,obstacle,cash,0x8001,True,True);count+=1
    for variant in [-5,-1,16,256]:statue(3,2,variant);count+=1
    print('PASS whole statue ghost and actual dispatcher: all variants, rotations, obstacles, input and live callbacks:',count,flush=True)
    print('Verified target SHA256:',target_sha)
