#include "map_menu_scroll.h"
/* Original 00442730: cdecl DWORD direction; 8 returns without clamping. */
void FUN_00442730(int32_t direction)
{
    uint32_t a,b;
    if(direction==8)return;
    switch(direction){
    case 0:a=MSR(0x77590cu);MSW(0x77590cu,a-20u);break;
    case 1:
        a=MSR(0x77590cu);b=MSR(0x775910u);
        MSW(0x77590cu,a-20u);MSW(0x775910u,b+20u);break;
    case 2:b=MSR(0x775910u);MSW(0x775910u,b+20u);break;
    case 3:
        a=MSR(0x77590cu);b=MSR(0x775910u);
        MSW(0x77590cu,a+20u);MSW(0x775910u,b+20u);break;
    case 4:a=MSR(0x77590cu);MSW(0x77590cu,a+20u);break;
    case 5:
        a=MSR(0x77590cu);b=MSR(0x775910u);
        MSW(0x77590cu,a+20u);MSW(0x775910u,b-20u);break;
    case 6:b=MSR(0x775910u);MSW(0x775910u,b-20u);break;
    case 7:
        a=MSR(0x77590cu);b=MSR(0x775910u);
        MSW(0x77590cu,a-20u);MSW(0x775910u,b-20u);break;
    default:break;
    }
    FUN_00442800();
}
