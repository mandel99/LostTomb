#include "sprite_auxiliary.h"
#include "sprite_shutdown.h"
/* Whole55D960. Free the captured object even if a callback replaces the slot. */
void restore_sprite_auxiliary_release_slot(int32_t *slot)
{
    uint32_t saved=(uint32_t)*(volatile int32_t *)slot;
    if(saved){
        FUN_0055d8b0((RestoreSpriteAuxiliary *)(uintptr_t)saved);
        restore_sprite_shutdown_free((void *)(uintptr_t)saved);
        *(volatile int32_t *)slot=0;
    }
}
void FUN_0055d960(int32_t *slot){restore_sprite_auxiliary_release_slot(slot);}
