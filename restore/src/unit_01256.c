#include "map_cells_phase.h"
/* Whole 00486d30: clockwise square rings, upper radius bound exclusive. */
int32_t FUN_00486d30(int32_t x,int32_t y,uint32_t mask,int32_t radius_limit)
{
    int32_t radius=1,steps=8,top=mc_sub(y,1),row,dx,result=-1;
    if(radius_limit<=1)return -1;
    row=(int32_t)(MC_U(0xbca2d8)+(uint32_t)y*228u-228u);
    dx=mc_sub(x,y);
    do {
        int32_t i,cx,cy,offset,side;
        if(result>=0)return result;
        cx=mc_add(dx,top);cy=top;offset=row;side=steps/4;
        for(i=0;i<steps;i=mc_add(i,1)) {
            switch(i/side) {
            case 0:cx=mc_add(cx,1);break;
            case 1:offset=mc_add(offset,228);cy=mc_add(cy,1);break;
            case 2:cx=mc_sub(cx,1);break;
            case 3:offset=mc_sub(offset,228);cy=mc_sub(cy,1);break;
            }
            if(cx>=0 && cx<=(int32_t)MC_U(0xbca2d0) && cy>=0 && cy<=(int32_t)MC_U(0xbca2d4)) {
                uint32_t flags=MC_U(mc_terrain((uint32_t)offset+(uint32_t)cx));
                if((flags&mask)!=0 && (flags&0x80000u)==0) {result=radius;break;}
            }
        }
        row=mc_sub(row,228);top=mc_sub(top,1);steps=mc_add(steps,8);radius=mc_add(radius,1);
    }while(radius<radius_limit);
    return result;
}
