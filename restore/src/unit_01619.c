#include "message_panel_input.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
void FUN_004c5bf0(int32_t message,uint32_t background,uint8_t templated)
{
    uint32_t i,media,mode;uint8_t saved;
    B(0xd3b184)=templated;
    W(0xd3b186)=templated?W(0xe9524eu+U(0xe951e4u)*48u):0;
    for(i=0;i<200;++i)U(0xd3ae04u+i*4u)=0;
    for(i=0;i<200;++i)U(0xd3aae4u+i*4u)=0;
    U(0xd3b124)=(uint32_t)message;U(0xd3b140)=background;
    U(0xd3aae0)=0;U(0xd3b130)=0;
    media=U(0xe209f4u+(uint32_t)message*80u);mode=U(0xe950dcu);
    U(0xd3b138)=0;U(0xd3b12c)=0;U(0xd3b180)=media!=0;
    if(mode!=1)U(0xd3b180)=0;
    FUN_004c8040();B(0xe38dca)=1;saved=B(0xe38dda);
    B(0xe38dda)=5;U(0xd3b17c)=saved;
}
