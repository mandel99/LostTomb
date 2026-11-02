#ifndef RESTORE_MAP_RECORD_REFRESH_H
#define RESTORE_MAP_RECORD_REFRESH_H
#include <stdint.h>
uint32_t FUN_004425c0(uint32_t record_index);
void FUN_004426c0(void);
#ifdef MAP_RECORD_REFRESH_TRACE
uint32_t map_record_read(uint32_t address,uint32_t width);
void map_record_write16(uint32_t address,uint32_t value);
void map_record_enter(uint32_t index);
#define MRR(a,n) map_record_read((a),(n))
#define MRW16(a,v) map_record_write16((a),(uint16_t)(v))
#define MRE(i) map_record_enter(i)
#else
static __inline uint32_t map_record_read(uint32_t a,uint32_t n)
{
    if(n==1)return *(volatile uint8_t *)(uintptr_t)a;
    if(n==2)return *(volatile uint16_t *)(uintptr_t)a;
    return *(volatile uint32_t *)(uintptr_t)a;
}
#define MRR(a,n) map_record_read((a),(n))
#define MRW16(a,v) (*(volatile uint16_t *)(uintptr_t)(a)=(uint16_t)(v))
#define MRE(i) ((void)0)
#endif
#endif
