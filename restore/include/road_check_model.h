#ifndef RESTORE_RESTORE_C_0043F410_H
#define RESTORE_RESTORE_C_0043F410_H
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
uint32_t restore_construction_site_valid(int32_t kind);
uint32_t restore_construction_select_slot(int32_t x,int32_t y,int32_t *out_x,int32_t *out_y,int32_t primary_kind,int32_t secondary_kind,uint32_t inherited_x,uint32_t inherited_y);
uint32_t FUN_0043f410(int32_t kind);
uint32_t FUN_00441130(int32_t x,int32_t y,int32_t *out_x,int32_t *out_y,int32_t primary_kind,int32_t secondary_kind);
#endif
