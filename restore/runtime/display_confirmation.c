#include "display_confirmation.h"
extern int FUN_004d6740(int x,int y,int buttons,uint32_t count,int hit);
static int add(int x,uint32_t y){return (int)((uint32_t)x+y);}
void FUN_00402e10(void)
{
    uint32_t current=DC_U(0xe38e68);uint8_t mode=DC_B(0x70570c);
    DC_U(0xe38ee4)=current;DC_B(0xe38dda)=mode;DC_B(0xe38dca)=1;
}
void restore_display_confirmation_input(void)
{
    RestoreMenuButton *button=(RestoreMenuButton *)0x5c68a8u;
    int y=add(DC_I(0xe38da4),64),x=add(DC_I(0xe38da8),80),hit;
    uint32_t now,deadline;
    hit=FUN_00426450(x,y,button,1);
    y=add(DC_I(0xe38da4),64);x=add(DC_I(0xe38da8),80);
    hit=FUN_004d6740(x,y,0x5c68a8,1,hit);
    x=add(DC_I(0xe38da8),80);DC_I(0x7056c8)=hit;
    y=add(DC_I(0xe38da4),64);
    if(FUN_004261d0(x,y,button,1))return;
    now=restore_display_confirmation_clock();
    deadline=DC_U(0x6057e8)+10000u;
    if((int32_t)now>(int32_t)deadline)FUN_00402de0();
}
int restore_display_confirmation_action(uint32_t token)
{
    switch(token){case 0x402da0:FUN_00402da0();break;
    case 0x402de0:FUN_00402de0();break;case 0x402e10:FUN_00402e10();break;
    default:return 0;}return 1;
}
