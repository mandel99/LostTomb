#include "map_neighbors_phase.h"
/* Whole 0047dbc0; preserve the native lookahead read/store order. */
void FUN_0047dbc0(uint32_t mask)
{
    uint32_t tile=MN_U(0xbc9768),flags=MN_U(mn_terrain(tile,0)),next,last,i;
    for(i=0;i<5;++i){next=MN_U(mn_terrain(tile,i+1));MN_W(mn_slot(i),(flags&mask)==mask);flags=next;}
    next=MN_U(mn_terrain(tile,6));last=MN_U(mn_terrain(tile,7));
    MN_W(mn_slot(5),(flags&mask)==mask);
    MN_W(mn_slot(6),(next&mask)==mask);
    MN_W(mn_slot(7),(last&mask)==mask);
}
