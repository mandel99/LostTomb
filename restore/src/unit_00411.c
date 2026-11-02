#include "dropdown_menu.h"
int32_t FUN_00426030(RestoreDropdownBar *bars)
{
    uint32_t selected,mode;
    if(!DM_B(0xe38e03u)||DM_I(0x7056a8u)<=0)return 0;
    selected=DM_U(0x705730u);DM_U(0x70572cu)=0;mode=DM_B(0xe38ddau);DM_B(0xe38ddau)=3;
    DM_U(0x705728u)=mode;DM_U(0x705724u)=(uint32_t)(uintptr_t)bars+(selected<<4)-16u;
    DM_U(0xf53f74u)=1;DM_B(0xe38dcau)=1;return 1;
}
