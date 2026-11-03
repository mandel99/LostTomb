"""Whole-PE native/C resource supply and list rebuild, actual typed helpers.

Full legacy EAX, owned ordered writes, immediate child inputs, all final game
data, saved registers and stack balance. --quick runs a small smoke only.
"""
from verify_options_startup import *
from unicorn import UC_HOOK_MEM_WRITE
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import json

FRAME,VSP=S+0x8000,S+0x18000
catalogue=json.loads((ROOT/'metadata/resource-supply-native.json').read_text())
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
expected_jumps={(0x55950b,v) for v in struct.unpack('<9I',bytes.fromhex(catalogue['tables']['jump_targets']['bytes_hex']))}
points=set.union(*owned.values());names={0x559480:'restore_resource_event_supply',0x5596d0:'restore_resource_lists_rebuild'}
compositions=0
available=cache(lambda n:bool(re.search(r'\s_'+re.escape(n)+r'\s',symbols)))
publics=sorted({int(a,16) for a in re.findall(r'^\s+[0-9a-fA-F]{4}:[0-9a-fA-F]+\s+\S+\s+([0-9a-fA-F]{8})\s',symbols,re.M)})
compiled_ranges={t:(sym(n),next(a for a in publics if a>sym(n))) for t,n in names.items()}
calls={0x443b90:1,0x443d90:1,0x4439c0:1,0x559480:1}
machines=[]
for c,pe in enumerate(images):
    u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(pe.base,(len(pe.data)+4095)&~4095);u.mem_write(pe.base,bytes(pe.data))
    u.mem_map(0x30000000,0x10000);u.mem_map(STOP,4096);u.mem_map(S,0x50000)
    state=dict(writes=[],events=[],collect=False,compiled=c);machines.append((c,u,state))
    def stores(uc,access,at,size,value,data):
        if not data['collect']:return
        pc=uc.reg_read(UC_X86_REG_EIP)
        own=pc in points if not data['compiled'] else any(lo<=pc<hi for lo,hi in compiled_ranges.values())
        if own:data['writes'].append((at,size,value&((1<<(8*size))-1)))
        if not own:return
        mutation=data['mutation']
        if mutation=='zero_items' and size==4 and value==1:
            if at==0xf53e6c:put(uc,0xf53e70,0)
            if at==0xf53dd8:put(uc,0xf53ddc,0)
        if mutation=='publish' and size==1 and at==0xea4205+3*0x49d8+1 and not data['published']:
            data['published']=True;put(uc,0xeaac28,0);put(uc,0xf53dd8,2);put(uc,0xf53ddc,2);put(uc,0xf53de0,3)
    u.hook_add(UC_HOOK_MEM_WRITE,stores,state,begin=0x560000,end=0x126cfff)
    if not c:
        def observe(uc,pc,size,data):
            if not data['collect']:return
            hits.add(pc)
            if last[0] in branches:edges.add((last[0],pc))
            if last[0]==0x55950b:jump_edges.add((last[0],pc))
            last[0]=pc
        for rows in owned.values():u.hook_add(UC_HOOK_CODE,observe,state,begin=min(rows),end=max(rows))
    def entry(uc,pc,size,data):
        if not data['collect']:return
        t=data['entries'][pc];sp=uc.reg_read(UC_X86_REG_ESP);ret=get(uc,sp);args=(get(uc,sp+4),)
        immediate=ret in points if not data['compiled'] else any(lo<=ret<hi for lo,hi in compiled_ranges.values())
        if not immediate:return
        data['events'].append((t,args));mutation=data['mutation']
        key=(t,args[0]);data['counts'][key]=data['counts'].get(key,0)+1
        if mutation=='compound' and t==0x4439c0 and args==(11,):
            # Change genuine trade/event inputs after the earlier dependency
            # checks; execute the actual import query and recursive supply.
            byte(uc,0x77de00,1);byte(uc,0x77de02,1);byte(uc,0x77de2a+11,1)
            put(uc,0x785d70,1);byte(uc,0x785d76,23);word(uc,0x785d7a,9)
        if mutation=='player' and t==0x443b90 and args==(1,) and data['counts'][key]==1:
            put(uc,0xeaac28,3);uc.mem_write(0xea41fc,bytes(64))
    state['entries']={(sym(names[t]) if c and t==0x559480 else sym('FUN_%08x'%t) if c else t):t for t in calls}
    for at in state['entries']:u.hook_add(UC_HOOK_CODE,entry,state,begin=at,end=at)

def fixture(u,token,resource=1,rows=(),event_count=0,events=(),gate=0,flags=(),foodflags=(),flagvalue=1,player=0,fee=40,metrics=(0,0,0)):
    u.mem_write(0x560000,seed);u.mem_write(S,bytes(0x50000))
    u.mem_write(0x77de00,bytes(61*106));u.mem_write(0x785d74,bytes(258*124));put(u,0x785d70,event_count)
    for i in range(event_count):
        word(u,0x785d7a+i*124,-32768);word(u,0x785d7c+i*124,-32768);word(u,0x785d7e+i*124,-32768)
    for index,field,value in events:byte(u,0x785d76+index*124,23);word(u,0x785d7a+index*124+field*2,value)
    for i in range(4*36):put(u,0x77646c+i*4,fee)
    for index,kind,items in rows:
        a=0x77de00+index*106;byte(u,a,1);byte(u,a+2,kind);byte(u,a+4,0);byte(u,a+5,1)
        for item in items:byte(u,a+42+item,1)
    for city in [0,1,3,9]:
        u.mem_write(0xea41fc+city*0x49d8,bytes(64))
        for i,value in enumerate(metrics):word(u,0xea61f4+city*0x49d8+i*2,value)
        for item in flags:byte(u,0xea4205+city*0x49d8+item,flagvalue)
        for item in foodflags:byte(u,0xea41fc+city*0x49d8+item,flagvalue)
    word(u,0x784d32,gate);word(u,0x784d34,gate);put(u,0xeaac28,player)
    return [resource] if token==0x559480 else []

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
        if route=='frame':entry=sym('restore_avail_C_%08x'%token);argv=[FRAME]
        else:entry=sym('restore_resource_availability_dispatch');argv=[0,token,FRAME]
    value=run(0,u,entry,argv,budget=20000000)
    if c and route in ['frame','dispatch']:
        assert get(u,FRAME+16)==VSP+4 and [get(u,FRAME+i*4) for i in [3,5,6,7]]==saved
        assert bytes(u.mem_read(VSP+4+len(args)*4,64))==b'\x6b'*64;value=get(u,FRAME)
    return value if token==0x559480 else None

def exercise(token=0x559480,mutation='',**values):
    global checked
    expected=None
    for c,u,state in machines:
        routes=['named']+(['frame'] if available('restore_avail_C_%08x'%token) else [])+(['dispatch'] if available('restore_resource_availability_dispatch') else [])+(['legacy'] if available('FUN_%08x'%token) else [])
        for route in routes if c else ['native']:
            state['collect']=False;args=fixture(u,token,**values);state.update(writes=[],events=[],collect=True,counts={},mutation=mutation,published=False);last[0]=None
            try:value=invoke(c,u,token,args,route)
            except Exception as e:raise AssertionError(('execution',hex(token),values,mutation,route,hex(u.reg_read(UC_X86_REG_EIP)))) from e
            state['collect']=False;result=(tuple(state['writes']),tuple(state['events']),snapshot(u),value)
            if expected is None:expected=result
            elif result!=expected:
                differences=[next(((i,a,b) for i,(a,b) in enumerate(zip(x,y)) if a!=b),None) for x,y in zip(expected[:2],result[:2])]
                memory=next(((hex(0x560000+i),a,b) for i,(a,b) in enumerate(zip(expected[2],result[2])) if a!=b),None)
                raise AssertionError((hex(token),values,mutation,route,'stores/events',differences,'lengths',[(len(x),len(y)) for x,y in zip(expected[:2],result[:2])],'memory',memory,'EAX',expected[3],value))
    checked+=1

def storage_composition(kind,full=False):
    global compositions,invocations
    expected=None
    for c,u,state in machines:
        for route in ['named','legacy'] if c else ['native']:
            state['collect']=False
            fixture(u,0x5596d0,flags=(1,8,9,12,35),foodflags=(1,8))
            u.mem_write(0x7900b8,bytes(200*196));u.mem_write(0x7999d8,bytes(4000*264));put(u,0x78d788,0)
            word(u,0x7999e8+64*264,kind)
            if full:
                for i in range(1,200):byte(u,0x7900c0+i*196,1)
            state.update(writes=[],events=[],collect=True,counts={},mutation='',published=False);last[0]=None
            entry=0x46b100 if not c else sym('restore_storage_settings_allocate' if route=='named' else 'FUN_0046b100')
            value=run(0,u,entry,[64],budget=20000000);invocations+=1;state['collect']=False
            result=(value,tuple(state['writes']),tuple(state['events']),snapshot(u))
            if expected is None:expected=result
            else:assert result==expected,('actual storage composition',kind,full,route)
    compositions+=1

if __name__=='__main__':
    if '--quick' in sys.argv:
        exercise(resource=-1);exercise(resource=12);exercise(event_count=258,events=((257,0,1),));exercise(0x5596d0)
        print('PASS resource availability smoke:',checked,'scenarios; no full-coverage proof written',flush=True);sys.exit(0)
    for resource in list(range(40))+[-2147483648,-32768,-1,65535,65536,2147483647]:exercise(resource=resource)
    for count,index,field,value in itertools.product([1,258],[0],range(3),[-32768,-1,1,35,65535]):
        exercise(resource=value,event_count=count,events=((count-1-index,field,value),))
    for dependency in [11,14,16,18,22,20,29,32]:
        for resource in [10,13,15,17,19,23,28,33]:
            if {10:29,13:11,15:14,17:16,19:18,23:22,28:20,33:32}[resource]==dependency:
                for gate in [0,1,256]:exercise(resource=resource,gate=gate,event_count=1,events=((0,0,dependency),))
    for resource,kind in itertools.product([12,34],[0,1,3,5,2]):
        for items in [(1,),(9,),(11,),(13,),(31,),(1,11),(9,11),(11,31),(13,31)]:exercise(resource=resource,rows=((0,kind,items),))
    for rows,events in [(((0,0,(11,)),),((0,0,9),)),(((0,0,(1,)),),((0,0,11),)),(((0,1,(9,)),),((0,0,11),)),((),((0,0,11),(1,0,9))),(((0,1,(31,)),),((0,0,11),)),(((0,1,(31,)),),((0,0,13),))]:
        for resource in [12,34]:exercise(resource=resource,rows=rows,event_count=max(e[0] for e in events)+1,events=events)
    exercise(resource=12,mutation='compound')
    for fee,metrics in itertools.product([0,1,15,25,40,-1],[(0,0,0),(1,0,0),(0,1,0),(-1,0,0)]):exercise(resource=12,rows=((60,1,tuple(range(1,36))),),fee=fee,metrics=metrics)
    for player,scheme in itertools.product([0,1,3,9],range(6)):
        values=[{},dict(flags=(1,12,35)),dict(foodflags=(1,8)),dict(rows=((60,0,tuple(range(1,36))),),gate=256),dict(rows=((0,1,tuple(range(1,36))),)),dict(event_count=2,events=((0,0,1),(1,0,9)))][scheme]
        exercise(0x5596d0,player=player,**values)
    exercise(0x5596d0,flags=(1,2,35),foodflags=(1,8),flagvalue=128)
    exercise(0x5596d0,flags=(1,2),foodflags=(1,8),mutation='zero_items')
    exercise(0x5596d0,flags=(1,12,35),foodflags=(1,8),mutation='player')
    exercise(0x5596d0,flags=(1,12,35),foodflags=(1,8),player=3,mutation='publish')
    for kind in [70,71,72,190]:storage_composition(kind)
    storage_composition(71,full=True)
    excluded={0x55975c,0x55975d,0x559760,0x559762}
    assert points-hits==excluded,('unexpected instruction coverage',[hex(a) for a in sorted(points-hits)])
    excluded_edges={(0x5594a5,0x5594db),(0x5594a9,0x5594db),(0x5594b0,0x5594db),(0x55975a,0x55975c),(0x559760,0x559762),(0x559760,0x5597b6)}
    missing={(a,b) for a,targets in branches.items() for b in targets if (a,b) not in edges}
    assert missing==excluded_edges,('unexpected edge coverage',[(hex(a),hex(b)) for a,b in sorted(missing)])
    assert expected_jumps<=jump_edges
    sources=['CMakeLists.txt','include/resource_availability.h','runtime/resource_availability.c','src/unit_02629.c','src/unit_02630.c','direct_c/verify_resource_availability.py','metadata/resource-supply-native.json','include/training_queries.h','runtime/storage_settings.c','metadata/building-policies-native.json']+[f'src/unit_{i:05d}.c' for i in [625,626,631,632,669,1603,1605]]
    proof=dict(native_sha256=catalogue['native_sha256'],compiled_C_path=str(target.resolve()),compiled_C_sha256=target_sha,passed=True,checked=checked,storage_compositions=compositions,invocations=invocations,
               coverage={hex(a):dict(instructions=len(p),executed=len(p&hits)) for a,p in owned.items()},conditional_edges=len(edges),switch_targets=len(jump_edges),
               invariant_instruction_exclusions=[hex(a) for a in sorted(excluded)],invariant_edge_exclusions=[[hex(a),hex(b)] for a,b in sorted(excluded_edges)],
               controlled_algorithm_endpoints=[],controlled_platform_endpoints=[],source_hashes={p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest() for p in sources},commit=None,
               limitations=['Complete analyzed scopes; full legacy EAX, owned ordered writes, real helper inputs and all final original data compared.',
                            'Actual trade, fee/training and recursive event/dependency providers execute. Input mutations cover temporal compound queries, zero list items and cached publication player.',
                            'Four native food-index recovery instructions are unreachable; its private index is always1..8. Three backward event guards have fixed outcomes for mapped cached counts0..258. Six invariant outcomes are analytically excluded, not forced.',
                            'Mapped disjoint event/trade data and bounded counts0..258; no arbitrary pointer faults, concurrency, host game launch or complete-gameplay claim.'])
    suffix='diagnostic' if target.stem=='restore_link_check' else 'game'
    (ROOT/f'metadata/resource-supply-{suffix}.json').write_text(json.dumps(proof,indent=2)+'\n')
    print('PASS resource supply:',checked,'scenarios +',compositions,'storage compositions;',invocations,'invocations;',len(points-excluded),'/',len(points),'instructions,',len(edges),'conditional edges,',len(jump_edges),'switch targets;6 invariant edge exclusions',flush=True)
    print('Verified target SHA256:',target_sha,flush=True)
