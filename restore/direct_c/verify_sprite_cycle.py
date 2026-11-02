"""Seven whole animation frame/variant/cycle methods against original x86.

Actual state, x87 operations and calls execute. No endpoint is substituted.
Compare memory, defined EAX, stack, saved registers, CW and balanced x87 stack.
Valid nonzero frame counts; malformed pointers, zero-length infinite loops,
FPSW/traps and the still-unreconstructed higher animation controller are outside.
"""
from verify_options_startup import *

SELF,DESC,VARIANTS=S+0x1000,S+0x2000,S+0x3000
NAMES={0x55d290:'set_frame',0x55d2e0:'select_variant',0x55d300:'select_frame',
       0x55d480:'add_forward',0x55d4c0:'add_reverse',0x55d590:'wrap_forward',0x55d630:'wrap_reverse'}
BOOL={0x55d480,0x55d4c0,0x55d590,0x55d630}
bits=lambda f:struct.unpack('<I',struct.pack('<f',f))[0]

def case(token,args=(),frame=0.0,length=7,remaining=2,mode=4,repeats=3,cw=0x37f,
         descriptor=True,variant=True):
    results=[]
    for c,u,state in machines:
        reset(u,state)
        u.mem_write(SELF,b'\xa5'*0x80)
        put(u,SELF+0x40,DESC if descriptor else 0)
        put(u,SELF+0x44,VARIANTS if variant else 0)
        put(u,SELF+0x48,bits(frame));word(u,SELF+0x78,remaining)
        byte(u,DESC,mode);byte(u,DESC+3,repeats);put(u,DESC+12,VARIANTS)
        for i in range(8):word(u,VARIANTS+i*8+2,length)
        for reg,value in [(UC_X86_REG_FPCW,cw),(UC_X86_REG_FPSW,0),(UC_X86_REG_FPTAG,0xffff)]:u.reg_write(reg,value)
        explicit_only=c and not re.search(r'\s_FUN_%08x\s'%token,symbols)
        if explicit_only:value=run(0,u,sym('restore_sprite_cycle_'+NAMES[token]),[SELF,*args])
        else:value=run(c,u,token,args,receiver=SELF,cleanup=len(args)*4 if not c else 0)
        assert u.reg_read(UC_X86_REG_FPCW)==cw
        assert ((u.reg_read(UC_X86_REG_FPSW)>>11)&7)==0
        results.append((value if token in BOOL else None,bytes(u.mem_read(S,0x5000)),snapshot(u)))
        if c and not explicit_only:
            expected=results[-1]
            # Replay the same operation through the named explicit C API.
            reset(u,state);u.mem_write(S,results[0][1])
            # Restore pre-call state independently of the native result.
            u.mem_write(SELF,b'\xa5'*0x80)
            put(u,SELF+0x40,DESC if descriptor else 0);put(u,SELF+0x44,VARIANTS if variant else 0)
            put(u,SELF+0x48,bits(frame));word(u,SELF+0x78,remaining)
            u.reg_write(UC_X86_REG_FPSW,0);u.reg_write(UC_X86_REG_FPTAG,0xffff)
            value=run(0,u,sym('restore_sprite_cycle_'+NAMES[token]),[SELF,*args])
            assert (value if token in BOOL else None,bytes(u.mem_read(S,0x5000)),snapshot(u))==expected,('explicit API',hex(token))
    equal(results,(hex(token),args,frame,length,remaining,mode,repeats,hex(cw),descriptor,variant))

if __name__=='__main__':
    n=0
    for length,input in itertools.product([1,7,255,65535],[-32768,-15,-1,0,6,32767,0xffff8001,0x10002]):
        case(0x55d290,[input],length=length);n+=1
    case(0x55d290,[0x81234567],variant=False);n+=1
    for variant,frame,present in itertools.product([0,3,0xabcd0003],[0,0xffff,0x18001],[False,True]):
        case(0x55d300,[variant,frame],descriptor=present);n+=1
    for variant,present in itertools.product([0,3,0xabcd0003],[False,True]):
        case(0x55d2e0,[variant],descriptor=present);n+=1
    for token,length,remaining,mode,which in itertools.product(
            [0x55d590,0x55d630],[1,7,65535],[0,1,2],[0,4],range(6)):
        frame=[-length*2-.5,-.5,0,length-.5,length,length*3+.5][which]
        case(token,frame=frame,length=length,remaining=remaining,mode=mode);n+=1
    for token,cw,frame,step in itertools.product([0x55d480,0x55d4c0],
            [0x7f,0x27f,0x37f,0x47f,0x67f,0x77f,0x87f,0xa7f,0xb7f,0xc7f,0xe7f,0xf7f],
            [0.0,1.0],[-2.0**-60,2.0**-60]):
        case(token,[bits(step)],frame=frame,length=1,cw=cw);n+=1
    for token in [0x55d480,0x55d4c0]:
        for frame,step in [(6.5,.5),(-.5,.5),(100,-120)]:
            case(token,[bits(step)],frame=frame);n+=1
    print('PASS seven whole sprite-cycle methods; explicit API and linked legacy adapters:',n,flush=True)
    print('Verified target SHA256:',target_sha,flush=True)
