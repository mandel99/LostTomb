#include "city_worker_supply.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define H(a) ((int32_t)(int16_t)W(a))

/* Whole455CC0. Cached high-water/countdown, signed WORD coordinates/model,
   DWORD scratch distances, and the native table/store sequence are retained. */
void restore_city_refresh_nearby_house_needs(int32_t building,int32_t radius)
{
    uint32_t count=U(0x78d790u),cursor=0x799ae5u,off,model,value,dx,dy,size;
    int32_t x,y,house_x,house_y,level;
    if((int32_t)count<1)return;
    do{
        if(B(cursor-5u)==1u && B(cursor)!=0u && W(cursor+0x17u)!=0u){
            off=(uint32_t)building*264u;
            house_x=H(cursor+1u);house_y=H(cursor+3u);
            x=H(0x7999deu+off);y=H(0x7999e0u+off);
            dx=(uint32_t)(house_x>x?house_x-x:x-house_x);
            U(0xd3b4fcu)=dx;
            dy=(uint32_t)(house_y>y?house_y-y:y-house_y);
            U(0xd3b4f8u)=dy;
            if((int32_t)(dx>=dy?dx:dy)<=radius){
                level=H(cursor+0xdu);
                if(level<19)++level;
                model=(uint32_t)level*88u;
                size=B(cursor-2u);value=U(0x70ce04u+model);
                if(size>1u)value<<=2;
                if(value!=0u)W(0x799a7cu+off)=10u;
                size=B(cursor-2u);value=U(0x70cdf8u+model);
                if(size>1u)value<<=2;
                if(value!=0u)W(0x799a76u+off)=10u;
                size=B(cursor-2u);value=U(0x70cdfcu+model);
                if(size>1u)value<<=2;
                if(value!=0u)W(0x799a7au+off)=10u;
                value=B(cursor-2u);
                if(value>1u)value=U(0x70ce00u+model)<<2;
                else value=U(0x70ce00u+model);
                if(value!=0u)W(0x799a78u+off)=10u;
            }
        }
        cursor+=264u;
        --count;
    }while(count!=0u);
}
