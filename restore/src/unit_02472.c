#include "menu_keyboard_modal.h"
void FUN_00539220(uint32_t ignored)
{
    uint8_t mode=MM_B(0xf53ffc);uint32_t kind;
    (void)ignored;
    MM_B(0xe38dda)=mode;kind=MM_U(0xf54118);MM_U(0xf54054)=0;
    if(kind==2)MM_B(0xe38dca)=1;
    MM_U(0xf54118)=0;
}
