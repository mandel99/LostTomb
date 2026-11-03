"""Actual monument image members, full EAX/flag order and resident-library calls."""
from verify_options_startup import *
from unicorn import UC_HOOK_MEM_WRITE
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import json

OBJ,OUT,FRAME,VSP=S+0x1000,S+0x3000,S+0x8000,S+0x18000
BID=64
METHODS={0x401800:(0x56f438,16),0x40de80:(0x56f48c,25),0x417310:(0x56f4dc,9),0x4e6a70:(0x56f6e8,2)}
available=lambda name:bool(re.search(r'\s_'+re.escape(name)+r'\s',symbols))
catalogue=json.loads((ROOT/'metadata/monument-images-native.json').read_text())
assert catalogue['native_sha256']==hashlib.sha256(original.read_bytes()).hexdigest()
owned={};branches={};indirect={};hits=set();edges=set();jump_edges=set();previous=[None];checked=0;invocations=0
for row in catalogue['functions']:
    a=int(row['address'],16);b=bytes.fromhex(row['body_bytes_hex']);assert bytes(images[0].data[a-images[0].base:a-images[0].base+len(b)])==b
    code=list(Cs(CS_ARCH_X86,CS_MODE_32).disasm(b,a));assert sum(i.size for i in code)==len(b)
    owned[a]={i.address for i in code}
    for i in code:
        if i.mnemonic.startswith('j') and i.mnemonic!='jmp':branches[i.address]={int(i.op_str,16),i.address+i.size}
        elif i.mnemonic=='jmp' and '[' in i.op_str:indirect[i.address]=i.op_str
for row in catalogue['tables']:
    a=int(row['address'],16);b=bytes.fromhex(row['bytes_hex']);assert bytes(images[0].data[a-images[0].base:a-images[0].base+len(b)])==b
expected_jumps=set()
for site,operand in indirect.items():
    table_address=int(re.findall(r'0x[0-9a-f]+',operand)[-1],16)
    count=12 if table_address==0x401a48 else 25 if table_address==0x40eb20 else 4
    expected_jumps.update((site,destination) for destination in struct.unpack_from('<%dI'%count,images[0].data,table_address-images[0].base))
points=set.union(*owned.values());machines=[]
for c,pe in enumerate(images):
    u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(pe.base,(len(pe.data)+4095)&~4095);u.mem_write(pe.base,bytes(pe.data))
    u.mem_map(0x30000000,0x10000);u.mem_map(STOP,4096);u.mem_map(S,0x50000);u.mem_map(0,4096)
    state={'hooks':{},'calls':[],'collect':False,'writes':[],'loads':[]};machines.append((c,u,state))
    def writes(uc,access,at,size,value,data):
        if data['collect'] and (0x560000<=at<0x126d000 or S<=at<S+0x6000 or (data['output']==0 and at==0)):
            data['writes'].append((at,size,value&((1<<(size*8))-1)))
    u.hook_add(UC_HOOK_MEM_WRITE,writes,state)
    def loader(uc,pc,size,data,c=c):
        if not data['collect']:return
        sp=uc.reg_read(UC_X86_REG_ESP)
        if c:assert get(uc,sp+4)==0xf55bb0;name=get(uc,sp+8);slot=get(uc,sp+12)
        else:assert uc.reg_read(UC_X86_REG_ECX)==0xf55bb0;name=get(uc,sp+4);slot=get(uc,sp+8)
        data['loads'].append((name,slot,get(uc,data['output']) if data['output'] else None))
    entry=address(c,0x53dd90);u.hook_add(UC_HOOK_CODE,loader,state,begin=entry,end=entry)
    if not c:
        def observe(uc,pc,size,data):
            if not data['collect']:return
            hits.add(pc)
            if previous[0] in branches:edges.add((previous[0],pc))
            if previous[0] in indirect:jump_edges.add((previous[0],pc))
            if pc==0x417437:
                table_base=uc.reg_read(UC_X86_REG_ESP)+16
                assert list(struct.unpack('<36I',bytes(uc.mem_read(table_base,144))))==catalogue['nine_rotation']
            previous[0]=pc
        for addresses in owned.values():u.hook_add(UC_HOOK_CODE,observe,state,begin=min(addresses),end=max(addresses))

tags=struct.pack('<2000I',*[v for i in range(1000) for v in (0,100000+i*16)])
def fixture(u,token,category,stage,root_stage,rotation,part,zero,cached):
    u.mem_write(OBJ,bytes(0x1000));u.mem_write(OUT-8,b'\x6b'*64)
    put(u,OBJ,METHODS[token][0]);put(u,0xbca2e0,rotation)
    for i in range(METHODS[token][1]):put(u,OBJ+0x58+i*52,i)
    if token!=0x417310:put(u,OBJ+0x58+part*52,category)
    for bid in range(BID-32,BID+33):
        u.mem_write(0x7999d8+bid*264,bytes(264));put(u,0x799a30+bid*264,stage)
    word(u,0x7999ea+BID*264,part);put(u,0x799a30+(BID-part)*264,root_stage)
    if part:put(u,0x799a30+BID*264,stage)
    if token in [0x417310,0x4e6a70]:
        for i in range(32):
            p=S+0x5000+i*64;u.mem_write(p,('phase%02d\0'%i).encode())
            put(u,OBJ+(0x220 if token==0x417310 else 0xb4)+i*4,p)
    u.mem_write(0xf55bb0,bytes(len(tags)) if zero else tags)
    for slot,name in cached:
        lib=0xf55bb0+0x1f40+slot*0xcc3c;u.mem_write(lib,name+b'\0')
        put(u,lib+0xcc2c,1);put(u,lib+0xcc30,4)
        put(u,0xf55bb0+0x30f548+slot*4,5+slot*4)
        if slot:put(u,0xf55bb0+0x30f548+(slot-1)*4,1+slot*4)

def invoke(c,u,token,output,mode,probe=False):
    global invocations
    if not probe:invocations+=1
    for reg,v in zip(registers,[0x89abcdef,OBJ,0x98765432,saved[0],SP,*saved[1:]]):u.reg_write(reg,v)
    u.mem_write(SP-512,b'\xa6'*768);u.reg_write(UC_X86_REG_EFLAGS,0x202);put(u,SP,STOP)
    args=[BID,output];cleanup=0;frame_result=False
    if not c:entry=token;cleanup=8
    elif mode=='named':entry=sym('restore_monument_image_%08x'%token);args=[OBJ,BID,output]
    elif mode=='legacy':entry=address(c,token)
    else:
        frame_result=True;u.mem_write(VSP-512,b'\xa6'*768);put(u,VSP,STOP);put(u,VSP+4,BID);put(u,VSP+8,output)
        u.mem_write(FRAME,struct.pack('<10I',0x89abcdef,OBJ,0x98765432,saved[0],VSP,*saved[1:],0x202,sym('restore_placement_queries_dispatch')))
        entry=sym('restore_placement_queries_dispatch');args=[0,token,FRAME]
    for i,v in enumerate(args):put(u,SP+4+i*4,v)
    u.emu_start(entry,STOP,count=1000000)
    if probe and u.reg_read(UC_X86_REG_EIP)==0x53dd90:return
    assert u.reg_read(UC_X86_REG_EIP)==STOP,(c,hex(token),mode,hex(u.reg_read(UC_X86_REG_EIP)))
    assert u.reg_read(UC_X86_REG_ESP)==SP+4+cleanup
    assert [u.reg_read(registers[i]) for i in [3,5,6,7]]==saved
    if frame_result:
        assert get(u,FRAME+16)==VSP+12;assert [get(u,FRAME+i*4) for i in [3,5,6,7]]==saved
        assert bytes(u.mem_read(VSP-512,64))==b'\xa6'*64
        return get(u,FRAME)
    return u.reg_read(UC_X86_REG_EAX)

def exercise(token,category=0,stage=0,root_stage=None,rotation=0,part=1,zero=False,output=OUT):
    global checked
    if root_stage is None:root_stage=stage
    c,u,state=machines[0];state['collect']=False;reset(u,state);fixture(u,token,category,stage,root_stage,rotation,part,zero,[]);cached=[]
    def discover(uc,pc,n,data):
        sp=uc.reg_read(UC_X86_REG_ESP);name=get(uc,sp+4);slot=get(uc,sp+8)
        if name:
            assert slot<30;cached.append((slot,bytes(uc.mem_read(name,128)).split(b'\0')[0]));uc.emu_stop()
    h=u.hook_add(UC_HOOK_CODE,discover,begin=0x53dd90,end=0x53dd90)
    try:invoke(0,u,token,output,'native',probe=True)
    finally:u.hook_del(h)
    signed=struct.unpack('<i',struct.pack('<I',rotation&0xffffffff))[0]
    quarter=signed//2 if signed>=0 else-((-signed)//2)
    context_only=token==0x417310 and ((quarter*9+part)&0x3fffffff)>=36
    expected=None
    for c,u,state in machines:
        modes=['dispatch'] if context_only else (['named'] if available('restore_monument_image_%08x'%token) else [])+['dispatch']+(['legacy'] if available('FUN_%08x'%token) else [])
        for mode in (modes if c else ['native']):
            state['collect']=False;reset(u,state);fixture(u,token,category,stage,root_stage,rotation,part,zero,cached)
            state['writes']=[];state['loads']=[];state['output']=output;state['collect']=True;previous[0]=None
            try:value=invoke(c,u,token,output,mode)
            except Exception as error:raise AssertionError(('execution',c,hex(token),category,stage,root_stage,rotation,part,hex(output),mode,hex(u.reg_read(UC_X86_REG_EIP)))) from error
            state['collect']=False;result=(value,tuple(state['writes']),state['loads'])
            if expected is None:expected=result
            else:assert result==expected,(hex(token),category,stage,root_stage,rotation,part,zero,hex(output),mode,'native',expected,'C',result)
    checked+=1

if __name__=='__main__':
    phases=[-2147483648,-1,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,2147483647]
    for category,stage,rotation in itertools.product(range(25),phases,[0,2,4,6]):exercise(0x40de80,category=category,stage=stage,rotation=rotation)
    for category in [25,0xffffffff]:exercise(0x40de80,category=category)
    for category,stage,rotation in itertools.product(range(12),[-2147483648,-1,0,1,9,2147483647],[0,2,4,6]):exercise(0x401800,category=category,stage=stage,rotation=rotation)
    for category,rotation in itertools.product(range(12),[-2147483648,-3,-2,-1,1,3,5,7,8,2147483647]):exercise(0x401800,category=category,stage=1,rotation=rotation)
    for category in [12,0xffffffff]:exercise(0x401800,category=category)
    for part,rotation in itertools.product(range(9),[0,2,4,6]):exercise(0x417310,part=part,stage=19,rotation=rotation)
    for part,rotation in itertools.product([0,5,6,7,8],[-8,-2,8,10,-2147483648,2147483647]):exercise(0x417310,part=part,stage=3,rotation=rotation)
    for part in [1,2,3]:exercise(0x417310,part=part,rotation=8,stage=3)
    for part,category,stage in itertools.product([0,1],[0,1,2,0xffffffff],[0,1,9]):exercise(0x4e6a70,part=part,category=category,stage=stage)
    for category in [1,2,0xffffffff]:exercise(0x4e6a70,category=category,output=0)
    for token,stage,rotation in itertools.product(METHODS,[0,2,16,21],[0,2]):
        category=21 if token==0x40de80 else 9 if token==0x401800 else 0
        for output in [OUT+1,0xbca2e0,OBJ+0x58+52,0x7999ea+BID*264,0x799a30+BID*264,
                       0x799a30+(BID-1)*264,0xf56b54,0xf573bc,0xf57394,0xf5739c,0xf5753c]:
            exercise(token,category=category,stage=stage,rotation=rotation,output=output)
        exercise(token,category=category,stage=stage,rotation=rotation,zero=True)
    for stage,root_stage in [(1,21),(2,16),(3,1),(4,16),(21,0)]:exercise(0x40de80,category=21,stage=stage,root_stage=root_stage)
    print('PASS four image providers and actual library calls:',checked,flush=True)
    assert points<=hits,('unexecuted instructions',[hex(a) for a in sorted(points-hits)])
    missing={(a,b) for a,targets in branches.items() for b in targets if (a,b) not in edges}
    assert not missing,('unexecuted edges',[(hex(a),hex(b)) for a,b in sorted(missing)])
    assert jump_edges==expected_jumps
    import verify_monument_map as painting
    for stage,rotation in itertools.product([0,2,16,21],[0,2]):painting.exercise(0x56f48c,index=13,extra=21,stage=stage,rotation=rotation)
    for table,indices in [(0x56f438,[0,15]),(0x56f4dc,[0,8])]:
        for index,rotation in itertools.product(indices,[0,2,4,6]):painting.exercise(table,index=index,stage=3,rotation=rotation)
    for extra in [0,1,2]:painting.exercise(0x56f6e8,index=1,extra=extra,stage=3)
    import verify_monument_create as construction
    for token in [0x40da90,0x417180,0x4e6840]:construction.exercise(token)
    sources=['include/monument_images.h','runtime/monument_images.c','runtime/placement_queries.c','CMakeLists.txt',
             'direct_c/verify_monument_images.py','metadata/monument-images-native.json','direct_c/verify_monument_map.py',
             'direct_c/verify_monument_create.py','runtime/monument_map.c','runtime/monument_create.c']
    proof=dict(native_sha256=catalogue['native_sha256'],compiled_C_path=str(target.resolve()),compiled_C_sha256=target_sha,
               passed=True,checked=checked,invocations=invocations,
               coverage={hex(a):dict(instructions=len(v),executed=len(v&hits)) for a,v in owned.items()},conditional_edges=len(edges),
               indirect_jump_edges=len(jump_edges),map_compositions=painting.checked,map_composition_invocations=painting.invocations,
               creation_compositions=construction.checked,creation_composition_invocations=construction.invocations,
               legacy_entries_compared=[hex(a) for a in METHODS if available('FUN_%08x'%a)],
               source_hashes={p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest() for p in sources},
               controlled_algorithm_endpoints=[],controlled_platform_endpoints=[],
               limitations=['Body boundaries are inferred contiguous ranges authenticated by REA byte reads; these entries lack Ghidra function-manager definitions.',
                            'Mapped records/metadata/output, stable native control tables, no concurrency or fault-equivalence claim.',
                            'Actual resident-library matching executes; native preflight only observes/stops a requested load, without substituting verification callees.',
                            'Full EAX, flag writes and ordered game-global stores/loader requests compared; other volatile registers/flags are outside the C ABI.',
                            'Nine-part indices outside36 effective slots are compared through explicit native frames, including saved registers and inherited words; named/C aliases require a table slot.',
                            'No cold-file loading, pixel decoding or full-gameplay approval.'],commit=None)
    suffix='diagnostic' if target.stem=='restore_link_check' else 'game'
    (ROOT/f'metadata/monument-images-{suffix}.json').write_text(json.dumps(proof,indent=2)+'\n')
    print('Native instructions/edges/indirect targets:',len(hits&points),len(edges),len(jump_edges),flush=True)
    print('Map/whole-creation compositions:',painting.checked,construction.checked,flush=True)
    print('Verified target SHA256:',target_sha,flush=True)
