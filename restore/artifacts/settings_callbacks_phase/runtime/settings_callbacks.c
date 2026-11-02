#include "settings_callbacks.h"
void FUN_00402c10(void)
{
    uint32_t window=SSET_U(0xe39140);
    (void)restore_menu_action_platform()->post((HWND)(uintptr_t)window,0x100,0x75,0);
    SSET_PUT8(0xe38dda,1);SSET_PUT8(0xe38dca,1);
}
void FUN_00402c40(void)
{
    uint32_t selection=SSET_U(0xd3b198),window;
    if(selection==1){window=SSET_U(0xe39140);(void)restore_menu_action_platform()->post((HWND)(uintptr_t)window,0x100,0x76,0);}
    else if(selection==2){window=SSET_U(0xe39140);(void)restore_menu_action_platform()->post((HWND)(uintptr_t)window,0x100,0x77,0);}
    else if(selection==3){window=SSET_U(0xe39140);(void)restore_menu_action_platform()->post((HWND)(uintptr_t)window,0x100,0x78,0);}
    SSET_PUT8(0xe38dda,1);
}
void FUN_00402cc0(void)
{
    uint32_t index=SSET_U(0xd3b198)-1u,value;
    if(index>12u)return;
    if(index<12u){SSET_PUT32(0x6de510,SSET_U(0x6de510)^(2u<<index));return;}
    value=SSET_U(0x6de510);SSET_PUT8(0xe38dda,1);SSET_PUT8(0xe38dca,1);SSET_PUT32(0xe38ef0,value);
}
void FUN_00402f70(void)
{
    uint32_t first=SSET_U(0x6057bc),second;uint8_t flag1=SSET_B(0x6057b8),flag0=SSET_B(0x6057c0),other;
    SSET_PUT32(0xe38ec8,first);second=SSET_U(0x6057b4);SSET_PUT8(0xe38e65,flag1);other=SSET_B(0x6057b0);
    SSET_PUT32(0xe38ecc,second);second=SSET_U(0x6057ac);SSET_PUT8(0xe38e63,other);other=SSET_B(0x6057a8);
    SSET_PUT32(0xe38ec4,second);second=SSET_U(0x6057a4);SSET_PUT8(0xe38e64,flag0);SSET_PUT8(0xe38ec0,other);SSET_PUT32(0xe38ed0,second);
    if(flag0==0)FUN_0055f561(RESTORE_SOUND,0);
    FUN_0055f639(RESTORE_SOUND,0,SSET_I(0xe38ec8));FUN_0055f38d(RESTORE_SOUND,SSET_I(0xe38ec8));
    FUN_0055f639(RESTORE_SOUND,2,SSET_I(0xe38ed0));restore_settings_sound_55f3eb(RESTORE_SOUND,SSET_I(0xe38ed0));
    FUN_0055f639(RESTORE_SOUND,1,SSET_I(0xe38ecc));restore_settings_sound_55f32f(RESTORE_SOUND,SSET_I(0xe38ecc));
    SSET_PUT8(0xe38dda,1);SSET_PUT8(0xe38dca,1);
}
void FUN_00403060(void)
{
    uint32_t selection=SSET_U(0xd3b198);
    if(selection==1){
        if(SSET_B(0xe38e64)==0){SSET_PUT8(0xe38e64,1);SSET_PUT8(0xe38dca,1);return;}
        SSET_PUT8(0xe38e64,0);FUN_0055f561(RESTORE_SOUND,0);selection=SSET_U(0xd3b198);
    }
    if(selection==2){
        if(SSET_B(0xe38e65)==0){SSET_PUT8(0xe38e65,1);SSET_PUT8(0xe38dca,1);return;}
        SSET_PUT8(0xe38e65,0);FUN_0055f561(RESTORE_SOUND,1);selection=SSET_U(0xd3b198);
    }
    if(selection==3){uint8_t previous=SSET_B(0xe38e63);SSET_PUT8(0xe38dca,1);SSET_PUT8(0xe38e63,previous==0);return;}
    if(selection==4){
        if(SSET_B(0xe38ec0)==0){SSET_PUT8(0xe38ec0,1);SSET_PUT8(0xe38dca,1);return;}
        SSET_PUT8(0xe38ec0,0);FUN_0055f561(RESTORE_SOUND,2);
    }
    SSET_PUT8(0xe38dca,1);
}
void FUN_00403130(void)
{
    uint32_t selected=SSET_U(0xd3b198),value;
    if(selected==1)SSET_PUT32(0xe38ec8,SSET_U(0xe38ec8)-1u);else if(selected==0)SSET_PUT32(0xe38ec8,SSET_U(0xe38ec8)+1u);
    value=SSET_U(0xe38ec8);if((int32_t)value<0){value=0;SSET_PUT32(0xe38ec8,value);}else if((int32_t)value>100){value=100;SSET_PUT32(0xe38ec8,value);}
    FUN_0055f639(RESTORE_SOUND,0,(int32_t)value);FUN_0055f38d(RESTORE_SOUND,SSET_I(0xe38ec8));SSET_PUT8(0xe38dca,1);
}
void FUN_004031a0(void)
{
    uint32_t selected=SSET_U(0xd3b198),value;
    if(selected==1)SSET_PUT32(0xe38ecc,SSET_U(0xe38ecc)-1u);else if(selected==0)SSET_PUT32(0xe38ecc,SSET_U(0xe38ecc)+1u);
    value=SSET_U(0xe38ecc);if((int32_t)value<0){value=0;SSET_PUT32(0xe38ecc,value);}else if((int32_t)value>100){value=100;SSET_PUT32(0xe38ecc,value);}
    FUN_0055f639(RESTORE_SOUND,1,(int32_t)value);restore_settings_sound_55f32f(RESTORE_SOUND,SSET_I(0xe38ecc));SSET_PUT8(0xe38dca,1);
}
void FUN_00403210(void)
{
    uint32_t selected=SSET_U(0xd3b198),value;
    if(selected==1)SSET_PUT32(0xe38ec4,SSET_U(0xe38ec4)-1u);else if(selected==0)SSET_PUT32(0xe38ec4,SSET_U(0xe38ec4)+1u);
    value=SSET_U(0xe38ec4);if((int32_t)value<0){SSET_PUT32(0xe38ec4,0);SSET_PUT8(0xe38dca,1);return;}
    if((int32_t)value>100)SSET_PUT32(0xe38ec4,100);SSET_PUT8(0xe38dca,1);
}
void FUN_00403270(void)
{
    uint32_t selected=SSET_U(0xd3b198),value;
    if(selected==1)SSET_PUT32(0xe38ed0,SSET_U(0xe38ed0)-1u);else if(selected==0)SSET_PUT32(0xe38ed0,SSET_U(0xe38ed0)+1u);
    value=SSET_U(0xe38ed0);if((int32_t)value<0){value=0;SSET_PUT32(0xe38ed0,value);}else if((int32_t)value>100){value=100;SSET_PUT32(0xe38ed0,value);}
    restore_settings_sound_55f3eb(RESTORE_SOUND,(int32_t)value);SSET_PUT8(0xe38dca,1);
}
void FUN_004032f0(void)
{
    uint32_t speed=SSET_U(0x6057a0),scroll=SSET_U(0x60579c);
    SSET_PUT32(0xe38e6c,speed);SSET_PUT32(0xe38e70,scroll);SSET_PUT8(0xe38dda,1);SSET_PUT8(0xe38dca,1);
}
void FUN_00403390(void)
{
    uint32_t selected=SSET_U(0xd3b198),value;
    if(selected==1)SSET_PUT32(0xe38e70,SSET_U(0xe38e70)-10u);else if(selected==0)SSET_PUT32(0xe38e70,SSET_U(0xe38e70)+10u);
    value=SSET_U(0xe38e70);if((int32_t)value<0){SSET_PUT32(0xe38e70,0);SSET_PUT8(0xe38dca,1);return;}
    if((int32_t)value>100)SSET_PUT32(0xe38e70,100);SSET_PUT8(0xe38dca,1);
}
void FUN_004ea760(void){SSET_PUT8(0xe38dda,1);SSET_PUT8(0xe38dca,1);}
