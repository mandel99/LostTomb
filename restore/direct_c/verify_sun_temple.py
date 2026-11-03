"""Whole native/C Sun Temple creation and map writes, with actual descendants."""
from verify_options_startup import *
from unicorn import UC_HOOK_MEM_WRITE
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import json

CREATE,MAP,BID=0x5417b0,0x541920,64
NAMES={CREATE:'restore_sun_temple_create',MAP:'restore_sun_temple_map'}
FRAME,VSP=S+0x8000,S+0x18000
catalogue=json.loads((ROOT/'metadata/sun-temple-native.json').read_text())
assert catalogue['native_sha256']==hashlib.sha256(original.read_bytes()).hexdigest()
layout=catalogue['layout'];layout_bytes=bytes(images[0].data[0x601dd8-images[0].base:0x603228-images[0].base])
assert len(layout_bytes)==layout['bytes']==100*52
assert hashlib.sha256(layout_bytes).hexdigest()==layout['sha256']
tuples=[struct.unpack_from('<4i',layout_bytes,i*52) for i in range(100)]
representatives={v[3]:i for i,v in enumerate(tuples)}
owned={};branches={};hits=set();edges=set();previous=[None];checked={'map':0,'create':0};invocations=0
for f in catalogue['functions']:
    a=int(f['address'],16);b=bytes.fromhex(f['rea_bytes']['bytes_hex'])
    assert f['rea_bytes']['complete'] and b==bytes(images[0].data[a-images[0].base:a-images[0].base+len(b)])
    code=list(Cs(CS_ARCH_X86,CS_MODE_32).disasm(b,a));assert sum(i.size for i in code)==len(b)
    assert [hex(i.address) for i in code]==[s.split(':')[0] for s in f['result']['instructions']]
    owned[a]={i.address for i in code}
    branches.update({i.address:{int(i.op_str,16),i.address+i.size} for i in code if i.mnemonic.startswith('j') and i.mnemonic!='jmp'})
points=set.union(*owned.values())
available=lambda name:bool(re.search(r'\s_'+re.escape(name)+r'\s',symbols))
publics=sorted(set(int(x,16) for x in re.findall(r'^\s+[0-9A-Fa-f]{4}:[0-9A-Fa-f]+\s+\S+\s+([0-9A-Fa-f]{8})\s',symbols,re.M)))
compiled_ranges=[(sym(n),next(a for a in publics if a>sym(n))) for n in NAMES.values()]
calls={MAP:1,0x5414b0:2,0x46a400:6,0x485100:1,0x4f4f80:1,0x47bf50:7}
callee_names={MAP:NAMES[MAP],0x5414b0:'restore_placement_image_005414b0',0x46a400:'restore_building_allocate_0046a400',
              0x485100:'FUN_00485100',0x4f4f80:'FUN_004f4f80',0x47bf50:'FUN_0047bf50'}
machines=[]
for c,pe in enumerate(images):
    u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(pe.base,(len(pe.data)+4095)&~4095);u.mem_write(pe.base,bytes(pe.data))
    u.mem_map(0x30000000,0x10000);u.mem_map(STOP,4096);u.mem_map(S,0x50000);u.mem_map(0,4096)
    state={'hooks':{},'calls':[],'collect':False};machines.append((c,u,state))
    def stores(uc,access,at,size,value,data,c=c):
        if not data['collect'] or not 0x560000<=at<0x126d000:return
        pc=uc.reg_read(UC_X86_REG_EIP)
        own=pc in points if not c else any(lo<=pc<hi for lo,hi in compiled_ranges)
        if own:data['writes'].append((at,size,value&((1<<(size*8))-1)))
    u.hook_add(UC_HOOK_MEM_WRITE,stores,state)
    def observe(uc,pc,size,data):
        if not data['collect']:return
        hits.add(pc)
        if previous[0] in branches:edges.add((previous[0],pc))
        previous[0]=pc
    if not c:
        for addresses in owned.values():u.hook_add(UC_HOOK_CODE,observe,state,begin=min(addresses),end=max(addresses))
    def entry(uc,pc,size,data,t,c=c):
        if not data['collect']:return
        sp=uc.reg_read(UC_X86_REG_ESP);args=tuple(get(uc,sp+4+i*4) for i in range(calls[t]))
        if t==0x5414b0:args=(args[0],)  # Private output pointer differs between C/native frames.
        if t==0x47bf50:args=(args[0]&65535,*args[1:])
        bid=args[0] if t in [MAP,0x5414b0,0x485100,0x4f4f80] else 0
        record=bytes(uc.mem_read((0x7999d8+bid*264)&0xffffffff,264))
        data['events'].append((t,args,record))
        if data.get('mutation')=='allocate' and t==0x46a400 and not data['mutated']:
            data['mutated']=True;put(uc,0xeaac28,3);put(uc,0x601e14,3)
        if data.get('mutation')=='image' and t==0x5414b0 and not data['mutated']:
            data['mutated']=True;put(uc,0x601de0+get(uc,0x7999ea+bid*264)%65536*52,3)
    for t,n in calls.items():
        at=t if not c else sym(callee_names[t])
        def on_entry(uc,pc,size,data,t=t,handler=entry):handler(uc,pc,size,data,t)
        u.hook_add(UC_HOOK_CODE,on_entry,state,begin=at,end=at)

def cache_libraries(u,stage):
    which=0 if stage<9 else 2 if stage>=13 else 1
    ptr=get(u,0x603228+which*4);name=bytes(u.mem_read(ptr,64)).split(b'\0')[0]
    for slot in [5,6,7]:
        lib=0xf55bb0+0x1f40+slot*0xcc3c
        u.mem_write(lib,name+b'\0');put(u,lib+0xcc2c,1);put(u,lib+0xcc30,0)
        put(u,0xf55bb0+0x30f548+(slot-1)*4,1);put(u,0xf55bb0+0x30f548+slot*4,1)

def fixture(u,token,index=0,stage=0,size=None,rotation=0,zero=False,x=50,y=50,root=BID,
            registration=None,occupied=0,undo=0,dirty=False,shift=False,kind=None,mutation=None):
    u.mem_write(0x7999d8,bytes(264*4001));u.mem_write(0xba746c,bytes(0x22000))
    for a,v in [(0xbca2d8,10000),(0xbca2d0,100),(0xbca2d4,100),(0xbca2e0,rotation),(0xe94e04,rotation),
                (0xeaac28,0),(0x78d790,root),(0x78d7b4,100),(0x70e120,0),(0xe38dcc,0),(0xbc9694,1)]:put(u,a,v)
    for tag in range(1000):put(u,0xf55bb4+tag*8,0 if zero else 90000+tag*16)
    if shift:
        for i in range(100):
            at=0x601dd8+i*52;put(u,at,get(u,at)+1);put(u,at+4,get(u,at+4)-2)
    at=0x601dd8+index*52
    if size is not None:put(u,at+8,size)
    if kind is not None:put(u,at+12,kind)
    r=(root*264)&0xffffffff
    if dirty:u.mem_write(0x7999d8+r,b'\xa5'*264)
    byte(u,0x7999d8+r,3);word(u,0x7999e8+r,0xd5);word(u,0x799a10+r,0)
    word(u,0x7999de+r,x);word(u,0x7999e0+r,y);word(u,0x7999ea+r,index)
    put(u,0x7999e4+r,10000+y*228+x);put(u,0x799a30+r,stage);put(u,0x799a2c+r,0 if registration is None else registration)
    if occupied:byte(u,0x7999d8+(root+occupied)*264,3)
    if registration is not None and token==CREATE:
        # Native4F4B70 selects the original requirement row for type213.
        n=run(0,machines[0][1],0x4f4b70,[root]) if u is machines[0][1] else REQUIREMENT
        assert n==REQUIREMENT
        for i in range(3):word(u,0x784de0+i*2,n if i==registration else -1);put(u,0xeaa978+i*4,0)
    if undo==1:word(u,0xba746c,root)
    elif undo==2:u.mem_write(0xba746c,b'\xff\x7f'*525)
    cache_libraries(u,stage)
    if token==MAP:
        for row in range(-1,8):
            for col in range(-1,8):
                cell=10000+(y+row)*228+x+col
                put(u,0xaf598c+cell*4,0x6fabcd92);word(u,0xb4e6fc+cell*2,0x9876)
                byte(u,0xb285cc+cell,0xe5);byte(u,0xb67d1c+cell,0x83)
                byte(u,0xacf85c+cell,(row*3+col*5)&255);put(u,0xb7482c+cell*4,0x12345678)
        word(u,0x7999de+r,x);word(u,0x7999e0+r,y);word(u,0x7999ea+r,index);put(u,0x799a30+r,stage)

def invoke(c,u,token,args,mode):
    global invocations
    invocations+=1
    if not c:return run(0,u,token,args,budget=100000000)
    if mode=='legacy':return run(1,u,token,args,budget=100000000)
    entry=sym(NAMES[token]);argv=args
    if mode=='dispatch':
        u.mem_write(VSP-512,b'\x6b'*576);put(u,VSP,STOP)
        for i,v in enumerate(args):put(u,VSP+4+i*4,v)
        u.mem_write(FRAME,struct.pack('<10I',0x89abcdef,0x22222222,0x98765432,saved[0],VSP,*saved[1:],0x202,sym('restore_building_create_dispatch')))
        entry=sym('restore_building_create_dispatch');argv=[0,token,FRAME]
    for reg,v in zip(registers,[0x89abcdef,0x22222222,0x98765432,saved[0],SP,*saved[1:]]):u.reg_write(reg,v)
    u.reg_write(UC_X86_REG_EFLAGS,0x202);put(u,SP,STOP)
    for i,v in enumerate(argv):put(u,SP+4+i*4,v)
    u.emu_start(entry,STOP,count=100000000)
    assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==SP+4
    assert [u.reg_read(registers[i]) for i in [3,5,6,7]]==saved
    if mode=='dispatch':
        assert get(u,FRAME+16)==VSP+4
        assert bytes(u.mem_read(VSP-512,64))==b'\x6b'*64 and bytes(u.mem_read(VSP+4+len(args)*4,60-len(args)*4))==b'\x6b'*(60-len(args)*4)

def exercise(token,**kw):
    expected=None
    for c,u,state in machines:
        modes=['named','dispatch']+(['legacy'] if available('FUN_%08x'%token) else [])
        for mode in modes if c else ['native']:
            state['collect']=False;reset(u,state);fixture(u,token,**kw)
            state.update(events=[],writes=[],collect=True,mutation=kw.get('mutation'),mutated=False);previous[0]=None
            args=[kw.get('root',BID)] if token==MAP else [kw.get('x',50),kw.get('y',50),kw.get('root',BID)]
            try:invoke(c,u,token,args,mode)
            except Exception as e:raise AssertionError(('execution',c,hex(token),kw,mode,hex(u.reg_read(UC_X86_REG_EIP)))) from e
            state['collect']=False;result=(tuple(state['events']),tuple(state['writes']),snapshot(u))
            if expected is None:expected=result
            elif result!=expected:
                diff=[next(((i,a,b) for i,(a,b) in enumerate(zip(x,y)) if a!=b),None) for x,y in zip(expected[:2],result[:2])]
                memory=next(((hex(0x560000+i),a,b) for i,(a,b) in enumerate(zip(expected[2],result[2])) if a!=b),None)
                raise AssertionError((hex(token),kw,mode,'events/stores',diff,'lengths',[(len(x),len(y)) for x,y in zip(expected[:2],result[:2])],'memory',memory))
    checked['create' if token==CREATE else 'map']+=1

# Discover the requirement index using the unchanged native query, not a stub.
_u=machines[0][1];reset(_u,machines[0][2]);word(_u,0x7999e8+BID*264,0xd5)
REQUIREMENT=run(0,_u,0x4f4b70,[BID]);assert REQUIREMENT<38

if __name__=='__main__':
    for index,stage in itertools.product(range(100),[0,1,2,4]):exercise(MAP,index=index,stage=stage,rotation=(index%4)*2)
    for kind in [0,1,6,8,12]:
        for stage,rotation in itertools.product([-2147483648,-1,3,9,13,2147483647],[0,2]):exercise(MAP,index=representatives[kind],stage=stage,rotation=rotation)
    for size in [-2147483648,-1,0,1,2,3,6]:exercise(MAP,index=representatives[12],size=size,stage=0)
    for kind,stage in itertools.product([8,12],[0,1,2,4]):exercise(MAP,index=representatives[kind],stage=stage,zero=True)
    for index in [-1,0,1,99]:exercise(MAP,index=index,kind=99,size=2)
    for root in [BID+0x20000000,BID+0x80000000]:exercise(MAP,root=root,index=representatives[8],stage=1)
    for at,base in [(0xf56b54,0xaf598c),(0xbc9768,0xb7482c)]:
        cell=(at-base)//4;y,x=divmod(cell-10000,228)
        exercise(MAP,index=representatives[8],size=3,stage=1,x=x,y=y)
    exercise(MAP,index=0,mutation='image')
    for root,rotation in itertools.product([0,10,3900],[0,2,4,6]):exercise(CREATE,root=root,rotation=rotation)
    for registration in [0,1,2]:exercise(CREATE,registration=registration)
    for occupied in [1,50,99]:exercise(CREATE,occupied=occupied)
    for undo in [1,2]:exercise(CREATE,undo=undo)
    for x,y in [(-1,0),(0,-1),(120,90)]:exercise(CREATE,x=x,y=y)
    for options in [dict(dirty=True),dict(shift=True),dict(mutation='allocate'),dict(mutation='image')]:exercise(CREATE,**options)
    assert points<=hits,('unexecuted instructions',[hex(a) for a in sorted(points-hits)])
    missing={(a,b) for a,targets in branches.items() for b in targets if (a,b) not in edges}
    assert not missing,('unexecuted edges',[(hex(a),hex(b)) for a,b in sorted(missing)])
    sources=['include/sun_temple.h','runtime/sun_temple.c','runtime/building_methods.c','src/unit_02546.c','src/unit_02547.c',
             'runtime/placement_palette.c','CMakeLists.txt','direct_c/verify_sun_temple.py','metadata/sun-temple-native.json']
    proof=dict(native_sha256=catalogue['native_sha256'],compiled_C_path=str(target.resolve()),compiled_C_sha256=target_sha,
               passed=True,checked=checked,invocations=invocations,coverage={hex(a):dict(instructions=len(x),executed=len(x&hits)) for a,x in owned.items()},
               conditional_edges=len(edges),controlled_algorithm_endpoints=[],controlled_platform_endpoints=[],
               source_hashes={p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest() for p in sources},
               limitations=['Valid mapped layout/records and resident sprite-library descriptors; no cold-file loading or fault-equivalence claim.',
                            'Whole callees execute. Compare all game globals, child-call record snapshots and ordered stores from the reconstructed members.',
                            'Mutation cases alter input globals at actual child entries without replacing calls or return values.',
                            'Void C ABI: private scratch-stack data, caller-saved registers and flags are outside comparison; no complete-gameplay approval.'],commit=None)
    suffix='diagnostic' if target.stem=='restore_link_check' else 'game'
    (ROOT/f'metadata/sun-temple-{suffix}.json').write_text(json.dumps(proof,indent=2)+'\n')
    print('PASS Sun Temple:',checked,'invocations',invocations,flush=True)
    print('Native instructions/conditional edges:',len(hits&points),len(edges),flush=True)
    print('Verified target SHA256:',target_sha,flush=True)
