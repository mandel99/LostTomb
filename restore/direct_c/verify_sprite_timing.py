"""Whole animation timing/finished/stop/hold methods and their real callees.

No endpoint substitutions. The original FTOL and compiled explicit raw80
conversion both execute. Positive frame counts and masked x87 exceptions;
FPSW/trap equivalence and the action/direction controller are outside scope.
"""
from verify_sprite_cycle import *

TIMING={0x55d4f0:'tick',0x55d760:'finished',0x55d780:'stop_when_finished',0x55d7b0:'hold_last_frame'}

def prepare_timing(u,state,frame,rate,action,timed,elapsed,fps,length,remaining,cw,depth):
    reset(u,state)
    u.mem_write(SELF,b'\xa5'*0x80)
    put(u,SELF+0x34,0xa5a5c0c0);put(u,SELF+0x40,DESC);put(u,SELF+0x44,VARIANTS)
    put(u,SELF+0x48,bits(frame));put(u,SELF+0x4c,bits(rate));put(u,SELF+0x50,bits(action))
    word(u,SELF+0x78,remaining)
    byte(u,DESC,4);byte(u,DESC+3,3);word(u,VARIANTS+2,length)
    put(u,0x6044b0,timed);put(u,0x12651a0,elapsed);put(u,0x6044ac,bits(fps))
    for reg,value in [(UC_X86_REG_FPCW,cw),(UC_X86_REG_FPSW,0),(UC_X86_REG_FPTAG,0xffff)]:u.reg_write(reg,value)
    if depth:
        u.mem_write(S+0x4800,b'\xd9\xe8'*depth)
        u.emu_start(S+0x4800,S+0x4800+depth*2)

def timing_case(token,frame=.5,rate=300,action=20,timed=0,elapsed=16,fps=60,
                length=7,remaining=1,cw=0x37f,depth=0):
    results=[]
    for c,u,state in machines:
        for explicit in ([False,True] if c else [False]):
            prepare_timing(u,state,frame,rate,action,timed,elapsed,fps,length,remaining,cw,depth)
            top=(u.reg_read(UC_X86_REG_FPSW)>>11)&7;tags=u.reg_read(UC_X86_REG_FPTAG)
            if explicit:value=run(0,u,sym('restore_sprite_cycle_'+TIMING[token]),[SELF])
            else:value=run(c,u,token,receiver=SELF)
            assert u.reg_read(UC_X86_REG_FPCW)==cw
            assert ((u.reg_read(UC_X86_REG_FPSW)>>11)&7)==top
            assert u.reg_read(UC_X86_REG_FPTAG)==tags
            result=(value,bytes(u.mem_read(S,0x5000)),snapshot(u))
            if explicit:assert result==results[-1],('named C API',hex(token))
            else:results.append(result)
    equal(results,('timing',hex(token),frame,rate,action,timed,hex(elapsed),fps,length,remaining,hex(cw),depth))

if __name__=='__main__':
    n=0
    for token,remaining,timed,rate,frame in itertools.product(TIMING,[0,1,2],[0,1,256],[300,-300],[.5,6.5]):
        timing_case(token,remaining=remaining,timed=timed,rate=rate,frame=frame);n+=1
    for token,elapsed,fps in itertools.product(TIMING,[0,1,16,33,0x80000000,0xffffffff],[60,59.94]):
        timing_case(token,timed=1,elapsed=elapsed,fps=fps);n+=1
    for token,cw,profile in itertools.product(TIMING,
            [0x7f,0x27f,0x37f,0x47f,0x67f,0x77f,0x87f,0xa7f,0xb7f,0xc7f,0xe7f,0xf7f],range(3)):
        if profile==0:timing_case(token,cw=cw,frame=6.5,timed=1,elapsed=17,fps=59.94)
        elif profile==1:timing_case(token,cw=cw,frame=.5,rate=-900,timed=1,elapsed=33,fps=29.97)
        else:timing_case(token,cw=cw,frame=6.5,rate=12000)
        n+=1
    for frame,length in [(6,7),(6.75,7),(-1,7),(32767,65535),(65535,65535),(32768,65535),(float('nan'),7)]:
        timing_case(0x55d7b0,frame=frame,length=length);n+=1
    for token in TIMING:timing_case(token,timed=1,depth=2);n+=1
    print('PASS whole animation timing and completion compositions:',n,flush=True)
    print('Verified target SHA256:',target_sha,flush=True)
