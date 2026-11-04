# Building removal — 2026-11-03

Four further native bodies now execute named C algorithms in
`runtime/building_removal.c`. Their canonical numbered sources and existing god,
invasion and city adapters call those algorithms.

| Native entry | C algorithm | Recovered behavior |
| --- | --- | --- |
| 004C03D0 | `restore_building_clear_four_parts` | Four footprint clears; one signed orientation adjustment; cached layout pointer with record fields reloaded between children |
| 0043FD00 | `restore_building_clear_farm_tiles` | Stored-cell origin, signed division/remainder by228, sizes3/2/4/5, exact field masks and nine ordered refresh calls |
| 004695D0 | `restore_building_clear_linked_parts` | Independent signed WORD backward/forward links;525-link cap per direction; footprint clearing or the real destruction boundary |
| 0048C0E0 | `restore_building_spawn_removal_effects` | Four or16 class6 actors, WORD position arithmetic, projection, BYTE direction, DWORD direction test and optional sound17 |

48C0E0 tests only the low BYTE of the expanded flag. By contrast4695D0 tests its
whole DWORD destruction argument. Actor owner and direction inputs consume only
their native BYTE widths. The effect's direction publication atBCD651 and its
DWORD atBCD754 are distinct; the latter is1 precisely when direction<3.

43FD00 clears only the WORD atEA6084 when the record's actual type is209,
independently of its size-selection argument. Its origin is the DWORD at
`799AA8 + building*264`, not the record's X/Y. Its child rectangles retain the
native extra far-edge row/column. It reloads the77567C refresh gate between
tiles and resets that gate before the ordered final refreshes.

## Verification

Both current Release executables pass160 scenarios and757 invocations for these
four bodies. All432 owned instructions,41 feasible conditional outcomes and
five switch arms execute. The sole excluded conditional outcome is48C103→48C1EC:
the preceding BYTE normalization produces exactly4 or16, never a nonpositive
count. No native instruction is excluded.

The corpus compares ordered owned stores, consumed child inputs, final original
static memory, stack cleanup and saved registers across named functions,
canonical entries, raw frame adapters and the three existing compatibility APIs.
It includes negative/sentinel/cyclic links, both525 caps, orientation changes
between children, signed-coordinate overflow, Word wrapping, failed/reused actor
slots and direction/owner changes at child boundaries.

Ten scenarios execute actual deeper children: two four-part footprint clears,
one linked-footprint clear, three farm clears with every ordered map-refresh
child, one linked full-destruction case and three actor allocation/projection paths including the full actor
pool. The actual sound helper runs with sound disabled in these cases. Other
scenarios control child algorithms to cover the complete owned callers; complete
child coverage and audible/physical output are separate obligations.

The extended access-search corpus has414 scenarios and1656 invocations per
Release executable. Its three new458270 compositions run every direct child
without substituted results for types31,202 and1. The fixture contains one
selected building at matching record/grid coordinates, an empty actor pool and
an empty notification queue. Each native/C pair finishes with building status4,
16 allocated actors and one queued event. Those are finite complete removal
compositions; they do not prove all game states or playable gameplay.

`building-removal-{diagnostic,game}.json` and
`building-access-search-{diagnostic,game}.json` pin the results to current source
and executable hashes. The REA native corpus and caller records pin the original
executable to SHA256
`fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d`.

```powershell
python direct_c/verify_building_removal.py build/c-only/Release/restore_link_check.exe
python direct_c/verify_building_removal.py build/c-only/Release/PharaohRestoreC.exe
python direct_c/verify_building_access_search.py build/c-only/Release/restore_link_check.exe
python direct_c/verify_building_access_search.py build/c-only/Release/PharaohRestoreC.exe
```

46D100/4C04A0 and their supporting bodies now execute named ordinary C with
199 scenarios per binary; see [building destruction](BUILDING_DESTRUCTION_RECONSTRUCTION.md).
Finite real15-pass access-repair compositions now pass; see
[map access repair](MAP_ACCESS_REPAIR_RECONSTRUCTION.md). Next work includes
remaining region/city/input/actor instruction models and broader state coverage. Event publication
is exercised in the three removal fixtures; general notifications, enabled sound,
physical screen behavior and full gameplay remain unverified.
