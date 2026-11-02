#ifndef RESTORE_RESTORE_C_00470990_H
#define RESTORE_RESTORE_C_00470990_H
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
void restore_C_00470990(struct restore_menu_input_frame *frame);
/* Complete native body and genuine63-target closure. Standard cdecl eight-
   DWORD facade; only recognized original vtable data tokens are dispatched. */
void FUN_00470990(uint32_t owner,int32_t kind,int32_t building,int32_t x,int32_t y,int32_t size,uint32_t initialize,uint32_t mode);
void restore_placement_create_00470990(uint32_t owner,int32_t kind,int32_t building,int32_t x,int32_t y,int32_t size,uint32_t initialize,uint32_t mode);
void restore_building_create_dispatch(uint32_t site,uint32_t target,struct restore_menu_input_frame *frame);
#endif
