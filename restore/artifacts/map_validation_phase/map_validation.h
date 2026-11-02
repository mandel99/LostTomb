#ifndef RESTORE_MAP_VALIDATION_H
#define RESTORE_MAP_VALIDATION_H
#include <stdint.h>
void FUN_00442a40(uint32_t record_index);
uint32_t FUN_00443000(uint32_t record_index,uint32_t kind);
uint32_t FUN_004437b0(uint32_t kind);
uint32_t FUN_00443820(uint32_t kind);
uint32_t FUN_00443a80(uint32_t kind);
uint8_t FUN_00442870(uint32_t kind);
uint8_t FUN_0040c8b0(int32_t product);
void FUN_005344c0(int32_t message);
#ifdef MAP_VALIDATION_TRACE
uint32_t map_validation_read(uint32_t address,uint32_t width);
void map_validation_write(uint32_t address,uint32_t width,uint32_t value);
void map_validation_enter(uint32_t address,uint32_t first,uint32_t second);
#define MVR(a,n) map_validation_read((a),(n))
#define MVW(a,n,v) map_validation_write((a),(n),(v))
#define MVE(a,x,y) map_validation_enter((a),(uint32_t)(x),(uint32_t)(y))
#else
static __inline uint32_t map_validation_read(uint32_t a,uint32_t n)
{if(n==1)return *(volatile uint8_t *)(uintptr_t)a;return *(volatile uint16_t *)(uintptr_t)a;}
#define MVR(a,n) map_validation_read((a),(n))
#define MVW(a,n,v) do{if((n)==1)*(volatile uint8_t *)(uintptr_t)(a)=(uint8_t)(v);else *(volatile uint16_t *)(uintptr_t)(a)=(uint16_t)(v);}while(0)
#define MVE(a,x,y) ((void)0)
#endif
#endif
