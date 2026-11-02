#ifndef RESTORE_RESTORE_C_00440700_H
#define RESTORE_RESTORE_C_00440700_H
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
void restore_C_00440700(struct restore_menu_input_frame *frame);
void restore_C_00440850(struct restore_menu_input_frame *frame);
void restore_C_00440b10(struct restore_menu_input_frame *frame);
void restore_C_00440ed0(struct restore_menu_input_frame *frame);
uint32_t FUN_00440700(uint32_t preview,uint32_t ignored_a,uint32_t ignored_b,int32_t x,int32_t y);
uint32_t FUN_00440850(uint32_t preview,uint32_t ignored_a,uint32_t ignored_b,int32_t x,int32_t y);
uint32_t FUN_00440b10(uint32_t preview,uint32_t ignored_a,uint32_t ignored_b,int32_t x,int32_t y);
uint32_t FUN_00440ed0(uint32_t preview,uint32_t ignored_a,uint32_t ignored_b,int32_t x,int32_t y);
#endif
