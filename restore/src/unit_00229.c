#include "message_panel_input.h"
#include "campaign_menu.h"
#define BRIEF_U(a) (*(volatile uint32_t *)(uintptr_t)(a))
void FUN_00418fe0(void)
{
 uint32_t y=BRIEF_U(0x6fdc9cu),x=BRIEF_U(0x6fdca0u);
 FUN_00425bc0((int32_t)(x+0x230u),(int32_t)(y+0xa8u),(const void *)0x5c7df8u,1,12);
 y=BRIEF_U(0x6fdc9cu);x=BRIEF_U(0x6fdca0u);
 if(FUN_00425990((int32_t)(x+0x230u),(int32_t)(y+0xa8u),(const void *)0x5c7df8u,1)!=0)return;
 y=BRIEF_U(0x6fdc9cu);x=BRIEF_U(0x6fdca0u);
 FUN_00425bc0((int32_t)(x+0x230u),(int32_t)(y+0x17eu),(const void *)0x5c7e20u,1,13);
 y=BRIEF_U(0x6fdc9cu);x=BRIEF_U(0x6fdca0u);
 if(FUN_00425990((int32_t)(x+0x230u),(int32_t)(y+0x17eu),(const void *)0x5c7e20u,1)==0)(void)FUN_004c7db0();
}
