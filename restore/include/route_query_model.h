#ifndef RESTORE_RESTORE_C_00486A30_H
#define RESTORE_RESTORE_C_00486A30_H
#include <stdint.h>
#ifndef RESTORE_RESTORE_MENU_INPUT_FRAME_DEFINED
#define RESTORE_RESTORE_MENU_INPUT_FRAME_DEFINED
/* Registers: EAX,ECX,EDX,EBX,ESP,EBP,ESI,EDI.
   boundary receives the native entry or IAT slot and an explicit call frame.
   Bind genuine callees; unsupported calls must be reported by the caller. */
struct restore_menu_input_frame;
typedef void (*restore_menu_input_frame_call)(uint32_t,uint32_t,struct restore_menu_input_frame *);
struct restore_menu_input_frame { uint32_t r[8],flags; restore_menu_input_frame_call boundary; };
#endif
uint32_t FUN_00486a30(int32_t start_x,int32_t start_y,int32_t end_x,int32_t end_y,uint32_t layout,int32_t orientation,int32_t wanted_orientation,uint32_t *path,int32_t *count);
void restore_C_00486a30(struct restore_menu_input_frame *frame);
#endif
