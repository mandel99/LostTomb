#!/usr/bin/env python3
"""Index and retrieve saved Ghidra/REA pseudocode without another decompile."""

from __future__ import annotations

import argparse
import hashlib
import json
import re
from pathlib import Path
from typing import Any


ROOT = Path(__file__).resolve().parents[1]
EVIDENCE = ROOT / "evidence"
GHIDRA_FILE = EVIDENCE / "ghidra-typed-pseudocode-20261116-after-java-types.c"
FUNCTION_MARKER = re.compile(
    r"(?m)^/\*\s+([0-9a-fA-F]{8})\s+([A-Za-z_][A-Za-z0-9_]*)\s*\*/\s*$"
)
ADDRESS = re.compile(r"^(?:0x)?([0-9a-fA-F]{1,8})$")


def sha256(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as stream:
        for block in iter(lambda: stream.read(1024 * 1024), b""):
            digest.update(block)
    return digest.hexdigest()


def normalized_address(value: Any) -> str | None:
    match = ADDRESS.fullmatch(str(value).strip())
    return f"0x{int(match.group(1), 16):08X}" if match else None


def ghidra_functions() -> tuple[dict[str, dict[str, Any]], str | None]:
    if not GHIDRA_FILE.is_file():
        return {}, None
    text = GHIDRA_FILE.read_text(encoding="utf-8", errors="replace")
    markers = list(FUNCTION_MARKER.finditer(text))
    functions: dict[str, dict[str, Any]] = {}
    for index, marker in enumerate(markers):
        address = normalized_address(marker.group(1))
        if address is None:
            continue
        end = markers[index + 1].start() if index + 1 < len(markers) else len(text)
        body = text[marker.start():end].rstrip()
        functions[address] = {
            "name": marker.group(2),
            "source": GHIDRA_FILE.relative_to(ROOT).as_posix(),
            "sha256": hashlib.sha256(body.encode("utf-8")).hexdigest(),
            "characters": len(body),
        }
    return functions, sha256(GHIDRA_FILE)


def rea_functions() -> dict[str, list[dict[str, Any]]]:
    functions: dict[str, list[dict[str, Any]]] = {}
    for bundle in sorted(EVIDENCE.glob("rea-*.json")):
        try:
            data = json.loads(bundle.read_text(encoding="utf-8-sig"))
        except (OSError, json.JSONDecodeError):
            continue
        for record in data.get("records", []):
            if record.get("operation") != "procedure_pseudo_code":
                continue
            parameters = record.get("parameters") or {}
            result = record.get("normalized_result")
            address = normalized_address(parameters.get("procedure"))
            if address is None or not isinstance(result, str):
                continue
            functions.setdefault(address, []).append({
                "evidence_id": record.get("evidence_id"),
                "bundle": bundle.relative_to(ROOT).as_posix(),
                "sha256": hashlib.sha256(result.encode("utf-8")).hexdigest(),
                "characters": len(result),
            })
    return functions


def ghidra_body(address: str) -> str | None:
    if not GHIDRA_FILE.is_file():
        return None
    text = GHIDRA_FILE.read_text(encoding="utf-8", errors="replace")
    markers = list(FUNCTION_MARKER.finditer(text))
    for index, marker in enumerate(markers):
        if normalized_address(marker.group(1)) != address:
            continue
        end = markers[index + 1].start() if index + 1 < len(markers) else len(text)
        return text[marker.start():end].rstrip()
    return None


def rea_body(address: str) -> tuple[str, dict[str, Any]] | None:
    for bundle in sorted(EVIDENCE.glob("rea-*.json")):
        try:
            data = json.loads(bundle.read_text(encoding="utf-8-sig"))
        except (OSError, json.JSONDecodeError):
            continue
        for record in data.get("records", []):
            if record.get("operation") != "procedure_pseudo_code":
                continue
            parameters = record.get("parameters") or {}
            if normalized_address(parameters.get("procedure")) != address:
                continue
            body = record.get("normalized_result")
            if isinstance(body, str):
                return body, {
                    "evidence_id": record.get("evidence_id"),
                    "bundle": bundle.relative_to(ROOT).as_posix(),
                }
    return None


def rea_procedures() -> dict[str, dict[str, Any]]:
    procedures: dict[str, dict[str, Any]] = {}
    for bundle in sorted(EVIDENCE.glob("rea-*.json")):
        try:
            data = json.loads(bundle.read_text(encoding="utf-8-sig"))
        except (OSError, json.JSONDecodeError):
            continue
        for record in data.get("records", []):
            if record.get("operation") != "list_procedures":
                continue
            for item in record.get("normalized_result", []):
                raw_address = str(item.get("address", ""))
                address = normalized_address(raw_address)
                if address is None and raw_address.startswith("EXTERNAL:"):
                    address = raw_address
                if not address:
                    continue
                procedure = item.get("procedure") or {}
                procedures[address] = {
                    "name": item.get("value"),
                    "external": procedure.get("external"),
                    "thunk": procedure.get("thunk"),
                    "thunk_target": procedure.get("thunk_target"),
                    "evidence_id": record.get("evidence_id"),
                    "bundle": bundle.relative_to(ROOT).as_posix(),
                }
    return procedures


def build_index() -> dict[str, Any]:
    ghidra, ghidra_sha = ghidra_functions()
    rea = rea_functions()
    procedures = rea_procedures()
    addresses = sorted(set(ghidra) | set(rea) | set(procedures))
    internal = sum(not item["external"] for item in procedures.values())
    external = sum(bool(item["external"]) for item in procedures.values())
    thunks = sum(bool(item["thunk"]) for item in procedures.values())
    internal_with_ghidra = sum(
        not item["external"] and address in ghidra
        for address, item in procedures.items()
    )
    return {
        "schema": "pharaoh-decompilation-cache-v1",
        "pinned_original_sha256": (
            "fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d"
        ),
        "ghidra_source": GHIDRA_FILE.relative_to(ROOT).as_posix(),
        "ghidra_source_sha256": ghidra_sha,
        "counts": {
            "ghidra_function_bodies": len(ghidra),
            "rea_pseudocode_records": sum(map(len, rea.values())),
            "unique_cached_addresses": len(addresses),
            "rea_analyzed_procedures": len(procedures),
            "rea_internal_procedures": internal,
            "rea_external_procedures": external,
            "rea_thunks": thunks,
            "internal_with_ghidra_pseudocode": internal_with_ghidra,
            "internal_without_ghidra_pseudocode": internal - internal_with_ghidra,
            "ghidra_bodies_outside_rea_inventory": len(
                set(ghidra) - set(procedures)
            ),
        },
        "functions": {
            address: {
                "ghidra": ghidra.get(address),
                "rea": rea.get(address, []),
                "rea_inventory": procedures.get(address),
            }
            for address in addresses
        },
    }


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--address", help="show cached records for one native VA")
    parser.add_argument("--show", choices=("ghidra", "rea"),
                        help="print cached pseudocode for --address")
    parser.add_argument("--index", type=Path,
                        default=EVIDENCE / "decompilation-cache-index.json")
    parser.add_argument("--no-write", action="store_true",
                        help="report cache counts without writing the index")
    args = parser.parse_args()

    index = build_index()
    if args.address:
        address = normalized_address(args.address)
        if address is None:
            parser.error("address must be a hexadecimal 32-bit VA")
        cached = index["functions"].get(address)
        if cached is None:
            print(f"{address}: CACHE MISS; query REA once, export the Evidence bundle, then re-index.")
            return 1
        if args.show == "rea":
            result = rea_body(address)
            if result is None:
                print(f"{address}: no saved REA pseudocode")
                return 1
            body, record = result
            print(f"/* REA cache: {record['bundle']} ({record['evidence_id']}) */")
            print(body)
        elif args.show == "ghidra":
            body = ghidra_body(address)
            if body is None:
                print(f"{address}: no saved Ghidra pseudocode")
                return 1
            print(body)
        else:
            print(json.dumps({"address": address, **cached}, indent=2))
        return 0

    if not args.no_write:
        args.index.parent.mkdir(parents=True, exist_ok=True)
        args.index.write_text(json.dumps(index, indent=2) + "\n", encoding="utf-8")
    print(json.dumps(index["counts"], indent=2))
    if not args.no_write:
        print(f"index: {args.index.relative_to(ROOT).as_posix()}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
