"""Whole native/C pyramid-worker selection, claims, pairing and route gaps.

Runs all decision logic and complete compositions. Only explicit callback
mutation cases control providers; no host game or physical renderer runs.
"""
from verify_options_startup import *


def prepare(u,state):
    reset(u,state);u.mem_write(SP-0x5000,b'\xa5'*0x5000)


def compare(values,label):
    assert values[0][0]==values[1][0],(label,'return/calls',values[0][0],values[1][0])
    equal(values,label)


def endpoint(c,state,token,name,n,action):
    state['hooks'][sym(name) if c else token]=(n,0,action)


def setup(u,mode=1,kind=2,count=5):
    put(u,0x5e4ef4,mode);put(u,0xbca2d8,1000);put(u,0xbca2e0,2)
    put(u,(0x56f7ec if mode==5 else 0x56f7d8)+kind*4,count)
    for i in range(max(count,12)):
        r=0x7999d8+(400+i)*264;u.mem_write(r,b'\xa5'*264)
        word(u,r+6,70+i*2);word(u,r+8,80+i*3);put(u,r+12,20000+i)
        word(u,r+16,183);word(u,r+18,i);word(u,r+100,0)
        for off,value in [(84,kind),(85,0),(86,2),(87,2),(88,4),(89,0),(90,0)]:byte(u,r+off,value)
    a=0xbcd5f8+301*388;u.mem_write(a,b'\xa5'*388)
    byte(u,a+10,80);byte(u,a+14,1);word(u,a+0x14,60);word(u,a+0x16,80)
    put(u,a+0x20,21000);put(u,a+0x28,22000);word(u,0xb4e6fc+21000*2,401);word(u,0xb4e6fc+22000*2,401)
    for off,value in [(0x4a,400),(0x4c,0xabcd),(0x4e,401),(0x50,14),(0x120,2)]:word(u,a+off,value)
    return a


def assignable(mode,phase,root_phase,thresholds,position,occupied=(0,0),kind=2,piece=3):
    values=[]
    for c,u,state in machines:
        prepare(u,state);setup(u,mode,kind)
        r=0x7999d8+403*264;word(u,r+18,piece)
        byte(u,r+85,phase);byte(u,0x799a2d+(403-piece)*264,root_phase)
        byte(u,r+86,position%6);byte(u,r+87,position//6)
        byte(u,r+89,occupied[0]);byte(u,r+90,occupied[1])
        put(u,0x570070+kind*4,2);table=0x5f3878+kind*960
        put(u,table,999);put(u,table+20,piece)
        put(u,table+24,thresholds[0]);put(u,table+28,thresholds[1])
        value=run(c,u,0x4ef100,[403]);values.append((value,snapshot(u)))
    compare(values,('slot',mode,phase,root_phase,thresholds,position,occupied,kind,piece))


def pending(mode,row,height,column,alternate,piece=0):
    values=[]
    for c,u,state in machines:
        prepare(u,state);setup(u,mode)
        r=0x7999d8+400*264
        for off,value in [(86,column),(87,row),(88,height)]:byte(u,r+off,value)
        word(u,r+18,piece);byte(u,0x5e68d8+2*6+row,alternate)
        value=run(c,u,0x4efd90,[400])&255;values.append((value,snapshot(u)))
    compare(values,('pending',mode,row,height,column,alternate,piece))


def claim(kind,state_value,valid=1,saved_state=0,slot=0,target=400,owner=601):
    values=[]
    for c,u,state in machines:
        prepare(u,state);setup(u)
        r=0x7999d8+400*264;word(u,r+100,owner)
        a=0xbcd5f8+owner*388;byte(u,a+10,kind);byte(u,a+14,valid)
        byte(u,a+0x38,state_value);byte(u,a+16,saved_state);put(u,a+0x28,20000);word(u,0xb4e6fc+20000*2,target)
        # Real slot query executes, with both phases completed unless an
        # explicit worker-slot assignment is requested.
        byte(u,r+85,1 if not slot else 0)
        put(u,0x570070+2*4,1);table=0x5f3878+2*960
        put(u,table,0);put(u,table+4,1 if slot else 0);put(u,table+8,0)
        value=run(c,u,0x4eea10,[400])&255;values.append((value,snapshot(u)))
    compare(values,('claim',kind,state_value,valid,saved_state,slot,target,owner))


def pair(piece,listed=(2,3,4,5),owner=0,actor_type=80,actor_state=11,mode=1,kind=2):
    values=[]
    for c,u,state in machines:
        prepare(u,state);setup(u,mode,kind)
        table=(0x5f5d58+kind*160) if mode==5 else (0x5f5f38+kind*624)
        for i,value in enumerate([2,3,4,5,-1,-1]):put(u,table+i*4,value)
        word(u,0x7999ea+400*264,piece);put(u,0xeaaf58,len(listed))
        for i,value in enumerate(listed):
            id=700+i;put(u,0xeaac44+i*4,id);word(u,0x7999ea+id*264,value);word(u,0x799a3c+id*264,owner)
        a=0xbcd5f8+owner*388;byte(u,a+10,actor_type);byte(u,a+0x38,actor_state)
        u.mem_write(S,b'\xa5'*4)
        value=run(c,u,0x4f27b0,[400,S]);flag=bytes(u.mem_read(S,4))
        worker=run(c,u,0x4f28e0,[400]);values.append(((value,flag,worker),snapshot(u)))
    compare(values,('pair',piece,len(listed),owner,actor_type,actor_state,mode,kind))


def gap(delta,mode=1,change='',owner_kind=81,owner_state=10,kind=2,count=5,last=0):
    values=[]
    for c,u,state in machines:
        prepare(u,state);a=setup(u,mode,kind,count)
        word(u,a+0x3a,1);word(u,a+0x3c,20);word(u,a+0x3e,100)
        r=0x7999d8+(400+last)*264;word(u,r+100,600)
        other=0xbcd5f8+600*388;byte(u,other+10,owner_kind);byte(u,other+0x38,owner_state);word(u,other+0x68,601)
        linked=0xbcd5f8+601*388;byte(u,linked+10,85);byte(u,linked+0x38,17)
        word(u,linked+0x3a,1);word(u,linked+0x3c,-20);word(u,linked+0x3e,60+delta)
        if change=='slot':word(u,a+0x3a,-1)
        elif change=='empty':word(u,0xb4e6fc+22000*2,0)
        elif change=='kind':word(u,0x7999e8+401*264,0x1b7)
        elif change=='link':word(u,other+0x68,0)
        elif change=='type':byte(u,linked+10,84)
        elif change=='state':byte(u,linked+0x38,16)
        elif change=='linked_slot':word(u,linked+0x3a,-1)
        value=run(c,u,0x4f2ba0,[301])&255;values.append((value,snapshot(u)))
    compare(values,('gap',delta,mode,change,owner_kind,owner_state,kind,count,last))


def selection(mode=1,owners=(0,0,0,0,0),pairs=(-1,-1,-1,-1,-1),
        workers=(0,0,0,0,0),phases=(0,0,0,0,0),refresh=1,problem='',mutate='',real=False,kind=2):
    values=[]
    for c,u,state in machines:
        prepare(u,state);a=setup(u,mode,kind)
        coords=[(80,90),(100,90),(60,80),(110,130),(75,100)]
        if problem=='overflow':coords=[(-32768,-32768),(32767,32767),(0,0),(30000,-30000),(-30000,30000)]
        for i,(x,y) in enumerate(coords):
            r=0x7999d8+(400+i)*264;word(u,r+6,x);word(u,r+8,y);word(u,r+100,owners[i]);byte(u,r+85,phases[i])
        for owner in [301,600,601,602,-1]:
            actor=0xbcd5f8+owner*388;byte(u,actor+14,1);byte(u,actor+10,80);byte(u,actor+0x38,11)
        put(u,0x570070+kind*4,0)
        table=(0x5f5498+kind*128) if mode==5 else (0x5f4b38+kind*480)
        for i in range(16 if mode==5 else 60):put(u,table+i*8,6);put(u,table+i*8+4,14 if problem!='table_end' else -1)
        pair_table=(0x5f5d58+kind*160) if mode==5 else (0x5f5f38+kind*624)
        for i,value in enumerate([0,1,2,3,-1,-1]):put(u,pair_table+i*4,value)
        if problem=='kind':word(u,0x7999e8+401*264,0x1b7)
        elif problem=='position':word(u,a+0x50,15)
        elif problem=='type':byte(u,a+10,81)
        elif problem=='zero_position':byte(u,0x799a2e+400*264,0)
        elif problem=='negative_claim':word(u,0x799a3c+404*264,-1)
        calls=[]
        if not real:
            def collect(cpu,args):
                calls.append(('collect',args));put(cpu,0xeaaf58,0 if problem=='empty' else 5)
                for i in range(5):put(cpu,0xeaac44+i*4,400+i)
                return 0
            endpoint(c,state,0x4ef450,'restore_pyramid_collect_work_pieces',1,collect)
            def phase(cpu,args):
                i=args[0]-400;calls.append(('pending',args))
                if mutate=='phase' and i==0:
                    put(cpu,0x5e4ef4,2);byte(cpu,a+10,80);word(cpu,0x7999de+400*264,111)
                    put(cpu,0xeaaf58,1)
                return 0x12340000|(i&1)
            endpoint(c,state,0x4efd90,'restore_pyramid_piece_has_pending_work',1,phase)
            def paired(cpu,args):
                i=args[0]-400;calls.append(('pair',args[:1]));byte(cpu,args[1],1)
                if mutate=='pair' and i==0:word(cpu,0x7999de+401*264,-400);word(cpu,0x799a3c+400*264,601)
                return pairs[i]
            endpoint(c,state,0x4f27b0,'restore_pyramid_find_paired_work_piece',2,paired)
            def paired_worker(cpu,args):
                calls.append(('worker',args));return workers[args[0]-400]
            endpoint(c,state,0x4f28e0,'restore_pyramid_paired_worker',1,paired_worker)
            def active(cpu,args):
                calls.append(('claim',args))
                if mutate=='claim':byte(cpu,0x799a2d+args[0]*264,1);word(cpu,0x799a3c+args[0]*264,0)
                return 0x12340000|(problem=='active')
            endpoint(c,state,0x4eea10,'restore_pyramid_worker_claim_active',1,active)
            endpoint(c,state,0x4f02f0,'restore_monument_worker_begin_travel',4,
                lambda cpu,args:calls.append(('travel',(args[0],args[1],args[2]&65535,args[3]&255))) or 0)
        run(c,u,0x4f0ef0,[301,refresh,0x102,0x103,0x104],budget=5000000)
        values.append((calls,snapshot(u)))
    compare(values,('selection',mode,owners,pairs,workers,phases,refresh,problem,mutate,real,kind))


if __name__=='__main__':
    n=0
    for mode,position in itertools.product([1,5,257],[0,1,6,7,14,35,255]):
        for occupied in [(0,0),(1,0),(1,1)]:assignable(mode,0,0,(6,14),position,occupied);n+=1
    for phase,root_phase,thresholds in itertools.product([0,1],[0,1],[(0,0),(-1,1),(0x10001,14)]):
        assignable(1,phase,root_phase,thresholds,14);n+=1
    print('PASS worker slots: %d native/C threshold, ownership and full mode cases'%n,flush=True)
    n=0
    for mode,alternate,piece in itertools.product([1,2,3,5,257],[0,1],[0,3]):
        for row,height,column in [(0,0,0),(0,1,1),(0,1,3),(0,1,6),(1,2,6),(1,3,6),(255,255,255)]:
            pending(mode,row,height,column,alternate,piece);n+=1
    print('PASS pending work: %d native/C alternate rows, roots and boundary cases'%n,flush=True)
    n=0
    for actor_type,state_value in itertools.product([0,79,80,81,82,85,86],range(32)):
        claim(actor_type,state_value);n+=1
    for actor_type,state_value,saved_state in itertools.product([79,80,81,85],[6,7],[9,10,17,30]):
        claim(actor_type,state_value,saved_state=saved_state);n+=1
    for valid,slot,target in itertools.product([0,1],[0,1],[400,401]):
        claim(81,10,valid=valid,slot=slot,target=target);n+=1
    claim(80,11,owner=-1);claim(80,11,owner=0);n+=2
    print('PASS worker claims: %d native/C all state branches, saved states and cleanup cases'%n,flush=True)
    n=0
    for mode,piece in itertools.product([1,5,257],[-1,2,3,4,5,6]):
        for listed in [(2,3,4,5),(2,4),(9,9),()]:pair(piece,listed,mode=mode);n+=1
    for owner,actor_type,actor_state in itertools.product([0,301,-1],[80,81,85],[10,11,12]):
        pair(2,owner=owner,actor_type=actor_type,actor_state=actor_state);n+=1
    pair(2,(9,)*256+(3,),owner=301);n+=1
    print('PASS paired pieces/workers: %d native/C direction, sentinel, signed owner and full list cases'%n,flush=True)
    n=0
    for mode,delta in itertools.product([1,5,257],[-8,-7,-6,-1,0,1,6,7,8]):gap(delta,mode);n+=1
    for change in ['slot','empty','kind','link','type','state','linked_slot']:gap(0,change=change);n+=1
    for actor_type,actor_state in [(80,11),(81,10),(81,11),(80,10),(85,17)]:gap(0,owner_kind=actor_type,owner_state=actor_state);n+=1
    gap(0,count=257,last=256);n+=1
    print('PASS route gaps: %d native/C seven-step threshold, guards and DWORD scan cases'%n,flush=True)
    n=0
    for mode,problem in itertools.product([0,1,2,3,4,5,257],['','kind','position','type','empty','overflow','table_end']):
        selection(mode,problem=problem,refresh=0 if problem=='position' else 1);n+=1
    for owners,pairs,workers in [((600,0,0,0,0),(-1,)*5,(0,)*5),
            ((600,601,602,601,600),(-1,)*5,(0,)*5),((301,301,301,301,301),(-1,)*5,(0,)*5),
            ((0,)*5,(401,400,403,402,-1),(0,301,-1,0,0)),((0,0,0,0,600),(-1,)*5,(0,)*5)]:
        selection(owners=owners,pairs=pairs,workers=workers);n+=1
    for mutate in ['phase','pair','claim']:
        selection(owners=(600,)*5,pairs=(401,400,403,402,-1),mutate=mutate);n+=1
    for problem in ['active','negative_claim']:
        selection(owners=(600,)*5,problem=problem);n+=1
    selection(phases=(1,)*5);n+=1
    print('PASS selection decisions: %d native/C ranking, conflicts, mode and callback cases'%n,flush=True)
    n=0
    for mode,kind in [(m,k) for m in [1,2,3,4,257] for k in range(5)]+[(5,k) for k in range(3)]:
        for owners in [(0,)*5,(600,)*5,(600,601,0,301,0)]:
            selection(mode,kind=kind,owners=owners,real=True);n+=1
    selection(1,phases=(1,)*5,real=True);n+=1
    print('PASS actual selection compositions: %d native/C list, pairing, claims and travel cases'%n,flush=True)
    print('target SHA256',target_sha)
