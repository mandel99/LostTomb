#ifndef RESTORE_RESTORE_REMAINING_SELECTION_C_00547A60_H
#define RESTORE_RESTORE_REMAINING_SELECTION_C_00547A60_H
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
void restore_remaining_selection_C_00547a60(struct restore_menu_input_frame *frame);
/* Native 547A60 is nullary cdecl/RET0. Its sole member call is genuine
   5468D0, ECX=self +one DWORD building ID/RET4. The raw entry preserves
   caller-supplied scratch/registers; the convenience entry has no live-in
   argument or consumed return. No original code address is invoked. */
void restore_remaining_selection_00547a60(void);
/* All five callbacks are nullary cdecl/RET0. Their callers discard volatile
   register returns. Selection indices and counts retain native unchecked
   DWORD addressing; these interfaces add no bounds or fallback values. */
void restore_remaining_selection_00409da0(void);
void restore_remaining_selection_0040a6c0(void);
void restore_remaining_selection_0040ab00(void);
void restore_remaining_selection_0040b4e0(void);
void restore_remaining_selection_0040d130(void);
int restore_remaining_selection_action(uint32_t token);
#endif
