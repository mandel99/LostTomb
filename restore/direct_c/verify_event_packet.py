"""Offline event packets/flood actions with actual queries, RNG and publisher.

Never launches a game or replaces an algorithm/platform endpoint. Scheduler
composition uses the existing complete event frame graph, including immediate
follow-ups; void functions promise state, stack cleanup and saved registers.
"""
from verify_options_startup import *
import json
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
from unicorn import UC_HOOK_MEM_WRITE

Q,P= S+0x1000,S+0x3000
NAMES={0x4449d0:'restore_resource_lookup_slot',0x44c0d0:'restore_event_record_lookup',
       0x4496d0:'restore_event_link_distinct_result',0x44d750:'restore_event_remaining_months',
       0x44e6f0:'restore_event_message_parameters_0044e6f0',0x44f660:'restore_event_message_kind_0044f660',
       0x44e7b0:'restore_flood_event_apply'}
PLAIN={0x4449d0,0x44c0d0};VOID={0x44e6f0,0x44e7b0}
RET4={0x44c0d0,0x4496d0,0x44e6f0,0x44e7b0}
available=lambda n:bool(re.search(r'\s_'+re.escape(n)+r'\s',symbols))
assert all(available(n) for n in NAMES.values())
legacy={a for a in NAMES if available('FUN_%08x'%a)}
if '--require-all' in sys.argv:assert legacy==set(NAMES),legacy
catalogue=json.loads((ROOT/'metadata/event-packet-native.json').read_text())
assert catalogue['native_sha256']==hashlib.sha256(original.read_bytes()).hexdigest()
owned={};branches={};hits=set();edges=set();previous=[None];invocations=0;checked=0
for f in catalogue['functions']:
    a=int(f['address'],16);n=f['result']['procedure']['body']['total_bytes']
    code=list(Cs(CS_ARCH_X86,CS_MODE_32).disasm(bytes(images[0].data[a-images[0].base:a-images[0].base+n]),a))
    assert sum(i.size for i in code)==n
    owned[a]={i.address for i in code}
    branches.update({i.address:{int(i.op_str,16),i.address+i.size}
                     for i in code if i.mnemonic.startswith('j') and i.mnemonic!='jmp'})
points=set.union(*owned.values())
for c,u,state in machines:
    u.mem_map(0x2fe00000,0x200000);reset(u,state)
    state.update(watch=None,writes=[],packets=[])
    def writes(cpu,access,at,size,value,data):
        p=data['watch']
        if p is not None and p<=at<p+32:data['writes'].append((at-p,size,value&((1<<(size*8))-1)))
    def observe(cpu,pc,size,data,c=c):
        if not c:
            if pc in points:hits.add(pc)
            if previous[0] in branches:edges.add((previous[0],pc))
            previous[0]=pc
        pub=sym('FUN_004e0cf0') if c else 0x4e0cf0
        if pc==pub:
            sp=cpu.reg_read(UC_X86_REG_ESP);argv=[get(cpu,sp+4+i*4) for i in range(5)]
            data['packets'].append((tuple(argv[:3]),bytes(cpu.mem_read(argv[3],30)),argv[4]&255))
    u.hook_add(UC_HOOK_MEM_WRITE,writes,state);u.hook_add(UC_HOOK_CODE,observe,state)

def invoke(c,u,token,obj,args,mode='named',budget=2000000):
    global invocations
    invocations+=1
    if not c:entry=token;argv=list(args);cleanup=4 if token in RET4 else 0
    elif mode=='legacy':entry=address(c,token);argv=list(args);cleanup=0
    else:entry=sym(NAMES[token]);argv=list(args) if token in PLAIN else [obj,*args];cleanup=0
    for r,v in zip(registers,[0x89abcdef,obj,0x76543210,saved[0],SP,*saved[1:]]):u.reg_write(r,v)
    u.reg_write(UC_X86_REG_EFLAGS,0x202);put(u,SP,STOP)
    for i,v in enumerate(argv):put(u,SP+4+i*4,v)
    try:u.emu_start(entry,STOP,count=budget)
    except Exception as error:
        raise AssertionError(('execution failed',c,hex(token),mode,hex(u.reg_read(UC_X86_REG_EIP)),
                              [hex(u.reg_read(r)) for r in registers])) from error
    assert u.reg_read(UC_X86_REG_EIP)==STOP,(c,hex(token),mode,hex(u.reg_read(UC_X86_REG_EIP)))
    assert u.reg_read(UC_X86_REG_ESP)==SP+4+cleanup,(c,hex(token),mode,'cleanup')
    assert [u.reg_read(registers[i]) for i in [3,5,6,7]]==saved,(c,hex(token),'saved registers')
    return None if token in VOID else u.reg_read(UC_X86_REG_EAX)

def table(u):
    u.mem_write(0x77de00,bytes(61*106))
    for i in range(61):byte(u,0x77de00+i*106,1);byte(u,0x77de04+i*106,(i*7+3)&255)

def leaf(token,setup,obj=Q,args=(),watch=None,regions=()):
    global checked
    expected=None
    for c,u,state in machines:
        for mode in (['named','legacy'] if c and token in legacy else ['named']):
            setup(u);state.update(watch=watch,writes=[],packets=[])
            result=invoke(c,u,token,obj,args,mode)
            out=(result,tuple(state['writes']),tuple(bytes(u.mem_read(a,n)) for a,n in regions))
            if expected is None:expected=out
            else:assert out==expected,(hex(token),obj,args,mode,out,expected)
    checked+=1
    return expected[0]

def leaves():
    for value in [*range(256),256,0x80000000,0xffffffff]:
        for disabled in [False,True]:
            def prepare(u,value=value,disabled=disabled):
                table(u);byte(u,0x77de00+60*106,not disabled);byte(u,0x77de04+60*106,value)
            leaf(0x4449d0,prepare,args=[value])
    for count,index in itertools.product([-1,0,1,150,0x7fffffff,0x80000000],[-1,0,1,149,150,0x7fffffff]):
        leaf(0x44c0d0,lambda u:put(u,0x785d70,count),args=[index])
    for count,index,flags,identity in itertools.product([0,3],[-1,0,2,3],[0,1,4,255],[-32768,-1,0,2,32767]):
        def prepare(u):
            put(u,0x785d70,count);word(u,Q,identity)
            for i in range(3):byte(u,0x785d74+i*124+0x28,flags)
        leaf(0x4496d0,prepare,args=[index])
    for offset,base,current,month in itertools.product([-32768,-1,0,32767],[-32768,0,32767],
            [0,-3490,0x7fffffff,0x80000000,0xffffffff],[0,255]):
        def prepare(u):
            word(u,Q+0x14,offset);word(u,0x7848d8,base);put(u,0x78d7bc,current)
            word(u,Q+0x2c,-32768);byte(u,Q+3,month);put(u,0x78d7c0,0xffffffff)
        leaf(0x44d750,prepare)
    for kind,subtype,flag in [(k,0,0) for k in range(256)]+[
            (k,s,f) for k,s,f in itertools.product([1,19,20],range(256),[0,255])]+[
            (2,s,0) for s in [0,1,2,3,4,5,255,65535]]+[(23,0,f) for f in [1,255]]:
        def prepare(u):byte(u,Q+2,kind);byte(u,Q+0x60,subtype);byte(u,Q+0x56,flag);word(u,Q+4,subtype)
        leaf(0x44f660,prepare)
    print('PASS full resource/event lookups, partial AL, dates and message selector:',checked,flush=True)

def packets():
    for alignment,extension,reason,commodity in itertools.product(range(4),[-1,0,2],[-32768,-1,0,32767],[0,3,143,255]):
        packet_case(Q+alignment,P+alignment,extension,reason,commodity)
    for delta in [-16,-4,0,2,4,8,12,16,20,24,28,32,36,40,44,48,52,56,80]:
        for reason in [-1,7]:packet_case(Q,Q+delta,0,reason,143)
    for primary in [-32768,-1,255,32767]:packet_case(Q,P,-1,-1,0,primary)
    print('PASS real30-byte packet builder, preserved caller fields and ordered alias writes:',checked,flush=True)

def packet_case(obj,out,extension,reason,commodity,primary=143):
    def prepare(u):
        table(u);u.mem_write(Q-32,b'\xa7'*256);u.mem_write(out-8,b'\x5a'*48)
        put(u,0x785d70,2);word(u,obj+0x34,extension);word(u,obj+0x1c,primary)
        word(u,obj+12,-321);word(u,obj+4,678);word(u,obj+0x2c,-7);word(u,obj+0x14,10)
        word(u,0x7848d8,-3500);put(u,0x78d7bc,-3490);put(u,0x78d7c0,4);byte(u,obj+3,4)
        byte(u,obj+0x36,0xab);byte(u,obj+0x37,0xcd);word(u,obj+0x64,reason)
        byte(u,obj+0x63,commodity);word(u,obj+0x68,456);word(u,obj+0x66,-789)
    leaf(0x44e6f0,prepare,obj,[out],out,[(Q-32,256),(out-8,48)])

def event(u,at,identity,kind=21,delay=10,link=-1,policy=0):
    u.mem_write(at,bytes(124));word(u,at,identity);byte(u,at+2,kind);byte(u,at+3,4)
    word(u,at+0x14,delay);word(u,at+0x24,link);word(u,at+0x26,-1)
    word(u,at+0x34,-1);word(u,at+0x52,-1);word(u,at+0x54,-1)
    byte(u,at+0x6a,policy);word(u,at+0x1c,3);word(u,at+12,345)
    # Each randomized value has a native8-byte parameter group. A fixed
    # choice lives in WORD1, with WORD2/3=-1; an empty negative range divides
    # by zero in the original and is not a valid event-template fixture.
    for field in [4,12,0x14,0x1c,0x58]:
        value=int.from_bytes(u.mem_read(at+field,2),'little')
        word(u,at+field+2,value);word(u,at+field+4,-1);word(u,at+field+6,-1)

def action_fixture(u,editor,flags,due,kind,links,delay,policy,full):
    table(u);put(u,0x605a3c,editor);put(u,0x785d70,max(1,links+1))
    u.mem_write(0x785d74,bytes(150*124));event(u,Q,-1,kind,link=1 if links else -1,policy=policy)
    put(u,Q+0x28,flags);word(u,0x7848d8,-3500);put(u,0x78d7bc,-3490+(due==1))
    put(u,0x78d7c0,4+(due==2));put(u,0x5d9a40,-1);byte(u,0x78a634,1)
    for i in range(1,links+1):
        at=0x785d74+i*124;event(u,at,i,21+i%2,delay,i+1 if i<links else -1,policy);put(u,at+0x28,1)
    u.mem_write(0xd38360,bytes(36));put(u,0xd38360+28,50);put(u,0x604950,1)
    u.mem_write(0xd3b1c0,bytes(0x1cc));put(u,0xd3b384,0x12345678);put(u,0xd3b380,0x87654321)
    byte(u,0xe38dda,2);put(u,0xf55b88,0);put(u,0xe38ef0,0)
    u.mem_write(0xe95080,bytes(0x88));u.mem_write(0xe951e8,bytes(20))
    u.mem_write(0xe95228,bytes(1000*48));u.mem_write(0xe209b8+0x158*80,bytes(6*80))
    if full:
        for i in range(1000):word(u,0xe95228+i*48+10,1)

def raw(c,u,token,obj):
    # Entry to the actual composed event graph, with a genuine shared arena.
    vsp=S+0x48000;regs=S+0x2e000;fl=regs+40
    frame=[0x89abcdef,obj,0x76543210,saved[0],vsp,*saved[1:]]
    put(u,vsp,STOP);u.mem_write(regs,struct.pack('<8I',*frame));put(u,fl,0x202)
    for i,v in enumerate([STOP,token,regs,fl]):put(u,SP+4*i,v)
    u.reg_write(UC_X86_REG_ESP,SP);u.emu_start(sym('restore_event_actions_raw'),STOP,count=50000000)
    assert u.reg_read(UC_X86_REG_EIP)==STOP,('raw stopped',hex(u.reg_read(UC_X86_REG_EIP)))
    assert u.reg_read(UC_X86_REG_ESP)==SP+4
    after=struct.unpack('<8I',u.mem_read(regs,32))
    assert after[4]==vsp+4 and [after[i] for i in [3,5,6,7]]==saved

def action(editor=0,flags=0,due=0,kind=21,increase=0,links=0,delay=6,policy=0,full=False,dispatch=False,enumerate_events=False):
    global checked
    expected=None
    for c,u,state in machines:
        for mode in (['named','legacy'] if c and not dispatch and 0x44e7b0 in legacy else ['named']):
            reset(u,state);action_fixture(u,editor,flags,due,kind,links,delay,policy,full)
            if enumerate_events:
                u.mem_write(0x785d74,bytes(u.mem_read(Q,124)));word(u,0x785d74,0)
            state.update(watch=None,writes=[],packets=[])
            if dispatch:
                token,obj=(0x44ba90,0x785d70) if enumerate_events else (0x44c660,Q)
                if c:raw(c,u,token,obj)
                else:run(c,u,token,receiver=obj,budget=50000000)
            else:invoke(c,u,0x44e7b0,Q,[increase],mode,50000000)
            out=(tuple(state['packets']),snapshot(u),bytes(u.mem_read(Q,124)))
            if expected is None:expected=out
            else:
                if out!=expected:
                    diffs=[(hex(0x560000+i),a,b) for i,(a,b) in enumerate(zip(out[1],expected[1])) if a!=b][:16]
                    raise AssertionError(('flood action',editor,flags,due,kind,increase,links,delay,policy,full,dispatch,enumerate_events,mode,out[0],expected[0],diffs,out[2].hex(),expected[2].hex()))
            active=editor!=1 and not(flags&5) and due==0
            assert len(state['packets'])==(1+(links if delay==0 else 0) if active else 0),('expected published packets',len(state['packets']),links,delay)
            if active and links:assert get(u,0x785d70)==links+1+(links if delay==0 else 1)
    checked+=1

def actions():
    for editor,flags,due,increase in itertools.product([0,1,256,257],[0,1,4,8],[0,1,2],[0,1]):
        action(editor,flags,due,increase=increase)
    for kind,increase,policy,delay,full in itertools.product([21,22],[0,100,0xffffffff],range(8),[0,6],[False,True]):
        action(kind=kind,increase=increase,links=1,policy=policy,delay=delay,full=full)
    for kind,links,delay in itertools.product([21,22],[0,3,59],[0,6]):
        action(kind=kind,links=links,delay=delay,dispatch=True)
    for kind,links,delay in itertools.product([21,22],[0,3,59],[0,6]):
        action(kind=kind,links=links,delay=delay,dispatch=True,enumerate_events=True)
    print('PASS flood effects, real publisher and immediate scheduled chains through dispatcher:',checked,flush=True)

if __name__=='__main__':
    leaves();packets();actions()
    assert points<=hits,('unexecuted instructions',[hex(a) for a in sorted(points-hits)])
    missing={(a,b) for a,targets in branches.items() for b in targets if (a,b) not in edges}
    assert not missing,('unexecuted branch edges',[(hex(a),hex(b)) for a,b in sorted(missing)])
    sources=['include/event_packet.h','runtime/event_packet.c','runtime/event_message_kind.c','runtime/flood_event.c',
             'runtime/menu_event_actions.c','runtime/menu_event_model.c','include/menu_event_actions.h','CMakeLists.txt',
             'direct_c/verify_event_packet.py']+[f'src/unit_{i:05d}.c' for i in [641,712,727,747,767,768,779]]
    proof=dict(native_sha256=catalogue['native_sha256'],compiled_C_path=str(target.resolve()),compiled_C_sha256=target_sha,
               passed=True,checked=checked,invocations=invocations,methods_compared=[hex(a) for a in NAMES],
               source_hashes={p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest() for p in sources},
               coverage={hex(a):dict(instructions=len(x),executed=len(x&hits)) for a,x in owned.items()},
               conditional_edges=len(edges),controlled_algorithm_endpoints=[],controlled_platform_endpoints=[],
               limitations=['Mapped nonfaulting event/resource/calendar fixtures; no concurrency.',
                            'Void providers compare observable state, saved registers and ABI cleanup, not unused volatile registers/flags.',
                            'Flood scheduler chains reuse the existing event frame implementations; their unrelated event kinds are not approved here.',
                            'Publisher runs with sound disabled and nonmodal mode2, with empty/full queues. UI/audio rendering is outside this proof.'],commit=None)
    suffix='diagnostic' if target.stem=='restore_link_check' else 'game'
    (ROOT/f'metadata/event-packet-{suffix}.json').write_text(json.dumps(proof,indent=2)+'\n')
    print('PASS seven complete event methods, actual composed descendants:',checked,'cases',invocations,'invocations',flush=True)
    print('Verified target SHA256:',target_sha,flush=True)
