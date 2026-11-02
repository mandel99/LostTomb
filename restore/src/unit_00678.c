#include "menu_list_scroll.h"
#define BAR_U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define BAR_B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
uint32_t FUN_004483f0(void)
{
 uint32_t count=BAR_U(0x785d70u),extra,x,y,origin_x,origin_y,position,rate,value;
 if((int32_t)count<=12&&!BAR_U(0x78a620u))return 0;
 if(!BAR_B(0xe38e00u))return 0;
 x=BAR_U(0xe38e34u);extra=count-12u;origin_x=BAR_U(0xe38da8u);
 if((int32_t)x<(int32_t)(origin_x+0x1fcu))return 0;
 if((int32_t)x>(int32_t)(origin_x+0x21cu))return 0;
 origin_y=BAR_U(0xe38da4u);y=BAR_U(0xe38e30u);
 if((int32_t)y<(int32_t)(origin_y+80u))return 0;
 if((int32_t)y>(int32_t)(origin_y+300u))return 0;
 if((int32_t)extra<0)extra=0;
 position=y-origin_y-80u;if((int32_t)position>196)position=196;
 rate=(uint32_t)FUN_004cde60((int32_t)position,196);
 value=(uint32_t)FUN_004cde40((int32_t)extra,(int32_t)rate);
 if((int32_t)value>=0&&(int32_t)value<(int32_t)BAR_U(0x785d70u))BAR_U(0x78a620u)=value;
 BAR_B(0xe38dcau)=2;return 1;
}
