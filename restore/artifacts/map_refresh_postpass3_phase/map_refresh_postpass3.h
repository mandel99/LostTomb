#ifndef RESTORE_MAP_REFRESH_POSTPASS3_H
#define RESTORE_MAP_REFRESH_POSTPASS3_H
#include <stdint.h>
#include <stddef.h>
#include "city_prepare.h"
#include "terrain_redraw.h"
#include "map_refresh_postpass2.h"
#include "map_neighbors_phase.h"
#include "map_refresh_arithmetic.h"
#include "map_x87_conversion.h"
uint32_t FUN_0047a1c0(int32_t);
#ifdef MAP_REFRESH_POSTPASS3_TRACE
uint64_t map_post3_read(uint32_t,uint32_t);
void map_post3_write(uint32_t,uint32_t,uint32_t);
void map_post3_fp(uint32_t,MRFloat);
void map_post3_bounds(int32_t,int32_t,uint32_t);
void map_post3_water(int32_t,int32_t,uint32_t);
void map_post3_helper(uint32_t,uint32_t,uint32_t);
#define P3_R(a,n) map_post3_read((uint32_t)(a),(n))
#define P3_W(a,n,v) map_post3_write((uint32_t)(a),(n),(uint32_t)(v))
#define P3_FP(a,v) map_post3_fp((a),(v))
#define P3_BOUNDS(x,y,v) map_post3_bounds((x),(y),(v))
#define P3_WATER(x,y,v) map_post3_water((x),(y),(v))
#define P3_HELPER(a,phase,arg) map_post3_helper((a),(phase),(arg))
#else
static __inline uint64_t p3_read(uint32_t a,uint32_t n){if(n==1)return *(volatile uint8_t *)(uintptr_t)a;if(n==4)return *(volatile uint32_t *)(uintptr_t)a;return *(volatile uint64_t *)(uintptr_t)a;}
static __inline void p3_write(uint32_t a,uint32_t n,uint32_t v){if(n==1)*(volatile uint8_t *)(uintptr_t)a=(uint8_t)v;else *(volatile uint32_t *)(uintptr_t)a=v;}
#define P3_R(a,n) p3_read((uint32_t)(a),(n))
#define P3_W(a,n,v) p3_write((uint32_t)(a),(n),(uint32_t)(v))
#define P3_FP(a,v) ((void)0)
#define P3_BOUNDS(x,y,v) ((void)0)
#define P3_WATER(x,y,v) ((void)0)
#define P3_HELPER(a,phase,arg) ((void)0)
#endif
void FUN_004777c0(int32_t,int32_t,int32_t,int32_t);
void FUN_00479e70(int32_t,int32_t,int32_t);
/* Native caller pushes x,y; complete17-instruction callee consumes x only. */
static __inline uint8_t p3_water(int32_t x,int32_t ignored_y)
{
    uint32_t value=FUN_0047a1c0(x);P3_WATER(x,ignored_y,value);return (uint8_t)value;
}
static __inline void p3_gather(uint32_t mask)
{
    P3_HELPER(0x47db00u,0,mask);FUN_0047db00(mask);P3_HELPER(0x47db00u,1,mask);
}
static __inline void p3_invert(void)
{
    P3_HELPER(0x484f00u,0,0);FUN_00484f00();P3_HELPER(0x484f00u,1,0);
}
#endif
