#ifndef RESTORE_ROUTE_SEED_H
#define RESTORE_ROUTE_SEED_H
#include <stdint.h>
void FUN_00405c20(void);
uint32_t FUN_00484f60(void);
uint32_t FUN_0051b680(uint32_t ignored,int32_t x,int32_t y,int32_t mode);
uint32_t FUN_0051c790(uint32_t ignored,int32_t x,int32_t y);
uint32_t FUN_0051c390(uint32_t cell);
uint32_t FUN_0051c510(uint32_t cell);
uint32_t FUN_0051c630(uint32_t cell);
uint32_t FUN_0047b8e0(uint32_t cell);
uint32_t FUN_0047e0d0(uint32_t cell);
#ifdef RESTORE_ROUTE_SEED_INTERNAL
uint32_t FUN_0047e090(uint32_t cell);
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
void FUN_0040f8f0(void *destination,uint32_t bytes,uint8_t value);
void FUN_0040f860(const void *source,void *destination,int32_t bytes);
#endif
#endif
