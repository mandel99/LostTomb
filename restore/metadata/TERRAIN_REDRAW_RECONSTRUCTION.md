# Terrain redraw and water-boundary reconstruction

This checkpoint restores `004767d0` and the footprint/predicate helpers it
actually uses. The owned C SAV confirmation now passes that stage, the
subsequent water-boundary failure, trade-price preparation and music updating.
The latest run reaches gameplay mode 1, passes the repaired river/flood
simulation and actor/prey updating. It now stops at the missing animation-record
receiver in `0041fdb0`; see ACTOR_UPDATE_RECONSTRUCTION.md. A completed city frame
and independent gameplay remain unproven.

## Behavioral authority

REA `terrain-redraw-rea-function.json` and the Ghidra instruction dossier
`terrain-redraw-next-native.json` both identify the full 635-instruction body
of `004767d0` in the original SHA-256
`fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d`.
The latter filename retains its historical name; the restored behavior is
verified by `terrain-redraw-emulation.json`, not by a static dossier alone.
Helper bodies and exact native constants are retained in
`terrain-redraw-helpers-native.json`.
The sibling C# project is a read-only reference; its approximate/incremental
terrain pipeline is not substituted for the original behavior.

## Actual C implementation

- `004767d0` keeps integer rectangle bounds and independent per-pass state.
  It restores native clipping, one-tile expansion, low-byte option flags,
  the building-retention cases, world-region tests and all three map passes.
- Building records have a 264-byte stride. Their types/links and tile building
  IDs are signed 16-bit fields. World pointers use four-byte strides; random,
  fertility, bitfield and draw-edge grids use byte strides. Image IDs remain
  DWORD values.
- Floodplain variants retain the exact native branches for soil values 0..100
  and encoded values 101..255. Native doubles are 12.0 and 96.0: their exact
  ratio is 0.125, so nonnegative x87 truncation is faithfully expressed as
  unsigned division by eight. Signed orientation division still truncates
  toward zero. Neighborhood selection retains its 0x225/0x226/0x229 groups.
- `0047c140` writes real footprints using the original 6x6 offset table,
  sequential frames, terrain masks, word-sized building IDs and byte flags.
  It preserves the native error flag and empty-dimension behavior.
- `0047c880` performs actual footprint checks: map margins, the real
  `0046f9a0` diamond-margin predicate, region availability, uniform floodplain
  state, word-grid obstructions and the required image value.
- `0046f9a0` retains the native upper EAX bits as well as AL, including wrapped
  signed arithmetic. `0047db00` gathers all eight neighbors and `00484f40`
  counts the full-width flags.
- `0047a1c0` and `0047a1f0` use full-width boundary globals and defined
  arithmetic. Their Boolean result is AL. With the boundary disabled, native
  high EAX bits are unspecified incoming scratch state; C defines those unused
  bits as zero. Enabled paths preserve the computed high EAX bits.
  Nineteen C caller expressions now pass the one actual native argument; the
  discarded second expression contained no function call or required side effect.
  Caller edits are retained in `water-boundary-native.json`.

## Verification

Both Debug and Release pass 69 standalone CTests.
The current aggregate is 96,442 native comparisons across 34 suites.
The new terrain-redraw suite contributes 4,624 cases. The expanded
terrain-update suite has 2,131 cases, including 882 boundary cases.

The redraw suite executes all six repaired terrain functions plus actual geometry
and the original footprint table. It compares complete watched memory,
callback order/arguments and intermediate globals, result EAX, stack cleanup
and saved registers. Soil probes cover all 256 byte values, five orientations,
both plain/rotated-variant paths, all reviewed building groups, pointer states,
clipped/empty rectangles and footprints 1..6. All 17 selected branch probes,
including retained building cases and the 1x1 through 4x4 fallback choices,
are required to be reached. The boundary suite checks full EAX when enabled
and AL when disabled.

Remaining building-state callbacks, final queued-building refresh, the
connected-neighbor graphic selector and rotated-variant callback are
controlled in the redraw suite. Valid geometry is 0..228; footprint table
dimensions are 0..6 and object/building pointers are valid fixtures.
Full water rendering and unrelated menu/gameplay algorithms are not proved by
these comparisons. No original game process or desktop automation is used.

## Owned C confirmation and remaining work

`city-confirm-redraw` passed the former terrain access violation and stopped
at the missing water-boundary return in `0047a1c0`.
After repairing that contract, `city-confirm-boundary` reached
`00443330` and stopped in `0044baf0` with a Debug uninitialized-receiver
check. The isolated C-created profile and copied SAV remained unchanged;
the temporary SAV was removed.

`city-price-next-native.json` retains complete native bodies for
`0044baf0`, `0044d4c0` and `00443330`. The later city-price checkpoint restores
their explicit list/record receivers, the actual 36-element local accumulation
array and byte-addressed player stride, with native caller validation.
The existing `0044bb90` town iterator covers a different record selection.
`water-render-next-native.json` retains the complete remaining water body
for later validation; passing the diagnostic path does not prove that whole
algorithm.

```powershell
.\build.ps1 -Configuration Debug -Verify -BuildDirectory build/c-only-debug
.\build.ps1 -Configuration Release -Verify
python tools/refresh_menu_native_evidence.py
.\tools\Test-MenuIntegration.ps1 -Cases load,map,campaign,mission,duplicate,cancel,delete
.\tools\Test-CityConfirm.ps1 -RunName city-confirm-next
```
