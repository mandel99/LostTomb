#ifndef RESTORE_SMALL_BUTTON_ACTIONS2_H
#define RESTORE_SMALL_BUTTON_ACTIONS2_H
#include <stdint.h>
/* Logical nullary cdecl/RET0 callbacks, no incoming ECX. Their native return
   registers are discarded by the actual36-byte button dispatcher. */
void restore_small_button2_0040aac0(void);
void restore_small_button2_0040aae0(void);
void restore_small_button2_00409510(void);
void restore_small_button2_00418390(void);
void restore_small_button2_00511c50(void);
void restore_small_button2_00409520(void);
int restore_small_button_action2(uint32_t token);
#endif
