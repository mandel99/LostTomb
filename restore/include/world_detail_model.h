#ifndef RESTORE_RESTORE_BUILDING_WORLD_00439F10_H
#define RESTORE_RESTORE_BUILDING_WORLD_00439F10_H
#include <stdint.h>
#ifndef RESTORE_RESTORE_BUILDING_WORLD_FRAME_DEFINED
#define RESTORE_RESTORE_BUILDING_WORLD_FRAME_DEFINED
/* Registers: EAX,ECX,EDX,EBX,ESP,EBP,ESI,EDI.
   boundary receives the native entry or IAT slot and an explicit call frame.
   Bind genuine callees; unsupported calls must be reported by the caller. */
struct restore_building_world_frame;
typedef void (*restore_building_world_frame_call)(uint32_t,uint32_t,struct restore_building_world_frame *);
struct restore_building_world_frame { uint32_t r[8],flags; restore_building_world_frame_call boundary; };
#endif
void restore_building_world_00439f10(struct restore_building_world_frame *frame);
#endif
