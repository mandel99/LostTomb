#ifndef RESTORE_OVERLAY_FOOTPRINTS_H
#define RESTORE_OVERLAY_FOOTPRINTS_H
#include "ground_render.h"
/* Native tables contain signed screen-coordinate pairs, not tile indices. */
static __inline uint32_t overlay_pairs(uint32_t footprint,uint32_t *count)
{
    static const uint32_t tables[]={0,0x5d0050u,0x5d0070u,0x5d00b8u,0x5d0138u,0x5d0200u};
    if(footprint>=1u&&footprint<=5u){*count=(footprint+1u)*(footprint+1u);return tables[footprint];}
    *count=1;return 0;
}
static __inline void overlay_draw_pairs(uint32_t table,uint32_t count,uint32_t image,
                                        uint32_t x,uint32_t y,int transparent,int reload_image)
{
    uint32_t i,px,py;
    for(i=0;i<count;++i){
        py=y+(table?GR_U(table+i*8u+4u):0u);
        px=x+(table?GR_U(table+i*8u):0u);
        if(reload_image)image=GR_U(0xd3b5c0u);
        if(transparent)(void)FUN_0053f250(GR_S(image),GR_S(px),GR_S(py));
        else (void)FUN_0053e590(GR_S(image),GR_S(px),GR_S(py));
    }
}
#endif
