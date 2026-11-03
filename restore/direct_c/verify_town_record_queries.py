"""Nine complete event/town leaves, explicit C inputs and retained facades.

No controlled algorithm/platform endpoints. Invalid native lookup reads2:
compare that fault with an unmapped low page, and its continuation with an
explicit mapped low-page fixture. The global gameplay facade retains its
separate pre-existing index guard. No UI or host game is started.
"""
from verify_options_startup import *
import json
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
from unicorn import UC_HOOK_MEM_WRITE,UC_HOOK_MEM_INVALID,UcError

Q,L,G=S+0x1000,S+0x10000,0x785d70
NAMES={0x448a60:'restore_town_actor_links_00448a60',0x449640:'restore_event_kind_is_1_19_20',
       0x449660:'restore_event_kind2_range_at_least9',0x449690:'restore_event_kind1_mode3',
       0x44d680:'restore_event_due_result',0x44d6d0:'restore_event_is_future_result',
       0x44d700:'restore_event_within_window_result',0x44bb40:'restore_event_list_record_kind2',
       0x44bd00:'restore_event_list_count_kind3'}
SIZES={0x448a60:35,0x449640:21,0x449660:40,0x449690:18,0x44d680:66,
       0x44d6d0:48,0x44d700:80,0x44bb40:71,0x44bd00:24}
EAX_INPUT={0x449640,0x449660,0x449690,0x44d680,0x44d700}
available=lambda name:bool(re.search(r'\s_'+re.escape(name)+r'\s',symbols))
legacy={a for a in NAMES if available('FUN_%08x'%a)}
supported={a for a in NAMES if available(NAMES[a])}
if '--require-all' in sys.argv:assert legacy==supported==set(NAMES),(legacy,supported)
owned={};branches={};hits=set();edges=set();previous=[None];invocations=0
for a,n in SIZES.items():
    code=list(Cs(CS_ARCH_X86,CS_MODE_32).disasm(bytes(images[0].data[a-images[0].base:a-images[0].base+n]),a))
    assert sum(i.size for i in code)==n
    owned[a]={i.address for i in code}
    branches.update({i.address:{int(i.op_str,16),i.address+i.size} for i in code if i.mnemonic.startswith('j') and i.mnemonic!='jmp'})
points=set.union(*owned.values())
for c,u,state in machines:
    reset(u,state);u.mem_map(0,4096);state['low_mapped']=True
    state['writes']=[];state['faults']=[]
    def writes(cpu,access,at,size,value,data):
        if 0x30000000<=at<0x30010000:return
        data['writes'].append((at,size,value))
    def invalid(cpu,access,at,size,value,data):data['faults'].append((at,size));return False
    u.hook_add(UC_HOOK_MEM_WRITE,writes,state);u.hook_add(UC_HOOK_MEM_INVALID,invalid,state)
    if not c:
        def visit(cpu,pc,size,data):
            if pc in points:hits.add(pc)
            if previous[0] in branches:edges.add((previous[0],pc))
            previous[0]=pc
        u.hook_add(UC_HOOK_CODE,visit)

def invoke(c,u,state,token,obj,args,incoming,mode):
    global invocations
    invocations+=1
    if mode=='named':
        entry=sym(NAMES[token]);argv=[obj,*args]
        if token in EAX_INPUT:argv.append(incoming)
    elif mode=='due-byte':entry=sym('restore_event_record_due_0044d680');argv=[obj,*args]
    else:entry=address(c,token);argv=args
    for reg,value in zip(registers,[incoming,obj,0x789abcde,saved[0],SP,*saved[1:]]):u.reg_write(reg,value)
    u.reg_write(UC_X86_REG_EFLAGS,0x202);put(u,SP,STOP)
    for i,v in enumerate(argv):put(u,SP+4+i*4,v)
    try:u.emu_start(entry,STOP,count=200000)
    except UcError:
        assert state['faults'] and state['faults'][-1][0]==2,('unexpected fault',c,hex(token),mode,state['faults'])
        return ('fault',2)
    assert u.reg_read(UC_X86_REG_EIP)==STOP,(c,hex(token),mode)
    cleanup=4 if not c and token in [0x44d680,0x44bb40] else 0
    assert u.reg_read(UC_X86_REG_ESP)==SP+4+cleanup
    assert [u.reg_read(registers[i]) for i in [3,5,6,7]]==saved
    value=u.reg_read(UC_X86_REG_EAX)
    if mode=='due-byte':value&=255
    return None if token==0x448a60 else value

def exercise(token,kind=2,primary=-1,secondary=-1,fallback=9,modebyte=3,offset=0,base=0,current=0,
             month=0,current_month=0,flags=0,ignore=0,duration=2,incoming=0xa5b6c7d8,alignment=0,
             count=4,index=0,pattern=0,global_object=False,unmapped=False):
    if token not in supported:return 0
    obj=(G if global_object else L+alignment) if token in [0x44bb40,0x44bd00] else Q+alignment
    expected=None
    for c,u,state in machines:
        modes=['legacy'] if not c else ['named']+(['legacy'] if token in legacy and (token not in [0x44bb40,0x44bd00] or global_object) else [])
        if c and token==0x44d680 and available('restore_event_record_due_0044d680'):modes.append('due-byte')
        for call_mode in modes:
            if unmapped and state['low_mapped']:u.mem_unmap(0,4096);state['low_mapped']=False
            if not unmapped and not state['low_mapped']:u.mem_map(0,4096);state['low_mapped']=True
            if not unmapped:byte(u,2,kind)
            if token in [0x44bb40,0x44bd00]:
                u.mem_write(obj-8,b'\x5a'*(0x4900+16));put(u,obj,count)
                for i in range(150):byte(u,obj+6+i*124,3 if pattern==1 else (i%4) if pattern==2 else kind)
                args=[index] if token==0x44bb40 else []
                extent=0x4900
            else:
                u.mem_write(obj-8,b'\xa5'*144)
                byte(u,obj+2,kind);byte(u,obj+3,month);word(u,obj+0x14,offset)
                Uvals=[(obj+0x28,flags),(0x7848d8,(base&65535)|0xdead0000),(0x78d7bc,current),(0x78d7c0,current_month)]
                for a,v in Uvals:put(u,a,v)
                for a,v in [(obj+0x1e,primary),(obj+0x20,secondary),(obj+0x22,fallback),(obj+0x2c,duration)]:word(u,a,v)
                byte(u,obj+0x60,modebyte);args=[ignore] if token==0x44d680 else [];extent=128
            state['writes']=[];state['faults']=[]
            value=invoke(c,u,state,token,obj,args,incoming,call_mode)
            current_state=(value,tuple(state['writes']),bytes(u.mem_read(obj-8,extent+16)),
                           get(u,0x7848d8),get(u,0x78d7bc),get(u,0x78d7c0))
            if expected is None:expected=current_state
            elif call_mode=='due-byte':
                assert current_state[0]==expected[0]&255 and current_state[1:]==expected[1:],('byte facade',expected,current_state)
            elif token==0x44bb40 and call_mode=='legacy' and index!=140 and (index<0 or index>=count):
                assert value==0 and not state['faults'] and current_state[1:]==expected[1:],('guarded global facade',index,count)
            else:assert current_state==expected,('record query',hex(token),kind,primary,secondary,fallback,offset,base,current,month,current_month,flags,ignore,duration,hex(incoming),alignment,count,index,global_object,unmapped,call_mode,current_state[:2],expected[:2])
    return 1

if __name__=='__main__':
    total=0
    for kind in range(256):
        for token in [0x449640,0x449660,0x449690]:total+=exercise(token,kind=kind,incoming=0xffffffff if kind&1 else 0x12345678,modebyte=kind%4)
    for modebyte,incoming in itertools.product([0,1,2,3,4,255],[0,0xffffffff]):
        total+=exercise(0x449690,kind=1,modebyte=modebyte,incoming=incoming,alignment=3)
    for primary,secondary,fallback,incoming in itertools.product([-32768,-1,0,8,9,32767],[-1,0],[-1,8,9,32767],[0,0xabcdef01]):
        total+=exercise(0x449660,primary=primary,secondary=secondary,fallback=fallback,incoming=incoming,alignment=3)
    for offset,base in [(-32768,-32768),(-1,-1000),(0,0),(300,-1000),(32767,32767),(0,32767)]:
        for delta,(month,current_month) in itertools.product([-1,0,1],[(0,0),(11,11),(255,255),(0,256),(1,0),(11,0x80000000)]):
            current=offset+base+delta
            for token in [0x44d680,0x44d6d0,0x44d700]:total+=exercise(token,offset=offset,base=base,current=current,month=month,current_month=current_month,duration=11,alignment=1)
    for token,flags,ignore,incoming in itertools.product([0x44d680,0x44d6d0,0x44d700],[0,1,4,5,16,17,0x100,0x80000000],[0,1,256],[0,0xffffffff]):
        total+=exercise(token,flags=flags,ignore=ignore,incoming=incoming)
    for duration,current in itertools.product([-32768,-1,0,1,32767],[0,1,0x7fffffff,0x80000000,0xffffffff]):
        total+=exercise(0x44d700,offset=-32768,base=-32768,current=current,duration=duration,month=11,current_month=255)
    for token,alignment in itertools.product([0x448a60,0x449640,0x449660,0x449690,0x44d680,0x44d6d0,0x44d700],range(4)):
        total+=exercise(token,alignment=alignment)
    for count,pattern,global_object in itertools.product([-1,0,1,4,140,150],range(3),[False,True]):
        total+=exercise(0x44bd00,count=count,pattern=pattern,global_object=global_object)
    for count,index,kind,global_object in itertools.product([-1,0,4,140],[0,3,140],[0,2,3],[False,True]):
        total+=exercise(0x44bb40,count=count,index=index,kind=kind,global_object=global_object,alignment=3)
    for index,unmapped,global_object in itertools.product([-1,4,0x7fffffff],[False,True],[False,True]):
        total+=exercise(0x44bb40,count=4,index=index,unmapped=unmapped,global_object=global_object)
    expected_points=set.union(*(owned[a] for a in supported));checked_branches={a:b for a,b in branches.items() if a in expected_points}
    assert expected_points<=hits,('unexecuted native instructions',sorted(expected_points-hits))
    assert all((a,b) in edges for a,targets in checked_branches.items() for b in targets),('native conditional edges',sorted((a,b) for a,targets in checked_branches.items() for b in targets if (a,b) not in edges))
    sources=['include/town_record_queries.h','runtime/town_record_queries.c','runtime/editor_event_list_actions.c','runtime/mission_unlock.c',
             'runtime/actor_mission_init.c','include/actor_mission_init.h','CMakeLists.txt','direct_c/verify_town_record_queries.py']
    sources += [f'src/unit_{i:05d}.c' for i in [684,708,709,710,719,723,744,745,746]]
    proof=dict(native_sha256=hashlib.sha256(original.read_bytes()).hexdigest(),compiled_C_path=str(target.resolve()),compiled_C_sha256=target_sha,
        passed=True,checked=total,invocations=invocations,methods_compared=[hex(a) for a in sorted(supported)],legacy_entries_linked=[hex(a) for a in sorted(legacy)],
        source_hashes={p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest() for p in sources},
        coverage={hex(a):dict(bytes=SIZES[a],instructions=len(x),executed=len(x&hits)) for a,x in owned.items()},conditional_edges=sum(map(len,checked_branches.values())),
        controlled_algorithm_endpoints=[],controlled_platform_endpoints=[],
        scope=['Nine whole methods when linked; named C interfaces and retained legacy adapters/facades are compared separately',
               'All BYTE kinds, signed WORD ranges/base years, DWORD calendar/month, inherited EAX, exact field writes and normalized stack cleanup',
               'Actual future-date dependency executes; invalid raw lookup fault/low-page continuation and separate existing guarded global facade'],
        limitations=['Mapped objects/positive list extents except explicitly checked native low-page faults; no concurrent mutation.',
                     'Legacy lookup/count facades intentionally select the known global receiver; private objects use named APIs.',
                     'Existing CPU-frame caller models and complete event simulation/UI are not approved by this comparison.'],commit=None)
    suffix='diagnostic' if target.stem=='restore_link_check' else 'game'
    (ROOT/f'metadata/town-record-queries-{suffix}.json').write_text(json.dumps(proof,indent=2)+'\n')
    print('PASS whole event/town queries, partial returns, calendar and guarded facades:',total,flush=True)
    print('Native instruction/conditional-edge coverage:',len(hits),sum(map(len,checked_branches.values())),flush=True)
    print('Verified target SHA256:',target_sha,flush=True)
