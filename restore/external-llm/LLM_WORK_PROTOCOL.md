# External LLM work protocol

## Non-negotiable rules

- Treat all recovered signatures, local types, globals, and control flow as
  hypotheses unless an evidence JSON explicitly validates them.
- Never equate “compiles” with “equivalent.” Use the label
  `compiled_unverified` until behavioral evidence exists.
- Do not copy or embed original `.text` bytes in a standalone artifact.
- Do not remove `restore_unresolved_return(...)` just to compile. First recover the
  real return contract from callers/disassembly.
- Do not invent stack arguments for register-based calls. Resolve `ECX`, `EDX`,
  callee cleanup, and preserved registers explicitly.
- Preserve 32-bit wraparound, signedness, memory widths, aliasing, overlap,
  evaluation order, and error paths.
- Do not perform broad formatting, renaming, or mass rewrites. Function names may
  remain address-based.
- Limit a patch to one architectural primitive or a cohesive group of at most ten
  recovered functions.
- Never activate an unverified replacement in the game or standalone registry.
- Never modify retained evidence to fit a proposed implementation.

## Required response for every proposed patch

Return these sections:

1. **Scope** — exact files/functions/addresses changed.
2. **Evidence** — callers, callees, ABI observations, globals, and source records
   used.
3. **Contract** — inputs, outputs, side effects, failures, and calling convention.
4. **Patch** — a unified diff, not a full rewritten project.
5. **Tests** — exact build/test commands and what each test proves.
6. **Unproven assumptions** — anything compilation or static analysis cannot
   establish.
7. **Status label** — one of `analysis_only`, `compiled_unverified`,
   `differentially_verified`, or `integrated_verified`.

If evidence is insufficient, produce an evidence-request checklist instead of
guessing code.

## Source precedence

Use sources in this order:

1. focused evidence JSON under `project/metadata/`;
2. REA evidence and procedure records under `evidence/rea/`;
3. combined REA output `evidence/Pharaoh.rea.c`;
4. prepared unit in `project/src/`;
5. baseline Ghidra output only as a cross-check.

An Ollama/reviewer status is workflow metadata, not behavioral evidence.

## Parallel-work boundary

Independent analysis may be parallelized by subsystem, but two workers must not
edit the same unit, shared ABI header, data registry, or CMake file concurrently.
Merge evidence/metadata first, then implementation. A human/Codex reviewer owns
final corrections, local original-binary differential tests, and activation.

