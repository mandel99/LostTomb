#ifndef RESTORE_NUMERIC_COMPLETION_H
#define RESTORE_NUMERIC_COMPLETION_H
#include <stdint.h>
/* Native nullary cdecl/RET0 callbacks. No incoming GPR/FPU value is used. */
void restore_numeric_completion_00407d40(void);
void restore_numeric_completion_00408120(void);
void restore_numeric_completion_004084b0(void);
void restore_numeric_completion_00449c90(void);
void restore_numeric_completion_00449e40(void);
void restore_numeric_completion_00449ff0(void);
void restore_numeric_completion_0044a1a0(void);
void restore_numeric_completion_0044a350(void);
void restore_numeric_completion_0044a530(void);
void restore_numeric_completion_0044a5b0(void);
int restore_numeric_completion_action(uint32_t token);
void restore_event_range_pack_0044c780(uint32_t self,int16_t first,int16_t second,int16_t third);
#endif
