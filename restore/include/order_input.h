#ifndef RESTORE_ORDER_INPUT_H
#define RESTORE_ORDER_INPUT_H
#include <stdint.h>
#include "menu_window_cursor_consumer.h"
#define OI_U(a) (*(volatile uint32_t *)(uintptr_t)(a))
#define OI_S(a) (*(volatile int16_t *)(uintptr_t)(a))
#define OI_B(a) (*(volatile uint8_t *)(uintptr_t)(a))
#ifdef ORDER_INPUT_TRACE
void order_input_store(uint32_t address,uint32_t value,uint32_t size);
#define OI_W(a,v) order_input_store((a),(uint32_t)(v),4)
#define OI_WB(a,v) order_input_store((a),(uint8_t)(v),1)
#else
#define OI_W(a,v) (OI_U(a)=(uint32_t)(v))
#define OI_WB(a,v) (OI_B(a)=(uint8_t)(v))
#endif
uint32_t FUN_0040b630(void),FUN_0040b6d0(void),FUN_0040b790(void);
uint32_t FUN_0040b5e0(int32_t group);
int32_t FUN_004b55b0(uint32_t tile),FUN_004b5570(uint32_t tile);
#endif
