#ifndef RESTORE_MAP_REFRESH_PHASE_H
#define RESTORE_MAP_REFRESH_PHASE_H
#include <stdint.h>
#include <stddef.h>
#ifdef MAP_REFRESH_TRACE
uint32_t map_refresh_read(uint32_t address,uint32_t width);
#define MR_B(a) map_refresh_read((uint32_t)(a),1)
#define MR_U(a) map_refresh_read((uint32_t)(a),4)
#else
#define MR_B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define MR_U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#endif
int32_t FUN_00481aa0(int32_t x,int32_t y,int32_t family,int32_t ring,int32_t point);
#endif
