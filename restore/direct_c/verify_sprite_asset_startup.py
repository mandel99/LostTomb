"""Whole 414420 orchestration with controlled loader/filesystem/render backends.

Tests initialization placement, allocation arithmetic, call order and state
preservation. It does not claim to execute the controlled backends themselves.
"""
from verify_options_startup import *
import json
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
BASE,SIZE=0xf55bb0,0x30f5d8
code=list(Cs(CS_ARCH_X86,CS_MODE_32).disasm(bytes(images[0].data[0x414420-images[0].base:0x414727-images[0].base]),0x414420))
assert sum(i.size for i in code)==775
owned={i.address for i in code};hits=set();edges=set();previous=[None]
branches={i.address:{int(i.op_str,16),i.address+i.size} for i in code if i.mnemonic.startswith('j') and i.mnemonic!='jmp'}
def visit(cpu,pc,size,data):
    if pc in owned:hits.add(pc)
    if previous[0] in branches:edges.add((previous[0],pc))
    previous[0]=pc
machines[0][1].hook_add(UC_HOOK_CODE,visit)
NAMES=[0x579218,0x579210,0x579208,0x5791f8,0x5791e8,0x5791dc,0x5791d0,0x5791c4,
       0x5791b8,0x5791a8,0x57919c,0x579194,0x579188,0x579178,0x57916c,0x579160,
       0x579154,0x579144,0x579134,0x579124,0x57922c,0x57922c,0x57922c,0x576284,
       0x579110,0x579100,0x5790f0,0x5790e0,0x5790d0,0x5790c0]

def exercise(width,height,allocated,mutation):
    expected=None
    for c,u,state in machines:
        reset(u,state);u.mem_write(BASE,b'\xa5'*SIZE)
        put(u,0xe38db0,width);put(u,0xe38dac,height);put(u,0xe20768,0x12345678)
        state['loads']=0
        def mapping(cpu,args):
            assert args==(0x57924c,0xbca2b8,0)
            state['calls'].append(('mapping',args));put(cpu,0xbca2b8,0x17181920);return 1
        hook(c,state,0x4ceb60,3,mapping)
        def directory(cpu,args):
            state['calls'].append(('directory',args));return 1
        slot=sym('_imp__CreateDirectoryA@8') if c else 0x56f078
        put(u,slot,S+0x49000);state['hooks'][S+0x49000]=(2,8,directory)
        def allocate(cpu,args):
            state['calls'].append(('allocate',args))
            product=(width*height*2)&0xffffffff
            wanted=product if product<0x80000000 and product>1200000 else 1200000
            assert args==(wanted,),('signed allocation bound',width,height,args,wanted)
            return S+0x10000 if allocated else 0
        hook(c,state,0x561b68,1,allocate)
        for token in [0x53dd10,0x53dd40]:
            def load(cpu,args,token=token):
                obj,names=args if c else (cpu.reg_read(UC_X86_REG_ECX),args[0])
                assert obj==BASE
                table=tuple(get(cpu,names+i*4) for i in range(30))
                if state['loads']==0:assert table==tuple(NAMES)
                digest=hashlib.sha256(cpu.mem_read(BASE,SIZE)).hexdigest()
                state['calls'].append(('load',token,table,digest))
                state['loads']+=1
                put(cpu,BASE+0x30f5c8,state['loads']);put(cpu,BASE+0x30f5d4,state['loads'])
                if mutation:put(cpu,names+12,0x57922c)
                return 0x12345600|state['loads']
            hook(c,state,token,2 if c else 1,load,0 if c else 4)
        endpoints={0x520b20:1,0x4cc110:2,0x4cf7b0:1,0x4d0280:1,0x520af0:0,0x4ce6e0:3}
        for token,nargs in endpoints.items():
            def backend(cpu,args,token=token):
                state['calls'].append((token,args))
                if token==0x4cf7b0:
                    put(cpu,0xe20768,get(cpu,0xe20768)+7)
                    if state['loads']==1 and mutation==2:put(cpu,0xe92e1c,0)
                    if state['loads']==1 and mutation==3:put(cpu,0xe92e1c,S+0x10000)
                return 0xabcdef01
            hook(c,state,token,nargs,backend)
        result=run(c,u,0x414420)
        # Do not compare the fixture's import-address substitution itself.
        if not c:put(u,slot,struct.unpack_from('<I',seed,slot-0x560000)[0])
        current=(result,tuple(state['calls']),snapshot(u))
        if expected is None:expected=current
        else:
            assert current[:2]==expected[:2],('asset call order/state',width,height,allocated,mutation,current[:2],expected[:2])
            assert current[2]==expected[2],('asset memory',width,height,allocated,mutation,
                next((hex(i+0x560000),a,b) for i,(a,b) in enumerate(zip(current[2],expected[2])) if a!=b))

if __name__=='__main__':
    count=0
    for width,height in [(640,480),(1024,768),(1,1),(0,0),(0x7fffffff,2),(0xffffffff,0xffffffff),(0x80000000,1),(100000,100000)]:
        for allocated in [False,True]:exercise(width,height,allocated,0);count+=1
    for allocated,mutation in itertools.product([False,True],[1,2,3]):
        exercise(1024,768,allocated,mutation);count+=1
    assert hits==owned,('native asset instructions',sorted(owned-hits))
    assert all((a,b) in edges for a,targets in branches.items() for b in targets),('native asset branches',edges)
    proof=dict(native_sha256=hashlib.sha256(original.read_bytes()).hexdigest(),compiled_C_path=str(target.resolve()),
        compiled_C_sha256=target_sha,passed=True,checked=count,
        coverage=dict(bytes=775,instructions=len(owned),executed=len(hits),conditional_edges=sum(map(len,branches.values()))),
        source_hashes={p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest() for p in ['src/unit_00179.c','direct_c/verify_sprite_asset_startup.py']},
        scope=['Whole414420 allocation/orchestration, all30 library names, persistent manager state and callback reloads'],
        controlled_algorithm_endpoints=['53DD10/53DD40 library loaders','520B20/4CC110/4CF7B0/4D0280/520AF0 graphics/progress helpers'],
        controlled_platform_endpoints=['4CEB60 mapping read','561B68 allocation','CreateDirectoryA','4CE6E0 status output'],
        limitations=['Backends are controlled; this does not certify their implementations, actual assets or rendered output.'],commit=None)
    suffix='diagnostic' if target.stem=='restore_link_check' else 'game'
    (ROOT/f'metadata/sprite-asset-startup-{suffix}.json').write_text(json.dumps(proof,indent=2)+'\n')
    print('PASS whole asset startup orchestration and retained manager state:',count,flush=True)
    print('Verified target SHA256:',target_sha,flush=True)
