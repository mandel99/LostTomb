"""Eight whole native ring methods, public C entries and shared game providers.

No substituted endpoints; pop/full execute their real dependencies. Includes
unaligned objects, source/cursor aliases, full refusal with an unreadable source,
full DWORD count arithmetic and persistent push/pop sequences. No host game.
"""
from verify_options_startup import *
import json
from capstone import Cs,CS_ARCH_X86,CS_MODE_32,CS_GRP_JUMP

WORK,WORK_SIZE,PTR=S+0xfc0,0x1100,S+0x2000
METHODS={0x4f6520:'initialize',0x4f6560:'clear',0x4f6570:'contains',
         0x4f65b0:'push',0x4f65f0:'next',0x4f6620:'pop',0x4f6630:'count',0x4f6650:'full'}
ONE={0x4f6570,0x4f65b0,0x4f65f0}
available=lambda name:bool(re.search(r'\s_'+re.escape(name)+r'\s',symbols))
PROVIDERS={0x4f6520:['restore_ring_004f6520'],0x4f6560:['restore_ring_004f6560'],
  0x4f6570:['restore_ring_004f6570','restore_actor_496140_queue_contains'],
  0x4f65b0:['restore_actor_walking_queue_push'],0x4f65f0:['restore_ring_004f65f0'],
  0x4f6620:['restore_ring_004f6620'],0x4f6630:['restore_ring_004f6630','restore_actor_walking_queue_count'],
  0x4f6650:['restore_actor_walking_queue_full']}
legacy_entries=[token for token in METHODS if available('FUN_%08x'%token)]
providers_available={token:[name for name in names if available(name)] for token,names in PROVIDERS.items()}
if '--require-all' in sys.argv:
    assert len(legacy_entries)==8,legacy_entries
    assert sum(map(len,providers_available.values()))==10,providers_available
RANGES={0x4f6520:51,0x4f6560:9,0x4f6570:53,0x4f65b0:49,
        0x4f65f0:42,0x4f6620:10,0x4f6630:19,0x4f6650:23}
decoder=Cs(CS_ARCH_X86,CS_MODE_32);decoder.detail=True
owned={};branches={};visited=set();edges=set();previous=[None];invocations=0
for at,size in RANGES.items():
    decoded=list(decoder.disasm(bytes(images[0].data[at-images[0].base:at-images[0].base+size]),at))
    assert sum(x.size for x in decoded)==size
    owned[at]={x.address for x in decoded}
    for ins in decoded:
        if ins.group(CS_GRP_JUMP) and ins.mnemonic!='jmp':
            branches[ins.address]={ins.address+ins.size,ins.operands[0].imm}
def observe(cpu,pc,size,data):
    if previous[0] in branches:edges.add((previous[0],pc))
    if any(pc in addresses for addresses in owned.values()):visited.add(pc)
    previous[0]=pc
machines[0][1].hook_add(UC_HOOK_CODE,observe)

for c,u,state in machines:reset(u,state)
baseline=[snapshot(u) for c,u,state in machines]

def fixture(capacity=7,head=2,tail=5,alignment=0,cursor=2,source=0x89abcdef):
    u=machines[0][1];queue=S+0x1000+alignment
    u.mem_write(WORK,b'\xa5'*WORK_SIZE)
    put(u,queue+4,head);put(u,queue+8,tail);put(u,queue+12,capacity)
    for i in range(50):put(u,queue+20+i*4,(i*0x10204081)^0x80000000)
    put(u,PTR,cursor);put(u,PTR+4,source)
    return queue,bytes(u.mem_read(WORK,WORK_SIZE))

def exercise(token,queue,payload,args=(),providers=True):
    global invocations
    expected=None
    for c,u,state in machines:
        modes=['native'] if not c else (['legacy'] if token in legacy_entries else [])+['named']+(providers_available[token] if providers else [])
        for mode in modes:
            invocations+=1
            u.mem_write(WORK,payload)
            if mode in ('native','legacy'):
                result=run(c,u,token,args,receiver=queue,cleanup=4 if not c and token in ONE else 0)
            else:
                name='restore_actor_ring_'+METHODS[token] if mode=='named' else mode
                result=run(0,u,sym(name),[queue,*args])
            memory=bytes(u.mem_read(WORK,WORK_SIZE))
            if expected is None:expected=(result,memory)
            else:
                assert memory==expected[1],('queue state/guards',hex(token),mode,hex(queue),args)
                # The older map-facing clear wrapper is intentionally void.
                if mode!='restore_ring_004f6560':
                    assert result==expected[0],('queue full EAX',hex(token),mode,args,hex(result),hex(expected[0]))
    return expected

def case(token,capacity=7,head=2,tail=5,alignment=0,value=0x76543210,alias=None,cursor=2):
    queue,payload=fixture(capacity,head,tail,alignment,cursor)
    argument=value
    if token==0x4f65b0:argument=queue+alias if alias is not None else PTR+4
    elif token==0x4f65f0:
        argument=queue+alias if alias is not None else PTR
        if alias is not None:
            u=machines[0][1];put(u,argument,cursor);payload=bytes(u.mem_read(WORK,WORK_SIZE))
    return exercise(token,queue,payload,[argument] if token in ONE else [])

if __name__=='__main__':
    n=0
    for capacity in [1,2,3,7,17,50]:
        positions=sorted({0,capacity//2,capacity-1})
        for head,tail,alignment,token in itertools.product(positions,positions,range(4),METHODS):
            case(token,capacity,head,tail,alignment,cursor=head);n+=1
    print('PASS all eight public/named methods and existing game providers:',n,flush=True)
    for capacity,head,tail,value in itertools.product([2,7,50],[0,1],[0,1],[0,0x80000000,0x90204081,0xffffffff]):
        case(0x4f6570,capacity,head,tail,value=value);n+=1
    for alias,head,tail in itertools.product([0,4,8,12,16,20,216,220],[0,3],[0,6]):
        case(0x4f65b0,7,head,tail,alias=alias);n+=1
    for alias,cursor,alignment in itertools.product([0,4,8,12,16,20,24,216,220],[0,3,6],range(4)):
        case(0x4f65f0,7,2,5,alignment,alias=alias,cursor=cursor);n+=1
    for capacity,head,tail in [(1,0,0),(7,0,6),(7,3,2),(50,0,49)]:
        queue,payload=fixture(capacity,head,tail)
        result,_=exercise(0x4f65b0,queue,payload,[0xdead0000])
        assert result==0;n+=1
    for capacity,head,tail,token in itertools.product([0,1,50,0x7fffffff,0x80000000,0xffffffff],
            [0,0x7fffffff,0x80000000,0xffffffff],[0,0x7fffffff,0x80000000,0xffffffff],[0x4f6630,0x4f6650]):
        case(token,capacity,head,tail);n+=1
    print('PASS aliases, unreadable full-queue sources and DWORD count overflow:',n,flush=True)
    # Each operation consumes the preceding result rather than a fresh fixture.
    for capacity in [1,2,7,50]:
        queue,payload=fixture(capacity,0,0)
        for step in range(256):
            data=bytearray(payload)
            struct.pack_into('<I',data,PTR+4-WORK,(step*0x87654321)&0xffffffff)
            token=0x4f6620 if step%3==2 else 0x4f65b0
            args=[] if token==0x4f6620 else [PTR+4]
            _,payload=exercise(token,queue,bytes(data),args,providers=False);n+=1
            _,payload=exercise(0x4f6630,queue,payload,providers=False);n+=1
    for i,(c,u,state) in enumerate(machines):assert snapshot(u)==baseline[i],('global mutation',c)
    missing=sorted(set.union(*owned.values())-visited)
    missing_edges=sorted((a,b) for a,targets in branches.items() for b in targets if (a,b) not in edges)
    assert not missing,('unexecuted native instructions',[hex(a) for a in missing])
    assert not missing_edges,('unexecuted native conditional edges',[(hex(a),hex(b)) for a,b in missing_edges])
    sources=['include/actor_ring_queue.h','runtime/ring_queue.c','CMakeLists.txt',
        'runtime/actor_walking_queue_count.c','runtime/actor_walking_queue_full.c',
        'runtime/actor_walking_queue_push.c','runtime/actor_callback_496140_queue.c',
        'runtime/menu_drag_actor_create.c','runtime/map_commit_links.c','runtime/building_support.c',
        'direct_c/verify_actor_ring_queue.py']+[f'src/unit_{i:05d}.c' for i in range(2185,2193)]
    report=dict(native_sha256=hashlib.sha256(original.read_bytes()).hexdigest(),
        compiled_C_path=str(target.resolve()),compiled_C_sha256=target_sha,
        source_hashes={p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest() for p in sources},
        checked=n,invocations=invocations,passed=True,controlled_algorithm_endpoints=[],
        legacy_entries_tested=[hex(x) for x in legacy_entries],providers_tested=providers_available,
        coverage={hex(a):dict(bytes=RANGES[a],instructions=len(points),executed=len(points&visited)) for a,points in owned.items()},
        native_conditional_edges=sum(map(len,branches.values())),missing_native_instructions=[],missing_native_edges=[],
        scope=['Eight complete native methods and named C APIs; linked legacy ECX bridges/providers listed separately',
               'Full EAX except the intentionally void map-facing clear wrapper; full workspace, guard bytes and stack/NV contract',
               'Mapped valid objects, finite valid scans, capacities1..50, source/cursor aliases and unaligned objects',
               'Count/full also compare arbitrary DWORD head/tail/capacity arithmetic',
               'Four independent512-operation persistent sequences; unreadable full-queue source pointers'],
        limitations=['Does not certify every actor simulation caller, global queue construction or full gameplay.',
                     'No concurrent memory mutation, invalid-object handling, EFLAGS or FP status equivalence claimed.'],commit=None)
    report_name='actor-ring-queue-diagnostic.json' if target.stem=='restore_link_check' else 'actor-ring-queue-reconstruction.json'
    (ROOT/'metadata'/report_name).write_text(json.dumps(report,indent=2)+'\n')
    print('PASS whole native/C queues, linked entries, all providers and sequences:',n,flush=True)
    print('Native instruction/conditional-edge coverage:',len(visited),sum(map(len,branches.values())),flush=True)
    print('Verified target SHA256:',target_sha,flush=True)
