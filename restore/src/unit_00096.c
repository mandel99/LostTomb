#include "menu_list_scroll.h"
#define LIST_U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define LIST_B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
uint32_t FUN_0040d1b0(void)
{
 uint32_t rows=FUN_00502780(),extra,span,origin,x,y,position,percent,value,building;
 if((int32_t)LIST_U(0xf53e6cu)<=(int32_t)rows)return 0;
 if(!LIST_B(0xe38e00u))return 0;
 rows=FUN_00502780();extra=LIST_U(0xf53e6cu)-rows;
 span=((uint32_t)FUN_004cde80((int32_t)(FUN_00502780()<<2),3)-6u)<<4;
 origin=LIST_U(0xeadd90u);x=LIST_U(0xe38e34u);
 if((int32_t)x<(int32_t)(origin+0x19eu))return 0;
 if((int32_t)x>(int32_t)(origin+0x1beu))return 0;
 y=LIST_U(0xe38e30u);
 if((int32_t)y<106)return 0;
 if((int32_t)y>(int32_t)(span+122u))return 0;
 position=y-106u;if((int32_t)position>(int32_t)span)position=span;
 percent=(uint32_t)FUN_004cde60((int32_t)position,(int32_t)span);
 value=(uint32_t)FUN_004cde40((int32_t)extra,(int32_t)percent);
 building=LIST_U(0xeadd6cu);value+=1u;
 LIST_B(0xe38dcau)=2;LIST_U(0x799ab8u+building*264u)=value;
 return 1;
}
