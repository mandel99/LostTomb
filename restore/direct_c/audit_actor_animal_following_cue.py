"""Reproduce five local cue scratch-discard paths; global RAW remains retained.

The frozen bit/flag propagation engine is used with no imported API or callee
summaries. One path enters an authenticated4B1D50 prefix to prove prior-scratch
independence. No game/candidate/platform code is executed.
"""
from pathlib import Path
import argparse,hashlib,json,sys
sys.dont_write_bytecode=True
HERE=Path(__file__).resolve().parent
ROOT=next(p for p in [HERE,*HERE.parents] if (p/'direct_c/native_test_pe.py').exists() and (p/'runtime').is_dir())
DATA=HERE if (HERE/'actor-animal-following-native.json').exists() else ROOT/'metadata'
sys.path[:0]=[str(ROOT/'build/offline-verification-deps'),str(ROOT/'direct_c')]
from native_test_pe import PE
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
from capstone import x86_const as X
NATIVE_SHA='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
native=ROOT/'build/smoke-game/Pharaoh.exe';assert hashlib.sha256(native.read_bytes()).hexdigest()==NATIVE_SHA
pe=PE(native);decoder=Cs(CS_ARCH_X86,CS_MODE_32);decoder.detail=True
paths=[DATA/'actor-animal-following-rea-records.json',DATA/'actor-animal-following-cue-local-rea-records.json',ROOT/'metadata/actor-animal-parent-abi-rea-records.json']
instructions={}
for path in paths:
    for wrapped in json.loads(path.read_text())['records']:
        e=wrapped.get('result',wrapped)
        assert e['authority']=='shipped-artifact' and e['subject']['digest']['sha256']==NATIVE_SHA
        if e['operation']!='read_bytes':continue
        z=e['normalized_result'];assert z['complete'];a=int(z['address'],16);raw=bytes.fromhex(z['bytes_hex'])
        assert raw==bytes(pe.data[a-pe.base:a-pe.base+len(raw)])
        if not 0x400000<=a<0x560000:continue
        for i in decoder.disasm(raw,a):
            if i.address in instructions:assert bytes(instructions[i.address].bytes)==bytes(i.bytes)
            instructions[i.address]=i
bits={}
for whole,half,low,high in [('eax','ax','al','ah'),('ecx','cx','cl','ch'),('edx','dx','dl','dh')]:
    for name,mask in [(whole,0xffffffff),(half,65535),(low,255),(high,0xff00)]:bits[name]=(whole,mask)
flag_names=['CF','PF','AF','ZF','SF','OF'];all_flags=(1<<len(flag_names))-1
opaque_imports={};switch_targets={}

def flags_mask(instruction,prefixes):
    result=0
    for n,flag in enumerate(flag_names):
        if any(instruction.eflags & getattr(X,'X86_EFLAGS_'+prefix+'_'+flag,0) for prefix in prefixes):result|=1<<n
    return result

def audit(entry,initial_masks=(0xffffffff,)*3,initial_flags=all_flags,branch_assumptions=None):
    pending=[(entry+5,initial_masks,initial_flags,())];seen=set();kills=set();unknown=[];inlined_calls=set();mask_kills=set();shared_edx_calls=set();debug_calls=set();arithmetic_propagations=set();assumed_branches=set()
    branch_assumptions=branch_assumptions or {}
    while pending:
        pc,masks,flags,returns=pending.pop()
        if not any(masks) and not flags:
            kills.add((pc,returns));continue
        key=pc,masks,flags,returns
        if key in seen:continue
        seen.add(key)
        if pc not in instructions:
            unknown.append(dict(kind='missing_window',pc=hex(pc),register_masks=[hex(v) for v in masks],flags=flags));continue
        i=instructions[pc];next_pc=pc+i.size
        if i.mnemonic=='call':
            if i.op_str in opaque_imports:
                debug_calls.add((pc,i.op_str));dirty=any(masks) or flags
                output_masks=(0xffffffff if opaque_imports[i.op_str]['return_type']=='VOID' else 0,0xffffffff,0xffffffff) if dirty else (0,0,0)
                pending.append((next_pc,output_masks,all_flags if dirty else 0,returns));continue
            if i.op_str in ['0x4ea9c0','0x4eaec0'] and not masks[0] and not masks[1] and not flags:
                # Existing production proof authenticates classifier EDX
                # transparency and periodic's kill-or-preserve paths. Retain
                # every input EDX bit conservatively, never assume a kill.
                shared_edx_calls.add((pc,int(i.op_str,16)))
                pending.append((next_pc,masks,flags,returns));continue
            if not i.op_str.startswith('0x') or int(i.op_str,16) not in instructions:
                unknown.append(dict(kind='callee_not_authenticated',pc=hex(pc),callee=i.op_str,register_masks=[hex(v) for v in masks],flags=flags));continue
            target=int(i.op_str,16);inlined_calls.add((pc,target))
            pending.append((target,masks,flags,returns+(next_pc,)));continue
        if i.mnemonic=='ret':
            if returns:pending.append((returns[-1],masks,flags,returns[:-1]))
            else:unknown.append(dict(kind='outward_void_return',pc=hex(pc),register_masks=[hex(v) for v in masks],flags=flags))
            continue
        taint=dict(zip(['eax','ecx','edx'],masks))
        reads,writes=i.regs_access()
        lea_taint=None
        if i.mnemonic=='lea' and i.operands[0].type==X.X86_OP_REG and i.operands[1].type==X.X86_OP_MEM:
            destination=bits.get(i.reg_name(i.operands[0].reg))
            if destination:
                memory=i.operands[1].mem;source_taint=0
                for reg,scale in [(memory.base,1),(memory.index,memory.scale)]:
                    source=bits.get(i.reg_name(reg))
                    if source:source_taint|=(taint[source[0]]&source[1])*scale&0xffffffff
                # Addition, constant displacement and scaled addition may
                # propagate a differing bit upward through carries, never
                # downward. Keep that conservative suffix of bits tainted.
                lea_taint=(destination,((0xffffffff<<(source_taint&-source_taint).bit_length()-1)&0xffffffff) if source_taint else 0)
                if source_taint:arithmetic_propagations.add(pc)
        same_register=(len(i.operands)==2 and i.operands[0].type==X.X86_OP_REG and i.operands[1].type==X.X86_OP_REG and i.operands[0].reg==i.operands[1].reg)
        independent_gpr=(same_register and i.mnemonic in ['xor','sub','sbb'])
        masked_read=None
        if i.mnemonic in ['and','or','test'] and len(i.operands)==2 and i.operands[0].type==X.X86_OP_REG and i.operands[1].type==X.X86_OP_IMM:
            name=i.reg_name(i.operands[0].reg);spec=bits.get(name)
            if spec:
                immediate=(i.operands[1].imm<<(8 if name in ['ah','ch','dh'] else 0))&spec[1]
                retained=immediate if i.mnemonic in ['and','test'] else ~immediate&spec[1]
                if not taint[spec[0]]&spec[1]&retained:
                    masked_read=name
                    if taint[spec[0]]&spec[1]:mask_kills.add(pc)
        blocked=[]
        for reg in reads:
            name=i.reg_name(reg);spec=bits.get(name)
            if spec and taint[spec[0]]&spec[1] and not independent_gpr and name!=masked_read and lea_taint is None:blocked.append(name)
        # PRIOR metadata describes preserved bits, not a value consumer. They
        # remain tainted until a later definition; TEST bits are actual inputs.
        used_flags=flags_mask(i,['TEST'])
        if 'eflags' in [i.reg_name(reg) for reg in reads] and not used_flags:used_flags=all_flags
        if flags&used_flags:blocked.append('eflags:'+','.join(flag_names[n] for n in range(len(flag_names)) if flags&used_flags&(1<<n)))
        if blocked:
            unknown.append(dict(kind='scratch_read_before_kill',pc=hex(pc),instruction=i.mnemonic+' '+i.op_str,reads=blocked,register_masks=[hex(v) for v in masks],flags=flags));continue
        for reg in writes:
            # The installed decoder incorrectly lists EAX as written by A9
            # TEST EAX,imm32. Architectural TEST defines flags, not a GPR.
            if i.mnemonic=='test' and i.reg_name(reg)!='eflags':continue
            spec=bits.get(i.reg_name(reg))
            if spec:taint[spec[0]]&=~spec[1]&0xffffffff
        if lea_taint:
            (name,mask),propagated=lea_taint
            taint[name]|=propagated&mask
        flags&=~flags_mask(i,['MODIFY','RESET','SET'])
        masks=tuple(taint.values())
        if i.mnemonic=='jmp':
            if i.op_str.startswith('0x'):pending.append((int(i.op_str,16),masks,flags,returns))
            elif pc in switch_targets:pending.extend((target,masks,flags,returns) for target in switch_targets[pc])
            else:unknown.append(dict(kind='indirect_transfer',pc=hex(pc),instruction=i.mnemonic+' '+i.op_str,register_masks=[hex(v) for v in masks],flags=flags))
        elif i.mnemonic.startswith('j'):
            target=int(i.op_str,16)
            if pc in branch_assumptions:
                selected=branch_assumptions[pc];assert selected in [target,next_pc]
                assumed_branches.add((pc,selected));pending.append((selected,masks,flags,returns))
            else:pending.extend([(target,masks,flags,returns),(next_pc,masks,flags,returns)])
        else:pending.append((next_pc,masks,flags,returns))
    # Preserve each distinct failure/escape rather than repeating loop states.
    unique={json.dumps(item,sort_keys=True):item for item in unknown}
    return dict(states_checked=len(seen),kill_addresses=[dict(pc=hex(pc),within_callee=bool(stack)) for pc,stack in sorted(kills)],
                inlined_calls=[dict(site=hex(pc),target=hex(target)) for pc,target in sorted(inlined_calls)],
                reused_native_EDX_noninterference_calls=[dict(site=hex(pc),target=hex(target)) for pc,target in sorted(shared_edx_calls)],
                opaque_import_calls=[dict(site=hex(pc),target=target) for pc,target in sorted(debug_calls)],
                immediate_mask_kill_addresses=[hex(pc) for pc in sorted(mask_kills)],
                conservative_LEA_taint_propagations=[hex(pc) for pc in sorted(arithmetic_propagations)],
                diagnostic_assumed_branches=[dict(site=hex(pc),selected=hex(target)) for pc,target in sorted(assumed_branches)],unknown=list(unique.values()),checked_instruction_addresses=sorted(hex(key[0]) for key in seen))

def close_parent_graph(items_by_parent,parent_by_ret):
    closed=set(items_by_parent);previous=None
    while previous!=closed:
        previous=set(closed)
        closed={parent for parent in closed if all(all(unknown['kind']=='outward_void_return' and parent_by_ret.get(unknown['pc']) in previous for unknown in result['unknown']) for result in items_by_parent[parent].values())}
    return closed

arguments=argparse.ArgumentParser(description=__doc__)
arguments.add_argument('--scope',choices=['current-batch'],required=True)
options=arguments.parse_args()
current=['0x49da20','0x49e692','0x49ea25','0x49f3dd'];supported=[*current,'0x49c50c']
results={}
for entry in supported:
    pc=int(entry,16);i=instructions[pc]
    assert i.mnemonic=='call' and i.op_str=='0x4b5050'
    result=audit(pc)
    assert not result['unknown'] and not result['opaque_import_calls']
    assert not result['reused_native_EDX_noninterference_calls'] and not result['diagnostic_assumed_branches']
    assert result['inlined_calls']==([dict(site='0x49ea47',target='0x4b1d50')] if entry=='0x49ea25' else [])
    result['unresolved']=[];result['resolved_via_outer_void_parent']=[];results[entry]=result
summary=dict(native_sha256=NATIVE_SHA,passed=False,production_raw_adapter_ready=False,
    selected_scope='current-batch',selected_scope_passed=True,diagnostic_allow_open=False,
    current_batch_callers=current,current_batch_prior_cue_scratch_dead=True,
    supported_typed_callers=supported,caller_results=results,additional_import_contracts=[],
    global_diagnostic=dict(status='OPEN; not recomputed by this local release gate',location='0x5653dd CALL EAX',mutable_handler_slot='0x1266480',global_raw_drop=False),
    source_hashes={p.relative_to(ROOT).as_posix():hashlib.sha256(p.read_bytes()).hexdigest() for p in [*paths,Path(__file__),ROOT/'direct_c/native_test_pe.py']},
    limitations=['Only five directly authenticated post-call paths are analyzed. One path enters exact native4B1D50 bytes until all incoming cue scratch bits are gone; no callee-output equivalence is inferred.',
        'The global caller graph and opaque API/CRT new-handler behavior remain unproved and authorize no RAW cue replacement. Original RAW/model/ROM/reader/prototype must remain unchanged.',
        'No imported API/callee summary or branch assumption is installed. A new call path, missing bytes or surviving scratch read fails this local proof.',
        'Valid ordinary stack/nonvolatile ABI, DF0 and no arbitrary private-stack/code corruption are required.'])
(DATA/'actor-animal-following-cue-caller-audit.json').write_text(json.dumps(summary,indent=2)+'\n')
print('PASS current-batch4 plus explicit existing prey site; global passed=false; RAW cue retained')
