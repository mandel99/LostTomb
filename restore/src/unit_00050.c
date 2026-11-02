#include "menu_cursor_queries.h"
#define HIT_I(a) (*(volatile int32_t *)(uintptr_t)(a))
void FUN_00406190(void)
{
 int32_t x=HIT_I(0xe38e34u),left=HIT_I(0x70e3d4u),y,top;
 HIT_I(0x7056a0u)=0;
 if(x<left||x>=HIT_I(0x70e3ccu))return;
 y=HIT_I(0xe38e30u);top=HIT_I(0x70e3d0u);
 if(y<top||y>=HIT_I(0x70e3c8u))return;
 HIT_I(0x7056a0u)=1;
}
