#include "world_grid_values.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define I(v) ((int32_t)(uint32_t)(v))

/* Whole43E300: row/column state is reloaded only after a real cell draw.
   The original viewport producer supplies valid projection-table indices. */
void FUN_0043e300(void)
{
    uint32_t row,width,x,y,tile,column,count,rows=0,alternate=0;
    if(!U(0x78d7d8u)&&!U(0x78d7dcu)&&!U(0x78d7d4u))return;
    y=U(0x70e48cu)-U(0x70e44cu);row=U(0x70e474u);U(0x70e47cu)=row;
    if(I(U(0x70e460u))<=0)return;
    width=U(0x70e464u);
    do{
        x=U(0x70e490u);
        if(alternate)x-=U(0x70e45cu)+1u;
        alternate=!alternate;count=0;U(0x70e480u)=U(0x70e478u);
        if(I(width)>=0)do{
            column=U(0x70e480u);
            tile=U(0x70e494u+(row*231u+column)*4u);U(0x70e488u)=tile;
            if(I(tile)<0x0fff0000){
                FUN_0043e420(I(x),I(y));row=U(0x70e47cu);width=U(0x70e464u);
            }
            column=U(0x70e480u);++count;x+=U(0x70e458u)+2u;U(0x70e480u)=column+1u;
        }while(I(count)<=I(width));
        y+=U(0x70e44cu);++rows;++row;U(0x70e47cu)=row;
    }while(I(rows)<I(U(0x70e460u)));
}
