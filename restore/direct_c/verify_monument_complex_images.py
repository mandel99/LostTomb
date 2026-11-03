"""Offline original/C image members, actual material/progress calls and ABI routes.

No host game or renderer runs. Optional child-entry mutations test load order;
the actual child bodies and their return computations still execute.
"""
from verify_options_startup import *
from unicorn import UC_HOOK_MEM_WRITE
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
import json

SELF, OUT, FRAME, VSP = S+0x1000, S+0x3000, S+0x8000, S+0x18000
BID = 64
TABLES = {0x545760:0x570160, 0x558c00:0x570268, 0x555ea0:0x570228, 0x54f490:0x5701e8}
COUNTS = dict(zip(TABLES, [9,10,21,31]))
PROGRESS = dict(zip(TABLES, [0x545740,0x558be0,0x555e80,0x54f470]))
available = cache(lambda n: bool(re.search(r'\s_'+re.escape(n)+r'\s', symbols)))
catalogue = json.loads((ROOT/'metadata/monument-complex-images-native.json').read_text())
assert catalogue['native_sha256'] == hashlib.sha256(original.read_bytes()).hexdigest()
decoder = Cs(CS_ARCH_X86,CS_MODE_32)
owned, branches, jumps, expected_jumps = {}, {}, {}, set()
hits, edges, jump_edges, last = set(), set(), set(), [None]
checked = invocations = 0
compositions = 0
for f in catalogue['functions']:
    a = int(f['address'],16); raw = bytes.fromhex(f['bytes_hex'])
    assert raw == bytes(images[0].data[a-images[0].base:a-images[0].base+len(raw)])
    insns = list(decoder.disasm(raw[:f['body_bytes']],a))
    assert sum(i.size for i in insns) == f['body_bytes']
    owned[a] = {i.address for i in insns}
    for field in ['jump_table','case_map']:
        t = f[field]; start = int(t['address'],16); data = bytes.fromhex(t['bytes_hex'])
        assert data == bytes(images[0].data[start-images[0].base:start-images[0].base+len(data)])
    table = struct.unpack('<%dI'%f['jump_table']['count'], bytes.fromhex(f['jump_table']['bytes_hex']))
    targets = {table[i] for i in bytes.fromhex(f['case_map']['bytes_hex'])}
    for i in insns:
        if i.mnemonic.startswith('j') and i.mnemonic != 'jmp': branches[i.address] = {int(i.op_str,16),i.address+i.size}
        if i.mnemonic == 'jmp' and 'ptr [' in i.op_str:
            jumps[i.address] = targets; expected_jumps.update((i.address,t) for t in targets)
    assert struct.unpack_from('<I',images[0].data,TABLES[a]+28-images[0].base)[0] == a
points = set.union(*owned.values())
machines = []
for c,pe in enumerate(images):
    u = Uc(UC_ARCH_X86,UC_MODE_32)
    u.mem_map(pe.base,(len(pe.data)+4095)&~4095); u.mem_write(pe.base,bytes(pe.data))
    u.mem_map(0x30000000,0x10000); u.mem_map(STOP,4096); u.mem_map(S,0x50000)
    u.mem_write(0x560000,seed)
    state = {'writes':[], 'calls':[], 'token':None, 'mutation':''}
    machines.append((c,u,state))
    def stores(uc,access,at,size,value,data): data['writes'].append((at,size,value&((1<<(size*8))-1)))
    u.hook_add(UC_HOOK_MEM_WRITE,stores,state,begin=0x560000,end=0x126cfff)
    u.hook_add(UC_HOOK_MEM_WRITE,stores,state,begin=S,end=S+0x3fff)
    def child(uc,pc,size,data):
        kind = data['entry_kinds'][pc]
        if kind == 'progress':
            sp = uc.reg_read(UC_X86_REG_ESP)
            args = (get(uc,sp+4),get(uc,sp+8),get(uc,sp+12),get(uc,sp+16)) if data['compiled'] else (uc.reg_read(UC_X86_REG_ECX),get(uc,sp+4),COUNTS[data['token']],get(uc,sp+8))
        else: args = ()
        data['calls'].append((kind,args,bytes(uc.mem_read(SELF,0x58)),bytes(uc.mem_read(0x7999d8+BID*264,264))))
        r = BID*264
        if data['mutation'] == kind:
            if kind == 'material':
                put(uc,0xbca2e0,8); put(uc,0x799a68+r,3); put(uc,0x799a38+r,5)
                put(uc,SELF+0x50,0); put(uc,SELF+0x54,0); put(uc,0x799a40+r,1)
            else:
                byte(uc,0x799a6c+r,0); word(uc,0x7999ee+r,0xffff)
    kinds = {sym('FUN_004ea930') if c else 0x545fb0:'material'}
    kinds.update({sym('restore_monument_progress'):'progress'} if c else {p:'progress' for p in PROGRESS.values()})
    state.update(entry_kinds=kinds,compiled=c)
    for at in kinds: u.hook_add(UC_HOOK_CODE,child,state,begin=at,end=at)
    if not c:
        def observe(uc,pc,size,data):
            hits.add(pc)
            if last[0] in branches: edges.add((last[0],pc))
            if last[0] in jumps: jump_edges.add((last[0],pc))
            last[0] = pc
        for rows in owned.values(): u.hook_add(UC_HOOK_CODE,observe,begin=min(rows),end=max(rows))

def fixture(u,token,shape,step,mode,material,active,marker,complete,piece,terminal,camera,orientation,zero,worker):
    u.mem_write(SELF,bytes(0x100)); put(u,SELF,TABLES[token]); put(u,SELF+0x50,terminal); put(u,SELF+0x54,mode)
    u.mem_write(OUT-8,b'\x6b'*0x100)
    u.mem_write(0x7999d8+32*264,bytes(96*264)); r=BID*264
    put(u,0x799a38+r,step); put(u,0x799a40+r,shape); put(u,0x799a68+r,orientation)
    word(u,0x7999ea+r,piece); word(u,0x7999ee+r,marker); byte(u,0x799a6c+r,active); byte(u,0x799a74+r,37)
    put(u,0xbca2e0,camera)
    u.mem_write(0x784de8,bytes(0x130)); put(u,0x784dec,0 if material else 1)
    for at in range(0xf5746c,0xf5784c,8): put(u,at,0 if zero else 0x12000000+(at-0xf5746c)*17)
    if complete:
        root=BID-(piece if piece<32768 else piece-65536)
        other=root+COUNTS[token]-1
        if other==BID: other=root
        put(u,0x799a38+other*264,1000000)
    u.mem_write(0xbcd600+7*388,bytes(388))
    if worker:
        root=BID-(piece if piece<32768 else piece-65536)
        put(u,0x799a34+root*264,7)
        byte(u,0xbcd606+7*388,worker!=1); byte(u,0xbcd602+7*388,0x51 if worker==2 else 0x6c if worker==3 else 0x99)
        byte(u,0xbcd630+7*388,9)
    for reg,v in [(UC_X86_REG_FPCW,0x37f),(UC_X86_REG_FPSW,0),(UC_X86_REG_FPTAG,0xffff)]: u.reg_write(reg,v)

def invoke(c,u,token,out,route):
    global invocations
    invocations+=1; argv=[BID,out]; cleanup=0; frame=False
    if not c: entry=token; cleanup=8
    elif route=='legacy': entry=sym('FUN_%08x'%token)
    elif route=='named': entry=sym('restore_complex_image_%08x'%token); argv=[SELF,*argv]
    elif route=='common': entry=sym('restore_complex_image_query'); argv=[token,SELF,*argv]
    elif route=='facade': entry=sym('restore_placement_image_query'); argv=[SELF,*argv]
    else:
        frame=True; u.mem_write(VSP-128,b'\x6b'*192); put(u,VSP,STOP); put(u,VSP+4,BID); put(u,VSP+8,out)
        u.mem_write(FRAME,struct.pack('<10I',0x89abcdef,SELF,0x98765432,saved[0],VSP,*saved[1:],0x202,sym('restore_building_create_dispatch')))
        entry=sym('restore_placement_queries_dispatch' if route=='placement' else 'restore_building_create_dispatch'); argv=[0,token,FRAME]
    for reg,v in zip(registers,[0x89abcdef,SELF,0x98765432,saved[0],SP,*saved[1:]]): u.reg_write(reg,v)
    u.reg_write(UC_X86_REG_EFLAGS,0x202); put(u,SP,STOP)
    for i,v in enumerate(argv): put(u,SP+4+i*4,v)
    u.emu_start(entry,STOP,count=300000)
    assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==SP+4+cleanup
    assert [u.reg_read(registers[i]) for i in [3,5,6,7]]==saved
    if frame:
        assert get(u,FRAME+16)==VSP+12 and [get(u,FRAME+i*4) for i in [3,5,6,7]]==saved
        assert bytes(u.mem_read(VSP-128,64))==b'\x6b'*64 and bytes(u.mem_read(VSP+12,48))==b'\x6b'*48
        return get(u,FRAME)
    return u.reg_read(UC_X86_REG_EAX)

def exercise(token,shape=0,step=3,mode=0,material=0,active=0,marker=0,complete=False,piece=0,terminal=7,camera=0,orientation=0,zero=False,worker=0,out=OUT,mutation=''):
    global checked
    expected=None
    for c,u,state in machines:
        routes=['common','placement','building']
        for name,route in [('restore_placement_image_query','facade'),('restore_complex_image_%08x'%token,'named'),('FUN_%08x'%token,'legacy')]:
            if available(name): routes.append(route)
        for route in routes if c else ['native']:
            fixture(u,token,shape,step,mode,material,active,marker,complete,piece,terminal,camera,orientation,zero,worker)
            state.update(writes=[],calls=[],token=token,mutation=mutation); last[0]=None
            try: value=invoke(c,u,token,out,route)
            except Exception as e: raise AssertionError(('execution',hex(token),shape,step,mode,material,active,marker,complete,piece,hex(out),mutation,route,hex(u.reg_read(UC_X86_REG_EIP)))) from e
            result=(value,tuple(state['writes']),tuple(state['calls']))
            if expected is None: expected=result
            else: assert result==expected,(hex(token),shape,step,mode,material,active,marker,complete,piece,terminal,camera,orientation,zero,worker,hex(out),mutation,route,'native',expected,'C',result)
    checked+=1

def footprint(token,shape,step,piece):
    global compositions
    expected=None
    for c,u,state in machines:
        fixture(u,token,shape,step,0,1,1,0,False,piece,7,0,0,False,0)
        r=0x7999d8+BID*264
        byte(u,r+3,2); word(u,r+6,90); word(u,r+8,80); put(u,r+80,0)
        for at,value in [(0xbca2d8,1000),(0xbca2d0,400),(0xbca2d4,400),(0xe94e0c,0),(0xe94e04,0)]: put(u,at,value)
        for i in range(36): put(u,0x5cc8c0+i*8,(i//6)*228+i%6)
        for start,scale in [(0xb285cc,1),(0xb67d1c,1),(0xacf85c,1),(0xb7482c,4),(0xaf598c,4),(0xb4e6fc,2)]: u.mem_write(start+18000*scale,b'\xa5'*(3000*scale))
        state.update(writes=[],calls=[],token=token,mutation=''); last[0]=None
        entry=sym('restore_monument_commit_footprint') if c else 0x5468d0
        try: run(0,u,entry,[SELF,BID] if c else [BID],cleanup=0 if c else 4,receiver=None if c else SELF)
        except Exception as e: raise AssertionError(('footprint execution',hex(token),shape,step,piece,c,hex(u.reg_read(UC_X86_REG_EIP)))) from e
        result=(tuple(state['writes']),tuple(state['calls']))
        if expected is None: expected=result
        else: assert result==expected,('footprint',hex(token),shape,step,piece,'native',expected,'C',result)
    compositions+=1

if __name__=='__main__':
    for token in TABLES:
        for shape,step,mode in itertools.product([*range(30),30,256,0xffffffff],range(7),[0,1]): exercise(token,shape,step,mode)
        contexts=[(0,0,0xffff,False),(0,1,0xffff,False),(0,255,0,False),(1,0,0xffff,True),(1,1,0,False),(1,1,0xffff,False),(1,1,0,True),(1,255,0xffff,True)]
        for shape,step,context in itertools.product([0,18,27,28,29,30],[3,4,5],contexts): exercise(token,shape,step,material=context[0],active=context[1],marker=context[2],complete=context[3])
        for step,piece,match in itertools.product([3,4,5],[0,1,29,65535],[0,1,2]):
            terminal=(piece if piece<32768 else piece-65536)&0xffffffff
            terminal=terminal if match==0 else terminal^1 if match==1 else terminal^0x10000
            exercise(token,step=step,piece=piece,terminal=terminal)
        for camera,orientation,shape in itertools.product([0x80000000,-5,-3,-2,-1,0,1,2,3,4,6,8,0x7fffffff],[0,1,2,3,4,0xffffffff,0x80000000,0x7fffffff],[0,18]): exercise(token,shape,camera=camera,orientation=orientation)
        aliases=[OUT,OUT+1,SELF,SELF+0x50,SELF+0x54,0xbca2e0,0xf574dc,0xf5746c,0xf57844]+[at+BID*264 for at in [0x799a38,0x799a40,0x799a68,0x799a6c,0x799a74,0x7999ea,0x7999ee]]
        for out,shape,step in itertools.product(aliases,[0,18,27,29],[0,3]): exercise(token,shape,step,mode=1,out=out)
        for shape,mode in itertools.product(range(30),[0,1]): exercise(token,shape,mode=mode,zero=True)
        for mutation,complete in itertools.product(['material','progress'],[False,True]): exercise(token,step=3,mode=1,material=1,active=1,complete=complete,mutation=mutation)
        for worker,complete in itertools.product(range(1,5),[False,True]): exercise(token,material=1,active=1,worker=worker,complete=complete)
        for shape,step,piece in itertools.product([0,1,18,27,28,29],[0,3],[0,1]): footprint(token,shape,step,piece)
    missing={(a,b) for a,targets in branches.items() for b in targets if (a,b) not in edges}
    assert points<=hits,('unexecuted instructions',[hex(a) for a in sorted(points-hits)])
    assert not missing,('unexecuted conditional edges',[(hex(a),hex(b)) for a,b in sorted(missing)])
    assert jump_edges==expected_jumps,('switch coverage',expected_jumps-jump_edges)
    sources=['include/monument_complex_images.h','runtime/monument_complex_images.c','runtime/placement_queries.c','runtime/monument_progress.c','src/unit_02064.c','src/unit_02581.c','CMakeLists.txt','direct_c/verify_monument_complex_images.py','metadata/monument-complex-images-native.json']
    proof=dict(native_sha256=catalogue['native_sha256'],compiled_C_path=str(target.resolve()),compiled_C_sha256=target_sha,passed=True,checked=checked,invocations=invocations,
               coverage={hex(a):dict(instructions=len(rows),executed=len(rows&hits)) for a,rows in owned.items()},conditional_edges=len(edges),indirect_jump_edges=len(jump_edges),
               actual_material_and_progress=True,actual_footprint_compositions=compositions,controlled_algorithm_endpoints=[],controlled_platform_endpoints=[],
               source_hashes={p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest() for p in sources},commit=None,
               limitations=['Native body boundaries inferred from RET8 and adjacent switch data; REA bytes authenticated against the pinned original PE.',
                            'Mapped output/record overlaps, full EAX, ordered global/output writes, actual child-entry observations, ABI preservation and frame cleanup compared.',
                            'Selected child-entry input mutations exercise cached versus fresh loads without replacing child return calculations.',
                            'No concurrent mutation, unmapped input certification, host game launch, rendering or full-gameplay claim.'])
    suffix='diagnostic' if target.stem=='restore_link_check' else 'game'
    (ROOT/f'metadata/monument-complex-images-{suffix}.json').write_text(json.dumps(proof,indent=2)+'\n')
    print('PASS complex monument images:',checked,'scenarios',invocations,'invocations',flush=True)
    print('Actual footprint/image/activity/stamping compositions:',compositions,flush=True)
    print('Native instructions/conditional/switch edges:',len(points),len(edges),len(jump_edges),flush=True)
    print('Verified target SHA256:',target_sha,flush=True)
