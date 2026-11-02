#include "map_neighbors_phase.h"
/* Whole 0047ddd0; retain full existing slot DWORDs and x86 shift masking. */
void FUN_0047ddd0(uint32_t mask,uint32_t shift)
{
    uint32_t tile=MN_U(0xbc9768),flags=MN_U(mn_terrain(tile,0)),old=MN_U(mn_slot(0)),next,last,i;
    shift&=31u;
    for(i=0;i<5;++i){
        uint32_t value=old|((uint32_t)((flags&mask)!=0)<<shift);
        next=MN_U(mn_terrain(tile,i+1));MN_W(mn_slot(i),value);
        flags=next;old=MN_U(mn_slot(i+1));
    }
    next=MN_U(mn_terrain(tile,6));last=MN_U(mn_terrain(tile,7));
    MN_W(mn_slot(5),old|((uint32_t)((flags&mask)!=0)<<shift));
    old=MN_U(mn_slot(6));MN_W(mn_slot(6),old|((uint32_t)((next&mask)!=0)<<shift));
    old=MN_U(mn_slot(7));MN_W(mn_slot(7),old|((uint32_t)((last&mask)!=0)<<shift));
}
