#include "menu_list_scroll.h"
#define BAR_U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define BAR_B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
uint32_t FUN_0055bb90(void)
{
 uint32_t count=BAR_U(0xf53e6cu),extra,x,y,origin_x,origin_y,position,rate,value,index;
 if((int32_t)count<=15)return 0;
 if(!BAR_B(0xe38e00u))return 0;
 x=BAR_U(0xe38e34u);extra=count-15u;origin_x=BAR_U(0xf53f0cu);
 if((int32_t)x<(int32_t)(origin_x+0x250u))return 0;
 if((int32_t)x>(int32_t)(origin_x+0x26cu))return 0;
 origin_y=BAR_U(0xf53f08u);y=BAR_U(0xe38e30u);
 if((int32_t)y<(int32_t)(origin_y+78u))return 0;
 if((int32_t)y>(int32_t)(origin_y+366u))return 0;
 position=y-origin_y-78u;if((int32_t)position>256)position=256;
 rate=(uint32_t)FUN_004cde60((int32_t)position,256);
 value=(uint32_t)FUN_004cde40((int32_t)extra,(int32_t)rate);
 index=BAR_U(0xeaac28u)*0x49d8u;value+=1u;
 BAR_B(0xe38dcau)=2;BAR_U(0xea60a4u+index)=value;
 return 1;
}
