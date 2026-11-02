"""Whole native/C 3D sound cache and playback; controlled Miles endpoints.

No host game or audio DLL is loaded. Real cache/reset helpers compose with
playback; separate boundary cases exercise the native signed-byte results.
"""
from verify_options_startup import *

SELF=S+0x1000;REQ=S+0x5000;H=S+0x10000;V=S+0x14000;C=S+0x1a000
FILES=S+0x20000;NAME=S+0x24000
MEDIA={
    'status':(0x56f27c,'_AIL_3D_sample_status_4',1),
    'free':(0x56f308,'_AIL_mem_free_lock_4',1),
    'read':(0x56f2b4,'_AIL_file_read_8',2),
    'file':(0x56f274,'_AIL_set_3D_sample_file_8',2),
    'loop':(0x56f2ac,'_AIL_set_3D_sample_loop_count_8',2),
    'volume':(0x56f278,'_AIL_set_3D_sample_volume_8',2),
    'position':(0x56f268,'_AIL_set_3D_position_16',4),
    'start':(0x56f26c,'_AIL_start_3D_sample_4',1),
    'environment':(0x56f270,'_AIL_set_3D_provider_preference_12',3),
    'orientation':(0x56f2a8,'_AIL_set_3D_orientation_28',7),
    'update':(0x56f294,'_AIL_3D_update_position_8',2),
}

def prepare(u,state,n=3):
    reset(u,state);u.mem_write(SP-0x5000,b'\xa5'*0x5000)
    u.mem_write(SELF,b'\x5a'*0x204)
    for off,value in [(4,6),(8,FILES),(0x164,0),(0x1e4,n),(0x1e8,17),
                      (0x1ec,1),(0x1f0,H),(0x1f4,V),(0x1f8,C),(0x200,0)]:
        put(u,SELF+off,value)
    # Prefix records explicitly cover signed-byte channel/cache indices.
    for i in range(-128,260):
        put(u,H+i*4,0x10000+i)
        u.mem_write(V+i*40,struct.pack('<10I',*(0xa000+j for j in range(10))))
        put(u,V+i*40,0x20000+i);put(u,V+i*40+4,5);put(u,V+i*40+24,i%3)
        u.mem_write(C+i*12,struct.pack('<3I',5,0x80000+i,7))
    for i in range(6):
        put(u,FILES+i*8,NAME+i*32);u.mem_write(NAME+i*32,('sound%d.wav\0'%i).encode())
    u.mem_write(REQ,struct.pack('<10I',77,2,91,64,3,0,0,0x3f800000,0xc0200000,0x7fc12345))

def endpoints(c,u,state,status=(),mutation='',read_result=0x12345678):
    calls=[];saved=[];status_count=0
    for j,(name,(slot,public,n)) in enumerate(MEDIA.items()):
        def media(cpu,args,name=name):
            nonlocal status_count
            if name=='environment':
                normalized=args[:2]+(get(cpu,args[2]),)
                assert get(cpu,SELF+0x1fc)==get(cpu,args[2])
            else:normalized=args
            calls.append((name,normalized))
            if name=='status':
                index=status_count;status_count+=1
                if mutation=='status-count' and index==0:put(cpu,SELF+0x1e4,1)
                if mutation=='status-tables' and index==0:
                    put(cpu,SELF+0x1f0,H+4);put(cpu,SELF+0x1f4,V+40);put(cpu,SELF+0x1f8,C+12)
                return status[index%len(status)] if status else 0
            if name=='free' and mutation=='free':
                put(cpu,SELF+0x1f4,V+40);put(cpu,SELF+0x1f8,C+12);put(cpu,REQ+4,3)
            if name=='read':
                if mutation=='read':
                    put(cpu,SELF+0x1f8,C+12);put(cpu,SELF+0x1f0,H+4);put(cpu,REQ+4,4)
                return read_result
            if name in ('file','loop','volume','position') and mutation==name:
                put(cpu,SELF+0x1f0,H+4);put(cpu,SELF+0x1f4,V+40)
                put(cpu,REQ+8,27);put(cpu,REQ+16,9);put(cpu,REQ+28,0xdeadbeef)
            if name=='environment' and mutation=='environment':put(cpu,args[2],0xdeadbeef)
            if name=='update' and mutation=='update':
                put(cpu,SELF+0x1f0,H+4);put(cpu,SELF+0x1e4,2)
            return 0x87654321
        if c:state['hooks'][sym(public)]=(n,0,media)
        else:
            saved.append((slot,get(u,slot)));put(u,slot,S+0x48000+j*16)
            state['hooks'][S+0x48000+j*16]=(n,n*4,media)
    return calls,saved

def execute(token,args=(),n=3,setup=None,status=(),mutation='',read_result=0x12345678,
            helper_results=None,void=False,expected=None,label=''):
    results=[]
    for c,u,state in machines:
        prepare(u,state,n)
        if setup:setup(u)
        calls,slots=endpoints(c,u,state,status,mutation,read_result)
        if helper_results is not None:
            for t,values in helper_results.items():
                argc={0x55faf7:0,0x55fbd8:0,0x55fb6f:1,0x55fca4:2}[t]
                def helper(cpu,a,t=t,values=values):
                    self=a[0] if c else cpu.reg_read(UC_X86_REG_ECX)
                    assert self==SELF
                    calls.append((hex(t),a[1:] if c else a))
                    return values
                hook(c,state,t,argc+c,helper,0 if c else argc*4)
        value=run(c,u,token,([SELF] if c else [])+list(args),
                  cleanup=0 if c else len(args)*4,receiver=None if c else SELF)
        if void:value=None
        if expected is not None:assert value==expected&0xffffffff,(hex(token),c,label,value,expected)
        for slot,original in slots:put(u,slot,original)
        results.append(((value,tuple(calls),bytes(u.mem_read(S,0x30000))),snapshot(u)))
    if results[0]!=results[1]:
        a,b=results[0][0],results[1][0]
        raise AssertionError((hex(token),label,'returns',a[0],b[0],'calls',a[1],b[1],
            'memory differences',[(hex(S+i),x,y) for i,(x,y) in enumerate(zip(a[2],b[2])) if x!=y][:12]))

def leaves():
    n=0
    for i in [-128,-1,0,1,2,127,128,255,256]:
        execute(0x55fa1c,[i],void=True,label=('reset',i));n+=1
    for count,disabled,provider in itertools.product([-1,0,1,3],[0,1],[0,5]):
        def setup(u):put(u,SELF+0x200,disabled);put(u,SELF+0x164,provider)
        for statuses in [(4,4,4),(4,2,4)]:
            execute(0x55faf7,n=count,setup=setup,status=statuses,label=('available',count,disabled,provider,statuses));n+=1
    for mutation in ['status-count','status-tables']:
        execute(0x55faf7,status=(4,2),mutation=mutation);n+=1
    for count,hit,data in itertools.product([-1,0,1,3],[0,1,2],[0,0x12340000]):
        def setup(u):put(u,C+hit*12,2);put(u,C+hit*12+4,data)
        execute(0x55fb6f,[REQ],n=count,setup=setup,label=('cache',count,hit,data));n+=1
    for count,statuses in itertools.product([-1,0,1,3],[(4,4,4),(4,0,4),(0,0,0)]):
        execute(0x55fbd8,n=count,status=statuses,void=True);n+=1
    for mutation in ['status-count','status-tables']:
        execute(0x55fbd8,status=(4,4,0),mutation=mutation,void=True);n+=1
    for disabled,device,supported,env in itertools.product([0,1],[0,17],[0,1],[0,1,0xffffffff]):
        def setup(u):
            put(u,SELF+0x200,disabled);put(u,SELF+0x1e8,device);put(u,SELF+0x1ec,supported)
        execute(0x55fec8,[env],setup=setup,mutation='environment',void=True);n+=1
    print('PASS 3D defaults, free channel, cache lookup/users and EAX preference:',n,flush=True)
    return n

def loading():
    n=0
    for slot,data,result in itertools.product([-1,0,1,2],[0,0x70000],[0,0x12345678]):
        def setup(u):
            if slot>=0:put(u,C+slot*12+8,0);put(u,C+slot*12+4,data)
        execute(0x55fca4,[REQ,2],setup=setup,read_result=result,label=(slot,data,result));n+=1
    for mode in ['null-name','empty-name','disabled','no-count','negative-count']:
        def setup(u):
            if mode=='null-name':put(u,FILES+16,0)
            elif mode=='empty-name':byte(u,NAME+64,0)
            elif mode=='disabled':put(u,SELF+0x200,1)
            else:put(u,SELF+0x1e4,0 if mode=='no-count' else -1)
        execute(0x55fca4,[REQ,1],setup=setup,expected=-1,label=mode);n+=1
    for mutation in ['free','read','file']:
        execute(0x55fca4,[REQ,2],setup=lambda u:put(u,C+8,0),mutation=mutation);n+=1
    # Requests overlap output before, at and after its first field. The exact
    # copy order and resetting before the copies affect the resulting values.
    for shift,evict in itertools.product([-4,0,4],[False,True]):
        request=V+40+shift
        def setup(u):
            u.mem_write(request,struct.pack('<10I',77,2,91,64,3,0,0,11,12,13))
            put(u,C+8,0);put(u,C+4,0x70000 if evict else 0)
        execute(0x55fca4,[request,1],setup=setup,label=('alias',shift,evict));n+=1
    print('PASS 3D file loading, eviction, failures, reloads and request aliasing:',n,flush=True)
    return n

def playback():
    n=0
    for count,cached,duplicate in itertools.product([0,1,3],[False,True],[False,True]):
        def setup(u):
            if cached:put(u,C+12,2)
            if duplicate:put(u,V,77);put(u,V+4,2)
        execute(0x55ff20,[REQ],n=count,setup=setup,status=(4,0,0) if duplicate else (0,0,0),
                label=('composed',count,cached,duplicate));n+=1
    for file_id,count,disabled in [(-1,6,0),(6,6,0),(0,0,0),(0,6,1),(0,0x80000000,1)]:
        def setup(u):put(u,REQ+4,file_id);put(u,SELF+4,count);put(u,SELF+0x200,disabled)
        execute(0x55ff20,[REQ],setup=setup,expected=-1);n+=1
    for mutation in ['status-count','status-tables','free','read','file','loop','volume','position']:
        execute(0x55ff20,[REQ],mutation=mutation);n+=1
    execute(0x55ff20,[REQ],read_result=0,expected=-1);n+=1
    execute(0x55ff20,[REQ],status=(4,4,4),expected=-1);n+=1
    # Independent return boundaries, without requiring hundreds of real
    # active voices. Only these helper-boundary cases substitute the helpers.
    for channel,cache in itertools.product([-1,0,127,128,255,256],[-1,0,127,128,255,256]):
        values={0x55faf7:channel,0x55fbd8:0,0x55fb6f:cache,0x55fca4:2}
        execute(0x55ff20,[REQ],helper_results=values,label=('byte results',channel,cache));n+=1
    for loaded in [-1,0,127,128,255,256]:
        values={0x55faf7:1,0x55fbd8:0,0x55fb6f:-1,0x55fca4:loaded}
        execute(0x55ff20,[REQ],helper_results=values,label=('loaded byte',loaded));n+=1
    print('PASS 3D playback with real cache helpers, raw positions and signed-byte boundaries:',n,flush=True)
    return n

def spatial():
    n=0
    for token,index,disabled in itertools.product([0x560113,0x560150],[-128,-1,0,2,127,128,256],[0,1]):
        raw=[0x3f800000,0x80000000,0x7fc12345,0xc0200000,0x00000001,0x7f800000]
        args=[index]+raw[:3 if token==0x560113 else 6]
        execute(token,args,setup=lambda u:put(u,SELF+0x200,disabled),void=True,
                label=('spatial bits',index,disabled));n+=1
    for count,disabled,statuses in itertools.product([-1,0,1,3],[0,1],[(4,4,4),(4,0,4),(0,0,0)]):
        execute(0x560199,n=count,setup=lambda u:put(u,SELF+0x200,disabled),status=statuses,void=True);n+=1
    for mutation in ['status-count','status-tables','update']:
        execute(0x560199,status=(4,4,0),mutation=mutation,void=True);n+=1
    print('PASS 3D position/orientation raw bits and active-voice update reloads:',n,flush=True)
    return n

if __name__=='__main__':
    total=leaves()+loading()+playback()+spatial()
    print('PASS total:',total,'target SHA256',target_sha,flush=True)
