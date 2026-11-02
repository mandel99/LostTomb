#ifndef RESTORE_ACTOR_PREDICATE_H
#define RESTORE_ACTOR_PREDICATE_H
#include <stdint.h>

/* cdecl, caller removes the stack argument. Native EAX has domain {0,1}.
   A byte return preserves the existing two callers' Boolean contract. */
uint8_t FUN_004387d0(int32_t actor_id);
/* cdecl stack arguments; neither helper receives an ECX object/receiver.
   5120 returns a full dword category (0..4 or UINT32_MAX).
   52d0 guarantees only AL; 4387d0 normalizes that byte to 0/1. */
uint32_t FUN_004c5120(int32_t building_id);
uint8_t FUN_004c52d0(int32_t building_id, uint32_t desired);

#ifdef RESTORE_ACTOR_PREDICATE_TRACE_READS
uint32_t restore_actor_predicate_trace_read(uint32_t address, uint32_t width);
#endif
#endif
