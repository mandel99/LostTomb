"""Original/C house requirements, desirability and global-mode guard, offline.

Compare full EAX, ordered global writes and real image-change/stamping calls.
The native algorithms and their children execute without substituted endpoints.
"""
from verify_options_startup import *
from unicorn import UC_HOOK_MEM_WRITE
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import json,random

BID,FRAME,VSP=64,S+0x8000,S+0x18000
names={0x4539a0:'restore_house_check_global_mode',0x453af0:'restore_house_compare_desirability',0x453b40:'restore_house_check_requirements'}
available=cache(lambda n:bool(re.search(r'\s_'+re.escape(n)+r'\s',symbols)))
catalogue=json.loads((ROOT/'metadata/house-evolution-native.json').read_text())
assert catalogue['native_sha256']==hashlib.sha256(original.read_bytes()).hexdigest()
owned,branches,jumps={}, {}, {}
hits,edges,jump_edges,last=set(),set(),set(),[None]
decoder=Cs(CS_ARCH_X86,CS_MODE_32)
for f in catalogue['functions']:
    a=int(f['address'],16);raw=bytes.fromhex(f['bytes_hex'])
    assert raw==bytes(images[0].data[a-images[0].base:a-images[0].base+len(raw)])
    owned[a]={int(x,16) for x in f['instruction_addresses'].split()}
    insns=list(decoder.disasm(raw,a));assert {i.address for i in insns}==owned[a]
    for i in insns:
        if i.mnemonic.startswith('j') and i.mnemonic!='jmp':branches[i.address]={int(i.op_str,16),i.address+i.size}
        if i.mnemonic=='jmp' and 'ptr [' in i.op_str:jumps[i.address]=set()
t=catalogue['jump_table'];a=int(t['address'],16);raw=bytes.fromhex(t['bytes_hex'])
assert raw==bytes(images[0].data[a-images[0].base:a-images[0].base+len(raw)])
expected_jumps={(p,v) for p in jumps for v in struct.unpack('<19I',raw)}
points=set.union(*owned.values());checked=invocations=compositions=0
machines=[]
for c,pe in enumerate(images):
    u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(pe.base,(len(pe.data)+4095)&~4095);u.mem_write(pe.base,bytes(pe.data))
    u.mem_map(0x30000000,0x10000);u.mem_map(STOP,4096);u.mem_map(S,0x50000);u.mem_write(0x560000,seed)
    state={'writes':[]};machines.append((c,u,state))
    def stores(uc,access,at,size,value,data):data['writes'].append((at,size,value&((1<<(8*size))-1)))
    u.hook_add(UC_HOOK_MEM_WRITE,stores,state,begin=0x560000,end=0x126cfff)
    if not c:
        def observe(uc,pc,size,data):
            hits.add(pc)
            if last[0] in branches:edges.add((last[0],pc))
            if last[0] in jumps:jump_edges.add((last[0],pc))
            last[0]=pc
        for rows in owned.values():u.hook_add(UC_HOOK_CODE,observe,begin=min(rows),end=max(rows))

def fixture(u,level=5,next_level=0,size=2,model=None,fields=None,food_ids=(),stock=0,player=0,desire=0,low=-10,high=10,global_mode=0,special=0,counter=0):
    r=BID*264;u.mem_write(0x7999d8+r,bytes(264));word(u,0x7999ea+r,level);byte(u,0x7999db+r,size)
    word(u,0x7999e8+r,level+10);byte(u,0x7999dc+r,special);byte(u,0x799a96+r,desire)
    word(u,0x7999de+r,90);word(u,0x7999e0+r,80);put(u,0x7999e4+r,19330)
    row=(level+(1 if next_level else 0))*88
    u.mem_write(0x70cdcc+level*88,bytes(88));u.mem_write(0x70cdcc+row,bytes(88))
    put(u,0x70cdcc+level*88,low);put(u,0x70cdd0+level*88,high)
    for col,value in (model or {}).items():put(u,0x70cdcc+row+col*4,value)
    for at,size,value in (fields or []):
        (byte if size==1 else word if size==2 else put)(u,at+r,value)
    put(u,0xf53dd8,len(food_ids));u.mem_write(0xf53ddc,bytes(128))
    for i,v in enumerate(food_ids):put(u,0xf53ddc+i*4,v)
    # Includes the native unsupported-ID slot immediately before stock[0].
    for i in range(-1,12):word(u,0x799a2c+r+i*2,stock)
    for at,size,value in (fields or []):
        (byte if size==1 else word if size==2 else put)(u,at+r,value)
    put(u,0xeaac28,player);offset=(player*0x49d8)&0xffffffff
    for at in range(0xea5b74,0xea5c00,4):put(u,(at+offset)&0xffffffff,counter)
    for at in [0xea5f14,0xea5f6c,0xea5fb8,0xea5fbc]:put(u,(at+offset)&0xffffffff,counter)
    put(u,0x78d838,BID);put(u,0x978d70,global_mode)
    for at,v in [(0xbca2d8,1000),(0xbca2d0,400),(0xbca2d4,400),(0xe94e04,0)]:put(u,at,v)
    for start,scale in [(0xb285cc,1),(0xb67d1c,1),(0xacf85c,1),(0xb7482c,4),(0xaf598c,4),(0xb4e6fc,2)]:u.mem_write(start+19000*scale,b'\xa5'*(1000*scale))

def invoke(c,u,token,next_level,route):
    global invocations
    invocations+=1;argv=[] if token==0x4539a0 else [BID] if token==0x453af0 else [BID,next_level]
    frame=route=='frame'
    if not c:entry=token
    elif route=='named':entry=sym(names[token])
    elif route=='legacy':entry=sym('FUN_%08x'%token)
    else:
        u.mem_write(VSP-128,b'\x6b'*192);put(u,VSP,STOP)
        for i,v in enumerate(argv):put(u,VSP+4+i*4,v)
        u.mem_write(FRAME,struct.pack('<10I',0x89abcdef,0x87654321,0x98765432,saved[0],VSP,*saved[1:],0x202,0))
        entry=sym('restore_building_control_C_%08x'%token);argv=[FRAME]
    for reg,v in zip(registers,[0x89abcdef,0x87654321,0x98765432,saved[0],SP,*saved[1:]]):u.reg_write(reg,v)
    u.reg_write(UC_X86_REG_EFLAGS,0x202);put(u,SP,STOP)
    for i,v in enumerate(argv):put(u,SP+4+i*4,v)
    u.emu_start(entry,STOP,count=2000000)
    assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==SP+4
    assert [u.reg_read(registers[i]) for i in [3,5,6,7]]==saved
    if frame:
        assert get(u,FRAME+16)==VSP+4 and [get(u,FRAME+i*4) for i in [3,5,6,7]]==saved
        return get(u,FRAME)
    return u.reg_read(UC_X86_REG_EAX)

def exercise(token=0x453b40,**values):
    global checked
    expected=None
    for c,u,state in machines:
        routes=['named']
        if available('FUN_%08x'%token):routes.append('legacy')
        if available('restore_building_control_C_%08x'%token):routes.append('frame')
        for route in routes if c else ['native']:
            fixture(u,**values);state['writes']=[];last[0]=None
            try:value=invoke(c,u,token,values.get('next_level',0),route)
            except Exception as e:raise AssertionError(('execution',hex(token),values,route,hex(u.reg_read(UC_X86_REG_EIP)))) from e
            result=(value,tuple(state['writes']))
            if expected is None:expected=result
            else:assert result==expected,(hex(token),values,route,'native',expected,'C',result)
    checked+=1

def housing_controller(token,level,desire):
    global compositions
    expected=None
    for c,u,state in machines:
        fixture(u,level=level,next_level=1,special=1,size=1,desire=desire)
        r=BID*264;byte(u,0x7999d8+r,1);byte(u,0x7999dd+r,1);word(u,0x7999f4+r,10)
        state['writes']=[];last[0]=None
        if c:
            u.mem_write(VSP-256,b'\x6b'*320);put(u,VSP,STOP)
            u.mem_write(FRAME,struct.pack('<8I',0x89abcdef,0x87654321,0x98765432,saved[0],VSP,*saved[1:]))
            put(u,FRAME+32,0x202)
            run(0,u,sym('restore_building_control_raw'),[token,FRAME,FRAME+32])
            assert get(u,FRAME+16)==VSP+4 and [get(u,FRAME+i*4) for i in [3,5,6,7]]==saved
        else:run(0,u,token)
        result=tuple(state['writes'])
        if expected is None:expected=result
        else:assert result==expected,('actual housing controller',hex(token),level,desire,'native',expected,'C',result)
    compositions+=1

if __name__=='__main__':
    for level,desire,low,high in itertools.product([-2,-1,0,5,18,19,20,255,32767],[-128,-1,0,1,127,128,255],[-128,0,127],[-1,1,1000]):exercise(0x453af0,level=level,desire=desire,low=low,high=high)
    for mode,special,level in itertools.product([0,1,2,0xffffffff],[0,1,255],[0,5,19]):exercise(0x4539a0,level=level,size=1,global_mode=mode,special=special)
    columns={2:(0x799a58,1),3:(0x799a0d,1),4:(0x799a5b,1),5:(0x799a59,1),7:(0x799a4d,1),8:(0x799a48,1),9:(0x799a5a,1),10:None,11:(0x799a3e,2),12:(0x799a40,2),13:(0x799a42,2),14:(0x799a3c,2)}
    for col,requirement,value,size in itertools.product(columns,[-2147483648,-1,0,1,2,3,4,2147483647],[-128,-1,0,1,2,3,127,128,255,32767,65535],[1,2]):
        field=columns[col];fields=[] if field is None else [(*field,value)]
        exercise(model={col:requirement},fields=fields,size=size)
    for need,well,supply in itertools.product([0,1,2,3],[0,1,255],[0,1,255]):exercise(model={3:need},fields=[(0x799a0d,1,supply),(0x799a17,1,well)])
    for foods,stock,required in itertools.product([(),(0,),(1,),(1,1),(1,2,3,4,5,6,7,8,13,15,17,19),tuple(range(21)),(0xffffffff,30,256)],[-1,0,1,32767],[-1,0,1,2,12,30]):exercise(model={10:required},food_ids=foods,stock=stock)
    for col,value,counter in itertools.product([5,7,8,9,13],[1,2,3],[0,1,2,0xffffffff]):exercise(model={col:value},fields=[(0x799a59,1,127),(0x799a4d,1,255),(0x799a48,1,255),(0x799a5a,1,127),(0x799a42,2,32767)],counter=counter)
    for player,next_level in itertools.product([0,1,3,0x20000000,0x80000000],[0,1,2,0xffffffff]):exercise(player=player,next_level=next_level,model={5:2,8:2},fields=[(0x799a59,1,2),(0x799a48,1,2)])
    rng=random.Random(0x453b40)
    for i in range(300):
        model={col:rng.choice([-1,0,1,2,3]) for col in columns}
        fields=[]
        for field in columns.values():
            if field:fields.append((*field,rng.choice([0,1,2,3,127,255])))
        exercise(model=model,fields=fields,size=rng.choice([1,2,4]),food_ids=tuple(rng.randrange(21) for _ in range(8)),stock=rng.randrange(2))
    controllers=[0x452980,0x452a20,0x452b00,0x452bd0,0x452ca0,0x452d70,0x452e40,0x452f10,0x452fe0,0x4530b0,0x4531c0,0x453280,0x453340,0x453400,0x453500,0x4535c0,0x453680,0x453740,0x453840,0x453900]
    for level,token in enumerate(controllers):
        for desire in [-20,0]:housing_controller(token,level,desire)
    assert points<=hits,('unexecuted instructions',[hex(a) for a in sorted(points-hits)])
    missing={(a,b) for a,targets in branches.items() for b in targets if (a,b) not in edges}
    assert not missing,('unexecuted conditional edges',[(hex(a),hex(b)) for a,b in sorted(missing)])
    assert expected_jumps<=jump_edges,('unexecuted switch targets',expected_jumps-jump_edges)
    sources=['include/house_evolution.h','runtime/house_evolution.c','runtime/building_controllers.c','runtime/building_controllers_model.c','runtime/building_support.c','CMakeLists.txt','direct_c/verify_house_evolution.py','metadata/house-evolution-native.json']
    proof=dict(native_sha256=catalogue['native_sha256'],compiled_C_path=str(target.resolve()),compiled_C_sha256=target_sha,passed=True,checked=checked,invocations=invocations,
               coverage={hex(a):dict(instructions=len(p),executed=len(p&hits)) for a,p in owned.items()},conditional_edges=len(edges),indirect_jump_edges=len(jump_edges),
               actual_housing_controller_compositions=compositions,controlled_algorithm_endpoints=[],controlled_platform_endpoints=[],source_hashes={p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest() for p in sources},commit=None,
               limitations=['Original analyzed bodies, full EAX, exact ordered global stores, saved registers and caller-cleanup frame checks.',
                            'Actual global-mode house image changes and map stamping execute; no substituted endpoints or host game launch.',
                            'Mapped fixture levels and memory only; no concurrent changes, malformed stack certification or complete gameplay claim.'])
    suffix='diagnostic' if target.stem=='restore_link_check' else 'game'
    (ROOT/f'metadata/house-evolution-{suffix}.json').write_text(json.dumps(proof,indent=2)+'\n')
    print('PASS house evolution:',checked,'scenarios',invocations,'invocations',flush=True)
    print('Actual housing-controller compositions:',compositions,flush=True)
    print('Native instructions/conditional/switch edges:',len(points),len(edges),len(jump_edges),flush=True)
    print('Verified target SHA256:',target_sha,flush=True)
