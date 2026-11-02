#ifndef RESTORE_MAP_REFRESH_POSTPASS2_H
#define RESTORE_MAP_REFRESH_POSTPASS2_H
#include <stdint.h>
#include <stddef.h>
#ifdef MAP_REFRESH_POSTPASS2_TRACE
uint32_t map_post2_read(uint32_t,uint32_t);
void map_post2_write(uint32_t,uint32_t,uint32_t);
#define P2_R(a,n) map_post2_read((uint32_t)(a),(n))
#define P2_W(a,n,v) map_post2_write((uint32_t)(a),(n),(uint32_t)(v))
#else
static __inline uint32_t p2_read(uint32_t a,uint32_t n){return n==1?*(volatile uint8_t *)(uintptr_t)a:*(volatile uint32_t *)(uintptr_t)a;}
static __inline void p2_write(uint32_t a,uint32_t n,uint32_t value){if(n==1)*(volatile uint8_t *)(uintptr_t)a=(uint8_t)value;else *(volatile uint32_t *)(uintptr_t)a=value;}
#define P2_R(a,n) p2_read((uint32_t)(a),(n))
#define P2_W(a,n,v) p2_write((uint32_t)(a),(n),(uint32_t)(v))
#endif
int32_t FUN_00484be0(void *records,int32_t count,int32_t animation,uint8_t reverse);
int32_t FUN_00484d70(void *records,int32_t count);
void FUN_00484f00(void);
static __inline int32_t p2_direction(void){return (int32_t)P2_R(0xbca2e0u,4)/2;}
static __inline void p2_publish(uint32_t row,int32_t direction)
{
    uint32_t value=P2_R(row+8u+(uint32_t)direction,1);P2_W(0xbc9748u,4,value);
    value=P2_R(row+12u,1);P2_W(0xbc9744u,4,value);
}
static __inline void p2_cycle(uint32_t row)
{
    uint32_t old=P2_R(row+14u,1),frames=P2_R(row+13u,1);uint8_t next=(uint8_t)(old+1u);
    P2_W(row+14u,1,next);if(next>=frames)P2_W(row+14u,1,0);
    P2_W(0xbc9740u,4,P2_R(row+14u,1));
}
#endif
