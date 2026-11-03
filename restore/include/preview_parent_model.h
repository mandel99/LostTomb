#ifndef RESTORE_RESTORE_C_0046FEF0_H
#define RESTORE_RESTORE_C_0046FEF0_H
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
void restore_construction_preview(int32_t x1,int32_t y1,int32_t x2,int32_t y2,int32_t tool);
void restore_construction_drag_update(void);
/* Native nullary input entry and five-DWORD preview controller, both RET0. */
void FUN_00405d90(void);
void FUN_0046fef0(int32_t x1,int32_t y1,int32_t x2,int32_t y2,int32_t tool);
#endif
