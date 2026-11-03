"""Monument multipart creation, with actual allocation, undo and map routines."""
from verify_options_startup import *
from unicorn import UC_HOOK_MEM_WRITE,UC_HOOK_MEM_INVALID
from collections import deque
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import json

OBJ=S+0x1000
METHODS={0x4015a0:(16,0xe6,0x56f438),0x40da90:(25,0xe1,0x56f48c),0x417180:(9,0xe3,0x56f4dc),0x4e6840:(2,0xe4,0x56f6e8)}
recent=deque(maxlen=16)
DISPATCH=sym('restore_building_create_dispatch')
METHOD_NAMES={t:'restore_monument_create_%08x'%t for t in METHODS}
catalogue=json.loads((ROOT/'metadata/monument-create-native.json').read_text())
assert catalogue['native_sha256']==hashlib.sha256(original.read_bytes()).hexdigest()
owned={};branches={};hits=set();edges=set();last=[None];checked=0;invocations=0
for method in catalogue['functions']:
    a=int(method['address'],16);n=method['result']['procedure']['body']['total_bytes']
    if 'rea_bytes' in method:
        evidence=method['rea_bytes'];assert evidence['complete'] and bytes.fromhex(evidence['bytes_hex'])==bytes(images[0].data[a-images[0].base:a-images[0].base+n])
    code=list(Cs(CS_ARCH_X86,CS_MODE_32).disasm(bytes(images[0].data[a-images[0].base:a-images[0].base+n]),a))
    assert sum(i.size for i in code)==n
    assert [hex(i.address) for i in code]==[x.split(':')[0] for x in method['result']['instructions']]
    owned[a]={i.address for i in code}
    branches.update({i.address:{int(i.op_str,16),i.address+i.size} for i in code if i.mnemonic.startswith('j') and i.mnemonic!='jmp'})
points=set.union(*owned.values())
ENDPOINTS={0x46a400:6,0x485100:1,0x4f4f80:1,0x401af0:3,0x40eb90:3,0x40dca0:1,0x4017a0:1,0x4e69c0:1,0x4dea50:0}
MEMBERS={0x401af0,0x40eb90,0x40dca0,0x4017a0,0x4e69c0}
def observe_call(u,state,t,sp,receiver):
    args=tuple(get(u,sp+4+i*4) for i in range(ENDPOINTS[t]))
    if t in [0x401af0,0x40eb90]:args=(args[0],get(u,args[1]),get(u,args[2]))
    # Observe the complete record before each real child call. This catches
    # undo-vs-map order as well as stage/link writes that final state can hide.
    selected=args[0] if ENDPOINTS[t]==1 or t in [0x401af0,0x40eb90] else 0
    assert selected<=4000,(hex(t),tuple(hex(x) for x in args),hex(sp),hex(receiver),list(recent))
    record=bytes(u.mem_read(0x7999d8+selected*264,264))
    if t==0x4dea50:
        root,x,y,occupied=state['anchor']
        if root and x>=0 and y>=0 and not occupied:
            assert struct.unpack('<H',bytes(u.mem_read(0xb4e6fc+2*(10000+y*228+x),2)))[0]==root,('map before full redraw',root,x,y)
    state['events'].append((t,args,receiver if t in MEMBERS else 0,record))
# No call substitution is needed here. Use machines without the general
# harness's global interception hook, and observe only the relevant entries.
machines=[]
for c,pe in enumerate(images):
    u=Uc(UC_ARCH_X86,UC_MODE_32)
    u.mem_map(pe.base,(len(pe.data)+4095)&~4095);u.mem_write(pe.base,bytes(pe.data))
    u.mem_map(0x30000000,0x10000);u.mem_map(STOP,4096);u.mem_map(S,0x50000)
    state={'hooks':{},'calls':[]};machines.append((c,u,state))
    u.mem_map(0,0x1000)
    def invalid(uc,access,at,size,value,data):
        print('INVALID',access,hex(at),size,'pc',hex(uc.reg_read(UC_X86_REG_EIP)),flush=True)
        return False
    u.hook_add(UC_HOOK_MEM_INVALID,invalid)
    def observer(uc,pc,size,data,c=c):
        if not c:
            if pc in points:hits.add(pc)
            if last[0] in branches:edges.add((last[0],pc))
            last[0]=pc
        elif pc==DISPATCH:
            sp=uc.reg_read(UC_X86_REG_ESP);t=get(uc,sp+8)
            if t in ENDPOINTS:
                frame=get(uc,sp+12)
                observe_call(uc,data,t,get(uc,frame+16),get(uc,frame+4))
    if not c:
        for method,addresses in owned.items():u.hook_add(UC_HOOK_CODE,observer,state,begin=min(addresses),end=max(addresses))
        def entry_observer(uc,pc,size,data):observe_call(uc,data,pc,uc.reg_read(UC_X86_REG_ESP),uc.reg_read(UC_X86_REG_ECX))
        for t in ENDPOINTS:u.hook_add(UC_HOOK_CODE,entry_observer,state,begin=t,end=t)
    else:u.hook_add(UC_HOOK_CODE,observer,state,begin=DISPATCH,end=DISPATCH)

# Derive shipped geometry from the actual original constructor, including
# all16 descriptors and repeated image categories; no constructor substitute.
_layout_machine=machines[0][1]
run(0,_layout_machine,0x401030,receiver=OBJ)
ABU_LAYOUT=bytes(_layout_machine.mem_read(OBJ+0x4c,16*52))
assert struct.unpack_from('<4i',ABU_LAYOUT)==(1,7,3,5)

def fixture(u,token,root=10,x=50,y=50,skip=0,rotation=0,occupied=0,registration=None,undo=0,size=2,dirty=False,layout=False):
    count,kind,table=METHODS[token]
    u.mem_write(0x7999d8,bytes(264*4001))
    u.mem_write(0xba746c,bytes(0x22000))
    u.mem_write(OBJ,bytes(0x600))
    put(u,OBJ,table);put(u,OBJ+4,2);put(u,OBJ+8,0x76543210)
    # Already resident libraries: execute the real loader's matching-name
    # fast path, without filesystem hooks or launching game/graphics APIs.
    u.mem_write(S+0x3000,b'monument-fixture\0')
    for slot in [18,19,26]:
        lib=0xf55bb0+0x1f40+slot*0xcc3c
        u.mem_write(lib,(b'AbuSimbel' if slot==26 else b'monument-fixture')+b'\0');put(u,lib+0xcc2c,1)
        put(u,lib+0xcc30,0)
        put(u,0xf55bb0+0x30f548+(slot-1)*4,1)
        put(u,0xf55bb0+0x30f548+slot*4,1)
    put(u,OBJ+0x220,S+0x3000);put(u,OBJ+0xb4,S+0x3000)
    put(u,0xf57394,90000);put(u,0xf5739c,100000)
    for i in range(count):
        piece=OBJ+0x4c+i*52
        put(u,piece,(i%5)*3);put(u,piece+4,(i//5)*3);put(u,piece+8,size)
        put(u,piece+12,i%12 if token==0x4015a0 else i)
    if layout:
        assert token==0x4015a0
        u.mem_write(OBJ+0x4c,ABU_LAYOUT)
    for a,v in [(0xbca2d8,10000),(0xbca2d0,max(80,x+16)),(0xbca2d4,max(80,y+16)),(0xbca2e0,rotation),(0xe94e04,rotation),(0xeaac28,0),(0x78d790,root),
                (0x78d7b4,100),(0x70e120,0),(0xe38dcc,0),(0xbc9694,1)]:put(u,a,v)
    r=root*264
    byte(u,0x7999d8+r,3);word(u,0x7999e8+r,kind)
    word(u,0x7999de+r,x);word(u,0x7999e0+r,y);put(u,0x7999e4+r,10000+y*228+x)
    if dirty:
        put(u,0x799a38+r,0xabcdef03);put(u,0x799a3c+r,0x12345678);put(u,0x799a40+r,0x87654321);byte(u,0x799a44+r,0xff)
    if occupied:byte(u,0x7999d8+(root+occupied)*264,3)
    if registration is not None:
        subtype={0xe1:6,0xe3:7,0xe4:5,0xe6:14}[kind]
        selected=next(i for i in range(38) if struct.unpack('<hh',bytes(u.mem_read(0x5d19c8+i*16,4)))==(2,subtype))
        for i in range(3):word(u,0x784de0+i*2,selected if i==registration else -1);put(u,0xeaa978+i*4,0)
    if undo==1:word(u,0xba746c,root)
    elif undo==2:u.mem_write(0xba746c,b'\xff\x7f'*525)

def call(c,u,token,args,mode):
    global invocations
    invocations+=1
    if not c:return run(c,u,token,args,cleanup=16,receiver=OBJ,budget=100000000)
    if mode=='legacy':return run(c,u,token,args,receiver=OBJ,budget=100000000)
    for reg,value in zip(registers,[0x11111111,OBJ,0x33333333,saved[0],SP,*saved[1:]]):u.reg_write(reg,value)
    u.reg_write(UC_X86_REG_EFLAGS,2);put(u,SP,STOP)
    entry=None;argv=[OBJ,*args]
    if mode=='named':entry=sym(METHOD_NAMES[token])
    if mode in ['frame','dispatch']:
        frame=S+0x8000;virtual_sp=S+0x18000
        u.mem_write(virtual_sp-512,b'\x6b'*576)
        u.mem_write(frame,struct.pack('<10I',0x11111111,OBJ,0x33333333,saved[0],virtual_sp,*saved[1:],2,DISPATCH))
        put(u,virtual_sp,STOP)
        for i,value in enumerate(args):put(u,virtual_sp+4+i*4,value)
        entry=sym('restore_monument_create_frame');argv=[token,frame]
        if mode=='dispatch':entry=DISPATCH;argv=[0,token,frame]
    for i,value in enumerate(argv):put(u,SP+4+i*4,value)
    u.emu_start(entry,STOP,count=100000000)
    assert u.reg_read(UC_X86_REG_EIP)==STOP
    assert u.reg_read(UC_X86_REG_ESP)==SP+4
    assert [u.reg_read(registers[i]) for i in [3,5,6,7]]==saved
    if mode in ['frame','dispatch']:
        assert get(u,frame+16)==virtual_sp+20
        assert [get(u,frame+i*4) for i in [3,5,6,7]]==saved
        assert bytes(u.mem_read(virtual_sp-512,64))==b'\x6b'*64
        assert bytes(u.mem_read(virtual_sp+20,44))==b'\x6b'*44

def exercise(token,root=10,x=50,y=50,skip=0,rotation=0,occupied=0,registration=None,undo=0,size=2,dirty=False,layout=False):
    global checked
    expected=None
    for c,u,state in machines:
        legacy=bool(re.search(r'\s_FUN_%08x\s'%token,symbols))
        named=bool(re.search(r'\s_'+METHOD_NAMES[token]+r'\s',symbols))
        modes=(['named'] if named else [])+(['legacy'] if legacy else [])+['dispatch']
        for mode in (modes if c else ['native']):
            reset(u,state);fixture(u,token,root,x,y,skip,rotation,occupied,registration,undo,size,dirty,layout)
            state['events']=[];state['anchor']=(root,x+(1 if layout else 0),y+(7 if layout else 0),occupied);last[0]=None
            call(c,u,token,[x,y,root,skip],mode)
            if token not in [0x4015a0,0x4e6840] and root and x>=0 and y>=0 and not occupied:
                assert struct.unpack('<H',bytes(u.mem_read(0xb4e6fc+2*(10000+y*228+x),2)))[0]==root,('map owner',hex(token),root,x,y,mode)
            result=(state['events'],snapshot(u),bytes(u.mem_read(OBJ,0x600)))
            if expected is None:expected=result
            elif result!=expected:
                diff=next(((hex(0x560000+i),a,b) for i,(a,b) in enumerate(zip(result[1],expected[1])) if a!=b),None)
                eventdiff=next(((i,a,b) for i,(a,b) in enumerate(zip(result[0],expected[0])) if a!=b),None)
                raise AssertionError((hex(token),root,x,y,skip,rotation,occupied,registration,undo,size,mode,'memory',diff,'events',len(result[0]),len(expected[0]),eventdiff))
    checked+=1

if __name__=='__main__':
    for token,rotation,skip,root in itertools.product(METHODS,[0,2,4,6],[0,1,255,256,0x12340000],[0,10,3975]):
        exercise(token,root=root,rotation=rotation,skip=skip)
    for token,(x,y),size in itertools.product(METHODS,[(-1,0),(0,-1),(120,90)],[1,3]):exercise(token,x=x,y=y,size=size)
    for token,registration,skip in itertools.product(METHODS,range(3),[0,1]):exercise(token,registration=registration,skip=skip)
    for token,undo in itertools.product(METHODS,[1,2]):exercise(token,undo=undo)
    for token in METHODS:
        for occupied in sorted({1,METHODS[token][0]//2,METHODS[token][0]-1}):exercise(token,occupied=occupied)
    for rotation,skip,dirty in itertools.product([0,2,4,6],[0,1],[False,True]):
        exercise(0x4015a0,rotation=rotation,skip=skip,dirty=dirty,layout=True)
    for dirty in [False,True]:exercise(0x4015a0,dirty=dirty)
    print('PASS actual allocator, undo, map callbacks and resource fast path:',checked,flush=True)
    assert points<=hits,('missing native instructions',[hex(a) for a in sorted(points-hits)])
    missing={(a,b) for a,targets in branches.items() for b in targets if (a,b) not in edges}
    assert not missing,('missing conditional edges',[(hex(a),hex(b)) for a,b in sorted(missing)])
    sources=['include/monument_create.h','runtime/monument_create.c','runtime/building_methods.c',
             'include/monument_entrance.h','runtime/monument_entrance.c','runtime/placement_queries.c',
             'CMakeLists.txt','direct_c/verify_monument_create.py','metadata/monument-create-native.json']+[f'src/unit_{i:05d}.c' for i in [8,106,201,2000]]
    proof=dict(native_sha256=catalogue['native_sha256'],compiled_C_path=str(target.resolve()),compiled_C_sha256=target_sha,
               passed=True,checked=checked,invocations=invocations,
               compiled_entry_modes=(['named','legacy','dispatch'] if target.stem=='restore_link_check' else ['dispatch']),
               coverage={hex(a):dict(instructions=len(x),executed=len(x&hits)) for a,x in owned.items()},conditional_edges=len(edges),
               controlled_algorithm_endpoints=[],controlled_platform_endpoints=[],
               source_hashes={p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest() for p in sources},
               limitations=['Valid mapped metadata and map cells; no concurrency or overlapping metadata/building storage.',
                            'Sprite library names match resident library descriptors; actual loader fast path executes without file I/O.',
                            'Void methods: caller-saved register/flag values and private scratch-stack bytes are outside comparison.',
                            'Compares all game globals, metadata and record snapshots at actual child entries; does not approve full gameplay.'],commit=None)
    suffix='diagnostic' if target.stem=='restore_link_check' else 'game'
    (ROOT/f'metadata/monument-create-{suffix}.json').write_text(json.dumps(proof,indent=2)+'\n')
    print('Native instructions/conditional edges:',len(hits&points),len(edges),'invocations:',invocations,flush=True)
    print('Verified target SHA256:',target_sha,flush=True)
