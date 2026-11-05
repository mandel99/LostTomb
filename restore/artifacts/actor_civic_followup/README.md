# Civic and route-pixel C owners awaiting main selection

These seventeen ordinary C algorithms are selected directly by the main build.
Eight source owners were created in a5e4547 and are reused unchanged; nine
new Engineer/Fireman, warehouse and patrol algorithms were added in the second cohort. Original
frame models remain available. Selection does not count an algorithm twice.

| Native entry | Named C owner | Native instructions |
| --- | --- | ---: |
| 48A720 | restore_actor_update_vagrant | 491 |
| 48BFF0 | restore_actor_update_ghost | 63 |
| 48C200 | restore_actor_update_explosion | 54 |
| 491420 | restore_actor_update_trade_caravan | 425 |
| 4B3FF0 | restore_actor_advance_route_pixels | 118 |
| 4B4180 | restore_actor_update_route_error | 35 |
| 4B43A0 | restore_actor_step_route_pixel_y | 16 |
| 4B43E0 | restore_actor_step_route_pixel_x | 16 |
| 48C6A0 | restore_actor_update_engineer | 259 |
| 48DED0 | restore_actor_update_fireman | 463 |
| 48E5E0 | restore_fireman_try_assign_fire | 112 |
| 469A50 | restore_fireman_find_nearest_fire | 92 |

The total is 3,682 static native instructions, not execution coverage.
The eight C files are the source owners. Keep them selected directly; do not
copy their bodies into another runtime module or count selection as another
new algorithm. Existing removal-effects, service-state, storage and grid
algorithms are called through their APIs and receive no new rewrite credit.

The block passes shared strict Win32 C11 object compilation and both main
Release links. Native/C behavioral comparison for this new cohort remains
pending. The standalone CMake project only compiles object files; it does
not launch the host game. Detailed current-main comparison belongs to the
next connected verification milestone.

The six callbacks, fire queries and route advancement have full DWORD contracts; the
three movement leaf APIs promise their BYTE/WORD RAM effects and are void.
The vagrant's
death/non-special result deliberately clears the low byte of signed
half-age; it is not the sprite ID. The movement result is a literal local
0/1, independent of child scratch returns. Native child upper register bits
and CF can remain unpromised under the movement void APIs; no global RAW or
cue gate is closed by this block.

The current record is `../../metadata/actor-cohort-2.json`; the original
eight-owner creation record remains historical at a5e4547. Exact REA envelopes
are in `../../metadata/actor-cohort-2-rea-records.json`. The older vagrant
queue's 1,823-byte extent is corrected by fresh authenticated bytes and a
complete direct CFG to 1,840 bytes /491 instructions. No historical report
has been rewritten.

Warehouseman, delivery/query helpers and two service-patrol owners are also
selected. The qualified formation-soldier draft remains excluded.
