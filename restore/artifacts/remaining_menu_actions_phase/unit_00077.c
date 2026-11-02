#include "remaining_menu_actions.h"
void FUN_0040a0e0(void)
{
    uint32_t gate=RA_U(0xe38ebc),selected=RA_U(0xd3b198),player;
    if(!gate && !RA_B(0x6fdc88u+selected)){
        if(RA_B(0xe38dda)==5){FUN_004c7f30();RA_W(0xe38dda,1,1);}
        restore_remaining_message_4e2970(13,0,0);return;
    }
    player=RA_U(0xeaac28);
    RA_W(0xe38dda,1,7);RA_W(0x605a44,4,selected);RA_W(0xe38ea4,4,selected);
    RA_W(0xea623cu+player*0x49d8u,1,0);
    FUN_0040a160();RA_W(0xe38dca,1,1);RA_W(0xf53f48,4,6);
}
