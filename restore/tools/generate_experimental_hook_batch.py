"""Generate a reversible batch of conservative experimental C entry hooks.

The generated functions are intentionally not described as verified.  Selection
only removes candidates that are already known to depend on hidden registers,
an unresolved stack model, unresolved return values, decompiler temporaries, or
other recovered functions.  The original entry bytes and cdecl-style returns
are checked before a candidate is emitted.
"""
from __future__ import annotations

import argparse
import hashlib
import json
from pathlib import Path
import re
import struct

from capstone import Cs, CS_ARCH_X86, CS_MODE_32


ROOT = Path(__file__).resolve().parents[1]
EXPECTED_ORIGINAL = "fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d"
DEFAULT_ORIGINAL = Path(r"C:\Program Files (x86)\GOG.com\Pharaoh\Pharaoh.exe")
BAD_SOURCE = re.compile(
    r"\b(?:in_[A-Za-z0-9_]*|unaff_[A-Za-z0-9_]*|extraout_[A-Za-z0-9_]*|"
    r"stack0x[0-9A-Fa-f]+|register0x[0-9A-Fa-f]+|restore_unresolved_return)\b"
    r"|ExceptionList|\(\*\*?\(code|extern\s+code\s+|\(\*\(undefined\s*\*\)"
)
FUNCTION_REFERENCE = re.compile(r"\bFUN_[0-9A-Fa-f]+\s*\(")


class PeImage:
    def __init__(self, path: Path) -> None:
        self.path = path
        self.data = path.read_bytes()
        digest = hashlib.sha256(self.data).hexdigest()
        if digest != EXPECTED_ORIGINAL:
            raise ValueError(f"Unexpected original SHA-256: {digest}")
        pe = struct.unpack_from("<I", self.data, 0x3C)[0]
        if self.data[pe : pe + 4] != b"PE\0\0":
            raise ValueError("Missing PE signature")
        section_count = struct.unpack_from("<H", self.data, pe + 6)[0]
        optional_size = struct.unpack_from("<H", self.data, pe + 20)[0]
        optional = pe + 24
        self.image_base = struct.unpack_from("<I", self.data, optional + 28)[0]
        table = optional + optional_size
        self.sections: list[tuple[int, int, int]] = []
        for index in range(section_count):
            offset = table + index * 40
            virtual_size, virtual_address, raw_size, raw_offset = struct.unpack_from(
                "<IIII", self.data, offset + 8
            )
            self.sections.append((virtual_address, max(virtual_size, raw_size), raw_offset))

    def read(self, address: int, size: int) -> bytes:
        rva = address - self.image_base
        for virtual_address, span, raw_offset in self.sections:
            if virtual_address <= rva and rva + size <= virtual_address + span:
                start = raw_offset + rva - virtual_address
                return self.data[start : start + size]
        raise ValueError(f"Address range is not file-backed: 0x{address:08x}+0x{size:x}")


def select(units: list[dict], image: PeImage, active: set[int], count: int) -> tuple[list[dict], dict]:
    disassembler = Cs(CS_ARCH_X86, CS_MODE_32)
    eligible: list[dict] = []
    rejected: dict[str, int] = {}

    def reject(reason: str) -> None:
        rejected[reason] = rejected.get(reason, 0) + 1

    addresses = [int(unit["provenance"]["address"], 16) for unit in units]
    for index, unit in enumerate(units):
        address = addresses[index]
        if address in active:
            reject("already-active")
            continue
        if unit.get("register_inputs"):
            reject("register-input")
            continue
        if unit.get("unresolved_stack_layout"):
            reject("unresolved-stack")
            continue
        if unit.get("unresolved_void_results", 0):
            reject("unresolved-void-result")
            continue
        source_path = ROOT / unit["file"]
        source = source_path.read_text(encoding="utf-8")
        if BAD_SOURCE.search(source):
            reject("unsafe-decompiler-token")
            continue
        if index + 1 >= len(addresses):
            reject("missing-next-boundary")
            continue
        span = addresses[index + 1] - address
        if span < 5:
            reject("entry-shorter-than-jump")
            continue
        machine_code = image.read(address, span)
        returns = [instruction for instruction in disassembler.disasm(machine_code, address)
                   if instruction.mnemonic in {"ret", "retf"}]
        if not returns:
            reject("no-decoded-return")
            continue
        if any(instruction.mnemonic != "ret" or instruction.op_str for instruction in returns):
            reject("callee-cleans-stack-or-far-return")
            continue
        entry = image.read(address, 5)
        references = FUNCTION_REFERENCE.findall(source)
        reference_names = [reference.split("(", 1)[0].strip() for reference in references]
        dependencies = set(reference_names)
        dependencies.discard(unit["name"])
        if reference_names.count(unit["name"]) > 1:
            dependencies.add(unit["name"])
        eligible.append({
            "sequence": index,
            "name": unit["name"],
            "address": f"0x{address:08x}",
            "source": unit["file"],
            "source_sha256": hashlib.sha256(source.encode("utf-8")).hexdigest(),
            "original_prefix": entry.hex(),
            "original_plain_returns": [f"0x{instruction.address:08x}" for instruction in returns],
            "recovered_dependencies": sorted(dependencies),
        })

    depth_by_name: dict[str, int] = {}
    pending = eligible[:]
    while pending:
        admitted = []
        for item in pending:
            dependencies = item["recovered_dependencies"]
            if all(name in depth_by_name for name in dependencies):
                item["dependency_depth"] = 0 if not dependencies else 1 + max(
                    depth_by_name[name] for name in dependencies
                )
                depth_by_name[item["name"]] = item["dependency_depth"]
                admitted.append(item)
        if not admitted:
            break
        admitted_names = {item["name"] for item in admitted}
        pending = [item for item in pending if item["name"] not in admitted_names]

    rejected["unsafe-recovered-dependency"] = len(pending)
    selected = sorted(
        (item for item in eligible if item["name"] in depth_by_name),
        key=lambda item: (item["dependency_depth"], int(item["address"], 16)),
    )[:count]
    if len(selected) != count:
        raise ValueError(
            f"Only {len(selected)} dependency-closed candidates passed; requested {count}"
        )
    return selected, rejected


def render_header(selected: list[dict]) -> str:
    lines = [
        "/* Generated by tools/generate_experimental_hook_batch.py; do not hand-edit. */",
        "#ifndef RESTORE_EXPERIMENTAL_HOOKS_H",
        "#define RESTORE_EXPERIMENTAL_HOOKS_H",
        f"#define RESTORE_EXPERIMENTAL_HOOK_COUNT {len(selected)}",
        "",
    ]
    lines.extend(f"extern void {item['name']}();" for item in selected)
    lines.extend(["", "static const DWORD experimental_hook_addresses[RESTORE_EXPERIMENTAL_HOOK_COUNT]={"])
    for start in range(0, len(selected), 8):
        chunk = selected[start : start + 8]
        suffix = "," if start + 8 < len(selected) else ""
        lines.append("    " + ",".join(item["address"] for item in chunk) + suffix)
    lines.append("};")
    lines.append("static const void *const experimental_hook_functions[RESTORE_EXPERIMENTAL_HOOK_COUNT]={")
    for start in range(0, len(selected), 6):
        chunk = selected[start : start + 6]
        suffix = "," if start + 6 < len(selected) else ""
        lines.append("    " + ",".join(f"(const void *){item['name']}" for item in chunk) + suffix)
    lines.append("};")
    lines.append("static const unsigned char experimental_hook_prefix[RESTORE_EXPERIMENTAL_HOOK_COUNT][5]={")
    for item in selected:
        prefix = ",".join(f"0x{item['original_prefix'][i:i+2]}" for i in range(0, 10, 2))
        lines.append(f"    {{{prefix}}},")
    lines.extend(["};", "#endif", ""])
    return "\n".join(lines)


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("--original", type=Path, default=DEFAULT_ORIGINAL)
    parser.add_argument("--count", type=int, default=200)
    parser.add_argument("--batch", type=int, default=1)
    args = parser.parse_args()
    units = json.loads((ROOT / "units.json").read_text(encoding="utf-8"))
    active = {int(value, 16) for value in json.loads(
        (ROOT / "metadata/hook-addresses.json").read_text(encoding="utf-8")
    )}
    image = PeImage(args.original)
    cumulative_count = args.count * args.batch
    cumulative, rejected = select(units, image, active, cumulative_count)
    batch_start = (args.batch - 1) * args.count
    selected = cumulative[batch_start:]
    header = ROOT / "runtime/experimental_hooks.h"
    manifest = ROOT / f"metadata/experimental-hook-batch-{args.batch:02d}.json"
    header.write_text(render_header(cumulative), encoding="utf-8", newline="\n")
    manifest.write_text(json.dumps({
        "format": 1,
        "status": "experimental-not-equivalence-verified",
        "batch": args.batch,
        "count": len(selected),
        "cumulative_count": len(cumulative),
        "selection": "First procedures in address order passing conservative leaf/ABI filters",
        "original_sha256": EXPECTED_ORIGINAL,
        "active_verified_hooks_excluded": len(active),
        "rejections_before_batch_completed": rejected,
        "functions": selected,
    }, indent=2) + "\n", encoding="utf-8")
    print(f"Generated {len(cumulative)} cumulative hooks in {header}")
    print(f"Batch {args.batch} contributes {len(selected)} hooks")
    print(f"Manifest: {manifest}")


if __name__ == "__main__":
    main()
