#include "map_cells_phase.h"
/* Whole 00487390. Offset addition wraps before any cell access. */
uint32_t FUN_00487390(int32_t tile,int32_t offset)
{
    uint32_t t=(uint32_t)tile+(uint32_t)offset,value=mc_growth(t);
    if(mc_bank(mc_object(t)))return 100;
    if(MC_U(mc_terrain(t))&0x40004u)return 100;
    return value<128u?value:0;
}
