"""Prepare every existing review batch as private, hash-bound working inputs.

Consumes bulk_remaining.py's saved inventory, without scanning or generating C.
Exact source/cache bytes are shared by SHA256 under restore/build only.
"""
from __future__ import annotations

import argparse
from collections import Counter
import hashlib
import json
from pathlib import Path
import re

from bulk_reconstruct import PIN, ROOT

REVIEW = {"unsupported_rule", "review_needed", "uncached", "existing_C_reuse_review"}
SHA = re.compile(r"[0-9a-f]{64}\Z")
ADDRESS = re.compile(r"(?:0x)?[0-9a-fA-F]{6,8}\Z")


def digest(data):
    return hashlib.sha256(data).hexdigest()


def file_sha(path):
    hasher = hashlib.sha256()
    with path.open("rb") as stream:
        for block in iter(lambda: stream.read(1024 * 1024), b""):
            hasher.update(block)
    return hasher.hexdigest()


def encoded(value):
    return (json.dumps(value, indent=2, sort_keys=True) + "\n").encode()


def address(value):
    if not isinstance(value, str) or not ADDRESS.fullmatch(value):
        raise ValueError("Invalid inventory address: " + repr(value))
    return f"0x{int(value, 16):08x}"


def rooted(name):
    if not isinstance(name, str) or not name or Path(name).is_absolute():
        raise ValueError("Expected a restore-relative input path: " + repr(name))
    path = (ROOT / name).resolve()
    if not path.is_relative_to(ROOT.resolve()):
        raise ValueError("Input escapes restore: " + name)
    return path


def checked_hash(value):
    if not isinstance(value, str) or not SHA.fullmatch(value):
        raise ValueError("Invalid SHA256: " + repr(value))
    return value


def verify_files(bindings, label):
    for name, expected in bindings.items():
        if file_sha(rooted(name)) != checked_hash(expected):
            raise ValueError(label + name)


def prepare(inventory_path, output):
    output = output.resolve()
    build = (ROOT / "build").resolve()
    if output == build or not output.is_relative_to(build):
        raise ValueError("Output must be a separate directory under restore/build")
    inventory_bytes = inventory_path.read_bytes()
    inventory = json.loads(inventory_bytes)
    if inventory.get("schema") != "bulk-remaining-inventory-v1" or inventory.get("native_sha256") != PIN:
        raise ValueError("Wrong inventory schema or native PIN")
    binary = ROOT / "build/smoke-game/Pharaoh.exe"
    if file_sha(binary) != PIN:
        raise ValueError("Pinned original SHA256 mismatch")
    inputs = inventory.get("inputs_sha256")
    if not isinstance(inputs, dict) or not inputs:
        raise ValueError("Inventory must bind its inputs_sha256")
    verify_files(inputs, "Inventory input changed: ")

    rows = {}
    source_hashes = {}
    for row in inventory["functions"]:
        entry = address(row["address"])
        if entry in rows:
            raise ValueError("Duplicate function: " + entry)
        rows[entry] = row
        # Validate every explicitly named source/owner, without discovering files.
        for item in [row, *row.get("existing_owners", [])]:
            if "source" not in item:
                continue
            name, expected = item["source"], checked_hash(item["source_sha256"])
            rooted(name)
            if name in source_hashes and source_hashes[name] != expected:
                raise ValueError("Conflicting source hashes: " + name)
            source_hashes[name] = expected
    if inventory.get("catalogue_entries") != len(rows):
        raise ValueError("Inventory catalogue count differs from its rows")
    verify_files(source_hashes, "Inventory source changed: ")

    batches = inventory["review_batches"]
    wanted = {entry for entry, row in rows.items() if row["category"] in REVIEW}
    assigned, relevant, batch_ids, unresolved = set(), set(), set(), set()
    for batch in batches:
        number = batch["batch"]
        if type(number) is not int or number < 1 or number in batch_ids:
            raise ValueError("Duplicate/invalid review batch number")
        batch_ids.add(number)
        members = [address(entry) for entry in batch["addresses"]]
        if batch["entries"] != len(members) or len(set(members)) != len(members):
            raise ValueError("Review batch entry count/uniqueness mismatch")
        if not set(members) <= wanted or assigned.intersection(members):
            raise ValueError("Review batch repeats or includes a non-review function")
        if batch["category_counts"] != dict(Counter(rows[entry]["category"] for entry in members)):
            raise ValueError("Review batch category counts differ from its rows")
        assigned.update(members)
        relevant.update(members)
        for dependency in batch["dependencies"]:
            entry = address(dependency["address"])
            if entry not in rows and dependency["category"] == "outside_saved_catalogue":
                unresolved.add(entry)
                continue
            if entry not in rows or dependency["category"] != rows[entry]["category"]:
                raise ValueError("Unknown/misclassified dependency: " + entry)
            relevant.add(entry)
    if assigned != wanted or inventory.get("review_entries") != len(assigned):
        raise ValueError("Review batches do not cover all and only review entries")

    files, source_refs, cache_refs, function_refs = {}, {}, {}, {}

    def add(name, payload):
        if name in files and files[name] != payload:
            raise ValueError("Conflicting output bytes: " + name)
        files[name] = payload
        return dict(path=name, sha256=digest(payload))

    for entry in sorted(relevant):
        row = rows[entry]
        sources = sorted({item["source"] for item in [row, *row.get("existing_owners", [])] if "source" in item})
        for name in sources:
            if name not in source_refs:
                payload = rooted(name).read_bytes()
                expected = source_hashes[name]
                if digest(payload) != expected:
                    raise ValueError("Source changed while preparing: " + name)
                source_refs[name] = dict(source=name, **add(f"shared/sources/{expected}.bytes", payload))
        native = []
        evidence = row["native_evidence"]
        for name in evidence["cache_files"]:
            if name not in inputs:
                raise ValueError("Native cache is not bound by inputs_sha256: " + name)
            if name not in cache_refs:
                payload = rooted(name).read_bytes()
                if digest(payload) != inputs[name]:
                    raise ValueError("Native cache changed while preparing: " + name)
                cache = json.loads(payload)
                if cache.get("native_sha256") != PIN:
                    raise ValueError("Wrong native cache PIN: " + name)
                cache_refs[name] = (dict(source=name, **add(f"shared/native-cache/{inputs[name]}.json", payload)), cache)
            reference, cache = cache_refs[name]
            indices = [i for i, item in enumerate(cache["entries"]) if address(item["requested_address"]) == entry]
            if not indices:
                raise ValueError("Native cache lacks its referenced address: " + entry + " / " + name)
            native.append(dict(**reference, entry_indices=indices))
        record = dict(schema="remaining-review-function-v1", inventory_row=row,
                      work_kind="review_existing_C" if row["category"] == "existing_C_reuse_review" else "review_native_and_existing_sources",
                      source_refs=[source_refs[name] for name in sources], native_cache_refs=native,
                      correctness_proved=False, replacement_C_generated=False)
        function_refs[entry] = dict(address=entry, **add(f"shared/functions/{entry[2:]}.json", encoded(record)))

    summaries = []
    for batch in batches:
        dependencies = []
        for item in batch["dependencies"]:
            entry = address(item["address"])
            dependencies.append(function_refs[entry] if entry in rows else
                dict(address=entry, inventory_dependency=item, status="unresolved_outside_saved_catalogue",
                     source_record_available=False, native_cache_record_available=False))
        manifest = dict(schema="remaining-review-batch-v1", native_sha256=PIN,
                        inventory_sha256=digest(inventory_bytes), batch=batch,
                        members=[function_refs[address(entry)] for entry in batch["addresses"]],
                        dependencies=dependencies,
                        instructions=["Reuse existing C and inspect its saved contract before writing any replacement.",
                                      "existing_C_reuse_review is a reuse lead, not a newly missing function.",
                                      "Keep native evidence gaps and unresolved indirect calls explicit; compilation is not behavior equivalence."],
                        source_installation_available=False, selected_cohort_advance_allowed=False,
                        behavior_comparison_complete=False, host_execution=False)
        reference = add(f"batch-{batch['batch']:03d}/manifest.json", encoded(manifest))
        summaries.append(dict(batch=batch["batch"], entries=batch["entries"], **reference))
    index = dict(schema="remaining-review-packets-v1", native_sha256=PIN,
                 native_binary=dict(path="build/smoke-game/Pharaoh.exe", sha256=PIN, copied=False),
                 inventory=dict(path=str(inventory_path.resolve()), sha256=digest(inventory_bytes)),
                 tool_sha256=file_sha(Path(__file__)), inputs_sha256=inputs,
                 review_entries=len(assigned), batches=summaries, shared_function_records=len(relevant),
                 original_source_paths=len(source_refs), unique_source_blobs=len({r["sha256"] for r in source_refs.values()}),
                 native_cache_files=len(cache_refs), source_expansion_suspended=inventory["source_expansion_suspended"],
                 unresolved_dependency_addresses=sorted(unresolved),
                 source_installation_available=False, selected_cohort_advance_allowed=False,
                 existing_C_reuse_review_is_missing_function_count=False,
                 replacement_C_generated=False, behavior_comparison_complete=False, whole_game_equivalence=False,
                 indirect_call_coverage_complete=False, host_execution=False)
    add("index.json", encoded(index))
    # Preflight every managed destination before creating anything. Preserve
    # exact bytes/line endings; identical repeats do not rewrite existing files.
    for name, payload in files.items():
        destination = (output / name).resolve()
        if not destination.is_relative_to(output):
            raise ValueError("Output symlink escapes its packet: " + name)
        if destination.exists() and (not destination.is_file() or destination.read_bytes() != payload):
            raise ValueError("Existing output differs; choose a fresh directory: " + str(destination))
    for name, payload in files.items():
        if name == "index.json":
            # The index is the completion marker. Recheck live inputs after
            # preparation/copying so a mid-run edit cannot bless a stale packet.
            if file_sha(inventory_path) != digest(inventory_bytes):
                raise ValueError("Inventory changed while preparing")
            verify_files(inputs, "Inventory input changed while preparing: ")
            verify_files({name: source_hashes[name] for name in source_refs}, "Source changed while preparing: ")
            if file_sha(binary) != PIN or file_sha(Path(__file__)) != index["tool_sha256"]:
                raise ValueError("Native binary or preparation tool changed while preparing")
        destination = output / name
        if not destination.exists():
            destination.parent.mkdir(parents=True, exist_ok=True)
            with destination.open("xb") as stream:
                stream.write(payload)
    return dict(review_entries=len(assigned), batches=len(batches),
                shared_function_records=len(relevant), unique_source_blobs=index["unique_source_blobs"],
                native_cache_files=len(cache_refs), files=len(files), output=str(output),
                unresolved_dependencies=len(unresolved),
                source_installation_performed=False, execution_performed=False)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--inventory", type=Path, required=True)
    parser.add_argument("--output", type=Path, required=True)
    args = parser.parse_args()
    try:
        print(json.dumps(prepare(args.inventory, args.output)))
    except (KeyError, TypeError, ValueError, OSError) as error:
        parser.exit(1, str(error) + "\n")


if __name__ == "__main__":
    main()
