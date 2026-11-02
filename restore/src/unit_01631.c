#include "message_panel_input.h"
void FUN_004c8040(void)
{
    uint32_t address;
    for(address=0xd3a6f8u;address<0xd3aae0u;address+=4u)
        *(volatile uint32_t *)(uintptr_t)address=0;
    *(volatile uint32_t *)0xd3a6f0u=0;*(volatile uint32_t *)0xd3a6ecu=0;
}
