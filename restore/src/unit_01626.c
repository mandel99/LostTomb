#include "message_panel_input.h"
static const void *action_button(int32_t action)
{
    uint32_t address;
    switch(action){case 1:address=0x5c7ec0;break;case 2:address=0x5c7f88;break;case 3:address=0x5c7fb0;break;
    case 4:address=0x5c7f38;break;case 5:address=0x5c7ee8;break;case 6:address=0x5c7fd8;break;case 7:address=0x5c8050;break;default:address=0x5c7e48;break;}
    return (const void *)(uintptr_t)address;
}
static int32_t bottom(void){return mi_sub(mi_add(MI_I(0xd3b15c),mi_cells(restore_message_record()->rows)),36);}
static int32_t close_x(void){return mi_sub(mi_add(MI_I(0xd3b160),mi_cells(restore_message_record()->columns)),38);}
static int32_t scroll_x(void){return mi_sub(mi_add(MI_I(0xd3b154),mi_cells(MI_I(0xd3b14c))),1);}
static int32_t scroll_bottom(void){return mi_sub(mi_add(MI_I(0xd3b150),mi_cells(MI_I(0xd3b148))),26);}
void FUN_004c7770(void)
{
    int32_t action=MI_I(0xd3a6e8),link;
    uint8_t choice=action==8&&MI_W(0xd3b186)!=0;
    if(MI_B(0xe38df8)&&!choice){FUN_004c7f30();return;}
    if(MI_I(0xd3b180)) {
        if(choice) {
            MI_B(0xe38dc8)=0;MI_I(0x7056c8)=FUN_00426450(MI_I(0xd3b160),MI_I(0xd3b15c),(const RestoreMenuButton *)0x5e02c0u,3);
            FUN_004261d0(MI_I(0xd3b160),MI_I(0xd3b15c),(RestoreMenuButton *)0x5e02c0u,3);return;
        }
        if(FUN_00425990(mi_add(MI_I(0xd3b160),16),mi_add(MI_I(0xd3b15c),408),action_button(action),1))return;
        if(!FUN_00425990(mi_add(MI_I(0xd3b160),436),mi_add(MI_I(0xd3b15c),410),(void *)0x5c7dd0u,1))MI_B(0xe38dc8)=0;
        return;
    }
    if(restore_message_record()->type==0) {
        FUN_00425bc0(mi_add(MI_I(0xd3b160),16),bottom(),(void *)0x5c7da8u,1,10);
        if(FUN_00425990(mi_add(MI_I(0xd3b160),16),bottom(),(void *)0x5c7da8u,1))return;
        action=MI_I(0xd3a6e8);
    }
    if(restore_message_record()->type==2) {
        if(choice) {
            MI_I(0x7056c8)=FUN_00426450(MI_I(0xd3b160),MI_I(0xd3b15c),(const RestoreMenuButton *)0x5e02c0u,3);
            if(FUN_004261d0(MI_I(0xd3b160),MI_I(0xd3b15c),(RestoreMenuButton *)0x5e02c0u,3))return;
        } else {
            if(FUN_00425990(mi_add(MI_I(0xd3b160),16),bottom(),action_button(action),1))return;
            if(restore_message_record()->subtype==1||restore_message_record()->subtype==7) {
                FUN_00425bc0(mi_add(MI_I(0xd3b160),64),mi_add(MI_I(0xd3b150),36),(void *)0x5c7e70u,1,15);
                if(FUN_00425990(mi_add(MI_I(0xd3b160),64),mi_add(MI_I(0xd3b150),36),(void *)0x5c7e70u,1))return;
            }
        }
    }
    if(!choice) {
        FUN_00425bc0(close_x(),bottom(),(void *)0x5c7dd0u,1,11);
        if(FUN_00425990(close_x(),bottom(),(void *)0x5c7dd0u,1))return;
    }
    FUN_00425bc0(scroll_x(),MI_I(0xd3b150),(void *)0x5c7df8u,1,12);
    if(FUN_00425990(scroll_x(),MI_I(0xd3b150),(void *)0x5c7df8u,1))return;
    FUN_00425bc0(scroll_x(),scroll_bottom(),(void *)0x5c7e20u,1,13);
    if(FUN_00425990(scroll_x(),scroll_bottom(),(void *)0x5c7e20u,1))return;
    if(FUN_004c7db0()||choice)return;
    link=FUN_004c7f70();if(link<0)return;
    if(MI_I(0xd3aae0)<199) {
        uint32_t index=MI_U(0xd3aae0);MI_U(0xd3ae04u+index*4u)=MI_U(0xd3b124);MI_U(0xd3aae4u+index*4u)=MI_U(0xd3b130);MI_U(0xd3aae0)+=1;
    }
    MI_I(0xd3b124)=link;MI_U(0xd3b130)=MI_U(0xd3b138)=MI_U(0xd3b12c)=0;FUN_004c8040();MI_B(0xe38dca)=1;
}
