"""Native/C public unit-entry regressions, using the shared production providers.
Most commands compose real rank/search/link/return/route helpers. Controlled
boundaries are map-neighbor path queries, formation notifications and selection.
No host game runs.
"""
from verify_options_startup import *

def actor(i):return 0xbcd5f8+i*388
def building(i):return 0x7999d8+i*264
def group(i):return 0xbcb9c8+i*144

def prepare(u,state):
    reset(u,state);u.mem_write(SP-0x5000,b'\xa5'*0x5000)
    u.mem_write(0x7999d8,bytes(4000*264))
    for i in [301,302,303,310]:
        a=actor(i);u.mem_write(a,b'\xa5'*388)
        byte(u,a+10,77);byte(u,a+56,8);word(u,a+20,257);word(u,a+22,-513)
        put(u,a+296,75);put(u,a+300,100);byte(u,a+304,0);word(u,a+306,-1)
        word(u,a+90,400);word(u,a+94,333);word(u,a+58,i-280);word(u,0xd04e58+(i-280)*2,i)
    for a,v in [(0xeadcc8,301),(0xeadcc4,301),(0xd056f8,302),(0xd05700,0xa5a51234),
                (0xd05704,0xa5a51234),(0xd05708,0xa5a51234),(0xd0570c,0xa5a51234),
                (0xd3b198,0),(0xbca2d8,1000),(0xd3b4e0,0x1234abcd),(0xd3b4e4,0x2345ef01),
                (0xeadccc,3),(0xbcb9c0,4),(0xd3b194,0)]:
        put(u,a,v)
    byte(u,0xe38e60,0);byte(u,0xe38dda,0xa5);byte(u,0xe38dca,0xa5)
    b=building(400);byte(u,b,1);byte(u,b+3,3);word(u,b+16,74);word(u,b+64,1)
    byte(u,b+203,1);word(u,b+6,90);word(u,b+8,80);word(u,b+180,0);put(u,0x9783b0+28,15)
    g=group(3);u.mem_write(g,b'\xa5'*144)
    word(u,g+130,-1);byte(u,g+3,0);word(u,g+4,13)
    for off,v in [(8,301),(10,303),(12,0),(42,1),(46,83),(48,90),(54,97),(56,104)]:word(u,g+off,v)
    byte(u,g+102,0);byte(u,g+115,0);byte(u,g+119,254);put(u,g+132,310)


def endpoints(c,u,state,calls,mutation='',real_map=False):
    def change(cpu):
        if mutation=='neighbor':
            word(cpu,actor(301)+20,1234);word(cpu,actor(301)+22,-1234);put(cpu,0xd056f8,303)
    if not real_map:
        def neighbor(cpu,args):
            calls.append(('neighbor',args));put(cpu,0xd3b4e0,0x3456abcd);put(cpu,0xd3b4e4,0x4567ef01)
            change(cpu);return 1
        hook(c,state,0x4806c0,3,neighbor)
    else:
        table=0x5cc9e0+3*96;put(u,table,4);put(u,table+4,0)
        put(u,0xaf598c+(1000+80*228+90+4)*4,4)
        put(u,0x5cce98+3*24*8,-5);put(u,0x5cce9c+3*24*8,9)
        hook(c,state,0x47db00,1,lambda cpu,args:calls.append(('path begin',args)) or 0)
        hook(c,state,0x484f40,0,lambda cpu,args:8)
    def adjust(cpu,args):
        calls.append(('formation',args));put(cpu,0xd056f8,303);return 0
    hook(c,state,0x4b7b50,2,adjust)
    def project(cpu,args):
        calls.append(('project',args[:4]));put(cpu,args[4],777);put(cpu,args[5],-999);return 0
    hook(c,state,0x40c180,6,project)
    hook(c,state,0x406720,0,lambda cpu,args:calls.append(('selection',get(cpu,0xeadccc))) or 0)
    if mutation=='percentage':
        def percentage(cpu,args):
            calls.append(('percentage',args));put(cpu,0xd3b198,2);put(cpu,0xeadcc8,303);return 0x12340064
        hook(c,state,0x4cde60,2,percentage)


def verify(token,args=(),configure=None,mutation='',real_map=False,result=False):
    values=[]
    for c,u,state in machines:
        prepare(u,state)
        if configure:configure(u)
        calls=[];endpoints(c,u,state,calls,mutation,real_map)
        value=run(c,u,token,args,budget=3000000)
        values.append(((value if result else 0,calls),snapshot(u)))
    assert values[0][0]==values[1][0],(hex(token),'result/trace',values[0][0],values[1][0])
    equal(values,(hex(token),args,mutation,real_map))


def command(token,mode=0,st=8,health=75,paused=0,linked=-1,cargo=0,kind=77,selected=301,mutation='',real_map=False):
    def setup(u):
        put(u,0xd3b198,mode);byte(u,actor(301)+56,st);put(u,actor(301)+296,health)
        byte(u,0xe38e60,paused);word(u,actor(301)+306,linked);put(u,actor(301)+304,cargo)
        word(u,actor(301)+306,linked);byte(u,actor(301)+10,kind)
        put(u,0xeadcc8,selected);put(u,0xeadcc4,selected)
    verify(token,configure=setup,mutation=mutation,real_map=real_map)


def formation(mode=0,kind=13,leader_state=19,order=0,busy=0,status=0,gate=-1,paused=0,full=False):
    def setup(u):
        g=group(3);put(u,0xd3b198,mode);word(u,g+4,kind);byte(u,actor(301)+56,leader_state)
        byte(u,g+102,order);byte(u,g+115,busy);byte(u,g+3,status);word(u,g+130,gate)
        byte(u,0xe38e60,paused)
        if full:
            for i in range(16):word(u,g+8+i*2,301 if i%2==0 else 303)
    verify(0x406fb0,configure=setup)


def toggle(order=1,previous=4,refresh=0,paused=0,value=254):
    def setup(u):
        put(u,0xbcb9c0,previous);put(u,0xd3b194,refresh);word(u,group(3)+42,order)
        byte(u,group(3)+119,value);byte(u,0xe38e60,paused)
    verify(0x407110,configure=setup)


def rank(kind=74,status=1,index=1,score=513):
    def setup(u):
        b=building(400);word(u,b+16,kind);byte(u,b,status);byte(u,b+203,index)
        put(u,0x9783b0+index*28,score)
    verify(0x4069e0,[400],configure=setup,result=True)


def nearest(rows):
    def setup(u):
        u.mem_write(0x7999d8,bytes(4000*264))
        for i,x,y,score,workers in rows:
            b=building(i);byte(u,b,1);word(u,b+16,74);word(u,b+64,workers);byte(u,b+203,i%7+1)
            word(u,b+6,x);word(u,b+8,y);put(u,0x9783b0+(i%7+1)*28,score)
    verify(0x406a30,[301],configure=setup,result=True)


if __name__=='__main__':
    n=0
    for t,mode,st in itertools.product([0x406b40,0x406c90,0x406de0,0x406e80],
                                      [0,1,2,3,4,256,-1],[8,11,13,15]):
        command(t,mode,st);n+=1
    for t,health,paused in itertools.product([0x406b40,0x406c90,0x406de0,0x406e80],[15,16,100],[0,1]):
        command(t,3,15,health,paused);n+=1
    for t,mutation in itertools.product([0x406b40,0x406c90,0x406de0,0x406e80],['percentage','neighbor']):
        command(t,3,mutation=mutation);n+=1
    for t,selected in itertools.product([0x406b40,0x406c90,0x406de0,0x406e80],[0,-1]):
        command(t,selected=selected);n+=1
    for cargo in [0,1,256]:command(0x406b40,2,cargo=cargo);n+=1
    for linked in [0,1,256]:command(0x406c90,2,linked=linked);n+=1
    print('PASS unit command aliases: %d native/C states, modes, gates and real helper compositions'%n,flush=True)
    n=0
    for kind,status,index in itertools.product([74,330],[0,1],[0,1,255]):rank(kind,status,index);n+=1
    for rows in [
        [],[(1,0,0,200,1)],[(1,0,0,201,1)],[(1,0,0,-1,1)],
        [(1,0,0,199,0)],[(1,32767,-32768,200,1)],
        [(1,0,0,5,1),(7,257,-513,5,1)],[(1,257,-513,5,1),(7,257,-513,5,1)],
        [(1,257,-513,6,1),(7,100,100,5,-1)],[(3999,90,80,0,1)],
    ]:nearest(rows);n+=1
    print('PASS building rank/search aliases: %d native/C filters, priorities and signed distance ties'%n,flush=True)
    n=0
    for t,kind in itertools.product([0x4a45d0,0x4bc670],[76,77,78,79]):
        verify(t,[301],configure=lambda u,k=kind:byte(u,actor(301)+10,k));n+=1
        verify(t,[301],configure=lambda u,k=kind:byte(u,actor(301)+10,k),mutation='neighbor');n+=1
    for kind in [76,77,78,79]:
        def empty(u,k=kind):byte(u,actor(301)+10,k);word(u,building(400)+64,0)
        verify(0x4a45d0,[301],configure=empty);n+=1
    for token in [0x406b40,0x406c90,0x406de0,0x406e80]:
        command(token,3 if token in (0x406b40,0x406c90) else 4,real_map=True);n+=1
    for token in [0x4a45d0,0x4bc670]:verify(token,[301],real_map=True);n+=1
    print('PASS assignment/return aliases: %d native/C targets, global ownership, route release and actual map-query compositions'%n,flush=True)
    n=0
    for mode,order,busy in itertools.product([0,1,256],[0,1,2,255],[0,1]):formation(mode=mode,order=order,busy=busy);n+=1
    for settings in [{'kind':12},{'leader_state':4},{'status':1},{'gate':0},{'paused':1},{'full':True},{'order':1,'full':True}]:
        formation(**settings);n+=1
    print('PASS formation alias: %d native/C conditions,16 slots and cached current-actor cases'%n,flush=True)
    n=0
    for order,previous,refresh,paused in itertools.product([1,2,3,4,15,16],[0,3,4],[0,1],[0,1]):
        toggle(order,previous,refresh,paused);n+=1
    print('PASS toggle alias: %d native/C remap, BYTE toggle, previous selection and tail refresh cases'%n,flush=True)
    print('target SHA256',target_sha)
