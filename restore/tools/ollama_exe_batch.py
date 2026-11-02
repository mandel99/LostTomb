"""Ask Ollama to reconstruct complete functions from the original EXE only.

The input is exact x86 disassembly bounded by the next known function entry.
No recovered C source, ZIP proposal, or partial source fragment is included.
Ollama writes proposals under artifacts/; Codex reviews and integrates them.
"""
from __future__ import annotations

import argparse
from datetime import datetime, timezone
import json
import os
from pathlib import Path
import subprocess
import urllib.request


ROOT = Path(__file__).resolve().parents[1]
DEFAULT_MANIFEST = ROOT / "metadata/ollama-exe-batch-03.json"
DEFAULT_OUTPUT = ROOT / "artifacts/ollama-exe-batch-03"

SCHEMA = {
    "type": "object",
    "properties": {
        "signature": {"type": "string"},
        "declarations": {"type": "string"},
        "definition": {"type": "string"},
        "abi_notes": {"type": "string", "maxLength": 1200},
        "uncertainties": {"type": "string", "maxLength": 1200},
    },
    "required": ["signature", "declarations", "definition", "abi_notes", "uncertainties"],
    "additionalProperties": False,
}

SYSTEM = """You reconstruct one COMPLETE 32-bit x86 MSVC function as conservative C.
The only semantic source is the complete original executable disassembly supplied by the user.
Return JSON only. Do not use assembly, intrinsics, placeholders, TODOs, invented helper behavior,
or code from any previous decompiler. Preserve cdecl/stdcall/fastcall ABI evidence, return-register
behavior when observable, exact access widths, signedness, wrapping arithmetic, global addresses,
call order, side effects, and branch order. Absolute memory must use explicit uintptr_t addresses.
If a called function's signature is not provable, declare it with the least-assuming old-style extern
and record the uncertainty. The definition must contain the entire function, never a fragment.
Compiler success is not behavior equivalence; Codex will review, repair, and test the proposal.
Be concise: emit only necessary declarations and the complete definition; keep notes under 200 words."""


def now() -> str:
    return datetime.now(timezone.utc).isoformat()


def read_json(path: Path):
    return json.loads(path.read_text(encoding="utf-8-sig"))


def write_json(path: Path, value) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    temporary = path.with_suffix(path.suffix + ".tmp")
    temporary.write_text(json.dumps(value, ensure_ascii=False, indent=2) + "\n", encoding="utf-8")
    os.replace(temporary, path)


def find_dumpbin() -> Path:
    roots = [
        Path(os.environ.get("ProgramFiles", "C:/Program Files")),
        Path(os.environ.get("ProgramFiles(x86)", "C:/Program Files (x86)")),
    ]
    matches = []
    for root in roots:
        base = root / "Microsoft Visual Studio"
        if base.exists():
            matches.extend(base.glob("**/VC/Tools/MSVC/*/bin/Hostx64/x86/dumpbin.exe"))
    if not matches:
        raise FileNotFoundError("Visual Studio x86 dumpbin.exe was not found")
    return sorted(matches)[-1]


def disassemble(dumpbin: Path, exe: Path, start: str, end: str) -> str:
    command = [str(dumpbin), "/nologo", "/disasm:nobytes", f"/range:{start},{end}", str(exe)]
    result = subprocess.run(command, check=True, capture_output=True, text=True, encoding="utf-8", errors="replace")
    lines = []
    in_code = False
    for line in result.stdout.splitlines():
        if line.startswith("  00"):
            in_code = True
            lines.append(line.strip())
        elif in_code and line.strip() == "":
            break
    if not lines:
        raise RuntimeError(f"No instructions returned for {start}..{end}")
    return "\n".join(lines) + "\n"


def post_ollama(endpoint: str, model: str, prompt: str, num_ctx: int):
    body = {
        "model": model,
        "system": SYSTEM,
        "prompt": prompt,
        "stream": False,
        "format": SCHEMA,
        "options": {"temperature": 0, "num_ctx": num_ctx, "num_predict": 4096},
        "keep_alive": "10m",
    }
    request = urllib.request.Request(
        endpoint + "/api/generate",
        json.dumps(body).encode("utf-8"),
        {"Content-Type": "application/json"},
    )
    with urllib.request.urlopen(request, timeout=900) as response:
        result = json.load(response)
    return result


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--manifest", type=Path, default=DEFAULT_MANIFEST)
    parser.add_argument("--output", type=Path, default=DEFAULT_OUTPUT)
    parser.add_argument("--model", default="pharaoh-coder:7b-32k")
    parser.add_argument("--endpoint", default="http://127.0.0.1:11434")
    parser.add_argument("--limit", type=int, default=0, help="0 processes every manifest entry")
    args = parser.parse_args()

    manifest = read_json(args.manifest)
    exe = ROOT / manifest["exe"]
    dumpbin = find_dumpbin()
    entries = manifest["functions"][: args.limit or None]
    summary = {
        "created": now(),
        "model": args.model,
        "source": str(exe),
        "input_policy": "complete original EXE disassembly only; no ZIP or recovered C input",
        "functions": [],
    }
    for index, entry in enumerate(entries, 1):
        unit = args.output / entry["name"]
        unit.mkdir(parents=True, exist_ok=True)
        if (unit / "proposal.json").exists():
            print(f"[{index}/{len(entries)}] {entry['name']} already complete", flush=True)
            summary["functions"].append({
                "name": entry["name"], "start": entry["start"], "end": entry["end"],
                "state": "needs_codex_review", "proposal": str(unit / "proposal.json"),
            })
            continue
        assembly = disassemble(dumpbin, exe, entry["start"], entry["end"])
        prompt = (
            f"Function: {entry['name']}\n"
            f"Complete analyzed range: {entry['start']} inclusive to {entry['end']} exclusive\n"
            f"Known context: {entry.get('context', 'none')}\n\n"
            "Complete original x86 disassembly:\n"
            f"{assembly}\n"
            "Produce one complete standalone C function definition plus only the declarations it needs."
        )
        (unit / "assembly.txt").write_text(assembly, encoding="utf-8")
        (unit / "prompt.txt").write_text(SYSTEM + "\n\n" + prompt, encoding="utf-8")
        print(f"[{index}/{len(entries)}] {entry['name']}", flush=True)
        raw = post_ollama(args.endpoint, args.model, prompt, manifest.get("num_ctx", 32768))
        (unit / "raw-response.txt").write_text(raw.get("response", ""), encoding="utf-8")
        write_json(unit / "ollama-metadata.json", {
            key: raw.get(key) for key in (
                "model", "created_at", "done_reason", "total_duration", "load_duration",
                "prompt_eval_count", "eval_count", "eval_duration"
            )
        })
        try:
            proposal = json.loads(raw["response"])
            write_json(unit / "proposal.json", proposal)
            state = "needs_codex_review"
        except (KeyError, json.JSONDecodeError) as error:
            write_json(unit / "parse-error.json", {"error": str(error), "created": now()})
            state = "incomplete_response"
        summary["functions"].append({
            "name": entry["name"], "start": entry["start"], "end": entry["end"],
            "state": state, "proposal": str(unit / "proposal.json") if state == "needs_codex_review" else None,
        })
        write_json(args.output / "summary.json", summary)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
