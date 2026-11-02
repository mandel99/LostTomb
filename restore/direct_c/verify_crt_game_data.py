"""Native CRT vegetation/farm/map schema initialization and real consumers.

Independent VMs execute whole constructors and farm progress percentage.
Map chunks execute real4D0E10 with controlled file/decompression endpoints.
No host game, original/port process, or user file mutation is involved.
"""
from verify_options_startup import *

for c,u,state in machines:
    # Existing raw UI facades own a large local register/stack frame.
    u.mem_map(0x2fe00000,0x200000)


def initialize(c,u):
    for token in (0x486080,0x489940,0x4d4a50):run(c,u,token)


def constructors():
    tokens=[0x486080,0x486090,0x489940,0x489950,0x4d4a50,0x4d4a60]
    for token,pattern in itertools.product(tokens,[0,0xa5,0xff]):
        results=[]
        for c,u,state in machines:
            reset(u,state)
            for at,n in [(0x978d80,40),(0xbca340,32),(0x5e17d8,24*20)]:
                u.mem_write(at,bytes([pattern])*n)
            u.reg_write(UC_X86_REG_FPCW,0x37f)
            u.reg_write(UC_X86_REG_FPSW,0)
            u.reg_write(UC_X86_REG_FPTAG,0xffff)
            run(c,u,token)
            assert u.reg_read(UC_X86_REG_FPCW)==0x37f
            if token in (0x486080,0x486090):
                assert struct.unpack('<5I',bytes(u.mem_read(0x978d88,20)))==(25,35,0,0,0)
            elif token in (0x489940,0x489950):assert get(u,0xbca34c)==2000
            else:
                assert get(u,0x5e197c)==pattern*0x01010101
                assert struct.unpack('<5I',bytes(u.mem_read(0x5e19a4,20)))==(1,0xa509bc,1,51984,0)
            results.append(snapshot(u))
        assert results[0]==results[1],('whole CRT initializer',hex(token),pattern)
    print('PASS six whole CRT entries, complete data snapshots and untouched fields: 18',flush=True)


def farm_percentage():
    for building,progress in itertools.product([1,257,3999],[0,500,1999,2000,2100,-1]):
        results=[]
        for c,u,state in machines:
            reset(u,state);run(c,u,0x489940)
            at=0x799a2c+building*264
            put(u,at,0x5a5a0000|(progress&65535))
            value=run(c,u,0x4883d0,[building],cleanup=0 if c else 4)
            signed=struct.unpack('<i',struct.pack('<I',value))[0]
            expected=(abs(progress)*100//2000)*(-1 if progress<0 else 1)
            assert signed==expected,('actual farm percentage',c,building,progress,signed,expected)
            results.append((value,get(u,at),get(u,0xbca34c)))
        assert results[0]==results[1]
    print('PASS initialized farm percentage with real division and WORD record fields: 18',flush=True)


def map_chunks():
    for index,at,size in [(21,0xd38360,36),(22,0x5d2c48,288),(23,0xa509bc,51984)]:
        payload=(bytes(range(256))*(size//256+1))[:size]
        results=[]
        for c,u,state in machines:
            reset(u,state);run(c,u,0x4d4a50)
            schema=bytes(u.mem_read(0x5e17d8,24*20))
            u.mem_write(0xe92e50,schema+bytes(6000-len(schema)))
            put(u,0xe945c0,160);put(u,0xe945c4,24)
            u.mem_write(at,b'\xcc'*size)
            def read(cpu,args):
                assert args==(37,at,size),('map routing',c,index,args)
                state['calls'].append(args)
                cpu.mem_write(args[1],payload);return size
            hook(c,state,0x5660ed,3,read)
            hook(c,state,0x55bcf0,3,read)
            value=run(c,u,0x4d0e10,[index,0x5e17d8,37])
            assert value&255==1
            actual=bytes(u.mem_read(at,size));assert actual==payload
            results.append((tuple(state['calls']),actual))
        assert results[0]==results[1]
    print('PASS real map chunk routes for settings, seasonal records and grass: 3',flush=True)


if __name__=='__main__':
    constructors()
    farm_percentage()
    map_chunks()
    print('Verified target SHA256:',target_sha)
