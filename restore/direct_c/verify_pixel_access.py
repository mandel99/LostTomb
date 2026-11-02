"""Whole native/C pixel access and grey rectangles, offline in Unicorn.

Real rectangle compositions retain all three pixel helpers. Independent pixel
expectations cover RGB555/565 and native masked high bits. Controlled callbacks
separately check live format reads; they do not certify the framebuffer path.
"""
from verify_options_startup import *

SCREEN, SIZE = 0x35000000, 0x200000
PATTERN = bytes(range(256))*(SIZE//256)
for c, u, state in machines:
    u.mem_map(SCREEN, SIZE)


def setup(u, state, width=640, fmt=0x565, base=SCREEN):
    reset(u, state)
    u.mem_write(SCREEN, PATTERN)
    put(u, 0xe92e18, base)
    put(u, 0xe38d94, width*2)
    put(u, 0xe3914c, fmt)
    put(u, 0x605b70, 0xaabbccdd)


def pixel(token, x, y, pitch, mask, base=SCREEN):
    results=[]
    row=(y*pitch)&0xffffffff
    at=(base+(x*2)+row)&0xffffffff
    assert SCREEN <= at < SCREEN+SIZE-1
    for c,u,state in machines:
        setup(u,state,base=base)
        put(u,0xe38d94,pitch)
        before=bytes(u.mem_read(SCREEN,SIZE))
        old=struct.unpack_from('<H',before,at-SCREEN)[0]
        value=run(c,u,token,[x,y,mask])
        actual=bytes(u.mem_read(SCREEN,SIZE))
        expected=bytearray(before)
        if token==0x40faa0:
            expected_value=((row&0xffff0000)|old)&mask
            assert value==expected_value,('masked sample result',c,x,y,hex(mask),hex(value),hex(expected_value))
            assert get(u,0x605b70)==expected_value,('masked sample DWORD scratch',c)
        else:
            value=None  # Native incidental EAX/EDX are not consumed by callers.
            new=old&(mask&65535) if token==0x40fa70 else old|(mask&65535)
            struct.pack_into('<H',expected,at-SCREEN,new)
            assert get(u,0x605b70)==0xaabbccdd
        assert actual==expected,('pixel write or source preservation',c,hex(token))
        results.append((value,actual,get(u,0x605b70)))
    assert results[0]==results[1],('native/C pixel access',hex(token),x,y,pitch,mask)


def rectangle(width,fmt,shift,bounds=(300,260,4,3)):
    results=[]
    x,y,w,h=bounds
    for c,u,state in machines:
        setup(u,state,width,fmt)
        before=bytes(u.mem_read(SCREEN,SIZE))
        run(c,u,0x4cf090,[x,y,w,h,shift])
        actual=bytes(u.mem_read(SCREEN,SIZE))
        expected=bytearray(before)
        for yy in range(y,y+h):
            for xx in range(x,x+w):
                at=yy*width*2+xx*2
                old=struct.unpack_from('<H',before,at)[0]
                red=(old>>11)&31 if fmt==0x565 else (old>>10)&31
                green=(old>>6)&31 if fmt==0x565 else (old>>5)&31
                grey=((red+green+(old&31))//3)>>(shift&31)
                value=(grey<<11)|(grey<<6)|grey if fmt==0x565 else (grey<<10)|(grey<<5)|grey
                struct.pack_into('<H',expected,at,value)
        assert actual==expected,('independent grey rectangle',c,width,hex(fmt),shift,bounds)
        results.append((actual,get(u,0x605b70),get(u,0xe3914c)))
    assert results[0]==results[1],('native/C whole grey rectangle',width,fmt,shift,bounds)


def format_reload(first,second,shift):
    results=[]
    for c,u,state in machines:
        setup(u,state,fmt=first)
        def read(uc,args):
            state['calls'].append(('read',args))
            put(uc,0xe3914c,second)
            uc.reg_write(UC_X86_REG_ECX,0xdeadbeef)
            uc.reg_write(UC_X86_REG_EDX,0xbaadf00d)
            return args[2]&0x6b5a
        def clear(uc,args):
            state['calls'].append(('clear',args))
            put(uc,0xe3914c,second)
            return 0
        hook(c,state,0x40faa0,3,read)
        hook(c,state,0x40fa70,3,clear)
        hook(c,state,0x40fae0,3,lambda uc,args:state['calls'].append(('or',args)) or 0)
        run(c,u,0x4cf090,[300,260,2,2,shift])
        results.append((tuple(state['calls']),get(u,0xe3914c)))
    assert results[0]==results[1],('native format reload order',first,second,shift)


if __name__=='__main__':
    count=0
    for token,(x,y,pitch,base),mask in itertools.product(
            [0x40fa70,0x40faa0,0x40fae0],
            [(300,260,1280,SCREEN),(-3,260,1600,SCREEN+128),
             (300,-1,2048,SCREEN+8192),(0x80000003,0x80000000,1280,SCREEN)],
            [0,0xf863,0x1f,0xffffffff]):
        pixel(token,x,y,pitch,mask,base);count+=1
    print('PASS complete pixel AND/sample/OR, full pointers and native row high bits:',count,flush=True)
    count=0
    for width,fmt,shift in itertools.product([640,800,1024],[0x555,0x565,0x10000565],[0,1,5,32,255]):
        rectangle(width,fmt,shift);count+=1
    for bounds in [(300,260,0,3),(300,260,3,0),(300,260,-1,2),(300,260,2,-1),(179,222,25,25)]:
        rectangle(640,0x565,1,bounds);count+=1
    print('PASS whole grey rectangle with real pixel helpers and independent expected pixels:',count,flush=True)
    for first,second,shift in [(0x565,0x555,0),(0x555,0x565,1),(0x10000565,0x565,33)]:
        format_reload(first,second,shift)
    print('PASS controlled format reload and callback order: 3',flush=True)
    print('Verified target SHA256:',target_sha)
