"""Whole multiple-goal actor route coordinator and actual weighted callers."""
from verify_actor_weighted_paths import *

ENDS={0x51a9d0:6,0x51b220:4,0x51a220:4,0x518b30:4,0x519e30:4,
      0x5190a0:4,0x518dd0:4,0x51d6d0:4,0x51b400:6,0x51dc10:6,
      0x51d980:6,0x51d160:2,0x51ce90:2}
for off in [36,38,58,60,62]:WATCH.add(a(255)+off)
WATCH.update([0xd04e5a,0xd04e5c,0xd04e5e])


def parent(terrain=2,water=0,editor=0,kind=4,action=0,saved=0,query=1,built=3,prepare=None):
    results=[]
    for c,u,state in machines:
        setup(u,state);p=a(255)
        u.mem_write(0xd04e58,bytes(2000));put(u,0xd056f8,255);put(u,0xbca2d8,7000)
        for off,value in [(10,kind),(14,1),(56,action),(114,terrain),(118,water)]:byte(u,p+off,value)
        for off,value in [(20,10),(22,11),(36,18),(38,19),(58,77),(60,88),(62,99)]:word(u,p+off,value)
        put(u,p+348,saved);put(u,p+320,123);put(u,0x605a3c,editor)
        word(u,0x784c8c,-3);word(u,0x784c8e,31)
        for address_,tile in [(S+8,7000+228*9+8),(S+16,7000+228*13+12),(S+20,7000+228*15+14)]:put(u,address_,tile)
        calls=[]
        for endpoint,nargs in ENDS.items():
            def observed(cpu,args,endpoint=endpoint):
                calls.append((endpoint,args,get(cpu,0xd056f8),bytes(cpu.mem_read(p+20,44))))
                return built if endpoint in [0x51b400,0x51dc10,0x51d980] else query if ENDS[endpoint]>=4 else 0
            hook(c,state,endpoint,nargs,observed)
        if prepare:prepare(c,u,state,calls)
        state['service_writes'].clear();value=run(c,u,0x4b28f0,[255,S+16,2])
        results.append(((value,calls,list(state['service_writes'])),snapshot(u)))
    same(results,('goal route',terrain,water,editor,kind,action,saved,query,built))
    return results[1]


def verify_branches():
    count=0
    for terrain,query,built in itertools.product([*range(13),255],[0,1,2],[0,3,0xffffffff]):
        parent(terrain=terrain,query=query,built=built);count+=1
    for water,editor,kind,action in itertools.product([1,2,3],[0,1,256],[4,78],[0,9]):
        parent(water=water,editor=editor,kind=kind,action=action);count+=1
    parent(water=1,kind=78,saved=9);count+=1
    for terrain,first,second in itertools.product([1,3,4],[0,1,-1],[0,2,-1]):
        def construction(c,u,state,calls):
            def build(cpu,args):calls.append((0x51b400,args));return first if args[0]==4 else second
            hook(c,state,0x51b400,6,build)
        result=parent(terrain=terrain,query=2,prepare=construction)
        assert result[0][0]==2;count+=1
    for terrain,first,second in itertools.product([2,3],[0,1],[0,1,2]):
        def fallback(c,u,state,calls):
            first_token=0x51a9d0 if terrain==2 else 0x519e30
            second_token=0x51b220 if terrain==2 else 0x5190a0
            def query(cpu,args,endpoint,value):calls.append((endpoint,args));return value
            hook(c,state,first_token,ENDS[first_token],lambda cpu,args:query(cpu,args,first_token,first))
            hook(c,state,second_token,4,lambda cpu,args:query(cpu,args,second_token,second))
        parent(terrain=terrain,prepare=fallback);count+=1
    print('PASS all coordinator land/water/editor branches, query fallbacks, signed construction gates and retained goal-index result:',count,flush=True)


def verify_coordinates_and_mutations():
    count=0
    for difference,selected in itertools.product([-2147483648,-229,-228,-1,0,227,228,2147483647],[1,2,0xffffffff]):
        def coordinates(c,u,state,calls):put(u,(S+16+selected*4-4)&0xffffffff,7000+difference)
        parent(query=selected,prepare=coordinates);count+=1
    for answer in [0,1,256,257]:
        def allocation(c,u,state,calls):
            def slot(cpu,args):calls.append((0x489970,args[0]));put(cpu,args[1],1);return answer
            hook(c,state,0x489970,2,slot)
        parent(prepare=allocation);count+=1
    def changed(c,u,state,calls):
        def query(cpu,args):
            calls.append((0x51a9d0,args));put(cpu,0xd056f8,1);put(cpu,0xbca2d8,8000)
            word(cpu,a(255)+20,40);word(cpu,a(255)+22,50);put(cpu,S+20,8000+20*228+30);return 2
        hook(c,state,0x51a9d0,6,query)
    parent(prepare=changed);count+=1
    def escaped(c,u,state,calls):
        ptr=[]
        def slot(cpu,args):ptr[:]=[args[1]];calls.append((0x489970,args[0]));put(cpu,args[1],1);return 1
        def build(cpu,args):calls.append((0x51b400,args));put(cpu,ptr[0],2);return 3
        def clear(cpu,args):calls.append((0x4b3250,args));put(cpu,ptr[0],3);put(cpu,0xd056f8,1);return 0
        hook(c,state,0x489970,2,slot);hook(c,state,0x51b400,6,build);hook(c,state,0x4b3250,1,clear)
    result=parent(prepare=escaped)
    assert struct.unpack_from('<H',result[1],a(255)+58-0x560000)[0]==3;count+=1
    def old_route(c,u,state,calls):
        def query(cpu,args):
            calls.append((0x51a9d0,args));word(cpu,a(255)+58,2);word(cpu,0xd04e58+4,255);return 1
        hook(c,state,0x51a9d0,6,query)
    result=parent(prepare=old_route)
    assert struct.unpack_from('<H',result[1],0xd04e58+4-0x560000)[0]==0;count+=1
    print('PASS signed goal conversion, AL allocation, cached actor, fresh goal/coordinate data, escaped slot reload and actual route cleanup:',count,flush=True)


def verify_actual_routes():
    count=0
    for entry in [0x4b28f0,0x4b2d20]:
        results=[]
        for c,u,state in machines:
            setup(u,state);p=a(255);put(u,0xeadfd8,0xffffffff)
            for y in range(7,17):
                for x in range(7,17):
                    tile=y*228+x;byte(u,0xec7630+tile,0);put(u,0xaf598c+tile*4,0)
                    word(u,0xb4e6fc+tile*2,0)
            for off,value in [(10,4),(14,1),(56,9),(57,14),(114,2)]:byte(u,p+off,value)
            for off,value in [(20,10),(22,10),(36,13),(38,11)]:word(u,p+off,value)
            put(u,p+32,2290);word(u,0xb350dc+2290*2,255);put(u,0xd056f8,255)
            put(u,0xbca2d0,228);put(u,0xbca2d4,228);u.mem_write(0xd04e58,bytes(2000))
            for i,tile in enumerate([11*228+13,2291,2291]):put(u,S+i*4,tile)
            state['service_writes'].clear()
            value=run(c,u,entry,[255,S,3] if entry==0x4b28f0 else [255])
            if entry==0x4b28f0:assert value==3
            goal=(11,10) if entry==0x4b28f0 else (13,11)
            history=[]
            for tick in range(65):
                run(c,u,0x4a5da0,[255,6]);history.append(bytes(u.mem_read(p+17,49)))
            assert struct.unpack('<hh',u.mem_read(p+20,4))==goal,(entry,c,goal)
            results.append(((value if entry==0x4b28f0 else None,history,list(state['service_writes'])),snapshot(u)))
        same(results,('actual weighted coordinator and full walk',hex(entry)));count+=1
    print('PASS actual single/multiple weighted callers, duplicate-goal index, construction, ownership and complete walk to selected target:',count,flush=True)


if __name__=='__main__':
    verify_branches();verify_coordinates_and_mutations();verify_actual_routes()
    print('Verified target SHA256:',target_sha,flush=True)
