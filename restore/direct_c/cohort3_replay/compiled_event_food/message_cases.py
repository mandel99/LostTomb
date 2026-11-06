"""511C50 signed DWORD message selection; reuse its existing complete C."""
import struct


def definitions(registry=None):
    cases=[]
    for index,(value,wanted) in enumerate([(0,10),(1,1),(10,10),(0x100,0x100),
            (0x7fffffff,0x7fffffff),(0x80000000,10),(0xffffff00,10),(0xffffffff,10)]):
        dirty=(index*17+0x80)&255
        response=dict(observe=[(0xeadd54,4),(0xe38dca,1)],
            mutations=[(0xeadd54,4,0x11223344),(0xe38dca,1,0x77)])
        c=dict(id=f'c7-message-button/00511c50/{value:08x}',entry=0x511c50,
            receiver=0x87654321,initial_eax=0xabcdef01,initial_edx=0xdead1234,
            arguments=[],named_arguments=[],canonical_arguments=[],eflags=0xad7,
            native_cleanup=0,named_cleanup=0,canonical_cleanup=0,
            stores=[(0xeadd54,struct.pack('<I',value)),(0xe38dc9,bytes([0xa6,dirty,0xb7]))],
            watched=[(0xeadd54,4),(0xe38dc9,3)],write_ranges=[(0xeadd54,4),(0xe38dca,1)],
            result_mask=None,canonical_registers_and_flags=False,
            endpoints=[dict(token='message-panel',native=0x4c5bf0,compiled_symbols=['FUN_004c5bf0'],
                arity=3,native_cleanup=0,compiled_cleanup=0,result_kind='void',
                clobber_ecx=0xdeadbeef,clobber_edx=0xbeefdead,responses=[response])],
            expected_endpoint_counts={'message-panel':1},expected_endpoint_arguments={'message-panel':[[wanted,0,0]]},
            authored_endpoint_order=['message-panel'],
            authored_endpoint_observations=[[[0xeadd54,4,value],[0xe38dca,1,dirty]]],
            authored_final_memory=[(0xeadd54,'44332211'),(0xe38dc9,'a601b7')],
            initial_fpcw=0x27f,initial_x87=['0000000000000080ff3f','000000000000008000c0'],
            compare_x87_values=True,compare_x87_control=True)
        cases.append(c)
    if registry is not None:
        members={int(m['address'],16):m for m in registry['members']}
        cases=[c for c in cases if c['entry'] in members]
        for c in cases:
            c['expected_invocations']=1+sum(bool(p['named'])+bool(p['canonical'])for p in members[c['entry']]['providers'].values())
    return cases
