#include "menu_rotation_actions.h"
#include <stdlib.h>

/* All public callers, including mission setup and city ticks, use the same
   complete C46D840 graph as the rotation menu. Its member calls retain their
   ECX receivers and RETn contracts through the existing typed boundaries. */
void FUN_0046d840(void)
{
    uint32_t stack[4096],registers[8]={0},flags=2;
    uint32_t sp=(uint32_t)(uintptr_t)(stack+4000);
    stack[4000]=0;registers[4]=sp;
    restore_rotation_raw(0x46d840u,registers,&flags);
    if(registers[4]!=sp+4u||registers[3]||registers[5]||registers[6]||registers[7])abort();
}
