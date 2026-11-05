#ifndef RESTORE_ACTOR_CADENCE_CLOSURE_H
#define RESTORE_ACTOR_CADENCE_CLOSURE_H
#include <stdint.h>

/* Full EAX plus native memory effects; other volatile registers/flags are
   not logical outputs. The frame form preserves all actual entry inputs. */
void restore_actor_cadence_step_raw(uint32_t registers[8],uint32_t *flags);
void restore_actor_cadence_raw(uint32_t registers[8],uint32_t *flags);
uint32_t restore_actor_animation_step_eax(int32_t actor,int32_t steps,uint32_t incoming_eax);
uint32_t restore_actor_advance_cadence(int32_t actor,int32_t period,uint32_t incoming_eax);
int32_t FUN_004a3b00(int32_t actor,int32_t period);
#endif
