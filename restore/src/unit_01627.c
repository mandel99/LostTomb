#include "message_panel_input.h"
int32_t FUN_004c7db0(void)
{
    int32_t right,range,position,percent;
    if(MI_I(0xd3b134)<1||!MI_B(0xe38e00))return 0;
    right=mi_add(mi_cells(MI_I(0xd3b14c)),MI_I(0xd3b154));range=mi_sub(mi_cells(MI_I(0xd3b148)),52);
    if(MI_I(0xe38e34)<mi_add(right,1)||MI_I(0xe38e34)>mi_add(right,41)||MI_I(0xe38e30)<mi_add(MI_I(0xd3b150),26)||MI_I(0xe38e30)>mi_add(mi_add(MI_I(0xd3b150),26),range))return 0;
    position=mi_sub(mi_sub(MI_I(0xe38e30),MI_I(0xd3b150)),11);if(position>range)position=range;
    percent=FUN_004cde60(position,range);MI_I(0xd3b130)=FUN_004cde40(MI_I(0xd3b134),percent);
    MI_U(0xd3b12c)=1;MI_I(0xd3b128)=mi_sub(position,25);FUN_004c8040();if(MI_I(0xd3b128)<0)MI_U(0xd3b128)=0;
    MI_B(0xe38dca)=2;return 1;
}
