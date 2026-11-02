#ifndef RESTORE_MAP_REFRESH_POSTPASS8_H
#define RESTORE_MAP_REFRESH_POSTPASS8_H
#include "city_prepare.h"
#include "map_refresh_postpass2.h"
#include "map_neighbors_phase.h"
uint32_t FUN_0047a1c0(int32_t);
uint32_t FUN_0047a1f0(int32_t);
void FUN_0047db00(uint32_t);
void FUN_00484ee0(void *,int32_t);
int32_t FUN_00484f40(void);
#ifdef MAP_REFRESH_POSTPASS8_TRACE
uint32_t map_post8_read(uint32_t,uint32_t);
void map_post8_write(uint32_t,uint32_t,uint32_t);
void map_post8_helper(uint32_t,uint32_t,uint32_t,uint32_t,uint32_t);
#define P8_R(a,n) map_post8_read((uint32_t)(a),(n))
#define P8_W(a,n,v) map_post8_write((uint32_t)(a),(n),(uint32_t)(v))
#define P8_H(a,p,x,y,v) map_post8_helper((a),(p),(uint32_t)(x),(uint32_t)(y),(v))
#else
static __inline uint32_t p8_read(uint32_t a,uint32_t n){return n==1?*(volatile uint8_t *)(uintptr_t)a:*(volatile uint32_t *)(uintptr_t)a;}
static __inline void p8_write(uint32_t a,uint32_t n,uint32_t v){if(n==1)*(volatile uint8_t *)(uintptr_t)a=(uint8_t)v;else *(volatile uint32_t *)(uintptr_t)a=v;}
#define P8_R(a,n) p8_read((uint32_t)(a),(n))
#define P8_W(a,n,v) p8_write((uint32_t)(a),(n),(uint32_t)(v))
#define P8_H(a,p,x,y,v) ((void)0)
#endif
static __inline uint32_t p8_bounds(int32_t x,int32_t y){uint32_t r;P8_H(0x46f930,0,x,y,0);r=FUN_0046f930(x,y);P8_H(0x46f930,1,x,y,r);return r;}
static __inline uint32_t p8_water(int32_t x,int32_t y,uint32_t equal){uint32_t r,a=equal?0x47a1f0u:0x47a1c0u;P8_H(a,0,x,y,0);r=equal?FUN_0047a1f0(x):FUN_0047a1c0(x);P8_H(a,1,x,y,*(volatile int32_t *)(uintptr_t)0x78d7b0u<=0?r&255u:r);return r;}
static __inline void p8_gather(uint32_t mask){P8_H(0x47db00,0,mask,0,0);FUN_0047db00(mask);P8_H(0x47db00,1,mask,0,0);}
static __inline void p8_invert(void){P8_H(0x484f00,0,0,0,0);FUN_00484f00();P8_H(0x484f00,1,0,0,0);}
static __inline int32_t p8_count(void){int32_t r;P8_H(0x484f40,0,0,0,0);r=FUN_00484f40();P8_H(0x484f40,1,0,0,(uint32_t)r);return r;}
static __inline void p8_reset(void){P8_H(0x484ee0,0,0x5cbfb8,26,0);FUN_00484ee0((void *)(uintptr_t)0x5cbfb8u,26);P8_H(0x484ee0,1,0x5cbfb8,26,0);}
static __inline uint32_t p8_context(uint32_t i){return P8_R(0xbc97c0u+(i&7u)*4u,4);}
void FUN_0047a220(int32_t,int32_t,int32_t);
#endif

