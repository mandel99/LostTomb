#include "message_panel.h"
#define I(a) (*(int32_t *)(uintptr_t)(a))
#define W(a) (*(uint16_t *)(uintptr_t)(a))
#define B(a) (*(uint8_t *)(uintptr_t)(a))
extern void FUN_00425730(int,int,const void *,int);
extern void FUN_004ccbb0(int,int,int,int,int,int);
static const void *action_buttons(void)
{
    switch(I(0xd3a6e8)) {
    case 1:return (const void *)0x5c7ec0u;case 2:return (const void *)0x5c7f88u;
    case 3:return (const void *)0x5c7fb0u;case 4:return (const void *)0x5c7f38u;
    case 5:return (const void *)0x5c7ee8u;case 6:return (const void *)0x5c7fd8u;
    case 7:return (const void *)0x5c8050u;default:return (const void *)0x5c7e48u;
    }
}
void FUN_004c7340(void)
{
    int special;
    B(0xe38dc9)=1;special=I(0xd3a6e8)==8&&W(0xd3b186)!=0;
    if(I(0xd3b180)) {
        if(special)FUN_004c5ce0();
        else {
            FUN_00425730(I(0xd3b160)+16,I(0xd3b15c)+408,action_buttons(),1);
            FUN_00425730(I(0xd3b160)+436,I(0xd3b15c)+410,(const void *)0x5c7dd0u,1);
        }
        B(0xe38dc8)=0;return;
    }
    if(restore_message_record()->type==0&&I(0xd3aae0)>0) {
        FUN_00425730(I(0xd3b160)+16,restore_message_record()->rows*16-36+I(0xd3b15c),(const void *)0x5c7da8u,1);
        FUN_004ccbb0(12,0,I(0xd3b160)+52,restore_message_record()->rows*16-31+I(0xd3b15c),I(0xf56f8c),0);
    }
    if(restore_message_record()->type==2) {
        if(special)FUN_004c5ce0();
        else {
            FUN_00425730(I(0xd3b160)+16,restore_message_record()->rows*16-40+I(0xd3b15c),action_buttons(),1);
            if(restore_message_record()->subtype==1||restore_message_record()->subtype==7)
                FUN_00425730(I(0xd3b160)+64,I(0xd3b150)+36,(const void *)0x5c7e70u,1);
        }
    }
    FUN_00425730(restore_message_record()->columns*16-38+I(0xd3b160),restore_message_record()->rows*16-36+I(0xd3b15c),(const void *)0x5c7dd0u,1);
    if(I(0xd3b134)) {
        FUN_00425730(I(0xd3b14c)*16-1+I(0xd3b154),I(0xd3b150),(const void *)0x5c7df8u,1);
        FUN_00425730(I(0xd3b14c)*16-1+I(0xd3b154),I(0xd3b148)*16-26+I(0xd3b150),(const void *)0x5c7e20u,1);
    }
}
