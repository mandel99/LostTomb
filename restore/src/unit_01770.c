#include "campaign_map_input.h"
#define HIT_U(a) (*(volatile uint32_t *)(uintptr_t)(a))
__declspec(noinline) int32_t FUN_004cf620(uint32_t x,uint32_t y,uint32_t width,uint32_t height)
{
 uint32_t mouse=HIT_U(0xe38e34u);
 if((int32_t)x>(int32_t)mouse)return 0;
 if((int32_t)(x+width)<=(int32_t)mouse)return 0;
 mouse=HIT_U(0xe38e30u);
 if((int32_t)y>(int32_t)mouse)return 0;
 if((int32_t)(y+height)<=(int32_t)mouse)return 0;
 return 1;
}
