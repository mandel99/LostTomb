#ifndef RESTORE_ROUTE_HELPER_CANDIDATES_H
#define RESTORE_ROUTE_HELPER_CANDIDATES_H
#include <stdint.h>

/* Native 496640 returns the complete input actor in EAX only when its link
   WORD is zero; a nonzero link enters the original fixed assertion loop. */
int32_t restore_transport_assert_no_predecessor(int32_t actor);
void restore_actor_increment_tile_heat(int32_t tile, int32_t limit);
void restore_transport_build_road_distance_map(int32_t x, int32_t y);

/* Root-provided cdecl3 bridge to existing whole-native 469140. The logical
   heat helper discards this physical EAX result: native 4690F0 is void. */
uint32_t restore_actor_tile_heat_overflow_raw(int32_t x, int32_t y, int32_t tile);
#endif
