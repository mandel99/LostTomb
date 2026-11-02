#include "menu_cursor_queries.h"
#define HIT_I(a) (*(volatile int32_t *)(uintptr_t)(a))
void FUN_00406250(void)
{
 int32_t y=HIT_I(0xe38e30u),x,left;uint32_t index;
 HIT_I(0x70569cu)=0;
 if(y<4||y>=18)return;
 x=HIT_I(0xe38e34u);
 for(index=0;index<3;++index){
  left=HIT_I(0xf53dc4u-index*4u);
  if(x>left&&x<(int32_t)((uint32_t)left+128u))HIT_I(0x70569cu)=(int32_t)(index+1u);
 }
}
