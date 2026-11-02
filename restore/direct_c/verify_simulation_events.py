"""Native/C simulation event dispatch and the191-byte effect packet writer.

No game is launched. Full enumeration and451540/44E6F0 execute. Effect queries
are controlled so packet writes and receiver/RETn contracts are observable.
Saved-state replay below uses the user's read-only autosave as a pre-crash
fixture, not a complete reconstruction of the later crash-time heap.
"""
from verify_options_startup import *
from unicorn import UC_HOOK_MEM_WRITE

for c,u,state in machines:u.mem_map(0x2fe00000,0x200000)


def enumeration(editor,count,mutate=False):
    outputs=[]
    for c,u,state in machines:
        reset(u,state);put(u,0x605a3c,editor);put(u,0x785d70,count);put(u,0x5d9a40,100)
        u.mem_write(0x785d74,bytes(150*124));calls=[]
        for i in range(count):
            word(u,0x785d74+i*124,i);byte(u,0x785d76+i*124,[0,1,2,255][i%4])
        def update(cpu,self):
            calls.append(self)
            if mutate:byte(cpu,self+2,0)
            return 0
        if not c:
            hook(c,state,0x451540,0,lambda cpu,args:update(cpu,cpu.reg_read(UC_X86_REG_ECX)))
        else:
            def member(cpu,args):
                f=args[0];sp=get(cpu,f+16);update(cpu,get(cpu,f+4));put(cpu,f+16,sp+4);return 0
            state['hooks'][sym('restore_event_C_00451540')]=(1,0,member)
        run(c,u,0x4464f0)
        outputs.append((calls,get(u,0x5d9a40),bytes(u.mem_read(0x785d70,18628))))
    assert outputs[0]==outputs[1],('event enumeration',editor,count,mutate,outputs[0][:2],outputs[1][:2])


trace=[]
for c,u,state in machines:
    capture={'packet':None,'writes':[],'calls':[],'record':None};trace.append(capture)
    def observe(cpu,pc,size,unused,c=c,state=state,capture=capture):
        expected=sym('restore_event_C_0044e6f0') if c else 0x44e6f0
        if pc!=expected:return
        if c:
            f=get(cpu,cpu.reg_read(UC_X86_REG_ESP)+4);self=get(cpu,f+4);sp=get(cpu,f+16)
        else:self=cpu.reg_read(UC_X86_REG_ECX);sp=cpu.reg_read(UC_X86_REG_ESP)
        assert self==capture['record'],('lost event receiver',c,hex(self),capture['record'])
        capture['packet']=get(cpu,sp+4)
    def writes(cpu,access,at,size,value,unused,capture=capture):
        p=capture['packet']
        if p is not None and p<=at< p+30:capture['writes'].append((at-p,size,value&((1<<(size*8))-1)))
    u.hook_add(UC_HOOK_CODE,observe)
    u.hook_add(UC_HOOK_MEM_WRITE,writes)


def install(c,u,state,record,extension,stage):
    capture=trace[c];capture.update(packet=None,writes=[],calls=[],record=record)
    desc={0x4449d0:(1,0),0x44d750:(0,0),0x44d700:(0,0),0x44ef60:(0,0),0x44c0d0:(1,4)}
    def value(cpu,t,args,self):
        if t!=0x4449d0:assert self==record,('query receiver',hex(t),c,hex(self),hex(record))
        capture['calls'].append((t,args,self if t!=0x4449d0 else 0))
        if t==0x44d700:capture['packet']=None
        return (args[0]*5+0x1234)&0xffffffff if t==0x4449d0 else stage if t==0x44d750 else extension if t==0x44c0d0 else 0
    if not c:
        for t,(n,cleanup) in desc.items():
            def native(cpu,args,t=t):
                result=value(cpu,t,args,cpu.reg_read(UC_X86_REG_ECX))
                cpu.reg_write(UC_X86_REG_ECX,0xdeadbeef);cpu.reg_write(UC_X86_REG_EDX,0xbaadf00d);return result
            hook(c,state,t,n,native,cleanup=cleanup)
    else:
        def raw_query(cpu,args):
            t,regs,flags=args;assert t in desc,(hex(t),'unexpected resource query')
            n,cleanup=desc[t];sp=get(cpu,regs+16);argv=tuple(get(cpu,sp+4+i*4) for i in range(n))
            result=value(cpu,t,argv,get(cpu,regs+4));put(cpu,regs,result)
            put(cpu,regs+4,0xdeadbeef);put(cpu,regs+8,0xbaadf00d);put(cpu,regs+16,sp+4+cleanup);return 0
        state['hooks'][sym('restore_resource_render_raw')]=(3,0,raw_query)
        for t in [0x44ef60,0x44c0d0]:
            def member(cpu,args,t=t):
                f=args[0];n,cleanup=desc[t];sp=get(cpu,f+16);argv=tuple(get(cpu,sp+4+i*4) for i in range(n))
                result=value(cpu,t,argv,get(cpu,f+4));put(cpu,f,result)
                put(cpu,f+4,0xdeadbeef);put(cpu,f+8,0xbaadf00d);put(cpu,f+16,sp+4+cleanup);return 0
            state['hooks'][sym('restore_event_C_%08x'%t)]=(1,0,member)


def effect(kind,extension,stage,override):
    outputs=[];record=0x785f64
    for c,u,state in machines:
        reset(u,state);u.mem_write(0x785d74,bytes(150*124));put(u,0x785d70,5);put(u,0x605a3c,0)
        word(u,record,4);byte(u,record+2,2);word(u,record+4,kind)
        word(u,record+12,345);word(u,record+0x1c,77);word(u,record+0x2c,456)
        word(u,record+0x34,-1);byte(u,record+0x36,0xab);byte(u,record+0x37,0xcd)
        byte(u,record+0x63,143 if override else 0);word(u,record+0x64,7 if override else -1)
        word(u,record+0x66,-321);word(u,record+0x68,678);put(u,record+0x28,0)
        install(c,u,state,record,extension,stage)
        run(c,u,0x4464f0,budget=5000000)
        outputs.append((trace[c]['calls'],trace[c]['writes'],get(u,0x5d9a40)))
    assert outputs[0]==outputs[1],('event packet through actual dispatcher',kind,extension,stage,override,outputs)
    assert outputs[1][1],('packet was not generated',kind)


def saved_dispatch():
    from replay_saved_actor_tick import saved_regions
    regions=saved_regions(ROOT/'build/debug-startup-game/Save/Restore/autosave.sav')
    outputs=[]
    for c,u,state in machines:
        reset(u,state)
        for at,payload in regions:u.mem_write(at,payload)
        put(u,0x605a3c,0)
        records=[0x785d74+i*124 for i in range(get(u,0x785d70)) if u.mem_read(0x785d76+i*124,1)==b'\x02']
        # This fixture isolates the first active record from the real pre-crash
        # save and still performs the genuine iterator and packet generation.
        assert records
        record=records[0]
        for p in records[1:]:byte(u,p+2,0)
        install(c,u,state,record,0,0)
        run(c,u,0x4464f0,budget=5000000)
        outputs.append((trace[c]['calls'],trace[c]['writes'],bytes(u.mem_read(0x785d70,18628))))
    assert outputs[0]==outputs[1],('saved event dispatch differs',outputs[0][:2],outputs[1][:2])
    print('PASS native/C dispatch and packet writer from read-only pre-crash autosave, first active event:',hex(record),flush=True)


if __name__=='__main__':
    count=0
    for editor,n,mutate in itertools.product([0,1,256,257],[0,1,6,150],[False,True]):
        enumeration(editor,n,mutate);count+=1
    print('PASS whole event enumeration, DWORD editor guard, receiver binding and live record status:',count,flush=True)
    count=0
    for kind,ext,stage,override in itertools.product([1,2,3,4],[0,1],[0,7],[False,True]):
        effect(kind,ext,stage,override);count+=1
    print('PASS451540 plus real44E6F0, all record types, extension fields and ordered packet writes:',count,flush=True)
    saved_dispatch()
    print('Verified target SHA256:',target_sha)
