#include "city_region_children.h"
#include "byte_fill.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))

/* Whole004689E0. The original 1000-DWORD ring is scratch memory, not a
   recursive call stack. Shade is cached until the final publication. */
void restore_city_color_road_region(uint32_t tile,uint32_t region)
{
    uint32_t tail=0u,shade,steps=1u,first,neighbor,direction,head,old_steps;
    int32_t classification;
    FUN_0040f8f0((uint32_t *)(uintptr_t)0x78c414u,4000u,0);
    shade=U(0x78c408u);
    U(0x78c410u)=0u;
    U(0x78c40cu)=0u;
    for(;;){
        B(0xa69fdcu+tile)=(uint8_t)shade;
        B(0xa76aecu+tile)=(uint8_t)region;
        first=UINT32_MAX;
        for(direction=0u;direction<4u;++direction){
            neighbor=tile+U(0x5cd678u+direction*4u);
            classification=(int32_t)(int8_t)B(0xee0c50u+neighbor);
            if(classification>=0 && classification<=2 &&
               (classification!=2 || (U(0xaf598cu+neighbor*4u)&0x400u)!=0u) &&
               B(0xa76aecu+neighbor)==0u){
                B(0xa69fdcu+neighbor)=(uint8_t)shade;
                B(0xa76aecu+neighbor)=(uint8_t)region;
                U(0x78c400u)=U(0x78c400u)+1u;
                if(first==UINT32_MAX){
                    U(0x78c404u)=U(0x78c404u)+1u;
                    first=neighbor;
                }else{
                    U(0x78c414u+tail*4u)=neighbor;
                    ++tail;
                    if((int32_t)tail>=1000)tail=0u;
                }
            }
        }
        if(first==UINT32_MAX){
            head=U(0x78c410u);
            if(head==tail)break;
            first=U(0x78c414u+head*4u);
            ++head;
            U(0x78c410u)=head;
            if((int32_t)head>=1000)U(0x78c410u)=0u;
        }
        if((int32_t)U(0x78c404u)>30){
            ++shade;
            U(0x78c404u)=0u;
        }
        tile=first;
        if((int32_t)first<0)break;
        old_steps=steps++;
        if((int32_t)old_steps>=51984)break;
    }
    U(0x78c40cu)=tail;
    U(0x78c408u)=shade;
}

void FUN_004689e0(int32_t tile,uint8_t region)
{
    restore_city_color_road_region((uint32_t)tile,(uint32_t)region);
}
