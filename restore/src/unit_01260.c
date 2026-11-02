#include "map_cells_phase.h"
/* Whole 00487260. All five native callers consume Boolean AL only. */
uint8_t FUN_00487260(int32_t tile)
{
    uint32_t a=mc_object((uint32_t)tile);
    if(mc_bank(a-912u)&&mc_bank(a+912u))return 1;
    return (uint8_t)(mc_bank(a-4u)&&mc_bank(a+4u));
}
