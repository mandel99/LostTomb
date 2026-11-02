#ifndef RESTORE_RESTORE_BUILDING_IMAGE_C_004DF360_H
#define RESTORE_RESTORE_BUILDING_IMAGE_C_004DF360_H
#include <stdint.h>
#ifndef RESTORE_RESTORE_BUILDING_IMAGE_FRAME_DEFINED
#define RESTORE_RESTORE_BUILDING_IMAGE_FRAME_DEFINED
/* Registers: EAX,ECX,EDX,EBX,ESP,EBP,ESI,EDI.
   boundary receives the native entry or IAT slot and an explicit call frame.
   Bind genuine callees; unsupported calls must be reported by the caller. */
struct restore_building_image_frame;
typedef void (*restore_building_image_frame_call)(uint32_t,uint32_t,struct restore_building_image_frame *);
struct restore_building_image_frame { uint32_t r[8],flags; restore_building_image_frame_call boundary; };
#endif
void restore_building_image_C_004df360(struct restore_building_image_frame *frame);
int32_t restore_building_image_004df360(int32_t building,uint32_t *mode_out);
int32_t restore_building_slot_004f4aa0(int32_t building);
int32_t FUN_004df360(int32_t building,uint32_t *mode_out);
int32_t FUN_004f4aa0(int32_t building);
#endif
