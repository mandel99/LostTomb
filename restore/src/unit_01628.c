#include "message_panel_input.h"
extern void FUN_00539170(void);
void FUN_004c7f30(void)
{
    if(MI_U(0xd3b180))FUN_00539170();
    MI_U(0xd3b180)=0;MI_B(0xe38dda)=MI_B(0xd3b17c);MI_U(0xd3a6e8)=0;MI_B(0xe38dca)=1;
}
