"""Whole native/C monument-worker reset, travel setup and movement.

The real direction classifier and extended sqrt/truncation execute. Actor
grid unlink/link are controlled, including mutations and their ordering.
This checks memory, ABI and return AL; it does not run the host game.
"""
from verify_options_startup import *


def prepare(u,state,cw=0x37f):
    reset(u,state);u.mem_write(SP-0x5000,b'\xa5'*0x5000)
    for reg,value in [(UC_X86_REG_FPCW,cw),(UC_X86_REG_FPSW,0),(UC_X86_REG_FPTAG,0xffff)]:
        u.reg_write(reg,value)


def compare(values,label):
    assert values[0][0]==values[1][0],(label,'calls/AL',values[0][0],values[1][0])
    equal(values,label)


def setup(u,mode=1,kind=2,shape=2,rotation=0,actor=301):
    a=0xbcd5f8+actor*388
    u.mem_write(a,b'\xa5'*388)
    for i,(x,y) in enumerate([(90,80),(94,86),(88,84),(98,88),(92,82)]):
        r=0x7999d8+(400+i)*264
        u.mem_write(r,b'\xa5'*264)
        word(u,r+6,x);word(u,r+8,y);put(u,r+12,1000+y*228+x)
        word(u,r+18,i);byte(u,r+84,kind)
    for offset,value in [(0x14,70),(0x16,75),(0x24,90),(0x26,80),
                         (0x4a,400),(0x4c,257),(0x4e,401),(0x50,258),
                         (0x52,0),(0x54,30),(0x120,2)]:word(u,a+offset,value)
    for offset,value in [(0x20,19000),(0x28,19330)]:put(u,a+offset,value)
    for address,value in [(0x5e4ef4,mode),(0xbca2e0,rotation),(0xbca2d8,1000),(0xd056f8,302)]:put(u,address,value)
    byte(u,0xbcd609+302*388,5)
    table=0x5e6578 if mode==5 else 0x5e4ef8;stride=36 if mode==5 else 144
    for i in range(5):put(u,table+(kind*stride+i)*8,shape)
    return a


def observe_grid(c,u,state,a,calls,mutate=False):
    def grid(cpu,args,t):
        calls.append((t,args,bytes(cpu.mem_read(a+0x14,16))))
        if mutate:
            word(cpu,a+0x52,321);word(cpu,a+0x54,1)
            if t==0x4b5420:
                word(cpu,a+0x14,-20);word(cpu,a+0x16,-30);put(cpu,0xbca2d8,4321)
        return 0xabcdef01
    for t in [0x4b5420,0x4b52f0]:hook(c,state,t,1,lambda cpu,args,t=t:grid(cpu,args,t))


def begin(mode=1,shape=2,rotation=0,select=1,xy=(70,75),target=(94,86),cw=0x37f,mutate=False):
    values=[]
    for c,u,state in machines:
        prepare(u,state,cw);a=setup(u,mode,2,shape,rotation)
        word(u,a+0x14,xy[0]);word(u,a+0x16,xy[1])
        word(u,0x7999de+401*264,target[0]);word(u,0x7999e0+401*264,target[1])
        calls=[]
        if mutate:
            def direction(cpu,args):
                calls.append(('direction',args));put(cpu,0x5e4ef4,5);put(cpu,0xbca2e0,6)
                put(cpu,0x5e6578+(2*36+1)*8,4)
                word(cpu,a+0x4a,403);return 0x12345607
            hook(c,state,0x51cd80,4,direction)
        run(c,u,0x4f02f0,[301,401,0x1234abcd,select])
        assert u.reg_read(UC_X86_REG_FPCW)==cw
        assert (u.reg_read(UC_X86_REG_FPSW)>>11)&7==0
        values.append((calls,snapshot(u)))
    compare(values,('begin',mode,shape,rotation,select,xy,target,cw,mutate))


def advance(mode=1,shape=2,rotation=0,action=2,progress=0,length=30,amount=1,
            facing=5,neighbor=-1,from_id=400,same=False,mutate='',kind=2):
    values=[]
    for c,u,state in machines:
        prepare(u,state);a=setup(u,mode,kind,shape,rotation)
        word(u,a+0x52,progress);word(u,a+0x54,length);word(u,a+0x4a,from_id)
        word(u,a+0x120,action);byte(u,0xbcd609+302*388,facing)
        # Filled map ownership deliberately belongs to another building,
        # outside this monument, except one selectable neighboring tile.
        u.mem_write(0xb4e6fc,struct.pack('<H',404)*51984)
        put(u,(0x56f7ec if mode==5 else 0x56f7d8)+kind*4,4)
        r=0x7999d8+400*264
        if neighbor>=0:
            index=rotation//2 if action==4 else ((rotation//2-2)&3)
            base=get(u,0x7999e4+404*264) if action==4 else get(u,r+12)
            offset=get(u,0x5700f4+index*12+neighbor*4)
            word(u,0xb4e6fc+((base+offset)&0xffffffff)*2,402)
        if same:
            # Source for a straight non-probing shape; cell still gets written.
            word(u,a+0x14,90);word(u,a+0x16,80)
        calls=[];observe_grid(c,u,state,a,calls,mutate=='grid')
        if mutate=='direction':
            def direction(cpu,args):
                calls.append(('direction',args));put(cpu,0x5e4ef4,5);put(cpu,0xbca2e0,6)
                word(cpu,a+0x52,22);word(cpu,a+0x54,40)
                put(cpu,0x5e6578+kind*36*8,4);word(cpu,a+0x4e,403)
                return 0x12345607
            hook(c,state,0x51cd80,4,direction)
        value=run(c,u,0x4f1d10,[301,amount])&255
        values.append(((value,calls),snapshot(u)))
    compare(values,('advance',mode,shape,rotation,action,progress,length,amount,facing,neighbor,from_id,same,mutate,kind))


def reset_motion(owner):
    values=[]
    for c,u,state in machines:
        prepare(u,state);a=setup(u);word(u,0xb4e6fc+19330*2,owner)
        run(c,u,0x4f1cb0,[301]);values.append(([],snapshot(u)))
    compare(values,('reset',owner))


def composition(mode,shape,rotation):
    values=[]
    for c,u,state in machines:
        prepare(u,state);a=setup(u,mode,1,shape,rotation)
        word(u,0xb4e6fc+19330*2,400)
        run(c,u,0x4f1cb0,[301]);run(c,u,0x4f02f0,[301,401,11,1])
        calls=[];observe_grid(c,u,state,a,calls)
        # No reset between steps: all three reconstructed public functions
        # and the actual direction helper execute on the evolving state.
        for amount in [1,13,29,127]:
            calls.append(('AL',run(c,u,0x4f1d10,[301,amount])&255))
        values.append((calls,snapshot(u)))
    compare(values,('composition',mode,shape,rotation))


def next_piece(piece):
    values=[]
    for c,u,state in machines:
        prepare(u,state);setup(u);word(u,0x7999ea+600*264,piece)
        value=run(c,u,0x4f2190,[600]);values.append((value,snapshot(u)))
    compare(values,('next piece',piece))


def collect(mode,count,variant):
    values=[]
    for c,u,state in machines:
        prepare(u,state);setup(u,mode)
        put(u,(0x56f7ec if mode==5 else 0x56f7d8)+2*4,count)
        for i in range(max(1,count)):
            r=0x7999d8+(400+i)*264;word(u,r+18,i);byte(u,r+84,2)
            # Limit tied output to the actual list capacity. A count above255
            # still scans every piece and distinguishes DWORD/BYTE counters.
            if i>=16:phase,column,row=1,0,7
            elif variant==0:phase,column,row=0,1,2
            elif variant==1:phase,column,row=1,1+i%3,(5-i)%4
            elif variant==2:phase,column,row=i%3,(i*7)%4,(i*11)%5
            else:phase,column,row=0,(15-i)%4,(15-i)%5
            byte(u,r+85,phase);byte(u,r+86,column);byte(u,r+87,row)
        run(c,u,0x4ef450,[400]);values.append(([],snapshot(u)))
    compare(values,('collect',mode,count,variant))


def snap(global_actor=302,mutate=False):
    values=[]
    for c,u,state in machines:
        prepare(u,state);a=setup(u)
        current=0xbcd5f8+global_actor*388;put(u,0xd056f8,global_actor)
        word(u,current+0x24,-127);word(u,current+0x26,310);put(u,current+0x28,123456)
        byte(u,0x799a2e+401*264,255);byte(u,0x799a2f+401*264,255)
        calls=[]
        def grid(cpu,args,t):
            calls.append((t,args,bytes(cpu.mem_read(a+0x14,16))))
            if mutate:
                if t==0x4b5420:
                    put(cpu,0xd056f8,301);word(cpu,a+0x24,123);word(cpu,a+0x26,234)
                else:word(cpu,a+0x4e,403);byte(cpu,0x799a2e+403*264,11);byte(cpu,0x799a2f+403*264,12)
            return 0
        for t in [0x4b5420,0x4b52f0]:hook(c,state,t,1,lambda cpu,args,t=t:grid(cpu,args,t))
        run(c,u,0x4f2410,[301]);values.append((calls,snapshot(u)))
    compare(values,('snap',global_actor,mutate))


def find_work(mode=1,rotation=0,owners=(0,0,0,0),preferred=(0,1,2,3),
              length=0,action=2,mutate=False):
    values=[]
    for c,u,state in machines:
        prepare(u,state);a=setup(u,mode,0,2,rotation);put(u,0xd056f8,301)
        word(u,a+0x54,length);word(u,a+0x120,action)
        put(u,(0x56f7ec if mode==5 else 0x56f7d8),4)
        for i,owner in enumerate(owners):
            r=0x7999d8+(400+i)*264;word(u,r+100,owner)
            byte(u,r+84,0);byte(u,r+85,0);byte(u,r+86,1 if i in preferred else 2);byte(u,r+87,1)
        for owner,kind,state_value in [(500,81,11),(501,80,12),(502,81,12),(503,80,11),(504,0,11)]:
            r=0xbcd5f8+owner*388;byte(u,r+10,kind);byte(u,r+0x38,state_value)
        calls=[];observe_grid(c,u,state,a,calls,mutate)
        # All seven reconstructed bodies execute. Only grid maintenance is
        # controlled; the actual work list, ring walk and begin/advance run.
        run(c,u,0x4f2230,[301],budget=5000000)
        values.append((calls,snapshot(u)))
    compare(values,('find work',mode,rotation,owners,preferred,length,action,mutate))


if __name__=='__main__':
    n=0
    for owner in [0,1,400,0x7fff,0x8000,0xffff]:reset_motion(owner);n+=1
    print('PASS reset: %d native/C exact WORD stores'%n,flush=True)
    n=0
    for mode,shape,rotation in itertools.product([1,5,257],range(10),[0,2,4,6]):
        begin(mode,shape,rotation);n+=1
    for cw,xy,target in itertools.product([0x37f,0x77f,0xb7f,0xf7f],
            [(90,80),(94,86),(-300,310),(32767,32767)],[(94,86),(-32768,-32768)]):
        begin(select=0,xy=xy,target=target,cw=cw);n+=1
    begin(mutate=True);n+=1
    print('PASS travel setup: %d native/C length, shape, rotation, x87 and callback cases'%n,flush=True)
    n=0
    for mode,shape,rotation in itertools.product([1,5,257],range(10),[0,2,4,6]):
        advance(mode,shape,rotation);n+=1
    for rotation,neighbor in itertools.product([0,2,4,6],[-1,0,1,2]):
        advance(action=4,rotation=rotation,neighbor=neighbor);n+=1
        advance(shape=7-rotation//2,rotation=rotation,neighbor=neighbor);n+=1
    for progress,length,amount in [(0,0,1),(0,-1,1),(0,-32768,1),(0,30,0),
            (29,30,1),(29,30,100),(32760,32767,20),(2,30,-5),(0,1,0x10001)]:
        advance(action=4,progress=progress,length=length,amount=amount);n+=1
    for facing in [0,1,2,3,4,5,6,7,8,127,128,255]:
        advance(shape=5,facing=facing);n+=1
    for kind in [0,1,2,3,4]:advance(kind=kind);n+=1
    for mutate in ['', 'grid','direction']:
        advance(mutate=mutate);n+=1
    advance(from_id=0);advance(same=True);n+=2
    print('PASS movement: %d native/C interpolation, neighbor, completion, signedness and reload cases'%n,flush=True)
    n=0
    for mode,shape in itertools.product([1,5,257],[1,3,4,5,8,9]):
        composition(mode,shape,0);n+=1
    print('PASS whole reset/setup/movement composition: %d native/C cases'%n,flush=True)
    n=0
    for piece in list(range(145))+[-32768,-1,32767]:next_piece(piece);n+=1
    print('PASS next work piece: %d native/C ring positions and signed boundaries'%n,flush=True)
    n=0
    for mode,count,variant in itertools.product([1,3,5,257],[-1,0,1,4,36,257],range(4)):
        collect(mode,count,variant);n+=1
    print('PASS work list: %d native/C phase ranking, ties and DWORD counts'%n,flush=True)
    n=0
    for actor,mutate in itertools.product([301,302,600],[False,True]):snap(actor,mutate);n+=1
    print('PASS target snap: %d native/C current actor, field widths and callback reload cases'%n,flush=True)
    n=0
    for mode,rotation in itertools.product([1,5,257],[0,2,4,6]):
        for owners,preferred in [((0,0,0,0),(0,1,2,3)),((0,500,0,0),(0,1,2,3)),
                ((500,501,500,501),(0,1,2,3)),((500,501,502,503),(2,)),
                ((500,501,504,500),(3,)),((-1,0,0,0),(0,))]:
            find_work(mode,rotation,owners,preferred);n+=1
    for length,action in [(30,2),(0,0),(0,-1),(0,256)]:find_work(length=length,action=action);n+=1
    find_work(mutate=True);n+=1
    print('PASS actual work selection composition: %d native/C reservation, wrap and delayed argument cases'%n,flush=True)
    print('target SHA256',target_sha)
