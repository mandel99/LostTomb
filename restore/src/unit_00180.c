#include "settings_persistence.h"
#include <stdint.h>
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
extern uint32_t FUN_004ced90(const char *path,const void *data,uint32_t bytes);
extern void FUN_004ce6e0(const char *text,uint32_t argument,uint32_t value);
extern void FUN_00561bdc(void *allocation);
extern void FUN_00561a2b(void *allocation);

/* Whole414750. Settings include the difficulty DWORD at byte552 of564.
   Save both blocks before cleanup. Heap handles and the diagnostic value are
   full DWORDs, not their low bytes; only the two language pointers are cleared. */
void restore_settings_save_and_cleanup(void)
{
    uint32_t allocation;
    (void)FUN_004ced90((const char *)0x579274u,(const void *)0xe38e58u,564);
    (void)FUN_004ced90((const char *)0x57924cu,(const void *)0xbca2b8u,84);
    FUN_004ce6e0((const char *)0x57925cu,0,U(0xf535bcu)+1u);
    allocation=U(0xe92e1cu);if(allocation)FUN_00561bdc((void *)(uintptr_t)allocation);
    allocation=U(0xe36c80u);if(allocation)FUN_00561bdc((void *)(uintptr_t)allocation);
    allocation=U(0xe36c70u);
    if(allocation){FUN_00561a2b((void *)(uintptr_t)allocation);U(0xe36c70u)=0;}
    allocation=U(0xe36c74u);
    if(allocation){FUN_00561a2b((void *)(uintptr_t)allocation);U(0xe36c74u)=0;}
}
void FUN_00414750(void){restore_settings_save_and_cleanup();}
