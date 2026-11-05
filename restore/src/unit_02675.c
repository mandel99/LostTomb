#include "sprite_controller.h"

uint32_t restore_sprite_cycle_next_action(RestoreSpriteCycle *self)
{
    uint32_t descriptor,next;
    if(!restore_sprite_cycle_tick(self))return 0;
    /* The descriptor is reloaded after the timing/cycle call completes. */
    descriptor=*(volatile uint32_t *)(uintptr_t)((uint32_t)(uintptr_t)self+0x40u);
    next=*(volatile uint8_t *)(uintptr_t)(descriptor+4u);
    restore_sprite_cycle_set_action(self,next);
    return 1;
}
