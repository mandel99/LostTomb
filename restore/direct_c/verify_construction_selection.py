"""Whole-PE native/C construction-site and sub-building selection checks.

Run actual footprint/road/empty-slot children. Compare full EAX, their scalar
inputs and slot results, ordered game/output stores, all original data and ABI.
Native stack-corrupting >16-candidate inputs have separate prefix/C checks.
"""
from verify_options_startup import *
from unicorn import UC_HOOK_MEM_WRITE
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import json,random

catalogue=json.loads((ROOT/'metadata/construction-selection-native.json').read_text())
assert catalogue['native_sha256']==hashlib.sha256(original.read_bytes()).hexdigest()
names={0x43f410:'restore_construction_site_valid',0x441130:'restore_construction_select_slot'}
owned={};branches={};hits=set();edges=set();last=[None];checked=invocations=compositions=hardening=0
decoder=Cs(CS_ARCH_X86,CS_MODE_32)
for f in catalogue['functions']:
    a=int(f['address'],16);raw=bytes.fromhex(f['bytes_hex'])
    assert raw==bytes(images[0].data[a-images[0].base:a-images[0].base+len(raw)])
    instructions=list(decoder.disasm(raw,a));owned[a]={int(p,16) for p in f['instruction_addresses'].split()}
    assert {i.address for i in instructions}==owned[a]
    for i in instructions:
        if i.mnemonic.startswith('j') and i.mnemonic!='jmp':branches[i.address]={int(i.op_str,16),i.address+i.size}
for table in catalogue['tables']:
    a=int(table['address'],16);raw=bytes.fromhex(table['bytes_hex'])
    assert table['complete'] and raw==bytes(images[0].data[a-images[0].base:a-images[0].base+len(raw)])
points=set.union(*owned.values());OUT=S+0x2000;machines=[]
publics=sorted({int(a,16) for a in re.findall(r'^\s+[0-9a-fA-F]{4}:[0-9a-fA-F]+\s+\S+\s+([0-9a-fA-F]{8})\s',symbols,re.M)})
ranges=[(sym(n),next(a for a in publics if a>sym(n))) for n in names.values()]
for c,pe in enumerate(images):
    u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(pe.base,(len(pe.data)+4095)&~4095);u.mem_write(pe.base,bytes(pe.data))
    u.mem_map(0x30000000,0x10000);u.mem_map(STOP,4096);u.mem_map(S,0x50000)
    state=dict(collect=False,writes=[],events=[],returns=set(),pending={},compiled=c);machines.append((c,u,state))
    def write(uc,access,at,size,value,data):
        if data['collect']:data['writes'].append((at,size,value&((1<<(size*8))-1)))
    u.hook_add(UC_HOOK_MEM_WRITE,write,state,begin=0x560000,end=0x126cfff)
    u.hook_add(UC_HOOK_MEM_WRITE,write,state,begin=OUT,end=OUT+63)
    if not c:
        def code(uc,pc,size,data):
            if not data['collect']:return
            hits.add(pc)
            if last[0] in branches:edges.add((last[0],pc))
            last[0]=pc
        for p in owned.values():u.hook_add(UC_HOOK_CODE,code,state,begin=min(p),end=max(p))
    def result(uc,pc,size,data):
        if not data['collect'] or pc not in data['pending']:return
        count,locations=data['pending'].pop(pc);n=get(uc,count)
        assert n<=25,n
        coordinates=tuple(struct.unpack('<%dI'%(n*2),bytes(uc.mem_read(locations,n*8)))) if n else ()
        data['events'].append(('slots-result',n,coordinates))
        if data['prefix']:data['prefix_result']=(n,coordinates,get(uc,SP));uc.emu_stop()
    def child(uc,pc,size,data):
        if not data['collect']:return
        t=data['entries'][pc];sp=uc.reg_read(UC_X86_REG_ESP);ret=get(uc,sp)
        immediate=ret in points if not data['compiled'] else any(lo<=ret<hi for lo,hi in ranges)
        if not immediate:return
        count=5 if t==0x43f250 else 3;args=tuple(get(uc,sp+4+i*4) for i in range(count))
        data['events'].append((t,args))
        if data['mutation']=='origin' and not data['mutated'] and t==0x43f3a0:
            data['mutated']=True;put(uc,0xbca2d8,2000);put(uc,0xbca304,123);put(uc,0xbca308,456)
        if t==0x441040:
            data['pending'][ret]=(get(uc,sp+16),get(uc,sp+20))
            if ret not in data['returns']:uc.hook_add(UC_HOOK_CODE,result,data,begin=ret,end=ret);data['returns'].add(ret)
    state['entries']={(sym('FUN_%08x'%t) if c else t):t for t in [0x43f250,0x43f3a0,0x441040]}
    for a in state['entries']:u.hook_add(UC_HOOK_CODE,child,state,begin=a,end=a)

def fixture(u,fn,v):
    u.mem_write(0x560000,seed);u.mem_write(S,bytes(0x50000));u.mem_write(OUT,b'\x6b'*64)
    put(u,SP-128,v.get('inherited_x',0x76543210));put(u,SP-124,v.get('inherited_y',0xfedcba98))
    u.mem_write(0xaf598c,bytes(228*228*4));u.mem_write(0xd05720,bytes(228*228*4));u.mem_write(0xb4e6fc,bytes(228*228*2))
    if fn==0x43f410:
        kind=v.get('kind',33);n={31:2,30:3,33:4}.get(kind,5);x,y=v.get('position',(90,80))
        put(u,0xbca304,x);put(u,0xbca308,y);put(u,0xbca2d8,1000-y*228-x+80*228+90)
        # Keep two disjoint origin mappings for the genuine-input mutation.
        for base in [1000,2000]:
            for dy in range(-5,n+5):
                for dx in range(-5,n+5):
                    tile=base+(80+dy)*228+90+dx;road=v.get('roads','all')=='all' or (v.get('roads')=='cross' and (dx==1 or dy==1))
                    put(u,0xaf598c+tile*4,(0x40 if road else 0)|v.get('terrain_extra',0))
            cell=base+80*228+90+v.get('cell',0)
            if 'terrain' in v:put(u,0xaf598c+cell*4,v['terrain'])
            if 'object' in v:put(u,0xd05720+cell*4,S+0x8000);byte(u,S+0x8018,v['object'])
            word(u,0xb4e6fc+cell*2,v.get('occupancy',0))
        return [kind]
    primary,secondary=v.get('kinds',(33,31));n={31:2,30:3,33:4,209:5}.get(primary,0)
    rng=random.Random(v.get('pattern',0));free=v.get('free')
    for x in range(5):
        for y in range(5):
            value=(0 if (x,y) in free else 1) if free is not None else rng.choice([0,0,1,0x100,0x80000000])
            put(u,0x775614+(x*5+y)*4,value)
    for at,val in v.get('ranks',()):put(u,at,val)
    return list(v.get('position',(90,80)))+[v.get('out_x',OUT),v.get('out_y',OUT+4),primary,secondary]

def invoke(c,u,fn,args,route,v):
    global invocations
    invocations+=1;entry=fn if not c else sym(names[fn] if route=='named' else 'FUN_%08x'%fn)
    if c and fn==0x441130 and route=='named':args=args+[v.get('inherited_x',0x76543210),v.get('inherited_y',0xfedcba98)]
    return run(0,u,entry,args,budget=2000000)

def exercise(fn,**v):
    global checked
    expected=None
    for c,u,state in machines:
        for route in ['named','legacy'] if c else ['native']:
            state['collect']=False;args=fixture(u,fn,v);state.update(writes=[],events=[],pending={},collect=True,mutation=v.get('mutation',''),mutated=False,prefix=False);last[0]=None
            try:value=invoke(c,u,fn,args,route,v)
            except Exception as e:raise AssertionError(('execute',hex(fn),v,route,hex(u.reg_read(UC_X86_REG_EIP)))) from e
            state['collect']=False;answer=(value,tuple(state['writes']),tuple(state['events']),snapshot(u),bytes(u.mem_read(OUT,64)))
            if expected is None:expected=answer
            elif answer!=expected:
                changes=[next(((i,a,b) for i,(a,b) in enumerate(zip(x,y)) if a!=b),None) for x,y in zip(expected[1:3],answer[1:3])]
                memory=next(((hex(0x560000+i),a,b) for i,(a,b) in enumerate(zip(expected[3],answer[3])) if a!=b),None)
                raise AssertionError((hex(fn),v,route,'EAX',expected[0],value,'stores/events',changes,'lengths',[(len(x),len(y)) for x,y in zip(expected[1:3],answer[1:3])],'memory',memory,'outputs',expected[4]==answer[4]))
    checked+=1

def composition(kind):
    global compositions
    expected=None
    for c,u,state in machines:
        state['collect']=False;fixture(u,0x43f410,dict(kind=kind,roads='cross'));state.update(writes=[],events=[],pending={},collect=True,mutation='',mutated=False,prefix=False);last[0]=None
        value=invoke(c,u,0x43f410,[kind],'legacy',{})
        run(0,u,sym('FUN_004413b0') if c else 0x4413b0,[90,80,kind])
        placed=[]
        for secondary in ([33,30,31] if kind==33 else [30,31] if kind==30 else [31]):
            placed.append(invoke(c,u,0x441130,[90,80,OUT,OUT+4,kind,secondary],'legacy',{}))
        state['collect']=False;answer=(value,tuple(placed),tuple(state['writes']),tuple(state['events']),snapshot(u),bytes(u.mem_read(OUT,64)))
        if expected is None:expected=answer
        else:assert answer==expected,('real construction composition',kind)
    compositions+=1

def expanded_capacity(secondary):
    global hardening
    v=dict(kinds=(209,secondary),free=set(itertools.product(range(5),repeat=2)))
    c,u,state=machines[0];state['collect']=False;args=fixture(u,0x441130,v);state.update(writes=[],events=[],pending={},collect=True,mutation='',mutated=False,prefix=True);last[0]=None
    for r,val in zip(registers,[0x11111111,0x22222222,0x33333333,saved[0],SP,*saved[1:]]):u.reg_write(r,val)
    u.reg_write(UC_X86_REG_EFLAGS,2);put(u,SP,STOP)
    for i,val in enumerate(args):put(u,SP+4+i*4,val)
    u.emu_start(0x441130,STOP,count=1000000);state['collect']=False
    count,coords,return_word=state['prefix_result'];assert count==(25 if secondary==31 else 20) and return_word!=STOP
    baseline=snapshot(u)
    for route in ['named','legacy']:
        c,u,state=machines[1];args=fixture(u,0x441130,v);state.update(writes=[],events=[],pending={},collect=True,mutation='',mutated=False,prefix=False)
        value=invoke(c,u,0x441130,args,route,v);state['collect']=False
        assert value==0 and (get(u,OUT),get(u,OUT+4))==(90,80)
        assert state['events'][-1]==('slots-result',count,coords)
        expected=bytearray(baseline)
        for at in ([0x775614,0x775618] if secondary==30 else [0x775614]):struct.pack_into('<I',expected,at-0x560000,1)
        assert snapshot(u)==expected
    hardening+=1

if __name__=='__main__':
    quick='--quick' in sys.argv
    for kind in [31,30,33,209,0,-1]:
        for roads in ['all','cross','none']:exercise(0x43f410,kind=kind,roads=roads)
    for cell,field,val in [(0,'terrain',1),(1,'terrain',0x41),(0,'terrain',0x11002880),(0,'object',0),(0,'object',1),(0,'object',255),(0,'occupancy',1),(0,'occupancy',32767),(0,'occupancy',32768),(0,'occupancy',65535)]:exercise(0x43f410,cell=cell,**{field:val})
    for cell in [0,1,228,229,3*228+3]:exercise(0x43f410,cell=cell,terrain=1)
    for position in [(2147483647,80),(90,2147483647)]:exercise(0x43f410,position=position)
    exercise(0x43f410,mutation='origin',roads='cross',occupancy=1)
    for primary,secondary in itertools.product([31,30,33,209,0],[31,30,33,0]):
        for pattern in range(1 if quick else 6):exercise(0x441130,kinds=(primary,secondary),pattern=pattern)
    for primary in [31,30,33]:
        n={31:2,30:3,33:4}[primary]
        exercise(0x441130,kinds=(primary,31),free=set(itertools.product(range(n),repeat=2)))
        exercise(0x441130,kinds=(primary,30),free=set())
        for x,y in itertools.product(range(n),repeat=2):exercise(0x441130,kinds=(primary,31),free={(x,y)})
    exercise(0x441130,kinds=(31,30),free={(0,0),(1,0)})
    for ranks in [((0x5d2960,5),),((0x5d2960,0xffffffff),),tuple((0x5d2960+i*4,5) for i in range(9))]:exercise(0x441130,kinds=(30,31),free={(0,0),(1,1),(2,2)},ranks=ranks)
    for ox,oy in [(OUT,OUT),(0x775614,OUT),(OUT,0x775614),(0x5d2960,OUT)]:exercise(0x441130,kinds=(30,31),free={(0,0),(2,2)},out_x=ox,out_y=oy)
    exercise(0x441130,kinds=(0,0),free=set(),position=(-2147483648,2147483647))
    for kind in [31,30,33]:composition(kind)
    for secondary in [31,30]:expanded_capacity(secondary)
    missing=points-hits;missing_edges={(a,b) for a,ends in branches.items() for b in ends if (a,b) not in edges}
    # EBP is the cached slot count. Both incoming edges to441264 already
    # require EBP>0 and no instruction changes it before the redundant test.
    assert not missing,('instruction coverage',[hex(a) for a in sorted(missing)])
    assert missing_edges=={(0x441268,0x4412b9)},('edge coverage',missing_edges)
    print('PASS construction selection:',checked,'scenarios +',compositions,'compositions +',hardening,'capacity cases;',invocations,'invocations;',len(points)-len(missing),'/',len(points),'instructions,',len(edges),'conditional edges',flush=True)
    print('Unexecuted instructions:',[hex(a) for a in sorted(missing)],'; edge outcomes:',[(hex(a),hex(b)) for a,b in sorted(missing_edges)],flush=True)
    if quick:sys.exit(0)
    sources=['CMakeLists.txt','include/road_check_model.h','include/road_scan_model.h','runtime/menu_drag_construction_parent.c','runtime/menu_drag_construction_scan.c','direct_c/verify_construction_selection.py','metadata/construction-selection-native.json','metadata/construction-scan-native.json']
    proof=dict(native_sha256=catalogue['native_sha256'],compiled_C_path=str(target.resolve()),compiled_C_sha256=target_sha,passed=True,checked=checked,compositions=compositions,capacity_cases=hardening,invocations=invocations,
        coverage={hex(a):dict(instructions=len(p),executed=len(p&hits)) for a,p in owned.items()},conditional_edges=len(edges),unexecuted_instructions=[hex(a) for a in sorted(missing)],unexecuted_edges=[[hex(a),hex(b)] for a,b in sorted(missing_edges)],
        source_hashes={p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest() for p in sources},commit=None,controlled_algorithm_endpoints=[],controlled_platform_endpoints=[],
        limitations=['Real children, full EAX, scalar child inputs/slot results, owned game/output writes, all original data and explicit outputs, preserved stack/saved registers.',
            'Normal native comparisons have at most16 candidates. Separate20/25-candidate prefixes prove native return-address corruption; C uses50 coordinate words and preserves ABI.',
            '441268 nonpositive-count edge is unreachable: both predecessors require the unchanged cached EBP>0. It is an analytical exclusion, not a forced helper outcome.',
            'Unknown-secondary failure inherits entrySP-128/-124 via a small x86 ABI capture; algorithm is C. Native private stack scratch and arbitrary private-stack pointer aliases are excluded. No host game launch or whole-gameplay claim.'])
    suffix='diagnostic' if target.stem=='restore_link_check' else 'game'
    (ROOT/f'metadata/construction-selection-{suffix}.json').write_text(json.dumps(proof,indent=2)+'\n')
