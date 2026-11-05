#include "city_capacity.h"
#include "city_updates.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define I(v) ((int32_t)(uint32_t)(v))

void restore_city_rebuild_building_requirements(void)
{
    uint32_t id=1,p=0x799b98u,limit=U(0x78d790u),value;
    if(I(limit)<1)return;
    do{
        if(B(p-184u)==1u){
            U(p)=0;
            if(!B(p-179u)){value=FUN_0045a140(I(id));U(p)=value;}
        }
        limit=U(0x78d790u);++id;p+=264u;
    }while(I(id)<=I(limit));
}
