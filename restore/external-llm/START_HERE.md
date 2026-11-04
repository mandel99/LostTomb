# Pharaoh Restore: external LLM hand-off

This package is a frozen, text-only engineering snapshot for turning the current
hybrid Pharaoh + Cleopatra 2.1 reconstruction into a genuinely standalone x86
Windows executable.

## Read this first

The 4,402 files in `project/src/` are mechanically prepared decompiler output.
They compile as a static-library model, but that does **not** establish correct
calling conventions, return values, stack layouts, global-data layout, or game
behavior. Only the 23 addresses listed in
`project/metadata/runtime-bindings.json` are active, reviewed C replacements in
the working hybrid game.

The current hybrid executable is not standalone. It copies the complete original
PE image, loads it, patches 23 entry points to C, and calls the original entry at
`0x00562fea`. Read `MASTER_PLAN.md` before proposing implementation work.

## Package map

- `MASTER_PLAN.md` -- staged plan, architecture, milestones, and acceptance gates.
- `LLM_WORK_PROTOCOL.md` -- mandatory rules for patches and behavioral claims.
- `PROMPT_TO_EXTERNAL_LLM.txt` -- ready-to-paste task prompt.
- `generated/CURRENT_STATE.json` -- machine-readable snapshot and PE layout.
- `generated/FUNCTION_INDEX.csv` -- all functions, risks, calls, hashes, and status.
- `generated/CALL_GRAPH.json` -- direct recovered-function call graph.
- `generated/BOOTSTRAP_FRONTIER.json` -- CRT/game entry and their direct callees.
- `project/` -- current C sources, runtime, tests, metadata, and generation tools.
- `evidence/` -- combined REA output and retained text/JSON evidence.
- `MANIFEST.json` and `SHA256SUMS.txt` -- exact package integrity inventory.

## Trust labels

1. `differentially_verified_active_hook`: behavior and ABI were tested against the
   canonical original and the C implementation is active in the hybrid runtime.
2. `reviewed_compiled_unverified`: reviewed and compilation-tested, but not proven
   behaviorally equivalent and not active in the game.
3. `compilation_model_only`: mechanical decompiler output; treat signatures and
   types as hypotheses.
4. `blocked_*`: known register ABI, return-contract, or stack-layout uncertainty.

## What is intentionally absent

The original `Pharaoh.exe`, original DLLs, compiled binaries, object files, and
machine-code test fixtures are not included. The canonical original is identified
only by SHA-256:

`fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d`

Differential tests that execute original instructions must be run locally by the
owner against that exact binary. An external LLM can analyze this package and
produce small patches, but may not claim equivalence from compilation alone.

## Recommended first task

Implement only Milestones S0 and S1 from `MASTER_PLAN.md`: a new Win32 x86 shell
with its own entry point, a compatibility-data arena, a code-address registry,
and audits proving that the original `.text` is neither copied nor executable.
Do not try to link all 4,402 recovered functions into the first standalone build.

