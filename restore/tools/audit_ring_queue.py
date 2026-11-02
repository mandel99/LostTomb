"""Validate all six relocation-free native ring queue test references."""
import hashlib
import json
from pathlib import Path
import sys
from capstone import Cs, CS_ARCH_X86, CS_MODE_32, CS_GRP_CALL, CS_GRP_JUMP
from capstone.x86_const import X86_OP_IMM, X86_OP_MEM

ROOT=Path(__file__).resolve().parents[1]
EXPECTED='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
RANGES={0x4f6520:0x33,0x4f6560:9,0x4f6570:0x35,0x4f65b0:0x31,0x4f65f0:0x2a,0x4f6630:0x13}

def audit(source):
    data=source.read_bytes()
    assert hashlib.sha256(data).hexdigest()==EXPECTED
    evidence={}
    for file in (ROOT/'raw/rea').glob('batch-*.json'):
        batch=json.loads(file.read_text())
        for item in batch['normalized_result']['items']:
            address=int(item['address'],16)
            if address in RANGES:
                evidence[address]=dict(evidence_id=batch['evidence_id'],file=str(file.relative_to(ROOT)),
                    pseudocode_sha256=hashlib.sha256(item['pseudocode'].encode()).hexdigest())
    cs=Cs(CS_ARCH_X86,CS_MODE_32); cs.detail=True
    routines=[]
    for address,length in RANGES.items():
        code=data[address-0x400000:address-0x400000+length]
        instructions=list(cs.disasm(code,address))
        assert sum(i.size for i in instructions)==length
        assert instructions[-1].mnemonic=='ret'
        cleanup=4 if address in [0x4f6570,0x4f65b0,0x4f65f0] else 0
        for i in instructions:
            assert not i.group(CS_GRP_CALL)
            if i.group(CS_GRP_JUMP):
                assert i.operands[0].type==X86_OP_IMM and address<=i.operands[0].imm<address+length
            if i.mnemonic=='ret': assert (i.operands[0].imm if i.operands else 0)==cleanup
            for operand in i.operands:
                if operand.type==X86_OP_MEM: assert operand.mem.base or operand.mem.index
        routines.append(dict(address=f'0x{address:08x}',length=length,bytes_sha256=hashlib.sha256(code).hexdigest(),
            evidence=evidence[address],abi=dict(object='ECX',stack_argument_bytes=cleanup,callee_cleanup_bytes=cleanup,return_value='EAX'),
            relocation='No calls, absolute memory references, or external branches; full original body copied independently.',
            instructions=[dict(address=hex(i.address),mnemonic=i.mnemonic,operands=i.op_str) for i in instructions]))
    result=dict(original_sha256=EXPECTED,routines=routines,layout=dict(head_offset=4,tail_offset=8,capacity_offset=12,
        first_slot_offset=20,default_capacity=50,object_bytes=220,untouched_words=[0,16]),
        contracts={'0x004f6520':'Sets head/tail to zero and capacity to 50; clears 50 slots; returns 1.',
            '0x004f6560':'Clears tail then head only; returns exact original EAX=0.',
            '0x004f6570':'Searches occupied slots from head to tail with signed wrap comparison; returns 0 or 1.',
            '0x004f65b0':'Reserves one slot to distinguish full/empty; full queue never dereferences source; returns 0 or 1.',
            '0x004f65f0':'Returns next slot or zero at tail; writes cursor before rereading capacity, preserving alias behavior.',
            '0x004f6630':'Returns wrapped tail-head difference; preserves 32-bit x86 arithmetic.'},
        verification=dict(cases_per_routine_per_phase=25000,randomized_comparisons=300000,
            sequence_comparisons=32896,protected_page_comparisons=112,total_comparisons=333008,
            phases=['C adapters before patch','patched game entries'],
            coverage=['Capacities 1 through 50','Empty/full/wrapped queues','Zero and high-bit payloads',
                'All object alignments','Exact return values and full object/guard byte comparisons',
                'Aliased source pointers','Cursor aliasing head, tail, capacity, and data words',
                'Signed count values and unsigned wraparound','64 operation sequences of 256 steps per phase',
                'Unreadable guard pages','Full-queue refusal with unreadable source']))
    (ROOT/'metadata/ring-queue-evidence.json').write_text(json.dumps(result,indent=2)+'\n')
    print('Audited six independent thiscall queue references and their retained REA evidence.')

if __name__=='__main__':
    audit(Path(sys.argv[1]) if len(sys.argv)>1 else Path(r'C:\Program Files (x86)\GOG.com\Pharaoh\Pharaoh.exe'))
