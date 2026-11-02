#ifndef RESTORE_MAP_REFRESH_POSTPASS5_H
#define RESTORE_MAP_REFRESH_POSTPASS5_H
#include <stdint.h>
#include <stddef.h>
#include "map_refresh_postpass2.h"
#include "map_refresh_postpass4.h"
#include "terrain_redraw.h"
void FUN_0047b590(int32_t,int32_t,int32_t,int32_t);
void FUN_0047b670(void);
uint32_t FUN_0047b920(void);
int32_t FUN_0051c620(int32_t);
int32_t FUN_0051c5f0(uint32_t,int32_t);
void FUN_00484f20(void);
#ifdef MAP_REFRESH_POSTPASS5_TRACE
uint32_t map_post5_read(uint32_t,uint32_t);
void map_post5_write(uint32_t,uint32_t,uint32_t);
void map_post5_helper(uint32_t,uint32_t,uint32_t);
#define P5_R(a,n) map_post5_read((uint32_t)(a),(n))
#define P5_W(a,n,v) map_post5_write((uint32_t)(a),(n),(uint32_t)(v))
#define P5_HELPER(a,p,v) map_post5_helper((a),(p),(v))
#else
static __inline uint32_t p5_read(uint32_t a,uint32_t n){if(n==1)return *(volatile uint8_t *)(uintptr_t)a;if(n==2)return *(volatile uint16_t *)(uintptr_t)a;return *(volatile uint32_t *)(uintptr_t)a;}
static __inline void p5_write(uint32_t a,uint32_t n,uint32_t v){if(n==1)*(volatile uint8_t *)(uintptr_t)a=(uint8_t)v;else if(n==2)*(volatile uint16_t *)(uintptr_t)a=(uint16_t)v;else *(volatile uint32_t *)(uintptr_t)a=v;}
#define P5_R(a,n) p5_read((uint32_t)(a),(n))
#define P5_W(a,n,v) p5_write((uint32_t)(a),(n),(uint32_t)(v))
#define P5_HELPER(a,p,v) ((void)0)
#endif
static __inline void p5_gather(uint32_t mask){P5_HELPER(0x47db00u,0,mask);FUN_0047db00(mask);P5_HELPER(0x47db00u,1,mask);}
static __inline void p5_normalize(void){P5_HELPER(0x484f20u,0,0);FUN_00484f20();P5_HELPER(0x484f20u,1,0);}
#endif
