#ifndef RESTORE_RESTORE_MAP_COMMIT_C_004728A0_H
#define RESTORE_RESTORE_MAP_COMMIT_C_004728A0_H
#include <stdint.h>
#ifndef RESTORE_RESTORE_MAP_COMMIT_FRAME_DEFINED
#define RESTORE_RESTORE_MAP_COMMIT_FRAME_DEFINED
/* Registers: EAX,ECX,EDX,EBX,ESP,EBP,ESI,EDI.
   boundary receives the native entry or IAT slot and an explicit call frame.
   Bind genuine callees; unsupported calls must be reported by the caller. */
struct restore_map_commit_frame;
typedef void (*restore_map_commit_frame_call)(uint32_t,uint32_t,struct restore_map_commit_frame *);
struct restore_map_commit_frame { uint32_t r[8],flags; restore_map_commit_frame_call boundary; };
#endif
void restore_map_commit_C_004728a0(struct restore_map_commit_frame *frame);
#endif
