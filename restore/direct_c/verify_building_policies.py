"""Storage allocation, gatehouse display and god lookup with real children.

Complete native scopes, ordered owned writes, actual immediate child inputs,
final original data, full DWORD returns, saved registers and stack balance.
"""
from verify_options_startup import *
from unicorn import UC_HOOK_MEM_WRITE
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import json

BID,FRAME,VSP=64,S+0x8000,S+0x18000
catalogue=json.loads((ROOT/'metadata/building-policies-native.json').read_text())
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
expected_jumps={(0x46b19f,v) for v in struct.unpack_from('<4I',bytes.fromhex(catalogue['tables']['storage_switch']['bytes_hex']))}
expected_jumps|={(0x4bfbd5,v) for v in struct.unpack('<8I',bytes.fromhex(catalogue['tables']['display_switch']['bytes_hex']))}
points=set.union(*owned.values());names={0x46b100:'restore_storage_settings_allocate',0x4bfb30:'restore_gatehouse_redraw',0x4c53d0:'restore_religion_find_god_by_status'}
available=cache(lambda n:bool(re.search(r'\s_'+re.escape(n)+r'\s',symbols)))
publics=sorted({int(a,16) for a in re.findall(r'^\s+[0-9a-fA-F]{4}:[0-9a-fA-F]+\s+\S+\s+([0-9a-fA-F]{8})\s',symbols,re.M)})
compiled_ranges=[(sym(n),next(a for a in publics if a>sym(n))) for n in names.values()]
calls={0x5596d0:0,0x40f8f0:3,0x5013a0:1,0x4c53d0:1,0x47bf50:7,0x47afd0:3}
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
            if last[0] in [0x46b19f,0x4bfbd5]:jump_edges.add((last[0],pc))
            last[0]=pc
        for rows in owned.values():u.hook_add(UC_HOOK_CODE,observe,state,begin=min(rows),end=max(rows))
    def entry(uc,pc,size,data):
        if not data['collect']:return
        t=data['entries'][pc];sp=uc.reg_read(UC_X86_REG_ESP);ret=get(uc,sp)
        args=tuple(get(uc,sp+4+i*4) for i in range(calls[t]))
        if t==0x47bf50:args=(args[0]&65535,*args[1:])
        immediate=ret in points if not data['compiled'] else any(lo<=ret<hi for lo,hi in compiled_ranges)
        if immediate:data['events'].append((t,args))
        if not immediate:return
        data['counts'][t]=data['counts'].get(t,0)+1;first=data['counts'][t]==1;mutation=data['mutation']
        if t==0x40f8f0 and args[1]==196:
            slot=(args[0]-0x7900b8)//196;values=data['overrides']
            if values is not None:
                resources,foods,resource_count,food_count=values
                u32=put
                for at,items,count in [(0xf53e6c,resources,resource_count),(0xf53dd8,foods,food_count)]:
                    u32(uc,at,len(items) if count is None else count)
                    for i,item in enumerate(items):
                        if item=='count':item=at-(0x7900c2+slot*196)
                        u32(uc,at+4+i*4,item)
            if mutation=='clear':word(uc,0x7999e8+BID*264,72);put(uc,0x78d788,0xffffffff)
        if t==0x5013a0 and first and mutation=='query':put(uc,0xf53e6c,1)
        if t==0x4c53d0 and first and mutation=='god':
            word(uc,0x7999de+BID*264,110);word(uc,0x7999e0+BID*264,100);byte(uc,0x799a82+BID*264,1);put(uc,0xbca2e0,4)
            for i,v in enumerate([0,0,2,0,0]):word(uc,0x7848e2+i*2,v)
        if t==0x47bf50 and first and mutation=='stamp':
            word(uc,0x7999de+BID*264,110);word(uc,0x7999e0+BID*264,100);byte(uc,0x799a82+BID*264,1);put(uc,0xbca2e0,6)
            put(uc,0x5df778,900);word(uc,0x7848e2,0);word(uc,0x7848e4,2);put(uc,0xf55bb4+486*8,0x7fffffff)
        if t==0x47bf50 and first and mutation=='layout':
            put(uc,0x5df78c+4,3);put(uc,0x5df768+4,430);put(uc,0x5df814+3*16,1);byte(uc,0x5df810+3*16,1)
    state['entries']={(sym(names[t]) if c and t==0x4c53d0 else sym('FUN_%08x'%t) if c else t):t for t in calls}
    for at in state['entries']:u.hook_add(UC_HOOK_CODE,entry,state,begin=at,end=at)

def base(u):
    u.mem_write(0x560000,seed);u.mem_write(S,bytes(0x50000));u.mem_write(0x7900b8,bytes(200*196));u.mem_write(0x7999d8,bytes(4000*264))
    u.mem_write(0x77de00,bytes(61*106));u.mem_write(0x785d74,bytes(258*124));put(u,0x785d70,0)
    for city in [0,1,3]:
        u.mem_write(0xea41fc+city*0x49d8,bytes(64))
        for i in range(3):word(u,0xea61f4+city*0x49d8+i*2,0)
    for at,v in [(0xeaac28,0),(0xbca2d8,1000),(0xbca2d0,200),(0xbca2d4,200),(0xbca2dc,28),(0xbca2e0,0),(0xe94e04,0),(0x78d788,0)]:put(u,at,v)
    for i in range(36):put(u,0x5cc8c0+i*8,(i//6)*228+i%6)
    for reg,v in [(UC_X86_REG_FPCW,0x37f),(UC_X86_REG_FPSW,0),(UC_X86_REG_FPTAG,0xffff)]:u.reg_write(reg,v)

def storage(u,kind=71,free=1,maximum=0,resources=(),foods=(),resource_count=None,food_count=None,override=False,root=BID):
    base(u);r=(root*264)&0xffffffff;word(u,(0x7999e8+r)&0xffffffff,kind)
    for i in range(1,free if free else 200):byte(u,0x7900c0+i*196,1)
    put(u,0x78d788,maximum)
    for item in resources:
        if isinstance(item,int) and 0<=item<36:byte(u,0xea4205+item,1)
    for item in foods:
        if isinstance(item,int) and 0<=item<9:byte(u,0xea41fc+item,1)
    return [root],(resources,foods,resource_count,food_count) if override else None

def display(u,orientation=0,rotation=0,gods=(0,0,0,0,0),root=BID,position=(90,80)):
    base(u);r=(root*264)&0xffffffff;word(u,(0x7999e8+r)&0xffffffff,202)
    word(u,(0x7999de+r)&0xffffffff,position[0]);word(u,(0x7999e0+r)&0xffffffff,position[1]);byte(u,(0x799a82+r)&0xffffffff,orientation)
    put(u,0xbca2e0,rotation);put(u,0xe94e04,rotation)
    for i,v in enumerate(gods):word(u,0x7848e2+i*2,v)
    for i in range(1000):put(u,0xf55bb4+i*8,100000+i*16)
    # Nonstandard camera/record values get an explicit mapped permutation
    # row; neither the native nor C algorithm is replaced or normalized.
    combined=(orientation+rotation)&0xffffffff
    signed=combined if combined<0x80000000 else combined-0x100000000
    if signed>=8:combined=(combined-8)&0xffffffff
    if combined>=8:
        for i in range(4):put(u,(0x5df78c+combined*16+i*4)&0xffffffff,i)
    return [root],None

def god(u,status,values):
    base(u)
    for i,v in enumerate(values):word(u,0x7848e2+i*2,v)
    return [status],None

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
    value=run(0,u,entry,argv,budget=15000000)
    if c and route in ['frame','raw']:
        assert get(u,FRAME+16)==VSP+4 and [get(u,FRAME+i*4) for i in [3,5,6,7]]==saved
        assert bytes(u.mem_read(VSP+4+len(args)*4,64))==b'\x6b'*64;value=get(u,FRAME)
    return value if token!=0x4bfb30 else None

def exercise(token,prepare,label,mutation=''):
    global checked
    expected=None
    for c,u,state in machines:
        routes=['named','frame','raw']+(['legacy'] if available('FUN_%08x'%token) else [])
        for route in routes if c else ['native']:
            state['collect']=False;args,overrides=prepare(u);state.update(writes=[],events=[],collect=True,counts={},mutation=mutation,overrides=overrides);last[0]=None
            try:value=invoke(c,u,token,args,route)
            except Exception as e:raise AssertionError(('execution',hex(token),label,mutation,route,hex(u.reg_read(UC_X86_REG_EIP)))) from e
            state['collect']=False;result=(tuple(state['writes']),tuple(state['events']),snapshot(u),value)
            if expected is None:expected=result
            elif result!=expected:
                differences=[next(((i,a,b) for i,(a,b) in enumerate(zip(x,y)) if a!=b),None) for x,y in zip(expected[:2],result[:2])]
                memory=next(((hex(0x560000+i),a,b) for i,(a,b) in enumerate(zip(expected[2],result[2])) if a!=b),None)
                raise AssertionError((hex(token),label,mutation,route,'stores/events',differences,'lengths',[(len(x),len(y)) for x,y in zip(expected[:2],result[:2])],'memory',memory,'return',expected[3],value))
    checked+=1

if __name__=='__main__':
    for kind,resources,foods in itertools.product([-32768,-1,0,69,70,71,72,73,189,190,191,32767],[(),(1,8,9,13,15,17,19,20,35)], [(),(1,2,8)]):
        exercise(0x46b100,lambda u,k=kind,r=resources,f=foods:storage(u,kind=k,resources=r,foods=f),('storage kind/resources',kind,resources,foods))
    for free,maximum in itertools.product([1,2,199,0],[0,1,199,0xffffffff]):exercise(0x46b100,lambda u,f=free,m=maximum:storage(u,free=f,maximum=m),('storage slot/max',free,maximum))
    for kind,count in itertools.product([70,71,72,190],[-1,0,1,3]):
        exercise(0x46b100,lambda u,k=kind,n=count:storage(u,kind=k,resources=(0,1,9),foods=(0,1,8),resource_count=n,food_count=n,override=True),('signed/cached counts',kind,count))
    for kind in [70,71,72,190]:
        exercise(0x46b100,lambda u,k=kind:storage(u,kind=k,resources=(-1,0,1,8,9,35,36),foods=(-1,0,1,8,35,36),override=True),('unclamped ids',kind))
    for kind in [71,72]:exercise(0x46b100,lambda u,k=kind:storage(u,kind=k,resources=('count',9),foods=('count',2),override=True),('count aliases',kind))
    exercise(0x46b100,lambda u:storage(u,kind=70,resources=(1,2,3),override=True),'market fresh count','query')
    exercise(0x46b100,lambda u:storage(u,kind=70,resources=(1,9,13),override=True),'clear changes kind/max','clear')
    exercise(0x46b100,lambda u:storage(u,root=BID+0x20000000),'full owner id')
    for orientation,rotation in itertools.product([0,1],[0,1,2,3,4,5,6,7,8]):exercise(0x4bfb30,lambda u,o=orientation,r=rotation:display(u,orientation=o,rotation=r),('gate orientation/view',orientation,rotation))
    for orientation,rotation in [(2,0),(255,0),(0,-1),(0,24)]:exercise(0x4bfb30,lambda u,o=orientation,r=rotation:display(u,orientation=o,rotation=r),('gate mapped fallback',orientation,rotation))
    for index,status in itertools.product(range(5),[1,2]):
        values=[0]*5;values[index]=status
        exercise(0x4bfb30,lambda u,v=values:display(u,gods=v),('god style',index,status))
    for values in [(1,0,2,0,0),(2,2,2,2,2)]:exercise(0x4bfb30,lambda u,v=values:display(u,gods=v),('first/preferred god',values))
    for root in [0,BID+0x20000000]:exercise(0x4bfb30,lambda u,r=root:display(u,root=r),('gate full owner',root))
    for mutation in ['god','stamp','layout']:exercise(0x4bfb30,display,('gate mutation',mutation),mutation)
    for status,values in itertools.product([-32768,-1,0,1,2,32767,65535,0x10002],[(0,0,0,0,0),(-32768,-1,1,2,32767),(1,1,1,1,1)]):exercise(0x4c53d0,lambda u,s=status,v=values:god(u,s,v),('god full argument',status,values))
    assert points<=hits,('unexecuted instructions',[hex(a) for a in sorted(points-hits)])
    missing={(a,b) for a,targets in branches.items() for b in targets if (a,b) not in edges}
    assert not missing,('unexecuted conditional edges',[(hex(a),hex(b)) for a,b in sorted(missing)])
    assert expected_jumps<=jump_edges
    assert not (ROOT/'runtime/building_controllers_model.c').exists()
    assert 'building_controllers_model' not in symbols
    changed=['direct_c/verify_'+name+'.py' for name in ['formation_control','house_evolution','house_layout','house_levels','house_split','housing_cycle','temple_complex']]
    sources=['CMakeLists.txt','include/storage_settings.h','runtime/storage_settings.c','include/gatehouse_display.h','runtime/gatehouse_display.c','runtime/building_controllers.c','direct_c/verify_building_policies.py','metadata/building-policies-native.json']+[f'src/unit_{i:05d}.c' for i in [967,1574,1609]]+changed
    proof=dict(native_sha256=catalogue['native_sha256'],compiled_C_path=str(target.resolve()),compiled_C_sha256=target_sha,passed=True,checked=checked,invocations=invocations,
               coverage={hex(a):dict(instructions=len(p),executed=len(p&hits)) for a,p in owned.items()},conditional_edges=len(edges),switch_targets=len(jump_edges),
               controlled_algorithm_endpoints=[],controlled_platform_endpoints=[],source_hashes={p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest() for p in sources},commit=None,
               retired_sources=['runtime/building_controllers_model.c'],
               limitations=['Complete analyzed bodies; full DWORD storage/god results and void gate display. Owned ordered stores, immediate child inputs and all final original data compared.',
                            'Actual availability, clear, resource predicate, map stamping and terrain refresh execute; no algorithm/platform returns are replaced.',
                            'Selected clear/query/stamp inputs test fresh counts, cached limits, signed ids, cache boundaries and mapped nonstandard view rows.',
                            'Mapped fixtures and resident data; no arbitrary invalid memory, concurrency, host game launch, visual or complete-gameplay claim.'])
    suffix='diagnostic' if target.stem=='restore_link_check' else 'game'
    (ROOT/f'metadata/building-policies-{suffix}.json').write_text(json.dumps(proof,indent=2)+'\n')
    print('PASS building policies:',checked,'scenarios',invocations,'invocations;',len(points),'instructions,',len(edges),'conditional edges,',len(jump_edges),'switch targets',flush=True)
    print('Verified target SHA256:',target_sha,flush=True)
