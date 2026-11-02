#include "city_top_chrome.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(a))
#define S(a) (*(volatile int16_t *)(uintptr_t)(a))
extern void FUN_004cc110(int group,int entry);
extern int FUN_004cbf30(const uint8_t *text,int32_t font);
extern void FUN_004ccbb0(int group,int entry,int32_t x,int32_t y,uint32_t font,uint32_t color);
void FUN_00425330(int16_t *records,int32_t count)
{
    uintptr_t record=(uintptr_t)records;
    uint32_t index=1,cursor;
    int32_t group,x,y;
    B(0x00e38de1u)=1;B(0x00e38de0u)=1;
    U(0x00e20778u)=0;
    U(0x00e20778u)=(uint32_t)(int32_t)S(record);
    if(count<1)return;
    do {
        group=S(record+6u);y=S(record+4u);cursor=U(0x00e20778u);
        W(record)=(uint16_t)cursor;x=(int16_t)cursor;
        FUN_004cc110(group,0);
        if(index==U(0x00705730u)){
            uint32_t font=U(0x00f56f84u),text=U(0x00e20768u);
            U(0x00d3b510u)=1;
            FUN_004cbf30((const uint8_t *)(uintptr_t)text,(int32_t)font);
            FUN_004ccbb0(group,0,x,y,U(0x00f56f8cu),0xfac1u);
            U(0x00d3b510u)=0;
        }else FUN_004ccbb0(group,0,x,y,U(0x00f56f8cu),0);
        W(record+2u)=W(0x00e20778u);
        U(0x00e20778u)=U(0x00e20778u)+10u;
        record+=16u;++index;
    }while((int32_t)index<=count);
}
