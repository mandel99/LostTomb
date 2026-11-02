#ifndef RESTORE_TOOL_SELECTOR_INPUT_H
#define RESTORE_TOOL_SELECTOR_INPUT_H
#include "tool_selector.h"
#include "menu_input.h"
#define TI_U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define TI_I(a) ((int32_t)TI_U(a))
#define TI_W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define TI_S(a) ((int32_t)*(volatile int16_t *)(uintptr_t)(uint32_t)(a))
#define TI_B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
int32_t FUN_00403e00(void);
int32_t FUN_00403f10(int32_t category,int32_t previous);
int32_t FUN_004d5ed0(void);
#endif
