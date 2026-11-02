"""Whole native/C venue ghost drawing with controlled draw/placement endpoints.

Ordered image IDs, coordinates, mask widths and final placement flag are
compared. This does not certify actual pixels or final construction commits.
"""
from verify_options_startup import *

def exercise(kind,orientation,valid=True,missing=0,band=0):
    results=[]
    for c,u,state in machines:
        reset(u,state)
        for at,value in [(0xbca304,300),(0xbca308,260),(0xbca2e0,orientation),
                (0x605994,0xaabbccdd),(0x70e120,0xabcd1234),
                (0xf567ac,0x12345),(0xf5679c,0x23456),(0xf565ec,0x34567),
                (0xf565ac,0x45678),(0xf565a4,0x56789),(0xf5659c,0x6789a),
                (0xf56774,0x789ab),(0xf56be4,0x89abc)]:put(u,at,value)
        calls=[]
        def observe(cpu,args,token):
            if token==0x43f410:record=args;value=int(valid)
            elif token==0x441130:
                x,y,a,b,primary,secondary=args
                dx,dy={31:(0,0),30:(0,1),33:(1,1)}[secondary]
                put(cpu,a,x+dx);put(cpu,b,y+dy)
                record=(x,y,primary,secondary)
                value=0xffffffff if secondary==missing else band if secondary==30 else 0
            elif token==0x4f0ba0:
                x,y,a,b=args;put(cpu,a,x*30-y*30+1000);put(cpu,b,(x+y)*15+500)
                record=(x,y);value=0
            elif token==0x4405c0:
                value=0;record=(args[0],);put(cpu,args[1],0x9abcd);put(cpu,args[2],0xabcde)
            else:record=args;value=0
            calls.append((token,record))
            cpu.reg_write(UC_X86_REG_ECX,0xdeadbeef);cpu.reg_write(UC_X86_REG_EDX,0xbaadf00d)
            return value
        for token,n in [(0x43f410,1),(0x4413b0,3),(0x441130,6),(0x4f0ba0,4),
                (0x4405c0,3),(0x53e590,3),(0x53f250,3),(0x53f4f0,4)]:
            cleanup=(16 if token==0x53f4f0 else 12 if token in (0x53e590,0x53f250) else 0) if not c else 0
            hook(c,state,token,n,lambda cpu,args,t=token:observe(cpu,args,t),cleanup=cleanup)
        run(c,u,0x43f5b0,[kind])
        results.append((tuple(calls),get(u,0x605994),get(u,0x70e120)))
    assert results[0]==results[1],('whole venue preview',kind,orientation,valid,missing,band,
        next(((i,a,b) for i,(a,b) in enumerate(zip(results[0][0],results[1][0])) if a!=b),None),results[0][1:],results[1][1:])

def festival_commit():
    results=[]
    for c,u,state in machines:
        reset(u,state)
        for at,n in [(0xaf598c,51984*4),(0xd05720,51984*4),(0xb4e6fc,51984*2),
                     (0xae8e7c,51984),(0xb285cc,51984),(0xb67d1c,51984)]:u.mem_write(at,bytes(n))
        for at,value in [(0xbca2d0,112),(0xbca2d4,112),(0xbca2d8,13282),
                (0xbca2dc,116),(0xbca304,20),(0xbca308,20),(0xbca2e0,0),
                (0xeaac28,1),(0xea6010+0x49d8,0xffffffff),(0xea6014+0x49d8,0xffffffff),
                (0xf565a4,25000)]:put(u,at,value)
        for y in range(5):
            for x in range(5):
                if x==2 or y==2:put(u,0xaf598c+(13282+(20+y)*228+20+x)*4,0x40)
        assert run(c,u,0x43f410,[209])==1
        for source,dest,n in [(0xaf598c,0x9de62c,51984*4),(0xae8e7c,0x9d1b1c,51984),
                (0xb285cc,0x9b84fc,51984),(0xb67d1c,0x9ab9ec,51984)]:
            u.mem_write(dest,bytes(u.mem_read(source,n)))
        calls=[]
        def allocate(cpu,args):calls.append(args);return 17
        hook(c,state,0x46a400,6,allocate)
        if c:state['hooks'][sym('restore_building_allocate_0046a400')]=(6,0,allocate)
        hook(c,state,0x485830,0,lambda cpu,args:0)
        hook(c,state,0x485100,1,lambda cpu,args:0)
        assert run(c,u,0x440ed0,[0,0,0,20,20],budget=20000000)==1
        assert calls==[(1,209,20,20,0,0)],('allocator arguments',c,calls)
        assert get(u,0xea6010+0x49d8)==get(u,0xea6014+0x49d8)==20
        for y in range(5):
            for x in range(5):
                at=13282+(20+y)*228+20+x
                assert struct.unpack('<H',bytes(u.mem_read(0xb4e6fc+at*2,2)))[0]==17
        results.append((tuple(calls),bytes(u.mem_read(0xaf598c,51984*4)),
            bytes(u.mem_read(0xb7482c,51984*4)),bytes(u.mem_read(0xb4e6fc,51984*2))))
    assert results[0]==results[1],'actual festival constructor and tile commit'
    print('PASS native/C festival crossroad validation and complete tile commit; allocation/finalization endpoints controlled',flush=True)

if __name__=='__main__':
    count=0
    for kind,orientation,valid in itertools.product([31,30,33,209],[0,2,4,6,1,7],[False,True]):
        exercise(kind,orientation,valid);count+=1
    for kind,orientation,missing,band in itertools.product([31,30,33],[0,2,4,6],[31,30,33],[0,1]):
        exercise(kind,orientation,True,missing,band);count+=1
    print('PASS whole Booth/bandstand/pavilion/festival ghost: images, rotation, missing parts, tint and placement flag:',count,flush=True)
    festival_commit()
    print('Verified target SHA256:',target_sha)
