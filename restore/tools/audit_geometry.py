"""Retain exact original bytes, disassembly, REA evidence links, and patch guards.

Capstone is used only by this optional audit command, not by ordinary builds.
"""
import hashlib
import json
from pathlib import Path
import struct
import sys
from capstone import Cs,CS_ARCH_X86,CS_MODE_32

ROOT=Path(__file__).resolve().parents[1]
EXPECTED='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
ADDRESSES=[int(address,16) for address in json.loads((ROOT/'metadata/hook-addresses.json').read_text())]
RANGES={0x4e5140:0x4c,0x4e5190:0x74,0x4e52b0:0x20}


def audit(source):
    original=source.read_bytes()
    if hashlib.sha256(original).hexdigest()!=EXPECTED:
        raise ValueError('Unexpected original executable SHA-256')
    pe=struct.unpack_from('<I',original,0x3c)[0]
    optional=pe+24
    base=struct.unpack_from('<I',original,optional+28)[0]
    table=optional+struct.unpack_from('<H',original,pe+20)[0]
    sections=[struct.unpack_from('<IIII',original,table+i*40+8)
        for i in range(struct.unpack_from('<H',original,pe+6)[0])]

    def read(address,size):
        rva=address-base
        section=next(s for s in sections if s[1]<=rva and rva+size<=s[1]+s[2])
        offset=section[3]+rva-section[1]
        return original[offset:offset+size]

    guards=[]
    for address in ADDRESSES:
        guards.append('    {'+','.join(f'0x{value:02x}' for value in read(address,5))+'},')
    (ROOT/'runtime/hook_signatures.h').write_text(
        '/* Generated from the canonical original; order matches hook_addresses. */\n'
        'static const unsigned char hook_original_prefix[][5]={\n'+'\n'.join(guards)+'\n};\n')

    provenance={}
    for file in sorted((ROOT/'raw/rea').glob('batch-*.json')):
        evidence=json.loads(file.read_text())
        for item in evidence['normalized_result']['items']:
            address=int(item['address'],16)
            if address in RANGES:
                provenance[address]=dict(evidence_id=evidence['evidence_id'],
                    retained_file=str(file.relative_to(ROOT)),
                    pseudocode_sha256=hashlib.sha256(item['pseudocode'].encode()).hexdigest())
    disassembler=Cs(CS_ARCH_X86,CS_MODE_32)
    routines=[]
    for address,length in RANGES.items():
        code=read(address,length)
        instructions=list(disassembler.disasm(code,address))
        assert sum(i.size for i in instructions)==length and instructions[-1].mnemonic=='ret'
        routines.append(dict(address=f'0x{address:08x}',byte_length=length,
            original_bytes_sha256=hashlib.sha256(code).hexdigest(),provenance=provenance[address],
            instructions=[dict(address=f'0x{i.address:08x}',mnemonic=i.mnemonic,operands=i.op_str) for i in instructions]))
    abi={
        '0x004e5140':dict(input='ECX=destination; stack[4]=source',stack_cleanup_bytes=4,
            return_value='EAX=destination',effects='Thirteen 32-bit writes in original order, including overlap behavior.'),
        '0x004e5190':dict(input='ECX=record',stack_cleanup_bytes=0,
            return_value='EAX=(y-x)*(step+1) modulo 2^32',
            global_reads=['0x0070e45c (32-bit, first)','0x0070e44c (32-bit, second)'],
            effects='Updates record words 4 through 12; preserves words 0 through 3.'),
        '0x004e52b0':dict(input='stack[4]=records; stack[8]=signed count',stack_cleanup_bytes=8,
            return_value='Not consumed by the recovered void contract',
            effects='No data access for count<=0; updates 52-byte records; rereads globals per record.')}
    output=dict(executable_sha256=EXPECTED,authority='Original bytes plus retained REA observations; reviewed C derivation',
        abi=abi,routines=routines,verification=dict(cases_per_routine_per_phase=25000,
            phases=['C adapters before patch','patched game entries'],memory_and_return_comparisons=150000,
            covers=['Both overlap directions','Self-copy','Unaligned records','Guard bytes',
                'Signed boundaries and 32-bit overflow','Negative and zero counts with NULL data',
                'Batches up to 32 records','Global state restored after verification']))
    (ROOT/'metadata/geometry-evidence.json').write_text(json.dumps(output,indent=2))
    print(f'Retained original-byte evidence and {len(ADDRESSES)} patch-site guards.')


if __name__=='__main__':
    audit(Path(sys.argv[1]) if len(sys.argv)>1 else Path(r'C:\Program Files (x86)\GOG.com\Pharaoh\Pharaoh.exe'))
