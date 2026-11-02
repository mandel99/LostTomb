"""Whole native/C5208F0 overlay menu layout, hover and submenu rendering.

Panel/text/sprite endpoints are controlled; ordered arguments and live callback
reloads are compared, not final framebuffer pixels or in-game overlay maps.
"""
from verify_options_startup import *

def exercise(width,compact,selection,submenu,mutate=False):
    results=[]
    for c,u,state in machines:
        reset(u,state)
        for at,value in [(0xf53f64,width),(0xf53f60,width-80),(0xf53f78,compact),
                (0x7056c8,selection),(0x7056c4,2),(0x7056dc,submenu),
                (0x7056d8,3 if submenu else 0),(0x7056d0,5),(0x7056e8,300),
                (0xe38da8,0x120),(0xe38da4,0x210),(0xf56f8c,0x12345678),
                (0xf5690c,0x1234)]:put(u,at,value)
        calls=[]
        def observe(cpu,args,token):
            if token==0x53e590 and not c:assert cpu.reg_read(UC_X86_REG_ECX)==0xf55bb0
            calls.append((token,args))
            if mutate:
                put(cpu,0xf53f64,2000);put(cpu,0xf53f60,1800)
                put(cpu,0xe38da8,0x220);put(cpu,0xe38da4,0x310)
                put(cpu,0xf56f8c,0x33445566)
                put(cpu,0x7056c8,3);put(cpu,0x7056c4,1)
                if len(calls)==20:
                    put(cpu,0x7056dc,2);put(cpu,0x7056d8,2)
                if token==0x53e590:put(cpu,0x7056d8,4)
            cpu.reg_write(UC_X86_REG_ECX,0xdeadbeef);cpu.reg_write(UC_X86_REG_EDX,0xbaadf00d)
            return 0
        for token,n in [(0x4d6480,8),(0x4d6250,9),(0x53e590,3)]:
            hook(c,state,token,n,lambda cpu,args,t=token:observe(cpu,args,t),
                 cleanup=12 if not c and token==0x53e590 else 0)
        run(c,u,0x5208f0)
        results.append((tuple(calls),bytes(u.mem_read(0x7056c0,48))))
    assert results[0]==results[1],('whole overlay menu',width,compact,selection,submenu,mutate,
        next(((i,a,b) for i,(a,b) in enumerate(zip(results[0][0],results[1][0])) if a!=b),None))

if __name__=='__main__':
    count=0
    for width,compact,selection,submenu in itertools.product([640,800,1024],[0,1],[0,1,5,10],[0,1,3]):
        exercise(width,compact,selection,submenu);count+=1
    for width in [640,800,1024]:
        exercise(width,0,1,0,mutate=True);count+=1
    print('PASS whole overlay menu: full layout/fonts, both sidebars, main/submenu hover and live callbacks:',count,flush=True)
    print('Verified target SHA256:',target_sha)
