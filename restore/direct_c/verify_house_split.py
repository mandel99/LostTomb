"""Five original/C subdivision and reduction methods, actual children offline.

Void contracts: compare exact ordered game-state writes, real child arguments,
saved registers and frame balance. Child-entry input mutations preserve actual
allocation, clearing, stamping and road refresh return computations.
"""
from verify_house_layout import *

catalogue=json.loads((ROOT/'metadata/house-split-native.json').read_text())
assert catalogue['native_sha256']==hashlib.sha256(original.read_bytes()).hexdigest()
split_owned={};split_branches={};split_hits=set();split_edges=set();split_last=[None]
for f in catalogue['functions']:
    a=int(f['address'],16);b=bytes.fromhex(f['bytes_hex'])
    assert b==bytes(images[0].data[a-images[0].base:a-images[0].base+len(b)])
    split_owned[a]={int(p,16) for p in f['instruction_addresses'].split()}
    instructions=list(decoder.disasm(b,a));assert {i.address for i in instructions}==split_owned[a]
    for i in instructions:
        if i.mnemonic.startswith('j') and i.mnemonic!='jmp':split_branches[i.address]={int(i.op_str,16),i.address+i.size}
split_points=set.union(*split_owned.values());checked=invocations=0
configs={0x46be90:(5,2,1),0x46c1c0:(10,2,0),0x46c500:(14,3,0),0x46cb30:(14,3,0),0x46ccf0:(18,4,0)}
for c,u,state in machines:
    state.update(split_mutation='',stamp_count=0,allocate_count=0)
    def split_child(uc,pc,size,data):
        kind,n=data['split_entries'][pc];sp=uc.reg_read(UC_X86_REG_ESP)
        args=tuple(get(uc,sp+4+i*4) for i in range(n))
        if kind=='stamp':args=(args[0]&65535,*args[1:]);data['stamp_count']+=1
        if kind=='allocate':data['allocate_count']+=1
        data['calls'].append(('split '+kind,args,bytes(uc.mem_read(0x7999d8+BID*264,264))))
        r=BID*264;mutation=data['split_mutation']
        if kind=='clear' and mutation=='clear':
            word(uc,0x7999f4+r,301)
            for i in range(12):word(uc,0x799a2c+r+i*2,200+i)
            word(uc,0x7999e8+r,16);word(uc,0x7999ea+r,6)
            for level in [6,9,13,17]:put(uc,0x5d043c+level*4,0x123)
        if kind=='stamp' and data['stamp_count']==1 and mutation=='stamp':
            word(uc,0x7999de+r,92);word(uc,0x7999e0+r,81)
            word(uc,0x7999e8+r,16);word(uc,0x7999ea+r,6);put(uc,0xeaac28,1)
        if kind=='allocate' and data['allocate_count']==2 and mutation=='allocate':
            put(uc,0xeaac28,3);word(uc,0x7999e8+r,18);word(uc,0x7999ea+r,8)
            word(uc,0x7999de+r,99);word(uc,0x7999e0+r,90)
            for level in [6,9]:put(uc,0x5d043c+level*4,0x7000)
        if kind=='refresh' and mutation=='refresh':
            word(uc,0x7999de+r,99);word(uc,0x7999e0+r,90);put(uc,0xbca2d8,1200)
    native_entries=[(0x47c480,'clear',3),(0x47bf50,'stamp',7),(0x476530,'refresh',0),(0x516f90,'connect',5),(0x5173a0,'cost',4),(0x46a400,'allocate',6)]
    state['split_entries']={sym('restore_building_allocate_0046a400') if c and t==0x46a400 else sym('FUN_%08x'%t) if c else t:(name,n) for t,name,n in native_entries}
    for at in state['split_entries']:u.hook_add(UC_HOOK_CODE,split_child,state,begin=at,end=at)
    if not c:
        def observe_split(uc,pc,size,data):
            split_hits.add(pc)
            if split_last[0] in split_branches:split_edges.add((split_last[0],pc))
            split_last[0]=pc
        for rows in split_owned.values():u.hook_add(UC_HOOK_CODE,observe_split,state,begin=min(rows),end=max(rows))

def prepare_split(u,token,population,stock,owner,rotation,full):
    level,size,special=configs[token]
    base_prepare(u,level,0,0,population,0,True,True,True,None,special)
    # Mutations in one invocation must not affect later routes/cases.
    u.mem_write(0x5d03ec,seed[0x5d03ec-0x560000:0x5d03ec-0x560000+80])
    u.mem_write(0x5d043c,seed[0x5d043c-0x560000:0x5d043c-0x560000+80])
    r=BID*264;byte(u,0x7999db+r,size);byte(u,0x7999dd+r,size)
    word(u,0x7999f4+r,population);put(u,0xeaac28,owner)
    for i in range(12):word(u,0x799a2c+r+i*2,stock+i*5)
    for dy in range(4):
        for dx in range(4):
            cell=19330+dy*228+dx
            put(u,0xaf598c+cell*4,8);word(u,0xb4e6fc+cell*2,BID);byte(u,0xacf85c+cell,(rotation+dx+dy)&7)
    # Nearest-road/water quality checks occur before new coordinates are set.
    put(u,0xaf598c+1001*4,4);put(u,0xaf598c+1228*4,0x40000)
    if full:
        for i in range(1,4000):byte(u,0x7999d8+i*264,1)

def call_split(c,u,token,route):
    global invocations
    invocations+=1;argv=[BID]
    if not c:entry=token
    elif route=='named':entry=sym('restore_house_subdivide');argv=[token,BID]
    elif route=='legacy':entry=sym('FUN_%08x'%token)
    else:
        u.mem_write(VSP-256,b'\x6b'*320);put(u,VSP,STOP);put(u,VSP+4,BID)
        u.mem_write(FRAME,struct.pack('<10I',0x89abcdef,0x87654321,0x98765432,saved[0],VSP,*saved[1:],0x202,0))
        if route=='frame':entry=sym('restore_building_control_C_%08x'%token);argv=[FRAME]
        else:entry=sym('restore_building_control_raw');argv=[token,FRAME,FRAME+32]
    run(0,u,entry,argv,budget=5000000)
    if c and route in ['frame','raw']:
        assert get(u,FRAME+16)==VSP+4 and [get(u,FRAME+i*4) for i in [3,5,6,7]]==saved
        assert bytes(u.mem_read(VSP-256,64))==b'\x6b'*64 and bytes(u.mem_read(VSP+8,48))==b'\x6b'*48

def exercise_split(token,population=17,stock=17,owner=0,rotation=0,mutation='',full=False):
    global checked
    expected=None
    for c,u,state in machines:
        routes=['named','frame','raw']
        if available('FUN_%08x'%token):routes.append('legacy')
        for route in routes if c else ['native']:
            prepare_split(u,token,population,stock,owner,rotation,full)
            state.update(writes=[],calls=[],mutation='',layout_mutation='',split_mutation=mutation,stamp_count=0,allocate_count=0);split_last[0]=None
            try:call_split(c,u,token,route)
            except Exception as e:raise AssertionError(('execution',hex(token),population,stock,owner,rotation,mutation,full,route,hex(u.reg_read(UC_X86_REG_EIP)))) from e
            result=(tuple(state['writes']),tuple(state['calls']))
            if expected is None:expected=result
            else:
                differences=[(i,(hex(a[0]),*a[1:]),(hex(b[0]),*b[1:])) for i,(a,b) in enumerate(zip(expected[0],result[0])) if a!=b][:8]
                calls=[(i,a[:2],b[:2]) for i,(a,b) in enumerate(zip(expected[1],result[1])) if a!=b][:6]
                assert result==expected,('split',hex(token),population,stock,owner,rotation,mutation,full,route,'write counts',len(expected[0]),len(result[0]),differences,'call counts',len(expected[1]),len(result[1]),calls)
    checked+=1

if __name__=='__main__':
    for token in configs:
        for value in [-32768,-17,-9,-8,-7,-6,-5,-4,-3,-2,-1,0,1,2,3,4,5,6,7,8,9,17,32767]:exercise_split(token,population=value,stock=value)
        for stock,owner,rotation in itertools.product([-32768,-1,0,32760],[0,1,3],[0,1,7]):exercise_split(token,stock=stock,owner=owner,rotation=rotation)
        for mutation in ['clear','stamp','allocate','refresh']:exercise_split(token,population=-17,stock=-17,mutation=mutation)
    exercise_split(0x46c1c0,full=True)
    assert split_points<=split_hits,('unexecuted instructions',[hex(a) for a in sorted(split_points-split_hits)])
    missing={(a,b) for a,targets in split_branches.items() for b in targets if (a,b) not in split_edges}
    assert not missing,('unexecuted conditional edges',[(hex(a),hex(b)) for a,b in sorted(missing)])
    sources=['include/house_split.h','include/house_layout.h','include/house_levels.h','runtime/house_split.c','runtime/house_layout.c','runtime/house_levels.c','runtime/building_controllers.c','runtime/storage_settings.c','runtime/gatehouse_display.c','runtime/building_allocator.c','CMakeLists.txt','direct_c/verify_house_split.py','direct_c/verify_house_layout.py','direct_c/verify_house_levels.py','direct_c/verify_house_evolution.py','metadata/house-split-native.json','metadata/house-layout-native.json','metadata/house-levels-native.json']+[f'src/unit_{i:05d}.c' for i in range(978,983)]
    proof=dict(native_sha256=catalogue['native_sha256'],compiled_C_path=str(target.resolve()),compiled_C_sha256=target_sha,passed=True,checked=checked,invocations=invocations,
               coverage={hex(a):dict(instructions=len(p),executed=len(p&split_hits)) for a,p in split_owned.items()},conditional_edges=len(split_edges),
               controlled_algorithm_endpoints=[],controlled_platform_endpoints=[],source_hashes={p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest() for p in sources},commit=None,
               limitations=['Five complete analyzed native bodies; void ABI, ordered global writes, actual child arguments, saved registers and frame balance compared.',
                            'Actual allocator, quality/distance queries, map clearing/stamping and road refresh/connect/cost providers execute, including full-table allocation failure.',
                            'Selected child-entry input mutations test cached population/stocks, image banks, root coordinates and fresh owner/type reads without replacing returns.',
                            'Mapped fixture coordinates and model rows; no concurrency, arbitrary invalid memory, host game launch, visual or complete-gameplay claim.'])
    suffix='diagnostic' if target.stem=='restore_link_check' else 'game'
    (ROOT/f'metadata/house-split-{suffix}.json').write_text(json.dumps(proof,indent=2)+'\n')
    print('PASS five house split/reduction methods:',checked,'scenarios',invocations,'invocations',flush=True)
    print('Native instructions/conditional edges:',len(split_points),len(split_edges),flush=True)
    print('Verified target SHA256:',target_sha,flush=True)
