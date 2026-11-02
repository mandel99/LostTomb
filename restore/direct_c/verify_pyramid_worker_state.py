"""Whole native/C travel states, exact work-list sort and worker recruitment.

Actual compositions use the existing C qsort/comparator, work/claim queries,
and motion helpers. Controlled provider cases check callback lifetimes.
No host game or physical rendering runs.
"""
from verify_options_startup import *


def prepare(u,state):
    reset(u,state);u.mem_write(SP-0x5000,b'\xa5'*0x5000)
    for reg,value in [(UC_X86_REG_FPCW,0x37f),(UC_X86_REG_FPSW,0),(UC_X86_REG_FPTAG,0xffff)]:u.reg_write(reg,value)


def endpoint(c,state,token,name,n,action):
    state['hooks'][sym(name) if c else token]=(n,0,action)


def compare(values,label):
    assert values[0][0]==values[1][0],(label,'return/trace',values[0][0],values[1][0])
    equal(values,label)


def setup(u,family=1,kind=2):
    put(u,0x5e4ef4,family);put(u,0xd056f8,302);put(u,0xbca2d8,1000);put(u,0xbca2e0,0)
    put(u,(0x56f7ec if family==5 else 0x56f7d8)+kind*4,5)
    for i in range(8):
        r=0x7999d8+(400+i)*264;u.mem_write(r,bytes(264))
        word(u,r+6,90+i);word(u,r+8,80+i);put(u,r+12,1000+(80+i)*228+90+i)
        word(u,r+16,183);word(u,r+18,i);byte(u,r+84,kind);byte(u,r+86,2);byte(u,r+87,2);byte(u,r+88,4)
    a=0xbcd5f8+301*388;u.mem_write(a,b'\xa5'*388)
    for off,value in [(0x14,80),(0x16,70),(0x24,93),(0x26,83),(0x4a,400),(0x4c,257),
                     (0x4e,403),(0x50,7),(0x52,0),(0x54,0),(0x120,2)]:word(u,a+off,value)
    byte(u,a+10,81);byte(u,a+14,1);put(u,a+0x20,19000);put(u,a+0x28,22000);word(u,0xb4e6fc+22000*2,403)
    for table,count in [(0x5f4b38+kind*480,60),(0x5f5498+kind*128,16)]:
        for i in range(count+1):put(u,table+i*8,-1);put(u,table+i*8+4,-1)
        put(u,table,3);put(u,table+4,7);put(u,table+8,4);put(u,table+12,8)
    return a


def travel(family=1,mode=0,action=2,problem='',slots=0,owner='none',mutate='',real=False):
    values=[]
    for c,u,state in machines:
        prepare(u,state);a=setup(u,family);r=0x7999d8+403*264
        word(u,a+0x120,action);word(u,r+100,301 if owner=='self' else 0)
        if owner=='next':word(u,0x799a3c+404*264,301)
        table=(0x5f5498+2*128) if family==5 else (0x5f4b38+2*480)
        if problem=='kind':word(u,r+16,0x1b7)
        elif problem=='zero_kind':word(u,r+16,0)
        elif problem=='not_arrived':word(u,a+0x54,30)
        elif problem=='ready':word(u,a+0x50,14)
        elif problem=='missing_row':word(u,a+0x50,6)
        elif problem=='sentinel':put(u,table+8,-1);byte(u,0x799a2d+400*264,1)
        elif problem=='same':put(u,table+8,3)
        elif problem=='last_row':
            index=(16 if family==5 else 60)-1
            put(u,table,-1);put(u,table+index*8,3);put(u,table+index*8+4,7)
        calls=[]
        if not real:
            def advance(cpu,args):
                calls.append(('advance',args))
                if mutate=='advance':put(cpu,0x5e4ef4,5);put(cpu,a+0x28,23000)
                return 0x12340000 if problem=='not_arrived' else 0x12340001
            endpoint(c,state,0x4f1d10,'restore_monument_worker_advance_travel',2,advance)
            def slot(cpu,args):
                calls.append(('slot',args))
                if mutate=='slot':word(cpu,r+18,2)
                return slots
            endpoint(c,state,0x4ef100,'restore_pyramid_available_worker_slot',1,slot)
            def collect(cpu,args):
                calls.append(('collect',args));put(cpu,0xeaaf58,0 if problem=='empty' else 1);put(cpu,0xeaac44,400)
                if mutate=='collect':word(cpu,a+0x50,14)
                return 0
            endpoint(c,state,0x4ef450,'restore_pyramid_collect_work_pieces',1,collect)
            def next_piece(cpu,args):
                calls.append(('next piece',args))
                if mutate=='next':word(cpu,a+0x50,0xabcd)
                return 404
            endpoint(c,state,0x4f2190,'restore_pyramid_next_work_piece',1,next_piece)
            def begin(cpu,args):
                calls.append(('begin',(args[0],args[1],args[2]&65535,args[3]&255)))
                word(cpu,a+0x54,55);return 0
            endpoint(c,state,0x4f02f0,'restore_monument_worker_begin_travel',4,begin)
        else:
            put(u,0x570070+2*4,1);desc=0x5f3878+2*960;put(u,desc,3);put(u,desc+4,1 if slots else 0);put(u,desc+8,0)
        def grid(cpu,args,t):
            calls.append((t,args,bytes(cpu.mem_read(a+0x14,16))))
            if mutate=='grid':
                put(cpu,0x5e4ef4,5);word(cpu,a+0x24,94);word(cpu,a+0x26,84);put(cpu,a+0x28,23000)
            return 0
        for t in [0x4b5420,0x4b52f0]:hook(c,state,t,1,lambda cpu,args,t=t:grid(cpu,args,t))
        run(c,u,0x4f04d0,[301,mode,0x102,0x103],budget=3000000)
        values.append((calls,snapshot(u)))
    compare(values,('travel',family,mode,action,problem,slots,owner,mutate,real))


def linked(state_value,saved_state=0,valid=1,kind=85,owner=301,expected=301):
    values=[]
    for c,u,state in machines:
        prepare(u,state);a=0xbcd5f8+600*388
        byte(u,a+10,kind);byte(u,a+14,valid);byte(u,a+0x38,state_value);byte(u,a+16,saved_state);word(u,a+0x68,owner)
        value=run(c,u,0x4f26a0,[600,expected])&255;values.append((value,snapshot(u)))
    compare(values,('linked',state_value,saved_state,valid,kind,owner,expected))


def second(piece,listed,mode=1):
    values=[]
    for c,u,state in machines:
        prepare(u,state);setup(u,mode);table=(0x5f5d58+2*160) if mode==5 else (0x5f5f38+2*624)
        for i,v in enumerate([2,3,-1,-1]):put(u,table+i*4,v)
        word(u,0x7999ea+400*264,piece);put(u,0xeaaf58,len(listed))
        for i,p in enumerate(listed):put(u,0xeaac44+i*4,600+i);word(u,0x7999ea+(600+i)*264,p)
        value=run(c,u,0x4f28b0,[400])&255;values.append((value,snapshot(u)))
    compare(values,('second',piece,listed,mode))


@cache
def sort_machines():
    # This complete sort has no external endpoints. Avoid a Python callback
    # for every emulated instruction of the existing instruction-level C CRT.
    result=[]
    for c,pe in enumerate(images):
        u=Uc(UC_ARCH_X86,UC_MODE_32)
        u.mem_map(pe.base,(len(pe.data)+4095)&~4095);u.mem_write(pe.base,bytes(pe.data))
        u.mem_map(0x30000000,0x10000);u.mem_map(STOP,4096);u.mem_map(S,0x50000)
        result.append((c,u,{'hooks':{},'calls':[]}))
    return result


def sort_work(count,mode=1,ties=False):
    values=[]
    for c,u,state in sort_machines():
        prepare(u,state);setup(u,mode);put(u,0xeaaf58,count)
        for i in range(max(count,8)):
            r=0x7999d8+(600+i)*264;u.mem_write(r,bytes(264))
            word(u,r+18,i);byte(u,r+84,2);byte(u,r+86,1);byte(u,r+87,1)
            word(u,r+6,100 if ties else 100+(i*17)%31);word(u,r+8,100 if ties else 110-(i*13)%29)
            if i<count:put(u,0xeaac44+i*4,600+count-1-i)
        for table,n in [(0x5f4b38+2*480,60),(0x5f5498+2*128,16)]:
            for i in range(n):put(u,table+i*8,3);put(u,table+i*8+4,999)
        run(c,u,0x4f25c0,budget=200000000);values.append(([],snapshot(u)))
    compare(values,('sort',count,mode,ties))


def recruit(detach=0,linked_state=0,problem='',mutate='',real=False,family=1):
    values=[]
    for c,u,state in machines:
        prepare(u,state);setup(u,family)
        for i in range(5):
            r=0x7999d8+(400+i)*264;owner=500+i
            word(u,r+100,owner);actor=0xbcd5f8+owner*388
            byte(u,actor+10,80);byte(u,actor+14,1);byte(u,actor+0x38,11);word(u,actor+0x68,600+i if linked_state else 0)
            worker=0xbcd5f8+(600+i)*388;byte(u,worker+10,85);byte(u,worker+14,1)
            byte(u,worker+0x38,linked_state);byte(u,worker+16,15);word(u,worker+0x68,owner)
        table=(0x5f5d58+2*160) if family==5 else (0x5f5f38+2*624)
        for i,v in enumerate([0,1,2,3,-1,-1]):put(u,table+i*4,v)
        if problem=='invalid':byte(u,0xbcd606+600*388,0)
        elif problem=='dead':byte(u,0xbcd606+500*388,0)
        elif problem=='owner_state':byte(u,0xbcd630+500*388,12)
        calls=[]
        if not real:
            def collect(cpu,args):
                calls.append(('collect',args));put(cpu,0xeaaf58,0 if problem=='empty' else 3)
                for i in range(3):put(cpu,0xeaac44+i*4,400+i)
                return 0
            endpoint(c,state,0x4ef450,'restore_pyramid_collect_work_pieces',1,collect)
            endpoint(c,state,0x4f25c0,'restore_pyramid_sort_work_list',0,lambda cpu,args:calls.append(('sort',)) or 0)
            def side(cpu,args):calls.append(('side',args));return 1 if problem=='second' else 0x12340000
            endpoint(c,state,0x4f28b0,'restore_pyramid_is_second_work_piece',1,side)
            def claim(cpu,args):
                calls.append(('claim',args))
                if mutate=='claim':word(cpu,0x799a3c+args[0]*264,502)
                return 0x12340000 if problem=='inactive' else 0x12340001
            endpoint(c,state,0x4eea10,'restore_pyramid_worker_claim_active',1,claim)
            def valid_link(cpu,args):
                calls.append(('link',args))
                if mutate=='link':byte(cpu,0xbcd630+args[0]*388,17)
                return 0 if problem=='invalid' else 0x12340001
            endpoint(c,state,0x4f26a0,'restore_pyramid_linked_worker_active',2,valid_link)
            def paired_worker(cpu,args):
                calls.append(('paired worker',args))
                if mutate=='count':put(cpu,0xeaaf58,1)
                return 0 if problem=='no_pair_worker' else -1
            endpoint(c,state,0x4f28e0,'restore_pyramid_paired_worker',1,paired_worker)
        value=run(c,u,0x4f29e0,[403,detach],budget=10000000)
        values.append(((value,calls),snapshot(u)))
    compare(values,('recruit',detach,linked_state,problem,mutate,real,family))


if __name__=='__main__':
    for name in ['restore_pyramid_worker_update_travel','restore_pyramid_sort_work_list',
                 'restore_pyramid_linked_worker_active','restore_pyramid_is_second_work_piece',
                 'restore_pyramid_find_available_worker','restore_pyramid_sort_entries']:sym(name)
    n=0
    for family,mode,action in itertools.product([1,5,257],[0,1,2,3,257,-1],[1,2,256]):
        travel(family,mode,action);n+=1
    for problem in ['kind','zero_kind','not_arrived','ready','missing_row','empty','sentinel','same','last_row']:
        for mode in [0,1,2]:travel(mode=mode,problem=problem,slots=1,owner='next');n+=1
    for owner,slots in itertools.product(['self','next'],[0,1,256]):travel(mode=1,owner=owner,slots=slots);n+=1
    for mutation in ['advance','grid','slot','collect','next']:
        travel(mode=0 if mutation=='next' else 1,owner='next',slots=1,problem='sentinel' if mutation=='next' else '',mutate=mutation);n+=1
    print('PASS travel state: %d native/C modes, claims, rows, cached values and callback cases'%n,flush=True)
    n=0
    for st in range(32):linked(st);n+=1
    for st,saved_state in itertools.product([6,7],[15,16,17,18,19,20,21,29,30]):linked(st,saved_state);n+=1
    for valid,kind,owner,expected in [(0,85,301,301),(1,84,301,301),(1,85,1,0x10001),(1,85,-1,-1)]:linked(17,valid=valid,kind=kind,owner=owner,expected=expected);n+=1
    print('PASS linked worker: %d native/C every state, saved state and signed WORD comparison'%n,flush=True)
    n=0
    for piece,listed,mode in itertools.product([2,3,4],[(2,3),(2,),(3,),()],[1,5,257]):second(piece,listed,mode);n+=1
    print('PASS second piece: %d native/C paired-side cases'%n,flush=True)
    n=0
    for count,mode,ties in itertools.product([0,1,2,3,8,16,17,33,65,128,196],[1,5,257],[False,True]):sort_work(count,mode,ties);n+=1
    print('PASS exact C sort: %d native/C list sizes, duplicates and tie-order cases'%n,flush=True)
    n=0
    for detach,st,problem in itertools.product([0,1,256],[0,15,16,17,18,6],['','invalid','inactive','second','empty','no_pair_worker','owner_state']):
        recruit(detach,st,problem);n+=1
    for mutation in ['claim','link','count']:recruit(1,15,mutate=mutation);n+=1
    print('PASS recruitment: %d native/C linked/paired workers, cached ownership and detach cases'%n,flush=True)
    n=0
    for family,mode,problem in itertools.product([1,5,257],[0,1,2],['','ready','sentinel']):travel(family,mode,slots=1,owner='next',problem=problem,real=True);n+=1
    for family,detach,st in itertools.product([1,5,257],[0,1],[0,15,17,6]):recruit(detach,st,real=True,family=family);n+=1
    print('PASS actual state/recruit compositions: %d native/C work, sort, claim and motion cases'%n,flush=True)
    print('target SHA256',target_sha)
