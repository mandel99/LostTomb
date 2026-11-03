"""Twenty whole file-list lifetime entries, callbacks and global CRT objects.

Heap free and exit registration are controlled platform boundaries. Constructors,
destructors, deleting destructors and original atexit-result conversion execute.
Compare object reads/writes, callback-sensitive reloads, registration, full defined
EAX, legacy ECX/EBP inputs, stack cleanup and saved registers. No host game.
"""
from verify_options_startup import *
import json
from unicorn import UC_HOOK_MEM_READ,UC_HOOK_MEM_WRITE,UC_MEM_WRITE
from capstone import Cs,CS_ARCH_X86,CS_MODE_32,CS_GRP_JUMP

Q=S+0x1000
OBJECTS=[0x6057c8,0x704c00,0xeadfa8]
GROUPS={0x402360:(0x402370,0x402380,0x402390,OBJECTS[0]),
        0x41e050:(0x41e060,0x41e070,0x41e080,OBJECTS[1]),
        0x516570:(0x516580,0x516590,0x5165a0,OBJECTS[2])}
SIZES={0x41c6d0:29,0x41c6f0:30,0x41c710:43,0x41e090:5,0x41e0a0:18,0x41e0c0:30,
       0x56b690:11,0x56b750:11}
for startup,(ctor,reg,fini,obj) in GROUPS.items():SIZES.update({startup:10,ctor:10,reg:12,fini:10})
NAMES={0x41c6d0:'construct',0x41e0a0:'construct_mission',0x41c710:'destroy',
       0x41e090:'destroy',0x41c6f0:'delete',0x41e0c0:'delete',0x56b690:'destroy_frame',0x56b750:'destroy_frame'}
DELETING={0x41c6f0,0x41e0c0};FRAMES={0x56b690:0x12c,0x56b750:0x230}
CTORS={0x41c6d0,0x41e0a0}|{v[0] for v in GROUPS.values()}
STATUS=set(GROUPS)|{v[1] for v in GROUPS.values()}
available=lambda name:bool(re.search(r'\s_'+re.escape(name)+r'\s',symbols))
legacy=[a for a in SIZES if available('FUN_%08x'%a)]
supported={a for a in SIZES if a in legacy or (a in NAMES and available('restore_file_list_'+NAMES[a]))}
if '--require-all' in sys.argv:assert len(legacy)==20,legacy
decoder=Cs(CS_ARCH_X86,CS_MODE_32);decoder.detail=True
owned={};branches={};hits=set();edges=set();previous=[None];invocations=0
for at,size in SIZES.items():
    decoded=list(decoder.disasm(bytes(images[0].data[at-images[0].base:at-images[0].base+size]),at))
    assert sum(x.size for x in decoded)==size
    owned[at]={x.address for x in decoded}
    for ins in decoded:
        if ins.group(CS_GRP_JUMP) and ins.mnemonic!='jmp':branches[ins.address]={ins.address+ins.size,ins.operands[0].imm}
for c,u,state in machines:
    reset(u,state);state['self']=Q;state['events']=[]
    def memory(cpu,access,at,size,value,data):
        watched=any(a<=at<a+28 for a in [data['self'],*OBJECTS])
        if watched:data['events'].append(('write',at,size,value) if access==UC_MEM_WRITE else ('read',at,size))
        elif access==UC_MEM_WRITE and 0x560000<=at<0x126d000:raise AssertionError(('unexpected global write',hex(at),size))
    u.hook_add(UC_HOOK_MEM_READ|UC_HOOK_MEM_WRITE,memory,state)
    if not c:
        def trace(cpu,pc,size,data):
            if previous[0] in branches:edges.add((previous[0],pc))
            if any(pc in x for x in owned.values()):hits.add(pc)
            previous[0]=pc
        u.hook_add(UC_HOOK_CODE,trace)
baseline=[snapshot(u) for c,u,state in machines]

def object_fixture(u,obj,pointers,seed):
    u.mem_write(obj-8,bytes(((seed+i*37)&255) for i in range(44)))
    offsets=S+0x4000 if pointers&1 else 0
    strings=S+0x3000 if pointers&2 else 0
    put(u,obj+4,offsets);put(u,obj+16,strings)
    return offsets,strings

def setup(c,u,state,obj,pointers,mutation,success,seed):
    state['self']=obj;state['events']=[];state['registered']=[];state['frees']=[]
    state['mutation']=mutation
    for p in OBJECTS:object_fixture(u,p,pointers,seed)
    _,strings=object_fixture(u,obj,pointers,seed)
    state['strings']=strings
    state['hooks'].clear()
    def free(cpu,args):
        allocation=args[0];state['frees'].append(allocation);state['events'].append(('free',allocation))
        if len(state['frees'])==1 and state['strings'] and allocation==state['strings']:
            if mutation==1:put(cpu,obj+4,S+0x5000)
            elif mutation==2:put(cpu,obj+4,0)
            elif mutation==3:put(cpu,obj,0xdeadbeef);put(cpu,obj+8,0xabcdef01)
            elif mutation==4:put(cpu,obj+16,S+0x6000)
        cpu.reg_write(UC_X86_REG_ECX,0x789abcde);cpu.reg_write(UC_X86_REG_EDX,0x6789abcd)
        return 0xbadc0de
    if not c:hook(c,state,0x561a2b,1,free)
    else:state['hooks'][sym('restore_heap_free')]=(1,0,free)
    def register(cpu,args):
        address=args[0]
        candidates={sym('FUN_%08x'%v[2]) if c else v[2]:v[2] for v in GROUPS.values()}
        assert address in candidates,('unexpected exit callback',c,args)
        logical=candidates[address];state['events'].append(('register',logical))
        if success:state['registered'].append(logical)
        return (address if success else 0) if not c else (0 if success else 0xffffffff)
    if not c:hook(c,state,0x5618e6,1,register)
    else:state['hooks'][sym('atexit')]=(1,0,register)

def call(c,u,token,obj,args=(),mode='legacy'):
    global invocations
    invocations+=1
    frame=obj+FRAMES[token] if token in FRAMES else saved[1]
    if mode=='named':
        entry=sym('restore_file_list_'+NAMES[token]);argv=[frame,FRAMES[token]] if token in FRAMES else [obj,*args]
    else:entry=address(c,token);argv=args
    for reg,value in zip(registers,[0x11111111,obj,0x33333333,saved[0],SP,frame,saved[2],saved[3]]):u.reg_write(reg,value)
    u.reg_write(UC_X86_REG_EFLAGS,2);put(u,SP,STOP)
    for i,v in enumerate(argv):put(u,SP+4+i*4,v)
    u.emu_start(entry,STOP,count=2000000)
    assert u.reg_read(UC_X86_REG_EIP)==STOP,(c,hex(token),mode)
    assert u.reg_read(UC_X86_REG_ESP)==SP+4+(4 if not c and token in DELETING else 0)
    assert [u.reg_read(registers[i]) for i in [3,5,6,7]]==[saved[0],frame,saved[2],saved[3]]
    return u.reg_read(UC_X86_REG_EAX) if token in CTORS|DELETING|STATUS else None

def exercise(token,pointers=3,flags=0,mutation=0,success=True,seed=0xa5,alignment=0):
    if token not in supported:return 0
    obj=next((v[3] for k,v in GROUPS.items() if token in [k,*v[:3]]),Q+alignment)
    expected=None
    for c,u,state in machines:
        modes=['legacy'] if not c or token not in NAMES else (['legacy'] if token in legacy else [])+(['named'] if available('restore_file_list_'+NAMES[token]) else [])
        for mode in modes:
            setup(c,u,state,obj,pointers,mutation,success,seed)
            result=call(c,u,token,obj,[flags] if token in DELETING else [],mode)
            current=(result,tuple(state['events']),tuple(bytes(u.mem_read(p-8,44)) for p in [obj,*OBJECTS]))
            if expected is None:expected=current
            else:assert current==expected,('file lifetime',hex(token),pointers,flags,mutation,success,seed,alignment,mode,current,expected)
    return 1

if __name__=='__main__':
    n=0
    for token,alignment,seed in itertools.product([0x41c6d0,0x41e0a0],range(4),[0,0xa5,0xff]):
        n+=exercise(token,alignment=alignment,seed=seed)
    for token,pointers,mutation,alignment in itertools.product([0x41c710,0x41e090],range(4),range(5),[0,3]):
        n+=exercise(token,pointers=pointers,mutation=mutation,alignment=alignment)
    for token,flags,pointers,mutation,alignment in itertools.product(DELETING,[0,1,2,3,0x100,0x101,0x80000000,0xffffffff],range(4),[0,1],[0,3]):
        n+=exercise(token,flags=flags,pointers=pointers,mutation=mutation,alignment=alignment)
    for token,pointers,mutation in itertools.product(FRAMES,range(4),[0,1,2]):
        n+=exercise(token,pointers=pointers,mutation=mutation)
    for ctor,reg,fini,obj in GROUPS.values():
        for seed in [0,0xa5,0xff]:n+=exercise(ctor,seed=seed)
        for pointers,mutation in itertools.product(range(4),[0,1,2]):n+=exercise(fini,pointers=pointers,mutation=mutation)
        for success in [False,True]:n+=exercise(reg,success=success)
    for startup,success,seed in itertools.product(GROUPS,[False,True],[0,0xa5]):
        n+=exercise(startup,success=success,seed=seed)
    # Three CRT objects together, followed by their actual registered C/native
    # callbacks in exit-list order. Populate owned slots before destruction.
    for mutation in [0,1,2]:
        expected=None
        for c,u,state in machines:
            setup(c,u,state,Q,3,mutation,True,0xa5)
            for startup in GROUPS:call(c,u,startup,GROUPS[startup][3])
            for obj in OBJECTS:object_fixture(u,obj,3,0x37)
            for fini in reversed(state['registered']):
                obj=next(v[3] for v in GROUPS.values() if v[2]==fini)
                state['self']=obj;state['frees']=[];state['strings']=S+0x3000
                call(c,u,fini,obj)
            observed=(tuple(state['events']),tuple(bytes(u.mem_read(p-8,44)) for p in OBJECTS))
            if expected is None:expected=observed
            else:assert observed==expected,('combined CRT lifecycle',mutation,observed,expected)
        n+=1
    expected_points=set.union(*(owned[a] for a in supported))
    checked_branches={a:targets for a,targets in branches.items() if a in expected_points}
    missing=sorted(expected_points-hits)
    missing_edges=sorted((a,b) for a,targets in checked_branches.items() for b in targets if (a,b) not in edges)
    assert not missing,('native instruction coverage',[hex(a) for a in missing])
    assert not missing_edges,('native edge coverage',missing_edges)
    for i,(c,u,state) in enumerate(machines):
        for obj in OBJECTS:u.mem_write(obj-8,baseline[i][obj-8-0x560000:obj+36-0x560000])
        assert snapshot(u)==baseline[i],('unrelated global mutation',c)
    unit_ids=[18,19,266,267,268,288,289,290,291,292,2274,2275,3091,3099]
    sources=['runtime/file_list_lifecycle.c','include/file_list.h','runtime/standalone_link_check.c','CMakeLists.txt',
             'direct_c/verify_file_list_lifecycle.py']+[f'src/unit_{i:05d}.c' for i in unit_ids]
    proof=dict(native_sha256=hashlib.sha256(original.read_bytes()).hexdigest(),compiled_C_path=str(target.resolve()),
        compiled_C_sha256=target_sha,source_hashes={p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest() for p in sources},
        passed=True,checked=n,invocations=invocations,legacy_entries_tested=[hex(a) for a in legacy],
        methods_compared=[hex(a) for a in SIZES if a in supported],not_linked_in_target=[hex(a) for a in SIZES if a not in supported],
        coverage={hex(a):dict(bytes=SIZES[a],instructions=len(x),executed=len(x&hits)) for a,x in owned.items()},
        native_conditional_edges=sum(map(len,checked_branches.values())),controlled_algorithm_endpoints=[],
        controlled_platform_endpoints=['Native heap-free561A2B / hosted C restore_heap_free','Native_onexit5618E6 / hosted C atexit'],
        scope=['Lifetime methods and legacy entries actually compared are listed separately; diagnostic requires all20, game may discard unused code',
               'Exact object read/write order, post-free pointer reload, nullable allocation slots, flags, unaligned objects and normal-return ABI'],
        limitations=['Heap and exit-list mechanics are platform endpoints; heap safety and C++ exception dispatch/unwinding are not certified.',
                     'Caller-stack/output aliasing, concurrent mutation and full gameplay remain outside scope.'],commit=None)
    suffix='diagnostic' if target.stem=='restore_link_check' else 'game'
    (ROOT/f'metadata/file-list-lifecycle-{suffix}.json').write_text(json.dumps(proof,indent=2)+'\n')
    print('PASS file-list lifetime entries, callback-sensitive frees, EBP cleanup and CRT globals:',n,flush=True)
    print('Native instruction/conditional-edge coverage:',len(hits),sum(map(len,checked_branches.values())),flush=True)
    print('Verified target SHA256:',target_sha,flush=True)
