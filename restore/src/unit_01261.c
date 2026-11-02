#include "map_cells_phase.h"
/* Whole 004872c0. Preserve mask high bits left by native MOV/XOR AL. */
uint32_t FUN_004872c0(int32_t tile,uint32_t mask)
{
    uint32_t a=mc_terrain((uint32_t)tile),hit;
    hit=((MC_U(a-912u)&mask)!=0 && (MC_U(a+912u)&mask)!=0);
    if(!hit)hit=((MC_U(a-4u)&mask)!=0 && (MC_U(a+4u)&mask)!=0);
    return (mask&0xffffff00u)|hit;
}
