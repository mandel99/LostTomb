#ifndef RESTORE_SPRITE_CONTROLLER_H
#define RESTORE_SPRITE_CONTROLLER_H
#include "sprite_cycle.h"
#include "sprite_direction.h"

/* Whole 55D330. The native second stack parameter is unused. The last
   pointer optionally exposes writes to the native first argument slot;
   ordinary C callers pass NULL. Only the returned direction's low WORD is
   consumed here, so the mapper's inherited padding WORD is not live. */
void restore_sprite_cycle_select_action(RestoreSpriteCycle *self,
    uint32_t action,uint32_t frame,uint32_t *native_action_slot);
/* Whole 55D460 and 55D810 respectively. */
void restore_sprite_cycle_set_action(RestoreSpriteCycle *self,uint32_t action);
uint32_t restore_sprite_cycle_next_action(RestoreSpriteCycle *self);
/* Whole 55D6D0 mode dispatcher; mode1 returns its child result directly. */
uint32_t restore_sprite_cycle_dispatch(RestoreSpriteCycle *self);
/* 55D840's observed logical return is a WORD index: both recorded calls in
   55D330 reach 55D2E0's AND index,0xffff. No data/vtable reference is recorded.
   Native EAX high16 retains incoming EAX on the null-source path, or private
   stack padding sampled after the callback. It is outside this logical API.
   Targetless indirect flows
   are not covered by the cached references; full-EAX equivalence is unproved. */
uint16_t restore_sprite_cycle_direction(RestoreSpriteCycle *self);
#endif
