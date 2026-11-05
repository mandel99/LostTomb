"""Emit the closed, byte-authenticated placement draw ABI repair packet.

The shared C algorithms are reused unchanged. This does not run a game, compile,
or replay machine code. The forwarder rule is reusable only for the explicitly
contracted void draw providers below; arbitrary CALL results are not inferred.
"""
from __future__ import annotations
import argparse
import hashlib
import importlib.util
import json
from pathlib import Path
import re
import sys
import xml.etree.ElementTree as ET

PIN = 'fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
SHARED = {
    0x4e5490: ('src/unit_01970.c', 'restore_placement_member_004e5490'),
    0x545fc0: ('src/unit_02577.c', 'restore_placement_member_00545fc0'),
}
FORWARDERS = {
    0x401420: ('src/unit_00004.c', 0x4e5490, 0x4c, 0x10),
    0x40d870: ('src/unit_00102.c', 0x4e5490, 0x4c, 0x19),
    0x416f90: ('src/unit_00197.c', 0x4e5490, 0x4c, 9),
    0x4e6610: ('src/unit_01996.c', 0x4e5490, 0x4c, 2),
    0x5456e0: ('src/unit_02565.c', 0x545fc0, 0x1e5c, 0xa7),
    0x54f410: ('src/unit_02593.c', 0x545fc0, 0x5c88, 0x201),
    0x555e20: ('src/unit_02606.c', 0x545fc0, 0x4ee0, 0x1b5),
    0x558b80: ('src/unit_02619.c', 0x545fc0, 0x1cbc, 0x9e),
}


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def save(path, value):
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_bytes((json.dumps(value, indent=2) + '\n').encode())


def forwarding_rule(ins):
    """Recognize complete ECX-relative, constant-count, RET0 draw forwarders."""
    from capstone.x86_const import X86_OP_REG, X86_OP_IMM, X86_OP_MEM
    if len(ins) != 5:
        return None
    lea, count, pointer, call, ret = ins
    if [x.mnemonic for x in ins] != ['lea', 'push', 'push', 'call', 'ret'] or ret.op_str:
        return None
    a, b = lea.operands
    if not (a.type == X86_OP_REG and lea.reg_name(a.reg) == 'eax' and
            b.type == X86_OP_MEM and lea.reg_name(b.mem.base) == 'ecx' and
            not b.mem.index and not b.mem.segment and b.size == 4):
        return None
    if not (count.operands[0].type == X86_OP_IMM and
            pointer.operands[0].type == X86_OP_REG and
            pointer.reg_name(pointer.operands[0].reg) == 'eax' and
            call.operands[0].type == X86_OP_IMM):
        return None
    target = call.operands[0].imm & 0xffffffff
    if target not in SHARED:
        return None
    return dict(kind='constant_draw_forwarder', child=target,
                offset=b.mem.disp & 0xffffffff,
                count=count.operands[0].imm & 0xffffffff,
                result='void', native_stack_bytes=0)


def source(address, rule):
    provider = SHARED[rule['child']][1] if rule['kind'] == 'constant_draw_forwarder' else SHARED[address][1]
    lines = ['#include "placement_preview.h"',
             '#if !defined(_MSC_VER) || !defined(_M_IX86)',
             '#error This canonical entry requires the original MSVC i386 member ABI.',
             '#endif',
             '/* Native ECX receiver is explicit at the unchanged C algorithm boundary.',
             '   Logical result is void. Volatile registers and flags are not C outputs. */']
    if rule['kind'] == 'constant_draw_forwarder':
        name = f'restore_placement_draw_{address:08x}'
        lines += [f'void {name}(uint32_t self)', '{',
                  f' {provider}((void *)(uintptr_t)self,',
                  f'  (const void *)(uintptr_t)(self+0x{rule["offset"]:x}u),0x{rule["count"]:x});', '}',
                  f'__declspec(naked) void FUN_{address:08x}(void)', '{', ' __asm {',
                  '  push ecx', f'  call {name}', '  add esp,4', '  ret', ' }', '}', '']
    else:
        name = provider
        lines += [f'__declspec(naked) void FUN_{address:08x}(void)', '{', ' __asm {',
                  '  push dword ptr [esp+8]', '  push dword ptr [esp+8]',
                  '  push ecx', f'  call {provider}', '  add esp,12', '  ret 8', ' }', '}', '']
    return name, '\n'.join(lines).encode()


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--root', type=Path, required=True)
    ap.add_argument('--output', type=Path, required=True)
    args = ap.parse_args()
    root = args.root.resolve()
    out = args.output.resolve()
    spec = importlib.util.spec_from_file_location('bulk_rules', root/'tools/bulk_reconstruct.py')
    bulk = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(bulk)
    sys.path[:0] = [str(root/'build/offline-verification-deps'), str(root/'direct_c')]
    from capstone import Cs, CS_ARCH_X86, CS_MODE_32
    from native_test_pe import PE
    binary = root/'build/smoke-game/Pharaoh.exe'
    if sha(binary) != PIN:
        raise ValueError('Pinned original SHA256 mismatch')
    pe = PE(binary)
    dis = Cs(CS_ARCH_X86, CS_MODE_32)
    dis.detail = True
    entries, conflicts = bulk.catalogue()
    project = root/'build/c-only/pharaoh_restored.vcxproj'
    ns = {'m': 'http://schemas.microsoft.com/developer/msbuild/2003'}
    active = {Path(x.attrib['Include']).resolve() for x in ET.parse(project).findall('.//m:ClCompile[@Include]', ns)}
    members = []
    candidate_hashes = {}
    original_hashes = {}
    for address in sorted(set(SHARED) | set(FORWARDERS)):
        e = entries[address]
        raw = bulk.native_bytes(e)
        if address in conflicts or raw is None or bytes(pe.data[address-pe.base:address-pe.base+len(raw)]) != raw:
            raise ValueError('Conflicting, incomplete or mismatched native body: '+hex(address))
        ins = list(dis.disasm(raw, address))
        if address in FORWARDERS:
            path, child, offset, count = FORWARDERS[address]
            rule = forwarding_rule(ins)
            if rule is None or (rule['child'], rule['offset'], rule['count']) != (child, offset, count):
                raise ValueError('Unsupported native forwarder: '+hex(address))
        else:
            path, provider = SHARED[address]
            if ins[-1].mnemonic != 'ret' or ins[-1].op_str != '8':
                raise ValueError('Shared native member no longer has RET8')
            rule = dict(kind='existing_draw_algorithm_binding', result='void', native_stack_bytes=8)
        original = root/path
        obj = root/'build/c-only/pharaoh_restored.dir/Release'/(original.stem+'.obj')
        if original not in active or not obj.exists() or obj.stat().st_mtime_ns < original.stat().st_mtime_ns or '_FUN_'+f'{address:08x}' not in bulk.defined_symbols(obj):
            raise ValueError('Current active COFF canonical owner unproved: '+path)
        original_text = original.read_text()
        if not re.search(r'\bin_ECX\s*;', original_text) or re.search(r'\bin_ECX\s*=(?!=)', original_text):
            raise ValueError('Legacy unbound-ECX defect no longer present; reuse the repaired owner: '+path)
        name, code = source(address, rule)
        dest = out/path
        dest.parent.mkdir(parents=True, exist_ok=True)
        if dest.exists() and dest.read_bytes() != code:
            raise ValueError('Candidate bytes would change: '+str(dest))
        dest.write_bytes(code)
        candidate_hashes[path] = sha(dest)
        original_hashes[path] = sha(original)
        members.append(dict(address=f'0x{address:08x}', source=path, name=name,
                            canonical_symbol=f'FUN_{address:08x}', logical_signature=(
                                'void cdecl1(uint32_t self)' if address in FORWARDERS else
                                'void cdecl3(void *self,const void *pieces,int32_t count)'),
                            native_bytes=len(raw), native_instructions=len(ins),
                            native_final_mnemonic='ret', native_byte_sha256=hashlib.sha256(raw).hexdigest(),
                            canonical_abi=dict(receiver='ECX', stack_dwords=rule['native_stack_bytes']//4,
                                               callee_cleanup_bytes=rule['native_stack_bytes'], result='void',
                                               nonvolatile_preserved=['EBX', 'EBP', 'ESI', 'EDI'],
                                               observed_flags=False, observed_volatile_register_result=False),
                            repair_kind='Bind actual ECX and native stack cleanup to existing complete C',
                            new_algorithm_count=0, rule=rule,
                            ownership=dict(object_path=obj.relative_to(root).as_posix(), object_sha256=sha(obj),
                                           original_source_sha256=sha(original))))
    reused = ['runtime/placement_preview_members.c', 'runtime/placement_preview_predicates.c', 'include/placement_preview.h']
    save(out/'members.json', dict(members=members))
    save(out/'hashes.json', dict(candidate_sha256=candidate_hashes, original_source_sha256=original_hashes,
                                reused_unchanged_sha256={p: sha(root/p) for p in reused}))
    print(json.dumps(dict(emitted=len(members), native_instructions=sum(m['native_instructions'] for m in members),
                          new_algorithms=0, native_sha256=PIN)))


if __name__ == '__main__':
    main()
