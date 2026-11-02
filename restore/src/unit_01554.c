#include "flood_simulation.h"
#define U(a) (*(uint32_t *)(uintptr_t)(a))
static uint32_t next_payload(uint32_t payload)
{
    uint32_t node=U(U(payload+4));return node?U(node+12):0;
}
void FUN_004bd850(RestoreWorldRegion *self,uint32_t flooding,int32_t percent,uint8_t complete)
{
    int32_t count=(int32_t)self->words[0],budget;
    uint32_t payload;
    if (!count) return;
    if (!self->words[6]) {
        self->words[6]=self->words[4]?U(self->words[4]+12):0;
        if (self->words[6] && percent) {
            int32_t skip=(int32_t)((uint32_t)(percent<0?0:percent)*(uint32_t)count)/100;
            while (skip && self->words[6]) {
                self->words[6]=next_payload(self->words[6]);
                skip=(int32_t)((uint32_t)skip-1u);
            }
        }
    }
    payload=self->words[6];if (!payload) return;
    budget=count;
    if (!complete) {budget=(int32_t)((uint32_t)(count/25)+1u);if(budget<1)budget=1;}
    while (budget && payload) {
        if (*(uint8_t *)(uintptr_t)(payload+24)) FUN_004bd950(payload,(uint8_t)flooding);
        self->words[6]=next_payload(self->words[6]);payload=self->words[6];
        budget=(int32_t)((uint32_t)budget-1u);
    }
}
