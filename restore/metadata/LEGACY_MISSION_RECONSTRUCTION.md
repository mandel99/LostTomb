# Legacy mission compatibility and complete SAV loading

Changes are implemented in the independent `restore` C project. `PharaohRewrite`
is a read-only address/structure reference. Original x86 instructions from the
pinned Pharaoh.exe remain the behavior oracle; no original game process was
launched, and no desktop input was sent.

## Restored contracts

- `004d0a40` processes ten snapshots, not eight. Each snapshot is 820 bytes and
  each dynamic object is 28 bytes. The previous temporary array was too small.
  The native import receives the whole snapshot by value plus an ECX receiver;
  C now uses explicit pointers and makes the required local snapshot copy.
- `004f67e0` imports metadata and resizes storage when necessary. The native
  function deliberately does not copy the snapshot's 200-value payload. Export
  at `004f6810` preserves padding, clamps capacity to 200, normalizes signed
  indices and reproduces the differing null/non-null buffer behavior.
- `004d19e0` converts all 200 city records using a correctly sized 98-byte
  temporary. Its three old bit masks become 32 byte fields with native priority.
- `004d40c0` uses the native signed 16-bit town count and explicit `004493c0`
  receiver. That predicate indexes a byte table rather than a DWORD table.
- `004d32f0` uses 388-byte walker records, 264-byte buildings, byte frame fields,
  16-bit owner/type fields and full-width counters. `004c5120` uses the same
  correct building layout. `004d3d00` likewise reads byte walker fields.
- Six post-load defaults and existing building conversion helpers execute in
  the checked path, including full-width zeroing at `006fdce0`.
- `0041fca0` now receives its object explicitly and walks the native 40-byte
  state rows and 48-byte sample records. All 65 calls in 41 C files pass the
  recovered receiver. Ghidra caller windows are recorded in
  `record-state-callers-native.json`. Other contracts in those callers, including
  `0041fd80` and the receiver of `0041fdb0`, remain separate work.

## Evidence

`legacy-mission-emulation.json` contains 1,566 native comparisons. They cover
city conversion, town links, walker kinds and frame thresholds, actual building
classification, all ten history objects, direct alternate history receivers,
allocation/free order, normalization and defaults. Complete watched-region
hashes include guards; allocations, return values and native ABI are checked.

`mission-save-emulation.json` compares the complete `004d41e0` controller and
all reached helpers for four local SAV streams: builtin missions 0 and 1 in
version 147, builtin mission 52 and the owned autosave in version 160. All 181
post-load descriptors agree per file: 724 block hashes, seven extra regions,
complete allocation contents/guards/lifetime, file read position/count,
random-call count and native stack/register preservation. The nonempty modern
mission rebuilds four routes with 240 points. A further 106 state-selection
cases check invalid IDs, row/choice boundaries, flags, sample bounds and 16-bit
state truncation, including an alternate receiver and output guards.

Only file/allocator/memmove endpoints, Global memory APIs and the deterministic
CRT-random endpoint are controlled in the full controller comparison. Codec,
record migrations, route construction, compatibility helpers and state resets
execute their real original instructions and real recovered C bodies.

REA evidence for `004d0a40` is `legacy-mission-rea-function.json`, evidence
`ev_cc634919877c82a1b2853e8312dfd4db73818d61187615db18cb0e4bf3f9b46a`.
Full Ghidra bodies are retained in `legacy-mission-native.json` and
`mission-save-native.json`.

## Reproduce and limits

```powershell
.\build.ps1 -Configuration Debug -Verify -BuildDirectory build/c-only-debug
.\build.ps1 -Configuration Release -Verify
python tools/verify_legacy_mission_emulation.py
python tools/verify_mission_save_emulation.py
```

Normal CTests use synthetic data and need neither original EXE nor game assets.
Native comparisons require the pinned local reference and retained corpus.
Negative history capacities and allocation failures abort safely in C instead
of reproducing invalid writes or infinite native loops.

This is finite coverage of v147/v160 loading and helper contracts. Pre-v147
controller branches fail explicitly in the harness. The nested load/save selector
now has native and real C selection/cancel checks; see
[the file-menu notes](LOAD_SAVE_MENU_RECONSTRUCTION.md).
Builtin preview integration, confirmed city loading/preparation, briefing and independent
gameplay remain unfinished. Existing nonempty live C preview coverage is for
the separately verified derived v160 MAP.
