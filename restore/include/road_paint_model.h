#ifndef RESTORE_RESTORE_C_0047B310_H
#define RESTORE_RESTORE_C_0047B310_H
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
void FUN_0047b310(int32_t x,int32_t y,int32_t set);
void FUN_0047b960(int32_t x,int32_t y,int32_t alternate,int32_t set);
void restore_C_0047b960(struct restore_menu_input_frame *frame);
void restore_C_0047b310(struct restore_menu_input_frame *frame);
#endif
