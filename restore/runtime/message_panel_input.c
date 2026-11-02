#include "message_panel_input.h"
void FUN_004c7e90(void)
{
    if(!MI_I(0xd3b198)){if(MI_I(0xd3b130)>0)MI_U(0xd3b130)-=1;}
    else if(MI_I(0xd3b198)==1&&MI_I(0xd3b130)<MI_I(0xd3b134))MI_U(0xd3b130)+=1;
    FUN_004c8040();MI_U(0xd3b12c)=0;MI_B(0xe38dca)=2;
}
void FUN_004c7ee0(void)
{
    uint32_t count=MI_U(0xd3aae0);if((int32_t)count<=0)return;
    MI_U(0xd3b124)=MI_U(0xd3ae00u+count*4u);MI_U(0xd3aae0)=count-1;MI_U(0xd3b130)=MI_U(0xd3aae0u+count*4u);
    FUN_004c8040();MI_U(0xd3b138)=MI_U(0xd3b12c)=0;MI_B(0xe38dca)=1;
}
