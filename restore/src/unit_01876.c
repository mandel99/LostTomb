#include "simulation_tick.h"
#include "graphics_session.h"
uint32_t FUN_004ddcb0(void)
{
    uint32_t now=restore_graphics_platform()->ticks();
    int32_t elapsed=(int32_t)(now-GFX_U(0x00e94db8));
    int32_t delay_index=(int32_t)(100u-GFX_U(0x00e38e6c))/10;
    unsigned int mode=GFX_B(0x00e38dda);
    if(elapsed<0) elapsed=10000;
    if(delay_index>=10||GFX_B(0x00e38e60)) return 0;
    if(mode==0x12||mode==0x23) {
        if(GFX_I(0x00d3a6c4)!=1) return 0;
    } else if(GFX_I(0x005e1c50u+mode*4u)) return 0;
    if(GFX_I(0x006059e4)||GFX_I(0x00e38dcc)) return 0;
    if(delay_index<0) delay_index=0;
    if(elapsed<(int32_t)(GFX_U(0x005e1db4u+(uint32_t)delay_index*4u)+2u)) return 0;
    GFX_U(0x00e94db8)=now;
    return 10000;
}
