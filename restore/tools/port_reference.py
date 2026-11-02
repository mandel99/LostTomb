"""Read the supplied port's embedded address/name tables; never execute it.

Names and block sizes describe this port, not proven original source types.
No reports or extracted tables are written. This is an analyst tool only;
the reconstructed game does not depend on the port executable.
"""
from pathlib import Path
import argparse
import hashlib
import json
import struct

PORT_SHA256 = "f66a27e62a433e06bdb6b01651e632c8ff6cd1eb9070126f590d49c1bdf8fda1"


class PortReference:
    def __init__(self, path):
        self.raw = Path(path).read_bytes()
        if hashlib.sha256(self.raw).hexdigest() != PORT_SHA256:
            raise ValueError("Port identity differs; its embedded table layout needs review")
        raw = self.raw
        word = lambda offset: struct.unpack_from("<H", raw, offset)[0]
        dword = lambda offset: struct.unpack_from("<I", raw, offset)[0]
        pe = dword(60)
        optional = pe + 24
        if raw[pe:pe + 4] != b"PE\0\0" or word(pe + 4) != 0x14c or word(optional) != 0x10b:
            raise ValueError("Expected the reviewed x86 PE32 image")
        self.base = dword(optional + 28)
        section_table = optional + word(pe + 20)
        self.sections = []
        for index in range(word(pe + 6)):
            offset = section_table + index * 40
            self.sections.append((dword(offset + 12), dword(offset + 8),
                                  dword(offset + 20), dword(offset + 16), dword(offset + 36)))
        self.functions = {}
        self.globals = {}
        # Complete contiguous tables, reviewed in this exact SHA-identified file.
        for offset in range(0x2add50, 0x2bb460, 16):
            original, code, auxiliary, name = struct.unpack_from("<4I", raw, offset)
            if original in self.functions or not 0x401000 <= original < 0x570000:
                raise ValueError("Invalid function table entry")
            if code and not any(self.base + va <= code < self.base + va + size and flags & 0x20000000
                                for va, size, _, _, flags in self.sections):
                raise ValueError("Mapped function is outside port executable sections")
            self.functions[original] = dict(kind="function", name=self.string(name),
                original_address=f"0x{original:08x}", port_address=f"0x{code:08x}" if code else None)
            if auxiliary:
                # The two nonzero fields are not classified as code or flags.
                self.functions[original]["opaque_auxiliary_address"] = f"0x{auxiliary:08x}"
        for offset in range(0x2d5f60, 0x2de8b0, 16):
            original, size, port, name = struct.unpack_from("<4I", raw, offset)
            if original in self.globals or not size or not any(
                self.base + va <= port and port + size <= self.base + va + max(virtual, stored)
                for va, virtual, _, stored, _ in self.sections):
                raise ValueError("Invalid global block table entry")
            self.globals[original] = dict(kind="global-block", name=self.string(name),
                original_address=f"0x{original:08x}", port_address=f"0x{port:08x}", block_bytes=size)
        if len(self.functions) != 3441 or len(self.globals) != 2197:
            raise ValueError("Unexpected embedded table coverage")

    def string(self, address):
        for va, _, offset, stored, flags in self.sections:
            if self.base + va <= address < self.base + va + stored and not flags & 0x80000000:
                start = offset + address - self.base - va
                end = self.raw.find(b"\0", start, min(offset + stored, start + 256))
                if end < 0:
                    raise ValueError("Unterminated reference name")
                name = self.raw[start:end].decode("ascii")
                if not name or not all(32 <= ord(char) < 127 for char in name):
                    raise ValueError("Invalid reference name")
                return name
        raise ValueError("Reference name is not in file-backed read-only data")

    def lookup(self, original):
        return self.functions.get(original) or self.globals.get(original)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("exe", type=Path)
    parser.add_argument("addresses", nargs="*", type=lambda value: int(value, 16))
    args = parser.parse_args()
    reference = PortReference(args.exe)
    print(json.dumps(dict(sha256=PORT_SHA256, functions=len(reference.functions),
        explicit_code_entries=sum(row["port_address"] is not None for row in reference.functions.values()),
        global_blocks=len(reference.globals), authority="port-provided reference; original behavior unproven")))
    for address in args.addresses:
        print(json.dumps(reference.lookup(address) or dict(original_address=f"0x{address:08x}", status="unmapped")))


if __name__ == "__main__":
    main()
