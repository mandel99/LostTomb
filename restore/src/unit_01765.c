#include "pixel_access.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define I(v) ((int32_t)(uint32_t)(v))
/* Whole4CF090. RGB565 compares the entire format DWORD. Read it once for
   channel extraction, then again after the clear for channel composition. */
void FUN_004cf090(int32_t x,int32_t y,int32_t width,int32_t height,uint32_t shift)
{
    uint32_t row=(uint32_t)y,end_y=row+(uint32_t)height,column,end_x,grey,blue;
    int32_t red,green;
    if(I(row)>=I(end_y))return;
    end_x=(uint32_t)x+(uint32_t)width;
    do{
        for(column=(uint32_t)x;I(column)<I(end_x);++column){
            if(U(0xe3914cu)==0x565u){
                red=I(FUN_0040faa0(I(column),row,0xf800u))>>11;
                green=I(FUN_0040faa0(I(column),row,0x7e0u))>>6;
            }else{
                red=I(FUN_0040faa0(I(column),row,0x7c00u))>>10;
                green=I(FUN_0040faa0(I(column),row,0x3e0u))>>5;
            }
            blue=FUN_0040faa0(I(column),row,0x1fu);
            grey=(uint32_t)(I((uint32_t)red+(uint32_t)green+blue)/3);
            grey=(uint32_t)(I(grey)>>(shift&31u));
            FUN_0040fa70(I(column),I(row),0);
            if(U(0xe3914cu)==0x565u){
                FUN_0040fae0(I(column),I(row),(uint16_t)(grey<<11));
                FUN_0040fae0(I(column),I(row),(uint16_t)(grey<<6));
            }else{
                FUN_0040fae0(I(column),I(row),(uint16_t)(grey<<10));
                FUN_0040fae0(I(column),I(row),(uint16_t)(grey<<5));
            }
            FUN_0040fae0(I(column),I(row),(uint16_t)grey);
        }
        ++row;
    }while(I(row)<I(end_y));
}
