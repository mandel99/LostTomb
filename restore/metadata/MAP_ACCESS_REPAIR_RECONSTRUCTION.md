# Map surface and access repair, 2026-11-04

Three more native bodies now use named ordinary C. Their original canonical
entries and frame adapters call the same typed algorithms.

| Native entry | C implementation | Owned instructions |
| --- | --- | ---: |
| 00476530 | restore_map_refresh_garden_tiles | 168 |
| 0047B1A0 | restore_map_refresh_canal_tiles | 86 |
| 00517C20 | restore_access_clear_first_blocker | 192 |

The first two names describe the inferred visual role; the authoritative
contracts below state the exact terrain masks, image bases and data flow.
The evidence does not recover original source names. The full algorithm bodies
are in `runtime/map_surface_repair.c` and `runtime/access_repair_flood.c`.

## Exact recovered behavior

476530 first visits row spans from BC9F24/BC9B94/BC9804. Terrain bit20 qualifies
unless bits600 are present. It reads the access byte, clears the image DWORD,
stores the access high nibble, then reads/marks the sprite byte. The initial
height is cached for this complete first pass. A separate second pass scans
the row spans again, attempting3x3 then2x2 matching footprints before falling
back to single tiles. Image offsets are7 for3x3,6/5/4 according to the low2
variation bits for2x2, and X/Y parity0..3 for single tiles, added to F565F4.
The second pass caches each row limit and cell sequence. It reloads height only
after a nonempty row; an empty row retains the previously cached height.

47B1A0 initializes the shared cell/row cursors and scans terrain4000 without
building bit8. It computes neighbor patterns through47DB00/484F00/484BE0 and
publishes image F5686C plus the two pattern offsets. The access and sprite
bytes are read before either is stored; the first pattern offset is read
between those stores. It subsequently tests47E000 and optionally computes the
second pattern through47E750. Width is reloaded only after processing an
eligible cell. Cell/column/row cursors, padding and height retain their distinct
native load phases.51F780 is exactly one RET and consumes neither pushed mode
argument. The C calls preserve its invocation count without assigning meaning
to unused stack data or incidental volatile registers.

517C20 clears the DWORD distance grid and starts an ordered north/east/south/
west breadth-first search in a51984-entry ring. It increments the search serial
with DWORD wrapping and initializes shared start, head and tail fields in their
native order. Signed-nonnegative cost bytes enqueue unvisited neighbors. The
first signed-negative neighbor whose terrain has any4120 bits ends the search
after three stores: sprite OR40, terrain AND03052E80, then access ANDF0.
That early-clear path returns0. Normal termination returns full-EAX0/1 from
the target's current distance. Both ring cursors publish the incremented value
before a separate wrap-to-zero publication. The cached tail survives directions
which do not enqueue. The public declaration now states the full DWORD Boolean
contract instead of leaving EAX's high bits unspecified by an8-bit return type.

## Native/C verification

Both Release executables pass the following retained differential suites:

* Surface passes:113 scenarios,452 invocations,254/254 instructions and all64
  conditional outcomes. Nine cases execute every direct child without result
  substitution, including the second neighbor-pattern path. Three fixtures
  assert every resulting image for3x3,2x2 and1x1 footprints.
* First-blocker flood:49 scenarios,196 invocations,192/192 instructions and all38
  conditional outcomes. Tests cover each ordered clear direction, signed costs,
  already visited neighbors, serial wrapping, each enqueue site's ring wrap,
  head wrap, early target success and final distance reload. Six cases execute
  the actual memory-clear child.
* The extended access-controller suite:193 scenarios,772 invocations,594/594
  instructions and all132 conditional outcomes. Five complete457AF0 fixtures
  execute every reached direct child without substituted results: initially
  connected, successful first repair, first repair with active surface tiles,
  a further scene with active region tiles, and all15 attempts exhausted. They assert initial/repeated flood counts,
  repair-pass counts, success-message publication and final removal dispatch.
  These fixtures have a16x16 map and no active buildings. Existing populated
  removal and search compositions remain separate evidence.

Comparisons cover ordered owned stores, consumed child inputs, complete original
static memory, defined results, saved registers and stack cleanup. Enabled real
children run without nested interception. The larger emulated stack accommodates
existing frame-based region callees; it does not alter their results or memory.

The two native catalogues and REA evidence bundles authenticate exact bytes and
all instruction addresses against original SHA256
`fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d`.
`map-surface-{diagnostic,game}.json`, `access-repair-flood-{diagnostic,game}.json`
and the updated access reports pin the tested source and executable hashes.

```powershell
python direct_c/verify_map_surface.py build/c-only/Release/restore_link_check.exe
python direct_c/verify_map_surface.py build/c-only/Release/PharaohRestoreC.exe
python direct_c/verify_access_repair_flood.py build/c-only/Release/restore_link_check.exe
python direct_c/verify_access_repair_flood.py build/c-only/Release/PharaohRestoreC.exe
python direct_c/verify_building_access.py build/c-only/Release/restore_link_check.exe
python direct_c/verify_building_access.py build/c-only/Release/PharaohRestoreC.exe
python direct_c/verify_labor_access_current.py
```

Complete gameplay remains unfinished. The finite real repair fixtures do not
prove all map/building states or every deeper-child instruction.475720 and
47BAE0 now execute named ordinary C with all378 instructions checked; see
[region updates](MAP_REGION_RECONSTRUCTION.md). Other city, input and actor models,
unknown decompiler assignments, physical rendering and enabled sound also remain.
