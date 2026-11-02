"""Native/C auxiliary sprite cache lifetime; no host game or media DLL.

Allocation release and child virtual deletion are controlled endpoints. The
native vector iterator and actual record/free-stack methods compose normally.
Child virtual-destructor target reconstruction is explicitly outside this proof.
"""
from verify_options_startup import *

SELF=S+0x1000;SLOT=S+0x2000;FIRST=S+0x3000;LAST=S+0x3100
CHILD=S+0x4000;TABLE=S+0x4100;RECORDS=S+0x5000;FREE=S+0x8000
ALT_FIRST=S+0xc000;ALT_LAST=S+0xc100;ALT_RECORDS=S+0xd000;OTHER=S+0xe000
for c,u,state in machines:u.mem_map(0,4096)

def prepare(u,state,mask=31,count=3):
    reset(u,state);u.mem_write(SP-0x5000,b'\xa5'*0x5000);put(u,0,0x12345678)
    u.mem_write(SELF,b'\x5a'*0x124)
    put(u,SLOT,SELF)
    for off,bit,value in [(0x114,1,CHILD),(0x118,2,FIRST),(0x11c,4,RECORDS),(0x120,8,LAST)]:
        put(u,SELF+off,value if mask&bit else 0)
    put(u,0x12652f8,S+0xb000 if mask&16 else 0);put(u,0x12652f4,0x87654321)
    put(u,CHILD,TABLE);put(u,TABLE,S+0x49000)
    for p,allocation in [(FIRST,S+0x9000),(LAST,S+0xa000),(ALT_FIRST,S+0xf000),(ALT_LAST,S+0xf100)]:
        put(u,p,FREE+0x100);put(u,p+8,allocation)
    for p,n in [(RECORDS,count),(ALT_RECORDS,1)]:
        put(u,p-4,n)
        for i in range(4):
            u.mem_write(p+i*24,struct.pack('<6I',0x11111111,SELF if mask&8 else 0,
                S+0x10000+i*0x100,0x22222222,i,0x33333333))
    u.mem_write(OTHER,bytes(0x124));put(u,OTHER+0x11c,ALT_RECORDS);put(u,OTHER+0x120,ALT_LAST)

def endpoints(c,u,state,mutation=''):
    calls=[]
    def release(cpu,args):
        allocation=args[0];calls.append(('free',allocation))
        if mutation=='first' and allocation==S+0x9000:
            put(cpu,SELF+0x118,ALT_FIRST);put(cpu,SELF+0x11c,ALT_RECORDS)
            put(cpu,SELF+0x120,ALT_LAST);put(cpu,0x12652f8,S+0xf200)
        if mutation=='record-owner' and S+0x10000<=allocation<S+0x10400:
            for i in range(4):put(cpu,RECORDS+i*24+4,OTHER);put(cpu,RECORDS+i*24+16,2)
        if mutation=='record-array' and S+0x10000<=allocation<S+0x10400:
            put(cpu,SELF+0x11c,ALT_RECORDS)
        if mutation=='last' and allocation==S+0xa000:put(cpu,SELF+0x120,ALT_LAST)
        if mutation=='global' and allocation==S+0xb000:
            put(cpu,0x12652f8,S+0xf200);put(cpu,0x12652f4,0x12345678)
        if mutation=='slot':put(cpu,SLOT,OTHER)
        return 0xabcdef01
    def child(cpu,args):
        object,flags=args if c else (cpu.reg_read(UC_X86_REG_ECX),args[0])
        calls.append(('child',object,flags))
        if mutation=='child':
            put(cpu,SELF+0x114,S+0xf300);put(cpu,SELF+0x118,ALT_FIRST)
            put(cpu,SELF+0x11c,ALT_RECORDS);put(cpu,SELF+0x120,ALT_LAST)
            put(cpu,0x12652f8,S+0xf200)
        return 0x98765432
    if c:
        state['hooks'][sym('restore_sprite_shutdown_free')]=(1,0,release)
        state['hooks'][sym('restore_sprite_shutdown_delete')]=(2,0,child)
    else:
        hook(0,state,0x561a2b,1,release)
        state['hooks'][S+0x49000]=(1,4,child)
    return calls

def execute(token,args=(),mask=31,count=3,mutation='',setup=None,global_cleanup=False):
    results=[]
    for c,u,state in machines:
        prepare(u,state,mask,count)
        if setup:setup(u)
        calls=endpoints(c,u,state,mutation)
        if global_cleanup:
            put(u,0xea136c,0);put(u,0xea1370,0);put(u,0xea1368,SELF)
            def manager(cpu,a):
                object=a[0] if c else cpu.reg_read(UC_X86_REG_ECX)
                calls.append(('manager',object));return 1
            hook(c,state,0x53db60,1 if c else 0,manager)
            value=run(c,u,0x4e63b0)
        else:
            receiver=RECORDS if token==0x55db90 else SELF
            member=token!=0x55d960
            value=run(c,u,token,([receiver] if c and member else [])+list(args),
                cleanup=len(args)*4 if not c and member else 0,
                receiver=receiver if not c and member else None)
        if token==0x55d990 and not global_cleanup:assert value==0
        assert get(u,0)==0x12345678
        results.append(((tuple(calls),bytes(u.mem_read(S,0x20000))),snapshot(u)))
    if results[0]!=results[1]:
        a,b=results[0],results[1]
        raise AssertionError((hex(token),mask,count,mutation,global_cleanup,'calls',a[0][0],b[0][0],
            'local diff',[(hex(S+i),x,y) for i,(x,y) in enumerate(zip(a[0][1],b[0][1])) if x!=y][:12],
            'global diff',[(hex(0x560000+i),x,y) for i,(x,y) in enumerate(zip(a[1],b[1])) if x!=y][:12]))

def cases():
    n=0
    for index in [-2147483648,-2,-1,0,1,123,0x7fffffff]:
        execute(0x55d990,[index]);n+=1
    for owner,allocation,index in itertools.product([0,SELF],[0,S+0x10000],[-2,-1,0,2,0x7fffffff]):
        def setup(u):put(u,RECORDS+4,owner);put(u,RECORDS+8,allocation);put(u,RECORDS+16,index)
        execute(0x55db90,setup=setup);n+=1
    execute(0x55db90,mutation='record-owner');n+=1
    print('PASS returning/destroying auxiliary records, signed indices and callback reloads:',n,flush=True)
    whole=0
    for mask,count in itertools.product(range(32),[0,1,3]):
        execute(0x55d8b0,mask=mask,count=count);whole+=1
    for count in [0x80000001,0xffffffff]:
        execute(0x55d8b0,count=count);whole+=1
    for mutation in ['child','first','record-owner','record-array','last','global']:
        execute(0x55d8b0,mutation=mutation);whole+=1
    for count in [0,1,3,0x80000001,0xffffffff]:
        execute(0x55d960,[SLOT],count=count);whole+=1
    execute(0x55d960,[SLOT],setup=lambda u:put(u,SLOT,0));whole+=1
    execute(0x55d960,[SLOT],mutation='slot');whole+=1
    for mask in [0,2,4,8,16,31]:
        execute(0x55d8b0,mask=mask,global_cleanup=True);whole+=1
    print('PASS whole owner/vector/slot cleanup and actual global sprite cleanup composition:',whole,flush=True)
    print('PASS total:',n+whole,'target SHA256',target_sha,flush=True)

if __name__=='__main__':cases()
