#include "city_worker_supply.h"
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))

/* Whole455C30; reuse the existing algorithm with WORD fields/264-byte stride. */
void restore_city_reset_dispatch_reservations(int32_t building)
{
    uint32_t off=(uint32_t)building*264u;
    uint16_t value;
    W(0x799a2cu+off)=0u;
    if(W(0x7999eau+off)!=0u){
        value=W(0x799a14u+off);
        if(value!=0u)W(0x799a14u+off)=(uint16_t)(value-100u);
    }
    if(W(0x799aa6u+off)!=0u){
        value=W(0x799aa4u+off);
        if(value!=0u){
            if(W(0x7999e8u+off)==204u)W(0x799aa4u+off)=(uint16_t)(value-25u);
            else W(0x799aa4u+off)=(uint16_t)(value-100u);
        }
    }
}
