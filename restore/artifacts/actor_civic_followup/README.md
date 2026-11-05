# Civic, delivery and walking C owners

These51 native entrypoints map to49 named C functions selected by the main
build. Shared generic providers receive no additional creation credit. Eight owners
were created before611f3cc; 33 were created since it and 10 prior-C
owners were reused, including narrow type/access repairs. Selection never counts a body twice.

| Native entry | Named C owner | Native instructions |
| --- | --- | ---: |
| 401B40 | restore_monument_worker_query_00401b40 | 56 |
| 401D40 | restore_actor_kind230_accept | 97 |
| 401ED0 | restore_actor_find_approach_kind230_mode | 35 |
| 401F30 | restore_actor_find_approach_kind230 | 9 |
| 40EC20 | restore_monument_base_worker_result | 71 |
| 40F070 | restore_actor_find_approach_kind225 | 34 |
| 417590 | restore_monument_base_worker_result | 59 |
| 4178E0 | restore_actor_find_approach_kind227 | 34 |
| 420430 | FUN_00420430 | 42 |
| 469A50 | restore_fireman_find_nearest_fire | 92 |
| 48A720 | restore_actor_update_vagrant | 491 |
| 48AE50 | restore_actor_find_remaining_delivery_destination | 61 |
| 48AEF0 | restore_actor_apply_cart_delivery | 156 |
| 48B320 | restore_actor_resource_cargo_image_group | 75 |
| 48B4A0 | restore_actor_select_cargo_image | 104 |
| 48B620 | restore_actor_update_cartpusher | 611 |
| 48BFF0 | restore_actor_update_ghost | 63 |
| 48C200 | restore_actor_update_explosion | 54 |
| 48C6A0 | restore_actor_update_engineer | 259 |
| 48CAA0 | restore_actor_apply_warehouse_delivery | 148 |
| 48CE30 | restore_actor_update_warehouseman | 1053 |
| 48DED0 | restore_actor_update_fireman | 463 |
| 48E5E0 | restore_fireman_try_assign_fire | 112 |
| 491420 | restore_actor_update_trade_caravan | 425 |
| 4A34F0 | restore_actor_find_linked_kind85 | 25 |
| 4B1DF0 | restore_actor_prepare_service_patrol | 73 |
| 4B1EF0 | restore_actor_advance_service_patrol | 203 |
| 4B21D0 | restore_actor_choose_nearby_road_direction | 88 |
| 4B23C0 | restore_actor_follow_leader_closely | 255 |
| 4B3FF0 | restore_actor_advance_route_pixels | 118 |
| 4B4180 | restore_actor_update_route_error | 35 |
| 4B43A0 | restore_actor_step_route_pixel_y | 16 |
| 4B43E0 | restore_actor_step_route_pixel_x | 16 |
| 4BC140 | restore_actor_approach_destination_result | 71 |
| 4BC1E0 | restore_actor_approach_fallback_chain_result | 99 |
| 4BC2E0 | restore_actor_find_approach_building_state2 | 71 |
| 4BC3B0 | restore_actor_find_approach_building_state3 | 71 |
| 4BC480 | restore_actor_approach_common_result | 9 |
| 4E05F0 | restore_actor_accept_kind222_work | 429 |
| 4E0C00 | restore_actor_find_approach_kind222 | 31 |
| 4E6B20 | restore_monument_base_worker_result | 59 |
| 4E6E10 | restore_actor_find_approach_kind228 | 34 |
| 5421D0 | restore_actor_accept_kind213_work | 576 |
| 542A80 | restore_actor_find_approach_kind213 | 31 |
| 545BE0 | restore_actor_find_approach_kind229 | 33 |
| 5477E0 | restore_monument_worker_active | 47 |
| 547C10 | restore_actor_member_accept_building_core | 125 |
| 54FB40 | restore_actor_find_approach_kind236 | 35 |
| 556430 | restore_actor_find_approach_kind235 | 33 |
| 5590C0 | restore_actor_find_approach_kind234 | 33 |
| 559110 | restore_actor_member_accept_building | 8 |

The total is 7,228 static native instructions, not execution coverage.
Bodies remain in their existing source owners; canonical facades delegate.

The block passes shared strict Win32 C11 object compilation and both main
Release links. Native/C behavioral comparison for this new cohort remains
pending. The standalone CMake project only compiles object files; it does
not launch the host game. Detailed current-main comparison belongs to the
next connected verification milestone.

The callbacks, fire queries and route advancement have full DWORD contracts; the
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

The acceptance predicates4E05F0/5421D0 keep their original source owners
(unit01906/unit02554) and receive zero new algorithm credit. Named C APIs
return BYTE; actual typed adapters zero-extend that result for the existing
logical loops. Their canonical full-result entrypoints retain the original
RAW models. A probe with commit0 can still clear stale links.

The explicit-receiver547C10 C owner captures kind85 before its virtual
child. Native SETZ overwrites a byte of the saved argument area; the later
loads refer to that captured flag and to the separate mode byte. This
stack phase is represented explicitly in the logical C owner. Physical
caller-stack/register/flags behavior remains available through RAW.

The virtual worker queries preserve incoming EAX high24 and define AL.
547C10 loads EAX from the receiver vtable before its virtual call. Its full
C result therefore forwards that cached value through the query adapter;
a zeroed generic frame loses observable high bits. One before/after offline
return-value witness now agrees with both main images. Other query targets
keep RAW with explicit incoming EAX; no complete runtime domain is claimed.
