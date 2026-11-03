"""Whole native/C entrance queries: full EAX, ordered stores and real dispatchers."""
from verify_options_startup import *
from unicorn import UC_HOOK_MEM_WRITE
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import json

OBJ,OUT,FRAME,VSP=S+0x1000,S+0x3000,S+0x8000,S+0x18000
BID=64
METHODS={0x40eb90:0x56f48c,0x417490:0x56f4dc,0x4e6ae0:0x56f6e8}
NAMES={t:'restore_monument_entrance_%08x'%t for t in METHODS}
available=lambda name:bool(re.search(r'\s_'+re.escape(name)+r'\s',symbols))
catalogue=json.loads((ROOT/'metadata/monument-entrance-native.json').read_text())
assert catalogue['native_sha256']==hashlib.sha256(original.read_bytes()).hexdigest()
owned={};branches={};hits=set();edges=set();jump_edges=set();previous=[None];checked=0;invocations=0
for f in catalogue['functions']:
    a=int(f['address'],16);n=f['result']['procedure']['body']['total_bytes']
    code=list(Cs(CS_ARCH_X86,CS_MODE_32).disasm(bytes(images[0].data[a-images[0].base:a-images[0].base+n]),a))
    assert sum(i.size for i in code)==n
    assert [hex(i.address) for i in code]==[x.split(':')[0] for x in f['result']['instructions']]
    owned[a]={i.address for i in code}
    branches.update({i.address:{int(i.op_str,16),i.address+i.size} for i in code if i.mnemonic.startswith('j') and i.mnemonic!='jmp'})
points=set.union(*owned.values())
table=catalogue['jump_table'];table_at=int(table['address'],16);table_bytes=bytes.fromhex(table['bytes_hex'])
assert bytes(images[0].data[table_at-images[0].base:table_at-images[0].base+16])==table_bytes
jump_targets=set(struct.unpack('<4I',table_bytes))
machines=[]
for c,pe in enumerate(images):
    u=Uc(UC_ARCH_X86,UC_MODE_32)
    u.mem_map(pe.base,(len(pe.data)+4095)&~4095);u.mem_write(pe.base,bytes(pe.data))
    u.mem_map(0x30000000,0x10000);u.mem_map(STOP,4096);u.mem_map(S,0x50000)
    u.mem_write(0x560000,seed)
    state={'hooks':{},'calls':[],'writes':[]};machines.append((c,u,state))
    def writes(uc,access,at,size,value,data):
        if 0x560000<=at<0x126d000 or S<=at<S+0x4000:data['writes'].append((at,size,value&((1<<(size*8))-1)))
    u.hook_add(UC_HOOK_MEM_WRITE,writes,state)
    if not c:
        def observe(uc,pc,size,data):
            hits.add(pc)
            if previous[0] in branches:edges.add((previous[0],pc))
            if previous[0]==0x4174b8:jump_edges.add(pc)
            previous[0]=pc
        for addresses in owned.values():u.hook_add(UC_HOOK_CODE,observe,begin=min(addresses),end=max(addresses))

def fixture(u,token,stage,delta,size,x,y,rotation):
    u.mem_write(OBJ,b'\xa5'*0x900);u.mem_write(OUT-8,b'\x6b'*0x300)
    put(u,OBJ,METHODS[token]);put(u,0xbca2e0,rotation)
    for bid in range(BID-32,BID+33):
        r=bid*264;u.mem_write(0x7999d8+r,b'\xa5'*264)
        word(u,0x7999de+r,x+(bid-BID)*17);word(u,0x7999e0+r,y-(bid-BID)*23)
    word(u,0x7999ea+BID*264,delta);put(u,0x799a30+BID*264,stage)
    put(u,OBJ+0x54+delta*52,size)

def invoke(c,u,token,building,out_x,out_y,mode):
    global invocations
    invocations+=1;args=[building,out_x,out_y];frame_result=False
    for r,v in zip(registers,[0x89abcdef,OBJ,0x98765432,saved[0],SP,*saved[1:]]):u.reg_write(r,v)
    u.reg_write(UC_X86_REG_EFLAGS,0x202);put(u,SP,STOP);cleanup=0
    if not c:entry=token;cleanup=12
    elif mode=='legacy':entry=address(c,token)
    elif mode=='named':
        entry=sym(NAMES[token]);args=([OBJ]+args) if token==0x40eb90 else args
    else:
        frame_result=True
        u.mem_write(VSP-256,b'\x6b'*320);put(u,VSP,STOP)
        for i,v in enumerate(args):put(u,VSP+4+i*4,v)
        u.mem_write(FRAME,struct.pack('<10I',0x89abcdef,OBJ,0x98765432,saved[0],VSP,*saved[1:],0x202,sym('restore_building_create_dispatch')))
        if mode=='building':entry=sym('restore_building_create_dispatch');args=[0,token,FRAME]
        elif mode=='placement':entry=sym('restore_placement_queries_dispatch');args=[0,token,FRAME]
        elif mode=='city':entry=sym('restore_city_dispatch');args=[0,token,FRAME]
        elif mode=='laborer':entry=sym('restore_actor_laborer_raw');args=[token,FRAME,FRAME+32]
        else:raise AssertionError(mode)
    for i,v in enumerate(args):put(u,SP+4+i*4,v)
    u.emu_start(entry,STOP,count=1000000)
    assert u.reg_read(UC_X86_REG_EIP)==STOP,(hex(token),mode,hex(u.reg_read(UC_X86_REG_EIP)))
    assert u.reg_read(UC_X86_REG_ESP)==SP+4+cleanup
    assert [u.reg_read(registers[i]) for i in [3,5,6,7]]==saved
    if frame_result:
        assert get(u,FRAME+16)==VSP+16
        assert [get(u,FRAME+i*4) for i in [3,5,6,7]]==saved
        assert bytes(u.mem_read(VSP-256,64))==b'\x6b'*64
        assert bytes(u.mem_read(VSP+16,48))==b'\x6b'*48
        return get(u,FRAME)
    return u.reg_read(UC_X86_REG_EAX)

def exercise(token,stage=1,delta=1,size=6,x=50,y=60,rotation=0,outputs=None,building=BID):
    global checked
    expected=None
    if outputs is None:outputs=(OUT,OUT+8)
    for c,u,state in machines:
        modes=['named','placement','building','city']
        if token==0x40eb90:modes.append('laborer')
        if available('FUN_%08x'%token):modes.append('legacy')
        for mode in (modes if c else ['native']):
            fixture(u,token,stage,delta,size,x,y,rotation)
            state['writes']=[];previous[0]=None
            value=invoke(c,u,token,building,*outputs,mode)
            result=(value,tuple(state['writes']))
            if expected is None:expected=result
            else:assert result==expected,(hex(token),stage,delta,size,x,y,hex(rotation&0xffffffff),tuple(hex(a) for a in outputs),hex(building),mode,'native',expected,'C',result)
    checked+=1

if __name__=='__main__':
    for stage,size,delta in itertools.product([-2147483648,-1,0,1,2,3,4,21,2147483647],
            [-2147483648,-65537,-513,-257,-3,-1,0,1,3,255,256,511,65535,2147483647],[-1,0,1,24]):
        exercise(0x40eb90,stage=stage,size=size,delta=delta)
    for token,x,y,delta in itertools.product(METHODS,[-32768,-1,0,255,32767],[-32768,-1,0,255,32767],[-1,0,1]):
        exercise(token,x=x,y=y,delta=delta,stage=4)
    rotations=[-2147483648,-65537,-3,-2,-1,0,1,2,3,4,5,6,7,8,255,256,65535,2147483647]
    for rotation,delta in itertools.product(rotations,[-1,0,1,24]):exercise(0x417490,rotation=rotation,delta=delta)
    for rotation in [-2147483648,-65537,-3,-2,8,255,256,2147483647]:exercise(0x417490,rotation=rotation,outputs=(0,0))
    for token,stage,rotation in itertools.product(METHODS,[1,4],[0,2,4,6]):
        root=(BID-1)*264
        record=(BID if token==0x4e6ae0 or (token==0x40eb90 and stage<3) else BID-1)*264
        for outputs in [(OUT,OUT),(OUT+1,OUT+3),(OUT+0xf1,OUT+0x201),
                        (0x7999de+record,OUT),(0x7999e0+record,OUT),(0x7999e0+record-1,OUT),
                        (0x7999ea+root,OUT),(0x799a30+BID*264,OUT),(OBJ+0x54+52,OUT),
                        (0xbca2e0,OUT),(OUT,0x7999e0+record),(OBJ+4,OBJ+5)]:
            exercise(token,stage=stage,rotation=rotation,outputs=outputs)
    for token,building in itertools.product(METHODS,[BID+0x20000000,BID+0x80000000]):exercise(token,building=building)
    assert points<=hits,('unexecuted instructions',[hex(a) for a in sorted(points-hits)])
    missing={(a,b) for a,targets in branches.items() for b in targets if (a,b) not in edges}
    assert not missing,('missing conditional edges',missing)
    assert jump_edges==jump_targets
    # The creation member calls 40EB90 through its genuine vtable. Run the
    # whole method with its allocator, undo and map descendants as well.
    import verify_monument_create as construction
    for rotation,skip,size in itertools.product([0,2,4,6],[0,1],[1,3]):
        construction.exercise(0x40da90,rotation=rotation,skip=skip,size=size)
    for root in [0,3975]:construction.exercise(0x40da90,root=root)
    for occupied in [1,12,24]:construction.exercise(0x40da90,occupied=occupied)
    sources=['include/monument_entrance.h','runtime/monument_entrance.c','runtime/placement_queries.c',
             'runtime/actor_laborer_model.c','CMakeLists.txt','direct_c/verify_monument_entrance.py',
             'metadata/monument-entrance-native.json','direct_c/verify_monument_create.py',
             'runtime/monument_create.c','runtime/building_methods.c','include/monument_create.h']+[f'src/unit_{i:05d}.c' for i in [108,202,2002]]
    proof=dict(native_sha256=catalogue['native_sha256'],compiled_C_path=str(target.resolve()),compiled_C_sha256=target_sha,
               passed=True,checked=checked,invocations=invocations,
               coverage={hex(a):dict(instructions=len(v),executed=len(v&hits)) for a,v in owned.items()},conditional_edges=len(edges),
               indirect_jump_targets=[hex(a) for a in sorted(jump_edges)],
               complete_creation_compositions=construction.checked,composition_invocations=construction.invocations,
               source_hashes={p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest() for p in sources},
               controlled_algorithm_endpoints=[],controlled_platform_endpoints=[],
               limitations=['Nonfaulting mapped records/metadata/output addresses; no concurrency.',
                            'Full EAX and exact ordered DWORD writes compared; other volatile registers/flags are outside the C ABI.',
                            'Output aliases include records, metadata and rotation; private ABI frame corruption/fault behavior is outside scope.',
                            'Actual placement/building/city/laborer dispatchers execute, without substituted endpoints; no full-gameplay approval.'],commit=None)
    suffix='diagnostic' if target.stem=='restore_link_check' else 'game'
    (ROOT/f'metadata/monument-entrance-{suffix}.json').write_text(json.dumps(proof,indent=2)+'\n')
    print('PASS three whole entrance methods:',checked,'cases',invocations,'invocations',flush=True)
    print('Native instructions/conditional edges/indirect targets:',len(hits&points),len(edges),len(jump_edges),flush=True)
    print('Complete 25-part creation compositions:',construction.checked,'invocations:',construction.invocations,flush=True)
    print('Verified target SHA256:',target_sha,flush=True)
