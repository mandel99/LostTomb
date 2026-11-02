#ifndef RESTORE_MAP_MENU_SCROLL_H
#define RESTORE_MAP_MENU_SCROLL_H
#include <stdint.h>
void FUN_00442730(int32_t direction);
void FUN_00442800(void);
#ifdef MAP_MENU_SCROLL_TRACE
uint32_t map_scroll_read(uint32_t address);
void map_scroll_write(uint32_t address,uint32_t value);
void map_scroll_enter(void);
#define MSR(a) map_scroll_read(a)
#define MSW(a,v) map_scroll_write((a),(v))
#define MSE() map_scroll_enter()
#else
#define MSR(a) (*(volatile uint32_t *)(uintptr_t)(a))
#define MSW(a,v) (*(volatile uint32_t *)(uintptr_t)(a)=(v))
#define MSE() ((void)0)
#endif
#endif
