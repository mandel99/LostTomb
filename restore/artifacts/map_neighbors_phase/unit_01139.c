#include "map_neighbors_phase.h"
/* Whole 0047dc80; any included bit and no excluded bit. */
void FUN_0047dc80(uint32_t include,uint32_t exclude)
{
    uint32_t tile=MN_U(0xbc9768),flags,last,i;
    for(i=0;i<6;++i){flags=MN_U(mn_terrain(tile,i));MN_W(mn_slot(i),(flags&include)!=0&&(flags&exclude)==0);}
    flags=MN_U(mn_terrain(tile,6));last=MN_U(mn_terrain(tile,7));
    MN_W(mn_slot(6),(flags&include)!=0&&(flags&exclude)==0);
    MN_W(mn_slot(7),(last&include)!=0&&(last&exclude)==0);
}
