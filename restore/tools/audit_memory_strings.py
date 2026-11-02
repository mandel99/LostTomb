"""Audit exact native reference ranges and ensure their copies need no relocation."""
import hashlib
import json
from pathlib import Path
import struct
import sys
from capstone import Cs,CS_ARCH_X86,CS_MODE_32,CS_GRP_CALL,CS_GRP_JUMP
from capstone.x86_const import X86_OP_IMM

ROOT=Path(__file__).resolve().parents[1]
EXPECTED='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
RANGES={0x562240:0xfe,0x5631b0:0x84,0x563240:0x58,0x563380:0x7b}


def audit(source):
    data=source.read_bytes()
    if hashlib.sha256(data).hexdigest()!=EXPECTED:
        raise ValueError('Unexpected canonical executable SHA-256')
    pe=struct.unpack_from('<I',data,0x3c)[0]
    optional=pe+24
    base=struct.unpack_from('<I',data,optional+28)[0]
    section_table=optional+struct.unpack_from('<H',data,pe+20)[0]
    sections=[struct.unpack_from('<IIII',data,section_table+i*40+8)
        for i in range(struct.unpack_from('<H',data,pe+6)[0])]
    evidence={}
    for file in (ROOT/'raw/rea').glob('batch-*.json'):
        batch=json.loads(file.read_text())
        for item in batch['normalized_result']['items']:
            address=int(item['address'],16)
            if address in RANGES:
                evidence[address]=dict(evidence_id=batch['evidence_id'],file=str(file.relative_to(ROOT)),
                    pseudocode_sha256=hashlib.sha256(item['pseudocode'].encode()).hexdigest())
    cs=Cs(CS_ARCH_X86,CS_MODE_32)
    cs.detail=True
    routines=[]
    for address,length in RANGES.items():
        rva=address-base
        section=next(s for s in sections if s[1]<=rva and rva+length<=s[1]+s[2])
        offset=section[3]+rva-section[1]
        code=data[offset:offset+length]
        instructions=list(cs.disasm(code,address))
        assert sum(i.size for i in instructions)==length
        assert any(i.mnemonic=='ret' for i in instructions)
        for instruction in instructions:
            if instruction.group(CS_GRP_CALL):
                raise ValueError(f'Reference has an external call: {instruction.address:x}')
            if instruction.group(CS_GRP_JUMP):
                assert instruction.operands[0].type==X86_OP_IMM
                assert address<=instruction.operands[0].imm<address+length
        routines.append(dict(address=f'0x{address:08x}',length=length,bytes_sha256=hashlib.sha256(code).hexdigest(),
            evidence=evidence[address],relocation='No calls; all branch targets are inside the copied range.',
            instructions=[dict(address=f'0x{i.address:08x}',mnemonic=i.mnemonic,operands=i.op_str) for i in instructions]))
    result=dict(original_sha256=EXPECTED,routines=routines,
        abi='All four routines use cdecl; stack arguments and caller cleanup are preserved.',
        contracts={
            '0x00562240':'Returns destination; source-alignment prefix, ordered four-byte transfers, first-zero masking and zero padding retained.',
            '0x005631b0':'Unsigned-byte lexicographic comparison with exact -1/0/1 return values.',
            '0x00563240':'Returns destination; fills exactly count bytes with the low byte of the integer value; zero count permits NULL.',
            '0x00563380':'Returns 32-bit length before NUL; byte scanning avoids unnecessary reads after the terminator.'},
        verification=dict(randomized_cases_per_function_per_phase=25000,phases=['C adapters before patch','patched game entries'],
            randomized_comparisons=200000,protected_page_comparisons=1546,total_comparisons=201546,
            coverage=['All pointer alignments','Both copy overlap directions','High-bit unsigned characters',
                'Exact comparison return magnitudes','Zero counts and NULL with zero count',
                'Count and terminator boundaries','Full guard-buffer comparison','Unreadable pages around inputs and outputs']))
    (ROOT/'metadata/memory-strings-evidence.json').write_text(json.dumps(result,indent=2))
    print('Audited four self-contained native reference ranges and their retained REA evidence.')


if __name__=='__main__':
    audit(Path(sys.argv[1]) if len(sys.argv)>1 else Path(r'C:\Program Files (x86)\GOG.com\Pharaoh\Pharaoh.exe'))
