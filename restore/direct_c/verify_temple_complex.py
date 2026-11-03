"""Four temple-complex methods and shared upgrade predicate, actual children.

Compare owned ordered global stores, all final original data, real child
arguments, AL-only predicate results, saved registers and stack balance.
"""
from verify_options_startup import *
from unicorn import UC_HOOK_MEM_WRITE
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import json

BID,FRAME,VSP=64,S+0x8000,S+0x18000
catalogue=json.loads((ROOT/'metadata/temple-complex-native.json').read_text())
assert catalogue['native_sha256']==hashlib.sha256(original.read_bytes()).hexdigest()
owned={};branches={};hits=set();edges=set();last=[None];jump_edges=set();checked=invocations=0
decoder=Cs(CS_ARCH_X86,CS_MODE_32)
for f in catalogue['functions']:
    a=int(f['address'],16);raw=bytes.fromhex(f['bytes_hex'])
    assert raw==bytes(images[0].data[a-images[0].base:a-images[0].base+len(raw)])
    instructions=list(decoder.disasm(raw,a));owned[a]={int(p,16) for p in f['instruction_addresses'].split()}
    assert {i.address for i in instructions}==owned[a]
    for i in instructions:
        if i.mnemonic.startswith('j') and i.mnemonic!='jmp':branches[i.address]={int(i.op_str,16),i.address+i.size}
for table in catalogue['tables'].values():
    a=int(table['address'],16);raw=bytes.fromhex(table['bytes_hex'])
    assert table['complete'] and raw==bytes(images[0].data[a-images[0].base:a-images[0].base+len(raw)])
expected_jumps={(0x4c362b,v) for v in struct.unpack('<7I',bytes.fromhex(catalogue['tables']['constructor_switch']['bytes_hex']))}
points=set.union(*owned.values())
names={0x4c0860:'restore_temple_complex_tile',0x4c0be0:'restore_temple_complex_decorate',0x4c0d50:'restore_temple_complex_redraw',0x4c3420:'restore_temple_complex_construct',0x4c52d0:'restore_temple_upgrade_present'}
available=cache(lambda n:bool(re.search(r'\s_'+re.escape(n)+r'\s',symbols)))
publics=sorted({int(a,16) for a in re.findall(r'^\s+[0-9a-fA-F]{4}:[0-9a-fA-F]+\s+\S+\s+([0-9a-fA-F]{8})\s',symbols,re.M)})
compiled_ranges=[(sym(n),next(a for a in publics if a>sym(n))) for n in names.values()]
calls={0x4c0860:4,0x4c0be0:1,0x4c52d0:2,0x46f930:2,0x46a400:6,0x485100:1,0x47bf50:7,0x4746d0:3,0x475b50:1,0x4c5120:1}
callee_names={**names,0x46a400:'restore_building_allocate_0046a400',**{t:'FUN_%08x'%t for t in [0x46f930,0x485100,0x47bf50,0x4746d0,0x475b50,0x4c5120]}}
callee_names[0x4c52d0]='FUN_004c52d0'
machines=[]
for c,pe in enumerate(images):
    u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(pe.base,(len(pe.data)+4095)&~4095);u.mem_write(pe.base,bytes(pe.data))
    u.mem_map(0x30000000,0x10000);u.mem_map(STOP,4096);u.mem_map(S,0x50000)
    state=dict(writes=[],events=[],collect=False,compiled=c);machines.append((c,u,state))
    def stores(uc,access,at,size,value,data):
        if not data['collect']:return
        pc=uc.reg_read(UC_X86_REG_EIP)
        own=pc in points if not data['compiled'] else any(lo<=pc<hi for lo,hi in compiled_ranges)
        if own:data['writes'].append((at,size,value&((1<<(8*size))-1)))
    u.hook_add(UC_HOOK_MEM_WRITE,stores,state,begin=0x560000,end=0x126cfff)
    if not c:
        def observe(uc,pc,size,data):
            if not data['collect']:return
            hits.add(pc)
            if last[0] in branches:edges.add((last[0],pc))
            if last[0]==0x4c362b:jump_edges.add((last[0],pc))
            last[0]=pc
        for rows in owned.values():u.hook_add(UC_HOOK_CODE,observe,state,begin=min(rows),end=max(rows))
    def entry(uc,pc,size,data):
        if not data['collect']:return
        t=data['entries'][pc];sp=uc.reg_read(UC_X86_REG_ESP)
        args=tuple(get(uc,sp+4+i*4) for i in range(calls[t]))
        if t==0x47bf50:args=(args[0]&65535,*args[1:])
        # Observe immediate leaf calls. Existing stamp helpers may inline
        # their own terrain refresh; decoration may be a C tail call.
        ret=get(uc,sp)
        immediate=ret in points if not data['compiled'] else any(lo<=ret<hi for lo,hi in compiled_ranges)
        if immediate and t!=0x4c0be0:data['events'].append((t,args))
        data['counts'][t]=data['counts'].get(t,0)+1
        first=data['counts'][t]==1;mutation=data['mutation']
        if mutation=='bounds' and t==0x46f930 and first:
            put(uc,0xbca2d8,1200);put(uc,0xf55bb4+570*8,0x12345678);put(uc,0xeaac28,3)
        if mutation=='tile' and t==0x4c0860 and first:
            byte(uc,0x799a82+BID*264,4);word(uc,0x7999de+BID*264,110);put(uc,0xbca2e0,4)
            put(uc,0x5df9b0+90*20,0)
        if mutation=='stamp' and t==0x47bf50 and first:
            word(uc,0x799a12+BID*264,BID+2);put(uc,0xbca2e0,2)
            byte(uc,0x799a82+BID*264,4)
        if mutation=='allocate' and t==0x46a400 and first:
            put(uc,0xbca2e0,4);put(uc,0x5df970,4);put(uc,0x5df974,1)
        if mutation=='undo' and t==0x485100 and data['counts'][t]==2:
            word(uc,0x799a12+BID*264,71)
        if mutation=='category' and t==0x4c5120 and first:
            word(uc,0x7999e8+args[0]*264,69);put(uc,0xeaac28,3)
    state['entries']={(sym(callee_names[t]) if c else t):t for t in calls}
    for at in state['entries']:u.hook_add(UC_HOOK_CODE,entry,state,begin=at,end=at)

def fixture(u,token,direction=0,rotation=0,kind=65,upgrades=0,initialize=1,parent=0,root=BID,offset=0,chain=3,player=0,xy=(90,80),clip=False,full=False):
    u.mem_write(0x560000,seed);u.mem_write(S,bytes(0x50000))
    u.mem_write(0x7999d8,bytes(4000*264));u.mem_write(0xba746c,bytes(0x22000));u.mem_write(0x78d580,bytes(0x320))
    for at,v in [(0xbca2d8,1000),(0xbca2d0,200),(0xbca2d4,200),(0xbca2dc,28),(0xbca2e0,rotation),(0xe94e04,rotation),
                 (0xbca2e8,0),(0xbca2ec,0),(0xbca2f0,200),(0xbca2f4,200),(0xbca2f8,100),(0xbca2fc,100),
                 (0xeaac28,player),(0xbc9694,1),(0x78d7b4,100),(0xbc9714,((direction+rotation)//2)%4)]:put(u,at,v)
    for i in range(1000):put(u,0xf55bb4+i*8,100000+i*16)
    for i in range(36):put(u,0x5cc8c0+i*8,(i//6)*228+i%6)
    for at,scale in [(0xaf598c,4),(0xb4e6fc,2),(0xb7482c,4),(0xb285cc,1),(0xb67d1c,1),(0xae8e7c,1),(0xacf85c,1)]:
        u.mem_write(at+14000*scale,bytes(14000*scale))
    for j in range(max(chain,1)):
        id=(root+j)&0xffffffff;r=(id*264)&0xffffffff
        byte(u,(0x7999d8+r)&0xffffffff,1);byte(u,(0x7999db+r)&0xffffffff,3)
        word(u,(0x7999e8+r)&0xffffffff,kind);word(u,(0x799a10+r)&0xffffffff,parent if not j else id-1)
        word(u,(0x799a12+r)&0xffffffff,id+1 if j+1<chain else 0);byte(u,(0x799a82+r)&0xffffffff,direction)
        word(u,(0x7999de+r)&0xffffffff,xy[0]+j*3);word(u,(0x7999e0+r)&0xffffffff,xy[1])
        byte(u,(0x799a81+r)&0xffffffff,upgrades)
    for city in [0,1,3]:put(u,0xea5f9c+city*0x49d8,root)
    for reg,v in [(UC_X86_REG_FPCW,0x37f),(UC_X86_REG_FPSW,0),(UC_X86_REG_FPTAG,0xffff)]:u.reg_write(reg,v)
    if full:
        for i in range(1,4000):byte(u,0x7999d8+i*264,1)
    if clip:put(u,0xbca2e8,10000)
    if token==0x4c0860:
        cell=1000+xy[1]*228+xy[0]
        put(u,0xaf598c+cell*4,0x10203040);byte(u,0xb67d1c+cell,0x81);byte(u,0xb285cc+cell,0xab);byte(u,0xae8e7c+cell,0x7e)
        return [xy[0],xy[1],570,offset]
    if token==0x4c0d50:return [root,offset]
    if token==0x4c3420:return [root,xy[0],xy[1],player,kind,initialize]
    if token==0x4c52d0:return [root,offset]
    return [root]

def invoke(c,u,token,args,route):
    global invocations
    invocations+=1;argv=args
    if not c:entry=token
    elif route=='named':entry=sym(names[token])
    elif route=='legacy':entry=sym('FUN_%08x'%token)
    else:
        u.mem_write(VSP-256,b'\x6b'*384);put(u,VSP,STOP)
        for i,v in enumerate(args):put(u,VSP+4+i*4,v)
        u.mem_write(FRAME,struct.pack('<10I',0x89abcdef,0x22222222,0x98765432,saved[0],VSP,*saved[1:],0x202,0))
        if route=='frame':entry=sym('restore_building_control_C_%08x'%token);argv=[FRAME]
        else:entry=sym('restore_building_control_raw');argv=[token,FRAME,FRAME+32]
    value=run(0,u,entry,argv,budget=12000000)
    if c and route in ['frame','raw']:
        assert get(u,FRAME+16)==VSP+4 and [get(u,FRAME+i*4) for i in [3,5,6,7]]==saved
        assert bytes(u.mem_read(VSP+4+len(args)*4,64))==b'\x6b'*64
        value=get(u,FRAME)
    return value

def exercise(token,mutation='',**values):
    global checked
    expected=None;native_result=None
    for c,u,state in machines:
        routes=['named','frame','raw']+(['legacy'] if available('FUN_%08x'%token) else [])
        for route in routes if c else ['native']:
            state['collect']=False;args=fixture(u,token,**values)
            state.update(writes=[],events=[],collect=True,counts={},mutation=mutation);last[0]=None
            try:value=invoke(c,u,token,args,route)
            except Exception as e:raise AssertionError(('execution',hex(token),values,mutation,route,hex(u.reg_read(UC_X86_REG_EIP)))) from e
            state['collect']=False
            if not c:native_result=value
            if token==0x4c52d0 and c and route in ['frame','raw']:assert value==native_result
            result=(tuple(state['writes']),tuple(state['events']),snapshot(u),value&255 if token==0x4c52d0 else None)
            if expected is None:expected=result
            elif result!=expected:
                differences=[next(((i,a,b) for i,(a,b) in enumerate(zip(x,y)) if a!=b),None) for x,y in zip(expected[:2],result[:2])]
                memory=next(((hex(0x560000+i),a,b) for i,(a,b) in enumerate(zip(expected[2],result[2])) if a!=b),None)
                raise AssertionError((hex(token),values,mutation,route,'stores/events',differences,'lengths',[(len(x),len(y)) for x,y in zip(expected[:2],result[:2])],'memory',memory,'AL',expected[3],result[3]))
    checked+=1

if __name__=='__main__':
    for xy,clip,offset,player in itertools.product([(90,80),(-1,0),(201,80),(80,201),(1,1)],[False,True],[0,0xffffffff],[0,3]):
        exercise(0x4c0860,xy=xy,clip=clip,offset=offset,player=player)
    exercise(0x4c0860,mutation='bounds',offset=7)
    for direction,rotation in itertools.product([0,2,4,6],[0,2,4,6]):
        exercise(0x4c0be0,direction=direction,rotation=rotation)
    for direction in [1,3,5,7]:exercise(0x4c0be0,direction=direction,clip=True)
    exercise(0x4c0be0,mutation='tile')
    for direction,rotation in itertools.product([0,2,4,6],[0,2,4,6]):
        exercise(0x4c0d50,direction=direction,rotation=rotation,upgrades=(direction//2+rotation//2)%4,offset=17)
    for parent,root,chain in [(1,BID,3),(-1,BID,3),(0,0,0),(0,BID,1),(0,BID,2),(0,BID,4)]:
        exercise(0x4c0d50,parent=parent,root=root,chain=chain,offset=0xffffffff)
    exercise(0x4c0d50,upgrades=3,offset=17)
    for rotation in [-1,-2]:exercise(0x4c0d50,direction=6,rotation=rotation,upgrades=3)
    exercise(0x4c0d50,mutation='stamp',offset=7)
    for direction,rotation in itertools.product([0,2,4,6],[0,2,4,6]):
        exercise(0x4c3420,direction=direction,rotation=rotation,chain=1)
    for kind in range(66,70):exercise(0x4c3420,kind=kind,chain=1)
    for direction in [1,7]:exercise(0x4c3420,direction=direction,rotation=1,chain=1)
    for direction in range(8):exercise(0x4c3420,direction=direction,rotation=(direction*2)%8,initialize=0)
    exercise(0x4c3420,rotation=1,initialize=0)
    for parent,initialize in itertools.product([1,-1],[0,1]):exercise(0x4c3420,parent=parent,initialize=initialize)
    for mutation in ['allocate','undo','category']:exercise(0x4c3420,kind=69,mutation=mutation,chain=1)
    for kind,initialize in [(156,1),(160,1),(65,2),(69,0xffffffff)]:exercise(0x4c3420,kind=kind,initialize=initialize,chain=1)
    exercise(0x4c3420,full=True,chain=1)
    for upgrades,desired,root in itertools.product([0,1,2,3,255],[0,1,2,3,255,256,0xffffffff],[BID,BID+0x20000000]):
        exercise(0x4c52d0,upgrades=upgrades,offset=desired,root=root,chain=1)
    assert points<=hits,('unexecuted instructions',[hex(a) for a in sorted(points-hits)])
    missing={(a,b) for a,targets in branches.items() for b in targets if (a,b) not in edges}
    assert not missing,('unexecuted conditional edges',[(hex(a),hex(b)) for a,b in sorted(missing)])
    assert expected_jumps<=jump_edges
    sources=['CMakeLists.txt','include/temple_complex.h','runtime/temple_complex.c','runtime/building_controllers.c','runtime/building_controllers_model.c','include/temple_upgrade.h','src/unit_01606.c','direct_c/verify_temple_complex.py','metadata/temple-complex-native.json']+[f'src/unit_{i:05d}.c' for i in [1583,1585,1586,1590]]
    proof=dict(native_sha256=catalogue['native_sha256'],compiled_C_path=str(target.resolve()),compiled_C_sha256=target_sha,passed=True,checked=checked,invocations=invocations,
               coverage={hex(a):dict(instructions=len(p),executed=len(p&hits)) for a,p in owned.items()},conditional_edges=len(edges),switch_targets=len(jump_edges),
               controlled_algorithm_endpoints=[],controlled_platform_endpoints=[],source_hashes={p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest() for p in sources},commit=None,
               limitations=['Four complete analyzed void bodies and the shared AL-only upgrade predicate. Owned ordered stores, actual child inputs and all final original data compared.',
                            'Actual allocator, undo capture, map stamping, bounds, root/category queries and terrain refresh execute, including full-table allocation failure.',
                            'Child-entry input mutations test cached direction/geometry/view, fresh images/links/current player and constructor steps without replacing returns.',
                            'Mapped fixtures and resident data; no arbitrary invalid memory, concurrent access, host game launch, visual or complete-gameplay claim.'])
    suffix='diagnostic' if target.stem=='restore_link_check' else 'game'
    (ROOT/f'metadata/temple-complex-{suffix}.json').write_text(json.dumps(proof,indent=2)+'\n')
    print('PASS temple complex:',checked,'scenarios',invocations,'invocations;',len(points),'instructions,',len(edges),'conditional edges,',len(jump_edges),'switch targets',flush=True)
    print('Verified target SHA256:',target_sha,flush=True)
