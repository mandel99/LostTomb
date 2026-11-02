#include "display_confirmation.h"
void FUN_00402da0(void)
{
    int32_t current=DC_I(0xe38e68),confirmed=DC_I(0xe38ee4);
    if(current>confirmed&&current>1){
        uint32_t now=restore_display_confirmation_clock();
        uint32_t mode;
        DC_U(0x6057e8)=now;mode=DC_B(0xe38dda);
        DC_B(0xe38dda)=0x3e;DC_U(0x70570c)=mode;DC_B(0xe38dca)=1;
    }
}
