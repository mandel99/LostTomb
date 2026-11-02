"""Native/C provider selection, including explicit native stack prestate.

Absent categories and an unwritten environment result are undefined native
locals. Compare an explicit 556-byte prestate, never invent zero handles.
Public-wrapper cases enumerate all categories and write the attribute.
Miles and allocation endpoints are controlled; no physical audio is claimed.
"""
from verify_sound_3d import *

SCRATCH=S+0x3000
PROVIDERS={
    'enumerate':(0x56f280,'_AIL_enumerate_3D_providers_12',3),
    'open':(0x56f284,'_AIL_open_3D_provider_4',1),
    'attribute':(0x56f288,'_AIL_3D_provider_attribute_12',3),
    'allocate3d':(0x56f2ec,'_AIL_allocate_3D_sample_handle_4',1),
    'end3d':(0x56f2c8,'_AIL_end_3D_sample_4',1),
    'end2d':(0x56f2cc,'_AIL_end_sample_4',1),
}
ALL=['EAX','A3D','Intel','hardware','software']

def provider_case(names=ALL,success=0,attribute=-1,supported=0,pool=1,
                  disabled=0,prestate=0xa5a5a5a5,mutation='',public=False,real_pool=False):
    results=[]
    for c,u,state in machines:
        prepare(u,state)
        put(u,SELF+0x1ec,supported);put(u,SELF+0x200,disabled)
        if real_pool:
            for off in [0x198,0x1e4,0x1f0,0x1f4,0x1f8]:put(u,SELF+off,0)
        frame=SCRATCH if c else SP-560
        initial=bytearray(b'\xa5'*556)
        struct.pack_into('<I',initial,12,prestate)
        struct.pack_into('<5I',initial,536,901,902,903,904,905)
        u.mem_write(frame,bytes(initial))
        for i,name in enumerate(names):u.mem_write(NAME+i*128,name.encode()+b'\0')
        calls,slots=endpoints(c,u,state,mutation='environment')
        enumerated=opened=allocated=0;actual_frame=frame
        for j,(name,(slot,export,n)) in enumerate(PROVIDERS.items()):
            def media(cpu,args,name=name):
                nonlocal enumerated,opened,allocated,actual_frame
                if name=='enumerate':
                    iterator,handle,text=args;actual_frame=iterator-4
                    calls.append(('enumerate',get(cpu,iterator)))
                    if enumerated==len(names):return 0
                    put(cpu,iterator,enumerated+1);put(cpu,handle,101+enumerated)
                    put(cpu,text,NAME+enumerated*128);enumerated+=1;return 1
                if name=='open':
                    index=opened;opened+=1;calls.append(('open',args))
                    if mutation=='open':put(cpu,actual_frame+536+index*4,2001+index)
                    return 0 if index==success else 1
                if name=='attribute':
                    calls.append(('attribute',args[:2]))
                    if attribute is not None:put(cpu,args[2],attribute)
                    return 0x87654321
                calls.append((name,args))
                if name=='allocate3d':
                    value=0x30000+allocated;allocated+=1;return value
                return 0
            if c:state['hooks'][sym(export)]=(n,0,media)
            else:
                slots.append((slot,get(u,slot)));put(u,slot,S+0x49000+j*16)
                state['hooks'][S+0x49000+j*16]=(n,n*4,media)
        def text(cpu,p):
            return bytes(cpu.mem_read(p,100)).split(b'\0',1)[0]
        def format_name(cpu,args):
            destination,fmt,p=args;value=text(cpu,p)
            calls.append(('format',fmt,value));cpu.mem_write(destination,value+b'\0');return len(value)
        hook(c,state,0x561a36,3,format_name)
        if not real_pool:
            def resize(cpu,args):
                receiver,count=args if c else (cpu.reg_read(UC_X86_REG_ECX),args[0])
                calls.append(('pool',receiver,count))
                if pool:put(cpu,SELF+0x1e4,3)
                return pool
            hook(c,state,0x55e14f,2 if c else 1,resize,0 if c else 4)
        else:
            def allocate(cpu,args):
                calls.append(('malloc',args));return {20:S+0x38000,200:S+0x39000,60:S+0x3a000}[args[0]]
            hook(c,state,0x561a1d,1,allocate)
            if c:state['hooks'][sym('restore_heap_allocate')]=(1,0,allocate)
        if mutation=='reset':
            def channel(cpu,args):
                receiver,index=args if c else (cpu.reg_read(UC_X86_REG_ECX),args[0])
                calls.append(('reset',receiver,index));put(cpu,SELF+0x1e4,1)
                put(cpu,SELF+0x1f8,C+12);return 0
            hook(c,state,0x55fa1c,2 if c else 1,channel,0 if c else 4)
        if c and not public:
            run(0,u,sym('restore_sound_select_3d_body'),[SELF,frame])
        else:run(c,u,0x55f697,[SELF] if c else [],receiver=None if c else SELF)
        scratch=None if public else bytes(u.mem_read(frame,556))
        # Private frames live at different addresses; compare their contents
        # separately, excluding only their duplicate copy from shared memory.
        u.mem_write(SCRATCH,bytes(556))
        for slot,old in slots:put(u,slot,old)
        results.append(((tuple(calls),scratch,bytes(u.mem_read(S,0x40000))),snapshot(u)))
    if results[0]!=results[1]:
        a,b=results[0][0],results[1][0]
        def diff(x,y,base):return [(hex(base+i),v,w) for i,(v,w) in enumerate(zip(x,y)) if v!=w][:12]
        raise AssertionError(('providers',names,success,attribute,supported,pool,disabled,mutation,public,real_pool,
            'calls',a[0],b[0],'scratch',diff(a[1] or b'',b[1] or b'',0),
            'memory',diff(a[2],b[2],S)))

def cases():
    n=0
    sets=[[],['unrelated'],ALL,list(reversed(ALL)),['EAX A3D Intel hardware software'],
          ['EAX old','hardware','EAX new'],['eax A3d intel Hardware Software']]
    for names,success in itertools.product(sets,range(-1,5)):
        provider_case(names,success);n+=1
    for success,attribute,supported,pool in itertools.product(range(5),[-1,0,7],[0,1],[0,1]):
        provider_case(success=success,attribute=attribute,supported=supported,pool=pool);n+=1
    for prestate,supported in itertools.product([0,0xffffffff,0xa5a5a5a5],[0,1]):
        provider_case(['hardware'],2,attribute=None,prestate=prestate,supported=supported);n+=1
    for mutation in ['open','reset']:
        provider_case(mutation=mutation,attribute=1);n+=1
    provider_case(disabled=1);n+=1
    # All private outputs are defined on the public wrapper path.
    for success in range(-1,5):provider_case(success=success,attribute=3,public=True);n+=1
    for success in [0,2,4]:provider_case(success=success,attribute=3,real_pool=True);n+=1
    print('PASS provider priority, repeated/multiple matches, failures, exact prestate and real pool composition:',n,flush=True)
    print('PASS target SHA256',target_sha,flush=True)
    return n

if __name__=='__main__':cases()
