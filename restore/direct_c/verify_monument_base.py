"""Whole monument base queries and real worker predicates, without stubs."""
from verify_options_startup import *
import json
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
from unicorn import UC_HOOK_MEM_WRITE

OBJ,OUT,BID,AID=S+0x1000,S+0x2000,20,3
QUERIES={0x40d940:'restore_monument_base_0040d940',0x417050:'restore_monument_base_00417050',0x4e6710:'restore_monument_base_004e6710'}
TABLES={0x40d940:0x56f48c,0x417050:0x56f4dc,0x4e6710:0x56f6e8}
WORKERS={0x40ec20,0x417590,0x4e6b20}
available=lambda n:bool(re.search(r'\s_'+re.escape(n)+r'\s',symbols))
methods=set(QUERIES)|WORKERS
legacy={a for a in methods if available('FUN_%08x'%a)}
worker_result=available('restore_monument_base_worker_result')
if '--require-all' in sys.argv:assert legacy==methods and worker_result
catalogue=json.loads((ROOT/'metadata/monument-base-native.json').read_text())
assert catalogue['native_sha256']==hashlib.sha256(original.read_bytes()).hexdigest()
owned={};branches={};hits=set();edges=set();previous=[None];checked=0;invocations=0
for f in catalogue['functions']:
    a=int(f['address'],16);n=f['result']['procedure']['body']['total_bytes']
    code=list(Cs(CS_ARCH_X86,CS_MODE_32).disasm(bytes(images[0].data[a-images[0].base:a-images[0].base+n]),a))
    assert sum(i.size for i in code)==n;owned[a]={i.address for i in code}
    branches.update({i.address:{int(i.op_str,16),i.address+i.size} for i in code if i.mnemonic.startswith('j') and i.mnemonic!='jmp'})
points=set.union(*owned.values())
for c,u,state in machines:
    state['ordered']=[]
    def writes(cpu,access,at,size,value,data):
        if 0x560000<=at<0x126d000 or S<=at<S+0x3000:
            data['ordered'].append((at,size,value&((1<<(size*8))-1)))
    u.hook_add(UC_HOOK_MEM_WRITE,writes,state)
    if not c:
        def observe(cpu,pc,size,data):
            if pc in points:hits.add(pc)
            if previous[0] in branches:edges.add((previous[0],pc))
            previous[0]=pc
        u.hook_add(UC_HOOK_CODE,observe,state)

def fixture(u,table,stage,coefficient,delta,actor_type,actor_state,secondary,active,missing,cw,depth):
    u.mem_write(0x7999d8+(BID-2)*264,bytes(5*264));u.mem_write(0xbcd5f0+AID*388,bytes(2*388))
    u.mem_write(OBJ,b'\xa5'*96);u.mem_write(OUT-8,b'\x6b'*80)
    put(u,OBJ,table);put(u,OBJ+4,coefficient)
    root=BID-delta;word(u,0x7999ea+BID*264,delta)
    put(u,0x799a30+root*264,stage);put(u,0x799a34+root*264,0 if missing else AID)
    byte(u,0xbcd602+AID*388,actor_type);byte(u,0xbcd606+AID*388,active)
    byte(u,0xbcd630+AID*388,actor_state);byte(u,0xbcd608+AID*388,secondary)
    for reg,value in [(UC_X86_REG_FPCW,cw),(UC_X86_REG_FPSW,0),(UC_X86_REG_FPTAG,0xffff)]:u.reg_write(reg,value)
    u.mem_write(S+0x4800,b'\xd9\xe8'*2)
    if depth:u.emu_start(S+0x4800,S+0x4800+depth*2)

def call(c,u,token,output,mode,eax,cw,depth):
    global invocations
    invocations+=1
    if not c:entry=token;argv=[BID,output] if token in QUERIES else [BID];cleanup=8 if token in QUERIES else 4
    elif mode=='legacy':entry=address(c,token);argv=[BID,output] if token in QUERIES else [BID];cleanup=0
    elif token in QUERIES:entry=sym(QUERIES[token]);argv=[OBJ,BID,output];cleanup=0
    elif worker_result:entry=sym('restore_monument_base_worker_result');argv=[token,BID,eax];cleanup=0
    else:entry=sym('restore_monument_base_worker');argv=[token,BID];cleanup=0
    top=(u.reg_read(UC_X86_REG_FPSW)>>11)&7;tags=u.reg_read(UC_X86_REG_FPTAG)
    guard=[u.reg_read(UC_X86_REG_FP0+i) for i in range(8)]
    for r,v in zip(registers,[eax,OBJ,0x98765432,saved[0],SP,*saved[1:]]):u.reg_write(r,v)
    u.reg_write(UC_X86_REG_EFLAGS,0x202);put(u,SP,STOP)
    for i,v in enumerate(argv):put(u,SP+4+i*4,v)
    u.emu_start(entry,STOP,count=1000000)
    assert u.reg_read(UC_X86_REG_EIP)==STOP,(c,hex(token),mode,hex(u.reg_read(UC_X86_REG_EIP)))
    assert u.reg_read(UC_X86_REG_ESP)==SP+4+cleanup
    assert [u.reg_read(registers[i]) for i in [3,5,6,7]]==saved
    assert u.reg_read(UC_X86_REG_FPCW)==cw
    assert (u.reg_read(UC_X86_REG_FPSW)>>11)&7==top
    assert u.reg_read(UC_X86_REG_FPTAG)==tags
    if depth:
        for i in range(8):
            if ((tags>>(i*2))&3)!=3:assert u.reg_read(UC_X86_REG_FP0+i)==guard[i]
    result=u.reg_read(UC_X86_REG_EAX)
    return result&255 if c and token in WORKERS and mode=='named' and not worker_result else result

def exercise(token,stage=3,coefficient=2,delta=0,actor_type=0x51,actor_state=10,secondary=9,active=1,missing=False,
             output=OUT,table=None,cw=0x37f,depth=0,eax=0x89abcdef,normalize_return=False):
    global checked
    expected=None
    if table is None:table=TABLES.get(token,0x56f48c)
    for c,u,state in machines:
        for mode in (['named','legacy'] if c and token in legacy else ['named']):
            reset(u,state);fixture(u,table,stage,coefficient,delta,actor_type,actor_state,secondary,active,missing,cw,depth)
            state['ordered']=[];value=call(c,u,token,output,mode,eax,cw,depth)
            if normalize_return:value&=255
            out=(value,tuple(state['ordered']),bytes(u.mem_read(OBJ,96)),bytes(u.mem_read(OUT-8,80)),
                 bytes(u.mem_read(0x7999d8+(BID-2)*264,5*264)),bytes(u.mem_read(0xbcd5f0+AID*388,2*388)))
            if expected is None:expected=out
            else:assert out==expected,('monument',hex(token),stage,coefficient,delta,actor_type,actor_state,secondary,active,missing,hex(output),hex(table),hex(cw),depth,mode,'values',out[0],expected[0],'writes',out[1],expected[1])
    checked+=1

def worker_cases():
    for token,kind,state in itertools.product(sorted(WORKERS),[0,0x4f,0x51,0x55,255],[0,6,7,9,10,11,12,15,16,31,32,33,34,35,255]):
        exercise(token,actor_type=kind,actor_state=state,normalize_return=not worker_result)
    for token,kind,state,secondary in itertools.product(sorted(WORKERS),[0x4f,0x51,0x55],[6,7],[0,9,10,15,34]):
        exercise(token,actor_type=kind,actor_state=state,secondary=secondary,normalize_return=not worker_result)
    for token,active,missing in itertools.product(sorted(WORKERS),[0,255],[False,True]):
        exercise(token,active=active,missing=missing,normalize_return=not worker_result)
    # Different root and child slots distinguish conditional root normalization.
    for token,stage,delta in itertools.product(sorted(WORKERS),[-1,2,3,20],[-1,1]):
        exercise(token,stage=stage,delta=delta,actor_type=0x55,actor_state=34,normalize_return=not worker_result)
    print('PASS actual worker predicates, whole EAX/AL, travel states and stale-link cleanup:',checked,flush=True)

def query_cases():
    for token,stage,coefficient,output in itertools.product(QUERIES,[-1,0,1,2,3,4,8,9,18,19,20,21,22,38,0x7fffffff,0x80000000],[0,2,0xffffffff],[0,OUT]):
        exercise(token,stage=stage,coefficient=coefficient,missing=True,output=output)
    for token,table,kind,state in itertools.product(QUERIES,TABLES.values(),[0x4f,0x51,0x55],[6,7,9,10,11,15,16,31,34,35]):
        exercise(token,table=table,actor_type=kind,actor_state=state,secondary=10)
    for token,delta in itertools.product(QUERIES,[-1,1]):exercise(token,delta=delta)
    for token,output in itertools.product(QUERIES,[OBJ,OBJ+4,0x799a34+BID*264,0xbcd602+AID*388,OUT+1]):
        exercise(token,output=output)
    for cw,stage in itertools.product([0x7f,0x27f,0x37f,0x47f,0x67f,0x77f,0x87f,0xa7f,0xb7f,0xc7f,0xe7f,0xf7f],[1,20,0x80000000]):
        exercise(0x40d940,stage=stage,cw=cw,depth=2)
    print('PASS progress/cost outputs, real virtual predicates, float conversion, aliases and x87 guards:',checked,flush=True)

if __name__=='__main__':
    worker_cases();query_cases()
    # With each of the three actual shipped predicates, AL1 implies a
    # nonzero worker link and type4F/51/55; successful predicates never write
    # the slot/type. The defensive null-after-success and unknown-type arms
    # therefore require a different callback or concurrent mutation.
    dead={0x40d9c4,0x40d9c5,0x40d9c6,0x40d9c7,0x40d9c9,0x40d9ca,0x40d9cd,
          0x4170ba,0x4170bb,0x4170bc,0x4170bd,0x4170bf,0x4170c0,0x4170c3,
          0x4e6778,0x4e6779,0x4e677a,0x4e677b,0x4e677d,0x4e677e,0x4e6781}
    excluded={(0x40d9c2,0x40d9c4),(0x4170b8,0x4170ba),(0x4e6776,0x4e6778),
              (0x40d9f0,0x40da1a),(0x4170e6,0x417110),(0x4e67a4,0x4e67d2)}
    assert points-dead<=hits,('unexecuted instructions',[hex(a) for a in sorted(points-dead-hits)])
    missing={(a,b) for a,targets in branches.items() for b in targets if (a,b) not in edges}-excluded
    assert not missing,('unexecuted branch edges',[(hex(a),hex(b)) for a,b in sorted(missing)])
    sources=['include/monument_base_queries.h','runtime/monument_base_queries.c','runtime/campaign_score_numeric.c',
             'CMakeLists.txt','direct_c/verify_monument_base.py']+[f'src/unit_{i:05d}.c' for i in [105,109,200,203,1999,2003]]
    proof=dict(native_sha256=catalogue['native_sha256'],compiled_C_path=str(target.resolve()),compiled_C_sha256=target_sha,
               passed=True,checked=checked,invocations=invocations,legacy_entries_compared=[hex(a) for a in sorted(legacy)],
               coverage={hex(a):dict(instructions=len(x),executed=len(x&hits)) for a,x in owned.items()},conditional_edges=len(edges),
               source_hashes={p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest() for p in sources},
               unreachable_instructions=[hex(a) for a in sorted(dead)],unreachable_edges=[(hex(a),hex(b)) for a,b in sorted(excluded)],
               controlled_algorithm_endpoints=[],controlled_platform_endpoints=[],
               limitations=['Mapped nonfaulting building/actor records, three actual shipped vtables and no concurrency.',
                            'Defensive query arms are unreachable with genuine shipped worker predicates; custom callbacks are outside this proof.',
                            'Masked x87 exceptions; CW/TOP/tags and live guards retained. FPSW/trap equivalence is not claimed.',
                            'Other monument simulation, building placement and full gameplay are not approved here.'],commit=None)
    suffix='diagnostic' if target.stem=='restore_link_check' else 'game'
    (ROOT/f'metadata/monument-base-{suffix}.json').write_text(json.dumps(proof,indent=2)+'\n')
    print('PASS six whole monument query/worker entries:',checked,'cases',invocations,'invocations',flush=True)
    print('Native executed/unreachable instructions:',len(hits&points),len(dead),flush=True)
    print('Verified target SHA256:',target_sha,flush=True)
