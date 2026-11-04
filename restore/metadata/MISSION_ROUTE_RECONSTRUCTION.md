# Mission routes, load controllers and nonempty preview

All changes belong to `restore`. The C# project remains a read-only address and
format reference. Native instructions from Pharaoh.exe with SHA-256
`fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d`
determine behavior. No original game process or desktop automation is used.

## Recovered behavior

`004d4720` now receives its 324-byte route object explicitly. It estimates
capacity, preserves the native allocation/free order, samples line segments,
retains segment IDs and identifies the endpoint cities. Interpolation uses
the native single-precision direction components and truncating conversions;
the previous C source discarded both floating-point expressions before calls
to `0056133c`. Sprite dimensions are signed WORDs in 64-byte records, rather
than DWORDs addressed at twice the correct stride. The texture count is a full
DWORD. Both MAP/SAV controller callers now pass the route receiver.

`mission-routes-emulation.json` compares 938 cases with actual original FSQRT
and float-to-integer instructions. Full old/new allocation contents, guards,
route fields, allocator order, return byte and register/stack ABI agree. Cases
include signed coordinates, duplicate points, zero step, 50 control points,
existing buffers, graphics IDs above 255 and overlapping endpoint rectangles.

C rejects malformed counts above 50, missing/failed buffers and writes beyond
capacity. The original maximum-axis estimate can underallocate long diagonals.
These cases have separate C-only safety checks; the native comparison does
not deliberately reproduce a heap overflow.

`004d4ae0` and `004d41e0` retain full versions/counts, DWORD route lengths,
WORD legacy fields and the proper 0x49d8-byte player-record stride. The wrappers
`004d5820` and `004d58b0` preserve signed mission IDs and years, full difficulty
and editor flags, header-only ordering and the floating-point editor result.
The old generated code discarded that result before calling `0056133c` without
an operand. The real result is now consumed by C.

The settings methods `004bd330`/`004bd360` and constructor `004bd2d0` have
explicit receivers. Their global callers use 00d38360, while the parent
constructor uses object+0x32c40. Initializer `004bd280` passes 00d05720.
The omitted local constructor 004bd650 initializes four fields in each of
thirty 28-byte records; unknown fields are preserved. All corresponding caller
contexts and native bodies are retained in `mission-loaders-native.json`.

`mission-loaders-emulation.json` compares 340 controller/wrapper cases,
including format boundaries 114..160/256, failing chunks, full-width flags,
signed IDs, missing files, failed PAK lookups, field restoration and alternate
constructor receivers. Nested compatibility routines are controlled in this
suite; their implementation is not established by these controller checks.

## Complete modern MAP path

`mission-map-emulation.json` joins the actual controller, chunk loader,
block envelope reader and route reconstruction. Two modern MAP fixtures are
derived from the previously verified C decoding of a v160 mission and isolated
save. They use the native 24-entry MAP schema and raw-fallback envelopes.
They are not represented as separately authored original custom maps.

All 24 post-load chunk SHA-256 values for each map agree with the complete
native path, as do allocation contents/guards, lifetimes, read order/position,
route totals and ABI. The mission fixture reconstructs four routes with 240
points; the save-derived map correctly contains no nonempty routes. Legacy
migration and error endpoints are forbidden in this v160 comparison, rather
than replaced with simulated successful behavior.

The owned C startup diagnostic then loads this map through the actual family
menu and custom-mission selector. The read-only framebuffer capture also reads
the live list index, exact scenario-information hash and route totals from
the verified owned child. Its result is recorded in `mission-map-integration.json`.
The temporary fixture is hash-checked and removed after the bounded run.

## Numeric text found by preview inspection

The first nonempty preview exposed truncated numbers and a missing BC year.
Numeric composition walked the buffer at 00e34239 in four-byte steps, and
`004cccb0` omitted four arguments on its negative-year branch. Full-width text
advances, byte strides and actual draw arguments are restored in units 01697,
01698 and 01700..01702. Negating the minimum signed integer is defined through
unsigned wrap, matching native NEG without C signed-overflow assumptions.

`numeric-text-emulation.json` compares 912 cases against complete native number,
date, suffix/prefix and plural composition. Only final text/catalog drawing
uses controlled metrics. Full text buffers and guards, metric state, draw text,
arguments, order and ABI agree. The original digit-count quirk for ten-digit
values is preserved. Actual glyph output is checked separately in the C preview.

REA evidence for `004d4720` is retained in `mission-route-rea-function.json`
(evidence `ev_7b40f0e6cb5d03584a98d5c3037e3c45a28c422ba303cf19c6be62e57477c6c6`).
Ghidra disassembly and native emulation use the same pinned executable.

## Reproduce

```powershell
.\build.ps1 -Configuration Debug -Verify -BuildDirectory build/c-only-debug *> artifacts/menu-routes-debug-build.log
.\build.ps1 -Configuration Release -Verify *> artifacts/menu-routes-release-build.log
python tools/refresh_menu_native_evidence.py
.\tools\Test-MenuIntegration.ps1 -Cases map,campaign,mission,duplicate,cancel,delete
python tools/record_campaign_menu.py
python tools/record_mission_menu.py
python tools/record_menu_dialogs.py
python tools/record_mission_map.py
python tools/record_menu_reconstruction.py
```

Ordinary builds and CTests need neither the original EXE nor game assets.
Native comparisons and the owned-child integration require the local pinned
reference and isolated fixtures. The native refresh regenerates derived MAPs
when required by changed evidence.

## Remaining scope

The nonempty preview is currently captured at 1024x768. Other menu resolutions
have native draw-call checks but still need equivalent live C captures.
The Go to city path, briefing and complete city preparation are not proven.

Builtin v147 compatibility helpers and the complete SAV controller now have
native comparisons; see [the legacy mission notes](LEGACY_MISSION_RECONSTRUCTION.md).
The load/save selector has separate native and owned C checks in
[the file-menu notes](LOAD_SAVE_MENU_RECONSTRUCTION.md). Builtin preview,
confirmed city loading, editor, settings, rankings and root-menu branches
remain. Complete independent gameplay has not been established.
