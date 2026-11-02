#ifndef RESTORE_EDITOR_MODAL_ACTIONS3_H
#define RESTORE_EDITOR_MODAL_ACTIONS3_H
#include <stdint.h>
/* Reviewed nullary cdecl / RET0 callbacks, no implicit receiver. */
void restore_editor_modal3_00408ac0(void);
void restore_editor_modal3_00408b00(void);
void restore_editor_modal3_00408b40(void);
void restore_editor_modal3_00408b80(void);
void restore_editor_modal3_00408bc0(void);
void restore_editor_modal3_00408c00(void);
/* Native one-DWORD/RET0 helper consumes the argument's low BYTE;
   its real XOR EAX,EAX epilogue returns a full DWORD zero. */
uint32_t restore_editor_palette_00414730(uint32_t palette);
int restore_editor_modal3_action(uint32_t token);
#endif
