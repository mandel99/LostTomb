#include "actor_city_refresh.h"
#define SENTRIES_U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define SENTRIES_B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define SENTRIES_H(a) ((int32_t)*(volatile int16_t *)(uintptr_t)(uint32_t)(a))

/* Whole4B7510,49 instructions. Reuse the original complete scan with BYTE
   fields, 388-byte actors and DWORD scratch distances. Signed comparisons
   follow each original wrapping x86 subtraction, including extreme inputs. */
void restore_actor_mark_sentries_near_cell(int32_t x,int32_t y)
{
    uint32_t cursor=0xbcd630u,dx,dy,distance;
    int32_t coord_x,coord_y;

    do {
        if(SENTRIES_B(cursor-0x2au)==1u &&
           SENTRIES_B(cursor-0x2eu)==42u && SENTRIES_B(cursor)!=4u){
            coord_x=SENTRIES_H(cursor-0x24u);
            coord_y=SENTRIES_H(cursor-0x22u);
            if(x<coord_x)dx=(uint32_t)coord_x-(uint32_t)x;
            else if(coord_x<x)dx=(uint32_t)x-(uint32_t)coord_x;
            else dx=0u;
            SENTRIES_U(0xd3b4fcu)=dx;
            if(y<coord_y)dy=(uint32_t)coord_y-(uint32_t)y;
            else if(coord_y<y)dy=(uint32_t)y-(uint32_t)coord_y;
            else dy=0u;
            SENTRIES_U(0xd3b4f8u)=dy;
            distance=dy;
            if((int32_t)dy<=(int32_t)dx)distance=dx;
            if((int32_t)distance<=1)SENTRIES_B(cursor-0x2au)=2u;
        }
        cursor+=388u;
    }while((int32_t)cursor<(int32_t)0xc8ad70u);
}

void FUN_004b7510(int32_t x,int32_t y)
{
    restore_actor_mark_sentries_near_cell(x,y);
}
