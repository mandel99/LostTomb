#include "route_helper_internal.h"

/* Repair the original complete 4690F0 owner: heat is a WORD, while the
   map base is a DWORD. Native arithmetic wraps before signed division. */
void restore_actor_increment_tile_heat(int32_t tile, int32_t limit)
{
    uint32_t address = 0xa9cc1cu + (uint32_t)tile * 2u;
    int32_t relative;
    RH_W(address) = (uint16_t)(RH_W(address) + 1u);
    if ((int32_t)(int16_t)RH_W(address) > limit) {
        relative = RH_I((uint32_t)tile - RH_U(0xbca2d8u));
        (void)restore_actor_tile_heat_overflow_raw(relative % 228,
                                                 relative / 228, tile);
    }
}
