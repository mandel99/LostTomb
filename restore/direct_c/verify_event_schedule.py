"""Whole native/C scheduling and rearm with genuine event/RNG descendants.

No game is launched. Only the diagnostic-file sink is captured for the full
event-pool failure; gameplay queries, copying, reset and callbacks execute.
"""
from verify_event_packet import *
from unicorn import UC_HOOK_MEM_WRITE

L=S+0x5000
NAMES={0x44bc20:'restore_event_rearm_completed',0x44c430:'restore_event_dispatch_deferred',
       0x44c490:'restore_event_schedule_0044c490',0x44c960:'restore_event_parameter_count'}
RETURNS={0x44c430,0x44c960};RET={0x44bc20:0,0x44c430:4,0x44c490:8,0x44c960:0}
legacy={a for a in NAMES if available('FUN_%08x'%a)}
if '--require-all' in sys.argv:assert legacy==set(NAMES),legacy
catalogue=json.loads((ROOT/'metadata/event-schedule-native.json').read_text())
assert catalogue['native_sha256']==hashlib.sha256(original.read_bytes()).hexdigest()
owned={};branches={};hits=set();edges=set();previous=[None];checked=0;invocations=0
for f in catalogue['functions']:
    a=int(f['address'],16);n=f['result']['procedure']['body']['total_bytes']
    code=list(Cs(CS_ARCH_X86,CS_MODE_32).disasm(bytes(images[0].data[a-images[0].base:a-images[0].base+n]),a))
    assert sum(i.size for i in code)==n;owned[a]={i.address for i in code}
    branches.update({i.address:{int(i.op_str,16),i.address+i.size} for i in code if i.mnemonic.startswith('j') and i.mnemonic!='jmp'})
points=set.union(*owned.values())
for c,u,state in machines:
    u.mem_map(0xfffff000,4096);state['ordered']=[]
    def writes(cpu,access,at,size,value,data):
        if 0x560000<=at<0x126d000 or S<=at<S+0x10000:
            data['ordered'].append((at,size,value&((1<<(size*8))-1)))
    u.hook_add(UC_HOOK_MEM_WRITE,writes,state)
    if not c:
        def observe(cpu,pc,size,data):
            if pc in points:hits.add(pc)
            if previous[0] in branches:edges.add((previous[0],pc))
            previous[0]=pc
        u.hook_add(UC_HOOK_CODE,observe,state)

def call(c,u,token,obj,args,mode,eax):
    global invocations
    invocations+=1
    if not c:entry=token;argv=list(args);cleanup=RET[token]
    elif mode=='legacy':entry=address(c,token);argv=list(args);cleanup=0
    else:
        entry=sym(NAMES[token]);argv=[obj,*args];cleanup=0
        if token==0x44c430:argv.append(eax)
    for r,v in zip(registers,[eax,obj,0x98765432,saved[0],SP,*saved[1:]]):u.reg_write(r,v)
    u.reg_write(UC_X86_REG_EFLAGS,0x202);put(u,SP,STOP)
    for i,v in enumerate(argv):put(u,SP+4+i*4,v)
    try:u.emu_start(entry,STOP,count=10000000)
    except Exception as error:raise AssertionError(('execution',c,hex(token),mode,hex(u.reg_read(UC_X86_REG_EIP)))) from error
    assert u.reg_read(UC_X86_REG_EIP)==STOP,(c,hex(token),mode,hex(u.reg_read(UC_X86_REG_EIP)))
    assert u.reg_read(UC_X86_REG_ESP)==SP+4+cleanup,(c,hex(token),mode,'SP')
    assert [u.reg_read(registers[i]) for i in [3,5,6,7]]==saved
    return u.reg_read(UC_X86_REG_EAX) if token in RETURNS else None

def compare(token,prepare,obj=Q,args=(),full=False,label=None,eax=0xabcdef01):
    global checked
    expected=None
    for c,u,state in machines:
        for mode in (['named','legacy'] if c and token in legacy else ['named']):
            reset(u,state);prepare(u)
            def diagnostic(cpu,argv):state['calls'].append(('diagnostic',argv));return 0
            hook(c,state,0x4ce6e0,3,diagnostic)
            state.update(ordered=[],packets=[])
            value=call(c,u,token,obj,args,mode,eax)
            memory=(snapshot(u),bytes(u.mem_read(S,0x10000))) if full else (bytes(u.mem_read(Q,160)),get(u,0x5d9a40),bytes(u.mem_read(0x78a634,1)))
            out=(value,tuple(state['ordered']),tuple(state['calls']),tuple(state['packets']),memory)
            if expected is None:expected=out
            elif out!=expected:
                diff=[]
                if full:diff=[(hex(0x560000+i),a,b) for i,(a,b) in enumerate(zip(out[-1][0],expected[-1][0])) if a!=b][:12]
                wi=next(((i,a,b) for i,(a,b) in enumerate(zip(out[1],expected[1])) if a!=b),None)
                raise AssertionError(('schedule comparison',hex(token),hex(obj),args,label,mode,
                                      'returns',out[0],expected[0],'write',wi,'write counts',len(out[1]),len(expected[1]),'state',diff,'calls',out[2],expected[2]))
    checked+=1

def query_cases():
    for first,second,third in itertools.product([-32768,-1,0,1,32767],repeat=3):
        def prepare(u):
            for off,value in [(2,first),(4,second),(6,third)]:word(u,Q+off,value)
        compare(0x44c960,prepare,label=(first,second,third))
    for count,index,current,phase,kind in itertools.product([0,3,0x80000000],[-300,-1,0,2,3,255],[-1,0,2,256],[0,1,255],[1,2]):
        def prepare(u):
            put(u,Q,count);put(u,0x5d9a40,current);byte(u,0x78a634,phase)
            for i in range(3):byte(u,Q+4+i*124+2,kind)
        compare(0x44c430,prepare,args=[index],label=(count,index,current,phase,kind))
    def null_pointer(u):put(u,0xfffffffc,1);put(u,0x5d9a40,0)
    compare(0x44c430,null_pointer,obj=0xfffffffc,args=[0],label='wrapped null record')
    print('PASS complete parameter count and full-EAX deferred-dispatch predicate:',checked,flush=True)

def rearm_cases():
    for flags,month,hold,count in itertools.product([0,4,6,20,22,0x80000016],[0,1,2,255],[0,1],[0,1,3]):
        def prepare(u):
            action_fixture(u,0,0,0,21,0,6,0,False);put(u,L,count)
            for i in range(count):
                at=L+4+i*124;event(u,at,i,3 if i==1 else 21)
                put(u,at+0x28,flags);byte(u,at+3,month);byte(u,at+0x32,hold);byte(u,at+0x36,month&1)
        compare(0x44bc20,prepare,obj=L,full=True,label=(flags,month,hold,count))
    for count in [-1,300]:
        def prepare(u):
            put(u,L,count)
            if count>0:u.mem_write(L+4,bytes(count*124))
        compare(0x44bc20,prepare,obj=L,full=True,label=('DWORD count',count))
    print('PASS recurring/completed event rearm, countdown wrap and live DWORD list count:',checked,flush=True)

def group(u,at,mode):
    choices={'one':(5,-1,-1),'many':(5,6,-1),'range':(-1,3,8)}[mode]
    for i,v in enumerate(choices):word(u,at+2+i*2,v)

def scheduled(template_kind=13,parent_kind=13,commodity=5,subtype=5,resource_mode='range',subtype_mode='range',
              delay=6,month=4,year=-3490,base=0x1234f254,reason=0x89abcdef,template_id=1,current=-1,phase=0,
              reuse=-1,self_alias=False,full=False,editor=0):
    obj=0x785d74+reuse*124 if self_alias else Q
    def prepare(u):
        action_fixture(u,editor,0,0,parent_kind,0,6,0,False)
        put(u,0x785d70,140 if full else 2)
        if full:
            for i in range(140):put(u,0x785d74+i*124+0x28,1)
        at=0x785d74+124;event(u,at,1,template_kind,delay=max(delay,0))
        # Type2's actual callback checks its eligibility window after building
        # the packet. A closed window exercises that real path without needing
        # an initialized invasion/army world for this scheduler-only fixture.
        if template_kind==2:word(u,at+0x2c,-1)
        group(u,at+0x1c,resource_mode);group(u,at+4,subtype_mode)
        if delay<0:
            word(u,at+0x16,-1);word(u,at+0x18,delay);word(u,at+0x1a,0)
        if reuse>=0:
            r=0x785d74+reuse*124
            if reuse!=1:event(u,r,reuse,21,delay=1)
            put(u,r+0x28,12);word(u,r+0x14,1)
        if not self_alias:
            word(u,Q,0);byte(u,Q+2,parent_kind);word(u,Q+0x1c,commodity);word(u,Q+4,subtype);word(u,Q+12,-321)
        put(u,0x604950,1);put(u,0xd3b384,0);put(u,0xd3b380,0x87654321)
        put(u,0x78d7c0,month);put(u,0x78d7bc,year);put(u,0x7848d8,base)
        put(u,0x5d9a40,current);byte(u,0x78a634,phase)
    label=(template_kind,parent_kind,commodity,subtype,resource_mode,subtype_mode,delay,month,reason,template_id,current,phase,reuse,self_alias,full,editor)
    compare(0x44c490,prepare,obj=obj,args=[template_id,reason],full=True,label=label)

def scheduler_cases():
    for template_id in [-32768,-1,2,32767]:scheduled(template_id=template_id)
    scheduled(full=True) # genuine append failure; diagnostic sink only
    for kind,parent,rmode,smode in itertools.product([13,15,21],[13,15,21],['one','many','range'],['one','many','range']):
        scheduled(template_kind=kind,parent_kind=parent,resource_mode=rmode,subtype_mode=smode)
    for commodity,subtype in itertools.product([-1,2,3,5,8,9],[0,2,3,5,8,9,21,27,36]):scheduled(commodity=commodity,subtype=subtype)
    for delay,month in itertools.product([-32768,-13,-1,0,1,11,12,13,32767],[0,11,0x7fffffff,0x80000000,0xffffffff]):
        scheduled(template_kind=21,parent_kind=21,delay=delay,month=month,editor=1)
    for kind,current,phase in itertools.product([2,21,22],[-1,0,10],[0,1]):
        scheduled(template_kind=kind,parent_kind=21,delay=0,current=current,phase=phase,editor=1)
    for reuse,alias in itertools.product([0,1],[False,True]):scheduled(reuse=reuse,self_alias=alias)
    for reason in [0,0x7fff,0x8000,0xffffffff]:scheduled(reason=reason)
    print('PASS full scheduler, resource/subtype inheritance, date wrapping, immediate callbacks and reused slots:',checked,flush=True)

if __name__=='__main__':
    query_cases();rearm_cases();scheduler_cases()
    # current>=0 and current<=index before this test imply index>=0.
    unreachable={(0x44c44e,0x44c463)}
    assert points<=hits,('unexecuted instructions',[hex(a) for a in sorted(points-hits)])
    missing={(a,b) for a,targets in branches.items() for b in targets if (a,b) not in edges}-unreachable
    assert not missing,('unexecuted edges',[(hex(a),hex(b)) for a,b in sorted(missing)])
    import verify_event_packet as packets
    composed=0
    for kind,links,delay in itertools.product([21,22],[0,3,59],[0,6]):
        packets.action(kind=kind,links=links,delay=delay,dispatch=True,enumerate_events=True);composed+=1
    # current-index=-1 makes these chains recursively dispatch immediately,
    # rather than append records for the live outer iterator to visit later.
    for kind in [21,22]:packets.action(kind=kind,links=59,delay=0,dispatch=True)
    sources=['include/event_schedule.h','runtime/event_schedule.c','runtime/menu_event_actions.c','runtime/menu_event_model.c',
             'include/menu_event_actions.h','CMakeLists.txt','direct_c/verify_event_schedule.py']+[f'src/unit_{i:05d}.c' for i in [721,731,732,737]]
    proof=dict(native_sha256=catalogue['native_sha256'],compiled_C_path=str(target.resolve()),compiled_C_sha256=target_sha,
               passed=True,checked=checked,invocations=invocations,composed_iterator_cases=composed,recursive_dispatch_cases=2,
               source_hashes={p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest() for p in sources},
               coverage={hex(a):dict(instructions=len(x),executed=len(x&hits)) for a,x in owned.items()},
               conditional_edges=len(edges),unreachable_edges=[(hex(a),hex(b)) for a,b in unreachable],
               controlled_algorithm_endpoints=[],controlled_platform_endpoints=['4CE6E0 diagnostic-file sink on full event-pool failure'],
               limitations=['Mapped nonfaulting parameter/record fixtures and private lists up to300 records; no concurrency.',
                            'Void scheduler/rearm compare state, ordered writes and saved registers; unused volatile outputs and copied scratch argument slots are not logical outputs.',
                            'Immediate type2 callback is composed with a closed eligibility window; full request/invasion simulation is outside this proof.',
                            'Diagnostic sink buffers, timestamps and host files are outside the controlled logging boundary.',
                            'Other event/mission functions are not approved by this comparison.'],commit=None)
    suffix='diagnostic' if target.stem=='restore_link_check' else 'game'
    (ROOT/f'metadata/event-schedule-{suffix}.json').write_text(json.dumps(proof,indent=2)+'\n')
    print('PASS four complete event scheduling methods:',checked,'cases;',composed,'whole iterator compositions;2 recursive59-link chains',flush=True)
    print('Native instructions/conditional edges:',len(hits&points),len(edges),flush=True)
    print('Verified target SHA256:',target_sha,flush=True)
