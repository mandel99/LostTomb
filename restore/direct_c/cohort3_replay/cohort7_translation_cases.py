"""Necessary continuation/stack checks for the connected translation guard.

Only the OS unwind and user translator/handler boundaries are controlled.
Shared561603/5616B7/561505 algorithms execute; whole SEH is not certified.
"""
import struct

S, SP = 0x32000000, 0x30008000
EXC, REG, DESC, FRAME, CTX, GUARD, CHAIN, TRY = [S+i*0x1000 for i in range(1,9)]
TRANSLATOR, CONTINUATION = 0x31000b00, 0x31000c00


def put(case, at, value):
    case['stores'].append((at, struct.pack('<I', value & 0xffffffff)))


def base(entry, label, args, result=None, cleanup=0):
    blocks = [EXC, REG, DESC, FRAME, CTX, GUARD, CHAIN, TRY]
    return dict(id=f'c7-translation/{entry:08x}/{label}', entry=entry,
        receiver=0x89abcdef, arguments=args, named_arguments=args, eflags=0x602,
        initial_edx=0x76543210, stores=[(at, b'\xa5'*128) for at in blocks]+[(0x18,struct.pack('<I',0))],
        watched=[(at, 128) for at in blocks]+[(0,4)], write_ranges=[(at,128) for at in blocks]+[(0,4)],
        write_trace_ranges=[(EXC+4,4),(GUARD,4),(0,4),(REG+36,4)],
        initial_fs0=GUARD, result_mask=result, native_cleanup=cleanup,
        named_cleanup=0, canonical_cleanup=cleanup, canonical_registers_and_flags=False)


def unwind_endpoint(mutate=False):
    return dict(token='RtlUnwind', import_name='RtlUnwind', arity=4,
        native_cleanup=16, compiled_cleanup=16, result_kind='void',
        continuation_args={1:'relocated-unwind-resumption'},
        mutations=[(0,4,CHAIN)] if mutate else [])


def seeded(case):
    for at,value in [(EXC,0xc0000005),(EXC+4,2),(DESC,0x19930520),(DESC+4,0),
        (DESC+12,0),(FRAME+8,0xffffffff),(GUARD,0x13579bdf),(CHAIN,0x2468ace0)]:
        put(case,at,value)
    return case


def definitions(registry):
    present={int(member['address'],16) for member in registry['members']}
    out=[]
    if 0x561505 in present:
        for flags in [0,2,0x80000066]:
            c=seeded(base(0x561505,f'flags-{flags:x}',[REG,EXC],cleanup=8))
            put(c,EXC+4,flags)
            c.update(endpoints=[unwind_endpoint(True)],expected_endpoint_counts={'RtlUnwind':1})
            out.append(c)
    if 0x561603 in present:
        for mode in ['return','resume','unwind-return','unwind-resume']:
            c=seeded(base(0x561603,mode,[EXC,FRAME,CTX,0x76543210,DESC,2,REG],result=0xffffffff))
            e=dict(token='translator',controlled_address=TRANSLATOR,arity=2,
                native_cleanup=0,compiled_cleanup=0,result_kind='void',
                private_input_args={1:dict(role='EXCEPTION_POINTERS',width=8)},
                fs0_registration=dict(size=40,native_handler=0x5616b7,
                    compiled_handler_symbol='FUN_005616b7',
                    fields=[(0,4),(8,4),(12,4),(16,4),(20,4),(28,4),(32,4),(36,4)]))
            put(c,0x1266410,TRANSLATOR)
            c['watched'].append((0x1266410,4))
            if mode.startswith('unwind'):
                # The fixed native-layout bridge puts guard.unwind_seen at
                # entrySP-8. A user/OS boundary can mark it and replace FS0.
                c['write_ranges'].append((SP-8,4))
                e['mutations']=[(SP-8,4,1)]
                if mode.endswith('return'):
                    e['mutations'].append((0,4,CHAIN))
            if mode.endswith('resume'):
                e['resume_fs_registration']=True
            c.update(endpoints=[e],expected_endpoint_counts={'translator':1},
                     expected_eax=int(mode.endswith('resume')))
            out.append(c)
    if 0x5616b7 in present:
        for flags in [2,4,0x20,0x40,0x80000002]:
            c=seeded(base(0x5616b7,f'unwind-{flags:x}',[EXC,REG,CTX],result=0xffffffff))
            put(c,EXC+4,flags)
            c.update(expected_eax=1,expected_flags=(0x400,0))
            out.append(c)
        for seen in [0,1]:
            c=seeded(base(0x5616b7,f'dispatch-resume-{seen}',[EXC,REG,CTX]))
            for at,value in [(EXC+4,0),(REG+8,DESC),(REG+12,FRAME),(REG+16,0),
                (REG+20,REG),(REG+24,CONTINUATION),(REG+28,SP+0x1000),
                (REG+32,FRAME+64),(REG+36,seen)]:
                put(c,at,value)
            end=dict(token='saved-continuation',controlled_address=CONTINUATION,
                arity=0,native_cleanup=0,compiled_cleanup=0,result_kind='void',
                noreturn=True,registers=['esp','ebp','ebx'])
            eps=[end] if seen else [unwind_endpoint(True),end]
            c.update(endpoints=eps,noreturn_endpoint='saved-continuation',
                expected_endpoint_counts={'saved-continuation':1,**({} if seen else {'RtlUnwind':1})})
            out.append(c)
    if 0x563c95 in present:
        for target in [0,REG]:
            c=seeded(base(0x563c95,f'typed-unwind-target-{target:x}',
                [EXC,FRAME,CTX,0,DESC,TRY+64,0,TRY,2,target]))
            put(c,TRY,0xffffffff);put(c,TRY+4,0)
            c['write_trace_ranges']=[(EXC+4,4),(GUARD,4)]
            child=dict(token='catch-handler',native=0x563d10,
                compiled_symbols=['FUN_00563d10'],arity=7,native_cleanup=0,
                compiled_cleanup=0,result_kind='full32',value=0)
            c.update(endpoints=[unwind_endpoint(True),child],
                expected_endpoint_counts={'RtlUnwind':1,'catch-handler':1})
            out.append(c)
    return out
