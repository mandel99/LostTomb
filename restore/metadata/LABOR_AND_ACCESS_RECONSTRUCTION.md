# Labor and building access — 2026-11-03

The current Release library and both final executables compile and link. The
diagnostic executable initializes the recovered data and exits successfully.
No host game process was launched for these checks.

## Recovered contracts

| Native entry | Named C algorithm | Result and publications |
| --- | --- | --- |
| 00459070 | `restore_housing_displace_overcrowding` | Signed WORD capacity/population; real displacement child; live housing cursor/count |
| 00459100 | `restore_labor_rebuild_demand` | Ten demand records; BYTE category; signed WORD coverage; DWORD labor demand |
| 00459400 | `restore_labor_allocate_groups` | Ten groups in initial totals; nine priority/weighted slots; wrapping counters and actual percentage helper |
| 00459350 | `restore_labor_update_unemployment` | Five-point increase limit, upper cap100 and shortage notification |
| 004595A0 | `restore_labor_rebuild_shares` | WORD per-building shares; reloads follow actual child boundaries |
| 00459670 | `restore_labor_assign_buildings` | Three ordered scans; WORD workers, remaining-group allocation and linked-gate updates |
| 00457A40 | `restore_building_refresh_route_flags` | Refresh BYTE flags for kinds75/192 using the real distance grid |
| 00457AF0 | `restore_building_update_access` | Full514-instruction controller, household timers/displacement, special-building entrances, pending warning and up to15 map repairs |
| 00481420 | `restore_building_has_access_neighbor` | Full EAX Boolean;24-slot offset table with immediate zero terminator; terrain BYTE bit4 and DWORD distance |
| 00481060 | `restore_building_find_access` | Ordered region preference followed by minimum positive DWORD distance; two ordered DWORD output coordinates |
| 00481210 | `restore_building_find_wide_access` | Three horizontal footprints; native fallback adds the winning shift twice |
| 00487D00 | `restore_building_fixed_entry` | Y+6 then X+9 publications; zero-based cell result; ignored third argument |
| 004F30B0 | `restore_building_special_entry` | Root/variant/level lookup, aliased output pointers and native full-EAX high bits |
| 00458270 | `restore_building_remove_highest_serial` | First strict maximum positive signed WORD creation serial among status1/3; cached bound and ordered cleanup |
| 004C03D0 | `restore_building_clear_four_parts` | Four layout-driven footprint clears with live record fields |
| 0043FD00 | `restore_building_clear_farm_tiles` | Exact farm masks, stored tile origin and ordered map refreshes |
| 004695D0 | `restore_building_clear_linked_parts` | Two signed WORD link chains, each capped at525 |
| 0048C0E0 | `restore_building_spawn_removal_effects` | Four/16 actors, wrapped positions, direction fields and sound gate |

The new access implementations are in `runtime/building_access.c`. Canonical
entries and the existing city/placement frame adapters route to these bodies.
The five additional access algorithms are in `runtime/building_access_search.c`.
Their canonical numbered files and city frame entries now call these named C
bodies. Four further removal bodies are in `runtime/building_removal.c`; see
[building removal](BUILDING_REMOVAL_RECONSTRUCTION.md) for their contracts and
the three complete real458270 removal compositions.

## Current native comparisons

Both `Release/restore_link_check.exe` and `Release/PharaohRestoreC.exe` pass:

* Labor:180 scenarios and843 invocations per binary, covering631/631 native
  instructions and213 feasible conditional outcomes. Actual child algorithms
  execute. The final weighted-loop edge45951C→459531 is infeasible: every ESI
  update has already exited on ESI≤0 at45950D, and skipped slots preserve ESI.
* Access:193 scenarios and772 invocations per binary, covering594/594 native
  instructions and all132 conditional outcomes. Controller children have
  explicit controlled results/output memory to exercise their caller contracts.
  Three additional scenarios inside this corpus execute the actual51CE90 flood
  and481420 neighbor child without result substitution. Six cases also compose
  the actual481060/481210/487D00/4F30B0 searches with the full457AF0 parent.
  Five further cases execute every reached direct child through initial
  connectivity, first-pass success and all15 exhausted real repair attempts.
* Access searches/removal:414 scenarios and1656 invocations per binary,
  covering524/524 native instructions, all84 conditional outcomes and all four
  switch arms. The four queries execute without controlled endpoints. Three
  removal cases execute the actual475B50 root lookup,47C2B0 footprint clearing
  and516F90/5173A0 cost updates. Its remaining children are explicitly controlled.
  Three further cases execute every direct child without substitutions and
  check status4,16 created actors and one queued event.
* Deeper removal:160 scenarios and757 invocations per binary, covering432/432
  instructions,41 feasible conditional outcomes and five switch arms. Ten
  cases run actual deeper algorithms; the impossible nonpositive4/16 count
  outcome is documented in the removal notes.

The comparisons check ordered owned stores, consumed child arguments, final
original static memory, stack cleanup and saved registers. BYTE return tests,
WORD timer overflow, live bounds/owner changes, distinct distance-load phases,
first/second/fifteenth repair success and repair exhaustion are covered. Void
functions expose no incidental volatile-register/flag contract.

The twenty-two `labor-allocation-{diagnostic,game}.json`,
`building-access-{diagnostic,game}.json` and
`building-access-search-{diagnostic,game}.json` plus
`building-removal-{diagnostic,game}.json` and
`building-destruction-{diagnostic,game}.json`,
`map-surface-{diagnostic,game}.json` and
`access-repair-flood-{diagnostic,game}.json` plus
`map-region-{diagnostic,game}.json`,
`building-occupancy-{diagnostic,game}.json` and
`ring-growth-{diagnostic,game}.json` and
`building-processing-{diagnostic,game}.json` reports bind these results to the
exact current source and executable hashes. REA byte records bind the native corpus
to executable SHA256
`fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d`.

The creator at46A490 reads the DWORD serial counter, stores its low WORD at
`7999EE + building*264`, increments it and publishes it at46A4AB. This establishes
the removal selection's field meaning. It retains signed WORD comparison and
excludes zero/negative/sentinel/wrapped values exactly as native; it does not
assume that a numerically highest signed value always means the newest building.
The producer and caller evidence are retained in the access search metadata.

## Five discarded-return crashes

At512D3C,5133C5,513757,514082 and51454E the instruction after CALL457AF0 is
`MOV EAX,[00EADD6C]`. The return is discarded before the building access WORD
query. Five synthetic `restore_unresolved_return(00457AF0)` aborts were removed
from `src/unit_02216.c`; the regeneration repair is restricted to that exact
embedded comma/query expression. Assigned results and unrelated queries remain
fail-closed. The native evidence, regeneration rule and current source pass
`direct_c/audit_building_access_returns.py`; MSVC x86 syntax checking also passes.
This is a source repair of the historical rendering body; it does not establish
complete rendering or change its currently selected linked provider.

## Reproduce

```powershell
python direct_c/verify_labor_allocation.py build/c-only/Release/restore_link_check.exe
python direct_c/verify_labor_allocation.py build/c-only/Release/PharaohRestoreC.exe
python direct_c/verify_building_access.py build/c-only/Release/restore_link_check.exe
python direct_c/verify_building_access.py build/c-only/Release/PharaohRestoreC.exe
python direct_c/verify_building_access_search.py build/c-only/Release/restore_link_check.exe
python direct_c/verify_building_access_search.py build/c-only/Release/PharaohRestoreC.exe
python direct_c/verify_building_removal.py build/c-only/Release/restore_link_check.exe
python direct_c/verify_building_removal.py build/c-only/Release/PharaohRestoreC.exe
python direct_c/verify_building_destruction.py build/c-only/Release/restore_link_check.exe
python direct_c/verify_building_destruction.py build/c-only/Release/PharaohRestoreC.exe
python direct_c/verify_map_surface.py build/c-only/Release/restore_link_check.exe
python direct_c/verify_map_surface.py build/c-only/Release/PharaohRestoreC.exe
python direct_c/verify_access_repair_flood.py build/c-only/Release/restore_link_check.exe
python direct_c/verify_access_repair_flood.py build/c-only/Release/PharaohRestoreC.exe
python direct_c/audit_building_access_returns.py
```

These offline oracles require the local original executable and bundled offline
verification dependencies. The C product build itself requires neither REA nor
the original executable. Complete child behavior, general map recovery, physical
screen output and full usable gameplay remain unverified. Next access work is
timed-record predicates and broader map/building states.
Five finite real repair compositions now pass without child substitutions; see
[map access repair](MAP_ACCESS_REPAIR_RECONSTRUCTION.md).
46D100/4C04A0 and three supporting algorithms now have501/501 instructions
and96 conditional outcomes checked in199 scenarios/995 invocations per binary,
including17 genuine compositions; see
[building destruction](BUILDING_DESTRUCTION_RECONSTRUCTION.md).

475720/47BAE0 now have378/378 instructions and94 conditional outcomes checked
in128 scenarios per binary; see [region updates](MAP_REGION_RECONSTRUCTION.md).

The occupancy-query closure and literal gameplay ring growth are now compared;
see [occupancy and ring growth](BUILDING_OCCUPANCY_AND_RING_RECONSTRUCTION.md).

The processing-site parent, permission and classification queries now have105
owned instructions checked in260 cases per binary; see
[processing queries](BUILDING_PROCESSING_QUERY_RECONSTRUCTION.md).
