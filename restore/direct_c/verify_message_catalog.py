"""Whole native/C message catalog construction, lookup and destruction.

Only allocator, file IO and strcmpi endpoints are controlled. The actual two
parsing passes execute against synthetic inputs and read-only EVENTMSG.TXT.
"""
from verify_options_startup import *

HEAP,CAPACITY,SELF=0x36000000,0x400000,0x784f20
for c,u,state in machines:u.mem_map(HEAP,CAPACITY)

def string(u,at):
    if not at:return None
    out=bytearray()
    while True:
        part=bytes(u.mem_read(at,128));end=part.find(b'\0')
        if end>=0:return bytes(out)+part[:end]
        out.extend(part);at+=128

def exercise(data,keys=(b'ONE',b'TWO'),fail=0,opened=True,filename=True,real=False):
    results=[]
    for c,u,state in machines:
        reset(u,state);u.mem_write(HEAP,b'\xa5'*CAPACITY)
        table=0x5d3010 if real else S+0x2000
        if not real:
            if keys is None:table=0
            else:
                at=S+0x3000
                for i,key in enumerate(keys):
                    put(u,table+i*8,i);put(u,table+i*8+4,at)
                    u.mem_write(at,key+b'\0');at+=len(key)+1
                put(u,table+len(keys)*8+4,0)
        u.mem_write(S+0x1000,b'EVENTMSG.TXT\0')
        cursor=HEAP;allocations=[];calls=[]
        def allocate(cpu,args):
            nonlocal cursor
            calls.append(('allocate',args));allocations.append(args[0])
            if len(allocations)==fail:return 0
            at=cursor;cursor+=(args[0]+31)&~15
            assert cursor<HEAP+CAPACITY
            return at
        def read(cpu,args):
            assert args[1:]==(len(data),1,S+0x4000)
            cpu.mem_write(args[0],data);calls.append(('read',args));return 1
        compared={}
        def compare(cpu,args):
            # Immutable keys and already NUL-terminated source keywords stay
            # stable across the two passes. Native562370 is strcmpi.
            for at in args:
                if at not in compared:compared[at]=string(cpu,at)
            a,b=compared[args[0]].lower(),compared[args[1]].lower()
            cpu.reg_write(UC_X86_REG_ECX,0xdeadbeef);cpu.reg_write(UC_X86_REG_EDX,0xbaadf00d)
            return (a>b)-(a<b)
        hook(c,state,0x561a1d,1,allocate)
        hook(c,state,0x561a2b,1,lambda cpu,args:calls.append(('free',args)) or 0)
        hook(c,state,0x561a0a,2,lambda cpu,args:calls.append(('open',args)) or (S+0x4000 if opened else 0))
        hook(c,state,0x56b22e,1,lambda cpu,args:37)
        hook(c,state,0x56b1b6,1,lambda cpu,args:len(data))
        hook(c,state,0x561ca1,4,read)
        hook(c,state,0x561994,1,lambda cpu,args:calls.append(('close',args)) or 0)
        hook(c,state,0x562370,2,compare)
        args=[S+0x1000 if filename else 0,table]
        result=run(c,u,0x4cba60,([SELF]+args if c else args),
            receiver=None if c else SELF,cleanup=0 if c else 8,budget=40000000)
        assert result==SELF
        count=get(u,SELF+12);entries=get(u,SELF+4)
        phrases=[]
        for i in range(count):
            ptr=get(u,entries+i*4)
            phrases.append(string(u,ptr))
            value=run(c,u,0x4cbdb0,[SELF,i] if c else [i],
                receiver=None if c else SELF,cleanup=0 if c else 4)
            assert value==ptr
        before=(bytes(u.mem_read(SELF,20)),bytes(u.mem_read(HEAP,cursor-HEAP)))
        run(c,u,0x4cbd70,[SELF] if c else [],receiver=None if c else SELF)
        assert get(u,SELF+4)==get(u,SELF+8)==0
        results.append((tuple(calls),before,tuple(phrases),bytes(u.mem_read(SELF,20))))
    assert results[0]==results[1],('whole native/C catalog',data[:50],keys,fail,opened,filename,real)
    return results[1][2]

if __name__=='__main__':
    fixtures=[b'',b'ONE "Hello world"',b'ONE "Alpha"\r\nTWO "Beta"',
        b'; comment\r\r\nONE "a;b"\r\r\nTWO "two"\r\r\n',
        b'ONE "first"\nONE "last"',b'ONE "line\ninside"',
        b'ONE "Path\\work"',b'UNKNOWN "stop"\nONE "not parsed"',
        b'one "case-insensitive match"',b'ONE not-a-quote',b'ONE "\x80\xff"']
    for data in fixtures:exercise(data)
    assert exercise(b'ONE "first"\nONE "last"')==(b'last',None)
    for fail in (1,2,3):exercise(b'ONE "valid"',fail=fail)
    exercise(b'ONE "valid"',opened=False)
    exercise(b'ONE "valid"',filename=False)
    exercise(b'',keys=())
    exercise(b'',keys=None)
    print('PASS whole catalog parsing, lifecycle, lookup, comments, duplicates and allocation/IO failures: 19',flush=True)
    data=(ROOT/'build/debug-startup-game/EVENTMSG.TXT').read_bytes()
    phrases=exercise(data,real=True)
    assert len(phrases)==601 and all(p is not None for p in phrases)
    print('PASS actual EVENTMSG.TXT with native key table:',len(phrases),'slots,',sum(p is not None for p in phrases),'populated;',hashlib.sha256(data).hexdigest(),flush=True)
    print('Verified target SHA256:',target_sha)
