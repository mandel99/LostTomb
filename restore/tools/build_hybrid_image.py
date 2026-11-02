"""Build a local hybrid PE: native image layout, imported C runtime entry.

Original file bytes are never modified. The generated executable still
contains original machine code and is not a standalone C reconstruction.
"""
import hashlib
import json
from pathlib import Path
import struct
import sys

EXPECTED = 'fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'


def align(value, boundary):
    return (value+boundary-1)//boundary*boundary


def build(source, destination):
    blob = bytearray(source.read_bytes())
    if hashlib.sha256(blob).hexdigest()!=EXPECTED:
        raise ValueError('Input executable SHA-256 does not match the canonical original.')
    pe = struct.unpack_from('<I',blob,0x3c)[0]
    optional = pe+24
    sections = struct.unpack_from('<H',blob,pe+6)[0]
    section_table = optional+struct.unpack_from('<H',blob,pe+20)[0]
    file_alignment = struct.unpack_from('<I',blob,optional+36)[0]
    section_alignment = struct.unpack_from('<I',blob,optional+32)[0]
    header_size = struct.unpack_from('<I',blob,optional+60)[0]
    base = struct.unpack_from('<I',blob,optional+28)[0]
    original_entry = struct.unpack_from('<I',blob,optional+16)[0]
    original_sections = [struct.unpack_from('<IIII',blob,section_table+i*40+8) for i in range(sections)]
    if section_table+(sections+1)*40>header_size or base!=0x400000:
        raise ValueError('Unsupported PE header layout')

    def file_offset(rva):
        for virtual_size,address,raw_size,offset in original_sections:
            if address<=rva<address+raw_size:
                return offset+rva-address
        raise ValueError(f'Unmapped file RVA {rva:x}')

    import_rva,import_size=struct.unpack_from('<II',blob,optional+104)
    import_offset=file_offset(import_rva)
    descriptors=[]
    for i in range(import_size//20):
        descriptor=bytes(blob[import_offset+i*20:import_offset+(i+1)*20])
        if not any(descriptor):
            break
        descriptors.append(descriptor)
    else:
        raise ValueError('Missing import descriptor terminator')
    new_rva=align(max(address+max(virtual_size,raw_size) for virtual_size,address,raw_size,_ in original_sections),section_alignment)
    content=bytearray((len(descriptors)+2)*20)

    def append(data,alignment=1):
        content.extend(b'\0'*(align(len(content),alignment)-len(content)))
        rva=new_rva+len(content)
        content.extend(data)
        return rva

    dll_name=append(b'PharaohRestoreRuntime.dll\0')
    export_name=append(b'\0\0RestoreStart\0',2)
    lookup=append(struct.pack('<II',export_name,0),4)
    iat=append(struct.pack('<II',export_name,0),4)
    entry=append(b'\xff\x25'+struct.pack('<I',base+iat),16)
    for i,descriptor in enumerate(descriptors):
        content[i*20:(i+1)*20]=descriptor
    struct.pack_into('<IIIII',content,len(descriptors)*20,lookup,0,0,dll_name,iat)
    raw_size=align(len(content),file_alignment)
    raw_offset=align(len(blob),file_alignment)
    section=section_table+sections*40
    struct.pack_into('<8sIIIIIIHHI',blob,section,b'.restore',len(content),new_rva,raw_size,raw_offset,0,0,0,0,0xe0000060)
    struct.pack_into('<H',blob,pe+6,sections+1)
    struct.pack_into('<I',blob,optional+16,entry)
    struct.pack_into('<I',blob,optional+56,align(new_rva+len(content),section_alignment))
    struct.pack_into('<I',blob,optional+64,0)  # No signature/checksum is claimed.
    struct.pack_into('<II',blob,optional+104,new_rva,(len(descriptors)+2)*20)
    # Preserve the original IAT directory so Windows makes those read-only
    # .rdata pages writable while binding. The additional IAT is in .restore,
    # whose section is already writable; the complete import table lists it.
    blob.extend(b'\0'*(raw_offset-len(blob)))
    blob.extend(content)
    blob.extend(b'\0'*(raw_size-len(content)))
    destination.parent.mkdir(parents=True,exist_ok=True)
    destination.write_bytes(blob)
    manifest=dict(kind='hybrid-original-image-with-compiled-C-runtime',standalone_c_reconstruction=False,
        original_sha256=EXPECTED,original_entry=f'0x{base+original_entry:08x}',
        original_image_base=f'0x{base:08x}',new_entry=f'0x{base+entry:08x}',
        imported_runtime='PharaohRestoreRuntime.dll!RestoreStart',
        output_sha256=hashlib.sha256(blob).hexdigest(),original_code_fallback=True)
    destination.with_suffix('.manifest.json').write_text(json.dumps(manifest,indent=2))
    print(f'Built hybrid executable: {destination}')


if __name__=='__main__':
    build(Path(sys.argv[1]),Path(sys.argv[2]))
