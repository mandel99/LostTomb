#ifndef RESTORE_INITIAL_MENU_DISPATCH_H
#define RESTORE_INITIAL_MENU_DISPATCH_H
#include "restore_types.h"
#include <stdint.h>
#ifdef INITIAL_DISPATCH_TRACE
uint32_t restore_initial_dispatch_read(uint32_t address,uint32_t width);
void restore_initial_dispatch_write(uint32_t address,uint32_t value);
#define ID_READ8(a) restore_initial_dispatch_read(a,1)
#define ID_READ32(a) restore_initial_dispatch_read(a,4)
#define ID_WRITE8(a,v) restore_initial_dispatch_write(a,(uint8_t)(v))
#else
#define ID_READ8(a) (*(volatile uint8_t *)(uintptr_t)(a))
#define ID_READ32(a) (*(volatile uint32_t *)(uintptr_t)(a))
#define ID_WRITE8(a,v) (*(volatile uint8_t *)(uintptr_t)(a)=(uint8_t)(v))
#endif
void FUN_004d71c0(int force);
void restore_initial_dispatch_training(uint32_t native_context);
#endif
