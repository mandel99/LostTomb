"""Extract five audited, self-contained x86 routines for differential testing.

All branches stay within these byte ranges. The routines only read arguments
from the stack, make no calls, and access no game data. No game is launched.
"""
import hashlib
from pathlib import Path
import struct
import sys

EXPECTED = 'fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
ROUTINES = [(0x41ba00,0x1a),(0x41bad0,9),(0x451bc0,0x14),(0x45b490,0x37),(0x45bb30,0x19)]


def generate(source, destination):
    blob = source.read_bytes()
    if hashlib.sha256(blob).hexdigest() != EXPECTED:
        raise ValueError('The original executable does not match the audited Pharaoh 2.1 binary.')
    pe = struct.unpack_from('<I',blob,0x3c)[0]
    if blob[pe:pe+4] != b'PE\0\0':
        raise ValueError('Missing PE signature')
    count = struct.unpack_from('<H',blob,pe+6)[0]
    optional = pe+24
    base = struct.unpack_from('<I',blob,optional+28)[0]
    table = optional+struct.unpack_from('<H',blob,pe+20)[0]
    sections = [struct.unpack_from('<IIII',blob,table+i*40+8) for i in range(count)]
    output = ['/* Generated local fixtures; original machine code must not be redistributed. */']
    for address,size in ROUTINES:
        rva = address-base
        section = next(s for s in sections if s[1]<=rva and rva+size<=s[1]+s[2])
        offset = section[3]+rva-section[1]
        data = blob[offset:offset+size]
        output.append(f'static const unsigned char original_{address:08x}[] = {{'+','.join(f'0x{x:02x}' for x in data)+'};')
    destination.parent.mkdir(parents=True,exist_ok=True)
    destination.write_text('\n'.join(output)+'\n')


if __name__ == '__main__':
    generate(Path(sys.argv[1]),Path(sys.argv[2]))
