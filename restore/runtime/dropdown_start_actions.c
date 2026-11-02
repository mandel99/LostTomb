#include "dropdown_start_actions.h"
#include "dropdown_menu.h"
#include "family_menu.h"
#include "sound_streams.h"
extern void FUN_004d3c40(void);
extern void FUN_004cb6a0(void);
/* Existing selector's first DWORD parameter is unused; preserve its ABI. */
extern void FUN_004cdff0(uint32_t pattern,char maps);
void FUN_004023a0(void)
{
    uint32_t editor;
    FUN_004d3c40();editor=DM_U(0x605a3c);
    DM_U(0x78d7b0)=0;DM_U(0x78d7a8)=0;DM_U(0x78d7a4)=0;DM_U(0x78d7a0)=0;
    DM_U(0x6059e8)=0;DM_U(0xbc9694)=0;DM_U(0xe94e0c)=0;DM_U(0xe94e10)=0;
    if(editor==1){DM_B(0xe38dda)=0;DM_B(0xe38dca)=1;}
    else switch(DM_B(0x7027b4)){
    case 1:FUN_00516bb0();FUN_00516990();break;
    case 0:case 2:case 3:FUN_004cb6a0();break;
    }
    if(DM_B(0xe38e64)&&DM_U(0xf55b88))
        FUN_0055f449(RESTORE_SOUND,(const char *)0x5761acu,0,0);
}
void FUN_00409580(void)
{
    FUN_004cdff0(0x576260u,1);
    DM_U(0xe20998)=0;DM_B(0xe38dda)=16;DM_B(0xd3b1c0)=1;
    DM_U(0xf53f2c)=0;DM_B(0xe38dca)=1;
}
int restore_dropdown_start_action(uint32_t token)
{
    switch(token){
    case 0x4023a0:FUN_004023a0();return 1;
    case 0x409580:FUN_00409580();return 1;
    default:return 0;
    }
}
