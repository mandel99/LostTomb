"""Original4E61F0 clock/key prefix and C integration with null preview objects.

Only timeGetTime/GetAsyncKeyState are controlled. Preserve full DWORD state,
unsigned clock wrap, key masks, binary32 stores and x87 comparisons. Optional
object animation/render dispatch after4E62B8 is outside this proof.
"""
from verify_options_startup import *
import json
from capstone import Cs,CS_ARCH_X86,CS_MODE_32

code=list(Cs(CS_ARCH_X86,CS_MODE_32).disasm(bytes(images[0].data[0x4e61f0-images[0].base:0x4e62b8-images[0].base]),0x4e61f0))
assert sum(i.size for i in code)==200
owned={i.address for i in code};hits=set();edges=set();previous=[None]
branches={i.address:{int(i.op_str,16),i.address+i.size} for i in code if i.mnemonic.startswith('j')}
def visit(cpu,pc,size,data):
    if pc in owned:hits.add(pc)
    if previous[0] in branches:edges.add((previous[0],pc))
    previous[0]=pc
machines[0][1].hook_add(UC_HOOK_CODE,visit)

def clock_fields(u):
    return bytes(u.mem_read(0x6044ac,12)),bytes(u.mem_read(0x12651a0,12))

def exercise(enabled=1,toggle=0,rate=0x41f00000,keys=(0,0,0),before=0xabcdef00,now=0xabcdef01,cw=0x37f,mutation=0,tick=False):
    expected=None
    for c,u,state in machines:
        for named in ([False,True] if c else [False]):
            reset(u,state);state['calls']=[]
            for a,v in [(0x6044ac,rate),(0x6044b0,toggle),(0x6044b4,enabled),(0x12651a0,0x13579bdf),
                        (0x12651a4,0x2468ace0),(0x12651a8,before),(0xea136c,0),(0xea1370,0)]:put(u,a,v)
            actor,descriptor,variant=S+0x8000,S+0x9000,S+0xa000
            if tick:
                u.mem_write(actor,b'\xa5'*0x80)
                for a,v in [(actor+0x40,descriptor),(actor+0x44,variant),(actor+0x48,0x3f000000),
                            (actor+0x4c,0x43960000),(actor+0x50,0x41a00000)]:put(u,a,v)
                byte(u,descriptor,4);byte(u,descriptor+3,3);word(u,variant+2,7);word(u,actor+0x78,1)
            def clock(cpu,args):
                assert not args
                state['calls'].append(('time',clock_fields(cpu)))
                if mutation==1:put(cpu,0x12651a8,0x11223344);put(cpu,0x6044b4,0x100)
                return now
            def keyboard(cpu,args):
                key,=args;assert key in [0x71,0x74,0x75]
                state['calls'].append(('key',key,clock_fields(cpu)))
                if mutation==2 and key==0x71:put(cpu,0x6044b0,0x100);put(cpu,0x6044ac,0x42a00000)
                if mutation==3 and key==0x74:put(cpu,0x6044b4,0);put(cpu,0x6044ac,0x7fc12345)
                if mutation==4 and key==0x75:put(cpu,0x12651a8,0x55667788)
                return keys[[0x71,0x74,0x75].index(key)]
            for token,name,n,cleanup,fn,stub in [(0x56f22c,'_imp__timeGetTime@0',0,0,clock,S+0x49000),
                                               (0x56f1e4,'_imp__GetAsyncKeyState@4',1,4,keyboard,S+0x49100)]:
                put(u,sym(name) if c else token,stub);state['hooks'][stub]=(n,cleanup,fn)
            for reg,value in [(UC_X86_REG_FPCW,cw),(UC_X86_REG_FPSW,0),(UC_X86_REG_FPTAG,0xffff)]:u.reg_write(reg,value)
            u.mem_write(S+0x4800,b'\xd9\xe8'*2);u.emu_start(S+0x4800,S+0x4804)
            tags=u.reg_read(UC_X86_REG_FPTAG);guards=[u.reg_read(UC_X86_REG_FP0+i) for i in range(8)]
            run(0,u,sym('restore_sprite_frame_clock_update')) if named else run(c,u,0x4e61f0)
            assert u.reg_read(UC_X86_REG_FPCW)==cw
            assert u.reg_read(UC_X86_REG_FPTAG)==tags
            for i in range(8):
                if ((tags>>(i*2))&3)!=3:assert u.reg_read(UC_X86_REG_FP0+i)==guards[i],('inherited x87',i)
            clock_status=u.reg_read(UC_X86_REG_FPSW)
            advanced=None
            if tick:
                value=run(0,u,sym('restore_sprite_cycle_tick'),[actor]) if c else run(0,u,0x55d4f0,receiver=actor)
                advanced=(value,bytes(u.mem_read(actor,0x80)))
            if not c:
                for slot in [0x56f22c,0x56f1e4]:put(u,slot,struct.unpack_from('<I',seed,slot-0x560000)[0])
            current=(tuple(state['calls']),clock_status,snapshot(u),advanced)
            if expected is None:expected=current
            else:
                assert current[:2]==expected[:2],('clock callbacks/x87',enabled,toggle,hex(rate),keys,hex(cw),mutation,named,current[:2],expected[:2])
                assert current[2]==expected[2],('clock memory',enabled,toggle,hex(rate),keys,hex(cw),mutation,named,
                    next((hex(i+0x560000),a,b) for i,(a,b) in enumerate(zip(current[2],expected[2])) if a!=b))
                assert current[3]==expected[3],('clock to real animation tick',before,now,named)

if __name__=='__main__':
    count=0
    for enabled,toggle,pressed in itertools.product([0,1,0x100,0x80000000],[0,0x100],range(8)):
        exercise(enabled=enabled,toggle=toggle,keys=tuple(0x8001 if pressed&(1<<i) else 0 for i in range(3)));count+=1
    for before,delta in itertools.product([0,0x12345678,0xffffff00],[0,255,256,1000,0x80000000,0xffffffff]):
        exercise(enabled=0,before=before,now=(before+delta)&0xffffffff);count+=1
    for keymask in [1,0x8000,0x10000,0xffff,0xffff8001]:
        exercise(keys=(keymask,keymask,keymask));count+=1
    for cw in [0x7f,0x27f,0x37f,0x47f,0x67f,0x77f,0x87f,0xa7f,0xb7f,0xc7f,0xe7f,0xf7f]:
        for rate,keys in [(0x41200001,(0,0x8001,0)),(0x411fffff,(0,0x8001,0)),
                          (0x429fffff,(0,0,0x8001)),(0x42a00001,(0,0,0x8001))]:
            exercise(rate=rate,keys=keys,cw=cw);count+=1
    for rate,keys in itertools.product([0x7fc12345,0x7f800000,0xff800000,0x80000000],[(0,0x8001,0),(0,0,0x8001),(0,0x8001,0x8001)]):
        exercise(rate=rate,keys=keys);count+=1
    for mutation,keys in itertools.product(range(1,5),[(0,0,0),(0x8001,0x8001,0x8001)]):
        exercise(enabled=0 if mutation==1 else 1,mutation=mutation,keys=keys);count+=1
    for delta in [0,255,256,1000,0x80000000]:
        exercise(enabled=0,toggle=1,before=0xffffff00,now=(0xffffff00+delta)&0xffffffff,tick=True);count+=1
    assert hits==owned,('unexecuted clock prefix',sorted(owned-hits))
    assert all((a,b) in edges for a,targets in branches.items() for b in targets),('clock branch coverage',edges)
    sources=['runtime/sprite_frame_clock.c','include/sprite_frame_clock.h','src/unit_01989.c','CMakeLists.txt','direct_c/verify_sprite_frame_clock.py']
    proof=dict(native_sha256=hashlib.sha256(original.read_bytes()).hexdigest(),compiled_C_path=str(target.resolve()),
        compiled_C_sha256=target_sha,passed=True,checked=count,
        source_hashes={p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest() for p in sources},
        coverage=dict(start='0x4e61f0',end_inclusive='0x4e62b7',bytes=200,instructions=len(owned),executed=len(hits),conditional_edges=sum(map(len,branches.values()))),
        controlled_algorithm_endpoints=[],controlled_platform_endpoints=['timeGetTime','GetAsyncKeyState'],
        scope=['Actual compiled4E61F0 integration and named C clock/key helper, full DWORD writes and callback reloads',
               'Unsigned timer wrap, key masks, full-width flags, x87 PC/RC, NaN/Inf and inherited x87 values/status',
               'Five clock-to-real-animation-tick compositions, including DWORD wrap and unsigned-high elapsed time'],
        limitations=['Optional preview-object pointers are null; dispatch after4E62B8 is not reconstructed or approved by this proof.',
                     'Masked FP exceptions; no live game, keyboard state or multimedia timer is exercised.',
                     'Exact FPSW is compared for the clock/key prefix, not for the later timing-method composition.'],commit=None)
    suffix='diagnostic' if target.stem=='restore_link_check' else 'game'
    (ROOT/f'metadata/sprite-frame-clock-{suffix}.json').write_text(json.dumps(proof,indent=2)+'\n')
    print('PASS native/C sprite frame-clock prefix and actual entry integration:',count,flush=True)
    print('Native prefix instructions/conditional edges:',len(hits),sum(map(len,branches.values())),flush=True)
    print('Verified target SHA256:',target_sha,flush=True)
