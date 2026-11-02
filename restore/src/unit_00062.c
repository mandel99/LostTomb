#include "message_panel_input.h"
void FUN_004072c0(void)
{
 uint32_t selector=*(volatile uint32_t *)0x70569cu;
 if(selector==1)FUN_004c5bf0(15,0,0);
 else if(selector==2)FUN_004c5bf0(16,0,0);
 else if(selector==3)FUN_004c5bf0(17,0,0);
 *(volatile uint8_t *)0xe38dcau=1;
}
