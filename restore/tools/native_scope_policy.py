"""Narrow, reviewed scope exceptions for saved native inventory records.

Body ranges authenticate coverage, not behavior or a replacement C algorithm.
Shared/pre-entry ranges are never flattened into automatic template candidates.
"""
from __future__ import annotations

import hashlib
import json
from pathlib import Path


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def address(value):
    if not isinstance(value, str):
        raise ValueError("Expected an observed hexadecimal address")
    return int(value, 16)


def ranges(body):
    result = [(address(item["start"]), address(item["end"])) for item in body["ranges"]]
    if not result or any(start > end for start, end in result):
        raise ValueError("Empty or reversed native body range")
    if any(start <= previous + 1 for (_, previous), (start, _) in zip(result, result[1:])):
        raise ValueError("Native ranges must be ordered, disjoint and maximally merged")
    total = sum(end - start + 1 for start, end in result)
    if (body.get("available") is not True or body.get("contains_entry") is not True or
            body.get("provenance") != "ghidra-function-body-address-set" or
            body.get("non_contiguous") != (len(result) > 1) or
            body.get("total_bytes") != total or
            body.get("span_bytes") != result[-1][1] - result[0][0] + 1):
        raise ValueError("Inconsistent observed native body metadata")
    return result


def load_reviewed(root, pin, inputs):
    """Require the curated finding, two agreeing REA operations and pinned bytes.

    Only explicitly reviewed findings are eligible. Other catalogue scope gaps
    retain ordinary review, including the qualified 48A720 foreign epilogue.
    """
    name = "metadata/native-function-scope-findings.json"
    path = root / name
    if not path.exists():
        return {}
    findings = json.loads(path.read_bytes())
    if (findings.get("schema") != "native-scope-conflicts-two-address-audit-v1" or
            findings.get("native_sha256") != pin):
        raise ValueError("Wrong reviewed native-scope findings")
    evidence_name = findings["evidence"]["path"]
    evidence_path = (root / evidence_name).resolve()
    if (not evidence_path.is_relative_to(root.resolve()) or
            sha(evidence_path) != findings["evidence"]["sha256"]):
        raise ValueError("Reviewed native-scope evidence is missing or changed")
    inputs[name] = sha(path)
    inputs[evidence_name] = sha(evidence_path)
    records = json.loads(evidence_path.read_bytes())["records"]
    excluded = {address(value) for value in findings["explicitly_excluded"]}
    expected = {address(value) for value in findings["scope"]}
    accepted = {}
    for finding in findings["findings"]:
        entry = address(finding["address"])
        if (entry in excluded or entry in accepted or entry not in expected or
                address(finding["analyzed_entry"]) != entry or
                finding.get("new_entry_credit") != 0 or finding.get("proposed_C_repair") is not None):
            raise ValueError("Unreviewed or contradictory native-scope finding")
        matched = {}
        for record in records:
            operation = record.get("operation")
            if operation not in ("procedure_info", "resolve_containing_procedure"):
                continue
            query = record.get("parameters", {}).get("procedure" if operation == "procedure_info" else "address")
            if query is None or address(query) != entry:
                continue
            raw = record.get("raw_result")
            # procedure_info retains the provider's value/limitations envelope.
            if operation == "procedure_info" and isinstance(raw, dict) and "value" in raw:
                raw = raw["value"]
            if (record.get("subject", {}).get("digest", {}).get("sha256") != pin or
                    record.get("provider", {}).get("id") != "ghidra" or
                    record.get("normalized_result") != raw or operation in matched):
                raise ValueError("Conflicting or unpinned native-scope REA record")
            matched[operation] = record
        if set(matched) != {"procedure_info", "resolve_containing_procedure"}:
            raise ValueError("Native scope requires both observed entry and containment")
        info = matched["procedure_info"]["normalized_result"]
        containing = matched["resolve_containing_procedure"]["normalized_result"]
        body = info["body"]
        body_ranges = ranges(body)
        if (address(info["entrypoint"]) != entry or containing.get("found") is not True or
                address(containing["query_address"]) != entry or
                address(containing["procedure"]["address"]) != entry or
                containing["procedure"]["body"] != body or
                info["length"] != body["total_bytes"] or
                finding["native_bytes"] != body["total_bytes"] or
                [(address(item["start"]), address(item["end"])) for item in finding["analyzed_body_ranges"]] != body_ranges or
                not any(start <= entry <= end for start, end in body_ranges)):
            raise ValueError("Reviewed native entry/ranges disagree with REA observations")
        accepted[entry] = dict(entrypoint=f"0x{entry:08x}", ranges=body["ranges"],
                               total_bytes=body["total_bytes"], template_eligible=False,
                               findings=name, evidence=evidence_name,
                               evidence_ids=sorted(record["evidence_id"] for record in matched.values()),
                               behavior_equivalence_proved=False)
    if set(accepted) != expected:
        raise ValueError("Reviewed native scope list is incomplete")
    return accepted


def check_cached(entrypoint, entry, pe, reviewed):
    """Check every instruction and range against the pinned loaded image."""
    info, assembly = entry.get("info", {}), entry.get("assembly", {})
    body_ranges = [(address(item["start"]), address(item["end"])) for item in reviewed["ranges"]]
    if (address(info["address"]) != entrypoint or address(info["body_start"]) != body_ranges[0][0] or
            address(info["body_end"]) != body_ranges[-1][1] or info.get("body_size") != reviewed["total_bytes"]):
        return "conflicting", "Cached scope differs from reviewed REA entry/ranges", entry, None
    items = assembly.get("items")
    if not isinstance(items, list) or not items or assembly.get("has_more") or assembly.get("total") != len(items):
        return "incomplete", "Reviewed scope needs the complete saved instruction list", entry, None
    region, cursor, boundaries = 0, body_ranges[0][0], set()
    try:
        for item in items:
            start = address(item["address"])
            fragment = bytes.fromhex(item["bytes"])
            if region >= len(body_ranges) or start != cursor or not fragment or cursor + len(fragment) - 1 > body_ranges[region][1]:
                return "incomplete", "Saved instructions do not exactly cover reviewed body ranges", entry, None
            offset = start - pe.base
            if offset < 0 or bytes(pe.data[offset:offset + len(fragment)]) != fragment:
                return "conflicting", "Reviewed instruction bytes differ from the pinned image", entry, None
            boundaries.add(start)
            cursor += len(fragment)
            if cursor == body_ranges[region][1] + 1:
                region += 1
                if region < len(body_ranges):
                    cursor = body_ranges[region][0]
    except (KeyError, TypeError, ValueError):
        return "incomplete", "Malformed reviewed-scope instruction bytes", entry, None
    if region != len(body_ranges) or entrypoint not in boundaries:
        return "incomplete", "Reviewed body coverage or execution entry is missing", entry, None
    return "complete_pinned", "Reviewed entry and all body ranges match the pinned image; flat template emission disabled", entry, None
