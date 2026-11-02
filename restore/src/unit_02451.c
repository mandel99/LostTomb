#include "briefing_draw.h"
#include "order_actions_finish.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
/* 536FD0 indexes DWORD animation state with a four-byte stride. */
int32_t FUN_00536fd0(int32_t style)
{
    U(0xf53f28u)=100;
    if(U(0xf53f10u+(uint32_t)style*4u)==7u)(void)FUN_0047d350();
    return 1;
}
