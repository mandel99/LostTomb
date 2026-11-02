#include "world_grid_values.h"
#include "actor_render.h"
extern int32_t FUN_004b54f0(int32_t tile);
extern uint32_t FUN_0047e230(uint32_t channel,int32_t tile);
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define H(a) ((int32_t)*(volatile int16_t *)(uintptr_t)(uint32_t)(a))
#define C(a) ((int32_t)*(volatile int8_t *)(uintptr_t)(uint32_t)(a))
#define I(v) ((int32_t)(uint32_t)(v))

/* Whole43E420: full selector/font, byte-addressed grids and signed values.
   Negative values publish their modular magnitude, including INT_MIN. */
void FUN_0043e420(int32_t x,int32_t y)
{
    uint32_t tile=U(0x70e488u),mode=U(0xe38e94u),value,offset,color=0xffffu;
    int32_t building=H(0xb4e6fcu+tile*2u);
    offset=(uint32_t)building*264u;value=(uint32_t)building;
    switch(mode){
    case 0:break;
    case 1:value=U(0xaf598cu+tile*4u);break;
    case 2:value=B(0xb285ccu+tile);break;
    case 3:value=U(0xb7482cu+tile*4u);break;
    case 4:value=B(0xb67d1cu+tile);break;
    case 5:value=B(0xae8e7cu+tile);break;
    case 6:value=B(0xacf85cu+tile);break;
    case 7:value=(uint32_t)H(0xb350dcu+tile*2u);break;
    case 8:value=B(0xadc36cu+tile);break;
    case 9:value=(uint32_t)C(0xee0c50u+tile);break;
    case 10:value=U(0xeed760u+tile*4u);break;
    case 11:value=B(0x799a0du+offset);break;
    case 12:value=(uint32_t)H(0x7999f0u+offset);break;
    case 13:value=(uint32_t)H(0x7999f8u+offset);break;
    case 14:value=(uint32_t)H(0x799a1eu+offset);break;
    case 15:value=(uint32_t)H(0x7999f4u+offset);break;
    case 16:value=(uint32_t)C(0xac2d4cu+tile);break;
    case 17:value=B(0xab623cu+tile);break;
    case 18:value=(uint32_t)C(0xed4140u+tile);break;
    case 19:value=(uint32_t)C(0xec7630u+tile);break;
    case 20:value=(uint32_t)H(0xa9cc1cu+tile*2u);break;
    case 21:value=(uint32_t)FUN_004b54f0(I(tile));break;
    case 22:value=B(0xa9010cu+tile);break;
    case 23:value=B(0xa835fcu+tile);break;
    case 24:value=(uint32_t)C(0xebab20u+tile);break;
    case 25:value=B(0xa76aecu+tile);break;
    case 26:value=B(0xa69fdcu+tile);break;
    case 27:value=FUN_0047e230(1,I(tile));break;
    default:return;
    }
    if(!value)return;
    if(I(value)<0){value=0u-value;color=0xf800u;}
    FUN_004cce00(value,0x20,(const char *)(uintptr_t)0x57c048u,
                 (uint32_t)x+24u,(uint32_t)y+10u,U(0xf56f84u),color);
}
