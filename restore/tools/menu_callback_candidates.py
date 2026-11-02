"""Read-only discovery of literal menu-table callbacks, not coverage proof.

Uses existing authenticated assembly and the original PE. Prints candidates
only; does not decompile, launch programs, alter sources or create reports.
Only one literal predecessor setup per call is recovered here. Alternate CFG
predecessors, dynamic tables, omitted Ghidra blocks and unresolved setup remain
outside this discovery. A claimed token still needs its own semantic audit.
"""
from __future__ import annotations

import argparse
import hashlib
import json
from pathlib import Path
import re
import struct
import sys

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "direct_c"))
from native_test_pe import PE

NATIVE_SHA = "fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d"
ENGINES = {0x425990: (36, 16, 2), 0x4261D0: (28, 8, 2),
           0x425C40: (24, 8, 1)}
COMMENTS = re.compile(r'//[^\n]*|/\*[\s\S]*?\*/|"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\'')


def code_only(text: str) -> str:
    return COMMENTS.sub(lambda match: " " * len(match[0]), text)


def function_body(text: str, name: str) -> str | None:
    match = re.search(r"\b(?:void|int|int32_t|uint32_t|BOOL|bool)\s+" +
                      re.escape(name) + r"\s*\([^;{}]*\)\s*\{", text)
    if not match:
        return None
    start = index = match.end()
    depth = 1
    while depth and index < len(text):
        depth += (text[index] == "{") - (text[index] == "}")
        index += 1
    return text[start:index - 1] if depth == 0 else None


def claimed_tokens() -> tuple[set[int], list[str]]:
    sources = [code_only(path.read_text(encoding="utf-8"))
               for path in (ROOT / "runtime").glob("*.c")]
    pending = ["restore_menu_callback", "restore_dropdown_callback"]
    seen, tokens, unresolved = set(), set(), []
    while pending:
        name = pending.pop()
        if name in seen:
            continue
        seen.add(name)
        for source in sources:
            body = function_body(source, name)
            if body is not None:
                break
        else:
            unresolved.append(name)
            continue
        literals = re.findall(
            r"case\s+(?:UINT32_C\s*\(\s*)?0x([\da-fA-F]+)|"
            r"\b(?:token|address|entry)\s*[!=]=\s*(?:UINT32_C\s*\(\s*)?0x([\da-fA-F]+)", body)
        tokens.update(int(first or second, 16) for first, second in literals)
        # Compact token tables (God commands, identical mode-write bodies).
        if re.search(r"\]\s*\.\s*token", body):
            tokens.update(int(value, 16) for value in re.findall(
                r"\{\s*0x([\da-fA-F]+)u?\s*,", source))
        pending.extend(re.findall(
            r"\b(restore_\w+)\s*\(\s*(?:token|address|entry)\s*\)", body))
    return {value for value in tokens if 0x401000 <= value < 0x56F000}, unresolved


def immediate_push(instruction: str) -> int | None:
    match = re.fullmatch(r"PUSH (0x[\da-fA-F]+|\d+)", instruction)
    return int(match[1], 0) if match else None


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--entry", action="append", help="Native hex entry; may repeat")
    parser.add_argument("--limit", type=int, default=40)
    parser.add_argument("--reference-exe", type=Path, help="Optional reviewed port; annotate candidates with its names")
    args = parser.parse_args()
    reference = None
    if args.reference_exe:
        from port_reference import PortReference
        reference = PortReference(args.reference_exe)
    entries = {int(value, 16) for value in (args.entry or ["4d8310", "4f96e0"])}
    original = ROOT / "build/smoke-game/Pharaoh.exe"
    if hashlib.sha256(original.read_bytes()).hexdigest() != NATIVE_SHA:
        raise ValueError("Original PE identity differs from the reconstruction target")
    pe, claimed = PE(original), claimed_tokens()
    pending_entries = set(entries)
    candidates: dict[int, set[tuple[int, int, int, int, int, int]]] = {}
    input_calls = parsed = 0
    for path in (ROOT / "metadata/native-function-audit-cache").glob("batch-*.json"):
        batch = json.loads(path.read_text(encoding="utf-8"))
        if batch.get("native_sha256") != NATIVE_SHA:
            continue
        for entry in batch.get("entries", []):
            value = entry.get("requested_address")
            address = int(value, 16) if isinstance(value, str) else value
            if address not in pending_entries:
                continue
            pending_entries.remove(address)
            assembly = entry["assembly"]["items"]
            for item in assembly:
                offset = int(item["address"], 16) - pe.base
                data = bytes.fromhex(item["bytes"])
                if pe.data[offset:offset + len(data)] != data:
                    raise ValueError("Cached instruction differs at " + item["address"])
            for index, item in enumerate(assembly):
                match = re.fullmatch(r"CALL 0x([\da-fA-F]+)", item["instruction"])
                if not match or int(match[1], 16) not in ENGINES:
                    continue
                input_calls += 1
                pushes = []
                for previous in reversed(assembly[max(0, index - 22):index]):
                    instruction = previous["instruction"]
                    if instruction.startswith(("CALL ", "J", "RET", "POP ", "ADD ESP", "SUB ESP")):
                        break
                    if instruction.startswith("PUSH "):
                        pushes.append(instruction)
                    if len(pushes) == 4:
                        break
                if len(pushes) != 4:
                    continue
                pushes.reverse()
                count, table = immediate_push(pushes[0]), immediate_push(pushes[1])
                if not (count and table and 0 < count <= 100 and 0x570000 <= table < 0x650000):
                    continue
                parsed += 1
                stride, callback_offset, sides = ENGINES[int(match[1], 16)]
                for row in range(count):
                    offset = table - pe.base + row * stride
                    left, top, width, height = struct.unpack_from("<hhhh", pe.data, offset)
                    if stride == 28:
                        valid = width > left and height > top and 1 <= pe.data[offset + 19] <= 4
                    elif stride == 24:
                        valid = height > 0 and pe.data[offset + 15] in (2, 4, 5)
                    else:
                        valid = width > 0 and height > 0
                    if not valid:
                        continue
                    for side in range(sides):
                        token = struct.unpack_from("<I", pe.data, offset + callback_offset + side * 4)[0]
                        if 0x401000 <= token < 0x56F000 and token not in claimed[0]:
                            location = (table, count, row, side, int(item["address"], 16), stride)
                            candidates.setdefault(token, set()).add(location)
    print(f"Literal setup: {parsed}/{input_calls} cached input calls; {len(candidates)} candidate tokens.")
    print("Candidates only: alternate predecessors, dynamic tables, omitted blocks and other setup remain outside this scan.")
    if pending_entries or claimed[1]:
        print("Missing cache entries:", sorted(hex(value) for value in pending_entries),
              "unparsed dispatchers:", sorted(claimed[1]))
    for token, locations in sorted(candidates.items())[:max(0, args.limit)]:
        table, count, row, side, call, stride = sorted(locations)[0]
        name = reference.functions.get(token, {}).get("name") if reference else None
        print(f"{token:08X} table={table:08X} count={count} row={row} side={side} "
              f"call={call:08X} stride={stride} refs={len(locations)}" + (f" port-name={name}" if name else ""))


if __name__ == "__main__":
    main()
