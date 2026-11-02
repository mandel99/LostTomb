"""Create deterministic review ZIPs containing at most 100 recovered C functions."""
from __future__ import annotations

import hashlib
import io
import json
from pathlib import Path
import zipfile


ROOT = Path(__file__).resolve().parents[1]
OUTPUT = ROOT / "artifacts" / "function-batches"
BATCH_SIZE = 100
ZIP_TIME = (2026, 10, 27, 0, 0, 0)


def digest(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def info(name: str) -> zipfile.ZipInfo:
    item = zipfile.ZipInfo(name, ZIP_TIME)
    item.compress_type = zipfile.ZIP_DEFLATED
    item.external_attr = 0o100644 << 16
    return item


def add(archive: zipfile.ZipFile, name: str, data: bytes) -> None:
    archive.writestr(info(name), data, compress_type=zipfile.ZIP_DEFLATED, compresslevel=9)


def ollama_status(name: str) -> str | None:
    result = ROOT / "artifacts" / "ollama-batch" / "units" / name / "result.json"
    if not result.exists():
        return None
    try:
        return json.loads(result.read_text(encoding="utf-8")) .get("status") or "in_progress"
    except (OSError, ValueError):
        return "unreadable_result"


def main() -> None:
    OUTPUT.mkdir(parents=True, exist_ok=True)
    units = json.loads((ROOT / "units.json").read_text(encoding="utf-8"))
    hooks = {value.lower() for value in json.loads((ROOT / "metadata" / "hook-addresses.json").read_text())}
    dependencies = [ROOT / "include" / "restore_types.h", ROOT / "include" / "restore_intrinsics.h"]
    readme = (
        "Pharaoh + Cleopatra 2.1 recovered C function batch\n"
        "\n"
        "Each functions/*.c file contains one mechanically prepared recovered procedure.\n"
        "The files are compilation models for translation/review, not proof of behavioral\n"
        "equivalence and not standalone programs. Preserve address-based memory widths,\n"
        "calling convention observations, side-effect order, and 32-bit x86 wraparound.\n"
        "Do not remove restore_unresolved_return guards without an independently reviewed ABI.\n"
        "manifest.json records provenance, unresolved observations, active-hook status,\n"
        "current Ollama review status, and the exact SHA-256 of every included source.\n"
        "The include/ directory is supplied only to make the source vocabulary understandable.\n"
    ).encode("utf-8")

    catalog_batches = []
    expected_zips = []
    for batch_number, start in enumerate(range(0, len(units), BATCH_SIZE), 1):
        batch = units[start:start + BATCH_SIZE]
        first, last = start + 1, start + len(batch)
        zip_name = f"pharaoh-c-functions-{first:04d}-{last:04d}.zip"
        zip_path = OUTPUT / zip_name
        expected_zips.append(zip_path)
        records = []
        with zipfile.ZipFile(zip_path, "w", allowZip64=True) as archive:
            add(archive, "README.txt", readme)
            for dependency in dependencies:
                add(archive, f"include/{dependency.name}", dependency.read_bytes())
            for sequence, unit in enumerate(batch, first):
                source_path = ROOT / unit["file"]
                source = source_path.read_bytes()
                address = unit["provenance"]["address"].lower()
                output_name = f"functions/{sequence:04d}_{unit['name']}.c"
                add(archive, output_name, source)
                records.append({
                    "sequence": sequence,
                    "name": unit["name"],
                    "address": address,
                    "signature": unit["signature"],
                    "source_in_restore": unit["file"],
                    "source_in_zip": output_name,
                    "source_sha256": digest(source),
                    "source_bytes": len(source),
                    "provenance": unit["provenance"],
                    "unresolved": {
                        "globals": unit["globals"],
                        "void_results": unit["unresolved_void_results"],
                        "stack_layout": unit["unresolved_stack_layout"],
                        "register_inputs": unit["register_inputs"],
                    },
                    "active_verified_hybrid_hook": address in hooks,
                    "ollama_batch_status": ollama_status(unit["name"]),
                })
            manifest = {
                "format": 1,
                "batch": batch_number,
                "function_count": len(records),
                "sequence_first": first,
                "sequence_last": last,
                "total_functions": len(units),
                "maximum_functions_per_batch": BATCH_SIZE,
                "functions": records,
            }
            add(archive, "manifest.json", (json.dumps(manifest, indent=2) + "\n").encode("utf-8"))
        payload = zip_path.read_bytes()
        catalog_batches.append({
            "batch": batch_number,
            "file": zip_name,
            "function_count": len(batch),
            "sequence_first": first,
            "sequence_last": last,
            "bytes": len(payload),
            "sha256": digest(payload),
        })

    catalog = {
        "format": 1,
        "total_functions": len(units),
        "batch_size": BATCH_SIZE,
        "batch_count": len(catalog_batches),
        "full_batches": sum(item["function_count"] == BATCH_SIZE for item in catalog_batches),
        "last_batch_functions": catalog_batches[-1]["function_count"],
        "batches": catalog_batches,
    }
    catalog_data = (json.dumps(catalog, indent=2) + "\n").encode("utf-8")
    (OUTPUT / "catalog.json").write_bytes(catalog_data)
    checksums = "".join(f"{item['sha256']}  {item['file']}\n" for item in catalog_batches).encode("ascii")
    (OUTPUT / "SHA256SUMS.txt").write_bytes(checksums)

    master = OUTPUT / "pharaoh-c-function-batches-all.zip"
    with zipfile.ZipFile(master, "w", allowZip64=True) as archive:
        add(archive, "catalog.json", catalog_data)
        add(archive, "SHA256SUMS.txt", checksums)
        for path in expected_zips:
            add(archive, f"batches/{path.name}", path.read_bytes())

    # Read every archive back and prove both integrity and the requested grouping.
    for path, batch in zip(expected_zips, catalog_batches):
        with zipfile.ZipFile(path) as archive:
            if archive.testzip() is not None:
                raise RuntimeError(f"Corrupt archive: {path}")
            manifest = json.loads(archive.read("manifest.json"))
            function_files = [name for name in archive.namelist() if name.startswith("functions/")]
            if manifest["function_count"] != batch["function_count"] or len(function_files) != batch["function_count"]:
                raise RuntimeError(f"Unexpected function count: {path}")
    with zipfile.ZipFile(master) as archive:
        if archive.testzip() is not None or len([n for n in archive.namelist() if n.startswith("batches/")]) != len(expected_zips):
            raise RuntimeError("Master archive verification failed")

    print(json.dumps({
        "functions": len(units),
        "batches": len(catalog_batches),
        "full_batches": catalog["full_batches"],
        "last_batch_functions": catalog["last_batch_functions"],
        "master_zip": str(master),
        "master_bytes": master.stat().st_size,
        "master_sha256": digest(master.read_bytes()),
    }, indent=2))


if __name__ == "__main__":
    main()
