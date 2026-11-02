#include "sprite_libraries.h"

/* Whole native53D850,32 instructions. Player is a DWORD, variation a signed
   WORD, and each name-table lookup uses a wrapping12-byte record offset.
   The three queries reload player/variation after each genuine load call. */
static const char *sphinx_name(uint32_t table)
{
    uint32_t player=*(volatile uint32_t *)(uintptr_t)0xeaac28u;
    uint32_t row=0xea5baau+player*0x49d8u;
    uint32_t variation=(uint32_t)(int32_t)*(volatile int16_t *)(uintptr_t)row;
    uint32_t name=*(volatile uint32_t *)(uintptr_t)(table+variation*12u);
    return (const char *)(uintptr_t)name;
}
void FUN_0053d850(void)
{
    (void)FUN_0053dd90(RESTORE_SPRITE_LIBRARIES,sphinx_name(0x601660u),5);
    (void)FUN_0053dd90(RESTORE_SPRITE_LIBRARIES,sphinx_name(0x601664u),6);
    (void)FUN_0053dd90(RESTORE_SPRITE_LIBRARIES,sphinx_name(0x601668u),7);
    (void)FUN_0053dd90(RESTORE_SPRITE_LIBRARIES,(const char *)(uintptr_t)0x5791b8u,8);
}
