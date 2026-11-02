#include "dropdown_tiler.h"
#include "sprite_render.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(a))
void FUN_00536990(int32_t x,int32_t y,int32_t width,int32_t height)
{
    uint32_t row_phase=0,row_step=0,py=(uint32_t)y;
    int32_t rows=height;
    if(rows<=0)return;
    do {
        uint32_t column_phase=0,px=(uint32_t)x;
        int32_t columns=width;
        if(columns>0){
            row_step=12;
            do {
                uint32_t image=U(0xf5683cu)+column_phase+row_phase+13u;
                U(0xd3b5e0u)=px;U(0xd3b5dcu)=py;U(0xd3b5c0u)=image;
                ++column_phase;
                FUN_0053e590((int32_t)image,(int32_t)px,(int32_t)py);
                if(column_phase>=10)column_phase=0;
                px+=16u;
                --columns;
            }while(columns);
        }
        row_phase+=row_step;if(row_phase>=120)row_phase=0;
        py+=16u;--rows;
    }while(rows);
}
