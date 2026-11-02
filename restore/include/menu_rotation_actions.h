#ifndef RESTORE_MENU_ROTATION_ACTIONS_H
#define RESTORE_MENU_ROTATION_ACTIONS_H
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
void restore_rotation_C_00409eb0(struct restore_menu_input_frame *frame);
void restore_rotation_C_00430b20(struct restore_menu_input_frame *frame);
void restore_rotation_C_00430be0(struct restore_menu_input_frame *frame);
void restore_rotation_C_0047d6e0(struct restore_menu_input_frame *frame);
void restore_rotation_C_0047d720(struct restore_menu_input_frame *frame);
void restore_rotation_C_0047fb70(struct restore_menu_input_frame *frame);
void restore_rotation_C_004dfd90(struct restore_menu_input_frame *frame);
void restore_rotation_C_004ee560(struct restore_menu_input_frame *frame);
void restore_rotation_C_0046d840(struct restore_menu_input_frame *frame);
void restore_rotation_raw(uint32_t entry,uint32_t registers[8],uint32_t *flags);
void FUN_0046d840(void);
int restore_menu_rotation_action(uint32_t token);
#endif
