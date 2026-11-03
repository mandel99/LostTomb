"""Native/C formation creation, recruitment and complete refresh, offline.

All real descendants execute; compare owned ordered global stores, immediate
child inputs, all final original data, saved registers and stack balance.
"""
from verify_options_startup import *
from unicorn import UC_HOOK_MEM_WRITE
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import json

BID,FRAME,VSP=64,S+0x8000,S+0x18000
catalogue=json.loads((ROOT/'metadata/formation-control-native.json').read_text())
assert catalogue['native_sha256']==hashlib.sha256(original.read_bytes()).hexdigest()
owned={};branches={};hits=set();edges=set();last=[None];checked=invocations=0
decoder=Cs(CS_ARCH_X86,CS_MODE_32)
for f in catalogue['functions']:
    a=int(f['address'],16);raw=bytes.fromhex(f['bytes_hex'])
    assert raw==bytes(images[0].data[a-images[0].base:a-images[0].base+len(raw)])
    instructions=list(decoder.disasm(raw,a));owned[a]={int(p,16) for p in f['instruction_addresses'].split()}
    assert {i.address for i in instructions}==owned[a]
    for i in instructions:
        if i.mnemonic.startswith('j') and i.mnemonic!='jmp':branches[i.address]={int(i.op_str,16),i.address+i.size}
points=set.union(*owned.values())
names={0x4b6e10:'restore_formation_create',0x472790:'restore_formation_recruit',0x4b7c80:'restore_formation_refresh'}
available=cache(lambda n:bool(re.search(r'\s_'+re.escape(n)+r'\s',symbols)))
publics=sorted({int(a,16) for a in re.findall(r'^\s+[0-9a-fA-F]{4}:[0-9a-fA-F]+\s+\S+\s+([0-9a-fA-F]{8})\s',symbols,re.M)})
compiled_ranges=[(sym(n),next(a for a in publics if a>sym(n))) for n in names.values()]
eligible_entries=re.findall(r'\s_eligible\s+([0-9a-fA-F]{8})\s+[^\r\n]*formation_control.obj',symbols)
assert len(eligible_entries)==1
eligible_entry=int(eligible_entries[0],16)
compiled_call_ranges=compiled_ranges+[(eligible_entry,next(a for a in publics if a>eligible_entry))]
calls={0x4b77d0:0,0x4b4800:5,0x518400:4,0x4ea9a0:1,0x4ea9c0:1,0x47d350:0,0x40f8f0:3,0x40b5e0:1}
NULL_EVENT=((-0x785d74//4)*pow(31,-1,1<<30))%(1<<30)
assert (0x785d74+NULL_EVENT*124)&0xffffffff==0
machines=[]
for c,pe in enumerate(images):
    u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(pe.base,(len(pe.data)+4095)&~4095);u.mem_write(pe.base,bytes(pe.data))
    u.mem_map(0x30000000,0x10000);u.mem_map(STOP,4096);u.mem_map(S,0x50000)
    state=dict(writes=[],events=[],collect=False,compiled=c);machines.append((c,u,state))
    def stores(uc,access,at,size,value,data):
        if not data['collect'] or not data['in_owned']:return
        pc=uc.reg_read(UC_X86_REG_EIP)
        own=pc in points if not data['compiled'] else any(lo<=pc<hi for lo,hi in compiled_ranges)
        if own:data['writes'].append((at,size,value&((1<<(8*size))-1)))
        if data['mutation']=='readiness' and at==0xbcb9c8+144+90 and size==2 and value==1:
            byte(uc,0xbcb9c8+144+40,0)
    u.hook_add(UC_HOOK_MEM_WRITE,stores,state,begin=0x560000,end=0x126cfff)
    if not c:
        def observe(uc,pc,size,data):
            if not data['collect']:return
            hits.add(pc)
            if last[0] in branches:edges.add((last[0],pc))
            last[0]=pc
        for rows in owned.values():u.hook_add(UC_HOOK_CODE,observe,state,begin=min(rows),end=max(rows))
    def entry(uc,pc,size,data):
        if not data['collect']:return
        t=data['entries'][pc];sp=uc.reg_read(UC_X86_REG_ESP)
        args=tuple(get(uc,sp+4+i*4) for i in range(calls[t]));ret=get(uc,sp)
        if t==0x4b4800:args=(args[0]&255,*args[1:4],args[4]&255)
        immediate=ret in points if not data['compiled'] else any(lo<=ret<hi for lo,hi in compiled_call_ranges)
        if immediate:data['events'].append((t,args))
        data['counts'][t]=data['counts'].get(t,0)+1;first=data['counts'][t]==1;mutation=data['mutation']
        if mutation=='allocate' and t==0x4b4800 and first:
            word(uc,0xbcb9ce+7*144,80);byte(uc,0x7999d9+BID*264,3);byte(uc,0x7999d9+80*264,1)
            word(uc,0xbcb9cc+7*144,13);put(uc,0xeaac28,3)
        if mutation=='path' and t==0x518400 and first:
            put(uc,0xeaac28,3);word(uc,0xbcb9f6+7*144,92);word(uc,0xbcb9f8+7*144,80)
        if mutation=='query' and t==0x40b5e0 and first:
            put(uc,0xeaac28,3)
            for at in [0xea5ed5,0xea5ed6,0xea5eda,0xea5edb,0xea5edc,0xea5edd,0xea5edf]:byte(uc,at+3*0x49d8,255)
        if mutation=='null_event' and t==0x40b5e0 and first:
            put(uc,0xbca45c,NULL_EVENT);put(uc,0x785d70,NULL_EVENT+1)
        if mutation=='kind_a' and t==0x4ea9a0 and first:byte(uc,0xbcd602+388,82)
        if mutation=='kind_c' and t==0x4ea9c0 and first:byte(uc,0xbcd602+388,99)
    state['entries']={(sym('FUN_%08x'%t) if c else t):t for t in calls}
    for at in state['entries']:u.hook_add(UC_HOOK_CODE,entry,state,begin=at,end=at)
    # Bulk clears in real path searches must execute, but their individual
    # stores are not owned stores. Skip the costly EIP lookup in those callees.
    phases={}
    ranges=compiled_ranges if c else [(min(p),max(p)+1) for p in owned.values()]
    for lo,hi in ranges:
        phases[lo]=True
        for ins in decoder.disasm(bytes(pe.data[lo-pe.base:hi-pe.base]),lo):
            if ins.mnemonic=='call':phases[ins.address]=False;phases[ins.address+ins.size]=True
    def ownership(uc,pc,size,data):data['in_owned']=data['phases'][pc]
    state['phases']=phases
    for at in phases:u.hook_add(UC_HOOK_CODE,ownership,state,begin=at,end=at)

def base(u):
    u.mem_write(0x560000,seed);u.mem_write(S,bytes(0x50000))
    u.mem_write(0xbcb9c8,bytes(50*144));u.mem_write(0xbcd5f8,bytes(2000*388))
    u.mem_write(0x7999d8,bytes(4000*264));u.mem_write(0xeaaf68,bytes(0x2bc0));u.mem_write(0x9784d8,bytes(0x8a0))
    u.mem_write(0xb350dc,bytes(0x19620));u.mem_write(0xee0c50,b'\xff'*0xcb10)
    for at,v in [(0xeaac28,0),(0xbca2d8,1000),(0xbca2d0,200),(0xbca2d4,200),(0xbca2dc,28),(0x785d70,150),
                 (0xbcb9bc,0),(0xbcb9b4,0),(0xd056fc,10),(0xbcd5f0,0),(0x604950,1234567)]:put(u,at,v)
    r=BID*264;byte(u,0x7999d9+r,0);word(u,0x7999ea+r,11);word(u,0x7999de+r,90);word(u,0x7999e0+r,80)
    for city in [0,3]:
        off=city*0x49d8
        for at,v in [(0xea3fe8,90),(0xea3fea,80),(0xea3ff0,92),(0xea3ff2,80)]:word(u,at+off,v)

def group(u,id=1,active=1,military=1,skip=0,kind=11,event=1,flag=1,ready=0,colour=0):
    g=0xbcb9c8+id*144
    for off,val in [(0,active),(2,colour),(66,military),(105,skip),(103,flag)]:byte(u,g+off,val)
    for off,val in [(4,kind),(6,BID),(90,ready),(120,event),(46,90),(48,80)]:word(u,g+off,val)

def actor(u,id=1,kind=11,formation=1,state=1,action=16,marker=8,tag=1,contribution=0):
    a=0xbcd5f8+id*388
    for off,val in [(10,kind),(14,state),(56,action),(17,marker),(99,tag),(29,contribution)]:byte(u,a+off,val)
    word(u,a+96,formation)

def create(u,free=1,colours=(),defaults=(0,1,2),maximum=0,position=(90,80),full_actors=False,flags=1,military=1):
    base(u)
    for i in range(1,free if free else 50):group(u,i,colour=(i-1)%8,active=flags,military=military)
    for i,colour in enumerate(colours):group(u,i,colour=colour)
    for at,v in zip([0x6fdce4,0x6fdcf0,0x6fdcfc],defaults):put(u,at,v)
    put(u,0xbcb9bc,maximum);word(u,0x7999de+BID*264,position[0]);word(u,0x7999e0+BID*264,position[1])
    if full_actors:
        for i in range(1,2000):byte(u,0xbcd606+i*388,1)
    return [BID]

def recruit(u,count=1,path=0,kind=11,full=False):
    base(u);group(u,7,kind=kind)
    word(u,0xbcb9f6+7*144,90 if path==0 else 92 if path==1 else 94)
    if full:
        for i in range(1,2000):byte(u,0xbcd606+i*388,1)
    return [count,7,95,81]

def refresh(u,kinds=(11,),population=None,military=1,active=1,skip=0,event=1,ready=0,marker=8,action=16,state=1,tag=1,flag=1,weight=0,contribution=0,group_id=1):
    base(u);group(u,group_id,military=military,active=active,skip=skip,event=event,ready=ready,kind=kinds[0] if kinds else 11,flag=flag)
    if population is None:population=len(kinds)
    for i in range(population):actor(u,i+1,kind=kinds[i%len(kinds)],formation=group_id,state=state,action=action,marker=marker,tag=tag,contribution=contribution)
    for kind in kinds:word(u,0x7090d0+kind*56,weight)
    return []

def event_groups(u,scenario):
    base(u)
    if scenario=='many':
        for i in range(1,50):group(u,i,military=0,event=i)
    else:
        group(u,1,military=0,event=-1 if scenario=='negative' else 150 if scenario=='outside' else 1)
        group(u,2,military=0,event=1 if scenario=='duplicate' else 2)
        if scenario in ['surviving','nonmatch']:
            group(u,3,military=0,event=1 if scenario=='surviving' else 3);actor(u,formation=3)
        if scenario=='null':group(u,3);actor(u,formation=3)
    return []

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
    return value if token==0x4b6e10 else None

def exercise(token,prepare,label,mutation=''):
    global checked
    expected=None
    for c,u,state in machines:
        routes=['named','frame','raw']+(['legacy'] if available('FUN_%08x'%token) else [])
        for route in routes if c else ['native']:
            state['collect']=False;args=prepare(u);state.update(writes=[],events=[],collect=True,counts={},mutation=mutation,in_owned=True);last[0]=None
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
    for free in [1,2,7,49,0]:exercise(0x4b6e10,lambda u,f=free:create(u,free=f),('free slot',free))
    for colour in range(9):exercise(0x4b6e10,lambda u,c=colour:create(u,colours=tuple(range(c)),defaults=(255,255,255)),('palette',colour))
    for defaults in [(0,0,0),(7,6,5),(0x100,0x101,0x102)]:exercise(0x4b6e10,lambda u,d=defaults:create(u,defaults=d),('defaults',defaults))
    for maximum,position in itertools.product([0,1,50,0xffffffff],[(-32768,-32768),(32767,32767)]):exercise(0x4b6e10,lambda u,m=maximum,p=position:create(u,maximum=m,position=p),('maximum/WORD coordinates',maximum,position))
    exercise(0x4b6e10,lambda u:create(u,full_actors=True),'create actor allocation failure')
    exercise(0x4b6e10,lambda u:create(u,free=3,flags=2),'occupied non-one flag')
    for military in [0,2]:exercise(0x4b6e10,lambda u,m=military:create(u,free=3,military=m),('occupied group excluded from palette',military))
    for count in [-1,0]:exercise(0x472790,lambda u,c=count:recruit(u,c),('nonpositive recruitment',count))
    for count,path in itertools.product([1,3],range(3)):
        kind=11 if count==1 else 13
        exercise(0x472790,lambda u,c=count,p=path,k=kind:recruit(u,c,p,k),('recruit',count,path,kind))
    exercise(0x472790,lambda u:recruit(u,full=True),'recruit actor allocation failure')
    for mutation in ['allocate','path']:exercise(0x472790,lambda u:recruit(u,3),('recruit mutation',mutation),mutation)
    for kind in [0,10,11,12,13,14,42,43,53,54,55,56,57,58,68,69,70,71,82,83,84,85,99,102,103,104,105,255]:exercise(0x4b7c80,lambda u,k=kind:refresh(u,kinds=(k,)),('eligibility',kind))
    for state,action,tag in itertools.product([0,1,2],[4,16,18],[0,1]):exercise(0x4b7c80,lambda u,s=state,a=action,t=tag:refresh(u,state=s,action=a,tag=t),('actor fields',state,action,tag))
    for military,active,skip in itertools.product([0,1,2],[0,1,2],[0,1]):exercise(0x4b7c80,lambda u,m=military,a=active,s=skip:refresh(u,military=m,active=a,skip=s),('group fields',military,active,skip))
    for population in [0,1,16,17,255,256,257]:exercise(0x4b7c80,lambda u,p=population:refresh(u,population=p),('population/list limit',population))
    for ready,marker,kind in itertools.product([-1,0,1],[0,8],[11,13]):exercise(0x4b7c80,lambda u,r=ready,m=marker,k=kind:refresh(u,ready=r,marker=m,kinds=(k,)),('readiness',ready,marker,kind))
    exercise(0x4b7c80,lambda u:refresh(u,population=17,weight=32767,contribution=255),'WORD sums wrap')
    exercise(0x4b7c80,lambda u:refresh(u,group_id=0),'group zero is not reset')
    exercise(0x4b7c80,lambda u:refresh(u,flag=0,action=18),'city qualification flag zero')
    for scenario in ['many','negative','outside','duplicate','surviving','nonmatch','null']:exercise(0x4b7c80,lambda u,s=scenario:event_groups(u,s),('event groups',scenario),'null_event' if scenario=='null' else '')
    for mutation in ['query','readiness','kind_a','kind_c']:exercise(0x4b7c80,lambda u:refresh(u,kinds=(0,) if mutation.startswith('kind') else (13,),action=18),('refresh mutation',mutation),mutation)
    assert points<=hits,('unexecuted instructions',[hex(a) for a in sorted(points-hits)])
    missing={(a,b) for a,targets in branches.items() for b in targets if (a,b) not in edges}
    assert not missing,('unexecuted conditional edges',[(hex(a),hex(b)) for a,b in sorted(missing)])
    sources=['CMakeLists.txt','include/formation_control.h','runtime/formation_control.c','runtime/building_controllers.c','runtime/storage_settings.c','runtime/gatehouse_display.c','runtime/menu_drag_actor_create.c','direct_c/verify_formation_control.py','metadata/formation-control-native.json']+[f'src/unit_{i:05d}.c' for i in [1021,1471,1490]]
    proof=dict(native_sha256=catalogue['native_sha256'],compiled_C_path=str(target.resolve()),compiled_C_sha256=target_sha,passed=True,checked=checked,invocations=invocations,
               coverage={hex(a):dict(instructions=len(p),executed=len(p&hits)) for a,p in owned.items()},conditional_edges=len(edges),
               controlled_algorithm_endpoints=[],controlled_platform_endpoints=[],source_hashes={p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest() for p in sources},commit=None,
               limitations=['Three complete analyzed bodies; DWORD creation return and void other contracts. Owned ordered stores, actual immediate child inputs and all final original data compared.',
                            'Actual group preparation, path searches, actor allocation/profile/walking, RNG, classification, removal and status providers execute, including full actor tables.',
                            'Selected child-entry input changes and a readiness-store input mutation test native fresh reads and the otherwise unreachable null event-pointer guard; no returns are replaced.',
                            'Native eight-byte palette precondition: active military colours0..7. Mapped fixtures; no arbitrary stack corruption, concurrency, host game launch or complete-gameplay claim.'])
    suffix='diagnostic' if target.stem=='restore_link_check' else 'game'
    (ROOT/f'metadata/formation-control-{suffix}.json').write_text(json.dumps(proof,indent=2)+'\n')
    print('PASS formation control:',checked,'scenarios',invocations,'invocations;',len(points),'instructions,',len(edges),'conditional edges',flush=True)
    print('Verified target SHA256:',target_sha,flush=True)
