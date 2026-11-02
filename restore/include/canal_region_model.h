#ifndef RESTORE_RESTORE_C_0047BAE0_H
#define RESTORE_RESTORE_C_0047BAE0_H
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
void restore_C_0047bae0(struct restore_menu_input_frame *frame);
void FUN_0047bae0(int32_t left,int32_t top,int32_t right,int32_t bottom,uint32_t refresh);
#endif
