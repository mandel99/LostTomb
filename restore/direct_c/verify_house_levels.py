"""Twenty native/C housing callbacks with actual queries and layout helpers.

These are void callbacks: the native4528B0 owner reloads EAX immediately after
its indirect call. Compare ordered game-state writes, query inputs, saved
registers and frame balance, without prescribing volatile return registers.
"""
from verify_house_evolution import *

catalogue=json.loads((ROOT/'metadata/house-levels-native.json').read_text())
assert catalogue['native_sha256']==hashlib.sha256(original.read_bytes()).hexdigest()
base=int(catalogue['address'],16);raw=bytes.fromhex(catalogue['bytes_hex'])
assert raw==bytes(images[0].data[base-images[0].base:base-images[0].base+len(raw)])
level_owned={};level_branches={};level_hits=set();level_edges=set();level_last=[None]
entries=[];checked=invocations=owner_compositions=0
for f in catalogue['functions']:
    a=int(f['address'],16);entries.append(a);b=raw[a-base:a-base+f['body_bytes']]
    level_owned[a]={int(p,16) for p in f['instruction_addresses'].split()}
    instructions=list(decoder.disasm(b,a));assert {i.address for i in instructions}==level_owned[a]
    for i in instructions:
        if i.mnemonic.startswith('j') and i.mnemonic!='jmp':level_branches[i.address]={int(i.op_str,16),i.address+i.size}
level_points=set.union(*level_owned.values())
table=catalogue['callback_table'];at=int(table['address'],16);table_raw=bytes.fromhex(table['bytes_hex'])
assert table_raw==bytes(images[0].data[at-images[0].base:at-images[0].base+len(table_raw)])
assert struct.unpack('<20I',table_raw)==tuple(entries)
for c,u,state in machines:
    state.update(calls=[],compiled=c,mutation='')
    def query(uc,pc,size,data):
        name,n=data['query_entries'][pc];sp=uc.reg_read(UC_X86_REG_ESP)
        args=tuple(get(uc,sp+4+i*4) for i in range(n))
        data['calls'].append((name,args,get(uc,0x78d838),get(uc,0x78d7e4)))
        if data['mutation']==name:
            if name=='requirements':put(uc,0x78d7e4,0)
            else:put(uc,0x78d838,BID+1)
    state['query_entries']={sym(names[t]) if c else t:(name,n) for t,name,n in [(0x4539a0,'mode',0),(0x453af0,'desirability',1),(0x453b40,'requirements',2)]}
    for at in state['query_entries']:u.hook_add(UC_HOOK_CODE,query,state,begin=at,end=at)
    if not c:
        def observe_level(uc,pc,size,data):
            level_hits.add(pc)
            if level_last[0] in level_branches:level_edges.add((level_last[0],pc))
            level_last[0]=pc
        for rows in level_owned.values():u.hook_add(UC_HOOK_CODE,observe_level,state,begin=min(rows),end=max(rows))

def prepare(u,level,desire,delay,population,mode,blocked,current_ok,next_ok,model_level,special):
    # Layout changes can allocate additional houses; reset the allocator and
    # actual record array rather than replacing allocation or map providers.
    for at,n in [(0x78d580,0x320),(0x7999d8,4000*264),(0xd3b1c4,0x1d0)]:u.mem_write(at,seed[at-0x560000:at-0x560000+n])
    actual_level=level if model_level is None else model_level
    fixture(u,level=actual_level,next_level=1,size=1,desire=desire,global_mode=mode,special=special)
    row=0x70cdcc+actual_level*88
    put(u,row+8,0 if current_ok else 1);put(u,row+88+8,0 if next_ok else 1)
    for j in [BID,BID+1]:
        r=j*264
        if j!=BID:u.mem_write(0x7999d8+r,bytes(u.mem_read(0x7999d8+BID*264,264)))
        byte(u,0x7999d8+r,1);byte(u,0x7999dd+r,1);word(u,0x7999f4+r,population)
        byte(u,0x799a5c+r,delay);word(u,0x7999e8+r,level+10)
    for start,scale in [(0xb285cc,1),(0xb67d1c,1),(0xacf85c,1),(0xb7482c,4),(0xaf598c,4),(0xb4e6fc,2)]:u.mem_write(start+18000*scale,bytes(6000*scale))
    if blocked:
        u.mem_write(0xaf598c+18000*4,struct.pack('<I',1)*6000)
    for row_i in range(4):
        for col_i in range(4):
            cell=19330+row_i*228+col_i
            put(u,0xaf598c+cell*4,8 if not blocked else 1);word(u,0xb4e6fc+cell*2,BID)
    # Allocation computes real nearest-road/water distances. Provide nearby
    # terrain so this fixture avoids an irrelevant full-map spiral per parcel.
    put(u,0xaf598c+19329*4,4);put(u,0xaf598c+(19329+228)*4,0x40000)
    # Native allocation refreshes quality before assigning the new record's
    # coordinates, so its first distance queries originate at (0,0).
    put(u,0xaf598c+1001*4,4);put(u,0xaf598c+1228*4,0x40000)
    put(u,0xbca2d0,200);put(u,0xbca2d4,200)
    for i in range(36):put(u,0x5cc8c0+i*8,(i//6)*228+i%6)
    for reg,v in [(UC_X86_REG_FPCW,0x37f),(UC_X86_REG_FPSW,0),(UC_X86_REG_FPTAG,0xffff)]:u.reg_write(reg,v)

def call_level(c,u,level,route):
    global invocations
    invocations+=1;token=entries[level]
    if not c:run(0,u,token);return
    if route=='named':run(0,u,sym('restore_house_update_level'),[level]);return
    u.mem_write(VSP-256,b'\x6b'*320);put(u,VSP,STOP)
    u.mem_write(FRAME,struct.pack('<10I',0x89abcdef,0x87654321,0x98765432,saved[0],VSP,*saved[1:],0x202,0))
    if route=='frame':run(0,u,sym('restore_building_control_C_%08x'%token),[FRAME])
    else:run(0,u,sym('restore_building_control_raw'),[token,FRAME,FRAME+32])
    assert get(u,FRAME+16)==VSP+4 and [get(u,FRAME+i*4) for i in [3,5,6,7]]==saved
    assert bytes(u.mem_read(VSP-256,64))==b'\x6b'*64 and bytes(u.mem_read(VSP+4,48))==b'\x6b'*48

def exercise_level(level,desire=0,delay=0,population=10,mode=0,blocked=True,current_ok=True,next_ok=True,model_level=None,special=1,mutation=''):
    global checked
    expected=None
    for c,u,state in machines:
        for route in ['named','frame','raw'] if c else ['native']:
            prepare(u,level,desire,delay,population,mode,blocked,current_ok,next_ok,model_level,special)
            state.update(writes=[],calls=[],mutation=mutation);level_last[0]=None
            try:call_level(c,u,level,route)
            except Exception as e:raise AssertionError(('execution',level,desire,delay,population,mode,blocked,current_ok,next_ok,model_level,special,mutation,route,hex(u.reg_read(UC_X86_REG_EIP)))) from e
            result=(tuple(state['writes']),tuple(state['calls']))
            if expected is None:expected=result
            else:
                differences=[(i,(hex(a[0]),*a[1:]),(hex(b[0]),*b[1:])) for i,(a,b) in enumerate(zip(expected[0],result[0])) if a!=b][:8]
                assert result==expected,('house level',level,desire,delay,population,mode,blocked,current_ok,next_ok,model_level,special,mutation,route,'write lengths',len(expected[0]),len(result[0]),'first differences',differences,'queries',expected[1],result[1])
    checked+=1

def housing_owner(populated):
    global owner_compositions
    expected=None
    for c,u,state in machines:
        prepare(u,0,0,0,10,0,True,True,True,None,1)
        u.mem_write(0x7999d8,bytes(4000*264));u.mem_write(0x70cdcc,bytes(22*88))
        for level in range(20):
            put(u,0x70cdcc+level*88,-128);put(u,0x70cdd0+level*88,1000)
            if populated:
                r=(BID+level)*264
                byte(u,0x7999d8+r,1);byte(u,0x7999db+r,1);byte(u,0x7999dc+r,1);byte(u,0x7999dd+r,1)
                word(u,0x7999e8+r,level+10);word(u,0x7999ea+r,level);word(u,0x7999f4+r,10)
                cell=19330+level
                word(u,0x7999de+r,90+level);word(u,0x7999e0+r,80);put(u,0x7999e4+r,cell)
                word(u,0xb4e6fc+cell*2,BID+level);put(u,0xaf598c+cell*4,8)
        put(u,0x78d7c4,1);state.update(writes=[],calls=[],mutation='');level_last[0]=None
        run(0,u,sym('FUN_004528b0') if c else 0x4528b0,budget=5000000)
        result=(tuple(state['writes']),tuple(state['calls']))
        if expected is None:expected=result
        else:
            differences=[(i,a,b) for i,(a,b) in enumerate(zip(expected[0],result[0])) if a!=b][:8]
            assert result==expected,('owner',populated,'write counts',len(expected[0]),len(result[0]),differences,'query counts',len(expected[1]),len(result[1]))
    owner_compositions+=1

if __name__=='__main__':
    for level in range(20):
        for desire,delay,current_ok,next_ok in itertools.product([-20,0,20],[0,1,2,255],[False,True],[False,True]):exercise_level(level,desire,delay,current_ok=current_ok,next_ok=next_ok)
        for mode,population in itertools.product([0,1,255],[0,-1,1,32767]):exercise_level(level,mode=mode,population=population)
        for mutation in ['mode','desirability','requirements']:exercise_level(level,desire=20,mutation=mutation)
    for level in [0,1,9,13,17]:
        for blocked,special in itertools.product([False,True],[0,1]):exercise_level(level,desire=20,blocked=blocked,special=special)
    exercise_level(19,desire=20,model_level=18)
    for populated in [False,True]:housing_owner(populated)
    missing={(a,b) for a,targets in level_branches.items() for b in targets if (a,b) not in level_edges}
    assert level_points<=level_hits,('unexecuted instructions',[hex(a) for a in sorted(level_points-level_hits)])
    assert not missing,('unexecuted conditional edges',[(hex(a),hex(b)) for a,b in sorted(missing)])
    sources=['include/house_levels.h','runtime/house_levels.c','runtime/building_controllers.c','runtime/building_controllers_model.c','runtime/house_evolution.c','runtime/building_support.c','CMakeLists.txt','direct_c/verify_house_levels.py','direct_c/verify_house_evolution.py','metadata/house-levels-native.json']
    proof=dict(native_sha256=catalogue['native_sha256'],compiled_C_path=str(target.resolve()),compiled_C_sha256=target_sha,passed=True,checked=checked,invocations=invocations,
               coverage={hex(a):dict(instructions=len(p),executed=len(p&level_hits)) for a,p in level_owned.items()},conditional_edges=len(level_edges),
               actual_owner_compositions=owner_compositions,controlled_algorithm_endpoints=[],controlled_platform_endpoints=[],source_hashes={p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest() for p in sources},commit=None,
               limitations=['Native callback instruction scopes are inferred and authenticated against REA bytes and pinned original PE.',
                            'Void callback ABI: owner4528B0 reloads EAX immediately. Saved registers, stack balance, ordered global stores and real query arguments compared.',
                            'Actual merge/expansion/reduction helpers execute through existing model bodies; their semantic C reconstruction remains separate work.',
                            'Selected actual query-entry input mutations and a top-callback/model-level mismatch exercise fresh loads and otherwise unavailable branches.',
                            'No concurrent execution, host game launch, visual-rendering or complete-gameplay claim.'])
    suffix='diagnostic' if target.stem=='restore_link_check' else 'game'
    (ROOT/f'metadata/house-levels-{suffix}.json').write_text(json.dumps(proof,indent=2)+'\n')
    print('PASS twenty housing levels:',checked,'scenarios',invocations,'invocations',flush=True)
    print('Whole housing-owner compositions:',owner_compositions,flush=True)
    print('Native instructions/conditional edges:',len(level_points),len(level_edges),flush=True)
    print('Verified target SHA256:',target_sha,flush=True)
