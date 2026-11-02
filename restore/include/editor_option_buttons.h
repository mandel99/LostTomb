#ifndef RESTORE_EDITOR_OPTION_BUTTONS_H
#define RESTORE_EDITOR_OPTION_BUTTONS_H
#include <stdint.h>
/* Whole native nullary cdecl/RET0 callbacks; input handlers discard EAX. */
void restore_editor_option_00408f10(void);
void restore_editor_option_00408f30(void);
void restore_editor_option_00408f60(void);
void restore_editor_option_00408f90(void);
void restore_editor_option_00408fd0(void);
void restore_editor_option_00408ff0(void);
int restore_editor_option_action(uint32_t token);
#endif
