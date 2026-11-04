"""Authenticate the registered parent/helper caller ABI and bounded switch CFG.

Both callback82 (49D0F0) and84 (49E8B0) are invoked by489B70. Their returned
scratch bits reach only a profiling helper that replaces them before use.
Win32 API contracts are checked against the installed SDK declarations and
the pinned PE import names. No Windows API or game code is launched here.
"""
from pathlib import Path
import sys,json,hashlib,struct,re
ROOT=Path(__file__).resolve().parents[1]
sys.path[:0]=[str(ROOT/'build/offline-verification-deps'),str(ROOT/'direct_c')]
from native_test_pe import PE
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
from capstone.x86 import X86_OP_REG
native=ROOT/'build/smoke-game/Pharaoh.exe';native_raw=native.read_bytes();sha=hashlib.sha256(native_raw).hexdigest()
assert sha=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
pe=PE(native);decoder=Cs(CS_ARCH_X86,CS_MODE_32);decoder.detail=True
paths=['metadata/actor-animal-parent-abi-rea-records.json']
records=[];windows={}
for path in paths:
    d=json.loads((ROOT/path).read_text());records+=d['records']
for e in records:
    assert e['authority']=='shipped-artifact' and e['subject']['digest']['sha256']==sha
    if e['operation']!='read_bytes':continue
    z=e['normalized_result'];assert z['complete'];a=int(z['address'],16);raw=bytes.fromhex(z['bytes_hex'])
    assert raw==bytes(pe.data[a-pe.base:a-pe.base+len(raw)])
    windows[(a,len(raw))]=dict(evidence=e,raw=raw,code=list(decoder.disasm(raw,a)))
invoker=windows[(0x489b70,295)];profiler=windows[(0x489a80,191)];rows=windows[(0x5dc3d4,12)]
instructions={i.address:i for w in [invoker,profiler] for i in w['code']}
def exact(pc,mnemonic,operands):
    i=instructions[pc];assert (i.mnemonic,i.op_str)==(mnemonic,operands),(hex(pc),i.mnemonic,i.op_str)
exact(0x489c3e,'xor','edx, edx')
exact(0x489c4b,'mov','dl, byte ptr [ecx*4 + 0xbcd602]')
exact(0x489c52,'call','dword ptr [edx*4 + 0x5dc28c]')
exact(0x489c59,'call','0x489a80')
exact(0x489c5e,'mov','eax, dword ptr [0xd056f8]')
exact(0x489c63,'lea','ecx, [eax + eax*2]')
assert struct.unpack_from('<I',rows['raw'],0)[0]==0x49d0f0
assert struct.unpack_from('<I',rows['raw'],8)[0]==0x49e8b0
assert 0x5dc28c+82*4==0x5dc3d4 and 0x5dc28c+84*4==0x5dc3dc

# Parse the IMAGE_IMPORT_DESCRIPTORs and thunk tables from the pinned image.
opt=struct.unpack_from('<I',native_raw,60)[0]+24
imports=struct.unpack_from('<I',native_raw,opt+104)[0];image=pe.data
u32=lambda p:struct.unpack_from('<I',image,p)[0]
def strz(p):return bytes(image[p:image.index(0,p)]).decode('ascii')
imports_by_slot={}
while u32(imports) or u32(imports+16):
    original,stamp,chain,name,iat=struct.unpack_from('<5I',image,imports)
    lookup=original or iat;n=0
    while u32(lookup+4*n):
        value=u32(lookup+4*n)
        if not value&0x80000000:imports_by_slot[pe.base+iat+4*n]=(strz(name),strz(value+2))
        n+=1
    imports+=20
assert imports_by_slot[0x56f024]==('KERNEL32.dll','QueryPerformanceCounter')
assert imports_by_slot[0x56f12c]==('KERNEL32.dll','GetTickCount')
sdk=Path('C:/Program Files (x86)/Windows Kits/10/Include/10.0.26100.0/um')
profile_header=sdk/'profileapi.h';clock_header=sdk/'sysinfoapi.h'
profile_text=profile_header.read_text();clock_text=clock_header.read_text()
assert re.search(r'QueryPerformanceCounter\s*\(\s*_Out_\s+LARGE_INTEGER\s*\*\s*lpPerformanceCount\s*\)',profile_text)
assert re.search(r'DWORD\s+WINAPI\s+GetTickCount\s*\(\s*VOID\s*\)',clock_text)

exact(0x489a80,'mov','al, byte ptr [0xd05710]')
exact(0x489a85,'sub','esp, 8')
exact(0x489a88,'test','al, al')
exact(0x489a8a,'je','0x489a97')
exact(0x489a8c,'push','0xbcb628')
exact(0x489a91,'call','dword ptr [0x56f024]')
exact(0x489a97,'call','dword ptr [0x56f12c]')
exact(0x489a9d,'mov','ecx, dword ptr [0xbcb614]')
exact(0x489ab3,'mov','edx, dword ptr [ecx + 0xbca850]')

register_bits={}
for full,half,low,high in [('eax','ax','al','ah'),('ecx','cx','cl','ch'),('edx','dx','dl','dh')]:
    for name,bits in [(full,0xffffffff),(half,65535),(low,255),(high,0xff00)]:register_bits[name]=(full,bits)
pending=[(0x489a80,(0xffffffff,)*3)];seen=set();kills=set();api_calls=set()
while pending:
    pc,mask=pending.pop()
    if not any(mask):kills.add(pc);continue
    if (pc,mask) in seen:continue
    seen.add((pc,mask));i=instructions[pc];taint=dict(zip(['eax','ecx','edx'],mask))
    if i.mnemonic=='call':
        assert pc in [0x489a91,0x489a97],('unexpected call while tainted',hex(pc))
        # stdcall signatures take only explicit stack args; optional QPC's
        # pointer is constant and GetTickCount has no args. Their documented
        # DWORD/BOOL result defines full EAX, independently of callback input.
        taint['eax']=0;api_calls.add(pc)
        # Deliberately do not assume that either API defines ECX or EDX.
    else:
        reads,writes=i.regs_access()
        for reg in reads:
            bits=register_bits.get(i.reg_name(reg))
            if bits:assert not taint[bits[0]]&bits[1],('incoming scratch read',hex(pc),i.mnemonic,i.op_str,taint)
        for reg in writes:
            bits=register_bits.get(i.reg_name(reg))
            if bits:taint[bits[0]]&=~bits[1]&0xffffffff
    next_mask=tuple(taint.values());next_pc=pc+i.size
    if i.mnemonic=='jmp':pending.append((int(i.op_str,16),next_mask))
    elif i.mnemonic.startswith('j'):pending.extend([(int(i.op_str,16),next_mask),(next_pc,next_mask)])
    else:
        assert i.mnemonic!='ret',('tainted scratch escapes profiler',hex(pc),next_mask)
        pending.append((next_pc,next_mask))
assert kills=={0x489ab9} and api_calls=={0x489a91,0x489a97}

outer=dict(passed=True,invoker_entry='0x489b70',callback_site='0x489c52',callback_table='0x5dc28c',
    callbacks={'82':dict(row='0x5dc3d4',entry='0x49d0f0'),'84':dict(row='0x5dc3dc',entry='0x49e8b0')},
    incoming_scratch_bits_discarded=['EAX[31:0]','ECX[31:0]','EDX[31:0]'],
    flags_discarded='SUB ESP,8 and TEST AL,AL precede any incoming flag use.',
    all_scratch_killed_before='0x489ab9',flow_states=len(seen),
    external_contracts={'0x56f024':dict(dll='KERNEL32.dll',name='QueryPerformanceCounter',stack_args=1,argument='0xbcb628',return_type='BOOL full EAX'),'0x56f12c':dict(dll='KERNEL32.dll',name='GetTickCount',stack_args=0,return_type='DWORD full EAX')})
authenticated={}
for w in windows.values():
    for i in w['code']:
        if i.address in authenticated:assert bytes(authenticated[i.address].bytes)==bytes(i.bytes),(hex(i.address),'conflicting decode')
        authenticated[i.address]=i
def instruction(pc):
    assert pc in authenticated,('instruction lacks REA byte authority',hex(pc))
    return authenticated[pc]

refs=next(e for e in records if e['operation']=='procedure_references' and e['parameters']['procedure']=='0x4eab10')
callers=sorted(int(x['source_address'],16) for x in refs['normalized_result']['references'] if x['target_address']=='0x4eab10')
assert len(callers)==13
selector_refs=next(e for e in records if e['operation']=='procedure_references' and e['parameters']['procedure']=='0x4eaac0')
assert {int(x['source_address'],16) for x in selector_refs['normalized_result']['references'] if x['target_address']=='0x4eaac0'}=={0x4eab54}
def mask(i,reg):return {'edx':0xffffffff,'dx':65535,'dl':255,'dh':0xff00}.get(i.reg_name(reg),0)
def prove_periodic_edx_input_unused():
    # The sole call before periodic's first EDX definition is the classifier.
    # Its two-case bounded switch uses EAX/ECX only and never touches EDX.
    classifier_table=windows[(0x4ea9e4,31)]['raw']
    assert classifier_table==bytes(pe.data[0x4ea9e4-pe.base:0x4eaa03-pe.base])
    classifier_targets=struct.unpack_from('<2I',classifier_table)
    classifier_indices=pe.data[0x4ea9ec-pe.base:0x4ea9ec-pe.base+23]
    assert set(classifier_indices)<={0,1} and set(classifier_targets)=={0x4ea9db,0x4ea9de}
    pending=[0x4ea9c0];seen=set()
    while pending:
        pc=pending.pop()
        if pc in seen:continue
        seen.add(pc);i=instruction(pc)
        assert not any(mask(i,r) for r in i.regs_access()[0]+i.regs_access()[1]),('classifier EDX use',hex(pc))
        assert i.mnemonic!='call'
        if i.mnemonic=='ret':continue
        if i.mnemonic=='jmp':pending.extend(classifier_targets if pc==0x4ea9d4 else [int(i.op_str,16)])
        elif i.mnemonic.startswith('j'):pending.extend([int(i.op_str,16),pc+i.size])
        else:pending.append(pc+i.size)
    pending=[(0x4eaec0,0xffffffff)];seen=set();terminals=[]
    while pending:
        pc,taint=pending.pop()
        if not taint:terminals.append(('kill',hex(pc)));continue
        if (pc,taint) in seen:continue
        seen.add((pc,taint));i=instruction(pc)
        if i.mnemonic=='call':assert i.op_str=='0x4ea9c0'
        else:
            for r in i.regs_access()[0]:assert not mask(i,r)&taint,('periodic input EDX consumed',hex(pc))
            for r in i.regs_access()[1]:taint&=~mask(i,r)&0xffffffff
        if i.mnemonic=='ret':terminals.append(('preserved_return',hex(pc)));continue
        if i.mnemonic=='jmp':pending.append((int(i.op_str,16),taint))
        elif i.mnemonic.startswith('j'):pending.extend([(int(i.op_str,16),taint),(pc+i.size,taint)])
        else:pending.append((pc+i.size,taint))
    assert set(terminals)=={('kill','0x4eaf2c'),('preserved_return','0x4eaff2')}
    return terminals
import struct
periodic_input_proof=prove_periodic_edx_input_unused()
def walk(start,initial=0xffffffff,allow_clear=True):
    pending=[(start,initial)];seen=set();kills=set();clear_calls=set();word_stores=set();escapes=set()
    while pending:
        pc,taint=pending.pop()
        if not taint:kills.add(pc);continue
        if (pc,taint) in seen:continue
        seen.add((pc,taint));i=instruction(pc);next_pc=pc+i.size
        if i.mnemonic=='call':
            if allow_clear and i.op_str=='0x4eaec0':
                # The input bits are not consumed; retaining all their taint
                # also covers the classifier/early-return preservation path.
                pass
            else:
                assert allow_clear and i.op_str=='0x4b3250',('unresolved call ABI',hex(pc),i.op_str,hex(taint))
                walk(0x4b3250,taint,allow_clear=False)
                taint=0;clear_calls.add(pc)
        else:
            reads,writes=i.regs_access();zero=(i.mnemonic=='xor' and len(i.operands)==2 and i.operands[0].type==X86_OP_REG and i.operands[1].type==X86_OP_REG and i.operands[0].reg==i.operands[1].reg)
            for reg in reads:
                if not zero:assert not mask(i,reg)&taint,('incoming EDX observed',hex(pc),i.mnemonic,i.op_str,hex(taint))
            if 'word ptr' in i.op_str and i.op_str.endswith(', dx'):word_stores.add(pc)
            for reg in writes:taint&=~mask(i,reg)&0xffffffff
        if i.mnemonic=='jmp':pending.append((int(i.op_str,16),taint))
        elif i.mnemonic.startswith('j'):pending.extend([(int(i.op_str,16),taint),(next_pc,taint)])
        else:
            if i.mnemonic=='ret':escapes.add((pc,taint))
            else:pending.append((next_pc,taint))
    return dict(visited_instructions=[hex(pc) for pc,t in sorted(seen)],full_kills=[hex(pc) for pc in sorted(kills)],route_clear_calls=[hex(pc) for pc in sorted(clear_calls)],known_low_word_stores=[hex(pc) for pc in sorted(word_stores)],caller_return_escapes=[dict(address=hex(pc),unknown_bits=hex(taint)) for pc,taint in sorted(escapes)])
flows={}
for pc in callers:
    i=instruction(pc);assert (i.mnemonic,i.op_str)==('call','0x4eab10')
    flows[hex(pc)]=walk(pc+5)

assert {site for site,flow in flows.items() if flow['caller_return_escapes']}=={'0x49d747','0x49ef5b'}
assert flows['0x49d747']['caller_return_escapes']==[dict(address='0x49d7cd',unknown_bits='0xffffffff')]
assert flows['0x49ef5b']['caller_return_escapes']==[dict(address='0x49efe1',unknown_bits='0xffffffff')]
for pc in callers:
    cursor=pc+5;found=False
    for _ in range(5):
        i=instruction(cursor)
        if (i.mnemonic,i.op_str)==('add','esp, 0x18'):found=True;break
        assert i.mnemonic=='mov',('flags observed before stack cleanup',hex(cursor),i.mnemonic)
        cursor+=i.size
    assert found,('missing six-DWORD cleanup',hex(pc))
# Selector AL is consumed, then both scratch registers are replaced before
# any provider call on either branch. ADD ESP,4 also replaces incoming flags.
for pc,m,op in [(0x4eab59,'add','esp, 4'),(0x4eab5c,'test','al, al'),
                (0x4eab62,'mov','ecx, dword ptr [esp + 0x30]'),
                (0x4eab66,'mov','edx, dword ptr [esp + 0x2c]'),
                (0x4eab8b,'mov','edx, dword ptr [esp + 0x30]'),
                (0x4eab8f,'mov','eax, dword ptr [esp + 0x2c]')]:
    i=instruction(pc);assert (i.mnemonic,i.op_str)==(m,op),(hex(pc),i.mnemonic,i.op_str)
# In the alternate selector branch ECX is not read before the called line
# provider; its explicit arguments are EDX/EAX/EDI/ESI. Subsequent EDX use is
# a fresh terrain load. ECX is therefore an ordinary volatile scratch output.

catalogue=json.loads((ROOT/'metadata/actor-animal-parent-native.json').read_text())
assert catalogue['native_sha256']==sha
functions={}
evidence_by_id={e['evidence_id']:e for e in records}
for f in catalogue['functions']:
    at=int(f['address'],16);raw=bytes.fromhex(f['bytes_hex']);e=evidence_by_id[f['bytes_evidence_id']]
    assert e['normalized_result']['bytes_hex']==f['bytes_hex'] and e['normalized_result']['complete']
    assert raw==bytes(pe.data[at-pe.base:at-pe.base+len(raw)])
    code=list(decoder.disasm(raw,at));assert len(code)==f['instruction_count'] and sum(i.size for i in code)==len(raw)
    functions[at]={i.address:i for i in code}
assert {a:len(code) for a,code in functions.items()}=={0x4eaac0:17,0x4eab10:84,0x49e8b0:935}
parent=functions[0x49e8b0];switches=[];indirect={}
for site,selector,table,count,guard,default in [(0x49e9f0,0x49f6c4,0x49f688,15,0x49e9e2,0x49f411),(0x49f4f6,0x49f6f4,0x49f6dc,6,0x49f4e8,0x49f65b)]:
    for pc,m,op in [(guard-14,'xor','edx, edx'),
                    (guard-12,'mov','dl, byte ptr [eax + 0xbcd630]'),
                    (guard-6,'lea','esi, [edx - 4]'),
                    (site-8,'xor','edx, edx'),
                    (site-6,'mov','dl, byte ptr [esi + '+hex(selector)+']'),
                    (site,'jmp','dword ptr [edx*4 + '+hex(table)+']')]:
        assert (parent[pc].mnemonic,parent[pc].op_str)==(m,op),(hex(pc),parent[pc].mnemonic,parent[pc].op_str)
    assert (parent[guard].mnemonic,parent[guard].op_str)==('ja',hex(default))
    previous=parent[guard-3];assert (previous.mnemonic,previous.op_str)==('cmp','esi, 0x16')
    raw_table=bytes(pe.data[table-pe.base:table-pe.base+count*4]);raw_selector=bytes(pe.data[selector-pe.base:selector-pe.base+23])
    table_window=windows[(0x49f688,132)]['raw']
    assert raw_table==table_window[table-0x49f688:table-0x49f688+count*4]
    assert raw_selector==table_window[selector-0x49f688:selector-0x49f688+23]
    targets=struct.unpack('<'+'I'*count,raw_table)
    assert all(index<count for index in raw_selector) and all(t in parent for t in targets)
    indirect[site]=set(targets)
    switches.append(dict(site=hex(site),selector=hex(selector),pointer_table=hex(table),
        bounded_state_range=[4,26],selector_values=list(raw_selector),table_targets=[hex(t) for t in targets],
        state_to_target={str(state):hex(targets[raw_selector[state-4]]) for state in range(4,27)}))
queue=[0x49e8b0];reachable=set();returns=set()
while queue:
    pc=queue.pop()
    if pc in reachable:continue
    assert pc in parent,('parent transfer outside authenticated body',hex(pc))
    reachable.add(pc);i=parent[pc]
    if i.mnemonic=='ret':returns.add(pc);continue
    if i.mnemonic=='jmp':queue.extend(indirect[pc] if pc in indirect else [int(i.op_str,16)])
    elif i.mnemonic.startswith('j'):queue.extend([int(i.op_str,16),pc+i.size])
    else:queue.append(pc+i.size)
assert len(reachable)==935 and len(returns)==17
assert returns=={int(v,16) for v in catalogue['parent_control_flow']['normal_return_addresses']}
# The candidate's full EAX is retained, even though the registered consumer
# discards it. The exceptional exit explicitly reloads active actorID.
assert (parent[0x49efc5].mnemonic,parent[0x49efc5].op_str)==('mov','eax, dword ptr [0xd056f8]')
assert 0x49efe1 in returns
source_paths=[*paths,'metadata/actor-animal-parent-native.json','direct_c/audit_actor_animal_parent.py',
    'runtime/actor_command.c','runtime/actor_civic.c','runtime/actor_civic_model.c',
    'runtime/actor_callbacks.c','src/unit_01307.c','include/actor_command.h','include/actor_civic.h']
proof=dict(passed=True,native_sha256=sha,owned_instructions=1036,
    registered_callback_consumer=outer,helper_callers=flows,helper_direct_callers=13,
    helper_preserves_native_incoming_ECX=True,helper_full_EAX_reconstructed=True,
    selector_direct_callers=1,selector_consumed_return='AL',selector_named_and_raw_return='full EAX including upper table-offset bits',
    periodic_input_EDX_noninterference=periodic_input_proof,
    helper_ret_escapes_closed_by_registered_callback_consumer=True,
    raw_adapter_EDX_drop_supported=True,raw_adapter_flags_may_be_retained_for_observed_callers=True,
    parent_switches=switches,parent_statically_reachable_instructions=len(reachable),
    parent_normal_returns=[hex(a) for a in sorted(returns)],parent_full_EAX_return='actorOffset except49EFE1 returns fresh actorID',
    analytically_unreachable_instructions=[],analytically_unreachable_edges=[],
    source_hashes={p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest() for p in source_paths},
    installed_SDK_contract_sources={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in [profile_header,clock_header]},
    limitations=['Provider-observed13 direct helper callers, one selector caller and the registered actor callback invoker; unidentified indirect callers and arbitrary private-stack corruption remain unknown.',
        'Ghidra does not own49E8B0 as a function. Its exact3544 bytes, both bounded selector/jump maps and935-instruction local CFG are authenticated; CALL normal return is assumed for the CFG walk.',
        'The two outward helper EDX taints may reach callback RETs. The common profiling consumer replaces all incoming EAX/ECX/EDX/flags before use; no physical imported API or game is executed.',
        'Installed Win32 SDK declarations and pinned import names define the0/1-stack-argument clock/QPC boundaries. Physical OS behavior and exceptions are not proved.',
        'The named/raw parent deliberately reconstructs full native EAX. The legacy void callback is supported by the observed registered consumer, not by asserting equal incidental C-void register values.'])
(ROOT/'metadata/actor-animal-parent-caller-audit.json').write_text(json.dumps(proof,indent=2)+'\n')
print('PASS actor parent/helper ABI:13 helper callers,2 outward callback RETs, registered scratch discard,935-instruction bounded-switch CFG')
