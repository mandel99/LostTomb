#include "minimap_input.h"
#include "camera_navigation.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
uint32_t FUN_004061e0(void)
{
 uint32_t tile,x,y;
 if(U(0x7056a0u)!=1)return 0;
 if(!B(0xe38e03u)&&!B(0xe38df7u))return 0;
 y=U(0x605a3cu)?30u:59u;x=U(0xf53f64u)+8u;
 tile=FUN_0043ef10((int32_t)x,(int32_t)y,73,111);
 if((int32_t)tile<=0)return 0;
 FUN_00430cd0(tile);return 1;
}
