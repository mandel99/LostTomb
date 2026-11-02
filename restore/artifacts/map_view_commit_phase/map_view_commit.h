#ifndef RESTORE_MAP_VIEW_COMMIT_H
#define RESTORE_MAP_VIEW_COMMIT_H
#include <stdint.h>
void FUN_00409520(void);
void FUN_0040a090(void);
void FUN_00442800(void);
#ifdef MAP_VIEW_COMMIT_TRACE
uint32_t map_commit_read(uint32_t address,uint32_t width);
void map_commit_write(uint32_t address,uint32_t width,uint32_t value);
void map_commit_enter(uint32_t address);
#define MCR(a,n) map_commit_read((a),(n))
#define MCW(a,n,v) map_commit_write((a),(n),(v))
#define MCE(a) map_commit_enter(a)
#else
static __inline uint32_t map_commit_read(uint32_t a,uint32_t n)
{if(n==2)return *(volatile uint16_t *)(uintptr_t)a;return *(volatile uint32_t *)(uintptr_t)a;}
#define MCR(a,n) map_commit_read((a),(n))
#define MCW(a,n,v) do{if((n)==1)*(volatile uint8_t *)(uintptr_t)(a)=(uint8_t)(v);else *(volatile uint16_t *)(uintptr_t)(a)=(uint16_t)(v);}while(0)
#define MCE(a) ((void)0)
#endif
#endif
