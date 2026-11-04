# Plan: remove the dependency on the original executable

## 1. Definition of the goal

The final program must be a new 32-bit Windows executable built from reviewed C,
reconstructed data/resources, and explicitly selected third-party/runtime
dependencies. It must not load, map, authenticate, copy, jump into, or read
`Pharaoh.exe` at runtime. The final reproducible build must also not require the
original executable as an input.

Two separate milestones must remain visible:

- **Runtime independence:** the produced executable starts without the original
  EXE being installed or present. A locally generated, non-code data snapshot may
  still be an intermediate build input.
- **Build independence:** a clean checkout builds without reading the original
  EXE. All required data has been reconstructed or checked in as generated local
  assets with provenance. No original `.text` bytes are present.

Compilation of decompiler output is not a completion criterion.

## 2. Current baseline

- Target: Pharaoh + Cleopatra 2.1, x86 PE32, image base `0x00400000`.
- Recovered procedure bodies: 4,402 (4,400 REA, 2 retained fallbacks).
- Active reviewed C hooks in the hybrid: 23.
- Recovered functions observing register inputs: 463.
- Unresolved void-return observations: 2,011 across 257 functions.
- Unresolved stack layouts: 20 functions.
- Original CRT entry: `0x00562fea` (`FUN_00562fea`).
- Recovered game-level WinMain-like routine: `0x00413ed0`
  (`FUN_00413ed0`). Its current signature and stack reconstruction are not yet
  trustworthy.
- The current hybrid copies the original PE and calls its original entry. It is a
  useful behavioral oracle, not a standalone reconstruction.

Original absolute address ranges:

| Region | RVA | Absolute range/purpose |
|---|---:|---|
| `.text` | `0x00001000` | `0x00401000` through approximately `0x0056e24d` |
| `.rdata` | `0x0016f000` | starts at `0x0056f000`; includes old IAT/data |
| `.data` | `0x00176000` | starts at `0x00576000`; virtual size `0x00cf1aa8` |
| `.rsrc` | `0x00e68000` | starts at `0x01268000` |

The source currently accesses many globals through absolute addresses. Rewriting
all of them before the first independent boot would combine too many risks.

## 3. Standalone architecture

### 3.1 New process and entry point

Create `PharaohRestoreStandalone.exe` as a normal MSVC Win32 x86 target with a new
`WinMain`/CRT startup. Do not reuse `FUN_00562fea`; it is recovered Microsoft CRT
startup code with unresolved SEH/stack state. Link the new executable at a high
base or with normal ASLR so the legacy absolute data interval remains available.

Initially call no game routine. The shell must initialize logging, validate its
data arena and code registry, run a self-test, and exit successfully.

### 3.2 Compatibility data arena

Reserve the old non-code address interval beginning at `0x0056f000` with
`VirtualAlloc` at exact addresses. Load a generated snapshot of the original
`.rdata` and `.data` initial bytes, then apply an explicit fixup table. Keep BSS
zeroed. Resources may initially be supplied as a separate generated blob or as
new PE resources.

This preserves current absolute `DAT_...` and string references while types are
recovered incrementally. The snapshot generator must:

1. accept only the canonical original SHA-256;
2. extract no `.text` bytes;
3. record each source section, RVA, file range, and output SHA-256;
4. separate initialized bytes from zero-filled virtual data;
5. identify old IAT entries and pointers into original `.text`;
6. emit a deterministic manifest and fixup inventory.

The checked-in/generated snapshot is an intermediate compatibility artifact. It
must never be described as fully reconstructed source data.

### 3.3 Make stale code impossible to hide

Reserve `0x00401000..0x0056f000` as `PAGE_NOACCESS` when possible. Never put new
C code at the old function addresses. Install a vectored exception handler that
logs attempted execution in that range, including the stale address and caller.
This turns every missed callback/vtable/function pointer into actionable evidence.

### 3.4 Code-address registry and fixups

Create one authoritative registry row per recovered function:

```text
old_address, C_symbol, calling_convention, argument_contract, return_contract,
verification_level, subsystem
```

Only verified rows receive callable C targets. Scan the data snapshot and source
for pointers into original `.text`. Replace known pointers with registry targets;
leave unknown pointers invalid and report them. The registry must be used for
window procedures, callbacks, vtables, constructor tables, exception tables, and
any address-valued globals. Do not cast an unreviewed C function merely to make a
type error disappear.

### 3.5 Imports and external libraries

Resolve Win32 imports through normal link libraries, not copied IAT bytes. Classify
the remaining external symbols in `project/metadata/link-audit.json` into:

- compiler/CRT helpers to be provided by MSVC;
- Win32, DirectDraw, and multimedia imports;
- Miles Sound System (`AIL_*`, `mss32.dll`);
- Bink/Smacker video APIs;
- decompiler artifacts that need corrected prototypes rather than linkage.

For the first independent boot, optional video/audio subsystems may use explicit
stubs that return a controlled “disabled” result. A stub must log every call and
must not fake success if the caller would later dereference an uninitialized
object.

## 4. Work plan and gates

### S0 — Freeze evidence and add standalone audits

Deliverables:

- deterministic extractor for non-code data/resources;
- manifest of old code pointers in data and source;
- standalone link/map audit;
- test proving no original `.text` bytes are packaged;
- test proving no runtime file access to `Pharaoh.exe`.

Gate: extraction is reproducible for the canonical hash, refuses a changed EXE,
and emits no byte from the original `.text` range.

### S1 — Independent shell

Deliverables:

- new `PharaohRestoreStandalone.exe` target, disabled by default until its inputs
  exist;
- own `WinMain`, logging, crash report, and `--self-test`;
- exact-address compatibility data arena;
- old-code `PAGE_NOACCESS` guard;
- empty code registry with validation.

Gate: it builds and runs after the original EXE is renamed or unavailable. It
does not call game code yet.

### S2 — ABI registry and bootstrap frontier

Start from `FUN_00413ed0`, not the recovered CRT entry. For it and every direct
callee:

1. recover the real prototype and calling convention from call sites/disassembly;
2. replace synthetic `restore_stack`/SEH artifacts with normal C locals;
3. resolve each used global’s width and initialization;
4. identify callbacks written to memory;
5. add focused unit/differential tests;
6. register only after tests pass.

Gate: a standalone bootstrap coordinator can execute deterministic initialization
steps and stop before graphics/audio without entering the old code range.

### S3 — Platform and resource layer

Recover or replace, behind small interfaces:

- command line, paths, INI/settings, locale, and save directories;
- window creation and message loop;
- logging/error reporting;
- text, campaign, image, and resource loading;
- timing/input;
- DirectDraw surface creation and palette handling;
- optional sound/video adapters.

Gate: standalone executable opens a window, loads text/campaign data, pumps the
message loop, and shuts down cleanly with audio/video disabled.

### S4 — Menu and front end

Work bottom-up by strongly connected subsystem, not numeric address order. For
each subsystem, recover leaf helpers first, then callers. Move absolute globals
used by that subsystem into typed state structures while retaining compatibility
aliases until all callers migrate.

Gate: main menu is rendered and interactive without any old-code fault.

### S5 — City/game loop

Prioritize simulation tick, map/grid access, UI dispatch, rendering, input,
serialization, and scenario loading. Maintain deterministic state hashes at
selected frame/tick boundaries and compare them with hybrid traces.

Gate: load/start a city, advance simulation, save, reload, and exit.

### S6 — Full-content compatibility

Cover campaign progression, combat, monuments, floods, trade, audio/video, and
edge/error paths. Replace remaining vendor-DLL use where required by the desired
distribution model.

Gate: scenario regression suite and long-running simulation pass.

### S7 — Remove compatibility artifacts

Replace the raw data arena with typed/generated data and normal resources. Remove
absolute-address macros, legacy fixups, and the old-address registry once no
consumer remains.

Gate: clean build contains no reference, pointer, section, fixture, or input from
the original executable and passes the full regression suite.

## 5. Function-level workflow

Use `generated/FUNCTION_INDEX.csv` and `generated/CALL_GRAPH.json`. Select a small
cohesive slice (normally 1–10 functions, never a blind 100-function rewrite):

1. establish callers/callees and subsystem;
2. recover ABI from multiple call sites;
3. list every global access and width;
4. write a behavioral contract before editing;
5. make the smallest C correction;
6. compile the complete affected unit;
7. run focused tests and, locally, differential checks against the canonical EXE;
8. run the hybrid game when the function affects integrated behavior;
9. update evidence and verification labels;
10. only then add it to the standalone registry.

Functions with `unresolved_stack_layout`, register inputs, or unresolved return
observations are blocked until those contracts are explicitly resolved.

## 6. Required automated checks

Every standalone change must keep these checks green:

- Win32 x86 MSVC build from a clean directory;
- intrinsic-width tests;
- focused subsystem tests;
- registry uniqueness and complete metadata;
- data snapshot bounds and hashes;
- no pointer/fixup target in original `.text` unless it has a verified registry
  replacement;
- import audit;
- PE section audit;
- no `Pharaoh.exe` open/read attempt during standalone startup;
- old-code execution guard test;
- smoke run with original installation path unavailable.

Compilation-only review is recorded as `compiled_unverified`, never “done.”

## 7. First concrete patch series

Keep the first series small and reviewable:

1. `standalone_data_extract.py`: deterministic `.rdata`/`.data`/`.rsrc` extraction
   and pointer inventory; no C changes.
2. `standalone/legacy_arena.[ch]`: reserve/load/protect exact-address non-code
   memory with unit tests using a synthetic fixture.
3. `standalone/code_registry.[ch]`: metadata, lookup, fixup validation, duplicate
   checks; initially register the 23 verified routines only.
4. `standalone/crash_guard.c`: old-`.text` no-access reservation and diagnostic
   exception logging.
5. `standalone/main.c` plus CMake target: initialize, self-test, and exit; no call
   to `FUN_00413ed0` yet.
6. `audit_standalone.py`: inspect the output PE and runtime trace for forbidden
   dependencies.

After these six patches pass S1, begin the `FUN_00413ed0` bootstrap frontier.

