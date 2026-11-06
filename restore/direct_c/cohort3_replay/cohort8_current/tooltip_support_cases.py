"""Native52DA40 DWORD branches and BYTE-stride cases; no host execution."""
ENTRY=0x52da40
SELECTED,MODE,TABLE=0x784f14,0x5d2e88,0x77f748
X,Y,HEIGHT,FONT=0xe38da8,0xe38da4,0xe38dac,0xf56f8c
BOUNDARIES=[('fallback',0x52e610,0),('text',0x4d6250,9),('first',0x52db10,0),('third',0x51f780,0)]


def packed(value):return(value&0xffffffff).to_bytes(4,'little')


def fixture(label,selected,mode,cell,episodes,changed=None):
    address=(TABLE+((selected-1)&0xffffffff)*98)&0xffffffff if 0<selected<=256 else TABLE
    memory={SELECTED:packed(selected),MODE:packed(mode),X:packed(0xfffffed4),Y:packed(0x12345678),HEIGHT:packed(0x80000020),FONT:packed(0xabcdef12),address:packed(cell)}
    final=dict(memory)
    responses={token:[]for token,_,_ in BOUNDARIES};arguments={token:[]for token,_,_ in BOUNDARIES};order=[];observations=[]
    for token in episodes:
        order.append(token)
        observations.append([[at,len(payload),int.from_bytes(payload,'little')]for at,payload in final.items()])
        mutations=[(address,1,changed)]if token=='first'and changed is not None else[]
        responses[token].append(dict(mutations=mutations))
        arguments[token].append([0x2f,9,0xfffffed4,0x7fffffd4,0x280,0xabcdef12,0,0xfffffed4,0x12345678]if token=='text'else[])
        if mutations:final[address]=bytes([changed])+final[address][1:]
    endpoints=[dict(token=token,native=at,compiled_symbols=[f'FUN_{at:08x}'],arity=arity,native_cleanup=0,compiled_cleanup=0,result_kind='void',observe=[(at,len(payload))for at,payload in memory.items()],responses=responses[token],clobber_ecx=0xdeadbeef,clobber_edx=0xbeefdead)for token,at,arity in BOUNDARIES]
    return dict(id='c8-tooltip-support/'+label,entry=ENTRY,receiver=0x88776655,arguments=[],named_arguments=[],canonical_arguments=[],native_cleanup=0,named_cleanup=0,canonical_cleanup=0,eflags=0xad7,result_mask=None,canonical_registers_and_flags=False,required_retained_route='canonical',routes=['native','canonical'],routes_reason='Execute the canonical nullary C procedure once per image; there is no separately exposed named algorithm ABI.',stores=list(memory.items()),watched=[(at,len(payload))for at,payload in memory.items()],write_ranges=[(address,4)],endpoints=endpoints,expected_endpoint_counts={token:len(responses[token])for token,_,_ in BOUNDARIES},expected_endpoint_arguments=arguments,authored_endpoint_order=order,authored_endpoint_observations=observations,authored_final_memory=[(at,payload.hex())for at,payload in final.items()],expected_invocations=4,initial_fpcw=0x27f,initial_x87=['0000000000000080ff3f','000000000000008000c0'],compare_x87_values=True,compare_x87_control=True)


def cases(registry=None):
    # Explicit expectations follow the native TEST/JG/JGE, byte addressing,
    # nine PUSH arguments and the second MOV AL,[ESI] after the first callback.
    out=[fixture('zero-nonnegative-mode',0,0,0,['fallback']),
        fixture('negative-selected',-1,0,0,['fallback']),
        fixture('signed-minimum-selected',0x80000000,-1,0,['text']),
        fixture('negative-mode-full-geometry',0,-1,0,['text']),
        fixture('signed-minimum-mode',0,0x80000000,0,['text']),
        fixture('positive-mode-high-byte',0,0x100,0,['fallback']),
        fixture('selected-256',256,0,0x88776601,['first']),
        fixture('byte-one-with-nonzero-neighbours',2,0,0x88776601,['first']),
        fixture('byte-three-with-nonzero-neighbours',2,0,0x88776603,['third']),
        fixture('reload-first-to-three',2,0,0x88776601,['first','third'],3),
        fixture('reload-first-to-zero',2,0,0x88776601,['first'],0),
        fixture('unhandled-byte',1,0,0x88776600,[])]
    if registry is not None:
        included={int(m['address'],16)for m in registry['members']}
        out=[c for c in out if c['entry']in included]
    return out
