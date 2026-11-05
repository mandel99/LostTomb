#ifndef RESTORE_ACCEPTANCE_PREDICATES_H
#define RESTORE_ACCEPTANCE_PREDICATES_H
#include <stdint.h>

/* Native 4E05F0/5421D0 take three cdecl slots. Only the low BYTE of the
   third slot controls committing the accepted work. These APIs expose AL;
   the existing RAW owners retain physical EAX, registers, flags and slots.
   A zero commit byte still permits stale-worker cleanup by native children. */
uint8_t restore_actor_accept_kind222_work(int32_t building, int32_t actor,
                                        uint8_t commit_changes);
uint8_t restore_actor_accept_kind213_work(int32_t building, int32_t actor,
                                        uint8_t commit_changes);

#endif
