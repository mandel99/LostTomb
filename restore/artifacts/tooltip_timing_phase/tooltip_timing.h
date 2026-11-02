#ifndef RESTORE_TOOLTIP_TIMING_H
#define RESTORE_TOOLTIP_TIMING_H
#include <stdint.h>
uint32_t FUN_00409e40(void);
int32_t FUN_00405170(uint32_t group);
#ifdef TOOLTIP_TIMING_TRACE
uint32_t tooltip_read(uint32_t address);
void tooltip_write(uint32_t address,uint32_t value);
void tooltip_enter(uint32_t address,uint32_t argument);
#define TTR(a) tooltip_read(a)
#define TTW(a,v) tooltip_write((a),(v))
#define TTE(a,v) tooltip_enter((a),(v))
#else
#define TTR(a) (*(volatile uint32_t *)(uintptr_t)(a))
#define TTW(a,v) (*(volatile uint32_t *)(uintptr_t)(a)=(v))
#define TTE(a,v) ((void)0)
#endif
#endif
