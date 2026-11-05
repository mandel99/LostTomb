# Civic and route-pixel C owners awaiting main selection

These eight ordinary C algorithms were missing from the current main source.
They are prepared for the next connected block and have not replaced the
current native-address facades or frame models yet. The main cohort still
contains 92 new algorithms plus 13 existing-code operations.

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

The total is 1,218 static native instructions, not execution coverage.
The four C files are the intended source owners. When integrating them,
select these files and wire the existing facades; do not copy or rewrite
their bodies into another runtime module or count the selection as another
new algorithm. Existing removal-effects, service-state, storage and grid
algorithms are called through their APIs and receive no new rewrite credit.

The block passes shared strict Win32 C11 object compilation. No current-main
integration or native/C behavioral comparison is claimed for these eight
functions. The standalone CMake project only compiles object files; it does
not launch the host game. Detailed current-main comparison belongs to the
next connected verification milestone.

The four callbacks and route advancement have full DWORD contracts; the
three movement leaf APIs promise their BYTE/WORD RAM effects and are void.
The vagrant's
death/non-special result deliberately clears the low byte of signed
half-age; it is not the sprite ID. The movement result is a literal local
0/1, independent of child scratch returns. Native child upper register bits
and CF can remain unpromised under the movement void APIs; no global RAW or
cue gate is closed by this block.

The metadata record binds complete scopes, exact source hashes, provider
searches, bounded static review and limitations. Exact REA envelopes are in
`../../metadata/actor-civic-followup-rea-records.json`. The older vagrant
queue's 1,823-byte extent is corrected by fresh authenticated bytes and a
complete direct CFG to 1,840 bytes /491 instructions. No historical report
has been rewritten.
