#ifndef RESTORE_REGION_VALIDATION_H
#define RESTORE_REGION_VALIDATION_H
#include "restore_types.h"
uint32_t FUN_00482650(int32_t x,int32_t y,int32_t size,int32_t radius,uint32_t bits);
uint32_t FUN_00482720(int32_t x,int32_t y,int32_t size,int32_t radius,code *predicate);
uint32_t FUN_004828e0(int32_t x,int32_t y,int32_t size,int32_t radius,uint32_t bits);
uint32_t FUN_004829b0(int32_t x,int32_t y,int32_t size,int32_t radius,uint32_t bits,int32_t required);
#define RV_U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define RV_I(a) (*(volatile int32_t *)(uintptr_t)(uint32_t)(a))
#define RV_ROW 0x00d3b4e0u
#define RV_COL 0x00d3b4e4u
static __inline int32_t rv_add(int32_t a,int32_t b){return (int32_t)((uint32_t)a+(uint32_t)b);}
static __inline int32_t rv_sub(int32_t a,int32_t b){return (int32_t)((uint32_t)a-(uint32_t)b);}
typedef struct {int32_t left,right,top,bottom;uint32_t index,stride;} RestoreRegionBounds;
static __inline RestoreRegionBounds rv_bounds(int32_t x,int32_t y,int32_t size,int32_t radius)
{
    RestoreRegionBounds b;int32_t width,height;uint32_t origin;
    b.left=rv_sub(x,radius);b.right=rv_add(rv_add(rv_add(x,size),radius),-1);
    b.top=rv_sub(y,radius);b.bottom=rv_add(rv_add(rv_add(y,size),radius),-1);
    if(b.left<0)b.left=0;if(b.top<0)b.top=0;
    width=RV_I(0x00bca2d0u);if(b.right>=width)b.right=rv_add(width,-1);
    height=RV_I(0x00bca2d4u);if(b.bottom>=height)b.bottom=rv_add(height,-1);
    origin=RV_U(0x00bca2d8u);RV_U(RV_ROW)=(uint32_t)b.top;
    b.index=origin+(uint32_t)b.top*228u+(uint32_t)b.left;
    b.stride=(uint32_t)b.left-(uint32_t)b.right+227u;return b;
}
#endif
