# Building destruction: ordinary C, 2026-11-03

Five former register-frame implementations now execute named ordinary C in
`runtime/building_destruction.c`. Canonical entries, legacy god APIs and frame
adapters all route to these bodies. The adapters retain saved virtual registers
and advance their virtual stack by the native return address.

| Native entry | C name | Native instructions |
| --- | --- | ---: |
| 0046D100 | restore_building_destroy | 262 |
| 004C04A0 | restore_building_destroy_four_parts | 157 |
| 0046D0F0 | restore_building_destruction_root | 5 |
| 004BFEE0 | restore_building_clear_four_part_occupancy | 46 |
| 004E9170 | restore_population_remove_destroyed | 31 |

`building-destruction-native.json` retains full assembly, exact bytes and every
instruction address. `building-destruction-rea-evidence.json` authenticates them
against the shipped executable SHA256
`fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d`.
Capstone decoding is checked against the retained address set and original PE.
No inferred decompiler return value or register input remains in these bodies.

## Recovered behavior and load phases

46D100 delegates type202 immediately. Other types clear service/population state,
remove inhabitants when the pre-clear house flag and population are nonzero,
release type-dependent links, optionally clear the root farm and convert the
footprint to type99 rubble. It caches exactly4/9/16/25/36 tiles for sizes2..6;
other sizes have no additional tiles. Road cells (terrain bit4) are skipped.
A road at the original cell marks the root status5. The extra-tile loop still
runs. Kinds75/76/74/181/182/192 request the final refresh.

The special image flag requires the reloaded house byte to be nonzero and its
signed WORD level to be **at most1**, including negative values. The initial
house byte is read before the BC9694 clear; the later image flag is read after
child calls. Image selection reads the cell's variation byte even when using
the special house image. The normal image is F56C5C plus9 times low2 variation
bits; the frame is low3 record variation bits plus1. The destruction mode uses
only its low BYTE. Root metadata is stored before publication; additional
records receive metadata after publication. Allocation failure0 and a reused
root slot are deliberately preserved, including writes to record0.

4C04A0 clears four occupancy parts, converts the original record to rubble,
then allocates each part's template count **directly**, without squaring it.
View plus orientation is cached and reduced by8 only once if signed >=8.
Each part reloads the root coordinates, then caches its own origin. The template
count is reloaded after every publisher and before the frame store. Orientation
and layout changes at child boundaries do not retroactively change the cached
angle. This path does not write the ordinary destruction mode byte.

4BFEE0 caches root X/Y for all four clear calls but reloads the orientation for
each. It publishes both DWORD footprint dimensions before each clear child.
46D0F0 preserves the full-EAX result of475B50's capped root traversal.
4E9170 increments the city's destruction-loss counter, executes demographic
removal, sums100 signed WORD cohorts with a live DWORD accumulator, and updates
the signed high-water total. The explicit city offset survives global city
changes in children; the final comparison uses the last computed sum.

## Verification

Both Release executables pass199 scenarios/995 invocations each, through named,
canonical, frame and compatibility routes. All501 owned native instructions
and all96 conditional outcomes execute. Tests compare ordered owned writes,
consumed child inputs, full original static memory, defined query results,
stack cleanup and saved registers. Controlled child mutations distinguish
cached versus live fields, widths, coordinates, owners, image variations and
load/store phases; signed limits, mode truncation, road skips and failed/reused
allocations are included.

Seventeen cases run real children: three root traversals (including a cycle),
three demographic removals (including exhaustion), two occupancy clears, seven
whole ordinary/farm/refresh/house/6x6/special destruction paths and two direct
four-part destructions. Nine whole-destruction cases execute every direct child
without substituted results, using native templates and offset tables. Eight
assert the resulting4/7/36 rubble-record counts. An additional linked-destruction
composition in the removal corpus raises that corpus to160 scenarios and757
invocations per executable, with ten genuine deeper compositions.

```powershell
python direct_c/verify_building_destruction.py build/c-only/Release/restore_link_check.exe
python direct_c/verify_building_destruction.py build/c-only/Release/PharaohRestoreC.exe
```

The paired `building-destruction-{diagnostic,game}.json` reports bind sources and
executables by hash. These finite fixtures establish the owned algorithms and
selected real compositions. They do not establish every deeper-child path,
concurrency, enabled physical sound, screen output or complete playable gameplay.
The476530/47B1A0 surface bodies and517C20 blocker scan now execute ordinary C;
finite full real access-repair compositions pass. See
[map access repair](MAP_ACCESS_REPAIR_RECONSTRUCTION.md). Remaining work includes
the actor/building query closure and other city/input/actor instruction models.
The475720/47BAE0 region bodies now execute ordinary C; see
[region updates](MAP_REGION_RECONSTRUCTION.md).
