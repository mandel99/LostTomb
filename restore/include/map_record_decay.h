#ifndef RESTORE_MAP_RECORD_DECAY_H
#define RESTORE_MAP_RECORD_DECAY_H
#include <stdint.h>
void FUN_00442ef0(void);
void FUN_00442f80(void);
#ifdef MAP_RECORD_DECAY_TRACE
uint32_t map_decay_read(uint32_t address,uint32_t width);
void map_decay_write(uint32_t address,uint32_t width,uint32_t value);
#define MDR(a,n) map_decay_read((a),(n))
#define MDW(a,n,v) map_decay_write((a),(n),(v))
#else
static __inline uint32_t map_decay_read(uint32_t a,uint32_t n)
{if(n==1)return *(volatile uint8_t *)(uintptr_t)a;return *(volatile uint32_t *)(uintptr_t)a;}
#define MDR(a,n) map_decay_read((a),(n))
#define MDW(a,n,v) do{if((n)==1)*(volatile uint8_t *)(uintptr_t)(a)=(uint8_t)(v);else *(volatile uint16_t *)(uintptr_t)(a)=(uint16_t)(v);}while(0)
#endif
#endif
