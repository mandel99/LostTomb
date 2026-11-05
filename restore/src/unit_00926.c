#include "actor_basic_profile.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))

/* Whole004620C0,30 instructions. Preserve the three separate kind loads and
   the narrow flags; this initializes fewer fields than00461F90. */
void restore_actor_initialize_basic_profile(int32_t actor)
{
    uint32_t offset=(uint32_t)actor*388u;
    U(0xbcd71cu+offset)=0u;
    U(0xbcd720u+offset)=U(0x7090d0u+(uint32_t)B(0xbcd602u+offset)*56u);
    U(0xbcd724u+offset)=U(0x7090d0u+(uint32_t)B(0xbcd602u+offset)*56u);
    B(0xbcd728u+offset)=0u;
    U(0xbcd734u+offset)=U(0x7090ecu+(uint32_t)B(0xbcd602u+offset)*56u);
    B(0xbcd74cu+offset)=0u;
}

void FUN_004620c0(int32_t actor)
{
    restore_actor_initialize_basic_profile(actor);
}
