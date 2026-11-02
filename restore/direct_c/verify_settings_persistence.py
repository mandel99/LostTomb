"""Native/C default difficulty, complete exit writer and restart persistence.

All file and allocation endpoints are controlled. Captured bytes are held in
memory; this script does not launch the host game or write game settings.
"""
from verify_options_startup import *

def bootstrap_payload(c,u,state,payload):
    hook(c,state,0x414a90,0,lambda cpu,args:1)
    hook(c,state,0x417dd0,1,lambda cpu,args:0)
    hook(c,state,0x561a0a,2,lambda cpu,args:0 if payload is None else S+0x4000)
    hook(c,state,0x56b22e,1,lambda cpu,args:37)
    hook(c,state,0x56b1b6,1,lambda cpu,args:len(payload))
    def read(cpu,args):
        assert args==(0xe38e58,1,len(payload),S+0x4000)
        cpu.mem_write(args[0],payload);return len(payload)
    hook(c,state,0x561ca1,4,read);hook(c,state,0x561994,1,lambda cpu,args:0)
    slot=sym('_imp__GetSystemMetrics@4') if c else 0x56f1a0
    before=get(u,slot);put(u,slot,S+0x49000)
    state['hooks'][S+0x49000]=(1,4,lambda cpu,args:1024)
    run(c,u,0x4147f0);put(u,slot,before)

def exit_capture(c,u,state,mask=0,mutate=False,fail=-1):
    state['hooks'].clear();calls=[];writes={};opened=[]
    slots=[0xe92e1c,0xe36c80,0xe36c70,0xe36c74]
    for i,a in enumerate(slots):put(u,a,0x12340000+i*0x100 if mask&(1<<i) else 0)
    put(u,0xf535bc,0x12345678)
    def open_file(cpu,args):
        path,flags,mode=args;assert flags==0x8301 and mode==0x180
        index=len(opened);opened.append(path);calls.append(('open',args))
        return -1 if index==fail else 37+index
    def write_file(cpu,args):
        fd,p,n=args;data=bytes(cpu.mem_read(p,n));writes[opened[fd-37]]=data
        calls.append(('write',fd,p,data));return n
    def close_file(cpu,args):calls.append(('close',args));return 0
    def diagnostic(cpu,args):calls.append(('diagnostic',args));return 0
    def release(cpu,args):
        calls.append(('release',args))
        if mutate:
            # A destructor may replace any later pointer. Native reloads it.
            put(cpu,0xe36c70,0x23456700);put(cpu,0xe36c74,0x34567800)
        return 0
    for token,n,fn in [(0x568eab,3,open_file),(0x5664d5,3,write_file),
                       (0x5647eb,1,close_file),(0x4ce6e0,3,diagnostic),
                       (0x561bdc,1,release),(0x561a2b,1,release)]:hook(c,state,token,n,fn)
    run(c,u,0x414750)
    assert len(opened)==2
    return tuple(calls),writes

def cleanup():
    n=0
    for mask,mutate,fail in itertools.product(range(16),[False,True],[-1,0,1]):
        results=[]
        for c,u,state in machines:
            reset(u,state);u.mem_write(SP-0x5000,b'\xa5'*0x5000)
            put(u,0xe39080,2)
            calls,writes=exit_capture(c,u,state,mask,mutate,fail)
            if fail!=0:assert struct.unpack_from('<I',writes[0x579274],552)[0]==2
            results.append(((calls,writes),snapshot(u)))
        equal(results,('settings exit',mask,mutate,fail));n+=1
    print('PASS complete exit, actual binary writer, failures and full DWORD cleanup/reloads:',n,flush=True)
    return n

def restart():
    n=0;source=(ROOT/'build/debug-startup-game/Pharaoh.inf').read_bytes()
    for level in [None,*range(5)]:
        results=[]
        for c,u,state in machines:
            reset(u,state);payload=None if level is None else bytearray(source)
            if payload is not None:struct.pack_into('<I',payload,552,level);payload=bytes(payload)
            bootstrap_payload(c,u,state,payload)
            expected=2 if level is None else level
            assert get(u,0xe39080)==expected
            before=snapshot(u)
            calls,writes=exit_capture(c,u,state)
            persisted=writes[0x579274];assert len(persisted)==564
            reset(u,state);bootstrap_payload(c,u,state,persisted)
            assert get(u,0xe39080)==expected
            results.append(((before,persisted),snapshot(u)))
        equal(results,('startup/save/restart',level));n+=1
    print('PASS missing config defaults to Normal and all five choices survive save/restart:',n,flush=True)
    return n

def family_profile():
    n=0
    for level,mode in itertools.product(range(5),['full','old','missing']):
        results=[]
        payload=bytearray(0x4a9c);payload[0x2c90:0x2c97]=b'Family\0'
        for i in range(3):struct.pack_into('<I',payload,0x2d1c+i*76,i%2)
        for c,u,state in machines:
            reset(u,state);u.mem_write(SP-0x7000,b'\xa5'*0x7000)
            put(u,0xe39080,level);put(u,0x6fdd08+0x2c8c,3)
            u.mem_write(S+0x2000,b'family.dat\0');reads=[]
            hook(c,state,0x561a0a,2,lambda cpu,args:0 if mode=='missing' else S+0x4000)
            def read(cpu,args):
                destination,size,count,stream=args
                assert count==1 and stream==S+0x4000
                reads.append(size);cpu.mem_write(destination,bytes(payload[:size]))
                return 0 if mode=='old' and len(reads)==1 else 1
            hook(c,state,0x561ca1,4,read)
            hook(c,state,0x561ea4,3,lambda cpu,args:0)
            hook(c,state,0x561994,1,lambda cpu,args:0)
            result=run(c,u,0x41b680,[0x6fdd08,S+0x2000] if c else [S+0x2000],
                       cleanup=0 if c else 4,receiver=None if c else 0x6fdd08)&255
            assert get(u,0xe39080)==level
            results.append(((result,tuple(reads)),snapshot(u)))
        equal(results,('family profile preserves selected difficulty',level,mode));n+=1
    print('PASS complete original/C family-profile load leaves global difficulty unchanged:',n,flush=True)
    return n

if __name__=='__main__':
    total=cleanup()
    if '--cleanup-only' not in sys.argv:total+=restart()+family_profile()
    print('PASS total cases:',total,'target SHA256',target_sha,flush=True)
