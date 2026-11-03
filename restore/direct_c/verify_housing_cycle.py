"""Complete native/C housing owner, map repair and goods use, offline only.

Observe ordered global and caller-argument writes, real child calls, saved
registers and stack balance. Actual algorithms execute without replacements.
"""
from verify_house_levels import prepare as base_prepare
from verify_house_evolution import *

catalogue=json.loads((ROOT/'metadata/housing-cycle-native.json').read_text())
assert catalogue['native_sha256']==hashlib.sha256(original.read_bytes()).hexdigest()
owned={};branches={};hits=set();edges=set();last=[None];switch_edges=set()
for f in catalogue['functions']:
    a=int(f['address'],16);raw=bytes.fromhex(f['bytes_hex'])
    assert raw==bytes(images[0].data[a-images[0].base:a-images[0].base+len(raw)])
    instructions=list(decoder.disasm(raw,a));owned[a]={int(p,16) for p in f['instruction_addresses'].split()}
    assert {i.address for i in instructions}==owned[a]
    for i in instructions:
        if i.mnemonic.startswith('j') and i.mnemonic!='jmp':branches[i.address]={int(i.op_str,16),i.address+i.size}
table=catalogue['consumption_switch'];a=int(table['address'],16);raw=bytes.fromhex(table['bytes_hex'])
assert raw==bytes(images[0].data[a-images[0].base:a-images[0].base+len(raw)])
expected_switch={(0x454c11,v) for v in struct.unpack('<4I',raw)}
points=set.union(*owned.values());checked=invocations=0
providers={0x4528b0:'restore_housing_update_all',0x454b40:'restore_house_consume_goods',0x46b740:'restore_house_repair_position'}
level_entries=[0x452980,0x452a20,0x452b00,0x452bd0,0x452ca0,0x452d70,0x452e40,0x452f10,0x452fe0,0x4530b0,0x4531c0,0x453280,0x453340,0x453400,0x453500,0x4535c0,0x453680,0x453740,0x453840,0x453900]
CTX=S+0x28000
machines=[]
for c,pe in enumerate(images):
    u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(pe.base,(len(pe.data)+4095)&~4095);u.mem_write(pe.base,bytes(pe.data))
    u.mem_map(0x30000000,0x10000);u.mem_map(STOP,4096);u.mem_map(S,0x50000);u.mem_write(0x560000,seed)
    state=dict(writes=[],calls=[],argument=None,mutation='',owner=False,consume_count=0);machines.append((c,u,state))
    def stores(uc,access,at,size,value,data):data['writes'].append((at,size,value&((1<<(8*size))-1)))
    u.hook_add(UC_HOOK_MEM_WRITE,stores,state,begin=0x560000,end=0x126cfff)
    def arguments(uc,access,at,size,value,data):
        if at==data['argument']:data['writes'].append(('argument',size,value&((1<<(8*size))-1)))
    u.hook_add(UC_HOOK_MEM_WRITE,arguments,state,begin=0x30000000,end=0x3204ffff)
    if not c:
        def observe(uc,pc,size,data):
            hits.add(pc)
            if last[0] in branches:edges.add((last[0],pc))
            if last[0]==0x454c11:switch_edges.add((last[0],pc))
            last[0]=pc
        for rows in owned.values():u.hook_add(UC_HOOK_CODE,observe,state,begin=min(rows),end=max(rows))
    def children(uc,pc,size,data):
        if not data['owner']:return
        kind,n=data['entries'][pc];sp=uc.reg_read(UC_X86_REG_ESP)
        args=tuple(get(uc,sp+4+i*4) for i in range(n))
        if kind=='level':args=(level_entries[args[0]],) if data['compiled'] else (pc,)
        data['calls'].append((kind,args,get(uc,0x78d838),get(uc,0x78d7e4)))
        mutation=data['mutation']
        first=kind not in data['mutated']
        data['mutated'].add(kind)
        if kind=='repair' and mutation=='repair' and first:
            put(uc,0x78d838,BID+1)
        if kind=='level' and mutation=='level' and first:
            put(uc,0x78d838,BID+1);put(uc,0x78d7c4,7)
        if kind=='consume':
            data['consume_count']+=1
            if mutation=='consume' and data['consume_count']==1:
                put(uc,0x78d838,BID+1);put(uc,0x78d7c4,7)
        if kind=='connect' and mutation=='connect':put(uc,0x78d588,0)
    native_children=[(0x454d70,'reset',0),(0x46b740,'repair',1),(0x454b40,'consume',1),(0x517370,'connect',0),(0x517660,'cost',0)]
    state['compiled']=c
    state['entries']={(sym(providers[t]) if t==0x46b740 else sym('FUN_%08x'%t)) if c else t:(name,n) for t,name,n in native_children}
    state['entries'].update({sym('restore_house_update_level'):('level',1)} if c else {t:('level',0) for t in level_entries})
    for at in state['entries']:u.hook_add(UC_HOOK_CODE,children,state,begin=at,end=at)

def invoke(c,u,state,token,route,building=BID,slots=(2,3,4)):
    global invocations
    invocations+=1;argv=[] if token==0x4528b0 else [building];argument=SP+4
    u.mem_write(S,bytes(0x50000));u.mem_write(SP-0x1000,b'\x6b'*0x1100)
    u.mem_write(SP-12,struct.pack('<3I',*slots))
    if not c:entry=token
    elif route=='named':
        entry=sym(providers[token])
        if token==0x454b40:
            # The context describes the caller of the native ABI shim, not
            # this backend's own compiler-owned argument/spill area.
            argument=CTX+0x80;put(u,argument,building)
            u.mem_write(CTX,struct.pack('<5I',saved[0],*slots,argument));argv=[building,CTX]
    elif route=='legacy':entry=sym('FUN_%08x'%token)
    else:
        argument=VSP+4;put(u,VSP,STOP)
        if argv:put(u,VSP+4,building)
        u.mem_write(VSP-12,struct.pack('<3I',*slots))
        u.mem_write(FRAME,struct.pack('<10I',0x89abcdef,0x87654321,0x98765432,saved[0],VSP,*saved[1:],0x202,0))
        if route=='frame':entry=sym('restore_building_control_C_%08x'%token);argv=[FRAME]
        else:entry=sym('restore_building_control_raw');argv=[token,FRAME,FRAME+32]
    state['argument']=argument if token==0x454b40 else None
    # run() writes call arguments through mem_write, outside emulated stores.
    run(0,u,entry,argv,budget=6000000)
    if c and route in ['frame','raw']:
        assert get(u,FRAME+16)==VSP+4 and [get(u,FRAME+i*4) for i in [3,5,6,7]]==saved
        assert bytes(u.mem_read(VSP+8,48))==bytes(48)
    return get(u,argument) if token==0x454b40 else None

def exercise(token,prepare,label):
    global checked
    expected=None
    for c,u,state in machines:
        routes=['named','frame','raw']+(['legacy'] if available('FUN_%08x'%token) else [])
        for route in routes if c else ['native']:
            values=prepare(u)
            state.update(writes=[],calls=[],argument=None,mutation=values.pop('mutation',''),owner=token==0x4528b0,consume_count=0,mutated=set());last[0]=None
            try:argument=invoke(c,u,state,token,route,**values)
            except Exception as e:raise AssertionError(('execution',hex(token),label,route,hex(u.reg_read(UC_X86_REG_EIP)))) from e
            result=(tuple(state['writes']),tuple(state['calls']),argument)
            if expected is None:expected=result
            else:
                differences=[(i,a,b) for i,(a,b) in enumerate(zip(expected[0],result[0])) if a!=b][:6]
                call_differences=[(i,a,b) for i,(a,b) in enumerate(zip(expected[1],result[1])) if a!=b][:6]
                assert result==expected,(hex(token),label,route,'writes',len(expected[0]),len(result[0]),differences,'calls',len(expected[1]),len(result[1]),call_differences,'argument',expected[2],argument)
    checked+=1

def goods(u,phase,size,values,stocks,level=5,slots=(2,3,4)):
    r=BID*264;u.mem_write(0x7999d8+r,bytes(264))
    word(u,0x7999ea+r,level);byte(u,0x7999db+r,size);put(u,0x78d7c4,phase)
    for i,v in enumerate(values):put(u,(0x70cdf8+level*88+i*4)&0xffffffff,v)
    for i,v in enumerate(stocks):word(u,0x799a3c+r+i*2,v)
    return dict(slots=slots)

def repair(u,style,width=4,height=3,padding=224,found=0,counter=0,building=BID):
    r=(building*264)&0xffffffff
    def at(offset):return (offset+r)&0xffffffff
    u.mem_write(at(0x7999d8),bytes(264));u.mem_write(0xb4e6fc,bytes(120000))
    byte(u,at(0x7999d8),1);byte(u,at(0x799a56),255)
    word(u,at(0x7999de),3);word(u,at(0x7999e0),2);put(u,at(0x7999e4),1459 if style=='fast' else 1300)
    for absolute,v in [(0xbca2d8,1000),(0xbca2d0,width),(0xbca2d4,height),(0xbca2dc,padding),(0x78d5c4,counter),(0x78d5c0,counter)]:put(u,absolute,v)
    if style=='fast':word(u,0xb4e6fc+1459*2,building)
    elif style=='owner':put(u,at(0x7999e4),1459)
    if style=='found':word(u,0xb4e6fc+found*2,building)
    return dict(building=building)

def owner(u,phase=1,populated=True,desire=0,mutation='',invalid=False,remap=False,grow=False):
    base_prepare(u,0,0,0,10,0,True,True,True,None,1)
    put(u,0xbca2dc,28)
    u.mem_write(0x7999d8,bytes(4000*264));u.mem_write(0x70cdcc,bytes(22*88))
    u.mem_write(0x5da3fc,seed[0x5da3fc-0x560000:0x5da3fc-0x560000+80])
    for level in range(20):
        put(u,0x70cdcc+level*88,-10);put(u,0x70cdd0+level*88,10)
        if populated:
            r=(BID+level)*264;cell=19330+level
            byte(u,0x7999d8+r,1);byte(u,0x7999db+r,1);byte(u,0x7999dc+r,1);byte(u,0x7999dd+r,1)
            word(u,0x7999e8+r,level+10);word(u,0x7999ea+r,level);word(u,0x7999f4+r,10)
            word(u,0x7999de+r,90+level);word(u,0x7999e0+r,80);put(u,0x7999e4+r,cell)
            word(u,0xb4e6fc+cell*2,BID+level);put(u,0xaf598c+cell*4,8);byte(u,0x799a96+r,desire)
            for i in range(4):put(u,0x70cdf8+level*88+i*4,4);word(u,0x799a3c+r+i*2,20)
    # Exercise all state/type eligibility edges without out-of-range dispatch.
    for i,kind in enumerate([-1,0,9,30,32767]):byte(u,0x7999d8+(10+i)*264,1);word(u,0x7999e8+(10+i)*264,kind)
    if invalid:
        word(u,0xb4e6fc+19330*2,0);put(u,0xbca2d0,0);put(u,0xbca2d4,0)
    if grow:
        # One expanding house takes the real layout path that sets changed,
        # then executes both complete road rebuilds after the owner loop.
        for level in range(20):
            if level!=9:byte(u,0x7999d8+(BID+level)*264,0)
        for at,scale in [(0xaf598c,4),(0xb4e6fc,2)]:u.mem_write(at+18000*scale,bytes(6000*scale))
        word(u,0xb4e6fc+19339*2,BID+9);put(u,0xaf598c+19339*4,8)
    if remap:put(u,0x5da3fc,level_entries[1])
    put(u,0x78d7c4,phase)
    return dict(mutation=mutation)

if __name__=='__main__':
    for phase,size,value,stock in itertools.product([0,1,2,3,7,-1,-2,-3,-4,0x7fffffff,0x80000000],[0,1,2,255],[-2147483648,-7,-6,-5,-4,-3,-2,-1,0,1,2,3,4,7,8,0x7fffffff],[-32768,0,1,32767]):
        exercise(0x454b40,lambda u,p=phase,s=size,v=value,t=stock:goods(u,p,s,[v]*4,[t]*4),('goods',phase,size,value,stock))
    for level in [-2,-1,0,19,255]:
        exercise(0x454b40,lambda u,l=level:goods(u,-1,1,[1,2,3,4],[4,3,2,1],l,slots=(0xffffffff,7,0x80000000)),('goods level',level))
    rng=random.Random(0x454b40)
    for i in range(128):
        values=[rng.choice([-2147483648,-7,-6,-5,-4,-3,-2,-1,0,1,2,3,4,7,8,32767,0x7fffffff]) for _ in range(4)]
        stocks=[rng.choice([-32768,-1,0,1,2,3,7,32767]) for _ in range(4)]
        phase=rng.choice([-4,-3,-2,-1,0,1,2,3,7]);size=rng.choice([0,1,2,4,255])
        slots=tuple(rng.choice([0,1,7,0xffffffff,0x80000000]) for _ in range(3))
        exercise(0x454b40,lambda u,p=phase,s=size,v=values,t=stocks,z=slots:goods(u,p,s,v,t,slots=z),('goods mixed',i,phase,size,values,stocks,slots))
    for style in ['fast','owner','missing']:
        for width,height in itertools.product([-1,0,1,4],[-1,0,1,3]):
            exercise(0x46b740,lambda u,s=style,w=width,h=height:repair(u,s,w,h,counter=0xffffffff),('repair',style,width,height))
    for padding in [0,1,224,300,-3,-300]:
        for row,column in itertools.product(range(3),range(4)):
            found=1000+row*(4+padding)+column
            exercise(0x46b740,lambda u,p=padding,f=found:repair(u,'found',padding=p,found=f),('repair found',padding,row,column))
    for building,style in itertools.product([0,0xffffffff,32767,32768,0x20000040],['fast','found']):
        exercise(0x46b740,lambda u,b=building,s=style:repair(u,s,found=1001,building=b),('repair full owner',building,style))
    for phase,populated,desire in itertools.product([0,1,7],[False,True],[-20,0,20]):
        exercise(0x4528b0,lambda u,p=phase,b=populated,d=desire:owner(u,p,b,d),('owner',phase,populated,desire))
    for mutation in ['repair','level','consume']:
        exercise(0x4528b0,lambda u,m=mutation:owner(u,0,True,20,m),('owner mutation',mutation))
    for mutation in ['', 'connect']:
        exercise(0x4528b0,lambda u,m=mutation:owner(u,0,True,20,m,grow=True),('owner real expansion',mutation))
    for invalid,remap in [(True,False),(False,True)]:
        exercise(0x4528b0,lambda u,i=invalid,r=remap:owner(u,7,True,0,invalid=i,remap=r),('owner binding',invalid,remap))
    assert points<=hits,('unexecuted instructions',[hex(a) for a in sorted(points-hits)])
    missing={(a,b) for a,targets in branches.items() for b in targets if (a,b) not in edges}
    assert not missing,('unexecuted conditional edges',[(hex(a),hex(b)) for a,b in sorted(missing)])
    assert expected_switch<=switch_edges
    sources=['CMakeLists.txt','include/housing_cycle.h','runtime/housing_cycle.c','include/house_levels.h','runtime/house_levels.c','runtime/building_controllers.c','runtime/building_controllers_model.c','direct_c/verify_housing_cycle.py','metadata/housing-cycle-native.json','src/unit_00798.c','src/unit_00804.c','src/unit_00970.c']
    proof=dict(native_sha256=catalogue['native_sha256'],compiled_C_path=str(target.resolve()),compiled_C_sha256=target_sha,passed=True,checked=checked,invocations=invocations,
               coverage={hex(a):dict(instructions=len(p),executed=len(p&hits)) for a,p in owned.items()},conditional_edges=len(edges),switch_edges=len(switch_edges),
               controlled_algorithm_endpoints=[],controlled_platform_endpoints=[],source_hashes={p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest() for p in sources},commit=None,
               limitations=['Complete analyzed native bodies; void ABI, ordered global and caller-argument writes, actual child arguments, saved registers and frame balance compared.',
                            'Actual level callbacks, requirements, layout, map repair and road providers execute; selected child-entry mutations change inputs without replacing returns.',
                            'Inherited native EBX and three stack words are explicit context, including signed negative phase/model remainder cases.',
                            'Mapped fixtures; no concurrency, arbitrary invalid memory, host game launch, visual or complete-gameplay claim.'])
    suffix='diagnostic' if target.stem=='restore_link_check' else 'game'
    (ROOT/f'metadata/housing-cycle-{suffix}.json').write_text(json.dumps(proof,indent=2)+'\n')
    print('PASS housing cycle:',checked,'scenarios',invocations,'invocations;',len(points),'instructions,',len(edges),'conditional edges,',len(switch_edges),'switch targets',flush=True)
    print('Verified target SHA256:',target_sha,flush=True)
