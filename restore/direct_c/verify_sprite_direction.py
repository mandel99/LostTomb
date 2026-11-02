"""Whole55D9C0 direction selection, no substituted callees or host game.

Preserve native private-stack padding as an explicit input. Compare full EAX,
all output bytes, saved registers, cleanup and inherited x87 CW/TOP/tags.
Mirrored inputs are finite and bounded: native normalization loops on NaN/Inf.
FPSW/trap equivalence is not claimed. Real native and C FTOL bodies execute.
"""
from verify_options_startup import *

OUT,ACTION=S+0x1000,S+0x2000
for c,u,state in machines:
    reset(u,state)
    u.mem_write(S+0x4800,b'\xd9\xe8'*2)
baseline=[snapshot(u) for c,u,state in machines]

def direction_case(count,flags,angle,cw=0x37f,padding=0xa5b6,depth=0,alias=0):
    results=[];argument_states=[]
    for c,u,state in machines:
        for explicit in ([False,True] if c else [False]):
            output=ACTION+alias if alias else OUT
            u.mem_write(ACTION,b'\x5a'*64);u.mem_write(OUT,b'\x6b'*32)
            byte(u,ACTION+1,flags);byte(u,ACTION+11,count)
            u.mem_write(SP-0x1000,b'\xcd'*0x1000);word(u,SP-6,padding)
            for reg,value in [(UC_X86_REG_FPCW,cw),(UC_X86_REG_FPSW,0),(UC_X86_REG_FPTAG,0xffff)]:u.reg_write(reg,value)
            if depth:u.emu_start(S+0x4800,S+0x4800+depth*2)
            top=(u.reg_read(UC_X86_REG_FPSW)>>11)&7
            tags=u.reg_read(UC_X86_REG_FPTAG)
            guards=[u.reg_read(UC_X86_REG_FP0+i) for i in range(8)]
            args=[output,ACTION,angle]
            if explicit:
                value=run(0,u,sym('restore_sprite_direction_from_angle'),[*args,padding,0])
            else:value=run(c,u,0x55d9c0,args)
            if not explicit:argument_states.append(bytes(u.mem_read(SP+4,12)))
            assert value==output,('returned pointer',c,explicit,hex(value),hex(output))
            assert u.reg_read(UC_X86_REG_FPCW)==cw
            assert ((u.reg_read(UC_X86_REG_FPSW)>>11)&7)==top
            assert u.reg_read(UC_X86_REG_FPTAG)==tags
            if depth:
                # FLD1 guards occupy the two live physical stack registers.
                now=[u.reg_read(UC_X86_REG_FP0+i) for i in range(8)]
                for i in range(8):
                    if ((tags>>(i*2))&3)!=3:assert now[i]==guards[i],('FP guard',i)
            observed=(value,bytes(u.mem_read(ACTION,64)),bytes(u.mem_read(OUT,32)))
            if explicit:assert observed==results[-1],('explicit API',count,flags,hex(angle),hex(cw),padding,depth,alias)
            else:results.append(observed)
    assert results[0]==results[1],('direction',count,flags,hex(angle),hex(cw),padding,depth,alias,results)
    assert argument_states[0]==argument_states[1],('native by-value argument writes',count,flags,hex(angle),argument_states)

def bits(f):return struct.unpack('<I',struct.pack('<f',f))[0]

if __name__=='__main__':
    n=0
    # Every BYTE count, both action kinds, normal angles and repeated wrapping.
    for count,flags,angle in itertools.product(range(256),[0,0x80],[-1080,-1,0,45,135,180,270,359.99,360,1440]):
        direction_case(count,flags,bits(angle));n+=1
    print('PASS all256 direction counts and both action flags:',n,flush=True)
    # Adjacent binary32 inputs around every sector edge for representative
    # counts. These expose the original extended comparison and float spills.
    for count,flags in itertools.product([2,3,5,8,9,16,127,255],[0,0x80]):
        steps=count*2-2 if flags else count
        offset=360//(steps*2)
        origin=495 if flags else 135
        for edge in range(steps+1):
            center=bits(origin+offset-edge*(360/steps))
            adjacent=[0x80000001,0,1] if center==0 else [center-1,center,center+1]
            for encoded in adjacent:
                direction_case(count,flags,encoded&0xffffffff);n+=1
    print('PASS neighboring binary32 sector boundaries:',n,flush=True)
    for cw,count,flags,angle in itertools.product(
            [0x7f,0x27f,0x37f,0x47f,0x67f,0x77f,0x87f,0xa7f,0xb7f,0xc7f,0xe7f,0xf7f],
            [2,3,8,255],[0,0x80],[.00001,44.99999,179.99999,270.00003]):
        direction_case(count,flags,bits(angle),cw=cw);n+=1
    for count,flags,padding,alias in itertools.product([0,1,3,8],[0,0x80],[0,0xffff,0x1234],[0,1,8]):
        direction_case(count,flags,bits(179.5),padding=padding,depth=2,alias=alias);n+=1
    # Native one-direction and unmirrored paths do terminate on these values.
    for count,flags,encoded in itertools.product([0,1,2,8],[0,0x80],[0x7fc12345,0x7f800000,0xff800000]):
        if count>1 and flags:continue
        direction_case(count,flags,encoded);n+=1
    for i,(c,u,state) in enumerate(machines):assert snapshot(u)==baseline[i],('global memory mutation',c)
    print('PASS whole55D9C0/public and explicit C API, edge, CW, padding, alias, FP guards:',n,flush=True)
    print('Verified target SHA256:',target_sha,flush=True)
