"""Native/C position queries: complete EAX, ordered writes and actual frame routes."""
from verify_options_startup import *
from unicorn import UC_HOOK_MEM_WRITE
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import json

SELF,OUT,FRAME,VSP=S+0x1000,S+0x3000,S+0x8000,S+0x18000
BID=64
TABLES={0x545a30:0x570160,0x558f00:0x570268,0x556230:0x570228,0x54f8a0:0x5701e8}
NAMES={t:'restore_complex_position_%08x'%t for t in TABLES}
available=lambda n:bool(re.search(r'\s_'+re.escape(n)+r'\s',symbols))
catalogue=json.loads((ROOT/'metadata/monument-complex-positions-native.json').read_text())
assert catalogue['native_sha256']==hashlib.sha256(original.read_bytes()).hexdigest()
owned={};branches={};jumps={};expected_jumps=set();hits=set();edges=set();jump_edges=set();last=[None];checked=0;invocations=0
decoder=Cs(CS_ARCH_X86,CS_MODE_32)
for f in catalogue['functions']:
    a=int(f['address'],16);raw=bytes.fromhex(f['bytes_hex'])
    assert raw==bytes(images[0].data[a-images[0].base:a-images[0].base+len(raw)])
    owned[a]={int(s,16) for s in f['instruction_addresses'].split()}
    table=f['table'];t=int(table['address'],16);b=bytes.fromhex(table['bytes_hex'])
    assert b==bytes(images[0].data[t-images[0].base:t-images[0].base+len(b)])
    targets=set(struct.unpack('<%dI'%table['count'],b))
    for pc in owned[a]:
        ins=next(decoder.disasm(raw[pc-a:pc-a+16],pc,count=1))
        if ins.mnemonic.startswith('j') and ins.mnemonic!='jmp':branches[pc]={int(ins.op_str,16),pc+ins.size}
        if ins.mnemonic=='jmp' and 'ptr [' in ins.op_str:jumps[pc]=targets;expected_jumps.update((pc,x) for x in targets)
    assert struct.unpack_from('<I',images[0].data,TABLES[a]+32-images[0].base)[0]==a
points=set.union(*owned.values())
machines=[]
for c,pe in enumerate(images):
    u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(pe.base,(len(pe.data)+4095)&~4095);u.mem_write(pe.base,bytes(pe.data))
    u.mem_map(0x30000000,0x10000);u.mem_map(STOP,4096);u.mem_map(S,0x50000)
    # Direct-entry emulation bypasses startup's global-data initialization.
    u.mem_write(0x560000,seed)
    state={'hooks':{},'calls':[],'writes':[]};machines.append((c,u,state))
    def stores(uc,access,at,size,value,data):data['writes'].append((at,size,value&((1<<(size*8))-1)))
    u.hook_add(UC_HOOK_MEM_WRITE,stores,state,begin=0x560000,end=0x126cfff)
    u.hook_add(UC_HOOK_MEM_WRITE,stores,state,begin=S,end=S+0x3fff)
    if not c:
        def observe(uc,pc,size,data):
            hits.add(pc)
            if last[0] in branches:edges.add((last[0],pc))
            if last[0] in jumps:jump_edges.add((last[0],pc))
            last[0]=pc
        for rows in owned.values():u.hook_add(UC_HOOK_CODE,observe,begin=min(rows),end=max(rows))

def fixture(u,token,piece,x,y,building):
    u.mem_write(SELF,b'\xa5'*0x400);put(u,SELF,TABLES[token]);u.mem_write(OUT-8,b'\x6b'*0x300)
    r=(building*264)&0xffffffff;u.mem_write(0x7999d8+r,b'\xa5'*264)
    word(u,0x7999de+r,x);word(u,0x7999e0+r,y);word(u,0x7999ea+r,piece)

def invoke(c,u,token,building,out_x,out_y,mode,ecx):
    global invocations
    invocations+=1;argv=[building,out_x,out_y];cleanup=0;frame=False
    if not c:entry=token;cleanup=12
    elif mode=='legacy':entry=address(1,token)
    elif mode=='named':entry=sym(NAMES[token])
    elif mode=='common':entry=sym('restore_complex_position_query');argv=[token,*argv]
    elif mode=='facade':entry=sym('restore_placement_position_query');argv=[SELF,*argv]
    else:
        frame=True;u.mem_write(VSP-256,b'\x6b'*320);put(u,VSP,STOP)
        for i,v in enumerate(argv):put(u,VSP+4+i*4,v)
        u.mem_write(FRAME,struct.pack('<10I',0x89abcdef,ecx,0x98765432,saved[0],VSP,*saved[1:],0x202,sym('restore_building_create_dispatch')))
        entry=sym({'placement':'restore_placement_queries_dispatch','building':'restore_building_create_dispatch',
                   'civic':'restore_actor_civic_dispatch','city':'restore_city_dispatch'}[mode]);argv=[0,token,FRAME]
    for reg,v in zip(registers,[0x89abcdef,ecx,0x98765432,saved[0],SP,*saved[1:]]):u.reg_write(reg,v)
    u.reg_write(UC_X86_REG_EFLAGS,0x202);put(u,SP,STOP)
    for i,v in enumerate(argv):put(u,SP+4+i*4,v)
    u.emu_start(entry,STOP,count=100000)
    assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==SP+4+cleanup
    assert [u.reg_read(registers[i]) for i in [3,5,6,7]]==saved
    if frame:
        assert get(u,FRAME+16)==VSP+16 and [get(u,FRAME+i*4) for i in [3,5,6,7]]==saved
        assert bytes(u.mem_read(VSP-256,64))==b'\x6b'*64 and bytes(u.mem_read(VSP+16,48))==b'\x6b'*48
        return get(u,FRAME)
    return u.reg_read(UC_X86_REG_EAX)

def exercise(token,piece=0,x=50,y=60,outputs=None,building=BID,ecx=0x8e47b123):
    global checked
    if outputs is None:outputs=(OUT,OUT+8)
    expected=None
    for c,u,state in machines:
        modes=['common','placement','building','civic','city']
        if available('restore_placement_position_query'):modes.append('facade')
        if available(NAMES[token]):modes.append('named')
        if available('FUN_%08x'%token):modes.append('legacy')
        for mode in modes if c else ['native']:
            fixture(u,token,piece,x,y,building);state['writes']=[];last[0]=None
            try:value=invoke(c,u,token,building,*outputs,mode,ecx)
            except Exception as e:raise AssertionError(('execution',c,hex(token),piece,x,y,tuple(hex(a) for a in outputs),hex(building),mode,hex(u.reg_read(UC_X86_REG_EIP)))) from e
            result=(value,tuple(state['writes']))
            if expected is None:expected=result
            else:assert result==expected,(hex(token),piece,x,y,tuple(hex(a) for a in outputs),hex(building),mode,'native',expected,'C',result)
    checked+=1

if __name__=='__main__':
    for f in catalogue['functions']:
        token=int(f['address'],16);count=f['table']['count']
        for piece,x,y in itertools.product(range(count),[-32768,-1,0,255,32767],[-32768,-1,0,255,32767]):exercise(token,piece,x,y)
        for piece in list(range(count))+[-1,-32768,count,count+1,32767,65535,65536,65537,0x7fffffff,0x80000000]:
            r=BID*264
            for pair in [(OUT,OUT),(OUT+1,OUT+3),(OUT+0xf1,OUT+0x201),
                         (0x7999de+r,OUT),(0x7999e0+r,OUT),(0x7999e0+r-1,OUT),
                         (0x7999ea+r,OUT),(OUT,0x7999de+r),(OUT,0x7999e0+r),
                         (0xbca2e0,OUT),(SELF,SELF+5)]:
                exercise(token,piece=piece,x=-257,y=511,outputs=pair)
        if token!=0x545a30:
            for piece in [-1,count,32767]:exercise(token,piece=piece,outputs=(0,0))
        for building,piece,ecx in itertools.product([BID+0x20000000,BID+0x80000000],[0,count,-1],[0,0xffffffff]):exercise(token,piece=piece,building=building,ecx=ecx)
    assert points<=hits,('unexecuted instructions',[hex(a) for a in sorted(points-hits)])
    missing={(a,b) for a,targets in branches.items() for b in targets if (a,b) not in edges}
    assert not missing,('unexecuted conditional edges',[(hex(a),hex(b)) for a,b in sorted(missing)])
    assert jump_edges==expected_jumps,('missing switch edges',expected_jumps-jump_edges)
    sources=['include/monument_complex_positions.h','runtime/monument_complex_positions.c','runtime/placement_queries.c','CMakeLists.txt',
             'direct_c/verify_monument_complex_positions.py','metadata/monument-complex-positions-native.json',
             'runtime/building_create.c','runtime/actor_civic.c','runtime/city_updates.c']+[f'src/unit_{i:05d}.c' for i in [2569,2623,2610,2597]]
    proof=dict(native_sha256=catalogue['native_sha256'],compiled_C_path=str(target.resolve()),compiled_C_sha256=target_sha,passed=True,
               checked=checked,invocations=invocations,position_facade_compared=available('restore_placement_position_query'),coverage={hex(a):dict(instructions=len(x),executed=len(x&hits)) for a,x in owned.items()},
               conditional_edges=len(edges),indirect_jump_edges=len(jump_edges),controlled_algorithm_endpoints=[],controlled_platform_endpoints=[],
               source_hashes={p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest() for p in sources},
               limitations=['Mapped writable output/record addresses, including unaligned overlaps; no concurrency or ABI-argument-frame corruption.',
                            'Full EAX, exact ordered output/global writes, callee-saved registers and frame balance are compared.',
                            'Placement/building/civic/city dispatchers execute; the position facade is compared when retained by the linker. No substituted providers or full-gameplay approval.'],commit=None)
    suffix='diagnostic' if target.stem=='restore_link_check' else 'game'
    (ROOT/f'metadata/monument-complex-positions-{suffix}.json').write_text(json.dumps(proof,indent=2)+'\n')
    print('PASS four complex positions:',checked,'scenarios',invocations,'invocations',flush=True)
    print('Native instructions/conditional/switch edges:',len(points),len(edges),len(jump_edges),flush=True)
    print('Verified target SHA256:',target_sha,flush=True)
