#include "editor_option_buttons.h"
#include "numeric_input.h"
#include "selection_popup.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))

/* Table5C63D8/count18, main input call4D9841. Six entire native bodies,
   6/11/11/19/7/13 instructions. No incoming register, argument or x87 input.
   Reuse actual numeric403790 and selection403C00, not legacy code pointers. */
void restore_editor_option_00408f10(void)
{
    B(0xe38dcau)=1;
    W(0x784b60u)=(uint16_t)(W(0x784b60u)==0u);
}
void restore_editor_option_00408f30(void)
{
    FUN_00403790(300,200,1,0x784cbeu,0,0,0);
    B(0xe38dcau)=1;
}
void restore_editor_option_00408f60(void)
{
    FUN_00403790(300,200,4,0,0,0x784c34u,0);
    B(0xe38dcau)=1;
}
void restore_editor_option_00408f90(void)
{
    FUN_00403c00(300,100,20,29,0,0,0,0x784c38u,0,4,0,0,0,-1,0);
    B(0xe38dcau)=1;
}
void restore_editor_option_00408fd0(void)
{
    uint32_t offset=U(0xd3b198u);
    B(0xe38dcau)=1;
    B(0x784c3cu+offset)=(uint8_t)(B(0x784c3cu+offset)==0u);
}
void restore_editor_option_00408ff0(void)
{
    uint32_t offset=U(0xd3b198u);
    FUN_00403790(300,200,4,0,0,0x784c24u+offset*4u,0);
    B(0xe38dcau)=1;
}
int restore_editor_option_action(uint32_t token)
{
    switch(token){
    case 0x408f10u:restore_editor_option_00408f10();return 1;
    case 0x408f30u:restore_editor_option_00408f30();return 1;
    case 0x408f60u:restore_editor_option_00408f60();return 1;
    case 0x408f90u:restore_editor_option_00408f90();return 1;
    case 0x408fd0u:restore_editor_option_00408fd0();return 1;
    case 0x408ff0u:restore_editor_option_00408ff0();return 1;
    default:return 0;
    }
}
