"""Keep executable actor code after the fixed legacy data arena in MSVC images.

LINK orders every CNT_CODE section before the arena, even custom code_seg
sections. Retag .zactors as initialized data while preserving EXECUTE/READ.
Instructions, symbols and relocations remain unchanged; the Windows loader
uses the unchanged memory permission flags. Only compiler-owned COFF output
is edited. No original executable or fixed-address game data is modified.
"""
from pathlib import Path
import argparse
import struct


def place_actor_code(path):
    patches = []
    sections = 0
    with path.open("r+b") as stream:
        length = path.stat().st_size
        assert stream.read(8) == b"!<arch>\n", "Expected an MSVC static library"
        position = 8
        while position < length:
            stream.seek(position)
            header = stream.read(60)
            assert len(header) == 60 and header[58:] == b"`\n", "Invalid archive header"
            size = int(header[48:58])
            start = position + 60
            assert size >= 0 and start + size <= length, "Invalid archive member"
            prefix = stream.read(min(size, 56))
            table = count = 0
            if len(prefix) >= 20 and prefix[:2] == b"\x4c\x01":
                count = struct.unpack_from("<H", prefix, 2)[0]
                table = 20 + struct.unpack_from("<H", prefix, 16)[0]
            elif (len(prefix) == 56 and prefix[:4] == b"\0\0\xff\xff"
                  and struct.unpack_from("<H", prefix, 4)[0] == 2
                  and prefix[6:8] == b"\x4c\x01"):
                count = struct.unpack_from("<I", prefix, 44)[0]
                table = 56
            if count:
                assert table + count * 40 <= size, "Invalid COFF section table"
                stream.seek(start + table)
                headers = stream.read(count * 40)
                for index in range(count):
                    offset = index * 40
                    if headers[offset:offset + 8] != b".zactors":
                        continue
                    sections += 1
                    flags = struct.unpack_from("<I", headers, offset + 36)[0]
                    assert flags & 0xe0000000 == 0x60000000, "Expected read/execute actor code"
                    assert flags & 0xe0 in (0x20, 0x40), "Unexpected actor section type"
                    replacement = (flags & ~0x20) | 0x40
                    if replacement != flags:
                        patches.append((start + table + offset + 36, replacement))
            position = start + size + (size & 1)
        assert position == length and sections, "Actor code section is missing"
        for offset, flags in patches:
            stream.seek(offset)
            stream.write(struct.pack("<I", flags))
    print(f"Actor code layout: {sections} executable sections, {len(patches)} retagged.")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("library", type=Path)
    place_actor_code(parser.parse_args().library)
