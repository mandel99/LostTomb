#ifndef RESTORE_TOOL_SELECTOR_H
#define RESTORE_TOOL_SELECTOR_H
#include "restore_types.h"
#include <stdint.h>
#define TS_I(a) (*(int32_t *)(uintptr_t)(a))
#define TS_U(a) (*(uint32_t *)(uintptr_t)(a))
#define TS_W(a) (*(uint16_t *)(uintptr_t)(a))
#define TS_S(a) (*(int16_t *)(uintptr_t)(a))
#define TS_B(a) (*(uint8_t *)(uintptr_t)(a))
void FUN_00405200(void);
int32_t FUN_00404120(void);
void FUN_00486060(int32_t index);
void FUN_0053d850(void);
uint32_t FUN_00403ef0(void),FUN_004f5470(int32_t building);
int32_t FUN_004f5020(void),FUN_004f4ff0(void);
uint8_t FUN_004c52d0(int32_t building,int32_t phase);
#endif
