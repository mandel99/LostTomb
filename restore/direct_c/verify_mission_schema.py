"""Missing native CRT schema constructor and actual campaign/grass chunks.

The constructor's complete bytes are checked against the Ghidra memory read.
Chunk routing runs real4D0E10; only file/decompression endpoints are controlled
with read-only PAK payloads decoded by the SHA-pinned original DCL fixture.
Recovery is explicitly new compatibility behavior, not native equivalence.
"""
from verify_options_startup import *


def call_named(u,name):
    # Use the compiled entry as a direct VM address, without inventing an
    # original counterpart for this new compatibility helper.
    return run(0,u,sym(name))


def verify_constructor():
    cases=0
    for token,pattern in itertools.product([0x4d1340,0x4d1350],[None,0,0xa5,0xff]):
        results=[]
        for c,u,state in machines:
            reset(u,state)
            if pattern is not None:u.mem_write(0x5e09a0,bytes([pattern])*(181*20))
            before=bytes(u.mem_read(0x5e09a0,147*20))
            codec=get(u,0x5e151c)
            run(c,u,token)
            assert bytes(u.mem_read(0x5e09a0,147*20))==before
            assert get(u,0x5e151c)==codec
            assert struct.unpack('<5I',bytes(u.mem_read(0x5e16d4,20)))==(0,0x7027b4,1,1,0)
            assert struct.unpack('<5I',bytes(u.mem_read(0x5e16e8,20)))==(1,0xa509bc,1,51984,0)
            results.append(snapshot(u))
        assert results[0]==results[1],('complete schema constructor',hex(token),pattern)
        cases+=1
    print('PASS whole constructor and CRT thunk, full data snapshot and untouched fields:',cases,flush=True)


def pak_chunks(scene):
    path=ROOT/'build/debug-startup-game/mission1.pak'
    data=path.read_bytes();start=struct.unpack_from('<I',data,scene*4)[0]
    count=struct.unpack_from('<I',data,start+8)[0]
    assert count==175
    rows=[struct.unpack_from('<5I',data,start+12+i*20) for i in range(count)]
    schema=next(i for i,d in enumerate(rows) if d[0]==0 and d[2:4]==(20,300))
    source=ROOT/'tools/verify_compression_emulation.py'
    decoder={'__file__':str(source),'__name__':'schema_dcl_decoder'}
    exec(compile(source.read_text().split('sample=bytes.fromhex',1)[0],str(source),'exec'),decoder)
    position=start+6012;payloads={}
    for i in range(schema+1,count):
        codec,_,size,n,_=rows[i];length=size*n
        marker=struct.unpack_from('<I',data,position)[0] if codec else None
        position+=4 if codec else 0
        stored=length if marker in (None,0x80000000) else marker
        payload=data[position:position+stored];position+=stored
        if i not in (169,170):continue
        if codec and marker!=0x80000000:
            result=decoder['native'](0,length,0xffffffff,payload)
            assert result['result']==1
            payload=bytes.fromhex(result['output'])[:length]
        assert len(payload)==length
        payloads[i]=payload
    return data[start+12:start+6012],payloads


def verify_pak_routing():
    for scene in (0,1):
        schema,payloads=pak_chunks(scene)
        assert any(payloads[170]),'original mission grass is not an empty map'
        for index in (169,170):
            results=[]
            for c,u,state in machines:
                reset(u,state);run(c,u,0x4d1340)
                u.mem_write(0xe92e50,schema)
                put(u,0xe945c0,147);put(u,0xe945c4,175)
                at=get(u,0x5e09a0+index*20+4)
                length=len(payloads[index]);u.mem_write(at,b'\xcc'*length)
                def read(uc,args):
                    assert args==(37,at,length),('real chunk destination',index,args)
                    state['calls'].append((args[1],args[2]))
                    uc.mem_write(args[1],payloads[index])
                    return length
                hook(c,state,0x5660ed,3,read)
                hook(c,state,0x55bcf0,3,read)
                value=run(c,u,0x4d0e10,[index,0x5e09a0,37])
                assert value&255==1
                actual=bytes(u.mem_read(at,length))
                assert actual==payloads[index]
                results.append((tuple(state['calls']),actual))
            assert results[0]==results[1],('real native/C chunk routing',scene,index)
        print('PASS original PAK scene',scene,'real mode/grass chunk routing; grass nonzero cells',
              sum(bool(b) for b in payloads[170]),'SHA256',hashlib.sha256(payloads[170]).hexdigest(),flush=True)


def verify_save_schema():
    results=[]
    for c,u,state in machines:
        reset(u,state);run(c,u,0x4d1340)
        u.mem_write(S,b'save\\schema-fixture.sav\0')
        u.mem_write(0xe38e74,b'Restore\0')
        u.mem_write(0x776fb0,bytes(50*324))
        byte(u,0x7027b4,1)
        grass=bytes(range(256))*(51984//256)+bytes(range(51984%256))
        u.mem_write(0xa509bc,grass)
        for token,n in [(0x4d0a40,1),(0x4ce2d0,0),(0x4771c0,0),(0x4e2970,3),(0x5647eb,1)]:
            hook(c,state,token,n,lambda uc,args:0)
        hook(c,state,0x568eab,3,lambda uc,args:37)
        writes=[]
        def write(cpu,args,codec):
            assert args[0]==37
            data=bytes(cpu.mem_read(args[1],args[2]))
            writes.append((codec,args[1],data))
            return args[2]
        hook(c,state,0x5664d5,3,lambda uc,args:write(uc,args,0))
        hook(c,state,0x55bc50,3,lambda uc,args:write(uc,args,1))
        assert run(c,u,0x4d4e20,[S])==0
        assert len(writes)==181
        assert writes[169]==(0,0x7027b4,b'\1')
        assert writes[170]==(1,0xa509bc,grass)
        assert writes[6][2][169*20:171*20]==bytes(u.mem_read(0x5e16d4,40))
        results.append(writes)
    assert results[0]==results[1],'real save writer complete181-descriptor stream'
    print('PASS whole native/C save writer publishes181 descriptors including actual mode and grass; IO controlled',flush=True)


def setup_recovery(u,state,valid=False,version=160,custom=0,selected=0,mode=0):
    reset(u,state)
    profile=(ROOT/'build/debug-startup-game/Save/Restore.dat').read_bytes()
    data=(ROOT/'build/debug-startup-game/Save/Restore/autosave.sav').read_bytes()
    u.mem_write(0x6fdd08,profile)
    u.mem_write(0xe92e50,data[12:6012])
    if valid:
        run(1,u,0x4d1340)
        u.mem_write(0xe92e50,bytes(u.mem_read(0x5e09a0,181*20))+bytes(6000-181*20))
    put(u,0xe945c0,version);put(u,0xe945c4,181);put(u,0xe38ebc,custom)
    byte(u,0xe38ea8,0);byte(u,0xe38eab,selected);byte(u,0x7027b4,mode)


def verify_recovery():
    c,u,state=machines[1]
    for valid,version,custom,selected,mode in [
            (False,160,0,0,0),(True,160,0,0,0),(False,159,0,0,0),
            (False,160,1,0,0),(False,160,0,255,0),(False,160,0,0,2)]:
        setup_recovery(u,state,valid,version,custom,selected,mode)
        before=bytes(u.mem_read(0xe92e50,6000))
        call_named(u,'restore_mission_schema_recover_loaded')
        expected=1 if not valid and version==160 and not custom and selected==0 and not mode else mode
        assert u.mem_read(0x7027b4,1)==bytes([expected])
        assert bytes(u.mem_read(0xe92e50,6000))==before
        visits=[]
        def regenerate(cpu,args):
            assert cpu.mem_read(0xbca30c,1)==b'\1'
            visits.append(args);byte(cpu,0xbca30c,0);return 0
        hook(c,state,0x486e70,0,regenerate)
        call_named(u,'restore_mission_schema_recover_terrain')
        call_named(u,'restore_mission_schema_recover_terrain')
        assert len(visits)==(not valid and version==160),('bounded compatibility recovery',valid,version)
    print('PASS early C-only save recovery: identity/format guards and one native terrain regeneration: 6',flush=True)


if __name__=='__main__':
    verify_constructor()
    verify_pak_routing()
    verify_save_schema()
    verify_recovery()
    print('Verified target SHA256:',target_sha)
