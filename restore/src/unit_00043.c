#include "menu_tool_return.h"
#include "tool_selector.h"
#define RETURN_U(a) (*(volatile uint32_t *)(uintptr_t)(a))
#define RETURN_B(a) (*(volatile uint8_t *)(uintptr_t)(a))
uint32_t FUN_00405ba0(void)
{
    uint32_t changed=RETURN_B(0xe38ddau)!=1u,action;
    if(RETURN_U(0x6059e8u)!=0u)changed=1;
    action=RETURN_U(0xbca310u);
    RETURN_B(0xe38ddau)=1;
    RETURN_U(0x6059e8u)=0;RETURN_U(0x6059e4u)=0;
    RETURN_U(0x6059dcu)=0;RETURN_U(0x6059e0u)=0;
    RETURN_U(0x6059d4u)=0;RETURN_U(0x6059d8u)=0;
    if(action==1u)FUN_00486060((int32_t)RETURN_U(0xbc9800u));
    if(RETURN_U(0xbca310u)==2u)RETURN_U(0xbca310u)=0;
    RETURN_B(0xe38dcau)=1;
    return changed;
}
