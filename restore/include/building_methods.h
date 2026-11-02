#ifndef RESTORE_RESTORE_BUILDING_METHOD_C_004015A0_H
#define RESTORE_RESTORE_BUILDING_METHOD_C_004015A0_H
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
void restore_building_method_C_004015a0(struct restore_menu_input_frame *frame);
void restore_building_method_C_004017a0(struct restore_menu_input_frame *frame);
void restore_building_method_C_0040da90(struct restore_menu_input_frame *frame);
void restore_building_method_C_0040dca0(struct restore_menu_input_frame *frame);
void restore_building_method_C_00417180(struct restore_menu_input_frame *frame);
void restore_building_method_C_004e6840(struct restore_menu_input_frame *frame);
void restore_building_method_C_004e69c0(struct restore_menu_input_frame *frame);
void restore_building_method_C_005417b0(struct restore_menu_input_frame *frame);
void restore_building_method_C_00541920(struct restore_menu_input_frame *frame);
void restore_building_method_C_00545c30(struct restore_menu_input_frame *frame);
void restore_building_method_C_005468d0(struct restore_menu_input_frame *frame);
void restore_building_method_C_0054fba0(struct restore_menu_input_frame *frame);
void restore_building_method_C_00556480(struct restore_menu_input_frame *frame);
void restore_building_method_C_00559130(struct restore_menu_input_frame *frame);
int restore_building_method_has(uint32_t target);
void restore_building_method_dispatch(uint32_t site,uint32_t target,struct restore_menu_input_frame *frame);
#endif
