"""Whole native/C3D sound pool resize and shutdown, without a host game.

Only allocation and Miles API endpoints are controlled. Composed cases use
the real stop-all and channel-default helpers. No audio DLL is loaded.
"""
from verify_options_startup import *

SELF=S+0x1000;H2=S+0x4000;C2=S+0x5000;V2=S+0x6000
H3=S+0x8000;C3=S+0x9000;V3=S+0xa000;NEW=S+0x10000
MEDIA={
    'release3d':(0x56f2e8,'_AIL_release_3D_sample_handle_4',1),
    'allocate3d':(0x56f2ec,'_AIL_allocate_3D_sample_handle_4',1),
    'free_locked':(0x56f308,'_AIL_mem_free_lock_4',1),
    'release2d':(0x56f2f0,'_AIL_release_sample_handle_4',1),
    'close_stream':(0x56f2d0,'_AIL_close_stream_4',1),
    'close_provider':(0x56f2d4,'_AIL_close_3D_provider_4',1),
    'shutdown':(0x56f2d8,'_AIL_shutdown_0',0),
    'end2d':(0x56f2cc,'_AIL_end_sample_4',1),
    'end3d':(0x56f2c8,'_AIL_end_3D_sample_4',1),
}

def prepare(u,state,n2=2,n3=3,disabled=0,provider=0,mask=7):
    reset(u,state);u.mem_write(SP-0x5000,b'\xa5'*0x5000)
    u.mem_write(SELF,b'\x5a'*0x204);u.mem_write(NEW,b'\xa5'*0x10000)
    for a,v in [(0x198,n2),(0x1e4,n3),(0x200,disabled),(0x164,provider),
                (0x19c,H2),(0x1a8,V2),(0x1ac,C2),(0x1a0,S+0x7000),
                (0x1f0,H3 if mask&1 else 0),(0x1f4,V3 if mask&2 else 0),
                (0x1f8,C3 if mask&4 else 0),(0x1e8,0x12345678),
                (0x1d8,17),(0x1dc,0),(0x1e0,19)]:put(u,SELF+a,v)
    for i in range(max(n2,n3,0)):
        put(u,H2+i*4,0x100+i);put(u,H3+i*4,0x200+i)
        for p in [C2,C3]:put(u,p+i*12+4,0x800+i if i%2==0 else 0)

def endpoints(c,u,state,kind,real=False,fail_array=-1,fail_handle=-1,mutation=''):
    calls=[];saved_iat=[];allocated=[];samples=[]
    def malloc(cpu,args):
        i=len(allocated);allocated.append(args[0]);calls.append(('malloc',args))
        return 0 if i==fail_array else NEW+i*0x4000
    def free(cpu,args):
        calls.append(('free',args))
        if mutation=='free':put(cpu,SELF+0x1f4,V3+0x100)
        return 0
    hook(c,state,0x561a1d,1,malloc);hook(c,state,0x561a2b,1,free)
    if c:
        state['hooks'][sym('restore_heap_allocate')]=(1,0,malloc)
        state['hooks'][sym('restore_heap_free')]=(1,0,free)
    for j,(name,(slot,public,n)) in enumerate(MEDIA.items()):
        def media(cpu,args,name=name):
            calls.append((name,args))
            if name=='allocate3d':
                i=len(samples);samples.append(i)
                if mutation=='allocate' and i==0:
                    put(cpu,SELF+0x1e4,1);put(cpu,SELF+0x1f0,NEW+0xc000)
                return 0 if i==fail_handle else 0x100000+i
            if mutation=='release' and name in ('release2d','release3d'):
                put(cpu,SELF+(0x198 if name=='release2d' else 0x1e4),1)
            if mutation=='cache' and name=='free_locked':
                put(cpu,SELF+(0x1ac if kind=='shutdown' and len(calls)<4 else 0x1f8),C2+0x100)
            if mutation=='stream' and name=='close_stream':put(cpu,SELF+0x1e0,27)
            return 0x87654321
        if c:state['hooks'][sym(public)]=(n,0,media)
        else:
            saved_iat.append((slot,get(u,slot)));put(u,slot,S+0x48000+j*16)
            state['hooks'][S+0x48000+j*16]=(n,n*4,media)
    if not real:
        if kind=='pool':
            def stop(cpu,args):
                self=args[0] if c else cpu.reg_read(UC_X86_REG_ECX)
                calls.append(('stop_all',self));return 0
            hook(c,state,0x55ec7c,1 if c else 0,stop)
        else:
            def defaults(cpu,args):
                self,index=args if c else (cpu.reg_read(UC_X86_REG_ECX),args[0])
                calls.append(('defaults',self,index));return 0
            hook(c,state,0x55e5d2,2 if c else 1,defaults,0 if c else 4)
    return calls,saved_iat

def execute(kind,count=5,real=False,fail_array=-1,fail_handle=-1,mutation='',**settings):
    results=[]
    for c,u,state in machines:
        prepare(u,state,**settings)
        calls,slots=endpoints(c,u,state,kind,real,fail_array,fail_handle,mutation)
        token=0x55e14f if kind=='pool' else 0x55e92b
        args=([SELF] if c else [])+([count] if kind=='pool' else [])
        value=run(c,u,token,args,cleanup=4 if not c and kind=='pool' else 0,receiver=None if c else SELF)
        for slot,original in slots:put(u,slot,original)
        results.append(((value,tuple(calls),bytes(u.mem_read(S,0x20000))),snapshot(u)))
    equal(results,(kind,count,real,fail_array,fail_handle,mutation,settings))

def cases():
    pool=shutdown=0
    for count in [-1,0,1,5,256]:
        execute('pool',count=count);pool+=1
    for mask in range(8):
        # Nonzero handles imply a live cache table in the original contract.
        if mask&1 and not mask&4:continue
        for fail in [-1,0,1,2]:execute('pool',mask=mask,fail_array=fail);pool+=1
    for i in [-1,0,2,4]:execute('pool',fail_handle=i);pool+=1
    for mutation in ['free','release','cache','allocate']:
        execute('pool',mutation=mutation);pool+=1
    execute('pool',real=True);pool+=1
    for disabled,provider,n2,n3 in itertools.product([0,1,256],[0,5],[-1,0,3],[-1,0,3]):
        execute('shutdown',disabled=disabled,provider=provider,n2=n2,n3=n3);shutdown+=1
    for mutation in ['release','stream','cache']:
        execute('shutdown',mutation=mutation);shutdown+=1
    for provider in [0,5]:execute('shutdown',provider=provider,real=True);shutdown+=1
    print('PASS complete3D pool allocation, failure/partial-handle and callback reload cases:',pool,flush=True)
    print('PASS complete sound shutdown, streams, provider and real helper composition:',shutdown,flush=True)
    print('PASS total cases:',pool+shutdown,'target SHA256',target_sha,flush=True)

if __name__=='__main__':cases()
