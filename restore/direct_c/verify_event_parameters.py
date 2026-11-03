"""Native/C parameter selection and event reset, with real RNG and callers."""
from verify_options_startup import *
import json
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
from unicorn import UC_HOOK_MEM_WRITE

Q=S+0x1000
NAMES={0x4487f0:'restore_event_parameter_choose',0x448a90:'restore_event_record_reset'}
MISSION={0x4487f0:'restore_mission_range_004487f0',0x448a90:'restore_mission_record_00448a90'}
available=lambda n:bool(re.search(r'\s_'+re.escape(n)+r'\s',symbols))
legacy={a for a in NAMES if available('FUN_%08x'%a)}
if '--require-all' in sys.argv:assert legacy==set(NAMES)
catalogue=json.loads((ROOT/'metadata/event-parameters-native.json').read_text())
assert catalogue['native_sha256']==hashlib.sha256(original.read_bytes()).hexdigest()
owned={};branches={};hits=set();edges=set();previous=[None];checked=0;invocations=0
for f in catalogue['functions'][:2]:
    a=int(f['address'],16);n=f['result']['procedure']['body']['total_bytes']
    code=list(Cs(CS_ARCH_X86,CS_MODE_32).disasm(bytes(images[0].data[a-images[0].base:a-images[0].base+n]),a))
    assert sum(i.size for i in code)==n;owned[a]={i.address for i in code}
    branches.update({i.address:{int(i.op_str,16),i.address+i.size} for i in code if i.mnemonic.startswith('j') and i.mnemonic!='jmp'})
points=set.union(*owned.values())
for c,u,state in machines:
    reset(u,state);state.update(watch=Q,writes=[])
    def writes(cpu,access,at,size,value,data):
        p=data['watch']
        if 0xd3b1c0<=at<0xd3b38c or (p is not None and p-16<=at<p+160):
            data['writes'].append((at,size,value&((1<<(size*8))-1)))
    u.hook_add(UC_HOOK_MEM_WRITE,writes,state)
    if not c:
        def observe(cpu,pc,size,data):
            if pc in points:hits.add(pc)
            if previous[0] in branches:edges.add((previous[0],pc))
            previous[0]=pc
        u.hook_add(UC_HOOK_CODE,observe,state)

def call(c,u,token,obj,eax,mode):
    global invocations
    invocations+=1
    argv=[]
    if not c:entry=token
    elif mode=='legacy':entry=address(c,token)
    elif mode=='mission':entry=sym(MISSION[token]);argv=[obj]
    else:entry=sym(NAMES[token]);argv=[obj,eax] if token==0x4487f0 else [obj]
    for r,v in zip(registers,[eax,obj,0x98765432,saved[0],SP,*saved[1:]]):u.reg_write(r,v)
    u.reg_write(UC_X86_REG_EFLAGS,0x202);put(u,SP,STOP)
    for i,v in enumerate(argv):put(u,SP+4+i*4,v)
    u.emu_start(entry,STOP,count=200000)
    assert u.reg_read(UC_X86_REG_EIP)==STOP,(c,hex(token),mode,hex(u.reg_read(UC_X86_REG_EIP)))
    assert u.reg_read(UC_X86_REG_ESP)==SP+4
    assert [u.reg_read(registers[i]) for i in [3,5,6,7]]==saved
    if not c or mode=='legacy':assert u.reg_read(UC_X86_REG_ECX)==(obj if token==0x4487f0 else 0),('native member ECX',c,hex(token),mode)
    return u.reg_read(UC_X86_REG_EAX)

def fixture(u,obj,index,a,b):
    u.mem_write(obj-16,b'\xa5'*176);u.mem_write(0xd3b1c0,b'\x69'*0x1cc)
    put(u,0xd3b1c4,index);put(u,0xd3b374,0x1234567f);put(u,0xd3b384,a);put(u,0xd3b380,b)
    word(u,0x784c60,0x1234);word(u,0x784c62,0x5678)

def compare(token,prepare,obj=Q,eax=0xabcdef01):
    global checked
    expected=None
    for c,u,state in machines:
        modes=['native'] if not c else ['named']+(['legacy'] if token in legacy else [])+(['mission'] if available(MISSION[token]) else [])
        for mode in modes:
            prepare(u);state.update(watch=obj,writes=[])
            result=call(c,u,token,obj,eax,mode)
            out=(tuple(state['writes']),bytes(u.mem_read(obj-16,176)),bytes(u.mem_read(0xd3b1c0,0x1cc)),bytes(u.mem_read(0x784c60,4)))
            if expected is None:expected=(result,out)
            else:
                assert out==expected[1],('state/write order',hex(token),hex(obj),mode,out[0],expected[1][0])
                if mode!='mission':assert result==expected[0],('full EAX',hex(token),mode,hex(result),hex(expected[0]))
    checked+=1

def parameter_cases():
    vals=[-32768,-1,0,1,32767]
    for first,second,third,index,a in itertools.product(vals,vals,vals,[0,99],[0,0x12345678,0xffffffff]):
        if first<0 and second==third and second<0:continue # native zero-divisor #DE
        def prepare(u):
            fixture(u,Q,index,a,a^0x87654321)
            for off,v in [(0,0xa55a),(2,first),(4,second),(6,third)]:word(u,Q+off,v)
        compare(0x4487f0,prepare,eax=(a^0xabcdef01)&0xffffffff)
    for offset in range(1,4):
        def prepare(u):
            fixture(u,Q+offset,98,0x80000000,0xffffffff)
            for off,v in [(2,100),(4,200),(6,300)]:word(u,Q+offset+off,v)
        compare(0x4487f0,prepare,obj=Q+offset)
    # Inputs can alias the RNG globals; selection reloads its WORD parameters
    # after all RNG stores, as the original does.
    for obj in [0xd3b370-2,0xd3b378-4,0xd3b380,0xd3b384-6,0xd3b1c8+99*4]:
        def prepare(u):
            fixture(u,obj,99,0x12345678,0x87654321)
            for off,v in [(2,100),(4,200),(6,300)]:word(u,obj+off,v)
        compare(0x4487f0,prepare,obj=obj)
    print('PASS parameter choices, signed ranges, complete EAX and ordered RNG aliases:',checked,flush=True)

def record_cases():
    for kind,index,a,variant in itertools.product([0,3,255],[0,94,98,99],[0,1,0x12345678,0xffffffff],range(3)):
        def prepare(u):
            fixture(u,Q,index,a,a^0x87654321);byte(u,Q+2,kind);byte(u,Q+3,255);put(u,Q+0x28,0xffffffff)
            for off in [4,12,20,28,88]:
                values=[(10,-1,-1),(10,20,30),(-1,-32768,32767)][variant]
                for j,v in enumerate(values):word(u,Q+off+2+j*2,v)
        compare(0x448a90,prepare)
    for obj in [Q+1,Q+2,Q+3,0x784c60-0x40]:
        def prepare(u):
            fixture(u,obj,99,0x12345678,0x87654321);byte(u,obj+2,3);byte(u,obj+3,4)
            for off in [4,12,20,28,88]:
                for j,v in enumerate([10,-1,-1]):word(u,obj+off+2+j*2,v)
        compare(0x448a90,prepare,obj=obj)
    print('PASS shared event reset, five actual parameter groups and RNG/cell-coordinate write order:',checked,flush=True)

if __name__=='__main__':
    parameter_cases();record_cases()
    # The apparent signed-remainder fixup is unreachable: EDX was masked to
    # 15 bits, then AND80000001 yields0/1 and cannot set the sign flag.
    dead={0x448c8b,0x448c8c,0x448c8f};dead_edges={(0x448c89,0x448c8b)}
    assert points-dead<=hits,('unexecuted instructions',[hex(a) for a in sorted(points-dead-hits)])
    missing={(a,b) for a,targets in branches.items() if a not in dead for b in targets if (a,b) not in edges}-dead_edges
    assert not missing,('unexecuted branch edges',[(hex(a),hex(b)) for a,b in sorted(missing)])
    # The changed provider is also exercised by the actual scheduling graph
    # and full event iterator. No descendant algorithms are replaced.
    import verify_event_packet as packets
    composed=0
    for kind,links,delay in itertools.product([21,22],[0,3,59],[0,6]):
        packets.action(kind=kind,links=links,delay=delay,dispatch=True,enumerate_events=True);composed+=1
    sources=['include/event_parameters.h','runtime/event_parameters.c','runtime/mission_city_commit.c',
             'runtime/menu_event_actions.c','runtime/menu_event_model.c','include/menu_event_actions.h',
             'src/unit_00681.c','src/unit_00685.c','CMakeLists.txt','direct_c/verify_event_parameters.py']
    proof=dict(native_sha256=catalogue['native_sha256'],compiled_C_path=str(target.resolve()),compiled_C_sha256=target_sha,
               passed=True,checked=checked,invocations=invocations,composed_iterator_cases=composed,
               legacy_entries_compared=[hex(a) for a in sorted(legacy)],
               mission_facades_compared=[n for n in MISSION.values() if available(n)],
               source_hashes={p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest() for p in sources},
               coverage={hex(a):dict(instructions=len(x),executed=len(x&hits)) for a,x in owned.items()},
               conditional_edges=len(edges),unreachable_instructions=[hex(a) for a in sorted(dead)],
               controlled_algorithm_endpoints=[],controlled_platform_endpoints=[],
               limitations=['Mapped nonfaulting WORD groups and history indices0..99; no concurrency.',
                            'A zero native IDIV divisor has no successful result; the existing C abort policy is preserved.',
                            'Mission facades discard EAX. Canonical/named methods compare full EAX; unused volatile EDX/flags are outside the logical boundary contract.',
                            'Other mission setup algorithms and event kinds are not approved by this comparison.'],commit=None)
    suffix='diagnostic' if target.stem=='restore_link_check' else 'game'
    (ROOT/f'metadata/event-parameters-{suffix}.json').write_text(json.dumps(proof,indent=2)+'\n')
    print('PASS event parameter/reset methods:',checked,'cases;',composed,'complete iterator compositions',flush=True)
    print('Native executed/unreachable instructions:',len(hits&points),len(dead),flush=True)
    print('Verified target SHA256:',target_sha,flush=True)
