#include "monument_entrance.h"
#include <stdlib.h>
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define H(a) ((uint32_t)(int32_t)(int16_t)W(a))
static uint32_t yes(uint32_t eax){return(eax&0xffffff00u)|1u;}

uint32_t restore_monument_entrance_0040eb90(uint32_t self,uint32_t building,uint32_t out_x,uint32_t out_y)
{
    uint32_t r=building*264u,half;
    if((int32_t)U(0x799a30u+r)<3){
        half=(uint32_t)((int32_t)U(self+0x54u+H(0x7999eau+r)*52u)/2);
        U(out_x)=H(0x7999deu+r)+half;
        /* The first store can alias the record's Y coordinate. */
        U(out_y)=H(0x7999e0u+r)+half;return yes(half);
    }
    r=(building-H(0x7999eau+r))*264u;
    U(out_x)=H(0x7999deu+r)+3u;
    U(out_y)=H(0x7999e0u+r)+11u;return yes(out_y);
}
uint32_t restore_monument_entrance_00417490(uint32_t building,uint32_t out_x,uint32_t out_y)
{
    uint32_t root=building-H(0x7999eau+building*264u),r=root*264u;
    uint32_t quarter=(uint32_t)((int32_t)U(0xbca2e0u)/2),x_delta,y_delta,y;
    switch(quarter){
    case 0:x_delta=8;y_delta=15;break;
    case 1:x_delta=0xffffffffu;y_delta=8;break;
    case 2:x_delta=6;y_delta=0xffffffffu;break;
    case 3:x_delta=15;y_delta=6;break;
    default:return yes(quarter); /* No output dereference on this arm. */
    }
    U(out_x)=H(0x7999deu+r)+x_delta;
    y=H(0x7999e0u+r)+y_delta;U(out_y)=y;
    return yes((quarter&1u)?out_y:y);
}
uint32_t restore_monument_entrance_004e6ae0(uint32_t building,uint32_t out_x,uint32_t out_y)
{
    uint32_t r=building*264u,y;
    U(out_x)=H(0x7999deu+r)+2u;
    y=H(0x7999e0u+r)+6u;U(out_y)=y;return yes(y);
}
uint32_t restore_monument_entrance_query(uint32_t target,uint32_t self,uint32_t building,uint32_t out_x,uint32_t out_y)
{
    switch(target){
    case 0x40eb90u:return restore_monument_entrance_0040eb90(self,building,out_x,out_y);
    case 0x417490u:return restore_monument_entrance_00417490(building,out_x,out_y);
    case 0x4e6ae0u:return restore_monument_entrance_004e6ae0(building,out_x,out_y);
    default:abort();
    }
}
