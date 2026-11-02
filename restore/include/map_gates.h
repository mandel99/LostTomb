#ifndef RESTORE_RESTORE_C_0047D870_H
#define RESTORE_RESTORE_C_0047D870_H
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
void restore_C_0047d870(struct restore_menu_input_frame *frame);
void restore_C_00482810(struct restore_menu_input_frame *frame);
uint32_t FUN_00482810(int32_t x,int32_t y,int32_t size,int32_t radius,int32_t ignored_cell);
void FUN_0047d870(uint32_t clear);
#endif
