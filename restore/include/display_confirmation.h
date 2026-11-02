#ifndef RESTORE_DISPLAY_CONFIRMATION_H
#define RESTORE_DISPLAY_CONFIRMATION_H
#include "menu_input.h"
#include <stdint.h>
#define DC_B(a) (*(uint8_t *)(uintptr_t)(a))
#define DC_U(a) (*(uint32_t *)(uintptr_t)(a))
#define DC_I(a) (*(int32_t *)(uintptr_t)(a))
void FUN_00402da0(void);
void FUN_00402de0(void);
void FUN_00402e10(void);
void FUN_00534540(void);
void FUN_004cf1c0(void);
void restore_display_confirmation_input(void);
int restore_display_confirmation_action(uint32_t token);
/* Production calls timeGetTime. Deterministic proofs supply this OS boundary. */
uint32_t restore_display_confirmation_clock(void);
#endif
