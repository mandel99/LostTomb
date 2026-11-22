#include "pharaoh/map/moisture.h"

/* Native00486D30, clockwise rings. The logical upper bounds are inclusive,
 * as in EXE; callers retain the original physically bordered map storage. */
int32_t pharaoh_map_nearest_terrain_ring(PharaohMapSurfaceContext *c,
    int32_t x,int32_t y,uint32_t mask,int32_t radius_limit)
{
    int32_t radius=1,steps=8,top=(int32_t)((uint32_t)y-1u),result=-1;
    if(radius_limit<=1)return -1;
    int32_t row=(int32_t)((uint32_t)c->map->origin+(uint32_t)y*228u-228u);
    const int32_t dx=(int32_t)((uint32_t)x-(uint32_t)y);
    do{
        if(result>=0)return result;
        int32_t cx=(int32_t)((uint32_t)dx+(uint32_t)top),cy=top,offset=row;
        const int32_t side=steps/4;
        for(int32_t i=0;i<steps;i=(int32_t)((uint32_t)i+1u)){
            switch(i/side){
            case 0:cx=(int32_t)((uint32_t)cx+1u);break;
            case 1:offset=(int32_t)((uint32_t)offset+228u);cy=(int32_t)((uint32_t)cy+1u);break;
            case 2:cx=(int32_t)((uint32_t)cx-1u);break;
            case 3:offset=(int32_t)((uint32_t)offset-228u);cy=(int32_t)((uint32_t)cy-1u);break;
            }
            if(cx>=0 && cx<=c->map->width && cy>=0 && cy<=c->map->height){
                const uint32_t flags=c->map->terrain[(uint32_t)offset+(uint32_t)cx];
                if((flags&mask) && !(flags&0x80000u)){result=radius;break;}
            }
        }
        row=(int32_t)((uint32_t)row-228u);top=(int32_t)((uint32_t)top-1u);
        steps=(int32_t)((uint32_t)steps+8u);radius=(int32_t)((uint32_t)radius+1u);
    }while(radius<radius_limit);
    return result;
}
