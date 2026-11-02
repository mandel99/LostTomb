#include "dropdown_menu.h"
int32_t FUN_00425f20(RestoreDropdownBar *bars,int32_t count)
{
    uintptr_t bar;int32_t hit;
    uint8_t clicked=DM_B(0xe38e03u);
    DM_U(0x70571cu)=0;
    if(clicked){
        uint32_t row=DM_U(0x70572cu);
        if(!row){FUN_00426090();return 1;}
        DM_B(0xe38de1u)=1;DM_B(0xe38de0u)=1;DM_B(0xe38ddau)=DM_B(0x705728u);
        bar=DM_U(0x705724u);
        {uint32_t item=DM_U(bar+8u)+(row<<4)-16u;
         DM_U(0x705720u)=item;DM_U(0xd3b198u)=DM_U(item+12u);restore_dropdown_callback(DM_U(item+4u));}
        if(DM_U(0x70571cu)!=1){FUN_004260d0();return 1;}
        DM_B(0xe38dcau)=1;DM_U(0xf53f74u)=1;DM_B(0xe38ddau)=3;return 1;
    }
    hit=FUN_00426100(bars,count);
    if((uint32_t)hit!=DM_U(0x705730u)&&hit){
        DM_U(0x705730u)=(uint32_t)hit;DM_B(0xe38de1u)=1;DM_B(0xe38de0u)=1;
        bar=(uint32_t)(uintptr_t)bars+((uint32_t)hit<<4)-16u;
        DM_U(0x70572cu)=0;DM_U(0x705724u)=(uint32_t)bar;DM_U(0xf53f74u)=1;DM_B(0xe38dcau)=1;
    }else bar=DM_U(0x705724u);
    DM_U(0x70572cu)=(uint32_t)FUN_00426150((const RestoreDropdownItem *)(uintptr_t)DM_U(bar+8u),DM_S(bar+12u),DM_S(bar),DM_S(bar+4u));
    return 0;
}
