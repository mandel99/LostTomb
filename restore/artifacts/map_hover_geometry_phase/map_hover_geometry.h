#ifndef RESTORE_MAP_HOVER_GEOMETRY_H
#define RESTORE_MAP_HOVER_GEOMETRY_H
#include <stdint.h>
/* Three observed native callers pass this DWORD; the body does not use it. */
int32_t FUN_00430790(uint32_t ignored);
#ifdef MAP_HOVER_GEOMETRY_TRACE
uint32_t map_hover_read(uint32_t address);
void map_hover_write(uint32_t address,uint32_t value);
void map_hover_enter(uint32_t argument);
#define MHR(a) map_hover_read(a)
#define MHW(a,v) map_hover_write((a),(v))
#define MHE(v) map_hover_enter(v)
#else
#define MHR(a) (*(volatile uint32_t *)(uintptr_t)(a))
#define MHW(a,v) (*(volatile uint32_t *)(uintptr_t)(a)=(v))
#define MHE(v) ((void)0)
#endif
#endif
