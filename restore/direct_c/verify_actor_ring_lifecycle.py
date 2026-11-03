"""Six complete ring lifecycle entries and their real native vector iterators.

Only registration in the process exit list is controlled. Original constructors,
destructors, normal SEH setup/finally paths, array order and clear all execute.
No native game process, resource writes or exception-unwinding claim.
"""
from verify_options_startup import *
import json
from capstone import Cs,CS_ARCH_X86,CS_MODE_32

BASE,ARRAY_SIZE=0xeaaf68,50*224
Q=S+0x1000
GDT,TEB=0x34000000,0x34001000
SIZES={0x4f64a0:10,0x4f64b0:28,0x4f64d0:12,0x4f64e0:23,0x4f6500:13,0x4f6510:5}
MEMBERS={0x4f6500:'construct',0x4f6510:'destroy'}
available=lambda name:bool(re.search(r'\s_'+re.escape(name)+r'\s',symbols))
legacy=[a for a in SIZES if available('FUN_%08x'%a)]
if '--require-all' in sys.argv:assert len(legacy)==6,legacy
decoder=Cs(CS_ARCH_X86,CS_MODE_32)
owned={}
for a,n in SIZES.items():
    code=list(decoder.disasm(bytes(images[0].data[a-images[0].base:a-images[0].base+n]),a))
    assert sum(i.size for i in code)==n
    owned[a]={i.address for i in code}
visited=set();calls=0
for c,u,state in machines:
    u.mem_map(GDT,0x2000)
    for selector,access in [(8,0x9b),(16,0x93)]:
        u.mem_write(GDT+selector,struct.pack('<HHBBBB',0xffff,0,0,access,0xcf,0))
    u.mem_write(GDT+24,struct.pack('<HHBBBB',0xfff,TEB&65535,(TEB>>16)&255,0x93,0x40,(TEB>>24)&255))
    u.reg_write(UC_X86_REG_GDTR,(0,GDT,0x3f,0));u.reg_write(UC_X86_REG_CS,8)
    for reg in [UC_X86_REG_DS,UC_X86_REG_ES,UC_X86_REG_SS]:u.reg_write(reg,16)
    u.reg_write(UC_X86_REG_FS,24)
    state['events']=[]
    def writes(cpu,access,at,size,value,data):
        if BASE<=at<BASE+ARRAY_SIZE or Q<=at<Q+256:
            data['events'].append(('write',at,size,value))
    from unicorn import UC_HOOK_MEM_WRITE
    u.hook_add(UC_HOOK_MEM_WRITE,writes,state)
    if not c:
        def visit(cpu,pc,size,data):
            if any(pc in x for x in owned.values()):visited.add(pc)
        u.hook_add(UC_HOOK_CODE,visit)

def setup(c,u,state,seed,success):
    reset(u,state);put(u,TEB,0xfeedbabe)
    state['events']=[];state['registered']=[]
    u.mem_write(BASE-16,bytes(((i*37+seed)&255) for i in range(ARRAY_SIZE+32)))
    u.mem_write(S,b'\x5a'*0x3000)
    def register(cpu,args):
        expected=sym('FUN_004f64e0') if c else 0x4f64e0
        assert args==(expected,),('registered destructor',c,args,hex(expected))
        state['events'].append(('register',0x4f64e0))
        if success:state['registered'].append(expected)
        # Native atexit executes its genuine _onexit-result conversion.
        return (expected if success else 0) if not c else (0 if success else 0xffffffff)
    if not c:hook(c,state,0x5618e6,1,register)
    else:state['hooks'][sym('atexit')]=(1,0,register)

def invoke(c,u,token,mode='legacy',receiver=Q):
    global calls
    calls+=1
    if mode=='named':value=run(0,u,sym('restore_actor_ring_'+MEMBERS[token]),[receiver])
    else:value=run(c,u,token,receiver=receiver)
    assert get(u,TEB)==0xfeedbabe,('SEH chain not restored',c,hex(token))
    return value if token in [0x4f64a0,0x4f64d0,0x4f6500,0x4f6510] else None

def exercise(token,seed=0xa5,success=True,alignment=0,repeat=1,finish=False):
    expected=None
    for c,u,state in machines:
        modes=['legacy'] if not c or token not in MEMBERS else (['legacy'] if token in legacy else [])+['named']
        for mode in modes:
            setup(c,u,state,seed,success)
            values=[]
            for _ in range(repeat):values.append(invoke(c,u,token,mode,Q+alignment))
            if finish:
                # Active queues at termination: preserve owner/capacity/data.
                recorded=len(state['events'])
                for i in range(50):
                    put(u,BASE+i*224,1);put(u,BASE+i*224+4,i);put(u,BASE+i*224+8,49-i)
                # Fixture writes are not part of the observed algorithm.
                del state['events'][recorded:]
                for callback in reversed(state['registered']):
                    invoke(c,u,0x4f64e0)
            current=(values,tuple(state['events']),bytes(u.mem_read(S,0x3000)),snapshot(u))
            if expected is None:expected=current
            else:assert current==expected,('ring lifecycle',hex(token),seed,success,alignment,repeat,finish,mode,
                current[:2],expected[:2])

if __name__=='__main__':
    n=0
    for token,seed,alignment in itertools.product(MEMBERS,[0,0xa5,0xff],range(4)):
        exercise(token,seed=seed,alignment=alignment);n+=1
    for token,seed in itertools.product([0x4f64b0,0x4f64e0],[0,1,0x55,0xa5,0xff]):
        exercise(token,seed=seed);n+=1
    for token,seed,success in itertools.product([0x4f64a0,0x4f64d0],[0,0xa5,0xff],[False,True]):
        exercise(token,seed=seed,success=success);n+=1
    for token,repeat,success in itertools.product([0x4f64a0,0x4f64d0],[1,2,3],[False,True]):
        exercise(token,repeat=repeat,success=success,finish=True);n+=1
    missing=sorted(set.union(*owned.values())-visited)
    assert not missing,('unexecuted native lifecycle instructions',[hex(a) for a in missing])
    sources=['runtime/actor_ring_lifecycle.c','src/unit_02183.c','src/unit_02184.c',
        'runtime/ring_queue.c','include/actor_ring_queue.h','runtime/standalone_link_check.c',
        'CMakeLists.txt','direct_c/verify_actor_ring_lifecycle.py']
    proof=dict(native_sha256=hashlib.sha256(original.read_bytes()).hexdigest(),
        compiled_C_path=str(target.resolve()),compiled_C_sha256=target_sha,
        source_hashes={p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest() for p in sources},
        passed=True,checked=n,invocations=calls,legacy_entries_tested=[hex(x) for x in legacy],
        coverage={hex(a):dict(bytes=SIZES[a],instructions=len(x),executed=len(x&visited)) for a,x in owned.items()},
        controlled_algorithm_endpoints=[],controlled_platform_endpoints=['Native5618E6 (_onexit)','Hosted C atexit'],
        scope=['Six complete lifecycle entries; 50 forward constructors and50 reverse destructors',
               'Actual native vector iterators, normal finally paths and ring-clear dependency execute',
               'Exact field-write order, unaligned members, registration success/failure and repeated exit callbacks',
               'Original SEH chain restored on normal return; no C++ exception-unwind certification'],
        limitations=['Valid mapped field-only objects; no exceptions or concurrent mutation.',
                     'Does not certify the entire original CRT initializer/destructor catalogue or gameplay.'],commit=None)
    suffix='diagnostic' if target.stem=='restore_link_check' else 'game'
    (ROOT/f'metadata/actor-ring-lifecycle-{suffix}.json').write_text(json.dumps(proof,indent=2)+'\n')
    print('PASS whole ring lifecycle, real native iterators, callback ordering, aliases and failure paths:',n,flush=True)
    print('Native lifecycle instruction coverage:',len(visited),flush=True)
    print('Verified target SHA256:',target_sha,flush=True)
