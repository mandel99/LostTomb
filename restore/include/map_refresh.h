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
void FUN_004818a0(int32_t x,int32_t y,int32_t size,int32_t ring,int8_t amount);
void FUN_00481830(int32_t x,int32_t y,int32_t size,int32_t amount,int32_t interval,int32_t step,int32_t range);
void FUN_0046a0c0(void);
void FUN_0046a140(void);
#endif
