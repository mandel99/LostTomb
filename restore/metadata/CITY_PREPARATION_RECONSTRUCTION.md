# City entry, coverage and terrain preparation checkpoint

The standalone C build now restores the geometry/grid/world preparation and
the ordered saved-game entry controllers. This is a partial checkpoint.
The final owned C SAV confirmation now passes terrain, trade-value preparation
and music updating, reaching gameplay mode 1. It then stops at missing x87
computations in `004be770`, the river/flood simulation, in the historical music
diagnostic. The later flood repair passes that boundary and reaches actor
updating. The subsequent actor/prey repair passes that callback and reaches
the missing animation-record receiver in `0041fdb0`. No completed city
frame or independent playable gameplay is established. See
TERRAIN_REDRAW_RECONSTRUCTION.md and CITY_PRICE_MUSIC_RECONSTRUCTION.md for
the later repairs and their limits, followed by FLOOD_SIMULATION_RECONSTRUCTION.md.

## Native authority and reference

The original SHA-256 is
`fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d`.
`metadata/city-prepare-native.json`, `city-coverage-native.json` and
`terrain-update-native.json` retain complete Ghidra instruction bodies.
REA's completed `city-prepare-rea-function.json` independently identifies that
binary and the `004d52a0` controller. The sibling `PharaohRewrite` remains a
read-only address/structure reference, not a behavioral specification.

## Restored C behavior

- `0046f7b0` computes the native 228-stride geometry and all three row tables.
  `0046f930` retains the complete native EAX value, including upper bits.
- `0046fa40`, `0046fc30`, `0046fc80` and `00475b80` reset the grids,
  mark borders, reproduce the native RNG state/grid and fill ground sprites.
  RNG and map flags use byte strides; scalar state uses full 32-bit widths.
- `004bd610`, `004bd6a0` and `004bd740` use explicit C receivers and
  restore region construction, node/payload destruction and sorted insertion.
  Equal-priority insertion behavior and untouched payload padding are retained.
  The small `004bd660` destructor wrapper calls the explicit C destructor;
  its native compiler-generated exception-unwind wrapper is not directly emulated.
- `004be0f0` resets the world and all 30 child regions. `004be160` builds
  regions using the native neighborhood propagation/filtering, allocation and
  linked-list behavior. Its scratch clear follows the original memset API.
  The native global-world writes are retained even with an alternate receiver.
- `004d5100` restores load ordering and its error return. `004d52a0`
  restores the ordered post-load controller, full-width player state, explicit
  world receiver and native version-dependent branches.
- `00468170` restores coverage percentages and the average over enabled
  scenario categories. Population records use a 0x49d8-byte stride; modes are
  signed 16-bit values. Category lookup, population-bin sums and household
  percentages execute as actual C functions in the comparison.
  The missing x87 sum/division is explicit C arithmetic. With no enabled
  categories the original masked invalid conversion leaves a zero low DWORD.
- `004cde60` deliberately wraps numerator multiplication to 32 bits before
  signed division. Negative results are retained; upper clamps belong to the
  callers. Zero denominators return zero. Signed IDIV overflow aborts in C.
- `00479770` uses full-width row/column/index counters and byte-sized
  auxiliary map grids. This removes the observed counter-wrap hang.
  `00475bf0` restores the rectangle-walk geometry widths; `004dea50`
  restores full-width dimensions/configuration and original pass ordering.

## Verification and limits

Both Debug and Release pass 69 standalone CTests without an original EXE.
The native emulator executes original instructions as comparison material,
never an original game process. The new suites pass 2,449 city-preparation
cases, 2,131 terrain-update/boundary cases and 4,624 terrain-redraw cases.
The current aggregate, including trade prices, music and flood simulation,
is 96,442 cases across 34 suites.

Comparisons check complete watched memory hashes, ordered endpoint arguments
and intermediate controller state, native stack and saved registers. City
cases also check allocation padding/guards and node/payload lifetime. Terrain
probes exercise interior/edge/outside points, clipped/empty rectangles and
configuration values whose high bytes matter.

The city suite controls allocation/free, memset, final region redraw, asset,
model and remaining deep city endpoints. The terrain suite executes the three
repaired functions plus actual geometry/predicate, while sprite resolution
`00486600` and the other individual terrain passes are controlled.
These are caller-contract proofs, not a claim that all deep algorithms work.
Geometry is bounded to valid 0..228 dimensions and valid list storage.
Invalid allocation/indexing aborts safely rather than reproducing a native
invalid-memory access. Pre-v147 loading and remaining menu/gameplay paths
also need restoration.

## Owned C diagnostics

`tools/Test-CityConfirm.ps1` copies a hash-verified v147 SAV to the isolated
`build/debug-startup-game` assets. Internal diagnostic input uses the actual
C buttons through title, family options, file selection and confirmation.
There is no desktop input automation or original executable launch.
The profile and fixture are hash-checked and the temporary SAV is removed.

Retained diagnostic evidence:

1. `city-confirm-first`: actual load/model preparation reached the missing
   x87 average in `00468170`, reported by the Debug uninitialized-value check.
2. `city-confirm-coverage`: that failure disappeared; the bounded debugger
   then found a hang in `00479770` during `00475bf0`.
3. `city-confirm-terrain`: the hang disappeared; execution reached
   `004767d0` and stopped with access violation c0000005. This run is
   explicitly a failed city-entry diagnostic, not a passed gameplay test.

The subsequent checkpoint restored the full contract and indexing of
`004767d0`, including its real footprint helpers. `city-confirm-redraw` then
found the water-boundary return defect; `city-confirm-boundary` passes it and
stops at `0044baf0` during `00443330`. The static 635-instruction body remains
in `terrain-redraw-next-native.json`; restored behavior is separately checked
by `terrain-redraw-emulation.json`. These are historical diagnostic boundaries.
The later `city-confirm-prices` passes the repaired trade-price preparation
and reaches the music receiver defect. `city-confirm-music` passes that stage,
reports `STARTUP frame mode=1`, then stops with `_RTC_UninitUse` from the
missing x87 calculations in `004be770` during `004d7090`. This trace does not
establish a completed rendered city frame. River/flood simulation is the next
concrete repair; its complete 303-instruction body is retained in
`city-simulation-next-native.json`.

`city-confirm-flood` subsequently passes that repaired controller and stops
at the raw `0049bd40` actor callback in `00489b70`. See the flood-simulation
notes for the full comparison and current next task.

```powershell
.\build.ps1 -Configuration Debug -Verify -BuildDirectory build/c-only-debug
.\build.ps1 -Configuration Release -Verify
python tools/refresh_menu_native_evidence.py
.\tools\Test-CityConfirm.ps1 -RunName city-confirm-next
```
