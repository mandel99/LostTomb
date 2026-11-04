"""Prepare isolated C translation units from retained decompiler output.

This is a compilation model, not an ABI-correct replacement for the game.
The untouched input and every mechanical transformation remain reviewable.
"""
import argparse
import json
import hashlib
import re
from pathlib import Path
from generate_intrinsics import generate_intrinsics
from repair_discarded_returns import repair as repair_discarded_return_guards
from repair_member_receivers import repair as repair_member_receivers

ROOT = Path(__file__).resolve().parents[1]


def split_export(text):
    # Ghidra's exporter puts the function opening brace at column zero.
    pattern = re.compile(r"^([A-Za-z_][\w *]+?)(?:\s+|(?<=\*))([\w@$?]+)\s*(\([^;{}]*?\))\s*\n\s*\{", re.M)
    items = []
    for m in pattern.finditer(text):
        end = text.find("\n}", m.end())
        if end == -1:
            raise ValueError(f"Unterminated function {m[2]}")
        items.append(dict(name=m[2], signature=m[0][:m[0].rfind('{')].strip(),
                          code=text[m.start():end + 2], returns=m[1].strip()))
    return items


def prepare(path):
    original = path.read_text(encoding="utf-8-sig")
    functions = split_export(original)
    if not functions:
        raise ValueError("No functions in input")
    is_rea = path.name == 'Pharaoh.rea.c'
    first = original.index(functions[0]["code"])
    preamble = original[:first]
    if is_rea:
        globals = json.loads((ROOT/'metadata/global_types.json').read_text())
    else:
        types_end = preamble.index("undefined DAT_")
        types = preamble[:types_end]
        declarations = preamble[types_end:]
        globals = {}
        for line in declarations.splitlines():
            m = re.match(r"(.+?)\s+(\w+)\s*;", line.strip())
            if m:
                globals[m[2]] = m[1]
    # Native file-descriptor lookup uses 64 DWORD pointer slots and a DWORD
    # total count. A scalar-byte hint truncates both (debug-session.json).
    globals.update({'DAT_01266960': 'unsigned int', 'DAT_01266a60': 'unsigned int'})
    globals.update({name: 'unsigned int' for name in ('DAT_00e20764','DAT_00e20768','DAT_00e36c70','DAT_00e36c74')})
    globals['DAT_00e92e1c'] = 'unsigned int'  # shared frame-buffer pointer
    globals['DAT_00e92dfc'] = 'unsigned int'  # DWORD pixel-format selector
    globals.update({'DAT_012663ac': 'unsigned int', 'DAT_012663b0': 'unsigned int'})  # sound object/counter
    globals.update({'DAT_01266a90': 'unsigned int', 'DAT_01267aa0': 'int'})
    if is_rea:
        types = '''
#include <windows.h>
#include <stddef.h>
#include <stdio.h>
typedef unsigned char undefined;
typedef unsigned char byte;
typedef unsigned int uint;
typedef unsigned short ushort;
typedef unsigned char uchar;
typedef unsigned short word;
typedef unsigned int dword;
typedef unsigned short wchar16;
typedef unsigned char CRect;
typedef unsigned char CAtlWinModule;
typedef RECT tagRECT;
typedef MSG tagMSG;
typedef POINT tagPOINT;
typedef SIZE tagSIZE;
typedef struct _WIN32_FIND_DATAA _WIN32_FIND_DATAA;
typedef OSVERSIONINFOA _OSVERSIONINFOA;
typedef TIME_ZONE_INFORMATION _TIME_ZONE_INFORMATION;
typedef SYSTEMTIME _SYSTEMTIME;
typedef STARTUPINFOA _STARTUPINFOA;
typedef SECURITY_ATTRIBUTES _SECURITY_ATTRIBUTES;
typedef EXCEPTION_POINTERS _EXCEPTION_POINTERS;
typedef CPINFO _cpinfo;
extern void *restore_type_info_vftable;
/* Original pre-UCRT stream layout; keep it separate from the host CRT. */
typedef struct {
    char *_ptr;
    int _cnt;
    char *_base;
    int _flag;
    int _file;
    int _charbuf;
    int _bufsiz;
    char *_tmpfname;
} restore_legacy_FILE;
'''
    # Keep the original integer widths and use explicit image-relative lvalues.
    base = """#ifndef RESTORE_TYPES_H
#define RESTORE_TYPES_H
typedef unsigned char undefined1;
typedef unsigned short undefined2;
typedef unsigned int undefined4;
typedef unsigned int undefined3;
typedef unsigned int uint3;
typedef int int3;
typedef unsigned __int64 undefined8;
typedef unsigned __int64 undefined6;
typedef unsigned __int64 ulonglong;
typedef __int64 longlong;
typedef long double float10;
typedef unsigned int pointer32;
typedef unsigned char bool;
typedef int code();
extern int restore_interrupt3();
extern void *ExceptionList;
extern unsigned int restore_unresolved_return(unsigned int address);
typedef struct { unsigned int value:24; } restore_u24;
#define RESTORE_SLICE1(v,o) (*(unsigned char *)((unsigned char *)&(v)+(o)))
#define RESTORE_SLICE2(v,o) (*(unsigned short *)((unsigned char *)&(v)+(o)))
#define RESTORE_SLICE3(v,o) (((restore_u24 *)((unsigned char *)&(v)+(o)))->value)
#define RESTORE_SLICE4(v,o) (*(unsigned int *)((unsigned char *)&(v)+(o)))
#define RESTORE_SLICE8(v,o) (*(unsigned __int64 *)((unsigned char *)&(v)+(o)))
#define true 1
#define false 0
""" + types + '\n#include "restore_intrinsics.h"\n#endif\n'
    intrinsics, intrinsic_names = generate_intrinsics(original)
    (ROOT/'include/restore_intrinsics.h').write_text(intrinsics)
    if not is_rea:
        base = base.replace('#define true', '#define __thiscall\n#define __fastcall\n#define __stdcall\n#define __cdecl\n#define true')
    (ROOT / "include/restore_types.h").write_text(base, encoding="utf-8")
    names = {f["name"] for f in functions}
    return_types = {f["name"]: re.sub(r"__(?:thiscall|fastcall|stdcall|cdecl)", "", f["returns"]).strip() for f in functions}
    if is_rea:
        return_types = {name: re.sub(r'\bFILE\b','restore_legacy_FILE',dtype) for name,dtype in return_types.items()}
    report = []
    provenance = {}
    if is_rea:
        provenance = {'FUN_'+f'{int(p["address"],16):08x}':p
            for p in json.loads((ROOT/'metadata/export_manifest.json').read_text())}
        missing = set(provenance)-names
        if missing:
            raise ValueError(f'Procedure bodies missing from parsed input: {sorted(missing)}')
    for i, f in enumerate(functions):
        code = f["code"]
        if is_rea:
            code = re.sub(r'\bFILE\b|struct _iobuf\b','restore_legacy_FILE',code)
        code = re.sub(r'\b__(?:thiscall|fastcall)\s+', '', code)
        code = code.replace('u___4<D_005c9b4a', 's_restore_005c9b4a')
        code = code.replace('type_info::vftable', 'restore_type_info_vftable')
        code = re.sub(r'\bint3(?=\s*;)', 'restore_interrupt3',code)
        code = re.sub(r'\bint3(?=\s*\()', 'restore_interrupt3',code)
        # Anonymous exporter namespace labels are not C identifiers.
        code = re.sub(r"\b(\w+)@([0-9a-f]+)\b", r"\1_\2", code)
        code = re.sub(r"\b(\w+(?:\[[^\]\n]+\])?)\._(\d+)_(1|2|3|4|8)_", lambda m:f'RESTORE_SLICE{m[3]}({m[1]},{m[2]})',code)
        uses = set(re.findall(r"\b_?(?:DAT|UNK|PTR(?:_[A-Za-z_0-9]*?)?|s_[A-Za-z_0-9]*?)_[0-9a-f]{8}\b", code))
        prefix = ['#include "restore_types.h"']
        # A LAB token can be a local jump label or an address-taken entry.
        # Only address-taken labels need a memory lvalue declaration.
        for g in sorted(set(re.findall(r'&\s*(LAB_[0-9a-f]{8})\b',code))):
            prefix.append(f'#define {g} (*(undefined1 *)0x{g[4:]}u)')
        for g in sorted(set(re.findall(r'\bFUN_[0-9a-f]{8}\b',code))-names):
            prefix.append(f'extern unsigned int {g}();')
        for g in sorted(set(re.findall(r'\b(?:[ciu]Ram[0-9a-f]{8}|BYTE_[0-9a-f]{8}|DAT_[0-9a-f]{8}_\d+)\b',code))):
            addr = re.search(r'[0-9a-f]{8}',g)[0]
            dtype = 'char' if g.startswith(('cRam','BYTE')) else 'int' if g.startswith('iRam') else 'undefined4'
            prefix.append(f'#define {g} (*({dtype} *)0x{addr}u)')
        for g in sorted(set(re.findall(r'\bram0x[0-9a-f]{8}\b',code))):
            prefix.append(f'#define {g} (*(undefined4 *)0x{g[5:]}u)')
        for g in sorted(set(re.findall(r'\b\w+_exref\b',code))):
            prefix.append(f'extern code {g};')
        for g in sorted(set(re.findall(r'\bregister0x[0-9a-f]+\b',code))):
            prefix.append(f'extern unsigned int {g};')
        for g in sorted(uses):
            addr = g.rsplit('_', 1)[1]
            dtype = globals.get(g, globals.get(g.lstrip('_'), 'undefined4'))
            if g.startswith('PTR_'):
                dtype = 'unsigned char *'
            elif g.startswith('s_'):
                # A string label denotes storage, not the first character.
                # An incomplete array preserves decay and address-of without
                # inventing a size that the export has not established.
                prefix.append(f'#define {g} (*(char (*)[])0x{addr}u)')
                continue
            if g.startswith('_') and dtype in ('undefined', 'undefined1', 'char', 'byte'):
                dtype = 'undefined4'
            prefix.append(f'#define {g} (*({dtype} *)0x{addr}u)')
        # Forward references also include function addresses used as callbacks.
        used_functions = set(re.findall(r"\b[a-zA-Z_]\w*\b", code)) & names
        for callee in sorted(used_functions - {f['name']}):
            prefix.append(f"extern {return_types[callee]} {callee}();")
        # Preserve a void call's side effects, but trap if the decompiler also
        # consumes an unspecified return register. Never invent a zero result.
        void_results = 0
        for m in reversed(list(re.finditer(r'\b([A-Za-z_]\w*)\s*\(',code))):
            if return_types.get(m[1]) != 'void' or m[1] == f['name']:
                continue
            start = m.start()
            previous = code[max(code.rfind(';',0,start),code.rfind('{',0,start),code.rfind('}',0,start))+1:start].strip()
            if not previous:
                continue
            depth, end = 1, m.end()
            while end < len(code) and depth:
                depth += (code[end] == '(') - (code[end] == ')')
                end += 1
            addr = re.search(r'([0-9a-f]{8})$',m[1])
            address = f'0x{addr[1]}u' if addr else '0'
            code = code[:start] + '(' + code[start:end] + f', restore_unresolved_return({address}))' + code[end:]
            void_results += 1
        # Ghidra can print a scalar address as a pointer before refining its type.
        code = re.sub(r'\*(\b_?DAT_[0-9a-f]{8}\b)',r'*(undefined4 *)(\1)',code)
        code = re.sub(r'\(\*\(undefined4 \*\)\((\w+)\)\)\)\(',r'((code *)\1)(',code)
        code = re.sub(r'\(\*\(undefined4 \*\)\((\w+)\)\)\s*\(',r'((code *)\1)(',code)
        code = re.sub(r'^\s*([A-Za-z_]\w*):\s*$',r'\1: ;',code,flags=re.M)
        code = re.sub(r'^(\s*(?:case\s+[^\n]+|default):)\s*$',r'\1 ;',code,flags=re.M)
        code = re.sub(r'\bswitch\((\w+)\)',r'switch((int)(\1))',code)
        code = re.sub(r'\((float|double|float10)\)(p\w+)\b',r'(\1)(int)\2',code)
        pointer_locals = set(re.findall(r'\b\w+\s*\*+\s*(\w+)\s*(?:;|,)',code))
        for p in pointer_locals:
            code = re.sub(r'\((float|double|float10)\)'+re.escape(p)+r'\b',r'(\1)(int)'+p,code)
        # Overlapping narrow argument names refer to the complete 32-bit slot.
        overlap_args = sorted(set(re.findall(r'\b_param_\d+\b',code)))
        for g in overlap_args:
            p = g[1:]
            signature = code[:code.index('{')]
            dtype = re.search(r'(char|byte|short|ushort)\s+'+p+r'\b',signature)
            if not dtype:
                prefix.append(f'#define {g} {p}')
                continue
            parameter_type = dtype[1]
            code = re.sub(r'(char|byte|short|ushort)\s+'+p+r'\b','unsigned int restore_'+p,code,count=1)
            prefix.append(f'#define {g} restore_{p}')
            size = 1 if parameter_type in ('char','byte') else 2
            code = re.sub(r'\b'+p+r'\b',f'RESTORE_SLICE{size}(restore_{p},0)',code)
            code = re.sub(r'\*'+re.escape(g)+r'\b',f'*(unsigned short *)(restore_{p})',code)
            code = re.sub(r'\b'+re.escape(g)+r'\[',f'((unsigned short *)restore_{p})[',code)
        for g in uses:
            code = re.sub(r'\b'+re.escape(g)+r'\[',r'((undefined4 *)&'+g+')[',code)
        stack = set(re.findall(r'\bstack0x[0-9a-f]+\b',code))
        if stack:
            for g in stack:
                offset = int(g[7:],16)
                if offset >= 0x80000000: offset -= 0x100000000
                prefix.append(f'#define {g} (restore_stack[{32768+offset}])')
            code = code.replace('{','{\n  unsigned char restore_stack[65536];',1)
        destination = f"src/unit_{i:05d}.c"
        code = repair_discarded_return_guards(code)
        void_results = code.count('restore_unresolved_return(')
        source = repair_member_receivers('\n'.join(prefix) + '\n\n' + code + '\n',f['name'])
        (ROOT / destination).write_text(source, encoding='utf-8')
        report.append(dict(name=f['name'], file=destination, signature=f['signature'], globals=len(uses),
                           unresolved_void_results=void_results, unresolved_stack_layout=bool(stack),
                           register_inputs=sorted(set(re.findall(r'\b(?:in_[A-Z]+|unaff_\w+|extraout_\w+)\b',code))),
                           provenance=provenance.get(f['name'])))
    (ROOT / 'build/units.json').write_text(json.dumps(report, indent=2), encoding='utf-8')
    (ROOT / 'units.json').write_text(json.dumps(report, indent=2), encoding='utf-8')
    (ROOT / 'src/units.cmake').write_text('set(RESTORE_UNITS\n' + ''.join('  "${CMAKE_CURRENT_SOURCE_DIR}/'+x['file']+'"\n' for x in report) + ')\n')
    (ROOT / 'generation.json').write_text(json.dumps(dict(input=str(path.resolve()),
        input_sha256=hashlib.sha256(path.read_bytes()).hexdigest(), functions=len(functions),
        compilation_model=True, playable_game=False,
        rea_functions=sum((x.get('provenance') or {}).get('status')=='rea' for x in report),
        historical_fallbacks=sum((x.get('provenance') or {}).get('status')=='historical-ghidra-fallback' for x in report),
        intrinsic_helpers=intrinsic_names,
        functions_with_register_inputs=sum(bool(x['register_inputs']) for x in report),
        unresolved_void_results=sum(x['unresolved_void_results'] for x in report),
        unresolved_stack_layouts=sum(x['unresolved_stack_layout'] for x in report)),indent=2))
    print(json.dumps(dict(functions=len(functions), global_declarations=len(globals))))


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('input', type=Path)
    prepare(parser.parse_args().input)
