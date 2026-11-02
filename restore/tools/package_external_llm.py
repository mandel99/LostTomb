"""Build a deterministic, text-only hand-off ZIP for an external LLM."""
from __future__ import annotations

import csv
import hashlib
import io
import json
import re
from collections import Counter, defaultdict
from pathlib import Path
import zipfile


ROOT = Path(__file__).resolve().parents[1]
OUTPUT = ROOT / "artifacts" / "external-llm"
ARCHIVE = OUTPUT / "pharaoh-restore-standalone-llm-kit.zip"
PREFIX = "pharaoh-restore-llm-kit"
ZIP_TIME = (2026, 10, 27, 0, 0, 0)
ORIGINAL_SHA256 = "fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d"
FUN_RE = re.compile(r"\b(FUN_[0-9a-fA-F]{8})\s*\(")
FORBIDDEN_SUFFIXES = {".exe", ".dll", ".lib", ".obj", ".pdb", ".zip"}


def digest(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def zip_info(name: str) -> zipfile.ZipInfo:
    item = zipfile.ZipInfo(f"{PREFIX}/{name}", ZIP_TIME)
    item.compress_type = zipfile.ZIP_DEFLATED
    item.external_attr = 0o100644 << 16
    return item


def json_bytes(value: object) -> bytes:
    return (json.dumps(value, indent=2, sort_keys=False) + "\n").encode("utf-8")


def normalized_address(value: str) -> str:
    return f"0x{int(value, 16):08x}"


def project_files() -> list[tuple[str, Path]]:
    files: list[tuple[str, Path]] = []
    top = [
        "CMakeLists.txt", "README.md", "OLLAMA_BATCH.md", "build.ps1",
        "regenerate.ps1", "run.ps1", "ollama-batch.cmd", "ollama-batch.ps1",
        "generation.json", "units.json",
    ]
    for name in top:
        path = ROOT / name
        if path.exists():
            files.append((f"project/{name}", path))
    for directory in ["include", "runtime", "src", "tests", "tools", "metadata"]:
        base = ROOT / directory
        for path in sorted(base.rglob("*")):
            if not path.is_file() or path.suffix.lower() in FORBIDDEN_SUFFIXES:
                continue
            if directory == "tools" and path.name == "package_external_llm.py":
                # Included below through the same deterministic path; this branch
                # merely documents that it is intentionally part of the package.
                pass
            files.append((f"project/{path.relative_to(ROOT).as_posix()}", path))
    for path in sorted((ROOT / "external-llm").glob("*")):
        if path.is_file():
            files.append((path.name, path))
    raw = ROOT / "raw"
    for path in sorted(raw.rglob("*")):
        if path.is_file() and path.suffix.lower() not in FORBIDDEN_SUFFIXES:
            files.append((f"evidence/{path.relative_to(raw).as_posix()}", path))
    return files


def ollama_statuses() -> tuple[dict[str, str], dict[str, int]]:
    by_unit: dict[str, str] = {}
    base = ROOT / "artifacts" / "ollama-batch" / "units"
    if base.exists():
        for result in sorted(base.glob("*/result.json")):
            try:
                value = json.loads(result.read_text(encoding="utf-8"))
                by_unit[result.parent.name] = value.get("status") or "in_progress"
            except (OSError, ValueError):
                by_unit[result.parent.name] = "unreadable_result"
    return by_unit, dict(sorted(Counter(by_unit.values()).items()))


def generated_indexes() -> dict[str, bytes]:
    units = json.loads((ROOT / "units.json").read_text(encoding="utf-8"))
    hooks = {
        normalized_address(value)
        for value in json.loads((ROOT / "metadata" / "hook-addresses.json").read_text(encoding="utf-8"))
    }
    ollama, ollama_summary = ollama_statuses()
    known = {unit["name"] for unit in units}
    calls: dict[str, list[str]] = {}
    incoming: defaultdict[str, set[str]] = defaultdict(set)
    source_hashes: dict[str, str] = {}
    for unit in units:
        source = (ROOT / unit["file"]).read_bytes()
        source_hashes[unit["name"]] = digest(source)
        found = sorted((set(FUN_RE.findall(source.decode("utf-8", errors="replace"))) & known) - {unit["name"]})
        calls[unit["name"]] = found
        for target in found:
            incoming[target].add(unit["name"])

    def trust(unit: dict) -> str:
        address = normalized_address(unit["provenance"]["address"])
        if address in hooks:
            return "differentially_verified_active_hook"
        status = ollama.get(unit["name"], "")
        if status == "codex_reviewed_differential_passed":
            return "differentially_verified_not_active"
        if unit["unresolved_stack_layout"]:
            return "blocked_stack_layout"
        if unit["register_inputs"]:
            return "blocked_register_abi"
        if unit["unresolved_void_results"]:
            return "blocked_return_contract"
        if status in {"codex_reviewed_compiled_unverified", "compiled_unverified"}:
            return "reviewed_compiled_unverified"
        return "compilation_model_only"

    output = io.StringIO(newline="")
    writer = csv.writer(output, lineterminator="\n")
    writer.writerow([
        "sequence", "name", "address", "source", "signature", "provenance",
        "trust", "active_hybrid_hook", "ollama_status", "globals",
        "unresolved_void_results", "unresolved_stack_layout", "register_inputs",
        "direct_callees", "incoming_callers", "source_sha256",
    ])
    for sequence, unit in enumerate(units, 1):
        name = unit["name"]
        address = normalized_address(unit["provenance"]["address"])
        writer.writerow([
            sequence, name, address, unit["file"], unit["signature"],
            unit["provenance"]["status"], trust(unit), address in hooks,
            ollama.get(name, "pending"), unit["globals"],
            unit["unresolved_void_results"], unit["unresolved_stack_layout"],
            ";".join(unit["register_inputs"]), ";".join(calls[name]),
            len(incoming[name]), source_hashes[name],
        ])

    entry_names = ["FUN_00562fea", "FUN_00413ed0"]
    by_name = {unit["name"]: unit for unit in units}
    frontier = {}
    for entry in entry_names:
        unit = by_name[entry]
        frontier[entry] = {
            "role": "recovered_crt_entry_do_not_reuse" if entry == "FUN_00562fea" else "game_winmain_like_bootstrap",
            "unit": unit,
            "trust": trust(unit),
            "direct_callees": [
                {
                    "name": target,
                    "unit": by_name[target],
                    "trust": trust(by_name[target]),
                }
                for target in calls[entry]
            ],
        }

    status_counts = Counter(trust(unit) for unit in units)
    current = {
        "snapshot_date": "2026-10-27",
        "package_kind": "text-only external LLM engineering hand-off",
        "goal": "standalone x86 Windows executable with no runtime or build-time Pharaoh.exe dependency",
        "canonical_original_sha256": ORIGINAL_SHA256,
        "original_binary_included": False,
        "compiled_binaries_included": False,
        "recovered_functions": len(units),
        "rea_functions": sum(unit["provenance"]["status"] == "rea" for unit in units),
        "historical_fallbacks": sum(unit["provenance"]["status"] != "rea" for unit in units),
        "active_verified_hybrid_hooks": len(hooks),
        "functions_with_register_inputs": sum(bool(unit["register_inputs"]) for unit in units),
        "register_input_observations": sum(len(unit["register_inputs"]) for unit in units),
        "functions_with_unresolved_void_results": sum(unit["unresolved_void_results"] > 0 for unit in units),
        "unresolved_void_result_observations": sum(unit["unresolved_void_results"] for unit in units),
        "unresolved_stack_layouts": sum(bool(unit["unresolved_stack_layout"]) for unit in units),
        "trust_counts": dict(sorted(status_counts.items())),
        "ollama_snapshot_note": "Informational only; the local batch may continue after this package is created.",
        "ollama_unit_statuses": ollama_summary,
        "hybrid": {
            "standalone": False,
            "original_code_fallback": True,
            "original_entry_absolute": "0x00562fea",
            "game_bootstrap_candidate": "0x00413ed0",
        },
        "original_pe_layout": {
            "image_base": "0x00400000",
            "text": {"rva": "0x00001000", "absolute_start": "0x00401000", "virtual_size": "0x0016d24d"},
            "rdata": {"rva": "0x0016f000", "absolute_start": "0x0056f000", "virtual_size": "0x000061a6"},
            "data": {"rva": "0x00176000", "absolute_start": "0x00576000", "virtual_size": "0x00cf1aa8"},
            "rsrc": {"rva": "0x00e68000", "absolute_start": "0x01268000", "virtual_size": "0x00004590"},
        },
    }
    function_index = output.getvalue().encode("utf-8")
    parsed_rows = list(csv.reader(io.StringIO(function_index.decode("utf-8"))))
    if len(parsed_rows) != len(units) + 1:
        raise RuntimeError("Generated function index row count is invalid")

    call_graph = {
        "format": 1,
        "note": "Direct FUN_* calls parsed from prepared C; indirect calls and decompiler mistakes are not represented.",
        "functions": {
            name: {"calls": calls[name], "called_by": sorted(incoming[name])}
            for name in sorted(known)
        },
    }
    return {
        "generated/CURRENT_STATE.json": json_bytes(current),
        "generated/FUNCTION_INDEX.csv": function_index,
        "generated/CALL_GRAPH.json": json_bytes(call_graph),
        "generated/BOOTSTRAP_FRONTIER.json": json_bytes(frontier),
    }


def main() -> None:
    OUTPUT.mkdir(parents=True, exist_ok=True)
    entries: dict[str, bytes] = {}
    for name, path in project_files():
        if name in entries:
            raise RuntimeError(f"Duplicate package path: {name}")
        entries[name] = path.read_bytes()
    entries.update(generated_indexes())

    forbidden = [
        name for name in entries
        if Path(name).suffix.lower() in FORBIDDEN_SUFFIXES
        or "/artifacts/" in f"/{name}/"
        or "/build/" in f"/{name}/"
    ]
    if forbidden:
        raise RuntimeError(f"Forbidden binary/build content: {forbidden[:5]}")

    manifest_files = [
        {"path": name, "bytes": len(data), "sha256": digest(data)}
        for name, data in sorted(entries.items())
    ]
    manifest = {
        "format": 1,
        "package": PREFIX,
        "purpose": "External LLM hand-off for standalone Pharaoh reconstruction",
        "original_binary_included": False,
        "compiled_binaries_included": False,
        "file_count_before_manifest": len(manifest_files),
        "files": manifest_files,
    }
    entries["MANIFEST.json"] = json_bytes(manifest)
    entries["SHA256SUMS.txt"] = "".join(
        f"{digest(data)}  {name}\n" for name, data in sorted(entries.items())
    ).encode("ascii")

    with zipfile.ZipFile(ARCHIVE, "w", allowZip64=True) as archive:
        for name, data in sorted(entries.items()):
            archive.writestr(zip_info(name), data, compress_type=zipfile.ZIP_DEFLATED, compresslevel=9)

    with zipfile.ZipFile(ARCHIVE) as archive:
        bad = archive.testzip()
        if bad is not None:
            raise RuntimeError(f"Corrupt ZIP member: {bad}")
        names = archive.namelist()
        if len(names) != len(entries) or len(names) != len(set(names)):
            raise RuntimeError("ZIP member count/uniqueness check failed")

    payload = ARCHIVE.read_bytes()
    summary = {
        "archive": str(ARCHIVE),
        "files": len(entries),
        "bytes": len(payload),
        "sha256": digest(payload),
        "original_binary_included": False,
        "compiled_binaries_included": False,
    }
    (OUTPUT / "package-summary.json").write_bytes(json_bytes(summary))
    print(json.dumps(summary, indent=2))


if __name__ == "__main__":
    main()
