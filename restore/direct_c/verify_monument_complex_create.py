"""Four complete multipart methods with original constructors and actual callees."""
from verify_options_startup import *
from unicorn import UC_HOOK_MEM_WRITE,UC_HOOK_BLOCK
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import json

catalogue=json.loads((ROOT/'metadata/monument-complex-create-native.json').read_text())
assert catalogue['native_sha256']==hashlib.sha256(original.read_bytes()).hexdigest()
CLASSES={int(c['address'],16):{k:(int(v,16) if isinstance(v,str) else v) for k,v in c.items()} for c in catalogue['classes']}
NAMES={t:'restore_complex_create_%08x'%t for t in CLASSES}
FRAME,VSP=S+0x20000,S+0x40000
owned={};branches={};hits=set();edges=set();last=[None];checked=0;prefix_checked=0;invocations=0
decoder=Cs(CS_ARCH_X86,CS_MODE_32)
for f in catalogue['functions']:
    a=int(f['address'],16);b=bytes.fromhex(f['bytes_hex'])
    assert b==bytes(images[0].data[a-images[0].base:a-images[0].base+len(b)])
    if a not in CLASSES:continue
    owned[a]={int(s,16) for s in f['instruction_addresses'].split()}
    for at in owned[a]:
        ins=next(decoder.disasm(b[at-a:at-a+16],at,count=1))
        if ins.mnemonic.startswith('j') and ins.mnemonic!='jmp':branches[at]={int(ins.op_str,16),at+ins.size}
points=set.union(*owned.values())
available=lambda n:bool(re.search(r'\s_'+re.escape(n)+r'\s',symbols))
publics=sorted(set(int(x,16) for x in re.findall(r'^\s+[\da-fA-F]{4}:[\da-fA-F]+\s+\S+\s+([\da-fA-F]{8})\s',symbols,re.M)))
def private(name):
    matches=re.findall(r'\s_'+name+r'\s+([\da-fA-F]{8})\s[^\n]*monument_complex_create\.obj',symbols)
    assert len(matches)==1,(name,matches)
    return int(matches[0],16)
scope=[(a,next(b for b in publics if b>a)) for a in [private('create'),private('copy_words')]]
ENDPOINTS={0x46a400:6,0x485100:1,0x4f4f80:1,0x5468d0:1,0x4dea50:0}
NATIVE_NAMES={0x46a400:'restore_building_allocate_0046a400',0x485100:'FUN_00485100',0x4f4f80:'FUN_004f4f80',0x5468d0:'restore_monument_commit_footprint',0x4dea50:'FUN_004dea50'}
machines=[]
for c,pe in enumerate(images):
    u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(pe.base,(len(pe.data)+4095)&~4095);u.mem_write(pe.base,bytes(pe.data))
    u.mem_map(0x30000000,0x10000);u.mem_map(STOP,4096);u.mem_map(S,0x50000);u.mem_map(0,4096)
    state={'hooks':{},'calls':[],'collect':False};machines.append((c,u,state))
    def stores(uc,access,at,size,value,data,c=c):
        if not data['collect'] or not 0x560000<=at<0x126d000:return
        if data.get('own',False):data['writes'].append((at,size,value&((1<<(size*8))-1)))
    u.hook_add(UC_HOOK_MEM_WRITE,stores,state,begin=0x560000,end=0x126cfff)
    def coverage(uc,pc,size,data):
        if not data['collect']:return
        data['own']=True;hits.add(pc)
        if last[0] in branches:edges.add((last[0],pc))
        last[0]=pc
    if not c:
        for x in owned.values():u.hook_add(UC_HOOK_CODE,coverage,state,begin=min(x),end=max(x))
    if c:
        def own_block(uc,pc,size,data):data['own']=True
        for lo,hi in scope:u.hook_add(UC_HOOK_BLOCK,own_block,state,begin=lo,end=hi-1)
    def entry(uc,pc,size,data,t,c=c):
        if not data['collect']:return
        data['own']=False
        sp=uc.reg_read(UC_X86_REG_ESP)
        args=tuple(get(uc,sp+4+i*4) for i in range(ENDPOINTS[t]+(1 if c and t==0x5468d0 else 0)))
        if t==0x5468d0:args=(args[0],args[1]) if c else (uc.reg_read(UC_X86_REG_ECX),args[0])
        bid=args[-1] if t==0x5468d0 else args[0] if t in [0x485100,0x4f4f80] else 0
        record=bytes(uc.mem_read((0x7999d8+bid*264)&0xffffffff,264))
        data['events'].append((t,args,record))
        if t==0x485100 and data['mutate'] and not data['mutated']:
            data['mutated']=True;d=data['definition'];part=d['self']+d['offset']+52
            put(uc,part,get(uc,part)+1);put(uc,part+4,get(uc,part+4)-1);byte(uc,part+8,3);put(uc,0xeaac28,3)
        if t==0x5468d0 and data['prefix']:
            data['stopped']=True;uc.emu_stop()
    for t in ENDPOINTS:
        at=t if not c else sym(NATIVE_NAMES[t])
        def listener(uc,pc,size,data,t=t,handler=entry):handler(uc,pc,size,data,t)
        u.hook_add(UC_HOOK_CODE,listener,state,begin=at,end=at)

# Original constructor + actual geometry produce every descriptor and stream.
profiles={}
for t,d in CLASSES.items():
    c,u,state=machines[0];reset(u,state);put(u,0x604950,1);put(u,0x70e44c,271);put(u,0x70e45c,283)
    run(0,u,d['constructor'],receiver=d['self'],budget=100000000)
    run(0,u,d['geometry'],receiver=d['self'],budget=100000000)
    profiles[t]=bytes(u.mem_read(d['self'],d['offset']+d['count']*52))
    assert (0x58+46*d['count']+3)&~3==d['offset']

def fixture(u,token,root=10,skip=0,x=90,y=80,prefix=False,mutate=False):
    d=CLASSES[token]
    for reg,value in [(UC_X86_REG_FPCW,0x37f),(UC_X86_REG_FPSW,0),(UC_X86_REG_FPTAG,0xffff)]:u.reg_write(reg,value)
    for t,blob in profiles.items():u.mem_write(CLASSES[t]['self'],blob)
    u.mem_write(0x7999d8,bytes(4001*264));u.mem_write(0xba746c,bytes(0x22000))
    for a,v in [(0xbca2d8,10000),(0xbca2d0,200),(0xbca2d4,200),(0xbca2e0,0),(0xe94e04,0),(0xe94e0c,0),
                (0xeaac28,0),(0x78d790,root),(0x78d7b4,100),(0x70e120,0),(0xe38dcc,0),(0xbc9694,1)]:put(u,a,v)
    for tag in range(1000):put(u,0xf55bb4+tag*8,90000+tag*16)
    r=root*264;byte(u,0x7999d8+r,3);word(u,0x7999e8+r,d['kind']);word(u,0x7999de+r,x);word(u,0x7999e0+r,y)
    if prefix:
        n=d['count'];self=d['self']
        for i in range(n):
            put(u,self+0x58+i*4,0x87650000+i);put(u,self+0x58+n*4+i*4,0x12340000+i)
            for j in range(6):put(u,self+0x58+n*8+i*24+j*4,0x45670000+i*17+j)
            for j in range(3):put(u,self+0x58+n*32+i*12+j*4,0x789a0000+i*11+j)
            byte(u,self+0x58+n*44+i,i*7);byte(u,self+0x58+n*45+i,i*13+3)

def invoke(c,u,state,token,root,skip,x,y,mode):
    global invocations
    invocations+=1;d=CLASSES[token];args=[x,y,root,skip]
    if not c:entry=token;cleanup=16;argv=args
    elif mode=='legacy':entry=address(1,token);cleanup=0;argv=args
    elif mode=='named':entry=sym(NAMES[token]);cleanup=0;argv=[d['self'],*args]
    else:
        u.mem_write(VSP-512,b'\x6b'*576);put(u,VSP,STOP)
        for i,v in enumerate(args):put(u,VSP+4+i*4,v)
        u.mem_write(FRAME,struct.pack('<10I',0x89abcdef,d['self'],0x98765432,saved[0],VSP,*saved[1:],0x202,sym('restore_building_create_dispatch')))
        entry=sym('restore_building_create_dispatch');cleanup=0;argv=[0,token,FRAME]
    for reg,v in zip(registers,[0x89abcdef,d['self'],0x98765432,saved[0],SP,*saved[1:]]):u.reg_write(reg,v)
    u.reg_write(UC_X86_REG_EFLAGS,0x202);put(u,SP,STOP)
    for i,v in enumerate(argv):put(u,SP+4+i*4,v)
    u.emu_start(entry,STOP,count=200000000)
    if state['prefix']:assert state['stopped'];return
    assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==SP+4+cleanup
    assert [u.reg_read(registers[i]) for i in [3,5,6,7]]==saved
    if c and mode=='dispatch':
        assert get(u,FRAME+16)==VSP+20
        assert bytes(u.mem_read(VSP-512,64))==b'\x6b'*64 and bytes(u.mem_read(VSP+20,44))==b'\x6b'*44

def exercise(token,root=10,skip=0,x=90,y=80,prefix=False,mutate=False):
    global checked,prefix_checked
    expected=None
    for c,u,state in machines:
        modes=(['named'] if available(NAMES[token]) else [])+(['legacy'] if available('FUN_%08x'%token) else [])+['dispatch']
        for mode in modes if c else ['native']:
            state['collect']=False;reset(u,state);fixture(u,token,root,skip,x,y,prefix,mutate)
            state.update(events=[],writes=[],collect=True,prefix=prefix,mutate=mutate,mutated=False,stopped=False,own=False,definition=CLASSES[token]);last[0]=None
            try:invoke(c,u,state,token,root,skip,x,y,mode)
            except Exception as e:raise AssertionError(('execution',c,hex(token),root,skip,prefix,mutate,mode,hex(u.reg_read(UC_X86_REG_EIP)))) from e
            state['collect']=False;result=(tuple(state['events']),tuple(state['writes']),snapshot(u))
            if expected is None:expected=result
            elif result!=expected:
                diff=[next(((i,a,b) for i,(a,b) in enumerate(zip(x,y)) if a!=b),None) for x,y in zip(expected[:2],result[:2])]
                memory=next(((hex(0x560000+i),a,b) for i,(a,b) in enumerate(zip(expected[2],result[2])) if a!=b),None)
                raise AssertionError((hex(token),root,skip,prefix,mutate,mode,'events/stores',diff,'lengths',[(len(x),len(y)) for x,y in zip(expected[:2],result[:2])],'memory',memory))
    if prefix:prefix_checked+=1
    else:checked+=1
    print('PASS',hex(token),'prefix' if prefix else 'whole','skip',skip,'mutate',mutate,flush=True)

if __name__=='__main__':
    for token in CLASSES:
        for skip in [0,1,256]:exercise(token,skip=skip)
        exercise(token,prefix=True)
    exercise(0x559130,mutate=True)
    assert points<=hits,('unexecuted instructions',[hex(a) for a in sorted(points-hits)])
    missing={(a,b) for a,targets in branches.items() for b in targets if (a,b) not in edges}
    assert not missing,('unexecuted branches',[(hex(a),hex(b)) for a,b in sorted(missing)])
    sources=['include/monument_complex_create.h','runtime/monument_complex_create.c','runtime/building_methods.c','src/unit_02581.c',
             'CMakeLists.txt','direct_c/verify_monument_complex_create.py','metadata/monument-complex-create-native.json']+[f'src/unit_{i:05d}.c' for i in [2572,2600,2613,2627]]
    proof=dict(native_sha256=catalogue['native_sha256'],compiled_C_path=str(target.resolve()),compiled_C_sha256=target_sha,passed=True,
               checked=checked,prefix_checked=prefix_checked,invocations=invocations,coverage={hex(a):dict(instructions=len(x),executed=len(x&hits)) for a,x in owned.items()},
               conditional_edges=len(edges),controlled_algorithm_endpoints=[],controlled_platform_endpoints=[],
               source_hashes={p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest() for p in sources},
               limitations=['Mapped shipped class objects; metadata/building storage do not overlap. No concurrency/fault-equivalence or full-gameplay claim.',
                            'Actual constructors/geometry supply fixtures, and all descendant algorithms execute without substituted endpoints.',
                            'Prefix scenarios stop before the first map call to isolate high-DWORD copy values; these are not whole-method approvals.',
                            'Compare full global snapshots, child record snapshots and reconstructed-member ordered stores. Void ABI excludes scratch-stack/caller-saved register values.'],commit=None)
    suffix='diagnostic' if target.stem=='restore_link_check' else 'game'
    (ROOT/f'metadata/monument-complex-create-{suffix}.json').write_text(json.dumps(proof,indent=2)+'\n')
    print('PASS complete complex monuments:',checked,'copy prefixes:',prefix_checked,'instructions/edges:',len(points),len(edges),flush=True)
    print('Verified target SHA256:',target_sha,flush=True)
