#ifndef RESTORE_RESTORE_C_004B4980_H
#define RESTORE_RESTORE_C_004B4980_H
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
void restore_actor_release(int32_t actor);
void FUN_004b4980(int32_t actor);
uint32_t restore_actor_building_gate(int32_t building);
uint8_t restore_actor_building_query(int32_t building);
uint8_t FUN_004a43a0(int32_t actor);
uint8_t restore_ring_004f6950(void *self,int32_t actor);
uint8_t restore_ring_004f6a10(void *self,volatile uint32_t *index);
uint32_t restore_ring_004f6a50(void *self,int32_t capacity);
/* C-callable ring members carry the native ECX receiver explicitly. */
uint32_t FUN_004f68a0(void *self);
uint32_t FUN_004f68b0(void *self);
void FUN_004f68f0(void *self,uint32_t value);
void FUN_004f6900(void *self,uint32_t value);
uint8_t FUN_004f6950(void *self,int32_t value);
uint8_t FUN_004f6a10(void *self,volatile uint32_t *index);
#endif
