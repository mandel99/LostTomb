#ifndef RESTORE_MAP_REFRESH_POSTPASS6_H
#define RESTORE_MAP_REFRESH_POSTPASS6_H
#include <stdint.h>
#include <stddef.h>
uint32_t FUN_004816b0(int32_t,int32_t,int32_t,uint32_t);
uint32_t FUN_00481770(int32_t,int32_t,int32_t,uint32_t);
uint32_t FUN_00487730(int32_t,int32_t,uint32_t);
uint32_t FUN_00487790(int32_t,int32_t,uint32_t);
void FUN_00487b70(int32_t,int32_t);
void FUN_0047db00(uint32_t);
#ifdef MAP_REFRESH_POSTPASS6_TRACE
uint32_t map_post6_read(uint32_t,uint32_t);
void map_post6_write(uint32_t,uint32_t,uint32_t);
void map_post6_helper(uint32_t,uint32_t,uint32_t);
void map_post6_recursion(uint32_t,int32_t,int32_t);
#define P6_R(a,n) map_post6_read((uint32_t)(a),(n))
#define P6_W(a,n,v) map_post6_write((uint32_t)(a),(n),(uint32_t)(v))
#define P6_HELPER(a,p,v) map_post6_helper((a),(p),(v))
#define P6_RECURSE(p,x,y) map_post6_recursion((p),(x),(y))
#else
#define P6_R(a,n) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define P6_W(a,n,v) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a)=(uint32_t)(v))
#define P6_HELPER(a,p,v) ((void)0)
#define P6_RECURSE(p,x,y) ((void)0)
#endif
static __inline uint32_t p6_address(uint32_t tile){return 0xaf598cu+tile*4u;}
static __inline uint32_t p6_radius(int32_t x,int32_t y,int32_t radius,uint32_t mask,uint32_t missing)
{
    int32_t xmin=(int32_t)((uint32_t)x+1u-(uint32_t)radius),xmax=(int32_t)((uint32_t)x+(uint32_t)radius-1u);
    int32_t ymin=(int32_t)((uint32_t)y+1u-(uint32_t)radius),ymax=(int32_t)((uint32_t)y+(uint32_t)radius-1u);
    uint32_t tile=P6_R(0xbca2d8u,4)+(uint32_t)ymin*228u+(uint32_t)xmin,step=(uint32_t)xmin-(uint32_t)xmax+227u,column,row=(uint32_t)ymin,flags;
    if(ymin>ymax)return 0;
    do{
        column=(uint32_t)xmin;
        if(xmin<=xmax){do{
            if((int32_t)column>=0&&(int32_t)row>=0&&(int32_t)column<(int32_t)P6_R(0xbca2d0u,4)&&(int32_t)row<(int32_t)P6_R(0xbca2d4u,4)){
                flags=P6_R(p6_address(tile),4);if((uint32_t)((flags&mask)==0)==missing)return 1;
            }
            ++column;++tile;
        }while((int32_t)column<=xmax);}
        tile+=step;++row;
    }while((int32_t)row<=ymax);
    return 0;
}
static __inline void p6_gather(uint32_t mask){P6_HELPER(0x47db00u,0,mask);FUN_0047db00(mask);P6_HELPER(0x47db00u,1,mask);}
static __inline uint32_t p6_has_two_run(void)
{
    uint32_t i=0,limit=16,slot;int32_t run=-1;
    do{
        if(run>=8)break;
        slot=P6_R(0xbc97c0u+(i&7u)*4u,4);
        if(slot){if(run>=0)++run;}
        else{if(run<0)limit=i+10u;else if(run==2)return 1;run=0;}
        ++i;
    }while(i<limit);
    return 0;
}
#endif
