"""Whole monument map methods; actual image queries, resident loader and footprint graph."""
from verify_options_startup import *
from unicorn import UC_HOOK_MEM_WRITE
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import json

OBJ,FRAME,VSP=S+0x1000,S+0x8000,S+0x18000
BID=64
TABLES={0x56f438:0x4017a0,0x56f48c:0x40dca0,0x56f4dc:0x4017a0,0x56f6e8:0x4e69c0}
NAMES={t:'restore_monument_map_%08x'%t for t in TABLES.values()}
available=lambda name:bool(re.search(r'\s_'+re.escape(name)+r'\s',symbols))
catalogue=json.loads((ROOT/'metadata/monument-map-native.json').read_text())
assert catalogue['native_sha256']==hashlib.sha256(original.read_bytes()).hexdigest()
owned={};branches={};hits=set();edges=set();previous=[None];checked=0;invocations=0
for f in catalogue['functions']:
    a=int(f['address'],16);n=f['result']['procedure']['body']['total_bytes']
    code=list(Cs(CS_ARCH_X86,CS_MODE_32).disasm(bytes(images[0].data[a-images[0].base:a-images[0].base+n]),a))
    assert sum(i.size for i in code)==n
    assert [hex(i.address) for i in code]==[x.split(':')[0] for x in f['result']['instructions']]
    owned[a]={i.address for i in code}
    branches.update({i.address:{int(i.op_str,16),i.address+i.size} for i in code if i.mnemonic.startswith('j') and i.mnemonic!='jmp'})
points=set.union(*owned.values())
for table,token in TABLES.items():assert struct.unpack_from('<I',images[0].data,table+24-images[0].base)[0]==token
machines=[]
for c,pe in enumerate(images):
    u=Uc(UC_ARCH_X86,UC_MODE_32)
    u.mem_map(pe.base,(len(pe.data)+4095)&~4095);u.mem_write(pe.base,bytes(pe.data))
    u.mem_map(0x30000000,0x10000);u.mem_map(STOP,4096);u.mem_map(S,0x50000);u.mem_map(0,4096)
    state={'hooks':{},'calls':[],'writes':[],'collect':False,'events':[]};machines.append((c,u,state))
    def writes(uc,access,at,size,value,data):
        if data['collect'] and 0x560000<=at<0x126d000:data['writes'].append((at,size,value&((1<<(size*8))-1)))
    u.hook_add(UC_HOOK_MEM_WRITE,writes,state)
    def footprint(uc,pc,size,data):
        if data['collect']:
            sp=uc.reg_read(UC_X86_REG_ESP);args=tuple(get(uc,sp+4+i*4) for i in range(7))
            data['events'].append((args[0]&65535,*args[1:]))
    entry=address(c,0x47bf50);u.hook_add(UC_HOOK_CODE,footprint,state,begin=entry,end=entry)
    if not c:
        def observe(uc,pc,size,data):
            if not data['collect']:return
            hits.add(pc)
            if previous[0] in branches:edges.add((previous[0],pc))
            previous[0]=pc
        for addresses in owned.values():u.hook_add(UC_HOOK_CODE,observe,state,begin=min(addresses),end=max(addresses))

def fixture(u,table,index,extra,stage,size,rotation,zero,x,y,cached):
    u.mem_write(OBJ,bytes(0x1000));put(u,OBJ,table)
    for i in range({0x56f438:12,0x56f48c:25,0x56f4dc:9,0x56f6e8:2}[table]):
        put(u,OBJ+0x54+i*52,size);put(u,OBJ+0x58+i*52,i)
    put(u,OBJ+0x58+index*52,extra)
    u.mem_write(S+0x5000,b'monument-fixture\0')
    if table==0x56f4dc:put(u,OBJ+0x220+stage*4,S+0x5000)
    elif table==0x56f6e8:put(u,OBJ+0xb4+stage*4,S+0x5000)
    for bid in [BID,BID-index]:
        r=bid*264;u.mem_write(0x7999d8+r,bytes(264))
        byte(u,0x7999d8+r,1);word(u,0x7999de+r,x);word(u,0x7999e0+r,y)
        word(u,0x7999e8+r,0xe1 if table==0x56f48c else 0xe4 if table==0x56f6e8 else 0xe3)
        put(u,0x799a30+r,stage)
    word(u,0x7999ea+BID*264,index)
    for a,v in [(0xbca2d8,10000),(0xbca2d0,100),(0xbca2d4,100),(0xbca2e0,rotation),(0xe94e04,rotation),
                (0x78d790,BID),(0xe38dcc,0),(0xbc9768,0x12345678)]:put(u,a,v)
    for tag in range(1000):put(u,0xf55bb4+tag*8,0 if zero else 100000+tag*16)
    for row in range(-1,8):
        for col in range(-1,8):
            cell=10000+(y+row)*228+x+col
            put(u,0xaf598c+cell*4,0x6fabcd92);word(u,0xb4e6fc+cell*2,0x9876)
            byte(u,0xb285cc+cell,0xe5);byte(u,0xb67d1c+cell,0x83)
            byte(u,0xacf85c+cell,(row*3+col*5)&255);put(u,0xb7482c+cell*4,0x12345678)
    # These writes define records after the raster seed as well. This permits
    # mapped out-of-map coordinates whose raster cells alias game globals.
    for bid in [BID,BID-index]:
        word(u,0x7999de+bid*264,x);word(u,0x7999e0+bid*264,y);put(u,0x799a30+bid*264,stage)
    word(u,0x7999ea+BID*264,index)
    # Resident cache descriptors exercise the genuine 53DD90 matching-name
    # branch. Filename/slot are observed in a stopped native preflight below;
    # neither verification execution substitutes a callee or return value.
    for slot,name in cached:
        lib=0xf55bb0+0x1f40+slot*0xcc3c
        u.mem_write(lib,name+b'\0');put(u,lib+0xcc2c,1);put(u,lib+0xcc30,4)
        put(u,0xf55bb0+0x30f548+slot*4,5+slot*4)
        if slot:put(u,0xf55bb0+0x30f548+(slot-1)*4,1+slot*4)

def invoke(c,u,token,mode,probe=False):
    global invocations
    if not probe:invocations+=1
    for reg,v in zip(registers,[0x89abcdef,OBJ,0x98765432,saved[0],SP,*saved[1:]]):u.reg_write(reg,v)
    u.reg_write(UC_X86_REG_EFLAGS,0x202);put(u,SP,STOP);args=[BID];cleanup=0
    if not c:entry=token;cleanup=4
    elif mode=='legacy':entry=address(c,token)
    elif mode=='named':entry=sym(NAMES[token]);args=[OBJ,BID]
    else:
        u.mem_write(VSP-512,b'\x6b'*576);put(u,VSP,STOP);put(u,VSP+4,BID)
        u.mem_write(FRAME,struct.pack('<10I',0x89abcdef,OBJ,0x98765432,saved[0],VSP,*saved[1:],0x202,sym('restore_building_create_dispatch')))
        entry=sym('restore_building_create_dispatch');args=[0,token,FRAME]
    for i,v in enumerate(args):put(u,SP+4+i*4,v)
    u.emu_start(entry,STOP,count=10000000)
    if probe and u.reg_read(UC_X86_REG_EIP)==0x53dd90:return
    assert u.reg_read(UC_X86_REG_EIP)==STOP,(c,hex(token),mode,hex(u.reg_read(UC_X86_REG_EIP)))
    assert u.reg_read(UC_X86_REG_ESP)==SP+4+cleanup
    assert [u.reg_read(registers[i]) for i in [3,5,6,7]]==saved
    if c and mode=='dispatch':
        assert get(u,FRAME+16)==VSP+8
        assert [get(u,FRAME+i*4) for i in [3,5,6,7]]==saved
        assert bytes(u.mem_read(VSP-512,64))==b'\x6b'*64
        assert bytes(u.mem_read(VSP+8,56))==b'\x6b'*56

def exercise(table,index=0,extra=None,stage=0,size=3,rotation=0,zero=False,x=50,y=50):
    global checked
    if extra is None:extra=index
    token=TABLES[table];expected=None;cached=[]
    c,u,state=machines[0];state['collect']=False;reset(u,state);fixture(u,table,index,extra,stage,size,rotation,zero,x,y,[])
    def discover(uc,pc,n,data):
        sp=uc.reg_read(UC_X86_REG_ESP);name=get(uc,sp+4);slot=get(uc,sp+8)
        if name:
            assert slot<30;cached.append((slot,bytes(uc.mem_read(name,128)).split(b'\0')[0]));uc.emu_stop()
    handle=u.hook_add(UC_HOOK_CODE,discover,begin=0x53dd90,end=0x53dd90)
    try:invoke(0,u,token,'native',probe=True)
    finally:u.hook_del(handle)
    for c,u,state in machines:
        modes=['named','dispatch']+(['legacy'] if available('FUN_%08x'%token) else [])
        for mode in (modes if c else ['native']):
            state['collect']=False;reset(u,state);fixture(u,table,index,extra,stage,size,rotation,zero,x,y,cached)
            state['writes']=[];state['events']=[];state['collect']=True;previous[0]=None
            try:invoke(c,u,token,mode)
            except Exception as error:
                raise AssertionError(('execution',c,hex(table),index,extra,stage,size,rotation,zero,x,y,mode,'pc',hex(u.reg_read(UC_X86_REG_EIP)))) from error
            state['collect']=False
            result=(state['events'],tuple(state['writes']),snapshot(u))
            if expected is None:expected=result
            elif result!=expected:
                memory=next(((hex(0x560000+i),a,b) for i,(a,b) in enumerate(zip(expected[2],result[2])) if a!=b),None)
                stores=next(((i,a,b) for i,(a,b) in enumerate(zip(expected[1],result[1])) if a!=b),None)
                raise AssertionError((hex(table),index,extra,stage,size,rotation,zero,x,y,mode,'memory',memory,'stores',len(expected[1]),len(result[1]),stores,'calls',expected[0],result[0]))
    checked+=1

if __name__=='__main__':
    for index,stage in itertools.product(range(25),[0,2,3,8,15,16,20,21]):
        exercise(0x56f48c,index=index,stage=stage,rotation=(index%4)*2)
    for index,rotation,size in itertools.product([0,13,14,15,16],[0,2,4,6],[1,3,6]):
        exercise(0x56f48c,index=index,extra=21,stage=0,rotation=rotation,size=size)
    for size in [0,-1,-2147483648]:exercise(0x56f48c,index=13,extra=21,size=size)
    for stage in [0,2,16,21]:exercise(0x56f48c,index=13,extra=21,stage=stage,zero=True)
    for table,limit in [(0x56f438,12),(0x56f4dc,9)]:
        for index,rotation in itertools.product(range(limit),[0,2,4,6]):exercise(table,index=index,stage=index%4,rotation=rotation)
    for index,extra,stage in itertools.product([0,1],[0,1,2,0xffffffff],[0,3,9]):exercise(0x56f6e8,index=index,extra=extra,stage=stage)
    for table in TABLES:
        for x,y in [(-1,50),(50,-1),(99,99)]:exercise(table,x=x,y=y,stage=3)
    for alias_address,base,stage in [(0xf573bc,0xaf598c,0),(0x799a30+(BID-13)*264,0xaf598c,0),(0xbc9768,0xb7482c,2)]:
        cell=(alias_address-base)//4;y,x=divmod(cell-10000,228)
        exercise(0x56f48c,index=13,extra=21,stage=stage,x=x,y=y)
    print('PASS whole map methods and actual image/footprint descendants:',checked,flush=True)
    assert points<=hits,('unexecuted instructions',[hex(a) for a in sorted(points-hits)])
    missing={(a,b) for a,targets in branches.items() for b in targets if (a,b) not in edges}
    assert not missing,('unexecuted conditional edges',[(hex(a),hex(b)) for a,b in sorted(missing)])
    import verify_monument_create as construction
    for token,rotations in [(0x40da90,[0,2,4,6]),(0x417180,[0,2,4,6]),(0x4e6840,[0,4])]:
        for rotation in rotations:construction.exercise(token,rotation=rotation)
    sources=['include/monument_map.h','runtime/monument_map.c','runtime/building_methods.c','CMakeLists.txt',
             'direct_c/verify_monument_map.py','metadata/monument-map-native.json','direct_c/verify_monument_create.py',
             'runtime/placement_queries.c','runtime/menu_drag_road_place.c']+[f'src/unit_{i:05d}.c' for i in [9,107,2001]]
    proof=dict(native_sha256=catalogue['native_sha256'],compiled_C_path=str(target.resolve()),compiled_C_sha256=target_sha,
               passed=True,checked=checked,invocations=invocations,
               coverage={hex(a):dict(instructions=len(v),executed=len(v&hits)) for a,v in owned.items()},conditional_edges=len(edges),
               complete_creation_compositions=construction.checked,composition_invocations=construction.invocations,
               source_hashes={p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest() for p in sources},
               controlled_algorithm_endpoints=[],controlled_platform_endpoints=[],
               limitations=['Mapped metadata, records and raster storage; no concurrency or fault-equivalence claim.',
                            'Actual shipped image members, resident-cache loader paths, footprint and terrain refresh execute.',
                            'Native preflight stops to observe a requested resident library name/slot; verification calls have no substituted callees.',
                            'Mapped out-of-map raster/global aliases exercise stage-per-row, origin-per-cell and sprite-base reloads.',
                            'Compares complete game globals, exact ordered stores and footprint calls; void-method volatile registers/flags and private frames are outside scope.',
                            'No pixel decoding or full-gameplay approval.'],commit=None)
    suffix='diagnostic' if target.stem=='restore_link_check' else 'game'
    (ROOT/f'metadata/monument-map-{suffix}.json').write_text(json.dumps(proof,indent=2)+'\n')
    print('Native instructions/conditional edges:',len(hits&points),len(edges),flush=True)
    print('Complete creation compositions:',construction.checked,'invocations:',construction.invocations,flush=True)
    print('Verified target SHA256:',target_sha,flush=True)
