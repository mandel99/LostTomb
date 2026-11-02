#ifndef RESTORE_LOOKUP_SENTINEL_H
#define RESTORE_LOOKUP_SENTINEL_H
#include <stdint.h>
#define LOOKUP_SENTINEL_U32(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define LOOKUP_SENTINEL_GRID 0x0070e494u
#define LOOKUP_SENTINEL_CELLS (231u*457u)
#define LOOKUP_SENTINEL_IMAGES 0x00b7482cu
void FUN_00430480(void);
void FUN_00430530(uint32_t orientation);
#endif
