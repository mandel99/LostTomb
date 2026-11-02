#ifndef RESTORE_MAP_REFRESH_POSTPASS1_H
#define RESTORE_MAP_REFRESH_POSTPASS1_H
#include <stdint.h>
#include <stddef.h>
#include "map_refresh_arithmetic.h"
#ifdef MAP_REFRESH_POSTPASS1_TRACE
uint64_t map_post1_read(uint32_t,uint32_t);
void map_post1_write(uint32_t,uint32_t,uint32_t);
void map_post1_fp(uint32_t,MRFloat);
void map_post1_bounds(int32_t,int32_t,uint32_t);
#define P1_R(a,n) map_post1_read((uint32_t)(a),(n))
#define P1_W(a,n,v) map_post1_write((a),(n),(v))
#define P1_FP(a,v) map_post1_fp((a),(v))
#define P1_BOUNDS(x,y,v) map_post1_bounds((x),(y),(v))
#else
static __inline uint64_t p1_read(uint32_t a,uint32_t n){if(n==1)return *(volatile uint8_t *)(uintptr_t)a;if(n==4)return *(volatile uint32_t *)(uintptr_t)a;return *(volatile uint64_t *)(uintptr_t)a;}
static __inline void p1_write(uint32_t a,uint32_t n,uint32_t v){if(n==1)*(volatile uint8_t *)(uintptr_t)a=(uint8_t)v;else *(volatile uint32_t *)(uintptr_t)a=v;}
#define P1_R(a,n) p1_read((uint32_t)(a),(n))
#define P1_W(a,n,v) p1_write((a),(n),(v))
#define P1_FP(a,v) ((void)0)
#define P1_BOUNDS(x,y,v) ((void)0)
#endif
void FUN_00477500(int32_t,int32_t,int32_t,int32_t);
void FUN_00479b70(int32_t,int32_t,int32_t);
#endif
