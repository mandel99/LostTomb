#ifndef RESTORE_MONUMENT_PLACEMENT_QUERIES_H
#define RESTORE_MONUMENT_PLACEMENT_QUERIES_H
#include <stdint.h>
/* The native low-byte result is boolean; the remaining EAX bits are retained. */
uint32_t FUN_00541ae0(uint32_t cell);
/* Explicit raw-frame entry. Incoming EAX matters when no eligible query runs. */
uint32_t restore_monument_ready_00542ee0(uint32_t incoming_eax);
/* Physical no-argument cdecl/RET0 entry captures EAX before a C prologue. */
uint32_t FUN_00542ee0(void);
#endif
