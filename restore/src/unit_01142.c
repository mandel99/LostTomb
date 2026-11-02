#include "map_neighbors_phase.h"
/* Whole 0047e000; cardinal slots contain raw 0x8000, not Boolean values. */
uint32_t FUN_0047e000(void)
{
    uint32_t tile=MN_U(0xbc9768),north=MN_U(mn_terrain(tile,0))&0x8000u,east=MN_U(mn_terrain(tile,2))&0x8000u;
    uint32_t south=MN_U(mn_terrain(tile,4))&0x8000u,west=MN_U(mn_terrain(tile,6))&0x8000u;
    MN_W(mn_slot(7),0);MN_W(mn_slot(5),0);MN_W(mn_slot(3),0);MN_W(mn_slot(1),0);
    MN_W(mn_slot(0),north);MN_W(mn_slot(2),east);MN_W(mn_slot(4),south);MN_W(mn_slot(6),west);
    return (north|east|south|west)!=0;
}
