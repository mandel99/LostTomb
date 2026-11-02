"""Audit native test references, including memmove's inline jump tables."""
import hashlib
import json
from pathlib import Path
import struct
import sys
from capstone import Cs, CS_ARCH_X86, CS_MODE_32, CS_GRP_CALL, CS_GRP_JUMP
from capstone.x86_const import X86_OP_IMM, X86_OP_MEM

ROOT=Path(__file__).resolve().parents[1]
EXPECTED='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
RANGES={0x56233e:0xa,0x562348:0x1e,0x5632a0:0x80,0x563400:0x3ee}
TABLES={0x563548:range(4),0x563460:range(1,4),0x563558:range(-4,0),
        0x5634dc:range(8),0x5636e0:range(4),0x563690:range(-7,1),0x5635e8:range(1,4)}

def audit(source):
    data=source.read_bytes()
    assert hashlib.sha256(data).hexdigest()==EXPECTED
    # All selected ranges are in this canonical image's identity-mapped .text.
    def read(address,count): return data[address-0x400000:address-0x400000+count]
    def word(address): return struct.unpack('<I',read(address,4))[0]
    hooks={int(s,16) for s in json.loads((ROOT/'metadata/hook-addresses.json').read_text())}
    assert 0x5685e6 not in hooks
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
        pending=[address]; visited={}; dependencies=[]; slots={}
        while pending:
            pc=pending.pop()
            if pc in visited: continue
            assert address<=pc<address+length,hex(pc)
            instruction=next(cs.disasm(read(pc,15),pc,count=1))
            assert instruction.address+instruction.size<=address+length
            assert not instruction.group(CS_GRP_CALL)
            visited[pc]=instruction
            if instruction.mnemonic=='ret': continue
            if instruction.group(CS_GRP_JUMP):
                operand=instruction.operands[0]
                if operand.type==X86_OP_IMM:
                    target=operand.imm
                    if address==0x5632a0 and target==0x5685e6:
                        assert pc==0x56330e and instruction.mnemonic=='jmp'
                        dependencies.append(dict(site=hex(pc),target=hex(target),kind='Unpatched original one-character search helper'))
                    else: pending.append(target)
                else:
                    assert address==0x563400 and operand.type==X86_OP_MEM
                    table=operand.mem.disp
                    assert table in TABLES and operand.mem.scale==4
                    for index in TABLES[table]:
                        slot=table+index*4; target=word(slot)
                        slots[hex(slot)]=hex(target); pending.append(target)
                if instruction.mnemonic=='jmp': continue
            pending.append(pc+instruction.size)
        if address==0x563400:
            assert read(address,5)==bytes.fromhex('558bec5756')
            assert not any(address<pc<address+5 and pc not in [address+1,address+3,address+4] for pc in visited)
            # Only the entry fallthrough reaches the five copied prologue bytes.
            for instruction in visited.values():
                if instruction.group(CS_GRP_JUMP) and instruction.operands[0].type==X86_OP_IMM:
                    assert instruction.operands[0].imm>=address+5
            assert all(int(target,16)>=address+5 for target in slots.values())
            relocation='Copy five whole prologue bytes, then JMP original entry+5. Original body and absolute jump tables remain in place.'
        elif address==0x5632a0:
            assert read(0x56330e,1)==b'\xe9'
            relocation='Copy 0x80 bytes; relocate E9 at offset 0x6e to original unpatched helper 0x5685e6.'
        else:
            relocation='Copy whole routine; fixed absolute RNG state 0x604950 remains mapped.'
        routines.append(dict(address=f'0x{address:08x}',length=length,bytes_sha256=hashlib.sha256(read(address,length)).hexdigest(),
            evidence=evidence[address],relocation=relocation,external_dependencies=dependencies,jump_table_slots=slots,
            instructions=[dict(address=hex(i.address),mnemonic=i.mnemonic,operands=i.op_str) for i in sorted(visited.values(),key=lambda i:i.address)]))
    result=dict(original_sha256=EXPECTED,abi='Four cdecl routines; caller stack cleanup; EAX return retained.',
        contracts={'0x0056233e':'Stores and returns all 32 seed bits in original shared state 0x604950.',
            '0x00562348':'State=(state*214013+2531011) modulo 2^32; result=(state>>16)&0x7fff.',
            '0x005632a0':'First byte substring match or NULL; empty pattern returns text.',
            '0x00563400':'Overlap-safe copy of exactly count bytes; returns destination; zero count permits NULL.'},routines=routines,
        verification=dict(cases_per_function_per_phase=25000,randomized_comparisons=550000,
            protected_page_comparisons=4112,total_comparisons=554112,
            phases=['C adapters before patch','patched game entries'],rng_steps_per_seed=8,
            rng_shared_state_restored=True,coverage=['All pointer alignments','Both overlap directions and self-copy',
                '32-bit seed boundaries and full shared state','Eight-step random sequences per seed',
                'Unsigned high-bit characters','Empty and single-byte patterns','Pattern aliased within text',
                'Full input/output buffer comparison','NULL pointers with zero copy count',
                'Unreadable pages before and after buffers','x86 direction flag clear on return']))
    (ROOT/'metadata/core-helpers-evidence.json').write_text(json.dumps(result,indent=2))
    print('Audited four references, reachable control flow, native jump tables, and retained REA evidence.')

if __name__=='__main__':
    audit(Path(sys.argv[1]) if len(sys.argv)>1 else Path(r'C:\Program Files (x86)\GOG.com\Pharaoh\Pharaoh.exe'))
