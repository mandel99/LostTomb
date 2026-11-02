"""Whole native/C queued-event opening; actual layouts, no host game.

Refresh, media existence and fallback sound are controlled endpoints. Half
the opening cases execute the actual panel setup/reset as well. Compare all
seeded globals, event bytes and ordered endpoint calls, including callback
mutations; retain the native early-stop-at-empty-slot behavior.
"""
from verify_options_startup import *

MM=(ROOT/'build/debug-startup-game/Pharaoh_MM.eng').read_bytes()
TEXT=0x36000000
for c,u,state in machines:
    u.mem_map(TEXT,0x200000)
    u.mem_write(TEXT,MM[0x13898:])
    u.mem_write(TEXT+0x100000,MM[0x13898:])

def exercise(slot=0,queue=0,body=-1,exists=0,composed=False,mutation=0,
             mode=1,delay=0,sequence=None,gap=False,empty=False):
    results=[]
    key=3000+slot if sequence is None else sequence
    for c,u,state in machines:
        reset(u,state)
        u.mem_write(0xe209b8,MM[24:0x13898])
        u.mem_write(0xe95228,bytes(48000));u.mem_write(0xe95080,bytes(80))
        for i in range(slot+1):
            p=0xe95228+i*48
            put(u,p,0x1234abcd+i);put(u,p+4,0xfedc1234+i)
            word(u,p+8,-3048);word(u,p+10,101);word(u,p+12,3000+i)
            byte(u,p+14,0);byte(u,p+15,9)
            word(u,p+16,body);word(u,p+18,-1);word(u,p+20,-1)
            word(u,p+38,0xa135)
        if empty:u.mem_write(0xe95080,bytes(80))
        else:put(u,0xe95080+queue*4,key)
        if gap:word(u,0xe95232,0)
        byte(u,0xe38dda,mode);put(u,0xea0da8,delay)
        put(u,0xe36c74,TEXT);put(u,0xe951e4,0xffffffff)
        calls=[]
        def refresh(cpu,args):
            calls.append(('refresh',get(cpu,0xe951e4),get(cpu,0xd3b170),get(cpu,0xd3b178),get(cpu,0xd3b174)))
            if mutation:
                put(cpu,0xe36c74,TEXT+0x100000);put(cpu,0xe951dc,100)
                put(cpu,0xe951e4,999)
                word(cpu,0xe95238+slot*48,7)
            return 0
        def media(cpu,args):calls.append(('media',args));return exists
        def sound(cpu,args):calls.append(('sound',args));return 0
        def panel(cpu,args):
            calls.append(('panel',(args[0],args[1],args[2]&255)))
            return 0
        for token,n,callback in [(0x4ca980,0,refresh),(0x4ce3f0,1,media),(0x47d350,1,sound)]:
            hook(c,state,token,n,callback)
        if not composed:hook(c,state,0x4c5bf0,3,panel)
        run(c,u,0x4e1320)
        active=mode==1 and (delay&0x80000000 or delay==0) and not empty and 0<key<=0x7fff and not gap and slot<999
        if active:
            p=0xe95228+slot*48
            assert bytes(u.mem_read(p+14,2))==b'\x01\x09',('opened/month',c,slot)
            expected=7 if mutation else body
            assert struct.unpack('<h',u.mem_read(p+16,2))[0]==expected,('template corruption',c,slot)
            if composed:assert get(u,0xd3b124)==(100 if mutation else 101)
        results.append((tuple(calls),snapshot(u)))
    equal(results,('queued event',slot,queue,body,exists,composed,mutation,mode,delay,key,gap,empty))

def population_publish(message):
    results=[]
    for c,u,state in machines:
        reset(u,state)
        u.mem_write(0xe209b8,MM[24:0x13898])
        u.mem_write(0xe95228,bytes(48000));u.mem_write(0xe95080,bytes(80))
        put(u,0xe36c74,TEXT);put(u,0xea0da8,0);put(u,0xe951ec,0)
        put(u,0xe951f0,3);word(u,0x78d7bc,-3048);byte(u,0x78d7c0,9)
        byte(u,0xe38dda,2)
        hook(c,state,0x53b090,1,lambda cpu,args:0)
        hook(c,state,0x4ce3f0,1,lambda cpu,args:0)
        hook(c,state,0x4ca980,0,lambda cpu,args:0)
        # Actual publisher, null template parameters and actual slot allocator.
        run(c,u,0x4e0cf0,[message,0,0,0,1])
        assert get(u,0xe95080)==3
        assert bytes(u.mem_read(0xe95238,6))==b'\xff'*6
        byte(u,0xe38dda,1)
        # Actual queue opener, action initializer, panel setup and link reset.
        run(c,u,0x4e1320)
        assert get(u,0xd3b124)==message and u.mem_read(0xd3b184,1)==b'\x00'
        assert bytes(u.mem_read(0xe95236,8))==b'\x01\x09'+b'\xff'*6
        assert get(u,0xd3b174)==9
        results.append((snapshot(u),))
    equal(results,('population publisher through queue and panel',message))

if __name__=='__main__':
    n=0
    for slot,queue,body,exists,composed in itertools.product(
            [0,1,9,998,999],[0,19],[-1,0,257],[0,1,256],[False,True]):
        exercise(slot,queue,body,exists,composed);n+=1
    for slot,composed in itertools.product([0,9],[False,True]):
        exercise(slot,composed=composed,mutation=1);n+=1
    for mode,delay in itertools.product([0,1,5,73,255],[0,1,256,0x80000000]):
        exercise(mode=mode,delay=delay);n+=1
    for key in [0,0xffffffff,0x80000000,70000]:exercise(sequence=key);n+=1
    exercise(slot=9,gap=True);exercise(empty=True);n+=2
    for message in range(100,110):population_publish(message);n+=1
    print('PASS whole queued-event opening and panel composition:',n,flush=True)
    print('Verified target SHA256:',target_sha,flush=True)
