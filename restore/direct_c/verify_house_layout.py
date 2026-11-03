"""Eight original/C layout methods, real splitting, clearing and map stamping.

The three query methods compare full EAX. Other methods are void. Child-entry
input mutation checks cached image selection and fresh relocation reads without
replacing child return calculations. No host game is launched.
"""
from verify_house_levels import *

base_prepare=prepare
catalogue=json.loads((ROOT/'metadata/house-layout-native.json').read_text())
assert catalogue['native_sha256']==hashlib.sha256(original.read_bytes()).hexdigest()
layout_owned={};layout_branches={};layout_hits=set();layout_edges=set();layout_last=[None]
for f in catalogue['functions']:
    a=int(f['address'],16);b=bytes.fromhex(f['bytes_hex'])
    assert b==bytes(images[0].data[a-images[0].base:a-images[0].base+len(b)])
    layout_owned[a]={int(p,16) for p in f['instruction_addresses'].split()}
    instructions=list(decoder.disasm(b,a));assert {i.address for i in instructions}==layout_owned[a]
    for i in instructions:
        if i.mnemonic.startswith('j') and i.mnemonic!='jmp':layout_branches[i.address]={int(i.op_str,16),i.address+i.size}
geometry=catalogue['geometry'];geom=bytes.fromhex(geometry['bytes_hex']);ga=int(geometry['address'],16)
assert geom==bytes(images[0].data[ga-images[0].base:ga-images[0].base+len(geom)])
offsets=struct.unpack_from('<16I',geom,48);anchors=struct.unpack_from('<4i',geom,32)
layout_points=set.union(*layout_owned.values());checked=invocations=0
queries={0x46b480,0x46b840,0x46b9e0}
names={0x46b480:'restore_house_can_expand',0x46b840:'restore_house_try_merge',0x46b940:'restore_house_clear_expansion',0x46b9e0:'restore_house_prepare_expansion'}
for c,u,state in machines:
    state['layout_mutation']=''
    def child(uc,pc,size,data):
        kind,n=data['layout_entries'][pc];sp=uc.reg_read(UC_X86_REG_ESP)
        args=tuple(get(uc,sp+4+i*4) for i in range(n))
        if kind=='stamp':args=(args[0]&65535,*args[1:])
        data['calls'].append((kind,args,bytes(uc.mem_read(0x7999d8+BID*264,264))))
        if kind=='clear' and data['layout_mutation']=='clear':
            put(uc,0x78d770,92);put(uc,0x78d76c,81);put(uc,0xbca2d8,1200)
            byte(uc,0x7999db+BID*264,1)
            put(uc,0x5d043c+10*4,0x40000000)
    state['layout_entries']={sym('FUN_%08x'%t) if c else t:(kind,n) for t,kind,n in [(0x47c480,'clear',3),(0x47bf50,'stamp',7)]}
    for at in state['layout_entries']:u.hook_add(UC_HOOK_CODE,child,state,begin=at,end=at)
    if not c:
        def observe_layout(uc,pc,size,data):
            layout_hits.add(pc)
            if layout_last[0] in layout_branches:layout_edges.add((layout_last[0],pc))
            layout_last[0]=pc
        for rows in layout_owned.values():u.hook_add(UC_HOOK_CODE,observe_layout,state,begin=min(rows),end=max(rows))

def fixture_layout(u,token,area,case,anchor,flags,status,occupied,donor_level,population,stock,special,size,rotation,x,y,building):
    base_prepare(u,5,0,0,10,0,True,True,True,None,0)
    u.mem_write(ga,geom)
    # The clear-entry mutation deliberately changes one image-offset field.
    for level in [5,10]:
        at=0x5d043c+level*4;u.mem_write(at,seed[at-0x560000:at-0x560000+4])
    if case=='zero-variant':put(u,0x5d043c+5*4,0)
    r=(building*264)&0xffffffff
    if building!=BID:u.mem_write(0x7999d8+r,bytes(u.mem_read(0x7999d8+BID*264,264)))
    byte(u,0x7999dc+r,special);byte(u,0x7999db+r,size)
    word(u,0x7999de+r,x);word(u,0x7999e0+r,y);put(u,0x7999e4+r,19330)
    byte(u,0xacf85c+19330,rotation);put(u,0x78d770,90);put(u,0x78d76c,80)
    for j in [-1,0,70,71,72,73]:
        dr=j*264;u.mem_write(0x7999d8+dr,bytes(264))
        byte(u,0x7999d8+dr,status);byte(u,0x7999dd+dr,occupied);byte(u,0x7999db+dr,size)
        byte(u,0x7999dc+dr,special);word(u,0x7999ea+dr,donor_level);word(u,0x7999e8+dr,donor_level+10)
        word(u,0x7999de+dr,90);word(u,0x7999e0+dr,80);put(u,0x7999e4+dr,19330)
        word(u,0x7999f4+dr,population)
        for k in range(12):word(u,0x799a2c+dr+k*2,stock+k)
    for k in range(12):put(u,0x78d590+k*4,0x12340000+k*0x10001)
    for start,scale in [(0xb285cc,1),(0xb67d1c,1),(0xacf85c,1),(0xb7482c,4),(0xaf598c,4),(0xb4e6fc,2)]:u.mem_write(start+18000*scale,bytes(6000*scale))
    u.mem_write(0xaf598c+18000*4,struct.pack('<I',1)*6000)
    byte(u,0xacf85c+19330,rotation)
    put(u,0xaf598c+1001*4,4);put(u,0xaf598c+1228*4,0x40000)
    base=19330+anchors[anchor] if token==0x46b480 else 19330
    for k,off in enumerate(offsets):
        cell=base+off
        if case=='self':owner=building
        elif case=='duplicate':owner=70
        elif case=='negative':owner=-1
        elif case=='zero':owner=0
        else:owner=70+k%4
        put(u,0xaf598c+cell*4,flags);word(u,0xb4e6fc+cell*2,owner)
    if case=='mixed':
        word(u,0xb4e6fc+(base+offsets[0])*2,building)
        put(u,0xaf598c+(base+offsets[1])*4,0)
        byte(u,0x7999dd+70*264,0)
    if case=='special-neighbour':byte(u,0x7999dc+70*264,1)
    if case in ['mixed-blank','mixed-road']:
        word(u,0xb4e6fc+(base+offsets[0])*2,70)
        word(u,0xb4e6fc+(base+offsets[1])*2,building)
        put(u,0xaf598c+(base+offsets[2])*4,0)
        put(u,0xaf598c+(base+offsets[3])*4,32 if case=='mixed-road' else 0)
    if case=='split':
        # Keep a single donor so real split allocation does not depend on a
        # contradictory multi-owner footprint. Other selected cells are self.
        for off in offsets:word(u,0xb4e6fc+(19330+off)*2,building)
        word(u,0xb4e6fc+19330*2,70)

def call_layout(c,u,token,building,area,route):
    global invocations
    invocations+=1;argv=[building] if token==0x46b840 else [building,area]
    if not c:entry=token
    elif route=='legacy':entry=sym('FUN_%08x'%token)
    elif route=='named':
        if token in names:entry=sym(names[token])
        else:entry=sym('restore_house_apply_layout');argv=[token,*argv]
    else:
        u.mem_write(VSP-256,b'\x6b'*320);put(u,VSP,STOP)
        for i,v in enumerate(argv):put(u,VSP+4+i*4,v)
        u.mem_write(FRAME,struct.pack('<10I',0x89abcdef,0x87654321,0x98765432,saved[0],VSP,*saved[1:],0x202,0))
        if route=='frame':entry=sym('restore_building_control_C_%08x'%token);argv=[FRAME]
        else:entry=sym('restore_building_control_raw');argv=[token,FRAME,FRAME+32]
    value=run(0,u,entry,argv,budget=3000000)
    if c and route in ['frame','raw']:
        assert get(u,FRAME+16)==VSP+4 and [get(u,FRAME+i*4) for i in [3,5,6,7]]==saved
        assert bytes(u.mem_read(VSP-256,64))==b'\x6b'*64
        return get(u,FRAME)
    return value

def exercise_layout(token,area=4,case='self',anchor=0,flags=8,status=1,occupied=1,donor_level=5,population=7,stock=3,special=0,size=1,rotation=0,x=90,y=80,building=BID,mutation=''):
    global checked
    expected=None
    for c,u,state in machines:
        routes=['named','frame','raw']
        if available('FUN_%08x'%token):routes.append('legacy')
        for route in routes if c else ['native']:
            fixture_layout(u,token,area,case,anchor,flags,status,occupied,donor_level,population,stock,special,size,rotation,x,y,building)
            state.update(writes=[],calls=[],mutation='',layout_mutation=mutation);layout_last[0]=None
            try:value=call_layout(c,u,token,building,area,route)
            except Exception as e:raise AssertionError(('execution',hex(token),area,case,anchor,flags,status,occupied,donor_level,population,stock,special,size,rotation,x,y,hex(building),mutation,route,hex(u.reg_read(UC_X86_REG_EIP)))) from e
            result=(value if token in queries else None,tuple(state['writes']),tuple(state['calls']))
            if expected is None:expected=result
            else:
                diff=[(i,(hex(a[0]),*a[1:]),(hex(b[0]),*b[1:])) for i,(a,b) in enumerate(zip(expected[1],result[1])) if a!=b][:8]
                assert result==expected,('layout',hex(token),area,case,anchor,flags,status,occupied,donor_level,population,stock,special,size,rotation,x,y,hex(building),mutation,route,'returns',expected[0],result[0],'write counts',len(expected[1]),len(result[1]),diff,'call counts',len(expected[2]),len(result[2]))
    checked+=1

if __name__=='__main__':
    for area,anchor,flags in itertools.product([0,1,2,3,4,9,16,0xffffffff,0x80000000],range(4),[0,8,32,40,1]):exercise_layout(0x46b480,area=area,anchor=anchor,flags=flags)
    for anchor,flags,status,occupied,level in itertools.product(range(4),[8,40],[0,1,3],[0,1],[-1,5,6]):exercise_layout(0x46b480,case='donors',anchor=anchor,flags=flags,status=status,occupied=occupied,donor_level=level)
    for x,y in itertools.product([-32768,-1,0,32767],[-32768,-1,0,32767]):exercise_layout(0x46b480,x=x,y=y)
    for case in ['mixed-blank','mixed-road']:exercise_layout(0x46b480,case=case)
    for area,case,flags,occupied,population in itertools.product([0,1,4,9,16,0xffffffff],['self','duplicate','negative','zero','mixed'],[0,8],[0,1],[-32768,-1,0,32767]):exercise_layout(0x46b9e0,area=area,case=case,flags=flags,occupied=occupied,population=population,stock=population)
    for special,size,flags in itertools.product([0,1,2],[1,2,3,4],[0,8]):exercise_layout(0x46b940,case='split',special=special,size=size,flags=flags,area=1)
    for area in [0,0xffffffff,0x80000000]:exercise_layout(0x46b940,area=area)
    exercise_layout(0x46b940,case='self',area=4)
    exercise_layout(0x46b940,case='duplicate',area=4,occupied=0)
    for case,flags,status,occupied,level,special,rotation in itertools.product(['self','donors'],[0,8],[0,1],[0,1],[4,5,6],[0,1],[0,5]):exercise_layout(0x46b840,case=case,flags=flags,status=status,occupied=occupied,donor_level=level,special=special,rotation=rotation)
    for token,people,stock,mutation in itertools.product([0x46bab0,0x46bbc0,0x46bcb0,0x46bda0],[-32768,-1,0,1,32767,65536,0x80000000],[-32768,32767],['','clear']):exercise_layout(token,area=people,stock=stock,mutation=mutation)
    exercise_layout(0x46b840,case='special-neighbour')
    exercise_layout(0x46bda0,case='zero-variant')
    for token,building in itertools.product([0x46b480,0x46b9e0],[BID+0x20000000,BID+0x80000000]):exercise_layout(token,building=building)
    assert layout_points<=layout_hits,('unexecuted instructions',[hex(a) for a in sorted(layout_points-layout_hits)])
    missing={(a,b) for a,targets in layout_branches.items() for b in targets if (a,b) not in layout_edges}
    assert not missing,('unexecuted conditional edges',[(hex(a),hex(b)) for a,b in sorted(missing)])
    sources=['include/house_layout.h','include/house_levels.h','runtime/house_layout.c','runtime/house_levels.c','runtime/building_controllers.c','runtime/storage_settings.c','runtime/gatehouse_display.c','CMakeLists.txt','direct_c/verify_house_layout.py','direct_c/verify_house_levels.py','direct_c/verify_house_evolution.py','metadata/house-layout-native.json','metadata/house-levels-native.json']+[f'src/unit_{i:05d}.c' for i in [969,971,972,973,974,975,976,977]]
    proof=dict(native_sha256=catalogue['native_sha256'],compiled_C_path=str(target.resolve()),compiled_C_sha256=target_sha,passed=True,checked=checked,invocations=invocations,
               coverage={hex(a):dict(instructions=len(p),executed=len(p&layout_hits)) for a,p in layout_owned.items()},conditional_edges=len(layout_edges),
               controlled_algorithm_endpoints=[],controlled_platform_endpoints=[],source_hashes={p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest() for p in sources},commit=None,
               limitations=['Eight complete analyzed native bodies. Full query EAX, ordered global stores, child inputs, saved registers and frame balance compared.',
                            'Actual clear, stamping, allocation and pending split models execute. Input mutation at actual clear entry checks load order; return values are not replaced.',
                            'Mapped selected geometry up to16 cells and signed-negative area guards; no arbitrary oversized buffers or concurrency certification.',
                            'No host game launch, visual rendering or complete-gameplay claim.'])
    suffix='diagnostic' if target.stem=='restore_link_check' else 'game'
    (ROOT/f'metadata/house-layout-{suffix}.json').write_text(json.dumps(proof,indent=2)+'\n')
    print('PASS eight house layout methods:',checked,'scenarios',invocations,'invocations',flush=True)
    print('Native instructions/conditional edges:',len(layout_points),len(layout_edges),flush=True)
    print('Verified target SHA256:',target_sha,flush=True)
