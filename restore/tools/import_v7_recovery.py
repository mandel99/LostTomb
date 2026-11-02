"""Import evidence-backed v4-v7 recovered functions into the hybrid source tree.

The v7 archive uses a simulated restore_global_ptr address space.  The working
hybrid executes inside the original mapped x86 image, so this importer converts
those lvalues back to their exact absolute addresses.  It imports only functions
whose normalized bodies differ, never all 4402 files blindly.
"""
from __future__ import annotations

import argparse
import hashlib
import json
from pathlib import Path
import re
import shutil


ROOT = Path(__file__).resolve().parents[1]
WORKSPACE = ROOT.parent
DEFAULT_V7 = WORKSPACE / "restore-v7"
GLOBAL_PTR = re.compile(r"restore_global_ptr\((0x[0-9a-fA-F]+u?),\s*sizeof\([^)]*\)\)")


def sha(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def normalize(text: str) -> str:
    text = GLOBAL_PTR.sub(r"PTR(\1)", text)
    text = re.sub(r"\*\([^)]*\*\)0x([0-9a-fA-F]+)u", r"*PTR(0x\1u)", text)
    text = re.sub(r"^#define .*?$", "", text, flags=re.M)
    text = re.sub(r"^#include .*?$", "", text, flags=re.M)
    return re.sub(r"\s+", " ", text).strip()


def hybridize(text: str, function_name: str) -> str:
    converted, count = GLOBAL_PTR.subn(r"\1", text)
    if "restore_global_ptr(" in converted:
        raise ValueError("Unconverted restore_global_ptr expression remains")
    if function_name == "FUN_00462180":
        converted = converted.replace("extern code timeGetTime;\n", "")
        converted = converted.replace(
            "  code *pcVar25;\n",
            "  DWORD (WINAPI *pcVar25)(void);\n",
        )
    if function_name == "FUN_0040ffd0":
        widths = {
            "DAT_00d3b53c": "int",
            "DAT_00d3b5be": "short",
            "DAT_00d3b5d4": "int",
            "DAT_00d3b5dc": "uint",
            "DAT_00d3b5e0": "uint",
            "DAT_00e38d94": "uint",
            "DAT_00e92e18": "uint",
        }
        for symbol, c_type in widths.items():
            converted = re.sub(
                rf"(#define {symbol} )\(\*\(undefined \*\)(0x[0-9a-fA-F]+u)\)",
                rf"\1(*({c_type} *)\2)",
                converted,
            )
    return converted


def lineage(v7: Path, relative: str) -> str:
    name = Path(relative).name
    groups = {
        "v5": {Path(x).name for x in (v7 / "metadata/v5_changed_sources.txt").read_text().splitlines() if x},
        "v6": {Path(x).name for x in (v7 / "metadata/v6_changed_sources_vs_v5.txt").read_text().splitlines() if x},
        "v7": {Path(x).name for x in (v7 / "metadata/v7_changed_sources.txt").read_text().splitlines() if x},
    }
    return "+".join(key for key, names in groups.items() if name in names) or "v4_or_earlier"


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("--v7", type=Path, default=DEFAULT_V7)
    parser.add_argument("--apply", action="store_true")
    args = parser.parse_args()
    v7 = args.v7.resolve()
    units = json.loads((ROOT / "units.json").read_text(encoding="utf-8"))
    hooks = {int(x, 16) for x in json.loads((ROOT / "metadata/hook-addresses.json").read_text())}
    changes = []
    for sequence, unit in enumerate(units, 1):
        source = ROOT / unit["file"]
        candidate = v7 / "functions" / f"{sequence:04d}_{unit['name']}.c"
        if not candidate.exists():
            raise SystemExit(f"Missing v7 function: {candidate}")
        before = source.read_text(encoding="utf-8")
        raw = candidate.read_text(encoding="utf-8")
        after = hybridize(raw, unit["name"])
        if normalize(before) == normalize(after):
            continue
        address = int(unit["provenance"]["address"], 16)
        if address in hooks:
            raise SystemExit(f"Refusing to replace active verified hook {unit['name']}")
        changes.append({
            "sequence": sequence,
            "name": unit["name"],
            "address": f"0x{address:08x}",
            "destination": unit["file"],
            "v7_source": str(candidate.relative_to(v7)).replace("\\", "/"),
            "lineage": lineage(v7, str(candidate.relative_to(v7))),
            "before_sha256": sha(before.encode("utf-8")),
            "v7_sha256": sha(raw.encode("utf-8")),
            "after_sha256": sha(after.encode("utf-8")),
            "after_text": after,
            "before_text": before,
        })

    summary = {
        "source_project": str(v7),
        "functions_total": len(units),
        "functions_changed": len(changes),
        "active_hooks_replaced": 0,
        "apply": args.apply,
        "by_lineage": {},
    }
    for item in changes:
        summary["by_lineage"][item["lineage"]] = summary["by_lineage"].get(item["lineage"], 0) + 1
    if not args.apply:
        print(json.dumps(summary, indent=2))
        return

    backup = ROOT / "artifacts/v7-import-backup"
    backup.mkdir(parents=True, exist_ok=True)
    records = []
    for item in changes:
        destination = ROOT / item["destination"]
        backup_path = backup / destination.name
        if backup_path.exists() and sha(backup_path.read_bytes()) != item["before_sha256"]:
            raise SystemExit(f"Backup collision: {backup_path}")
        if not backup_path.exists():
            shutil.copyfile(destination, backup_path)
        destination.write_text(item["after_text"], encoding="utf-8", newline="\n")
        records.append({key: value for key, value in item.items() if not key.endswith("_text")})

    manifest = {
        "format": 1,
        "source": "pharaoh-c-reconstruction-v7",
        "source_project": str(v7),
        "canonical_original_sha256": "fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d",
        "address_model": "v7 restore_global_ptr converted to exact hybrid x86 absolute lvalues",
        "functions_total": len(units),
        "functions_imported": len(records),
        "active_hooks_replaced": 0,
        "by_lineage": summary["by_lineage"],
        "functions": records,
    }
    (ROOT / "metadata/v7-import.json").write_text(json.dumps(manifest, indent=2) + "\n", encoding="utf-8")
    print(json.dumps(summary, indent=2))


if __name__ == "__main__":
    main()
