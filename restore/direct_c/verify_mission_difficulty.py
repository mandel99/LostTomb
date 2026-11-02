"""Offline difficulty persistence and real language lookup; no host game or writes.

The original PAK controller executes, with file/chunk and city-postload
boundaries controlled. Bootstrap executes its real persisted-settings reader;
only CRT file calls, INI/hardware and campaign selection are controlled.
"""
from verify_options_startup import *


def verify_persisted():
    data = (ROOT/'build/debug-startup-game/Pharaoh.inf').read_bytes()
    assert len(data) == 564
    for level in range(5):
        payload = bytearray(data)
        struct.pack_into('<I', payload, 552, level)
        results = []
        for c,u,state in machines:
            reset(u,state)
            hook(c,state,0x414a90,0,lambda cpu,args:1)
            hook(c,state,0x417dd0,1,lambda cpu,args:0)
            hook(c,state,0x561a0a,2,lambda cpu,args:S+0x4000)
            hook(c,state,0x56b22e,1,lambda cpu,args:37)
            hook(c,state,0x56b1b6,1,lambda cpu,args:len(payload))
            def read(cpu,args):
                assert args == (0xe38e58,1,len(payload),S+0x4000)
                cpu.mem_write(args[0],bytes(payload))
                return len(payload)
            hook(c,state,0x561ca1,4,read)
            hook(c,state,0x561994,1,lambda cpu,args:0)
            slot=sym('_imp__GetSystemMetrics@4') if c else 0x56f1a0
            before=get(u,slot);put(u,slot,S+0x49000)
            state['hooks'][S+0x49000]=(1,4,lambda cpu,args:1024)
            run(c,u,0x4147f0)
            put(u,slot,before)
            assert get(u,0xe39080)==level
            results.append(snapshot(u))
        assert results[0]==results[1],('persisted startup difficulty',level)
    print('PASS real persisted-settings reader: all five difficulties',flush=True)


def verify_pak_preserves():
    for level,loaded,mode in itertools.product(range(5),range(5),[0,1]):
        results=[]
        for c,u,state in machines:
            reset(u,state);put(u,0xe39080,level);put(u,0xeaac28,1)
            byte(u,0xe38ea8,48)  # Cleopatra's Capital, observed selected scenario.
            for token,n in [(0x4ce2d0,0),(0x568eab,2),(0x5662e3,3),
                            (0x5647eb,1),(0x4d52a0,0)]:
                hook(c,state,token,n,lambda cpu,args:37)
            def offset(cpu,args):
                assert args[2:] == (4,192)
                put(cpu,args[1],4096);return 1
            hook(c,state,0x4ce480,4,offset)
            hook(c,state,0x4d41e0,1,
                 lambda cpu,args:put(cpu,0xe39080,loaded) or 0)
            def settings(cpu,args):
                assert get(cpu,0xe39080)==level
                return 0
            hook(c,state,0x4bd360,1 if c else 0,settings)
            if c:state['hooks'][sym('restore_game_trace')]=(0,0,lambda cpu,args:0)
            result=run(c,u,0x4d58b0,[0,mode])
            assert result==1 and get(u,0xe39080)==level
            results.append(snapshot(u))
        assert results[0]==results[1],('PAK difficulty',level,loaded,mode)
    print('PASS PAK preserves selected difficulty over scenario values: 50 cases',flush=True)


def verify_catalog():
    data=(ROOT/'build/debug-startup-game/Pharaoh_Text.eng').read_bytes()
    labels=[]
    for level in range(5):
        results=[]
        for c,u,state in machines:
            reset(u,state);u.mem_write(0xe34c38,data[28:8028])
            u.mem_write(S+0x1000,data[8028:]);put(u,0xe36c70,S+0x1000)
            run(c,u,0x4cc110,[153,level+1])
            results.append(bytes(u.mem_read(get(u,0xe20768),64)).split(b'\0',1)[0])
        assert results[0]==results[1]
        labels.append(results[1])
    assert labels[0]==b'Very easy' and labels[2]==b'Normal',labels
    print('PASS real difficulty labels:',labels,flush=True)


if __name__=='__main__':
    verify_persisted()
    verify_pak_preserves()
    verify_catalog()
    print('target SHA256',target_sha)
