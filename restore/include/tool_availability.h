#ifndef RESTORE_RESTORE_UNLOCK_C_00404350_H
#define RESTORE_RESTORE_UNLOCK_C_00404350_H
#include <stdint.h>
#ifndef RESTORE_RESTORE_UNLOCK_FRAME_DEFINED
#define RESTORE_RESTORE_UNLOCK_FRAME_DEFINED
/* Registers: EAX,ECX,EDX,EBX,ESP,EBP,ESI,EDI.
   boundary receives the native entry or IAT slot and an explicit call frame.
   Bind genuine callees; unsupported calls must be reported by the caller. */
struct restore_unlock_frame;
typedef void (*restore_unlock_frame_call)(uint32_t,uint32_t,struct restore_unlock_frame *);
struct restore_unlock_frame { uint32_t r[8],flags; restore_unlock_frame_call boundary; };
#endif
void restore_unlock_C_00404350(struct restore_unlock_frame *frame);
#endif
