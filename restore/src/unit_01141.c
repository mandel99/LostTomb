#include "map_neighbors_phase.h"
/* Whole 0047dee0; byte-grid reads and transient 1->0 stores are observable. */
void FUN_0047dee0(void)
{
    uint32_t tile=MN_U(0xbc9768),i;
    for(i=0;i<8;++i){
        uint32_t flags=MN_B(mn_terrain(tile,i));
        if(flags&2u){uint32_t secondary=MN_B(mn_secondary(tile,i));MN_W(mn_slot(i),1);if(!(secondary&128u))MN_W(mn_slot(i),0);}
        else MN_W(mn_slot(i),0);
    }
}
