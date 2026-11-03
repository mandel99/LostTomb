"""Original/C sprite-manager lifetime comparison, without launching the game.

Native vector construction/destruction and normal SEH/finally paths execute.
Only allocator release and process-exit registration are controlled endpoints.
This does not certify C++ exception dispatch or unresolved virtual destructors.
"""
from verify_options_startup import *
import json
from capstone import Cs, CS_ARCH_X86, CS_MODE_32

BASE, SIZE, STRIDE = 0xf55bb0, 0x30f5d8, 0xcc3c
Q, OTHER, FRAME = 0x35000040, 0x35400040, S+0x3000
GDT, TEB = 0x34000000, 0x34001000
SIZES = {0x53da50:10,0x53da60:10,0x53da70:12,0x53da80:10,
         0x53da90:101,0x53db00:90,0x53db60:431,0x5404e0:31,
         0x540500:54,0x56b770:27,0x56b7a0:27}
MEMBERS = {0x53da90,0x53db00,0x53db60,0x5404e0,0x540500}
FRAMES = {0x56b770,0x56b7a0}
GLOBALS = {0x53da50,0x53da60,0x53da70,0x53da80}
STATUS = {0x53da50,0x53da70}
available = lambda name:bool(re.search(r'\s_'+re.escape(name)+r'\s',symbols))
legacy = {a for a in SIZES if available('FUN_%08x'%a)}
supported = legacy | (FRAMES if available('restore_sprite_libraries_destroy_frame') else set())
if '--require-all' in sys.argv:assert len(legacy)==len(SIZES),legacy
decoder=Cs(CS_ARCH_X86,CS_MODE_32)
owned={a:{i.address for i in decoder.disasm(bytes(images[0].data[a-images[0].base:a-images[0].base+n]),a)} for a,n in SIZES.items()}
hits=set();invocations=0
for c,u,state in machines:
    u.mem_map(Q&~4095,0x310000);u.mem_map(OTHER&~4095,0x310000)
    u.mem_map(GDT,0x2000)
    for selector,access in [(8,0x9b),(16,0x93)]:
        u.mem_write(GDT+selector,struct.pack('<HHBBBB',0xffff,0,0,access,0xcf,0))
    u.mem_write(GDT+24,struct.pack('<HHBBBB',0xfff,TEB&65535,(TEB>>16)&255,0x93,0x40,(TEB>>24)&255))
    u.reg_write(UC_X86_REG_GDTR,(0,GDT,0x3f,0));u.reg_write(UC_X86_REG_CS,8)
    for reg in [UC_X86_REG_DS,UC_X86_REG_ES,UC_X86_REG_SS]:u.reg_write(reg,16)
    u.reg_write(UC_X86_REG_FS,24)
    if not c:
        points=set.union(*owned.values())
        def visit(cpu,pc,size,data):
            if pc in points:hits.add(pc)
        u.hook_add(UC_HOOK_CODE,visit)

def library(obj,index):return obj+0x1f40+index*STRIDE
def pixel(index):return 0x36000000+index*0x100
def observed_fields(u,obj):
    return tuple(bytes(u.mem_read(library(obj,i)+0xcc24,24)) for i in range(30)),bytes(u.mem_read(obj+0x30f548,0x90))

def setup(c,u,state,obj,pattern,mutation,success,fill):
    reset(u,state);put(u,TEB,0xfeedbabe)
    for a in [BASE,Q,OTHER]:u.mem_write(a-16,bytes([fill])*(SIZE+48))
    put(u,FRAME-16,obj);state['events']=[];state['registered']=[]
    for i in range(30):
        p=0 if pattern==0 or pattern==2 and i%2 else pixel(i)
        put(u,library(obj,i)+0xcc24,p)
    shared=pixel(40);put(u,0xe92e1c,shared)
    put(u,obj+0x30f5c0,shared if pattern==3 else pixel(31) if pattern else 0)
    put(u,obj+0x30f5c4,shared if pattern==3 else pixel(30) if pattern else 0)
    def release(cpu,args):
        p,=args;assert p
        state['events'].append(('free',p,observed_fields(cpu,obj)))
        if len(state['events'])==1:
            if mutation==1:
                put(cpu,library(obj,0)+0xcc24,pixel(60));put(cpu,library(obj,29)+0xcc24,pixel(61))
            elif mutation==2:
                put(cpu,obj+0x30f5c0,pixel(62));put(cpu,obj+0x30f5d4,0x1234)
                put(cpu,obj+0x30f548,0x5678)
            elif mutation==4:put(cpu,FRAME-16,OTHER)
        if mutation==3 and p==pixel(29):put(cpu,library(obj,0)+0xcc24,pixel(63))
        return 0xdeadbeef
    if c:state['hooks'][sym('restore_heap_free')]=(1,0,release)
    else:hook(c,state,0x561a2b,1,release)
    def register(cpu,args):
        expected=sym('FUN_0053da80') if c else 0x53da80
        assert args==(expected,),('callback address',c,args)
        state['events'].append(('register',0x53da80))
        if success:state['registered'].append(expected)
        return (0 if success else 0xffffffff) if c else (expected if success else 0)
    if c:state['hooks'][sym('atexit')]=(1,0,register)
    else:hook(c,state,0x5618e6,1,register)

def invoke(c,u,token,obj,mode='legacy'):
    global invocations
    invocations+=1
    frame=FRAME if token in FRAMES else saved[1]
    args=[obj] if c and token in MEMBERS else []
    if mode=='named':entry=sym('restore_sprite_libraries_destroy_frame');args=[FRAME]
    else:entry=address(c,token)
    for reg,value in zip(registers,[0x11111111,obj,0x33333333,saved[0],SP,frame,saved[2],saved[3]]):u.reg_write(reg,value)
    u.reg_write(UC_X86_REG_EFLAGS,2);put(u,SP,STOP)
    for i,v in enumerate(args):put(u,SP+4+i*4,v)
    u.emu_start(entry,STOP,count=2000000)
    assert u.reg_read(UC_X86_REG_EIP)==STOP,(c,hex(token),mode)
    assert u.reg_read(UC_X86_REG_ESP)==SP+4
    assert [u.reg_read(registers[i]) for i in [3,5,6,7]]==[saved[0],frame,saved[2],saved[3]]
    assert get(u,TEB)==0xfeedbabe,('normal SEH restoration',c,hex(token))
    return u.reg_read(UC_X86_REG_EAX) if token in STATUS|{0x53da60,0x53da90,0x53db60,0x5404e0} else None

def exercise(token,pattern=1,mutation=0,success=True,fill=0xa5,alignment=0,finish=False):
    if token not in supported:return 0
    obj=BASE if token in GLOBALS else Q+alignment
    if token in [0x5404e0,0x540500]:obj=library(Q,7)+alignment
    expected=None
    for c,u,state in machines:
        modes=['legacy'] if not c or token not in FRAMES else (['legacy'] if token in legacy else [])+['named']
        for mode in modes:
            # A single library uses the same fixture manager, with its actual
            # member address passed separately from the manager observer.
            owner=Q if token in [0x5404e0,0x540500] else obj
            setup(c,u,state,owner,pattern,mutation,success,fill)
            if token in [0x5404e0,0x540500]:put(u,obj+0xcc24,pixel(7) if pattern else 0)
            value=invoke(c,u,token,obj,mode)
            if finish:
                for i in range(30):put(u,library(BASE,i)+0xcc24,pixel(i))
                for callback in reversed(state['registered']):invoke(c,u,0x53da80,BASE)
            current=(value,tuple(state['events']),get(u,FRAME-16),snapshot(u),
                     bytes(u.mem_read(Q-16,SIZE+48)),bytes(u.mem_read(OTHER-16,SIZE+48)))
            if expected is None:expected=current
            else:
                assert current[:3]==expected[:3],('lifetime return/callbacks',hex(token),pattern,mutation,success,alignment,mode,current[:3],expected[:3])
                for j in [3,4,5]:
                    assert current[j]==expected[j],('lifetime memory',hex(token),pattern,mutation,success,alignment,mode,j,
                        next((i,a,b) for i,(a,b) in enumerate(zip(current[j],expected[j])) if a!=b))
    return 1

if __name__=='__main__':
    count=0
    for token,fill,alignment in itertools.product([0x53da90,0x5404e0],[0,0xa5,0xff],[0,1,3]):
        count+=exercise(token,fill=fill,alignment=alignment)
    for token,pattern,mutation in itertools.product([0x53db00,0x53db60,0x53da80,*FRAMES],range(4),range(5)):
        count+=exercise(token,pattern=pattern,mutation=mutation)
    for pattern,mutation,alignment in itertools.product([0,1],range(4),[0,3]):
        count+=exercise(0x540500,pattern=pattern,mutation=mutation,alignment=alignment)
    for token,success,fill in itertools.product([0x53da50,0x53da60,0x53da70],[False,True],[0x5a,0xa5]):
        count+=exercise(token,success=success,fill=fill,finish=token in STATUS)
    missing=set.union(*(owned[a] for a in supported))-hits
    assert not missing,('unexecuted native instructions',[hex(a) for a in sorted(missing)])
    sources=['include/sprite_shutdown.h','runtime/sprite_shutdown_lifecycle.c','runtime/standalone_link_check.c',
             'runtime/sprite_libraries.c','src/unit_00179.c','CMakeLists.txt','direct_c/verify_sprite_startup.py']
    sources += [f'src/unit_{i:05d}.c' for i in [2512,2513,2514,2515,2516,2529,2533,3100,3101]]
    proof=dict(native_sha256=hashlib.sha256(original.read_bytes()).hexdigest(),compiled_C_path=str(target.resolve()),
        compiled_C_sha256=target_sha,source_hashes={p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest() for p in sources},
        passed=True,checked=count,invocations=invocations,legacy_entries_tested=[hex(a) for a in sorted(legacy)],
        methods_compared=[hex(a) for a in sorted(supported)],
        coverage={hex(a):dict(bytes=SIZES[a],instructions=len(x),executed=len(x&hits)) for a,x in owned.items()},
        controlled_algorithm_endpoints=[],controlled_platform_endpoints=['Original561A2B / C restore_heap_free','Original5618E6 / C atexit'],
        scope=['Real native vector helpers, normal finally paths, constructors/reset/destructors and field preservation',
               'Reverse frame cleanup, captured EBP receiver, callback mutations, shared-buffer sentinel and exit registration'],
        limitations=['Valid mapped objects and normal return; no C++ exception dispatch/unwinding or virtual-destructor certification.',
                     'Does not certify graphics loading, rendering, concurrent mutation or the complete CRT table.'],commit=None)
    suffix='diagnostic' if target.stem=='restore_link_check' else 'game'
    (ROOT/f'metadata/sprite-startup-{suffix}.json').write_text(json.dumps(proof,indent=2)+'\n')
    print('PASS sprite lifetime, native iterators, frame capture and registered exit:',count,flush=True)
    print('Native instruction coverage:',len(hits),flush=True)
    print('Verified target SHA256:',target_sha,flush=True)
