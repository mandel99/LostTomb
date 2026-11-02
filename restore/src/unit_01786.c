#include "graphics_session.h"
extern void FUN_0051e990(void);
void FUN_004d0280(int force)
{
    int32_t counter;
    uint8_t mode;
    if(GFX_U(0x5e06b0)==1||(GFX_U(0xf54054)&&GFX_U(0xe92e04))) return;
    counter=GFX_I(0x78d7a0); mode=GFX_B(0xe38dda);
    if(mode!=5&&counter>0&&counter%136==0) {
        int i;
        FUN_004d05f0(RESTORE_SURFACE_CONTEXT,1,15,0,1,0);
        FUN_004d03f0();
        for(i=0;i<8;++i) {FUN_004d0650(RESTORE_SURFACE_CONTEXT,1); FUN_004d03f0();}
        return;
    }
    GFX_U(0xe92e08)+=(uint32_t)FUN_004d0040();
    if(force==1) GFX_U(0xe92e08)=50;
    if(!GFX_B(0xe38dc8)&&GFX_I(0xe92e08)<50) return;
    GFX_U(0xe92e08)=0; GFX_B(0xe38dc8)=0;
    mode=GFX_B(0xe38dda);
    if(mode!=5&&GFX_I(0x78d7a0)>0) {
        switch(mode) {
        case 1:case 3:case 30:case 31:case 32:case 73:
            /* 004d0340 is a side-effect call. No return register is consumed. */
            FUN_0051e990();
            FUN_004d05f0(RESTORE_SURFACE_CONTEXT,1,1,0,0,1);
            break;
        }
    }
    FUN_004d03f0();
}
