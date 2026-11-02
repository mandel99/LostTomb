#include "menu_keyboard_modal.h"
extern int FUN_004d6740(int x,int y,int buttons,uint32_t count,int hit);
void FUN_00538920(void)
{
    uint32_t index=MM_U(0xd3b198);
    MM_B(0xf535c0)=1;MM_U(0xf535c4)=index;
}
void restore_menu_modal_input(void)
{
    RestoreMenuButton *buttons=(RestoreMenuButton *)0x5f8910u;
    int hit=FUN_00426450(0,0,buttons,MM_I(0xf535c8));
    hit=FUN_004d6740(0,0,0x5f8910,MM_U(0xf535c8),hit);
    MM_I(0x7056c8)=hit;
    FUN_004261d0(0,0,buttons,MM_I(0xf535c8));
}
int restore_menu_modal_action(uint32_t token)
{
    if(token!=0x538920)return 0;
    FUN_00538920();return 1;
}
