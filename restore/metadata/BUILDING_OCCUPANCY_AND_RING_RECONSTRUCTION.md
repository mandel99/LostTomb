# Occupancy queries and literal ring growth, 2026-11-04

Three remaining register-frame queries now execute named ordinary C in
`runtime/building_occupancy_query.c`. The numbered C APIs, legacy actor-message
frames and gameplay building-query facade use the same algorithms.

| Native entry | C algorithm | Owned instructions |
| --- | --- | ---: |
| 004893C0 | restore_region_has_blocking_tile | 64 |
| 00489470 | restore_region_has_active_occupant | 12 |
| 004894A0 | restore_building_has_active_occupant | 13 |

4893C0 publishes the origin cell at BC9768 before checking dimensions. It uses
the5CC8C0 table's48-byte row stride and8-byte column stride. A mask containing
10000 first checks the tile's object pointer and byte24: a nonnull object with
byte0 returns true before the terrain test. Otherwise the tile's terrain DWORD
is tested against the complete mask. The first matching tile wins.

AL alone is the predicate. On success native EAX retains the matching tile's
high24 bits with AL1. On failure EAX retains height's high24 bits with AL0,
including negative/nonpositive-height cases and positive height with empty
width. The named algorithms preserve this full result instead of returning a
guessed Boolean.489470 fixes the mask to10000;4894A0 reads signed WORD building
Y then X and queries3x3. The incoming ECX receiver is not consumed by this
closure. The existing gameplay facade intentionally exposes only AL.

Typed numbered C APIs use explicit cdecl arguments. Legacy frames preserve
native RET4 for4894A0, RET16 for489470 and RET0 for4893C0 by advancing the
virtual stack correctly. Saved virtual registers remain unchanged.

Both Release executables pass182 scenarios/766 invocations each for these
queries, with all89 instructions and all16 conditional outcomes executed.
No child results are substituted: all three actual algorithms run. Cases
cover every position of a6x6 footprint, object state0/1/255, terrain/object
precedence, combined masks, signed WORD coordinate boundaries, nonpositive
dimensions, full-EAX false results, arbitrary unused ECX values and origin
publication mutations. Ordered stores, full original static memory, results,
saved registers and both physical/virtual cleanup contracts are compared.

## Removed gameplay divergence

`restore_ring_004f6a50` previously moved logical FIFO entries into a larger
zeroed allocation, recalculated the tail and added validation/abort conditions.
That was a compatibility repair rather than the original growth algorithm.
The gameplay adapter now calls the existing literal `FUN_004f6a50` C body in
`runtime/dynamic_history.c`, sharing the same behavior as canonical callers.

The original chain first copy-constructs a temporary ring at the old capacity,
reserves the requested destination, copies the temporary back, then destroys
it.4F6790 copies `(destination_capacity*4)>>2` DWORDs in forward order, after
32-bit byte-count wrapping. During growth the source temporary is smaller than
that destination-sized read. Its head/count/tail are copied without relocating
wrapped entries. These accesses and metadata values are retained; the repaired
FIFO relocation, padding zeros and extra abort guards are gone.

The growth oracle authenticates all33 native4F6A50 instructions and executes
both outcomes of its capacity comparison.19 new scenarios/49 invocations per
Release executable compare the canonical API, gameplay adapter, actual push
member and actual actor-to-building enrollment.35 additional canonical
constructor/reserve/copy/growth cases also execute per executable. All class
children run; only allocation/free receive controlled endpoints with identical
readable backing on both sides. Allocation order, direct-growth ordered
nonstack writes, final object fields, full heap backing/static data, saved ABI
and restored FS chain match. Assertions explicitly check copied DB padding,
unchanged head/count, native tail placement and enrolled values. The previous
repaired gameplay algorithm would fail these checks.

This establishes the finite normal-return algorithm, not arbitrary host heap
adjacency, allocation failure or C++ exception unwinding. The original source
extent issue still exists in the literal algorithm. A modern host allocator
does not establish original private-heap contents or layout. Those platform
and exceptional behaviors remain part of the unfinished full reconstruction.

```powershell
python direct_c/verify_building_occupancy.py build/c-only/Release/restore_link_check.exe
python direct_c/verify_building_occupancy.py build/c-only/Release/PharaohRestoreC.exe
python direct_c/verify_ring_growth.py build/c-only/Release/restore_link_check.exe
python direct_c/verify_ring_growth.py build/c-only/Release/PharaohRestoreC.exe
python direct_c/verify_labor_access_current.py
```

The paired building-occupancy and ring-growth reports bind current sources and
executables by hash. Native bytes and instruction sets are authenticated by
REA against original SHA256
`fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d`.
Full-game equivalence remains unproven. The4BF9D0 parent and its classification/permission queries now execute ordinary
C; see [processing queries](BUILDING_PROCESSING_QUERY_RECONSTRUCTION.md).
Next work includes timed-record predicates and remaining city/input/actor models, unresolved decompiler
fields, heap fidelity, exception paths and end-to-end gameplay.
