"""Inventory every saved function and optionally emit separate exact-rule batches.

Read-only with respect to reconstruction sources, selected cohorts and evidence.
Reports and opt-in candidates must stay under restore/build. Existing C (including
inactive models and macro entrypoints) is reserved for reuse, never regenerated.
This is a work queue, not an arbitrary decompiler or an equivalence certificate.
"""
from __future__ import annotations

import argparse
from collections import Counter, defaultdict
import hashlib
import json
from pathlib import Path
import re
import sys
import xml.etree.ElementTree as ET

import bulk_reconstruct as rules

ROOT = rules.ROOT
PIN = rules.PIN
ADDRESS = re.compile(r"(?:0x)?[0-9a-fA-F]{6,8}\Z")
CANONICAL = re.compile(r"\bFUN_([0-9a-fA-F]{8})\b")
MODEL_ADDRESS = re.compile(r"(?:^|_)(00[0-9a-fA-F]{6})(?:_|$)")
SYMBOL = re.compile(r"\b((?:FUN_|restore_)\w*)\s*\(")
MACRO = re.compile(r"(?m)^\s*([A-Z_][A-Z_0-9]*)\s*\(")
COMMENT_STRING = re.compile(r'/\*[\s\S]*?\*/|//[^\n]*|"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\'')
MARKER = re.compile(r"\b(?:in_(?:E[A-Z]{2}|stack_[0-9a-fA-F]+)|unaff_\w+|extraout_\w+|restore_unresolved\w*)\b")
NAMESPACE = {"m": "http://schemas.microsoft.com/developer/msbuild/2003"}


def load(path):
    return json.loads(path.read_bytes())


def addr(value):
    if isinstance(value, int):
        return value
    if isinstance(value, str) and ADDRESS.fullmatch(value):
        return int(value, 16)
    return None


def label(address):
    return f"0x{address:08x}"


def relative(path):
    return path.resolve().relative_to(ROOT).as_posix()


def build_output(path):
    path = path.resolve()
    if not path.is_relative_to((ROOT / "build").resolve()):
        raise ValueError("Output must be under restore/build; source installation is unavailable: " + str(path))
    return path


def save_exact(path, data):
    """Idempotent emission; never silently replace a different candidate packet."""
    path = build_output(path)
    if path.exists() and path.read_bytes() != data:
        raise ValueError("Existing candidate differs; choose a fresh output directory: " + str(path))
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_bytes(data)


def encoded(value):
    return (json.dumps(value, indent=2) + "\n").encode()


def catalogue(inputs):
    """Do not drop records with missing/incomplete/conflicting native evidence."""
    addresses = set()
    records = defaultdict(list)
    units = defaultdict(list)
    for filename in ("units.json", "metadata/export_manifest.json"):
        path = ROOT / filename
        data = load(path)
        inputs[filename] = rules.sha(path)
        for item in data:
            address = addr(item.get("address") or item.get("provenance", {}).get("address"))
            if address is not None:
                addresses.add(address)
                if "file" in item:
                    units[address].append(item)
    cache = ROOT / "metadata/native-function-audit-cache"
    for filename in ("index.json", "requested.json"):
        path = cache / filename
        if not path.exists():
            continue
        data = load(path)
        if data.get("native_sha256") != PIN:
            raise ValueError("Wrong pinned cache: " + str(path))
        inputs[relative(path)] = rules.sha(path)
        addresses.update(a for a in (addr(x) for x in data.get("addresses", [])) if a is not None)
    for path in sorted(cache.glob("batch-*.json")):
        data = load(path)
        if data.get("native_sha256") != PIN:
            raise ValueError("Wrong pinned cache: " + str(path))
        inputs[relative(path)] = rules.sha(path)
        for item in data.get("entries", []):
            address = addr(item.get("requested_address"))
            if address is None:
                raise ValueError("Cache entry has no requested address: " + str(path))
            addresses.add(address)
            records[address].append((relative(path), item))
    return sorted(addresses), records, units


def cached_scope(address, variants, pe):
    if not variants:
        return "uncached", "No saved native record for catalogue address", None, None
    signatures = set()
    for _, entry in variants:
        signatures.add(json.dumps({"info": entry.get("info"), "assembly": entry.get("assembly")}, sort_keys=True))
    if len(signatures) > 1:
        return "conflicting", "Saved records disagree on native scope or assembly", None, None
    entry = variants[0][1]
    info = entry.get("info", {})
    assembly = entry.get("assembly", {})
    if addr(info.get("address")) != address or addr(info.get("body_start")) != address:
        return "conflicting", "Requested address is not the declared native entry/body start", entry, None
    items = assembly.get("items")
    if not isinstance(items, list) or not items or assembly.get("has_more"):
        return "incomplete", "Missing, empty or paginated instruction list", entry, None
    raw = bytearray()
    try:
        for item in items:
            if addr(item.get("address")) != address + len(raw):
                return "incomplete", "Instruction ranges are not contiguous from the entry", entry, None
            fragment = bytes.fromhex(item["bytes"])
            if not fragment:
                return "incomplete", "Instruction has no saved bytes", entry, None
            raw.extend(fragment)
    except (KeyError, TypeError, ValueError):
        return "incomplete", "Malformed saved instruction bytes", entry, None
    if len(raw) != info.get("body_size") or addr(info.get("body_end")) != address + len(raw) - 1:
        return "incomplete", "Instruction bytes do not cover the declared complete native extent", entry, None
    offset = address - pe.base
    if offset < 0 or bytes(pe.data[offset:offset + len(raw)]) != raw:
        return "conflicting", "Cached instruction bytes differ from the pinned native image", entry, None
    return "complete_pinned", "Complete contiguous scope matches the pinned image", entry, bytes(raw)


def call_edges(entry):
    direct = set()
    indirect = 0
    if entry:
        for item in entry.get("assembly", {}).get("items", []):
            instruction = item.get("instruction", "").strip()
            if re.match(r"CALL\b", instruction, re.I):
                match = re.fullmatch(r"CALL\s+(?:0x)?([0-9a-fA-F]{6,8})", instruction, re.I)
                if match:
                    direct.add(int(match[1], 16))
                else:
                    indirect += 1
    return sorted(direct), indirect


def active_sources(project, inputs):
    """Follow the main target's object-library references, not every test project."""
    active = set()
    projects = set()
    pending = [project.resolve()]
    while pending:
        path = pending.pop()
        if path in projects:
            continue
        projects.add(path)
        tree = ET.parse(path)
        inputs[relative(path)] = rules.sha(path)
        for node in tree.findall('.//m:ClCompile[@Include]', NAMESPACE):
            source = Path(node.attrib["Include"])
            active.add((path.parent / source).resolve())
        for node in tree.findall('.//m:ProjectReference[@Include]', NAMESPACE):
            reference = Path(node.attrib["Include"])
            pending.append((path.parent / reference).resolve())
    return active


def closing_paren(text, opening):
    depth = 1
    for position in range(opening + 1, len(text)):
        if text[position] == "(":
            depth += 1
        elif text[position] == ")":
            depth -= 1
            if not depth:
                return position
    return None


def scan_sources(active):
    """Conservatively reserve real definitions and macro/model ownership clues.

    Prototypes and ordinary calls are not definitions. A named inactive model is
    a reuse lead requiring review; its mere presence never proves correctness.
    """
    owners = defaultdict(list)
    source_info = {}
    for folder in ("src", "runtime", "include", "direct_c", "artifacts", "generated"):
        for path in sorted((ROOT / folder).rglob("*")):
            if path.suffix.lower() not in (".c", ".h", ".asm"):
                continue
            data = path.read_bytes()
            text = data.decode("utf-8-sig", errors="replace")
            clean = COMMENT_STRING.sub(lambda match: "\n" * match[0].count("\n") + " ", text)
            markers = sorted(set(MARKER.findall(clean)))
            found = set()
            for match in SYMBOL.finditer(clean):
                end = closing_paren(clean, clean.index("(", match.start(), match.end()))
                if end is None or not clean[end + 1:].lstrip().startswith("{"):
                    continue
                symbol = match[1]
                canonical = CANONICAL.fullmatch(symbol)
                address_match = canonical or MODEL_ADDRESS.search(symbol)
                if address_match:
                    found.add((int(address_match[1], 16), "canonical_definition" if canonical else "named_model_definition", symbol))
            # Only FUN tokens in an uppercase invocation count as macro entries;
            # numeric data-address macros are not function owners.
            for match in MACRO.finditer(clean):
                end = closing_paren(clean, clean.index("(", match.start(), match.end()))
                if end is not None:
                    for token in CANONICAL.finditer(clean[match.end():end]):
                        found.add((int(token[1], 16), "macro_entrypoint", match[1]))
            info = dict(source=relative(path), source_sha256=hashlib.sha256(data).hexdigest(),
                        active=path.resolve() in active, repair_markers=markers,
                        correctness_proved=False, ownership_meaning="Reuse lead; correctness unproved")
            if folder == "src":
                source_info[relative(path)] = (info, clean)
            for address, kind, symbol in sorted(found):
                owners[address].append(dict(**info, kind=kind, symbol=symbol))
    return owners, source_info


def manifest_addresses(path):
    data = load(path)
    result = set()
    for key in ("members", "entries", "new_entries"):
        for item in data.get(key, []):
            value = item.get("address") if isinstance(item, dict) else item
            address = addr(value)
            if address is not None:
                result.add(address)
    return result


def reuse_dispositions(inputs, owners):
    """Reuse saved source triage only while its active model bytes still agree.

    This proves that an existing C definition is present, not native behavior.
    Stale evidence falls back to ordinary review and cannot generate C.
    """
    accepted = {}
    unavailable = []
    for path in sorted((ROOT / "metadata").glob("*-reuse-dispositions.json")):
        data = load(path)
        inputs[relative(path)] = rules.sha(path)
        name, expected = data.get("model_source"), data.get("model_sha256")
        model = (ROOT / name).resolve() if isinstance(name, str) else None
        if (model is None or not model.is_relative_to(ROOT.resolve()) or
                not model.is_file() or rules.sha(model) != expected):
            unavailable.append(dict(path=relative(path), reason="Missing or changed model source; review is retained"))
            continue
        inputs[relative(model)] = expected
        for item in data.get("primary_excluded", []):
            address = addr(item.get("address"))
            if address is None or item.get("new_entry_credit") != 0:
                continue
            current = [owner for owner in owners[address]
                       if owner["source"] == name and owner["source_sha256"] == expected
                       and owner["active"] and owner["kind"] == "named_model_definition"]
            if not current:
                continue
            accepted[address] = dict(path=relative(path), sha256=inputs[relative(path)],
                                     model_source=name, model_sha256=expected,
                                     symbols=sorted({owner["symbol"] for owner in current}),
                                     reason=item["reason"], correctness_proved=False, new_entry_credit=0)
    return accepted, unavailable


def connected_batches(candidates, calls, size):
    """Pack whole weak call components where possible; declare oversized splits."""
    eligible = set(candidates)
    adjacent = {address: set() for address in eligible}
    for address in eligible:
        for child in calls[address]:
            if child in eligible:
                adjacent[address].add(child)
                adjacent[child].add(address)
    components = []
    unseen = set(eligible)
    while unseen:
        pending = [min(unseen)]
        component = set()
        while pending:
            address = pending.pop()
            if address in component:
                continue
            component.add(address)
            pending.extend(sorted(adjacent[address] - component, reverse=True))
        unseen -= component
        components.append(sorted(component))
    batches = []
    current = []
    for component in components:
        if current and len(current) + len(component) > size:
            batches.append(current)
            current = []
        while len(component) > size:
            batches.append(component[:size])
            component = component[size:]
        current.extend(component)
    if current:
        batches.append(current)
    component_for = {address: number for number, component in enumerate(components, 1) for address in component}
    return batches, component_for, components


def describe_batches(batches, component_for, components, calls, rows, size):
    batch_for = {address: number for number, batch in enumerate(batches, 1) for address in batch}
    summaries = []
    for number, batch in enumerate(batches, 1):
        dependencies = sorted({child for address in batch for child in calls[address] if child not in batch})
        summaries.append(dict(batch=number, entries=len(batch), addresses=[label(a) for a in batch],
                              category_counts=dict(sorted(Counter(rows[a]["category"] for a in batch).items())),
                              call_components=sorted({component_for[a] for a in batch}),
                              component_split=any(len(components[component_for[a] - 1]) > size for a in batch),
                              dependencies=[dict(address=label(child), category=rows[child]["category"] if child in rows else "outside_saved_catalogue")
                                            for child in dependencies],
                              cross_batch_dependencies=sorted({batch_for[child] for child in dependencies if child in batch_for}),
                              cached_indirect_calls=sum(rows[a]["cached_indirect_calls"] for a in batch),
                              native_scope_issues=[label(a) for a in batch if rows[a]["native_evidence"]["status"] != "complete_pinned"],
                              indirect_call_coverage_complete=False))
    return summaries


def emit_candidates(directory, batches, summaries, candidates, suspended):
    directory = build_output(directory)
    for summary, batch in zip(summaries, batches):
        packet = directory / f"batch-{summary['batch']:04d}"
        members = []
        for address in batch:
            row = candidates[address]
            canonical = f"FUN_{address:08x}"
            name, code = rules.generate(address, canonical, row["recognized_rule"])
            destination = (packet / row["source"]).resolve()
            if not destination.is_relative_to(packet.resolve()):
                raise ValueError("Candidate source escapes its separate packet: " + row["source"])
            save_exact(destination, code)
            members.append(dict(address=row["address"], source=row["source"], name=name,
                                canonical_symbol=canonical, rule=row["recognized_rule"], ownership=row["ownership"],
                                native_evidence=row["native_evidence"], candidate_sha256=hashlib.sha256(code).hexdigest()))
        save_exact(packet / "members.json", encoded(dict(schema="bulk-remaining-candidates-v1", native_sha256=PIN,
                   source_expansion_suspended=suspended, installation_performed=False, comparison_pending=True,
                   selected_cohort_advance_allowed=False, members=members, batch=summary)))
    save_exact(directory / "index.json", encoded(dict(schema="bulk-remaining-packets-v1", native_sha256=PIN,
               candidates=len(candidates), batches=summaries, source_expansion_suspended=suspended,
               source_installation_available=False, behavior_comparison_complete=False)))


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--report", type=Path, default=ROOT / "build/bulk-reconstruction/remaining-inventory.json")
    parser.add_argument("--write-candidates", type=Path, help="Opt-in separate packets under restore/build; never installs sources")
    parser.add_argument("--batch-size", type=int, default=100)
    parser.add_argument("--exclude-manifest", type=Path, action="append", default=[])
    parser.add_argument("--progress-manifest", type=Path, help="Default: highest-numbered cohort source-progress manifest")
    parser.add_argument("--project", type=Path, default=ROOT / "build/c-only/pharaoh_restored.vcxproj")
    parser.add_argument("--binary", type=Path, default=ROOT / "build/smoke-game/Pharaoh.exe")
    args = parser.parse_args()
    if args.progress_manifest is None:
        choices = [(int(match[1]), path) for path in (ROOT / "metadata").glob("cohort*-source-progress.json")
                   if (match := re.fullmatch(r"cohort(\d+)-source-progress\.json", path.name))]
        if not choices:
            parser.error("No cohort progress manifest; pass --progress-manifest explicitly")
        args.progress_manifest = max(choices)[1]
    if args.batch_size < 1:
        parser.error("--batch-size must be positive")
    args.report = build_output(args.report)
    if args.write_candidates:
        args.write_candidates = build_output(args.write_candidates)
    if rules.sha(args.binary) != PIN:
        raise ValueError("Pinned original SHA256 mismatch")
    sys.path[:0] = [str(ROOT / "build/offline-verification-deps"), str(ROOT / "direct_c")]
    from capstone import Cs, CS_ARCH_X86, CS_MODE_32
    from native_test_pe import PE
    pe = PE(args.binary)
    disassembler = Cs(CS_ARCH_X86, CS_MODE_32)
    disassembler.detail = True
    inputs = {"tools/bulk_remaining.py": rules.sha(Path(__file__)),
              "tools/bulk_reconstruct.py": rules.sha(ROOT / "tools/bulk_reconstruct.py"),
              "tools/bulk_compound_rules.py": rules.sha(ROOT / "tools/bulk_compound_rules.py")}
    addresses, records, units = catalogue(inputs)
    active = active_sources(args.project, inputs)
    owners, source_info = scan_sources(active)
    dispositions, unavailable_dispositions = reuse_dispositions(inputs, owners)
    selected = defaultdict(list)
    manifests = [ROOT / "metadata/city-active-buildings-reconstruction.json",
                 ROOT / "metadata/actor-cohort-100.json", ROOT / "metadata/actor-cohort-2.json",
                 args.progress_manifest, *args.exclude_manifest]
    for path in manifests:
        inputs[relative(path)] = rules.sha(path)
        for address in manifest_addresses(path):
            selected[address].append(relative(path))
    progress = load(args.progress_manifest)
    suspended = progress.get("source_expansion_suspended")
    if not isinstance(suspended, bool):
        raise ValueError("Progress manifest must declare source_expansion_suspended")
    rows = []
    candidates = {}
    calls = {}
    for address in addresses:
        evidence_status, evidence_reason, entry, raw = cached_scope(address, records[address], pe)
        direct, indirect = call_edges(entry)
        calls[address] = direct
        row = dict(address=label(address), category="review_needed", reason="Source ownership requires review",
                   native_evidence=dict(status=evidence_status, reason=evidence_reason,
                                        cache_files=sorted({path for path, _ in records[address]})),
                   cached_direct_callees=[label(child) for child in direct], cached_indirect_calls=indirect,
                   existing_owners=owners[address], selected_by=selected[address])
        if address in dispositions:
            row["existing_C_source_disposition"] = dispositions[address]
        primary = units[address]
        source = primary[0]["file"] if len(primary) == 1 else None
        if source:
            row["source"] = source
            if source in source_info:
                row["source_sha256"] = source_info[source][0]["source_sha256"]
                row["source_active"] = source_info[source][0]["active"]
        if selected[address]:
            row.update(category="existing_selected", reason="Reserved selected/cohort owner; reuse without regeneration")
        elif evidence_status == "uncached":
            row.update(category="uncached", reason=evidence_reason)
        elif evidence_status != "complete_pinned":
            row.update(reason=evidence_reason)
        elif address in dispositions:
            row.update(category="existing_C_preserved", reason="Saved source triage and matching active model preserve existing C; native behavior remains unproved")
        elif len(primary) > 1:
            row.update(reason="Multiple catalogue source mappings; canonical ownership requires review")
        else:
            other = [owner for owner in owners[address] if owner["source"] != source]
            # Register-like names may be explicit live inputs in a correct
            # frame model. Markers are triage heuristics, never correctness
            # signals: every separate definition is reserved for reuse review.
            protected = other
            info, clean = source_info.get(source, ({}, ""))
            if source and info and not info["repair_markers"]:
                row.update(category="existing_C_preserved", reason="C exists without a recognized repair marker; preserve it, correctness remains unproved")
            elif protected:
                row.update(category="existing_C_reuse_review", reason="Separate canonical/macro/frame/model C is a reuse lead; correctness remains unproved")
            elif not source or not info:
                row.update(reason="No unique readable catalogue C source; inspect saved/inactive models before translation")
            elif any(owner["kind"] == "macro_entrypoint" and owner["source"] == source for owner in owners[address]):
                row.update(category="existing_C_reuse_review", reason="Macro entrypoint shares an existing C algorithm; inspect the shared contract")
            else:
                row["repair_markers"] = info["repair_markers"]
                instructions = list(disassembler.disasm(raw, address))
                recognized = rules.rule(instructions) if sum(item.size for item in instructions) == len(raw) else None
                if recognized:
                    row["recognized_rule"] = recognized
                # Preserve the original generator's narrow ECX repair contract.
                marker = re.search(r"\b(?:u?int|undefined[124]|uint32_t|int32_t|char|byte|short|ushort|bool)\s+(?:\*\s*)*in_ECX\s*;", clean)
                canonical = f"FUN_{address:08x}"
                obj = args.project.parent / "pharaoh_restored.dir/Release" / (Path(source).stem + ".obj")
                if not recognized:
                    row.update(category="unsupported_rule", reason="Complete native scope is outside the existing proven rule set; semantic reconstruction required")
                elif not marker or len(re.findall(r"\bin_ECX\b", clean)) < 2 or re.search(r"\bin_ECX\s*=(?!=)", clean):
                    row.update(reason="Recognized native rule, but source is outside the existing unbound-ECX repair contract")
                elif not info["active"]:
                    row.update(reason="Recognized native rule, but source is not in the main target or its object libraries")
                elif not obj.is_file() or obj.stat().st_mtime_ns < (ROOT / source).stat().st_mtime_ns:
                    row.update(reason="Recognized native rule, but current canonical object ownership is unavailable/stale")
                elif "_" + canonical not in rules.defined_symbols(obj):
                    row.update(reason="Recognized native rule, but expected canonical symbol is absent from the source object")
                else:
                    row.update(category="safe_rule_candidate", reason="Complete pinned native template and live canonical object satisfy the existing generator contract")
                    row["ownership"] = dict(object_path=relative(obj), object_sha256=rules.sha(obj),
                                            original_source_sha256=info["source_sha256"])
                    row["native_evidence"].update(byte_sha256=hashlib.sha256(raw).hexdigest(),
                                                  bytes=len(raw), instructions=len(instructions))
                    candidates[address] = row
        rows.append(row)
    rows_by_address = {int(row["address"], 16): row for row in rows}
    batches, component_for, components = connected_batches(candidates, calls, args.batch_size)
    summaries = describe_batches(batches, component_for, components, calls, rows_by_address, args.batch_size)
    review = {address for address, row in rows_by_address.items()
              if row["category"] in ("unsupported_rule", "review_needed", "uncached", "existing_C_reuse_review")}
    review_groups, review_component_for, review_components = connected_batches(review, calls, args.batch_size)
    review_summaries = describe_batches(review_groups, review_component_for, review_components, calls, rows_by_address, args.batch_size)
    counts = dict.fromkeys(("existing_selected", "existing_C_preserved", "existing_C_reuse_review",
                            "review_needed", "safe_rule_candidate", "unsupported_rule", "uncached"), 0)
    counts.update(Counter(row["category"] for row in rows))
    report = dict(schema="bulk-remaining-inventory-v1", native_sha256=PIN, catalogue_entries=len(rows),
                  category_counts=counts, native_evidence_counts=dict(sorted(Counter(row["native_evidence"]["status"] for row in rows).items())),
                  inputs_sha256=inputs, active_source_count=len(active), source_expansion_suspended=suspended,
                  selected_cohort_advance_allowed=False, source_installation_available=False,
                  emission_allowed_during_suspension="Separate review candidates only; selected cohort is never modified",
                  whole_game_equivalence=False, complete_game_reconstruction=False, behavior_comparison_complete=False,
                  existing_C_classification_proves_correctness=False, native_catalogue_closed=False,
                  category_counts_are_missing_function_counts=False,
                  grouping="Deterministic weak components of cached direct candidate calls; whole components packed to batch size, oversized splits declared",
                  review_grouping="Same grouping restricted to unresolved/reuse-review functions; planning only, never emits replacement C",
                  indirect_call_coverage_complete=False, batches=summaries,
                  source_reuse_dispositions=dict(matched_entries=len(dispositions),
                      preserved_entries=sum(row["category"] == "existing_C_preserved" and "existing_C_source_disposition" in row for row in rows),
                      native_scope_review_retained=sum(row["category"] == "review_needed" and "existing_C_source_disposition" in row for row in rows),
                      unavailable=unavailable_dispositions, correctness_proved=False),
                  review_entries=len(review), review_batches=review_summaries, functions=rows)
    if args.write_candidates:
        emit_candidates(args.write_candidates, batches, summaries, candidates, suspended)
    rules.save(args.report, report)
    print(json.dumps(dict(catalogue_entries=len(rows), category_counts=counts,
                          native_evidence_counts=report["native_evidence_counts"], candidate_batches=len(batches),
                          review_entries=len(review), review_batches=len(review_groups),
                          emitted_candidates=len(candidates) if args.write_candidates else 0,
                          source_expansion_suspended=suspended, report=str(args.report))))


if __name__ == "__main__":
    main()
