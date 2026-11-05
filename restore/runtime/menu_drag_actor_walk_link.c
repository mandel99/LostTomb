#include "actor_walking.h"
#include "game_diagnostics.h"
#include <intrin.h>

/* Refactor of the already ordinary provider: the algorithm exists only here.
   caller_site preserves the existing diagnostic event's original caller. */
static uint32_t actor_link_grid_at(int32_t actor, uint32_t caller_site)
{
    uint32_t offset = (uint32_t)actor * 388u;
    int32_t tile = AW_I(0xbcd618u + offset), tail, next;
    uint32_t result = (uint32_t)tile;

    restore_game_actor_grid_event(caller_site, actor, 1);
    if (tile >= 0) {
        tail = AW_S(0xb350dcu + (uint32_t)tile * 2u);
        result = (uint32_t)tail;
        AW_B(0xbcd682u + offset) = 0;
        if (tail == 0) {
            tile = AW_I(0xbcd618u + offset);
            AW_W(0xb350dcu + (uint32_t)tile * 2u) = (uint16_t)actor;
            FUN_0051f780();
            return (uint32_t)tile;
        }
        AW_B(0xbcd682u + offset) = 1;
        next = AW_S(0xbcd600u + (uint32_t)tail * 388u);
        while (next != 0) {
            tail = next;
            result = (uint32_t)tail;
            if (tail == actor)
                goto complete;
            AW_B(0xbcd682u + offset) = (uint8_t)(AW_B(0xbcd682u + offset) + 1u);
            next = AW_S(0xbcd600u + (uint32_t)tail * 388u);
        }
        if (AW_B(0xbcd682u + offset) > 20)
            AW_B(0xbcd682u + offset) = 20;
        AW_W(0xbcd600u + (uint32_t)tail * 388u) = (uint16_t)actor;
    }
complete:
    FUN_0051f780();
    return result;
}

__declspec(noinline) uint32_t restore_actor_link_to_grid(int32_t actor)
{
    return actor_link_grid_at(actor, (uint32_t)(uintptr_t)_ReturnAddress());
}

/* Existing callers retain their exact void API; no physical void result read. */
__declspec(noinline) void FUN_004b52f0(int32_t actor)
{
    (void)actor_link_grid_at(actor, (uint32_t)(uintptr_t)_ReturnAddress());
}
