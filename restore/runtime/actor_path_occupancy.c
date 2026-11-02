#include "actor_routing.h"
#include "actor_order_path.h"
#define H(a) ((int32_t)*(volatile int16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))

/* Whole51AFA0. The first nonpositive head is empty; subsequent links use
   the native nonzero test. Byte13==0 together with action5 blocks this query. */
uint32_t FUN_0051afa0(int32_t cell)
{
    int32_t id=H(0xb350dcu+(uint32_t)cell*2u);
    if(id<=0)return 1;
    do{
        uint32_t off=(uint32_t)id*388u;
        if(!B(0xbcd605u+off)&&B(0xbcd630u+off)==5u)return 0;
        id=H(0xbcd600u+off);
    }while(id);
    return 1;
}

/* Whole51AFF0. This query instead checks byte13==1 and retains the original
   self-link exit after checking the current actor's blocking state. */
uint32_t FUN_0051aff0(int32_t cell)
{
    int32_t id=H(0xb350dcu+(uint32_t)cell*2u),next;
    if(id<=0)return 1;
    do{
        uint32_t off=(uint32_t)id*388u;
        if(B(0xbcd605u+off)==1u&&B(0xbcd630u+off)==5u)return 0;
        next=H(0xbcd600u+off);if(id==next)return 1;id=next;
    }while(id);
    return 1;
}
