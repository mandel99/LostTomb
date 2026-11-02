#ifndef RESTORE_MENU_LABOR_ACTIONS_H
#define RESTORE_MENU_LABOR_ACTIONS_H
#include <stdint.h>
#ifndef RESTORE_RESTORE_LABOR_FRAME_DEFINED
#define RESTORE_RESTORE_LABOR_FRAME_DEFINED
/* Registers: EAX,ECX,EDX,EBX,ESP,EBP,ESI,EDI.
   boundary receives the native entry or IAT slot and an explicit call frame.
   Bind genuine callees; unsupported calls must be reported by the caller. */
struct restore_labor_frame;
typedef void (*restore_labor_frame_call)(uint32_t,uint32_t,struct restore_labor_frame *);
struct restore_labor_frame { uint32_t r[8],flags; restore_labor_frame_call boundary; };
#endif
void restore_labor_C_0040a730(struct restore_labor_frame *frame);
void restore_labor_C_00459400(struct restore_labor_frame *frame);
void restore_labor_C_004595a0(struct restore_labor_frame *frame);
void restore_labor_C_00459670(struct restore_labor_frame *frame);
/* Whole nullary RET0 priority callback; explicit virtual caller stack. */
int restore_menu_labor_action(uint32_t token);
void restore_labor_raw(uint32_t target,uint32_t registers[8],uint32_t *flags);
#endif
