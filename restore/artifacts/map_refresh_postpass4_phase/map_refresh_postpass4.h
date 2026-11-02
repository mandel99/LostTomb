#ifndef RESTORE_MAP_REFRESH_POSTPASS4_H
#define RESTORE_MAP_REFRESH_POSTPASS4_H
#include <stdint.h>
#include <stddef.h>
#ifdef MAP_REFRESH_POSTPASS4_TRACE
uint32_t map_post4_read(uint32_t,uint32_t);
void map_post4_write(uint32_t,uint32_t,uint32_t);
#define P4_R(a,n) map_post4_read((uint32_t)(a),(n))
#define P4_W(a,n,v) map_post4_write((uint32_t)(a),(n),(uint32_t)(v))
#else
static __inline uint32_t p4_read(uint32_t a,uint32_t n){if(n==1)return *(volatile uint8_t *)(uintptr_t)a;if(n==2)return *(volatile uint16_t *)(uintptr_t)a;return *(volatile uint32_t *)(uintptr_t)a;}
static __inline void p4_write(uint32_t a,uint32_t n,uint32_t v){if(n==1)*(volatile uint8_t *)(uintptr_t)a=(uint8_t)v;else if(n==2)*(volatile uint16_t *)(uintptr_t)a=(uint16_t)v;else *(volatile uint32_t *)(uintptr_t)a=v;}
#define P4_R(a,n) p4_read((uint32_t)(a),(n))
#define P4_W(a,n,v) p4_write((uint32_t)(a),(n),(uint32_t)(v))
#endif
void FUN_0047e5c0(void);
void FUN_0047e870(void);
void FUN_0047eb60(int32_t direction,uint32_t *count);
static __inline uint32_t p4_terrain(uint32_t tile,int32_t offset){return 0xaf598cu+(tile+(uint32_t)offset)*4u;}
static __inline uint32_t p4_record(uint32_t tile,int32_t offset){int32_t id=(int16_t)P4_R(0xb4e6fcu+(tile+(uint32_t)offset)*2u,2);return 0x7999e8u+(uint32_t)id*264u;}
#endif
