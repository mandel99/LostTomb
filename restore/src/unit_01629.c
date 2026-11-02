#include "message_panel_input.h"
int32_t FUN_004c7f70(void)
{
    int32_t i;
    if(MI_B(0xe38e03))for(i=0;i<MI_I(0xd3a6ec);++i) {
        uint32_t row=0xd3a6f8u+(uint32_t)i*20u;
        if(MI_I(row)<=MI_I(0xe38e34)&&MI_I(0xe38e34)<=MI_I(row+4)&&MI_I(row+8)<=MI_I(0xe38e30)&&MI_I(0xe38e30)<=MI_I(row+12))return MI_I(row+16);
    }
    return -1;
}
