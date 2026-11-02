#ifndef RESTORE_ORDER_BRANCH_H
#define RESTORE_ORDER_BRANCH_H
#include <stdint.h>
#include "menu_window_cursor_consumer.h"
#define OB_U(a) (*(volatile uint32_t *)(uintptr_t)(a))
#define OB_S(a) (*(volatile int16_t *)(uintptr_t)(a))
#define OB_B(a) (*(volatile uint8_t *)(uintptr_t)(a))
#ifdef ORDER_BRANCH_TRACE
void order_input_store(uint32_t address,uint32_t value,uint32_t size);
#define OB_W(a,v) order_input_store((a),(uint32_t)(v),4)
#define OB_WS(a,v) order_input_store((a),(uint16_t)(v),2)
#define OB_WB(a,v) order_input_store((a),(uint8_t)(v),1)
#else
#define OB_W(a,v) (OB_U(a)=(uint32_t)(v))
#define OB_WS(a,v) (*(volatile uint16_t *)(uintptr_t)(a)=(uint16_t)(v))
#define OB_WB(a,v) (OB_B(a)=(uint8_t)(v))
#endif
void FUN_0040b830(void);
int32_t FUN_004b5640(uint32_t tile);
#endif
